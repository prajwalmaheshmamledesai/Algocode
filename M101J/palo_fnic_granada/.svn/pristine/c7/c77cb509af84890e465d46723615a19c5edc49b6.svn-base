//////////////////////////////////////////////////////////////////
// $Author: skadari $
// $Revision: 1.2 $
//
// A test to scrub all int registers in redwood
// XXX update to check interrupt set condition in real world
//////////////////////////////////////////////////////////////////
#include "red_hw_int_reg_test.h"
#include "red_hw_error.h"

////////////////////////////////////////////////////////////////////////
// externally defined symbols
////////////////////////////////////////////////////////////////////////
extern int red_reg_wr(int inst, unsigned int address, unsigned int  data);  // address is byte aligned
extern int red_reg_rd(int inst, unsigned int address, unsigned int* data); // address is byte aligned

#ifdef RED_VERIFICATION
#include "Knob.h"
#include "msgFormat.h"
#include "TestBuilder.h"
#define RED_SRAND(seed) RandKnob my_rand(#seed)
#define RED_RAND my_rand.eval()
tbvSignalHdlT interrupt("top.O_INT0",tbvEnumsT::READ_ONLY);
#else
#include <stdio.h>
#include <stdlib.h>
#define RED_SRAND(seed) srand(seed)  
#define RED_RAND rand()
#define EXIT return
#endif


int red_hw_int_reg_test (int inst, const int seed, unsigned int* last_addr, unsigned int* exp_data, unsigned int* got_data) {
        int ret_val, i;
        unsigned int read_data;
        const int TABLE_SIZE = sizeof(addr_table)/sizeof(addr_table_t);
        RED_SRAND(seed);

        for (i=0; i<TABLE_SIZE; ++i) {
                const int byte_address = addr_table[i].word_addr<<2;

                *last_addr=byte_address;
#ifdef RED_VERIFICATION
                NU_LOG << " clearing at address 0x" << hex<< *last_addr << dec <<endl;
#endif
                // clear all interrupts
                if ((ret_val=red_reg_wr(inst, byte_address, ~0))!=0) {
                    return ret_val;
                }

                *last_addr=byte_address+4;
#ifdef RED_VERIFICATION
                NU_LOG << " enabling at address 0x" << hex<< *last_addr << dec <<endl;
#endif
                // enable all interrupts
                if ((ret_val=red_reg_wr(inst, *last_addr, ~0))!=0) {
                    return ret_val;
                }
        }
        for (i=0; i<TABLE_SIZE; ++i) {
                const unsigned int byte_address = addr_table[i].word_addr<<2;
                const unsigned int write_data= RED_RAND | 1/*make sure at least one bit set*/;
                const unsigned int exp_int_bit = addr_table[i].int_bit;

                *last_addr=byte_address+8;
#ifdef RED_VERIFICATION
                NU_LOG << " forcing int at address 0x" << hex<< *last_addr<< dec <<endl;
#endif
                // generate an interrupt on this register
                if ((ret_val=red_reg_wr(inst, *last_addr, write_data))!=0) {
                    return ret_val;
                }
                // flush write
                *last_addr=byte_address;
                if ((ret_val=red_reg_rd(inst, *last_addr, &read_data))!=0) {
                    return ret_val;
                }

#ifdef RED_VERIFICATION
                // make sure interrupt is set.
                if (interrupt!=1) {
                    NU_LOG << " ERROR: interrupt should be 1\n";
                    EXIT(1);
                }

                NU_LOG << " reading address 0x" << RED_MISC_RNAXI_INTERRUPT_READ << endl;
#endif
                *last_addr=RED_MISC_RNAXI_INTERRUPT_READ<<2;
                if ((ret_val=red_reg_rd(inst, *last_addr, &read_data))!=0) {
                    return ret_val;
                }
                if (read_data!=exp_int_bit) {
#ifdef RED_VERIFICATION
                    NU_LOG << " ERROR: int_bit got 0x" << hex << read_data << " exp 0x" << exp_int_bit << endl;
#else
		    printf("ERROR: int_bit got 0x%X, exp 0x%X\n", read_data, exp_int_bit);
#endif
                    EXIT(1);
                }

                *last_addr=byte_address;
#ifdef RED_VERIFICATION
                NU_LOG << " clear interrupt at 0x" << hex << *last_addr << dec << endl;
#endif
                if ((ret_val=red_reg_wr(inst, *last_addr, write_data))!=0) {
                    return ret_val;
                }
                // flush
                if ((ret_val=red_reg_rd(inst, *last_addr, &read_data))!=0) {
                    return ret_val;
                }
#ifdef RED_VERIFICATION
                // make sure interrupt is unset.
                if (interrupt!=0) {
                        NU_LOG << " ERROR: interrupt should be 0\n";
                        EXIT(1);
                }
#endif
        }
        return 0; // success
}
