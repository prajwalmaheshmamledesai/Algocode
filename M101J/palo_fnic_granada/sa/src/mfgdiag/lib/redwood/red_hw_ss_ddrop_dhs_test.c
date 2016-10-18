//////////////////////////////////////////////////////////////////
// $Author: pkaplan $
// $Revision: 1.1 $
//
// A test to scrub the DDROP DHS in SS. 
//////////////////////////////////////////////////////////////////
#include "red_hw_error.h"
#include "red_hw_globals.h"
#include "red_red_naxislave_define.h"

#ifdef RED_VERIFICATION
#include "msgFormat.h"
#include "Knob.h"
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
extern int red_reg_wr(int inst, unsigned int address, unsigned int  data);  // address is byte aligned
extern int red_reg_rd(int inst,unsigned int address, unsigned int* data); // address is byte aligned

const static unsigned int dhs_table[RED_NUM_SS] = 
        { RED_SS0_DHS_DDROP
        , RED_SS1_DHS_DDROP
        , RED_SS2_DHS_DDROP
        , RED_SS3_DHS_DDROP
};


const static unsigned int mask_table[RED_SS_DHS_DDROP_ARRAYSIZE] = 
        { 0x3fff 
        , 0x3fff
        , 0x3fff
        , 0x3fff

        , 0x3fff
        , 0x3fff
        , 0x3fff
        , 0x3fff

        , 0x3fff
        , 0x3fff
        , 0x3fff
        , 0x3fff

        , 0x3fff
        , 0x3fff
        , 0x3fff
        , 0x3fff

        , 0x3fff
        , 0x3fff
        , 0x3fff
        , 0x3fff

        , 0 //20
        , 0
        , 0
        , 0
        , 0
        , 0
        , 0
        , 0
        , 0
        , 0
        , 0
        , 0

};

/**********************************************************
A test that scrubs the DDROP DHS in SS
writes a bunch of random values to all memories behind the DHS
read back those values
\param seed used to initialize random generator
\param ss_bitmap a bit per SS  (bit 0=ss0, bit 1=ss1 ...)
\param addr_error returns the address where the first error occurred (byte aligned)
\param exp_data returns the data that was expected
\param got_data returns the data that was received
\return 0 on success, an error code otherwise
**********************************************************/
int red_hw_ss_ddrop_dhs_test (int inst, const int seed, const int ss_bitmap, unsigned int* addr_error, unsigned  int* exp_data, unsigned  int* got_data) {
        int ret_val;
        unsigned int row, column;
	unsigned int which_ss;

        {   RED_SRAND(seed); // fill-up phase, seed random generator 
            for( which_ss=0; which_ss<RED_NUM_SS; ++which_ss) {

                if ( !(ss_bitmap&(1<<which_ss))) {
                        continue;
                }

                // write a bunch of garbage into the device
                for (row=0; row<RED_SS_DHS_DDROP_ARRAYSIZE; ++row) {
                    for (column=0; column<RED_SS_DHS_DDROP_ARRAYSKIP; ++column) { 
                        *addr_error = dhs_table[which_ss]
                                    + row*RED_SS_DHS_DDROP_ARRAYSKIP
                                    + column; // words 
                        *exp_data = RED_RAND;
                        #ifdef RED_VERIFICATION
                                NU_LOG << " filling addr 0x" << hex << *addr_error << " row " << row << " column " << column << dec <<endl;
                        #endif
                        if ((ret_val=red_reg_wr(inst, (*addr_error)*4, *exp_data))!=0) {
                            return ret_val;
                        }
                    }
                }
            }
        }
        {   RED_SRAND(seed); //read & check phase: seed random generator again 
            for( which_ss=0; which_ss<RED_NUM_SS; ++which_ss) {

                if ( !(ss_bitmap&(1<<which_ss))) {
                        continue;
                }

                for (row=0; row<RED_SS_DHS_DDROP_ARRAYSIZE; ++row) {
                    for (column=0; column<RED_SS_DHS_DDROP_ARRAYSKIP; ++column) { 
                        *addr_error = dhs_table[which_ss]
                                    + row*RED_SS_DHS_DDROP_ARRAYSKIP
                                    + column;  // words
                        *exp_data = RED_RAND & mask_table[row] ;
                        #ifdef RED_VERIFICATION
                                NU_LOG << " checking addr 0x" << hex << *addr_error << " row " << row << " column " << column << dec <<endl;
                        #endif
                        if ((ret_val=red_reg_rd(inst,(*addr_error)*4, got_data))!=0) {
                            return ret_val;
                        }
                        if (*got_data!=*exp_data) { 
                            return -1;
                        }
                    }
                } 
            }
        }
        return 0; /*success*/
}
