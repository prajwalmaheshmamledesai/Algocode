/**************************************************************************
 *
 * 	File:	ami_mg9082.h
 *
 * 	Description:
 *
 *
 * Copyright (c) 1985-2007 by Cisco Systems, Inc.
 * All rights reserved.
 *
 *
 * ****************************************************************************/

#ifndef _AMI_MG9082_H_
#define _AMI_MG9082_H_

#define HD0_DATA		0x00
#define HD1_DATA                0x01
#define HD2_DATA                0x02
#define HD3_DATA                0x03
#define HD4_DATA                0x04
#define HD5_DATA                0x05
#define HD6_DATA                0x06
#define HD7_DATA                0x07

#define CONFIGURATION		0x30

#define TOTAL_DRIVES		0x34	// total supported drives, typically 4 or 8
#define SGPIO_CONFIG		0x35	// Bit 7-4: # of drives SGPOIO1; Bit 3-0: SGPIO0

#define SLOTS_USED_BITMASK	0x36

#define SLOTS_MATED_BITMASK	0x38

#define ACTIVITY_BITMASK	0x40
#define PRED_FAILURE_BITMASK	0x41
#define LOCATE_BITMASK		0x42
#define CRIT_ARRAY_BITMASK	0x43
#define FAIL_BITMASK		0x44
#define FAILED_ARRAY_BITMASK	0x45
#define REBUILD_BITMASK		0x46
#define HOTSPARE_BITMASK	0x47
#define SDO_BITMASK		0x48

#define SMBUS_ALERTS		0x50

#define SMBUS_EVENT		0x52

#define TEMP_SENSOR		0x56	// on chip temp in celsius
#define AMBIENT_TEMP		0x57	// not supported in FW version up to and including 003
#define TEMP_SENSOR_THRESH	0x58

#define FIRMWARE_VER		0x60
#define FIRMWARE_MAJOR		0x61
#define FIRMWARE_MINOR		0x62
#define CHIP_ID			0x63
#define CHIP_REV		0x64
#define SMBUS_TIMEOUT		0x65
#define LAMP_TEST_ENABLE	0x66

#define HD0_ODN1_FREQ           0x70
#define HD1_ODN1_FREQ           0x71
#define HD2_ODN1_FREQ           0x72
#define HD3_ODN1_FREQ           0x73
#define HD4_ODN1_FREQ           0x74
#define HD5_ODN1_FREQ           0x75
#define HD6_ODN1_FREQ           0x76
#define HD7_ODN1_FREQ           0x77
#define HD0_ODN2_FREQ		0x80
#define HD1_ODN2_FREQ           0x81
#define HD2_ODN2_FREQ           0x82
#define HD3_ODN2_FREQ           0x83
#define HD4_ODN2_FREQ           0x84
#define HD5_ODN2_FREQ           0x85
#define HD6_ODN2_FREQ           0x86
#define HD7_ODN2_FREQ           0x87

#define COMBINED_SLOT_DATA	0xA0


int ami_mg9082_rd (int fd, uint8_t bus, uint8_t i2c_addr, uint8_t reg, uint8_t *rdata);
int ami_mg9082_wr (int fd, uint8_t bus, uint8_t i2c_addr, uint8_t reg, uint8_t data);
int ami_mg9082_info_dump (int fd, uint8_t bus, uint8_t i2c_addr);
int ami_mg9082_fail_led_toggle(int fd, uint8_t bus, uint8_t i2c_addr, uint8_t led, uint8_t onoff);
#endif //mg9082.h


