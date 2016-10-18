/*****************************************************************************
 *      File:   prt_hw_sat_reg_test.c
 *      Name:   Raghu Boreda
 *
 *      Description: A test to check read-onlyness on all status registers 
 *                   in Portola
 *      Copyright (c) 2009 by Cisco Systems, Inc.
 *****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "woo_ops.h"
#include "prt_hw_sta_reg_test.h"
#include "woo_hw_error.h"
#include "diag_main.h"
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
static prt_sta_reg_table_t prt_sta_table[] = {
       {"Portola Misc Registers",PRT_MISC_BASE_ADDR, 
         PRT_STAT_MISC_TBL_SZ, prt_sta_reg_misc},
       {"Portola BI Registers",PRT_BI_BASE_ADDR,
         PRT_STAT_BI_TBL_SZ, prt_sta_reg_bi},
       {"Portola CI Registers",PRT_CI_BASE_ADDR,
         PRT_STAT_CI_TBL_SZ, prt_sta_reg_ci},
       {"Portola PC Registers",PRT_PC_BASE_ADDR,
         PRT_STAT_PC_TBL_SZ, prt_sta_reg_pc},
       {"Portola RS EDMA Registers",PRT_RS_EDMA_BASE_ADDR, 
         PRT_STAT_RS_EDMA_TBL_SZ, prt_sta_reg_rs_edma},
       {"Portola RS PCIE Registers",PRT_RS_PCIE_BASE_ADDR,
         PRT_STAT_RS_PCIE_TBL_SZ, prt_sta_reg_rs_pcie},
       {"Portola NR Registers",PRT_NR_BASE_ADDR,
         PRT_STAT_NR_TBL_SZ, prt_sta_reg_nr},
       {"Portola HR Registers",PRT_HR_BASE_ADDR,
         PRT_STAT_HR_TBL_SZ, prt_sta_reg_hr},
       {"Portola NF Registers",PRT_NF_BASE_ADDR,
         PRT_STAT_NF_TBL_SZ, prt_sta_reg_nf},
};

static int status_reg_test(int inst, unsigned int byte_address, 
                           unsigned int init_value,
                           unsigned int *exp_data, unsigned int *got_data) 
{
    int ret_val = 0;
    unsigned int read_data_old;
    unsigned int read_data_new, write_data;
    if ((ret_val=asic_rd(inst, byte_address, &read_data_old))!=0) {
        return ret_val;
    }
  
    if (read_data_old != init_value) {
        DIAG_PRINT(DIAG_PRINT_TEST_DEBUG, "Warning: Offset Address 0x%X "
                   "Expected initial value %x  actual value %x\n",
               (byte_address>>2), init_value, read_data_old);
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
    return (ret_val);
}

int prt_hw_sta_reg_test (int inst, unsigned int* addr_error,
                         unsigned int* exp_data, unsigned int* got_data) 
{
    unsigned int index, i;
    int rc;
    unsigned int ba;

    const unsigned int PRT_STA_TABLE_SIZE = 
              (sizeof(prt_sta_table)/sizeof(prt_sta_reg_table_t));
    for (index=0; index<PRT_STA_TABLE_SIZE; index++) {
        for(i=0;i < prt_sta_table[index].size; i++) {
            ba = (prt_sta_table[index].base_addr + 
                  prt_sta_table[index].table[i].word_addr)<<2;
            *addr_error=ba;
            rc = status_reg_test(inst, ba, 
                                 prt_sta_table[index].table[i].init,
                                 exp_data, got_data); 
            if (rc == WOODSIDE_ERROR_DEFAULT) {
                printf("Sec %s Index %d Base 0x%X offset 0x%X E 0x%X R 0x%X\n", 
                     prt_sta_table[index].name, i, 
                     prt_sta_table[index].base_addr, (ba>>2),
                     prt_sta_table[index].table[i].init, *got_data);
            }
        }
    }
    /* TEST HI Port Status registers */
    for (index=0; index<PRT_HI_COUNT; index++) {
        for(i=0;i < PRT_STAT_HI_TBL_SZ; i++) {
            ba = (HI_BASE_ADDR(index) + 
                  prt_sta_reg_hi[i].word_addr)<<2;
            *addr_error=ba;
            rc = status_reg_test(inst, ba, 
                                 prt_sta_reg_hi[i].init,
                                 exp_data, got_data); 
            if (rc == WOODSIDE_ERROR_DEFAULT) {
                printf("Sec %s HI%d Index %d offset 0x%X E 0x%X R 0x%X\n", 
                     "HI Registers", index, i, (ba>>2),
                     prt_sta_reg_hi[i].init, *got_data);
            }
        }
    }
    /* TEST NI Port Status registers */
    for (index=0; index<PRT_NI_COUNT; index++) {
        for(i=0;i < PRT_STAT_NI_TBL_SZ; i++) {
            ba = (NI_BASE_ADDR(index) + 
                  prt_sta_reg_ni[i].word_addr)<<2;
            *addr_error=ba;
            rc = status_reg_test(inst, ba, 
                                 prt_sta_reg_ni[i].init,
                                 exp_data, got_data); 
            if (rc == WOODSIDE_ERROR_DEFAULT) {
                printf("Sec %s NI%d Index %d offset 0x%X E 0x%X R 0x%X\n", 
                     "NI Registers", index, i, (ba>>2),
                     prt_sta_reg_ni[i].init, *got_data);
            }
        }
    }
    /* TEST SS Port Status registers */
    for (index=0; index<PRT_SS_COUNT; index++) {
        for(i=0;i < PRT_STAT_SS_TBL_SZ; i++) {
            ba = (SS_BASE_ADDR(index) + 
                  prt_sta_reg_ss[i].word_addr)<<2;
            *addr_error=ba;
            rc = status_reg_test(inst, ba, 
                                 prt_sta_reg_ss[i].init,
                                 exp_data, got_data); 
            if (rc == WOODSIDE_ERROR_DEFAULT) {
                printf("Sec %s SS%d Index %d offset 0x%X E 0x%X R 0x%X\n", 
                     "SS Registers", index, i, (ba>>2),
                     prt_sta_reg_ss[i].init, *got_data);
            }
        }
    }
    /* TEST HF Port Status registers */
    for (index=0; index<PRT_HF_COUNT; index++) {
        for(i=0;i < PRT_STAT_HF_TBL_SZ; i++) {
            ba = (HF_BASE_ADDR(index) + 
                  prt_sta_reg_hf[i].word_addr)<<2;
            *addr_error=ba;
            rc = status_reg_test(inst, ba, 
                                 prt_sta_reg_hf[i].init,
                                 exp_data, got_data); 
            if (rc == WOODSIDE_ERROR_DEFAULT) {
                printf("Sec %s HF%d Index %d offset 0x%X E 0x%X R 0x%X\n", 
                     "HF Registers", index, i, (ba>>2),
                     prt_sta_reg_hf[i].init, *got_data);
            }
        }
    }
    return 0; /*success*/
}
