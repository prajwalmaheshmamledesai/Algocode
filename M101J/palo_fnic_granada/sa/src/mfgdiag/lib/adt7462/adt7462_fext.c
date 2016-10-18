/********************************************************************
 *
 *      File:  adt7462_fext.c
 *      Name:  Raghu Boreda
 *  
 *      Description: FexT  adt7462 Initialization routines
 *  
 * Copyright (c) 2009 by Cisco Systems, Inc.
 * All rights reserved.
 *  
 *  
 * $Id:$
 * $Source:$
 * $Author:$
 * ---------------------------------------------------------------
 **********************************************************************/
/*******************************************************************//**
@file    adt7462_fext.c
@author  Raghu Boreda
@brief   adt7462 chip access routines

************************************************************************/
#include <stdio.h>
#include <sys/types.h>

#include "adt7462.h"
#include "ppc_lib.h"

int
adt7462_init (int fd, uint16_t i2c_addr, uint8_t fan_msk ) 
{
    int rc = 0;
    uint8_t cnt, count;
    uint8_t addr_data[][2] = {
	{ADT7462_REG_CONFIG1,    0x00},
	{ADT7462_REG_EASY_CN,    ADT7462_EASY_CN_OPTION2},
	{ADT7462_REG_PWM2_CN,    ADT7462_PWM_SPIN_TO_2sec |
				 ADT7462_PWM_OP_ACTIVE_LO |
				 ADT7462_PWM_BHVR_MANUAL},
	{ADT7462_REG_PWM3_CN,    ADT7462_PWM_SPIN_TO_2sec |
				 ADT7462_PWM_OP_ACTIVE_LO |
				 ADT7462_PWM_BHVR_MANUAL},
	{ADT7462_REG_PWM1_2_FRQ, (ADT7462_PWM_FRQ_LOW_35_3 << 
				  ADT7462_PWM_FRQ_PWM1_LOW) |
				 (ADT7462_PWM_FRQ_LOW_35_3 << 
				  ADT7462_PWM_FRQ_PWM2_LOW)},
	{ADT7462_REG_PWM3_4_FRQ, (ADT7462_PWM_FRQ_LOW_35_3 << 
				  ADT7462_PWM_FRQ_PWM3_LOW) |
				 (ADT7462_PWM_FRQ_LOW_35_3 << 
				  ADT7462_PWM_FRQ_PWM4_LOW)},
	{ADT7462_REG_CONFIG2,    ADT7462_CFG2_PWM_MODE |
				 ADT7462_CFG2_FAST_MEASURE},
	{ADT7462_REG_PWM_MAX,    ADT7462_PWM_MAX_DUTY_CYCLE},
	{ADT7462_REG_TACH_EN,    fan_msk},
	{ADT7462_REG_FAN_PRES,   fan_msk},
	{ADT7462_REG_PWM2,       ADT7462_PWM_HALF_DUTY_CYCLE},
	{ADT7462_REG_PWM3,       ADT7462_PWM_HALF_DUTY_CYCLE},
	{ADT7462_REG_CONFIG1,    ADT7462_CFG1_SETUP_COMPLETE |
				 ADT7462_CFG1_MONITOR},
	};

    count = sizeof(addr_data)/(2  * sizeof(uint8_t));
    for (cnt = 0; cnt < count; cnt++) {
        rc = adt7462_wr (fd, i2c_addr, addr_data[cnt][0], 1, &addr_data[cnt][1]);
	if (rc) {
		printf("Error: %s [0x%X]:0x%X\n", __func__, addr_data[cnt][0], addr_data[cnt][1]);
		return (rc);
	}
    }

    return ( rc );
}
