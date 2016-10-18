 /*********************************************************************
 *
 *      File:   adm1066_brockway.c
 *      Name:   Raghu Boreda
 *
 *      Description:
 *       adm1066_brockway - Brockway specific 
 *
 * Copyright (c) 2010 by Cisco Systems, Inc.
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
    {BROCKWAY_ADM1066_I2C_ADDR0, ADM1066_DAC_CODE_DAC1},
    {BROCKWAY_ADM1066_I2C_ADDR0, ADM1066_DAC_CODE_DAC2},
    {BROCKWAY_ADM1066_I2C_ADDR0, ADM1066_DAC_CODE_DAC3},
    {BROCKWAY_ADM1066_I2C_ADDR0, ADM1066_DAC_CODE_DAC4},
    {BROCKWAY_ADM1066_I2C_ADDR0, ADM1066_DAC_CODE_DAC5},
    {BROCKWAY_ADM1066_I2C_ADDR0, ADM1066_DAC_CODE_DAC6},
    {BROCKWAY_ADM1066_I2C_ADDR1, ADM1066_DAC_CODE_DAC1},
    {BROCKWAY_ADM1066_I2C_ADDR1, ADM1066_DAC_CODE_DAC2},
    {BROCKWAY_ADM1066_I2C_ADDR1, ADM1066_DAC_CODE_DAC3},
    {0xFF, 0x00},
    {0xFF, 0x00},
    {0xFF, 0x00},
    {0xFF, 0x00},
    {0xFF, 0x00},
};

static adm1066_mon_t monitor_fext[] = {
    /* rn = ratio_n, rd = ratio_d, vr = vref, 
     * am = adc_max, nom = nominal, cur = current 
     *i2c_addr,             adc_reg,                         rn, rd, vr,   
     *  am,  nom, cur
     */
    {BROCKWAY_ADM1066_I2C_ADDR0, ADM1066_ADC_RD_BACK_ADCHVP1, 8, 35, 2048, 
     0xffff, 5000,  5000}, //V5V0 VP1
    {BROCKWAY_ADM1066_I2C_ADDR0, ADM1066_ADC_RD_BACK_ADCHVP4, 1, 1, 2048,
     0xffff, 1000,  1000}, //V1V0 VP4
    {BROCKWAY_ADM1066_I2C_ADDR0, ADM1066_ADC_RD_BACK_ADCHVP2, 1, 1, 2048,
     0xffff, 1000,  1000}, //V1V0A VP2
    {BROCKWAY_ADM1066_I2C_ADDR0, ADM1066_ADC_RD_BACK_ADCHVP3, 1, 1, 2048,
     0xffff, 1000,  1000}, //V1V0B VP3
    {BROCKWAY_ADM1066_I2C_ADDR0, ADM1066_ADC_RD_BACK_ADCHVX1, 1, 1,  2048,
     0xffff, 1100,  1100}, //V1V1 VX1
    {BROCKWAY_ADM1066_I2C_ADDR0, ADM1066_ADC_RD_BACK_ADCHVX2, 1, 1,  2048,
     0xffff, 1100,  1100}, //V1V1A VX2
    {BROCKWAY_ADM1066_I2C_ADDR0, ADM1066_ADC_RD_BACK_ADCHVH,  2, 21, 2048, 
     0xffff, 12000, 12000}, //V12V0 VH
    {BROCKWAY_ADM1066_I2C_ADDR1, ADM1066_ADC_RD_BACK_ADCHVX2, 1, 2, 2048, 
     0xffff, 1800,  1800}, //V1V8 VX2
    {BROCKWAY_ADM1066_I2C_ADDR1, ADM1066_ADC_RD_BACK_ADCHVX3, 1, 2,  2048,
     0xffff, 2500,  2500}, //V2V5 VX3
    {BROCKWAY_ADM1066_I2C_ADDR1, ADM1066_ADC_RD_BACK_ADCHVP1, 8, 35,  2048, 
     0xffff, 3300,  3300}, //V1V0 VP1
    {BROCKWAY_ADM1066_I2C_ADDR1, ADM1066_ADC_RD_BACK_ADCHVP2, 10, 175,  2048, 
     0xffff, 12000,  12000}, //V12HS VP2
    {BROCKWAY_ADM1066_I2C_ADDR1, ADM1066_ADC_RD_BACK_ADCHVP3, 100, 218,  2048, 
     0xffff, 2500,  2500}, //V2V5HS VP3
    {BROCKWAY_ADM1066_I2C_ADDR1, ADM1066_ADC_RD_BACK_ADCHVP4, 8, 35,  2048, 
     0xffff, 3300,  3300}, //V3V3HS VP4
    {BROCKWAY_ADM1066_I2C_ADDR1, ADM1066_ADC_RD_BACK_ADCHVH, 2, 21, 2048, 
     0xffff, 12000, 12000}, //V12V0 VH
};

adm1066_channel_t channels_fext[] = {
    { "V5V0",  &trim_fext[0], 	&monitor_fext[0] },
    { "V1V0",  &trim_fext[1], 	&monitor_fext[1] },
    { "V1V0A", &trim_fext[2],   &monitor_fext[2] },
    { "V1V0B", &trim_fext[3], 	&monitor_fext[3] },
    { "V1V1",  &trim_fext[4], 	&monitor_fext[4] },
    { "V1V1A", &trim_fext[5], 	&monitor_fext[5] },
    { "V12V0A",  NULL, 	        &monitor_fext[6] },
    { "V1V8",  &trim_fext[6], 	&monitor_fext[7] },
    { "V2V5",  &trim_fext[7], 	&monitor_fext[8] },
    { "V3V3",  &trim_fext[8],   &monitor_fext[9] },
    { "V12HS",  NULL, 	        &monitor_fext[10] },
    { "V2V5HS",  NULL, 	        &monitor_fext[11]},
    { "V3V3HS",  NULL, 	        &monitor_fext[12]},
    { "V12V0B",  NULL, 		&monitor_fext[13] },
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
	uint8_t addr_data[][3] = {
		// 5.0 Volt on ADM1066A , V1V8 on ADM1066B
		{ADM1066_DACCTRL1,	ADM1066_DACCTRL_SEL_POINT_8V, 
                                        ADM1066_DACCTRL_SEL_POINT_8V},
		{ADM1066_DAC_CODE_DAC1,	ADM1066_DAC_CODE_1_22, 
                                        ADM1066_DAC_CODE_1_22},	
		{ADM1066_DAC_UL_DPLIM1,	ADM1066_DAC1_UPPER_LIMIT,    
                                        ADM1066_DAC1_UPPER_LIMIT},
		{ADM1066_DAC_LL_DNLIM1,	ADM1066_DAC1_LOWER_LIMIT,    
                                        ADM1066_DAC1_LOWER_LIMIT},
		{ADM1066_DACCTRL1,	(ADM1066_DACCTRL_ENA |
                                         ADM1066_DACCTRL_SEL_POINT_8V),         
                                        (ADM1066_DACCTRL_ENA |
					 ADM1066_DACCTRL_SEL_POINT_8V)},
                                                                                                     
		// 1.0 Volt On ADM1066A, V2V5 on ADM1066B                                                                          
		{ADM1066_DACCTRL2,	ADM1066_DACCTRL_SEL_POINT_6V,
                                        ADM1066_DACCTRL_SEL_POINT_6V},
		{ADM1066_DAC_CODE_DAC2,	ADM1066_DAC_CODE_0_501,      
                                        ADM1066_DAC_CODE_0_501},
		{ADM1066_DAC_UL_DPLIM2,	ADM1066_DAC2_UPPER_LIMIT,    
                                        ADM1066_DAC2_UPPER_LIMIT},
		{ADM1066_DAC_LL_DNLIM2,	ADM1066_DAC2_LOWER_LIMIT,    
                                        ADM1066_DAC2_LOWER_LIMIT},
		{ADM1066_DACCTRL2,	(ADM1066_DACCTRL_ENA |
                                        ADM1066_DACCTRL_SEL_POINT_6V),         
                                        (ADM1066_DACCTRL_ENA | 
					 ADM1066_DACCTRL_SEL_POINT_8V)},
		// 1.0 VoltA on ADM1066A  V3V3 on ADM1066B                                                         
		{ADM1066_DACCTRL3,	ADM1066_DACCTRL_SEL_POINT_6V,
					ADM1066_DACCTRL_SEL_POINT_6V},
		{ADM1066_DAC_CODE_DAC3,	ADM1066_DAC_CODE_0_892,      
					ADM1066_DAC_CODE_0_892},
		{ADM1066_DAC_UL_DPLIM3,	ADM1066_DAC3_UPPER_LIMIT,    
					ADM1066_DAC3_UPPER_LIMIT},
		{ADM1066_DAC_LL_DNLIM3,	ADM1066_DAC3_LOWER_LIMIT,    
					ADM1066_DAC3_LOWER_LIMIT},
		{ADM1066_DACCTRL3,	(ADM1066_DACCTRL_ENA |
                                         ADM1066_DACCTRL_SEL_POINT_6V),         
                                        (ADM1066_DACCTRL_ENA | 
					 ADM1066_DACCTRL_SEL_POINT_6V)},
                                                                                                     
		// 1.0 VoltB                                                                         
		{ADM1066_DACCTRL4,	ADM1066_DACCTRL_SEL_POINT_6V,
					ADM1066_DACCTRL_SEL_POINT_6V},
		{ADM1066_DAC_CODE_DAC4,	ADM1066_DAC_CODE_0_890,      
					ADM1066_DAC_CODE_0_890},
		{ADM1066_DAC_UL_DPLIM4,	ADM1066_DAC4_UPPER_LIMIT,    
					ADM1066_DAC4_UPPER_LIMIT},
		{ADM1066_DAC_LL_DNLIM4,	ADM1066_DAC4_LOWER_LIMIT,    
					ADM1066_DAC4_LOWER_LIMIT},
		{ADM1066_DACCTRL4,	(ADM1066_DACCTRL_ENA |
                                         ADM1066_DACCTRL_SEL_POINT_6V),         
                                        (ADM1066_DACCTRL_ENA | 
					 ADM1066_DACCTRL_SEL_POINT_6V)},
		// V1V1                                                                              
		{ADM1066_DACCTRL5,	ADM1066_DACCTRL_SEL_POINT_6V,
					ADM1066_DACCTRL_SEL_POINT_6V},
		{ADM1066_DAC_CODE_DAC5,	ADM1066_DAC_CODE_0_890,      
					ADM1066_DAC_CODE_0_890},
		{ADM1066_DAC_UL_DPLIM5,	ADM1066_DAC5_UPPER_LIMIT,    
					ADM1066_DAC5_UPPER_LIMIT},
		{ADM1066_DAC_LL_DNLIM5,	ADM1066_DAC5_LOWER_LIMIT,    
					ADM1066_DAC5_LOWER_LIMIT},
		{ADM1066_DACCTRL5,	(ADM1066_DACCTRL_ENA |
                                         ADM1066_DACCTRL_SEL_POINT_6V),         
                                        (ADM1066_DACCTRL_ENA | 
					 ADM1066_DACCTRL_SEL_POINT_6V)},
		//  V1V1A                                                                            
		{ADM1066_DACCTRL6,	ADM1066_DACCTRL_SEL_POINT_6V,
					ADM1066_DACCTRL_SEL_POINT_6V},
		{ADM1066_DAC_CODE_DAC6,	ADM1066_DAC_CODE_0_890,      
					ADM1066_DAC_CODE_0_890},
		{ADM1066_DAC_UL_DPLIM6,	ADM1066_DAC6_UPPER_LIMIT,    
					ADM1066_DAC6_UPPER_LIMIT},
		{ADM1066_DAC_LL_DNLIM6,	ADM1066_DAC6_LOWER_LIMIT,    
					ADM1066_DAC6_LOWER_LIMIT},
		{ADM1066_DACCTRL6,	(ADM1066_DACCTRL_ENA |
                                         ADM1066_DACCTRL_SEL_POINT_6V),         
                                        (ADM1066_DACCTRL_ENA | 
					 ADM1066_DACCTRL_SEL_POINT_6V)},
	};

	count = sizeof(addr_data)/(3 * sizeof(uint8_t));
        if (i2c_addr == 0x34) {
	    for (cnt = 0; cnt < count; cnt++) {
	        data = addr_data[cnt][1];
    	        rc = adm1066_wr (fd, i2c_addr, addr_data[cnt][0], &data);
	        if (rc) {
	            printf("Error: %s:%d Wr i2c=0x%X, addr=0x%X, data=0x%X\n",
	            __func__, __LINE__, i2c_addr, addr_data[cnt][0], data);
	            return (rc);
	        }
	    }
        } else {
	    for (cnt = 0; cnt < count; cnt++) {
	        data = addr_data[cnt][2];
    	        rc = adm1066_wr (fd, i2c_addr, addr_data[cnt][0], &data);
	        if (rc) {
	            printf("Error: %s:%d Wr i2c=0x%X, addr=0x%X, data=0x%X\n",
	            __func__, __LINE__, i2c_addr, addr_data[cnt][0], data);
	            return (rc);
	        }
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
