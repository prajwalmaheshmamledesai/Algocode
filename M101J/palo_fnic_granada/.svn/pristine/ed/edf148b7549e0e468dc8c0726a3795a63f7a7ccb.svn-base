/*****************************************************************************
 *      File:   woo_hw_sat_reg_test.h
 *      Name:   Raghu Boreda
 *
 *      Description: ASIC level functions for Woodside 
 *      Copyright (c) 2009 by Cisco Systems, Inc.
 *****************************************************************************/
/*
 * This file cannot be included in multiple c files as it has variable 
 * definitions in it
 */
#ifndef _WOO_HW_SAT_TEST_
#define _WOO_HW_SAT_TEST_
#include "woo_woo_naxislave_define.h"
typedef struct _woo_sat_table_ { 
    unsigned int word_addr;
    unsigned int init;
    unsigned int mask;
} woo_sat_reg_table_entry_t;

const woo_sat_reg_table_entry_t woo_hi_sat_reg_table[] = 
{
    {WOO_HI0_SAT_ERR_ECC1, WOO_HI_SAT_ERR_ECC1_DEF, 0xf},
    {WOO_HI0_SAT_ERR_ECC2, WOO_HI_SAT_ERR_ECC2_DEF, 0xf}, 
    {WOO_HI0_SAT_ERR_CRC_TX, WOO_HI_SAT_ERR_CRC_TX_DEF, 0xf},
    {WOO_HI0_CT_SAT_MUX_RX_PAD, WOO_HI_CT_SAT_MUX_RX_PAD_DEF, 0xff},
    {WOO_HI0_CT_SAT_MUX_RX_DROPPED, WOO_HI_CT_SAT_MUX_RX_DROPPED_DEF, 0xff},
    {WOO_HI0_CT_SAT_MUX_RX_TRUNCATE, WOO_HI_CT_SAT_MUX_RX_TRUNCATE_DEF, 0xff},
    {WOO_HI0_CT_SAT_MUX_RX_LPBK_DROP, WOO_HI_CT_SAT_MUX_RX_LPBK_DROP_DEF, 0xff},
    {WOO_HI0_CT_SAT_MUX_TX_PAD, WOO_HI_CT_SAT_MUX_TX_PAD_DEF, 0xff}, 
    {WOO_HI0_CT_SAT_MUX_TX_TRUNCATE, WOO_HI_CT_SAT_MUX_TX_TRUNCATE_DEF, 0xff},
};

const woo_sat_reg_table_entry_t woo_ss_sat_reg_table[] = 
{
    {WOO_SS0_SAT_ERR_CNTL_ECC1, WOO_SS_SAT_ERR_CNTL_ECC1_DEF, 0xf},
    {WOO_SS0_SAT_ERR_CNTL_ECC2, WOO_SS_SAT_ERR_CNTL_ECC2_DEF, 0xf},
    {WOO_SS0_SAT_ERR_WO_CR, WOO_SS_SAT_ERR_WO_CR_DEF, 0xf},
    {WOO_SS0_SAT_ERR_RCVD_NO_CELLS, WOO_SS_SAT_ERR_RCVD_NO_CELLS_DEF, 0xf},
    {WOO_SS0_SAT_ERR_MTU_VIO, WOO_SS_SAT_ERR_MTU_VIO_DEF, 0xf},
};

const woo_sat_reg_table_entry_t woo_ni_sat_reg_table[] = 
{
    {WOO_NI0_SAT_MC_DROP, WOO_NI_SAT_MC_DROP_DEF, 0xff},
    {WOO_NI0_SAT_ERR_CRC_TX, WOO_NI_SAT_ERR_CRC_TX_DEF, 0xf},
    {WOO_NI0_CT_SAT_MUX_RX_PAD, WOO_NI_CT_SAT_MUX_RX_PAD_DEF, 0xff},
    {WOO_NI0_CT_SAT_MUX_RX_DROPPED, WOO_NI_CT_SAT_MUX_RX_DROPPED_DEF, 0xff},
    {WOO_NI0_CT_SAT_MUX_RX_TRUNCATE, WOO_NI_CT_SAT_MUX_RX_TRUNCATE_DEF, 0xff},
    {WOO_NI0_CT_SAT_MUX_RX_LPBK_DROP, WOO_NI_CT_SAT_MUX_RX_LPBK_DROP_DEF, 0xff},
    {WOO_NI0_CT_SAT_MUX_TX_PAD, WOO_NI_CT_SAT_MUX_TX_PAD_DEF, 0xff},
    {WOO_NI0_CT_SAT_MUX_TX_TRUNCATE, WOO_NI_CT_SAT_MUX_TX_TRUNCATE_DEF, 0xff},
};

const woo_sat_reg_table_entry_t woo_hf_sat_reg_table[] = 
{
    {WOO_HF0_SAT_ERR_ECC1, WOO_HF_SAT_ERR_ECC1_DEF, 0xf},
    {WOO_HF1_SAT_ERR_ECC1, WOO_HF_SAT_ERR_ECC1_DEF, 0xf},
    {WOO_HF2_SAT_ERR_ECC1, WOO_HF_SAT_ERR_ECC1_DEF, 0xf},
    {WOO_HF3_SAT_ERR_ECC1, WOO_HF_SAT_ERR_ECC1_DEF, 0xf},
    {WOO_HF0_SAT_ERR_ECC2, WOO_HF_SAT_ERR_ECC2_DEF, 0xf},
    {WOO_HF1_SAT_ERR_ECC2, WOO_HF_SAT_ERR_ECC2_DEF, 0xf},
    {WOO_HF2_SAT_ERR_ECC2, WOO_HF_SAT_ERR_ECC2_DEF, 0xf},
    {WOO_HF3_SAT_ERR_ECC2, WOO_HF_SAT_ERR_ECC2_DEF, 0xf},
    {WOO_NR_SAT_ERR_OQ0_ECC1, WOO_NR_SAT_ERR_OQ0_ECC1_DEF, 0xf},
    {WOO_NR_SAT_ERR_OQ0_ECC2, WOO_NR_SAT_ERR_OQ0_ECC2_DEF, 0xf},
    {WOO_NR_SAT_ERR_OQ1_ECC1, WOO_NR_SAT_ERR_OQ1_ECC1_DEF, 0xf},
    {WOO_NR_SAT_ERR_OQ1_ECC2, WOO_NR_SAT_ERR_OQ1_ECC2_DEF, 0xf},
    {WOO_NR_SAT_ERR_OQ2_ECC1, WOO_NR_SAT_ERR_OQ2_ECC1_DEF, 0xf},
    {WOO_NR_SAT_ERR_OQ2_ECC2, WOO_NR_SAT_ERR_OQ2_ECC2_DEF, 0xf},
    {WOO_NR_SAT_ERR_OQ3_ECC1, WOO_NR_SAT_ERR_OQ3_ECC1_DEF, 0xf},
    {WOO_NR_SAT_ERR_OQ3_ECC2, WOO_NR_SAT_ERR_OQ3_ECC2_DEF, 0xf},
    {WOO_HR_SAT_ERR_OQ0_ECC1, WOO_HR_SAT_ERR_OQ0_ECC1_DEF, 0xf},
    {WOO_HR_SAT_ERR_OQ0_ECC2, WOO_HR_SAT_ERR_OQ0_ECC2_DEF, 0xf},
    {WOO_HR_SAT_ERR_OQ1_ECC1, WOO_HR_SAT_ERR_OQ1_ECC1_DEF, 0xf},
    {WOO_HR_SAT_ERR_OQ1_ECC2, WOO_HR_SAT_ERR_OQ1_ECC2_DEF, 0xf},
    {WOO_HR_SAT_ERR_OQ2_ECC1, WOO_HR_SAT_ERR_OQ2_ECC1_DEF, 0xf},
    {WOO_HR_SAT_ERR_OQ2_ECC2, WOO_HR_SAT_ERR_OQ2_ECC2_DEF, 0xf},
    {WOO_HR_SAT_ERR_OQ3_ECC1, WOO_HR_SAT_ERR_OQ3_ECC1_DEF, 0xf},
    {WOO_HR_SAT_ERR_OQ3_ECC2, WOO_HR_SAT_ERR_OQ3_ECC2_DEF, 0xf},
};
#endif



