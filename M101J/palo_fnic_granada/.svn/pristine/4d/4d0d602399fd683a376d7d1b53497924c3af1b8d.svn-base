/********************************************************************
 *
 *      File:   redwood_utils.c
 *      Name:   Sudharshan Kadari
 *
 *      Description:
 *
 *
 * Copyright (c) 1985-2007 by Cisco Systems, Inc.
 * All rights reserved.
 *
 *
 *********************************************************************/

#include "diag_main.h"
#include "red_hw_lib.h"
#include "redwood_asic.h"
#include "pca9539.h"
#include "pca9539_lib.h"
#include "ppc_lib.h"
#ifdef DIAG_FEX
#include "diag_fex.h"
#else
#include "diag_iom.h"
#endif

typedef struct _rdw_gpio_rst_s_
{
	uint8_t	inst;
	uint8_t unit;
	uint8_t pll_mask;
	uint8_t reset_mask;
	uint32_t i2c_addr;
} rdw_gpio_rst_t;

#ifdef DIAG_FEX
rdw_gpio_rst_t rdw_rst[] = {
	{0, 1, 0x0E, 0x01, FEX_I2C_GPIO2_ADDR},
	{1, 1, 0xE0, 0x10, FEX_I2C_GPIO2_ADDR},
	{2, 0, 0x0E, 0x01, FEX_I2C_GPIO2_ADDR},
	{3, 0, 0xE0, 0x10, FEX_I2C_GPIO2_ADDR},
	{4, 1, 0x0E, 0x01, FEX_I2C_GPIO1_ADDR},
	{5, 1, 0xE0, 0x10, FEX_I2C_GPIO1_ADDR},
	{6, 0, 0x0E, 0x01, FEX_I2C_GPIO1_ADDR},
	};
#else
rdw_gpio_rst_t rdw_rst[] = {
	{0, 0, 0x07, 0x08, SI2C_IOM_GPIO3_ADDR},
};
#endif

int red_hw_pll_unreset(int i2c_fd, uint32_t inst)
{
	uint8_t	data = 0;
	int	rc = 0;
	uint32_t i2caddr = rdw_rst[inst].i2c_addr;
	uint32_t reg = rdw_rst[inst].unit ? 
			PCA9539_REG_OUTPUT_1 : 
			PCA9539_REG_OUTPUT_0;

	if (inst >= (sizeof(rdw_rst)/sizeof(rdw_gpio_rst_t))) {
		printf(" Error: %s(%d) Invalid instance %d\n", 
			__func__, __LINE__,  inst);
		return (-1);
	}

	rc = pca9539_rd(i2c_fd, i2caddr, reg, &data);
	if (rc) {
		printf("Error: Failed to read PCA9539 (i2c = 0x%X)\n", 
			i2caddr);
		return (rc);
	}
	data |= rdw_rst[inst].pll_mask;
	return (pca9539_wr(i2c_fd, i2caddr, reg, &data));
}

int red_hw_pll_reset(int i2c_fd, uint32_t inst)
{
	uint8_t	data = 0;
	int	rc = 0;
	uint32_t i2caddr = rdw_rst[inst].i2c_addr;
	uint32_t reg = rdw_rst[inst].unit ? 
			PCA9539_REG_OUTPUT_1 : 
			PCA9539_REG_OUTPUT_0;

	if (inst >= (sizeof(rdw_rst)/sizeof(rdw_gpio_rst_t))) {
		printf(" Error: %s(%d) Invalid instance %d\n", 
			__func__, __LINE__,  inst);
		return (-1);
	}

	rc = pca9539_rd(i2c_fd, i2caddr, reg, &data);
	if (rc) {
		printf("Error: Failed to read PCA9539 (i2c = 0x%X)\n", 
			i2caddr);
		return (rc);
	}

	data &=  ~(rdw_rst[inst].pll_mask);
	return (pca9539_wr(i2c_fd, i2caddr, reg, &data));
}

int red_hw_asic_unreset(int i2c_fd, uint32_t inst)
{
	uint8_t	data = 0;
	int	rc = 0;
	uint32_t i2caddr = rdw_rst[inst].i2c_addr;
	uint32_t reg = rdw_rst[inst].unit ? 
			PCA9539_REG_OUTPUT_1 : 
			PCA9539_REG_OUTPUT_0;

	if (inst >= (sizeof(rdw_rst)/sizeof(rdw_gpio_rst_t))) {
		printf(" Error: %s(%d) Invalid instance %d\n", 
			__func__, __LINE__,  inst);
		return (-1);
	}

	rc = pca9539_rd(i2c_fd, i2caddr, reg, &data);
	if (rc) {
		printf("Error: Failed to read PCA9539 (i2c = 0x%X)\n", 
			i2caddr);
		return (rc);
	}

	data |= rdw_rst[inst].reset_mask;
	return (pca9539_wr(i2c_fd, i2caddr, reg, &data));
}

int red_hw_asic_reset(int i2c_fd, uint32_t inst)
{
	uint8_t	data = 0;
	int	rc = 0;
	uint32_t i2caddr = rdw_rst[inst].i2c_addr;
	uint32_t reg = rdw_rst[inst].unit ? 
			PCA9539_REG_OUTPUT_1 : 
			PCA9539_REG_OUTPUT_0;

	if (inst >= (sizeof(rdw_rst)/sizeof(rdw_gpio_rst_t))) {
		printf(" Error: %s(%d) Invalid instance %d\n", 
			__func__, __LINE__,  inst);
		return (-1);
	}

	rc = pca9539_rd(i2c_fd, i2caddr, reg, &data);
	if (rc) {
		printf("Error: Failed to read PCA9539 (i2c = 0x%X)\n", 
			i2caddr);
		return (rc);
	}

	data &= ~rdw_rst[inst].reset_mask; 
	return (pca9539_wr(i2c_fd, i2caddr, reg, &data));
}

void set_field(unsigned int* variable, unsigned int field_end_bit, 
                unsigned int field_start_bit, unsigned int value)
{   
        const unsigned int mask = ((1<<(field_end_bit - field_start_bit +1))-1)<<field_start_bit;
        *variable = (*variable & ~mask)
                 | ((value << field_start_bit) & mask)
                 ;
}

void set_bit(unsigned int* variable, unsigned int field_bit, unsigned int value)
{   
        set_field(variable, field_bit, field_bit, value);
}

typedef struct _port_name_s {
        uint8_t port_num;
        uint8_t *port_name;
} rdw_port_name_t;

rdw_port_name_t rdw_pn[] = 
{
    {cif, "cif"},
    {bif, "bif"},
    {hif0, "hif0"},
    {hif1, "hif1"},
    {hif2, "hif2"},
    {hif3, "hif3"},
    {hif4, "hif4"},
    {hif5, "hif5"},
    {hif6, "hif6"},
    {hif7, "hif7"},
    {nif0, "nif0"},
    {nif1, "nif1"},
    {nif2, "nif2"},
    {nif3, "nif3"},
    {-1,   NULL},
};

uint8_t red_name_to_port (uint8_t *name, uint8_t *port)
{
        rdw_port_name_t *prdw = rdw_pn;

        while(prdw->port_name) {
                if (!strncasecmp(name, prdw->port_name, strlen(prdw->port_name))) {
                        *port = prdw->port_num;
                        return (0);
                }
                prdw++;
        }
        return (-1);
}

void red_hw_unreset (uint8_t bus, uint8_t inst)
{
        int fd;
        fd = ppc_i2c_open(bus);
        redwood_hw_reset(fd, inst);
        redwood_hw_unreset(fd, inst);
        ppc_i2c_close(fd);
}

void red_hw_unreset_all(uint8_t reset_bus, uint8_t rdw_bus, uint8_t inst)
{
        int fd0, fd1;
        fd0 = ppc_i2c_open(rdw_bus);
        fd1 = ppc_i2c_open(reset_bus);

	redwood_hw_reset (fd1, inst);
	redwood_bring_out_of_reset(fd0, fd1, inst);

	ppc_i2c_close(fd0);
	ppc_i2c_close(fd1);
}

int red_hw_init(uint8_t bus, uint8_t inst)
{
        int rc, fd;
        fd = ppc_i2c_open(bus);
	rc = redwood_init(fd, inst);
	ppc_i2c_close(fd);
	return (rc);
}

int red_core_init(uint8_t bus, uint8_t inst)
{
        int rc, fd0;
        fd0 = ppc_i2c_open(bus);
	rc = redwood_core_init(fd0, inst);
	ppc_i2c_close(fd0);
	return (rc);
}


int red_port_init(uint8_t bus, uint8_t inst)
{
        int rc, fd0;
        fd0 = ppc_i2c_open(bus);
	rc = redwood_port_init(fd0, inst);
	ppc_i2c_close(fd0);
	return (rc);
}

/***************************************************************
* port_is_hif
*
* Description:
* Return non-0 if the given asic_port corresponds to a HIF
* port.
******************************************************************/
int port_is_hif( int asic_port )
{
    return(((asic_port >= hif0) && (asic_port <=hif7)) ? 1 : 0);
}

