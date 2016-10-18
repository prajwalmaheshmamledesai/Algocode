/** 
 * @file hwmon_lib.h
 *
 * @brief
 *  include file for w83793 device
 */
/*
 * Author: Sudharshan Kadari (skadari) 
 *
 * Copyright (c), Cisco Systems, 1984-2009
 */
#ifndef _HWMON_LIB_H__
#define _HWMON_LIB_H__

#define HWMON_FAN_COUNT 10
#define HWMON_FAN_MASK  0x3FF
#define HWMON_FAN0_DUTY_CYCLE_OFF       3 // Duty registers 4-8 control speed

extern uint32_t hwmon_temp_show (int fd, uint8_t bus, uint32_t addr, uint8_t inst);
extern uint32_t hwmon_fan_show (int fd, uint8_t bus, uint32_t addr, uint8_t inst);
extern uint32_t hwmon_fan_init (int fd, uint8_t bus, uint32_t addr, uint8_t inst);
extern uint32_t hwmon_fan_spd_set (int fd, uint8_t bus, uint32_t addr, 
				uint8_t inst, uint8_t percent);
extern uint32_t hwmon_temp_test (int fd, uint8_t i2c_bus, uint32_t i2c_addr, 
				 uint8_t td_no, int8_t min, int8_t max);
extern uint32_t hwmon_fan_speed_test (int fd, uint8_t i2c_bus, uint32_t i2c_addr, 
		uint8_t fan_no, uint32_t exp_speed, uint8_t percent_tolerance);
extern uint32_t hwmon_fan_test (int fd, uint8_t i2c_bus, uint32_t i2c_addr, uint8_t fan_no,
                        uint32_t min, uint32_t max);
#endif

