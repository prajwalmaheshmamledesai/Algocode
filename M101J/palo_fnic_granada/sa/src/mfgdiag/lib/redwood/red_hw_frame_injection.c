//////////////////////////////////////////////////////////
// $Author: ravin $
//
// $Revision: 1.13 $
//
//////////////////
//
// packet injection thru PIOs routines
// 
// Note: there are 2 types of endianness we deal with in
// this file, one is the MAC nspi interface endianness and
// the other is the platform we compile for.
//
// MoreThanIp's MAC is big endian in nspi, while the Dover
// MAC is little endian. That is what the is_big_endian
// argument to the functions is used for below
//
// We can run this code in x86 (little endian) or in PPC
// which might be big endian or not.
// Depending on the platform's endianness we swizzle the buffer
// we construct when we do FOUT
// 
//////////////////////////////////////////////////////////
#include "red_hw_frame_injection.h"
#include "red_hw_error.h"
#include "red_red_naxislave_define.h"


////////////////////////////////////////////////////////////////////////
// externally defined symbols
////////////////////////////////////////////////////////////////////////
extern int red_reg_wr(int inst, unsigned int address, unsigned int  data);  // address is byte aligned
extern int red_reg_rd(int inst, unsigned int address, unsigned int* data); // address is byte aligned

/////////////////////////////////////////////////////////////////////////////
// internals
/////////////////////////////////////////////////////////////////////////////
enum    { FIN_SOP_MASK =1
        , FIN_EOP_MASK =2
        , FIN_ERR_MASK =4
        , FIN_SIZE_OFFSET=3 // sop, eop, err
        , FIN_SIZE_MASK=7 // 3 bits wide
        , DEAD_BEEF=0xdeadbeef
        , CTRL_FLIT=2
};

static unsigned int real_swizzle(const unsigned int data) {
        return    (data>>24) 
                | (data<<24)
                | ((data & 0x00ff0000)>>8)
                | ((data & 0x0000ff00)<<8)
                ;
}

static unsigned int nop_swizzle(const unsigned int data) {
        return data;
}
static const unsigned int offset_big_endian[2]    = { 0, 1}; 
static const unsigned int offset_little_endian[2] = { 1, 0};
static const unsigned int offset_4_bytes[2] = { 0, 0};


#define WRITE_FULL_FLIT \
	 { unsigned int word;		\
          for (word=0; word<WORDS_PER_FLIT; ++word) {\
              if ((ret_val=red_reg_wr(inst, (THE_DHS_ADDR+offset[word])<<2, (*swizzle)(packet_data[flit*WORDS_PER_FLIT+word])))!=0) { \
                   return ret_val; \
              } \
          } \
	  }

///////////////////////////////////////////////////////////////////////////////////////////
// frame_input_handler
// \param THE_DHS_ADDR is the base address of the DHS register (4 byte aligned)
// \param PktPtr is a pointer to a buffer that has the packet data in network byte order
// \param PktSize is the number of bytes in the buffer
// \param BPF bytes per flit is either 4 or 8. CI is 4 bytes wide, all other are 8 bytes wide
// \param is_big_endian is true when bypassing the moreThanIP MAC (NI and HI XGBE mode)
// \param inject_error allows the invoker to inject nspi errors with the packet
// \return i2c error code if any
///////////////////////////////////////////////////////////////////////////////////////////
int red_hw_frame_input_dhs_handler    
                                ( int inst,
				  const unsigned int THE_DHS_ADDR
                                , const unsigned char* PktPtr
                                , const unsigned int PktSize
                                , const unsigned int BPF //bytes per flit
                                , const int is_big_endian // true if non-zero
                                , const int inject_error
                                ) {
        const unsigned int* offset= (BPF==4) ? offset_4_bytes :
                           (is_big_endian ? offset_big_endian : offset_little_endian);
        unsigned int (*swizzle)(const unsigned int) = is_big_endian  ? real_swizzle : nop_swizzle;
        unsigned int* packet_data = (unsigned int*)PktPtr;
        unsigned int flit=0;
        int ret_val;

        const unsigned int WORDS_PER_FLIT=BPF/sizeof(unsigned int);

        const unsigned int CTRL_BYTE_ADDR = (THE_DHS_ADDR+CTRL_FLIT)<<2;

        // SOP flit
        WRITE_FULL_FLIT
        if ((ret_val=red_reg_wr(inst, CTRL_BYTE_ADDR, FIN_SOP_MASK))!=0) { // ctrl flit goes last
                return ret_val;
        }

        // everything but EOP flit
        const unsigned int EXTRA_BYTES = PktSize%BPF;
        const unsigned int FULL_FLITS= (PktSize/BPF) - (EXTRA_BYTES?0:1);
        for (flit=1; flit<FULL_FLITS; ++flit) {
              WRITE_FULL_FLIT
              if ((ret_val=red_reg_wr(inst, CTRL_BYTE_ADDR, 0))!=0){ // ctrl flit goes last
                        return ret_val;
              }
        }

        // EOP flit
        WRITE_FULL_FLIT
        ret_val=red_reg_wr(inst, CTRL_BYTE_ADDR, FIN_EOP_MASK | (inject_error?FIN_ERR_MASK:0) | (EXTRA_BYTES<<FIN_SIZE_OFFSET));
        return ret_val;
}

///////////////////////////////////////////////////////////////////////////////////////////
// red_hw_ss_output_packet_available
//
// NOTE: don't call this function from 2 threads with the same which_ss parameter:
//       they can stop on their accesses to the same debug port!
//
// \param DHS_ADDR is the base address of the SS output queue stat DHS (4 byte aligned)
// \param packet_available is 0 if no packet available or a positive number otherwise
// \return an error code in case of error or 0 otherwise
///////////////////////////////////////////////////////////////////////////////////////////
int red_hw_ss_output_packet_available(int inst, const unsigned int which_ss, const unsigned int which_port, int* packet_available) {
        const unsigned int sta_debug_reg[]= 
                                        { RED_SS0_STA_DEBUG
                                        , RED_SS1_STA_DEBUG
                                        , RED_SS2_STA_DEBUG
                                        , RED_SS3_STA_DEBUG
        };
        const unsigned int cfg_debug_reg[]= 
                                        { RED_SS0_CFG_DEBUG
                                        , RED_SS1_CFG_DEBUG
                                        , RED_SS2_CFG_DEBUG
                                        , RED_SS3_CFG_DEBUG
        };

        if (which_ss >= (sizeof(cfg_debug_reg)/sizeof(unsigned int))) {
                return -2; // index out of range
        }

        const unsigned int cfg_write_value = 1 //enable debug
                                             | ((5 + which_port)<<1) // debug port select
                                             ;
        int ret_val;
        // configure the debug port in this SS instance
        if ((ret_val=red_reg_wr(inst, cfg_debug_reg[which_ss]*4, cfg_write_value))!=0) {
                return ret_val;
        }

        // read the debug port to find the status of the rdch state machine
        unsigned int read_data;
        *packet_available=0;
        if ((ret_val=red_reg_rd(inst, sta_debug_reg[which_ss]*4, &read_data))!=0) {
                return ret_val;
        }
        // if bit 1 of read_data is set then the state machine is idle
        *packet_available = ((read_data>>1)&1) ? 0 : 1 ;
        return 0;
}

// determine the compiler's target platform here to figure out how to swizzle the
// output buffer we create with red_hw_frame_output_dhs_handler
#ifndef _BIG_ENDIAN_
#define _BIG_ENDIAN_
#endif

#ifdef _BIG_ENDIAN_
#       define I_AM_BIG_ENDIAN
#else
#       ifdef __BYTE_ORDER
#               if __BYTE_ORDER == __BIG_ENDIAN
#                     define I_AM_BIG_ENDIAN
#               endif
#       endif 
#endif

///////////////////////////////////////////////////////////////////////////////////////////
// red_hw_frame_output_dhs_handler
// \param THE_DHS_ADDR is the base address of the DHS register (4 byte aligned)
// \param buffer will be filled up with network byte ordering, with the first byte to go to the network on offset 0
// \param BPF bytes per flit, 4 for CI, 8 for all others
// \return number of bytes received, REDWOOD_ERROR_DEFAULT in case of error
///////////////////////////////////////////////////////////////////////////////////////////
int red_hw_frame_output_dhs_handler (int inst, const unsigned int THE_DHS_ADDR, unsigned char* buffer, const unsigned int BPF, const unsigned int is_big_endian) {
        unsigned int read_data;
        int ret_val;
        unsigned int* write_ptr=(unsigned int*)buffer;
        int size=0;
        int eop;
        const unsigned int misc_int_err_byte_addr=RED_MISC_INT_ERR<<2;
	unsigned int* swizzle_ptr;
	unsigned long long* swizzle_lptr;

	// Make compiler happy.
	swizzle_lptr = swizzle_lptr;

        // clear naxi error interrupt bit
        if ((ret_val=red_reg_wr(inst, misc_int_err_byte_addr, (1<<RED_MISC_INT_ERR_FLD_I2C_DRV_NAXI_RSP_AERR))!=0)) {
                return ret_val;
        }
        if ((ret_val=red_reg_rd(inst, misc_int_err_byte_addr, &read_data))!=0) { // flush
                return ret_val;
        }

        do {    // read upper 8 bits first
                if ((ret_val=red_reg_rd(inst, (THE_DHS_ADDR+CTRL_FLIT)<<2, &read_data))!=0) {
                        return ret_val;
                }
                if (read_data==DEAD_BEEF) { // this should never be deadbeef
                        return REDWOOD_ERROR_DEFAULT;
                }
                eop = read_data & FIN_EOP_MASK;
		if (eop) {
			const unsigned int got_size = (read_data >> FIN_SIZE_OFFSET) & FIN_SIZE_MASK;
			const unsigned int eff_size = got_size ? got_size : BPF;
			size += eff_size;
	 	} else {
			size += BPF;
		}

		unsigned int word;
	        const unsigned int WORDS_PER_FLIT= 2; // the DHS register is always 64 bits, and it expects 3 reads to move forward
                for (word=0; word<WORDS_PER_FLIT; ++word) {
                    if ((ret_val=red_reg_rd(inst, (THE_DHS_ADDR+word)<<2, write_ptr))!=0) {
                        return ret_val;
                    }
                    if (*write_ptr==DEAD_BEEF) { // this can be deadbeef, but we need to check interrupt in case it is
                            if ((ret_val=red_reg_rd(inst, misc_int_err_byte_addr, &read_data))!=0) {
                                return ret_val;
                            } 
                            if (read_data & (1<<RED_MISC_INT_ERR_FLD_I2C_DRV_NAXI_RSP_AERR)) { // clear the interrupt bit
                                    if ((ret_val=red_reg_wr(inst, misc_int_err_byte_addr, (1<<RED_MISC_INT_ERR_FLD_I2C_DRV_NAXI_RSP_AERR)))!=0) { 
                                        return ret_val;
                                    }
                                    return REDWOOD_ERROR_DEFAULT;
                            }
                    }
                    write_ptr++;
                }
                write_ptr -= (BPF==4) ? 1 : 0; // if 4 bytes per flit, then go back one word
        } while (!eop);

#ifdef I_AM_BIG_ENDIAN
        if (is_big_endian) { // this endianness is the MAC's nspi, not the platform's
		for (swizzle_ptr=(unsigned int*)buffer; swizzle_ptr<write_ptr; ++swizzle_ptr) {
		      *swizzle_ptr = (((*swizzle_ptr)&0xff000000)>>24)
				   | (((*swizzle_ptr)&0x00ff0000)>>8)
				   | (((*swizzle_ptr)&0x0000ff00)<<8)
				   | (((*swizzle_ptr)&0x000000ff)<<24)
				   ;
		}
		return size;
	}
	// nspi is little endian (BI and CI or HI 1G)
	if (BPF==8) { //bi
		// swap every 4 bytes
		for (swizzle_ptr=(unsigned int*)buffer; swizzle_ptr<write_ptr; swizzle_ptr+=2) {
			unsigned int tmp = *swizzle_ptr;
			*swizzle_ptr = *(swizzle_ptr+1);
			*(swizzle_ptr+1) = tmp;
		}
	}
	return size;
#else
        if (is_big_endian) { // this endianness is the MAC's nspi, not the platform's
                return size; // no need for further work
        }
        
        // need to swizzle 
        if (BPF==4) {
            for (swizzle_ptr=(unsigned int*)buffer; swizzle_ptr<write_ptr; ++swizzle_ptr) {
                      *swizzle_ptr = (((*swizzle_ptr)&0xff000000)>>24)
                                   | (((*swizzle_ptr)&0x00ff0000)>>8)
                                   | (((*swizzle_ptr)&0x0000ff00)<<8)
                                   | (((*swizzle_ptr)&0x000000ff)<<24)
                                   ;
            }
            return size;
        }

        for (swizzle_lptr=(unsigned long long*)buffer; swizzle_lptr<(unsigned long long*)write_ptr; ++swizzle_lptr) {
                      *swizzle_lptr = (((*swizzle_lptr)&0xff00000000000000ULL)>>(56-(8*0)))
                                   | (((*swizzle_lptr)&0x00ff000000000000ULL)>>(56-(8*2)))
                                   | (((*swizzle_lptr)&0x0000ff0000000000ULL)>>(56-(8*4)))
                                   | (((*swizzle_lptr)&0x000000ff00000000ULL)>>(56-(8*6)))
                                   | (((*swizzle_lptr)&0x00000000ff000000ULL)<<(56-(8*6)))
                                   | (((*swizzle_lptr)&0x0000000000ff0000ULL)<<(56-(8*4)))
                                   | (((*swizzle_lptr)&0x000000000000ff00ULL)<<(56-(8*2)))
                                   | (((*swizzle_lptr)&0x00000000000000ffULL)<<(56-(8*0)))
                                   ;
        }
        return size;
#endif
}
