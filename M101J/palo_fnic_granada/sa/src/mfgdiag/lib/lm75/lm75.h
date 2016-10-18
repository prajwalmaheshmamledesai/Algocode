/********************************************************************
 *
 *      File:   lm75.h 
 *      Name:   Sudharshan Kadari
 *
 *      Description:
 *       lm75 - Temp Sensor Access routines. 
 *
 *
 * Copyright (c) 1985-2007 by Cisco Systems, Inc.
 * All rights reserved.
 *
 *********************************************************************/

#ifndef _LM75_H_
#define _LM75_H_

#define LM75_TEMPERATURE	0x00
#define LM75_CONFIG		0x01
#define LM75_TOS_SET_POINT	0x02
#define LM75_THYST_SET_POINT	0x03

extern int lm75_ts_rd (int fd, uint8_t bus, uint8_t i2c_addr, uint16_t *val);
extern int lm75_reg_dump (int fd, uint8_t bus, uint8_t i2c_addr);

#endif // _LM75_H_
