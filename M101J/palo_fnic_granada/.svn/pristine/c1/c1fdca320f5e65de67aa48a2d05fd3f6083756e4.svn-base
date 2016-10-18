/********************************************************************
 *
 *      File:   max1617_lib.h 
 *      Name:   Sudharshan Kadari
 *
 *      Description:
 *       max1617 - Temp Sensor Access routines. 
 *
 *
 * Copyright (c) 1985-2007 by Cisco Systems, Inc.
 * All rights reserved.
 *
 *********************************************************************/

#ifndef _MAX1617_LIB_H_
#define _MAX1617_LIB_H_


extern int max1617_rd (int fd, uint16_t i2c_addr, uint8_t offset, uint8_t *data);
extern int max1617_wr (int fd, uint16_t i2c_addr, uint8_t offset, uint8_t *data);
extern int max1617_to_c ( int val );
extern int max1617_ts_rd (int fd, uint16_t i2c_addr, int channel, uint8_t *val);
extern int max1617_temp_show (int fd, uint16_t i2c_addr, int8_t min, int8_t max);
extern int max1617_access_test (int fd, uint16_t i2c_addr, int8_t min, int8_t max);
extern int max1617_reg_dump (int fd, uint16_t i2c_addr);

#endif // _MAX1617_LIB_H_
