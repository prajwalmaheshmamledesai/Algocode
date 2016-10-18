/*****************************************************************************
 *      File:   woo_hw_sat_reg_test.c
 *      Name:   Raghu Boreda
 *
 *      Description: A test to check read-onlyness on all status registers 
 *                   in woodside
 *      Copyright (c) 2009 by Cisco Systems, Inc.
 *****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "woo_hw_sta_reg_test.h"
#include "woo_hw_error.h"
#include "woo_ops.h"
/*******************************************************************************
 * A test that scrubs all status registers in woodside
 * It first reads the default value
 * Then it writes a value 0xffffffff
 * Then it reads it back and checks whether original value is read.
 * Then it writes a value 0x0
 * Then it reads it back and checks whether original value is read.
 * \param addr_error returns the address where the first error occurred (byte 
 *        aligned)
 * \param exp_data returns the data that was expected
 * \param got_data returns the data that was received
 * \return 0 on success, an error code otherwise
 ******************************************************************************/
int woo_hw_sta_reg_test (int inst, unsigned int* addr_error,
                         unsigned int* exp_data, unsigned int* got_data) 
{
    int ret_val, i;
    unsigned int read_data_old;
    unsigned int read_data_new, write_data;
    const int TABLE_SIZE = 
              (sizeof(woo_hw_sta_reg_table)/sizeof(woo_sta_reg_table_entry_t));
    for (i=0; i<TABLE_SIZE; ++i) {
        const int byte_address = woo_hw_sta_reg_table[i].word_addr<<2;
        *addr_error=byte_address;

        if ((ret_val=asic_rd(inst, byte_address, &read_data_old))!=0) {
            return ret_val;
        }

        if (read_data_old != woo_hw_sta_reg_table[i].init) {
          /*  printf("Warning: Index %d Expected initial value %x"
                   " actual value %x\n",
                   i, woo_hw_sta_reg_table[i].init, read_data_old);
           */
        }
        // write a 0xffffffff 
        write_data= 0xffffffff;
        if ((ret_val=asic_wr(inst,byte_address, write_data))!=0) {
            return ret_val;
        }
        // read it back
        if ((ret_val=asic_rd(inst, byte_address, &read_data_new))!=0) {
            return ret_val;
        }
        // compare it to the write value
        if (read_data_old != read_data_new) {
            *exp_data=read_data_old;
            *got_data=read_data_new;
            return WOODSIDE_ERROR_DEFAULT;
        }
        // write a 0x0 
        write_data= 0x0;
        if ((ret_val=asic_wr(inst,byte_address, write_data))!=0) {
            return ret_val;
        }
        // read it back
        if ((ret_val=asic_rd(inst, byte_address, &read_data_new))!=0) {
            return ret_val;
        }
        // compare it to the write value
        if (read_data_old != read_data_new) {
            *exp_data=read_data_old;
            *got_data=read_data_new;
            return WOODSIDE_ERROR_DEFAULT;
        }
    }
    return 0; /*success*/
}
