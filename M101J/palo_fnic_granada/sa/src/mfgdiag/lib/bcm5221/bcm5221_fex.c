/********************************************************************
 *
 *      File:   bcm5221_acc.c
 *      Name:   Sudharshan Kadari
 *
 *      Description:
 *       Broadcom Serdes 5221 register access
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
#include "mdio.h"
#include "mdio_lib.h"
#include "diag_fex.h"

extern uint16_t mdio_read_gbe( int asic_id, uint8_t portad, uint8_t dev,
                uint16_t reg);
extern void mdio_write_gbe( int asic_id, uint8_t portad, uint8_t dev,
                uint16_t reg, uint16_t data );
extern uint32_t diag_get_verbose();


int bcm5221_reg_read (int fd, uint8_t inst, uint8_t port_addr, 
			uint8_t addr, uint16_t *data)
{
	int rc = 0;
        if (diag_get_verbose())
		printf("Rd %s%d [0x%X.%d]\n", 
			((inst < (FEX_BCM5221_DEV_COUNT-1)) ? "RDW" : "CPU"), 
			inst, port_addr, addr);
	if (inst < (FEX_BCM5221_DEV_COUNT-1)) {
        	*data = mdio_read_gbe(inst, port_addr, 0, addr);
	} else {
		rc = diag_mdio_read(fd, port_addr, addr, data);
	}
	return (rc);
}

int bcm5221_reg_write (int fd, uint8_t inst, uint8_t port_addr,
			uint8_t addr, uint16_t data)
{
	int rc = 0;
        if (diag_get_verbose())
                printf("Wr %s%d [0x%X.%d]:0x%X\n", 
			((inst < (FEX_BCM5221_DEV_COUNT-1)) ? "RDW" : "CPU"), inst,
                        port_addr, addr, data);
	if (inst < (FEX_BCM5221_DEV_COUNT-1)) {
        	mdio_write_gbe(inst, port_addr, 0, addr, data);
	} else {
		rc = diag_mdio_write(fd, port_addr, addr, data);
	}
	return (rc);
}
