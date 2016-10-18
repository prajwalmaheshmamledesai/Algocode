/********************************************************************
 *
 *      File:   mv88e3019_reg.c
 *      Name:   Sudharshan Kadari
 *
 *      Description:
 *       Marvell Phy Register Definitions. 
 *
 *
 * Copyright (c) 1985-2007 by Cisco Systems, Inc.
 * All rights reserved.
 *
 *
 *********************************************************************/
#include <stdio.h>
#include "diag_reg.h"
#include "mvphy_reg.h"

reg_desc_t mvphy_reg_desc[] =
{
RCFG(MVPHY_CTRL)
BFLD(MVPHY_CTRL, RESET)
BFLD(MVPHY_CTRL, LOOPBACK_EN)
BFLD(MVPHY_CTRL, SPEED)
BFLD(MVPHY_CTRL, AUTONEG_EN)
BFLD(MVPHY_CTRL, POWERDOWN)
BFLD(MVPHY_CTRL, ISOLATE)
BFLD(MVPHY_CTRL, AUTONEG_RESTART)
BFLD(MVPHY_CTRL, FULL_DUPLEX)
BFLD(MVPHY_CTRL, COLLISION_TEST)
BFLD(MVPHY_CTRL, UNIDIR_ENA)
RCFG(MVPHY_STATUS)
BFLD(MVPHY_STATUS, 100BASE_T4)
BFLD(MVPHY_STATUS, 100BASE_FDX)
BFLD(MVPHY_STATUS, 100BASE_HDX)
BFLD(MVPHY_STATUS, 10BASE_FDX)
BFLD(MVPHY_STATUS, 10BASE_HDX)
BFLD(MVPHY_STATUS, 100BASE_T2_FDX)
BFLD(MVPHY_STATUS, 100BASE_T2_HDX)
BFLD(MVPHY_STATUS, EXT_STATUS)
BFLD(MVPHY_STATUS, UNIDIR_ABILITY)
BFLD(MVPHY_STATUS, MF_PREAMBLE_SUPP)
BFLD(MVPHY_STATUS, AUTO_NEG_COMPLETE)
BFLD(MVPHY_STATUS, REMOTE_FAULT)
BFLD(MVPHY_STATUS, AUTO_NEG_ABILITY)
BFLD(MVPHY_STATUS, LINK_STATUS)
BFLD(MVPHY_STATUS, JABBER_DETECT)
BFLD(MVPHY_STATUS, EXT_CAPABILITY)
RCFG(MVPHY_PHY_ID1_MSB)
RCFG(MVPHY_PHY_ID2_LSB)
RCFG(MVPHY_AUTONEG_ADVERTISEMENT)
BFLD(MVPHY_AUTONEG_ADVERTISEMENT, NEXT_PAGE)
BFLD(MVPHY_AUTONEG_ADVERTISEMENT, ACK)
BFLD(MVPHY_AUTONEG_ADVERTISEMENT, REMOTE_FAULT)
BFLD(MVPHY_AUTONEG_ADVERTISEMENT, ASYMM_PAUSE)
BFLD(MVPHY_AUTONEG_ADVERTISEMENT, PAUSE)
BFLD(MVPHY_AUTONEG_ADVERTISEMENT, 100BASE_T4)
BFLD(MVPHY_AUTONEG_ADVERTISEMENT, 100BASE_FDX)
BFLD(MVPHY_AUTONEG_ADVERTISEMENT, 100BASE_HDX)
BFLD(MVPHY_AUTONEG_ADVERTISEMENT, 10BASE_FDX)
BFLD(MVPHY_AUTONEG_ADVERTISEMENT, 10BASE_HDX)
RFLD(MVPHY_AUTONEG_ADVERTISEMENT, SECTOR_FIELD)
RCFG(MVPHY_LINK_PTNR_ABILITY)
BFLD(MVPHY_LINK_PTNR_ABILITY, NEXT_PAGE)
BFLD(MVPHY_LINK_PTNR_ABILITY, ACK)
BFLD(MVPHY_LINK_PTNR_ABILITY, REMOTE_FAULT)
RFLD(MVPHY_LINK_PTNR_ABILITY, LPTECH_ABLE)
RFLD(MVPHY_LINK_PTNR_ABILITY, SECTOR_FIELD)
RCFG(MVPHY_AUTONEG_EXP)
BFLD(MVPHY_AUTONEG_EXP, DETECT_FAULT)
BFLD(MVPHY_AUTONEG_EXP, LP_NEXT_PAGE_ABLE)
BFLD(MVPHY_AUTONEG_EXP, LOCAL_NEXT_PAGE_ABLE)
BFLD(MVPHY_AUTONEG_EXP, PAGE_RECEIVED)
BFLD(MVPHY_AUTONEG_EXP, LP_AUTO_NEG_ABLE)
RCFG(MVPHY_NEXT_PAGE_TX)
BFLD(MVPHY_NEXT_PAGE_TX, NEXT_PAGE)
BFLD(MVPHY_NEXT_PAGE_TX, MSG_PAGE_MODE)
BFLD(MVPHY_NEXT_PAGE_TX, ACK2)
BFLD(MVPHY_NEXT_PAGE_TX, TOGGLE)
RFLD(MVPHY_NEXT_PAGE_TX, DATA)
RCFG(MVPHY_LINK_PTNR_NEXT_TX)
BFLD(MVPHY_LINK_PTNR_NEXT_TX, NEXT_PAGE)
BFLD(MVPHY_LINK_PTNR_NEXT_TX, RX_ACK)
BFLD(MVPHY_LINK_PTNR_NEXT_TX, RX_MSG)
BFLD(MVPHY_LINK_PTNR_NEXT_TX, ACK2)
BFLD(MVPHY_LINK_PTNR_NEXT_TX, TOGGLE)
RFLD(MVPHY_LINK_PTNR_NEXT_TX, DATA)
RCFG(MVPHY_PHY_SPEC_CTRL)
BFLD(MVPHY_PHY_SPEC_CTRL, ENERGY_DETECT)
BFLD(MVPHY_PHY_SPEC_CTRL, DIS_NLP_CHECK)
BFLD(MVPHY_PHY_SPEC_CTRL, ENA_LP_NEXT_PAGE)
BFLD(MVPHY_PHY_SPEC_CTRL, DIS_LP_GEN)
BFLD(MVPHY_PHY_SPEC_CTRL, DIS_SCRAMBLER)
BFLD(MVPHY_PHY_SPEC_CTRL, DIS_FEFI)
BFLD(MVPHY_PHY_SPEC_CTRL, EXT_DIST_EN)
RFLD(MVPHY_PHY_SPEC_CTRL, MDI_CROSS_OVER)
BFLD(MVPHY_PHY_SPEC_CTRL, ASSERT_CRS_TX)
BFLD(MVPHY_PHY_SPEC_CTRL, SQE_TEST)
BFLD(MVPHY_PHY_SPEC_CTRL, SQE_TEST)
BFLD(MVPHY_PHY_SPEC_CTRL, AUTO_POL)
BFLD(MVPHY_PHY_SPEC_CTRL, DIS_JABBER)
RCFG(MVPHY_PHY_SPEC_STATUS)
BFLD(MVPHY_PHY_SPEC_STATUS, SPEED)
BFLD(MVPHY_PHY_SPEC_STATUS, DUPLEX)
BFLD(MVPHY_PHY_SPEC_STATUS, PAGE_RECEIVED)
BFLD(MVPHY_PHY_SPEC_STATUS, LINK)
BFLD(MVPHY_PHY_SPEC_STATUS, MDI_CROSS_OVER)
BFLD(MVPHY_PHY_SPEC_STATUS, SLEEP)
BFLD(MVPHY_PHY_SPEC_STATUS, POLARITY)
BFLD(MVPHY_PHY_SPEC_STATUS, JABBER)
RCFG(MVPHY_MDI0_IRQ_ENABLE)
BFLD(MVPHY_MDI0_IRQ_ENABLE, SPEED_CHANGED)
BFLD(MVPHY_MDI0_IRQ_ENABLE, DUPLEX_CHANGED)
BFLD(MVPHY_MDI0_IRQ_ENABLE, PAGE_RECIEVED)
BFLD(MVPHY_MDI0_IRQ_ENABLE, AUTO_NEG_COMPLETE)
BFLD(MVPHY_MDI0_IRQ_ENABLE, AUTO_LINK_STATUS_CHANGED)
BFLD(MVPHY_MDI0_IRQ_ENABLE, SYMBOL_ERROR)
BFLD(MVPHY_MDI0_IRQ_ENABLE, FALSE_CARRIER)
BFLD(MVPHY_MDI0_IRQ_ENABLE, FIFO_OVER_UNDER_FLOW)
BFLD(MVPHY_MDI0_IRQ_ENABLE, MDI_CROSS_OVER)
BFLD(MVPHY_MDI0_IRQ_ENABLE, ENERGY_DETECT)
BFLD(MVPHY_MDI0_IRQ_ENABLE, POLARITY_CHANGED)
BFLD(MVPHY_MDI0_IRQ_ENABLE, JABBER)
RCFG(MVPHY_MDI0_IRQ_STATUS)
BFLD(MVPHY_MDI0_IRQ_STATUS, SPEED_CHANGED)
BFLD(MVPHY_MDI0_IRQ_STATUS, DUPLEX_CHANGED)
BFLD(MVPHY_MDI0_IRQ_STATUS, PAGE_RECIEVED)
BFLD(MVPHY_MDI0_IRQ_STATUS, AUTO_NEG_COMPLETE)
BFLD(MVPHY_MDI0_IRQ_STATUS, AUTO_LINK_STATUS)
BFLD(MVPHY_MDI0_IRQ_STATUS, SYMBOL_ERROR)
BFLD(MVPHY_MDI0_IRQ_STATUS, FALSE_CARRIER)
BFLD(MVPHY_MDI0_IRQ_STATUS, FIFO_OVER_FLOW)
BFLD(MVPHY_MDI0_IRQ_STATUS, MDI_CROSS_OVER)
BFLD(MVPHY_MDI0_IRQ_STATUS, ENERGY_DETECT)
BFLD(MVPHY_MDI0_IRQ_STATUS, POLARITY_CHANGED)
BFLD(MVPHY_MDI0_IRQ_STATUS, JABBER)
RCFG(MVPHY_IRQ_PORT_SUMMARY)
RCFG(MVPHY_RX_ERR_COUNTER)
RCFG(MVPHY_LED_PARALLEL_SELECT)
RFLD(MVPHY_LED_PARALLEL_SELECT, LED1)
RFLD(MVPHY_LED_PARALLEL_SELECT, LED0)
RCFG(MVPHY_GLOBAL_STATUS)
RCFG(MVPHY_LED_CONTROL)
RFLD(MVPHY_LED_CONTROL, PULSE_STRETCH)
RFLD(MVPHY_LED_CONTROL, BLINK_RATE)
RFLD(MVPHY_LED_CONTROL, LED1_SPEED_SEL)
RFLD(MVPHY_LED_CONTROL, LED0_SPEED_SEL)
RCFG(MVPHY_MANUAL_LED_OVERRIDE)
BFLD(MVPHY_MANUAL_LED_OVERRIDE, INV_LED1)
BFLD(MVPHY_MANUAL_LED_OVERRIDE, INV_LED0)
RFLD(MVPHY_MANUAL_LED_OVERRIDE, FORCE_LED1)
RFLD(MVPHY_MANUAL_LED_OVERRIDE, FORCE_LED0)
RCFG(MVPHY_VCT_N0)
RCFG(MVPHY_VCT_N1)
RCFG(MVPHY_SPEC_CTRL2)
RFLD(MVPHY_SPEC_CTRL2, MAC_INTF_MODE)
BFLD(MVPHY_SPEC_CTRL2, NS_SEQ)
BFLD(MVPHY_SPEC_CTRL2, ENA_10_100_TEST)
BFLD(MVPHY_SPEC_CTRL2, DIS_BREAK_LINK)
BFLD(MVPHY_SPEC_CTRL2, ENA_LINE_LPBK)
BFLD(MVPHY_SPEC_CTRL2, SW_MEDIA_SEL)
BFLD(MVPHY_SPEC_CTRL2, TDR_WAIT_TIME)
BFLD(MVPHY_SPEC_CTRL2, ENA_RX_CLK)
BFLD(MVPHY_SPEC_CTRL2, SEL_CISA)
RCFG(MVPHY_TEST_MODE_SEL)
RCFG(MVPHY_PAGE_DATA)
{ TYP_NONE, "", 0, 0, "", "", 0, 0, ""}
};
