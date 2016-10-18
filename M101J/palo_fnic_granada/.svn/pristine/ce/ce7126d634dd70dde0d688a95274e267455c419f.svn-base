/********************************************************************
 *
 *      File:   red_hw_reg_desc.c
 *      Name:   Sudharshan Kadari
 *
 *      Description:
 *	 Redwood H/W register description.       
 *
 *
 * Copyright (c) 1985-2007 by Cisco Systems, Inc.
 * All rights reserved.
 *
 *
 *********************************************************************/

#include <stdio.h>
#include <sys/types.h>
#include "diag_reg.h"
#include "red_red_naxislave_define.h"

extern reg_desc_t* find_reg_desc_by_addr(reg_desc_t *p_reg, unsigned int reg_addr );

#define RDW_DEF( blk, adr, name, desc, mask, rst_val, typ ) \
	REG_DEF( blk, adr*4, name, desc, mask, rst_val, typ )

#define RDW_DEF_FLD( adr, name, hi, lo, desc, cause ) \
	REG_DEF_FLD( adr*4, name, hi, lo, desc, cause )

#define RDW_DEF_DHS( blk, adr, name, desc, mask, rst_val, typ) \
  {  TYP_REG, blk, adr*4, 4, name, desc, mask, rst_val, typ },

#define RDW_DEF_DHS_FLD( adr, name, hi, lo, desc, cause ) \
  { TYP_FLD, "fld",  adr*4, 0, name, desc, hi, lo, cause },

#define RDW_DEF_SBUS( blk, adr, len, name, desc, mask, rst_val, typ ) \
  {  TYP_REG, blk, adr*4, len, name, desc, mask, rst_val, typ },

reg_desc_t rdw_reg_desc[] =
{
	#include "red_hw_reg_desc.h"
	{ TYP_NONE, "", 0, 0, "", "", 0, 0, ""}
};


char *redwood_reg_addr_to_name(uint32_t addr) 
{
	reg_desc_t *preg = NULL;
	
	preg = find_reg_desc_by_addr(rdw_reg_desc, addr);

	if (preg && preg->name)
		return (preg->name);
	
	return (NULL);
}
