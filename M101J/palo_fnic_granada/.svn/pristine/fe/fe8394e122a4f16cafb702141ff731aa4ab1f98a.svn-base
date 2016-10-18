/********************************************************************
 *
 *      File:   asic_utils.c
 *      Name:   Sudharshan Kadari 
 *
 *      Description: ASIC level functions for Redwood USD
 *
 *
 *      Copyright (c) 1984-2009 by Cisco Systems, Inc.
 ********************************************************************/
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include "fext.h"
#include "woo_ops.h"
#include "ppc_lib.h"
#include "pca9539.h"
#include "pca9539_lib.h"

#define FEXT_WOO_PCIE_PE_RST_L		0x80
#define FEXT_WOO_PCIE_CORE_RST_L	0x40
#define FEXT_WOO_PCIE_PLL_RST_L		0x20
#define FEXT_WOO_XG_PLL_RST_L		0x10
#define FEXT_WOO_SYS_PLL_RST_L		0x08
#define FEXT_WOO_HRESET_L		0x04

uint32_t  asic_reset_i2c_addr_get(uint8_t inst)
{
	return (FEXT_I2C_GPIO1_ADDR);
}

uint32_t  asic_reset_i2c_bus_get(uint8_t inst)
{
	return (FEXT_I2C_GPIO_BUS);
}

int asic_brd_asic_reset(uint8_t inst)
{
	uint32_t i2caddr = asic_reset_i2c_addr_get(inst);
	uint32_t reg = PCA9539_REG_OUTPUT_1;
	int	 rc, fd = ppc_i2c_open(asic_reset_i2c_bus_get(inst));
	uint8_t	 data;

        rc = pca9539_rd(fd, i2caddr, reg, &data);
        if (rc) {
                printf("Error: Failed to read PCA9539 (i2c = 0x%X)\n",
                        i2caddr);
                return (rc);
        }
	// Reset is active lo
        data &=  ~FEXT_WOO_HRESET_L; 
        rc = pca9539_wr(fd, i2caddr, reg, &data);

	ppc_i2c_close(fd);
        printf("Asic in HRESET and PCI-E Unmapped\n");
        rc = woo_pci_munmap();
	return (rc);
}

int asic_brd_pcie_reset(uint8_t inst)
{
	uint32_t i2caddr = asic_reset_i2c_addr_get(inst);
	uint32_t reg = PCA9539_REG_OUTPUT_1;
	int	 rc, fd = ppc_i2c_open(asic_reset_i2c_bus_get(inst));
	uint8_t	 data;

        rc = pca9539_rd(fd, i2caddr, reg, &data);
        if (rc) {
                printf("Error: Failed to read PCA9539 (i2c = 0x%X)\n",
                        i2caddr);
                return (rc);
        }

        data &=  ~FEXT_WOO_PCIE_PE_RST_L; 
        rc = pca9539_wr(fd, i2caddr, reg, &data);
	if (rc) return (rc);

	usleep(500);

        data &=  ~FEXT_WOO_PCIE_CORE_RST_L; 
        rc = pca9539_wr(fd, i2caddr, reg, &data);
	if (rc) return (rc);

	ppc_i2c_close(fd);
	return (rc);
}

int asic_brd_pll_reset(uint8_t inst)
{
	uint32_t i2caddr = asic_reset_i2c_addr_get(inst);
	uint32_t reg = PCA9539_REG_OUTPUT_1;
	int	 rc, fd = ppc_i2c_open(asic_reset_i2c_bus_get(inst));
	uint8_t	 data;

        rc = pca9539_rd(fd, i2caddr, reg, &data);
        if (rc) {
                printf("Error: Failed to read PCA9539 (i2c = 0x%X)\n",
                        i2caddr);
                return (rc);
        }

        data &=  ~FEXT_WOO_XG_PLL_RST_L; 
        rc = pca9539_wr(fd, i2caddr, reg, &data);
	if (rc) return (rc);
	usleep(500);

        data &=  ~FEXT_WOO_PCIE_PLL_RST_L; 
        rc = pca9539_wr(fd, i2caddr, reg, &data);
	if (rc) return (rc);
	usleep(500);

        data &=  ~FEXT_WOO_SYS_PLL_RST_L; 
        rc = pca9539_wr(fd, i2caddr, reg, &data);
	if (rc) return (rc);
	usleep(500);

	ppc_i2c_close(fd);
	return (rc);
}

int asic_brd_pll_unreset(uint8_t inst)
{
	uint32_t i2caddr = asic_reset_i2c_addr_get(inst);
	uint32_t reg = PCA9539_REG_OUTPUT_1;
	int	 rc, fd = ppc_i2c_open(asic_reset_i2c_bus_get(inst));
	uint8_t	 data;

        rc = pca9539_rd(fd, i2caddr, reg, &data);
        if (rc) {
                printf("Error: Failed to read PCA9539 (i2c = 0x%X)\n",
                        i2caddr);
                return (rc);
        }

        data |=  FEXT_WOO_SYS_PLL_RST_L; 
        rc = pca9539_wr(fd, i2caddr, reg, &data);
	if (rc) return (rc);
	usleep(500);

        data |=  FEXT_WOO_PCIE_PLL_RST_L; 
        rc = pca9539_wr(fd, i2caddr, reg, &data);
	if (rc) return (rc);
	usleep(500);

        data |=  FEXT_WOO_XG_PLL_RST_L; 
        rc = pca9539_wr(fd, i2caddr, reg, &data);
	if (rc) return (rc);
	usleep(500);

	ppc_i2c_close(fd);
	return (rc);
}

int asic_brd_pcie_unreset(uint8_t inst)
{
	uint32_t i2caddr = asic_reset_i2c_addr_get(inst);
	uint32_t reg = PCA9539_REG_OUTPUT_1;
	int	 rc, fd = ppc_i2c_open(asic_reset_i2c_bus_get(inst));
	uint8_t	 data;

        rc = pca9539_rd(fd, i2caddr, reg, &data);
        if (rc) {
                printf("Error: Failed to read PCA9539 (i2c = 0x%X)\n",
                        i2caddr);
                return (rc);
        }

        data |=  FEXT_WOO_PCIE_CORE_RST_L; 
        rc = pca9539_wr(fd, i2caddr, reg, &data);
	if (rc) return (rc);

	usleep(500);

        data |=  FEXT_WOO_PCIE_PE_RST_L; 
        rc = pca9539_wr(fd, i2caddr, reg, &data);
	if (rc) return (rc);

	ppc_i2c_close(fd);
	return (rc);
}

int asic_brd_asic_unreset(uint8_t inst)
{
	uint32_t i2caddr = asic_reset_i2c_addr_get(inst);
	uint32_t reg = PCA9539_REG_OUTPUT_1;
	int	 rc, fd = ppc_i2c_open(asic_reset_i2c_bus_get(inst));
	uint8_t	 data;

        rc = pca9539_rd(fd, i2caddr, reg, &data);
        if (rc) {
                printf("Error: Failed to read PCA9539 (i2c = 0x%X)\n",
                        i2caddr);
                return (rc);
        }

        data |=  FEXT_WOO_HRESET_L; 
        rc = pca9539_wr(fd, i2caddr, reg, &data);

	ppc_i2c_close(fd);
        sleep(1);
        printf("Asic out of HRESET and PCI-E mapped\n");
        rc = ppc_pcie_recovery();
        sleep(1);
        rc = woo_pci_mmap();
	return (rc);
}

void set_field (unsigned int* variable, unsigned int field_end_bit,
                unsigned int field_start_bit, unsigned int value)
{
    const unsigned int mask = ((1<<(field_end_bit - field_start_bit +1))-1)
                              <<field_start_bit;
    *variable = (*variable & ~mask) | ((value << field_start_bit) & mask);
}

void set_bit(unsigned int* variable, unsigned int field_bit, unsigned int value){
    set_field(variable, field_bit, field_bit, value);
}

