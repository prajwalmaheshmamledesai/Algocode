/********************************************************************
 *
 *      File:   w83793_lib.c 
 *      Name:   Sudharshan Kadari
 *
 *      Description:
 *       w83793_lib.c - H/w Monitor Library. 
 *
 *
 * Copyright (c) 1985-2007 by Cisco Systems, Inc.
 * All rights reserved.
 *
 *********************************************************************/

#include <stdio.h>
#include <stdint.h>
#include "w83793.h"

extern int hwmon_rd(int fd, uint8_t i2c_bus, uint16_t i2c_addr, uint8_t offset, uint8_t *val);
extern int hwmon_wr(int fd, uint8_t i2c_bus, uint16_t i2c_addr, uint8_t offset, uint8_t val);

uint32_t  hwmon_fan_value_to_rpm(uint16_t value)
{
	return (((value >= 0xFFF) || (!value)) ? 0 : (1350000ul/value));
}

uint32_t  hwmon_fan_rpm_to_value(uint32_t rpm)
{
	return ((1350000 + (rpm>>1))/rpm);
}

uint32_t  hwmon_value_to_time(uint8_t value)
{
	return (value*100);
}

// Value should be between 0x00 & 0xFF
uint32_t  hwmon_time_to_value(uint32_t vtime)
{
	return (((vtime+50)/100)&0xFF);
}

uint32_t hwmon_fan_tach_rd (int fd, uint8_t i2c_bus, uint32_t i2c_addr, 
			    uint8_t fan_no, uint16_t *tach_val)
{
	int rc = 0;
	uint8_t reg_addr, reg_data, count=3;

	do {
	*tach_val = 0x00;
	if (fan_no >= 12) {
		printf("  Error: Invalid Fan num %d (allowed 0-11)\n", fan_no);
		return (-1);
	}
	reg_addr = W83793_BANK0_FAN1_COUNT_HI + (fan_no<<1);
	rc = hwmon_rd(fd, i2c_bus, i2c_addr, reg_addr, &reg_data);
	if (rc) return (rc);

	*tach_val = (reg_data << 8);
	rc = hwmon_rd(fd, i2c_bus, i2c_addr, reg_addr+1, &reg_data);
	if (rc) return (rc);

	*tach_val |= reg_data;

	if (*tach_val) break;
	} while (--count);
	return (rc);
}

uint32_t hwmon_fan_show (int fd, uint8_t i2c_bus, uint32_t i2c_addr, uint8_t fan_no)
{
	int rc = 0;
	uint16_t tach_rd = 0;

	rc = hwmon_fan_tach_rd (fd, i2c_bus, i2c_addr, fan_no, &tach_rd);
	if (rc) return (rc);
	printf("  Fan%d RPM=%d\n", fan_no, hwmon_fan_value_to_rpm(tach_rd));
	return (rc);
}

uint32_t hwmon_fan_test (int fd, uint8_t i2c_bus, uint32_t i2c_addr, uint8_t fan_no,
                        uint32_t min, uint32_t max)
{
        int rc = 0, rpm;
        uint16_t tach_rd = 0;


        rc = hwmon_fan_tach_rd (fd, i2c_bus, i2c_addr, fan_no, &tach_rd);
        if (rc) return (rc);

        rpm = hwmon_fan_value_to_rpm(tach_rd);

        printf("  Fan%d RPM=%d [Min=%d Max=%d]\n", fan_no, rpm, min, max);
        if ((rpm < min) || (rpm > max)) {
                return (-1);
        }

        return (rc);
}

uint32_t hwmon_fan_init (int fd, uint8_t i2c_bus, uint32_t i2c_addr, uint8_t fan_no)
{
	int rc  = 0;

	rc = hwmon_wr(fd, i2c_bus, i2c_addr, W83793_REG_BANKSEL, 0x00);
	if (rc) return (rc);

	rc = hwmon_wr(fd, i2c_bus, i2c_addr, W83793_BANK0_MFC, 0x28);
	if (rc) return (rc);

	rc = hwmon_wr(fd, i2c_bus, i2c_addr, W83793_BANK0_FANIN_CTRL, 0x7F);
	if (rc) return (rc);

	return (rc);
}

uint32_t hwmon_fan_spd_set (int fd, uint8_t i2c_bus, uint32_t i2c_addr, 
		uint8_t fan_duty_no, uint8_t percent)
{
	uint8_t	data = (0x3F * percent)/100;
	uint8_t addr = W83793_BANK0_FAN_DUTY_1 + fan_duty_no;
	return (hwmon_wr(fd, i2c_bus, i2c_addr, addr, data));
}

uint32_t hwmon_fan_speed_test (int fd, uint8_t i2c_bus, uint32_t i2c_addr, 
		uint8_t fan_no, uint32_t exp_speed, uint8_t percent_tolerance)
{
	int rc = 0;
	uint16_t tach_rd = 0, rpm, rpm_min, rpm_max;

	rc = hwmon_fan_tach_rd (fd, i2c_bus, i2c_addr, fan_no, &tach_rd);
	if (rc) return (rc);

	rpm = hwmon_fan_value_to_rpm(tach_rd);

	rpm_min = (exp_speed * (100-percent_tolerance))/100;	
	rpm_max = (exp_speed * (100+percent_tolerance))/100;	

	printf("  FAN%d RPM : %d [Min=%d Max=%d]\n", fan_no, rpm, 
			rpm_min, rpm_max);

	if ((rpm < rpm_min) || (rpm > rpm_max)) {
		return (-1);
	}
	return (rc);
}

uint32_t hwmon_temp_rd (int fd, uint8_t i2c_bus, uint32_t i2c_addr, 
			    uint8_t td_no, uint16_t *temp_val)
{
	int rc = 0;
	uint8_t reg_data, lo_data, reg_addr = W83793_BANK0_TD1_READ + td_no;

	*temp_val = 0x00;
	if (td_no >= 4 ) {
		printf("  Error: Invalid TD num %d (allowed 0-5)\n", td_no);
		return (-1);
	}

	rc = hwmon_rd(fd, i2c_bus, i2c_addr, reg_addr, &reg_data);
	if (rc) return (rc);
	*temp_val = (reg_data << 2);

	rc = hwmon_rd(fd, i2c_bus, i2c_addr, W83793_BANK0_TEMP_LOW_BITS, &lo_data);
	if (rc) return (rc);
	lo_data = (lo_data >> (2*td_no))&0x3;

	*temp_val |= lo_data; 
	return (rc);
}

uint32_t hwmon_temp_show (int fd, uint8_t i2c_bus, uint32_t i2c_addr, uint8_t td_no)
{
	int rc = 0;
	uint8_t  hi, lo;
	uint16_t temp_val = 0;

	rc = hwmon_temp_rd (fd, i2c_bus, i2c_addr, td_no, &temp_val);
	if (rc) return (rc);

	hi = temp_val>>2;
	lo = temp_val &3;

	if (hi & 0x80) {
		// 2' complement
		hi = (~hi + 1)& 0x7F;
	}

	printf("  TD%d : %d.%d\n", td_no, hi, lo*25);
	return (rc);
}

uint32_t hwmon_temp_test (int fd, uint8_t i2c_bus, uint32_t i2c_addr, uint8_t td_no,
		int8_t min, int8_t max)
{
	int rc = 0;
	uint16_t temp_val = 0;
	int8_t  hi_temp;

	rc = hwmon_temp_rd (fd, i2c_bus, i2c_addr, td_no, &temp_val);
	if (rc) return (rc);

	hi_temp = temp_val >> 2;

	printf(" TD%d : %d.%d [Min=%d Max=%d]\n", td_no, hi_temp, 
			25*(temp_val&3), min, max);

	if ((hi_temp < min) || (hi_temp > max)) {
		return (-1);
	}
	return (rc);
}


uint32_t hwmon_voltage_rd (int fd, uint8_t i2c_bus, uint32_t i2c_addr, uint8_t reg_addr,
			uint32_t *volt_mv)
{
	int rc;
	uint8_t reg_data, lo_data;

	rc = hwmon_rd(fd, i2c_bus, i2c_addr, reg_addr, &reg_data);
	if (rc) return (rc);

	rc = hwmon_rd(fd, i2c_bus, i2c_addr, W83793_BANK0_VIN_LOW_BITS, &lo_data);
	if (rc) return (rc);

	switch(reg_addr) {
		case	W83793_BANK0_VOLT_CORE_A:
			*volt_mv = ((reg_data << 2) + (lo_data &3))*2;
			break;

		case	W83793_BANK0_VOLT_CORE_B:
			*volt_mv = ((reg_data << 2) + ((lo_data>>2)&3))*2;
			break;

		case	W83793_BANK0_VTT_READ:
			*volt_mv = ((reg_data << 2) + ((lo_data>>4)&3))*2;
			break;

		case	W83793_BANK0_VSENS1_READ:
		case	W83793_BANK0_VSENS2_READ:
		case	W83793_BANK0_3VSEN_READ:
			*volt_mv = reg_data * 16;
			break;

		case	W83793_BANK0_12VSEN_READ:
			*volt_mv = reg_data * 8;
			break;

		case	W83793_BANK0_5VDD_READ:
		case	W83793_BANK0_5VSB_READ:
			*volt_mv = (reg_data * 24)+150;
			break;

			break;

		case	W83793_BANK0_VBAT_READ:
			*volt_mv = (reg_data * 16);
			break;

		default:
			break;
	}
	return (rc);	
}

typedef struct _hwmon_reg_addr_name_s_
{
	uint8_t addr;
	char	*name;
}hwmon_reg_addr_name_t;

int hwmon_voltage_dump (int fd, uint8_t i2c_bus, uint32_t i2c_addr)
{
	int	  rc = 0;
	uint32_t  volt_mv, cnt, count;
	hwmon_reg_addr_name_t hwmon_vname[] = {
			{W83793_BANK0_VOLT_CORE_A, "W83793_BANK0_VOLT_CORE_A"},
			{W83793_BANK0_VOLT_CORE_B, "W83793_BANK0_VOLT_CORE_B"},
			{W83793_BANK0_VTT_READ,    "W83793_BANK0_VTT_READ"},
			{W83793_BANK0_VSENS1_READ, "W83793_BANK0_VSENS1_READ"},
			{W83793_BANK0_VSENS2_READ, "W83793_BANK0_VSENS2_READ"},
			{W83793_BANK0_3VSEN_READ,  "W83793_BANK0_3VSEN_READ"},
			{W83793_BANK0_12VSEN_READ, "W83793_BANK0_12VSEN_READ"},
			{W83793_BANK0_5VDD_READ,   "W83793_BANK0_5VDD_READ"},
			{W83793_BANK0_5VSB_READ,   "W83793_BANK0_5VSB_READ"},
			{W83793_BANK0_VBAT_READ,   "W83793_BANK0_VBAT_READ"},
			};
	count = sizeof(hwmon_vname)/sizeof(hwmon_reg_addr_name_t);

	for (cnt = 0; cnt < count; cnt++) {
		rc = hwmon_voltage_rd (fd, i2c_bus, i2c_addr, hwmon_vname[cnt].addr, &volt_mv);
		if (rc) break;

		printf("  %-24s [0x%X] : %d mv\n", hwmon_vname[cnt].name, 
				hwmon_vname[cnt].addr, volt_mv);
	}

	return (rc);
}

int hwmon_voltage_test (int fd, uint8_t i2c_bus, uint32_t i2c_addr, uint8_t tolerance)
{
	int	  rc = 0;
	uint32_t  volt_mv, cnt, count, vmin, vmax;
	hwmon_reg_addr_name_t hwmon_vname[] = {
			{W83793_BANK0_5VDD_READ,   "W83793_BANK0_5VDD_READ"},
			{W83793_BANK0_5VSB_READ,   "W83793_BANK0_5VSB_READ"},
			};
	count = sizeof(hwmon_vname)/sizeof(hwmon_reg_addr_name_t);

	for (cnt = 0; cnt < count; cnt++) {
		rc = hwmon_voltage_rd (fd, i2c_bus, i2c_addr, hwmon_vname[cnt].addr, &volt_mv);
		if (rc) break;

		vmax = (5000 * (100+tolerance))/100;
		vmin = (5000 * (100-tolerance))/100;

		printf("  %-24s [0x%X] : %d mv [Min %d mv - Max %d mv]\n", 
				hwmon_vname[cnt].name, 
				hwmon_vname[cnt].addr, volt_mv, vmin, vmax);
	}

	return (rc);
}
