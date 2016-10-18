/********************************************************************
 *
 *      File:  sprom_bmc.c
 *      Name:  Sudharshan Kadari 
 *
 *      Description: Nuova sprom access
 *
 * Copyright (c) 1985-2007 by Cisco Systems, Inc.
 * All rights reserved.
 *
 **********************************************************************/
/********************************************************************//**
@file    sprom_bmc.c
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

// SPROM write enable/disable routines.
static void bmc_sprom_we ( void )
{
}

static void bmc_sprom_wd ( void )
{
}

static void bmc_ps_sprom_we ( void )
{
}

static void bmc_ps_sprom_wd ( void )
{
}

static void bmc_bp_sprom_we ( void )
{
}

static void bmc_bp_sprom_wd ( void )
{
}

static void bmc_mezz_sprom_we ( void )
{
}

static void bmc_mezz_sprom_wd ( void )
{
}

#define SI2C_CHS_FRU_ADDR	0x00
#define SI2C_HDDBP_FRU_ADDR	0x01
#define SI2C_BMC_FRU_ADDR	0x02
#define SI2C_PSU_FRU_ADDR_0	0x03
#define SI2C_PSU_FRU_ADDR_1	0x04
#define SI2C_FP_FRU_ADDR	0x05

//#define SI2C_BMC_FRU_BUS	1
//#define SI2C_BMC_FRU_ADDR	0xAC

//#define SI2C_PSU_FRU_BUS	4
//#define SI2C_PSU_FRU_ADDR_0	0xA0
//#define SI2C_PSU_FRU_ADDR_1	0xA2

//#define SI2C_HDDBP_FRU_BUS	0
//#define SI2C_HDDBP_FRU_ADDR	0x01

//#define SI2C_FP_FRU_BUS	1
//#define SI2C_FP_FRU_ADDR	0xA2

/////////////////////////////////
// BMC SPROM
/////////////////////////////////
static uint8_t sprom_sup_cache[sizeof(sprom_ipmi_bmc_t)];
diag_smb_dev_t ipmi_bmc_sup_dev[] =
{
{"/dev/i2c-1", 1, SI2C_BMC_FRU_ADDR, 0, 2, 1, 0},
};

diag_smb_acc_t ipmi_bmc_sprom_acc = 
{&ipmi_bmc_sup_dev[0], sprom_rd, sprom_wr};

diag_sprom_t ipmi_bmc_sprom[] = {
      { &ipmi_bmc_sprom_acc, bmc_sprom_we, bmc_sprom_wd,
        "BMC", SPROM_UTIL_TYPE_IBMC, 0, sprom_sup_cache },
};


/////////////////////////////////
// PS SPROM
/////////////////////////////////
#define LA_PS_SPROM_MAX	2
diag_smb_dev_t ipmi_la_ps_dev[] =
{
{"/dev/i2c-4", 4, SI2C_PSU_FRU_ADDR_0, 0, 2, 1, 0},
{"/dev/i2c-4", 4, SI2C_PSU_FRU_ADDR_1, 0, 2, 1, 0},
};

diag_smb_acc_t ipmi_la_ps_sprom_acc[] =
{
{&ipmi_la_ps_dev[0], sprom_rd, sprom_wr},
{&ipmi_la_ps_dev[1], sprom_rd, sprom_wr},
};

static uint8_t ipmi_sprom_ps_cache[2][sizeof(sprom_ipmi_iom_psu_t)];
diag_sprom_t ipmi_la_ps_sprom[] = {
      { &ipmi_la_ps_sprom_acc[0], bmc_ps_sprom_we, bmc_ps_sprom_wd,
        "PS", SPROM_UTIL_TYPE_PS, 0, ipmi_sprom_ps_cache[0] },
      { &ipmi_la_ps_sprom_acc[1], bmc_ps_sprom_we, bmc_ps_sprom_wd,
        "PS", SPROM_UTIL_TYPE_PS, 0, ipmi_sprom_ps_cache[1]},
};

/////////////////////////////////
// HDDBP SPROM
/////////////////////////////////
#define BMC_HDDBP_SPROM_MAX 1
diag_smb_dev_t ipmi_hddbp_dev[] =
{
{"/dev/i2c5", 5, SI2C_HDDBP_FRU_ADDR,  0, 2, 1, 0},
{"/dev/i2c1", 1, SI2C_FP_FRU_ADDR,     0, 2, 1, 0},
};

diag_smb_acc_t ipmi_hddbp_sprom_acc[] = 
{
{&ipmi_hddbp_dev[0], sprom_rd, sprom_wr},
{&ipmi_hddbp_dev[1], sprom_rd, sprom_wr},
};

static uint8_t ipmi_sprom_bp_cache[sizeof(sprom_ipmi_iom_bp_t)];
diag_sprom_t ipmi_hddbp_sprom[] = {
      { &ipmi_hddbp_sprom_acc[0], bmc_bp_sprom_we, bmc_bp_sprom_wd,
        "BP", SPROM_UTIL_TYPE_BP, 0, ipmi_sprom_bp_cache },
      { &ipmi_hddbp_sprom_acc[1], bmc_bp_sprom_we, bmc_bp_sprom_wd,
        "BP", SPROM_UTIL_TYPE_BP, 0, ipmi_sprom_bp_cache },
};

/////////////////////////////////
// MontereyPark SPROM
/////////////////////////////////
#define SI2C_SD1_MPARK_FRU_ADDR		6
#define SI2C_SD2_MPARK_FRU_ADDR_0	7
#define SI2C_SD2_MPARK_FRU_ADDR_1	8
#define SD_MP_SPROM_MAX			2

diag_smb_dev_t ipmi_mpark_dev[] =
{
{"/dev/i2c-12", 0xC, SI2C_SD2_MPARK_FRU_ADDR_0,  0, 2, 1, 0},
{"/dev/i2c-13", 0xD, SI2C_SD2_MPARK_FRU_ADDR_1,  0, 2, 1, 0},
};

diag_smb_acc_t ipmi_mpark_sprom_acc[] = 
{
{&ipmi_mpark_dev[0], sprom_rd, sprom_wr},
{&ipmi_mpark_dev[1], sprom_rd, sprom_wr},
};

static uint8_t ipmi_sprom_mpark_cache[sizeof(sprom_ipmi_mpark_t)];
diag_sprom_t ipmi_mpark_sprom[] = {
      { &ipmi_mpark_sprom_acc[0], NULL, NULL,
        "MPARK", SPROM_UTIL_TYPE_MONTEREYPARK, 0, ipmi_sprom_mpark_cache },
      { &ipmi_mpark_sprom_acc[1], NULL, NULL,
        "MPARK", SPROM_UTIL_TYPE_MONTEREYPARK, 0, ipmi_sprom_mpark_cache },
};

void ipmi_sd1_fix_mpark_fru_addr()
{
	ipmi_mpark_dev[0].addr = SI2C_SD1_MPARK_FRU_ADDR;
	ipmi_mpark_dev[1].addr = SI2C_SD1_MPARK_FRU_ADDR;
}

diag_sprom_t *
ipmi_sprom_init_pfm ( uint32_t util_type, uint32_t slot)
{
	switch(util_type) {
		case	SPROM_UTIL_TYPE_SUP:
		case	SPROM_UTIL_TYPE_IBMC:
    			return (ipmi_bmc_sprom);
			break;

		case	SPROM_UTIL_TYPE_BP:
			return((slot >= BMC_HDDBP_SPROM_MAX)?
				NULL: &ipmi_hddbp_sprom[slot]);
			break;

		case	SPROM_UTIL_TYPE_PS:
			return((slot >= LA_PS_SPROM_MAX)?
				NULL: &ipmi_la_ps_sprom[slot]);
			break;

		case	SPROM_UTIL_TYPE_MONTEREYPARK:
			return((slot >= SD_MP_SPROM_MAX)?
				NULL: &ipmi_mpark_sprom[slot]);
			break;

		default:
			printf(" %s: Unknown type specified\n", __func__);
			break;
	}
	return (NULL);
}

sprom_platform_t
ipmi_sprom_platform_probe ( void )
{
    return ( SPROM_PLATFORM_IBMC );
}


int ipmi_sprom_bmc_show(diag_sprom_t* const sprom)
{
        int rc = 0;
        sprom_ipmi_bmc_t        bmc_sprom;

        rc = ipmi_sprom_read(sprom, (uint8_t *)&bmc_sprom);
        if (rc) {
                printf(" Error: Failed to read the SPROM\n");
                return (-1);
        }

        ipmi_sprom_ibmc_dump((sprom_ipmi_ibmc_t*)&bmc_sprom, IPMI_IU_CARD_TYPE_IBMC);
        return (0);
}

