/** 
 * @file fan_lib.h
 *
 * @brief
 *  include file for adt7476 device
 */
/*
 * Author: Sudharshan Kadari (skadari) 
 *
 * Copyright (c), Cisco Systems, 1984-2009
 */
#ifndef _FAN_LIB_H__
#define _FAN_LIB_H__

#define HWMON_FAN_COUNT             8
#define HWMON_FAN_MASK              0xFF
#define HWMON_FAN0_DUTY_CYCLE_OFF   3 // Duty registers 4-8 control speed

extern int fan_rd	(int fd, uint8_t i2c_bus, uint16_t i2c_addr, uint8_t, uint8_t * );
extern int fan_wr	(int fd, uint8_t i2c_bus, uint16_t i2c_addr, uint8_t, uint8_t * );
extern int fan_to_c	(int va);
extern int fan_init	(int fd, uint8_t i2c_bus, uint16_t i2c_addr);

extern int fan_led	(int fd, uint8_t i2c_bus, uint16_t i2c_addr, uint8_t i, uint8_t r, uint8_t on);
extern int fan_ts_rd	(int fd, uint8_t i2c_bus, uint16_t i2c_addr, int, int8_t * );
extern int fan_pwm_rd	(int fd, uint8_t i2c_bus, uint16_t i2c_addr, uint8_t reg, uint8_t * );
extern int fan_pwm_wr	(int fd, uint8_t i2c_bus, uint16_t i2c_addr, uint8_t reg, uint8_t );
extern int fan_rpm_rd	(int fd, uint8_t i2c_bus, uint16_t i2c_addr, uint8_t reg, uint32_t *speed);
extern int fan_tach_rd	(int fd, uint8_t i2c_bus, uint16_t i2c_addr, uint8_t reg, uint16_t * );
extern int fan_gpio_rd	(int fd, uint8_t i2c_bus, uint16_t i2c_addr, uint8_t gpio, uint8_t *stat);
extern int fan_gpio_wr	(int fd, uint8_t i2c_bus, uint16_t i2c_addr, uint8_t gpio, uint8_t stat);
extern int fan_spd_set	(int fd, uint8_t i2c_bus, uint16_t i2c_addr, uint8_t inst, uint8_t percent);
extern int fan_spd_show	(int fd, uint8_t i2c_bus, uint16_t i2c_addr, uint8_t inst);
extern int fan_temp_show(int fd, uint8_t i2c_bus, uint16_t i2c_addr, uint8_t inst);
extern int fan_vltg_show(int fd, uint8_t i2c_bus, uint16_t i2c_addr, uint8_t inst);

extern int fan_tach_to_rpm	(uint32_t * );
extern int fan_gpio_init	(int fd, uint8_t i2c_bus, uint16_t i2c_addr, uint8_t gpio,
				 uint8_t output, uint8_t pol, uint8_t int_msk);
extern int fan_local_ts_rd	(int fd, uint8_t i2c_bus, uint16_t i2c_addr, int8_t *val);
extern int fan_remote_1_ts_rd	(int fd, uint8_t i2c_bus, uint16_t i2c_addr, int8_t *val);
extern int fan_remote_2_ts_rd	(int fd, uint8_t i2c_bus, uint16_t i2c_addr, int8_t *val);
extern int fan_reg_acc_test	(int fd, uint8_t i2c_bus, uint16_t i2c_addr, uint8_t inst, 
				 uint8_t e_dev, uint8_t e_cmpny, uint32_t *, 
				 uint32_t *exp_data, uint32_t *got_data);
extern int fan_speed_check	(int fd, uint8_t i2c_bus, uint16_t i2c_addr, uint8_t inst, 
				 uint8_t behv, uint32_t *addr_error, 
				 uint32_t *exp_data, uint32_t *got_data);
extern int fan_speed_test	(int fd, uint8_t i2c_bus, uint16_t i2c_addr, uint8_t inst, 
				 uint8_t slp_time, uint32_t *a_err, 
				 uint32_t *ed, uint32_t *gd);
extern int fan_probe_test(int fd, uint8_t i2c_bus, uint16_t i2c_addr, uint8_t inst);
#endif

