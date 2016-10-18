#include <stdio.h>
#include <sys/types.h>
#include "mdio_lib.h"

extern int prt_mdio_clause22_write     (uint8_t  inst, uint32_t mdio_num, uint32_t reg_addr,
                                         uint32_t prt_addr, uint32_t reg_data);
extern int prt_mdio_clause22_read      (uint8_t  inst, uint32_t mdio_num, uint32_t reg_addr,
                                         uint32_t prt_addr, uint32_t *reg_data);

int bcm54980_reg_read (int fd, uint8_t bcm_inst, uint8_t port_addr,
                        uint8_t reg_addr, uint16_t *data)
{
	int rc = 0;
	uint32_t reg_data;

	rc = prt_mdio_clause22_read (0, (uint32_t)bcm_inst, (uint32_t)port_addr, 
		(uint32_t)reg_addr, &reg_data);

	*data = (uint16_t) reg_data;
	return (rc);
}

int bcm54980_reg_write (int fd, uint8_t bcm_inst, uint8_t port_addr,
                        uint8_t reg_addr, uint16_t data)
{
	return (prt_mdio_clause22_write (0, bcm_inst, port_addr, 
		reg_addr, (uint32_t)data));
}

void bcm54x80_octphy_unreset(int asic_id)
{
}

void bcm54x80_octphy_reset(int asic_id)
{
}

