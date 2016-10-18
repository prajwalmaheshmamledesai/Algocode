/********************************************************************
 *
 *      File:  sprom_fex.c
 *      Name:  Sudharshan Kadari 
 *
 *      Description: Nuova sprom access
 *
 * Copyright (c) 1985-2007 by Cisco Systems, Inc.
 * All rights reserved.
 *
 **********************************************************************/
/********************************************************************//**
@file    sprom_fex.c
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
#include "diag_fex.h"

// SPROM write enable/disable routines.
static void fex_sprom_we ( void )
{
}

static void fex_sprom_wd ( void )
{
}

static void fex_ps_sprom_we ( void )
{
}

static void fex_ps_sprom_wd ( void )
{
}

/////////////////////////////////
// SUP SPROM
/////////////////////////////////
#define I2CDEV_PREFIX "/dev/i2c-"
#define QUOTE(X)	#X
#define I2CDEV_NAME(X)	I2CDEV_PREFIX QUOTE(X)	

static uint8_t sprom_sup_cache[sizeof(sup_sprom_t)];
diag_smb_dev_t fex_sup_dev[] =
{
//{"/dev/i2c-0", 0, SI2C_FEX_FRU_ADDR, 0, 2, 1, 0},
{I2CDEV_NAME(FEX_BRD_SPROM_I2C_BUS), FEX_BRD_SPROM_I2C_BUS, 
  FEX_BRD_SPROM_I2C_ADDR, 0, 2, 1, 0},
};

diag_smb_acc_t fex_sprom_acc = 
{&fex_sup_dev[0], sprom_rd, sprom_wr};

diag_sprom_t fex_sprom[] = {
      { &fex_sprom_acc, fex_sprom_we, fex_sprom_wd,
        "FEX", SPROM_UTIL_TYPE_SUP, 0, sprom_sup_cache },
};

/////////////////////////////////
// BP SPROM
/////////////////////////////////
static uint8_t sprom_bp_cache[sizeof(backplane_sprom_t)];
diag_smb_dev_t fex_bp_dev[] =
{
  {I2CDEV_NAME(FEX_BRD_SPROM_I2C_BUS), FEX_BRD_SPROM_I2C_BUS,
    FEX_BRD_SPROM_I2C_ADDR, 0x1000, 2, 1, 0},
};

diag_smb_acc_t fex_bp_sprom_acc =
{&fex_bp_dev[0], sprom_rd, sprom_wr};

diag_sprom_t fex_bp_sprom[] = {
      { &fex_bp_sprom_acc, fex_sprom_we, fex_sprom_wd,
        "BP", SPROM_UTIL_TYPE_BP, 0, sprom_bp_cache },
};

/////////////////////////////////
// PS SPROM
/////////////////////////////////
diag_smb_dev_t fex_ps_dev[] =
{
{I2CDEV_NAME(FEX_PSU_SPROM_I2C_BUS0), FEX_PSU_SPROM_I2C_BUS0, FEX_PSU_SPROM_I2C_ADDR0, 0, 2, 1, 0},
{I2CDEV_NAME(FEX_PSU_SPROM_I2C_BUS1), FEX_PSU_SPROM_I2C_BUS1, FEX_PSU_SPROM_I2C_ADDR1, 0, 2, 1, 0},
};

diag_smb_acc_t fex_ps_sprom_acc[] = 
{
{&fex_ps_dev[0], sprom_rd, sprom_psu_wr},
{&fex_ps_dev[1], sprom_rd, sprom_psu_wr},
};

static uint8_t sprom_ps_cache[4][sizeof(power_supply_sprom_t)];
diag_sprom_t fex_ps_sprom[] = {
      { &fex_ps_sprom_acc[0], fex_ps_sprom_we, fex_ps_sprom_wd,
        "PS", SPROM_UTIL_TYPE_PS, 0, sprom_ps_cache[0] },
      { &fex_ps_sprom_acc[1], fex_ps_sprom_we, fex_ps_sprom_wd,
        "PS", SPROM_UTIL_TYPE_PS, 0, sprom_ps_cache[1]},
};

diag_sprom_t * const
sprom_init_pfm ( uint32_t util_type, uint32_t slot)
{
	switch(util_type) {
		case	SPROM_UTIL_TYPE_SUP:
    			return (fex_sprom);
			break;

		case	SPROM_UTIL_TYPE_FAN:
			break;

		case	SPROM_UTIL_TYPE_PS:
			return((slot >= FEX_PSU_DEV_COUNT)?
				NULL: &fex_ps_sprom[slot]);
			break;

		case	SPROM_UTIL_TYPE_BP:
			return (fex_bp_sprom);
			break;

		default:
			break;
	}

	return (NULL);
}

sprom_platform_t
sprom_platform_probe_pfm ( void )
{
    return ( SPROM_PLATFORM_FEX );
}


int diag_sprom_fex_rd(uint32_t offset, uint32_t len, uint8_t *buf)
{
	return (sprom_rd (fex_sup_dev, offset, len, buf));
}

int diag_sprom_fex_wr(uint32_t offset, uint32_t len, uint8_t *buf)
{
	return (sprom_wr (fex_sup_dev, offset, len, buf));
}
