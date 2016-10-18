/** 
 * @file adt7473.h
 *
 * @brief
 *  include file for adt7473 device
 */
/*
 * Author: mstrathman
 *
 * Copyright (c), Nuova Systems, 2007
 */
#ifndef _ADT7473_H__
#define _ADT7473_H__

/* register defines */
#define ADT7473_REG_VCCP_READ	0x21
#define ADT7473_REG_VCC_READ	0x22
#define ADT7473_REG_RMT1_TEMP	0x25
#define ADT7473_REG_LCL_TEMP	0x26
#define ADT7473_REG_RMT2_TEMP	0x27
#define ADT7473_REG_TACH1_LOW	0x28
#define ADT7473_REG_TACH1_HIGH	0x29
#define ADT7473_REG_TACH2_LOW	0x2A
#define ADT7473_REG_TACH2_HIGH	0x2B
#define ADT7473_REG_TACH3_LOW	0x2C
#define ADT7473_REG_TACH3_HIGH	0x2D
#define ADT7473_REG_TACH4_LOW	0x2E
#define ADT7473_REG_TACH4_HIGH	0x2F
#define ADT7473_REG_PWM1			0x30
#define ADT7473_REG_PWM2			0x31
#define ADT7473_REG_PWM3			0x32
#define ADT7473_REG_RMT1_OP_POINT		0x33
#define ADT7473_REG_LCL_OP_POINT		0x34
#define ADT7473_REG_RMT2_OP_POINT		0x35
#define ADT7473_REG_DYN_TMIN_CTRL1		0x36
#define ADT7473_REG_DYN_TMIN_CTRL2		0x37
#define ADT7473_REG_PWM1_MAX_DUTY_CYCLE		0x38
#define ADT7473_REG_PWM2_MAX_DUTY_CYCLE		0x39
#define ADT7473_REG_PWM3_MAX_DUTY_CYCLE		0x3A
#define ADT7473_REG_DEV_ID_REG			0x3D
    #define ADT7473_REG_DEV_ID_DEFAULT 	0x73
#define ADT7473_REG_CMPY_ID_REG			0x3E
    #define ADT7462_REG_CMPY_ID_DEFAULT	0x41
#define ADT7473_REG_REV_ID_REG			0x3F
#define ADT7473_REG_CFG1			0x40
#define ADT7473_REG_IRQ_STAT1			0x41
#define ADT7473_REG_IRQ_STAT2			0x42
#define ADT7473_REG_VCCP_LOW_LIMIT		0x46
#define ADT7473_REG_VCCP_HI_LIMIT		0x47
#define ADT7473_REG_VCC_LOW_LIMIT		0x48
#define ADT7473_REG_VCC_HI_LIMIT		0x49
#define ADT7473_REG_RMT1_LOW_LIMIT		0x4E
#define ADT7473_REG_RMT1_HI_LIMIT		0x4F
#define ADT7473_REG_LCL_LOW_LIMIT		0x50
#define ADT7473_REG_LCL_HI_LIMIT		0x51
#define ADT7473_REG_RMT2_LOW_LIMIT		0x52
#define ADT7473_REG_RMT2_HI_LIMIT		0x53
#define ADT7473_REG_TACH1_MIN_LOW		0x54
#define ADT7473_REG_TACH1_MIN_HIGH		0x55
#define ADT7473_REG_TACH2_MIN_LOW		0x56
#define ADT7473_REG_TACH2_MIN_HIGH		0x57
#define ADT7473_REG_TACH3_MIN_LOW		0x58
#define ADT7473_REG_TACH3_MIN_HIGH		0x59
#define ADT7473_REG_TACH4_MIN_LOW		0x5A
#define ADT7473_REG_TACH4_MIN_HIGH		0x5B
#define ADT7473_REG_PWM1_CFG			0x5C
#define ADT7473_REG_PWM2_CFG			0x5D
#define ADT7473_REG_PWM3_CFG			0x5E
#define ADT7473_REG_RMT1_PWM1_HZ		0x5F
#define ADT7473_REG_RMT1_PWM2_HZ		0x60
#define ADT7473_REG_RMT1_PWM3_HZ		0x61
#define ADT7473_REG_ENHANCED_ACOUSTICS1		0x62
#define ADT7473_REG_ENHANCED_ACOUSTICS2		0x63
#define ADT7473_REG_PWM1_MIN_DUTY_CYCLE		0x64
#define ADT7473_REG_PWM2_MIN_DUTY_CYCLE		0x65
#define ADT7473_REG_PWM3_MIN_DUTY_CYCLE		0x66
#define ADT7473_REG_RMT1_TEMP_MIN		0x67
#define ADT7473_REG_LCL_TEMP_MIN		0x68
#define ADT7473_REG_RMT2_TEMP_MIN		0x69
#define ADT7473_REG_RMT1_TEMP_LIMIT		0x6A
#define ADT7473_REG_LCL_TEMP_LIMIT		0x6B
#define ADT7473_REG_RMT2_TEMP_LIMIT		0x6C
#define ADT7473_REG_RMT1_TEMP_HYST		0x6D
#define ADT7473_REG_RMT2_TEMP_HYST		0x6E
#define ADT7473_REG_XNOR_TREE_TEST_EN		0x6F
#define ADT7473_REG_RMT1_TEMP_OFFSET		0x70
#define ADT7473_REG_LCL_TEMP_OFFSET		0x71
#define ADT7473_REG_RMT2_TEMP_OFFSET		0x72
#define ADT7473_REG_CFG2			0x73
#define ADT7473_REG_IRQ_MASK1			0x74
#define ADT7473_REG_IRQ_MASK2			0x75
#define ADT7473_REG_EXT_RES1			0x76
#define ADT7473_REG_EXT_RES2			0x77
#define ADT7473_REG_CFG3			0x78
#define ADT7473_REG_TIMER_STAT			0x79
#define ADT7473_REG_TIMER_LIMIT			0x7A
#define ADT7473_REG_TACH_PULSES_PER_REV		0x7B
#define ADT7473_REG_CFG5			0x7C
#define ADT7473_REG_CFG4			0x7D
#define ADT7473_REG_TEST1			0x7E
#define ADT7473_REG_TEST2			0x7F
#define ADT7473_REG_TEST3			0x80

extern int adt7473_rd ( int fd, uint16_t i2c_addr, uint8_t, uint32_t, uint8_t * );
extern int adt7473_wr ( int fd, uint16_t i2c_addr, uint8_t, uint32_t, uint8_t * );

extern int adt7473_init ( int fd, uint16_t i2c_addr);

extern int adt7473_pwm_rd (int fd, uint16_t i2c_addr, uint8_t reg, uint8_t * );
extern int adt7473_pwm_wr  (int fd, uint16_t i2c_addr, uint8_t reg, uint8_t );

extern int adt7473_tach_rd (int fd, uint16_t i2c_addr, uint8_t reg, uint16_t * );
extern int adt7473_tach_to_rpm ( uint32_t * );

extern int adt7473_ts_rd  (int fd, uint16_t i2c_addr, int, int8_t * );
extern int adt7473_to_c   (int );
extern int fan_tach_rd    (int fd, uint16_t i2c_addr, uint8_t reg, uint32_t *speed);

extern int adt7473_gpio_init (int fd, uint16_t i2c_addr, uint8_t gpio,
                        uint8_t output, uint8_t pol, uint8_t int_msk);
extern int adt7473_gpio_rd (int fd, uint16_t i2c_addr, uint8_t gpio, uint8_t *stat);
extern int adt7473_gpio_wr (int fd, uint16_t i2c_addr, uint8_t gpio, uint8_t stat);
extern int adt7473_local_ts_rd (int fd, uint16_t i2c_addr, int8_t *val);
extern int adt7343_rpm_rd (int fd, uint16_t i2c_addr, uint8_t reg, uint32_t *speed);


#endif

