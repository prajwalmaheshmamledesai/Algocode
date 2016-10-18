/********************************************************************
 *
 *      File:   ics9db403.c
 *      Name:   Sudharshan Kadari
 *
 *      Description:
 *       Clock Buffer ICS chip lib 
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

#define ICS9DB403_REG_COUNT		7
#define ICS9DB403_FREQUENCY_SELECT	0x00
#define ICS9DB403_OUTPUT_CONTROL_0	0x01
#define ICS9DB403_OUTPUT_CONTROL_1	0x02
#define ICS9DB403_OUTPUT_CONTROL_2	0x03
#define ICS9DB403_VENDOR_REVISION	0x04
#define ICS9DB403_DEVICE_ID		0x05
#define ICS9DB403_BYTE_COUNT		0x06

extern int I2CByteRead_Block ( int Fh, unsigned char Bus, unsigned char Dev, unsigned char Reg, unsigned char *rxBuf, int bytes, int verbose);
extern int I2CByteWrite_Block (int Fh, unsigned char Bus, unsigned char Dev, unsigned char *txBuf, int bytes, int verbose);

int ics9db403_rd(int fd, uint8_t bus, uint8_t i2c_addr, 
		 uint8_t addr, uint8_t *data, uint8_t data_sz)
{
        return(I2CByteRead_Block(fd, bus, i2c_addr, addr, data, data_sz, 0));
}

int ics9db403_wr(int fd, uint8_t bus, uint8_t i2c_addr, 
	 	 uint8_t addr, uint8_t *data, uint8_t data_sz)
{
        uint8_t wr_data[256+1];

        wr_data[0] = addr;
	memcpy(wr_data+1, data, data_sz);

        return(I2CByteWrite_Block(fd, bus, i2c_addr, wr_data, data_sz+1, 0));
}

static char *ics_reg_names[] = {
	"FREQUENCY_SELECT",
	"OUTPUT_CONTROL_0",
	"OUTPUT_CONTROL_1",
	"OUTPUT_CONTROL_2",
	"VENDOR_REVISION",
	"DEVICE_ID",
	"BYTE_COUNT",
	}; 

int ics9db403_dump (int fd, uint8_t bus, uint8_t i2c_addr)
{
	int rc;
	uint8_t data[ICS9DB403_REG_COUNT+1], cnt;

	rc = ics9db403_rd(fd, bus, i2c_addr, ICS9DB403_FREQUENCY_SELECT,
			  data, ICS9DB403_REG_COUNT);
	if (rc) {
		printf("  Error: ics9db403 rd (rc=%d)\n", rc);
		return (rc);
	}

	for (cnt = 0; cnt < ICS9DB403_REG_COUNT; cnt++) {
		printf("  %-18s : 0x%X\n", ics_reg_names[cnt], data[cnt+1]);
	}
	return (rc);
}
