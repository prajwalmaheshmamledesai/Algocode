/********************************************************************
 *
 *      File:   ds1338_fex.c
 *      Name:   Sudharshan Kadari
 *
 *      Description:
 *       DS1338 RTC Timer. 
 *
 *
 * Copyright (c) 1985-2008 by Cisco Systems, Inc.
 * All rights reserved.
 *
 *
 *********************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include "ppc_lib.h" 
#include "ds1338_reg.h" 

int ds1338_rd (int fd, uint8_t i2c_addr, uint8_t offset, uint8_t *data)
{
        int rc = 0;
        rc = diag_i2c_rd(fd, i2c_addr, offset, sizeof(offset),
                       (uint8_t*)data, sizeof(uint8_t));
        return (rc);
}

int ds1338_wr (int fd, uint8_t i2c_addr, uint8_t offset, uint8_t data)
{
        int rc = 0;
        rc = diag_i2c_wr(fd, i2c_addr, offset, sizeof(offset),
                (uint8_t*)&data, sizeof(uint8_t));
        return (rc);
}


int rtc_test (int fd, uint8_t i2c_addr, uint8_t *addr, uint8_t *exp, uint8_t *read)
{
	uint8_t save_data[DS1338_RAM55 - DS1338_RAM0 + 1];
	uint8_t write_data[DS1338_RAM55 - DS1338_RAM0 + 1];
	uint8_t read_data[DS1338_RAM55 - DS1338_RAM0 + 1];
	uint8_t cnt, seed = 0xAC;
	int rc = 0;

	// Save Ram Data
	printf(" RTC Save Ram Data\n");
	for (cnt = 0; cnt < (sizeof(save_data)/sizeof(uint8_t)); cnt++) {
		rc = ds1338_rd(fd, i2c_addr, cnt, &save_data[cnt]);
		if (rc) {
			printf(" Error: RTC Read (i2c=0x%X, addr=0x%X)\n", i2c_addr, cnt);
			return (rc);
		}
	}

	// Create Seed Data and write.
	srand(seed);
	printf(" RTC Write Ram Data\n");
	for (cnt = 0; cnt < (sizeof(write_data)/sizeof(uint8_t)); cnt++) {
		write_data[cnt] = (uint8_t)rand();
		rc = ds1338_wr(fd, i2c_addr, cnt, write_data[cnt]);
		if (rc) {
			printf(" Error: RTC Write (i2c=0x%X, addr=0x%X)\n", i2c_addr, cnt);
			return (rc);
		}
	}

	// Read and Verify.
	printf(" RTC Read/Verify Ram Data\n");
	for (cnt = 0; cnt < (sizeof(write_data)/sizeof(uint8_t)); cnt++) {
		rc = ds1338_rd(fd, i2c_addr, cnt, &read_data[cnt]);
		if (rc) {
			printf(" Error: RTC Read (i2c=0x%X, addr=0x%X)\n", i2c_addr, cnt);
			return (rc);
		}

		if (read_data[cnt] != write_data[cnt]) {
			*addr  = cnt;
			*exp   = write_data[cnt];
			*read  = read_data[cnt];
		}
	}

	// Restore Ram Data
	printf(" RTC Restore Ram Data\n");
	for (cnt = 0; cnt < (sizeof(save_data)/sizeof(uint8_t)); cnt++) {
		rc = ds1338_wr(fd, i2c_addr, cnt, save_data[cnt]);
		if (rc) {
			printf(" Error: RTC Write (i2c=0x%X, addr=0x%X)\n", i2c_addr, cnt);
			return (rc);
		}
	}

	return (rc);
}
