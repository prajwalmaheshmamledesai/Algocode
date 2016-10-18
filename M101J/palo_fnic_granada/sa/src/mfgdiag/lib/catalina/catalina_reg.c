/********************************************************************
 *
 *      File:   catalina_reg.c
 *      Name:   Sudharshan Kadari
 *
 *      Description:
 *      catalina register file. 
 *
 *
 * Copyright (c) 1985-2009 by Cisco Systems, Inc.
 * All rights reserved.
 *
 *
 *********************************************************************/
#include <stdio.h>
#include "diag_reg.h"
#include "catalina.h"

reg_desc_t catalina_reg_desc[] =
{
RCFG(CAT_CFG_0)
BFLD(CAT_CFG_0, RATE_HI)
BFLD(CAT_CFG_0, RATE_LO)
BFLD(CAT_CFG_0, LATCH_DYNTIM)
BFLD(CAT_CFG_0, LATCH_OPEN)
BFLD(CAT_CFG_0, BYP_PLL)
BFLD(CAT_CFG_0, RESET)
BFLD(CAT_CFG_0, ENA_PLL)
BFLD(CAT_CFG_0, ENABLE)

RCFG(CAT_CFG_1)
BFLD(CAT_CFG_1, IBT_ON)
BFLD(CAT_CFG_1, CS_SWAP)
BFLD(CAT_CFG_1, SWAP_CLK_1)
BFLD(CAT_CFG_1, SWAP_CLK_0)
BFLD(CAT_CFG_1, IBT_1)
BFLD(CAT_CFG_1, IBT_0)
BFLD(CAT_CFG_1, EN_DIN)
BFLD(CAT_CFG_1, EN_CKIN)

RCFG(CAT_CFG_2)
RFLD(CAT_CFG_2, GM)
RFLD(CAT_CFG_2, I_CP)
RFLD(CAT_CFG_2, VCO_GAIN)

RCFG(CAT_CFG_3)
BFLD(CAT_CFG_3, QUAD_RANK)
BFLD(CAT_CFG_3, DUAL_RANK)
BFLD(CAT_CFG_3, SINGLE_RANK)
BFLD(CAT_CFG_3, 1GBIT)
BFLD(CAT_CFG_3, 2GBIT)
BFLD(CAT_CFG_3, 4GBIT)
BFLD(CAT_CFG_3, ODT_WRT)
BFLD(CAT_CFG_3, HOLD_EN)

RCFG(CAT_CFG_4)
RFLD(CAT_CFG_4, DPC_RL)
RFLD(CAT_CFG_4, DPC_WL)
	
RCFG(CAT_CFG_5)
RFLD(CAT_CFG_5, DIE_VER)
BFLD(CAT_CFG_5, SPARE_05_3)
BFLD(CAT_CFG_5, BLOCK_CKE)
BFLD(CAT_CFG_5, FLIP_PARITY)
BFLD(CAT_CFG_5, ACT_RD_LUT)

RCFG(CAT_CFG_6)
RFLD(CAT_CFG_6, PWRDN)
RFLD(CAT_CFG_6, ENA)

RCFG(CAT_CFG_7)
RFLD(CAT_CFG_7, TEST_MODE)
BFLD(CAT_CFG_7, SPARE_07_3)
BFLD(CAT_CFG_7, EN_FET_S)
BFLD(CAT_CFG_7, EN_D23)
BFLD(CAT_CFG_7, EN_D01)

RCFG(CAT_CFG_8)
RFLD(CAT_CFG_8, SPARE)
RFLD(CAT_CFG_8, SLEW_FET)
RFLD(CAT_CFG_8, ROUT_FET)
BFLD(CAT_CFG_8, EN_FET)

RCFG(CAT_CFG_9)
RFLD(CAT_CFG_9, SPARE)
RFLD(CAT_CFG_9, SLEW_DK)
RFLD(CAT_CFG_9, ROUT_DK)
BFLD(CAT_CFG_9, SPARE_0)

RCFG(CAT_CFG_A)
RFLD(CAT_CFG_A, SPARE)
RFLD(CAT_CFG_A, SLEW_FBOUT)
RFLD(CAT_CFG_A, ROUT_FBOUT)
BFLD(CAT_CFG_A, EN_FBOUT)

RCFG(CAT_CFG_B)
RFLD(CAT_CFG_B, SPARE)
RFLD(CAT_CFG_B, SLEW_FET)
RFLD(CAT_CFG_B, ROUT_FET)

RCFG(CAT_CFG_C)
RFLD(CAT_CFG_C, SLEW_D01_H)
RFLD(CAT_CFG_C, SLEW_D01_L)
RFLD(CAT_CFG_C, ROUT_D01)

RCFG(CAT_CFG_D)
RFLD(CAT_CFG_D, SLEW_D0_H)
RFLD(CAT_CFG_D, SLEW_D0_L)
RFLD(CAT_CFG_D, ROUT_D0)

RCFG(CAT_CFG_E)
RFLD(CAT_CFG_E, SLEW_D1_H)
RFLD(CAT_CFG_E, SLEW_D1_L)
RFLD(CAT_CFG_E, ROUT_D1)

RCFG(CAT_CFG_F)
RFLD(CAT_CFG_F, SLEW_D23_H)
RFLD(CAT_CFG_F, SLEW_D23_L)
RFLD(CAT_CFG_F, ROUT_D23)

RCFG(CAT_CFG_10)
RFLD(CAT_CFG_10, SLEW_D2_H)
RFLD(CAT_CFG_10, SLEW_D2_L)
RFLD(CAT_CFG_10, ROUT_D2)

RCFG(CAT_CFG_11)
RFLD(CAT_CFG_11, SLEW_D3_H)
RFLD(CAT_CFG_11, SLEW_D3_L)
RFLD(CAT_CFG_11, ROUT_D3)

RCFG(CAT_CFG_FET)
RCFG(CAT_CFG_D0)
RCFG(CAT_CFG_D1)
RCFG(CAT_CFG_D2)
RCFG(CAT_CFG_D3)
RCFG(CAT_CFG_FBOUT)
RCFG(CAT_CFG_DD)
RCFG(CAT_CFG_S)
RCFG(CAT_SPARE0)
RCFG(CAT_SPARE1)
RCFG(CAT_SPARE2)
RCFG(CAT_SPARE3)
RCFG(CAT_SPARE4)
RCFG(CAT_SPARE5)
RCFG(CAT_FUSE0)
RCFG(CAT_FUSE1)
RCFG(CAT_FUSE2)
RCFG(CAT_FUSE3)
RCFG(CAT_FUSE4)
RCFG(CAT_FUSE5)
RCFG(CAT_FUSE6)
RCFG(CAT_FUSE7)
RCFG(CAT_FUSE_ID0)
RCFG(CAT_FUSE_ID1)
RCFG(CAT_FUSE_ID2)
RCFG(CAT_FUSE_ID3)
RCFG(CAT_FUSE_ID4)
RCFG(CAT_FUSE_ID5)
RCFG(CAT_FUSE_ID6)
RCFG(CAT_FUSE_ID7)

{ TYP_NONE, "", 0, 0, "", "", 0, 0, ""}
};
