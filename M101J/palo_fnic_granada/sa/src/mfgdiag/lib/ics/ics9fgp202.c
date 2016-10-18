/********************************************************************
 *
 *      File:   ics9fgp202.c
 *      Name:   Sudharshan Kadari
 *
 *      Description:
 *       Timing Hub for Intel CPU's. 
 *
 *
 * Copyright (c) 1985-2009 by Cisco Systems, Inc.
 * All rights reserved.
 *
 *
 *********************************************************************/

#include <stdio.h>
#include <stdint.h>

#define ICS9FGP202_REG_COUNT	19
#define ICS9FGP202_FREQ_SELECT		0x00
#define ICS9FGP202_RMII_OUTPUT		0x01
#define ICS9FGP202_CPUCLK_CTRL		0x02
#define ICS9FGP202_RMII_STRENGTH	0x03
#define ICS9FGP202_RMII_STRENGTH_CTRL	0x04
#define ICS9FGP202_CLK_STRENGTH_CTRL	0x05
#define ICS9FGP202_VENDOR_REVISION	0x06
#define ICS9FGP202_DEVICE_ID		0x07
#define ICS9FGP202_BYTE_COUNT		0x08
#define ICS9FGP202_RESERVED_0		0x09
#define ICS9FGP202_PLL_PROG_ENA		0x0A
#define ICS9FGP202_PLL_VCO_FREQ_M	0x0B
#define ICS9FGP202_PLL_VCO_FREQ_N	0x0C
#define ICS9FGP202_PLL_SPECT_CTRL_L	0x0D
#define ICS9FGP202_PLL_SPECT_CTRL_H	0x0E
#define ICS9FGP202_PLL_FREQ_CTRL_M	0x0F
#define ICS9FGP202_PLL_FREQ_CTRL_N	0x10
#define ICS9FGP202_DOT_PLL_CTRL_L	0x11
#define ICS9FGP202_DOT_PLL_CTRL_H	0x12


int ics9fgp202_rd(int fd, uint8_t bus, uint8_t i2c_addr, 
		 uint8_t addr, uint8_t *data, uint8_t data_sz)
{
        return(I2CByteRead_Block(fd, bus, i2c_addr, addr, data, data_sz, 0));
}

int ics9fgp202_wr(int fd, uint8_t bus, uint8_t i2c_addr, 
	 	 uint8_t addr, uint8_t *data, uint8_t data_sz)
{
        uint8_t wr_data[256+1];

        wr_data[0] = addr;
	memcpy(wr_data+1, data, data_sz);

        return(I2CByteWrite_Block(fd, bus, i2c_addr, wr_data, data_sz+1, 0));
}

static char *ics_reg_names[] = {
	"FREQ_SELECT",
	"RMII_OUTPUT",
	"CPUCLK_CTRL",
	"RMII_STRENGTH",
	"RMII_STRENGTH_CTRL",
	"CLK_STRENGTH_CTRL",
	"VENDOR_REVISION",
	"DEVICE_ID",
	"BYTE_COUNT",
	"RESERVED_0",
	"PLL_PROG_ENA",
	"PLL_VCO_FREQ_M",
	"PLL_VCO_FREQ_N",
	"PLL_SPECT_CTRL_L",
	"PLL_SPECT_CTRL_H",
	"PLL_FREQ_CTRL_M",
	"PLL_FREQ_CTRL_N",
	"DOT_PLL_CTRL_L",
	"DOT_PLL_CTRL_H",
	}; 

int ics9fgp202_dump (int fd, uint8_t bus, uint8_t i2c_addr)
{
	int rc;
	uint8_t data[ICS9FGP202_REG_COUNT+1], addr, cnt;

	memset(data, 0, sizeof(data));
	rc = ics9fgp202_rd(fd, bus, i2c_addr, ICS9FGP202_FREQ_SELECT,
			  data, ICS9FGP202_REG_COUNT);
	if (rc) {
		printf("  Error: ics9fgp202 rd (rc=%d)\n", rc);
		return (rc);
	}

	for (cnt = 0; cnt < ICS9FGP202_REG_COUNT; cnt++) {
		printf("  %-18s : 0x%X\n", ics_reg_names[cnt], data[cnt+1]);
	}
	return (rc);
}
