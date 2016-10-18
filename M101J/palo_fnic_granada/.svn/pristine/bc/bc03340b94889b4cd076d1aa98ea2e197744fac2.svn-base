//////////////////////////////////////////////////////////////////
// $Author: pkaplan $
// $Revision: 1.1 $
//
// A test to scrub multicast memif
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
#include "red_hw_lib.h"
#define RED_SRAND(seed) srand(seed)  
#define RED_RAND rand()
#endif

////////////////////////////////////////////////////////////////////////
// externally defined symbols
////////////////////////////////////////////////////////////////////////

/**********************************************************
A test that uses the memif device to scrub the multicast memory
\param seed used to initialize random generator
\param addr_error returns the address where the first error occurred (byte aligned)
\param exp_data returns the data that was expected
\param got_data returns the data that was received
\return 0 on success, an error code otherwise
**********************************************************/
int red_hw_mcst_memif_autotest (int inst, const int seed, unsigned int* addr_error, unsigned  int* exp_data, unsigned  int* got_data) {
        int ret_val;
        int tries=50000;

        const static unsigned int ERR_MASK  =(1<<RED_NF_DHS_MCST_MEMIF_STA_FLD_AUTOVRFY_ERR);
        const static unsigned int DONE_MASK =(1<<RED_NF_DHS_MCST_MEMIF_STA_FLD_AUTOTEST_DONE);

        // Force ECC bypass to test all bits
        *addr_error = RED_NF_CFG_MCST_MEMIF;
        *exp_data= 0;
        set_bit  (exp_data, RED_NF_CFG_MCST_MEMIF_FLD_ECC_BYPASS, 1);
        set_field(exp_data, RED_NF_CFG_MCST_MEMIF_FLD_END, RED_NF_DHS_MCST_MEMIF_DATA_ARRAYSIZE-1);
        if ((ret_val=red_reg_wr(inst, (*addr_error)*4, *exp_data))!=0) {
            return ret_val;
        }

        RED_SRAND(seed); //read & check phase: seed random generator again 

        // setup the data
        *addr_error = RED_NF_DHS_MCST_MEMIF_DATA;
        *exp_data = RED_RAND; 
        if ((ret_val=red_reg_wr(inst, (*addr_error)*4, *exp_data))!=0) {
                return ret_val;
        }

        // launch the autotest
        *addr_error = RED_NF_DHS_MCST_MEMIF_CMD;
        *exp_data=0; // reset
        set_bit(exp_data, RED_NF_DHS_MCST_MEMIF_CMD_FLD_AUTOTEST, 1);
        if ((ret_val=red_reg_wr(inst, (*addr_error)*4, *exp_data))!=0) {
            return ret_val;
        }

        // poll for completion or error
        *addr_error = RED_NF_DHS_MCST_MEMIF_STA;
        do { 
            if ((ret_val=red_reg_rd(inst, (*addr_error)*4, got_data))!=0) {
                return ret_val;
            }
        } while (!((*got_data) & ( ERR_MASK | DONE_MASK)) && tries--);

        if (tries<1) {
                return REDWOOD_ERROR_MEMIF_AUTOTEST_TIMEOUT;  
        }

        // check for error
        if ((*got_data) & ERR_MASK) {
                // NOTE: bug workaround: the STA read is really read+clear and
                // there's a 2 cycle window where the error bit is set and the done bit is not
                // so we need to check that we really are done when we read the error bit
                for (tries=5; tries && (!((*got_data) & DONE_MASK)); --tries) {
                    if ((ret_val=red_reg_rd(inst, (*addr_error)*4, got_data))!=0) {
                        return ret_val;
                    }
                }
                if (!tries) {
                    return REDWOOD_ERROR_MEMIF_AUTOTEST_TIMEOUT;  
                }
                return REDWOOD_ERROR_MEMIF_AUTOVRFY_ERR;
        }
        
        return 0; /*success*/
}
