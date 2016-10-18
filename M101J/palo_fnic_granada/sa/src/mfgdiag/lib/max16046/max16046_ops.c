 /*********************************************************************
 *
 *      File:   max16046_lib.c
 *      Name:
 *
 *      Description:
 *       max16046_lib - Access Library 
 *
 *
 * Copyright (c) 1985-2007 by Cisco Systems, Inc.
 * All rights reserved.
 *
 *********************************************************************/
#include "diag_main.h"
#include "max16046_lib.h"
#include "ppc_lib.h"
#include "diag_main.h"

extern int max16046_init_by_inst (int fd, uint8_t inst);
extern int max16046_margin_channel (int fd, max16046_channel_t *s, int tuning_step);
extern int max16046_test_channel (int fd, max16046_channel_t *s, int thresh );
extern int max16046_channels_get_by_inst(max16046_channel_t **channels, int inst);
extern int max16046_adc_read (int fd, max16046_channel_t *channel, int *val);

static
int monitor_tune_channel_all (int fd, uint8_t inst, max16046_channel_t *s, int tuning_step )
{
	int i, rc=0, volt_max_inst = max16046_channels_get_by_inst(&s, inst);

	for ( i = 0; !rc && (i < volt_max_inst); i++, s++ ) {
		printf("  max16046-%d Margin %d : %s\n", inst, tuning_step, s->name);
		rc = max16046_margin_channel(fd, s, tuning_step); 
	}
	return (rc);
}

static
int monitor_test_channel_all (int fd, uint8_t inst, max16046_channel_t *s, int tuning_step, int thresh)
{
	int i, rc=0, volt_max_inst = max16046_channels_get_by_inst(&s, inst);

	for ( i = 0; !rc && (i < volt_max_inst); i++, s++ ) {
		printf("  max16046-%d Test   %d : %s\n", inst, tuning_step, s->name);
		rc = max16046_test_channel(fd, s, thresh); 
	}
	return (rc);
}


static int
monitor_voltage_dump_all (int fd, uint8_t inst, max16046_channel_t *s, double percent)
{
	// Get channel info.
	int volt_max_inst = max16046_channels_get_by_inst(&s, inst);
	int rc, i, adc_val;

	printf ("\n");
	printf ("  NAME               ADC         EXP         %s\n", "%");
	printf ("  ------------------|-----------|-----------|--------\n" );

	for ( i = 0; i < volt_max_inst; i++, s++ ) {
		printf ( "  %-18s|", s->name );
		if ((rc = max16046_adc_read(fd, s, &adc_val)) < 0 ) {
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

int max16046_diag (int fd, uint8_t inst, uint8_t thresh, uint16_t i2c_addr) 
{
        uint8_t  cnt, volt_rails;
        int      rc, mymarg[] = {1, 0, -1}; //1: up     0: nominal    -1: down
        
	max16046_channel_t *s;

	/*
	volt_rails = max16046_channels_get_by_inst(&s, inst);
	for (cnt = 0; cnt < volt_rails; cnt++) {
		printf("  max16046-%d Channel%d : %s\n", inst, cnt, s[cnt].name);
	}*/

	// Init max16046
	printf("  max16046-%d init\n", inst);
        rc = max16046_init_by_inst(fd, inst);

	for (cnt = 0; !rc &&  (cnt < 3); cnt++) {
		// Margin the voltage
		rc = monitor_tune_channel_all (fd, inst, s, mymarg[cnt]);
		if (rc) break; 

		rc = monitor_test_channel_all (fd, inst, s, mymarg[cnt], thresh);
		if (rc)  break;

		rc = monitor_voltage_dump_all (fd, inst, s, mymarg[cnt]);
		if (rc)  break;
        }

	// Init max16046
	printf("  max16046-%d Re-init\n", inst);
        rc = max16046_init_by_inst (fd, inst);

        return (rc);
}
