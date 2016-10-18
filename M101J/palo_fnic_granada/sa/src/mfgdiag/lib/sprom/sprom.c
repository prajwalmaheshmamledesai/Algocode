/********************************************************************
 *
 *      File:  sprom.c
 *      Name:  matt strathman
 *
 *      Description: private sprom util file 
 *
 * Copyright (c) 2007 by Cisco Systems, Inc.
 * All rights reserved.
 *
 **********************************************************************/
/**********************************************************************
@file    sprom.c
@author  matt strathman -- mstrathman@nuovasystems.com
@brief   private sprom util file 
************************************************************************/

#include <stdio.h>
#include <sys/types.h>
#include "sprom.h"
#include "diag_sprom_defs.h"

uint16_t
diag_sprom_compute_xsum (void *gen_block, uint32_t len)
{
    uint16_t      xsum = 0;
    uint8_t       *pByte;
    sprom_block_header_t *block;

    block = (sprom_block_header_t *)gen_block;

    /* Go through the block and add up all fields a byte at a time: */
    for (pByte = (uint8_t *) block;
         pByte < ((uint8_t *) block) + len;
         pByte++)
    {
        xsum += (uint16_t) *pByte;
    }
    
    /* Now subtract the checksum field itself from the computation: */
    pByte = (uint8_t *) &block->block_checksum;
    xsum -= (uint16_t) *pByte++;
    xsum -= (uint16_t) *pByte;

    /* The checksum is in the native endian-ness of the processor. We return
     * it in big endian format by converting if necessary: */
    /* And that's it! */
    return (xsum);

}
