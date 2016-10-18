/********************************************************************
 *
 *      File:   pca9539_lib.h
 *      Name:   Sudharshan Kadari
 *
 *      Description:
 *       pca9539 - Header File 
 *
 *
 * Copyright (c) 1985-2007 by Cisco Systems, Inc.
 * All rights reserved.
 *
 *********************************************************************/

#ifndef _PCA9539_LIB_H_
#define _PCA9539_LIB_H_

extern int pca9539_rd (int fd, uint16_t i2c_addr, uint8_t offset, uint8_t *data);
extern int pca9539_wr (int fd, uint16_t i2c_addr, uint8_t offset, uint8_t *data);
extern int pca9539_init (int fd, uint8_t i2c_addr);

extern int pca9539_gpio_rd (int fd, uint16_t i2c_addr, uint16_t *pdata);
extern int pca9539_gpio_wr (int fd, uint16_t i2c_addr, uint16_t *pdata);
extern int pca9539_gpio_set_dir (int fd, uint16_t i2c_addr, uint16_t *pdata);
extern int pca9539_gpio_get_dir (int fd, uint16_t i2c_addr, uint16_t *pdata);
extern int pca9539_gpio_set_pol_inv (int fd, uint16_t i2c_addr, uint16_t *data);

#endif // _PCA9539_LIB_H_
