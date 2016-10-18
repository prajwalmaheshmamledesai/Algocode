/********************************************************************
 *
 *      File:   pca9551.h
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

#ifndef _PCA9551_H_
#define _PCA9551_H_

#define PCA9551_REG_INPUT	0x00
#define PCA9551_REG_PSC0	0x01
#define PCA9551_REG_PWM0	0x02
#define PCA9551_REG_PSC1	0x03
#define PCA9551_REG_PWM1	0x04
#define PCA9551_REG_LS0		0x05
#define PCA9551_REG_LS1		0x06

#define PCA9551_LED_ON		0x00
#define PCA9551_LED_OFF		0x01
#define PCA9551_BLINK_0		0x02
#define PCA9551_BLINK_1		0x03
#endif // _PCA9551_H_
