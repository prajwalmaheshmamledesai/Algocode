/********************************************************************
 *
 * File:  woo_spico.c
 * Author: Raghu Boreda
 * based on File:  ath_spico.c
 * Original Author:  Sanjay Gupta
 *
 * Copyright (c) 2009 by Cisco Systems, Inc.
 * All rights reserved.
 *
 **********************************************************************/
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include "woo_ops.h"
#include "diag_main.h"

#define CLI_OUT printf
#define FAILURE 1
#define SPICO_MANUAL_TUNING 1


static uint32_t* uc_data_ptr;
static uint32_t uc_filesize;

uint32_t asic_sbus_read (uint8_t inst, const uint32_t node, uint32_t addr)
{
   uint32_t val;
   asic_rd(inst, ASIC_SBUS_ADDR(node,addr), &val);
   // CLI_OUT ("0x%x -> 0x%x\n",ATH_MISC_DHS_SBUS+(node<<8)+addr, val);
   return (val & 0xff);
}

uint32_t
asic_sbus_wr (uint8_t inst, const uint32_t node, uint32_t addr, uint32_t val)
{
   asic_wr(inst, ASIC_SBUS_ADDR(node,addr), val);
   // CLI_OUT ("0x%x <- 0x%x\n",ATH_MISC_DHS_SBUS+(node<<8)+addr, val);
   return 0x0;
}
#if defined(DEBUG_SBUS_TOGGLE)
static void get_sbus_dbg_info (uint8_t inst, uint32_t serdesNode,
                               uint32_t *pcsarray, uint32_t *serdesarray)
{
   uint32_t addr;
   asic_rd(inst, ASIC_PCS_ADDR(serdesNode,0xfe), pcsarray);
   asic_rd(inst, ASIC_PCS_ADDR(serdesNode,0xfd), pcsarray+1);
   asic_rd(inst, ASIC_PCS_ADDR(serdesNode,0xfc), pcsarray+2);
   asic_rd(inst, ASIC_PCS_ADDR(serdesNode,0xfb), pcsarray+3);
   asic_rd(inst, ASIC_PCS_ADDR(serdesNode,0xfa), pcsarray+4);
   for (addr=0;addr < 0x4f; addr++) {
        asic_rd(inst, ASIC_SBUS_ADDR(serdesNode,addr), (serdesarray+addr));
   }
}

int asic_toggle_serdes_data_gate (uint8_t inst, int port, int hifport)
{
#if defined(DEBUG_SBUS_FAILURES)
    int index, address;
    uint32_t val_b4_fe[5], val_b4[0x4f], val_af_fe[5], val_af[0x4f];
#endif
    uint32_t node;
    if (hifport) {
        node = ASIC_HI_SERDES_SBUS_DEVID(port);
    } else {
        node = ASIC_NI_SERDES_SBUS_DEVID(port);
    }
#if defined(DEBUG_SBUS_FAILURES)
    get_sbus_dbg_info(inst, node, val_b4_fe, val_b4);
#endif
    printf("Toggling Serdes gate %d\n", port);
    asic_wr(inst, ASIC_SBUS_ADDR(node,0x3), 0x1);
    usleep(10);
    asic_wr(inst, ASIC_SBUS_ADDR(node,0x3), 0);
    sleep(1);
#if defined(DEBUG_SBUS_FAILURES)
    get_sbus_dbg_info(inst, node, val_af_fe, val_af);
    address = 0xfe; 
    for (index = 0; index < 5; index++) {
        printf("PCS [0x%x] = %x\t[0x%x] = %x\n", 
               (address-index),val_b4_fe[index], (address-index), val_af_fe[index]);
    }
    for (index = 0; index < 0x4f; index++) {
        printf("SBUS [0x%x] = %x\t[0x%x] = %x\n", 
               index,val_b4[index], index, val_af[index]);
    }
#endif
    return (0);
}
#endif

int asic_load_spico (uint8_t inst, char *filename)
{
    int rc = 0;
    FILE *datafile;
    struct stat st;
    uint32_t *bptr;
    uint32_t *data_ptr;
    uint32_t idx, cnt = 0;
    uint32_t rv0, rv1, rv2;

    /* woo_reset */
    DIAG_PRINT(DIAG_PRINT_CONFIG, "Reading microcode file:%s\n", filename);

    datafile = fopen(filename, "r");
    if ( datafile != NULL )
    {
      if (stat(filename, &st) == 0)
      {
        uc_filesize = st.st_size;
        DIAG_PRINT(DIAG_PRINT_CONFIG, "Microcode file size is:%d\n", 
                   uc_filesize);

        uc_data_ptr = malloc(uc_filesize);
        bptr = uc_data_ptr;
        
        while ( !feof(datafile) )
        {
          fscanf( datafile, "%x", bptr );
          bptr++;
          cnt++;
        }
        cnt--;
        fclose(datafile);
      }
    }
    else {
      printf("Failure reading microcode file\n");
      rc = FAILURE;
    }

    data_ptr = (uint32_t *)uc_data_ptr;

    asic_sbus_wr (inst, ASIC(SPICO_DEV_ID), 0x0c, 0x03);
    asic_sbus_wr (inst, ASIC(SPICO_DEV_ID), 0x0c, 0x01);
    asic_sbus_wr (inst, ASIC(SPICO_DEV_ID), 0x06, 0x08);
    for (idx = 0; idx < cnt; idx++)
    {
        asic_sbus_wr (inst, ASIC(SPICO_DEV_ID), 0x04, (idx >> 8) & 0xff );
        asic_sbus_wr (inst, ASIC(SPICO_DEV_ID), 0x05, (idx & 0xff) );
        asic_sbus_wr (inst, ASIC(SPICO_DEV_ID), 0x07, (*data_ptr & 0xff) );
        asic_sbus_wr (inst, ASIC(SPICO_DEV_ID), 0x06, (((*data_ptr) >> 8) & 0x3) | 0x0c);
        asic_sbus_wr (inst, ASIC(SPICO_DEV_ID), 0x06, (((*data_ptr) >> 8) & 0x3) | 0x08);
        data_ptr++;
        if (((idx+1) % 512) == 0) {
            DIAG_PRINT(DIAG_PRINT_CONFIG, 
                       "0x%x %d words\n", ASIC(SPICO_DEV_ID), idx+1);
        }
    }
    DIAG_PRINT(DIAG_PRINT_CONFIG,"Done\n");
    asic_sbus_wr (inst, ASIC(SPICO_DEV_ID), 0x06, 0x00);
    asic_sbus_wr (inst, ASIC(SPICO_DEV_ID), 0x0c, 0x0d);

    // Make sure CRC check is Ok.
    asic_sbus_wr (inst, ASIC(SPICO_DEV_ID), 0x01, 0x00);
    asic_sbus_wr (inst, ASIC(SPICO_DEV_ID), 0x02, 0x02);
    asic_sbus_wr (inst, ASIC(SPICO_DEV_ID), 0x03, 0x00);
    asic_sbus_wr (inst, ASIC(SPICO_DEV_ID), 0x0c, 0x1d);
    asic_sbus_wr (inst, ASIC(SPICO_DEV_ID), 0x0c, 0x0d);
    usleep(1000);
    rv0 = asic_sbus_read (inst, ASIC(SPICO_DEV_ID), 0x00);
    rv1 = asic_sbus_read (inst, ASIC(SPICO_DEV_ID), 0x01);
    rv2 = asic_sbus_read (inst, ASIC(SPICO_DEV_ID), 0x02);
    if ( (rv0 == 0) && (rv1 == 0x1) & (rv2 == 0x1) ) {
       DIAG_PRINT(DIAG_PRINT_CONFIG, 
                  "Checking SPICO microcode CRC :: Passed\n");
    } else {
       printf("Checking SPICO microcode CRC ::");
       printf(" FAILED. rv0= %d, rv1=%d, rv2=%d\n", rv0, rv1, rv2);
       return 1; 
    } 


    /* Interrupt command to change address 0x10 
     * to reduce number of cycles for fine tuning
     */
    asic_sbus_wr(inst, ASIC(SPICO_DEV_ID), 0x01, 0x00);
    asic_sbus_wr(inst, ASIC(SPICO_DEV_ID), 0x02, 0x11);
    asic_sbus_wr(inst, ASIC(SPICO_DEV_ID), 0x03, 0x10);
    asic_sbus_wr(inst, ASIC(SPICO_DEV_ID), 0x0c, 0x1d);
    asic_sbus_wr(inst, ASIC(SPICO_DEV_ID), 0x0c, 0x0d);
    /* change address 0x10 to 0x14 */
    asic_sbus_wr(inst, ASIC(SPICO_DEV_ID), 0x01, 0x00);
    asic_sbus_wr(inst, ASIC(SPICO_DEV_ID), 0x02, 0x12);
    asic_sbus_wr(inst, ASIC(SPICO_DEV_ID), 0x03, 0xa); /* 0x14 */
    asic_sbus_wr(inst, ASIC(SPICO_DEV_ID), 0x0c, 0x1d);
    asic_sbus_wr(inst, ASIC(SPICO_DEV_ID), 0x0c, 0x0d);

    /* Interrupt command to change address 
     * dwell time from 0x14 to 0xa
     */
    asic_sbus_wr(inst, ASIC(SPICO_DEV_ID), 0x01, 0x00);
    asic_sbus_wr(inst, ASIC(SPICO_DEV_ID), 0x02, 0x11);
    asic_sbus_wr(inst, ASIC(SPICO_DEV_ID), 0x03, 0x15);
    asic_sbus_wr(inst, ASIC(SPICO_DEV_ID), 0x0c, 0x1d);
    asic_sbus_wr(inst, ASIC(SPICO_DEV_ID), 0x0c, 0x0d);
    /* change address 0x15 to 0xa */
    asic_sbus_wr(inst, ASIC(SPICO_DEV_ID), 0x01, 0x00);
    asic_sbus_wr(inst, ASIC(SPICO_DEV_ID), 0x02, 0x12);
    asic_sbus_wr(inst, ASIC(SPICO_DEV_ID), 0x03, 0xa);
    asic_sbus_wr(inst, ASIC(SPICO_DEV_ID), 0x0c, 0x1d);
    asic_sbus_wr(inst, ASIC(SPICO_DEV_ID), 0x0c, 0x0d);
    return rc;
}
#define TUNING_ENABLE             0x01
#define COARSE_TUNING_MASK        0x30
#define COARSE_TUNING_NOT_STARTED 0x00
#define COARSE_TUNING_IN_PROGRESS 0x10
#define COARSE_TUNING_COMPLETED   0x20
#define COARSE_TUNING_ERRORS   0x30

#define FINE_TUNING_MASK          0xC
#define FINE_TUNING_NOT_STARTED   0x0
#define FINE_TUNING_IN_PROGRESS   0x4
#define FINE_TUNING_COMPLETED     0x8
#define FINE_TUNING_ERRORS        0xC

static unsigned int sbus_addr_val[][3] = {
    {0x00, 0x01, 0x37}, /* ref_sel/gate */
    {0x36, 0x01, 0x40}, /* tx_rate_sel */
    {0x3b, 0x01, 0x40}, /* rx_rate_sel */
    {0x22, 0x01, 0x00}, /* disable TX/RX */
    {0x5a, 0x01, 0x01}, /* kr mode gate */
    {0x63, 0x01, 0x06}, /* kr training restart*/
    {0x19, 0x01, 0xa8}, /* core_to_analog 7:0 */
    {0x1a, 0x01, 0x01}, /* core_to_analog lsb gate */
    {0x22, 0x01, 0x03}, /* tx_en/rx_en */
    {0x15, 0x01, 0x00}, /* */
    {0x16, 0x01, 0x00}, /* */
    {0x17, 0x01, 0x10}, /* */
    {0x0d, 0x01, 0xd1}, /* enable IBL/output */
    {0x04, 0x01, 0xc7}, /* error_mon_sel */
    {0x04, 0x01, 0x87}, /* error_mon_sel */
    {0x04, 0x01, 0x07}, /* error_mon_sel */
    {0x01, 0x02, 0x00}, /* read error reg */
    {0x0e, 0x02, 0x00}, /* read status reg */
};


static void asic_pre_dfe_tune_channel (uint8_t inst, uint32_t node)
{
    int index = 0;
    uint32_t value;
    int size = (sizeof(sbus_addr_val)/(3*sizeof(unsigned int)));

    for (index = 0; index < size; index++) {
        if (sbus_addr_val[index][1] == 0x01) {
            asic_sbus_wr(inst, node, sbus_addr_val[index][0], 
                         sbus_addr_val[index][2]);
        } else if(sbus_addr_val[index][1] == 0x02) {
            value = asic_sbus_read(inst, node, sbus_addr_val[index][0]);
            /* printf("Serdes %d Reg %x = %x\n", node, 
                   sbus_addr_val[index][0], value); */
        }
        if (((sbus_addr_val[index][0] == 0x22) &&
             (sbus_addr_val[index][2] == 0x03))) {
            usleep(50*1000);
        } 
    }
    return;
}

static void asic_coarse_tune_spico_channel (uint8_t inst, uint32_t node)
{
    asic_sbus_wr (inst, node, 0x2b, 2);
    return;
}

static void asic_fine_tune_spico_channel (uint8_t inst, uint32_t node)
{
    asic_sbus_wr (inst, node, 0x2a, 4);
    return;
}

static int asic_verify_spico_coarse_tuning (uint8_t inst, uint32_t node)
{
    uint32_t  dfe1_pass1, dummyread;
    uint32_t cnt, status, expStatus;

    dummyread = asic_sbus_read(inst, node, 0x20);
    dummyread = asic_sbus_read(inst, node, 0x21);
    dummyread = asic_sbus_read(inst, node, 0x22);
    dummyread = asic_sbus_read(inst, node, 0x23);

    for (cnt = 0; cnt < 20; cnt++) {
        dfe1_pass1 = asic_sbus_read (inst, node, 0x1f);
        status = (dfe1_pass1 & COARSE_TUNING_MASK); 
        if (status == COARSE_TUNING_IN_PROGRESS) {
            usleep (100);
        } else if (status == COARSE_TUNING_COMPLETED) {
            break;
        } else if (status == COARSE_TUNING_ERRORS) {
            printf(" Error: port %d :: Coarse Tuning Errors\n", (node-1)/3);
            return 1;
        } 
    }

    if ((cnt >= 20) && (status == COARSE_TUNING_IN_PROGRESS)) {
       printf(" WARNING: port %d :: "
              "Coarse tuning in Progress(Recd 0x%02x)\n", 
              (node-1)/3, dfe1_pass1);
       return 1;
    }

    expStatus = COARSE_TUNING_COMPLETED | TUNING_ENABLE;
    
    if ((dfe1_pass1 & (COARSE_TUNING_MASK | TUNING_ENABLE)) != expStatus) {
       printf(" WARNING: port %d :: "
               "Coarse tuning failure (Recd 0x%02x, Exp : 0x%02x)\n", 
               (node-1)/3, dfe1_pass1, expStatus);
       return 1;
    }

    return 0;
}

static int asic_verify_spico_fine_tuning (uint8_t inst, uint32_t node)
{
    uint32_t dfe1_pass2;
    uint32_t cnt, status, expStatus;

    for (cnt = 0; cnt < 20; cnt++) {
        dfe1_pass2 = asic_sbus_read (inst, node, 0x1f);
        status = (dfe1_pass2 & FINE_TUNING_MASK); 
        if (status == FINE_TUNING_IN_PROGRESS) {
            usleep (100);
        } else if (status == FINE_TUNING_COMPLETED) {
            break;
        } else if (status == FINE_TUNING_ERRORS) {
            printf(" Error: port %d :: Fine Tuning Errors\n", (node-1)/3);
            return 1;
        } 
    }


    expStatus = (0x40 | COARSE_TUNING_COMPLETED | FINE_TUNING_COMPLETED | 
                 TUNING_ENABLE);
    
    if ((dfe1_pass2 & 0xFD) != expStatus) {
       printf(" ERROR: port %d :: Fine tuning failure "
               " (Recd 0x%02x, Exp : 0x%02x)\n", 
                node, dfe1_pass2, expStatus);
    }
    return 0;
}

int asic_spico_eye_info(uint8_t inst, uint32_t node, uint16_t *vert_eye, 
                        uint16_t *hort_eye)
{
    uint32_t rv0, rv1;
    
    /* Write to channel of interest */
    asic_sbus_wr (inst, node, 0x17, 0x10);
    usleep(100);
    asic_sbus_wr(inst, node, 0x2a, 0x08);
    usleep(1000*1000);
    /* Read DAC values */
    asic_sbus_wr(inst, ASIC(SPICO_DEV_ID), 0x01, 0x00);
    asic_sbus_wr(inst, ASIC(SPICO_DEV_ID), 0x02, 0x20);
    asic_sbus_wr(inst, ASIC(SPICO_DEV_ID), 0x03, node);
    asic_sbus_wr(inst, ASIC(SPICO_DEV_ID), 0x0c, 0x1d);
    asic_sbus_wr(inst, ASIC(SPICO_DEV_ID), 0x0c, 0x0d);
    usleep(100);

    rv0 = asic_sbus_read(inst, ASIC(SPICO_DEV_ID), 0x01);
    if (rv0  != 0x1) {
        *vert_eye = 0;
        printf("Error reading horizontal eye\n");
        return 1;
    }

    rv0 = asic_sbus_read(inst, ASIC(SPICO_DEV_ID), 0x02);
    rv1 = asic_sbus_read(inst, ASIC(SPICO_DEV_ID), 0x00);
    *vert_eye = ((rv1 & 0x3) << 8 | (rv0 & 0xFF));

    asic_sbus_wr(inst, node, 0x2a, 0x10);
    usleep(1000*1000);
    /* Read DAC values */
    asic_sbus_wr(inst, ASIC(SPICO_DEV_ID), 0x01, 0x00);
    asic_sbus_wr(inst, ASIC(SPICO_DEV_ID), 0x02, 0x20);
    asic_sbus_wr(inst, ASIC(SPICO_DEV_ID), 0x03, node);
    asic_sbus_wr(inst, ASIC(SPICO_DEV_ID), 0x0c, 0x1d);
    asic_sbus_wr(inst, ASIC(SPICO_DEV_ID), 0x0c, 0x0d);
    usleep(100);

    rv0 = asic_sbus_read(inst, ASIC(SPICO_DEV_ID), 0x01);
    if (rv0  != 0x2) {
        printf("Error reading horizontal eye\n");
        *hort_eye = 0;
        return 1;
    }

    rv0 = asic_sbus_read(inst, ASIC(SPICO_DEV_ID), 0x02);
    rv1 = asic_sbus_read(inst, ASIC(SPICO_DEV_ID), 0x00);
    *hort_eye = ((rv1 & 0x3) << 8 | (rv0 & 0xFF));
    return 0;
}

int asic_spico_pre_dfe_tune (uint8_t inst, uint64_t himask, uint64_t nimask)
{
    int port = 0;
   // Config SBUS for DFE Tuning
    asic_sbus_wr(inst, ASIC(SPICO_DEV_ID), 0x01, 0x00);
    asic_sbus_wr(inst, ASIC(SPICO_DEV_ID), 0x02, 0x11);
    asic_sbus_wr(inst, ASIC(SPICO_DEV_ID), 0x03, 0x16);
    asic_sbus_wr(inst, ASIC(SPICO_DEV_ID), 0x0c, 0x1d);
    asic_sbus_wr(inst, ASIC(SPICO_DEV_ID), 0x0c, 0x0d);

    asic_sbus_wr(inst, ASIC(SPICO_DEV_ID), 0x01, 0x00);
    asic_sbus_wr(inst, ASIC(SPICO_DEV_ID), 0x02, 0x12);
    asic_sbus_wr(inst, ASIC(SPICO_DEV_ID), 0x03, 0x0);
    asic_sbus_wr(inst, ASIC(SPICO_DEV_ID), 0x0c, 0x1d);
    asic_sbus_wr(inst, ASIC(SPICO_DEV_ID), 0x0c, 0x0d);

    for (port=0; port < ASIC(HI_COUNT); port++, himask >>= 1) {
        if (!(himask & 0x1)) continue;
        asic_pre_dfe_tune_channel(inst, ASIC_HI_SERDES_SBUS_DEVID(port));
    }

    for (port=0; port < ASIC(NI_COUNT); port++, nimask >>= 1) {
        if (!(nimask & 0x1)) continue;
        asic_pre_dfe_tune_channel(inst, ASIC_NI_SERDES_SBUS_DEVID(port));
    }
    return (0);
}

static int hif_dfe_regs[32][8];
static int nif_dfe_regs[8][8];


int asic_tune_spico(uint8_t inst, uint64_t hifmask, uint64_t nifmask, int flag)
{
    int port = 0, rc = 0, j = 0;
    uint64_t himask, nimask;
    uint32_t rv0;

    himask = hifmask;
    nimask = nifmask;
    for (port=0; port < ASIC(HI_COUNT); port++, himask >>= 1) {
        if (!(himask & 0x1)) continue;
        asic_coarse_tune_spico_channel(inst, ASIC_HI_SERDES_SBUS_DEVID(port));
    }

    for (port=0; port < ASIC(NI_COUNT); port++, nimask >>= 1) {
        if (!(nimask & 0x1)) continue;
        asic_coarse_tune_spico_channel(inst, ASIC_NI_SERDES_SBUS_DEVID(port));
    }
    sleep(2);

    himask = hifmask;
    nimask = nifmask;
    for (port=0; port < ASIC(HI_COUNT); port++, himask >>= 1) {
        if (!(himask & 0x1)) continue;
        rc = asic_verify_spico_coarse_tuning(inst, 
                                             ASIC_HI_SERDES_SBUS_DEVID(port));
        if (rc) {
            printf("HIF Port %d Coarse Tuning failed\n", port);
        }
    }
    for (port=0; port < ASIC(NI_COUNT); port++, nimask >>= 1) {
        if (!(nimask & 0x1)) continue;
        rc = asic_verify_spico_coarse_tuning(inst, 
                                             ASIC_NI_SERDES_SBUS_DEVID(port));
        if (rc) {
            printf("NIF Port %d Coarse Tuning failed\n", port);
        }
    }

    himask = hifmask;
    nimask = nifmask;
    for (port=0; port < ASIC(HI_COUNT); port++, himask >>= 1) {
        if (!(himask & 0x1)) continue;
        asic_fine_tune_spico_channel(inst, ASIC_HI_SERDES_SBUS_DEVID(port));
    }

    for (port=0; port < ASIC(NI_COUNT); port++, nimask >>= 1) {
        if (!(nimask & 0x1)) continue;
        asic_fine_tune_spico_channel(inst, ASIC_NI_SERDES_SBUS_DEVID(port));
    }
    sleep(10);

    himask = hifmask;
    nimask = nifmask;
    for (port=0; port < ASIC(HI_COUNT); port++, himask >>= 1) {
        if (!(himask & 0x1)) continue;
        rc = asic_verify_spico_fine_tuning(inst, 
                                           ASIC_HI_SERDES_SBUS_DEVID(port));
        if (rc) {
            printf("HIF Port %d Fine Tuning failed\n", port);
        }
    }

    for (port=0; port < ASIC(NI_COUNT); port++, nimask >>= 1) {
        if (!(nimask & 0x1)) continue;
        rc = asic_verify_spico_fine_tuning(inst, 
                                           ASIC_NI_SERDES_SBUS_DEVID(port));
        if (rc) {
            printf("NIF Port %d Fine Tuning failed\n", port);
        }
    }

    if (flag) {
        himask = hifmask;
        nimask = nifmask;
        for (port=0; port < ASIC(HI_COUNT); port++, himask >>= 1) {
            if (!(himask & 0x1)) continue;
            rv0 = asic_sbus_read(inst, ASIC_HI_SERDES_SBUS_DEVID(port), 0x1f);
            //printf("R %x.0x1f = 0x%x\n", ASIC_HI_SERDES_SBUS_DEVID(port), rv0);
            for (j=0; j<8; j++) {
                hif_dfe_regs[port][j] = asic_sbus_read(inst, 
                                        ASIC_HI_SERDES_SBUS_DEVID(port), (0x20+j));
                hif_dfe_regs[port][j] &= 0xFF; /* store only first 8 bits */
            }
        /*
        printf("Store HIF-%d DFE vals: %02x/%02x/%02x/%02x/%02x/%02x/%02x/%02x\n",
               port,
               hif_dfe_regs[port][0],
               hif_dfe_regs[port][1],
               hif_dfe_regs[port][2],
               hif_dfe_regs[port][3],
               hif_dfe_regs[port][4],
               hif_dfe_regs[port][5],
               hif_dfe_regs[port][6],
               hif_dfe_regs[port][7]);
         */
        }
        
        for (port=0; port < ASIC(NI_COUNT); port++, nimask >>= 1) {
            if (!(nimask & 0x1)) continue;
            rv0 = asic_sbus_read(inst, ASIC_NI_SERDES_SBUS_DEVID(port), 0x1f);
            //printf("R %x.0x1f = 0x%x\n", ASIC_NI_SERDES_SBUS_DEVID(port), rv0);
            for (j=0; j<8; j++) {
                nif_dfe_regs[port][j] = asic_sbus_read(inst, 
                                        ASIC_NI_SERDES_SBUS_DEVID(port), (0x20+j));
                nif_dfe_regs[port][j] &= 0xFF; /* store only first 8 bits */
            }
        /*
        printf("Store NIF-%d DFE vals: %02x/%02x/%02x/%02x/%02x/%02x/%02x/%02x\n",
               port,
               nif_dfe_regs[port][0],
               nif_dfe_regs[port][1],
               nif_dfe_regs[port][2],
               nif_dfe_regs[port][3],
               nif_dfe_regs[port][4],
               nif_dfe_regs[port][5],
               nif_dfe_regs[port][6],
               nif_dfe_regs[port][7]);
         */
        }
    }

    return 0;
}

int asic_tune_spico_disable(uint8_t inst, uint64_t hifmask, uint64_t nifmask)
{
    int port = 0;
    uint64_t himask, nimask;

    himask = hifmask;
    nimask = nifmask;
    for (port=0; port < ASIC(HI_COUNT); port++, himask >>= 1) {
        if (!(himask & 0x1)) continue;
        asic_sbus_wr(inst, ASIC_HI_SERDES_SBUS_DEVID(port), 0x2b, 0);
        asic_sbus_wr(inst, ASIC_HI_SERDES_SBUS_DEVID(port), 0x3, 0);
    }

    for (port=0; port < ASIC(NI_COUNT); port++, nimask >>= 1) {
        if (!(himask & 0x1)) continue;
        asic_sbus_wr(inst, ASIC_NI_SERDES_SBUS_DEVID(port), 0x2b, 0);
        asic_sbus_wr(inst, ASIC_NI_SERDES_SBUS_DEVID(port), 0x3, 0);
    }
    return (0);
}

int asic_spico_dump_eye_info(uint8_t inst, uint64_t hifmask, 
                             uint64_t nifmask) 
{
    int port = 0;
    uint16_t v_eye=0, h_eye=0;
    for (port=0; port < ASIC(HI_COUNT); port++, hifmask >>= 1) {
        if (!(hifmask & 0x1)) continue;
        asic_spico_eye_info(inst, ASIC_HI_SERDES_SBUS_DEVID(port), &v_eye,
                            &h_eye);
        if ((v_eye != 0) && (h_eye != 0)) {
            printf("HIF Port %d V_EYE %5.2f mv H_EYE %3d mUI wide\n", port,
               (float)(v_eye*6.25), (int)((h_eye*1000)/64)); 
        }
    }
    for (port=0; port < ASIC(NI_COUNT); port++, nifmask >>= 1) {
        if (!(nifmask & 0x1)) continue;
        asic_spico_eye_info(inst, ASIC_NI_SERDES_SBUS_DEVID(port), &v_eye,
                            &h_eye);
        if ((v_eye != 0) && (h_eye != 0)) {
            printf("NIF Port %d V_EYE %5.2f mv H_EYE %3d mUI wide\n", port,
               (float)(v_eye*6.25), (int)((h_eye*1000)/64)); 
        }
    }

    return 0;
}

int asic_serdes_sbus_test (uint8_t inst)
{
    int port = 0, index = 1000;
    uint32_t rv0 = 0;

    while (index-- > 1) {
        for (port=0; port < ASIC(HI_COUNT); port++) {
            asic_sbus_wr(inst, ASIC_HI_SERDES_SBUS_DEVID(port), 0x1a, 0xaa);
            asic_sbus_wr(inst, ASIC_HI_SERDES_SBUS_DEVID(port), 0x1a, 0x55);
        }
        for (port=0; port < ASIC(NI_COUNT); port++) {
            asic_sbus_wr(inst, ASIC_NI_SERDES_SBUS_DEVID(port), 0x1a, 0xaa);
            asic_sbus_wr(inst, ASIC_NI_SERDES_SBUS_DEVID(port), 0x1a, 0x55);
        }
        for (port=0; port < ASIC(HI_COUNT); port++) {
            rv0 = asic_sbus_read(inst, ASIC_HI_SERDES_SBUS_DEVID(port), 0x16);
            if (rv0 != 0x2b) {
                printf("Error HIF port %d Register 0x16 Value %x (expected 0x2b) "
                       "Iteration %d\n", 
                       port, rv0, index);
                return 0;
            }
        }
        for (port=0; port < ASIC(NI_COUNT); port++) {
            rv0 = asic_sbus_read(inst, ASIC_NI_SERDES_SBUS_DEVID(port), 0x16);
            if (rv0 != 0x2b) {
                printf("Error NIF port %d Register 0x16 Value %x (expected 0x2b) "
                       "Iteration %d\n", 
                       port, rv0, index);
                return 0;
            }
        }
        for (port=0; port < ASIC(HI_COUNT); port++) {
            asic_sbus_wr(inst, ASIC_HI_SERDES_SBUS_DEVID(port), 0x1a, 0x55);
            asic_sbus_wr(inst, ASIC_HI_SERDES_SBUS_DEVID(port), 0x1a, 0xaa);
        }
        for (port=0; port < ASIC(NI_COUNT); port++) {
            asic_sbus_wr(inst, ASIC_NI_SERDES_SBUS_DEVID(port), 0x1a, 0x55);
            asic_sbus_wr(inst, ASIC_NI_SERDES_SBUS_DEVID(port), 0x1a, 0xaa);
        }
        for (port=0; port < ASIC(HI_COUNT); port++) {
            rv0 = asic_sbus_read(inst, ASIC_HI_SERDES_SBUS_DEVID(port), 0x16);
            if (rv0 != 0x24) {
                printf("Error HIF port %d Register 0x16 Value %x (expected 0x24) "
                       "Iteration %d\n", 
                       port, rv0, index);
                return 0;
            }
        }
        for (port=0; port < ASIC(NI_COUNT); port++) {
            rv0 = asic_sbus_read(inst, ASIC_NI_SERDES_SBUS_DEVID(port), 0x16);
            if (rv0 != 0x24) {
                printf("Error NIF port %d Register 0x16 Value %x (expected 0x24) "
                       "Iteration %d\n", 
                       port, rv0, index);
                return 0;
            }
        }
    } /* End of while */
    return 0;
}

int asic_spico_load_static_value (uint8_t inst, uint64_t hifmask, uint64_t nifmask)
{ 
    uint64_t himask = hifmask;
    uint64_t nimask = nifmask;
    unsigned int dfe_reg_map[8] = {0x1f, 0x20, 0x21, 0x2e, 0x23, 0x24, 0x25, 0x2f};
    int port = 0, j = 0;
    for (port=0; port < ASIC(HI_COUNT); port++, himask >>= 1) {
        if (!(himask & 0x1)) continue;
        /* Enable DFE and put it in manual mode */
        asic_sbus_wr(inst, ASIC_HI_SERDES_SBUS_DEVID(port), 0x26,
                     0x07);
        for (j =0; j < 8; j++) {
             asic_sbus_wr(inst, ASIC_HI_SERDES_SBUS_DEVID(port), dfe_reg_map[j],
                          hif_dfe_regs[port][j]);
        }
        /*
        printf("Restore HIF-%d DFE vals: %02x/%02x/%02x/%02x/%02x/%02x/%02x/%02x\n",
               port,
               hif_dfe_regs[port][0],
               hif_dfe_regs[port][1],
               hif_dfe_regs[port][2],
               hif_dfe_regs[port][3],
               hif_dfe_regs[port][4],
               hif_dfe_regs[port][5],
               hif_dfe_regs[port][6],
               hif_dfe_regs[port][7]);
         */
    }
        
    for (port=0; port < ASIC(NI_COUNT); port++, nimask >>= 1) {
        if (!(nimask & 0x1)) continue;
        /* Enable DFE and put it in manual mode */
        asic_sbus_wr(inst, ASIC_NI_SERDES_SBUS_DEVID(port), 0x26,
                     0x07);
        for (j =0; j < 8; j++) {
             asic_sbus_wr(inst, ASIC_NI_SERDES_SBUS_DEVID(port), dfe_reg_map[j],
                          nif_dfe_regs[port][j]);
        }
        /*
        printf("Restore NIF-%d DFE vals: %02x/%02x/%02x/%02x/%02x/%02x/%02x/%02x\n",
               port,
               nif_dfe_regs[port][0],
               nif_dfe_regs[port][1],
               nif_dfe_regs[port][2],
               nif_dfe_regs[port][3],
               nif_dfe_regs[port][4],
               nif_dfe_regs[port][5],
               nif_dfe_regs[port][6],
               nif_dfe_regs[port][7]);
         */
    }
    return 0;
}
