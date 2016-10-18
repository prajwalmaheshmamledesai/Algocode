/********************************************************************
 *
 *      File:   bcm54980_acc.c
 *      Name:   Sudharshan Kadari
 *
 *      Description:
 *       Broadcom Serdes 54980 register access
 *
 *
 * Copyright (c) 1985-2008 by Cisco Systems, Inc.
 * All rights reserved.
 *
 *
 *********************************************************************/

#include <stdio.h>
#include <string.h>
#include <sys/types.h>

extern uint16_t mdio_read_gbe( int asic_id, uint8_t portad, uint8_t dev, 
		uint16_t reg);
extern void mdio_write_gbe( int asic_id, uint8_t portad, uint8_t dev, 
		uint16_t reg, uint16_t data );
extern uint32_t diag_get_verbose();

int bcm54980_reg_read (int fd, uint8_t bcm_inst, uint8_t port_addr, 
			uint8_t addr, uint16_t *data)
{
	int rdw_inst = bcm_inst >> 3;
	if (diag_get_verbose())
		printf("Rd RDW%d [0x%X.%d]\n", rdw_inst, port_addr, addr);
	*data = mdio_read_gbe(rdw_inst, port_addr, 0, addr);
	return (0);
}

int bcm54980_reg_write (int fd, uint8_t bcm_inst, uint8_t port_addr, 
			uint8_t addr, uint16_t data)
{
	int rdw_inst = bcm_inst >> 3;
	if (diag_get_verbose())
		printf("Wr RDW%d [0x%X.%d]:0x%X\n", rdw_inst, 
			port_addr, addr, data);
	mdio_write_gbe(rdw_inst, port_addr, 0, addr, data);
	return (0);
}

extern void redwood_octphy_unreset(int asic_id);
extern void redwood_octphy_reset(int asic_id);

void bcm54x80_octphy_unreset(int asic_id)
{
	redwood_octphy_unreset(asic_id);
}

void bcm54x80_octphy_reset(int asic_id)
{
	redwood_octphy_reset(asic_id);
}
