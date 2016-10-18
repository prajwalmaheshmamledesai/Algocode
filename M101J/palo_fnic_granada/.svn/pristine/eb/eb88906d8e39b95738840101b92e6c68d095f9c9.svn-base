/********************************************************************
 *
 *      File:   xcvr_defines.h
 *      Name:   Ramsundar Janakiraman
 *
 *       Description:  Defines for the XCVR driver.
 *
 *
 *      Copyright (c) 2001,2002,2003,2004 by Cisco Systems, Inc.
 *
 *       ALL RIGHTS RESERVED. THESE SOURCE FILES ARE THE SOLE PROPERTY
 *       OF CISCO SYTEMS, Inc. AND CONTAIN CONFIDENTIAL  AND PROPRIETARY
 *       INFORMATION.  REPRODUCTION OR DUPLICATION BY ANY MEANS OF ANY
 *       PORTION OF THIS SOFTWARE WITHOUT PRIOR WRITTEN CONSENT OF
 *       CISCO SYSTEMS, Inc. IS STRICTLY PROHIBITED.
 *
 * $Id $
 * $Source $
 * $Author $
 * ---------------------------------------------------------------
 *********************************************************************
 */
 
#ifndef __XCVR_DEFINES_H__
#define __XCVR_DEFINES_H__
 
//#include <isan/if_index.h>
//#include <isan/mts_intf.h>
//#include <isan/nodeid.h>
//#include <isan/platform_cmn_intf.h>
//#include <isan/pm_defs.h>
//#include <isan/sdwrap_intf.h>
//#include <isan/tg_intf.h>
//#include <isan/types.h>
//#include <isan/usd_impl.h>
//#include <isan/usd_intf.h>
//#include <isan/uuid.h>
//#include <isan/syserr_defs.h>
//#include <isan/syserr_intf.h>
//#include <isan/sysmgr_intf.h>
//#include <isan/gatosusd_intf.h>
//#include <platform/platform_common.h>
//#include <platform/sse_intf.h>
//#include <xcvr_enums.h>
 
//#include "gatosusd_isrhdlr.h"
//#include "gatosusd_logging_tracing.h"
//#include "gatosusd_private.h"
 
 
 
#define XCVRTHRESHVAL 10
#define CRIT_XCVRTHRESHVAL 5
#define XCVRTHRESHPERIOD 1000000
 
#define XCVR_BIT(n) (1 << (n))
 
#define XCVR_SET_BIT(flag,bit) ((flag) |= XCVR_BIT((bit)))
#define XCVR_CLEAR_BIT(flag,bit) ((flag) &= ~XCVR_BIT((bit)))
  
#define XCVR_SET_BITS(flag,mask) ((flag) |= (mask))
#define XCVR_CLEAR_BITS(flag,mask) ((flag) &= ~(mask))
  
#define XCVR_IS_BITS_SET(flag,mask) (((flag) & (mask)) == (mask))
#define XCVR_IS_BITS_CLEAR(flag,mask) ((~(flag) & (mask)) == (mask))
 
#define XCVR_CALC_MASK(offset, len)  (((1 << (len + offset))) - (1 << offset))
 
#define XCVR_DELAY(milli)
 
#define XCVR_NTOHL ntohl
 
#define XCVR_MAX_ASICS                    GATOS_MAX_ASICS_PER_BOARD
#define XCVR_MAX_SPIO                     24
#define XCVR_MAX_SLOTS                    GATOS_MAX_SLOTS
#define XCVR_MAX_PORTS                    GATOS_MAX_PORTS_PER_CARD
 
#define  XCVR_DEV_PMA_PMD 1
#define  XCVR_DEV_PCS     3
#define  XCVR_DEV_PHYXS   4
#define  XCVR_DEV_SFP     1
 
 
#define XCVR_INT_ALL                      ((1 << XCVR_INTR_MAX) - 1)
#define XCVR_INT_RX_PMA_PMD               XCVR_BIT(XCVR_INTR_LASI_RX_PMA_PMD)                                                                                 
#define XCVR_INT_RX_PCS                   XCVR_BIT(XCVR_INTR_LASI_RX_PCS)
#define XCVR_INT_RX_PHYXS                 XCVR_BIT(XCVR_INTR_LASI_RX_PHYXS)
#define XCVR_INT_RX_LASI_RX_PWR_HI        XCVR_BIT(XCVR_INTR_LASI_RX_PWR_HI)
#define XCVR_INT_RX_LASI_RX_PWR_LO        XCVR_BIT(XCVR_INTR_LASI_RX_PWR_LO)
#define XCVR_INT_TX_PMA_PMD               XCVR_BIT(XCVR_INTR_LASI_TX_PMA_PMD)                                                                                 
#define XCVR_INT_TX_PCS                   XCVR_BIT(XCVR_INTR_LASI_TX_PCS)
#define XCVR_INT_TX_PHYXS                 XCVR_BIT(XCVR_INTR_LASI_TX_PHYXS)                                                                                 
#define XCVR_INT_RX_LASI_TX_TEMP_HI       XCVR_BIT(XCVR_INTR_LASI_TX_TEMP_HI)
#define XCVR_INT_RX_LASI_TX_TEMP_LO       XCVR_BIT(XCVR_INTR_LASI_TX_TEMP_LO)
#define XCVR_INT_RX_LASI_TX_LSR_BIAS_HI   XCVR_BIT(XCVR_INTR_LASI_TX_LSR_BIAS_HI)
#define XCVR_INT_RX_LASI_TX_LSR_BIAS_LO   XCVR_BIT(XCVR_INTR_LASI_TX_LSR_BIAS_LO)
#define XCVR_INT_RX_LASI_TX_PWR_HI        XCVR_BIT(XCVR_INTR_LASI_TX_PWR_HI)
#define XCVR_INT_RX_LASI_TX_PWR_LO        XCVR_BIT(XCVR_INTR_LASI_TX_PWR_LO)
#define XCVR_INT_TX_FAULT                 XCVR_BIT(XCVR_INTR_LASI_TX_FAULT)                                                                                 
#define XCVR_INT_LNK_STATE                XCVR_BIT(XCVR_INTR_LASI_LINK_STATE)                                                                                 
 
#define XCVR_FLAG_PRESENT                 XCVR_BIT(XCVR_FLG_PRESENT)
#define XCVR_FLAG_XCVR_NOT_SFP            XCVR_BIT(XCVR_FLG_XCVR_NOT_SFP)
#define XCVR_FLAG_10G_CAPABLE             XCVR_BIT(XCVR_FLG_10G_CAPABLE)
#define XCVR_FLAG_CKSUM_OK                XCVR_BIT(XCVR_FLG_CKSUM_OK)
#define XCVR_FLAG_SECURITY_CRC_OK         XCVR_BIT(XCVR_FLG_SECURITY_CRC_OK)
#define XCVR_FLAG_CISCO_CHK_OK            XCVR_BIT(XCVR_FLG_CISCO_CHK_OK)
#define XCVR_FLAG_DDM_CAPABLE             XCVR_BIT(XCVR_FLG_DDM_CAPABLE)
#define XCVR_FLAG_PMA_PMD_PRESENT         XCVR_BIT(XCVR_FLG_PMA_PMD_PRESENT)
#define XCVR_FLAG_PCS_PRESENT             XCVR_BIT(XCVR_FLG_PCS_PRESENT)
#define XCVR_FLAG_PHYXS_PRESENT           XCVR_BIT(XCVR_FLG_PHY_XS_PRESENT)
#define XCVR_FLAG_XCVR_OPERATIONAL        XCVR_BIT(XCVR_FLG_XCVR_OPERATIONAL)
#define XCVR_FLAG_PMA_PMD_LOW_PWR_MODE    XCVR_BIT(XCVR_FLG_PMA_PMD_LOW_PWR_MODE)
#define XCVR_FLAG_PCS_LOW_PWR_MODE        XCVR_BIT(XCVR_FLG_PCS_LOW_PWR_MODE)
#define XCVR_FLAG_PHYXS_LOW_PWR_MODE      XCVR_BIT(XCVR_FLG_PHYXS_LOW_PWR_MODE)
#define XCVR_FLAG_LINK_UP                 XCVR_BIT(XCVR_FLG_LINK_UP)
#define XCVR_FLAG_TYPE_FIBER              XCVR_BIT(XCVR_FLG_TYPE_FIBER)
#define XCVR_FLAG_ALL                     ~(0)
 
#define XCVR_FLAGS(port_info) (port_info)->pss_data.xcvr_flags
 
#define XCVR_FLAG_SET(port_info, mask) \
    XCVR_SET_BITS((port_info)->pss_data.xcvr_flags, mask)
#define XCVR_FLAG_CLEAR(port_info, mask) \
    XCVR_CLEAR_BITS((port_info)->pss_data.xcvr_flags, mask)
 
#define XCVR_FLAG_IS_SET(port_info, mask) \
    ((port_info)->pss_data.xcvr_flags&(mask))
 
#define XCVR_FLAG_IS_CLEAR(port_info, mask) \
    ((~((port_info)->pss_data.xcvr_flags)&(mask)) == (mask))
 
 
 
#define XCVR_MAX_PORTS_PER_INST 1
 
#define XCVR_ENABLE_INTR TRUE
#define XCVR_DISABLE_INTR FALSE
 
#define XCVR_ENABLE_PORT TRUE
#define XCVR_DISABLE_PORT FALSE
 
#define XCVR_AVOID_DUP_LINKUP_MSG        TRUE
#define XCVR_FORCE_SEND_LINKUP_MSG       FALSE
 
#define XCVR_FLAG_INTR_REGISTERED 0x1
 
#define DBG_XCVRDRV_ERROR 100
#define DBG_XCVRDRV_TRACE 101
#define DBG_XCVRDRV_FLOW  102
 
#define DBG_XCVR_NELEM 4
 
#define DBG_XCVR_ERR(fmt,x...) if (verbose) printf(fmt, ##x)
#define DBG_XCVR_TRACE(fmt,x...) if (verbose) printf(fmt, ##x)
#define DBG_XCVR_FLOW(fmt,x...) if (verbose) printf(fmt, ##x)
 
#define XCVR_REG_XCVR_ID_1        2
#define XCVR_REG_XCVR_ID_2        3
 
#define XCVR_DEFAULT_DEBOUNCE_ENABLED_TIMEOUT_FIBER       100000 // Micro sec
#define XCVR_DEFAULT_DEBOUNCE_DISABLED_TIMEOUT_FIBER       10000
#define XCVR_DEFAULT_DEBOUNCE_ENABLED_TIMEOUT_COPPER     3100000 // Micro sec
#define XCVR_DEFAULT_DEBOUNCE_DISABLED_TIMEOUT_COPPER     300000
 
#define XCVR_DEFAULT_INSERT_DEBOUNCE_TIMEOUT_US 3000000 // Micro seconds.
 
 
/* Debug function */
//typedef void
//(*xcvr_debug_dump_t)(uint32 fd, void * data, uint32 size);
 
//
#define TRUE 1
#define FALSE 0


#endif /* __XCVR_DEFINES_H__ */
