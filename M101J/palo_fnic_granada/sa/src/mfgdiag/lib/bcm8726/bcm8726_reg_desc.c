/********************************************************************
 *
 *      File:   bcm8726_reg_desc.c 
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
#include <string.h>
#include <stdint.h>
#include <sys/types.h>
#include "diag_reg.h"
#include "bcm8726.h"

reg_desc_t bcm8726_reg_desc[] =
{
  REG_DEF("pmd", BCM8726_PMD_CTRL , "BCM8726_PMD_CTRL", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF_FLD(BCM8726_PMD_CTRL,     "RESET",         15, 15, "", "")
  REG_DEF_FLD(BCM8726_PMD_CTRL,     "SPEED_SELECT1", 13, 13, "", "")
  REG_DEF_FLD(BCM8726_PMD_CTRL,     "LOW_POWER",     11, 11, "", "")
  REG_DEF_FLD(BCM8726_PMD_CTRL,     "SPEED_SELECT0",  6, 6, "", "")
  REG_DEF_FLD(BCM8726_PMD_CTRL,     "SPEED_10GBS",    5, 2, "", "")
  REG_DEF_FLD(BCM8726_PMD_CTRL,     "PMD_LPBK",       0, 0, "", "")
  
  REG_DEF("pmd", BCM8726_PMD_STATUS,"BCM8726_PMD_STATUS", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF_FLD(BCM8726_PMD_STATUS,    "LOCAL_FAULT",      7, 7, "", "")
  REG_DEF_FLD(BCM8726_PMD_STATUS,    "RX_LINK_STATUS",   2, 2, "", "")
  REG_DEF_FLD(BCM8726_PMD_STATUS,    "LOW_POWER_ABILITY",1, 1, "", "")
  
  REG_DEF("pmd", BCM8726_PMD_DEV_ID0,       "BCM8726_PMD_DEV_ID0", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF("pmd", BCM8726_PMD_DEV_ID1,       "BCM8726_PMD_DEV_ID1", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF("pmd", BCM8726_PMD_SPEED_ABILITY, "BCM8726_PMD_SPEED_ABILITY", "", 0xFFFF, 0x00, "cfg") 
  
  REG_DEF("pmd", BCM8726_PMD_PKG1_DEVICES, "BCM8726_PMD_PKG1_DEVICES", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF_FLD(BCM8726_PMD_PKG1_DEVICES, "DTE_XS_PRESENT",   5, 5, "", "")
  REG_DEF_FLD(BCM8726_PMD_PKG1_DEVICES, "PHY_XS_PRESENT",   4, 4, "", "")
  REG_DEF_FLD(BCM8726_PMD_PKG1_DEVICES, "PCS_PRESENT",	    3, 3, "", "")
  REG_DEF_FLD(BCM8726_PMD_PKG1_DEVICES, "WIS_PRESENT",	    2, 2, "", "")
  REG_DEF_FLD(BCM8726_PMD_PKG1_DEVICES, "PMD_PRESENT",	    1, 1, "", "")
  REG_DEF_FLD(BCM8726_PMD_PKG1_DEVICES, "CLAUSE22_PRESENT", 0, 0, "", "")
  
  REG_DEF("pmd", BCM8726_PMD_PKG2_DEVICES, "BCM8726_PMD_PKG2_DEVICES", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF_FLD(BCM8726_PMD_PKG2_DEVICES,    "VENDOR_DEV0_PRESENT", 15, 15, "", "")
  REG_DEF_FLD(BCM8726_PMD_PKG2_DEVICES,    "VENDOR_DEV1_PRESENT", 14, 14, "", "")
  
  REG_DEF("pmd", BCM8726_PMD_CTRL2,   "BCM8726_PMD_CTRL2", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF("pmd", BCM8726_PMD_STATUS2, "BCM8726_PMD_STATUS2", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF_FLD(BCM8726_PMD_STATUS2,    "DEVICE_PRESENT",		15, 14, "", "")
  REG_DEF_FLD(BCM8726_PMD_STATUS2,    "TX_LOCAL_FAULT_ABILITY",	13, 13, "", "")
  REG_DEF_FLD(BCM8726_PMD_STATUS2,    "RX_LOCAL_FAULT_ABILITY",	12, 12, "", "")
  REG_DEF_FLD(BCM8726_PMD_STATUS2,    "TX_LOCAL_FAULT",		11, 11, "", "")
  REG_DEF_FLD(BCM8726_PMD_STATUS2,    "RX_LOCAL_FAULT",		10, 10, "", "")
  REG_DEF_FLD(BCM8726_PMD_STATUS2,    "TX_DISABLE_ABILITY",	8, 8, "", "")
  REG_DEF_FLD(BCM8726_PMD_STATUS2,    "10G_BASE_SR_ABILITY",	7, 7, "", "")
  REG_DEF_FLD(BCM8726_PMD_STATUS2,    "10G_BASE_LR_ABILITY",	6, 6, "", "")
  REG_DEF_FLD(BCM8726_PMD_STATUS2,    "10G_BASE_ER_ABILITY",	5, 5, "", "")
  REG_DEF_FLD(BCM8726_PMD_STATUS2,    "10G_BASE_LX4_ABILITY",	4, 4, "", "")
  REG_DEF_FLD(BCM8726_PMD_STATUS2,    "10G_BASE_SW_ABILITY",	3, 3, "", "")
  REG_DEF_FLD(BCM8726_PMD_STATUS2,    "10G_BASE_LW_ABILITY",	2, 2, "", "")
  REG_DEF_FLD(BCM8726_PMD_STATUS2,    "10G_BASE_SW_ABILITY1",	1, 1, "", "")
  REG_DEF_FLD(BCM8726_PMD_STATUS2,    "PMA_LOOPBACK_ABILITY",	0, 0, "", "")
  
  REG_DEF("pmd", BCM8726_PMD_TX_DISABLE, "BCM8726_PMD_TX_DISABLE", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF_FLD(BCM8726_PMD_TX_DISABLE,    "TX_DISABLE3",		4, 4, "", "")
  REG_DEF_FLD(BCM8726_PMD_TX_DISABLE,    "TX_DISABLE2",		3, 3, "", "")
  REG_DEF_FLD(BCM8726_PMD_TX_DISABLE,    "TX_DISABLE1",		2, 2, "", "")
  REG_DEF_FLD(BCM8726_PMD_TX_DISABLE,    "TX_DISABLE0",		1, 1, "", "")
  REG_DEF_FLD(BCM8726_PMD_TX_DISABLE,    "GLOBAL_TX_DISABLE",	0, 0, "", "")
  
  REG_DEF("pmd", BCM8726_PMD_RX_SIGNAL_DETECT, "BCM8726_PMD_RX_SIGNAL_DETECT", "", 0xFFFF, 0x00, "cfg")
  REG_DEF_FLD(BCM8726_PMD_RX_SIGNAL_DETECT,    "SIGNAL_3",		4, 4, "", "")
  REG_DEF_FLD(BCM8726_PMD_RX_SIGNAL_DETECT,    "SIGNAL_2",		3, 3, "", "")
  REG_DEF_FLD(BCM8726_PMD_RX_SIGNAL_DETECT,    "SIGNAL_1",		2, 2, "", "")
  REG_DEF_FLD(BCM8726_PMD_RX_SIGNAL_DETECT,    "SIGNAL_0",		1, 1, "", "")
  REG_DEF_FLD(BCM8726_PMD_RX_SIGNAL_DETECT,    "GLOBAL_RX_SIGNAL",	0, 0, "", "")
  
  REG_DEF("pmd", BCM8726_PMD_OUI_0, "BCM8726_PMD_OUI_0", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF("pmd", BCM8726_PMD_OUI_1, "BCM8726_PMD_OUI_1", "", 0xFFFF, 0x00, "cfg") 
  
  REG_DEF("pmd", BCM8726_UD_2WIRE_CTRL, "BCM8726_UD_2WIRE_CTRL", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF_FLD(BCM8726_UD_2WIRE_CTRL,    "2W_CS_OK	", 8, 8, "", "")
  REG_DEF_FLD(BCM8726_UD_2WIRE_CTRL,    "PAGE_SEL",   7, 6, "", "")
  REG_DEF_FLD(BCM8726_UD_2WIRE_CTRL,    "DATA_TXFR_CMD", 5, 4, "", "")
  REG_DEF_FLD(BCM8726_UD_2WIRE_CTRL,    "CTRL_STAT", 3, 2, "", "")
  REG_DEF_FLD(BCM8726_UD_2WIRE_CTRL,    "EXTENDED_COM	", 1, 1, "", "")
  REG_DEF_FLD(BCM8726_UD_2WIRE_CTRL,    "ENABLE		", 0, 0, "", "")
  
  REG_DEF("pmd", BCM8726_UD_2WIRE_CS, "BCM8726_UD_2WIRE_CS", "", 0xFFFF, 0x00, "cfg") 
  
  REG_DEF("pmd", BCM8726_UD_2WIRE_BYTE_CNT, "BCM8726_UD_2WIRE_BYTE_CNT", "", 0xFFFF, 0x00, "cfg")
  REG_DEF_FLD(BCM8726_UD_2WIRE_BYTE_CNT,    "CHP_ADDR", 15, 9, "", "")
  REG_DEF_FLD(BCM8726_UD_2WIRE_BYTE_CNT,    "CHP_DATA", 8, 0, "", "")
  
  REG_DEF("pmd", BCM8726_UD_2WIRE_MEM_ADDR, "BCM8726_UD_2WIRE_MEM_ADDR", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF_FLD(BCM8726_UD_2WIRE_MEM_ADDR, "SCL_SEL	", 14, 14, "", "")
  REG_DEF_FLD(BCM8726_UD_2WIRE_MEM_ADDR, "ADDR", 7, 0, "", "")
  
  REG_DEF("pmd", BCM8726_UD_2WIRE_MDIO_ADDR, "BCM8726_UD_2WIRE_MDIO_ADDR", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF("pmd", BCM8726_BSC_EEPROM_REG_BASE, "BCM8726_BSC_EEPROM_REG_BASE", "", 0xFFFF, 0x00, "cfg")
  
  REG_DEF("pmd", BCM8726_UD_RX_ALARM, "BCM8726_UD_RX_ALARM", "", 0xFFFF, 0x00, "cfg")
  REG_DEF_FLD(BCM8726_UD_RX_ALARM, "RX_PWR_FLT_ENA", 5, 5, "", "")
  REG_DEF_FLD(BCM8726_UD_RX_ALARM, "PMD_LCL_FLT_ENA", 4, 4, "", "")
  REG_DEF_FLD(BCM8726_UD_RX_ALARM, "PCS_FLT_ENA	", 3, 3, "", "")
  REG_DEF_FLD(BCM8726_UD_RX_ALARM, "RX_ALARM_FLAG", 1, 1, "", "")
  REG_DEF_FLD(BCM8726_UD_RX_ALARM, "XS_FLT_ENA", 0, 0, "", "")
  
  REG_DEF("pmd", BCM8726_UD_TX_ALARM, "BCM8726_UD_TX_ALARM", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF_FLD(BCM8726_UD_TX_ALARM, "LASER_BIAS_FLT_ENA", 9, 9, "", "")
  REG_DEF_FLD(BCM8726_UD_TX_ALARM, "LASER_TEMP_FLT_ENA", 8, 8, "", "")
  REG_DEF_FLD(BCM8726_UD_TX_ALARM, "LASER_PWR_FLT_ENA", 7, 7, "", "")
  REG_DEF_FLD(BCM8726_UD_TX_ALARM, "TX_FLT_ENA", 6, 6, "", "")
  REG_DEF_FLD(BCM8726_UD_TX_ALARM, "PMA_TX_FLT_ENA", 4, 4, "", "")
  REG_DEF_FLD(BCM8726_UD_TX_ALARM, "PCS_TX_FLT_ENA", 3, 3, "", "")
  REG_DEF_FLD(BCM8726_UD_TX_ALARM, "XS_TX_FLT_ENA", 0, 0, "", "")
  
  REG_DEF("pmd", BCM8726_UD_LASI_CTRL, "BCM8726_UD_LASI_CTRL", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF_FLD(BCM8726_UD_LASI_CTRL, "RX_ALARM_ENA", 2, 2, "", "")
  REG_DEF_FLD(BCM8726_UD_LASI_CTRL, "TX_ALARM_ENA", 1, 1, "", "")
  REG_DEF_FLD(BCM8726_UD_LASI_CTRL, "LS_ALARM_ENA", 0, 0, "", "")
  
  REG_DEF("pmd", BCM8726_UD_RX_ALARM_STATUS, "BCM8726_UD_RX_ALARM_STATUS", "", 0xFFFF, 0x00, "cfg")
  REG_DEF_FLD(BCM8726_UD_RX_ALARM_STATUS, "RX_OPT_FLT", 5, 5, "", "")
  REG_DEF_FLD(BCM8726_UD_RX_ALARM_STATUS, "PMD_FLT", 4, 4, "", "")
  REG_DEF_FLD(BCM8726_UD_RX_ALARM_STATUS, "PCSR_FLT", 3, 3, "", "")
  REG_DEF_FLD(BCM8726_UD_RX_ALARM_STATUS, "RX_ALARM_FLAG", 1, 1, "", "")
  REG_DEF_FLD(BCM8726_UD_RX_ALARM_STATUS, "XS_R_FLT", 0, 0, "", "")
  
  REG_DEF("pmd", BCM8726_UD_TX_ALARM_STATUS	, "BCM8726_UD_TX_ALARM_STATUS", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF_FLD(BCM8726_UD_TX_ALARM_STATUS, "LASER_BIAS_FLT", 9, 9, "", "")
  REG_DEF_FLD(BCM8726_UD_TX_ALARM_STATUS, "LASER_TEMP_FLT", 8, 8, "", "")
  REG_DEF_FLD(BCM8726_UD_TX_ALARM_STATUS, "LASER_PWR_FLT", 7, 7, "", "")
  REG_DEF_FLD(BCM8726_UD_TX_ALARM_STATUS, "TX_FLT", 6, 6, "", "")
  REG_DEF_FLD(BCM8726_UD_TX_ALARM_STATUS, "PMA_TX_FLT", 4, 4, "", "")
  REG_DEF_FLD(BCM8726_UD_TX_ALARM_STATUS, "PCS_TX_FLT", 3, 3, "", "")
  REG_DEF_FLD(BCM8726_UD_TX_ALARM_STATUS, "TX_ALARM_FLAG", 1, 1, "", "")
  REG_DEF_FLD(BCM8726_UD_TX_ALARM_STATUS, "XS_TX_FLT", 0, 0, "", "")
  
  REG_DEF("pmd", BCM8726_UD_LASI_STATUS	, "BCM8726_UD_LASI_STATUS", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF("pmd", BCM8726_UD_TX_FLAG_CTRL, "BCM8726_UD_TX_FLAG_CTRL", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF("pmd", BCM8726_UD_RX_FLAG_CTRL, "BCM8726_UD_RX_FLAG_CTRL", "", 0xFFFF, 0x00, "cfg") 
  
  REG_DEF("pmd", BCM8726_UD_PHY_ID, "BCM8726_UD_PHY_ID", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF("pmd", BCM8726_UD_CHIP_REV, "BCM8726_UD_CHIP_REV", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF("pmd", BCM8726_UD_CHIP_ID, "BCM8726_UD_CHIP_ID", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF("pmd", BCM8726_UD_PHY_STATUS, "BCM8726_UD_PHY_STATUS", "", 0xFFFF, 0x00, "cfg") 
  
  REG_DEF("pmd", BCM8726_UD_PMD_DIGITAL_CTRL, "BCM8726_UD_PMD_DIGITAL_CTRL", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF_FLD(BCM8726_UD_PMD_DIGITAL_CTRL, "FAULT_MODE	", 15, 15, "", "")
  REG_DEF_FLD(BCM8726_UD_PMD_DIGITAL_CTRL, "TX_PWR_DOWN	", 14, 14, "", "")
  REG_DEF_FLD(BCM8726_UD_PMD_DIGITAL_CTRL, "RX_PWR_DOWN	", 13, 13, "", "")
  REG_DEF_FLD(BCM8726_UD_PMD_DIGITAL_CTRL, "EXT_FLT_EN	", 12, 12, "", "")
  REG_DEF_FLD(BCM8726_UD_PMD_DIGITAL_CTRL, "OPT_RST", 11, 11, "", "")
  REG_DEF_FLD(BCM8726_UD_PMD_DIGITAL_CTRL, "PCS_TX_INV_B", 10, 10, "", "")
  REG_DEF_FLD(BCM8726_UD_PMD_DIGITAL_CTRL, "PCS_RX_INV", 9, 9, "", "")
  REG_DEF_FLD(BCM8726_UD_PMD_DIGITAL_CTRL, "GPIO_SEL",	6, 4, "", "")
  REG_DEF_FLD(BCM8726_UD_PMD_DIGITAL_CTRL, "LPBK_ERR_DIS", 3, 3, "", "")
  REG_DEF_FLD(BCM8726_UD_PMD_DIGITAL_CTRL, "TX_ON_OFF_PWDWN_DIS", 0, 0, "", "")
  
  REG_DEF("pmd", BCM8726_UD_PMD_DIGITAL_STATUS, "BCM8726_UD_PMD_DIGITAL_STATUS", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF("pmd", BCM8726_UD_TEST_CTRL, "BCM8726_UD_TEST_CTRL", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF("pmd", BCM8726_UD_BIST_CTRL, "BCM8726_UD_BIST_CTRL", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF_FLD(BCM8726_UD_BIST_CTRL, "PBIST_ON", 15, 15, "", "")
  REG_DEF_FLD(BCM8726_UD_BIST_CTRL, "XBIST_ON", 14, 14, "", "")
  REG_DEF_FLD(BCM8726_UD_BIST_CTRL, "R64_JITT_PTRN_DETECT", 13, 13, "", "")
  REG_DEF_FLD(BCM8726_UD_BIST_CTRL, "TX_GEN_PAT_ENA", 11, 11, "", "")
  REG_DEF_FLD(BCM8726_UD_BIST_CTRL, "TX_6SQ_WAVE_PATTERN", 10, 10, "", "")
  REG_DEF_FLD(BCM8726_UD_BIST_CTRL, "XGXS_TX_BIST_LANE_CHECK", 9, 9, "", "")
  REG_DEF_FLD(BCM8726_UD_BIST_CTRL, "XGXS_RX_BIST_LANE_CHECK", 8, 8, "", "")
  REG_DEF_FLD(BCM8726_UD_BIST_CTRL, "PMD_TX_BIST_ENA", 7, 7, "", "")
  REG_DEF_FLD(BCM8726_UD_BIST_CTRL, "PMD_RX_BIST_ENA", 6, 6, "", "")
  REG_DEF_FLD(BCM8726_UD_BIST_CTRL, "BIST_INJECT_ERROR", 5, 5, "", "")
  REG_DEF_FLD(BCM8726_UD_BIST_CTRL, "BIST_VTYPE", 4, 3, "", "")
  REG_DEF_FLD(BCM8726_UD_BIST_CTRL, "BIST_RESULT_SEL", 1, 1, "", "")
  REG_DEF_FLD(BCM8726_UD_BIST_CTRL, "BIST_IPG_SEL", 0, 0, "", "")
  
  REG_DEF("pmd", BCM8726_UD_TX_PATTERN_0, "BCM8726_UD_TX_PATTERN_0", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF("pmd", BCM8726_UD_TX_PATTERN_1, "BCM8726_UD_TX_PATTERN_1", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF("pmd", BCM8726_UD_GPIO_CTRL, "BCM8726_UD_GPIO_CTRL", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF("pmd", BCM8726_UD_PRBS31_WIN_0, "BCM8726_UD_PRBS31_WIN_0", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF("pmd", BCM8726_UD_PRBS31_WIN_1, "BCM8726_UD_PRBS31_WIN_1", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF("pmd", BCM8726_UD_PRBS31_WIN_2, "BCM8726_UD_PRBS31_WIN_2", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF("pmd", BCM8726_UD_BIST_PKT_SIZE, "BCM8726_UD_BIST_PKT_SIZE", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF("pmd", BCM8726_UD_BIST_TX_CNT_CTRL, "BCM8726_UD_BIST_TX_CNT_CTRL", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF("pmd", BCM8726_UD_XGXS_BIST_ERROR, "BCM8726_UD_XGXS_BIST_ERROR", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF("pmd", BCM8726_UD_PCS_BIST_ERROR, "BCM8726_UD_PCS_BIST_ERROR", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF("pmd", BCM8726_UD_PMD_TX_CTRL, "BCM8726_UD_PMD_TX_CTRL", "", 0xFFFF, 0x00, "cfg") 
  
  REG_DEF("pmd", BCM8726_EDC_CTRL_STATUS, "BCM8726_EDC_CTRL_STATUS", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF_FLD(BCM8726_EDC_CTRL_STATUS, "UC_BPASS", 12, 12, "", "")
  REG_DEF_FLD(BCM8726_EDC_CTRL_STATUS, "SEQ_DEF_BYPASS", 11, 11, "", "")
  REG_DEF_FLD(BCM8726_EDC_CTRL_STATUS, "EQ_BYPASS", 10, 10, "", "")
  REG_DEF_FLD(BCM8726_EDC_CTRL_STATUS, "PGA_CLK_SEL", 9, 9, "", "")
  REG_DEF_FLD(BCM8726_EDC_CTRL_STATUS, "UC_REF_CLK_SEL", 8, 8, "", "")
  REG_DEF_FLD(BCM8726_EDC_CTRL_STATUS, "UC_CLK_SEL", 7, 6, "", "")
  REG_DEF_FLD(BCM8726_EDC_CTRL_STATUS, "UC_DEBUF", 5, 4, "", "")
  REG_DEF_FLD(BCM8726_EDC_CTRL_STATUS, "UC_REBOOT", 3, 3, "", "")
  REG_DEF_FLD(BCM8726_EDC_CTRL_STATUS, "UC_RESET", 2, 2, "", "")
  REG_DEF_FLD(BCM8726_EDC_CTRL_STATUS, "SOFT_RESET", 1, 1, "", "")
  REG_DEF_FLD(BCM8726_EDC_CTRL_STATUS, "GLOBAL_RESET", 0, 0, "", "")
  
  REG_DEF("pmd", BCM8726_EDC_MESSAGE, "BCM8726_EDC_MESSAGE", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF("pmd", BCM8726_EDC_GP_DATA, "BCM8726_EDC_GP_DATA", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF("pmd", BCM8726_EDC_GP_LOAD, "BCM8726_EDC_GP_LOAD", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF("pmd", BCM8726_EEPROM_LOAD,  "BCM8726_EEPROM_LOAD", "", 0xFFFF, 0x00, "cfg") 
  
  REG_DEF("pcs", BCM8726_PRBS23_WIN_2, "BCM8726_PRBS23_WIN_2", "", 0xFFFF, 0x00, "cfg") 
  
  REG_DEF("pcs", BCM8726_PCS_CTRL, "BCM8726_PCS_CTRL", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF_FLD(BCM8726_PCS_CTRL, "RESET", 15, 15, "", "")
  REG_DEF_FLD(BCM8726_PCS_CTRL, "LOOPBACK", 14, 14, "", "")
  REG_DEF_FLD(BCM8726_PCS_CTRL, "SPEED_SELECT1", 13, 13, "", "")
  REG_DEF_FLD(BCM8726_PCS_CTRL, "LOW_POWER", 11, 11, "", "")
  REG_DEF_FLD(BCM8726_PCS_CTRL, "SPEED_SELECT0", 6, 6, "", "")
  REG_DEF_FLD(BCM8726_PCS_CTRL, "SPEED_10GBS", 5, 2, "", "")
  REG_DEF_FLD(BCM8726_PCS_CTRL, "PMD_LPBK", 0, 0, "", "")
  
  REG_DEF("pcs", BCM8726_PCS_STATUS , "BCM8726_PCS_STATUS", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF_FLD(BCM8726_PCS_STATUS, "LOCAL_FAULT", 7, 7, "", "")
  REG_DEF_FLD(BCM8726_PCS_STATUS, "RX_LINK_STATUS", 2, 2, "", "")
  REG_DEF_FLD(BCM8726_PCS_STATUS, "LOW_POWER_ABILITY", 1, 1, "", "")
  
  REG_DEF("pcs", BCM8726_PCS_DEV_ID0, "BCM8726_PCS_DEV_ID0", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF("pcs", BCM8726_PCS_DEV_ID1, "BCM8726_PCS_DEV_ID1", "", 0xFFFF, 0x00, "cfg") 
  
  REG_DEF("pcs", BCM8726_PCS_SPEED_ABILITY, "BCM8726_PCS_SPEED_ABILITY", "", 0xFFFF, 0x00, "cfg") 
  
  REG_DEF("pcs", BCM8726_PCS_PKG1_DEVICES, "BCM8726_PCS_PKG1_DEVICES", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF_FLD(BCM8726_PCS_PKG1_DEVICES, "DTE_XS_PRESENT",5, 5, "", "")
  REG_DEF_FLD(BCM8726_PCS_PKG1_DEVICES, "PHY_XS_PRESENT",4, 4, "", "")
  REG_DEF_FLD(BCM8726_PCS_PKG1_DEVICES, "PCS_PRESENT",3, 3, "", "")
  REG_DEF_FLD(BCM8726_PCS_PKG1_DEVICES, "WIS_PRESENT",2, 2, "", "")
  REG_DEF_FLD(BCM8726_PCS_PKG1_DEVICES, "PMD_PRESENT",1, 1, "", "")
  REG_DEF_FLD(BCM8726_PCS_PKG1_DEVICES, "CLAUSE22_PRESENT", 0, 0, "", "")
  
  REG_DEF("pcs", BCM8726_PCS_PKG2_DEVICES, "BCM8726_PCS_PKG2_DEVICES", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF_FLD(BCM8726_PCS_PKG2_DEVICES, "VENDOR_DEV0_PRESENT", 15, 15, "", "")
  REG_DEF_FLD(BCM8726_PCS_PKG2_DEVICES, "VENDOR_DEV1_PRESENT", 14, 14, "", "")
  
  REG_DEF("pcs", BCM8726_PCS_CTRL2, "BCM8726_PCS_CTRL2", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF("pcs", BCM8726_PCS_STATUS2, "BCM8726_PCS_STATUS2", "", 0xFFFF, 0x00, "cfg")
  REG_DEF_FLD(BCM8726_PCS_STATUS2, "DEVICE_PRESENT", 15, 14, "", "")
  REG_DEF_FLD(BCM8726_PCS_STATUS2, "TX_LOCAL_FAULT", 11, 11, "", "")
  REG_DEF_FLD(BCM8726_PCS_STATUS2, "RX_LOCAL_FAULT", 10, 10, "", "")
  REG_DEF_FLD(BCM8726_PCS_STATUS2, "10BASE_W_CAPABLE", 2, 2, "", "")
  REG_DEF_FLD(BCM8726_PCS_STATUS2, "10BASE_X_CAPABLE", 1, 1, "", "")
  REG_DEF_FLD(BCM8726_PCS_STATUS2, "10BASE_R_CAPABLE", 0, 0, "", "")
  
  REG_DEF("pcs", BCM8726_PCS_OUI_0, "BCM8726_PCS_OUI_0", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF("pcs", BCM8726_PCS_OUI_1, "BCM8726_PCS_OUI_1", "", 0xFFFF, 0x00, "cfg") 
  
  REG_DEF("pcs", BCM8726_PCS_10BASE_X_STATUS, "BCM8726_PCS_10BASE_X_STATUS", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF_FLD(BCM8726_PCS_10BASE_X_STATUS, "LANE_ALIGNMENT_STATUS", 12, 12, "", "")
  REG_DEF_FLD(BCM8726_PCS_10BASE_X_STATUS, "LANE_D_SYNC", 3, 3, "", "")
  REG_DEF_FLD(BCM8726_PCS_10BASE_X_STATUS, "LANE_C_SYNC", 2, 2, "", "")
  REG_DEF_FLD(BCM8726_PCS_10BASE_X_STATUS, "LANE_B_SYNC", 1, 1, "", "")
  REG_DEF_FLD(BCM8726_PCS_10BASE_X_STATUS, "LANE_A_SYNC", 0, 0, "", "")
  
  REG_DEF("pcs", BCM8726_PCS_10BASE_R_STATUS, "BCM8726_PCS_10BASE_R_STATUS", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF_FLD(BCM8726_PCS_10BASE_R_STATUS, "RX_LINK_STATUS", 12, 12, "", "")
  REG_DEF_FLD(BCM8726_PCS_10BASE_R_STATUS, "PRBS31_ABLE", 2, 2, "", "")
  REG_DEF_FLD(BCM8726_PCS_10BASE_R_STATUS, "HI_BER", 1, 1, "", "")
  REG_DEF_FLD(BCM8726_PCS_10BASE_R_STATUS, "BLOCK_LOCK", 0, 0, "", "")
  
  REG_DEF("pcs", BCM8726_PCS_10BASE_R_STATUS2	, "BCM8726_PCS_10BASE_R_STATUS2", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF_FLD(BCM8726_PCS_10BASE_R_STATUS2, "LATCH_BLK_LOCK", 15, 15, "", "")
  REG_DEF_FLD(BCM8726_PCS_10BASE_R_STATUS2, "LATCH_HI_BER", 14, 14, "", "")
  REG_DEF_FLD(BCM8726_PCS_10BASE_R_STATUS2, "BER", 13, 8, "", "")
  REG_DEF_FLD(BCM8726_PCS_10BASE_R_STATUS2, "ERR_BLK_CNT", 7, 0, "", "")
  
  REG_DEF("pcs", BCM8726_PCS_10BASE_R_JITTER_TEST_SPEED_A0, "BCM8726_PCS_10BASE_R_JITTER_TEST_SPEED_A0", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF("pcs", BCM8726_PCS_10BASE_R_JITTER_TEST_SPEED_A1, "BCM8726_PCS_10BASE_R_JITTER_TEST_SPEED_A1", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF("pcs", BCM8726_PCS_10BASE_R_JITTER_TEST_SPEED_A2, "BCM8726_PCS_10BASE_R_JITTER_TEST_SPEED_A2", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF("pcs", BCM8726_PCS_10BASE_R_JITTER_TEST_SPEED_A3, "BCM8726_PCS_10BASE_R_JITTER_TEST_SPEED_A3", "", 0xFFFF, 0x00, "cfg") 
  
  REG_DEF("pcs", BCM8726_PCS_10BASE_R_JITTER_TEST_SPEED_B0, "BCM8726_PCS_10BASE_R_JITTER_TEST_SPEED_B0", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF("pcs", BCM8726_PCS_10BASE_R_JITTER_TEST_SPEED_B1, "BCM8726_PCS_10BASE_R_JITTER_TEST_SPEED_B1", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF("pcs", BCM8726_PCS_10BASE_R_JITTER_TEST_SPEED_B2, "BCM8726_PCS_10BASE_R_JITTER_TEST_SPEED_B2", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF("pcs", BCM8726_PCS_10BASE_R_JITTER_TEST_SPEED_B3, "BCM8726_PCS_10BASE_R_JITTER_TEST_SPEED_B3", "", 0xFFFF, 0x00, "cfg") 
 
  REG_DEF("pcs", BCM8726_PCS_10BASE_R_JITTER_TEST_CONTROL, "BCM8726_PCS_10BASE_R_JITTER_TEST_CONTROL", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF_FLD(BCM8726_PCS_10BASE_R_JITTER_TEST_CONTROL, "PRBS31_CHK_ENA", 5, 5, "", "")
  REG_DEF_FLD(BCM8726_PCS_10BASE_R_JITTER_TEST_CONTROL, "PRBS31_TX_ENA", 4, 4, "", "")
  REG_DEF_FLD(BCM8726_PCS_10BASE_R_JITTER_TEST_CONTROL, "JITTER_TX_ENA	", 3, 3, "", "")
  REG_DEF_FLD(BCM8726_PCS_10BASE_R_JITTER_TEST_CONTROL, "JITTER_RX_PATTERN", 2, 2, "", "")
  REG_DEF_FLD(BCM8726_PCS_10BASE_R_JITTER_TEST_CONTROL, "JITTER_TEST_PATTERN", 1, 1, "", "")
  REG_DEF_FLD(BCM8726_PCS_10BASE_R_JITTER_TEST_CONTROL, "JITTER_DATA_PATTERN", 0, 0, "", "")
  
  REG_DEF("pcs", BCM8726_PCS_10BASE_R_JITTER_TEST_ERROR_COUNTER	, "BCM8726_PCS_10BASE_R_JITTER_TEST_ERROR_COUNTER", "", 0xFFFF, 0x00, "cfg") 
  
  REG_DEF("xgxs", BCM8726_PHY_XS_CTRL, "BCM8726_PHY_XS_CTRL", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF_FLD(BCM8726_PHY_XS_CTRL, "RESET",15, 15, "", "")
  REG_DEF_FLD(BCM8726_PHY_XS_CTRL, "LOOPBACK", 14, 14, "", "")
  REG_DEF_FLD(BCM8726_PHY_XS_CTRL, "SPEED_SELECT1", 13, 13, "", "")
  REG_DEF_FLD(BCM8726_PHY_XS_CTRL, "LOW_POWER", 11, 11, "", "")
  REG_DEF_FLD(BCM8726_PHY_XS_CTRL, "SPEED_SELECT0", 6, 6, "", "")
  REG_DEF_FLD(BCM8726_PHY_XS_CTRL, "SPEED_10GBS", 5, 2, "", "")
  REG_DEF_FLD(BCM8726_PHY_XS_CTRL, "PMD_LPBK", 0, 0, "", "")
  
  REG_DEF("xgxs", BCM8726_PHY_XS_STATUS , "BCM8726_PHY_XS_STATUS", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF_FLD(BCM8726_PHY_XS_STATUS, "LOCAL_FAULT",7, 7, "", "")
  REG_DEF_FLD(BCM8726_PHY_XS_STATUS, "TX_LINK_STATUS", 2, 2, "", "")
  REG_DEF_FLD(BCM8726_PHY_XS_STATUS, "LOW_POWER_ABILITY", 1, 1, "", "")
  
  REG_DEF("xgxs", BCM8726_PHY_XS_DEV_ID0, "BCM8726_PHY_XS_DEV_ID0", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF("xgxs", BCM8726_PHY_XS_DEV_ID1, "BCM8726_PHY_XS_DEV_ID1", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF("xgxs", BCM8726_PHY_XS_SPEED_ABILITY, "BCM8726_PHY_XS_SPEED_ABILITY", "", 0xFFFF, 0x00, "cfg") 
  
  REG_DEF("xgxs", BCM8726_PHY_XS_PKG1_DEVICES, "BCM8726_PHY_XS_PKG1_DEVICES", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF_FLD(BCM8726_PHY_XS_PKG1_DEVICES, "DTE_XS_PRESENT",5, 5, "", "")
  REG_DEF_FLD(BCM8726_PHY_XS_PKG1_DEVICES, "PHY_XS_PRESENT",4, 4, "", "")
  REG_DEF_FLD(BCM8726_PHY_XS_PKG1_DEVICES, "PCS_PRESENT",3, 3, "", "")
  REG_DEF_FLD(BCM8726_PHY_XS_PKG1_DEVICES, "WIS_PRESENT",2, 2, "", "")
  REG_DEF_FLD(BCM8726_PHY_XS_PKG1_DEVICES, "PMD_PRESENT",1, 1, "", "")
  REG_DEF_FLD(BCM8726_PHY_XS_PKG1_DEVICES, "CLAUSE22_PRESENT", 0, 0, "", "")
  
  REG_DEF("xgxs", BCM8726_PHY_XS_PKG2_DEVICES, "BCM8726_PHY_XS_PKG2_DEVICES", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF_FLD(BCM8726_PHY_XS_PKG2_DEVICES, "VENDOR_DEV0_PRESENT",15, 15, "", "")
  
  REG_DEF("xgxs", BCM8726_PHY_XS_STATUS2, "BCM8726_PHY_XS_STATUS2", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF_FLD(BCM8726_PHY_XS_STATUS2, "DEVICE_PRESENT", 15, 14, "", "")
  REG_DEF_FLD(BCM8726_PHY_XS_STATUS2, "TX_LOCAL_FAULT", 11, 11, "", "")
  REG_DEF_FLD(BCM8726_PHY_XS_STATUS2, "RX_LOCAL_FAULT", 10, 10, "", "")
  
  REG_DEF("xgxs", BCM8726_PHY_XS_OUI_0, "BCM8726_PHY_XS_OUI_0", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF("xgxs", BCM8726_PHY_XS_OUI_1, "BCM8726_PHY_XS_OUI_1", "", 0xFFFF, 0x00, "cfg") 
  
  REG_DEF("xgxs", BCM8726_PHY_XS_XGXS_LANE_STATUS     , "BCM8726_PHY_XS_XGXS_LANE_STATUS", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF_FLD(BCM8726_PHY_XS_XGXS_LANE_STATUS, "LANE_ALIGNMENT_STATUS", 12, 12, "", "")
  REG_DEF_FLD(BCM8726_PHY_XS_XGXS_LANE_STATUS, "PATTERN_TESTING_ABILITY", 11, 11, "", "")
  REG_DEF_FLD(BCM8726_PHY_XS_XGXS_LANE_STATUS, "LANE_D_SYNC", 3, 3, "", "")
  REG_DEF_FLD(BCM8726_PHY_XS_XGXS_LANE_STATUS, "LANE_C_SYNC", 2, 2, "", "")
  REG_DEF_FLD(BCM8726_PHY_XS_XGXS_LANE_STATUS, "LANE_B_SYNC", 1, 1, "", "")
  REG_DEF_FLD(BCM8726_PHY_XS_XGXS_LANE_STATUS, "LANE_A_SYNC", 0, 0, "", "")
  
  REG_DEF("xgxs", BCM8726_PHY_XS_XGXS_TEST_CONTROL, "BCM8726_PHY_XS_XGXS_TEST_CONTROL", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF_FLD(BCM8726_PHY_XS_XGXS_TEST_CONTROL, "MD_PAT_EN", 2, 2, "", "")
  REG_DEF_FLD(BCM8726_PHY_XS_XGXS_TEST_CONTROL, "MD_PTYPE", 1, 0, "", "")
  
  REG_DEF("xgxs", BCM8726_XGXS_MODE_CTRL,	"BCM8726_XGXS_MODE_CTRL", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF("xgxs", BCM8726_XGXS_STATUS,		"BCM8726_XGXS_STATUS", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF("xgxs", BCM8726_XGXS_LANE_CTRL,	"BCM8726_XGXS_LANE_CTRL", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF("xgxs", BCM8726_XGXS_TX_RAM, 		"BCM8726_XGXS_TX_RAM", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF("xgxs", BCM8726_XGXS_TX_RAM_DATA_LO, 	"BCM8726_XGXS_TX_RAM_DATA_LO", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF("xgxs", BCM8726_XGXS_TX_RAM_DATA_HI, 	"BCM8726_XGXS_TX_RAM_DATA_HI", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF("xgxs", BCM8726_XGXS_TX_POLY, 	"BCM8726_XGXS_TX_POLY", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF("xgxs", BCM8726_XGXS_TX_POLY_SPEED, 	"BCM8726_XGXS_TX_POLY_SPEED", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF("xgxs", BCM8726_XGXS_TX_BYTE_CNT_HI, 	"BCM8726_XGXS_TX_BYTE_CNT_HI", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF("xgxs", BCM8726_XGXS_TX_BYTE_CNT_LO, 	"BCM8726_XGXS_TX_BYTE_CNT_LO", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF("xgxs", BCM8726_XGXS_TX_PKT_CNT_HI, 	"BCM8726_XGXS_TX_PKT_CNT_HI", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF("xgxs", BCM8726_XGXS_TX_PKT_CNT_LO, 	"BCM8726_XGXS_TX_PKT_CNT_LO", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF("xgxs", BCM8726_XGXS_RX_RAM, 		"BCM8726_XGXS_RX_RAM", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF("xgxs", BCM8726_XGXS_RX_RAM_DATA_LO, 	"BCM8726_XGXS_RX_RAM_DATA_LO", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF("xgxs", BCM8726_XGXS_RX_RAM_DATA_HI, 	"BCM8726_XGXS_RX_RAM_DATA_HI", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF("xgxs", BCM8726_XGXS_RX_POLY, 	"BCM8726_XGXS_RX_POLY", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF("xgxs", BCM8726_XGXS_RX_POLY_SPEED, 	"BCM8726_XGXS_RX_POLY_SPEED", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF("xgxs", BCM8726_XGXS_RX_BYTE_CNT_HI, 	"BCM8726_XGXS_RX_BYTE_CNT_HI", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF("xgxs", BCM8726_XGXS_RX_BYTE_CNT_LO, 	"BCM8726_XGXS_RX_BYTE_CNT_LO", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF("xgxs", BCM8726_XGXS_RX_PKT_CNT_HI, 	"BCM8726_XGXS_RX_PKT_CNT_HI", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF("xgxs", BCM8726_XGXS_RX_PKT_CNT_LO, 	"BCM8726_XGXS_RX_PKT_CNT_LO", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF("xgxs", BCM8726_XGXS_RX_BIT_ERR_CNT, 	"BCM8726_XGXS_RX_BIT_ERR_CNT", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF("xgxs", BCM8726_XGXS_RX_PKT_ERR_CNT, 	"BCM8726_XGXS_RX_PKT_ERR_CNT", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF("xgxs", BCM8726_XGXS_RX_BYTE_ERR_CNT, "BCM8726_XGXS_RX_BYTE_ERR_CNT", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF("xgxs", BCM8726_XGXS_RX_STATUS_BITS, 	"BCM8726_XGXS_RX_STATUS_BITS", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF("xgxs", BCM8726_XGXS_ALL_RAM_ADDR, 	"BCM8726_XGXS_ALL_RAM_ADDR", "", 0xFFFF, 0x00, "cfg") 

  REG_DEF("xgxs", BCM8726_XGXS_ALL_RAM_RAM_DATA_LO, 	"BCM8726_XGXS_ALL_RAM_RAM_DATA_LO", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF("xgxs", BCM8726_XGXS_ALL_RAM_RAM_DATA_HI, 	"BCM8726_XGXS_ALL_RAM_RAM_DATA_HI", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF("xgxs", BCM8726_XGXS_ALL_RAM_POLY, 		"BCM8726_XGXS_ALL_RAM_POLY", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF("xgxs", BCM8726_XGXS_ALL_RAM_POLY_SPEED, 	"BCM8726_XGXS_ALL_RAM_POLY_SPEED", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF("xgxs", BCM8726_XGXS_ALL_RAM_BYTE_CNT_HI, 	"BCM8726_XGXS_ALL_RAM_BYTE_CNT_HI", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF("xgxs", BCM8726_XGXS_ALL_RAM_BYTE_CNT_LO, 	"BCM8726_XGXS_ALL_RAM_BYTE_CNT_LO", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF("xgxs", BCM8726_XGXS_ALL_RAM_PKT_CNT_HI, 	"BCM8726_XGXS_ALL_RAM_PKT_CNT_HI", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF("xgxs", BCM8726_XGXS_ALL_RAM_PKT_CNT_LO, 	"BCM8726_XGXS_ALL_RAM_PKT_CNT_LO", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF("xgxs", BCM8726_XGXS_ALL_RAM_BIT_ERR_CNT, 	"BCM8726_XGXS_ALL_RAM_BIT_ERR_CNT", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF("xgxs", BCM8726_XGXS_ALL_RAM_PKT_ERR_CNT, 	"BCM8726_XGXS_ALL_RAM_PKT_ERR_CNT", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF("xgxs", BCM8726_XGXS_ALL_RAM_BYTE_ERR_CNT, 	"BCM8726_XGXS_ALL_RAM_BYTE_ERR_CNT", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF("xgxs", BCM8726_XGXS_ALL_RAM_STATUS_BITS, 	"BCM8726_XGXS_ALL_RAM_STATUS_BITS", "", 0xFFFF, 0x00, "cfg") 

  REG_DEF("xgxs", BCM8726_TX0_ACONTROL_0, "BCM8726_TX0_ACONTROL_0", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF("xgxs", BCM8726_TX0_ACONTROL_3, "BCM8726_TX0_ACONTROL_3", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF("xgxs", BCM8726_TX1_ACONTROL_0, "BCM8726_TX1_ACONTROL_0", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF("xgxs", BCM8726_TX1_ACONTROL_3, "BCM8726_TX1_ACONTROL_3", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF("xgxs", BCM8726_TX2_ACONTROL_0, "BCM8726_TX2_ACONTROL_0", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF("xgxs", BCM8726_TX2_ACONTROL_3, "BCM8726_TX2_ACONTROL_3", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF("xgxs", BCM8726_TX3_ACONTROL_0, "BCM8726_TX3_ACONTROL_0", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF("xgxs", BCM8726_TX3_ACONTROL_3, "BCM8726_TX3_ACONTROL_3", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF("xgxs", BCM8726_TXA_ACONTROL_0, "BCM8726_TXA_ACONTROL_0", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF("xgxs", BCM8726_TXA_ACONTROL_3, "BCM8726_TXA_ACONTROL_3", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF("xgxs", BCM8726_RX3_STATUS, "BCM8726_RX3_STATUS", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF("xgxs", BCM8726_RX3_CONTROL, "BCM8726_RX3_CONTROL", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF("xgxs", BCM8726_RX3_CONTROL1, "BCM8726_RX3_CONTROL1", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF("xgxs", BCM8726_RX2_STATUS, "BCM8726_RX2_STATUS", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF("xgxs", BCM8726_RX2_CONTROL, "BCM8726_RX2_CONTROL", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF("xgxs", BCM8726_RX2_CONTROL1, "BCM8726_RX2_CONTROL1", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF("xgxs", BCM8726_RX1_STATUS, "BCM8726_RX1_STATUS", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF("xgxs", BCM8726_RX1_CONTROL, "BCM8726_RX1_CONTROL", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF("xgxs", BCM8726_RX1_CONTROL1, "BCM8726_RX1_CONTROL1", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF("xgxs", BCM8726_RX0_STATUS, "BCM8726_RX0_STATUS", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF("xgxs", BCM8726_RX0_CONTROL, "BCM8726_RX0_CONTROL", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF("xgxs", BCM8726_RX0_CONTROL1, "BCM8726_RX0_CONTROL1", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF("xgxs", BCM8726_RXA_STATUS, "BCM8726_RXA_STATUS", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF("xgxs", BCM8726_RXA_CONTROL, "BCM8726_RXA_CONTROL", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF("xgxs", BCM8726_RXA_CONTROL1, "BCM8726_RXA_CONTROL1", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF("xgxs", BCM8726_RXA_LANE_CTRL, "BCM8726_RXA_LANE_CTRL", "", 0xFFFF, 0x00, "cfg") 
  REG_DEF("xgxs", BCM8726_TXA_LANE_CTRL, "BCM8726_TXA_LANE_CTRL", "", 0xFFFF, 0x00, "cfg") 

{ TYP_NONE, "", 0, 0, "", "", 0, 0, ""}
};  

uint8_t bcm8726_device_class_get(reg_desc_t *preg)
{
        if (!preg) return (0);

        if (!strcasecmp(preg->blk, "pmd")) {
                return (BCM8726_PMA_PMD_ADDR);
        } else if (!strcasecmp(preg->blk, "pcs")) {
                return (BCM8726_PCS_ADDR);
        } else if (!strcasecmp(preg->blk, "xgxs")) {
                return (BCM8726_XGXS_ADDR);
        } else {
                printf("Error: Invalid block type \n");
        }
        return (0);
}

