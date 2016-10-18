//////////////////////////////////////////////////////////////////
// $Author: pkaplan $
// $Revision: 1.1 $
//
// A test to scrub all sat registers in redwood
//////////////////////////////////////////////////////////////////
#include "woo_hw_sat_reg_test.h"
#include "woo_hw_error.h"
#include <stdio.h>
#include <stdlib.h>
#include "woo_ops.h"
#define ASIC_SRAND(seed) srand(seed)  
#define ASIC_RAND rand()

/**********************************************************
A test that scrubs all saturating registers in redwood
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
static int sat_reg_test (int inst, const int check_default,
                         unsigned int byte_address, unsigned  int init,
                         unsigned  int mask, unsigned int* exp_data,
                         unsigned int* got_data) 
{
    int ret_val;
    unsigned int read_data;
    if (check_default) {
        if ((ret_val=asic_rd(inst, byte_address, &read_data))!=0) {
            return ret_val;
        }
        if (read_data!=init) {
            printf("Dflt [0x%08X : Exp 0x%08X] : 0x%08X\n",
                   byte_address, init, read_data);
            *exp_data=init;
            *got_data=read_data;
            return WOODSIDE_ERROR_DEFAULT;
        }
    }

    // write a random value
    const unsigned int write_data= ASIC_RAND;
    if ((ret_val=asic_wr(inst,byte_address, write_data))!=0) {
        return ret_val;
    }
    // read it back
    if ((ret_val=asic_rd(inst, byte_address, &read_data))!=0) {
        return ret_val;
    }

    // compare it to the write value
    if (read_data != (write_data & mask)) {
        *exp_data=write_data & mask;
        *got_data=read_data;
        return WOODSIDE_ERROR_DEFAULT;
    }
    return 0; /*success*/
}
/*
 * woo_hi_sat_reg_test
 * Test sat registers in HI block
 */
static int woo_hi_sat_reg_test (int inst, const int check_default, 
           unsigned int* addr_error, unsigned  int* exp_data, 
           unsigned  int* got_data) 
{
    int ret_val, i, which_hi;
    unsigned int init, mask,ba;
    const int SIZE = 
          (sizeof(woo_hi_sat_reg_table)/sizeof(woo_sat_reg_table_entry_t));
    for (i=0; i<SIZE; ++i) {
        init = woo_hi_sat_reg_table[i].init;
        mask = woo_hi_sat_reg_table[i].mask;
        for (which_hi = 0; which_hi < ASIC(HI_COUNT);which_hi++) {
           ba = (woo_hi_sat_reg_table[i].word_addr + 
                 (which_hi * (ASIC(HI1_BASE_ADDR)-ASIC(HI0_BASE_ADDR)))) << 2; 
           *addr_error=ba;
           ret_val = sat_reg_test(inst,check_default,ba,init,mask,exp_data,
                                  got_data);
           if (ret_val)
               return (ret_val);
        }
    }
    return 0; /*success*/
}
/*
 * woo_ss_sat_reg_test
 * Test sat registers in HI block
 */
static int woo_ss_sat_reg_test (int inst, const int check_default, 
           unsigned int* addr_error, unsigned  int* exp_data, 
           unsigned  int* got_data) 
{
    int ret_val, i, which_ss;
    unsigned int init, mask,ba;
    const int SIZE = 
          (sizeof(woo_ss_sat_reg_table)/sizeof(woo_sat_reg_table_entry_t));
    for (i=0; i<SIZE; ++i) {
        init = woo_ss_sat_reg_table[i].init;
        mask = woo_ss_sat_reg_table[i].mask;
        for (which_ss = 0; which_ss < ASIC(SS_COUNT);which_ss++) {
           ba = (woo_ss_sat_reg_table[i].word_addr + 
                 (which_ss * (ASIC(SS1_BASE_ADDR)-ASIC(SS0_BASE_ADDR)))) << 2; 
           *addr_error=ba;
           ret_val = sat_reg_test(inst,check_default,ba,init,mask,exp_data,
                                  got_data);
           if (ret_val)
               return (ret_val);
        }
    }
    return 0; /*success*/
}
/*
 * woo_ni_sat_reg_test
 * Test sat registers in NI block
 */
static int woo_ni_sat_reg_test (int inst, const int check_default, 
           unsigned int* addr_error, unsigned  int* exp_data, 
           unsigned  int* got_data) 
{
    int ret_val, i, which_ni;
    unsigned int init, mask,ba;
    const int SIZE = 
          (sizeof(woo_ni_sat_reg_table)/sizeof(woo_sat_reg_table_entry_t));
    for (i=0; i<SIZE; ++i) {
        init = woo_ni_sat_reg_table[i].init;
        mask = woo_ni_sat_reg_table[i].mask;
        for (which_ni = 0; which_ni < ASIC(NI_COUNT);which_ni++) {
           ba = (woo_ni_sat_reg_table[i].word_addr + 
                 (which_ni * (ASIC(NI1_BASE_ADDR)-ASIC(NI0_BASE_ADDR)))) << 2; 
           *addr_error=ba;
           ret_val = sat_reg_test(inst,check_default,ba,init,mask,exp_data,
                                  got_data);
           if (ret_val)
               return (ret_val);
        }
    }
    return 0; /*success*/
}
/*
 * woo_hf_sat_reg_test
 * Test sat registers in HF/NR/HR block
 */
static int woo_hf_sat_reg_test (int inst, const int check_default, 
           unsigned int* addr_error, unsigned  int* exp_data, 
           unsigned  int* got_data) 
{
    int ret_val, i;
    unsigned int init, mask,ba;
    const int SIZE = 
          (sizeof(woo_hf_sat_reg_table)/sizeof(woo_sat_reg_table_entry_t));
    for (i=0; i<SIZE; ++i) {
        init = woo_hf_sat_reg_table[i].init;
        mask = woo_hf_sat_reg_table[i].mask;
        ba = woo_hf_sat_reg_table[i].word_addr << 2; 
        *addr_error=ba;
        ret_val = sat_reg_test(inst,check_default,ba,init,mask,exp_data,
                               got_data);
        if (ret_val)
            return (ret_val);
    }
    return 0; /*success*/
}
/*
 * Tests all sat type registers in woodside
 */
int woo_hw_sat_reg_test (int inst, const int seed, const int check_default,
                         unsigned int* addr_error,unsigned  int* exp_data,
                         unsigned  int* got_data) {
    int ret_val = 0;
    ASIC_SRAND(seed);
    ret_val = woo_hi_sat_reg_test(inst,check_default,addr_error,exp_data,
                                  got_data);
    if (ret_val)
        return (ret_val);
    printf("HI Sat reg test passed\n");
    ret_val = woo_ss_sat_reg_test(inst,check_default,addr_error,exp_data,
                                  got_data);
    if (ret_val)
        return (ret_val);
    printf("SS Sat reg test passed\n");
    ret_val = woo_ni_sat_reg_test(inst,check_default,addr_error,exp_data,
                                  got_data);
    if (ret_val)
        return (ret_val);
    printf("NI Sat reg test passed\n");
    ret_val = woo_hf_sat_reg_test(inst,check_default,addr_error,exp_data,
                                  got_data);
    printf("HF Sat reg test passed\n");
    return (ret_val); /*success*/
}
