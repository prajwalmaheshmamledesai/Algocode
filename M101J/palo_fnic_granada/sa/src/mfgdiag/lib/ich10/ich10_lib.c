/********************************************************************
 *
 *      File:   adm1178_lib.c 
 *
 *      Description:
 *       adm1178_lib - Digtial Power Monitor
 *
 *
 * Copyright (c) 1985-2007 by Cisco Systems, Inc.
 * All rights reserved.
 *
 *********************************************************************/

#include <stdio.h>
#include <stdint.h>
#include "ich10.h"

extern int I2CByteRead_8BitIndex(int Fh, unsigned char Bus, unsigned char Dev,
		unsigned char Reg, unsigned char *data, int verbose);
extern int I2CByteWrite_8BitIndex(int Fh, unsigned char Bus, unsigned char Dev,
		unsigned char Reg, unsigned char Val, int send, int verbose);

int ich10_i2c_rd (int fd, uint8_t bus, uint8_t reg, uint8_t *data) {
	return I2CByteRead_8BitIndex(fd, bus, ICH10_ADDR, reg, data, 0);
}

int ich10_i2c_wr (int fd, uint8_t bus, uint8_t reg, uint8_t data) {
	return I2CByteWrite_8BitIndex(fd, bus, ICH10_ADDR, reg, data, 0, 0);
}
