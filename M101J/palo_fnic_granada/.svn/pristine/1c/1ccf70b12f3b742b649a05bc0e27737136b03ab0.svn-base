/********************************************************************
 *
 *      File:  sprom_iom.c
 *      Name:  Sudharshan Kadari 
 *
 *      Description: Nuova sprom access
 *
 * Copyright (c) 1985-2007 by Cisco Systems, Inc.
 * All rights reserved.
 *
 **********************************************************************/
/********************************************************************//**
@file    sprom_iom.c
@author  Sudharshan Kadari
@brief   Nuova Sprom Access
************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>

#include "sprom.h"
#include "diag_sprom_defs.h"
#include "diag_main.h"
#include "sprom_acc.h"

// SPROM write enable/disable routines.
static void iom_sprom_we ( void )
{
}

static void iom_sprom_wd ( void )
{
}

static void iom_fan_sprom_we ( void )
{
}

static void iom_fan_sprom_wd ( void )
{
}

static void iom_ps_sprom_we ( void )
{
}

static void iom_ps_sprom_wd ( void )
{
}

static void iom_bp_sprom_we ( void )
{
}

static void iom_bp_sprom_wd ( void )
{
}


#define IOM_FAN_SPROM_MAX	8
#define IOM_PS_SPROM_MAX	4
#define IOM_BP_SPROM_MAX	2
/////////////////////////////////
// SUP SPROM
/////////////////////////////////
#define I2CDEV_PREFIX "/dev/i2c-"
#define QUOTE(X)	#X
#define I2CDEV_NAME(X)	I2CDEV_PREFIX QUOTE(X)	

static uint8_t sprom_sup_cache[sizeof(sup_sprom_t)];
diag_smb_dev_t iom_sup_dev[] =
{
//{"/dev/i2c-0", 0, SI2C_IOM_FRU_ADDR, 0, 2, 1, 0},
{I2CDEV_NAME(SI2C_IOM_FRU_BUS), SI2C_IOM_FRU_BUS, SI2C_IOM_FRU_ADDR, 0, 2, 1, 0},
};

diag_smb_acc_t iom_sprom_acc = 
{&iom_sup_dev[0], sprom_rd, sprom_wr};

diag_sprom_t iom_sprom[] = {
      { &iom_sprom_acc, iom_sprom_we, iom_sprom_wd,
        "IOM", SPROM_UTIL_TYPE_SUP, 0, sprom_sup_cache },
};


/////////////////////////////////
// FAN SPROM
/////////////////////////////////
diag_smb_dev_t iom_fan_dev[] =
{
{I2CDEV_NAME(SI2C_F_FM0_FRU_BUS), SI2C_F_FM0_FRU_BUS, SI2C_F_FM0_FRU_ADDR, 0, 2, 1, 0},
{I2CDEV_NAME(SI2C_F_FM1_FRU_BUS), SI2C_F_FM1_FRU_BUS, SI2C_F_FM1_FRU_ADDR, 0, 2, 1, 0},
{I2CDEV_NAME(SI2C_F_FM2_FRU_BUS), SI2C_F_FM2_FRU_BUS, SI2C_F_FM2_FRU_ADDR, 0, 2, 1, 0},
{I2CDEV_NAME(SI2C_F_FM3_FRU_BUS), SI2C_F_FM3_FRU_BUS, SI2C_F_FM3_FRU_ADDR, 0, 2, 1, 0},
{I2CDEV_NAME(SI2C_F_FM4_FRU_BUS), SI2C_F_FM4_FRU_BUS, SI2C_F_FM4_FRU_ADDR, 0, 2, 1, 0},
{I2CDEV_NAME(SI2C_F_FM5_FRU_BUS), SI2C_F_FM5_FRU_BUS, SI2C_F_FM5_FRU_ADDR, 0, 2, 1, 0},
{I2CDEV_NAME(SI2C_F_FM6_FRU_BUS), SI2C_F_FM6_FRU_BUS, SI2C_F_FM6_FRU_ADDR, 0, 2, 1, 0},
{I2CDEV_NAME(SI2C_F_FM7_FRU_BUS), SI2C_F_FM7_FRU_BUS, SI2C_F_FM7_FRU_ADDR, 0, 2, 1, 0},
};

diag_smb_acc_t iom_fan_sprom_acc[] = 
{
{&iom_fan_dev[0], sprom_rd, sprom_wr},
{&iom_fan_dev[1], sprom_rd, sprom_wr},
{&iom_fan_dev[2], sprom_rd, sprom_wr},
{&iom_fan_dev[3], sprom_rd, sprom_wr},
{&iom_fan_dev[4], sprom_rd, sprom_wr},
{&iom_fan_dev[5], sprom_rd, sprom_wr},
{&iom_fan_dev[6], sprom_rd, sprom_wr},
{&iom_fan_dev[7], sprom_rd, sprom_wr},
};

static uint8_t sprom_fan_cache[8][sizeof(fan_sprom_t)];
diag_sprom_t iom_fan_sprom[] = {
      { &iom_fan_sprom_acc[0], iom_fan_sprom_we, iom_fan_sprom_wd,
        "FAN", SPROM_UTIL_TYPE_FAN, 0, sprom_fan_cache[0] },
      { &iom_fan_sprom_acc[1], iom_fan_sprom_we, iom_fan_sprom_wd,
        "FAN", SPROM_UTIL_TYPE_FAN, 0, sprom_fan_cache[1]},
      { &iom_fan_sprom_acc[2], iom_fan_sprom_we, iom_fan_sprom_wd,
        "FAN", SPROM_UTIL_TYPE_FAN, 0, sprom_fan_cache[2] },
      { &iom_fan_sprom_acc[3], iom_fan_sprom_we, iom_fan_sprom_wd,
        "FAN", SPROM_UTIL_TYPE_FAN, 0, sprom_fan_cache[3] },
      { &iom_fan_sprom_acc[4], iom_fan_sprom_we, iom_fan_sprom_wd,
        "FAN", SPROM_UTIL_TYPE_FAN, 0, sprom_fan_cache[4] },
      { &iom_fan_sprom_acc[5], iom_fan_sprom_we, iom_fan_sprom_wd,
        "FAN", SPROM_UTIL_TYPE_FAN, 0, sprom_fan_cache[5] },
      { &iom_fan_sprom_acc[6], iom_fan_sprom_we, iom_fan_sprom_wd,
        "FAN", SPROM_UTIL_TYPE_FAN, 0, sprom_fan_cache[6] },
      { &iom_fan_sprom_acc[7], iom_fan_sprom_we, iom_fan_sprom_wd,
        "FAN", SPROM_UTIL_TYPE_FAN, 0, sprom_fan_cache[7] },
};

/////////////////////////////////
// PS SPROM
/////////////////////////////////
diag_smb_dev_t iom_ps_dev[] =
{
{I2CDEV_NAME(SI2C_P_PSU0_FRU_BUS), SI2C_P_PSU0_FRU_BUS, SI2C_P_PSU0_FRU_ADDR, 0, 2, 1, 0},
{I2CDEV_NAME(SI2C_P_PSU1_FRU_BUS), SI2C_P_PSU1_FRU_BUS, SI2C_P_PSU1_FRU_ADDR, 0, 2, 1, 0},
{I2CDEV_NAME(SI2C_P_PSU2_FRU_BUS), SI2C_P_PSU2_FRU_BUS, SI2C_P_PSU2_FRU_ADDR, 0, 2, 1, 0},
{I2CDEV_NAME(SI2C_P_PSU3_FRU_BUS), SI2C_P_PSU3_FRU_BUS, SI2C_P_PSU3_FRU_ADDR, 0, 2, 1, 0},
};

diag_smb_acc_t iom_ps_sprom_acc[] = 
{
{&iom_ps_dev[0], sprom_rd, sprom_wr},
{&iom_ps_dev[1], sprom_rd, sprom_wr},
{&iom_ps_dev[2], sprom_rd, sprom_wr},
{&iom_ps_dev[3], sprom_rd, sprom_wr},
};

static uint8_t sprom_ps_cache[4][sizeof(power_supply_sprom_t)];
diag_sprom_t iom_ps_sprom[] = {
      { &iom_ps_sprom_acc[0], iom_ps_sprom_we, iom_ps_sprom_wd,
        "PS", SPROM_UTIL_TYPE_PS, 0, sprom_ps_cache[0] },
      { &iom_ps_sprom_acc[1], iom_ps_sprom_we, iom_ps_sprom_wd,
        "PS", SPROM_UTIL_TYPE_PS, 0, sprom_ps_cache[1]},
      { &iom_ps_sprom_acc[2], iom_ps_sprom_we, iom_ps_sprom_wd,
        "PS", SPROM_UTIL_TYPE_PS, 0, sprom_ps_cache[0] },
      { &iom_ps_sprom_acc[3], iom_ps_sprom_we, iom_ps_sprom_wd,
        "PS", SPROM_UTIL_TYPE_PS, 0, sprom_ps_cache[1]},
};


/////////////////////////////////
// BP/MP/Chassis SPROM
/////////////////////////////////
diag_smb_dev_t iom_bp_dev[] =
{
{I2CDEV_NAME(SI2C_C_FRU_BUS), SI2C_C_FRU_BUS, SI2C_C_FRU_ADDR,     0, 2, 1, 0},
{I2CDEV_NAME(SI2C_C_SEEPROM_BUS), SI2C_C_SEEPROM_BUS, SI2C_C_SEEPROM_ADDR, 0, 2, 1, 0},
};

diag_smb_acc_t iom_bp_sprom_acc[] = 
{
{&iom_bp_dev[0], sprom_rd, sprom_wr},
{&iom_bp_dev[1], sprom_rd, sprom_wr},
};

static uint8_t sprom_bp_cache[8][sizeof(backplane_sprom_t)];
diag_sprom_t iom_bp_sprom[] = {
      { &iom_bp_sprom_acc[0], iom_bp_sprom_we, iom_bp_sprom_wd,
        "BP", SPROM_UTIL_TYPE_BP, 0, sprom_bp_cache[0] },
      { &iom_bp_sprom_acc[1], iom_bp_sprom_we, iom_bp_sprom_wd,
        "BP", SPROM_UTIL_TYPE_BP, 0, sprom_bp_cache[1]},
};

diag_sprom_t * const
sprom_init_pfm ( uint32_t util_type, uint32_t slot)
{
	switch(util_type) {
		case	SPROM_UTIL_TYPE_SUP:
    			return (iom_sprom);
			break;

		case	SPROM_UTIL_TYPE_FAN:
			return((slot >= IOM_FAN_SPROM_MAX)?
				NULL: &iom_fan_sprom[slot]);
			break;

		case	SPROM_UTIL_TYPE_PS:
			return((slot >= IOM_PS_SPROM_MAX)?
				NULL: &iom_ps_sprom[slot]);
			break;

		case	SPROM_UTIL_TYPE_BP:
			return((slot >= IOM_BP_SPROM_MAX)?
				NULL: &iom_bp_sprom[slot]);
			break;

		default:
			break;
	}

	return (NULL);
}

sprom_platform_t
sprom_platform_probe_pfm ( void )
{
    return ( SPROM_PLATFORM_IOM );
}


int diag_sprom_iom_rd(uint32_t offset, uint32_t len, uint8_t *buf)
{
	return (sprom_rd (iom_sup_dev, offset, len, buf));
}

int diag_sprom_iom_wr(uint32_t offset, uint32_t len, uint8_t *buf)
{
	return (sprom_wr (iom_sup_dev, offset, len, buf));
}
