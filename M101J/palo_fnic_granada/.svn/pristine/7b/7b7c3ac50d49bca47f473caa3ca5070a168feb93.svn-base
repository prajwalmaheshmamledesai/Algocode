#ifndef RED_HW_INT_TEST
//////////////////////////////////////////////////////////////////
// $Author: skadari $
// $Revision: 1.2 $
//////////////////////////////////////////////////////////////////
#define RED_HW_INT_TEST
#include "red_red_naxislave_define.h"
        int red_hw_int_reg_test (int inst, const int seed, unsigned int* addr_error, unsigned int* exp_data, unsigned int* got_data) ;

    typedef struct addr_table_s { 
        unsigned int word_addr; 
        unsigned int int_bit;
    } addr_table_t;
    const addr_table_t addr_table[] = 
        { {RED_MISC_INT_ERR     , 1<<0 }
// XXX disabled because we need to figure out how to turn off inputs in real world!
//      , {RED_MISC_INT_GPIO_DATA_IN, 1<<0 }
        , {RED_CI0_INT_VEC      , 1<<18}
        , {RED_CI0_INT_ERR_VEC  , 1<<18}
        , {RED_CI0_GBE_INT      , 1<<18}
        , {RED_BI0_GBE_INT      , 1<<19}
        , {RED_BI0_INTR         , 1<<19}
        , {RED_HI0_GBE_INT      , 1<<1}
        , {RED_HI0_INTR         , 1<<1}
    	, {RED_HI1_GBE_INT      , 1<<2}
    	, {RED_HI1_INTR         , 1<<2}
    	, {RED_HI2_GBE_INT      , 1<<3}
    	, {RED_HI2_INTR         , 1<<3}
    	, {RED_HI3_GBE_INT      , 1<<4}
    	, {RED_HI3_INTR         , 1<<4}
    	, {RED_HI4_GBE_INT      , 1<<5}
    	, {RED_HI4_INTR         , 1<<5}
    	, {RED_HI5_GBE_INT      , 1<<6}
    	, {RED_HI5_INTR         , 1<<6}
    	, {RED_HI6_GBE_INT      , 1<<7}
    	, {RED_HI6_INTR         , 1<<7}
    	, {RED_HI7_GBE_INT      , 1<<8}
    	, {RED_HI7_INTR         , 1<<8}
    	, {RED_NI0_INTR         , 1<<9}
    	, {RED_NI1_INTR         , 1<<10}
    	, {RED_NI2_INTR         , 1<<11}
    	, {RED_NI3_INTR         , 1<<12}
        , {RED_NF_INT_VEC       , 1<<13}
    	, {RED_SS0_INT_NORM     , 1<<14}
    	, {RED_SS0_INT_OQ0      , 1<<14}
    	, {RED_SS0_INT_OQ1      , 1<<14}
    	, {RED_SS0_INT_OQ2      , 1<<14}
    	, {RED_SS0_INT_OQ3      , 1<<14}
    	, {RED_SS0_INT_OQ4      , 1<<14}
    	, {RED_SS0_INT_ERR      , 1<<14}
    	, {RED_SS0_INT_ERR_ECC1 , 1<<14}
    	, {RED_SS0_INT_ERR_ECC2 , 1<<14}
    	, {RED_SS1_INT_NORM     , 1<<15}
    	, {RED_SS1_INT_OQ0      , 1<<15}
    	, {RED_SS1_INT_OQ1      , 1<<15}
    	, {RED_SS1_INT_OQ2      , 1<<15}
    	, {RED_SS1_INT_OQ3      , 1<<15}
    	, {RED_SS1_INT_OQ4      , 1<<15}
    	, {RED_SS1_INT_ERR      , 1<<15}
    	, {RED_SS1_INT_ERR_ECC1 , 1<<15}
    	, {RED_SS1_INT_ERR_ECC2 , 1<<15}
    	, {RED_SS2_INT_NORM     , 1<<16}
    	, {RED_SS2_INT_OQ0      , 1<<16}
    	, {RED_SS2_INT_OQ1      , 1<<16}
    	, {RED_SS2_INT_OQ2      , 1<<16}
    	, {RED_SS2_INT_OQ3      , 1<<16}
    	, {RED_SS2_INT_OQ4      , 1<<16}
    	, {RED_SS2_INT_ERR      , 1<<16}
    	, {RED_SS2_INT_ERR_ECC1 , 1<<16}
    	, {RED_SS2_INT_ERR_ECC2 , 1<<16}
    	, {RED_SS3_INT_NORM     , 1<<17}
    	, {RED_SS3_INT_OQ0      , 1<<17}
    	, {RED_SS3_INT_OQ1      , 1<<17}
    	, {RED_SS3_INT_OQ2      , 1<<17}
    	, {RED_SS3_INT_OQ3      , 1<<17}
    	, {RED_SS3_INT_OQ4      , 1<<17}
    	, {RED_SS3_INT_ERR      , 1<<17}
    	, {RED_SS3_INT_ERR_ECC1 , 1<<17}
    	, {RED_SS3_INT_ERR_ECC2 , 1<<17}
    };
#endif
