/********************************************************************
 *
 *      File:   catalina.h
 *      Name:   Sudharshan Kadari
 *
 *      Description:
 *      catalina header file. 
 *
 *
 * Copyright (c) 1985-2009 by Cisco Systems, Inc.
 * All rights reserved.
 *
 *
 *********************************************************************/

#ifndef _CATALINA_H_
#define _CATALINA_H_

#define CAT_CFG_0			0x00
#define CAT_CFG_0_FLD_RATE_HI		7
#define CAT_CFG_0_FLD_RATE_LO		6
#define CAT_CFG_0_FLD_LATCH_DYNTIM	5
#define CAT_CFG_0_FLD_LATCH_OPEN	4
#define CAT_CFG_0_FLD_BYP_PLL		3
#define CAT_CFG_0_FLD_RESET		2
#define CAT_CFG_0_FLD_ENA_PLL		1
#define CAT_CFG_0_FLD_ENABLE		0

#define CAT_CFG_1			0x01
#define CAT_CFG_1_FLD_IBT_ON		7
#define CAT_CFG_1_FLD_CS_SWAP		6
#define CAT_CFG_1_FLD_SWAP_CLK_1	5
#define CAT_CFG_1_FLD_SWAP_CLK_0	4
#define CAT_CFG_1_FLD_IBT_1		3
#define CAT_CFG_1_FLD_IBT_0		2
#define CAT_CFG_1_FLD_EN_DIN		1
#define CAT_CFG_1_FLD_EN_CKIN		0

#define CAT_CFG_2			0x02
#define CAT_CFG_2_FLD_GM_E		7
#define CAT_CFG_2_FLD_GM_S		5
#define CAT_CFG_2_FLD_I_CP_E		4
#define CAT_CFG_2_FLD_I_CP_S		2
#define CAT_CFG_2_FLD_VCO_GAIN_E	1
#define CAT_CFG_2_FLD_VCO_GAIN_S	0

#define CAT_CFG_3	0x03
#define CAT_CFG_3_FLD_QUAD_RANK		7
#define CAT_CFG_3_FLD_DUAL_RANK		6
#define CAT_CFG_3_FLD_SINGLE_RANK	5
#define CAT_CFG_3_FLD_1GBIT		4
#define CAT_CFG_3_FLD_2GBIT		3
#define CAT_CFG_3_FLD_4GBIT		2
#define CAT_CFG_3_FLD_ODT_WRT		1
#define CAT_CFG_3_FLD_HOLD_EN		0

#define CAT_CFG_4			0x04
#define CAT_CFG_4_FLD_DPC_RL_E		7
#define CAT_CFG_4_FLD_DPC_RL_S		4
#define CAT_CFG_4_FLD_DPC_WL_E		3
#define CAT_CFG_4_FLD_DPC_WL_S		0
	
#define CAT_CFG_5			0x05
#define CAT_CFG_5_FLD_DIE_VER_E		7
#define CAT_CFG_5_FLD_DIE_VER_S		4
#define CAT_CFG_5_FLD_SPARE_05_3	3
#define CAT_CFG_5_FLD_BLOCK_CKE		2
#define CAT_CFG_5_FLD_FLIP_PARITY	1
#define CAT_CFG_5_FLD_ACT_RD_LUT	0

#define CAT_CFG_6			0x06
#define CAT_CFG_6_FLD_PWRDN_E		7
#define CAT_CFG_6_FLD_PWRDN_S		4
#define CAT_CFG_6_FLD_ENA_E		3
#define CAT_CFG_6_FLD_ENA_S		0

#define CAT_CFG_7			0x07
#define CAT_CFG_7_FLD_TEST_MODE_E	7
#define CAT_CFG_7_FLD_TEST_MODE_S	4
#define CAT_CFG_7_FLD_SPARE_07_3	3
#define CAT_CFG_7_FLD_EN_FET_S		2
#define CAT_CFG_7_FLD_EN_D23		1
#define CAT_CFG_7_FLD_EN_D01		0

#define CAT_CFG_8			0x08
#define CAT_CFG_8_FLD_SPARE_E		7
#define CAT_CFG_8_FLD_SPARE_S		6
#define CAT_CFG_8_FLD_SLEW_FET_E	5
#define CAT_CFG_8_FLD_SLEW_FET_S	3
#define CAT_CFG_8_FLD_ROUT_FET_E	2
#define CAT_CFG_8_FLD_ROUT_FET_S	1
#define CAT_CFG_8_FLD_EN_FET		0

#define CAT_CFG_9			0x09
#define CAT_CFG_9_FLD_SPARE_E		7
#define CAT_CFG_9_FLD_SPARE_S		6
#define CAT_CFG_9_FLD_SLEW_DK_E		5
#define CAT_CFG_9_FLD_SLEW_DK_S		3
#define CAT_CFG_9_FLD_ROUT_DK_E		2
#define CAT_CFG_9_FLD_ROUT_DK_S		1
#define CAT_CFG_9_FLD_SPARE_0		0

#define CAT_CFG_A			0x0A
#define CAT_CFG_A_FLD_SPARE_E		7
#define CAT_CFG_A_FLD_SPARE_S		6
#define CAT_CFG_A_FLD_SLEW_FBOUT_E	5
#define CAT_CFG_A_FLD_SLEW_FBOUT_S	3
#define CAT_CFG_A_FLD_ROUT_FBOUT_E	2
#define CAT_CFG_A_FLD_ROUT_FBOUT_S	1
#define CAT_CFG_A_FLD_EN_FBOUT		0

#define CAT_CFG_B			0x0B
#define CAT_CFG_B_FLD_SPARE_E		7
#define CAT_CFG_B_FLD_SPARE_S		5
#define CAT_CFG_B_FLD_SLEW_FET_E	4
#define CAT_CFG_B_FLD_SLEW_FET_S	2
#define CAT_CFG_B_FLD_ROUT_FET_E	1
#define CAT_CFG_B_FLD_ROUT_FET_S	0

#define CAT_CFG_C			0x0C
#define CAT_CFG_C_FLD_SLEW_D01_H_E	7
#define CAT_CFG_C_FLD_SLEW_D01_H_S	5
#define CAT_CFG_C_FLD_SLEW_D01_L_E	4
#define CAT_CFG_C_FLD_SLEW_D01_L_S	2
#define CAT_CFG_C_FLD_ROUT_D01_E	1
#define CAT_CFG_C_FLD_ROUT_D01_S	0

#define CAT_CFG_D			0x0D
#define CAT_CFG_D_FLD_SLEW_D0_H_E	7
#define CAT_CFG_D_FLD_SLEW_D0_H_S	5
#define CAT_CFG_D_FLD_SLEW_D0_L_E	4
#define CAT_CFG_D_FLD_SLEW_D0_L_S	2
#define CAT_CFG_D_FLD_ROUT_D0_E		1
#define CAT_CFG_D_FLD_ROUT_D0_S		0

#define CAT_CFG_E			0x0E
#define CAT_CFG_E_FLD_SLEW_D1_H_E	7
#define CAT_CFG_E_FLD_SLEW_D1_H_S	5
#define CAT_CFG_E_FLD_SLEW_D1_L_E	4
#define CAT_CFG_E_FLD_SLEW_D1_L_S	2
#define CAT_CFG_E_FLD_ROUT_D1_E		1
#define CAT_CFG_E_FLD_ROUT_D1_S		0

#define CAT_CFG_F			0x0F
#define CAT_CFG_F_FLD_SLEW_D23_H_E	7
#define CAT_CFG_F_FLD_SLEW_D23_H_S	5
#define CAT_CFG_F_FLD_SLEW_D23_L_E	4
#define CAT_CFG_F_FLD_SLEW_D23_L_S	2
#define CAT_CFG_F_FLD_ROUT_D23_E	1
#define CAT_CFG_F_FLD_ROUT_D23_S	0

#define CAT_CFG_10			0x10
#define CAT_CFG_10_FLD_SLEW_D2_H_E	7
#define CAT_CFG_10_FLD_SLEW_D2_H_S	5
#define CAT_CFG_10_FLD_SLEW_D2_L_E	4
#define CAT_CFG_10_FLD_SLEW_D2_L_S	2
#define CAT_CFG_10_FLD_ROUT_D2_E	1
#define CAT_CFG_10_FLD_ROUT_D2_S	0

#define CAT_CFG_11			0x11
#define CAT_CFG_11_FLD_SLEW_D3_H_E	7
#define CAT_CFG_11_FLD_SLEW_D3_H_S	5
#define CAT_CFG_11_FLD_SLEW_D3_L_E	4
#define CAT_CFG_11_FLD_SLEW_D3_L_S	2
#define CAT_CFG_11_FLD_ROUT_D3_E	1
#define CAT_CFG_11_FLD_ROUT_D3_S	0

#define CAT_CFG_FET	0x12
#define CAT_CFG_D0	0x13
#define CAT_CFG_D1	0x14
#define CAT_CFG_D2	0x15
#define CAT_CFG_D3	0x16
#define CAT_CFG_FBOUT	0x17
#define CAT_CFG_DD 	0x18
#define CAT_CFG_S 	0x19
#define CAT_SPARE0 	0x1A
#define CAT_SPARE1 	0x1B
#define CAT_SPARE2 	0x1C
#define CAT_SPARE3 	0x1D
#define CAT_SPARE4 	0x1E
#define CAT_SPARE5 	0x1F
#define CAT_FUSE0 	0x20
#define CAT_FUSE1 	0x21
#define CAT_FUSE2 	0x22
#define CAT_FUSE3 	0x23
#define CAT_FUSE4 	0x24
#define CAT_FUSE5 	0x25
#define CAT_FUSE6 	0x26
#define CAT_FUSE7 	0x27
#define CAT_FUSE_ID0 	0x28
#define CAT_FUSE_ID1 	0x29
#define CAT_FUSE_ID2 	0x2A
#define CAT_FUSE_ID3 	0x2B
#define CAT_FUSE_ID4 	0x2C
#define CAT_FUSE_ID5 	0x2D
#define CAT_FUSE_ID6 	0x2E
#define CAT_FUSE_ID7 	0x2F
#endif // _CATALINA_H_
