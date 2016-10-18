/********************************************************************
 *
 *      File:   usb251x_lib.c 
 *
 *      Description:
 *       usb251x_lib - USB 2.0 Hi-Speed Hub Controller
 *
 *
 * Copyright (c) 1985-2007 by Cisco Systems, Inc.
 * All rights reserved.
 *
 *********************************************************************/

#include <stdio.h>
#include <stdint.h>
#include "usb251x.h"


extern int I2CByteRead_Block(int Fh, unsigned char Bus, unsigned char Dev, 
		unsigned char Reg, unsigned char *rxBuf, int bytes, int verbose);
extern int I2CByteWrite_BLock(int Fh, unsigned char Bus, unsigned char Dev, 
		unsigned char *txBuf, int bytes, int verbose);

int usb251x_block_rd (int fd, uint8_t bus, uint8_t i2c_addr, uint8_t reg, uint8_t *rdata, int rs) {
	if( rs > MAX_I2C_DATA_SIZE ) {
		printf("I2C read exceeds allowed read size %d bytes\n", MAX_I2C_DATA_SIZE);
		return -1;
	}
	uint8_t rbuf[40];
	int rc=0;
	rc = I2CByteRead_Block(fd, bus, i2c_addr, reg, rbuf, rs+1, 0);
	memcpy(rdata, rbuf+1, rs); // get rid of byte count (first byte) and return actual data
	return rc;
}

int usb251x_block_wr (int fd, uint8_t bus, uint8_t i2c_addr, uint8_t reg, uint8_t *wdata, int ws) {
	if( ws > MAX_I2C_DATA_SIZE ) {
		printf("I2C write exceeds allowed write size %d bytes\n", MAX_I2C_DATA_SIZE);
		return -1;
	}
	uint8_t wbuf[40];
	wbuf[0] = reg;
	wbuf[1] = ws;  
	memcpy(wbuf+2, wdata, ws); // insert register val and byte count
	return I2CByteWrite_Block(fd, bus, i2c_addr, wbuf, ws+2, 0);
}
// treating UTF-16 as ASCII
static int ShowString(int fd, uint8_t bus, uint8_t i2c_addr, int start, int length) {
	if( length > MAX_STRING_LEN ) {
		printf("exceeding MAX STRING LENGTH %d\n", MAX_STRING_LEN);
		return -1;
	}
	uint8_t buf[4];
	int rc=0, i;
	for(i=0; i<length; i++) {
		if( rc = usb251x_block_rd(fd, bus, i2c_addr, start+i, buf, 2) ) return rc;
		printf("%c", (char)buf[0]);
	}

	printf("\n");
	return rc;
}

int usb251x_info_dump(int fd, uint8_t bus, uint8_t i2c_addr) {
	int rc=0;
	unsigned char rbuf[4];
	uint8_t len;
	// read vendor id
	if( rc = usb251x_block_rd(fd, bus, i2c_addr, VENDOR_ID_LSB, rbuf, 2) ) return rc;
	printf("VENDOR ID: 0x%02x%02x\n", rbuf[1], rbuf[0]);
	
	// read prodect id
	if( rc = usb251x_block_rd(fd, bus, i2c_addr, PRODUCT_ID_LSB, rbuf, 2) ) return rc;
	printf("PRODUCT ID: 0x%02x%02x\n", rbuf[1], rbuf[0]);

	// read device id
	if( rc = usb251x_block_rd(fd, bus, i2c_addr, DEVICE_ID_LSB, rbuf, 2) ) return rc;
	printf("DEVICE ID: 0x%02x%02x\n", rbuf[1], rbuf[0]);

	// get manufacturer string size and show string content
	printf("MANUFACTURER STRING INFO:\n");
	if( rc = usb251x_block_rd(fd, bus, i2c_addr, MFR_STRING_LEN, &len, 1) ) return rc;
	if( rc = ShowString(fd, bus, i2c_addr, MFR_STRING_START, len) ) return rc;

	// product string
	printf("PRODUCT STRING INFO:\n");
	if( rc = usb251x_block_rd(fd, bus, i2c_addr, PRODUCT_STRING_LEN, &len, 1) ) return rc;
	if( rc = ShowString(fd, bus, i2c_addr, PRODUCT_STRING_START, len) ) return rc;
	
	// serial string
	printf("SERIAL STRING INFO:\n");
	if( rc = usb251x_block_rd(fd, bus, i2c_addr, SERIAL_STRING_LEN, &len, 1) ) return rc;         
	if( rc = ShowString(fd, bus, i2c_addr, SERIAL_STRING_START, len) ) return rc;
	
	return rc;
}

