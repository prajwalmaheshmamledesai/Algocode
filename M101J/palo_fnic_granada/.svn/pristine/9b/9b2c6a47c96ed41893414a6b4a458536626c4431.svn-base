 /*********************************************************************
 *
 *      File:   ltc4215.h 
 *      Name:   Sudharshan Kadari
 *
 *      Description:
 *       ltc4215 - Header File
 *
 *
 * Copyright (c) 1985-2007 by Cisco Systems, Inc.
 * All rights reserved.
 *
 *********************************************************************/
#ifndef _LTC4215_H_
#define _LTC4215_H_

#define LTC_CONTROL 				0x00
#define LTC_CONTROL_FLD_GPIO1			7
#define LTC_CONTROL_FLD_GPIO0			6
#define LTC_CONTROL_FLD_TEST_ENA		5
#define LTC_CONTROL_FLD_MASS_WR_ENA		4
#define LTC_CONTROL_FLD_FET_ON			3
#define LTC_CONTROL_FLD_OVER_CURRENT_AR		2
#define LTC_CONTROL_FLD_UNDER_VOLT_AR		1
#define LTC_CONTROL_FLD_OVER_VOLT_AR		0

#define LTC_ALERT				0x01
#define LTC_ALERT_FLD_GPIO_OUTPUT		6
#define LTC_ALERT_FLD_FET_SHORT			5
#define LTC_ALERT_FLD_EN_STATE_CHG		4
#define LTC_ALERT_FLD_POWER_BAD			3
#define LTC_ALERT_FLD_OVER_CURRENT		2
#define LTC_ALERT_FLD_UNDER_VOLT		1
#define LTC_ALERT_FLD_OVER_VOLT			0

#define LTC_STATUS				0x02
#define LTC_STATUS_FLD_FET_ON			7
#define LTC_STATUS_FLD_GPIO_IN			6
#define LTC_STATUS_FLD_FET_SHORT		5
#define LTC_STATUS_FLD_EN			4
#define LTC_STATUS_FLD_POWER_BAD		3
#define LTC_STATUS_FLD_OVER_CURRENT		2
#define LTC_STATUS_FLD_UNDER_VOLT		1
#define LTC_STATUS_FLD_OVER_VOLT		0

#define LTC_FAULT				0x03
#define LTC_FAULT_FLD_FET_SHORT			5
#define LTC_FAULT_FLD_EN_CHG			4
#define LTC_FAULT_FLD_POWER_BAD			3
#define LTC_FAULT_FLD_OVER_CURRENT		2
#define LTC_FAULT_FLD_UNDER_VOLT		1
#define LTC_FAULT_FLD_OVER_VOLT			0

#define LTC_SENSE				0x04
#define LTC_SOURCE				0x05
#define LTC_ADIN				0x06

extern int ltc4215_rd (int fd, uint16_t i2c_addr, uint8_t offset, uint8_t *data);
extern int ltc4215_wr (int fd, uint16_t i2c_addr, uint8_t offset, uint8_t *data);
extern int ltc4215_power (int fd, uint16_t i2c_addr);
extern int ltc4215_access_test(int fd, uint16_t i2c_addr);

#define LTC_SOURCE_VOLTAGE_MV(data)     ((data * 15440)/255)
#define LTC_SENSE_VOLTAGE_UV(data)      ((data * 38450)/255)

#endif // _LTC4215_H_
