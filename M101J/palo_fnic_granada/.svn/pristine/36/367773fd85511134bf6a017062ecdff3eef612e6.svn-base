/********************************************************************
 *
 *      File:   lm75_lib.c 
 *      Name:   Sudharshan Kadari
 *
 *      Description:
 *       lm75_lib - Temp Sensor Access routines. 
 *
 *
 * Copyright (c) 1985-2007 by Cisco Systems, Inc.
 * All rights reserved.
 *
 *********************************************************************/

#include <stdio.h>
#include <stdint.h>
#include "lm75.h"

extern int I2CByteRead_Block ( int Fh, unsigned char Bus, unsigned char Dev,
		unsigned char Reg, unsigned char *rxBuf, int bytes, int verbose);
extern int I2CByteWrite_Block (int Fh, unsigned char Bus, unsigned char Dev, 
		unsigned char *txBuf, int bytes, int verbose);


int lm75_rd (int fd, uint8_t bus, uint8_t i2c_addr, uint8_t offset, uint16_t *data)
{
        return(I2CByteRead_Block(fd, bus, i2c_addr, offset, (uint8_t*)data, 2, 0));
}

int lm75_wr (int fd, uint8_t bus, uint8_t i2c_addr, uint8_t offset, uint16_t *data)
{
	uint8_t wr_data[3];

	wr_data[0] = offset;
	wr_data[1] = (*data >> 8) & 0xFF;
	wr_data[2] = *data & 0xFF;

        return(I2CByteWrite_Block(fd, bus, i2c_addr, wr_data, 3, 0));
}

void lm75_to_c_dump ( uint16_t val ) 
{
    printf("  LM75 Temperature Read : %d.%d\n", (val & 0xFF), ((val>>8)&0x80? 5 : 0)); 
}

int lm75_ts_rd (int fd, uint8_t bus, uint8_t i2c_addr, uint16_t *val)
{
	int rc = 0;

	rc = lm75_rd (fd, bus, i2c_addr, 0x00, val);
	if (!rc) {
		printf("Bus=%d I2c=0x%X data=0x%X\n", bus, i2c_addr, *val);
		lm75_to_c_dump(*val);
	}
	return (rc);
} 

int lm75_reg_dump (int fd, uint8_t bus, uint8_t i2c_addr)
{
	int   rc = 0, cnt;
	uint16_t data = 0x00;
	char *lm75_reg_name[4] = { "LM75_TEMPERATURE", "LM75_CONFIG",
				    "LM75_TOS_SET_POINT", "LM75_THYST_SET_POINT"};

	for (cnt = 0; cnt < 4; cnt++) {
		rc = lm75_rd (fd, bus, i2c_addr, cnt, &data);
		if (rc) {
			printf("Error: reading lm75 reg %d\n", cnt);
			return (rc);
		}
		if (cnt == 1) {
			printf("  %-22s : 0x%X\n", lm75_reg_name[cnt], data);
		} else {
			printf("  %-22s : 0x%X (%d.%d)\n", lm75_reg_name[cnt], data,
				(data & 0xFF), ((data>>8)&0x80? 5 : 0));
		}
	}

	return (rc);
}
