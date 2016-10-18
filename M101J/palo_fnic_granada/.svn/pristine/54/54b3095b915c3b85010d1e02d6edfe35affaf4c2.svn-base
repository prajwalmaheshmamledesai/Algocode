/********************************************************************
 *
 *      File:  adt7462.c
 *      Name:  Matt Strathman 
 *  
 *      Description: ad7462 chip access routines
 *  
 * Copyright (c) 1985-2007 by Cisco Systems, Inc.
 * All rights reserved.
 *  
 *  
 * $Id:$
 * $Source:$
 * $Author:$
 * ---------------------------------------------------------------
 **********************************************************************/
/*******************************************************************//**
@file    adt7462.h
@author  Matt Strathman -- mstrathman@nuovasystems.com
@brief   adt7462 chip access routines

************************************************************************/
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

#include "adt7462.h"
#include "ppc_lib.h"

extern uint16_t diag_get_next_i2c_addr(uint16_t i2c_addr);

int
adt7462_rd ( int fd, uint16_t i2c_addr, uint8_t offset, uint32_t len, uint8_t *val )
{
	int rc = 0;
	rc = diag_i2c_rd(fd, i2c_addr, offset, sizeof(offset), val, len);
#ifdef DIAG_IOM
	if (rc) {
		uint16_t next_i2c_addr;
		next_i2c_addr = diag_get_next_i2c_addr(i2c_addr);
		printf("  Warn - Try next I2C Addr (0x%X) (0x%X)\n", 
			next_i2c_addr, i2c_addr);
//		sleep(1);
		rc = diag_i2c_rd(fd, next_i2c_addr, offset, sizeof(offset), val, len);
		if (rc) {
			printf("  Failed - next I2C Addr (0x%X), i2c_addr=0x%X\n", next_i2c_addr, i2c_addr); 
			return (rc); 
		}
		rc = diag_i2c_rd(fd, i2c_addr, offset, sizeof(offset), val, len);
		if (rc) {
			printf("  Failed - retry after next I2C Addr (0x%X), i2c_addr=0x%X\n", 
				next_i2c_addr, i2c_addr);
			return (rc); 
		}
	}
#endif
	return (rc);
}

int
adt7462_wr (int fd, uint16_t i2c_addr, uint8_t offset, uint32_t len, uint8_t *val )
{
        return (diag_i2c_wr(fd, i2c_addr, offset, sizeof(offset), val, len));
}

/*
 * Fan Access Routines
 */
int
adt7462_pwm_rd ( int fd, uint16_t i2c_addr, uint8_t reg, uint8_t *val )
{
    return ( adt7462_rd (fd, i2c_addr, reg, 1, ( uint8_t *)val ) );
}

int
adt7462_pwm_wr ( int fd, uint16_t i2c_addr, uint8_t reg, uint8_t val )
{
    return ( adt7462_wr (fd, i2c_addr, reg, 1, ( uint8_t *)&val ) );
}

int
adt7462_tach_rd (int fd, uint16_t i2c_addr, uint8_t reg, uint16_t *val )
{
    int rc = 0;
    uint8_t tach = 0;

    if ( ( rc = adt7462_rd(fd, i2c_addr, reg, 1, &tach ) ) != 0 ) {
        return ( rc );
    }

    *val  = tach;

    if ( ( rc = adt7462_rd(fd, i2c_addr, reg + 1, 1, &tach)) != 0 ) {
        return ( rc );
    }

    *val |= tach << 8;

    return ( rc );
}

int
adt7462_tach_to_rpm ( uint32_t *val )
{
    *val = ( 90000 * 60 ) / 2 / *val;
    return 0;
}

int fan_tach_rd (int fd, uint16_t i2c_addr, uint8_t reg, uint32_t *speed)
{
	int rc = 0;
	uint16_t data16;

	 rc = adt7462_tach_rd(fd, i2c_addr, reg, &data16);
	if (rc) {
		printf("Error: i2c(0x%X), reg(0x%X)\n", i2c_addr, reg);
	}
	*speed = (uint32_t)data16;

	adt7462_tach_to_rpm(speed);

	return (0);
}


/*
 * Temperature Access Routines
 */
static int adt7462_ts_rd (int fd, uint16_t i2c_addr, int reg, uint16_t *val )
{
    int rc = 0;
    uint8_t temp;   

    if ( ( rc = adt7462_rd (fd, i2c_addr, reg, 1, &temp) ) < 0 ) {
        return ( rc );
    }

    *val = (uint16_t)temp;

    if (( rc = adt7462_rd(fd, i2c_addr, reg + 1, 1, &temp)) < 0 ) {
        return ( rc );
    }

    *val |= (uint16_t ) (temp << 8);

    return (0);
}

int
adt7462_to_c ( int val )
{
    val *= 0xff;
    val /= 0xffff;
    val -= 64;

    return ( val );
}

int adt7462_local_ts_rd (int fd, uint16_t i2c_addr, uint32_t *val)
{
	uint16_t data;
	int 	 rc = 0;
	rc = adt7462_ts_rd(fd, i2c_addr, ADT7462_REG_LCL_TMP_LSB, &data);
	if (rc) {
		printf("Error: Local TS rd (i2c=0x%X), rc=%d\n", i2c_addr, rc);
		return (rc);
	}

	*val = adt7462_to_c(data);

	return (rc);
}

int adt7462_remote_ts_rd (int fd, uint16_t i2c_addr, int channel, uint32_t *val)
{
	uint16_t data;
	int 	 rc = 0;
        /* 
         * assuming channel is 1-based
         * channel 1 == Sensor 1 etc
         */
        if (channel < 1 || channel > 3) {
		printf("Error: Remote TS channel=%d doesn't exist\n", 
                       channel);
		return (rc=-1);
        }
	rc = adt7462_ts_rd(fd, i2c_addr, 
                           (ADT7462_REG_RMT1_TMP_LSB + ((channel-1)*2)), 
                           &data);
	if (rc) {
		printf("Error: Local TS rd (i2c=0x%X), rc=%d\n", i2c_addr, rc);
		return (rc);
	}

	*val = adt7462_to_c(data);
	return (rc);
}

// GPIO

int adt7462_gpio_init (int fd, uint16_t i2c_addr, uint8_t gpio, 
			uint8_t output, uint8_t pol, uint8_t int_msk)
{
	int	rc;
	uint8_t gpio_addr = (gpio < 4) ? ADT7462_REG_GPIO1_B : ADT7462_REG_GPIO2_B;
	uint8_t data, lcl_gpio = gpio&0x03; 
	uint8_t mask = (0x03 << (lcl_gpio*2));

	// Set the GPIO Pin input/output and the active hi/lo
	rc = adt7462_rd(fd, i2c_addr, gpio_addr, 1, &data);
	if (rc) {
		printf("Error: %s: rd i2c(0x%X) addr(0x%X)\n", 
				__func__, i2c_addr, gpio_addr);
		return ( rc );
	}

	data &= ~mask;
	data |= (((output << 1) | pol) << (lcl_gpio * 2));

	rc = adt7462_wr(fd, i2c_addr, gpio_addr, 1, &data);
	if (rc) {
		printf("Error: %s: rd i2c(0x%X) addr(0x%X)\n", 
				__func__, i2c_addr, gpio_addr);
		return ( rc );
	}

	// Mask interrupt bit 
	gpio_addr = ADT7462_REG_GPIO_MASK;
	rc = adt7462_rd(fd, i2c_addr, gpio_addr, 1, &data);
	if (rc) {
		printf("Error: %s: rd i2c(0x%X) addr(0x%X)\n", 
				__func__, i2c_addr, gpio_addr);
		return ( rc );
	}

	if (int_msk) {
		data |= (0x01 << gpio);
	} else {
		data &= ~(0x01 << gpio);
	}

	rc = adt7462_wr(fd, i2c_addr, gpio_addr, 1, &data);
	if (rc) {
		printf("Error: %s: rd i2c(0x%X) addr(0x%X)\n", 
				__func__, i2c_addr, gpio_addr);
		return ( rc );
	}

	// Mask EDO assertion too.
	if (gpio < 4) {
		for (gpio_addr = ADT7462_REG_EDO_MASK1; gpio_addr <= ADT7462_REG_EDO_MASK2; gpio_addr++) {
			rc = adt7462_rd(fd, i2c_addr, gpio_addr, 1, &data);
			if (rc) {
				printf("Error: %s: rd i2c(0x%X) addr(0x%X)\n", 
					__func__, i2c_addr, gpio_addr);
				return ( rc );
			}

			if (int_msk) {
				data |= (0x01 << gpio);
			} else {
				data &= ~(0x01 << gpio);
			}

			rc = adt7462_wr(fd, i2c_addr, gpio_addr, 1, &data);
			if (rc) {
				printf("Error: %s: rd i2c(0x%X) addr(0x%X)\n", 
					__func__, i2c_addr, gpio_addr);
				return ( rc );
			}
		}
	}
	
	// Enable GPIO 
	gpio_addr = ADT7462_REG_CONFIG3;
	rc = adt7462_rd(fd, i2c_addr, gpio_addr, 1, &data);
	if (rc) {
		printf("Error: %s: rd i2c(0x%X) addr(0x%X)\n", 
				__func__, i2c_addr, gpio_addr);
		return ( rc );
	}

	data |= ADT7462_CFG3_GPIO_EN; 

	rc = adt7462_wr(fd, i2c_addr, gpio_addr, 1, &data);
	if (rc) {
		printf("Error: %s: rd i2c(0x%X) addr(0x%X)\n", 
				__func__, i2c_addr, gpio_addr);
		return ( rc );
	}

	return (rc);
}

int adt7462_gpio_rd (int fd, uint16_t i2c_addr, uint8_t gpio, uint8_t *stat)
{
	int rc = 0;
	uint8_t data;

	rc = adt7462_rd(fd, i2c_addr, ADT7462_REG_GPIO_STAT, 1, &data);
	if (rc) {
		printf("Error: %s: rd i2c(0x%X) addr(0x%X)\n", 
			__func__, i2c_addr, ADT7462_REG_GPIO_STAT);
		return ( rc );
	}

	*stat = (data & (0x01 << gpio)) ? 1 : 0;

	return (rc);
}
int adt7462_gpio_wr (int fd, uint16_t i2c_addr, uint8_t gpio, uint8_t stat)
{
	int rc = 0;
	uint8_t data;

	rc = adt7462_rd(fd, i2c_addr, ADT7462_REG_GPIO_STAT, 1, &data);
	if (rc) {
		printf("Error: %s: rd i2c(0x%X) addr(0x%X)\n", 
			__func__, i2c_addr, ADT7462_REG_GPIO_STAT);
		return ( rc );
	}

	if (stat) {
		data |= (0x01 << gpio);
	} else {
		data &= ~(0x01 << gpio);
	}

	rc = adt7462_wr(fd, i2c_addr, ADT7462_REG_GPIO_STAT, 1, &data);
	if (rc) {
		printf("Error: %s: wr i2c(0x%X) addr(0x%X)\n", 
			__func__, i2c_addr, ADT7462_REG_GPIO_STAT);
		return ( rc );
	}
	return (rc);
}
