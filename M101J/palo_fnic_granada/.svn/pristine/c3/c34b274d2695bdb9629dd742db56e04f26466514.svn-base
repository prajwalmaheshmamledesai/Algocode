/********************************************************************
 *
 * File: redwood_mac_rmon.h
 * Name: Bruce McLoughlin
 *
 * Description:
 *   MAC RMON definitions for Redwood.
 *
 * Copyright (c) 2006-2009 by Nuova Systems, Inc.
 **********************************************************************/
#ifndef REDWOOD_MAC_RMON_STATS_H_INCLUDED
#define REDWOOD_MAC_RMON_STATS_H_INCLUDED


/**************************************************************************
* RMON_STAT_OFFSETS
*
* These provide the logical offset of each RMON counter. For BI/CI the
* counters are 32-bits wide, or 2 bytes apart. For HIF/NIF the counters
* are 36-bit wide, require two accesses (the second access to the "_upper"
* register, and are 4 bytes apart.
**************************************************************************/
enum RMON_STAT_OFFSETS {
    STAT_TX_PKT_LT64 = 0
  , STAT_TX_PKT_64
  , STAT_TX_PKT_65
  , STAT_TX_PKT_128
  , STAT_TX_PKT_256
  , STAT_TX_PKT_512
  , STAT_TX_PKT_1024
  , STAT_TX_PKT_1519
  , STAT_TX_PKT_2048
  , STAT_TX_PKT_4096
  , STAT_TX_PKT_8192
  , STAT_TX_PKT_GT9216
  , STAT_TX_PKTTOTAL
  , STAT_TX_OCTETS
  , STAT_TX_PKTOK
  , STAT_TX_UCAST
  , STAT_TX_MCAST
  , STAT_TX_BCAST
  , STAT_TX_VLAN
  , STAT_TX_PAUSE
  , STAT_TX_USER_PAUSE
  , STAT_TX_FRM_ERROR

  , STAT_RX_PKT_LT64
  , STAT_RX_PKT_64
  , STAT_RX_PKT_65
  , STAT_RX_PKT_128
  , STAT_RX_PKT_256
  , STAT_RX_PKT_512
  , STAT_RX_PKT_1024
  , STAT_RX_PKT_1519
  , STAT_RX_PKT_2048
  , STAT_RX_PKT_4096
  , STAT_RX_PKT_8192
  , STAT_RX_PKT_GT9216
  , STAT_RX_PKTTOTAL
  , STAT_RX_OCTETS
  , STAT_RX_PKTOK
  , STAT_RX_UCAST
  , STAT_RX_MCAST
  , STAT_RX_BCAST
  , STAT_RX_VLAN
  , STAT_RX_OVERSIZE
  , STAT_RX_TOOLONG
  , STAT_RX_DISCARD
  , STAT_RX_UNDERSIZE
  , STAT_RX_FRAGMENT
  , STAT_RX_CRCERR
  , STAT_RX_INRANGEERR
  , STAT_RX_JABBER
  , STAT_RX_PAUSE
  , STAT_RX_USER_PAUSE
  , STAT_TX_OCTETSOK
  , STAT_RX_OCTETSOK
  , RMON_STAT_NUM
  };

#endif /*REDWOOD_MAC_RMON_STATS_H_INCLUDED*/

