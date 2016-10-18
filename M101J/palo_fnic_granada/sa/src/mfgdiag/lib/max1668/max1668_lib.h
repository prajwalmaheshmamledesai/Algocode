/********************************************************************
 *
 *      File:   max1668_lib.h 
 *      Name:   Sudharshan Kadari
 *
 *      Description:
 *       max1668 - Temp Sensor Access routines. 
 *
 *
 * Copyright (c) 1985-2007 by Cisco Systems, Inc.
 * All rights reserved.
 *
 *********************************************************************/

#ifndef _MAX1668_LIB_H_
#define _MAX1668_LIB_H_


extern int max1668_rd (int fd, uint16_t i2c_addr, uint8_t offset, uint8_t *data);
extern int max1668_wr (int fd, uint16_t i2c_addr, uint8_t offset, uint8_t *data);
extern int max1668_to_c ( int val );
extern int max1668_ts_rd (int fd, uint16_t i2c_addr, int channel, uint8_t *val);
extern int max1668_temp_show (int fd, uint16_t i2c_addr, uint8_t mask,
				int8_t *max, int8_t *min);
extern int max1668_access_test (int fd, uint16_t i2c_addr, uint8_t mask,
				int8_t *max, int8_t *min);
extern int max1668_reg_dump (int fd, uint16_t i2c_addr);
extern int max1668_temp_get (int fd, uint16_t i2c_addr, uint8_t rmt, uint8_t *temp_s);

#endif // _MAX1668_LIB_H_
