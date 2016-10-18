 /*********************************************************************
 *
 *      File:   adm1066_lib.c
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
#include "diag_iom.h"

static adm1066_trim_t trim_iom[] = {
    {SI2C_IOM_PWRSEQ_ADDR, ADM1066_DAC_CODE_DAC1},
    {SI2C_IOM_PWRSEQ_ADDR, ADM1066_DAC_CODE_DAC2},
    {SI2C_IOM_PWRSEQ_ADDR, ADM1066_DAC_CODE_DAC3},
    {SI2C_IOM_PWRSEQ_ADDR, ADM1066_DAC_CODE_DAC4},
    {0xFF, 0x00},
    {0xFF, 0x00},
    {0xFF, 0x00},
    {0xFF, 0x00},
    {0xFF, 0x00},
    {0xFF, 0x00},
    {0xFF, 0x00},
};

static adm1066_mon_t monitor_iom[] = {
    /*i2c_addr,             adc_reg,           ratio_n, ratio_d, vref, adc_max,nominal, current */
    { SI2C_IOM_PWRSEQ_ADDR, ADM1066_ADC_RD_BACK_ADCHVP4,  1000, 2181,  2048, 0xffff, 1500,  1500  }, //V1V5
    { SI2C_IOM_PWRSEQ_ADDR, ADM1066_ADC_RD_BACK_ADCHVP2,  1000, 2181,  2048, 0xffff, 2500,  2500  }, //V2V5
    { SI2C_IOM_PWRSEQ_ADDR, ADM1066_ADC_RD_BACK_ADCHVP3,  1000, 2181,  2048, 0xffff, 1800,  1800  }, //V1V8
    { SI2C_IOM_PWRSEQ_ADDR, ADM1066_ADC_RD_BACK_ADCHVX1,  1,    1,     2048, 0xffff, 1200,  1200  }, //V1V2
    { SI2C_IOM_PWRSEQ_ADDR, ADM1066_ADC_RD_BACK_ADCHVH,   1000, 10472, 2048, 0xffff, 12000, 12000 }, //V12V0
    { SI2C_IOM_PWRSEQ_ADDR, ADM1066_ADC_RD_BACK_ADCHVP1,  1000, 4363,  2048, 0xffff, 3300,  3300  }, //V3V3
    { SI2C_IOM_PWRSEQ_ADDR, ADM1066_ADC_RD_BACK_ADCHAUX1, 1000, 2181,  2048, 0xffff, 1500,  1500  }, //V3V3BP
    { SI2C_IOM_PWRSEQ_ADDR, ADM1066_ADC_RD_BACK_ADCHVX5,  1000, 2181,  2048, 0xffff, 1500,  1500  }, //HSOK
    { SI2C_IOM_PWRSEQ_ADDR, ADM1066_ADC_RD_BACK_ADCHVX4,  1,    1,     2048, 0xffff,  900,   900  }, //V0V9
//    { SI2C_IOM_PWRSEQ_ADDR, ADM1066_ADC_RD_BACK_ADCHVX3,  1,    1,     2048, 0xffff, 1120,  1120  }, //V1VB
    { SI2C_IOM_PWRSEQ_ADDR, ADM1066_ADC_RD_BACK_ADCHVX3,  1,    1,     2048, 0xffff, 1000,  1000  }, //V1VB
    { SI2C_IOM_PWRSEQ_ADDR, ADM1066_ADC_RD_BACK_ADCHVX2,  1,    1,     2048, 0xffff, 1000,  1000  }, //V1VA
};

adm1066_channel_t channels_iom[] = {
    { "V1V5",   &trim_iom[0], &monitor_iom[0] },
    { "V2V5",   &trim_iom[1], &monitor_iom[1] },
    { "V1V8",   &trim_iom[2], &monitor_iom[2] },
    { "V1V2",   &trim_iom[3], &monitor_iom[3] },
    { "V12V0",  NULL, &monitor_iom[4] },
    { "V3V3",   NULL, &monitor_iom[5] },
//    { "V3V3BP", NULL, &monitor_iom[6] },
//    { "HSOK",   NULL, &monitor_iom[7] },
    { "V0V9",   NULL, &monitor_iom[8] },
    { "V1VB",   NULL, &monitor_iom[9] },
    { "V1VA",   NULL, &monitor_iom[10] },
};

int adm1066_channels_get(adm1066_channel_t **channels)
{
	*channels = channels_iom;
	return (sizeof(channels_iom)/sizeof(adm1066_channel_t));
}

#define ADM1066_DAC1_UPPER_LIMIT	0xC6
#define ADM1066_DAC1_LOWER_LIMIT	0x1F
#define ADM1066_DAC2_UPPER_LIMIT	0x84
#define ADM1066_DAC2_LOWER_LIMIT	0x25
#define ADM1066_DAC3_UPPER_LIMIT	0xCC
#define ADM1066_DAC3_LOWER_LIMIT	0x7F
#define ADM1066_DAC4_UPPER_LIMIT	0xBA
#define ADM1066_DAC4_LOWER_LIMIT	0x90

#define ADM1066_DAC_CODE_1_22        0x72
#define ADM1066_DAC_CODE_0_501       0x55
#define ADM1066_DAC_CODE_0_892       0xA6
#define ADM1066_DAC_CODE_0_890       0xA5


int adm1066_init_dac_chan (int fd, uint16_t i2c_addr)
{
	int	cnt, count, rc = 0;
	uint8_t	data;
	// Write
	uint8_t addr_data[][2] = {
		{ADM1066_DACCTRL1,		0x00},
		{ADM1066_DAC_CODE_DAC1,		ADM1066_DAC_CODE_1_22},	
		{ADM1066_DAC_UL_DPLIM1,		ADM1066_DAC1_UPPER_LIMIT},
		{ADM1066_DAC_LL_DNLIM1,		ADM1066_DAC1_LOWER_LIMIT},
		{ADM1066_DACCTRL1,		ADM1066_DACCTRL_ENA |
						ADM1066_DACCTRL_SEL_1_POINT_25V},

		{ADM1066_DACCTRL2,		ADM1066_DACCTRL_SEL_POINT_6V},
		{ADM1066_DAC_CODE_DAC2,		ADM1066_DAC_CODE_0_501},
		{ADM1066_DAC_UL_DPLIM2,		ADM1066_DAC2_UPPER_LIMIT},
		{ADM1066_DAC_LL_DNLIM2,		ADM1066_DAC2_LOWER_LIMIT},
		{ADM1066_DACCTRL2,		ADM1066_DACCTRL_ENA | 
						ADM1066_DACCTRL_SEL_POINT_6V},

		{ADM1066_DACCTRL3,		ADM1066_DACCTRL_SEL_POINT_8V},
		{ADM1066_DAC_CODE_DAC3,		ADM1066_DAC_CODE_0_892},
		{ADM1066_DAC_UL_DPLIM3,		ADM1066_DAC3_UPPER_LIMIT},
		{ADM1066_DAC_LL_DNLIM3,		ADM1066_DAC3_LOWER_LIMIT},
		{ADM1066_DACCTRL3,		ADM1066_DACCTRL_ENA | 
						ADM1066_DACCTRL_SEL_POINT_8V},

		{ADM1066_DACCTRL4,		ADM1066_DACCTRL_SEL_POINT_8V},
		{ADM1066_DAC_CODE_DAC4,		ADM1066_DAC_CODE_0_890},
		{ADM1066_DAC_UL_DPLIM4,		ADM1066_DAC4_UPPER_LIMIT},
		{ADM1066_DAC_LL_DNLIM4,		ADM1066_DAC4_LOWER_LIMIT},
		{ADM1066_DACCTRL4,		ADM1066_DACCTRL_ENA | 
						ADM1066_DACCTRL_SEL_POINT_8V},

		// DAC5 and DAC6 are not used. So don't enable.
//		{ADM1066_DAC_UL_DPLIM5,		ADM1066_DAC_UPPER_LIMIT},
//		{ADM1066_DAC_LL_DNLIM5,		ADM1066_DAC_LOWER_LIMIT},
//		{ADM1066_DAC_CODE_DAC5,		ADM1066_DAC_CODE_MIDCODE},
//		{ADM1066_DACCTRL5,		ADM1066_DACCTRL_SEL_1_POINT_25V},

//		{ADM1066_DAC_UL_DPLIM6,		ADM1066_DAC_UPPER_LIMIT},
//		{ADM1066_DAC_LL_DNLIM6,		ADM1066_DAC_LOWER_LIMIT},
//		{ADM1066_DAC_CODE_DAC6,		ADM1066_DAC_CODE_MIDCODE},
//		{ADM1066_DACCTRL6,		ADM1066_DACCTRL_SEL_1_POINT_25V},
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

int adm1066_init_adc_iom (int fd, uint16_t i2c_addr)
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
//		{ADM1066_ADC_SETUP_RRSEL2,  (ADM1066_EXTS2_EXCLUDE |
//					     ADM1066_EXTS1_EXCLUDE)}, 
//		{ADM1066_ADC_SETUP_RRCTRL,   ADM1066_MSK_CLEARLIM},
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

#if 0
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
	rc = adm1066_init_adc_iom (fd, i2c_addr);
	if (rc) {
		printf("Error: adm1066_init_adc_iom failed. i2c=0x%X, rc=%d\n",
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
