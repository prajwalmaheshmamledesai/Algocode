/********************************************************************
 *
 *      File:   pca9551_lib.h
 *      Name:   Sudharshan Kadari
 *
 *      Description:
 *       pca9551 - Header File
 *
 *
 * Copyright (c) 1985-2007 by Cisco Systems, Inc.
 * All rights reserved.
 *
 *********************************************************************/

#ifndef _PCA9551_LIB_H_
#define _PCA9551_LIB_H_

extern int pca9551_rd (int fd, uint16_t i2c, uint8_t offset, uint8_t *d);
extern int pca9551_wr (int fd, uint16_t i2c, uint8_t offset, uint8_t *d);

extern int pca9551_pwm_rd(int fd, uint16_t i2c, uint8_t idx, uint8_t *d); 
extern int pca9551_pwm_wr (int fd, uint16_t i2c, uint8_t idx, uint8_t d);

extern int pca9551_psc_rd(int fd, uint16_t i2c, uint8_t idx, uint8_t *d);
extern int pca9551_psc_wr(int fd, uint16_t i2c, uint8_t idx, uint8_t d);

extern int pca9551_led_rd(int fd, uint16_t i2c, uint8_t idx, uint8_t *d);
extern int pca9551_led_wr(int fd, uint16_t i2c, uint8_t id, uint8_t d, uint8_t on);

extern int pca9551_input_rd(int fd, uint16_t i2c, uint8_t *d);
extern int pca9551_init(int fd, uint16_t i2c);

#endif // _PCA9551_LIB_H_
