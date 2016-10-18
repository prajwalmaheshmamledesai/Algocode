/********************************************************************
 *
 *      File:   pca9555.h
 *      Name: 
 *
 *      Description:
 *       pca9555 - Header File 
 *
 *
 * Copyright (c) 1985-2007 by Cisco Systems, Inc.
 * All rights reserved.
 *
 *********************************************************************/

#ifndef _PCA9555_H_
#define _PCA9555_H_

#define PCA9555_INPUT_0		0x00
#define PCA9555_INPUT_1		0x01
#define PCA9555_OUTPUT_0	0x02
#define PCA9555_OUTPUT_1	0x03
#define PCA9555_POL_0		0x04
#define PCA9555_POL_1		0x05
#define PCA9555_CFG_0		0x06
#define PCA9555_CFG_1		0x07

extern int pca9555_rd(int fd, uint8_t bus, uint8_t i2c_addr, uint8_t reg, uint8_t *data);
extern int pca9555_wr(int fd, uint8_t bus, uint8_t i2c_addr, uint8_t reg, uint8_t data);
extern int pca9555_gpio_rd(int fd, uint8_t bus, uint8_t i2c_addr, uint8_t gpio, uint8_t *val);
extern int pca9555_gpio_wr(int fd, uint8_t bus, uint8_t i2c_addr, uint8_t gpio, uint8_t val);

#endif // _PCA9555_H_
