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

#define ADM1066_NAME_MAX 32
#if defined(DIAG_BROCKWAY)
#include "fext.h"
static int adm0_acc = 0, adm1_acc = 0;
#endif
int adm1066_rd (int fd, uint16_t i2c_addr, uint8_t offset, uint8_t *data)
{
	int rc = 0;

	rc = diag_i2c_rd(fd, i2c_addr, offset, sizeof(offset), 
                       (uint8_t*)data, sizeof(uint8_t));
#if defined(DIAG_BROCKWAY)
        if (rc != 0) {
            if (((i2c_addr == BROCKWAY_ADM1066_I2C_ADDR0) && (adm0_acc == 0)) ||
                ((i2c_addr == BROCKWAY_ADM1066_I2C_ADDR1) && (adm1_acc == 0))) {
               usleep(1000);
               printf("ADM1066 I2C Retry in adm1066_rd()\n");
               printf("I2C address %x Reg addr %x\n", i2c_addr, offset);
	       rc = diag_i2c_rd(fd, i2c_addr, offset, sizeof(offset), 
                       (uint8_t*)data, sizeof(uint8_t));
               if (i2c_addr == BROCKWAY_ADM1066_I2C_ADDR0) {
                   adm0_acc = 1;
               } else if (i2c_addr == BROCKWAY_ADM1066_I2C_ADDR1) {
                   adm1_acc = 1;
               }
            }
        }
#endif
	if (!rc) return (rc);
	usleep(1000);
	return (rc);
}

int adm1066_wr (int fd, uint16_t i2c_addr, uint8_t offset, uint8_t *data)
{
	int rc = 0;
	
	rc = diag_i2c_wr(fd, i2c_addr, offset, sizeof(offset), 
		(uint8_t*)data, sizeof(uint8_t));
	if (!rc) return (rc);
	usleep(1000);
	return (rc);
}

int
adm1066_adc_read (int fd, adm1066_channel_t *channel, int *val)
{
    uint8_t  data = 0;
    int rc = 0, cnt = 0;
    uint32_t i2c_addr = (channel->mon)->i2c_addr;

    *val = 0x00;
    while (!(*val & 0xFF00) && (cnt++ < 100))
    {
        rc = adm1066_rd (fd, i2c_addr,  (channel->mon)->adc_reg, &data);
        if (rc) {
            printf ( "rc = %d\n", rc );
            return ( rc );
        }

        *val =  data << 8;

        rc = adm1066_rd (fd, i2c_addr,  (channel->mon)->adc_reg+1, &data);
        if (rc) {
            printf ( "rc = %d\n", rc );
            return ( rc );
        }
    	*val |= data;
	if (!(*val & 0xFF00)) {
		printf("  Retry ADC Read %d\n", cnt);
		usleep(10000);
	}
    }

    *val *= (channel->mon)->vref;
    *val /= (channel->mon)->adc_max;
    *val *= (channel->mon)->ratio_d;
    *val /= (channel->mon)->ratio_n;

    return (rc);
}

int
adm1066_dac_read (int fd, adm1066_channel_t *channel, uint8_t *data)
{
    int rc = 0;

    rc = adm1066_rd (fd, channel->trim->i2c_addr,  channel->trim->dac_reg, data);
    if (rc) {
        printf ( "%s:%d i2c:0x%X addr:0x%X rc = %d\n", 
		__func__, __LINE__, channel->trim->i2c_addr,
		channel->trim->dac_reg, rc );
    }
    return (rc);
}

int
adm1066_dac_write (int fd, adm1066_channel_t *channel, uint8_t data )
{
    int rc = 0;

    rc = adm1066_wr (fd, channel->trim->i2c_addr,  
		channel->trim->dac_reg, &data);
    if (rc) {
        printf ( "%s:%d i2c:0x%X addr:0x%X rc = %d\n", 
		__func__, __LINE__, channel->trim->i2c_addr,
		channel->trim->dac_reg, rc );
    }
    return (rc);
}

int adm1066_init_sfd_cfg(int fd, uint16_t i2c_addr)
{
	int	cnt, count, rc = 0;
	uint8_t	data;

	// Read Modify Write
	uint8_t addr_rmw_data[][2] = {
		{ADM1066_VP1_SFDV1CFG,  ADM1066_VP_SFDCFG_FLD_FAULT_SEL_OFF},
		{ADM1066_VP2_SFDV2CFG,  ADM1066_VP_SFDCFG_FLD_FAULT_SEL_OFF},
		{ADM1066_VP3_SFDV3CFG,  ADM1066_VP_SFDCFG_FLD_FAULT_SEL_OFF},
		{ADM1066_VP4_SFDV4CFG,  ADM1066_VP_SFDCFG_FLD_FAULT_SEL_OFF},
		{ADM1066_VH_PSVHDVHCFG, ADM1066_VP_SFDCFG_FLD_FAULT_SEL_OFF},
		{ADM1066_VX1_SFDX1CFG,  ADM1066_VP_SFDCFG_FLD_FAULT_SEL_OFF},
		{ADM1066_VX2_SFDX2CFG,  ADM1066_VP_SFDCFG_FLD_FAULT_SEL_OFF},
		{ADM1066_VX3_SFDX3CFG,  ADM1066_VP_SFDCFG_FLD_FAULT_SEL_OFF},
		{ADM1066_VX4_SFDX4CFG,  ADM1066_VP_SFDCFG_FLD_FAULT_SEL_OFF},
		{ADM1066_VX5_SFDX5CFG,  ADM1066_VP_SFDCFG_FLD_FAULT_SEL_OFF},
	};

	count = sizeof(addr_rmw_data)/(2 * sizeof(uint8_t));
	for (cnt = 0; cnt < count; cnt++) {
    		rc = adm1066_rd (fd, i2c_addr, addr_rmw_data[cnt][0], &data);
		if (rc) {
			printf("Error: %s:%d Rd i2c=0x%X, addr=0x%X\n",
				__func__, __LINE__, i2c_addr, addr_rmw_data[cnt][0]);
			return (rc);
		}
		data |= addr_rmw_data[cnt][1];

    		rc = adm1066_wr (fd, i2c_addr, addr_rmw_data[cnt][0], &data);
		if (rc) {
			printf("Error: %s:%d Wr i2c=0x%X, addr=0x%X, data=0x%X\n",
			__func__, __LINE__, i2c_addr, addr_rmw_data[cnt][0], data);
			return (rc);
		}
	}
	return (rc);
}


int adm1066_init_adc_lim(int fd, uint16_t i2c_addr)
{
	int	cnt, count, rc = 0;
	uint8_t	data;
	// Write
	uint8_t addr_data[][2] = {
#if 0
		{ADM1066_ADC_LIM_ADCVP1LIM,	ADM1066_ADC_LIM_CONVERSION_OFF},
		{ADM1066_ADC_LIM_ADCVP2LIM,	ADM1066_ADC_LIM_CONVERSION_OFF},
		{ADM1066_ADC_LIM_ADCVP3LIM,	ADM1066_ADC_LIM_CONVERSION_OFF},
		{ADM1066_ADC_LIM_ADCVP4LIM,	ADM1066_ADC_LIM_CONVERSION_OFF},
		{ADM1066_ADC_LIM_ADCVHLIM,	ADM1066_ADC_LIM_CONVERSION_OFF},
		{ADM1066_ADC_LIM_ADCVX1LIM,	ADM1066_ADC_LIM_CONVERSION_OFF},
		{ADM1066_ADC_LIM_ADCVX2LIM,	ADM1066_ADC_LIM_CONVERSION_OFF},
		{ADM1066_ADC_LIM_ADCVX3LIM,	ADM1066_ADC_LIM_CONVERSION_OFF},
		{ADM1066_ADC_LIM_ADCVX4LIM,	ADM1066_ADC_LIM_CONVERSION_OFF},
		{ADM1066_ADC_LIM_ADCVX5LIM,	ADM1066_ADC_LIM_CONVERSION_OFF},
		{ADM1066_ADC_LIM_ADCITLIM,	ADM1066_ADC_LIM_CONVERSION_OFF},
		{ADM1066_ADC_LIM_ADCXTS1LIM,	ADM1066_ADC_LIM_CONVERSION_OFF},
		{ADM1066_ADC_LIM_ADCXTS2LIM,	ADM1066_ADC_LIM_CONVERSION_OFF},
#endif
		{ADM1066_ADC_LIM_LSENSE1,	ADM1066_LSENS_WARN_ADC_LT_LIMIT},
#if 0
		{ADM1066_ADC_LIM_LSENSE2,	ADM1066_LSENS_WARN_EXTS2 |
						ADM1066_LSENS_WARN_EXTS1 |
						ADM1066_LSENS_WARN_INTS  |
						ADM1066_LSENS_WARN_VX5   |
						ADM1066_LSENS_WARN_VX4}
#endif
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

int monitor_tune_channel (int fd, adm1066_channel_t *s, int tuning_step )
{
	int rc		= 0;
	int to 		= 1000;
	int diff 	= 0;
	int alpha 	= 5;
	int clip 	= 0xf;
	int adc_val 	= 0;
	int adc_prev 	= 0;
	int thresh 	= 0;
	int dac_val 	= 0;
	int vtarget 	= 0;
	int adc_filter 	= 0; 
	int adc_filter_to = 100;
	uint8_t data;

	if (s->trim == NULL)
		return (0);

	printf ( "s->name, %s\n", s->name );

	/* adjust dac value */
	vtarget = (s->mon)->nominal + ( (s->mon)->nominal * tuning_step / 100 );

	if (( rc = adm1066_dac_read (fd, s, &data) )) {
		printf ( " ERROR: Could not read dac value, %d\n", dac_val );
		return ( rc );
	}
	dac_val = (int)data;

	to = 100;
	do {
		adc_filter_to	= 100;
		adc_filter	= 0;
		adc_prev	= 0;

		while ( --adc_filter_to ) {
			if ((rc = adm1066_adc_read(fd, s, &adc_val))) {
				printf ( "ERROR: %s:%d could not read adc value\n",
						__func__, __LINE__);
				return (rc);
			}

			if ((adc_val <= ( adc_prev + 5 ) ) &&
			    (adc_val >= ( adc_prev - 5 ) ) ) {
				if ( ++adc_filter >= 2 )
					break;
			} else {
				adc_filter = 0;
			}
			adc_prev = adc_val;
		}

		if ( vtarget != adc_val ) {
			diff   = adc_val - vtarget;
			thresh = (thresh == 0) ? 
				1 : ((s->mon)->nominal/1000);
			if ((diff <= thresh ) && 
			    (diff >= -thresh ) ) {
				break;
			}

			if ( diff < 0 ) {
				diff /= alpha;
				if (diff < -clip) {
					diff = -clip;
				}
				dac_val += ((diff == 0) ? -1 : diff );
			} else {
				diff /= alpha;
				if ( diff > clip ) {
				diff = clip;
				}
				dac_val += ( ( diff == 0 ) ? 1 : diff );
			}

			if ( dac_val >= 0xff ) {
				dac_val = 0xfe;
			} else if ( dac_val <= 0 ) {
				dac_val = 0x01;
			}

			printf(	"vtarget: %d, adc_val: %d,"
				" diff: %d, dac_val: %d\n",
				vtarget, adc_val, diff, dac_val );

			if ((rc = adm1066_dac_write(fd, s, dac_val))) {
				return ( rc );
			}
                        sleep(1);
		} else {
			break;
		}
	} while ( --to && ( dac_val < 0xfe ) && ( dac_val > 0x01 ) );

	return ( 0 );
}

int
monitor_test_channel (int fd, adm1066_channel_t *s, int thresh )
{
	int adc_val = 0, rc = 0;

	if ((rc = adm1066_adc_read (fd, s, &adc_val))) {
		printf ( "ERROR: could not read adc value\n" );
		return ( rc );
	} 

	if ( adc_val > ((s->mon)->current+((s->mon)->current*thresh/100)) ) {
		printf ("ERROR: %s out of range, exp -> %dmV, read -> %dmV\n", 
			s->name,(s->mon)->current, adc_val );
		return ( -1 );
	} else if ( adc_val < ((s->mon)->current-((s->mon)->current*thresh/100)) ) {
		printf ("ERROR: %s out of range, exp -> %dmV, read -> %dmV\n", 
			s->name,(s->mon)->current, adc_val );
		return ( -1 );
	}

	return ( 0 );
}


int monitor_ui(int argc, char **argv, int fd)
{
	int	tuning_step, i, len, rc = 0, all_flg=0;
	int	test = 0, volt_max_inst = 0, adc_val;
	char	name[ADM1066_NAME_MAX];
	double	percent = 0;
        uint32_t save_i2c_addr = 0;

	adm1066_channel_t *s;

	if ( argc < 2 ) {
		return (-1);
	}

	// Get channel info.
	volt_max_inst = adm1066_channels_get(&s);

	// test/init/info
	argc--;
	argv++;

	len = strlen ( *argv );
	if (strncmp( *argv, "test", len) == 0 ) {
		test = 1;
	} else if ( strncmp ( *argv, "init", len ) == 0 ) {
	    for ( i = 0; i < volt_max_inst; i++, s++ ) {
                if (save_i2c_addr != s->mon->i2c_addr) {    
                    save_i2c_addr = s->mon->i2c_addr;
                    if ((rc = adm1066_init(fd, s->mon->i2c_addr))) {
                        printf("Error: Failed adm1066 init (rc=%d)\n", rc);
                        return (rc);
                    }
                }
            }
            return (rc);
	} else  if ( strncmp ( *argv, "info", len ) == 0 ) {
		printf("Channal Info\n");
		for ( i = 0; i < volt_max_inst; i++, s++ ) {
			printf("Channel%d : %s\n", i, s->name); 
		}
		return (rc);
	} 

   
	// Could be test or margin here. 
	argc--; 
	argv++;
	if ( argc == 0 ) {
		return (-1);
	}

	// Get the name of channel or all
	if (strncpy ( name, *argv, ADM1066_NAME_MAX) == NULL ) {
		fprintf ( stderr, "ERROR: Invalid Arg, %s\n", *argv );
		rc = 1;
		return (rc);
	}

	all_flg = (strcmp(name, "all") == 0) ? 1 : 0;

	argc--;
	argv++;

	// no more arguments <margin all/name> <test all/name>
	if ( argc == 0 ) { 
		printf ( "NAME\t\tADC\tEXP\t%s\n", test? "STATUS":"%");
		printf ( "---------------|-------|-------|--------\n" );

		for ( i = 0; i < volt_max_inst; i++, s++ ) {
			if ((all_flg) || (strcmp ( name, s->name) == 0)) {
				printf ( "%s:\t", s->name );

				if (!( strlen( s->name)/8) ) {
					printf ( "\t");
				}

				if ((rc = adm1066_adc_read(fd, s, &adc_val)) < 0 ) {
					printf ( "Voltage read failed\n" );
					continue;
				}

				printf ("%dmv\t", adc_val );
				printf ("%dmv\t", (s->mon)->nominal );
				if ( (s->mon)->nominal == 0 ) {
					printf ("NA\n" );
				} else if ( test ) {
					printf ("%s", (monitor_test_channel(fd, s, 10)) ?
						"FAIL\n": "PASS\n" );
				} else {
					percent = ((double)((adc_val - (s->mon)->nominal) *
						100) / (double) (s->mon)->nominal);
					if ( percent >= 0 ) {
						printf ( " " );
					}
						printf ( "%2.02f%c\n", percent, '%' );
				}
			}
		}
	} else if ( argc == 1 ) { 
		// Get the tuning step.
		if ( sscanf ( *argv, "%d", &tuning_step ) != 1 ) {
			printf ( "ERROR: Invalid Arg, %s\n", *argv );
			return(1);
		}
		for ( i = 0; i < volt_max_inst; i++, s++ ) {
			if (all_flg || (strcmp( name, s->name) == 0)) {
				monitor_tune_channel(fd, s, tuning_step );
			}
		}
	} else {
		rc = -1;
	}

	return ( rc );
}
