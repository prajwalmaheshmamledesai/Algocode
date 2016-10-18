/********************************************************************
 *
 *      File:   at24c.c 
 *      Name:   Harry Zhao
 *
 *      Description:
 *       
 *
 *
 * Copyright (c) 1985-2007 by Cisco Systems, Inc.
 * All rights reserved.
 *
 *********************************************************************/

#include <stdio.h>
#include <stdint.h>
#include "at24c.h"

extern int I2CByteRead_Block(int Fh, unsigned char Bus, unsigned char Dev,
        unsigned char Reg, unsigned char *rxBuf, int bytes, int verbose);

extern int I2CByteWrite_Block(int Fh, unsigned char Bus, unsigned char Dev, 
        unsigned char *txBuf, int bytes, int verbose);

int at24c_rd(int fd, uint8_t bus, uint8_t i2c_addr, uint8_t offset, uint8_t *data, int len)
{
    return (I2CByteRead_Block(fd, bus, i2c_addr, offset, data, len, 0));
}

int at24c_wr(int fd, uint8_t bus, uint8_t i2c_addr, uint8_t offset, uint8_t *data, int len)
{
    return (I2CByteWrite_Block(fd, bus, i2c_addr, data, len, 0));
}
