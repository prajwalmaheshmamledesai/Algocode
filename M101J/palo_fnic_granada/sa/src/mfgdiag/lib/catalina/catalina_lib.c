/********************************************************************
 *
 *      File:   catalina_lib.c
 *      Name:   Sudharshan Kadari
 *
 *      Description:
 *       Digital PWM System Controller library 
 *
 *
 * Copyright (c) 1985-2009 by Cisco Systems, Inc.
 * All rights reserved.
 *
 *
 *********************************************************************/

#include <stdio.h>
#include <stdint.h>
#include "catalina.h"

extern int I2CByteRead_8BitIndex(int Fh, unsigned char Bus, unsigned char Dev, 
			  unsigned char Reg, unsigned char *data, int verbose);
extern int I2CByteWrite_8BitIndex (int Fh, unsigned char Bus, unsigned char Dev, 
			unsigned char Reg, unsigned char Val, int send, int verbose);
extern int I2CByteRead_Block ( int Fh, unsigned char Bus, unsigned char Dev,
                        unsigned char Reg, unsigned char *rxBuf, int bytes, int verbose);

int catalina_rd(int fd, uint8_t bus, uint8_t i2c_addr, uint8_t addr, uint8_t *data)
{
	return(I2CByteRead_8BitIndex(fd, bus, i2c_addr, addr, data, 1)); 
}

int catalina_wr(int fd, uint8_t bus, uint8_t i2c_addr, uint8_t addr, uint8_t data)
{
	return(I2CByteWrite_8BitIndex (fd, bus, i2c_addr, addr, data, 0, 1));
}
