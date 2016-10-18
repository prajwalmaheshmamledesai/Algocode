#ifndef _BCM54X80_REG_H_
#define _BCM54X80_REG_H_

#define BCM54X80_MII_CONTROL		0x00
#define BCM54X80_MII_CONTROL_FLD_RESET		15
#define BCM54X80_MII_CONTROL_FLD_LOOPBACK	14
#define BCM54X80_MII_CONTROL_FLD_SPEED_SEL	13
#define BCM54X80_MII_CONTROL_FLD_CU_ANET_ENA	12
#define BCM54X80_MII_CONTROL_FLD_POWER_DOWN	11
#define BCM54X80_MII_CONTROL_FLD_RESTART_ANEG	9
#define BCM54X80_MII_CONTROL_FLD_DUPLEX_MODE	8
#define BCM54X80_MII_CONTROL_FLD_SPEED_SEL_MSB	6

#define BCM54X80_SPEED_SEL_100_LSB		(1 << 13)
#define BCM54X80_SPEED_SEL_1000_MSB		(1 << 6)

#define BCM54X80_MII_STATUS		0x01
#define BCM54X80_MII_STATUS_FLD_100BT4_ABLE	15
#define BCM54X80_MII_STATUS_FLD_100BX_FD_ABLE	14
#define BCM54X80_MII_STATUS_FLD_100BX_HD_ABLE	13
#define BCM54X80_MII_STATUS_FLD_100BT_FD_ABLE	12
#define BCM54X80_MII_STATUS_FLD_100BT_HD_ABLE	11
#define BCM54X80_MII_STATUS_FLD_100BT2_FD_ABLE	10
#define BCM54X80_MII_STATUS_FLD_100BT2_HD_ABLE	9
#define BCM54X80_MII_STATUS_FLD_EXT_STATUS	8
#define BCM54X80_MII_STATUS_FLD_MGMT_PAMB_SUPR	6
#define BCM54X80_MII_STATUS_FLD_ANEG_COMPLETE	5
#define BCM54X80_MII_STATUS_FLD_REMOTE_FAULT	4
#define BCM54X80_MII_STATUS_FLD_ANEG_ABILITY	3
#define BCM54X80_MII_STATUS_FLD_CU_LINK_STATUS	2
#define BCM54X80_MII_STATUS_FLD_JABBER_DETECT	1
#define BCM54X80_MII_STATUS_FLD_EXT_CAPABILITY	0


#define BCM54X80_PHY_ID_MSB			0x02
#define BCM54980_PHY_ID_MSB_DEFAULT		0x0143
#define BCM54680_PHY_ID_MSB_DEFAULT		0x0362

#define BCM54X80_PHY_ID_LSB			0x03
#define BCM54X80_PHY_ID_LSB_FLD_OUI_MASK        (0x3F << 10)
#define BCM54X80_PHY_ID_LSB_FLD_MODEL_MASK      (0x3F << 4)
#define BCM54X80_PHY_ID_LSB_FLD_REV_MASK        0x0F


#define BCM54X80_PHY_ID_LSB_FLD_OUI_E		15
#define BCM54X80_PHY_ID_LSB_FLD_OUI_S		10
#define BCM54X80_PHY_ID_LSB_FLD_MODEL_E		9
#define BCM54X80_PHY_ID_LSB_FLD_MODEL_S		4
#define BCM54X80_PHY_ID_LSB_FLD_REV_E		3	
#define BCM54X80_PHY_ID_LSB_FLD_REV_S		0	
#define BCM54980_PHY_ID_LSB_DEFAULT		0xBEB0
#define BCM54680_PHY_ID_LSB_DEFAULT		0x5DC1

#define BCM54X80_ANEG_ADV			0x04
#define BCM54X80_ANEG_ADV_FLD_NEXT_PAGE		15
#define BCM54X80_ANEG_ADV_FLD_REMOTE_FLT	13
#define BCM54X80_ANEG_ADV_FLD_ASYM_PAUSE	11
#define BCM54X80_ANEG_ADV_FLD_PAUSE_CAPABLE	10
#define BCM54X80_ANEG_ADV_FLD_100BT4_CAPABLE	9
#define BCM54X80_ANEG_ADV_FLD_100BTX_FD		8
#define BCM54X80_ANEG_ADV_FLD_100BTX_HD		7
#define BCM54X80_ANEG_ADV_FLD_100BT_FD		6
#define BCM54X80_ANEG_ADV_FLD_100BT_HD		5 
#define BCM54X80_ANEG_ADV_FLD_PROT_SEL		4
#define BCM54X80_ANEG_ADV_FLD_CLR_10_100	0

#define BCM54X80_ANEG_LP_ABILITY		0x05
#define BCM54X80_ANEG_LP_ABILITY_FLD_NEXT_PAGE		15
#define BCM54X80_ANEG_LP_ABILITY_FLD_ACK		14
#define BCM54X80_ANEG_LP_ABILITY_FLD_REMOTE_FLT		13
#define BCM54X80_ANEG_LP_ABILITY_FLD_ASYM_PAUSE		11
#define BCM54X80_ANEG_LP_ABILITY_FLD_PAUSE_CAPABLE	10
#define BCM54X80_ANEG_LP_ABILITY_FLD_100BT4_CAPABLE	9
#define BCM54X80_ANEG_LP_ABILITY_FLD_100BTX_FD		8
#define BCM54X80_ANEG_LP_ABILITY_FLD_100BTX_HD		7
#define BCM54X80_ANEG_LP_ABILITY_FLD_100BT_FD		6
#define BCM54X80_ANEG_LP_ABILITY_FLD_100BT_HD		5 
#define BCM54X80_ANEG_LP_ABILITY_FLD_PROT_SEL		4

#define BCM54X80_ANEG_EXPANSION			0x06
#define BCM54X80_ANEG_EXPANSION_FLD_NP_RL_ABLE	6
#define BCM54X80_ANEG_EXPANSION_FLD_NP_RX_LOC	5
#define BCM54X80_ANEG_EXPANSION_FLD_PAR_FLT	4
#define BCM54X80_ANEG_EXPANSION_FLD_LP_NP_ABLE	3
#define BCM54X80_ANEG_EXPANSION_FLD_NP_ABLE	2
#define BCM54X80_ANEG_EXPANSION_FLD_PAGE_RX	1
#define BCM54X80_ANEG_EXPANSION_FLD_LPANEG_ABLE 0

#define BCM54X80_NP_TRANSMIT			0x07
#define BCM54X80_NP_TRANSMIT_FLD_NEXT_PAGE	15
#define BCM54X80_NP_TRANSMIT_FLD_MESSAGE_PAGE	13
#define BCM54X80_NP_TRANSMIT_FLD_ACK2		12
#define BCM54X80_NP_TRANSMIT_FLD_TOGGLE		11
#define BCM54X80_NP_TRANSMIT_FLD_CODE_FIELD_MASK     0x7FF
#define BCM54X80_NP_TRANSMIT_FLD_CODE_FIELD_E	10
#define BCM54X80_NP_TRANSMIT_FLD_CODE_FIELD_S	0

#define BCM54X80_LP_RX_NP			0x08
#define BCM54X80_LP_RX_NP_FLD_NEXT_PAGE		15
#define BCM54X80_LP_RX_NP_FLD_ACK		14
#define BCM54X80_LP_RX_NP_FLD_MESSAGE_PAGE	13
#define BCM54X80_LP_RX_NP_FLD_ACK2		12
#define BCM54X80_LP_RX_NP_FLD_TOGGLE		11
#define BCM54X80_LP_RX_NP_FLD_CODE_FIELD_MASK   0x7FF
#define BCM54X80_LP_RX_NP_FLD_CODE_FIELD_E	10
#define BCM54X80_LP_RX_NP_FLD_CODE_FIELD_S	0


#define BCM54X80_1000BT_CONTROL			0x09
#define BCM54X80_1000BT_CONTROL_FLD_TEST_MODE_MASK   (7 << 13)
#define BCM54X80_1000BT_CONTROL_FLD_TEST_MODE_E	15
#define BCM54X80_1000BT_CONTROL_FLD_TEST_MODE_S	13
#define BCM54X80_1000BT_CONTROL_FLD_MS_CFG_ENA	12
#define BCM54X80_1000BT_CONTROL_FLD_MS_CFG_VAL	11
#define BCM54X80_1000BT_CONTROL_FLD_REPEATER	10
#define BCM54X80_1000BT_CONTROL_FLD_ADV_FD_ABLE	9
#define BCM54X80_1000BT_CONTROL_FLD_ADV_HD_ABLE	8

#define BCM54X80_1000BT_STATUS			0x0A
#define BCM54X80_1000BT_STATUS_FLD_MS_CFG_FLT	15
#define BCM54X80_1000BT_STATUS_FLD_MS_CFG_RES	14
#define BCM54X80_1000BT_STATUS_FLD_LCL_RX_STAT	13
#define BCM54X80_1000BT_STATUS_FLD_REM_RX_STAT	12
#define BCM54X80_1000BT_STATUS_FLD_LP_FD_ABLE	11
#define BCM54X80_1000BT_STATUS_FLD_LP_HD_ABLE	10
#define BCM54X80_1000BT_STATUS_FLD_IDLE_ERR_CNT_MASK 0xFF
#define BCM54X80_1000BT_STATUS_FLD_IDLE_ERR_CNT_E 7	
#define BCM54X80_1000BT_STATUS_FLD_IDLE_ERR_CNT_S 0	

#define BCM54X80_IEEE_EXT_STATUS		0x0F
#define BCM54X80_IEEE_EXT_STATUS_FLD_1000BX_FD	15
#define BCM54X80_IEEE_EXT_STATUS_FLD_1000BX_HD	14
#define BCM54X80_IEEE_EXT_STATUS_FLD_1000BT_FD	13
#define BCM54X80_IEEE_EXT_STATUS_FLD_1000BT_HD	12

#define BCM54X80_PHY_EXT_CONTROL		0x10
#define BCM54X80_PHY_EXT_CONTROL_FLD_AMDI_DIS	14
#define BCM54X80_PHY_EXT_CONTROL_FLD_TX_DIS	13
#define BCM54X80_PHY_EXT_CONTROL_FLD_IRQ_DIS	12
#define BCM54X80_PHY_EXT_CONTROL_FLD_IRQ_FORCE	11
#define BCM54X80_PHY_EXT_CONTROL_FLD_BYP_4B5B	10
#define BCM54X80_PHY_EXT_CONTROL_FLD_BYP_SCRAMB	9
#define BCM54X80_PHY_EXT_CONTROL_FLD_BYP_MLT3	8
#define BCM54X80_PHY_EXT_CONTROL_FLD_BYP_RXSYMB	7
#define BCM54X80_PHY_EXT_CONTROL_FLD_RST_SCRAMB	6
#define BCM54X80_PHY_EXT_CONTROL_FLD_LED_TRF_EN	5
#define BCM54X80_PHY_EXT_CONTROL_FLD_LED_ON	4
#define BCM54X80_PHY_EXT_CONTROL_FLD_LED_OFF	3

#define BCM54X80_PHY_EXT_STATUS		0x11
#define BCM54X80_PHY_EXT_STATUS_FLD_AN_BP_SEL	15
#define BCM54X80_PHY_EXT_STATUS_FLD_ETH_WIRESPD	14
#define BCM54X80_PHY_EXT_STATUS_FLD_MDI_STATE	13
#define BCM54X80_PHY_EXT_STATUS_FLD_IRQ_STATUS	12
#define BCM54X80_PHY_EXT_STATUS_FLD_REM_RX_STAT	11
#define BCM54X80_PHY_EXT_STATUS_FLD_LCL_RX_STAT	10
#define BCM54X80_PHY_EXT_STATUS_FLD_LOCKED	9
#define BCM54X80_PHY_EXT_STATUS_FLD_LINK_STATUS	8
#define BCM54X80_PHY_EXT_STATUS_FLD_CRC_ERROR	7
#define BCM54X80_PHY_EXT_STATUS_FLD_CARR_ERROR	6
#define BCM54X80_PHY_EXT_STATUS_FLD_BAD_SSD	5
#define BCM54X80_PHY_EXT_STATUS_FLD_BAD_ESD	4
#define BCM54X80_PHY_EXT_STATUS_FLD_RX_ERROR	3
#define BCM54X80_PHY_EXT_STATUS_FLD_TX_ERROR	2
#define BCM54X80_PHY_EXT_STATUS_FLD_LOCK_ERROR	1
#define BCM54X80_PHY_EXT_STATUS_FLD_MLT3_ERROR	0

#define BCM54X80_RX_ERR_CNT		0x12
#define BCM54X80_TX_ERROR_CODE_CNT	0x13	//Multi Definition
#define BCM54X80_RX_ERROR_CODE_CNT	0x14	//Multi Definition
#define BCM54X80_EXP_DATA_REG		0x15
#define BCM54X80_EXP_ACCESS_REG		0x17	
#define BCM54X80_EXP_REG_SELECT		(0x0F << 8)
#define BCM54X80_AUX_CONTROL		0x18	//Multi Definition

#define BCM54X80_AUX_STATUS			0x19
#define BCM54X80_AUX_STATUS_FLD_ANEG_COMPLETE	15
#define BCM54X80_AUX_STATUS_FLD_ANEG_COMP_ACK	14
#define BCM54X80_AUX_STATUS_FLD_ANEG_ACK	13
#define BCM54X80_AUX_STATUS_FLD_ANEG_ABLE	12
#define BCM54X80_AUX_STATUS_FLD_ANEG_HCD	11
#define BCM54X80_AUX_STATUS_FLD_CURR_SPEED_MASK (7 << 8)
#define BCM54X80_AUX_STATUS_FLD_CURR_SPEED_E	10
#define BCM54X80_AUX_STATUS_FLD_CURR_SPEED_S	8
#define BCM54X80_AUX_STATUS_FLD_PAR_DET_FLT	7
#define BCM54X80_AUX_STATUS_FLD_REM_FLT		6
#define BCM54X80_AUX_STATUS_FLD_ANEG_PAGE_RX	5
#define BCM54X80_AUX_STATUS_FLD_LP_ANEG_ABLE	4
#define BCM54X80_AUX_STATUS_FLD_LP_NP_ABLE	3
#define BCM54X80_AUX_STATUS_FLD_LINK_STATUS	2
#define BCM54X80_AUX_STATUS_FLD_PAUSE_RES_RX	1
#define BCM54X80_AUX_STATUS_FLD_PAUSE_RES_TX	0

#define BCM54X80_IRQ_STATUS			0x1A
#define BCM54X80_IRQ_STATUS_FLD_SIGNAL_DET	15
#define BCM54X80_IRQ_STATUS_FLD_ILLEGAL_PAIR	14
#define BCM54X80_IRQ_STATUS_FLD_MDIX_STAUS_CHG	13
#define BCM54X80_IRQ_STATUS_FLD_EXC_HI_CNT_THR	12
#define BCM54X80_IRQ_STATUS_FLD_EXC_LO_CNT_THR	11
#define BCM54X80_IRQ_STATUS_FLD_ANEG_PAGE_RX	10
#define BCM54X80_IRQ_STATUS_FLD_NO_HCD_LINK	9
#define BCM54X80_IRQ_STATUS_FLD_NO_HCD		8
#define BCM54X80_IRQ_STATUS_FLD_NEG_UNSUP_HCD	7
#define BCM54X80_IRQ_STATUS_FLD_SCRAMB_SYNC_ERR	6
#define BCM54X80_IRQ_STATUS_FLD_REM_RX_STAT_CHG	5
#define BCM54X80_IRQ_STATUS_FLD_LCL_RX_STAT_CHG	4
#define BCM54X80_IRQ_STATUS_FLD_DUP_MODE_CHG	3
#define BCM54X80_IRQ_STATUS_FLD_LINK_SPD_CHG	2
#define BCM54X80_IRQ_STATUS_FLD_LINK_STAT_CHG	1
#define BCM54X80_IRQ_STATUS_FLD_RX_CRC_ERR	0

#define BCM54X80_IRQ_MASK			0x1B
#define BCM54X80_IRQ_MASK_FLD_SIGNAL_DET	15
#define BCM54X80_IRQ_MASK_FLD_ILLEGAL_PAIR	14
#define BCM54X80_IRQ_MASK_FLD_MDIX_STAUS_CHG	13
#define BCM54X80_IRQ_MASK_FLD_EXC_HI_CNT_THR	12
#define BCM54X80_IRQ_MASK_FLD_EXC_LO_CNT_THR	11
#define BCM54X80_IRQ_MASK_FLD_ANEG_PAGE_RX	10
#define BCM54X80_IRQ_MASK_FLD_NO_HCD_LINK	9
#define BCM54X80_IRQ_MASK_FLD_NO_HCD		8
#define BCM54X80_IRQ_MASK_FLD_NEG_UNSUP_HCD	7
#define BCM54X80_IRQ_MASK_FLD_SCRAMB_SYNC_ERR	6
#define BCM54X80_IRQ_MASK_FLD_REM_RX_STAT_CHG	5
#define BCM54X80_IRQ_MASK_FLD_LCL_RX_STAT_CHG	4
#define BCM54X80_IRQ_MASK_FLD_DUP_MODE_CHG	3
#define BCM54X80_IRQ_MASK_FLD_LINK_SPD_CHG	2
#define BCM54X80_IRQ_MASK_FLD_LINK_STAT_CHG	1
#define BCM54X80_IRQ_MASK_FLD_RX_CRC_ERR	0

#define BCM54X80_SPARE_CONTROL		0x1C
#define BCM54X80_SPARE_CONTROL_FLD_WRITE	15
#define BCM54X80_SPARE_CONTROL_FLD_ADDR_E	14
#define BCM54X80_SPARE_CONTROL_FLD_ADDR_S	10
#define BCM54X80_SPARE_CONTROL_FLD_DATA_E	9
#define BCM54X80_SPARE_CONTROL_FLD_DATA_S	0

#define BCM54X80_MS_SPEED		0x1D	//Multi Definition

#define BCM54X80_TEST_REG1		0x1E
#define BCM54X80_TEST_REG1_FLD_CRC_ERR_CNT	15
#define BCM54X80_TEST_REG1_FLD_TX_ERR_CNT	14
#define BCM54X80_TEST_REG1_FLD_FORCE_LINK	12
#define BCM54X80_TEST_REG1_FLD_MDI_SWAP		7

#define BCM54X80_SGMII_CONTROL		0x00
#define BCM54X80_SGMII_CONTROL_FLD_RESET	15
#define BCM54X80_SGMII_CONTROL_FLD_ANEG_ENA	12
#define BCM54X80_SGMII_CONTROL_FLD_POWER_DOWN	11
#define BCM54X80_SGMII_CONTROL_FLD_ANEG_RESTART	9

#define BCM54X80_SGMII_STATUS		0x01
#define BCM54X80_SGMII_STATUS_FLD_ANEG_COMPLETE	5
#define BCM54X80_SGMII_STATUS_FLD_LINK_STATUS	2

#define BCM54X80_SGMII_ANEG_ADV		0x04
#define BCM54X80_SGMII_ANEG_ADV_FLD_CU_LINK	15
#define BCM54X80_SGMII_ANEG_ADV_FLD_CU_DUPLEX	12
#define BCM54X80_SGMII_ANEG_ADV_FLD_CU_SPEED_MASK    (3 << 10)
#define BCM54X80_SGMII_ANEG_ADV_FLD_CU_SPEED_E	11
#define BCM54X80_SGMII_ANEG_ADV_FLD_CU_SPEED_S	10
#define BCM54X80_SGMII_ANEG_ADV_FLD_SGMII_SEL	0

#define BCM54X80_SGMII_ANEG_LP_ABILITY	0x05
#define BCM54X80_SGMII_ANEG_LP_ABILITY_FLD_ACK	14
#define BCM54X80_SGMII_ANEG_LP_ABILITY_FLD_SEL	0

#define BCM54X80_EXP_RX_CNT			0x00
#define BCM54X80_EXP_LED_SEL			0x04
#define BCM54X80_EXP_LED_SEL_FLD_FLASH_NOW	9
#define BCM54X80_EXP_LED_SEL_FLD_IN_PHASE	8
#define BCM54X80_EXP_LED_SEL_FLD_LED_SEL2_MASK  (0xF << 4)
#define BCM54X80_EXP_LED_SEL_FLD_LED_SEL1_MASK  (0xF)
#define BCM54X80_EXP_LED_SEL_FLD_LED_SEL2_E	7
#define BCM54X80_EXP_LED_SEL_FLD_LED_SEL2_S	4
#define BCM54X80_EXP_LED_SEL_FLD_LED_SEL1_E	3
#define BCM54X80_EXP_LED_SEL_FLD_LED_SEL1_S	0

#define BCM54X80_EXP_LED_FLASH_RATE	0x05
#define BCM54X80_EXP_LED_FLASH_RATE_FLD_ALT_RATE_E	11
#define BCM54X80_EXP_LED_FLASH_RATE_FLD_ALT_RATE_S	6
#define BCM54X80_EXP_LED_FLASH_RATE_FLD_FLASH_RATE_E	5
#define BCM54X80_EXP_LED_FLASH_RATE_FLD_FLASH_RATE_S	0
#define BCM54X80_EXP_LED_FLASH_RATE_FLD_ALT_RATE_MASK   (0x3F << 6)
#define BCM54X80_EXP_LED_FLASH_RATE_FLD_FLASH_RATE_MASK 0x3F

#define BCM54X80_EXP_LED_BLINK_CONTROL	0x06
#define BCM54X80_EXP_LED_BLINK_CONTROL_FLD_BLINK_UP	5
#define BCM54X80_EXP_LED_BLINK_CONTROL_FLD_BLINK_RATE_E 4
#define BCM54X80_EXP_LED_BLINK_CONTROL_FLD_BLINK_RATE_S 0
#define BCM54X80_EXP_LED_BLINK_CONTROL_FLD_BLINK_RATE_MASK   (0x1F)


#define BCM54X80_EXP_100BFX_FAR_END_FLT	0x07
#define BCM54X80_EXP_100BFX_FAR_END_FLT_FLD_SD_SEL	2
#define BCM54X80_EXP_100BFX_FAR_END_FLT_FLD_FAULTING	1
#define BCM54X80_EXP_100BFX_FAR_END_FLT_FLD_FLT_ENA	0

#define BCM54X80_EXP_CHAN_SWAP			0x09
#define BCM54X80_EXP_CHAN_SWAP_FLD_CHAN3_MASK	(3 << 6)
#define BCM54X80_EXP_CHAN_SWAP_FLD_CHAN2_MASK	(3 << 4)
#define BCM54X80_EXP_CHAN_SWAP_FLD_CHAN1_MASK	(3 << 2)
#define BCM54X80_EXP_CHAN_SWAP_FLD_CHAN0_MASK	(3 << 0)
#define BCM54X80_EXP_CHAN_SWAP_FLD_CHAN3_E 	7	
#define BCM54X80_EXP_CHAN_SWAP_FLD_CHAN3_S	6	
#define BCM54X80_EXP_CHAN_SWAP_FLD_CHAN2_E 	5	
#define BCM54X80_EXP_CHAN_SWAP_FLD_CHAN2_S	4	
#define BCM54X80_EXP_CHAN_SWAP_FLD_CHAN1_E 	3	
#define BCM54X80_EXP_CHAN_SWAP_FLD_CHAN1_S	2	
#define BCM54X80_EXP_CHAN_SWAP_FLD_CHAN0_E 	1	
#define BCM54X80_EXP_CHAN_SWAP_FLD_CHAN0_S	0	

#define BCM54X80_EXP_PORT_IRQ_STATUS	0x0B
#define BCM54X80_EXP_DIAG_CTRL_STAT	0x10
#define BCM54X80_EXP_DIAG_CTRL_STAT_FLD_CHAN_FINISHED_MASK	 (0xF << 12)
#define BCM54X80_EXP_DIAG_CTRL_STAT_FLD_CHAN_FINISHED_E  15	
#define BCM54X80_EXP_DIAG_CTRL_STAT_FLD_CHAN_FINISHED_S  12	
#define BCM54X80_EXP_DIAG_CTRL_STAT_FLD_PAIRS_SHORTED	 11
#define BCM54X80_EXP_DIAG_CTRL_STAT_FLD_OPEN_SHORT_PAIR	 10
#define BCM54X80_EXP_DIAG_CTRL_STAT_FLD_ALL_FINISHED	 9
#define BCM54X80_EXP_DIAG_CTRL_STAT_FLD_PAIR_SHORT_ENA	 8
#define BCM54X80_EXP_DIAG_CTRL_STAT_FLD_TWO_PASS	 7
#define BCM54X80_EXP_DIAG_CTRL_STAT_FLD_LINK_CABLE_THR	 6
#define BCM54X80_EXP_DIAG_CTRL_STAT_FLD_PHASE_TUNE	 3
#define BCM54X80_EXP_DIAG_CTRL_STAT_FLD_NATURAL_LINK	 2
#define BCM54X80_EXP_DIAG_CTRL_STAT_FLD_CABLE_DIAG_ST	 1
#define BCM54X80_EXP_DIAG_CTRL_STAT_FLD_CABLE_DIAG_MODE	 0

#define BCM54X80_EXP_DIAG_RESULTS	0x11
#define BCM54X80_EXP_DIAG_RESULTS_FLD_PAIR3_MASK	(3 << 6)
#define BCM54X80_EXP_DIAG_RESULTS_FLD_PAIR2_MASK	(3 << 4)
#define BCM54X80_EXP_DIAG_RESULTS_FLD_PAIR1_MASK	(3 << 2)
#define BCM54X80_EXP_DIAG_RESULTS_FLD_PAIR0_MASK	(3 << 0)
#define BCM54X80_EXP_DIAG_RESULTS_FLD_PAIR3_E 	7	
#define BCM54X80_EXP_DIAG_RESULTS_FLD_PAIR3_S 	6	
#define BCM54X80_EXP_DIAG_RESULTS_FLD_PAIR2_E 	5	
#define BCM54X80_EXP_DIAG_RESULTS_FLD_PAIR2_S 	4	
#define BCM54X80_EXP_DIAG_RESULTS_FLD_PAIR1_E 	3	
#define BCM54X80_EXP_DIAG_RESULTS_FLD_PAIR1_S 	2	
#define BCM54X80_EXP_DIAG_RESULTS_FLD_PAIR0_E 	1	
#define BCM54X80_EXP_DIAG_RESULTS_FLD_PAIR0_S 	0	

#define BCM54X80_EXP_DIAG_LENGTH		0x12
#define BCM54X80_EXP_DIAG_LENGTH_FLD_PAIR1_MASK	(0xFF << 8)
#define BCM54X80_EXP_DIAG_LENGTH_FLD_PAIR1_E	15	
#define BCM54X80_EXP_DIAG_LENGTH_FLD_PAIR1_S	8
#define BCM54X80_EXP_DIAG_LENGTH_FLD_PAIR0_MASK	(0xFF)
#define BCM54X80_EXP_DIAG_LENGTH_FLD_PAIR0_E	7	
#define BCM54X80_EXP_DIAG_LENGTH_FLD_PAIR0_S	0	

#define BCM54X80_EXP_DIAG_LENGTH1		0x13
#define BCM54X80_EXP_DIAG_LENGTH1_FLD_PAIR3_MASK	(0xFF << 8)
#define BCM54X80_EXP_DIAG_LENGTH1_FLD_PAIR2_MASK	(0xFF)
#define BCM54X80_EXP_DIAG_LENGTH1_FLD_PAIR3_E	15	
#define BCM54X80_EXP_DIAG_LENGTH1_FLD_PAIR3_S	8	
#define BCM54X80_EXP_DIAG_LENGTH1_FLD_PAIR2_E	7	
#define BCM54X80_EXP_DIAG_LENGTH1_FLD_PAIR2_S	0	

#define BCM54X80_EXP_OPERATING_MODE	0x42
#define BCM54X80_EXP_OPERATING_MODE_FLD_SERDES_LINK	15
#define BCM54X80_EXP_OPERATING_MODE_FLD_SERDES_SPEED_MASK	(3 << 13)
#define BCM54X80_EXP_OPERATING_MODE_FLD_SERDES_SPEED_E	14
#define BCM54X80_EXP_OPERATING_MODE_FLD_SERDES_SPEED_S	13
#define BCM54X80_EXP_OPERATING_MODE_FLD_SERDES_DUPLEX	12
#define BCM54X80_EXP_OPERATING_MODE_FLD_COPPER_LINK	11
#define BCM54X80_EXP_OPERATING_MODE_FLD_COPPER_SPEED_MASK	(3 << 9)
#define BCM54X80_EXP_OPERATING_MODE_FLD_COPPER_SPEED_E	10	
#define BCM54X80_EXP_OPERATING_MODE_FLD_COPPER_SPEED_S	9
#define BCM54X80_EXP_OPERATING_MODE_FLD_COPPER_DUPLEX	8
#define BCM54X80_EXP_OPERATING_MODE_FLD_COPPER_ENERGY	7
#define BCM54X80_EXP_OPERATING_MODE_FLD_FIBER_SIGNAL	6
#define BCM54X80_EXP_OPERATING_MODE_FLD_SYNC_STATUS	5
#define BCM54X80_EXP_OPERATING_MODE_FLD_OPERATING_MODE_MASK	0x1F
#define BCM54X80_EXP_OPERATING_MODE_FLD_OPERATING_MODE_E 4
#define BCM54X80_EXP_OPERATING_MODE_FLD_OPERATING_MODE_S 0

#define BCM54X80_EXP_LINE_LPBK_CONTROL	0x44
#define BCM54X80_EXP_LINE_LPBK_CONTROL_FLD_LINE_LPBK	3
#define BCM54X80_EXP_LINE_LPBK_CONTROL_FLD_SGMII_LPBK	2
#define BCM54X80_EXP_LINE_LPBK_CONTROL_FLD_TX_SUPPRESS	1
#define BCM54X80_EXP_LINE_LPBK_CONTROL_FLD_RX_SUPPRESS	0

#define BCM54X80_EXP_SGOUT_CONTROL	0x51
#define BCM54X80_EXP_SCLK_CONTROL	0x52
#define BCM54X80_EXP_ANEG_RX_STATUS	0x67
#define BCM54X80_EXP_SOFT_RESET		0x70
#define BCM54X80_EXP_SERIAL_LED1	0x71
#define BCM54X80_EXP_SERIAL_LED2	0x72
#define BCM54X80_EXP_BSC_CONTROL	0x7B
#define BCM54X80_EXP_BSC_STATUS		0x7C


// Shadow Registers.
#define BCM54X80_SHADOW_WRITE		0x8000	
#define BCM54X80_SHADOW_SELECT(addr)	((addr & 0x1F) << 10)	

#define BCM54X80_SHADOW_SPARE_CTRL1	0x02
#define BCM54X80_SHADOW_SPARE_CTRL1_FLD_LINKSPD		2
#define BCM54X80_SHADOW_SPARE_CTRL2	0x04
#define BCM54X80_SHADOW_SPARE_CTRL3	0x05
#define BCM54X80_SHADOW_LED_STATUS	0x08
#define BCM54X80_SHADOW_LED_CONTROL	0x09
#define BCM54X80_SHADOW_LED_CONTROL_FLD_ACTIVITY_ENA	3
#define BCM54X80_SHADOW_LED_CONTROL_FLD_REMOTE_FAULT	2
#define BCM54X80_SHADOW_LED_CONTROL_FLD_TX_RX_SEL_E	1
#define BCM54X80_SHADOW_LED_CONTROL_FLD_TX_RX_SEL_S	0
#define BCM54X80_SHADOW_LED_CONTROL_TX_RX_SEL		3

#define BCM54X80_SHADOW_AUTO_PWRDWN	0x0A
#define BCM54X80_SHADOW_SLED_1		0x0B
#define BCM54X80_SHADOW_SLED_2		0x0C
#define BCM54X80_SHADOW_LED_SEL1	0x0D
#define BCM54X80_SHADOW_LED_SEL2	0x0E
#define BCM54X80_SHADOW_LED_GPIO	0x0F
#define BCM54X80_SHADOW_SGMII_STATUS	0x15
#define BCM54X80_SHADOW_MISC_CTRL2	0x16
#define BCM54X80_SHADOW_MEDIA_CONV	0x18
#define BCM54X80_SHADOW_AUX_CTRL	0x1B
#define BCM54X80_SHADOW_AUX_CTRL_FLD_CRC_ENA	(1)
#define BCM54X80_SHADOW_AUX_CTRL_FLD_FIFO_LSB	(1 << 1)
#define BCM54X80_SHADOW_AUX_CTRL_FLD_TRRR_DIS	(1 << 5)
#define BCM54X80_SHADOW_AUX_CTRL_FLD_CAREXT_DIS	(1 << 6)
#define BCM54X80_SHADOW_AUX_CTRL_FLD_FALSE_CARR	(1 << 7)
#define BCM54X80_SHADOW_AUX_CTRL_FLD_SERDES_CNT	(1 << 9)

#define BCM54X80_SHADOW_MODE_CTRL	0x1F
#define BCM54X80_SHADOW_MODE_CTRL_FLD_SGMII_ENA		(1 << 0)
#define BCM54X80_SHADOW_MODE_CTRL_FLD_SGMII_SEL		(2 << 1)
#define BCM54X80_SHADOW_MODE_CTRL_MEDIA_CONV		(3 << 1)

// AUX Shadow Registers
#define BCM54X80_AUX_SHADOW_CTRL	0x00
#define BCM54X80_AUX_SHADOW_CTRL_FLD_EXT_LPBK	15
#define BCM54X80_AUX_SHADOW_CTRL_FLD_RX_EXT_PKT	14
#define BCM54X80_AUX_SHADOW_CTRL_FLD_DSP_ENA	11

#define BCM54X80_AUX_SHADOW_10BT	0x01

#define BCM54X80_AUX_SHADOW_MII_CTRL	0x02
#define BCM54X80_AUX_SHADOW_MII_CTRL_FLD_SUPER_ISOLATE	(1 << 5)

#define BCM54X80_AUX_SHADOW_MISC_TEST	0x04
#define BCM54X80_AUX_MISC_TEST_FLD_LPBK_EN	15
#define BCM54X80_AUX_MISC_TEST_FLD_WAKE_UP	10
#define BCM54X80_AUX_MISC_TEST_FLD_SWAP_RX_MDIX	4
#define BCM54X80_AUX_MISC_TEST_FLD_HALF_OUT	3

#define BCM54X80_AUX_SHADOW_MISC_CTRL	0x07

#define BCM54X80_LED_MODE_LINKSPD1	0x00
#define BCM54X80_LED_MODE_LINKSPD2	0x01
#define BCM54X80_LED_MODE_XMITLED	0x02
#define BCM54X80_LED_MODE_ACTIVITY	0x03
#define BCM54X80_LED_MODE_FDXLED	0x04
#define BCM54X80_LED_MODE_SLAVE		0x05
#define BCM54X80_LED_MODE_INTR		0x06
#define BCM54X80_LED_MODE_QUALITY	0x07
#define BCM54X80_LED_MODE_RCVLED	0x08
#define BCM54X80_LED_MODE_WIRESPEED	0x09
#define BCM54X80_LED_MODE_CABLE_DIAG	0x0B
#define BCM54X80_LED_MODE_CRS		0x0D
#define BCM54X80_LED_MODE_OFF		0x0E
#define BCM54X80_LED_MODE_ON		0x0F
#endif // _BCM54X80_REG_H_