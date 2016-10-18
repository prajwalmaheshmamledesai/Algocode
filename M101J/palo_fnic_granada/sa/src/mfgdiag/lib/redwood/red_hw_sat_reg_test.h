#ifndef RED_HW_SAT_TEST
//////////////////////////////////////////////////////////////////
// $Author: pkaplan $
// $Revision: 1.1 $
//////////////////////////////////////////////////////////////////
#define RED_HW_SAT_TEST
#include "red_red_naxislave_define.h"
        int red_hw_sat_reg_test (int inst, const int seed, const int check_default, unsigned int* addr_error, unsigned int* exp_data, unsigned int* got_data) ;

        typedef struct { 
                unsigned int word_addr;
                unsigned int init;
                unsigned int mask;
        } table_entry_t;

        const table_entry_t red_hw_sat_reg_table[] = 
                {{ RED_SS0_SAT_ERR_MTU_VIO , RED_SS_SAT_ERR_MTU_VIO_DEF,0x0000000f }
		,{ RED_SS1_SAT_ERR_MTU_VIO , RED_SS_SAT_ERR_MTU_VIO_DEF,0x0000000f }
		,{ RED_SS2_SAT_ERR_MTU_VIO , RED_SS_SAT_ERR_MTU_VIO_DEF,0x0000000f }
		,{ RED_SS3_SAT_ERR_MTU_VIO , RED_SS_SAT_ERR_MTU_VIO_DEF,0x0000000f }
		,{ RED_SS0_SAT_ERR_CNTL_ECC2 , RED_SS_SAT_ERR_CNTL_ECC2_DEF,0x0000000f }
		,{ RED_SS1_SAT_ERR_CNTL_ECC2 , RED_SS_SAT_ERR_CNTL_ECC2_DEF,0x0000000f }
		,{ RED_SS2_SAT_ERR_CNTL_ECC2 , RED_SS_SAT_ERR_CNTL_ECC2_DEF,0x0000000f }
		,{ RED_SS3_SAT_ERR_CNTL_ECC2 , RED_SS_SAT_ERR_CNTL_ECC2_DEF,0x0000000f }
		,{ RED_SS0_SAT_ERR_WO_CR , RED_SS_SAT_ERR_WO_CR_DEF,0x0000000f }
		,{ RED_SS1_SAT_ERR_WO_CR , RED_SS_SAT_ERR_WO_CR_DEF,0x0000000f }
		,{ RED_SS2_SAT_ERR_WO_CR , RED_SS_SAT_ERR_WO_CR_DEF,0x0000000f }
		,{ RED_SS3_SAT_ERR_WO_CR , RED_SS_SAT_ERR_WO_CR_DEF,0x0000000f }
		,{ RED_SS0_SAT_ERR_CNTL_ECC1 , RED_SS_SAT_ERR_CNTL_ECC1_DEF,0x0000000f }
		,{ RED_SS1_SAT_ERR_CNTL_ECC1 , RED_SS_SAT_ERR_CNTL_ECC1_DEF,0x0000000f }
		,{ RED_SS2_SAT_ERR_CNTL_ECC1 , RED_SS_SAT_ERR_CNTL_ECC1_DEF,0x0000000f }
		,{ RED_SS3_SAT_ERR_CNTL_ECC1 , RED_SS_SAT_ERR_CNTL_ECC1_DEF,0x0000000f }
		,{ RED_SS0_SAT_ERR_RCVD_NO_CELLS , RED_SS_SAT_ERR_RCVD_NO_CELLS_DEF,0x0000000f }
		,{ RED_SS1_SAT_ERR_RCVD_NO_CELLS , RED_SS_SAT_ERR_RCVD_NO_CELLS_DEF,0x0000000f }
		,{ RED_SS2_SAT_ERR_RCVD_NO_CELLS , RED_SS_SAT_ERR_RCVD_NO_CELLS_DEF,0x0000000f }
		,{ RED_SS3_SAT_ERR_RCVD_NO_CELLS , RED_SS_SAT_ERR_RCVD_NO_CELLS_DEF,0x0000000f }
                };
#endif
