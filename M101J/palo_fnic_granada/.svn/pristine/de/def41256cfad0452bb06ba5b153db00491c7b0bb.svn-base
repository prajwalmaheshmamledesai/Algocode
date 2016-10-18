/********************************************************************
 *
 *      File:   mv88e6095_lib.c
 *      Name:   Sudharshan Kadari
 *
 *      Description:
 *       Marvell Switch library 
 *
 *
 * Copyright (c) 1985-2007 by Cisco Systems, Inc.
 * All rights reserved.
 *
 *
 *********************************************************************/

#include <stdio.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include "stdint.h"
#include "unistd.h"

//#include "mdio_bus.h"
#include "mdio_lib.h"
#include "mv88e6095.h"
#include "mv88e6095_lib.h"

int mv88e6095_ppu(int fd, uint32_t address, int ena)
{
	int rc = 0;
	uint16_t data;

	rc = mv88e6095_read(fd, address,
                                PORT_GLOBAL1, MARVELL_PORTCTRL, &data);
	if (rc) {
		printf("Error: Failed to read PHY Control ppu ena%d\n",
				ena);
		return (rc);
	}

	if (ena) {
		data |= (0x01 << 14); 
	} else {
		data &= ~(0x01 << 14); 
	}
	rc = mv88e6095_write(fd, address,
                               PORT_GLOBAL1, MARVELL_PORTCTRL, data);
	return (rc);
}

int mv88e6095_lpbk_print(int fd, uint32_t address, int port)
{
	int rc = 0;
	uint16_t data;

	rc = mv88e6095_read(fd, address, port, 0, &data);
	if (rc) {
		printf("Error: Failed to read PHY Control pn port%d\n",
				port);
		return (rc);
	}

	printf("Loopback On Port %d %s\n", port, 
		(data & (0x01 << 14)) ? "Enable" : "Disabled");
	return (rc);
}

int mv88e6095_lpbk_ena(int fd, uint32_t address, int port)
{
	int rc = 0;
	uint16_t data;

	rc = mv88e6095_read(fd, address, port, 0, &data);
	if (rc) {
		printf("Error: Failed to read PHY Control pn port%d\n",
				port);
		return (rc);
	}

	// Set Reset bit, It is a auto clear bit 
	// Enable Loopback, set speed to 100MBps, enable FDX
	if (port < 8) {
		data |= (0x01 << 15);
	}
	data |= (0x01 << 14) | (0x01 << 13) | (0x01 << 8);
	// Disable Auto Negotiation.
	data &= ~(0x01 << 12);

	rc = mv88e6095_write(fd, address, port, 0, data);
	return (rc);
}

int mv88e6095_lpbk_dis(int fd, uint32_t address, int port)
{
	int rc = 0;
	uint16_t data;

	printf("  Loopback Disable on port %d\n", port);
	rc = mv88e6095_read(fd, address, port, 0, &data);
	if (rc) {
		printf("Error: Failed to read PHY Control pn port%d\n",
				port);
		return (rc);
	}

	// Set Reset bit, It is a auto clear bit 
	if (port < 8)
		data |= (0x01 << 15);
	// Clear loopback and duplex
	data &= ~((0x01 << 14) | (0x01 << 13) | (0x01 << 8));

	rc = mv88e6095_write(fd, address, port, 0, data);
	return (rc);
}

int mv88e6095_link_status_print(int fd, uint32_t address, int port)
{
	int rc = 0;
	uint16_t data;

	rc = mv88e6095_read(fd, address, 0x10 | port, 0, &data);
	if (rc) {
		printf("Error: Failed to read PHY Control pn port%d\n",
				port);
		return (rc);
	}

	printf("LinkStatus On Port %d is %s\n", port, 
		(data & (0x01 << 11)) ? "Up" : "Down");
	return (rc);
}

int mv88e6095_link_status(int fd, uint32_t address, int port, uint8_t *up)
{
	int rc = 0;
	uint16_t data;

	rc = mv88e6095_read(fd, address, 0x10 | port, 0, &data);
	if (rc) {
		printf("Error: Failed to read PHY Control pn port%d\n",
				port);
		return (rc);
	}

	*up =  (data & (0x01 << 11)) ? 1 : 0; 

	return (rc);
}

int mv88e6095_vlan_table_config(int fd, uint32_t address, int port, uint16_t vlan_table, 
		uint16_t learn_disable, uint16_t dbnum)
{
	int rc = 0;
	uint16_t data;

	data = ((vlan_table & 0x7FF) | (learn_disable << 11) | (dbnum << 12));
	rc = mv88e6095_write(fd, address, 0x10 | port, MARVELL_PORTVLANMAP, data);
	if (rc) {
		printf("Error: Failed to read Marvell Port%d Map\n",
				port);
		return (rc);
	}

	return (rc);
}


int mv88e6095_diag_init(int fd, uint32_t mdio_addr, int port_addr)
{
	int rc, cnt, count;
	uint16_t addr_data[][2] = {
		{0x00, 0x1E86},
		{0x01, 0xC503},
	};

	count = sizeof(addr_data)/(2 * sizeof(uint16_t));

	for (cnt = 0; cnt < count; cnt++) {
		rc = mv88e6095_write(fd, mdio_addr, 0x10 | port_addr,
				addr_data[cnt][0], addr_data[cnt][1]);
		if (rc) {
			printf("Error: Failed to write [0x%X.%d]:0x%X\n", 
				addr_data[cnt][0], port_addr, addr_data[cnt][1]);
		}
	}

	return (rc);
}

#ifndef PHY_IN_UNICAST
#define PHY_IN_UNICAST 4
#endif
int
mv88e6095_ingress_stats_get(int fd, int addr, int port, uint32_t *data)
{
    int             rc = 0;

    // Check if the op is busy.
    rc = mv88e6095_capture_ingress_port_stats(fd, addr, port);
    if (rc) {
        printf("Error: Failed to capture port%d stats\n", port);
        return (rc);
    }

    rc = mv88e6095_stats_reg_rd(fd, addr, port, PHY_IN_UNICAST, data, 1);
    if (rc) {
	printf("Error: Ingress count read (0x%X) of port%d\n",
		PHY_IN_UNICAST, port);
        return (rc);
    }
    return (rc);
}

