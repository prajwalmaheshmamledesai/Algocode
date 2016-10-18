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
#include "at24c.h"

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

/*
static void bmc_mezz_sprom_we ( void )
{
}
*/

/*
static void bmc_mezz_sprom_wd ( void )
{
}
*/

#define SI2C_CHS_FRU_ADDR       0x00
#define SI2C_HDDBP_FRU_ADDR     0x01
#define SI2C_BMC_FRU_ADDR       0x02
#define SI2C_PSU_FRU_ADDR_0     0x03
#define SI2C_PSU_FRU_ADDR_1     0x04
#define SI2C_PSU_FRU_ADDR_2     0x05
#define SI2C_PSU_FRU_ADDR_3     0x06
#define SI2C_MEZZ_FRU_ADDR_0    0x07
#define SI2C_MEZZ_FRU_ADDR_1    0x08
#define SI2C_MEZZ_FRU_ADDR_2    0x09
#define SI2C_MEZZ_FRU_ADDR_3    0x0A
#define SI2C_MEZZ_FRU_ADDR_4    0x0B
#define SI2C_MEZZ_FRU_ADDR_5    0x0C
#define SI2C_MEZZ_FRU_ADDR_6    0x0D
#define SI2C_MEZZ_FRU_ADDR_7    0x0E
#define SI2C_FP_FRU_ADDR        0x0F
#define SI2C_PDB_FRU_ADDR       0x10
#define SI2C_IOR_FRU_ADDR       0x11
#define SI2C_MPARK_FRU_ADDR_0   0x12
#define SI2C_MPARK_FRU_ADDR_1   0x13

#define PAGE_SIZE               32

extern int ppc_i2c_open(int inst);
extern void ppc_i2c_close(int fd);

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
#define LA_PS_SPROM_MAX	4
diag_smb_dev_t ipmi_la_ps_dev[] =
{
{"/dev/i2c-4", 4, SI2C_PSU_FRU_ADDR_0, 0, 2, 1, 0},
{"/dev/i2c-4", 4, SI2C_PSU_FRU_ADDR_1, 0, 2, 1, 0},
{"/dev/i2c-4", 4, SI2C_PSU_FRU_ADDR_2, 0, 2, 1, 0},
{"/dev/i2c-4", 4, SI2C_PSU_FRU_ADDR_3, 0, 2, 1, 0},
};

diag_smb_acc_t ipmi_la_ps_sprom_acc[] =
{
{&ipmi_la_ps_dev[0], sprom_rd, sprom_wr},
{&ipmi_la_ps_dev[1], sprom_rd, sprom_wr},
{&ipmi_la_ps_dev[2], sprom_rd, sprom_wr},
{&ipmi_la_ps_dev[3], sprom_rd, sprom_wr},
};

static uint8_t ipmi_sprom_ps_cache[LA_PS_SPROM_MAX][sizeof(sprom_ipmi_iom_psu_t)];
diag_sprom_t ipmi_la_ps_sprom[] = {
      { &ipmi_la_ps_sprom_acc[0], bmc_ps_sprom_we, bmc_ps_sprom_wd,
        "PS", SPROM_UTIL_TYPE_PS, 0, ipmi_sprom_ps_cache[0] },
      { &ipmi_la_ps_sprom_acc[1], bmc_ps_sprom_we, bmc_ps_sprom_wd,
        "PS", SPROM_UTIL_TYPE_PS, 0, ipmi_sprom_ps_cache[1]},
      { &ipmi_la_ps_sprom_acc[2], bmc_ps_sprom_we, bmc_ps_sprom_wd,
        "PS", SPROM_UTIL_TYPE_PS, 0, ipmi_sprom_ps_cache[2] },
      { &ipmi_la_ps_sprom_acc[3], bmc_ps_sprom_we, bmc_ps_sprom_wd,
        "PS", SPROM_UTIL_TYPE_PS, 0, ipmi_sprom_ps_cache[3]},
};

/////////////////////////////////
// HDDBP SPROM
/////////////////////////////////
#define BMC_HDDBP_SPROM_MAX 2
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
        "FP", SPROM_UTIL_TYPE_BP, 0, ipmi_sprom_bp_cache },
};

/////////////////////////////////
// MontereyPark SPROM
/////////////////////////////////
#define SD_MP_SPROM_MAX                 2

diag_smb_dev_t ipmi_mpark_dev[] =
{
{"/dev/i2c-11", 0x2, SI2C_MPARK_FRU_ADDR_0,  0, 2, 1, 0},
{"/dev/i2c-22", 0x12, SI2C_MPARK_FRU_ADDR_1,  0, 2, 1, 0},
};

extern int mp_sprom_rd (diag_smb_dev_t *smb_dev, uint32_t offset, uint32_t len, uint8_t *buf);
extern int mp_sprom_wr (diag_smb_dev_t *smb_dev, uint32_t offset, uint32_t len, uint8_t *buf);

diag_smb_acc_t ipmi_mpark_sprom_acc[] =
{
{&ipmi_mpark_dev[0], mp_sprom_rd, mp_sprom_wr},
{&ipmi_mpark_dev[1], mp_sprom_rd, mp_sprom_wr},
//{&ipmi_mpark_dev[0], sprom_rd, sprom_wr},
//{&ipmi_mpark_dev[1], sprom_rd, sprom_wr},
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
        ipmi_mpark_dev[0].addr = SI2C_MPARK_FRU_ADDR_0;
        ipmi_mpark_dev[1].addr = SI2C_MPARK_FRU_ADDR_1;
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

        case    SPROM_UTIL_TYPE_MONTEREYPARK:
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



uint32_t ipmi_fru_test(uint8_t slot, uint8_t platform, uint8_t type, uint32_t page_cnt, uint8_t verbose)
{
    uint8_t *orig_data = NULL;
    uint8_t *test_pattern = NULL;
    uint8_t *temp_data = NULL;
	diag_sprom_t *dsprom = NULL;
	int rc = 0;
    uint8_t  i2c_bus, i2c_addr;
    uint32_t start_addr;
    uint32_t len, i;

	// Verify SPROM
	dsprom = ipmi_sprom_init(platform, type, slot);
	if (!dsprom) {
		printf("Error: sprom information failed\n");
		return (-1);
	}

    i2c_bus  = dsprom->smb_acc->smb_dev->path;
	i2c_addr = dsprom->smb_acc->smb_dev->addr;
	start_addr = dsprom->smb_acc->smb_dev->offset;

    len = page_cnt * PAGE_SIZE;
    orig_data = malloc(len);
    temp_data = malloc(len);
    test_pattern = malloc(len);

    printf("[INFO] Backup FRU Data     ... ");
    rc = sprom_rd(dsprom->smb_acc->smb_dev, start_addr, len, orig_data);
    if (rc)
    {
        printf("FAIL\n");
    }
    else
    {
        printf("PASS\n");
    }

    if (verbose)
    {
        for (i = 0; i < len; i++)
        {
            printf("%2x ", orig_data[i]);
        }
        printf("\n\n");
    }
    
    printf("[INFO] Write Pattern 0xAA  ... ");
    memset(test_pattern, 0xAA, len);
    rc = sprom_wr(dsprom->smb_acc->smb_dev, start_addr, len, test_pattern);
    if (rc)
    {
        printf("FAIL\n");
    }
    else
    {
        printf("PASS\n");
    }

    if (verbose)
    {
        for (i = 0; i < len; i++)
        {
            printf("%2x ", test_pattern[i]);
        }
        printf("\n\n");
    }

    printf("[INFO] Read Test Pattern   ... ");
    rc = sprom_rd(dsprom->smb_acc->smb_dev, start_addr, len, temp_data);
    if (rc)
    {
        printf("FAIL\n");
    }
    else
    {
        printf("PASS\n");
    }

    if (verbose)
    {
        for (i = 0; i < len; i++)
        {
            printf("%2x ", temp_data[i]);
        }
        printf("\n\n");
    }

    printf("[INFO] Verify Test Pattern ... ");
    if (memcmp(temp_data, test_pattern, len))
    {
        rc = -1;
        printf("FAIL\n");
        goto cleanup;
    }
    else
    {
        printf("PASS\n");
    }

    printf("[INFO] Write Pattern 0x55  ... ");
    memset(test_pattern, 0x55, len);
    rc = sprom_wr(dsprom->smb_acc->smb_dev, start_addr, len, test_pattern);
    if (rc)
    {
        printf("FAIL\n");
    }
    else
    {
        printf("PASS\n");
    }

    if (verbose)
    {
        for (i = 0; i < len; i++)
        {
            printf("%2x ", test_pattern[i]);
        }
        printf("\n\n");
    }

    printf("[INFO] Read Test Pattern   ... ");
    memset(test_pattern, 0x55, len);
    rc = sprom_rd(dsprom->smb_acc->smb_dev, start_addr, len, temp_data);
    if (rc)
    {
        printf("FAIL\n");
    }
    else
    {
        printf("PASS\n");
    }

    if (verbose)
    {
        for (i = 0; i < len; i++)
        {
            printf("%2x ", temp_data[i]);
        }
        printf("\n\n");
    }

    printf("[INFO] Verify Test Pattern ... ");
    if (memcmp(temp_data, test_pattern, len))
    {
        rc = -1;
        printf("FAIL\n");
        goto cleanup;
    }
    else
    {
        printf("PASS\n");
    }

cleanup:
    printf("[INFO] Restore FRU Data    ... ");
    rc = sprom_wr(dsprom->smb_acc->smb_dev, start_addr, len, orig_data);
    if (rc)
    {
        printf("FAIL\n");
    }
    else
    {
        printf("PASS\n");
    }

    if (verbose)
    {
        for (i = 0; i < len; i++)
        {
            printf("%2x ", orig_data[i]);
        }
        printf("\n\n");
    }

    free(orig_data);
    free(temp_data);
    free(test_pattern);

    return rc;
}


uint32_t smbus_fru_test(uint8_t i2c_bus, uint8_t i2c_addr, uint32_t start_addr, uint32_t page_cnt, uint8_t verbose)
{
    uint8_t *orig_data = NULL;
    uint8_t *test_pattern = NULL;
    uint8_t *temp_data = NULL;
	int rc = 0, fd = -1;
    uint32_t len, i;

    fd = ppc_i2c_open(i2c_bus);
    
    len = page_cnt * PAGE_SIZE;
    orig_data = malloc(len);
    temp_data = malloc(len);
    test_pattern = malloc(len);

    printf("[INFO] Backup FRU Data     ... ");
    rc = at24c_rd(fd, i2c_bus, i2c_addr, start_addr, orig_data, len);
    if (rc)
    {
        printf("FAIL\n");
    }
    else
    {
        printf("PASS\n");
    }

    if (verbose)
    {
        for (i = 0; i < len; i++)
        {
            printf("%2x ", orig_data[i]);
        }
        printf("\n\n");
    }
    
    printf("[INFO] Write Pattern 0xAA  ... ");
    memset(test_pattern, 0xAA, len);
    rc = at24c_wr(fd, i2c_bus, i2c_addr, start_addr, test_pattern, len);
    if (rc)
    {
        printf("FAIL\n");
    }
    else
    {
        printf("PASS\n");
    }

    if (verbose)
    {
        for (i = 0; i < len; i++)
        {
            printf("%2x ", test_pattern[i]);
        }
        printf("\n\n");
    }

    printf("[INFO] Read Test Pattern   ... ");
    rc = at24c_rd(fd, i2c_bus, i2c_addr, start_addr, temp_data, len);
    if (rc)
    {
        printf("FAIL\n");
    }
    else
    {
        printf("PASS\n");
    }

    if (verbose)
    {
        for (i = 0; i < len; i++)
        {
            printf("%2x ", temp_data[i]);
        }
        printf("\n\n");
    }

    printf("[INFO] Verify Test Pattern ... ");
    if (memcmp(temp_data, test_pattern, len))
    {
        rc = -1;
        printf("FAIL\n");
        goto cleanup;
    }
    else
    {
        printf("PASS\n");
    }

    printf("[INFO] Write Pattern 0x55  ... ");
    memset(test_pattern, 0x55, len);
    rc = at24c_wr(fd, i2c_bus, i2c_addr, start_addr, test_pattern, len);
    if (rc)
    {
        printf("FAIL\n");
    }
    else
    {
        printf("PASS\n");
    }

    if (verbose)
    {
        for (i = 0; i < len; i++)
        {
            printf("%2x ", test_pattern[i]);
        }
        printf("\n\n");
    }

    printf("[INFO] Read Test Pattern   ... ");
    memset(test_pattern, 0x55, len);
    rc = at24c_rd(fd, i2c_bus, i2c_addr, start_addr, temp_data, len);
    if (rc)
    {
        printf("FAIL\n");
    }
    else
    {
        printf("PASS\n");
    }

    if (verbose)
    {
        for (i = 0; i < len; i++)
        {
            printf("%2x ", temp_data[i]);
        }
        printf("\n\n");
    }

    printf("[INFO] Verify Test Pattern ... ");
    if (memcmp(temp_data, test_pattern, len))
    {
        rc = -1;
        printf("FAIL\n");
        goto cleanup;
    }
    else
    {
        printf("PASS\n");
    }

cleanup:
    printf("[INFO] Restore FRU Data    ... ");
    rc = at24c_wr(fd, i2c_bus, i2c_addr, start_addr, orig_data, len);
    if (rc)
    {
        printf("FAIL\n");
    }
    else
    {
        printf("PASS\n");
    }

    if (verbose)
    {
        for (i = 0; i < len; i++)
        {
            printf("%2x ", orig_data[i]);
        }
        printf("\n\n");
    }

    ppc_i2c_close(fd);
    free(orig_data);
    free(temp_data);
    free(test_pattern);

    return rc;
}

