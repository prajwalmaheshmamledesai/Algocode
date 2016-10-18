/********************************************************************
 *
 *      File:   mv88e6061_lib.c
 *      Name:   Ross zhong 
 *
 *      Description:
 *       Monterey park library
 *
 *
 * Copyright (c) 2010-2017 by Cisco Systems, Inc.
 * All rights reserved.
 *
 *
 *********************************************************************/
#include "mp_alpine.h" 
#include "mp_i2c.h" 
#include "stdio.h" 
#define NCSI_MAC_SIZE 6

int mp_ncsi_mac_address_get( unsigned short bus, unsigned char * mac_buf)
{
        int fd, rc = 0;
	int i=0;
	unsigned short off = MP_NCSI_MAC_OFF;
	
	fd = mp_i2c_open();	
	if (fd < 0){
		printf("Error on open file /dev/pilot2_i2c_drv\n"); 
		return -1;
	}
	mp_arbiter_en(fd,bus);
	
	for( i=0; i< NCSI_MAC_SIZE; i++){
		rc = MP_I2CByteRead_16BitIndex(fd,bus,MP_FRU_I2C_ADDR,off, mac_buf);
		mac_buf++;
		off++;
	}
			
	mp_i2c_close(fd);

        return rc;
}


int mp_ncsi_package_id_get(unsigned short bus, unsigned char * pkg_id)
{
        int fd, ret = 0;
	unsigned short off = MP_CONFIG_BLOCK_OFF;
	unsigned char  magic_id0;
	
	fd = mp_i2c_open();	
	if (fd < 0){
		printf("Error on open file /dev/pilot2_i2c_drv\n"); 
		return -1;
	}
	mp_arbiter_en(fd,bus);
	ret = MP_I2CByteRead_16BitIndex(fd,bus,MP_POST_I2C_ADDR,off, &magic_id0);
	if ( MP_CONFIG_BLOCK_MAGIC_BYTE0 != magic_id0){
		printf("ERROR, Configuration block in POST EEPROM is not initialized\n"); 
		return -1;
	}
	off = MP_CONFIG_BLOCK_PKG_OFF;		
	ret = MP_I2CByteRead_16BitIndex(fd,bus,MP_POST_I2C_ADDR,off, pkg_id);
	//printf("pkg_id = %x\n", *pkg_id); 
	mp_i2c_close(fd);
        return ret;
}
