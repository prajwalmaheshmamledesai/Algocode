/********************************************************************
 *
 *      File:   red_sim_acc.c
 *      Name:   Sudharshan Kadari
 *
 *      Description:
 *       Redwood simulation access routines. 
 *
 *
 * Copyright (c) 1985-2007 by Cisco Systems, Inc.
 * All rights reserved.
 *
 *
 *********************************************************************/

#include <stdio.h>
#include <stdint.h>

extern unsigned int network_util_read_model_reg( unsigned int reg_addr , unsigned long *data);
extern unsigned int network_util_write_model_reg( unsigned int reg_addr, unsigned int reg_data );

int red_reg_rd(int addr, unsigned long *data)
{
	int rc = 0;
	printf("RD: [0x%08X] ", addr);
	rc = network_util_read_model_reg(addr, data);
	if (rc) {
		printf("Error: Failed to read 0x%08X\n", addr);
		return (rc);
	}
	printf(": 0x%08X\n", (unsigned int)*data);
	return (0);
}
int red_reg_wr(int addr, unsigned long data)
{
	int rc = 0;
	printf("WR: [0x%08X] : 0x%08X\n", addr, (unsigned int)data);
	rc = network_util_write_model_reg(addr, data);
	if (rc) {
		printf("Error: Failed to write 0x%08X @ 0x%08X rc = %d\n", (unsigned int)data, addr, rc);
		return (rc);
	}
	return (0);
}
