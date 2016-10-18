/********************************************************************
 *
 *      File:   max1617_lib.c 
 *      Name:   Sudharshan Kadari
 *
 *      Description:
 *       max1617 - Temp Sensor Access routines. 
 *
 *
 * Copyright (c) 1985-2007 by Cisco Systems, Inc.
 * All rights reserved.
 *
 *********************************************************************/

#include <stdio.h>
#include <sys/types.h>
#include "ppc_lib.h"
#include "max1617.h"
#include "max1617_lib.h"


int max1617_rd (int fd, uint16_t i2c_addr, uint8_t offset, uint8_t *data)
{
	return (diag_i2c_rd(fd, i2c_addr, offset, sizeof(offset), 
			(uint8_t*)data, sizeof(uint8_t)));
}

int max1617_wr (int fd, uint16_t i2c_addr, uint8_t offset, uint8_t *data)
{
	return (diag_i2c_wr(fd, i2c_addr, offset, sizeof(offset), 
			(uint8_t*)data, sizeof(uint8_t)));
}

int max1617_to_c ( int val ) 
{
    return ( val );
}

int max1617_ts_rd (int fd, uint16_t i2c_addr, int channel, uint8_t *val)
{
	int rc;

	rc = max1617_rd(fd, i2c_addr, channel, val);
	if (rc) {
		printf("Error: Failed to read temp sens 0x%X (rc=%d)\n",
			channel, rc);
	}

	return (rc);
} 

int max1617_temp_show (int fd, uint16_t i2c_addr, int8_t min, int8_t max)
{
	int8_t local, remote;
	int rc;

	rc = max1617_rd(fd, i2c_addr, MAX1617_REG_RLTS, (uint8_t*)&local);
	if (rc) {
		printf("Error: Failed to read MAX1617_REG_RLTS (rc=%d)\n", rc);
		return (rc);
	}

	rc = max1617_rd(fd, i2c_addr, MAX1617_REG_RRTE, (uint8_t*)&remote);
	if (rc) {
		printf("Error: Failed to read MAX1617_REG_RRTE (rc=%d)\n", rc);
		return (rc);
	}

	printf("\tMX1617:  LOCAL : [%d], REMOTE : [%d]\n", local, remote);

	if ((local < min) || (local > max) || (remote < min) || (remote > max))
		return (-1);

	return (rc);
}

int max1617_access_test (int fd, uint16_t i2c_addr, int8_t min, int8_t max)
{
	uint8_t mfgid, devid;
	int rc;

	rc = max1617_temp_show(fd, i2c_addr, min, max);
	if (rc) {
		printf("Error: reading max1617 temp info\n");
		return (rc);
	}

	rc = max1617_rd(fd, i2c_addr, MAX1617_REG_MFGID, &mfgid);
	if (rc) {
		printf("Error: Failed to read MAX1617_REG_MFGID (rc=%d)\n", rc);
		return (rc);
	}

	rc = max1617_rd(fd, i2c_addr, MAX1617_REG_DEVID, &devid);
	if (rc) {
		printf("Error: Failed to read MAX1617_REG_DEVID (rc=%d)\n", rc);
		return (rc);
	}
#if 0
	// This device is reading 0xFF for both these registers.
	if ((mfgid != MAX1617_MFGID) || (devid != MAX1617_DEVID)) {
		printf ("Failed: MFGID [Exp:0x%X R:0x%X], DEVID [Exp:0x%X Rd:0x%X]\n",
			MAX1617_MFGID, mfgid, MAX1617_DEVID, devid);
		return (-1);
	}
#endif
	printf("  MAX1617:  MFGID : [0x%X], DEVID[0x%X]\n", mfgid, devid);


	return (rc);
} 

typedef struct _max1617_na_s_{
	uint8_t *name;
	uint8_t  addr;
} max1617_name_addr_t;

int max1617_reg_dump (int fd, uint16_t i2c_addr)
{
	int	rc = 0, cnt, count;
	uint8_t	data;

	max1617_name_addr_t	mx_na[] = {
		{"MAX1617_RLTS",  MAX1617_REG_RLTS},
		{"MAX1617_RRTE",  MAX1617_REG_RRTE},
//		{"MAX1617_RSL",   MAX1617_REG_RSL},
		{"MAX1617_RCL",   MAX1617_REG_RCL},
		{"MAX1617_RCRA",  MAX1617_REG_RCRA},
		{"MAX1617_RLHN",  MAX1617_REG_RLHN},
		{"MAX1617_RLLI",  MAX1617_REG_RLLI},
		{"MAX1617_RRHI",  MAX1617_REG_RRHI},
		{"MAX1617_RRLS",  MAX1617_REG_RRLS},
//		{"MAX1617_WCA",   MAX1617_REG_WCA},
//		{"MAX1617_WCRW",  MAX1617_REG_WCRW},
//		{"MAX1617_WLHO",  MAX1617_REG_WLHO},
//		{"MAX1617_WLLM",  MAX1617_REG_WLLM},
//		{"MAX1617_WRHA",  MAX1617_REG_WRHA},
//		{"MAX1617_WRLN",  MAX1617_REG_WRLN},
//		{"MAX1617_OSHT",  MAX1617_REG_OSHT},
//		{"MAX1617_SPOR",  MAX1617_REG_SPOR},
		{"MAX1617_MFGID", MAX1617_REG_MFGID},
		{"MAX1617_DEVID", MAX1617_REG_DEVID},
	};
	count = sizeof(mx_na)/sizeof(max1617_name_addr_t);

	for (cnt = 0; cnt < count; cnt++) {
		rc = max1617_rd(fd, i2c_addr, mx_na[cnt].addr, &data);
		if (rc) {
			printf("Error: Failed to read %s (0x%X) (rc=%d)\n", 
				mx_na[cnt].name, mx_na[cnt].addr, rc);
			return (rc);
		}
		printf(" %s [0x%X] : 0x%X\n", mx_na[cnt].name, mx_na[cnt].addr, data);
	}
	return (rc);
}
