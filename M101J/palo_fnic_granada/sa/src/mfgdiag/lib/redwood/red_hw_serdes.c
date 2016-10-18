/////////////////////////////////////////////////////////////////////////////
// $Author: pkaplan $
// $Revision: 1.23 $
//
// serdes configuration library
//
// references:
// [1] WIZ5C2xxN3x2 Macro Family Datasheet 
// [2] AVAGO's serdes specification: /tools/vendors/avago/90nm_092407/documentation/serdes/SerDes_cln90g_flip_chip_33v_May07_External_Reference_Specification.pdf
// [3] AVAGO 90 nm SBus Specification: /tools/vendors/avago/90nm_092407/documentation/serdes/Sbus_cln90g_flip_chip_Sep06_External_Reference_Specification.pdf
//
/////////////////////////////////////////////////////////////////////////////
#include "red_hw_sbus.h"
#include "red_hw_error.h"
#include "red_hw_globals.h"
#include "red_hw_serdes.h"
#include "red_red_naxislave_define.h"
#ifdef RED_VERIFICATION
#include "libutilTB.h"
#include "cmdOpts.h"
#else 
#include "red_hw_lib.h" 
#define RDW_SERDES_INIT_TIME ( 200)

#endif

////////////////////////////////////////////////////////////////////////
// externally defined symbols
////////////////////////////////////////////////////////////////////////
extern int red_reg_wr(int inst, unsigned int address, unsigned int  data);  // address is byte aligned
extern int red_reg_rd(int inst, unsigned int address, unsigned int* data); // address is byte aligned

////////////////////////////////////////////////////////////////////
// pre rx_rdy serdes setup helper
// return 0 if success
///////////////////////////////////////////////////////////////////
static int red_hw_configure_serdes_pre_rx_rdy_helper(int inst, const int sbus_address, const int rate_sel, const int ref_sel) {
        int ret_val=0;
        // serdes register 0
        int reg_addr    = RED_SBUS_ADDR(sbus_address,0);
        int reg_data    = 1 // sbus_sbus_clk_gate
                        | (ref_sel<<1) 
                        ;
        if ((ret_val=red_reg_wr(inst, reg_addr,reg_data))!=0) {
                return ret_val;
        }

        // serdes register 0x22
        reg_addr    = RED_SBUS_ADDR(sbus_address,0x22);
        reg_data    = 2 // tx_en 
                    | 1 // rx_en 
                    | (rate_sel<<3) 
                    ;
        if ((ret_val=red_reg_wr(inst, reg_addr,reg_data))!=0) {
                return ret_val;
        }

        // serdes register 0x36
        reg_addr    = RED_SBUS_ADDR(sbus_address, 0x36);
        reg_data    = rate_sel; 
        if ((ret_val=red_reg_wr(inst, reg_addr,reg_data))!=0) {
                return ret_val;
        }

        return 0;
}

int red_hw_serdes_disable(int inst, uint32_t nimask, uint32_t himask) 
{
        int ret_val=0, port;
        // serdes register 0
        int reg_addr;
        int reg_data    = 0x00;


	for (port = 0; port < 32; port++) {
		const int hi_offset = ((port>>2)*RED_NUM_SERDES_PER_PORT)+(port&3);
		const int hi_sbus_addr = hi_offset + RED_SERDES_START_OFFSET;

		if (!(himask & (0x01 << port))) continue;
	        reg_addr    = RED_SBUS_ADDR(hi_sbus_addr,0x22);
		if ((ret_val=red_reg_wr(inst, reg_addr,reg_data))!=0) {
                	return ret_val;
        	}
	}

	for (port = 0; port < 16; port++) {
		const int ni_offset = ((port>>2)*RED_NUM_SERDES_PER_PORT)+(port&3);
		const int ni_sbus_addr =  RED_SERDES_START_OFFSET
                                  + (RED_NUM_HI*RED_NUM_SERDES_PER_PORT)
 + ni_offset;

		if (!(nimask & (0x01 << port))) continue;
	        reg_addr    = RED_SBUS_ADDR(ni_sbus_addr,0x22);
		if ((ret_val=red_reg_wr(inst, reg_addr,reg_data))!=0) {
                	return ret_val;
        	}
	}

        return 0;
}

////////////////////////////////////////////////////////////////////
// post rx_rdy serdes setup helper
// return 0 if success
///////////////////////////////////////////////////////////////////
static int red_hw_configure_serdes_post_rx_rdy_helper(int inst, const int sbus_address, const int rx_polarity_inv, const int tx_polarity_inv) {
        int ret_val=0;
        int reg_addr;
        int reg_data;


        if (rx_polarity_inv) {
            // rx polarity inversion
            reg_addr    = RED_SBUS_ADDR(sbus_address, 0x03); 
            reg_data    = 1 // gate, override input pins
                        | (1<<3) // align_en
                        ;
            if ((ret_val=red_reg_wr(inst, reg_addr, reg_data))!=0) {
                return ret_val;
            }

            reg_addr    = RED_SBUS_ADDR(sbus_address, 0x07); // rx polarity inversion
            reg_data    = (1<<4);// polarity inversion
            if ((ret_val=red_reg_wr(inst, reg_addr, reg_data))!=0) {
                return ret_val;
            }

            // because we set the rx_data_gate we need to reset all the bits controlled by it
            reg_addr    = RED_SBUS_ADDR(sbus_address, 0x06); // sig strength en
            reg_data    = (1<<3);// sig strength
            if ((ret_val=red_reg_wr(inst, reg_addr, reg_data))!=0) {
                return ret_val;
            }

            reg_addr    = RED_SBUS_ADDR(sbus_address, 0x09); // sig strength en
            reg_data    = (5<<1);// reserved
            if ((ret_val=red_reg_wr(inst, reg_addr, reg_data))!=0) {
                return ret_val;
            }
        }

        if (tx_polarity_inv) {
            // tx polarity inversion
            reg_addr    = RED_SBUS_ADDR(sbus_address, 0x0a); // enable sbus_tx_data gate
            reg_data    = 1 // gate, override input pins
                        | (1<<1) // sel core data
                        ;
            if ((ret_val=red_reg_wr(inst, reg_addr, reg_data))!=0) {
                return ret_val;
            }

            reg_addr    = RED_SBUS_ADDR(sbus_address, 0x0b); 
            reg_data    = (1<<2);// tx polarity inv
            if ((ret_val=red_reg_wr(inst, reg_addr, reg_data))!=0) {
                return ret_val;
            }
        }

        return 0;

}

/**
 Serdes configuration procedure

 \param gbe_mode HI ports are Gigabit Ethernet ports
 \param pll_bypass the clock reference to redwood bypasses the PLL
 \param hi_mask which HI instances to configure, one bit per serdes (there are 32 of them), HI0 serdes 0 is bit 0. To broadcast use -1
 \param ni_mask which NI instances to configure, one bit per serdes (there are 16 of them), NI0 serdes 0 is bit 0. To broadcast use -1
 \param MAX_TRIES number of polling for readiness iterations before giving up
 \param hi_polarity_rx_inv_mask  which HI instances to invert polarity on the rx side, bits mapped like hi_mask
 \param hi_polarity_tx_inv_mask  which HI instances to invert polarity on the tx side, bits mapped like hi_mask
 \param ni_polarity_rx_inv_mask  which NI instances to invert polarity on the rx side, bits mapped like ni_mask
 \param ni_polarity_tx_inv_mask  which NI instances to invert polarity on the tx side, bits mapped like ni_mask
 \param ni_mask which NI instances to configure, one bit per serdes (there are 16 of them), NI0 serdes 0 is bit 0. To broadcast use -1
 \return 0 on success, otherwise a code defined in red_hw_error.h

 Depending on PLL bypass, GBE or XGBE modes, serdes use different parameters
 For reference, here is the table of clock we use
 mode                       clock freq (MHz)
 ---------------            ----------------
 1G PLL bypass              125
 1G PLL                     62.5
 10G PLL bypass             312.5
 10G PLL                    156.25
*/
int red_hw_configure_serdes(int inst, const int gbe_mode
                          , const int pll_bypass
                          , const int   hi_mask
                          , const short ni_mask
                          , const int MAX_TRIES
                          , const int   hi_polarity_rx_inv_mask
                          , const int   hi_polarity_tx_inv_mask
                          , const short ni_polarity_rx_inv_mask
                          , const short ni_polarity_tx_inv_mask
                          ) {
    int tries=MAX_TRIES;
    int sleep_time=0;
    int ret_val;


    const int ref_sel_gbe  = pll_bypass ?  0xA :    8; // see [2, table 51]
    const int rate_sel_gbe = pll_bypass ?    2 :    2; // see [2, table 51]
    const int ref_sel_xgbe = pll_bypass ? 0x15 : 0x13; // see [2, table 51]
    const int rate_sel_xgbe= pll_bypass ?    1 :    5; // see [2, table 51]
    const int ref_sel_hi   = gbe_mode ? ref_sel_gbe : ref_sel_xgbe;
    const int rate_sel_hi  = gbe_mode ? rate_sel_gbe: rate_sel_xgbe;
    const int can_broadcast = (gbe_mode==0) && (hi_mask==-1) && (ni_mask==-1);
    const int ports_per_hi = gbe_mode ? 1 : RED_NUM_SERDES_PER_PORT;

    int num_serdes_accessed=0; // keep count of how many serdes we configure


    // see [1, table 1] and [2, table 51] for the maximum clocks speed 
    unsigned int reg_data=3; //  divide by 8. 156/8=19 MHz
    unsigned int reg_addr=RED_SBUS_ADDR(RED_SBUS_CONTROLLER, 0x0A);
    if ((ret_val=red_reg_wr(inst, reg_addr,reg_data))!=0) {
        return ret_val;
    }
    
#ifdef RED_VERIFICATION
    // testbench specific code
    if (gbe_mode) { // setup the testbench TI serdes
        tbvSignalHdlT CFG_RATE_E("top.CFG_RATE_E");
        tbvSignalHdlT CFGPLL_MPY_E("top.CFGPLL_MPY_E");
        CFG_RATE_E=1; // half rate. See [1] 
        CFGPLL_MPY_E=4; // multiplier for 1.25Gb/s see [1]
    }
    if (CmdOpts::access()->findPlusArg("I2C")) {
        sleep_time=50000; // nanosecs
    }
   
#else
    sleep_time=RDW_SERDES_INIT_TIME;
#endif
    
    if (sleep_time) {
        // this is the one that takes 2560 cycles to update the 128 divide in the sbus.
        // wait for that before issuing a new i2c request
        // otherwise the 2 i2c requests stop on each other.
        // the i2c slave has a limitation: it assumes rnaxi is much faster than i2c and it breaks
        // when there is a pending rnaxi request 
        RED_SLEEP(sleep_time); 
    }


    if (!can_broadcast) {
        // either we are programming GBE mode or not all HIs or NIs need to be programmed
        int hi,ni,serdes;
        // only program serdes 0 of each HI
        for (hi=0;  hi<RED_NUM_HI; ++hi) {
                for (serdes=0; serdes<ports_per_hi; ++serdes) {
                    const int hi_offset = (hi*RED_NUM_SERDES_PER_PORT)+serdes;
                    const int sbus_addr = hi_offset + RED_SERDES_START_OFFSET;
                    if (hi_mask & (1<<hi_offset)) {
                        reg_addr =  RED_SBUS_ADDR(sbus_addr, 0xf); // remember this address for later polling for rx_rdy
                        if ((ret_val=red_hw_configure_serdes_pre_rx_rdy_helper(inst, sbus_addr, rate_sel_hi, ref_sel_hi))!=0) {
                            return ret_val;
                        }
                        num_serdes_accessed++; // keep count of how many serdes we configure
                    }
                }
        }
        const int NI_START_OFFSET = RED_SERDES_START_OFFSET
                                  + (RED_NUM_HI*RED_NUM_SERDES_PER_PORT)
                                  ;
        // program the 16 NI serdes
        for (ni=0;  ni<RED_NUM_NI; ++ni) {
                for (serdes=0; serdes<RED_NUM_SERDES_PER_PORT; ++serdes) {
                    const int ni_offset = (ni*RED_NUM_SERDES_PER_PORT)+serdes;
                    const int sbus_addr = NI_START_OFFSET + ni_offset;
                    if (ni_mask & (1<<ni_offset)) {
                        reg_addr =  RED_SBUS_ADDR(sbus_addr, 0xf); // remember this address for later polling for rx_rdy
                        if ((ret_val=red_hw_configure_serdes_pre_rx_rdy_helper(inst, sbus_addr, rate_sel_xgbe, ref_sel_xgbe))!=0) {
                            return ret_val;
                        }
                        num_serdes_accessed++; // keep count of how many serdes we configure
                    }
                }
        }
    } else { // broadcast XGBE to all serdes
        if ((ret_val=red_hw_configure_serdes_pre_rx_rdy_helper(inst, RED_SBUS_BROADCAST, rate_sel_xgbe, ref_sel_xgbe))!=0) {
                return ret_val;
        }
        num_serdes_accessed     = RED_NUM_HI*RED_NUM_SERDES_PER_PORT
                                + RED_NUM_NI*RED_NUM_SERDES_PER_PORT
                                ;
        // poll last serdes programmed for rx_rdy until it sets
        reg_addr    =  RED_SBUS_ADDR( num_serdes_accessed
                                    , 0xf // register to read
                       );
    }

    if (num_serdes_accessed==0) { // did we configure any serdes?
          return 0; // no, nothing else to do
    }

    // poll the last serdes configured until it's ready or until an error occurs
    do { ret_val=red_reg_rd(inst, reg_addr, &reg_data);
    } while (  ((reg_data&1)==0)  // rx_rdy bit is not set yet
            && tries-- // we haven't timed out yet
            && ((reg_data>>8)==RED_SBUS_READ_SUCCESS) // read succeded, it just returned "not done yet"
            && !ret_val // no i2c error
    );
    if (tries<=0) {
                return REDWOOD_ERROR_SERDES_RDY_TIMEOUT;
    }
    if (ret_val) {
                return ret_val;
    }
    if ((reg_data>>8)!=RED_SBUS_READ_SUCCESS) {
                return REDWOOD_ERROR_SBUS_READ;
    }

    // enable tx output based on the mask
    if (!can_broadcast) {
        // either we are programming GBE mode or not all HIs or NIs need to be programmed
        int hi,ni,serdes;
        // only program serdes 0 of each HI
        for (hi=0;  hi<RED_NUM_HI; ++hi) {
                for (serdes=0; serdes<ports_per_hi; ++serdes) {
                    const int hi_offset = (hi*RED_NUM_SERDES_PER_PORT)+serdes;
                    const int sbus_addr = hi_offset + RED_SERDES_START_OFFSET;
                    if (hi_mask & (1<<hi_offset)) {
                        // serdes register 0xd
                        int reg_addr    = RED_SBUS_ADDR(sbus_addr,0xd);
                        int reg_data    = 1 // gate
                                    | (1<<4) // tx_output_en
                                    ;
                        if ((ret_val=red_reg_wr(inst, reg_addr, reg_data))!=0) {
                            return ret_val;
                        }
                    }
                }
        }
        const int NI_START_OFFSET = RED_SERDES_START_OFFSET
                                  + (RED_NUM_HI*RED_NUM_SERDES_PER_PORT)
                                  ;
        // program the 16 NI serdes
        for (ni=0;  ni<RED_NUM_NI; ++ni) {
                for (serdes=0; serdes<RED_NUM_SERDES_PER_PORT; ++serdes) {
                    const int ni_offset = (ni*RED_NUM_SERDES_PER_PORT)+serdes;
                    const int sbus_addr = NI_START_OFFSET + ni_offset;
                    if (ni_mask & (1<<ni_offset)) {
                        // serdes register 0xd
                        int reg_addr    = RED_SBUS_ADDR(sbus_addr,0xd);
                        int reg_data    = 1 // gate
                                    | (1<<4) // tx_output_en
                                    ;
                        if ((ret_val=red_reg_wr(inst, reg_addr, reg_data))!=0) {
                            return ret_val;
                        }
                    }
                }
        }
    } else { // broadcast tx_output_en to all serdes
        // serdes register 0xd
        int reg_addr    = RED_SBUS_ADDR(RED_SBUS_BROADCAST,0xd);
        int reg_data    = 1 // gate
                    | (1<<4) // tx_output_en
                    ;
        if ((ret_val=red_reg_wr(inst, reg_addr, reg_data))!=0) {
            return ret_val;
        }
    }

    // polarity inversion. 
    // NOTE: assume we are in the no polarity inversion state to begin with on all serdes
    if (hi_polarity_rx_inv_mask || hi_polarity_tx_inv_mask || ni_polarity_rx_inv_mask || ni_polarity_tx_inv_mask) {
        if (can_broadcast && (hi_polarity_rx_inv_mask==-1) && (hi_polarity_tx_inv_mask==-1) && (ni_polarity_rx_inv_mask==-1) && (ni_polarity_tx_inv_mask==-1)) {
            if ((ret_val=red_hw_configure_serdes_post_rx_rdy_helper(inst, RED_SBUS_BROADCAST, 1/*rx polarity inv*/, 1 /*tx polarity inv*/))!=0){
                    return ret_val;
            }
        } else { // case by case
            // either we are programming GBE mode or not all HIs or NIs need to be programmed
            int hi,ni,serdes;
            // only program serdes 0 of each HI
            for (hi=0;  hi<RED_NUM_HI; ++hi) {
                    for (serdes=0; serdes<ports_per_hi; ++serdes) {
                        const int hi_offset = (hi*RED_NUM_SERDES_PER_PORT)+serdes;
                        const int sbus_addr = hi_offset + RED_SERDES_START_OFFSET;
                        const int filter = 1<<hi_offset;
                        if (hi_mask & filter) {
                            if ((ret_val= red_hw_configure_serdes_post_rx_rdy_helper(inst, sbus_addr, hi_polarity_rx_inv_mask & filter, hi_polarity_tx_inv_mask & filter))!=0){
                                return ret_val;
                            }
                            num_serdes_accessed++; // keep count of how many serdes we configure
                        }
                    }
            }
            const int NI_START_OFFSET = RED_SERDES_START_OFFSET
                                      + (RED_NUM_HI*RED_NUM_SERDES_PER_PORT)
                                      ;
            // program the 16 NI serdes
            for (ni=0;  ni<RED_NUM_NI; ++ni) {
                    for (serdes=0; serdes<RED_NUM_SERDES_PER_PORT; ++serdes) {
                        const int ni_offset = (ni*RED_NUM_SERDES_PER_PORT)+serdes;
                        const int sbus_addr = NI_START_OFFSET + ni_offset;
                        const int filter = 1<<ni_offset;
                        if (ni_mask & filter) {
                            if ((ret_val= red_hw_configure_serdes_post_rx_rdy_helper(inst, sbus_addr, ni_polarity_rx_inv_mask & filter, ni_polarity_tx_inv_mask & filter))!=0){
                                return ret_val;
                            }
                            num_serdes_accessed++; // keep count of how many serdes we configure
                        }
                    }
            }
            
        }
    }
    
    return 0;
}

/**
 Serdes rx_rdy check

 check that each serdes's rx_rdy is asserted

 \param hi_mask which HI instances to configure, one bit per serdes (there are 32 of them), HI0 serdes 0 is bit 0. To broadcast use -1
 \param ni_mask which NI instances to configure, one bit per serdes (there are 16 of them), NI0 serdes 0 is bit 0. To broadcast use -1
 \return 0 on success, otherwise a code defined in red_hw_error.h
*/
int red_hw_serdes_rx_rdy_test(int inst, const int hi_mask, const short ni_mask, unsigned int* addr_error, unsigned int* exp_data, unsigned int* got_data) {
    int ret_val;
    int hi,ni,serdes;

    for (hi=0;  hi<RED_NUM_HI; ++hi) {
            for (serdes=0; serdes<RED_NUM_SERDES_PER_PORT; ++serdes) {
                const int hi_offset = (hi*RED_NUM_SERDES_PER_PORT)+serdes;
                const int sbus_addr = hi_offset + RED_SERDES_START_OFFSET;
                if (hi_mask & (1<<hi_offset)) {
                    *addr_error =  RED_SBUS_ADDR(sbus_addr, 0xf); 
                    *exp_data = 1;

                    if ((ret_val=red_reg_rd(inst, *addr_error, got_data))!=0) {
                        return ret_val;
                    }

                    if (((*got_data)>>8)!=RED_SBUS_READ_SUCCESS) {
                        return REDWOOD_ERROR_SBUS_READ;
                    }

                    if (((*got_data) & 1) ==0) { // rx_rdy check
                        printf("Error: HI%d serdes%d RxRdy [0x%X] : 0x%X \n", 
					hi, serdes, *addr_error, *got_data);
                        return REDWOOD_ERROR_DEFAULT;
                    } else {
                            printf("HI%d serdes%d RxRdy\n", hi, serdes);
		    }
                }
            }
    }
    const int NI_START_OFFSET = RED_SERDES_START_OFFSET
                              + (RED_NUM_HI*RED_NUM_SERDES_PER_PORT)
                              ;
    // program the 16 NI serdes
    for (ni=0;  ni<RED_NUM_NI; ++ni) {
            for (serdes=0; serdes<RED_NUM_SERDES_PER_PORT; ++serdes) {
                const int ni_offset = (ni*RED_NUM_SERDES_PER_PORT)+serdes;
                const int sbus_addr = NI_START_OFFSET + ni_offset;
                if (ni_mask & (1<<ni_offset)) {
                    *addr_error =  RED_SBUS_ADDR(sbus_addr, 0xf); 
                    *exp_data = 1;

                    if ((ret_val=red_reg_rd(inst, *addr_error, got_data))!=0) {
                        return ret_val;
                    }

                    if (((*got_data)>>8)!=RED_SBUS_READ_SUCCESS) {
                        return REDWOOD_ERROR_SBUS_READ;
                    }

                    if (((*got_data) & 1) ==0) { // rx_rdy check
                        printf("Error: NI%d serdes%d RxRdy [0x%X] : 0x%X \n", 
					ni, serdes, *addr_error, *got_data);
                        return REDWOOD_ERROR_DEFAULT;
                    } else {
                           printf("NI%d serdes%d RxRdy\n", ni, serdes);
                    }
                }
            }
    }
    return 0;
}

/**
 Serdes access test

 Read all registers in each serdes 
 check that the access does not return errors

 \param hi_mask which HI instances to configure, one bit per serdes (there are 32 of them), HI0 serdes 0 is bit 0. To broadcast use -1
 \param ni_mask which NI instances to configure, one bit per serdes (there are 16 of them), NI0 serdes 0 is bit 0. To broadcast use -1
 \return 0 on success, otherwise a code defined in red_hw_error.h
*/
int red_hw_serdes_access_test(int inst, const int hi_mask, const short ni_mask, unsigned int* addr_error, unsigned int* exp_data, unsigned int* got_data) {
    int ret_val;
    int hi,ni,serdes,reg;

    for (hi=0;  hi<RED_NUM_HI; ++hi) {
            for (serdes=0; serdes<RED_NUM_SERDES_PER_PORT; ++serdes) {
                const int hi_offset = (hi*RED_NUM_SERDES_PER_PORT)+serdes;
                const int sbus_addr = hi_offset + RED_SERDES_START_OFFSET;
                if (hi_mask & (1<<hi_offset)) {
                    for (reg=0; reg<256; reg++) {
                        *addr_error =  RED_SBUS_ADDR(sbus_addr, reg); 

                        if ((ret_val=red_reg_rd(inst, *addr_error, got_data))!=0) {
                            return ret_val;
                        }

                        if (((*got_data)>>8)!=RED_SBUS_READ_SUCCESS) {
                            return REDWOOD_ERROR_SBUS_READ;
                        }
                    }
                }
            }
    }
    const int NI_START_OFFSET = RED_SERDES_START_OFFSET
                              + (RED_NUM_HI*RED_NUM_SERDES_PER_PORT)
                              ;
    for (ni=0;  ni<RED_NUM_NI; ++ni) {
            for (serdes=0; serdes<RED_NUM_SERDES_PER_PORT; ++serdes) {
                const int ni_offset = (ni*RED_NUM_SERDES_PER_PORT)+serdes;
                const int sbus_addr = NI_START_OFFSET + ni_offset;
                if (ni_mask & (1<<ni_offset)) {
                    for (reg=0; reg<256; reg++) {
                        *addr_error =  RED_SBUS_ADDR(sbus_addr, reg); 

                        if ((ret_val=red_reg_rd(inst, *addr_error, got_data))!=0) {
                            return ret_val;
                        }

                        if (((*got_data)>>8)!=RED_SBUS_READ_SUCCESS) {
                            return REDWOOD_ERROR_SBUS_READ;
                        }

                    }
                }
            }
    }
    return 0;
}

////////////////////////////////////////////////////////////////////
// Given a serdes SBUS address (1..48), set it up in near loopback mode
// This is the mode where tx goes straight to rx within the serdes
//
// \param serdes_sbus_address is a number from 1..48 with the following map:
//                       HI0: serdes 1..4
//                       HI1: serdes 5..8
//                       ...
//                       NI0: serdes 33..36
//                       NI3: serdes 45..48
// \param enable if set enable loopback, otherwise disable loopback
// \return 0 on success, otherwise a code defined in red_hw_error.h
////////////////////////////////////////////////////////////////////
int red_hw_serdes_near_loopback(int inst, const int serdes_sbus_address, const int enable) {
       int reg_addr;
       unsigned int reg_data;
       int ret_val;
       const int enable_effective = enable ? 1 : 0; // we assume 1 or 0

       const int ignore_strength_table[] = 
                { RED_HI0_IGNORE_RX_SIG_STRENGTH
                , RED_HI1_IGNORE_RX_SIG_STRENGTH
                , RED_HI2_IGNORE_RX_SIG_STRENGTH
                , RED_HI3_IGNORE_RX_SIG_STRENGTH
                , RED_HI4_IGNORE_RX_SIG_STRENGTH
                , RED_HI5_IGNORE_RX_SIG_STRENGTH
                , RED_HI6_IGNORE_RX_SIG_STRENGTH
                , RED_HI7_IGNORE_RX_SIG_STRENGTH
                , RED_NI0_IGNORE_RX_SIG_STRENGTH
                , RED_NI1_IGNORE_RX_SIG_STRENGTH
                , RED_NI2_IGNORE_RX_SIG_STRENGTH
                , RED_NI3_IGNORE_RX_SIG_STRENGTH
       };

       const unsigned int port   = (serdes_sbus_address-1)/RED_NUM_SERDES_PER_PORT;
       const unsigned int serdes = (serdes_sbus_address-1)%RED_NUM_SERDES_PER_PORT;

       if (port>=(sizeof(ignore_strength_table)/sizeof(int))) {
                return REDWOOD_ERROR_DEFAULT;
       }

       // do read-modify-write of the signal strength register
       // there are 4 bits in the register, but we only need to touch one
       if ((ret_val=red_reg_rd(inst, ignore_strength_table[port]<<2,&reg_data))!=0) {
                return ret_val;
       }
      
       // set the ignore strength register so the MAC still thinks we have a signal
       // (it won't pass data otherwise)
       reg_data = (reg_data & ~(1<<serdes))  // clear bit irrespective of enable/disable
                | (enable_effective<<serdes) // set for enable, do nothing for disable
                ;

       if ((ret_val=red_reg_wr(inst, ignore_strength_table[port]<<2,reg_data))!=0) {
                return ret_val;
       }

       // setup serdes' loopback mode
       reg_addr = RED_SBUS_ADDR(serdes_sbus_address,0xd);
       reg_data = ((enable_effective&1)<<7) // loopback en
                | (((enable_effective^1)&1)<<4) // tx_output_en
                | (1<<0) // gate
                ;
       ret_val=red_reg_wr(inst, reg_addr,reg_data);

       return ret_val;
}

////////////////////////////////////////////////////////////////////
// Given a serdes SBUS address (1..48), run a prbs test on it
// Remember the mapping: HI0: serdes 1..4
//                       HI1: serdes 5..8
//                       ...
//                       NI0: serdes 33..36
//                       NI3: serdes 45..48
// return 0 on success, otherwise a code defined in red_hw_error.h
////////////////////////////////////////////////////////////////////
int red_hw_serdes_prbs_test( int inst, const int serdes_sbus_address
                           , const int wait_time // microseconds
                           ) {
       int reg_addr;
       unsigned int reg_data;
       int ret_val;

       // sanity
       if ((serdes_sbus_address==0) || (serdes_sbus_address>48)) {
                return REDWOOD_ERROR_DEFAULT;
       }
  
       reg_addr = RED_SBUS_ADDR(serdes_sbus_address,0xa);
       reg_data = (1<<0); // sbus_tx_data_gate, initial set tx_pattern_gen_sel_cntl & tx_data_sel_cntl to 0
       if ((ret_val=red_reg_wr(inst, reg_addr,reg_data))!=0) {
                return ret_val;
       }

       reg_addr = RED_SBUS_ADDR(serdes_sbus_address,0x3);
       reg_data = (1<<0); // sbus_rx_data_gate
       if ((ret_val=red_reg_wr(inst, reg_addr,reg_data))!=0) {
                return ret_val;
       }

       reg_addr = RED_SBUS_ADDR(serdes_sbus_address,0x7);
       reg_data = (0<<0); // initial set the rx_pattern_cmp_sel & rx_sel_cntl to 0
       if ((ret_val=red_reg_wr(inst, reg_addr,reg_data))!=0) {
                return ret_val;
       }

       reg_addr = RED_SBUS_ADDR(serdes_sbus_address,0xa);
       reg_data = (1<<7) // tx_patter_gen_en
		| (1<<0); // sbus_tx_data_gate
       if ((ret_val=red_reg_wr(inst, reg_addr,reg_data))!=0) {
                return ret_val;
       }

       reg_addr = RED_SBUS_ADDR(serdes_sbus_address,0x7);
       reg_data = (1<<7); // rx_patter_cmp_en
       if ((ret_val=red_reg_wr(inst, reg_addr,reg_data))!=0) {
                return ret_val;
       }

       // clear sticky bits
       reg_addr = RED_SBUS_ADDR(serdes_sbus_address,0x4);
       reg_data = (1<<7)  // sbus_rx_error_monitor_en_cntl: enable error monitoring
                | (1<<6)  // sbus_rx_error_monitor_reset_cntl: reset error sticky bits
                | (1<<0)  // sbus_rx_error_monitor_gate: override pins, use PIOs
                ;
       if ((ret_val=red_reg_wr(inst, reg_addr,reg_data))!=0) {
                return ret_val;
       }

       // remove reset from sticky bits
       reg_addr = RED_SBUS_ADDR(serdes_sbus_address,0x4);
       reg_data = (1<<7)  // sbus_rx_error_monitor_en_cntl: enable error monitoring
                | (0<<6)  // sbus_rx_error_monitor_reset_cntl: reset error sticky bits
                | (1<<0)  // sbus_rx_error_monitor_gate: override pins, use PIOs
                ;
       if ((ret_val=red_reg_wr(inst, reg_addr,reg_data))!=0) {
                return ret_val;
       }

       // wait for time wait_time
       RED_SLEEP (wait_time);

       // check if errors occurred
       reg_addr = RED_SBUS_ADDR(serdes_sbus_address,0x1);
       if ((ret_val=red_reg_rd(inst, reg_addr,&reg_data))!=0) {
                return ret_val;
       }
       if ((reg_data>>8)!=RED_SBUS_READ_SUCCESS) {
            return REDWOOD_ERROR_SBUS_READ;
       }
       if ((reg_data&0x3)!=0) {
            return REDWOOD_ERROR_SERDES_PRBS_FAILURE;
       } else {
		static int flag = 0;
		if (!flag) {
		int cnt = 0;
		for (cnt = 0; cnt <= 0x0F; cnt++) {
			reg_addr = RED_SBUS_ADDR(serdes_sbus_address,cnt);
			if ((ret_val=red_reg_rd(inst, reg_addr,&reg_data))!=0) {
				return ret_val;
			}
			if (diag_debug) {
			    printf("  [0x%04X (0x%X)] : 0x%X\n", reg_addr, cnt, reg_data);
			}
		}

		reg_addr = RED_SBUS_ADDR(serdes_sbus_address,0x14);
		if ((ret_val=red_reg_rd(inst, reg_addr,&reg_data))!=0) {
			return ret_val;
		}
		if (diag_debug) {
		    printf("  [0x%04X (0x14)] : 0x%X\n", reg_addr, reg_data);
		}
		}
		flag = 1;

       }
       return 0;
}

// Pattern = 0 for 2^7
// Pattern = 1 for 2^23
// Pattern = 2 for 2^15
// Pattern = 3 for 2^31
int red_hw_serdes_prbs_test_start( int inst, const int serdes_sbus_address
                           , const int wait_time // microseconds
                           , const int pattern) {
       int reg_addr;
       unsigned int reg_data;
       int ret_val;

       // sanity
       if ((serdes_sbus_address==0) || (serdes_sbus_address>48)) {
                return REDWOOD_ERROR_DEFAULT;
       }
  
       reg_addr = RED_SBUS_ADDR(serdes_sbus_address,0xa);
       reg_data = (1<<0); // sbus_tx_data_gate, initial set tx_pattern_gen_sel_cntl & tx_data_sel_cntl to 0

       if ((ret_val=red_reg_wr(inst, reg_addr,reg_data))!=0) {
                return ret_val;
       }

       reg_addr = RED_SBUS_ADDR(serdes_sbus_address,0x3);
       reg_data = (1<<0); // sbus_rx_data_gate
       if ((ret_val=red_reg_wr(inst, reg_addr,reg_data))!=0) {
                return ret_val;
       }

       reg_addr = RED_SBUS_ADDR(serdes_sbus_address,0x7);
       reg_data = (0<<0); // initial set the rx_pattern_cmp_sel & rx_sel_cntl to 0
       if ((ret_val=red_reg_wr(inst, reg_addr,reg_data))!=0) {
                return ret_val;
       }

       reg_addr = RED_SBUS_ADDR(serdes_sbus_address,0xa);
       reg_data = (1<<7) // tx_patter_gen_en
		| ((pattern&3) << 4)
		| (1<<0); // sbus_tx_data_gate
       if ((ret_val=red_reg_wr(inst, reg_addr,reg_data))!=0) {
                return ret_val;
       }

       reg_addr = RED_SBUS_ADDR(serdes_sbus_address,0x7);
       reg_data = ((1<<7) | (pattern&3)); // rx_patter_cmp_en
       if ((ret_val=red_reg_wr(inst, reg_addr,reg_data))!=0) {
                return ret_val;
       }

       // clear sticky bits
       reg_addr = RED_SBUS_ADDR(serdes_sbus_address,0x4);
       reg_data = (1<<7)  // sbus_rx_error_monitor_en_cntl: enable error monitoring
                | (1<<6)  // sbus_rx_error_monitor_reset_cntl: reset error sticky bits
                | (3<<1)  // Enable rx_cmp_en counters 
                | (1<<0)  // sbus_rx_error_monitor_gate: override pins, use PIOs
                ;
       if ((ret_val=red_reg_wr(inst, reg_addr,reg_data))!=0) {
                return ret_val;
       }

       // remove reset from sticky bits
       reg_addr = RED_SBUS_ADDR(serdes_sbus_address,0x4);
       reg_data = (1<<7)  // sbus_rx_error_monitor_en_cntl: enable error monitoring
                | (0<<6)  // sbus_rx_error_monitor_reset_cntl: reset error sticky bits
                | (3<<1)  // Enable rx_cmp_en counters 
                | (1<<0)  // sbus_rx_error_monitor_gate: override pins, use PIOs
                ;
       if ((ret_val=red_reg_wr(inst, reg_addr,reg_data))!=0) {
                return ret_val;
       }

       return 0;
}

int red_hw_serdes_prbs_test_stop( int inst, const int serdes_sbus_address) 
{
       int reg_addr;
       unsigned int reg_data;
       int ret_val;

       // Disable Rx
       reg_addr = RED_SBUS_ADDR(serdes_sbus_address,0x7);
       reg_data = 0x00; 
       if ((ret_val=red_reg_wr(inst, reg_addr,reg_data))!=0) {
                return ret_val;
       }

       // Disable Tx
       reg_addr = RED_SBUS_ADDR(serdes_sbus_address,0xa);
       reg_data = 0x03; 
       if ((ret_val=red_reg_wr(inst, reg_addr,reg_data))!=0) {
                return ret_val;
       }

	return (0);
}

int red_hw_serdes_prbs_test_check( int inst, const int serdes_sbus_address
                           , const int wait_time // microseconds
                           ) {
       int reg_addr;
       unsigned int reg_data;
       int ret_val, err=0;

       // wait for time wait_time
       RED_SLEEP (wait_time);

       // check if errors occurred
       reg_addr = RED_SBUS_ADDR(serdes_sbus_address,0x1);
       if ((ret_val=red_reg_rd(inst, reg_addr,&reg_data))!=0) {
                return ret_val;
       }
       if ((reg_data>>8)!=RED_SBUS_READ_SUCCESS) {
            return REDWOOD_ERROR_SBUS_READ;
       }
       if ((reg_data&0x3)!=0) {
		int cnt = 0;
		for (cnt = 0; cnt <= 0x0F; cnt++) {
			reg_addr = RED_SBUS_ADDR(serdes_sbus_address,cnt);
			if ((ret_val=red_reg_rd(inst, reg_addr,&reg_data))!=0) {
				return ret_val;
			}
		}

		reg_addr = RED_SBUS_ADDR(serdes_sbus_address,0x14);
		if ((ret_val=red_reg_rd(inst, reg_addr,&reg_data))!=0) {
			return ret_val;
		}
                err= REDWOOD_ERROR_SERDES_PRBS_FAILURE;
       }
       // clear sticky bits
       reg_addr = RED_SBUS_ADDR(serdes_sbus_address,0x4);
       reg_data = (1<<7)  // sbus_rx_error_monitor_en_cntl: enable error monitoring
                | (1<<6)  // sbus_rx_error_monitor_reset_cntl: reset error sticky bits
                | (1<<0)  // sbus_rx_error_monitor_gate: override pins, use PIOs
                ;
       if ((ret_val=red_reg_wr(inst, reg_addr,reg_data))!=0) {
                return ret_val;
       }

       // remove reset from sticky bits
       reg_addr = RED_SBUS_ADDR(serdes_sbus_address,0x4);
       reg_data = (1<<7)  // sbus_rx_error_monitor_en_cntl: enable error monitoring
                | (0<<6)  // sbus_rx_error_monitor_reset_cntl: reset error sticky bits
                | (1<<0)  // sbus_rx_error_monitor_gate: override pins, use PIOs
                ;
       if ((ret_val=red_reg_wr(inst, reg_addr,reg_data))!=0) {
                return ret_val;
       }
       return err;
}
/////////////////////////////////////////////////////////////
// configure a given serdes to do rx to tx loopback
// Remember the mapping: HI0: serdes 1..4
//                       HI1: serdes 5..8
//                       ...
//                       NI0: serdes 33..36
//                       NI3: serdes 45..48
// \return 0 on success, otherwise a code defined in red_hw_error.h
////////////////////////////////////////////////////////////////////
int red_hw_serdes_far_loopback(int inst, const int serdes_sbus_address, const int enable) {
           int reg_addr = RED_SBUS_ADDR(serdes_sbus_address,0x9);
           int reg_data = ((enable&1)<<4) // far loopback en
                        | (1<<0) // sbus_rx_far_loopback_gate
                        ;
           int ret_val=red_reg_wr(inst, reg_addr, reg_data);
           return ret_val;
}

typedef struct _serdes_debug_s_
{
	uint8_t sbus;
	uint8_t reg;
	uint8_t rd;
	uint8_t data;
} serdes_debug_t;

int red_hw_serdes_debug_info(int inst, const int serdes_sbus_address)
{
	int incr, cnt, count, reg_addr, reg_data = 0x00, ret_val = 0;

	serdes_debug_t srd_dbg_l[] = {
	{serdes_sbus_address, 0x04, 0,	0xC1},
	{serdes_sbus_address, 0x04, 0,	0x81},
	{serdes_sbus_address, 0x04, 0,	0x01},
	{serdes_sbus_address, 0x07, 1,	0x00},
	{serdes_sbus_address, 0x08, 1,	0x00},
	{serdes_sbus_address, 0x09, 1,	0x00},
	{serdes_sbus_address, 0x0A, 1,	0x00},
	};


	for (cnt = 0; cnt < 4; cnt++) {
		reg_addr = RED_SBUS_ADDR(0xFE, (0xA + cnt));
		ret_val = red_reg_rd(inst, reg_addr, &reg_data);
		if (ret_val) {
			printf("  Error: Reading SBUS (0xFE, 0x0A)\n");
			return (ret_val);
		}
		printf(" Rd [0xFE 0x%02X] 0x%02X : 0x%02X\n", (0xA+cnt), reg_addr, reg_data); 
	}

	count = sizeof(srd_dbg_l)/sizeof(serdes_debug_t);
	for (incr = 0; incr < 0x1F; incr+=2) {
	for (cnt = 0; cnt < count; cnt++) {
		reg_addr = RED_SBUS_ADDR(srd_dbg_l[cnt].sbus, srd_dbg_l[cnt].reg);
		if (srd_dbg_l[cnt].rd) {
			ret_val = red_reg_rd(inst, reg_addr, &reg_data);
		} else {
			reg_data = incr+srd_dbg_l[cnt].data;
			ret_val = red_reg_wr(inst, reg_addr, reg_data); 
		}
		if (ret_val) {
			printf("  Error: %s SBUS (0x%02X, 0x%02X)\n", 
					srd_dbg_l[cnt].rd ? "Rd" : "Wr",
					srd_dbg_l[cnt].sbus,
					srd_dbg_l[cnt].reg);
			return (ret_val);
		}
		printf(" %s [0x%02X 0x%02X] 0x%02X : 0x%02X\n", (srd_dbg_l[cnt].rd ? "Rd" : "Wr"), 
			srd_dbg_l[cnt].sbus, reg_addr, srd_dbg_l[cnt].reg, reg_data); 
	}
	}

	reg_addr = RED_SBUS_ADDR(0x1E, 0x1);
	reg_data = 0x04;
	ret_val = red_reg_wr(inst, reg_addr, reg_data); 
	printf(" Wr [0x1E 0x%02X] 0x01 : 0x%02X\n", reg_addr, reg_data); 

	for (cnt = 0; cnt < 0x2D; cnt++) {
		reg_addr = RED_SBUS_ADDR(serdes_sbus_address, cnt);
		ret_val = red_reg_rd(inst, reg_addr, &reg_data); 
		printf(" Rd [0x%02X 0x%02X] 0x%02X : 0x%02X\n", 	
			serdes_sbus_address, reg_addr, cnt, reg_data); 
	}

	reg_addr = RED_SBUS_ADDR(0x1E, 0x1);
	reg_data = 0x00;
	ret_val = red_reg_wr(inst, reg_addr, reg_data); 
	printf(" Wr [0x1E 0x%02X] 0x01 : 0x%02X\n", reg_addr, reg_data); 

	return (0);
}

int red_hw_serdes_avago_prbs(int inst, const int serdes_sbus_address)
{
	int cnt, count, reg_addr, reg_data = 0x00, ret_val = 0;

	serdes_debug_t srd_dbg_l[] = {
	{serdes_sbus_address, 0x00, 0,	0x01},
	{serdes_sbus_address, 0x02, 0,	0x00},
	{serdes_sbus_address, 0x08, 0,	0x00},
	{serdes_sbus_address, 0x0C, 0,	0x00},
	{serdes_sbus_address, 0x2C, 0,	0x00},
	{serdes_sbus_address, 0x0F, 0,	0x00},
	{serdes_sbus_address, 0x10, 0,	0x00},
	{serdes_sbus_address, 0x13, 0,	0x01},
	{serdes_sbus_address, 0x12, 0,	0x03},
	{serdes_sbus_address, 0x14, 0,	0x00},
	{serdes_sbus_address, 0x15, 0,	0x00},
	{serdes_sbus_address, 0x16, 0,	0x00},
	{serdes_sbus_address, 0x1A, 0,	0x00},
	{serdes_sbus_address, 0x1B, 0,	0x01},
	{serdes_sbus_address, 0x1C, 0,	0x00},
	{serdes_sbus_address, 0x1D, 0,	0x00},
	{serdes_sbus_address, 0x1E, 0,	0x00},
	{serdes_sbus_address, 0x26, 0,	0x00},

	{serdes_sbus_address, 0x22, 0,	0x00},
	{serdes_sbus_address, 0x36, 0,	0x00},
	{serdes_sbus_address, 0x37, 0,	0x01},
	{serdes_sbus_address, 0x38, 0,	0x03},
	{serdes_sbus_address, 0x35, 0,	0x0F},
	{serdes_sbus_address, 0x17, 0,	0x13},
	{serdes_sbus_address, 0x22, 0,	0x03},
	// 100 ms delay
	{serdes_sbus_address, 0x03, 0,	0x01},
	{serdes_sbus_address, 0x05, 0,	0xaa},
	{serdes_sbus_address, 0x06, 0,	0x00},
	{serdes_sbus_address, 0x07, 0,	0x83},
	{serdes_sbus_address, 0x09, 0,	0x01},
	{serdes_sbus_address, 0x0a, 0,	0xb1},
	{serdes_sbus_address, 0x0b, 0,	0x00},
	{serdes_sbus_address, 0x0d, 0,	0xf1},
	{serdes_sbus_address, 0x0e, 0,	0xaf},
	{serdes_sbus_address, 0x04, 0,	0xc7},
	{serdes_sbus_address, 0x04, 0,	0x87},
	{serdes_sbus_address, 0x01, 2,	0x24},
	};

	count = sizeof(srd_dbg_l)/sizeof(serdes_debug_t);
	for (cnt = 0; !ret_val && (cnt < count); cnt++) {
		reg_addr = RED_SBUS_ADDR(srd_dbg_l[cnt].sbus, srd_dbg_l[cnt].reg);
		if (srd_dbg_l[cnt].rd) {
			ret_val = red_reg_rd(inst, reg_addr, &reg_data);
		} else {
			reg_data = srd_dbg_l[cnt].data;
			ret_val = red_reg_wr(inst, reg_addr, reg_data); 
		}
		if (ret_val) {
			printf("  Error: %s SBUS (0x%02X, 0x%02X)\n", 
					srd_dbg_l[cnt].rd ? "Rd" : "Wr",
					srd_dbg_l[cnt].sbus,
					srd_dbg_l[cnt].reg);
			return (ret_val);
		}
		printf(" %s [0x%02X 0x%02X] 0x%02X : 0x%02X\n", (srd_dbg_l[cnt].rd ? "Rd" : "Wr"), 
			srd_dbg_l[cnt].sbus, reg_addr, srd_dbg_l[cnt].reg, reg_data); 

		if ((srd_dbg_l[cnt].reg == 0x22) && 
		    (srd_dbg_l[cnt].data == 0x3)) {
			printf(" Delay 100 ms\n");
			usleep (100 * 1000);
		}
	}

	return(ret_val);
}



int red_hw_serdes_diag(int inst, const int serdes_sbus_address)
{
	int cnt, count, reg_addr, reg_data = 0x00, ret_val = 0;

	serdes_debug_t srd_dbg_l[] = {
	{0xfe, 0x0a, 01, 00},
	{0xfe, 0x0b, 01, 00},
	{0xfe, 0x0c, 01, 00},
	{0xfe, 0x0d, 01, 00},

        {serdes_sbus_address,  0x04, 00, 0xc1},
        {serdes_sbus_address,  0x04, 00, 0x81},
        {serdes_sbus_address,  0x04, 00, 0x01},
        {serdes_sbus_address,  0x07, 01, 0x00},
        {serdes_sbus_address,  0x08, 01, 0x00},
        {serdes_sbus_address,  0x09, 01, 0x00},
        {serdes_sbus_address,  0x0a, 01, 0x00},

        {serdes_sbus_address,  0x04, 00, 0xc3},
        {serdes_sbus_address,  0x04, 00, 0x83},
        {serdes_sbus_address,  0x04, 00, 0x03},
        {serdes_sbus_address,  0x07, 01, 0x00},
        {serdes_sbus_address,  0x08, 01, 0x00},
        {serdes_sbus_address,  0x09, 01, 0x00},
        {serdes_sbus_address,  0x0a, 01, 0x00},

        {serdes_sbus_address,  0x04, 00, 0xc5},
        {serdes_sbus_address,  0x04, 00, 0x85},
        {serdes_sbus_address,  0x04, 00, 0x05},
        {serdes_sbus_address,  0x07, 01, 0x00},
        {serdes_sbus_address,  0x08, 01, 0x00},
        {serdes_sbus_address,  0x09, 01, 0x00},
        {serdes_sbus_address,  0x0a, 01, 0x00},

        {serdes_sbus_address,  0x04, 00, 0xc7},
        {serdes_sbus_address,  0x04, 00, 0x87},
        {serdes_sbus_address,  0x04, 00, 0x07},
        {serdes_sbus_address,  0x07, 01, 0x00},
        {serdes_sbus_address,  0x08, 01, 0x00},
        {serdes_sbus_address,  0x09, 01, 0x00},
        {serdes_sbus_address,  0x0a, 01, 0x00},

        {serdes_sbus_address,  0x04, 00, 0xc9},
        {serdes_sbus_address,  0x04, 00, 0x89},
        {serdes_sbus_address,  0x04, 00, 0x09},
        {serdes_sbus_address,  0x07, 01, 0x00},
        {serdes_sbus_address,  0x08, 01, 0x00},
        {serdes_sbus_address,  0x09, 01, 0x00},
        {serdes_sbus_address,  0x0a, 01, 0x00},

        {serdes_sbus_address,  0x04, 00, 0xcb},
        {serdes_sbus_address,  0x04, 00, 0x8b},
        {serdes_sbus_address,  0x04, 00, 0x0b},
        {serdes_sbus_address,  0x07, 01, 0x00},
        {serdes_sbus_address,  0x08, 01, 0x00},
        {serdes_sbus_address,  0x09, 01, 0x00},
        {serdes_sbus_address,  0x0a, 01, 0x00},

        {serdes_sbus_address,  0x04, 00, 0xcd},
        {serdes_sbus_address,  0x04, 00, 0x8d},
        {serdes_sbus_address,  0x04, 00, 0x0d},
        {serdes_sbus_address,  0x07, 01, 0x00},
        {serdes_sbus_address,  0x08, 01, 0x00},
        {serdes_sbus_address,  0x09, 01, 0x00},
        {serdes_sbus_address,  0x0a, 01, 0x00},

        {serdes_sbus_address,  0x04, 00, 0xcf},
        {serdes_sbus_address,  0x04, 00, 0x8f},
        {serdes_sbus_address,  0x04, 00, 0x0f},
        {serdes_sbus_address,  0x07, 01, 0x00},
        {serdes_sbus_address,  0x08, 01, 0x00},
        {serdes_sbus_address,  0x09, 01, 0x00},
        {serdes_sbus_address,  0x0a, 01, 0x00},

        {serdes_sbus_address,  0x04, 00, 0xd1},
        {serdes_sbus_address,  0x04, 00, 0x91},
        {serdes_sbus_address,  0x04, 00, 0x11},
        {serdes_sbus_address,  0x07, 01, 0x00},
        {serdes_sbus_address,  0x08, 01, 0x00},
        {serdes_sbus_address,  0x09, 01, 0x00},
        {serdes_sbus_address,  0x0a, 01, 0x00},

        {serdes_sbus_address,  0x04, 00, 0xd3},
        {serdes_sbus_address,  0x04, 00, 0x93},
        {serdes_sbus_address,  0x04, 00, 0x13},
        {serdes_sbus_address,  0x07, 01, 0x00},
        {serdes_sbus_address,  0x08, 01, 0x00},
        {serdes_sbus_address,  0x09, 01, 0x00},
        {serdes_sbus_address,  0x0a, 01, 0x00},

        {serdes_sbus_address,  0x04, 00, 0xd5},
        {serdes_sbus_address,  0x04, 00, 0x95},
        {serdes_sbus_address,  0x04, 00, 0x15},
        {serdes_sbus_address,  0x07, 01, 0x00},
        {serdes_sbus_address,  0x08, 01, 0x00},
        {serdes_sbus_address,  0x09, 01, 0x00},
        {serdes_sbus_address,  0x0a, 01, 0x00},

        {serdes_sbus_address,  0x04, 00, 0xd7},
        {serdes_sbus_address,  0x04, 00, 0x97},
        {serdes_sbus_address,  0x04, 00, 0x17},
        {serdes_sbus_address,  0x07, 01, 0x00},
        {serdes_sbus_address,  0x08, 01, 0x00},
        {serdes_sbus_address,  0x09, 01, 0x00},
        {serdes_sbus_address,  0x0a, 01, 0x00},

        {serdes_sbus_address,  0x04, 00, 0xd9},
        {serdes_sbus_address,  0x04, 00, 0x99},
        {serdes_sbus_address,  0x04, 00, 0x19},
        {serdes_sbus_address,  0x07, 01, 0x00},
        {serdes_sbus_address,  0x08, 01, 0x00},
        {serdes_sbus_address,  0x09, 01, 0x00},
        {serdes_sbus_address,  0x0a, 01, 0x00},

        {serdes_sbus_address,  0x04, 00, 0xdb},
        {serdes_sbus_address,  0x04, 00, 0x9b},
        {serdes_sbus_address,  0x04, 00, 0x1b},
        {serdes_sbus_address,  0x07, 01, 0x00},
        {serdes_sbus_address,  0x08, 01, 0x00},
        {serdes_sbus_address,  0x09, 01, 0x00},
        {serdes_sbus_address,  0x0a, 01, 0x00},

        {serdes_sbus_address,  0x04, 00, 0xdd},
        {serdes_sbus_address,  0x04, 00, 0x9d},
        {serdes_sbus_address,  0x04, 00, 0x1d},
        {serdes_sbus_address,  0x07, 01, 0x00},
        {serdes_sbus_address,  0x08, 01, 0x00},
        {serdes_sbus_address,  0x09, 01, 0x00},
        {serdes_sbus_address,  0x0a, 01, 0x00},

        {serdes_sbus_address,  0x04, 00, 0xdf},
        {serdes_sbus_address,  0x04, 00, 0x9f},
        {serdes_sbus_address,  0x04, 00, 0x1f},
        {serdes_sbus_address,  0x07, 01, 0x00},
        {serdes_sbus_address,  0x08, 01, 0x00},
        {serdes_sbus_address,  0x09, 01, 0x00},
        {serdes_sbus_address,  0x0a, 01, 0x00},

        {serdes_sbus_address,  0x1e, 00, 0x04},
        {serdes_sbus_address,  0x00, 01, 0x00},
        {serdes_sbus_address,  0x01, 01, 0x00},
        {serdes_sbus_address,  0x02, 01, 0x00},
        {serdes_sbus_address,  0x03, 01, 0x00},
        {serdes_sbus_address,  0x04, 01, 0x00},
        {serdes_sbus_address,  0x05, 01, 0x00},
        {serdes_sbus_address,  0x06, 01, 0x00},
        {serdes_sbus_address,  0x07, 01, 0x00},
        {serdes_sbus_address,  0x08, 01, 0x00},
        {serdes_sbus_address,  0x09, 01, 0x00},
        {serdes_sbus_address,  0x0a, 01, 0x00},
        {serdes_sbus_address,  0x0b, 01, 0x00},
        {serdes_sbus_address,  0x0c, 01, 0x00},
        {serdes_sbus_address,  0x0d, 01, 0x00},
        {serdes_sbus_address,  0x0e, 01, 0x00},
        {serdes_sbus_address,  0x0f, 01, 0x00},
        {serdes_sbus_address,  0x10, 01, 0x00},
        {serdes_sbus_address,  0x11, 01, 0x00},
        {serdes_sbus_address,  0x12, 01, 0x00},
        {serdes_sbus_address,  0x13, 01, 0x00},
        {serdes_sbus_address,  0x14, 01, 0x00},
        {serdes_sbus_address,  0x15, 01, 0x00},
        {serdes_sbus_address,  0x16, 01, 0x00},
        {serdes_sbus_address,  0x17, 01, 0x00},
        {serdes_sbus_address,  0x18, 01, 0x00},
        {serdes_sbus_address,  0x19, 01, 0x00},
        {serdes_sbus_address,  0x1a, 01, 0x00},
        {serdes_sbus_address,  0x1b, 01, 0x00},
        {serdes_sbus_address,  0x1c, 01, 0x00},
        {serdes_sbus_address,  0x1d, 01, 0x00},
        {serdes_sbus_address,  0x1e, 01, 0x00},
        {serdes_sbus_address,  0x1f, 01, 0x00},
        {serdes_sbus_address,  0x20, 01, 0x00},
        {serdes_sbus_address,  0x21, 01, 0x00},
        {serdes_sbus_address,  0x22, 01, 0x00},
        {serdes_sbus_address,  0x23, 01, 0x00},
        {serdes_sbus_address,  0x24, 01, 0x00},
        {serdes_sbus_address,  0x25, 01, 0x00},
        {serdes_sbus_address,  0x26, 01, 0x00},
        {serdes_sbus_address,  0x27, 01, 0x00},
        {serdes_sbus_address,  0x28, 01, 0x00},
        {serdes_sbus_address,  0x29, 01, 0x00},
        {serdes_sbus_address,  0x2a, 01, 0x00},
        {serdes_sbus_address,  0x2b, 01, 0x00},
        {serdes_sbus_address,  0x2c, 01, 0x00},
        {serdes_sbus_address,  0xfe, 01, 0x00},

        {serdes_sbus_address,  0x1e, 00, 0x00},

	};

	count = sizeof(srd_dbg_l)/sizeof(serdes_debug_t);
	for (cnt = 0; !ret_val && (cnt < count); cnt++) {
		reg_addr = RED_SBUS_ADDR(srd_dbg_l[cnt].sbus, srd_dbg_l[cnt].reg);
		if (srd_dbg_l[cnt].rd) {
			ret_val = red_reg_rd(inst, reg_addr, &reg_data);
		} else {
			reg_data = srd_dbg_l[cnt].data;
			ret_val = red_reg_wr(inst, reg_addr, reg_data); 
		}
		if (ret_val) {
			printf("  Error: %s SBUS (0x%02X, 0x%02X)\n", 
					srd_dbg_l[cnt].rd ? "Rd" : "Wr",
					srd_dbg_l[cnt].sbus,
					srd_dbg_l[cnt].reg);
			return (ret_val);
		}
		printf(" %s [0x%02X 0x%02X] 0x%02X : 0x%02X\n", (srd_dbg_l[cnt].rd ? "Rd" : "Wr"), 
			srd_dbg_l[cnt].sbus, reg_addr, srd_dbg_l[cnt].reg, reg_data); 

		if ((srd_dbg_l[cnt].reg == 0x22) && 
		    (srd_dbg_l[cnt].data == 0x3)) {
			printf(" Delay 100 ms\n");
			usleep (100 * 1000);
		}
	}

	return(ret_val);
}
