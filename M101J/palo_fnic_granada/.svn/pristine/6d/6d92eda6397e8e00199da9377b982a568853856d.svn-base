/********************************************************************
 *      File:  psu_fext.c
 *      Name:  Raghu Boreda
 *
 *      Description: fext specific psu routines
 *
 * Copyright (c) 2009 by Cisco Systems, Inc.
 * All rights reserved.
 *  
 * $Id:$
 * $Source:$
 * $Author:$
 * ---------------------------------------------------------------
 **********************************************************************/
/**********************************************************************
@file    psu_fext.c
@author  Raghu Boreda
@brief   fext specific psu routines
************************************************************************/
#include <stdlib.h>
#include "psu.h"
#include "ppc_lib.h"
#include "fext.h"
#include "diag_main.h"

#define FEXT_PSU_MIN_INST 1
#define FEXT_PSU_MAX_INST 2

uint16_t ps_i2c_addr[]= {
			FEXT_PSU_SPROM_I2C_ADDR0,
			FEXT_PSU_SPROM_I2C_ADDR1,
		        };

int psu_present(int fd, uint8_t ps_inst)
{
	return (1);
}

int
psu_rd (int fd,  uint8_t ps_inst, /**< psu instance index */
             uint8_t offset,  /**< register offset */
             uint8_t *val     /**< register read value */ )
{   
	int rc = 0;
	rc= (diag_i2c_rd(fd, ps_i2c_addr[ps_inst], offset, sizeof(offset), 
				(uint8_t*)val, sizeof(uint16_t)));
	if (!rc) {
		uint8_t tmp = val[0];
		val[0]=val[1];
		val[1]=tmp;
	}
	return (rc);
}
    
int 
psu_wr ( int fd, uint8_t ps_inst, /**< psu instance index */
             uint8_t offset,  /**< register offset */
             uint16_t val      /**< register read value */ )
{   
	return (diag_i2c_wr(fd, ps_i2c_addr[ps_inst], offset, sizeof(offset), 
				(uint8_t*)&val, sizeof(uint16_t)));
}   

int
psu_get_min_inst_iom ( void )
{
    return ( FEXT_PSU_MIN_INST );
}

int
psu_get_max_inst_iom ( void )
{
    return ( FEXT_PSU_MAX_INST );
}
