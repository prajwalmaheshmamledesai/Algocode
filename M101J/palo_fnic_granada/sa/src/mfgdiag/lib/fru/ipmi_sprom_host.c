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

#if 0
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
#endif

#define SI2C_BMC_FRU_ADDR	0x02
#define SI2C_MEZZ_FRU_ADDR_0	0x01
#define SI2C_MEZZ_FRU_ADDR_1	0x03

#define SI2C_MEZZ_FRU_ADDR_2    0x04
#define SI2C_MEZZ_FRU_ADDR_3    0x05
#define SI2C_MEZZ_FRU_ADDR_4    0x06

/////////////////////////////////
// BMC SPROM
/////////////////////////////////
diag_smb_dev_t ipmi_bmc_sup_dev[] =
{
{"/dev/i2c-0", 0, SI2C_BMC_FRU_ADDR, 0, 2, 1, 0},
};

diag_smb_acc_t ipmi_bmc_sprom_acc = 
{&ipmi_bmc_sup_dev[0], host_ipmi_sprom_rd, host_ipmi_sprom_wr};

diag_sprom_t ipmi_bmc_sprom[] = {
      { &ipmi_bmc_sprom_acc, NULL, NULL,
        "BMC", SPROM_UTIL_TYPE_IBMC, 0, NULL },
};

/////////////////////////////////
// MEZZ SPROM
/////////////////////////////////
diag_smb_dev_t ipmi_mezz_dev[] =
{
{"/dev/i2c-0", 0, SI2C_MEZZ_FRU_ADDR_0, 0, 2, 1, 0},
{"/dev/i2c-0", 0, SI2C_MEZZ_FRU_ADDR_1, 0, 2, 1, 0},
{"/dev/i2c-0", 0, SI2C_MEZZ_FRU_ADDR_2, 0, 2, 1, 0},
{"/dev/i2c-0", 0, SI2C_MEZZ_FRU_ADDR_3, 0, 2, 1, 0},
{"/dev/i2c-0", 0, SI2C_MEZZ_FRU_ADDR_4, 0, 2, 1, 0},
};

diag_smb_acc_t ipmi_mezz_sprom_acc[] = { 
  {&ipmi_mezz_dev[0], host_ipmi_sprom_rd, host_ipmi_sprom_wr},
  {&ipmi_mezz_dev[1], host_ipmi_sprom_rd, host_ipmi_sprom_wr},
  {&ipmi_mezz_dev[2], host_ipmi_sprom_rd, host_ipmi_sprom_wr},
  {&ipmi_mezz_dev[3], host_ipmi_sprom_rd, host_ipmi_sprom_wr},
  {&ipmi_mezz_dev[4], host_ipmi_sprom_rd, host_ipmi_sprom_wr},
};

diag_sprom_t ipmi_palo_sprom[] = {
  { &ipmi_mezz_sprom_acc[0], NULL, NULL,
    "PALO", SPROM_UTIL_TYPE_PALO, 0, NULL },
  { &ipmi_mezz_sprom_acc[1], NULL, NULL,
    "PALO", SPROM_UTIL_TYPE_PALO, 0, NULL },
  { &ipmi_mezz_sprom_acc[2], NULL, NULL,
    "PALO", SPROM_UTIL_TYPE_PALO, 0, NULL },
  { &ipmi_mezz_sprom_acc[3], NULL, NULL,
    "PALO", SPROM_UTIL_TYPE_PALO, 0, NULL },
  { &ipmi_mezz_sprom_acc[4], NULL, NULL,
    "PALO", SPROM_UTIL_TYPE_PALO, 0, NULL },
};

diag_sprom_t ipmi_menlo_sprom[] = {
  { &ipmi_mezz_sprom_acc[0], NULL, NULL,
    "MENLO", SPROM_UTIL_TYPE_MENLO, 0, NULL },
  { &ipmi_mezz_sprom_acc[1], NULL, NULL,
    "MENLO", SPROM_UTIL_TYPE_MENLO, 0, NULL },
};

diag_sprom_t ipmi_menloe_sprom[] = {
      { &ipmi_mezz_sprom_acc[0], NULL, NULL,
        "MENLOE", SPROM_UTIL_TYPE_MENLO_E, 0, NULL },
      { &ipmi_mezz_sprom_acc[1], NULL, NULL,
        "MENLOE", SPROM_UTIL_TYPE_MENLO_E, 0, NULL },
};

diag_sprom_t ipmi_oplin_sprom[] = {
      { &ipmi_mezz_sprom_acc[0], NULL, NULL,
        "OPLIN", SPROM_UTIL_TYPE_OPLIN, 0, NULL },
      { &ipmi_mezz_sprom_acc[1], NULL, NULL,
        "OPLIN", SPROM_UTIL_TYPE_OPLIN, 0, NULL },
};

/////////////////////////////////
// MONTEREYPARK SPROM
/////////////////////////////////
#define SI2C_LA_MP_FRU_ADDR_0	4
#define SI2C_LA_MP_FRU_ADDR_1	5
#define SI2C_SD1_MP_FRU_ADDR	6
#define SI2C_SD2_MP_FRU_ADDR_0	7
#define SI2C_SD2_MP_FRU_ADDR_1	8

diag_smb_dev_t ipmi_mpark_dev[] =
{
{"/dev/i2c-0", 0, SI2C_LA_MP_FRU_ADDR_0, 0, 2, 1, 0},
{"/dev/i2c-0", 0, SI2C_LA_MP_FRU_ADDR_1, 0, 2, 1, 0},
};

diag_smb_acc_t ipmi_mpark_sprom_acc[] = { 
  {&ipmi_mpark_dev[0], host_ipmi_sprom_rd, host_ipmi_sprom_wr},
  {&ipmi_mpark_dev[1], host_ipmi_sprom_rd, host_ipmi_sprom_wr},
};

diag_sprom_t ipmi_mpark_sprom[] = {
  { &ipmi_mpark_sprom_acc[0], NULL, NULL,
    "MPARK", SPROM_UTIL_TYPE_MONTEREYPARK, 0, NULL },
  { &ipmi_mpark_sprom_acc[1], NULL, NULL,
    "MPARK", SPROM_UTIL_TYPE_MONTEREYPARK, 0, NULL },
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
                        if ( slot >= sizeof ( ipmi_oplin_sprom ) / sizeof ( diag_sprom_t ) ) {
                            return ( NULL );
                        }
    			return (&ipmi_oplin_sprom[slot]);
			break;

		case	SPROM_UTIL_TYPE_MENLO:
                        if ( slot >= sizeof ( ipmi_menlo_sprom ) / sizeof ( diag_sprom_t ) ) {
                            return ( NULL );
                        }
    			return (&ipmi_menlo_sprom[slot]);
			break;

		case	SPROM_UTIL_TYPE_MENLO_E:
                        if ( slot >= sizeof ( ipmi_menloe_sprom ) / sizeof ( diag_sprom_t ) ) {
                            return ( NULL );
                        }
                        return (&ipmi_menloe_sprom[slot]);
                        break;

		case	SPROM_UTIL_TYPE_PALO:
                        if ( slot >= sizeof ( ipmi_palo_sprom ) / sizeof ( diag_sprom_t ) ) {
                            return ( NULL );
                        }
    			return (&ipmi_palo_sprom[slot]);
			break;

		case	SPROM_UTIL_TYPE_MONTEREYPARK:
                        if ( slot >= sizeof ( ipmi_mpark_sprom ) / sizeof ( diag_sprom_t ) ) {
                            return ( NULL );
                        }
    			return (&ipmi_mpark_sprom[slot]);
			break;

		default:
			break;
	}

	return (NULL);
}

sprom_platform_t
ipmi_sprom_platform_probe ( void )
{
    return ( SPROM_PLATFORM_HOST );
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

