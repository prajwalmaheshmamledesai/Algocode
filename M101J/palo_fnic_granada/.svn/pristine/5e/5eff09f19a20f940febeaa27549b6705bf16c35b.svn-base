/********************************************************************
 *
 *      File:   ad7416_lib.c 
 *      Name:   Sudharshan Kadari
 *
 *      Description:
 *       AD7416 - Temp Sensor Access routines. 
 *
 *
 * Copyright (c) 1985-2007 by Cisco Systems, Inc.
 * All rights reserved.
 *
 *********************************************************************/

#include <stdio.h>
#include <sys/types.h>
#include "ppc_lib.h"
#include "ad7416_lib.h"
#include "diag_err.h"

int ad7416_rd (int fd, uint16_t i2c_addr, uint8_t offset, uint16_t *data)
{
	return (diag_i2c_rd(fd, i2c_addr, offset, sizeof(offset), 
			(uint8_t*)data, sizeof(uint16_t)));
}

int ad7416_wr (int fd, uint16_t i2c_addr, uint8_t offset, uint16_t *data)
{
	return (diag_i2c_wr(fd, i2c_addr, offset, sizeof(offset), 
			(uint8_t*)data, sizeof(uint16_t)));
}

void ad7416_to_c_str (uint16_t data, uint8_t *cstr)
{
	int8_t data8 = (int8_t) (data >> 8);
	sprintf(cstr, "%d.%d", data8, ((data >> 6) & 0x03) * 25);
}

int8_t ad7416_to_c (uint16_t data)
{
	return ((char)(data >> 8));
}

int ad7416_init (int fd, uint16_t i2c_addr)
{
	int      rc = 0;
	uint16_t data = 0x00;
	rc = ad7416_wr (fd, i2c_addr, AD7416_CONFIG, &data);
        if (rc) {
                printf("Error: write AD7416_CONFIG (rc=%d)\n", rc);
	}
        return (rc);
}


int8_t ad7416_temp_get (int fd, uint16_t i2c_addr)
{
	int       rc = 0;
	uint16_t  temp;

        rc = ad7416_rd(fd, i2c_addr, AD7416_TEMP_VALUE, &temp);
        if (rc) {
                printf("Error: Failed to read AD7416_TEMP_VALUE (rc=%d)\n", rc);
                return (rc);
	}

	return (ad7416_to_c(temp));
}

int ad7416_temp_show (int fd, uint16_t i2c_addr, int max, int min)
{
	int       rc = 0;
	int8_t    temp1;
	uint16_t  temp;
	uint8_t   cstr[8];

        rc = ad7416_rd(fd, i2c_addr, AD7416_TEMP_VALUE, &temp);
        if (rc) {
                printf("Error: Failed to read AD7416_TEMP_VALUE (rc=%d)\n", rc);
                return (rc);
	}

	ad7416_to_c_str (temp, cstr);
	printf("  SENSOR TEMP: %s\n", cstr);

	temp1 = ad7416_to_c(temp);
	if ((temp1 > (int8_t)max) || (temp1 < (int8_t)min))  {
		printf(" Temperature is not within the range(i2c = 0x%X)\n",
			 i2c_addr);
		printf(" Read = %d, Max = %d, Min = %d\n",
				temp1, max, min);
		return (DIAG_RANGE_VIOLATE_ERROR);
	}
	return (rc);
}

int ad7416_access_test (int fd, uint16_t i2c_addr, int max, int min)
{
	int rc;
	rc = ad7416_init(fd, i2c_addr);
        if (rc) {
                printf("Error: Failed to init AD7416 (rc=%d)\n", rc);
                return (rc);
	}

        return (ad7416_temp_show(fd, i2c_addr, max, min));
}
