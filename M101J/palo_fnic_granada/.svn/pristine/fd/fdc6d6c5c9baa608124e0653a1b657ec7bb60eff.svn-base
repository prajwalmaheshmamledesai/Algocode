/********************************************************************
 *
 *      File:   ics932.c
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
#include <string.h>

#define ICS932_REG_COUNT	22
#define ICS932_SRC_OUTPUT	0x00
#define ICS932_CPU_REF_OP_ENA	0x01
#define ICS932_PCI_CLK_ENA	0x02
#define ICS932_CLKF_SRC_STOP	0x03
#define ICS932_PWR_DOWN_CTRL	0x04
#define ICS932_OP_SPECTURUM	0x05
#define ICS932_TEST_REG		0x06
#define ICS932_VENDOR_REVISION	0x07
#define ICS932_BYTE_COUNT	0x08
#define ICS932_DEVICE_ID	0x09
#define ICS932_PROG_CTRL	0x0A
#define ICS932_FREQ_CTRL_0	0x0B
#define ICS932_FREQ_CTRL_1	0x0C
#define ICS932_CPU_SPECT_0	0x0D
#define ICS932_CPU_SPECT_1	0x0E
#define ICS932_SRC_FREQ_0	0x0F
#define ICS932_SRC_FREQ_1	0x10
#define ICS932_SRC_SPECT_0	0x11
#define ICS932_SRC_SPECT_1	0x12
#define ICS932_OP_DIVIDER	0x13
#define ICS932_SRC_PCI_OP_DIV   0x14
#define ICS932_TEST_BYTE	0x15

extern int I2CByteRead_Block ( int Fh, unsigned char Bus, unsigned char Dev, unsigned char Reg, unsigned char *rxBuf, int bytes, int verbose);
extern int I2CByteWrite_Block (int Fh, unsigned char Bus, unsigned char Dev, unsigned char *txBuf, int bytes, int verbose);

int ics932_rd(int fd, uint8_t bus, uint8_t i2c_addr, 
		 uint8_t addr, uint8_t *data, uint8_t data_sz)
{
        return(I2CByteRead_Block(fd, bus, i2c_addr, addr, data, data_sz, 0));
}

int ics932_wr(int fd, uint8_t bus, uint8_t i2c_addr, 
	 	 uint8_t addr, uint8_t *data, uint8_t data_sz)
{
        uint8_t wr_data[256+1];

        wr_data[0] = addr;
	memcpy(wr_data+1, data, data_sz);

        return(I2CByteWrite_Block(fd, bus, i2c_addr, wr_data, data_sz+1, 0));
}

static char *ics_reg_names[] = {
	"SRC_OUTPUT",
	"CPU_REF_OP_ENA",
	"PCI_CLK_ENA",
	"CLKF_SRC_STOP",
	"PWR_DOWN_CTRL",
	"OP_SPECTURUM",
	"TEST_REG",
	"VENDOR_REVISION",
	"BYTE_COUNT",
	"DEVICE_ID",
	"PROG_CTRL",
	"FREQ_CTRL_0",
	"FREQ_CTRL_1",
	"CPU_SPECT_0",
	"CPU_SPECT_1",
	"SRC_FREQ_0",
	"SRC_FREQ_1",
	"SRC_SPECT_0",
	"SRC_SPECT_1",
	"OP_DIVIDER",
	"SRC_PCI_OP_DIV",
	"TEST_BYTE",
	}; 

int ics932_dump (int fd, uint8_t bus, uint8_t i2c_addr)
{
	int rc;
	uint8_t data[ICS932_REG_COUNT+1], cnt;

	rc = ics932_rd(fd, bus, i2c_addr, ICS932_SRC_OUTPUT,
			  data, ICS932_REG_COUNT);
	if (rc) {
		printf("  Error: ics932 rd (rc=%d)\n", rc);
		return (rc);
	}

	for (cnt = 0; cnt < ICS932_REG_COUNT; cnt++) {
		printf("  %-18s : 0x%X\n", ics_reg_names[cnt], data[cnt+1]);
	}
	return (rc);
}
