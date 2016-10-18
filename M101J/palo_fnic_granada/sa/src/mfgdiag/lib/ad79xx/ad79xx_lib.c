/********************************************************************
 *
 *      File:   ad79xx_lib.c 
 *      Name:   Sudharshan Kadari
 *
 *      Description:
 *       ad79xx_lib - Voltage Monitor 
 *
 *
 * Copyright (c) 1985-2009 by Cisco Systems, Inc.
 * All rights reserved.
 *
 *********************************************************************/

#include <stdio.h>
#include <stdint.h>
#include "ad79xx.h"

extern int I2CByteRead_Block ( int Fh, unsigned char Bus, unsigned char Dev,
		unsigned char Reg, unsigned char *rxBuf, int bytes, int verbose);
extern int I2CByteWrite_Block (int Fh, unsigned char Bus, unsigned char Dev, 
		unsigned char *txBuf, int bytes, int verbose);


int ad79xx_rd (int fd, uint8_t bus, uint8_t i2c_addr, uint8_t offset, uint16_t *data)
{
	int     rc = 0;
	uint8_t data8, *pdata = (uint8_t*)data;
	
        rc = I2CByteRead_Block(fd, bus, i2c_addr, offset, pdata, 2, 0);

	data8    = pdata[0]; 	
	pdata[0] = pdata[1]; 	
	pdata[1] = data8; 

	return (rc);
}

int ad79xx_wr (int fd, uint8_t bus, uint8_t i2c_addr, uint8_t offset, uint16_t *data)
{
	uint8_t wr_data[3];

	wr_data[0] = offset;
	wr_data[1] = (*data >> 8) & 0xFF;
	wr_data[2] = *data & 0xFF;

        return(I2CByteWrite_Block(fd, bus, i2c_addr, wr_data, 3, 0));
}

int ad79xx_chan_rd (int fd, uint8_t bus, uint8_t i2c_addr, uint8_t ch, uint16_t *data)
{
	if ((ch < 1) || (ch > 8)) {
		printf(" Invalid Ch number (%d)\n", ch);
		return (-1);
	}
	ch -= 1;
        return (ad79xx_rd (fd, bus, i2c_addr, AD79XX_VIN1_SEL + (ch * AD79XX_VIN_OFF), data));
}


int ad79xx_reg_dump (int fd, uint8_t bus, uint8_t i2c_addr)
{
        int   rc = 0, cnt;
        uint16_t data = 0x00;
        char *reg_name[] = {
		"AD79XX_CONVERSION_RESULT_REG",
		"AD79XX_ALERT_STATUS_REG",
		"AD79XX_CONFIGURATION_REG",
		"AD79XX_CYCLE_TIMER_REG",
		"AD79XX_DATA_LO_CH1_REG",
		"AD79XX_DATA_HI_CH1_REG",
		"AD79XX_DATA_HYST_CH1_REG",
		"AD79XX_DATA_LO_CH2_REG",
		"AD79XX_DATA_HI_CH2_REG",
		"AD79XX_DATA_HYST_CH2_REG",
		"AD79XX_DATA_LO_CH3_REG",
		"AD79XX_DATA_HI_CH3_REG",
		"AD79XX_DATA_HYST_CH3_REG",
		"AD79XX_DATA_LO_CH4_REG",
		"AD79XX_DATA_HI_CH4_REG",
		"AD79XX_DATA_HYST_CH4_REG",
		}; 

        for (cnt = 0; cnt < sizeof(reg_name)/sizeof(char*); cnt++) {
                rc = ad79xx_rd (fd, bus, i2c_addr, cnt, &data);
                if (rc) {
                        printf("Error: reading reg %d\n", cnt);
                        return (rc);
                }

                printf("  %-28s : 0x%04X\n", reg_name[cnt], data);
        }

        return (rc);
}

