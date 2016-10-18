/********************************************************************
 *
 *      File:  adt7473_iom.c
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
@file    adt7473_iom.c
@author  Sudharshan Kadari -- skadari@nuovasystems.com
@brief   adt7473 chip access routines

************************************************************************/
#include <stdio.h>
#include <sys/types.h>

#include "adt7473.h"
#include "ppc_lib.h"

/* INT MASK CONF 2 */
#define ENABLE_THERM_INT                (0x20)

/* THERM limit option */
#define DISABLE_THERM_LIMIT             (0x80)

/* PWM Config options */
#define PWM_SPIN_TIMEOUT        (0x02)          /* 250 ms */
#define PWM_SLOW                (0x01 << 3)     /* Slow spin */
#define PWM_INV                 (0x1 << 4)      /* PWM invert reading */
#define PWM_BHVR_MANUAL         (0x07 << 5)     /* Fan writable */

/* CONF1 OPTIONS */
#define ENABLE_PWM_MON          (0x1)           /* Enable PWM mon */
#define LOCK_LIMITS             (0x1 << 1)      /* lock all limits */
#define FAN_MAX_SPEED           (0x1 << 3)      /* Set fans to max speed */
#define DEV_READY               (0x1 << 2)      /* Status of device */

/* CONF2 OPTIONS */
#define FAN_PRESENCE_DT         (0x1)           /* Enable fan presence
                                                    detection */

/* CONF3 OPTIONS */
#define ENABLE_ALERT            (0x1)
#define ENABLE_THERM            (0x1 << 1) /* Enables THERM funcationality */
#define THERM_BOOST             (0x1 << 2) /* THERM is an input */
#define TACH_FAST_MEASURE       (0x1 << 3) /* Enable Fast tach measurement */
#define PREVENT_PS_DC1          (0x1 << 4) /* Prevents Pulse stretching */
#define PREVENT_PS_DC2          (0x1 << 5) /* Prevents Pulse stretching */

/* CONF5 OPTIONS */
#define TWOS_COMPL      (0x1)           /* Set temp to 2's compl */
#define TEMP_OFF        (0x1 << 1)      /* Range +-128 at 1C resol */
#define GPIOD_OUT       (0x1 << 2)      /* Set GPIO as output */
#define GPIOP           (0x1 << 3)      /* Set GPIO polarity */

/* CONF4 OPTIONS */
#define PIN9_FUNC       (0x01)          /* Set PIN9 as THERM */
#define THERM_DISABLE   (0x1 << 2)      /* Disable therm */

/* INT STATUS */
#define INT_THERM_STATUS        (0x20)  /* SMBALERT due to therm assert */

static int adt7343_config_pwm(int fd, uint16_t i2c_addr)
{
        uint8_t val = 0;

        /*
         * Set spin timeout to 250ms
         * Set manual mode
         */
        val = (PWM_SPIN_TIMEOUT | PWM_BHVR_MANUAL);

        return adt7473_wr(fd, i2c_addr, ADT7473_REG_PWM1_CFG, 1, &val);
}

static int adt7343_config_fan_detect(int fd, uint16_t i2c_addr)
{
        uint8_t val = 0;

        /* Enable fan detect */
        val = (FAN_PRESENCE_DT);

        return adt7473_wr(fd, i2c_addr, ADT7473_REG_CFG2, 1, &val);
}

static int adt7343_config_temp(int fd, uint16_t i2c_addr)
{
        uint8_t val = 0;

        /*
         * Set 2's complement temp reading
         * +-128 at 1C resolution
         * Set
         */
         val = (TWOS_COMPL|TEMP_OFF);

         return adt7473_wr(fd, i2c_addr, ADT7473_REG_CFG5, 1, &val);
}

static int adt7343_disable_therm_limit(int fd, uint16_t i2c_addr)
{
        uint8_t val = 0;
        int rc;

        val = DISABLE_THERM_LIMIT;

        /* Local temp */
        rc = adt7473_wr(fd, i2c_addr, ADT7473_REG_LCL_TEMP_LIMIT, 1, &val);

        /* We do not care about Remote 1 & 2. So ignore any errors */
        /* Remote 1 temp */
        adt7473_wr(fd, i2c_addr, ADT7473_REG_RMT1_TEMP_LIMIT, 1, &val);
        /* Remote 2 temp */
        adt7473_wr(fd, i2c_addr, ADT7473_REG_RMT2_TEMP_LIMIT, 1, &val);

        return (rc);
}

static int adt7343_config_therm_and_fan_tach(int fd, uint16_t i2c_addr)
{
        uint8_t val = 0;
        int rc;

        val = (ENABLE_ALERT | ENABLE_THERM | THERM_BOOST | TACH_FAST_MEASURE
                        | PREVENT_PS_DC1 | PREVENT_PS_DC2);

        rc = adt7473_wr(fd, i2c_addr, ADT7473_REG_CFG3, 1, &val);
        if (rc < 0) {
                return rc;
        }

        val = (PIN9_FUNC);
        return adt7473_wr(fd, i2c_addr, ADT7473_REG_CFG4, 1, &val);
}

static int adt7343_disable_interrupts(int fd, uint16_t i2c_addr)
{
        uint8_t val = 0;
        int rc;

        /* Enable only therm interrupt for now */
	val = 0xFF;
        rc = adt7473_wr(fd, i2c_addr, ADT7473_REG_IRQ_MASK1, 1, &val);
        if (rc < 0) {
                return rc;
        }

        val = ~(ENABLE_THERM_INT);
        return adt7473_wr(fd, i2c_addr, ADT7473_REG_IRQ_MASK2, 1, &val);
}

#define ADT7473_BOOST	0x4
static int adt7473_disable_boost(int fd, uint16_t i2c_addr)
{
        uint8_t val = 0;
        int rc;

        /* Enable only therm interrupt for now */
        rc = adt7473_rd(fd, i2c_addr, ADT7473_REG_CFG3, 1, &val);
	val &= ~ADT7473_BOOST; 
        rc = adt7473_wr(fd, i2c_addr, ADT7473_REG_CFG3, 1, &val);
        if (rc < 0) {
                return rc;
        }
	return (rc);
}

int adt7473_init(int fd, uint16_t i2c_addr)
{
        int rc=0;

        rc = adt7343_config_pwm(fd, i2c_addr);
        if (rc < 0) {
                return rc;
        }

        rc = adt7343_config_fan_detect(fd, i2c_addr);
        if (rc < 0) {
                return rc;
        }

        rc = adt7343_disable_therm_limit(fd, i2c_addr);
        if (rc < 0) {
                return rc;
        }

        rc = adt7343_config_temp(fd, i2c_addr);
        if (rc < 0) {
                return rc;
        }
        rc = adt7343_config_therm_and_fan_tach(fd, i2c_addr);
        if (rc < 0) {
                return rc;
        }

        rc = adt7343_disable_interrupts(fd, i2c_addr);
        if (rc < 0) {
                return rc;
        }

	rc = adt7473_disable_boost(fd, i2c_addr);
        if (rc < 0) {
                return rc;
        }
        return rc;
}

