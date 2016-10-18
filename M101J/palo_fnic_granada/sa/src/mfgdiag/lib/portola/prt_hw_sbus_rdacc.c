/*****************************************************************************
 *      File:  prt_sbus_rdacc.c
 *      Name:   Raghu Boreda
 *
 *      Based on file : woo_sbus_rdacc.c
 *      Description: SBUS Device read test functions for Woodside
 *      Copyright (c) 2009 by Cisco Systems, Inc.
 *****************************************************************************/
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include "woo_ops.h"
#include "woo_hw_error.h"

#define PRT_MAX_SBUS_DEVICES 64 
#define PRT_MAX_HI_SBUS_DEVICES 48 

int prt_sbus_rdacc_test(int inst, unsigned int* addr_error, 
                        unsigned int *exp_data, unsigned int *got_data) 
{
    int deviceId, regId, rc; 
    uint32_t data, expData;
    
    deviceId = 0x7f; /* SBUS Controller */
    regId = 0xa;

    portola_speedup_sbus(inst);
    //Read & check
    rc = asic_rd(inst, ASIC_SBUS_ADDR(deviceId, regId), &data);
    //Write
    if ((data&0xFF) != 0x4) {
        data = 0x4;
        rc = asic_wr(inst, ASIC_SBUS_ADDR(deviceId, regId), data);
        //Read & check
        rc = asic_rd(inst, ASIC_SBUS_ADDR(deviceId, regId), &data);
        if (data != 0x404) { //reg value updated to 0x3 earlier
            *addr_error = ASIC_SBUS_ADDR(deviceId, regId);
            *exp_data = 0x404;
            *got_data = data;
            printf("sbus controller clockdivider reg addr 0x%x "  
                   "read value 0x%x\n", ASIC_SBUS_ADDR(deviceId, regId), data); 
            return (WOODSIDE_ERROR_SBUS_WRITE);
        }
    }
    //Reading the device id registers
    //for the kr serdes, pcs & an 
    //reg 0xFE -> SBUS ID; reg 0xFF->IP IDCODE
    for (regId=0xfe; regId<0x100; regId=regId+1) {
        for (deviceId=1; deviceId<PRT_MAX_HI_SBUS_DEVICES; deviceId=deviceId+1) {
            if (regId == 0xfe) {
                expData = 0x400;          
            } else {
                expData = 0x401;
            }
            rc = asic_rd(inst, ASIC_SBUS_ADDR(deviceId, regId), &data);
            if (data != expData) { 
                *addr_error = ASIC_SBUS_ADDR(deviceId, regId);
                *exp_data = expData;
                *got_data = data;
                printf("Sbus id read value 0x%x expected 0x%x"  
                       "for device %x\n", data, expData, deviceId);     
                return (WOODSIDE_ERROR_SBUS_READ);
            }
        } //deviceId

        //for the pcie serdes
        deviceId = 64;
        rc = asic_rd(inst, ASIC_SBUS_ADDR(deviceId, regId), &data);
        if (regId == 0xfe) {
            expData = 0x400;          
        } else {
            expData = 0x401;
        }

        if (data != expData) { 
            *addr_error = ASIC_SBUS_ADDR(deviceId, regId);
            *exp_data = expData;
            *got_data = data;
            printf("Sbus id read value 0x%x expected 0x%x"  
                   "for device %x\n", data, expData, deviceId);     
            return (WOODSIDE_ERROR_SBUS_READ);
        }
        //for the spico
        deviceId = 123;
        rc = asic_rd(inst, ASIC_SBUS_ADDR(deviceId, regId), &data);
        if (regId == 0xfe) {
            expData = 0x400;          
        } else {
            expData = 0x403;
        }

        if (data != expData) { 
            *addr_error = ASIC_SBUS_ADDR(deviceId, regId);
            *exp_data = expData;
            *got_data = data;
            printf("Sbus id read value 0x%x expected 0x%x"  
                   "for device %x\n", data, expData, deviceId);     
            return (WOODSIDE_ERROR_SBUS_READ);
        }

          //for the sbus controller
        deviceId = 0x7f;
        rc = asic_rd(inst, ASIC_SBUS_ADDR(deviceId, regId), &data);
        if (regId == 0xfe) {
            expData = 0x4bc;          
        } else {
          expData = 0x402;
        }
        if (data != expData) { 
            *addr_error = ASIC_SBUS_ADDR(deviceId, regId);
            *exp_data = expData;
            *got_data = data;
            printf("Sbus id read value 0x%x expected 0x%x"  
                   "for device %x\n", data, expData, deviceId);     
            return (WOODSIDE_ERROR_SBUS_READ);
        }
    } //regId
    return (rc);
}
