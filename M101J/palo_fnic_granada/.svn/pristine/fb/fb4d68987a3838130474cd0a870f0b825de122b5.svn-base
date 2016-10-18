/********************************************************************
 *
 *      File:  psu_iom.c
 *      Name:  Matt Strathman
 *
 *      Description: iom specific psu routines
 *
 * Copyright (c) 2007 by Cisco Systems, Inc.
 * All rights reserved.
 *  
 *  
 * $Id:$
 * $Source:$
 * $Author:$
 * ---------------------------------------------------------------
 **********************************************************************/
/**********************************************************************
@file    psu_iom.c
@author  Matt Strathman -- mstrathman@nuovasystems.com
@brief   iom specific psu routines
************************************************************************/
#include <stdlib.h>
#include "psu.h"
#include "ppc_lib.h"
#include "diag_iom.h"
#include "diag_main.h"

#define SANTA_CLARA_PSU_MIN_INST 1
#define SANTA_CLARA_PSU_MAX_INST 4

uint16_t ps_i2c_addr[]= {
			SI2C_P_PSU0_PSMI_ADDR,
			SI2C_P_PSU1_PSMI_ADDR,
			SI2C_P_PSU2_PSMI_ADDR,
			SI2C_P_PSU3_PSMI_ADDR,
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
    return ( SANTA_CLARA_PSU_MIN_INST );
}

int
psu_get_max_inst_iom ( void )
{
    return ( SANTA_CLARA_PSU_MAX_INST );
}
