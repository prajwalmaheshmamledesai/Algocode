/******************************************************************************
 *
 * $Id: $
 *
 * File: xcvr_brcm8726.h
 * Name: Yulianto Ko
 * Description: Broadcom 8726 header file
 *
 * Copyright (c) 2007, Nuova Systems, Inc.
 * All rights reserved.
 *
 *****************************************************************************/
 
#ifndef __XCVR_BRCM8726_H__
#define __XCVR_BRCM8726_H__
 
// i2c address
#define SFP_MSA_A0                          0xA0
#define SFP_MSA_A2                          0xA2
 
// mdio device address for I2C access registers
#define BRCM_2W_DEV_ADDR                    0x1
 
// Registers for I2C operations
#define BRCM_REG_2WCONTROL                  0x8000
#define BRCM_REG_2WCKSUM                    0x8001
#define BRCM_REG_2WBCOUNT                   0x8002
#define BRCM_REG_2WMEMADD                   0x8003
 
// Registers for setting lane polarity
#define BRCM_REG_PMD_DIGITAL_CTRL           0xC808
 
// Where to store the I2C SPROM read operation
#define BRCM_SPROM_START_MDIO_ADDR          0xC820
#define BRCM_SPROM_END_MDIO_ADDR            0xC82F
 
// Max 16 bytes can be read
#define BRCM_MAX_I2C_READ_CNT               16
 
// Bit definition for BRCM_REG_2WCONTROL
#define BRCM_BIT_2WCONTROL_2WDTF            4
#define BRCM_BIT_2WCONTROL_2WDTF_SIZE       2
#define BRCM_BIT_2WCONTROL_2WCTRLSTAT       2
#define BRCM_BIT_2WCONTROL_2WCTRLSTAT_SIZE  2
#define BRCM_BIT_2WCONTROL_2WEXNDCOM        1
#define BRCM_BIT_2WCONTROL_2WEXNDCOM_SIZE   1
#define BRCM_BIT_2WCONTROL_2WENABLE         0
#define BRCM_BIT_2WCONTROL_2WENABLE_SIZE    1
 
// Bit definition for BRCM_REG_2WBCOUNT
#define BRCM_BIT_2WBCOUNT_2W_CHPADDR        9
#define BRCM_BIT_2WBCOUNT_2W_CHPADDR_SIZE   7
#define BRCM_BIT_2WBCOUNT_2W_CNTREG         0
#define BRCM_BIT_2WBCOUNT_2W_CNTREG_SIZE    9
 
// Bit definition for BRCM_REG_2WMEMADD
#define BRCM_BIT_2WMEMADD_2W_MEMADDR        0
#define BRCM_BIT_2WMEMADD_2W_MEMADDR_SIZE   8
 
// Bit definition for BRCM_REG_PMD_DIGITAL_CTRL
#define BRCM_BIT_PMD_DIGITAL_CTRL_PCS_TX_INV_B      10
#define BRCM_BIT_PMD_DIGITAL_CTRL_PCS_TX_INV_B_SIZE 1
#define BRCM_BIT_PMD_DIGITAL_CTRL_PCS_RX_INV        9
#define BRCM_BIT_PMD_DIGITAL_CTRL_PCS_RX_INV_SIZE   1
 
#endif /* __XCVR_BRCM8726_H__ */
 
/* end of file */
