/********************************************************************
 *
 * File: redwood_mdio.h
 * Name: Bruce McLoughlin
 *
 * Description:
 *
 * Copyright (c) 2006-2009 by Nuova Systems, Inc.
 **********************************************************************/
#ifndef REDWOOD_MDIO_H_INCLUDED
#define REDWOOD_MDIO_H_INCLUDED

/* Offsets into DHS array of the four MDIO Master Core registes
*/
#define MDIO_STATUS  0
#define MDIO_COMMAND 4
#define MDIO_DATA    8
#define MDIO_REGADDR 12

/* STATUS bits */
#define MDIO_STATUS_BUSY       1
#define MDIO_STATUS_ERROR      2

/* COMMAND bits */
#define MDIO_COMMAND_DEV_SHIFT   0
#define MDIO_COMMAND_PORT_SHIFT  5
#define MDIO_COMMAND_POST_INCR   0x4000
#define MDIO_COMMAND_READ        0x8000

/***************************************************************
* mdio_init
*
*   Initialize the MDIO master module in Redwood (if not
*   done already).
*
*   This function is called at ASIC bringup time because
*   MDIO is required to load the PHY micro-code. So, here
*   we enable both 1.2V and 3.3V MDIO masters, even though
*   only one is physically connected on the board.
**************************************************************/
void redwood_mdio_init( int asic_id );

uint16_t mdio_read_gbe(   int asic_id, uint8_t portad, uint8_t dev, uint16_t reg );
uint16_t mdio_read_1_2v(  int asic_id, uint8_t portad, uint8_t dev, uint16_t reg );
uint16_t mdio_read_3_3v(  int asic_id, uint8_t portad, uint8_t dev, uint16_t reg );

void     mdio_write_gbe(  int asic_id, uint8_t portad, uint8_t dev, uint16_t reg, uint16_t data );
void     mdio_write_1_2v( int asic_id, uint8_t portad, uint8_t dev, uint16_t reg, uint16_t data );
void     mdio_write_3_3v( int asic_id, uint8_t portad, uint8_t dev, uint16_t reg, uint16_t data );
                                                                                                         
#endif /*REDWOOD_MDIO_H_INCLUDED*/
