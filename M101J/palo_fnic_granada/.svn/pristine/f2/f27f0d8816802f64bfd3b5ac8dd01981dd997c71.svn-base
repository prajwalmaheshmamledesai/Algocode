/********************************************************************
 *
 *      File:   mdio_lib.c
 *      Name:   Sudharshan Kadari
 *
 *      Description:
 *       Diag MDIO Library 
 *
 *
 * Copyright (c) 1985-2007 by Cisco Systems, Inc.
 * All rights reserved.
 *
 *
 *********************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <linux/mdio_bus.h>
#include <fcntl.h>
#include <errno.h>
#include "mdio_lib.h"

#include "xcvr_gpio.h"

int verbose = 0;
static int mdio_fd = -1;

int mdio_open ()
{
        char *o_path = "/dev/mdio0";
        int fd = -1;

        fd = open(o_path, O_RDWR);
        if (fd < 0) {
                perror(o_path);
                return (-1);
        }
        return (fd);
}

void mdio_close (int fd)
{
        close(fd);
}

int
diag_mdio_read(int fd, int address, int reg, uint16_t *value)
{
    struct mdio_rdwr_ioctl_data arg;

    arg.address = address;
    arg.reg = reg;
    arg.data = 0;
    if (0 != ioctl(fd, IOC_MDIO_READ, &arg)) {
        return (errno);
    }
    if (value != NULL) {
        *value = arg.data;
    }
    if (verbose > 1) {
        printf("\tmdio_read %d:%d => 0x%04x\n", address, reg, arg.data);
    }
    return (0);
}

int
diag_mdio_write(int fd, int address, int reg, uint16_t value)
{
    struct mdio_rdwr_ioctl_data arg;

    if (verbose > 1) {
        printf("\tmdio_write %d:%d <= 0x%04x\n", address, reg, value);
    }

    arg.address = address;
    arg.reg = reg;
    arg.data = value;
    if (0 != ioctl(fd, IOC_MDIO_WRITE, &arg)) {
        return (errno);
    }
    return (0);
}

int mdio_hw_rd(int address, int reg, uint16_t *value)
{
	int rc = 0;
	if (mdio_fd < 0) {
		mdio_fd = mdio_open();
		if (mdio_fd <= 0) {
			printf("Failed to open Mdio device\n");
			return (-1);
		}
	}

	rc = mdio_read (mdio_fd, address, reg, value);
	
	mdio_close(mdio_fd);
	mdio_fd = -1;

	return (rc);
}

int mdio_hw_wr(int address, int reg, uint16_t value)
{
	int rc = 0;
	if (mdio_fd < 0) {
		mdio_fd = mdio_open();
		if (mdio_fd <= 0) {
			printf("Failed to open Mdio device\n");
			return (-1);
		}
	}

	rc = mdio_write (mdio_fd, address, reg, value);
	mdio_close(mdio_fd);
	mdio_fd = -1;

	return (rc);
}

void mdio_busscan(uint32_t *dev_found)
{
        int address;
        unsigned long id;
        uint16_t data;

	*dev_found = 0x00;
        for (address = 0; address <= 0x1f; ++address) {
                if (0 != mdio_hw_rd(address, 2, &data)) {
                        continue;
                }
                id = (0xffff & data) << 16;
                if (0 != mdio_hw_rd(address, 3, &data)) {
                        fprintf(stderr, "Error on read of addr %d:3\n",
                            address);
                        continue;
                }
                id |= data & 0xffff;
		printf("[%d] : 0x%X\n", address, (uint32_t)id);
                if (id != 0xffffffff) {
                        printf ("  mdio %d id = 0x%08x\n", 
				address, (unsigned int)id);
			*dev_found |= 0x01 << address;
                }
        }
}


extern void xcvr_gpio_mdio_write (gpio_pin_t *bus, u_int32_t prtad, u_int32_t devad,
                      u_int32_t regad, u_int16_t regval);
extern void xcvr_gpio_mdio_read (gpio_pin_t *bus, u_int32_t prtad, u_int32_t devad,
                     u_int32_t regad, u_int16_t *regval);

static gpio_pin_t phy_gpio;

#define GPIO2_MDIO_CLK                  19 // 12
#define GPIO2_MDIO_DATA                 18 // 13

int phyg_rd (int fd, uint8_t port_addr,
            uint8_t device_class, uint16_t reg_addr, uint16_t *data)
{
	if (!device_class) {
		printf("Error: Invalid device class\n");
		return (-1);
	}


	phy_gpio.mdio_pin = GPIO2_MDIO_DATA; 
	phy_gpio.mdc_pin = GPIO2_MDIO_CLK;

	if (verbose) 
		printf("%s:%d [P%d : C%d] 0x%X\n",
		__func__, __LINE__, port_addr, device_class, 
			reg_addr);

	xcvr_gpio_mdio_read (&phy_gpio, port_addr, device_class, reg_addr, data);

	return (0);
}

// add mdio clause 45 write access routine.
int phyg_wr (int fd, uint8_t port_addr,
            uint8_t device_class, uint16_t reg_addr, uint16_t data)
{
	if (!device_class) {
		printf("Error: Invalid device class\n");
		return (-1);
	}
	phy_gpio.mdio_pin = GPIO2_MDIO_DATA; 
	phy_gpio.mdc_pin = GPIO2_MDIO_CLK;

	if (verbose)
		printf("%s:%d [P%d : C%d] 0x%X\n",
			__func__, __LINE__, port_addr, device_class, 
			reg_addr);

	xcvr_gpio_mdio_write (&phy_gpio, port_addr, device_class, reg_addr, data);

	return (0);
}


extern void     mdio_write_3_3v(int fd, uint8_t mdio_port, uint8_t device_class,
			uint16_t reg_addr, uint16_t reg_data);
extern uint16_t mdio_read_3_3v(int fd, uint8_t mdio_port, uint8_t device_class,
			uint16_t reg_addr);
int
mdio_c45_write(int fd, int mdio_inst_or_addr, int mdio_port, int device_class,
                    uint16_t reg_addr, uint16_t reg_value)
{
#ifdef CAMPBELL
        return (phyg_wr(fd, mdio_port, device_class, reg_addr, reg_value));
#elif defined SARATOGA
	mdio_write_3_3v(mdio_inst_or_addr, mdio_port, device_class, reg_addr, reg_value);
	return (0);
#elif defined SOQUEL
       //TBD
        return (0); 
#endif

}

int
mdio_c45_read(int fd, int mdio_inst_or_addr, int mdio_port, int device_class,
                   uint16_t reg_addr, uint16_t *reg_value)
{
#ifdef CAMPBELL
        return (phyg_rd(fd, mdio_port, device_class, reg_addr, reg_value));
#elif defined SARATOGA
	*reg_value =  mdio_read_3_3v(mdio_inst_or_addr, mdio_port, device_class, reg_addr);
	return (0);
#elif defined SOQUEL
       //TBD
        return (0);
#endif
}


int
phy_g_wr (int fd, int mdio_inst_or_addr, int mdio_port, int device_class,
                    uint16_t reg_addr, uint16_t reg_value)
{
        return (phyg_wr(fd, mdio_port, device_class, reg_addr, reg_value));
}

int
phy_g_rd (int fd, int mdio_inst_or_addr, int mdio_port, int device_class,
                   uint16_t reg_addr, uint16_t *reg_value)
{
        return (phyg_rd(fd, mdio_port, device_class, reg_addr, reg_value));
}

