//////////////////////////////////////////////////////////////////
// $Author: pkaplan $
// $Revision: 1.1 $
//
// A test to check read-onlyness on all status registers in redwood
//////////////////////////////////////////////////////////////////
#include "red_hw_sta_reg_test.h"
#include "red_hw_error.h"

#ifdef RED_VERIFICATION
#include "Knob.h"
#include "msgFormat.h"
#define RED_SRAND(seed) RandKnob my_rand(#seed)
#define RED_RAND my_rand.eval()
#else
#include <stdlib.h>
#define RED_SRAND(seed) srand(seed)  
#define RED_RAND rand()
#endif

////////////////////////////////////////////////////////////////////////
// externally defined symbols
////////////////////////////////////////////////////////////////////////
extern int red_reg_wr(int inst,unsigned int address, unsigned int  data);  // address is byte aligned
extern int red_reg_rd(int inst, unsigned int address, unsigned int* data); // address is byte aligned

/**********************************************************
A test that scrubs all status registers in redwood
It first reads the default value
Then it writes a random value
Then it reads it back
It better match the original read since it's read only
\param seed used to initialize random generator
\param addr_error returns the address where the first error occurred (byte aligned)
\param exp_data returns the data that was expected
\param got_data returns the data that was received
\return 0 on success, an error code otherwise
**********************************************************/
int red_hw_sta_reg_test (int inst, const int seed, unsigned int* addr_error,unsigned  int* exp_data,unsigned  int* got_data) {
        int ret_val, i;
        unsigned int read_data_old;
        unsigned int read_data_new;
        const int TABLE_SIZE = sizeof(red_hw_sta_reg_table)/sizeof(table_entry_t);
        RED_SRAND(seed);
        for (i=0; i<TABLE_SIZE; ++i) {
                const int byte_address = red_hw_sta_reg_table[i].word_addr<<2;
                *addr_error=byte_address;
#ifdef RED_VERIFICATION
                NU_LOG << " reading default at 0x" << hex << (byte_address>>2) << dec <<endl;
#endif

                if ((ret_val=red_reg_rd(inst, byte_address, &read_data_old))!=0) {
                    return ret_val;
                }
                // write a random value
                const unsigned int write_data= RED_RAND;
#ifdef RED_VERIFICATION
                NU_LOG << " writing value 0x" << hex << write_data << dec <<endl;
#endif
                if ((ret_val=red_reg_wr(inst,byte_address, write_data))!=0) {
                    return ret_val;
                }
                // read it back
                if ((ret_val=red_reg_rd(inst, byte_address, &read_data_new))!=0) {
                    return ret_val;
                }
#ifdef RED_VERIFICATION
                NU_LOG << " read back value 0x" << hex << read_data_new << dec <<endl;
#endif


                const unsigned int MASK = red_hw_sta_reg_table[i].mask;
                // compare it to the write value
                if (read_data_old != read_data_new) {
                    *exp_data=write_data & MASK;
                    *got_data=read_data_new;
                    return REDWOOD_ERROR_DEFAULT;
                }
        }
        return 0; /*success*/
}
