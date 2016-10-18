/********************************************************************
 *
 *      File:   pmbus_lib.h
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

#ifndef _PMBUS_LIB_H_
#define _PMBUS_LIB_H_

extern int pmbus_rd(int fd, uint8_t bus, uint8_t i2c_Addr, uint16_t addr, uint8_t *data);
extern int pmbus_wr(int fd, uint8_t bus, uint8_t i2c_Addr, uint16_t addr, uint8_t data);
extern int pmbus_wr16(int fd, uint8_t bus, uint8_t i2c_addr, uint8_t addr, uint16_t *data);
extern int pmbus_rd16(int fd, uint8_t bus, uint8_t i2c_addr, uint8_t addr, uint16_t *data);
extern int pmbus_rd_blk( int fd, uint8_t bus, uint8_t i2c_addr,
                        uint16_t addr, uint8_t *rxBuf, int bytes);
extern int pmbus_wr_blk( int fd, uint8_t bus, uint8_t i2c_addr,
                        uint16_t addr, uint8_t *rxBuf, int bytes);
extern int pmbus_broadcast_wr(int, uint8_t, uint8_t, uint8_t, uint8_t);
extern int pmbus_broadcast_wr16(int, uint8_t, uint8_t, uint8_t, uint16_t*);

extern int pwr_info (int fd, uint8_t bus, uint8_t i2c_addr);
extern int pwr_state (int fd, uint8_t bus, uint8_t i2c_addr);
extern int ltc_state (int fd, uint8_t bus, uint8_t i2c_addr);
extern int pmbus_state (int fd, uint8_t bus, uint8_t i2c_addr);
extern int pmbus_margin (int fd, uint8_t bus, uint8_t i2c_addr, int pcent);
extern int pmbus_operation (int fd, uint8_t bus, uint8_t i2c_addr, uint8_t oper);
extern int pmbus_margin_lo (int fd, uint8_t bus, uint8_t i2c_addr);
extern int pmbus_margin_hi (int fd, uint8_t bus, uint8_t i2c_addr);
extern int pmbus_margin_norm (int fd, uint8_t bus, uint8_t i2c_addr);
extern int pmbus_info (int fd, uint8_t bus, uint8_t i2c_addr);
extern int pmbus_margin_test (int fd, uint8_t bus, uint8_t i2c_addr,
		     uint16_t vout_ref, uint16_t var, uint16_t *vread);

extern int pmbus_broadcast_operation (int fd, uint8_t bus, uint8_t i2c_addr, uint8_t oper);
extern int pmbus_broadcast_margin_lo (int fd, uint8_t bus, uint8_t i2c_addr);
extern int pmbus_broadcast_margin_hi (int fd, uint8_t bus, uint8_t i2c_addr);
extern int pmbus_broadcast_margin_norm (int fd, uint8_t bus, uint8_t i2c_addr);

extern int pmbus_open (int bus);
extern void pmbus_close(int fd);
extern int pmbus_data_literal_rd (int fd, uint8_t bus, uint8_t i2c_addr, 
		uint8_t addr, char *mystr, char *units);
extern int pmbus_data_mode_rd (int fd, uint8_t bus, uint8_t i2c_addr, 
		uint8_t addr, char *mystr, char *units);
extern int pmbus_mfr_rd (int fd, uint8_t bus, uint8_t i2c_addr, uint8_t addr, char *mystr);
extern int pmbus_scale_mode_rd (int fd, uint8_t bus, uint8_t i2c_addr, 
		uint8_t addr, char *mystr, char *units, int scale);
extern int pmbus_direct_rd (int fd, uint8_t bus, uint8_t i2c_addr,
                            uint8_t addr, char *mystr, char *units);
#endif // _PMBUS_LIB_H_
