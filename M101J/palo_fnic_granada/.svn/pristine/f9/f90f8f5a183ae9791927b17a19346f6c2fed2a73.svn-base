/********************************************************************
 *
 *      File:   ad79xx.h 
 *      Name:   Sudharshan Kadari
 *
 *      Description:
 *       ad79xx - Voltage Monitor 
 *
 *
 * Copyright (c) 1985-2009 by Cisco Systems, Inc.
 * All rights reserved.
 *
 *********************************************************************/

#ifndef _AD79XX_H_
#define _AD79XX_H_

#define AD79XX_CONVERSION_RESULT_REG	0x00
#define AD79XX_ALERT_STATUS_REG		0x01
#define AD79XX_CONFIGURATION_REG	0x02
#define AD79XX_CYCLE_TIMER_REG		0x03
#define AD79XX_DATA_LO_CH1_REG		0x04
#define AD79XX_DATA_HI_CH1_REG		0x05
#define AD79XX_DATA_HYST_CH1_REG	0x06
#define AD79XX_DATA_LO_CH2_REG		0x07
#define AD79XX_DATA_HI_CH2_REG		0x08
#define AD79XX_DATA_HYST_CH2_REG	0x09
#define AD79XX_DATA_LO_CH3_REG		0x0A
#define AD79XX_DATA_HI_CH3_REG		0x0B
#define AD79XX_DATA_HYST_CH3_REG	0x0C
#define AD79XX_DATA_LO_CH4_REG		0x0D
#define AD79XX_DATA_HI_CH4_REG		0x0E
#define AD79XX_DATA_HYST_CH4_REG	0x0F

#define AD79XX_VIN1_SEL			0x80
#define AD79XX_VIN2_SEL			0x90
#define AD79XX_VIN3_SEL			0xA0
#define AD79XX_VIN4_SEL			0xB0
#define AD79XX_VIN5_SEL			0xC0
#define AD79XX_VIN6_SEL			0xD0
#define AD79XX_VIN7_SEL			0xE0
#define AD79XX_VIN8_SEL			0xF0

#define AD79XX_VIN_OFF			0x10


typedef struct _ad79xx_channel_s_ {
	char *name;
	uint8_t bus;
	uint8_t i2c_addr;
	uint8_t chan;
	int low; // low range in mV
	int high; // high range in mV
} ad79xx_channel_t;

extern int ad79xx_reg_dump (int fd, uint8_t bus, uint8_t i2c_addr);
extern int ad79xx_chan_rd (int fd, uint8_t bus, uint8_t i2c_addr, uint8_t ch, uint16_t *data);
#endif // _AD79XX_H_
