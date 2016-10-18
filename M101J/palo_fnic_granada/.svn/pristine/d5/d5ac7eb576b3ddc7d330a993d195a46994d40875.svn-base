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
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>

#include "diag_sprom.h"
#include "ipmi_sprom.h"
#include "ipmi_sprom_ops.h"
#include "saratoga-i2caddr.h"

static char we_path[128];
#define SPROM_WE_SET(bus, addr, bval) \
{				\
	sprintf(we_path, "echo %d >> /sys/devices/platform/fsl-i2c.%d/i2c-%d/%01d-%04x/write_enable",\
		(bval), (bus)+1, (bus), (bus), (addr));	\
	system(we_path);	\
}

// SPROM write enable/disable routines.
static void iom_sprom_we ( void )
{
	SPROM_WE_SET(SI2C_IOM_FRU_BUS, SI2C_IOM_FRU_ADDR, 1);	
}

static void iom_sprom_wd ( void )
{
	SPROM_WE_SET(SI2C_IOM_FRU_BUS, SI2C_IOM_FRU_ADDR, 0);	
}

static void iom_fan_sprom_we ( void )
{
	SPROM_WE_SET(SI2C_F_FM0_FRU_BUS, SI2C_F_FM0_FRU_ADDR, 1);	
	SPROM_WE_SET(SI2C_F_FM1_FRU_BUS, SI2C_F_FM1_FRU_ADDR, 1);	
	SPROM_WE_SET(SI2C_F_FM2_FRU_BUS, SI2C_F_FM2_FRU_ADDR, 1);	
	SPROM_WE_SET(SI2C_F_FM3_FRU_BUS, SI2C_F_FM3_FRU_ADDR, 1);	
	SPROM_WE_SET(SI2C_F_FM4_FRU_BUS, SI2C_F_FM4_FRU_ADDR, 1);	
	SPROM_WE_SET(SI2C_F_FM5_FRU_BUS, SI2C_F_FM5_FRU_ADDR, 1);	
	SPROM_WE_SET(SI2C_F_FM6_FRU_BUS, SI2C_F_FM6_FRU_ADDR, 1);	
	SPROM_WE_SET(SI2C_F_FM7_FRU_BUS, SI2C_F_FM7_FRU_ADDR, 1);	
}

static void iom_fan_sprom_wd ( void )
{
	SPROM_WE_SET(SI2C_F_FM0_FRU_BUS, SI2C_F_FM0_FRU_ADDR, 0);	
	SPROM_WE_SET(SI2C_F_FM1_FRU_BUS, SI2C_F_FM1_FRU_ADDR, 0);	
	SPROM_WE_SET(SI2C_F_FM2_FRU_BUS, SI2C_F_FM2_FRU_ADDR, 0);	
	SPROM_WE_SET(SI2C_F_FM3_FRU_BUS, SI2C_F_FM3_FRU_ADDR, 0);	
	SPROM_WE_SET(SI2C_F_FM4_FRU_BUS, SI2C_F_FM4_FRU_ADDR, 0);	
	SPROM_WE_SET(SI2C_F_FM5_FRU_BUS, SI2C_F_FM5_FRU_ADDR, 0);	
	SPROM_WE_SET(SI2C_F_FM6_FRU_BUS, SI2C_F_FM6_FRU_ADDR, 0);	
	SPROM_WE_SET(SI2C_F_FM7_FRU_BUS, SI2C_F_FM7_FRU_ADDR, 0);	
}

static void iom_ps_sprom_we ( void )
{
	SPROM_WE_SET(SI2C_P_PSU0_FRU_BUS, SI2C_P_PSU0_FRU_ADDR, 1);	
	SPROM_WE_SET(SI2C_P_PSU1_FRU_BUS, SI2C_P_PSU1_FRU_ADDR, 1);	
	SPROM_WE_SET(SI2C_P_PSU2_FRU_BUS, SI2C_P_PSU2_FRU_ADDR, 1);	
	SPROM_WE_SET(SI2C_P_PSU3_FRU_BUS, SI2C_P_PSU3_FRU_ADDR, 1);	
}

static void iom_ps_sprom_wd ( void )
{
	SPROM_WE_SET(SI2C_P_PSU0_FRU_BUS, SI2C_P_PSU0_FRU_ADDR, 0);	
	SPROM_WE_SET(SI2C_P_PSU1_FRU_BUS, SI2C_P_PSU1_FRU_ADDR, 0);	
	SPROM_WE_SET(SI2C_P_PSU2_FRU_BUS, SI2C_P_PSU2_FRU_ADDR, 0);	
	SPROM_WE_SET(SI2C_P_PSU3_FRU_BUS, SI2C_P_PSU3_FRU_ADDR, 0);	
}

static void iom_bp_sprom_we ( void )
{
	SPROM_WE_SET(SI2C_C_FRU_BUS, SI2C_C_FRU_ADDR, 1);	
	SPROM_WE_SET(SI2C_C_SEEPROM_BUS, SI2C_C_SEEPROM_ADDR, 1);	
}

static void iom_bp_sprom_wd ( void )
{
	SPROM_WE_SET(SI2C_C_FRU_BUS, SI2C_C_FRU_ADDR, 0);	
	SPROM_WE_SET(SI2C_C_SEEPROM_BUS, SI2C_C_SEEPROM_ADDR, 0);	
}

#define IOM_FAN_SPROM_MAX	8
#define IOM_PS_SPROM_MAX	4
#define IOM_BP_SPROM_MAX	2

/////////////////////////////////
// SUP SPROM
/////////////////////////////////

static uint8_t sprom_sup_cache[sizeof(sprom_ipmi_iom_t)];
diag_smb_dev_t ipmi_iom_sup_dev[] =
{
{"/dev/i2c-0", SI2C_IOM_FRU_BUS, SI2C_IOM_FRU_ADDR, 0, 2, 1, 0},
};

diag_smb_acc_t ipmi_iom_sprom_acc = 
{&ipmi_iom_sup_dev[0], sprom_rd, sprom_wr};

diag_sprom_t ipmi_iom_sprom[] = {
      { &ipmi_iom_sprom_acc, iom_sprom_we, iom_sprom_wd,
        "IOM", SPROM_UTIL_TYPE_SUP, 0, sprom_sup_cache },
};


/////////////////////////////////
// FAN SPROM
/////////////////////////////////
diag_smb_dev_t ipmi_iom_fan_dev[] =
{
{"/dev/i2c1", SI2C_F_FM0_FRU_BUS, SI2C_F_FM0_FRU_ADDR, 0, 2, 1, 0},
{"/dev/i2c1", SI2C_F_FM1_FRU_BUS, SI2C_F_FM1_FRU_ADDR, 0, 2, 1, 0},
{"/dev/i2c1", SI2C_F_FM2_FRU_BUS, SI2C_F_FM2_FRU_ADDR, 0, 2, 1, 0},
{"/dev/i2c1", SI2C_F_FM3_FRU_BUS, SI2C_F_FM3_FRU_ADDR, 0, 2, 1, 0},
{"/dev/i2c1", SI2C_F_FM4_FRU_BUS, SI2C_F_FM4_FRU_ADDR, 0, 2, 1, 0},
{"/dev/i2c1", SI2C_F_FM5_FRU_BUS, SI2C_F_FM5_FRU_ADDR, 0, 2, 1, 0},
{"/dev/i2c1", SI2C_F_FM6_FRU_BUS, SI2C_F_FM6_FRU_ADDR, 0, 2, 1, 0},
{"/dev/i2c1", SI2C_F_FM7_FRU_BUS, SI2C_F_FM7_FRU_ADDR, 0, 2, 1, 0},
};

diag_smb_acc_t ipmi_iom_fan_sprom_acc[] = 
{
{&ipmi_iom_fan_dev[0], sprom_rd, sprom_wr},
{&ipmi_iom_fan_dev[1], sprom_rd, sprom_wr},
{&ipmi_iom_fan_dev[2], sprom_rd, sprom_wr},
{&ipmi_iom_fan_dev[3], sprom_rd, sprom_wr},
{&ipmi_iom_fan_dev[4], sprom_rd, sprom_wr},
{&ipmi_iom_fan_dev[5], sprom_rd, sprom_wr},
{&ipmi_iom_fan_dev[6], sprom_rd, sprom_wr},
{&ipmi_iom_fan_dev[7], sprom_rd, sprom_wr},
};

static uint8_t ipmi_sprom_fan_cache[8][sizeof(sprom_ipmi_iom_fan_t)];
diag_sprom_t ipmi_iom_fan_sprom[] = {
      { &ipmi_iom_fan_sprom_acc[0], iom_fan_sprom_we, iom_fan_sprom_wd,
        "FAN", SPROM_UTIL_TYPE_FAN, 0, ipmi_sprom_fan_cache[0] },
      { &ipmi_iom_fan_sprom_acc[1], iom_fan_sprom_we, iom_fan_sprom_wd,
        "FAN", SPROM_UTIL_TYPE_FAN, 0, ipmi_sprom_fan_cache[1]},
      { &ipmi_iom_fan_sprom_acc[2], iom_fan_sprom_we, iom_fan_sprom_wd,
        "FAN", SPROM_UTIL_TYPE_FAN, 0, ipmi_sprom_fan_cache[2] },
      { &ipmi_iom_fan_sprom_acc[3], iom_fan_sprom_we, iom_fan_sprom_wd,
        "FAN", SPROM_UTIL_TYPE_FAN, 0, ipmi_sprom_fan_cache[3] },
      { &ipmi_iom_fan_sprom_acc[4], iom_fan_sprom_we, iom_fan_sprom_wd,
        "FAN", SPROM_UTIL_TYPE_FAN, 0, ipmi_sprom_fan_cache[4] },
      { &ipmi_iom_fan_sprom_acc[5], iom_fan_sprom_we, iom_fan_sprom_wd,
        "FAN", SPROM_UTIL_TYPE_FAN, 0, ipmi_sprom_fan_cache[5] },
      { &ipmi_iom_fan_sprom_acc[6], iom_fan_sprom_we, iom_fan_sprom_wd,
        "FAN", SPROM_UTIL_TYPE_FAN, 0, ipmi_sprom_fan_cache[6] },
      { &ipmi_iom_fan_sprom_acc[7], iom_fan_sprom_we, iom_fan_sprom_wd,
        "FAN", SPROM_UTIL_TYPE_FAN, 0, ipmi_sprom_fan_cache[7] },
};

/////////////////////////////////
// PS SPROM
/////////////////////////////////
diag_smb_dev_t ipmi_iom_ps_dev[] =
{
{"/dev/i2c1", SI2C_P_PSU0_FRU_BUS, SI2C_P_PSU0_FRU_ADDR, 0, 2, 1, 0},
{"/dev/i2c1", SI2C_P_PSU1_FRU_BUS, SI2C_P_PSU1_FRU_ADDR, 0, 2, 1, 0},
{"/dev/i2c1", SI2C_P_PSU2_FRU_BUS, SI2C_P_PSU2_FRU_ADDR, 0, 2, 1, 0},
{"/dev/i2c1", SI2C_P_PSU3_FRU_BUS, SI2C_P_PSU3_FRU_ADDR, 0, 2, 1, 0},
};

diag_smb_acc_t ipmi_iom_ps_sprom_acc[] = 
{
{&ipmi_iom_ps_dev[0], sprom_rd, sprom_wr},
{&ipmi_iom_ps_dev[1], sprom_rd, sprom_wr},
{&ipmi_iom_ps_dev[2], sprom_rd, sprom_wr},
{&ipmi_iom_ps_dev[3], sprom_rd, sprom_wr},
};

static uint8_t ipmi_sprom_ps_cache[4][sizeof(sprom_ipmi_iom_psu_t)];
diag_sprom_t ipmi_iom_ps_sprom[] = {
      { &ipmi_iom_ps_sprom_acc[0], iom_ps_sprom_we, iom_ps_sprom_wd,
        "PS", SPROM_UTIL_TYPE_PS, 0, ipmi_sprom_ps_cache[0] },
      { &ipmi_iom_ps_sprom_acc[1], iom_ps_sprom_we, iom_ps_sprom_wd,
        "PS", SPROM_UTIL_TYPE_PS, 0, ipmi_sprom_ps_cache[1]},
      { &ipmi_iom_ps_sprom_acc[2], iom_ps_sprom_we, iom_ps_sprom_wd,
        "PS", SPROM_UTIL_TYPE_PS, 0, ipmi_sprom_ps_cache[0] },
      { &ipmi_iom_ps_sprom_acc[3], iom_ps_sprom_we, iom_ps_sprom_wd,
        "PS", SPROM_UTIL_TYPE_PS, 0, ipmi_sprom_ps_cache[1]},
};


/////////////////////////////////
// BP/MP/Chassis SPROM
/////////////////////////////////
diag_smb_dev_t ipmi_iom_bp_dev[] =
{
{"/dev/i2c1", SI2C_C_FRU_BUS, SI2C_C_FRU_ADDR,     0, 2, 1, 0},
{"/dev/i2c1", SI2C_C_SEEPROM_BUS, SI2C_C_SEEPROM_ADDR, 0, 2, 1, 0},
};

diag_smb_acc_t ipmi_iom_bp_sprom_acc[] = 
{
{&ipmi_iom_bp_dev[0], sprom_rd, sprom_wr},
{&ipmi_iom_bp_dev[1], sprom_rd, sprom_wr},
};

static uint8_t ipmi_sprom_bp_cache[2][sizeof(sprom_ipmi_iom_bp_t)];
diag_sprom_t ipmi_iom_bp_sprom[] = {
      { &ipmi_iom_bp_sprom_acc[0], iom_bp_sprom_we, iom_bp_sprom_wd,
        "BP", SPROM_UTIL_TYPE_BP, 0, ipmi_sprom_bp_cache[0] },
      { &ipmi_iom_bp_sprom_acc[1], iom_bp_sprom_we, iom_bp_sprom_wd,
        "BP", SPROM_UTIL_TYPE_BP, 0, ipmi_sprom_bp_cache[1]},
};

diag_sprom_t *
ipmi_sprom_init_pfm ( uint32_t util_type, uint32_t slot)
{
	switch(util_type) {
		case	SPROM_UTIL_TYPE_SUP:
    			return (ipmi_iom_sprom);
			break;

		case	SPROM_UTIL_TYPE_FAN:
			return((slot >= IOM_FAN_SPROM_MAX)?
				NULL: &ipmi_iom_fan_sprom[slot]);
			break;

		case	SPROM_UTIL_TYPE_PS:
			return((slot >= IOM_PS_SPROM_MAX)?
				NULL: &ipmi_iom_ps_sprom[slot]);
			break;

		case	SPROM_UTIL_TYPE_BP:
			return((slot >= IOM_BP_SPROM_MAX)?
				NULL: &ipmi_iom_bp_sprom[slot]);
			break;
		default:
			break;
	}

	return (NULL);
}

sprom_platform_t
ipmi_sprom_platform_probe ( void )
{
    return ( SPROM_PLATFORM_IOM );
}

int
ipmi_sprom_get_slot_sup_iom ( uint32_t *slot)
{
    *slot     = 0;
    return ( 0 );
}

int
ipmi_sprom_get_slot_bp_iom ( uint32_t *slot)
{
    *slot     = 0;
    return ( 0 );
}

int
ipmi_sprom_get_slot_chassis_iom ( uint32_t *slot)
{
    *slot = 0;
    return ( 0 );
}

int ipmi_sprom_iom_show(diag_sprom_t* const sprom)
{
        int rc = 0;
        sprom_ipmi_iom_t        iom_sprom;

        rc = ipmi_sprom_read(sprom, (uint8_t *)&iom_sprom);
        if (rc) {
                printf(" Error: Failed to read the SPROM\n");
                return (-1);
        }

        ipmi_sprom_iom_board_dump(&iom_sprom);
        return (0);
}

