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
#include <stdint.h>
#include <sys/types.h>
#include "ad7416_marin.h"
#include "diag_err.h"


extern int I2CByteWrite_8BitIndex(int Fh, unsigned char Bus, unsigned char Dev,
		                unsigned char Reg, unsigned char Val, int send, int verbose);
extern int I2CByteRead_NoIndex(int Fh, unsigned char Bus, unsigned char Dev,
		                unsigned char *data, int rs, int verbose);
extern int I2CByteRead_Block(int Fh, unsigned char Bus, unsigned char Dev,
		                unsigned char Reg, unsigned char *rxBuf, int bytes, int verbose);
extern int I2CByteWrite_BLock(int Fh, unsigned char Bus, unsigned char Dev,
		                unsigned char *txBuf, int bytes, int verbose);

int ad7416_rd (int fd, uint16_t i2c_addr, uint8_t offset, uint8_t *data, int rs)
{
	int rc=0;
	/*rc = I2CByteWrite_8BitIndex(fd, 1, i2c_addr, offset, 0, 1, 0);
	rc = I2CByteRead_NoIndex(fd, 1, i2c_addr, data, 2, 0);
	return rc;*/
	return I2CByteRead_Block(fd, 1, i2c_addr, offset, data, rs, 0);
}

int ad7416_wr (int fd, uint16_t i2c_addr, uint8_t offset, uint8_t *data, int ws)
{
	uint8_t wdata[4];
	wdata[0] = offset;
	memcpy(wdata+1, data, ws);
	return I2CByteWrite_Block(fd, 1, i2c_addr, &wdata, 1+ws, 0);
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
	uint8_t data = 0;
	rc = ad7416_wr (fd, i2c_addr, AD7416_CONFIG, &data, 1);
        if (rc) {
                printf("Error: write AD7416_CONFIG (rc=%d)\n", rc);
	}
        return (rc);
}


int ad7416_temp_raw (int fd, uint16_t i2c_addr, uint16_t *temp)
{
	int rc = 0;
	uint8_t data[2];

        rc = ad7416_rd(fd, i2c_addr, AD7416_TEMP_VALUE, data, 2);
        if (rc) {
                printf("Error: Failed to read AD7416_TEMP_VALUE (rc=%d)\n", rc);
                return (rc);
	}
	
	*temp = (data[0]<<8) + data[1];
	return rc;
}

int ad7416_temp_get(int fd, uint16_t i2c_addr, int *temp) {
	uint16_t raw;
	int rc=0;
	rc = ad7416_temp_raw(fd, i2c_addr, &raw);
	if( raw & 0x8000 ) // negative temp
		*temp = ( (raw>>6) - 512) / 4;
	else 
		*temp = (raw>>6)/4;
		
	return rc;
}

int ad7416_temp_show (int fd, uint16_t i2c_addr, int max, int min)
{
	int       rc = 0;
	int8_t    temp1;
	uint16_t  temp;
	uint8_t   cstr[8];

        rc = ad7416_temp_raw(fd, i2c_addr, &temp);
        if (rc) {
                printf("Error: Failed to read AD7416_TEMP_VALUE (rc=%d)\n", rc);
                return (rc);
	}
	
	printf("temp reg = 0x%X\n", temp);
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
