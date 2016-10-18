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
#include "diag_main.h"
#include "adm1066_lib.h"
#include "adm1066.h"
#include "ppc_lib.h"
#include "diag_main.h"

extern int adm1066_init_by_inst (int fd, uint16_t i2c_addr, uint8_t inst);
extern int monitor_tune_channel (int fd, adm1066_channel_t *s, int tuning_step);
extern int monitor_test_channel (int fd, adm1066_channel_t *s, int thresh );
extern int adm1066_channels_get_by_inst(adm1066_channel_t **channels, int inst);
extern int adm1066_adc_read (int fd, adm1066_channel_t *channel, int *val);
extern uint8_t adm_verbose;

static
int monitor_tune_channel_all (int fd, uint8_t inst, adm1066_channel_t *s, int tuning_step )
{
	int i, rc=0, volt_max_inst = adm1066_channels_get_by_inst(&s, inst);

	for ( i = 0; !rc && (i < volt_max_inst); i++, s++ ) {
		printf("  ADM1066-%d Margin %d : %s\n", inst, tuning_step, s->name);
		rc = monitor_tune_channel(fd, s, tuning_step); 
	}
	return (rc);
}

static
int monitor_test_channel_all (int fd, uint8_t inst, adm1066_channel_t *s, int tuning_step, int thresh)
{
	int i, rc=0, volt_max_inst = adm1066_channels_get_by_inst(&s, inst);

	for ( i = 0; !rc && (i < volt_max_inst); i++, s++ ) {
		printf("  ADM1066-%d Test   %d : %s\n", inst, tuning_step, s->name);
		rc = monitor_test_channel(fd, s, thresh); 
	}
	return (rc);
}


static int
monitor_voltage_dump_all (int fd, uint8_t inst, adm1066_channel_t *s, double percent)
{
	// Get channel info.
	int volt_max_inst = adm1066_channels_get_by_inst(&s, inst);
	int rc, i, adc_val;

	printf ("\n");
	printf ("  NAME               ADC         EXP         %s\n", "%");
	printf ("  ------------------|-----------|-----------|--------\n" );

	for ( i = 0; i < volt_max_inst; i++, s++ ) {
		printf ( "  %-18s|", s->name );
		if ((rc = adm1066_adc_read(fd, s, &adc_val)) < 0 ) {
			printf ( "Voltage read failed\n" );
			continue;
		}

		printf ("%dmv\t", adc_val );
		printf ("%dmv\t", (s->mon)->nominal );
		if ( (s->mon)->nominal == 0 ) {
			printf ("NA\n" );
		} else {
			percent = ((double)((adc_val - (s->mon)->nominal) *
					100) / (double) (s->mon)->nominal);
			if ( percent >= 0 ) {
				printf ( " " );
			}
                        printf ( "%2.02f%c\n", percent, '%' );
                }
	}
	printf("\n\n");
	return (rc);
}

int adm_diag (int fd, uint8_t inst, uint8_t thresh, uint16_t adm_addr) 
{
        uint8_t  cnt, volt_rails;
        int      rc, mymarg[] = {3, 0, -3};
#if 0
        uint16_t adm_addr[] =  {VEN_I2C_ADM1066_ADDR_0, VEN_I2C_ADM1066_ADDR_1,
                                VEN_I2C_ADM1066_ADDR_2};
#endif
        adm1066_channel_t *s;

        adm_verbose = 0;
	volt_rails = adm1066_channels_get_by_inst(&s, inst);
	for (cnt = 0; cnt < volt_rails; cnt++) {
		printf("  ADM1066-%d Channel%d : %s\n", inst, cnt, s[cnt].name);
	}

	// Init ADM1066
	printf("  ADM1066-%d init\n", inst);
        rc = adm1066_init_by_inst (fd, adm_addr, inst);

	for (cnt = 0; !rc &&  (cnt < 3); cnt++) {
		// Margin the voltage
		rc = monitor_tune_channel_all (fd, inst, s, mymarg[cnt]);
		if (rc) break; 

		rc = monitor_test_channel_all (fd, inst, s, mymarg[cnt], thresh);
		if (rc)  break;

		rc = monitor_voltage_dump_all (fd, inst, s, mymarg[cnt]);
		if (rc)  break;
        }

	// Init ADM1066
	printf("  ADM1066-%d Re-init\n", inst);
        rc = adm1066_init_by_inst (fd, adm_addr, inst);

        adm_verbose = 1;
        return (rc);
}
