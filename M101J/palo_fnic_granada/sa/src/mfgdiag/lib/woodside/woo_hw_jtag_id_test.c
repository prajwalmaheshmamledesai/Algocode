/*****************************************************************************
 *      File:   woo_hw_jtag_id_test.c
 *      Name:   Raghu Boreda
 *
 *      Description: ASIC level functions for Woodside 
 *      Copyright (c) 2009 by Cisco Systems, Inc.
 *****************************************************************************/
#include <stdio.h>
#include <stdint.h>
#include "woo_woo_naxislave_define.h"
#include "woo_ops.h"
#include "woo_hw_error.h"

int asic_hw_jtag_id_test (uint8_t inst, unsigned int* addr_error, 
                         unsigned int* exp_data, unsigned int* got_data) 
{
    int ret_val = 0;
    *addr_error = ASIC(MISC_JTAG_ID);
    *exp_data = 0x1036757f;
  
    if ((ret_val=asic_rd(inst,(*addr_error)*4, got_data))!=0) {
        return ret_val;
    }

    if ((*got_data != *exp_data)) {
        return WOODSIDE_ERROR_DEFAULT;
    }

    return ret_val;
}   
