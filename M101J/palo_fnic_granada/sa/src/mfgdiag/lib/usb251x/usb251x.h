/********************************************************************
 *
 *      File:   usb251x.h 
 *
 *      Description:
 *       usb251x - USB 2.0 Hi-Speed Hub Controller
 *
 *
 * Copyright (c) 1985-2007 by Cisco Systems, Inc.
 * All rights reserved.
 *
 *********************************************************************/

#ifndef _USB251X_H_
#define _USB251X_H_

#define MAX_I2C_DATA_SIZE	32
#define MAX_STRING_LEN		31

#define VENDOR_ID_LSB		0x00
#define VENDOR_ID_MSB		0x01
#define PRODUCT_ID_LSB		0x02
#define PRODUCT_ID_MSB		0x03
#define DEVICE_ID_LSB		0x04 
#define DEVICE_ID_MSB		0x05
#define CFG_DATA_BYTE1		0x06
#define CFG_DATA_BYTE2		0x07
#define CFG_DATA_BYTE3		0x08
#define NON_REMOVABLE_DEV	0x09
#define PORT_DISABLE_SELF	0x0A
#define PORT_DISABLE_BUS	0x0B
#define MAX_POWER_SELF		0x0C
#define MAX_POWER_BUS		0x0D
#define HUB_CTRL_MAX_CUR_SELF	0x0E
#define HUB_CTRL_MAX_CUR_BUS	0x0F
#define POWER_ON_TIME		0x10
#define LANG_ID_HIGH		0x11
#define LANG_ID_LOW		0x12
#define MFR_STRING_LEN		0x13
#define PRODUCT_STRING_LEN	0x14
#define SERIAL_STRING_LEN	0x15
#define MFR_STRING_START	0x16
#define MFR_STRING_END		0x53
#define PRODUCT_STRING_START	0x54
#define PRODUCT_STRING_END	0x91
#define SERIAL_STRING_START	0x92 
#define SERIAL_STRING_END	0xCF
#define BAT_CHARGING_ENA	0xD0
#define BOOST_UP		0xF6
#define BOOST_7_5		0xF7
#define BOOST_X_0		0xF8
#define PORT_SWAP		0xFA
#define PORT_MAP_12		0xFB
#define PORT_MAP_34		0xFC
#define PORT_MAP_56		0xFD
#define PORT_MAP_7		0xFE
#define STATUS_COMMAND		0xFF


int usb251x_block_rd (int fd, uint8_t bus, uint8_t i2c_addr, uint8_t reg, uint8_t *rdata, int rs);
int usb251x_block_wr (int fd, uint8_t bus, uint8_t i2c_addr, uint8_t reg, uint8_t *wdata, int ws);
int usb251x_info_dump (int fd, uint8_t bus, uint8_t i2c_addr);
#endif // _USB251X_H_
