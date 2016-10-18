/********************************************************************
 *
 *      File:   PCA9555_lib.c 
 *      Name: 
 *
 *      Description:
 *       PCA9555- GPIO 
 *
 *
 * Copyright (c) 1985-2007 by Cisco Systems, Inc.
 * All rights reserved.
 *
 *********************************************************************/

#include <stdio.h>
#include <stdint.h>
#include "pca9555.h"


extern int I2CByteRead_8BitIndex(int Fh, unsigned char Bus, unsigned char Dev,
                          unsigned char Reg, unsigned char *data, int verbose);
extern int I2CByteWrite_8BitIndex (int Fh, unsigned char Bus, unsigned char Dev,
                        unsigned char Reg, unsigned char Val, int send, int verbose);

int pca9555_rd (int fd, uint8_t bus, uint8_t i2c_addr, uint8_t reg, uint8_t *data)
{
        return(I2CByteRead_8BitIndex(fd, bus, i2c_addr, reg, data, 0));
}

int pca9555_wr (int fd, uint8_t bus, uint8_t i2c_addr, uint8_t reg, uint8_t data)
{
        return(I2CByteWrite_8BitIndex (fd, bus, i2c_addr, reg, data, 0, 0));
}

// IO 0-7 -- port0: 0-7 	IO 8-15: -- port1: 0-7
int pca9555_gpio_rd(int fd, uint8_t bus, uint8_t i2c_addr, uint8_t gpio, uint8_t *val) {
	int rc=0;
	uint8_t data;
	// read configuration
	rc = pca9555_rd(fd, bus, i2c_addr, PCA9555_CFG_0 + (gpio/8), &data);
	if( rc ) {
		printf("Error: can not read from PCA9555 CFG reg\n");
		return rc;
	}
	// check if the pin is configured as input	
	if( !(data & (0x01 << (gpio%8))) ) {
		data |= (0x01<<(gpio%8));
		rc = pca9555_wr(fd, bus, i2c_addr, PCA9555_CFG_0 + (gpio/8), data);
		if( rc ) {
			printf("Error: can not write to PCA9555 CFG reg\n");
			return rc;
		}
	}
	// read data
	rc = pca9555_rd(fd, bus, i2c_addr, PCA9555_INPUT_0 + (gpio/8), &data);
	if( rc ) {
		printf("Error: can not read from PCA9555 INPUT reg\n");
		return rc;
	}

	*val = data & (0x01 << (gpio%8));
	
	return rc;
}

// IO 0-7 -- port0: 0-7 	IO 8-15: -- port1: 0-7
int pca9555_gpio_wr(int fd, uint8_t bus, uint8_t i2c_addr, uint8_t gpio, uint8_t val) {
	int rc=0;
	uint8_t data;
	// read outout data
	rc = pca9555_rd(fd, bus, i2c_addr, PCA9555_OUTPUT_0 + (gpio/8), &data);
	if( rc ) {
		printf("Error: can not read from PCA9555 CFG reg\n");
		return rc;
	}
	// change output data reg 	
	data |= (val << (gpio%8));	
	// write to output data reg	
	rc = pca9555_wr(fd, bus, i2c_addr, PCA9555_OUTPUT_0 + (gpio/8), data);
	if( rc ) {
		printf("Error: can not write to PCA9555 OUTPUT reg\n");
		return rc;
	}
	// make sure direction is output	
	if( data & (0x01 << (gpio%7)) ) {
		data &= ~(0x01<<(gpio%7));
		rc = pca9555_wr(fd, bus, i2c_addr, PCA9555_CFG_0 + (gpio/8), data);
		if( rc ) {
			printf("Error: can not write to PCA9555 CFG reg\n");
			return rc;
		}
	}
	
	return rc;
}
