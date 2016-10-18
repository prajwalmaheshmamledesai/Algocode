//////////////////////////////////////////////////////////////////
// $Author: pkaplan $
// $Revision: 1.4 $
//
// A test to scrub multicast DHS 
//////////////////////////////////////////////////////////////////
#include "red_hw_mcst_dhs_test.h"
#include "red_hw_error.h"
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
extern int red_reg_rd(int inst, unsigned int address, unsigned int* data); // address is byte aligned

/**********************************************************
A test that scrubs the multicast DHS
writes a bunch of random values to the memory behind the DHS
read back those values
\param seed used to initialize random generator
\param addr_error returns the address where the first error occurred (byte aligned)
\param exp_data returns the data that was expected
\param got_data returns the data that was received
\return 0 on success, an error code otherwise
**********************************************************/
int red_hw_mcst_dhs_test (int inst, const int seed, unsigned int* addr_error, unsigned  int* exp_data, unsigned  int* got_data) {
        int ret_val;

        unsigned int row, column;

        const unsigned int MASK = (1<<RED_NF_DHS_MCST_MEMIF_DATA_SZ)-1;

        // NOTE: MCST's MEMIF needs to be set in DIRACC and DIRADDR mode before being able to do it
        *addr_error = RED_NF_DHS_MCST_MEMIF_CMD;
        *exp_data= (1<<RED_NF_DHS_MCST_MEMIF_CMD_FLD_DIRACC)
                 | (1<<RED_NF_DHS_MCST_MEMIF_CMD_FLD_DIRADDR)
                 ;
        if ((ret_val=red_reg_wr(inst, (*addr_error)*4, *exp_data))!=0) {
            return ret_val;
        }

        // Force ECC bypass to test all bits
        *addr_error = RED_NF_CFG_MCST_MEMIF;
        *exp_data= (1<<RED_NF_CFG_MCST_MEMIF_FLD_ECC_BYPASS);
        if ((ret_val=red_reg_wr(inst, (*addr_error)*4, *exp_data))!=0) {
            return ret_val;
        }
        

        {   RED_SRAND(seed); //seed random generator again 

            // write a bunch of garbage into the device
            for (row=0; row<RED_NF_DHS_MCST_MEMIF_DATA_ARRAYSIZE; ++row) {
                #ifdef RED_VERIFICATION
                        NU_LOG << " writing row " << row << endl;
                #endif
                for (column=0; column<RED_NF_DHS_MCST_MEMIF_DATA_ARRAYSKIP; ++column) { 

                    *addr_error = RED_NF_DHS_MCST_MEMIF_DATA
                                + row * RED_NF_DHS_MCST_MEMIF_DATA_ARRAYSKIP
                                + column; // words 

                    *exp_data = RED_RAND;

                    if ((ret_val=red_reg_wr(inst, *addr_error*4, *exp_data))!=0) {
                        return ret_val;
                    }
                }
            }
         }
         {  RED_SRAND(seed); //seed random generator again 
            for (row=0; row<RED_NF_DHS_MCST_MEMIF_DATA_ARRAYSIZE; ++row) {
                #ifdef RED_VERIFICATION
                        NU_LOG << " checking row " << row << endl;
                #endif
                for (column=0; column<RED_NF_DHS_MCST_MEMIF_DATA_ARRAYSKIP; ++column) { 

                    *addr_error = RED_NF_DHS_MCST_MEMIF_DATA
                                + row * RED_NF_DHS_MCST_MEMIF_DATA_ARRAYSKIP
                                + column; // words 

                    *exp_data = RED_RAND & MASK;

                    if ((ret_val=red_reg_rd(inst, (*addr_error)*4, got_data))!=0) {
                        return ret_val;
                    }

                    if ((*got_data) != (*exp_data)) { 
                        return REDWOOD_ERROR_DEFAULT;
                    }
                }
            } 
        }
        return 0; /*success*/
}
