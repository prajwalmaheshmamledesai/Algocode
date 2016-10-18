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

static void bmc_fan_sprom_we ( void )
{
}

static void bmc_fan_sprom_wd ( void )
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

#define SI2C_BMC_FRU_ADDR	0x02
#define SI2C_MEZZ_FRU_ADDR_0	0x01
#define SI2C_MEZZ_FRU_ADDR_1	0x03

/////////////////////////////////
// BMC SPROM
/////////////////////////////////
static uint8_t sprom_sup_cache[sizeof(sprom_ipmi_bmc_t)];
diag_smb_dev_t ipmi_bmc_sup_dev[] =
{
{"/dev/i2c-0", 0, SI2C_BMC_FRU_ADDR, 0, 2, 1, 0},
};

diag_smb_acc_t ipmi_bmc_sprom_acc = 
{&ipmi_bmc_sup_dev[0], sprom_rd, sprom_wr};

diag_sprom_t ipmi_bmc_sprom[] = {
      { &ipmi_bmc_sprom_acc, bmc_sprom_we, bmc_sprom_wd,
        "BMC", SPROM_UTIL_TYPE_IBMC, 0, sprom_sup_cache },
};

/////////////////////////////////
// BBU SPROM
/////////////////////////////////
#define SI2C_BBU_FRU_ADDR	0x04
static uint8_t sprom_bbu_cache[sizeof(sprom_ipmi_bbu_t)];
diag_smb_dev_t ipmi_bmc_bbu_dev[] =
{
{"/dev/i2c-1", 1, SI2C_BBU_FRU_ADDR, 0, 2, 1, 0},
};

diag_smb_acc_t ipmi_bbu_sprom_acc = 
{&ipmi_bmc_bbu_dev[0], sprom_rd, sprom_wr};

diag_sprom_t ipmi_bbu_sprom[] = {
      { &ipmi_bbu_sprom_acc, NULL, NULL,
        "BBU", SPROM_UTIL_TYPE_BBU, 0, sprom_bbu_cache },
};

/////////////////////////////////
// MEZZ SPROM
/////////////////////////////////
static uint8_t sprom_mezz_cache[sizeof(sprom_ipmi_mezz_t)];
diag_smb_dev_t ipmi_mezz_dev[] =
{
{"/dev/i2c-0", 0, SI2C_MEZZ_FRU_ADDR_0, 0, 2, 1, 0},
{"/dev/i2c-0", 0, SI2C_MEZZ_FRU_ADDR_1, 0, 2, 1, 0},
};

diag_smb_acc_t ipmi_mezz_sprom_acc[] = 
{
{&ipmi_mezz_dev[0], sprom_rd, sprom_wr},
{&ipmi_mezz_dev[1], sprom_rd, sprom_wr}
};

diag_sprom_t ipmi_palo_sprom[] = {
      { &ipmi_mezz_sprom_acc[0], NULL, NULL,
        "PALO", SPROM_UTIL_TYPE_PALO, 0, sprom_mezz_cache },
      { &ipmi_mezz_sprom_acc[1], NULL, NULL,
        "PALO", SPROM_UTIL_TYPE_PALO, 0, sprom_mezz_cache },
};

diag_sprom_t ipmi_menlo_sprom[] = {
      { &ipmi_mezz_sprom_acc[0], NULL, NULL,
        "MENLO", SPROM_UTIL_TYPE_MENLO, 0, sprom_mezz_cache },
      { &ipmi_mezz_sprom_acc[1], NULL, NULL,
        "MENLO", SPROM_UTIL_TYPE_MENLO, 0, sprom_mezz_cache },
};

diag_sprom_t ipmi_menloe_sprom[] = {
      { &ipmi_mezz_sprom_acc[0], NULL, NULL,
        "MENLOE", SPROM_UTIL_TYPE_MENLO_E, 0, sprom_mezz_cache },
      { &ipmi_mezz_sprom_acc[1], NULL, NULL,
        "MENLOE", SPROM_UTIL_TYPE_MENLO_E, 0, sprom_mezz_cache },
};

diag_sprom_t ipmi_oplin_sprom[] = {
      { &ipmi_mezz_sprom_acc[0], NULL, NULL,
        "OPLIN", SPROM_UTIL_TYPE_OPLIN, 0, sprom_mezz_cache },
      { &ipmi_mezz_sprom_acc[1], NULL, NULL,
        "OPLIN", SPROM_UTIL_TYPE_OPLIN, 0, sprom_mezz_cache },
};

diag_sprom_t ipmi_niantic_sprom[] = {
      { &ipmi_mezz_sprom_acc[0], NULL, NULL,
        "NIANTIC", SPROM_UTIL_TYPE_NIANTIC, 0, sprom_mezz_cache },
      { &ipmi_mezz_sprom_acc[1], NULL, NULL,
        "NIANTIC", SPROM_UTIL_TYPE_NIANTIC, 0, sprom_mezz_cache },
};

diag_sprom_t ipmi_neteffect_sprom[] = {
      { &ipmi_mezz_sprom_acc[0], NULL, NULL,
        "NETEFFECT", SPROM_UTIL_TYPE_NETEFFECT, 0, sprom_mezz_cache },
      { &ipmi_mezz_sprom_acc[1], NULL, NULL,
        "NETEFFECT", SPROM_UTIL_TYPE_NETEFFECT, 0, sprom_mezz_cache },
};

diag_sprom_t ipmi_schultz_sprom[] = {
      { &ipmi_mezz_sprom_acc[0], NULL, NULL,
        "SCHULTZ", SPROM_UTIL_TYPE_SCHULTZ, 0, sprom_mezz_cache },
      { &ipmi_mezz_sprom_acc[1], NULL, NULL,
        "SCHULTZ", SPROM_UTIL_TYPE_SCHULTZ, 0, sprom_mezz_cache },
};

diag_sprom_t ipmi_tigershark_sprom[] = {
      { &ipmi_mezz_sprom_acc[0], NULL, NULL,
        "TIGERSHARK", SPROM_UTIL_TYPE_TIGERSHARK, 0, sprom_mezz_cache },
      { &ipmi_mezz_sprom_acc[1], NULL, NULL,
        "TIGERSHARK", SPROM_UTIL_TYPE_TIGERSHARK, 0, sprom_mezz_cache },
};

diag_sprom_t ipmi_everest_sprom[] = {
      { &ipmi_mezz_sprom_acc[0], NULL, NULL,
        "EVEREST", SPROM_UTIL_TYPE_EVEREST, 0, sprom_mezz_cache },
      { &ipmi_mezz_sprom_acc[1], NULL, NULL,
        "EVEREST", SPROM_UTIL_TYPE_EVEREST, 0, sprom_mezz_cache },
};

diag_sprom_t *
ipmi_sprom_init_pfm ( uint32_t util_type, uint32_t slot)
{
	switch(util_type) {
		case	SPROM_UTIL_TYPE_SUP:
		case	SPROM_UTIL_TYPE_IBMC:
    			return (ipmi_bmc_sprom);
			break;

		case	SPROM_UTIL_TYPE_OPLIN:
    			return (&ipmi_oplin_sprom[slot]);
			break;

		case	SPROM_UTIL_TYPE_MENLO:
    			return (&ipmi_menlo_sprom[slot]);
			break;

		case	SPROM_UTIL_TYPE_MENLO_E:
    			return (&ipmi_menloe_sprom[slot]);
			break;

		case	SPROM_UTIL_TYPE_PALO:
    			return (&ipmi_palo_sprom[slot]);
			break;

		case	SPROM_UTIL_TYPE_NIANTIC:
    			return (&ipmi_niantic_sprom[slot]);
			break;

		case	SPROM_UTIL_TYPE_NETEFFECT:
    			return (&ipmi_neteffect_sprom[slot]);
			break;

		case	SPROM_UTIL_TYPE_SCHULTZ:
    			return (&ipmi_schultz_sprom[slot]);
			break;

		case	SPROM_UTIL_TYPE_TIGERSHARK:
    			return (&ipmi_tigershark_sprom[slot]);
			break;

		case	SPROM_UTIL_TYPE_EVEREST:
    			return (&ipmi_everest_sprom[slot]);
			break;

		case	SPROM_UTIL_TYPE_BBU:
    			return (ipmi_bbu_sprom);
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

