/********************************************************************
 *
 *      File:  adt7473_lib.c
 *      Name:  Sudharshan Kadari 
 *  
 *      Description: ad7473 chip access routines
 *  
 * Copyright (c) 1985-2009 by Cisco Systems, Inc.
 * All rights reserved.
 *  
 *  
 * $Id:$
 * $Source:$
 * $Author:$
 * ---------------------------------------------------------------
 **********************************************************************/
/*******************************************************************//**
@file    adt7473_lib.c
@author  Sudharshan Kadari -- skadari@nuovasystems.com
@brief   adt7473 chip access routines

************************************************************************/
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

#include "adt7473.h"
#include "ppc_lib.h"

extern int verbose;
extern uint16_t diag_get_next_i2c_addr(uint16_t i2c_addr);

int
adt7473_rd(int fd, uint16_t i2c_addr, uint8_t offset, 
	   uint32_t len, uint8_t *val )
{
	int rc = 0;
	uint16_t next_i2c_addr;
	rc = diag_i2c_rd(fd, i2c_addr, offset, sizeof(offset), val, len);
	if (rc) {
		next_i2c_addr = diag_get_next_i2c_addr(i2c_addr);
		printf("  Warn - Try next I2C Addr (0x%X)\n", next_i2c_addr);
		rc = diag_i2c_rd(fd, next_i2c_addr, offset, sizeof(offset), val, len);
		if (rc) {
			printf("  FAIL - next I2C Addr (0x%X)\n", next_i2c_addr);
			return (rc); 
		} 
		rc = diag_i2c_rd(fd, i2c_addr, offset, sizeof(offset), val, len);
		if (rc) {
			printf("  Failed - retry after next I2C Addr (0x%X), i2c_addr=0x%X\n", 
				next_i2c_addr, i2c_addr);
			return (rc); 
		}
	}
	return (rc);
//        return (diag_i2c_rd(fd, i2c_addr, offset, sizeof(offset), val, len));
}

int
adt7473_wr(int fd, uint16_t i2c_addr, uint8_t offset, 
	   uint32_t len, uint8_t *val )
{
        return (diag_i2c_wr(fd, i2c_addr, offset, sizeof(offset), val, len));
}

/*
 * Fan Access Routines
 */
int
adt7473_pwm_rd( int fd, uint16_t i2c_addr, uint8_t reg, uint8_t *val )
{
    return (adt7473_rd (fd, i2c_addr, reg, 1, ( uint8_t *)val ) );
}

int
adt7473_pwm_wr( int fd, uint16_t i2c_addr, uint8_t reg, uint8_t val )
{
    return (adt7473_wr (fd, i2c_addr, reg, 1, ( uint8_t *)&val ) );
}

int
adt7473_tach_rd (int fd, uint16_t i2c_addr, uint8_t reg, uint16_t *val )
{
    int rc = 0;
    uint8_t tach = 0;

    if ( ( rc = adt7473_rd(fd, i2c_addr, reg, 1, &tach ) ) != 0 ) {
        return ( rc );
    }

    *val  = (uint16_t)tach;

    if ( ( rc = adt7473_rd(fd, i2c_addr, reg + 1, 1, &tach)) != 0 ) {
        return ( rc );
    }

    *val |= (uint16_t)(tach << 8);

    return ( rc );
}

int
adt7473_tach_to_rpm ( uint32_t *val )
{
    *val = (( 90000 * 60 ) / *val);
    return 0;
}

int adt7343_rpm_rd (int fd, uint16_t i2c_addr, uint8_t reg, uint32_t *speed)
{
	int rc = 0;
	uint16_t data16;

	rc = adt7473_tach_rd(fd, i2c_addr, reg, &data16);
	if (rc) {
		printf("Error: i2c(0x%X), reg(0x%X)\n", i2c_addr, reg);
	}
	*speed = (uint32_t)data16;

	adt7473_tach_to_rpm(speed);

	if (verbose)
	    printf("Tach to RPM [0x%X] : %d\n", data16, *speed);

	return (0);
}


/*
 * Temperature Access Routines
 */
int
adt7473_ts_rd (int fd, uint16_t i2c_addr, int reg, int8_t *val )
{
    int rc = 0;
    uint8_t temp;   

    if ((rc = adt7473_rd (fd, i2c_addr, reg, 1, &temp)) < 0 ) {
        return (rc);
    }

    *val = (int8_t)temp;

    return (0);
}

int
adt7473_to_c ( int val )
{
    val *= 0xff;
    val /= 0xffff;
    val -= 64;

    return ( val );
}

int adt7473_local_ts_rd (int fd, uint16_t i2c_addr, int8_t *val)
{
	int 	 rc = 0;
	rc = adt7473_ts_rd(fd, i2c_addr, ADT7473_REG_LCL_TEMP, val);
	if (rc) {
		printf("Error: Local TS rd (i2c=0x%X), rc=%d\n", i2c_addr, rc);
		return (rc);
	}

	return (rc);
}

// GPIO
int adt7473_gpio_init (int fd, uint16_t i2c_addr, uint8_t gpio, 
			uint8_t output, uint8_t pol, uint8_t int_msk)
{
	int	rc;
	uint8_t data;

	// Set the GPIO Pin input/output and the active hi/lo
	rc = adt7473_rd(fd, i2c_addr, ADT7473_REG_CFG5, 1, &data);
	if (rc) {
		printf("Error: %s: rd i2c(0x%X) addr(0x%X)\n", 
				__func__, i2c_addr, ADT7473_REG_CFG5);
		return ( rc );
	}

	data &= ~0x0C; // Clear bits 2,3
	data |= ((output << 1) | pol);

	rc = adt7473_wr(fd, i2c_addr, ADT7473_REG_CFG5, 1, &data);
	if (rc) {
		printf("Error: %s: rd i2c(0x%X) addr(0x%X)\n", 
				__func__, i2c_addr, ADT7473_REG_CFG5);
		return ( rc );
	}

	// Enable GPIO Model
	rc = adt7473_rd(fd, i2c_addr, ADT7473_REG_CFG4, 1, &data);
	if (rc) {
		printf("Error: %s: rd i2c(0x%X) addr(0x%X)\n", 
				__func__, i2c_addr, ADT7473_REG_CFG4);
		return ( rc );
	}

	// Enable GPIO Mode.
	data |= 0x03; 

	rc = adt7473_wr(fd, i2c_addr, ADT7473_REG_CFG4, 1, &data);
	if (rc) {
		printf("Error: %s: rd i2c(0x%X) addr(0x%X)\n", 
				__func__, i2c_addr, ADT7473_REG_CFG4);
		return ( rc );
	}
	return (rc);
}

#define AD7473_GPIO_BIT	5
int adt7473_gpio_rd (int fd, uint16_t i2c_addr, uint8_t gpio, uint8_t *stat)
{
	int rc = 0;
	uint8_t data;

	rc = adt7473_rd(fd, i2c_addr, ADT7473_REG_IRQ_STAT2, 1, &data);
	if (rc) {
		printf("Error: %s: rd i2c(0x%X) addr(0x%X)\n", 
			__func__, i2c_addr, ADT7473_REG_IRQ_STAT2);
		return ( rc );
	}

	*stat = (data & (0x01 << AD7473_GPIO_BIT)) ? 1 : 0;

	return (rc);
}


int adt7473_gpio_wr (int fd, uint16_t i2c_addr, uint8_t gpio, uint8_t stat)
{
	int rc = 0;
	uint8_t data;

	rc = adt7473_rd(fd, i2c_addr, ADT7473_REG_IRQ_STAT2, 1, &data);
	if (rc) {
		printf("Error: %s: rd i2c(0x%X) addr(0x%X)\n", 
			__func__, i2c_addr, ADT7473_REG_IRQ_STAT2);
		return ( rc );
	}

	if (stat) {
		data |= (0x01 << AD7473_GPIO_BIT);
	} else {
		data &= ~(0x01 << AD7473_GPIO_BIT);
	}

	rc = adt7473_wr(fd, i2c_addr, ADT7473_REG_IRQ_STAT2, 1, &data);
	if (rc) {
		printf("Error: %s: wr i2c(0x%X) addr(0x%X)\n", 
			__func__, i2c_addr, ADT7473_REG_IRQ_STAT2);
		return ( rc );
	}
	return (rc);
}
