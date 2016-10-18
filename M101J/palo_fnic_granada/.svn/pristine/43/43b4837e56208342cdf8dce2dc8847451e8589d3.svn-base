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
#ifndef REDWOOD_MAC_RMON_H_INCLUDED
#define REDWOOD_MAC_RMON_H_INCLUDED

#include "redwood_mac_rmon_stats.h"

typedef struct rmon_stats_t {
  unsigned int ctr[RMON_STAT_NUM*2]; // x2 for xge, 36-bit counters
} rmon_stats_t;

void mac_rmon_stats_read_port( int asic_id, int asic_port, 
                                rmon_stats_t *rmon,
                                rmon_stats_t *rmon_prev );

#endif /*REDWOOD_MAC_RMON_H_INCLUDED*/

