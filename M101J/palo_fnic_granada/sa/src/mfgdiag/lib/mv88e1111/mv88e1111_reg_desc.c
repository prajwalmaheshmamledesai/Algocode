/********************************************************************
 *
 *      File:   mv88e1111_reg_desc.c 
 *      Name:   Sudharshan Kadari
 *
 *      Description:
 *       Marvell Phy register description 
 *
 *
 * Copyright (c) 1985-2007 by Cisco Systems, Inc.
 * All rights reserved.
 *
 *
 *********************************************************************/

#include <stdio.h>
#include "diag_reg.h"

reg_desc_t mv88e1111_reg_desc[] =
{

REG_DEF("phy", 0, "control", "", 0xffc0, 0x00, "cfg")
REG_DEF_FLD(0, "RESET",		15, 15, "", "")  
REG_DEF_FLD(0, "LOOPBACK_EN",   14, 14, "", "")
REG_DEF_FLD(0, "SPEED_LSB",     13, 13, "", "")
REG_DEF_FLD(0, "AUTONEG_EN",	12, 12, "", "")
REG_DEF_FLD(0, "POWERDOWN",	11, 11, "", "")
REG_DEF_FLD(0, "ISOLATE",	10, 10, "", "")
REG_DEF_FLD(0, "AUTONEG_RESTART",9, 9, "", "")
REG_DEF_FLD(0, "FULL_DUPLEX",	 8, 8, "", "")
REG_DEF_FLD(0, "COLLISION_TEST", 7, 7, "", "")
REG_DEF_FLD(0, "SPEED_MSB",	 6, 6, "", "")

REG_DEF("phy", 1, "status", "", 0xff7F, 0x00, "sta")
REG_DEF_FLD(1, "100BASE_T4",		15,15, "", "")
REG_DEF_FLD(1, "100BASE_X_FULL_DUP",	14,14, "", "")
REG_DEF_FLD(1, "100BASE_X_HALF_DUP",	13,13, "", "")
REG_DEF_FLD(1, "10BASE_X_FULL_DUP",	12,12, "", "")
REG_DEF_FLD(1, "10BASE_X_HALF_DUP",	11,11, "", "")
REG_DEF_FLD(1, "100BASE_T2_FULL_DUP",	10,10, "", "")
REG_DEF_FLD(1, "100BASE_T2_HALF_DUP",	9 ,9, "", "")
REG_DEF_FLD(1, "EXT_STATUS",		8 ,8, "", "")
REG_DEF_FLD(1, "MF_PREAMBLE_SUPP",	6 ,6, "", "")
REG_DEF_FLD(1, "AUTO_NEG_COMPLETE",	5 ,5, "", "")
REG_DEF_FLD(1, "REMOTE_FAULT",		4 ,4, "", "")
REG_DEF_FLD(1, "AUTO_NEG_ABILITY",	3 ,3, "", "")
REG_DEF_FLD(1, "LINK_STATUS",		2 ,2, "", "")
REG_DEF_FLD(1, "JABBER_DETECT",		1 ,1, "", "")
REG_DEF_FLD(1, "EXT_CAPABILITY",	0 ,0, "", "")

REG_DEF("phy", 2, "phy_id_msb", "", 0xffff, 0x00, "sta")

REG_DEF("phy", 3, "phy_id_lsb", "", 0xffff, 0x00, "sta")
REG_DEF_FLD(3, "OUI Lsb",		15,10, "", "")
REG_DEF_FLD(3, "Model Number",		9,4, "", "")
REG_DEF_FLD(3, "Revision Number",	3,0, "", "")

REG_DEF("phy", 4, "auto_neg_advertisement", "", 0xffff, 0x00, "cfg")
REG_DEF_FLD(4, "NEXT_PAGE",		15,15, "", "")
REG_DEF_FLD(4, "ACK",			14,14, "", "")
REG_DEF_FLD(4, "REMOTE_FAULT",		13,13, "", "")
REG_DEF_FLD(4, "ASYMM_PAUSE",		11,11, "", "")
REG_DEF_FLD(4, "PAUSE",			10,10, "", "")
REG_DEF_FLD(4, "100BASE_T4",		9 ,9, "", "")
REG_DEF_FLD(4, "100BASE_TX_FULL_DUP",	8 ,8, "", "")
REG_DEF_FLD(4, "100BASE_TX_HALF_DUP",	7 ,7, "", "")
REG_DEF_FLD(4, "100BASE_T_FULL_DUP",	6 ,6, "", "")
REG_DEF_FLD(4, "100BASE_T_HALF_DUP",	5 ,5, "", "")
REG_DEF_FLD(4, "SECTOR_FIELD",		4, 0, "", "")

REG_DEF("phy", 5, "link_partner_ability", "", 0xffff, 0x00, "sta")
REG_DEF_FLD(5, "NEXT_PAGE",		15,15, "", "")
REG_DEF_FLD(5, "ACK",			14,14, "", "")
REG_DEF_FLD(5, "REMOTE_FAULT",		13,13, "", "")
REG_DEF_FLD(5, "TECH_ABILITY",		12,12, "", "")
REG_DEF_FLD(5, "ASYMM_PAUSE",		11,11, "", "")
REG_DEF_FLD(5, "PAUSE",			10,10, "", "")
REG_DEF_FLD(5, "100BASE_T4",		9 ,9, "", "")
REG_DEF_FLD(5, "100BASE_TX_FULL_DUP",	8 ,8, "", "")
REG_DEF_FLD(5, "100BASE_TX_HALF_DUP",	7 ,7, "", "")
REG_DEF_FLD(5, "100BASE_T_FULL_DUP",	6 ,6, "", "")
REG_DEF_FLD(5, "100BASE_T_HALF_DUP",	5 ,5, "", "")
REG_DEF_FLD(5, "SECTOR_FIELD",		4, 0, "", "")

REG_DEF("phy", 6, "autoneg expansion", "", 0xffff, 0x00, "cfg")
REG_DEF_FLD(6, "DETECT_FAULT",		4 ,4, "", "")
REG_DEF_FLD(6, "LP_NEXT_PAGE_ABLE",	3 ,3, "", "")
REG_DEF_FLD(6, "LOCAL_NEXT_PAGE_ABLE",	2 ,2, "", "")
REG_DEF_FLD(6, "PAGE_RECEIVED",		1 ,1, "", "")
REG_DEF_FLD(6, "LP_AUTO_NEG_ABLE",	0 ,0, "", "")

REG_DEF("phy", 7, "next_page_tx", "", 0xBfff, 0x00, "cfg")
REG_DEF_FLD(7, "NEXT_PAGE",		15,15, "", "")
REG_DEF_FLD(7, "MSG_PAGE_MODE",		13,13, "", "")
REG_DEF_FLD(7, "ACK2",			12,12, "", "")
REG_DEF_FLD(7, "TOGGLE",		11,11, "", "")
REG_DEF_FLD(7, "MSG_UNFORMAT_FLD",	10, 0, "", "")

REG_DEF("phy", 8, "lnk_partner_nxt_tx", "", 0xBfff, 0x00, "sta")
REG_DEF_FLD(8, "NEXT_PAGE",		15, 15, "", "")
REG_DEF_FLD(8, "MSG_PAGE_MODE",		13, 13, "", "")
REG_DEF_FLD(8, "ACK2",			12, 12, "", "")
REG_DEF_FLD(8, "TOGGLE",		11, 11, "", "")
REG_DEF_FLD(8, "MSG_UNFORMAT_FLD",	10, 0, "", "")

REG_DEF("phy", 9, "1000BaseT_Control", "", 0xffff, 0x00, "cfg")
REG_DEF_FLD(9, "TEST_MODE",		15, 13, "", "")
REG_DEF_FLD(9, "MASTER_SLAVE_EN",	12,12, "", "")
REG_DEF_FLD(9, "MASTER_CFG",		11,11, "", "")
REG_DEF_FLD(9, "PORT_TYPE",		10,10, "", "")
REG_DEF_FLD(9, "FULL_DUP",		9 ,9, "", "")
REG_DEF_FLD(9, "HALF_DUP",		8 ,8, "", "")

REG_DEF("phy", 0xA, "1000BaseT_Status", "", 0xffff, 0x00, "sta")
REG_DEF_FLD(0xA, "MS_CFG_FAULT",	15,15, "", "")
REG_DEF_FLD(0xA, "MS_CFG_RESOLUTION",	14,14, "", "")
REG_DEF_FLD(0xA, "LOCAL_RX_STATUS",	13,13, "", "")
REG_DEF_FLD(0xA, "REMOTE_RX_STATUS",	12,12, "", "")
REG_DEF_FLD(0xA, "LP_1GT_FD",		11,11, "", "")
REG_DEF_FLD(0xA, "LP_1GT_HD",		10,10, "", "")
REG_DEF_FLD(0xA, "IDLE_ERR_CNT",	7, 0, "", "")

REG_DEF("phy", 0x0F, "Ext_Status", "", 0xf000, 0x00, "sta")
REG_DEF_FLD(0x0F, "1GX_FD",		15,15, "", "")
REG_DEF_FLD(0x0F, "1GX_HD",		14,14, "", "")
REG_DEF_FLD(0x0F, "1GT_FD",		13,13, "", "")
REG_DEF_FLD(0x0F, "1GT_HD",		12,12, "", "")

REG_DEF("phy", 0x10, "Specific_control", "", 0xffff, 0x00, "cfg")
REG_DEF_FLD(0x10, "TX_FIFO_DEPTH",	15, 14, "", "")
REG_DEF_FLD(0x10, "RX_FIFO_DEPTH",	13, 12, "", "")
REG_DEF_FLD(0x10, "ASSERT_CRS_TX",	11, 11, "", "")
REG_DEF_FLD(0x10, "FORCE_LINK_GOOD",	10, 10, "", "")
REG_DEF_FLD(0x10, "ENERGY_DETECT",	9, 8, "", "")
REG_DEF_FLD(0x10, "EXT_DIST_EN",	7, 7, "", "")
REG_DEF_FLD(0x10, "MDI_CROSS_OVER",	6, 5, "", "")
REG_DEF_FLD(0x10, "125CLK_DISABLE",	4, 4, "", "")
REG_DEF_FLD(0x10, "MAC_POWER_DOWN",	3, 3, "", "")
REG_DEF_FLD(0x10, "SQE_TEST",		2, 2, "", "")
REG_DEF_FLD(0x10, "POLARITY_REVERSAL",	1, 1, "", "")
REG_DEF_FLD(0x10, "JABBER_DISABLE",	0, 0, "", "")

REG_DEF("phy", 0x11, "Specific_Status", "", 0xffff, 0x00, "sta")
REG_DEF_FLD(0x11, "SPEED",		15, 14, "", "")
REG_DEF_FLD(0x11, "DUPLEX",		13 ,13, "", "")
REG_DEF_FLD(0x11, "PAGE_RECEIVED",	12 ,12, "", "")
REG_DEF_FLD(0x11, "SPD_DUP_RESOLVED",	11 ,11, "", "")
REG_DEF_FLD(0x11, "LINK",		10 ,10, "", "")
REG_DEF_FLD(0x11, "CABLE_LENGTH",	9, 7, "", "")
REG_DEF_FLD(0x11, "MDI_CROSS_OVER",	6 ,6, "", "")
REG_DEF_FLD(0x11, "DOWN_SHIFT",		5 ,5, "", "")
REG_DEF_FLD(0x11, "ENERGY_DETECT",	4 ,4, "", "")
REG_DEF_FLD(0x11, "TX_PAUSE_ENA",	3 ,3, "", "")
REG_DEF_FLD(0x11, "RX_PAUSE_ENA",	2 ,2, "", "")
REG_DEF_FLD(0x11, "POLARITY",		1 ,1, "", "")
REG_DEF_FLD(0x11, "JABBER",		0 ,0, "", "")

REG_DEF("phy", 0x12, "irq_enable", "", 0xfff7, 0x00, "cfg")
REG_DEF_FLD(0x12, "AUTO_NEG_ERROR",		15,15, "", "")
REG_DEF_FLD(0x12, "SPEED_CHANGED",		14,14, "", "")
REG_DEF_FLD(0x12, "DUPLEX_CHANGED",		13,13, "", "")
REG_DEF_FLD(0x12, "PAGE_RECIEVED",		12,12, "", "")
REG_DEF_FLD(0x12, "AUTO_NEG_COMPLETE",		11,11, "", "")
REG_DEF_FLD(0x12, "AUTO_LINK_STATUS_CHANGED",	10,10, "", "")
REG_DEF_FLD(0x12, "SYMBOL_ERROR",		9 ,9, "", "")
REG_DEF_FLD(0x12, "FALSE_CARRIER",		8 ,8, "", "")
REG_DEF_FLD(0x12, "FIFO_OVER_UNDER_FLOW",	7 ,7, "", "")
REG_DEF_FLD(0x12, "MDI_CROSS_OVER",		6 ,6, "", "")
REG_DEF_FLD(0x12, "DOWN_SHIFT",			5 ,5, "", "")
REG_DEF_FLD(0x12, "ENERGY_DETECT",		4 ,4, "", "")
REG_DEF_FLD(0x12, "DTE_POWER_DETECT_STAT",	2 ,2, "", "")
REG_DEF_FLD(0x12, "POLARITY_CHANGED",		1 ,1, "", "")
REG_DEF_FLD(0x12, "JABBER",			0 ,0, "", "")

REG_DEF("phy", 0x13, "irq_status", "", 0xfff7, 0x00, "int")
REG_DEF_FLD(0x13, "AUTO_NEG_ERROR",	15,15, "", "")
REG_DEF_FLD(0x13, "SPEED_CHANGED",	14,14, "", "")
REG_DEF_FLD(0x13, "DUPLEX_CHANGED",	13,13, "", "")
REG_DEF_FLD(0x13, "PAGE_RECIEVED",	12,12, "", "")
REG_DEF_FLD(0x13, "AUTO_NEG_COMPLETE",	11,11, "", "")
REG_DEF_FLD(0x13, "AUTO_LINK_STATUS",	10,10, "", "")
REG_DEF_FLD(0x13, "SYMBOL_ERROR",	9 ,9, "", "")
REG_DEF_FLD(0x13, "FALSE_CARRIER",	8 ,8, "", "")
REG_DEF_FLD(0x13, "FIFO_OVER_FLOW",	7 ,7, "", "")
REG_DEF_FLD(0x13, "MDI_CROSS_OVER",	6 ,6, "", "")
REG_DEF_FLD(0x13, "DOWN_SHIFT",		5 ,5, "", "")
REG_DEF_FLD(0x13, "ENERGY_DETECT",	4 ,4, "", "")
REG_DEF_FLD(0x13, "DTE_POWER_DETECT_STAT",3 ,2, "", "")
REG_DEF_FLD(0x13, "POLARITY_CHANGED",	1 ,1, "", "")
REG_DEF_FLD(0x13, "JABBER",		0 ,0, "", "")

REG_DEF("phy", 0x14, "ext_spec_ctrl1", "", 0xffff, 0x00, "sta")
REG_DEF_FLD(0x14, "BLK_CAR_EXT",	15,15, "", "")
REG_DEF_FLD(0x14, "LINE_LPBK",		14,14, "", "")
REG_DEF_FLD(0x14, "LINK_PULSE_DISABLE",	12,12, "", "")
REG_DEF_FLD(0x14, "DOWNSHIFT_COUNTER",	11, 9, "", "")
REG_DEF_FLD(0x14, "DOWNSHIFT_ENA",	8 ,8, "", "")
REG_DEF_FLD(0x14, "RGMII_TIMING_CTRL",	7 ,7, "", "")
REG_DEF_FLD(0x14, "MAC_SPEED_DFLT",	6, 4, "", "")
REG_DEF_FLD(0x14, "DTE_DETECT_ENA",	2 ,2, "", "")
REG_DEF_FLD(0x14, "RGMII_TX_TIMING_CTRL",1 ,1, "", "")
REG_DEF_FLD(0x14, "TX_ENABLE",		0 ,0, "", "")

REG_DEF("phy", 0x15, "rx_err_counter", "", 0xffff, 0x00, "cnt")
REG_DEF("phy", 0x16, "ext_page_addr1", "", 0xffff, 0x00, "cfg")
REG_DEF("phy", 0x17, "global_status", "", 0xffff, 0x00, "sta")

REG_DEF("phy", 0x18, "led_control", "", 0xffff, 0x00, "cfg")
REG_DEF_FLD(0x18, "DISABLE",		15,15, "", "")
REG_DEF_FLD(0x18, "PULSE_STRETCH",	14, 12, "", "")
REG_DEF_FLD(0x18, "FORCE_IRQ",		11,11, "", "")
REG_DEF_FLD(0x18, "BLINK_RATE",		10, 8, "", "")
REG_DEF_FLD(0x18, "DUPLEX",		7 ,7, "", "")
REG_DEF_FLD(0x18, "TX_CONTROL",		6 ,6, "", "")
REG_DEF_FLD(0x18, "LINK",		5, 3, "", "")
REG_DEF_FLD(0x18, "DUPLEX_CONTROL",	2 ,2, "", "")
REG_DEF_FLD(0x18, "RX",			1 ,1, "", "")
REG_DEF_FLD(0x18, "TX",			0 ,0, "", "")

REG_DEF("phy", 0x19, "manual_led_override", "", 0xCfff, 0x00, "cfg")
REG_DEF_FLD(0x19, "SGMII_AUTO_NEG",	15,14, "", "")
REG_DEF_FLD(0x19, "DUPLEX",		11,10, "", "")
REG_DEF_FLD(0x19, "LINK10",		9 ,8, "", "")
REG_DEF_FLD(0x19, "LINK100",		7 ,6, "", "")
REG_DEF_FLD(0x19, "LINK1000",		5 ,4, "", "")
REG_DEF_FLD(0x19, "RX",			3 ,2, "", "")
REG_DEF_FLD(0x19, "TX",			1, 0, "", "")

REG_DEF("phy", 0x1A, "ext_phy_spec_ctrl2", "", 0xffff, 0x00, "cfg")
REG_DEF_FLD(0x1A, "FCT_PLL_LOOP_BW",	14,14, "", "")
REG_DEF_FLD(0x1A, "FCT_TX_PRE_EMP",	13,13, "", "")
REG_DEF_FLD(0x1A, "AUTO_SEL_MEDIA",	11,10, "", "")
REG_DEF_FLD(0x1A, "SERDES_PATTERN_GEN",	9,8, "", "")
REG_DEF_FLD(0x1A, "EXT_SIG_DETECT_ENA",	7 ,7, "", "")
REG_DEF_FLD(0x1A, "FIBER_INPUT_IMP",	6 ,6, "", "")
REG_DEF_FLD(0x1A, "FIBER_OUTPUT_IMP",	5 ,5, "", "")
REG_DEF_FLD(0x1A, "FIBER_MODE_CLK_ENA",	4 ,4, "", "")
REG_DEF_FLD(0x1A, "FIBER_OUTPUT_AMP",	2, 0, "", "")

REG_DEF("phy", 0x1B, "ext_phy_spec_stat", "", 0xffff, 0x00, "sta")
REG_DEF_FLD(0x1B, "AUTO_SEL_DIS",	15,15, "", "")
REG_DEF_FLD(0x1B, "RESOLUTION",		13,13, "", "")
REG_DEF_FLD(0x1B, "SI_AN_BYPASS_EN",	12,12, "", "")
REG_DEF_FLD(0x1B, "SI_AN_BYPASS_ST",	11,11, "", "")
REG_DEF_FLD(0x1B, "IRQ_POLARITY",	10,10, "", "")
REG_DEF_FLD(0x1B, "AUTO_MEDIUM_SEL",	9 ,9, "", "")
REG_DEF_FLD(0x1B, "DTE_DROP_HYST",	8, 5, "", "")
REG_DEF_FLD(0x1B, "DTE_POWER_STATUS",	4 ,4, "", "")
REG_DEF_FLD(0x1B, "HW_CFG_MODE",	3, 0, "", "")

REG_DEF("phy", 0x1C, "vc_100BT_PAIR", "", 0xffff, 0x00, "cfg")
REG_DEF("phy", 0x1D, "ext_page_addr2", "", 0xffff, 0x00, "cfg")
REG_DEF("phy", 0x1E, "misc_control", "", 0xffff, 0x00, "cfg")

{ TYP_NONE, "", 0, 0, "", "", 0, 0, ""}
};
