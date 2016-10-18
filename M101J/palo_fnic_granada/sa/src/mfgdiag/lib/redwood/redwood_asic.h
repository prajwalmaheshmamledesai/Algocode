/********************************************************************
 *
 * File: redwood_asic.h
 * Name: Bruce McLoughlin
 *
 * Description:
 *   ASIC level definitions for Redwood.
 *
 * Copyright (c) 2006-2009 by Nuova Systems, Inc.
 **********************************************************************/
#ifndef _REDWOOD_ASIC_H_
#define _REDWOOD_ASIC_H_

extern void redwood_hw_reset (int reset_fd, uint8_t inst);
extern void redwood_hw_unreset (int reset_fd, uint8_t inst);

#endif /*_REDWOOD_ASIC_H_*/

