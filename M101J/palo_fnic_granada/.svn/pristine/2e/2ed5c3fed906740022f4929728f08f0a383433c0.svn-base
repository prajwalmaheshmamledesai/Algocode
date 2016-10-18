/********************************************************************
 *
 *      File:   mv88e1111_lib.c
 *      Name:   Sudharshan Kadari
 *
 *      Description:
 *       Marvell Phy Access Library 
 *
 *
 * Copyright (c) 1985-2007 by Cisco Systems, Inc.
 * All rights reserved.
 *
 *
 *********************************************************************/

#include <stdio.h>
#include <sys/types.h>
#include <stdint.h>
#include <unistd.h>

#include "mv88e6095_lib.h"
#include "mv88e1111.h"

extern int
mdio_read(int fd, int address, int reg, uint16_t *value);

extern int
mdio_write(int fd, int address, int reg, uint16_t value);


int mv88e1111_write(int fd, uint8_t mdio_addr, uint8_t port_addr, int reg, uint16_t value)
{
#ifdef CAMPBELL
	return(mdio_write(fd, port_addr, reg, value));
#else
	return (mv88e6095_write(fd, mdio_addr, port_addr, reg, value));
#endif
}

int mv88e1111_read(int fd, uint8_t mdio_addr, uint8_t port_addr, int reg, uint16_t *value)
{
#ifdef CAMPBELL
	return(mdio_read(fd, port_addr, reg, value));
#else
	return (mv88e6095_read(fd, mdio_addr, port_addr, reg, value));
#endif
}

int mv88e1111_lpbk(int fd, uint8_t mdio_addr, uint8_t port_addr, uint8_t ena)
{
	uint16_t value, timeout = 1000;
	int rc = 0;

	rc = mv88e1111_read(fd, mdio_addr, port_addr, MV8E1111_CTRL, &value);
	if (rc) {
		printf("Error: Failed to read MV8E1111_CTRL\n");
		return (rc);
	}

	if (ena) {
		value |= MV8E1111_CTRL_FLD_SPEED_LSB;
		value &= ~MV8E1111_CTRL_FLD_SPEED_MSB;

		rc = mv88e1111_write(fd, mdio_addr, port_addr, MV8E1111_CTRL, value);
		if (rc) {
			printf("Error: Failed to read MV8E1111_CTRL\n");
			return (rc);
		}

		value |= MV8E1111_CTRL_FLD_RESET;
		rc = mv88e1111_write(fd, mdio_addr, port_addr, MV8E1111_CTRL, value);
		if (rc) {
			printf("Error: Failed to read MV8E1111_CTRL\n");
			return (rc);
		}

		while (timeout--) {
			usleep(1000);
			rc = mv88e1111_read(fd, mdio_addr, port_addr, MV8E1111_CTRL, &value);
			if (rc) {
				printf("Error: Failed to read MV8E1111_CTRL\n");
				return (rc);
			}
			if (!(value & MV8E1111_CTRL_FLD_RESET))
				break;
		}
		value |= MV8E1111_CTRL_FLD_LOOPBACK_EN;
		rc = mv88e1111_write(fd, mdio_addr, port_addr, MV8E1111_CTRL, value);
		if (rc) {
			printf("Error: Failed to read MV8E1111_CTRL\n");
			return (rc);
		}
	} else {
	}

	return (rc);
}


int mv88e1111_lpbk_print(int fd, uint32_t mdio_addr, int port_addr)
{
	int rc = 0;
	uint16_t data;

	rc = mv88e1111_read(fd, mdio_addr, port_addr, MV8E1111_CTRL, &data);
	if (rc) {
		printf("Error: Failed to read PHY Control pn port%d\n",
				port_addr);
		return (rc);
	}

	printf("Loopback On Port %d %s\n", port_addr, 
		(data & (0x01 << 14)) ? "Enable" : "Disabled");
	return (rc);
}

int mv88e1111_lpbk_ena(int fd, uint32_t mdio_addr, int port_addr)
{
	return (mv88e1111_lpbk(fd, mdio_addr, port_addr, 1));
}

int mv88e1111_lpbk_dis(int fd, uint32_t mdio_addr, int port_addr)
{
	return (mv88e1111_lpbk(fd, mdio_addr, port_addr, 0));
}

int mv88e1111_link_status(int fd, uint32_t mdio_addr, int port_addr, uint8_t *up)
{
	int rc = 0;
	uint16_t data;

        rc = mv88e1111_read(fd, mdio_addr, port_addr, MV8E1111_PHY_SPEC_STATUS, &data);
	if (rc) {
		printf("Error: Failed to read MV8E1111_STATUS (0x%X) on port%d\n",
				MV8E1111_STATUS, port_addr);
		return (rc);
	}

        *up =  (data & MV8E1111_PHY_SPEC_STATUS_FLD_LINK) ? 1 : 0; 

	return (rc);
}

int mv88e1111_link_status_print(int fd, uint32_t mdio_addr, int port_addr)
{
	int rc = 0;
	uint8_t up;
	rc = mv88e1111_link_status(fd, mdio_addr, port_addr, &up);
	if (rc) {
		printf("Error: Failed to read link status port%d\n", port_addr);
		return (rc);
	}
        printf("LinkStatus On Port %d is %s\n", port_addr, up ? "Up" : "Down");
	return (rc);
}

int mv88e1111_an_restart(int fd, uint32_t mdio_addr, int port_addr)
{
	uint16_t data;
	int	 rc = 0;

	rc = mv88e1111_read(fd, mdio_addr, port_addr, MV8E1111_CTRL, &data);
	if (rc) {
		printf("Error: Failed to read MV8E1111_CTRL port%d\n", port_addr);
		return (rc);
	}

	rc = mv88e1111_write(fd, mdio_addr, port_addr, MV8E1111_CTRL, data);
	if (rc) {
		printf("Error: Failed to read MV8E1111_CTRL port%d\n", port_addr);
		return (rc);
	}

	return (rc);
}

int mv88e1111_init(int fd, uint32_t mdio_addr, int port_addr, uint8_t master)
{
	int rc, cnt, count;
	uint16_t addr_data[][2] = {
		{MV8E1111_CTRL, MV8E1111_CTRL_FLD_AUTONEG_EN |
				MV8E1111_CTRL_FLD_FULL_DUPLEX |
				MV8E1111_CTRL_FLD_SPEED_MSB},
		{MV8E1111_AUTONEG_ADVERTISEMENT, 
				MV8E1111_LINK_PTNR_ABILITY_FLD_100BASE_TX_FULL_DUP |
				MV8E1111_LINK_PTNR_ABILITY_FLD_100BASE_TX_HALF_DUP |
				MV8E1111_LINK_PTNR_ABILITY_FLD_100BASE_T_FULL_DUP  |
				MV8E1111_LINK_PTNR_ABILITY_FLD_100BASE_T_HALF_DUP  |
				0x01},
		{MV8E1111_AUTONEG_EXP,	
				MV8E1111_AUTONEG_EXP_FLD_LP_NEXT_PAGE_ABLE |
				MV8E1111_AUTONEG_EXP_FLD_LOCAL_NEXT_PAGE_ABLE |
				MV8E1111_AUTONEG_EXP_FLD_PAGE_RECEIVED |
				MV8E1111_AUTONEG_EXP_FLD_LP_AUTO_NEG_ABLE},
		{MV8E1111_1000BT_CTRL,  
				(master ? MV8E1111_1000BT_CTRL_FLD_MASTER_CFG : 0x00) |
				MV8E1111_1000BT_CTRL_FLD_PORT_TYPE |
				MV8E1111_1000BT_CTRL_FLD_FULL_DUP},

		{MV8E1111_CTRL, MV8E1111_CTRL_FLD_AUTONEG_EN |
				MV8E1111_CTRL_FLD_FULL_DUPLEX |
				MV8E1111_CTRL_FLD_AUTONEG_RESTART |
				MV8E1111_CTRL_FLD_SPEED_MSB},
	};

	count = sizeof(addr_data)/(2 * sizeof(uint16_t));

	for (cnt = 0; cnt < count; cnt++) {
		rc = mv88e1111_write(fd, mdio_addr, port_addr, 
				addr_data[cnt][0], addr_data[cnt][1]);
		if (rc) {
			printf("Error: Failed to write [0x%X.%d]:0x%X\n", 
				addr_data[cnt][0], port_addr, addr_data[cnt][1]);
		} else {
			printf("[0x%X.%d] : 0x%X\n", 
				addr_data[cnt][0], port_addr, addr_data[cnt][1]);
		}
	}

	return (rc);
}


int mv88e1111_clk_init(int fd, uint32_t mdio_addr, int port_addr)
{
	int rc, cnt, count;
	uint16_t data, addr_data[][2] = {
		{MV8E1111_EXT_PAGE_ADDR2, 0x01B},
		{MV8E1111_MISC_CONTROL,   0x418F}, 
		{MV8E1111_EXT_PAGE_ADDR2, 0x016},
		{MV8E1111_MISC_CONTROL,   0xA2DA}, 
	};

	count = sizeof(addr_data)/(2 * sizeof(uint16_t));

	for (cnt = 0; cnt < count; cnt++) {
		rc = mv88e1111_write(fd, mdio_addr, port_addr, 
				addr_data[cnt][0], addr_data[cnt][1]);
		if (rc) {
			printf("Error: Failed to write [0x%X.%d]:0x%X\n", 
				addr_data[cnt][0], port_addr, addr_data[cnt][1]);
		} else {
			printf("[0x%X.%d] : 0x%X\n", 
				addr_data[cnt][0], port_addr, addr_data[cnt][1]);
		}
	}

	rc = mv88e1111_read(fd, mdio_addr, port_addr, 
				MV8E1111_CTRL, &data);
	if (rc) return (rc);
	data |= MV8E1111_CTRL_FLD_RESET;
	rc = mv88e1111_write(fd, mdio_addr, port_addr, 
				MV8E1111_CTRL, data);
	return (rc);
}
