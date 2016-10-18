/********************************************************************
 *
 * File: redwood_mic_fpga.h
 * Name: Bruce McLoughlin
 *
 * Description:
 *   MIC FPGA interface from Redwood
 *
 * Copyright (c) 2006-2009 by Nuova Systems, Inc.
 **********************************************************************/
#ifndef REDWOOD_MIC_FPGA_INCLUDED
#define REDWOOD_MIC_FPGA_INCLUDED

#include "mic.h"

/****************************************************************************
* mic_read_xn
****************************************************************************/
int mic_read_xn( uint32_t  asic_id, uint32_t  reg, uint32_t *data );

/****************************************************************************
* mic_write_xn
****************************************************************************/
int mic_write_xn( uint32_t  asic_id, uint32_t  reg, uint32_t data );

/****************************************************************************
* mic_read_seq_xn
****************************************************************************/
int mic_read_seq_xn( uint32_t  asic_id, uint32_t  reg, uint32_t *data, int n_words );

/****************************************************************************
* mic_set_port_led
****************************************************************************/
void mic_set_port_led( int asic_id, int asic_port, int color );

#endif /*REDWOOD_MIC_FPGA_INCLUDED*/

