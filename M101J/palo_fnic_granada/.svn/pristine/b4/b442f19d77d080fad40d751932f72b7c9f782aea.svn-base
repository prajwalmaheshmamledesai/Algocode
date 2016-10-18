 /*********************************************************************
 *
 *      File:   adm1066_fex_lib.c
 *      Name:   Sudharshan Kadari
 *
 *      Description:
 *       adm1066_lib - Access Library 
 *
 *
 * Copyright (c) 1985-2007 by Cisco Systems, Inc.
 * All rights reserved.
 *
 *********************************************************************/
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include "adm1066_lib.h"
#include "adm1066.h"
#include "ppc_lib.h"
#include "diag_main.h"
#include "fext.h"

static adm1066_trim_t trim_fext[] = {
    {FEXT_ADM1066_I2C_ADDR, ADM1066_DAC_CODE_DAC1},
    {FEXT_ADM1066_I2C_ADDR, ADM1066_DAC_CODE_DAC2},
    {FEXT_ADM1066_I2C_ADDR, ADM1066_DAC_CODE_DAC3},
    {FEXT_ADM1066_I2C_ADDR, ADM1066_DAC_CODE_DAC4},
    {FEXT_ADM1066_I2C_ADDR, ADM1066_DAC_CODE_DAC5},
    {0xFF, 0x00},
    {0xFF, 0x00},
    {0xFF, 0x00},
    {0xFF, 0x00},
    {0xFF, 0x00},
    {0xFF, 0x00},
};

static adm1066_mon_t monitor_fext[] = {
    /* rn = ratio_n, rd = ratio_d, vr = vref, 
     * am = adc_max, nom = nominal, cur = current 
     *i2c_addr,             adc_reg,                    rn, rd, vr,   am,    
     * nom, cur
     */
    {FEXT_ADM1066_I2C_ADDR, ADM1066_ADC_RD_BACK_ADCHVP4, 8, 35, 2048, 0xffff, 
     3300,  3300}, //V3V3B VP4
    {FEXT_ADM1066_I2C_ADDR, ADM1066_ADC_RD_BACK_ADCHVP3, 8, 35, 2048, 0xffff,
     3300,  3300}, //V3V3 VP3
    {FEXT_ADM1066_I2C_ADDR, ADM1066_ADC_RD_BACK_ADCHVX1, 10, 25, 2048, 0xffff, 
     2500,  2500}, //V2V5 VX1
    {FEXT_ADM1066_I2C_ADDR, ADM1066_ADC_RD_BACK_ADCHVX2, 1, 2,  2048, 0xffff,
     1800,  1800}, //V1V8 VX2
    {FEXT_ADM1066_I2C_ADDR, ADM1066_ADC_RD_BACK_ADCHVX3, 1, 1,  2048, 0xffff,
     1100,  1100}, //V1V1 VX3
    {FEXT_ADM1066_I2C_ADDR, ADM1066_ADC_RD_BACK_ADCHVX4, 1, 1,  2048, 0xffff,
     1000,  1000}, //V1V0 VX4
    {FEXT_ADM1066_I2C_ADDR, ADM1066_ADC_RD_BACK_ADCHVH,  2, 21, 2048, 0xffff,
     12000, 12000}, //V12V0 VH
    {FEXT_ADM1066_I2C_ADDR, ADM1066_ADC_RD_BACK_ADCHVP2, 4, 35, 2048, 0xffff,
     7000,  7000}, //V7V0 VP2
    {FEXT_ADM1066_I2C_ADDR, ADM1066_ADC_RD_BACK_ADCHVP1, 8, 35, 2048, 0xffff,
     5000,  5000}, //V5V0 VP1
};

adm1066_channel_t channels_fext[] = {
    { "V3V3B",  &trim_fext[0], 	&monitor_fext[0] },
    { "V3V3",   &trim_fext[1], 	&monitor_fext[1] },
    { "V2V5",   &trim_fext[2], 	&monitor_fext[2] },
    { "V1V8",   &trim_fext[3], 	&monitor_fext[3] },
    { "V1V1",   &trim_fext[4], 	&monitor_fext[4] },
    { "V1V0",   NULL, 	        &monitor_fext[5] },
    { "V12V0",  NULL, 		&monitor_fext[6] },
    { "V7V0",   NULL, 		&monitor_fext[7] },
    { "V5V0",   NULL, 		&monitor_fext[8] },
};

int adm1066_channels_get(adm1066_channel_t **channels)
{
	*channels = channels_fext;
	return (sizeof(channels_fext)/sizeof(adm1066_channel_t));
}

/// bernward: stardard values for fext, to be tuned later if necessary
#define UPPER_LIMIT                 (0xfe)           
#define LOWER_LIMIT                 (0x02)           

#define ADM1066_DAC1_UPPER_LIMIT	0xfe
#define ADM1066_DAC1_LOWER_LIMIT	LOWER_LIMIT 
#define ADM1066_DAC2_UPPER_LIMIT	UPPER_LIMIT
#define ADM1066_DAC2_LOWER_LIMIT	LOWER_LIMIT 
#define ADM1066_DAC3_UPPER_LIMIT	UPPER_LIMIT
#define ADM1066_DAC3_LOWER_LIMIT	LOWER_LIMIT 
#define ADM1066_DAC4_UPPER_LIMIT	UPPER_LIMIT
#define ADM1066_DAC4_LOWER_LIMIT	LOWER_LIMIT 
#define ADM1066_DAC5_UPPER_LIMIT	UPPER_LIMIT
#define ADM1066_DAC5_LOWER_LIMIT	LOWER_LIMIT 
#define ADM1066_DAC6_UPPER_LIMIT	UPPER_LIMIT
#define ADM1066_DAC6_LOWER_LIMIT	LOWER_LIMIT 

#define ADM1066_DAC_CODE_1_22        0x7f
#define ADM1066_DAC_CODE_0_501       0x7f
#define ADM1066_DAC_CODE_0_892       0x7f
#define ADM1066_DAC_CODE_0_890       0x7f

static int adm1066_init_dac_chan (int fd, uint16_t i2c_addr)
{
	int	cnt, count, rc = 0;
	uint8_t	data;
	// Write
	uint8_t addr_data[][2] = {
		// 3.3 Volt
		{ADM1066_DACCTRL1,		ADM1066_DACCTRL_SEL_POINT_8V},
		{ADM1066_DAC_CODE_DAC1,		ADM1066_DAC_CODE_1_22},	
		{ADM1066_DAC_UL_DPLIM1,		ADM1066_DAC1_UPPER_LIMIT},
		{ADM1066_DAC_LL_DNLIM1,		ADM1066_DAC1_LOWER_LIMIT},
		{ADM1066_DACCTRL1,		ADM1066_DACCTRL_ENA |
						ADM1066_DACCTRL_SEL_POINT_8V},

		// 2.5 Volt
		{ADM1066_DACCTRL2,		ADM1066_DACCTRL_SEL_POINT_8V},
		{ADM1066_DAC_CODE_DAC2,		ADM1066_DAC_CODE_0_501},
		{ADM1066_DAC_UL_DPLIM2,		ADM1066_DAC2_UPPER_LIMIT},
		{ADM1066_DAC_LL_DNLIM2,		ADM1066_DAC2_LOWER_LIMIT},
		{ADM1066_DACCTRL2,		ADM1066_DACCTRL_ENA | 
						ADM1066_DACCTRL_SEL_POINT_8V},

		// 1.8 Volt
		{ADM1066_DACCTRL3,		ADM1066_DACCTRL_SEL_POINT_8V},
		{ADM1066_DAC_CODE_DAC3,		ADM1066_DAC_CODE_0_892},
		{ADM1066_DAC_UL_DPLIM3,		ADM1066_DAC3_UPPER_LIMIT},
		{ADM1066_DAC_LL_DNLIM3,		ADM1066_DAC3_LOWER_LIMIT},
		{ADM1066_DACCTRL3,		ADM1066_DACCTRL_ENA | 
						ADM1066_DACCTRL_SEL_POINT_8V},

		// 1.2 Volt
		{ADM1066_DACCTRL4,		ADM1066_DACCTRL_SEL_POINT_8V},
		{ADM1066_DAC_CODE_DAC4,		ADM1066_DAC_CODE_0_890},
		{ADM1066_DAC_UL_DPLIM4,		ADM1066_DAC4_UPPER_LIMIT},
		{ADM1066_DAC_LL_DNLIM4,		ADM1066_DAC4_LOWER_LIMIT},
		{ADM1066_DACCTRL4,		ADM1066_DACCTRL_ENA | 
						ADM1066_DACCTRL_SEL_POINT_8V},
		// V1VA 
		{ADM1066_DACCTRL5,		ADM1066_DACCTRL_SEL_POINT_8V},
		{ADM1066_DAC_CODE_DAC5,		ADM1066_DAC_CODE_0_890},
		{ADM1066_DAC_UL_DPLIM5,		ADM1066_DAC5_UPPER_LIMIT},
		{ADM1066_DAC_LL_DNLIM5,		ADM1066_DAC5_LOWER_LIMIT},
		{ADM1066_DACCTRL5,		ADM1066_DACCTRL_ENA | 
						ADM1066_DACCTRL_SEL_POINT_8V},
		//  V1VB
		{ADM1066_DACCTRL6,		ADM1066_DACCTRL_SEL_POINT_8V},
		{ADM1066_DAC_CODE_DAC6,		ADM1066_DAC_CODE_0_890},
		{ADM1066_DAC_UL_DPLIM6,		ADM1066_DAC6_UPPER_LIMIT},
		{ADM1066_DAC_LL_DNLIM6,		ADM1066_DAC6_LOWER_LIMIT},
		{ADM1066_DACCTRL6,		ADM1066_DACCTRL_ENA | 
						ADM1066_DACCTRL_SEL_POINT_8V},

	};

	count = sizeof(addr_data)/(2 * sizeof(uint8_t));
	for (cnt = 0; cnt < count; cnt++) {
		data = addr_data[cnt][1];
    		rc = adm1066_wr (fd, i2c_addr, addr_data[cnt][0], &data);
		if (rc) {
			printf("Error: %s:%d Wr i2c=0x%X, addr=0x%X, data=0x%X\n",
			__func__, __LINE__, i2c_addr, addr_data[cnt][0], data);
			return (rc);
		}
	}

	return (rc);
}

static int adm1066_init_adc_fext (int fd, uint16_t i2c_addr)
{
        int     cnt, count, rc = 0;
        uint8_t data;
        // Write
        uint8_t addr_data[][2] = {
		{ADM1066_ADC_SETUP_RRSEL1,  (uint8_t)
					   ~(ADM1066_VX3CHAN_EXCLUDE | 
					     ADM1066_VX2CHAN_EXCLUDE |
					     ADM1066_VX1CHAN_EXCLUDE | 
					     ADM1066_VHCHAN_EXCLUDE  | 
					     ADM1066_VP4CHAN_EXCLUDE |
					     ADM1066_VP3CHAN_EXCLUDE | 
					     ADM1066_VP2CHAN_EXCLUDE | 
					     ADM1066_VP1CHAN_EXCLUDE )}, 
                {ADM1066_ADC_SETUP_RRSEL2,  ~(ADM1066_VX4_EXCLUDE)},
		{ADM1066_ADC_SETUP_RRCTRL,   ADM1066_MSK_AVERAGE   | 
					     ADM1066_MSK_ENABLE    | 
					     ADM1066_MSK_GO},
	}; 

        count = sizeof(addr_data)/(2 * sizeof(uint8_t));
        for (cnt = 0; cnt < count; cnt++) {
                data = addr_data[cnt][1];
                rc = adm1066_wr (fd, i2c_addr, addr_data[cnt][0], &data);
                if (rc) {
                        printf("Error: %s:%d Wr i2c=0x%X, addr=0x%X, data=0x%X\n",
                        __func__, __LINE__, i2c_addr, addr_data[cnt][0], data);
                        return (rc);
                }
        }

        return (rc);

}

int adm1066_init (int fd, uint16_t i2c_addr)
{
	int rc;

#if 1
	//bernward: enable
	rc = adm1066_init_sfd_cfg(fd, i2c_addr);
	if (rc) {
		printf("Error: adm1066_init_sfd_cfg failed. i2c=0x%X, rc=%d\n",
			i2c_addr, rc);
		return (rc);
	}
#endif
	rc = adm1066_init_adc_lim(fd, i2c_addr);
	if (rc) {
		printf("Error: adm1066_init_adc_lim failed. i2c=0x%X, rc=%d\n",
			i2c_addr, rc);
		return (rc);
	}
	sleep(1);
	rc = adm1066_init_adc_fext (fd, i2c_addr);
	if (rc) {
		printf("Error: adm1066_init_adc_fext failed. i2c=0x%X, rc=%d\n",
			i2c_addr, rc);
		return (rc);
	}
	sleep(1);
	rc = adm1066_init_dac_chan (fd, i2c_addr);
	if (rc) {
		printf("Error: adm1066_init_dac_chan failed. i2c=0x%X, rc=%d\n",
			i2c_addr, rc);
		return (rc);
	}
	return (rc);
}
