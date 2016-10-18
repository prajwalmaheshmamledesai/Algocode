/********************************************************************
 *
 *      File:   ucd92xx_lib.h
 *      Name:   Sudharshan Kadari
 *
 *      Description:
 *       Digital PWM System Controller library  header file
 *
 *
 * Copyright (c) 1985-2009 by Cisco Systems, Inc.
 * All rights reserved.
 *
 *
 *********************************************************************/

#ifndef _UCD92XX_LIB_H_
#define _UCD92XX_LIB_H_

extern int ucd92xx_rd(int fd, uint8_t bus, uint8_t i2c_Addr, uint8_t addr, uint8_t *data);
extern int ucd92xx_wr(int fd, uint8_t bus, uint8_t i2c_Addr, uint8_t addr, uint8_t data);
extern int ucd92xx_wr16(int fd, uint8_t bus, uint8_t i2c_addr, uint8_t addr, uint16_t *data);
extern int ucd92xx_rd16(int fd, uint8_t bus, uint8_t i2c_addr, uint8_t addr, uint16_t *data);
extern int ucd_state (int fd, uint8_t bus, uint8_t i2c_addr);
extern int ucd_margin (int fd, uint8_t bus, uint8_t i2c_addr, int pcent);
extern int ucd_operation (int fd, uint8_t bus, uint8_t i2c_addr, uint8_t oper);
extern int ucd_margin_lo (int fd, uint8_t bus, uint8_t i2c_addr);
extern int ucd_margin_hi (int fd, uint8_t bus, uint8_t i2c_addr);
extern int ucd_margin_norm (int fd, uint8_t bus, uint8_t i2c_addr);
extern int ucd_info (int fd, uint8_t bus, uint8_t i2c_addr);
extern int ucd_margin_test (int fd, uint8_t bus, uint8_t i2c_addr,
		     uint16_t vout_ref, uint16_t var, uint16_t *vread);
extern int ucd_voltage_margin_lo (int fd, uint8_t bus, uint8_t i2c_addr, 
				  uint16_t voltage, uint8_t percentMargin);
extern int ucd_voltage_margin_hi (int fd, uint8_t bus, uint8_t i2c_addr, 
				  uint16_t voltage, uint8_t percentMargin);


#endif // _UCD92XX_LIB_H_
