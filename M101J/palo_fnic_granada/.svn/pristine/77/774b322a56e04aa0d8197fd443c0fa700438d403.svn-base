/********************************************************************
 *
 *      File:   max1668_lib.c 
 *      Name:   Sudharshan Kadari
 *
 *      Description:
 *       max1668 - Temp Sensor Access routines. 
 *
 *
 * Copyright (c) 1985-2007 by Cisco Systems, Inc.
 * All rights reserved.
 *
 *********************************************************************/

#include <stdio.h>
#include <sys/types.h>
#include "ppc_lib.h"
#include "max1668.h"
#include "max1668_lib.h"


int max1668_rd (int fd, uint16_t i2c_addr, uint8_t offset, uint8_t *data)
{
	return (diag_i2c_rd(fd, i2c_addr, offset, sizeof(offset), 
			(uint8_t*)data, sizeof(uint8_t)));
}

int max1668_wr (int fd, uint16_t i2c_addr, uint8_t offset, uint8_t *data)
{
	return (diag_i2c_wr(fd, i2c_addr, offset, sizeof(offset), 
			(uint8_t*)data, sizeof(uint8_t)));
}

int max1668_to_c ( int val ) 
{
    return ( val );
}

int max1668_ts_rd (int fd, uint16_t i2c_addr, int channel, uint8_t *val)
{
	int rc;

	rc = max1668_rd(fd, i2c_addr, channel, val);
	if (rc) {
		printf("Error: Failed to read temp sens 0x%X (rc=%d)\n",
			channel, rc);
	}

	return (rc);
} 

extern int diag_get_ts_name (uint8_t i2c_addr, uint8_t chan, 
			     uint8_t *name, uint8_t nsz);

int max1668_temp_show (int fd, uint16_t i2c_addr, uint8_t mask,
			int8_t *max, int8_t *min)
{
	int8_t  temp_s; 
	uint8_t errmask = 0, ts_name[20] ;
	int     rc = 0, cnt;

	for (cnt = 0; cnt < 5; cnt++) {
		if (!(mask & (0x01 << cnt)))
			continue;

		rc = max1668_rd(fd, i2c_addr, MAX1668_REG_RIT+cnt, (uint8_t*)&temp_s);
		if (rc) {
			printf("Error: Failed to read Addr=%d (rc=%d)\n", 
				MAX1668_REG_RIT+cnt, rc);
			return (rc);
		}

		diag_get_ts_name(i2c_addr, cnt, ts_name, sizeof(ts_name));
		if (cnt) {
			printf("\tMAX1668: %s RMT%d (0x%X) [%d - %d]: [%d]\n", 
			ts_name, 
			cnt-1, i2c_addr, min[cnt], max[cnt], temp_s);
		} else {
			printf("\tMAX1668: %s LCL  (0x%X) [%d - %d] : [%d]\n", 
			ts_name, 
			i2c_addr, min[cnt], max[cnt], temp_s);
		}

		if ((temp_s < min[cnt]) || (temp_s > max[cnt]))
			errmask |= (0x01 << cnt);
	}

	return (errmask? errmask : rc);
}

int max1668_temp_get (int fd, uint16_t i2c_addr, uint8_t rmt, uint8_t *temp_s)
{
	int rc;

	rc = max1668_rd(fd, i2c_addr, MAX1668_REG_RIT+rmt, temp_s);
	if (rc) {
		printf("Error: Failed to read Addr=%d (rc=%d)\n", 
			MAX1668_REG_RIT+rmt, rc);
		return (rc);
	}
	return (rc);
}

int max1668_access_test (int fd, uint16_t i2c_addr, uint8_t valid_mask,
		int8_t *max, int8_t *min)
{
	uint8_t mfgid, devid;
	int rc;

	rc = max1668_temp_show(fd, i2c_addr, valid_mask, max, min);
	if (rc) {
		printf("Error: reading max1668 temp info\n");
		return (rc);
	}

	rc = max1668_rd(fd, i2c_addr, MAX1668_REG_MFGID, &mfgid);
	if (rc) {
		printf("Error: Failed to read MAX1668_REG_MFGID (rc=%d)\n", rc);
		return (rc);
	}

	rc = max1668_rd(fd, i2c_addr, MAX1668_REG_DEVID, &devid);
	if (rc) {
		printf("Error: Failed to read MAX1668_REG_DEVID (rc=%d)\n", rc);
		return (rc);
	}

	// This device is reading 0xFF for both these registers.
	if ((mfgid != MAX1668_MFGID) || (devid != MAX1668_DEVID)) {
		printf ("Failed: MFGID [Exp:0x%X R:0x%X], DEVID [Exp:0x%X Rd:0x%X]\n",
			MAX1668_MFGID, mfgid, MAX1668_DEVID, devid);
		return (-1);
	}
	printf("  MAX1668:  MFGID : [0x%X], DEVID[0x%X]\n", mfgid, devid);


	return (rc);
} 

typedef struct _max1668_na_s_{
	uint8_t *name;
	uint8_t  addr;
} max1668_name_addr_t;

int max1668_reg_dump (int fd, uint16_t i2c_addr)
{
	int	rc = 0, cnt, count;
	uint8_t	data;

	max1668_name_addr_t	mx_na[] = {
            {"MAX1668_REG_RIT",   MAX1668_REG_RIT},
            {"MAX1668_REG_RET1",  MAX1668_REG_RET1},
            {"MAX1668_REG_RET2",  MAX1668_REG_RET2},
            {"MAX1668_REG_RET3",  MAX1668_REG_RET3},
            {"MAX1668_REG_RET4",  MAX1668_REG_RET4},
            {"MAX1668_REG_RS1",   MAX1668_REG_RS1},
            {"MAX1668_REG_RS2",   MAX1668_REG_RS2},
            {"MAX1668_REG_RC",    MAX1668_REG_RC},
            {"MAX1668_REG_RIHL",  MAX1668_REG_RIHL},
            {"MAX1668_REG_RILL",  MAX1668_REG_RILL},
            {"MAX1668_REG_REHL1", MAX1668_REG_REHL1},
            {"MAX1668_REG_RELL1", MAX1668_REG_RELL1},
            {"MAX1668_REG_REHL2", MAX1668_REG_REHL2},
            {"MAX1668_REG_RELL2", MAX1668_REG_RELL2},
            {"MAX1668_REG_REHL3", MAX1668_REG_REHL3},
            {"MAX1668_REG_RELL3", MAX1668_REG_RELL3},
            {"MAX1668_REG_REHL4", MAX1668_REG_REHL4},
            {"MAX1668_REG_RELL4", MAX1668_REG_RELL4},
            {"MAX1668_REG_WC",    MAX1668_REG_WC},
            {"MAX1668_REG_WIHL",  MAX1668_REG_WIHL},
            {"MAX1668_REG_WILL",  MAX1668_REG_WILL},
            {"MAX1668_REG_WEHI1", MAX1668_REG_WEHI1},
            {"MAX1668_REG_WELL1", MAX1668_REG_WELL1},
            {"MAX1668_REG_WEHI2", MAX1668_REG_WEHI2},
            {"MAX1668_REG_WELL2", MAX1668_REG_WELL2},
            {"MAX1668_REG_WEHI3", MAX1668_REG_WEHI3},
            {"MAX1668_REG_WELL3", MAX1668_REG_WELL3},
            {"MAX1668_REG_WEHI4", MAX1668_REG_WEHI4},
            {"MAX1668_REG_WELL4", MAX1668_REG_WELL4},
            {"MAX1668_REG_MFGID", MAX1668_REG_MFGID},
            {"MAX1668_REG_DEVID", MAX1668_REG_DEVID},
	};
	count = sizeof(mx_na)/sizeof(max1668_name_addr_t);

	for (cnt = 0; cnt < count; cnt++) {
		rc = max1668_rd(fd, i2c_addr, mx_na[cnt].addr, &data);
		if (rc) {
			printf("Error: Failed to read %s (0x%X) (rc=%d)\n", 
				mx_na[cnt].name, mx_na[cnt].addr, rc);
			return (rc);
		}
		printf(" %s [0x%X] : 0x%X\n", mx_na[cnt].name, mx_na[cnt].addr, data);
	}
	return (rc);
}
