/********************************************************************
 *
 *      File:  sprom_api.c
 *      Name:  matt strathman 
 *
 *      Description: Nuova sprom access api 
 *
 * Copyright (c) 2007 by Cisco Systems, Inc.
 * All rights reserved.
 *
 **********************************************************************/
/**********************************************************************
@file    sprom_api.c
@author  matt strathman -- mstrathman@nuovasystems.com
@brief   nuova sprom access api 
************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>

#include "sprom.h"
#include "diag_sprom_defs.h"

sprom_platform_t sprom_get_platform ( void )
{
	return (SPROM_PLATFORM_FEX);
}

int sprom_get_slot_sup (sprom_platform_t platform, uint32_t *slot) 
{
	int rc = 0;
	switch (platform) {
		case	SPROM_PLATFORM_FEX:
			*slot = 0;
			break;

		case	SPROM_PLATFORM_IOM:
			*slot = 0;
			break;

		default:
			rc = -SPROM_ERR_INVALID;
			break;
	}

	return (rc);
}

int sprom_get_slot_bp (sprom_platform_t platform, uint32_t *slot)
{
	int rc = 0;

	switch ( platform ) {
		case	SPROM_PLATFORM_FEX:
			*slot = 0;
			break;

		case	SPROM_PLATFORM_IOM:
			*slot = 0;
			break;

		default:
			rc = -SPROM_ERR_INVALID;
			break;
	}

	return (rc);
}


int
sprom_get_slot_chassis ( sprom_platform_t platform, uint32_t *slot)
{
	int rc = 0;

	switch (platform) {
		case	SPROM_PLATFORM_FEX:
			*slot = 0;
			break;

		case	SPROM_PLATFORM_IOM:
			*slot = 0;
			break;

		default:
			rc = -SPROM_ERR_INVALID;
			break;
	}

	return (rc);
}

diag_sprom_t * const sprom_init (sprom_platform_t platform, uint32_t util_type, 
				 uint32_t slot)
{
	diag_sprom_t *rc = 0;

	switch (platform) {
		case	SPROM_PLATFORM_FEX:
		case	SPROM_PLATFORM_FEXT:
			rc = sprom_init_pfm (util_type, slot);
			break;

		case	SPROM_PLATFORM_IOM:
			rc = sprom_init_pfm (util_type, slot);
			break;

		default:
			rc = NULL;
	}

	return (rc);
}

char * const sprom_get_name ( const diag_sprom_t *sprom ) 
{
    return (sprom->name);
}

int
sprom_get_size ( const diag_sprom_t *sprom ) 
{
	int siz = 0;
 
	switch (sprom->type) {

		case	SPROM_UTIL_TYPE_SUP:
		case	SPROM_UTIL_TYPE_BROCKWAY_UPLINK_SUP:
			siz = sizeof(sup_sprom_t);
			break;

		case	SPROM_UTIL_TYPE_BP:
		case	SPROM_UTIL_TYPE_BROCKWAY_UPLINK_BP:
			siz = sizeof(backplane_sprom_t);
			break;

		case	SPROM_UTIL_TYPE_PS:
			siz = sizeof(power_supply_sprom_t);
			break;

		case	SPROM_UTIL_TYPE_FAN:
			siz = sizeof(fan_sprom_t);
			break;

		default:
			siz = -SPROM_ERR_INVALID;
			break;
	}

	return (siz);
}

int
sprom_read (diag_sprom_t * const sprom, uint8_t *rbuf ) 
{
	int rc = 0;

	// need to add a semaphore here
	if (!sprom->cache_valid && sprom->cache ) {
		if ((rc = diag_sprom_rd (sprom, 0, 
				sprom_get_size (sprom), 
				sprom->cache)) < 0) {
			return ( rc );
		}    

		if ((rc = sprom_ntoh (sprom, sprom->cache)) < 0) {
			return ( rc );
		}
 
		sprom->cache_valid = 0;
	}

	memcpy (rbuf, sprom->cache, sprom_get_size ( sprom ) );

	return ( 0 );
}

int
sprom_write ( diag_sprom_t * const sprom, uint8_t *wbuf )
{
	int rc = 0;

	// need to add a semaphore here
	if ((rc = sprom_hton (sprom, wbuf)) < 0) {
		return ( rc );                             
	}

	if ((rc = diag_sprom_wr(sprom, 0,
				sprom_get_size (sprom), 
				wbuf)) < 0 ) {
		return ( rc );
	}

	sprom->cache_valid = 0;

	return ( 0 );
}

int
sprom_get_card_index ( diag_sprom_t *sprom , uint16_t* card_index)
{
	int	  rc = 0;
	uint8_t*  rbuf;

	assert (sprom);

	rbuf = malloc (sprom_get_size(sprom));
	if (rbuf == NULL) {
		return ( -SPROM_ERR_NOMEM );
	}

	if ((rc = sprom_read (sprom, rbuf)) < 0) {
		goto sprom_get_card_id_exit;
	}

	switch (sprom->type) {
		case	SPROM_UTIL_TYPE_SUP:
		case	SPROM_UTIL_TYPE_BROCKWAY_UPLINK_SUP:
			*card_index = ((sup_sprom_t *)rbuf)->sup_block.card_index;
			break;

		case	SPROM_UTIL_TYPE_PS:
		case	SPROM_UTIL_TYPE_BP:
		case	SPROM_UTIL_TYPE_BROCKWAY_UPLINK_BP:
		default:
			rc = -SPROM_ERR_INVALID;
			goto sprom_get_card_id_exit;
			break;
	}

sprom_get_card_id_exit:
	free ( rbuf );
	return ( 0 );
}

int sprom_get_hw_revision(diag_sprom_t *sprom, uint16_t *maj_rev, 
                          uint16_t *min_rev)
{
	int	  rc = 0;
	uint8_t*  rbuf;

	assert (sprom);

	rbuf = malloc (sprom_get_size(sprom));
	if (rbuf == NULL) {
		return ( -SPROM_ERR_NOMEM );
	}

	if ((rc = sprom_read (sprom, rbuf)) < 0) {
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
