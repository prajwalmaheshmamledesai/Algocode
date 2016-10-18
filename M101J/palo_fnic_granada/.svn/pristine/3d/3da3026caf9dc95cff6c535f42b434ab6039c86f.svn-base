/********************************************************************
 *
 *      File:   PCA9539_lib.c 
 *      Name:   Sudharshan Kadari
 *
 *      Description:
 *       PCA9539- GPIO 
 *
 *
 * Copyright (c) 1985-2007 by Cisco Systems, Inc.
 * All rights reserved.
 *
 *********************************************************************/

#include <stdio.h>
#include <sys/types.h>
#include "ppc_lib.h"
#include "pca9539_lib.h"

int pca9539_rd (int fd, uint16_t i2c_addr, uint8_t offset, uint8_t *data)
{
	return (diag_i2c_rd(fd, i2c_addr, offset, sizeof(offset), data, sizeof(uint8_t)));
}

int pca9539_wr (int fd, uint16_t i2c_addr, uint8_t offset, uint8_t *data)
{
	return (diag_i2c_wr(fd, i2c_addr, offset, sizeof(offset), data, sizeof(uint8_t)));
}
