/********************************************************************
 *
 *      File:   pmbus_lib.c
 *      Name:   Sudharshan Kadari
 *
 *      Description:
 *       Digital PWM System Controller library 
 *
 *
 * Copyright (c) 1985-2009 by Cisco Systems, Inc.
 * All rights reserved.
 *
 *
 *********************************************************************/

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "ppc_lib.h"
#include "fext.h"

static int psu0_acc = 0, psu1_acc = 0;

int pmbus_rd(int fd, uint8_t bus, uint8_t i2c_addr, uint8_t addr, uint8_t *data)
{
	usleep(10000);
        int rc = 0;
        
        rc = diag_i2c_rd(fd, i2c_addr, addr, sizeof(addr),
                       (uint8_t*)data, sizeof(uint8_t));
        /* Retry added for manufactuing issue where multiple astoria
         * and tahoe systems are failing in I2C reads
         * HW requested that only first access after Power UP should have a retry
         */
        if (rc != 0) {
            if (((bus == FEXT_I2C_BUS0_MUX0_CHAN1) && (psu0_acc == 0)) ||
                ((bus == FEXT_I2C_BUS0_MUX0_CHAN2) && (psu1_acc == 0))) {
	       usleep(10000);
               printf("Power Supply I2C Retry in pmbus_rd()\n");
               printf("Bus %x I2C address %x Reg addr %x\n",bus, i2c_addr, addr);
               rc = diag_i2c_rd(fd, i2c_addr, addr, sizeof(addr),
                                (uint8_t*)data, sizeof(uint8_t));
               if (bus == FEXT_I2C_BUS0_MUX0_CHAN1) {
                   psu0_acc = 1;
               } else if (bus == FEXT_I2C_BUS0_MUX0_CHAN2) {
                   psu1_acc = 1;
               }
            }
        }
        return (rc);
}

int pmbus_wr(int fd, uint8_t bus, uint8_t i2c_addr, uint8_t addr, uint8_t data)
{
	usleep(10000);
        return (diag_i2c_wr(fd, i2c_addr, addr, sizeof(addr),
                (uint8_t*)&data, sizeof(uint8_t)));
}

int pmbus_broadcast_wr(int fd, uint8_t bus, uint8_t i2c_addr, uint8_t addr, uint8_t data)
{
        uint16_t hw_rev_maj = 0, hw_rev_min = 0;
	uint32_t g_i2c_addr[4] = {0x5F, 0x59, 0x54, 0x50};
        int num_devices = 4;
        fext_get_hw_revision(&hw_rev_maj, &hw_rev_min);
        if (hw_rev_maj > 1) {
            num_devices = 3;
        }
	printf("Broadcast Write (i2c=0x%X) Num devices %d\n", 
               g_i2c_addr[0], num_devices);
	usleep(10000);
        return (diag_group_i2c_wr(fd, num_devices, g_i2c_addr, addr, 
                     sizeof(addr), (uint8_t*)&data, sizeof(uint8_t)));
}

int pmbus_rd16(int fd, uint8_t bus, uint8_t i2c_addr, uint8_t addr, uint16_t *data)
{
	uint16_t data16;
        int rc;
	usleep(10000);
        rc = (diag_i2c_rd(fd, i2c_addr, addr, sizeof(addr),
                       (uint8_t*)&data16, sizeof(data16)));
        /* Retry added for manufactuing issue where multiple astoria
         * and tahoe systems are failing in I2C reads
         */
        if (rc != 0) {
	    usleep(10000);
            printf("Power Supply I2C Retry in pmbus_rd16()\n");
            printf("Bus %x I2C address %x Reg addr %x\n",bus, i2c_addr, addr);
            rc = (diag_i2c_rd(fd, i2c_addr, addr, sizeof(addr),
                       (uint8_t*)&data16, sizeof(data16)));
        }
	*data = ((data16 & 0xFF)<<8)| ((data16>>8)&0xFF);	
	return (rc);
}

int pmbus_wr16(int fd, uint8_t bus, uint8_t i2c_addr, uint8_t addr, uint16_t *data)
{
	uint16_t data16 = ((*data & 0xFF)<<8)| ((*data>>8)&0xFF);	
	usleep(10000);
        return (diag_i2c_wr(fd, i2c_addr, addr, sizeof(addr),
                (uint8_t*)&data16, sizeof(uint16_t)));
}

int pmbus_broadcast_wr16(int fd, uint8_t bus, uint8_t i2c_addr, uint8_t addr, uint16_t *data)
{
        uint16_t hw_rev_maj = 0, hw_rev_min = 0;
	uint32_t g_i2c_addr[4] = {0x5F, 0x59, 0x54, 0x50};
	uint16_t data16 = ((*data & 0xFF)<<8)| ((*data>>8)&0xFF);	
        int num_devices = 4;
        fext_get_hw_revision(&hw_rev_maj, &hw_rev_min);
        if (hw_rev_maj > 1) {
            num_devices = 3;
        }
	usleep(10000);
        return (diag_group_i2c_wr(fd, num_devices, g_i2c_addr, addr, 
                sizeof(addr), (uint8_t*)&data16, sizeof(uint16_t)));
}

int pmbus_rd_blk( int fd, uint8_t bus, uint8_t i2c_addr,
                        uint8_t addr, uint8_t *rdBuf, int rdLen)
{
	int rc;
	usleep(10000);
        rc= (diag_i2c_rd(fd, i2c_addr, addr, sizeof(addr),
                       (uint8_t*)rdBuf, rdLen));
	return(rc);
}

int pmbus_wr_blk( int fd, uint8_t bus, uint8_t i2c_addr,
                        uint8_t addr, uint8_t *wrBuf, int wrLen)
{
	usleep(10000);
        return (diag_i2c_wr(fd, i2c_addr, addr, sizeof(addr),
                (uint8_t*)wrBuf, wrLen));
}

int pmbus_open (int bus)
{
	return (ppc_i2c_open(bus));
}

void pmbus_close (int fd)
{
	close (fd);
}
