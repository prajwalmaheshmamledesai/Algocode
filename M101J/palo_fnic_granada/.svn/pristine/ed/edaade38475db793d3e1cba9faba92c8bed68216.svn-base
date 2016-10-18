/********************************************************************
 *
 * File:  bcm84823_download.c
 * Name:  Sanjay Gupta
 * Original File:  gatos_phy84823.c
 * Original Author:  Raghu Boreda
 *
 * Copyright (c) 2010 by Cisco Systems, Inc.
 * All rights reserved.
 *
 **********************************************************************/
/********************************************************************//**
@file    bcm84823_download.c 
@author  Raghu Boreda-- boreda@cisco.com
@brief   Phy 84823 Related functions
************************************************************************/
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>
#include <sys/time.h>
#include "diag_main.h"

#include "bcm84823.h"
#include "bcm84823_lib.h"

#define USE_BROADCAST 1
#define NUM_COL_PHY_WR 4
static FILE *logfile = NULL;

static uint16_t bcm84823_setup_broadcast_seq[][NUM_COL_PHY_WR]=
{
 {0, 0x1e, 0x411e, 0xF003},
 {0, 0x1e, 0x410e, 0x0401}
};

static uint16_t bcm84823_disable_broadcast_seq[][NUM_COL_PHY_WR]=
{
 {0,0x1e, 0x411e, 0x0000},
 {0,0x1e, 0x410e, 0x0000}
};

#if defined(BCM84823_SPIROM)
static uint16_t bcm84823_Read_Firmware_Vers_seq[][NUM_COL_PHY_WR]=
{
 {0, 1, 0xA819, 0x14},
 {0, 1, 0xA81A, 0xc200},
 {0, 1, 0xA81B, 0x0000},
 {0, 1, 0xA81c, 0x0300},
 {0, 1, 0xA817, 0x0009}
};

static uint16_t bcm84823_Read_Firmware_Vers_seq1[][NUM_COL_PHY_WR]=
{
 {0, 1, 0xA819, 0x0000},
 {0, 1, 0xA81A, 0xc200},
 {0, 1, 0xA817, 0x000A},
};
#endif

static uint16_t bcm84823_halt_system_start_download[][NUM_COL_PHY_WR]=
{
  {1, 0x1e, 0x4186, 0x8000},
  {1, 0x1e, 0x4181, 0x017c},
  {1, 0x1e, 0x4181, 0x0040},
  {0, 0x1, 0xA819, 0x0000},
  {0, 0x1, 0xA81A, 0xc300},
  {0, 0x1, 0xA81B, 0x001e},
  {0, 0x1, 0xA81C, 0x0000},
  {0, 0x1, 0xA817, 0x0009},
  {0, 0x1, 0xA819, 0x0000},
  {0, 0x1, 0xA81A, 0xffff},
  {0, 0x1, 0xA81B, 0x1004},
  {0, 0x1, 0xA81C, 0xE59F},
  {0, 0x1, 0xA817, 0x0009},
  {0, 0x1, 0xA819, 0x0004},
  {0, 0x1, 0xA81A, 0xffff},
  {0, 0x1, 0xA81B, 0x1F11},
  {0, 0x1, 0xA81C, 0xEE09},
  {0, 0x1, 0xA817, 0x0009},
  {0, 0x1, 0xA819, 0x0008},
  {0, 0x1, 0xA81A, 0xffff},
  {0, 0x1, 0xA81B, 0xFFFE},
  {0, 0x1, 0xA81C, 0xEAFF},
  {0, 0x1, 0xA817, 0x0009},
  {0, 0x1, 0xA819, 0x000c},
  {0, 0x1, 0xA81A, 0xffff},
  {0, 0x1, 0xA81B, 0x0021},
  {0, 0x1, 0xA81C, 0x0002},
  {0, 0x1, 0xA817, 0x0009},
  {1, 0x1e, 0x4181, 0x0000},
  {0, 0x1, 0xA81A, 0x0000},
  {0, 0x1, 0xA819, 0x0000}
  /*{0, 0x1, 0xA81C, 0xEBFF},
  {0, 0x1, 0xA81B, 0xFFFF} */
};

static uint16_t bcm84823_finish_download_into_memory[][NUM_COL_PHY_WR]=
{
  // FIXTHIS - must be part of .bin file
/*  { 0, 0x1, 0xA81C, 0x513A},
  { 0, 0x1, 0xA819, 0x2880},
  { 0, 0x1, 0xA81A, 0x0029}, */
  { 0, 0x1, 0xA81C, 0xE594},
  { 0, 0x1, 0xA81B, 0xF000},
  { 0, 0x1, 0xA817, 0x0029}, 
  { 0, 0x1, 0xA81C, 0x159D},
  { 0, 0x1, 0xA81B, 0x4DBF},
  { 0, 0x1, 0xA817, 0x0029}, 
  { 0, 0x1, 0xA817, 0x0000},
  { 0, 0x1, 0xA819, 0x0000},
  { 0, 0x1, 0xA81A, 0xc300},
  { 0, 0x1, 0xA81B, 0x000c},
  { 0, 0x1, 0xA81C, 0x0000},
  { 0, 0x1, 0xA817, 0x0009},
};

static uint16_t bcm84823_restart_seq[][NUM_COL_PHY_WR]=
{
  {1, 0x1e, 0x4181, 0x0040},
  {1, 0x1e, 0x4181, 0x0000},
};

/////////////////////////////////////////////////////////////////////
//Function:     
//Description:  
//Parameters:   inst - instance
//              disable_mem_init - Bool for disabling Mem init
//              bypass_ecc - Bool for Bypassing ECC
//Return Value: 
//////////////////////////////////////////////////////////////////////
static int bcm84823_strobe_write_seq (int fd, uint16_t *data_ptr, 
           uint32_t data_size, uint8_t max_mdio, uint8_t *ports_mdio)
{
    uint32_t row_idx;
    uint8_t mdio_addr, port_addr;
    uint32_t rc = 0;
    /* Strobe write sequece across all MDIO busses by writing to PHY MDIO address
       0 while PHYs in broadcast mode */
    for (row_idx = 0; 
         row_idx < data_size/ (NUM_COL_PHY_WR * sizeof(uint16_t)); row_idx++) {
        if ((data_ptr[0] == 0) && (data_ptr[1] == 0x1)) {
            // Write to broadcast address
            port_addr = 0x0;
            for (mdio_addr = 0; mdio_addr < max_mdio; mdio_addr++) {
                BCM84823_WR(fd, mdio_addr, port_addr, data_ptr[1],
                            data_ptr[2], data_ptr[3]);
            }
	} else { /* Non broadcast addresses */
            for (mdio_addr = 0; mdio_addr < max_mdio; mdio_addr++) {
                for (port_addr = 0; port_addr < ports_mdio[mdio_addr]; 
                     port_addr++) {
                    BCM84823_WR(fd, mdio_addr, port_addr, data_ptr[1],
                                data_ptr[2], data_ptr[3]);
                }
            }
        } 
      data_ptr += NUM_COL_PHY_WR;
    }
    return rc;
}

static int bcm84823_check_download_needed (uint8_t max_mdio, uint8_t *ports_mdio)
{
    uint16_t hiVal;
    uint8_t mdio_addr, port_addr;
    int fd = 0;
    /* force to download as there seems to be with broadcom chip with respect to
     * revision register */
    return 1;
 
    for (mdio_addr = 0; mdio_addr < max_mdio; mdio_addr++) {
        for (port_addr = 0; port_addr < ports_mdio[mdio_addr]; port_addr++) {
            BCM84823_RD(fd, mdio_addr, port_addr, BCM84823_UD_30_ADDR, 
                        0x400F, &hiVal);
    fprintf (logfile,"mdio %d port %d Device 0x1E Register 0x400F == %x\n",
                      mdio_addr, port_addr, hiVal);
            
            if ((hiVal != BCM84823_UCODE_LATEST_VER)) {
               printf (" Version Upgrade Needed, Current 0x%04X\n", 
                       hiVal);
               printf (" Version Needed                  0x%x\n",
                       BCM84823_UCODE_LATEST_VER);
               return 1;
            } else {
               printf ("CUPHY %d.%d Version Up to Date, Current 0x%04X\n", 
                       mdio_addr, port_addr, hiVal);
            }
        }
    }
    return 0;
}

static int bcm84823_download_microcode_data (FILE *fp, uint32_t filesize,
                                             uint8_t max_mdio)
{
    uint8_t *uc_data_ptr, *bptr, mdio_addr;
    uint16_t hiVal, loVal, wrHiVal=0, wrLoVal=0;
    uint32_t idx, word_cnt, read_count;
    int num_bytes_read = 0;
    uint32_t *data_ptr = NULL;
    int fd = 0;

    uc_data_ptr = malloc(filesize);
    bptr = uc_data_ptr;
    read_count = 0;
    while (!feof(fp)) {
      num_bytes_read = fread(bptr, 1, 16384, fp);
      bptr += num_bytes_read;
      read_count += num_bytes_read;
    }
    fclose(fp);
    //printf("Total bytes read:%d\n", read_count);
    data_ptr = (uint32_t *)uc_data_ptr;

    word_cnt = filesize/sizeof(uint32_t);
    //printf ("      : Download Data Size %d\n", word_cnt);
    for (idx = 0; idx < word_cnt; idx++) {
      /* Shift Little Endian Binary data to Big Endian format */ 
      hiVal = (uint16_t)(((*data_ptr & 0x000000FF) << 8) | 
                         ((*data_ptr & 0x0000FF00) >> 8));
      loVal = (uint16_t)(((*data_ptr & 0x00FF0000) >> 8) | 
                         ((*data_ptr & 0xFF000000) >> 24)); 

      if ((hiVal != wrHiVal) || (loVal != wrLoVal)) { // SKG
          for (mdio_addr = 0; mdio_addr < max_mdio; mdio_addr++) {
              BCM84823_WR(fd, mdio_addr, 0x0, 1, 0xA81C, hiVal);
          }
          for (mdio_addr = 0; mdio_addr < max_mdio; mdio_addr++) {
              BCM84823_WR(fd, mdio_addr, 0x0, 1, 0xA81B, loVal);
          }
      }
      for (mdio_addr = 0; mdio_addr < max_mdio; mdio_addr++) {
          BCM84823_WR(fd, mdio_addr, 0x0, 1, 0xA817, 0x0029);
      }
      
      data_ptr++;
      wrHiVal = hiVal;
      wrLoVal = loVal;
    }
    free(uc_data_ptr);
    return (0);
}

static int bcm84823_download_microcode_data_single_phy (FILE *fp, 
           uint32_t filesize, uint8_t mdio, uint8_t port)
{
    uint8_t *uc_data_ptr, *bptr;
    uint16_t hiVal, loVal, wrHiVal=0, wrLoVal=0;
    uint32_t idx, word_cnt, read_count;
    int num_bytes_read = 0;
    uint32_t *data_ptr = NULL;
    int fd = 0;

    uc_data_ptr = malloc(filesize);
    bptr = uc_data_ptr;
    read_count = 0;
    while (!feof(fp)) {
      num_bytes_read = fread(bptr, 1, 16384, fp);
      bptr += num_bytes_read;
      read_count += num_bytes_read;
    }
    fclose(fp);
    //printf("Total bytes read:%d\n", read_count);
    data_ptr = (uint32_t *)uc_data_ptr;

    word_cnt = filesize/sizeof(uint32_t);
    //printf ("      : Download Data Size %d\n", word_cnt);
    for (idx = 0; idx < word_cnt; idx++) {
      /*hiVal = *data_ptr >> 16;
      loVal = *data_ptr & 0xFFFF; */
      /* Shift Little Endian Binary data to Big Endian format */
      hiVal = (uint16_t)(((*data_ptr & 0x000000FF) << 8) | 
                         ((*data_ptr & 0x0000FF00) >> 8));
      loVal = (uint16_t)(((*data_ptr & 0x00FF0000) >> 8) | 
                         ((*data_ptr & 0xFF000000) >> 24));
      if ((hiVal != wrHiVal) || (loVal != wrLoVal)) { // SKG
          BCM84823_WR(fd, mdio, port, 1, 0xA81C, hiVal);
          BCM84823_WR(fd, mdio, port, 1, 0xA81B, loVal);
        /*  fprintf(logfile, " mdio %x port %x device 1 addr 0xa81c data %x\n",
                    mdio, port, hiVal); 
          fprintf(logfile, " mdio %x port %x device 1 addr 0xa81b data %x\n",
                    mdio, port, loVal); */ 
      }
      BCM84823_WR(fd, mdio, port, 1, 0xA817, 0x0029);
      /* fprintf(logfile, " mdio %x port %x device 1 addr 0xa817 data 0x0029\n",
                    mdio, port); */ 
      data_ptr++;
      wrHiVal = hiVal;
      wrLoVal = loVal;
    }
    free(uc_data_ptr);
    return (0);
}

int bcm84823_gen_download_microcode (int fd, char *filename, uint8_t max_mdio, 
                                 uint8_t *ports_mdio)
{
    uint8_t mdio_addr, port_addr;
    uint16_t loadVal;
    FILE *datafile;
    struct stat st;
    uint32_t uc_filesize;
    /*time_t timer1, timer2;*/
    struct timeval stime;
    double t1, t2;

    if ((datafile = fopen(filename, "r+b")) == NULL) {
         printf("Failure opening microcode file %s\n", filename);
         return 1;
    }
    if ((logfile = fopen("/diag/phydownload.log", "w")) == NULL) {
         printf("Failure opening microcode file %s\n", filename);
         return 1;
    }
    if (stat(filename, &st) == -1) {
         printf("Failure reading microcode filesize %s\n", filename);
    } 
    uc_filesize = st.st_size;

    if (bcm84823_check_download_needed(max_mdio, ports_mdio) == 0) {
        fclose (logfile);
        fclose (datafile);
        return 0;
    }
    /*time(&timer1);*/
    gettimeofday(&stime, (struct timezone *)0);
    t1 = stime.tv_sec + (((double)stime.tv_usec)/1000000);

    // printf ("Step 1: Setup Broadcast mode\n");
    for (mdio_addr = 0; mdio_addr < max_mdio; mdio_addr++) {
        for (port_addr = 1; port_addr < ports_mdio[mdio_addr]; port_addr++) {
            bcm84823_write_seq(fd, mdio_addr, port_addr, 
                               bcm84823_setup_broadcast_seq[0],
                               sizeof(bcm84823_setup_broadcast_seq));
        }
    }

    // printf ("Step 2: Halt System setup for download\n");
    bcm84823_strobe_write_seq(fd, bcm84823_halt_system_start_download[0],
                              sizeof (bcm84823_halt_system_start_download),
                              max_mdio, ports_mdio);

    // printf ("Step 3: Download Data \n");
    bcm84823_download_microcode_data(datafile, uc_filesize, max_mdio);
   
    // printf ("Step 3: Finish Download\n");
    bcm84823_strobe_write_seq(fd, bcm84823_finish_download_into_memory[0],
                              sizeof (bcm84823_finish_download_into_memory),
                              max_mdio, ports_mdio);
   
    // printf ("Step 4: Turn off broadcast mode\n");
    for (mdio_addr = 0; mdio_addr < max_mdio; mdio_addr++) {
        for (port_addr = 1; port_addr < ports_mdio[mdio_addr]; port_addr++) {
            bcm84823_write_seq(fd, mdio_addr, port_addr, 
                               bcm84823_disable_broadcast_seq[0],
                               sizeof(bcm84823_disable_broadcast_seq));
        }
    }
    // printf ("Step 5: Restart Process on each phy\n");
    for (mdio_addr = 0; mdio_addr < max_mdio; mdio_addr++) {
        for (port_addr = 0; port_addr < ports_mdio[mdio_addr]; port_addr++) {
            bcm84823_write_seq(fd, mdio_addr, port_addr, 
                               bcm84823_restart_seq[0],
                               sizeof(bcm84823_restart_seq));
        }
    }
    /* TODO: Revisit Error Processsing in below two steps */
    usleep(500*1000);
    // printf ("Step 6: Check that Processor is running on each phy\n");
    for (mdio_addr = 0; mdio_addr < max_mdio; mdio_addr++) {
        for (port_addr = 0; port_addr < ports_mdio[mdio_addr]; port_addr++) {
            BCM84823_RD(fd, mdio_addr, port_addr, 1, 0x0, &loadVal);
            if (loadVal & 0x8000) {
                printf("CUPHY %d. %d failed to execute the firmware\n",
                       mdio_addr, port_addr);
                break;
            }
        }
    }
    // printf ("Step 7: Verify that firmware has loaded correctly\n");
    for (mdio_addr = 0; mdio_addr < max_mdio; mdio_addr++) {
        for (port_addr = 0; port_addr < ports_mdio[mdio_addr]; port_addr++) {
            BCM84823_RD(fd, mdio_addr, port_addr, 0x1e, 0x400D, &loadVal);
            loadVal = (loadVal & 0xC000) >> 14;
            if (loadVal == 0x01) {
                /* printf("PHY %d. %d loaded with good CRC\n",
                       mdio_addr, port_addr); */
                continue;
            } else {
                printf("CUPHY %d. %d loaded with bad CRC\n",
                       mdio_addr, port_addr);
            }
        }
    }
    /*time(&timer2);*/
    gettimeofday(&stime, (struct timezone *)0);
    t2 = stime.tv_sec + (((double)stime.tv_usec)/1000000);
    fprintf(logfile, "Cuphy Download time in %f milliseconds\n", ((t2-t1)*1000));
    // printf ("Step 7: Verify that firmware has loaded correctly\n");
    sleep(1);
    for (mdio_addr = 0; mdio_addr < max_mdio; mdio_addr++) {
        for (port_addr = 0; port_addr < ports_mdio[mdio_addr]; port_addr++) {
            BCM84823_RD(fd, mdio_addr, port_addr, 0x1e, 0x400F, &loadVal);
            if (loadVal == BCM84823_UCODE_LATEST_VER) {
                continue;
            } else {
                printf("PHY %d. %d version 0x%x is incorrect\n",
                       mdio_addr, port_addr, loadVal);
                fclose(logfile);
                return 1;
            }
        }
    }
    fclose(logfile);
    return 0;
}

int bcm84823_download_microcode_single_phy (int fd, char *filename, 
                                            uint8_t mdio, uint8_t port)
{
    uint16_t loadVal;
    FILE *datafile;
    struct stat st;
    uint32_t uc_filesize;
    time_t timer1, timer2;

    if ((datafile = fopen(filename, "r+b")) == NULL) {
         printf("Failure opening microcode file %s\n", filename);
         return 1;
    }
    if ((logfile = fopen("/diag/phydownload.log", "w")) == NULL) {
         printf("Failure opening microcode file %s\n", filename);
         return 1;
    }
    if (stat(filename, &st) == -1) {
         printf("Failure reading microcode filesize %s\n", filename);
    } 
    uc_filesize = st.st_size;
    time(&timer1);

    //printf ("Step 1: Halt System setup for download\n");
    bcm84823_write_seq(fd, mdio, port, bcm84823_halt_system_start_download[0],
                       sizeof (bcm84823_halt_system_start_download));

    //printf ("Step 2: Download Data \n");
    bcm84823_download_microcode_data_single_phy(datafile, uc_filesize, 
                                                mdio, port); 
   
    //printf ("Step 3: Finish Download\n");
    bcm84823_write_seq(fd, mdio, port,
                       bcm84823_finish_download_into_memory[0],
                       sizeof(bcm84823_finish_download_into_memory));
   
    //printf ("Step 4: Restart Process on each phy\n");
    bcm84823_write_seq(fd, mdio, port, bcm84823_restart_seq[0],
                               sizeof(bcm84823_restart_seq));
    usleep(500*1000);
    /* TODO: Revisit Error Processsing in below two steps */
    //printf ("Step 5: Check that Processor is running on each phy\n");
    BCM84823_RD(fd, mdio, port, 1, 0x0, &loadVal);
    if (loadVal & 0x8000) {
        printf("PHY %d. %d failed to execute the firmware\n",
               mdio, port);
    }
    //printf ("Step 6: Verify that firmware has loaded correctly\n");
    BCM84823_RD(fd, mdio, port, 0x1e, 0x400D, &loadVal);
    loadVal = (loadVal & 0xC000) >> 14;
    if (loadVal == 0x01) {
     /*   printf("PHY %d. %d loaded with good CRC\n",
               mdio, port); */
    } else {
        printf("PHY %d. %d loaded with bad CRC LoadVal %x\n",
               mdio, port, loadVal);
    }
    time(&timer2);
    printf("Download time in seconds %f\n", difftime(timer2, timer1));
    fclose(logfile);

    return 0;
}
