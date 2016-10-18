/********************************************************************
 *
 *      File:   woo_asic.c
 *      Name:   Bruce McLoughlin
 *
 *      Description: ASIC level functions for Redwood USD
 *
 *
 *      Copyright (c) 2001,2002,2003,2004 by Nuova Systems, Inc.
 ********************************************************************/
#include <sys/types.h>
#include "woo_ops.h"

#include "diag_sprom.h"
#include "diag_sprom_defs.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

static int board_sprom_get_hw_revision(diag_sprom_t *sprom, uint16_t *maj_rev, uint16_t *min_rev)
{
	int	  rc = 0;
	uint8_t*  rbuf;

	assert (sprom);

	rbuf = malloc (ipmi_sprom_get_size(sprom));
	if (rbuf == NULL) {
		return ( -SPROM_ERR_NOMEM );
	}

	if ((rc = ipmi_sprom_read (sprom, rbuf)) < 0) {
            free(rbuf);
            return (0);
	}

	switch (sprom->type) {
		case	SPROM_UTIL_TYPE_SUP:
		  *maj_rev = ((sup_sprom_t *)rbuf)->common_block.hw_rev_major;
		  *min_rev = ((sup_sprom_t *)rbuf)->common_block.hw_rev_minor;
                  rc = 0;
		  break;

		default:
		  rc = -SPROM_ERR_INVALID;
		  break;
	}

	free (rbuf);
	return (rc);
}

int fext_get_hw_revision(uint16_t *rev_maj, uint16_t *rev_min)
{
    int rc = 0;
    diag_sprom_t *dsprom = NULL;
    dsprom = ipmi_sprom_init(SPROM_PLATFORM_FEX, SPROM_UTIL_TYPE_SUP, 0);
    if (!dsprom) {
        printf("Error: sprom information failed\n");
        return (1);
    }
    rc = board_sprom_get_hw_revision(dsprom, rev_maj, rev_min);
    if (rc) {
        printf("Error: Failure in getting hw revision\n");
        return (1);
    }
    return (rc);
}


int woo_get_rev_id (int inst) 
{
    uint32_t data = 0;

    asic_rd(inst, ASIC(MISC_JTAG_ID), &data);

    if (data == 0x1033E57F) {
        return(1);
    } else if (data == 0x2033E57F) {
        return(2);
    }

    return(-1);
}
