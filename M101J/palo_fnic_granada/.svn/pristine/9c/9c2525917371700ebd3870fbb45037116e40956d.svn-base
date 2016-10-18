//////////////////////////////////////////////////////////////////
// $Author: pkaplan $
// $Revision: 1.1 $
//
// A test to scrub the RDARB DHS 
// Some entries in the DHS are read-only, so we assume we have values
// out of reset (zeroes)
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
#include <stdio.h>
#include <stdlib.h>
#include "red_hw_lib.h" 
#define RED_SRAND(seed) srand(seed)  
#define RED_RAND rand()
#endif

////////////////////////////////////////////////////////////////////////
// externally defined symbols
////////////////////////////////////////////////////////////////////////

const static unsigned int dhs_table[RED_NUM_RDARB] = 
        { RED_SS0_DHS_SS_RDARB
        , RED_SS1_DHS_SS_RDARB
        , RED_SS2_DHS_SS_RDARB
        , RED_SS3_DHS_SS_RDARB
};

// this table comes from looking into the RTL. No documentation available.
#define MASK_ENTRY(width) (1<<width)-1
const static unsigned int MASK[32] =
        { MASK_ENTRY(10) //0
        , MASK_ENTRY(16) //1
        , MASK_ENTRY(16) //2

        , MASK_ENTRY(0) //3
        , MASK_ENTRY(0) //4
        , MASK_ENTRY(0) //5
        , MASK_ENTRY(0) //6
        , MASK_ENTRY(0) //7

        , MASK_ENTRY(13) //8
        , MASK_ENTRY(13) //9
        , MASK_ENTRY(13) //10
        , MASK_ENTRY(13) //11

        , MASK_ENTRY(0) //12
        , MASK_ENTRY(0) //13
        , MASK_ENTRY(0) //14
        , MASK_ENTRY(0) //15

        , MASK_ENTRY(0) //16 // these are read-only
        , MASK_ENTRY(0) //17
        , MASK_ENTRY(0) //18
        , MASK_ENTRY(0) //19

        , MASK_ENTRY(0) //20
        , MASK_ENTRY(0) //21
        , MASK_ENTRY(0) //22
        , MASK_ENTRY(0) //23
        , MASK_ENTRY(0) //24
        , MASK_ENTRY(0) //25
        , MASK_ENTRY(0) //26
        , MASK_ENTRY(0) //27
        , MASK_ENTRY(0) //28
        , MASK_ENTRY(0) //29
        , MASK_ENTRY(0) //30
        , MASK_ENTRY(0) //31
};

/**********************************************************
A test that scrubs the rdarb DHS 
writes a bunch of random values to all memories behind the DHS
read back those values
\param seed used to initialize random generator
\param rdarb_bitmap a bit per RDARB (bit 0=rdarb0, bit 1=rdarb1 ...)
\param addr_error returns the address where the first error occurred (byte aligned)
\param exp_data returns the data that was expected
\param got_data returns the data that was received
\return 0 on success, an error code otherwise
**********************************************************/
int red_hw_rdarb_dhs_test (int inst, const int seed, const int rdarb_bitmap, unsigned int* addr_error, unsigned  int* exp_data, unsigned  int* got_data) {
        int ret_val;
        unsigned int row, column;
	unsigned int which_rdarb;
        const unsigned int MASK_TABLE_SIZE=sizeof(MASK)/sizeof(unsigned int);

        {   RED_SRAND(seed); // fill-up phase, seed random generator 
            for( which_rdarb=0; which_rdarb<RED_NUM_RDARB; ++which_rdarb) {

                if ( !(rdarb_bitmap&(1<<which_rdarb))) {
                        continue;
                }

                // write a bunch of garbage into the device
                for (row=0; row<MASK_TABLE_SIZE; ++row) {
                    for (column=0; column<RED_SS_DHS_SS_RDARB_ARRAYSKIP; ++column) { 
                        *addr_error = dhs_table[which_rdarb]
                                    + row*RED_SS_DHS_SS_RDARB_ARRAYSKIP
                                    + column; // words 
                        *exp_data = RED_RAND;
                        #ifdef RED_VERIFICATION
                                NU_LOG << " filling addr " << *addr_error << " row " << row << " column " << column << endl;
                        #endif
                        if ((ret_val=red_reg_wr(inst, (*addr_error)*4, *exp_data))!=0) {
				printf("[0x%08X] : r%d c%d 0x%X\n", *addr_error << 2, row, 
						column, *exp_data);
                            return ret_val;
                        }
                    } 
                }
            }
        }
	RED_SLEEP(2000);
        {   RED_SRAND(seed); //read & check phase: seed random generator again 
            for( which_rdarb=0; which_rdarb<RED_NUM_RDARB; ++which_rdarb) {

                if ( !(rdarb_bitmap&(1<<which_rdarb))) {
                        continue;
                }

                for (row=0; row<MASK_TABLE_SIZE; ++row) {
                    for (column=0; column<RED_SS_DHS_SS_RDARB_ARRAYSKIP; ++column) { 
                        *addr_error = dhs_table[which_rdarb]
                                    + row*RED_SS_DHS_SS_RDARB_ARRAYSKIP
                                    + column; // words 
                        *exp_data = RED_RAND & MASK[row];
                        #ifdef RED_VERIFICATION
                                NU_LOG << " checking addr " << *addr_error << " row " << row << " column " << column << endl;
                        #endif
			RED_SLEEP(10000);
                        if ((ret_val=red_reg_rd(inst, (*addr_error)*4, got_data))!=0) {
                            return ret_val;
                        }
                        if (*got_data!=*exp_data) { 
                              printf("Err: [0x%08X] : r%d c%d 0x%08X 0x%08X\n", *addr_error << 2, row, 
						column, *exp_data, *got_data);
//                            return -1;
                        }
                    }
                } 
            }
        }
        return 0; /*success*/
}
