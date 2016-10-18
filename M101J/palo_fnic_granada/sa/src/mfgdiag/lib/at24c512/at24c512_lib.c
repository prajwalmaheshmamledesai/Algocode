/********************************************************************
 *
 *      File:   at24c512_lib.c 
 *      Name:   Sudharshan Kadari
 *
 *      Description:
 *       AD7416 - Temp Sensor Access routines. 
 *
 *
 * Copyright (c) 1985-2007 by Cisco Systems, Inc.
 * All rights reserved.
 *
 *********************************************************************/

#include <stdio.h>
#include <sys/types.h>
#include "ppc_lib.h"

int at24c512_rd (int fd, uint16_t i2c_addr, uint16_t offset, uint8_t *data, int rd_len)
{
	return (diag_i2c_rd(fd, i2c_addr, offset, sizeof(offset), data, rd_len));
}

int at24c512_wr (int fd, uint16_t i2c_addr, uint16_t offset, uint8_t *data, int wr_len)
{
	return (diag_i2c_wr(fd, i2c_addr, offset, sizeof(offset), data, wr_len));
}
