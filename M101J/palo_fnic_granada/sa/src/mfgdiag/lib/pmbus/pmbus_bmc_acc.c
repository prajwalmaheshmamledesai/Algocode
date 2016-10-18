/********************************************************************
 *
 *      File:   pmbus_lib.c
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
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "ppc_lib.h"

extern int I2CByteRead_Block ( int Fh, unsigned char Bus, unsigned char Dev, unsigned char Reg, unsigned char *rxBuf, int bytes, int verbose);
extern int I2CByteWrite_Block (int Fh, unsigned char Bus, unsigned char Dev, unsigned char *txBuf, int bytes, int verbose);
extern int I2CByteRead_8BitIndex(int Fh, unsigned char Bus, unsigned char Dev, unsigned char Reg, unsigned char *data, int verbose);
extern int I2CByteWrite_8BitIndex (int Fh, unsigned char Bus, unsigned char Dev, unsigned char Reg, unsigned char Val, int send, int verbose);

int pmbus_rd(int fd, uint8_t bus, uint8_t i2c_addr, 
	     uint8_t addr, uint8_t *data)
{
        return(I2CByteRead_8BitIndex(fd, bus, i2c_addr, addr, data, 1));
}

int pmbus_wr (int fd, uint8_t bus, uint8_t i2c_addr, 
	      uint8_t addr, uint8_t data)
{
	return(I2CByteWrite_8BitIndex (fd, bus, i2c_addr, addr, data, 0, 1));
}

int pmbus_broadcast_wr(int fd, uint8_t bus, uint8_t i2c_addr, 
		uint8_t addr, uint8_t data)
{
	return(I2CByteWrite_8BitIndex (fd, bus, i2c_addr, addr, data, 0, 1));
}

int pmbus_rd16(int fd, uint8_t bus, uint8_t i2c_addr, uint8_t addr, 
		uint16_t *data)
{
        return(I2CByteRead_Block(fd, bus, i2c_addr, addr, (uint8_t*)data, 2, 0));
}

int pmbus_wr16(int fd, uint8_t bus, uint8_t i2c_addr, uint8_t addr, uint16_t *data)
{
        uint8_t wr_data[3];

        wr_data[0] = addr;
        wr_data[1] = (*data >> 8) & 0xFF;
        wr_data[2] = *data & 0xFF;

        //return(I2CByteWrite_Block(fd, bus, i2c_addr, addr, wr_data, 3, 0));
        return(I2CByteWrite_Block(fd, bus, i2c_addr, wr_data, 3, 0));
}

int pmbus_broadcast_wr16(int fd, uint8_t bus, uint8_t i2c_addr, uint8_t addr, uint16_t *data)
{
	return(pmbus_wr16(fd, bus, i2c_addr, addr, data));
}

int pmbus_rd_blk( int fd, uint8_t bus, uint8_t i2c_addr,
                        uint8_t addr, uint8_t *rdBuf, int rdLen)
{
        return(I2CByteRead_Block(fd, bus, i2c_addr, addr, rdBuf, rdLen, 0));
}

int pmbus_wr_blk( int fd, uint8_t bus, uint8_t i2c_addr,
                        uint8_t addr, uint8_t *wrBuf, int wrLen)
{
	uint8_t wr_data[128];

	if (wrLen > 127) {
		printf("  Block Write supports 127 bytes only\n");
		return (-1);
	}
	wr_data[0] = addr;
	memcpy(wr_data+1, wrBuf, wrLen);
	return(I2CByteWrite_Block(fd, bus, i2c_addr, wr_data, wrLen+1, 0));
}

int pmbus_open (int bus)
{
	return(ppc_i2c_open (bus));
}

void pmbus_close (int fd)
{
	close (fd);
}
