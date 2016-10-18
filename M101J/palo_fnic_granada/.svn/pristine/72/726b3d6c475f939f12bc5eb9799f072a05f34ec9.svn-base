//////////////////////////////////////////////////////////////////
// $Author: skadari $
// $Revision: 1.3 $
//
// A test to scrub all cfg registers in redwood
//////////////////////////////////////////////////////////////////
#include "red_hw_cfg_reg_test.h"
#include "red_hw_error.h"

#ifdef RED_VERIFICATION
#include "Knob.h"
#define RED_SRAND(seed) RandKnob my_rand(#seed)
#define RED_RAND my_rand.eval()
#else
#include <stdio.h>
#include <stdlib.h>
#define RED_SRAND(seed) srand(seed)  
#define RED_RAND rand()
#endif

////////////////////////////////////////////////////////////////////////
// externally defined symbols
////////////////////////////////////////////////////////////////////////
extern int red_reg_wr(int inst, unsigned int address, unsigned int  data);  // address is byte aligned
extern int red_reg_rd(int inst, unsigned int address, unsigned int* data); // address is byte aligned

/**********************************************************
A test that scrubs all config registers in redwood
It first reads the default value
Then it writes a random value
Then it reads it back
\param seed used to initialize random generator
\param check_default if 0, don't check default values
\param addr_error returns the address where the first error occurred (byte aligned)
\param exp_data returns the data that was expected
\param got_data returns the data that was received
\return 0 on success, an error code otherwise
**********************************************************/
int red_hw_cfg_reg_test (int inst, const int seed, const int check_default, unsigned int* addr_error,unsigned  int* exp_data,unsigned  int* got_data) {
        int ret_val, i;
        unsigned int read_data;
	const table_entry_t *pentry = red_hw_cfg_reg_table;
        const int TABLE_SIZE = sizeof(red_hw_cfg_reg_table)/sizeof(table_entry_t);

        if ((ret_val=red_reg_rd(inst, (RED_MISC_JTAG_ID)*4, &read_data))!=0) {
            return ret_val;
        }
	if (read_data == REDWOOD_VERIOSN_REVB) {
		pentry = red_hw_cfg_reg_table_b;
	}

        RED_SRAND(seed);
        for (i=0; i<TABLE_SIZE; ++i) {
                const int byte_address = pentry[i].word_addr<<2;
                *addr_error=byte_address;

                if (check_default) {
                    if ((ret_val=red_reg_rd(inst, byte_address, &read_data))!=0) {
                        return ret_val;
                    }
                    if (read_data!=pentry[i].init) {
                        *exp_data=pentry[i].init;
                        *got_data=read_data;
			printf("Dflt [0x%08X : Exp 0x%08X] : 0x%08X\n", byte_address>>2, *exp_data, *got_data);
                        return REDWOOD_ERROR_DEFAULT;
                    }
                }

                // write a random value
                const unsigned int write_data= RED_RAND;
                if ((ret_val=red_reg_wr(inst, byte_address, write_data))!=0) {
                    return ret_val;
                }
                // read it back
                if ((ret_val=red_reg_rd(inst, byte_address, &read_data))!=0) {
                    return ret_val;
                }

                const unsigned int MASK = pentry[i].mask;
                // compare it to the write value
                if (read_data != (write_data & MASK)) {
                    *exp_data=write_data & MASK;
                    *got_data=read_data;
                   return REDWOOD_ERROR_DEFAULT;
                }
        }
        return 0; /*success*/
}
