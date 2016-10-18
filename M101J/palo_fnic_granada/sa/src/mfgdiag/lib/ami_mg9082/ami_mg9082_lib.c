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
#include "ami_mg9082.h"

#define GET_BIT(data, x)	((data >> x) & 0x01) // get bit x

extern int I2CByteRead_8BitIndex(int Fh, unsigned char Bus, unsigned char Dev,
		unsigned char Reg, unsigned char *data, int verbose);
extern int I2CByteWrite_8BitIndex(int Fh, unsigned char Bus, unsigned char Dev,
		unsigned char Reg, unsigned char Val, int send, int verbose);

int ami_mg9082_rd (int fd, uint8_t bus, uint8_t i2c_addr, uint8_t reg, uint8_t *rdata) {
	return I2CByteRead_8BitIndex(fd, bus, i2c_addr, reg, rdata, 0);
}

int ami_mg9082_wr (int fd, uint8_t bus, uint8_t i2c_addr, uint8_t reg, uint8_t data) {
	return I2CByteWrite_8BitIndex(fd, bus, i2c_addr, reg, data, 0, 0);
}

int ami_mg9082_fail_led_toggle(int fd, uint8_t bus, uint8_t i2c_addr, uint8_t led, uint8_t onoff) {
	int rc=0;
	uint8_t data=0;

	
	if( rc = ami_mg9082_rd(fd, bus, i2c_addr, FAIL_BITMASK, &data) ) return rc;


	//printf("before toggle data = 0x%X   ", data);
	
	if( onoff ) data |= (0x01 << led);
	else data &= ~(0x01 << led);
	
	//printf("after toggle data = 0x%X\n", data);
	
	if( rc = ami_mg9082_wr(fd, bus, i2c_addr, FAIL_BITMASK, data) ) return rc;

	return rc;
}

// built-in LAMP TEST by writing non zero value to register 0x66
// clear the register to stop 
int ami_mg9082_lamp_test(int fd, uint8_t bus, uint8_t i2c_addr) {
	int rc = 0;
	if( rc = ami_mg9082_wr(fd, bus, i2c_addr, LAMP_TEST_ENABLE, 0x01) ) return rc;
	printf("AMI MG9082 LAMP TEST . . . End in 30 secs\n");
	fflush(stdout);
	sleep(30);
	if( rc = ami_mg9082_wr(fd, bus, i2c_addr, LAMP_TEST_ENABLE, 0x00) ) return rc;
	printf("LAMP TEST is compelete\n");
	return rc;
}

int ami_mg9082_info_dump (int fd, uint8_t bus, uint8_t i2c_addr) {
	int rc = 0;
	uint8_t data[4];
	// get firmware version
	if( rc = ami_mg9082_rd(fd, bus, i2c_addr, FIRMWARE_VER, &data[0]) ) return rc;
        if( rc = ami_mg9082_rd(fd, bus, i2c_addr, FIRMWARE_MAJOR, &data[1]) ) return rc;
        if( rc = ami_mg9082_rd(fd, bus, i2c_addr, FIRMWARE_MINOR, &data[2]) ) return rc;
	printf("Firmware Ver: 0x%02x Major: 0x%02x Minor: 0x%02x\n", data[0], data[1], data[2]);
	
	// get chip info
	if( rc = ami_mg9082_rd(fd, bus, i2c_addr, CHIP_ID, &data[0]) ) return rc; 
	if( rc = ami_mg9082_rd(fd, bus, i2c_addr, CHIP_REV, &data[1]) ) return rc;
	printf("Chip ID: 0x%02x Chip Rev: 0x%02x\n", data[0], data[1]);
	
	// get configuration 
	if( rc = ami_mg9082_rd(fd, bus, i2c_addr, CONFIGURATION, &data[0]) ) return rc;
	printf("Current Configuration: 0x%02x\n", data[0]);
	
	// get supported drive info
	if( rc = ami_mg9082_rd(fd, bus, i2c_addr, TOTAL_DRIVES, &data[0]) ) return rc;
	if( rc = ami_mg9082_rd(fd, bus, i2c_addr, SGPIO_CONFIG, &data[1]) ) return rc;
	printf("Total Number of Supported Drive: %d\n", data[0]);
	printf("SGPIO Configuration:\n");
	printf("\tNumber of Drives SGPIO_0: %d  Number of Drives SGPIO_1: %d\n",
			data[1] & 0x0f, (data[1] & 0xf0) >> 4);
	
	// get slots used and mated status
	if( rc = ami_mg9082_rd(fd, bus, i2c_addr, SLOTS_USED_BITMASK, &data[0]) ) return rc;
	if( rc = ami_mg9082_rd(fd, bus, i2c_addr, SLOTS_MATED_BITMASK, &data[1]) ) return rc;
	printf("Hard Disk:     |0|1|2|3|4|5|6|7|\n");
	printf("Sloted Used:   |%d|%d|%d|%d|%d|%d|%d|%d|  -- [0 = unused slot / 1 = used slot]\n",
			GET_BIT(data[0],0), GET_BIT(data[0],1), GET_BIT(data[0],2), GET_BIT(data[0],3),
			GET_BIT(data[0],4), GET_BIT(data[0],5), GET_BIT(data[0],6), GET_BIT(data[0],7));
	printf("Sloted Used:   |%d|%d|%d|%d|%d|%d|%d|%d|  -- [0 = drive absent / 1 = drive mated]\n",
			GET_BIT(data[1],0), GET_BIT(data[1],1), GET_BIT(data[1],2), GET_BIT(data[1],3),
			GET_BIT(data[1],4), GET_BIT(data[1],5), GET_BIT(data[1],6), GET_BIT(data[1],7));
	
	// get chip & ambient temp
	if( rc = ami_mg9082_rd(fd, bus, i2c_addr, TEMP_SENSOR, &data[0]) ) return rc;
	if( rc = ami_mg9082_rd(fd, bus, i2c_addr, AMBIENT_TEMP, &data[1]) ) return rc;
	printf("On-chip Temperature: %d C\n", data[0]);
	printf("Ambient Temperature: %d C\n", data[1]);
	// LED test
	if( rc = ami_mg9082_lamp_test(fd, bus, i2c_addr) ) return rc;
	return rc;
}

