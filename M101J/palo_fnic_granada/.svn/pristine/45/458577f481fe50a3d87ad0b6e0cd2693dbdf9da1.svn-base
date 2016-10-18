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
#include "w83793.h"

extern int I2CByteRead_8BitIndex(int Fh, unsigned char Bus, unsigned char Dev,
                          unsigned char Reg, unsigned char *data, int verbose);
extern int I2CByteWrite_8BitIndex (int Fh, unsigned char Bus, unsigned char Dev,
                        unsigned char Reg, unsigned char Val, int send, int verbose);

// Access Routines.
int
hwmon_rd(int fd, uint8_t i2c_bus, uint16_t i2c_addr, uint8_t offset, uint8_t *val )
{
        return(I2CByteRead_8BitIndex(fd, i2c_bus, i2c_addr, offset, val, 1));
}

int
hwmon_wr(int fd, uint8_t i2c_bus, uint16_t i2c_addr, uint8_t offset, uint8_t val )
{
        return(I2CByteWrite_8BitIndex (fd, i2c_bus, i2c_addr, offset, val, 0, 1));
}
