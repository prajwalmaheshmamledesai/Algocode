 /*********************************************************************
 *
 *      File:   adm1066_lib.h
 *      Name:   Sudharshan Kadari
 *
 *      Description:
 *       adm1066 - Header File 
 *
 *
 * Copyright (c) 1985-2007 by Cisco Systems, Inc.
 * All rights reserved.
 *
 *********************************************************************/

#ifndef _ADM1066_LIB_H_
#define _ADM1066_LIB_H_

typedef struct _adm1066_trim_s_
{
	uint32_t	i2c_addr;
	uint32_t	dac_reg;
} adm1066_trim_t;

typedef struct _adm1066_mon_s_
{
	uint32_t	i2c_addr;
	uint32_t	adc_reg;
	int		ratio_n;
	int		ratio_d;
	uint16_t	vref;
	uint16_t	adc_max;
	int		nominal;
	int		current;
} adm1066_mon_t;

typedef struct _adm1066_channel_s_
{
	char 		*name;
	adm1066_trim_t	*trim;
	adm1066_mon_t	*mon;
} adm1066_channel_t;

extern int adm1066_adc_read (int fd, adm1066_channel_t *channel, int *val);
extern int adm1066_dac_read (int fd, adm1066_channel_t *channel, uint8_t *data);
extern int adm1066_dac_write(int fd, adm1066_channel_t *channel, uint8_t data);
extern int adm1066_rd (int fd, uint16_t i2c_addr, uint8_t offset, uint8_t *data);
extern int adm1066_wr (int fd, uint16_t i2c_addr, uint8_t offset, uint8_t *data);
extern int adm1066_channels_get (adm1066_channel_t **);
extern int adm1066_init(int fd, uint16_t i2c_addr);
extern int adm1066_init_sfd_cfg(int fd, uint16_t i2c_addr);
extern int adm1066_init_adc_lim(int fd, uint16_t i2c_addr);

extern int adm_eeprom_diff (int fd, uint8_t adm_inst, uint8_t adm_bus,
                     uint16_t adm_addr, char *filename);
extern int adm_eeprom_erase (int fd, uint8_t adm_inst, uint8_t adm_bus,
                     uint16_t adm_addr, char *filename);
extern int adm_eeprom_dump (int fd, uint8_t adm_inst, uint8_t adm_bus,
                     uint16_t adm_addr, char *filename);
extern int adm_eeprom_write (int fd, uint8_t adm_inst, uint8_t adm_bus,
                     uint16_t adm_addr, char *filename);




#endif // _ADM1066_LIB_H_
