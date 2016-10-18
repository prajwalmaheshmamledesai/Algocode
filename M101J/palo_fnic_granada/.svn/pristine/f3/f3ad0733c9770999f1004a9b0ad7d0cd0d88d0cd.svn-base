/********************************************************************
 *
 *      File:   PCA9551_lib.c 
 *      Name:   Sudharshan Kadari
 *
 *      Description:
 *       PCA9551- GPIO 
 *
 *
 * Copyright (c) 1985-2007 by Cisco Systems, Inc.
 * All rights reserved.
 *
 *********************************************************************/

#include <stdio.h>
#include <stdint.h>
#include "ppc_lib.h"
#include "pca9551.h"
#include "pca9551_lib.h"

extern int I2CByteRead_8BitIndex(int Fh, unsigned char Bus, unsigned char Dev,
                          unsigned char Reg, unsigned char *data, int verbose);
extern int I2CByteWrite_8BitIndex (int Fh, unsigned char Bus, unsigned char Dev,
                        unsigned char Reg, unsigned char Val, int send, int verbose);

int pca9551_rd (int fd, uint16_t i2c_addr, uint8_t addr, uint8_t *data)
{
        return(I2CByteRead_8BitIndex(fd, 1, i2c_addr, addr, data, 1));
}

int pca9551_wr (int fd, uint16_t i2c_addr, uint8_t addr, uint8_t *data)
{
        return(I2CByteWrite_8BitIndex (fd, 1, i2c_addr, addr, *data, 0, 1));
}

int pca9551_pwm_rd(int fd, uint16_t i2c_addr, uint8_t idx, 
			uint8_t *data)
{
        return(pca9551_rd(fd, i2c_addr, idx ? PCA9551_REG_PWM1 : 
				PCA9551_REG_PWM0, data));
}

int pca9551_pwm_wr (int fd, uint16_t i2c_addr, uint8_t idx,
                        uint8_t data)
{
        return(pca9551_wr(fd, i2c_addr, idx ? PCA9551_REG_PWM1 : 
				PCA9551_REG_PWM0, &data));
}


int pca9551_psc_rd(int fd, uint16_t i2c_addr, uint8_t idx, uint8_t *data)
{
        return(pca9551_rd(fd, i2c_addr, idx ? PCA9551_REG_PSC1 : 
				PCA9551_REG_PSC0, data));
}

int pca9551_psc_wr(int fd, uint16_t i2c_addr, uint8_t idx, 
		uint8_t data)
{
        return(pca9551_wr(fd, i2c_addr, idx ? PCA9551_REG_PSC1 : 
				PCA9551_REG_PSC0, &data));
}


int pca9551_input_rd(int fd, uint16_t i2c_addr, uint8_t *data)
{
        return(pca9551_rd(fd, i2c_addr, PCA9551_REG_INPUT, data));
}

int pca9551_led_rd(int fd, uint16_t i2c_addr, uint8_t idx, uint8_t *data)
{
	int rc = 0;
        rc = pca9551_rd(fd, i2c_addr, (idx <= 3)? PCA9551_REG_LS0:
				PCA9551_REG_LS1, data);
        return rc;
} 

int pca9551_led_wr(int fd, uint16_t i2c_addr, uint8_t idx, 
			uint8_t data, uint8_t on)
{
        int rc;
        int shift = (idx % 4) * 2;
        int reg = (idx <= 3) ? PCA9551_REG_LS0 : PCA9551_REG_LS1;
        uint8_t val;

        rc = pca9551_rd(fd, i2c_addr, reg, &val);
        if (rc) return (rc);

        val &= ~(0x03 << shift);
        val |=  ((data &3) << shift); 

        return (pca9551_wr(fd, i2c_addr, reg, &val));
}

int pca9551_init(int fd, uint16_t i2c_addr)
{
        int rc;

        /* Values 1 sec and 0.5 sec for on & off */
        rc = pca9551_psc_wr(fd, i2c_addr, 0, 37);
        if (rc) return rc;

        return(pca9551_pwm_wr(fd, i2c_addr, 0, 128));
}
