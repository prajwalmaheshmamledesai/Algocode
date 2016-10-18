/********************************************************************
 *
 *      File:  fan_lib.c
 *      Name:  Sudharshan Kadari 
 *  
 *      Description: adt7476 chip access routines
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
#include <stdio.h>
#include <stdint.h>
#include "adt7476.h"
#include "fan_lib.h"

/*
 * Fan Access Routines
 */
int
fan_pwm_rd( int fd, uint8_t i2c_bus, uint16_t i2c_addr, uint8_t reg, uint8_t *val )
{
    return (fan_rd (fd,i2c_bus, i2c_addr, reg, ( uint8_t *)val ) );
}

int
fan_pwm_wr( int fd, uint8_t i2c_bus, uint16_t i2c_addr, uint8_t reg, uint8_t val )
{
    return (fan_wr (fd,i2c_bus, i2c_addr, reg, ( uint8_t *)&val ) );
}

int
fan_tach_rd (int fd, uint8_t i2c_bus, uint16_t i2c_addr, uint8_t reg, uint16_t *val )
{
    int rc = 0;
    uint8_t tach = 0;

    if ( ( rc = fan_rd(fd,i2c_bus, i2c_addr, reg, &tach ) ) != 0 ) {
        return ( rc );
    }

    *val  = (uint16_t)tach;

    if ( ( rc = fan_rd(fd,i2c_bus, i2c_addr, reg + 1, &tach)) != 0 ) {
        return ( rc );
    }

    *val |= (uint16_t)(tach << 8);

    return ( rc );
}

int
fan_tach_to_rpm ( uint32_t *val )
{
    if (*val) {
       *val = (( 90000 * 60 ) / *val);
    } else {
#ifdef DIAG_ALPINE
        *val = 0;
#else /* DIAG_ALPINE */
        return (-1);
#endif /* DIAG_ALPINE */
    }
    return 0;
}

int fan_rpm_rd (int fd, uint8_t i2c_bus, uint16_t i2c_addr, uint8_t reg, uint32_t *speed)
{
	int rc = 0;
	uint16_t data16;

	rc = fan_tach_rd(fd, i2c_bus, i2c_addr, reg, &data16);
	if (rc) {
		printf("Error: i2c(bus=0x%x, addr=0x%x), reg(0x%X)\n",i2c_bus, i2c_addr, reg);
	}
	*speed = (uint32_t)data16;

	return (fan_tach_to_rpm(speed));
}


/*
 * Temperature Access Routines
 */
int
fan_ts_rd (int fd, uint8_t i2c_bus, uint16_t i2c_addr, int reg, int8_t *val )
{
    int rc = 0;
    uint8_t temp;   

    if ((rc = fan_rd (fd,i2c_bus, i2c_addr, reg, &temp)) < 0 ) {
        return (rc);
    }

    *val = (int8_t)temp;

    return (0);
}

int
fan_to_c ( int val )
{
    val *= 0xff;
    val /= 0xffff;
    val -= 64;

    return ( val );
}

int fan_local_ts_rd (int fd, uint8_t i2c_bus, uint16_t i2c_addr, int8_t *val)
{
	int 	 rc = 0;
	rc = fan_ts_rd(fd,i2c_bus, i2c_addr, FAN_REG_LCL_TEMP, val);
	if (rc) {
		printf("Error: Local TS rd (bus=0x%x, addr=0x%x), rc=%d\n",i2c_bus, i2c_addr, rc);
		return (rc);
	}

	return (rc);
}

int fan_remote_1_ts_rd (int fd, uint8_t i2c_bus, uint16_t i2c_addr, int8_t *val)
{
	int 	 rc = 0;
	rc = fan_ts_rd(fd,i2c_bus, i2c_addr, FAN_REG_RMT1_TEMP, val);
	if (rc) {
		printf("Error: Remote_1 TS rd (bus=0x%x, addr=0x%x), rc=%d\n",i2c_bus, i2c_addr, rc);
		return (rc);
	}

	return (rc);
}

int fan_remote_2_ts_rd (int fd, uint8_t i2c_bus, uint16_t i2c_addr, int8_t *val)
{
	int 	 rc = 0;
	rc = fan_ts_rd(fd,i2c_bus, i2c_addr, FAN_REG_RMT2_TEMP, val);
	if (rc) {
		printf("Error: Remote_2 TS rd (bus=0x%x, addr=0x%x), rc=%d\n",i2c_bus, i2c_addr, rc);
		return (rc);
	}

	return (rc);
}

// GPIO
int fan_gpio_init (int fd, uint8_t i2c_bus, uint16_t i2c_addr, uint8_t gpio, 
			uint8_t output, uint8_t pol, uint8_t int_msk)
{
	int	rc;
	uint8_t data;

	// Set the GPIO Pin input/output and the active hi/lo
	rc = fan_rd(fd,i2c_bus, i2c_addr, FAN_REG_CFG5, &data);
	if (rc) {
		printf("Error: %s: rd i2c(bus=0x%x, addr=0x%x) reg(0x%x)\n", 
				__func__,i2c_bus, i2c_addr, FAN_REG_CFG5);
		return ( rc );
	}

	data &= ~0x0C; // Clear bits 2,3
	data |= ((output << 1) | pol);

	rc = fan_wr(fd,i2c_bus, i2c_addr, FAN_REG_CFG5, &data);
	if (rc) {
		printf("Error: %s: rd i2c(bus=0x%x, addr=0x%x) reg(0x%x)\n", 
				__func__,i2c_bus, i2c_addr, FAN_REG_CFG5);
		return ( rc );
	}

	// Enable GPIO Model
	rc = fan_rd(fd,i2c_bus, i2c_addr, FAN_REG_CFG4, &data);
	if (rc) {
		printf("Error: %s: rd i2c(bus=0x%x, addr=0x%x) reg(0x%X)\n", 
				__func__,i2c_bus, i2c_addr, FAN_REG_CFG4);
		return ( rc );
	}

	// Enable GPIO Mode.
	data |= 0x03; 

	rc = fan_wr(fd,i2c_bus, i2c_addr, FAN_REG_CFG4, &data);
	if (rc) {
		printf("Error: %s: rd i2c(bus=0x%x, addr=0x%x) reg(0x%x)\n", 
				__func__,i2c_bus, i2c_addr, FAN_REG_CFG4);
		return ( rc );
	}
	return (rc);
}

#define FAN_GPIO_BIT	5
int fan_gpio_rd (int fd, uint8_t i2c_bus, uint16_t i2c_addr, uint8_t gpio, uint8_t *stat)
{
	int rc = 0;
	uint8_t data;

	rc = fan_rd(fd,i2c_bus, i2c_addr, FAN_REG_IRQ_STAT2, &data);
	if (rc) {
		printf("Error: %s: rd i2c(bus=0x%x, addr=0x%x) reg(0x%x)\n", 
			__func__,i2c_bus, i2c_addr, FAN_REG_IRQ_STAT2);
		return ( rc );
	}

	*stat = (data & (0x01 << FAN_GPIO_BIT)) ? 1 : 0;

	return (rc);
}


int fan_gpio_wr (int fd, uint8_t i2c_bus, uint16_t i2c_addr, uint8_t gpio, uint8_t stat)
{
	int rc = 0;
	uint8_t data;

	rc = fan_rd(fd,i2c_bus, i2c_addr, FAN_REG_IRQ_STAT2, &data);
	if (rc) {
		printf("Error: %s: rd i2c(bus=0x%x, addr=0x%x) reg(0x%x)\n", 
			__func__,i2c_bus, i2c_addr, FAN_REG_IRQ_STAT2);
		return ( rc );
	}

	if (stat) {
		data |= (0x01 << FAN_GPIO_BIT);
	} else {
		data &= ~(0x01 << FAN_GPIO_BIT);
	}

	rc = fan_wr(fd,i2c_bus, i2c_addr, FAN_REG_IRQ_STAT2, &data);
	if (rc) {
		printf("Error: %s: wr i2c(bus=0x%x, addr=0x%x) reg(0x%x)\n", 
			__func__,i2c_bus, i2c_addr, FAN_REG_IRQ_STAT2);
		return ( rc );
	}
	return (rc);
}
