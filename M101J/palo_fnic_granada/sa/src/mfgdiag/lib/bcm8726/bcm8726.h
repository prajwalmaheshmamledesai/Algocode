
// Generic Defines.
#define BCM8726_PMA_PMD_ADDR	0x01
#define BCM8726_PCS_ADDR	0x03
#define BCM8726_XGXS_ADDR	0x04

#define BCM8726_UD_1_2_3_ADDR	0x01
#define BCM8726_UD_4_ADDR	0x04

#define BCM8726_EEPROM_SIG	0x4321
#define BCM8726_EEPROM_SIG1	0x8E53

////////////////////////////////////////////////////////////////////////
// PMA/PMD address
////////////////////////////////////////////////////////////////////////

#define BCM8726_PMD_CTRL 0x00
#define BCM8726_PMD_CTRL_FLD_RESET		(1<<15)	
#define BCM8726_PMD_CTRL_FLD_SPEED_SELECT1	(1<<13)
#define BCM8726_PMD_CTRL_FLD_LOW_POWER	(1<<11)
#define BCM8726_PMD_CTRL_FLD_SPEED_SELECT0	(1<<6)
#define BCM8726_PMD_CTRL_FLD_SPEED_10GBS	(0xF<<2)
#define BCM8726_PMD_CTRL_FLD_PMD_LPBK	(1<<0)

#define BCM8726_PMD_STATUS 0x01
#define BCM8726_PMD_STATUS_FLD_LOCAL_FAULT		(1<<7)
#define BCM8726_PMD_STATUS_FLD_RX_LINK_STATUS		(1<<2)
#define BCM8726_PMD_STATUS_FLD_LOW_POWER_ABILITY	(1<<1)

#define BCM8726_PMD_DEV_ID0		0x02
#define BCM8726_PMD_DEV_ID0_DFLT	0x20
#define BCM8726_PMD_DEV_ID1		0x03
#define BCM8726_PMD_DEV_ID1_DFLT	0x6035
#define BCM8726_PMD_SPEED_ABILITY	0x04

#define BCM8726_PMD_PKG1_DEVICES	0x05
#define BCM8726_PMD_PKG1_DEVICES_FLD_DTE_XS_PRESENT			(1<<5)
#define BCM8726_PMD_PKG1_DEVICES_FLD_PHY_XS_PRESENT			(1<<4)
#define BCM8726_PMD_PKG1_DEVICES_FLD_PCS_PRESENT			(1<<3)
#define BCM8726_PMD_PKG1_DEVICES_FLD_WIS_PRESENT			(1<<2)
#define BCM8726_PMD_PKG1_DEVICES_FLD_PMD_PRESENT			(1<<1)
#define BCM8726_PMD_PKG1_DEVICES_FLD_CLAUSE22_PRESENT		(1<<0)

#define BCM8726_PMD_PKG2_DEVICES	0x06
#define BCM8726_PMD_PKG2_DEVICES_FLD_VENDOR_DEV0_PRESENT		(1<<15)
#define BCM8726_PMD_PKG2_DEVICES_FLD_VENDOR_DEV1_PRESENT		(1<<14)

#define BCM8726_PMD_CTRL2	0x07

#define BCM8726_PMD_STATUS2	0x08
#define BCM8726_PMD_STATUS2_FLD_DEVICE_PRESENT		(3 << 14)
#define BCM8726_PMD_STATUS2_FLD_TX_LOCAL_FAULT_ABILITY	(1 << 13)
#define BCM8726_PMD_STATUS2_FLD_RX_LOCAL_FAULT_ABILITY	(1 << 12)
#define BCM8726_PMD_STATUS2_FLD_TX_LOCAL_FAULT		(1 << 11)
#define BCM8726_PMD_STATUS2_FLD_RX_LOCAL_FAULT		(1 << 10)
#define BCM8726_PMD_STATUS2_FLD_TX_DISABLE_ABILITY	(1 << 8)
#define BCM8726_PMD_STATUS2_FLD_10G_BASE_SR_ABILITY	(1 << 7)
#define BCM8726_PMD_STATUS2_FLD_10G_BASE_LR_ABILITY	(1 << 6)
#define BCM8726_PMD_STATUS2_FLD_10G_BASE_ER_ABILITY	(1 << 5)
#define BCM8726_PMD_STATUS2_FLD_10G_BASE_LX4_ABILITY	(1 << 4)
#define BCM8726_PMD_STATUS2_FLD_10G_BASE_SW_ABILITY	(1 << 3)
#define BCM8726_PMD_STATUS2_FLD_10G_BASE_LW_ABILITY	(1 << 2)
#define BCM8726_PMD_STATUS2_FLD_10G_BASE_SW_ABILITY1	(1 << 1)
#define BCM8726_PMD_STATUS2_FLD_PMA_LOOPBACK_ABILITY	(1 << 0)


#define BCM8726_PMD_TX_DISABLE	0x09
#define BCM8726_PMD_TX_DISABLE_FLD_TX_DISABLE3		(1 << 4)
#define BCM8726_PMD_TX_DISABLE_FLD_TX_DISABLE2		(1 << 3)
#define BCM8726_PMD_TX_DISABLE_FLD_TX_DISABLE1		(1 << 2)
#define BCM8726_PMD_TX_DISABLE_FLD_TX_DISABLE0		(1 << 1)
#define BCM8726_PMD_TX_DISABLE_FLD_GLOBAL_TX_DISABLE	(1 << 0)

#define BCM8726_PMD_RX_SIGNAL_DETECT   0x0A
#define BCM8726_PMD_RX_SIGNAL_DETECT_FLD_SIGNAL_3		(1 << 4)
#define BCM8726_PMD_RX_SIGNAL_DETECT_FLD_SIGNAL_2		(1 << 3)
#define BCM8726_PMD_RX_SIGNAL_DETECT_FLD_SIGNAL_1		(1 << 2)
#define BCM8726_PMD_RX_SIGNAL_DETECT_FLD_SIGNAL_0		(1 << 1)
#define BCM8726_PMD_RX_SIGNAL_DETECT_FLD_GLOBAL_RX_SIGNAL	(1 << 0)


#define BCM8726_PMD_OUI_0	0x0E
#define BCM8726_PMD_OUI_1	0x0F



// User define register devices.
#define BCM8726_UD_2WIRE_CTRL		0x8000
#define BCM8726_UD_2WIRE_CTRL_FLD_2W_CS_OK	(1 << 8)
#define BCM8726_UD_2WIRE_CTRL_FLD_PAGE_SEL	(3 << 6)
#define BCM8726_UD_2WIRE_CTRL_FLD_DATA_TXFR_CMD	(3 << 4)
#define BCM8726_UD_2WIRE_CTRL_FLD_CTRL_STAT	(3 << 2)
#define BCM8726_UD_2WIRE_CTRL_FLD_EXTENDED_COM	(1 << 1)
#define BCM8726_UD_2WIRE_CTRL_FLD_ENABLE		(1 << 0)

#define BCM8726_UD_2WIRE_CS		0x8001
#define BCM8726_UD_2WIRE_BYTE_CNT	0x8002

#define BCM8726_UD_2WIRE_BYTE_CNT_CHP_ADDR_START    9

#define BCM8726_UD_2WIRE_BYTE_CNT_FLD_CHP_ADDR	(0x7F << 9)
#define BCM8726_UD_2WIRE_BYTE_CNT_FLD_CHP_DATA	(0x1FF << 0)

#define BCM8726_UD_2WIRE_MEM_ADDR	0x8003
#define BCM8726_UD_2WIRE_MEM_ADDR_FLD_SCL_SEL	(1 << 14)	
#define BCM8726_UD_2WIRE_MEM_ADDR_FLD_ADDR	(0xFF << 0)	

#define BCM8726_UD_2WIRE_MDIO_ADDR	0x8004
#define BCM8726_BSC_EEPROM_REG_BASE	0x8007
#define BCM8726_BSC_EEPROM_REG_COUNT	0x100

#define BCM8726_UD_RX_ALARM		0x9000
#define BCM8726_UD_RX_ALARM_FLD_RX_PWR_FLT_ENA		(1 << 5)
#define BCM8726_UD_RX_ALARM_FLD_PMD_LCL_FLT_ENA		(1 << 4)
#define BCM8726_UD_RX_ALARM_FLD_PCS_FLT_ENA		(1 << 3)
#define BCM8726_UD_RX_ALARM_FLD_RX_ALARM_FLAG		(1 << 1)
#define BCM8726_UD_RX_ALARM_FLD_XS_FLT_ENA		(1 << 0)

#define BCM8726_UD_TX_ALARM		0x9001
#define BCM8726_UD_TX_ALARM_FLD_LASER_BIAS_FLT_ENA	(1 << 9)
#define BCM8726_UD_TX_ALARM_FLD_LASER_TEMP_FLT_ENA	(1 << 8)
#define BCM8726_UD_TX_ALARM_FLD_LASER_PWR_FLT_ENA	(1 << 7)
#define BCM8726_UD_TX_ALARM_FLD_TX_FLT_ENA	(1 << 6)
#define BCM8726_UD_TX_ALARM_FLD_PMA_TX_FLT_ENA	(1 << 4)
#define BCM8726_UD_TX_ALARM_FLD_PCS_TX_FLT_ENA	(1 << 3)
#define BCM8726_UD_TX_ALARM_FLD_XS_TX_FLT_ENA	(1 << 0)

#define BCM8726_UD_LASI_CTRL	0x9002
#define BCM8726_UD_LASI_CTRL_FLD_RX_ALARM_ENA		(1 << 2)
#define BCM8726_UD_LASI_CTRL_FLD_TX_ALARM_ENA		(1 << 1)
#define BCM8726_UD_LASI_CTRL_FLD_LS_ALARM_ENA		(1 << 0)

#define BCM8726_UD_RX_ALARM_STATUS	0x9003
#define BCM8726_UD_RX_ALARM_STATUS_FLD_RX_OPT_FLT	(1 << 5)
#define BCM8726_UD_RX_ALARM_STATUS_FLD_PMD_FLT		(1 << 4)
#define BCM8726_UD_RX_ALARM_STATUS_FLD_PCSR_FLT	(1 << 3)
#define BCM8726_UD_RX_ALARM_STATUS_FLD_RX_ALARM_FLAG	(1 << 1)
#define BCM8726_UD_RX_ALARM_STATUS_FLD_XS_R_FLT	(1 << 0)

#define BCM8726_UD_TX_ALARM_STATUS	0x9004
#define BCM8726_UD_TX_ALARM_STATUS_FLD_LASER_BIAS_FLT	(1 << 9)
#define BCM8726_UD_TX_ALARM_STATUS_FLD_LASER_TEMP_FLT	(1 << 8)
#define BCM8726_UD_TX_ALARM_STATUS_FLD_LASER_PWR_FLT	(1 << 7)
#define BCM8726_UD_TX_ALARM_STATUS_FLD_TX_FLT	(1 << 6)
#define BCM8726_UD_TX_ALARM_STATUS_FLD_PMA_TX_FLT	(1 << 4)
#define BCM8726_UD_TX_ALARM_STATUS_FLD_PCS_TX_FLT	(1 << 3)
#define BCM8726_UD_TX_ALARM_STATUS_FLD_TX_ALARM_FLAG	(1 << 1)
#define BCM8726_UD_TX_ALARM_STATUS_FLD_XS_TX_FLT	(1 << 0)

#define BCM8726_UD_LASI_STATUS	0x9005
#define BCM8726_UD_TX_FLAG_CTRL	0x9006
#define BCM8726_UD_RX_FLAG_CTRL	0x9007

#define BCM8726_UD_PHY_ID		0xC800
#define BCM8726_UD_CHIP_REV		0xC801
#define BCM8726_UD_CHIP_ID		0xC802
#define BCM8726_UD_PHY_STATUS		0xC804

#define BCM8726_UD_PMD_DIGITAL_CTRL	0xC808
#define BCM8726_UD_PMD_DIGITAL_CTRL_FLD_FAULT_MODE	(1 << 15)
#define BCM8726_UD_PMD_DIGITAL_CTRL_FLD_TX_PWR_DOWN	(1 << 14)
#define BCM8726_UD_PMD_DIGITAL_CTRL_FLD_RX_PWR_DOWN	(1 << 13)
#define BCM8726_UD_PMD_DIGITAL_CTRL_FLD_EXT_FLT_EN	(1 << 12)
#define BCM8726_UD_PMD_DIGITAL_CTRL_FLD_OPT_RST		(1 << 11)
#define BCM8726_UD_PMD_DIGITAL_CTRL_FLD_PCS_TX_INV_B	(1 << 10)
#define BCM8726_UD_PMD_DIGITAL_CTRL_FLD_PCS_RX_INV	(1 << 9)
#define BCM8726_UD_PMD_DIGITAL_CTRL_FLD_GPIO_SEL	(7 << 4)
#define BCM8726_UD_PMD_DIGITAL_CTRL_FLD_LPBK_ERR_DIS	(1 << 3)
#define BCM8726_UD_PMD_DIGITAL_CTRL_FLD_TX_ON_OFF_PWDWN_DIS	(1 << 0)

#define BCM8726_UD_PMD_DIGITAL_STATUS	0xC809
#define BCM8726_UD_TEST_CTRL		0xC80A
#define BCM8726_UD_TEST_CTRL_FLD_TX_PRBS7_EN	(1 << 12)
#define BCM8726_UD_TEST_CTRL_FLD_TX_SCRB_DIS	(1 << 11)
#define BCM8726_UD_TEST_CTRL_FLD_RX_DESCRB_DIS	(1 << 10)
#define BCM8726_UD_TEST_CTRL_FLD_TX_GBOX_RD_ON	(1 << 9)
#define BCM8726_UD_TEST_CTRL_FLD_RX_GBOX_RD_ON	(1 << 8)
#define BCM8726_UD_TEST_CTRL_FLD_DIG_LPBK	(1 << 5)
#define BCM8726_UD_BIST_CTRL		0xC80B
#define BCM8726_UD_BIST_CTRL_FLD_PBIST_ON			(1 << 15)
#define BCM8726_UD_BIST_CTRL_FLD_XBIST_ON			(1 << 14)
#define BCM8726_UD_BIST_CTRL_FLD_R64_JITT_PTRN_DETECT		(1 << 13)
#define BCM8726_UD_BIST_CTRL_FLD_TX_GEN_PAT_ENA			(1 << 11)
#define BCM8726_UD_BIST_CTRL_FLD_TX_6SQ_WAVE_PATTERN		(1 << 10)
#define BCM8726_UD_BIST_CTRL_FLD_XGXS_TX_BIST_LANE_CHECK	(1 << 9)
#define BCM8726_UD_BIST_CTRL_FLD_XGXS_RX_BIST_LANE_CHECK	(1 << 8)
#define BCM8726_UD_BIST_CTRL_FLD_PMD_TX_BIST_ENA		(1 << 7)
#define BCM8726_UD_BIST_CTRL_FLD_PMD_RX_BIST_ENA		(1 << 6)
#define BCM8726_UD_BIST_CTRL_FLD_BIST_INJECT_ERROR		(1 << 5)
#define BCM8726_UD_BIST_CTRL_FLD_BIST_VTYPE			(3 << 3)
#define BCM8726_UD_BIST_CTRL_FLD_BIST_RESULT_SEL		(1 << 1)
#define BCM8726_UD_BIST_CTRL_FLD_BIST_IPG_SEL			(1 << 0)


#define BCM8726_UD_TX_PATTERN_0		0xC80C
#define BCM8726_UD_TX_PATTERN_1		0xC80D

#define BCM8726_UD_GPIO_CTRL		0xC80E
#define BCM8726_UD_GPIO_CTRL_FLD_IN	(3 << 8)
#define BCM8726_UD_GPIO_CTRL_FLD_OUT_DS	(1 << 4)
#define BCM8726_UD_GPIO_CTRL_FLD_OUT	(3)

#define BCM8726_UD_PRBS31_WIN_0		0xC80F
#define BCM8726_UD_PRBS31_WIN_1		0xC810
#define BCM8726_UD_PRBS31_WIN_2		0xC811
#define BCM8726_UD_BIST_PKT_SIZE	0xC817
#define BCM8726_UD_BIST_TX_CNT_CTRL	0xC818
#define BCM8726_UD_XGXS_BIST_ERROR	0xC81A
#define BCM8726_UD_PCS_BIST_ERROR	0xC81B
#define BCM8726_UD_PMD_TX_CTRL		0xCA00

// EDC Core Control Status Registers

#define BCM8726_EDC_CTRL_STATUS		0xCA10
#define BCM8726_EDC_CTRL_STATUS_FLD_UC_BPASS	(1 << 12)
#define BCM8726_EDC_CTRL_STATUS_FLD_SEQ_DEF_BYPASS	(1 << 11)
#define BCM8726_EDC_CTRL_STATUS_FLD_EQ_BYPASS	(1 << 10)
#define BCM8726_EDC_CTRL_STATUS_FLD_PGA_CLK_SEL	(1 << 9)
#define BCM8726_EDC_CTRL_STATUS_FLD_UC_REF_CLK_SEL	(1 << 8)
#define BCM8726_EDC_CTRL_STATUS_FLD_UC_CLK_SEL	(3 << 6)
#define BCM8726_EDC_CTRL_STATUS_FLD_UC_CLK_SEL_2	(2 << 6)
#define BCM8726_EDC_CTRL_STATUS_FLD_UC_DEBUF	(3 << 4)
#define BCM8726_EDC_CTRL_STATUS_FLD_UC_REBOOT	(1 << 3)
#define BCM8726_EDC_CTRL_STATUS_FLD_UC_RESET	(1 << 2)
#define BCM8726_EDC_CTRL_STATUS_FLD_SOFT_RESET	(1 << 1)
#define BCM8726_EDC_CTRL_STATUS_FLD_GLOBAL_RESET	(1 << 0)

#define BCM8726_EDC_MESSAGE		0xCA11
#define BCM8726_EDC_GP_DATA		0xCA19
#define BCM8726_EDC_REVISION		0xCA1A

#define BCM8727_EDC_REV_B0		0x0406
#define BCM8727_EDC_REV_B		0x040B
#define BCM8727_EDC_REV_C		0x0501
#define BCM8727_EDC_REV_C_EXT		0x050D
#define BCM8727_EDC_REV_C1		0x0511

#define BCM8727_EEPROM_CHKSUM_EXT	0x600D
#define BCM8727_EEPROM_CHKSUM_INT	0x0000

#define BCM8726_EDC_GP_LOAD		0xCA1C

#define BCM8726_EEPROM_LOAD		0xCA85


////////////////////////////////////////////////////////////////////////
// PCS Address Space.
////////////////////////////////////////////////////////////////////////
#define BCM8726_PRBS23_WIN_2		0xC92D

#define BCM8726_PCS_CTRL 0x00
#define BCM8726_PCS_CTRL_FLD_RESET		(1<<15)	
#define BCM8726_PCS_CTRL_FLD_LOOPBACK	(1<<14)
#define BCM8726_PCS_CTRL_FLD_SPEED_SELECT1	(1<<13)
#define BCM8726_PCS_CTRL_FLD_LOW_POWER	(1<<11)
#define BCM8726_PCS_CTRL_FLD_SPEED_SELECT0	(1<<6)
#define BCM8726_PCS_CTRL_FLD_SPEED_10GBS	(0xF<<2)
#define BCM8726_PCS_CTRL_FLD_PMD_LPBK	(1<<0)

#define BCM8726_PCS_STATUS 0x01
#define BCM8726_PCS_STATUS_FLD_LOCAL_FAULT		(1<<7)
#define BCM8726_PCS_STATUS_FLD_RX_LINK_STATUS		(1<<2)
#define BCM8726_PCS_STATUS_FLD_LOW_POWER_ABILITY	(1<<1)

#define BCM8726_PCS_DEV_ID0		0x02
#define BCM8726_PCS_DEV_ID0_DFLT	0x0020
#define BCM8726_PCS_DEV_ID1		0x03
#define BCM8726_PCS_DEV_ID1_DFLT	0x6035

#define BCM8726_PCS_SPEED_ABILITY	0x04

#define BCM8726_PCS_PKG1_DEVICES        0x05
#define BCM8726_PCS_PKG1_DEVICES_FLD_DTE_XS_PRESENT                     (1<<5)
#define BCM8726_PCS_PKG1_DEVICES_FLD_PHY_XS_PRESENT                     (1<<4)
#define BCM8726_PCS_PKG1_DEVICES_FLD_PCS_PRESENT                        (1<<3)
#define BCM8726_PCS_PKG1_DEVICES_FLD_WIS_PRESENT                        (1<<2)
#define BCM8726_PCS_PKG1_DEVICES_FLD_PMD_PRESENT                        (1<<1)
#define BCM8726_PCS_PKG1_DEVICES_FLD_CLAUSE22_PRESENT           (1<<0)

#define BCM8726_PCS_PKG2_DEVICES	0x06
#define BCM8726_PCS_PKG2_DEVICES_FLD_VENDOR_DEV0_PRESENT		(1<<15)
#define BCM8726_PCS_PKG2_DEVICES_FLD_VENDOR_DEV1_PRESENT		(1<<14)

#define BCM8726_PCS_CTRL2		0x07
#define BCM8726_PCS_STATUS2		0x08
#define BCM8726_PCS_STATUS2_FLD_DEVICE_PRESENT			(3 << 14)
#define BCM8726_PCS_STATUS2_FLD_TX_LOCAL_FAULT			(1 << 11)
#define BCM8726_PCS_STATUS2_FLD_RX_LOCAL_FAULT			(1 << 10)
#define BCM8726_PCS_STATUS2_FLD_10BASE_W_CAPABLE		(1 << 2)
#define BCM8726_PCS_STATUS2_FLD_10BASE_X_CAPABLE		(1 << 1)
#define BCM8726_PCS_STATUS2_FLD_10BASE_R_CAPABLE		(1 << 0)

#define BCM8726_PCS_OUI_0	0x0E
#define BCM8726_PCS_OUI_1	0x0F

#define BCM8726_PCS_10BASE_X_STATUS	0x18
#define BCM8726_PCS_10BASE_X_STATUS_FLD_LANE_ALIGNMENT_STATUS		(1 << 12)
#define BCM8726_PCS_10BASE_X_STATUS_FLD_LANE_D_SYNC			(1 << 3)
#define BCM8726_PCS_10BASE_X_STATUS_FLD_LANE_C_SYNC			(1 << 2)
#define BCM8726_PCS_10BASE_X_STATUS_FLD_LANE_B_SYNC			(1 << 1)
#define BCM8726_PCS_10BASE_X_STATUS_FLD_LANE_A_SYNC			(1 << 0)

#define BCM8726_PCS_10BASE_R_STATUS	0x20
#define BCM8726_PCS_10BASE_R_STATUS_FLD_RX_LINK_STATUS		(1 << 12)
#define BCM8726_PCS_10BASE_R_STATUS_FLD_PRBS31_ABLE		(1 << 2)
#define BCM8726_PCS_10BASE_R_STATUS_FLD_HI_BER			(1 << 1)
#define BCM8726_PCS_10BASE_R_STATUS_FLD_BLOCK_LOCK		(1 << 0)

#define BCM8726_PCS_10BASE_R_STATUS2	0x21
#define BCM8726_PCS_10BASE_R_STATUS2_FLD_LATCH_BLK_LOCK		(1 << 15)
#define BCM8726_PCS_10BASE_R_STATUS2_FLD_LATCH_HI_BER		(1 << 14)
#define BCM8726_PCS_10BASE_R_STATUS2_FLD_BER		(0x3F << 8)
#define BCM8726_PCS_10BASE_R_STATUS2_FLD_ERR_BLK_CNT	(0xFF << 0)

#define BCM8726_PCS_10BASE_R_JITTER_TEST_SPEED_A0	0x22
#define BCM8726_PCS_10BASE_R_JITTER_TEST_SPEED_A1	0x23
#define BCM8726_PCS_10BASE_R_JITTER_TEST_SPEED_A2	0x24
#define BCM8726_PCS_10BASE_R_JITTER_TEST_SPEED_A3	0x25

#define BCM8726_PCS_10BASE_R_JITTER_TEST_SPEED_B0	0x26
#define BCM8726_PCS_10BASE_R_JITTER_TEST_SPEED_B1	0x27
#define BCM8726_PCS_10BASE_R_JITTER_TEST_SPEED_B2	0x28
#define BCM8726_PCS_10BASE_R_JITTER_TEST_SPEED_B3	0x29

#define BCM8726_PCS_10BASE_R_JITTER_TEST_CONTROL	0x2A
#define BCM8726_PCS_10BASE_R_JITTER_TEST_CONTROL_FLD_PRBS31_CHK_ENA		(1 << 5)
#define BCM8726_PCS_10BASE_R_JITTER_TEST_CONTROL_FLD_PRBS31_TX_ENA		(1 << 4)
#define BCM8726_PCS_10BASE_R_JITTER_TEST_CONTROL_FLD_JITTER_TX_ENA		(1 << 3)
#define BCM8726_PCS_10BASE_R_JITTER_TEST_CONTROL_FLD_JITTER_RX_PATTERN	(1 << 2)
#define BCM8726_PCS_10BASE_R_JITTER_TEST_CONTROL_FLD_JITTER_TEST_PATTERN	(1 << 1)
#define BCM8726_PCS_10BASE_R_JITTER_TEST_CONTROL_FLD_JITTER_DATA_PATTERN	(1 << 0)

#define BCM8726_PCS_10BASE_R_JITTER_TEST_ERROR_COUNTER	0x2B


////////////////////////////////////////////////////////////////////////
// PHY XS Register definitions
////////////////////////////////////////////////////////////////////////

#define BCM8726_PHY_XS_CTRL		0x00
#define BCM8726_PHY_XS_CTRL_FLD_RESET              (1<<15)
#define BCM8726_PHY_XS_CTRL_FLD_LOOPBACK   (1<<14)
#define BCM8726_PHY_XS_CTRL_FLD_SPEED_SELECT1      (1<<13)
#define BCM8726_PHY_XS_CTRL_FLD_LOW_POWER  (1<<11)
#define BCM8726_PHY_XS_CTRL_FLD_SPEED_SELECT0      (1<<6)
#define BCM8726_PHY_XS_CTRL_FLD_SPEED_10GBS        (0xF<<2)
#define BCM8726_PHY_XS_CTRL_FLD_PMD_LPBK   (1<<0)

#define BCM8726_PHY_XS_STATUS 0x01
#define BCM8726_PHY_XS_STATUS_FLD_LOCAL_FAULT              (1<<7)
#define BCM8726_PHY_XS_STATUS_FLD_TX_LINK_STATUS           (1<<2)
#define BCM8726_PHY_XS_STATUS_FLD_LOW_POWER_ABILITY        (1<<1)

#define BCM8726_PHY_XS_DEV_ID0		0x02
#define BCM8726_PHY_XS_DEV_ID0_DFLT	0x143
#define BCM8726_PHY_XS_DEV_ID1		0x03
#define BCM8726_PHY_XS_DEV_ID1_DFLT	0xBDC1
#define BCM8726_PHY_XS_SPEED_ABILITY	0x04


#define BCM8726_PHY_XS_PKG1_DEVICES        0x05
#define BCM8726_PHY_XS_PKG1_DEVICES_FLD_DTE_XS_PRESENT                     (1<<5)
#define BCM8726_PHY_XS_PKG1_DEVICES_FLD_PHY_XS_PRESENT                     (1<<4)
#define BCM8726_PHY_XS_PKG1_DEVICES_FLD_PCS_PRESENT                        (1<<3)
#define BCM8726_PHY_XS_PKG1_DEVICES_FLD_WIS_PRESENT                        (1<<2)
#define BCM8726_PHY_XS_PKG1_DEVICES_FLD_PMD_PRESENT                        (1<<1)
#define BCM8726_PHY_XS_PKG1_DEVICES_FLD_CLAUSE22_PRESENT           (1<<0)

#define BCM8726_PHY_XS_PKG2_DEVICES        0x06
#define BCM8726_PHY_XS_PKG2_DEVICES_FLD_VENDOR_DEV0_PRESENT                (1<<15)

#define BCM8726_PHY_XS_STATUS2              0x08
#define BCM8726_PHY_XS_STATUS2_FLD_DEVICE_PRESENT                   (3 << 14)
#define BCM8726_PHY_XS_STATUS2_FLD_TX_LOCAL_FAULT                   (1 << 11)
#define BCM8726_PHY_XS_STATUS2_FLD_RX_LOCAL_FAULT                   (1 << 10)

#define BCM8726_PHY_XS_OUI_0	0x0E
#define BCM8726_PHY_XS_OUI_1	0x0F

#define BCM8726_PHY_XS_XGXS_LANE_STATUS     0x18
#define BCM8726_PHY_XS_XGXS_LANE_STATUS_FLD_LANE_ALIGNMENT_STATUS           (1 << 12)
#define BCM8726_PHY_XS_XGXS_LANE_STATUS_FLD_PATTERN_TESTING_ABILITY         (1 << 11)
#define BCM8726_PHY_XS_XGXS_LANE_STATUS_FLD_LANE_D_SYNC                     (1 << 3)
#define BCM8726_PHY_XS_XGXS_LANE_STATUS_FLD_LANE_C_SYNC                     (1 << 2)
#define BCM8726_PHY_XS_XGXS_LANE_STATUS_FLD_LANE_B_SYNC                     (1 << 1)
#define BCM8726_PHY_XS_XGXS_LANE_STATUS_FLD_LANE_A_SYNC                     (1 << 0)

#define BCM8726_PHY_XS_XGXS_TEST_CONTROL     0x19
#define BCM8726_PHY_XS_XGXS_TEST_CONTROL_FLD_MD_PAT_EN     (1 << 2)
#define BCM8726_PHY_XS_XGXS_TEST_CONTROL_FLD_MD_PTYPE      (3 << 0)


// User Defined registers 4

#define BCM8726_XGXS_MODE_CTRL		0x8000
#define BCM8726_XGXS_MODE_CTRL_FLD_PGEN_ENA	(1 << 15)
#define BCM8726_XGXS_MODE_CTRL_FLD_PCMP_ENA	(1 << 14)
#define BCM8726_XGXS_MODE_CTRL_FLD_START_SEQ	(1 << 13)
#define BCM8726_XGXS_MODE_CTRL_FLD_RESET_ANALOG	(1 << 12)
#define BCM8726_XGXS_MODE_CTRL_FLD_MODE_MASK	(0xF << 8)
#define BCM8726_XGXS_MODE_CTRL_FLD_MODE_LPBK	(0x0 << 8)
#define BCM8726_XGXS_MODE_CTRL_FLD_MODE_FUNC	(0x1 << 8)
#define BCM8726_XGXS_MODE_CTRL_FLD_MODE_INDEPENDANT_LANE	(0x6 << 8)
#define BCM8726_XGXS_MODE_CTRL_FLD_PLL_BYPASS	(1 << 7)
#define BCM8726_XGXS_MODE_CTRL_FLD_RLOOP	(1 << 6)
#define BCM8726_XGXS_MODE_CTRL_FLD_CDET_ENA	(1 << 3)
#define BCM8726_XGXS_MODE_CTRL_FLD_EDEN		(1 << 2)
#define BCM8726_XGXS_MODE_CTRL_FLD_AFRST_EN	(1 << 1)
#define BCM8726_XGXS_MODE_CTRL_FLD_TXCKI_DIV	(1 << 0)

#define BCM8726_XGXS_STATUS		0x8001
#define BCM8726_XGXS_LANE_CTRL		0x8017
#define BCM8726_XGXS_LANE_CTRL_FLD_LANE_LPBK_MASK	(0xF << 4)
#define BCM8726_XGXS_TX_RAM		0x8020
#define BCM8726_XGXS_TX_RAM_DATA_LO	0x8021
#define BCM8726_XGXS_TX_RAM_DATA_HI	0x8022
#define BCM8726_XGXS_TX_POLY		0x8023
#define BCM8726_XGXS_TX_POLY_SPEED	0x8024
#define BCM8726_XGXS_TX_BYTE_CNT_HI	0x8026
#define BCM8726_XGXS_TX_BYTE_CNT_LO	0x8027
#define BCM8726_XGXS_TX_PKT_CNT_HI	0x8028
#define BCM8726_XGXS_TX_PKT_CNT_LO	0x8029
#define BCM8726_XGXS_RX_RAM		0x8030

#define BCM8726_XGXS_RX_RAM_DATA_LO	0x8031
#define BCM8726_XGXS_RX_RAM_DATA_HI	0x8032
#define BCM8726_XGXS_RX_POLY		0x8033
#define BCM8726_XGXS_RX_POLY_SPEED	0x8034
#define BCM8726_XGXS_RX_BYTE_CNT_HI	0x8036
#define BCM8726_XGXS_RX_BYTE_CNT_LO	0x8037
#define BCM8726_XGXS_RX_PKT_CNT_HI	0x8038
#define BCM8726_XGXS_RX_PKT_CNT_LO	0x8039

#define BCM8726_XGXS_RX_BIT_ERR_CNT	0x803A
#define BCM8726_XGXS_RX_PKT_ERR_CNT	0x803B
#define BCM8726_XGXS_RX_BYTE_ERR_CNT	0x803C
#define BCM8726_XGXS_RX_STATUS_BITS	0x803D

#define BCM8726_XGXS_ALL_RAM_ADDR		0x8040
#define BCM8726_XGXS_ALL_RAM_RAM_DATA_LO	0x8041
#define BCM8726_XGXS_ALL_RAM_RAM_DATA_HI	0x8042
#define BCM8726_XGXS_ALL_RAM_POLY		0x8043
#define BCM8726_XGXS_ALL_RAM_POLY_SPEED		0x8044
#define BCM8726_XGXS_ALL_RAM_BYTE_CNT_HI	0x8046
#define BCM8726_XGXS_ALL_RAM_BYTE_CNT_LO	0x8047
#define BCM8726_XGXS_ALL_RAM_PKT_CNT_HI		0x8048
#define BCM8726_XGXS_ALL_RAM_PKT_CNT_LO		0x8049
#define BCM8726_XGXS_ALL_RAM_BIT_ERR_CNT	0x804A
#define BCM8726_XGXS_ALL_RAM_PKT_ERR_CNT	0x804B
#define BCM8726_XGXS_ALL_RAM_BYTE_ERR_CNT	0x804C
#define BCM8726_XGXS_ALL_RAM_STATUS_BITS	0x804D

#define BCM8726_TX0_ACONTROL_0		0x8061
#define BCM8726_TX0_ACONTROL_3		0x8067
#define BCM8726_TX1_ACONTROL_0		0x8071
#define BCM8726_TX1_ACONTROL_3		0x8077
#define BCM8726_TX2_ACONTROL_0		0x8081
#define BCM8726_TX2_ACONTROL_3		0x8087
#define BCM8726_TX3_ACONTROL_0		0x8091
#define BCM8726_TX3_ACONTROL_3		0x8097
#define BCM8726_TXA_ACONTROL_0		0x80A1
#define BCM8726_TXA_ACONTROL_3		0x80A7

#define BCM8726_RX3_STATUS		0x80B0
#define BCM8726_RX3_CONTROL		0x80B1
#define BCM8726_RX3_CONTROL1		0x80B6
#define BCM8726_RX2_STATUS		0x80C0
#define BCM8726_RX2_CONTROL		0x80C1
#define BCM8726_RX2_CONTROL1		0x80C6
#define BCM8726_RX1_STATUS		0x80D0
#define BCM8726_RX1_CONTROL		0x80D1
#define BCM8726_RX1_CONTROL1		0x80D6
#define BCM8726_RX0_STATUS		0x80E0
#define BCM8726_RX0_CONTROL		0x80E1
#define BCM8726_RX0_CONTROL1		0x80E6
#define BCM8726_RXA_STATUS		0x80F0
#define BCM8726_RXA_CONTROL		0x80F1
#define BCM8726_RXA_CONTROL1		0x80F6

#define BCM8726_RXA_LANE_CTRL		0x8100
#define BCM8726_RXA_LANE_CTRL_FLD_SWAP	(1 << 15)
#define BCM8726_TXA_LANE_CTRL		0x8101
#define BCM8726_TXA_LANE_CTRL_FLD_SWAP	(1 << 15)