/********************************************************************
 *
 *      File:   bcm5221_api.c
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
#include <unistd.h>
#include <sys/types.h>
#include "bcm5221_reg.h"
#include "bcm5221_api.h"

int bcm5221_soft_reset (int fd, uint8_t inst, uint8_t port_addr)
{
	uint16_t data, reset_b = (1 <<  BCM5221_CONTROL_FLD_SOFT_RESET);
	int	 rc = 0, timeout = 100;	

	rc = bcm5221_reg_read (fd, inst, port_addr, BCM5221_CONTROL, &data);
	if (rc) {
		printf("Error: read MII_CONTROL (rc=%d)\n", rc);
		return (rc);
	}

	// Soft Reset is a self clearing bit. 
	data |= reset_b; 

	rc = bcm5221_reg_write (fd, inst, port_addr, BCM5221_CONTROL, data);
	if (rc) {
		printf("Error: write MII_CONTROL (rc=%d)\n", rc);
		return (rc);
	}

	// Poll to check the bit is cleared
	do {
		usleep(10);	// Sleep for 10 us
		rc = bcm5221_reg_read (fd, inst, port_addr, BCM5221_CONTROL, &data);
	} while (!rc && timeout-- && (data & reset_b)); 

	return( (data & reset_b) ? -1 : rc);
}

int bcm5221_loopback_ena (int fd, uint8_t inst, uint8_t port_addr, uint8_t ena)
{
	uint16_t data, lpbk_b = (1 << BCM5221_CONTROL_FLD_LOOPBACK);
	int	 rc = 0;	

	rc = bcm5221_reg_read (fd, inst, port_addr, BCM5221_CONTROL, &data);
	if (rc) {
		printf("Error: read MII_CONTROL (rc=%d)\n", rc);
		return (rc);
	}

	if (ena) {
		data |= lpbk_b;
	} else {
		data &= ~lpbk_b;
	}

	rc = bcm5221_reg_write (fd, inst, port_addr, BCM5221_CONTROL, data);
	if (rc) {
		printf("Error: write MII_CONTROL (rc=%d)\n", rc);
		return (rc);
	}
	return (0);
}


int bcm5221_init (int fd, uint8_t inst, uint8_t port_addr)
{
	int		rc = 0;
	uint32_t	cnt;
	uint16_t	data;
	uint16_t 	addr_data[][2] = {
			{BCM5221_10BT_AUX_GEN_STATUS, 
				(0x01 << BCM5221_10BT_AUX_GEN_STATUS_FLD_MDIX_SWAP)},
			};

	for (cnt = 0; !rc && cnt < 
		(sizeof(addr_data)/(2*(sizeof(uint16_t)))); cnt++) {
		rc = bcm5221_reg_read (fd, inst, port_addr, 
			(uint8_t)addr_data[cnt][0], &data); 
		if (rc) return (rc);
		rc = bcm5221_reg_write (fd, inst, port_addr, 
			(uint8_t)addr_data[cnt][0], (data | addr_data[cnt][1]));
	}

	return (rc);
}

int bcm5221_link_status_get (int fd, uint8_t inst, uint8_t port_addr, uint8_t *link_up)
{
	uint16_t data, link_b = (1 <<  BCM5221_STATUS_FLD_LINK_STATUS);
	uint16_t fault_b = (1 << BCM5221_STATUS_FLD_REMOTE_FAULT);
	int	 rc = 0;

	// This is a sticky bit. so read twice.
	rc = bcm5221_reg_read (fd, inst, port_addr, BCM5221_STATUS, &data);
	if (rc) {
		printf("Error: read MII_STATUS (rc=%d)\n", rc);
		return (rc);
	}

	rc = bcm5221_reg_read (fd, inst, port_addr, BCM5221_STATUS, &data);
	if (rc) {
		printf("Error: read MII_STATUS (rc=%d)\n", rc);
		return (rc);
	}

	if (data & fault_b) {
		printf(" BCM5221-%d Remote Fault seen\n", inst);
		*link_up = 0;
	} else {
		*link_up = (data & link_b) ? 1 : 0;
	}	
	return (rc);
}

int bcm5221_loopback_print (int fd, uint8_t inst, uint8_t port_addr)
{

        uint16_t data, lpbk_b = (1 << BCM5221_CONTROL_FLD_LOOPBACK);
        int      rc = 0;

        rc = bcm5221_reg_read (fd, inst, port_addr, BCM5221_CONTROL, &data);
        if (rc) {
                printf("Error: read MII_CONTROL (rc=%d)\n", rc);
                return (rc);
        }

        printf("BCMSRD%d\t : %s\n", inst, (data & lpbk_b) ? "Ena" : "Dis");
	return (rc);

}

int bcm5221_shadow_reg_enable (int fd, uint8_t inst, uint8_t port_addr, uint8_t ena)
{
	uint16_t	data, shd_en = (0x01 << BCM5221_BROADCOM_TEST_FLD_SHADOW_ENA);
	int		rc = 0;

	rc = bcm5221_reg_read(fd, inst, port_addr, BCM5221_BROADCOM_TEST, &data);
	if (rc) {
		printf(" Error: rd BCM5221_BROADCOM_TEST(rc=%d)\n", rc);
		return (rc);
	}

	if (ena)  {
		if (data & shd_en) return (rc);
		data |= shd_en;
	} else {
		if (!(data & shd_en)) return (rc);
		data &= ~shd_en;
	}

	rc = bcm5221_reg_write(fd, inst, port_addr, BCM5221_BROADCOM_TEST, data);
	if (rc) {
		printf(" Error: wr BCM5221_BROADCOM_TEST(rc=%d)\n", rc);
		return (rc);
	}

	return (rc);
}

int bcm5221_shadow_reg_read (int fd, uint8_t inst, uint8_t port_addr, uint8_t addr, uint16_t *data)
{
	int		rc = 0, err = 0;

	if ((addr < BCM5221_AUX_MODE4) || (addr > BCM5221_AUX_STATUS4)) {
		printf(" Error: Invalid Shadow register address (0x%X)\n", addr);
		return (-1);
	}

	// Enable Shadow reg	
	rc = bcm5221_shadow_reg_enable (fd, inst, port_addr, 1);
	if (rc) return (rc);

	err = bcm5221_reg_read(fd, inst, port_addr, addr, data);

	// Disable Shadow reg	
	rc = bcm5221_shadow_reg_enable (fd, inst, port_addr, 0);
	if (rc) return (rc);
	return (err? err: rc);
}

int bcm5221_shadow_reg_write (int fd, uint8_t inst, uint8_t port_addr, uint8_t addr, uint16_t data)
{
	int		rc = 0, err = 0;

	if ((addr < BCM5221_AUX_MODE4) || (addr > BCM5221_AUX_STATUS4)) {
		printf(" Error: Invalid Shadow register address (0x%X)\n", addr);
		return (-1);
	}

	// Enable Shadow reg	
	rc = bcm5221_shadow_reg_enable (fd, inst, port_addr, 1);
	if (rc) return (rc);

	err = bcm5221_reg_write(fd, inst, port_addr, addr, data);

	// Disable Shadow reg	
	rc = bcm5221_shadow_reg_enable (fd, inst, port_addr, 0);
	if (rc) return (rc);
	return (err? err: rc);
}
