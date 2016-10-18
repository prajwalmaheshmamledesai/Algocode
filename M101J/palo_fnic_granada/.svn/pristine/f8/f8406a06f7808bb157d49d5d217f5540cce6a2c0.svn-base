//////////////////////////////////////////////////////////////////
// $Author: pkaplan $
// $Revision: 1.1 $
//
// A test to drive the mbist state machine provided by avago
//////////////////////////////////////////////////////////////////
#include "red_hw_error.h"
#include "red_hw_globals.h"
#include "red_red_naxislave_define.h"

#ifndef RED_VERIFICATION
#include "red_hw_lib.h"
#else
#include "stdio.h"
#endif

////////////////////////////////////////////////////////////////////////
// externally defined symbols
////////////////////////////////////////////////////////////////////////

/**********************************************************
A test launches the avago provided MBIST state machine and checks the status 
\param addr_error returns the address where the first error occurred (word aligned)
\param exp_data returns the data that was expected
\param got_data returns the data that was received
\return 0 on success, an error code otherwise
**********************************************************/
int red_hw_mbist_test (int inst, unsigned int* addr_error, unsigned  int* exp_data,unsigned  int* got_data) {

	int ret_val;

        // bring out of BIST reset.
        // Cpu::writeField(RED_MISC_CFG_AVAGO, u32(1), 1);
	printf("  BIST bring out of reset\n");
        *addr_error = RED_MISC_CFG_AVAGO;
        *exp_data   = RED_MISC_CFG_AVAGO_DEF;
        set_bit (exp_data, RED_MISC_CFG_AVAGO_FLD_MEM_BIST_RST_L, 1);

        unsigned int old_cfg_avago = *exp_data;
        
        if ((ret_val=red_reg_wr(inst, (*addr_error)*4, *exp_data))!=0) {
            return ret_val;
        }
        
        // default is 0 . use the clock from TAP. Our logic doesn't wiggle Tck
        // Cpu::writeField(RED_MISC_CFG_JPC, u32(0), 9);
        *addr_error = RED_MISC_CFG_JPC;
        *exp_data   = RED_MISC_CFG_JPC_DEF;
        set_bit (exp_data, RED_MISC_CFG_JPC_FLD_SELECT_CPU_TCK, 0);
        
        if ((ret_val=red_reg_wr(inst, (*addr_error)*4, *exp_data))!=0) {
            return ret_val;
        }

        // tbvWait(40,TBV_NS);
        RED_SLEEP(40);

        // BIST reset
        // Cpu::writeField(RED_MISC_CFG_AVAGO, u32(0), 1); // mbist rst is asserted
        *addr_error = RED_MISC_CFG_AVAGO;
        *exp_data   = old_cfg_avago;
        set_bit (exp_data, RED_MISC_CFG_AVAGO_FLD_MEM_BIST_RST_L, 0);
        old_cfg_avago = *exp_data;
        
        if ((ret_val=red_reg_wr(inst, (*addr_error)*4, *exp_data))!=0) {
            return ret_val;
        }

        //tbvWait(100,TBV_NS);
        RED_SLEEP(100);

        // remove bist reset
        // Cpu::writeField(RED_MISC_CFG_AVAGO, u32(1), 1); // deassert mbist reset
        *addr_error = RED_MISC_CFG_AVAGO;
        *exp_data   = old_cfg_avago;
        set_bit (exp_data, RED_MISC_CFG_AVAGO_FLD_MEM_BIST_RST_L, 1);
        
        if ((ret_val=red_reg_wr(inst, (*addr_error)*4, *exp_data))!=0) {
            return ret_val;
        }

        // tbvWait(500,TBV_NS);
        RED_SLEEP(500);

	printf("  BIST launch\n");
        // BIST launch
        // Cpu::writeField(RED_MISC_CFG_AVAGO, u32(1), 0);

        *addr_error = RED_MISC_CFG_AVAGO;
        *exp_data   = old_cfg_avago;
        set_bit (exp_data, RED_MISC_CFG_AVAGO_FLD_MEM_BIST_LAUNCH, 1);
        set_bit (exp_data, RED_MISC_CFG_AVAGO_FLD_MEM_BIST_RST_L, 1);
        old_cfg_avago = *exp_data;
        
        if ((ret_val=red_reg_wr(inst, (*addr_error)*4, *exp_data))!=0) {
            return ret_val;
        }

        //tbvWait(1700000,TBV_NS);
	printf("  BIST sleep 1700\n");
        RED_SLEEP(1700); // microsecs

        int done = 0;
        int fail = 0;
        int tries= 2000;
        do {
               // BitVector bv32Tmp (Cpu::readHW(RED_MISC_STA_AVAGO), 32);
               *addr_error = RED_MISC_STA_AVAGO;

               if ((ret_val=red_reg_rd(inst, (*addr_error)*4, got_data))!=0) {
                    return ret_val;
               }

               done = ((*got_data) >> RED_MISC_STA_AVAGO_FLD_MEM_BIST_READY)&1;
               fail = ((*got_data) >> RED_MISC_STA_AVAGO_FLD_MEM_BIST_GLOBAL_FAIL)&1;

	       if ((tries & 0x1FF) == 0)
			printf("  BIST tries = %d\n", tries);
        } while ( !done && !fail && tries--);

        if (tries<1) { // too many tries
                return REDWOOD_ERROR_MBIST_TIMEOUT; 
        }

        if (fail) {
		*exp_data =  (0x01 << RED_MISC_STA_AVAGO_FLD_MEM_BIST_READY);
                return REDWOOD_ERROR_MBIST_FAILURE; 
        }

        return 0; /*success*/
}
