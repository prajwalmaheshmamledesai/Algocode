/** 
 * @file adt7462.h
 *
 * @brief
 *  include file for adt7462 device
 */
/*
 * Author: mstrathman
 *
 * Copyright (c), Nuova Systems, 2007
 */
#ifndef _ADT7462_H__
#define _ADT7462_H__

/* register defines */
#define ADT7462_REG_CONFIG0      0x00
#define ADT7462_REG_CONFIG1      0x01
#define ADT7462_CFG1_MONITOR		0x01
#define ADT7462_CFG1_ALERT		0x08
#define ADT7462_CFG1_FAST_SPINUP_DIS	0x10
#define ADT7462_CFG1_SETUP_COMPLETE	0x20
#define ADT7462_CFG1_LOCK		0x40
#define ADT7462_CFG1_RDY		0x80

#define ADT7462_REG_CONFIG2      0x02

#define ADT7462_CFG2_FAST_MEASURE 	0x01
#define ADT7462_CFG2_PWM_MODE	 	0x04
#define ADT7462_CFG2_NO_VRD1_BOOST	0x08
#define ADT7462_CFG2_NO_VRD2_BOOST	0x10
#define ADT7462_CFG2_FAN_FULL_SPD	0x20
#define ADT7462_CFG2_TACH_PULSES_1X	0x00	
#define ADT7462_CFG2_TACH_PULSES_2X	0x40	
#define ADT7462_CFG2_TACH_PULSES_3X	0x80	
#define ADT7462_CFG2_TACH_PULSES_4X	0xC0	

#define ADT7462_REG_CONFIG3      0x03
#define ADT7462_CFG3_GPIO_EN		0x01

#define ADT7462_REG_TACH_EN      0x07
#define ADT7462_REG_TACH_CN      0x08
#define ADT7462_REG_GPIO1_B      0x09
#define ADT7462_REG_GPIO2_B      0x0A
#define ADT7462_REG_TM_CAL1      0x0B
#define ADT7462_REG_TM_CAL2      0x0C
#define ADT7462_REG_THERM_CN     0x0D
#define ADT7462_REG_THERM1_CN    0x0E
#define ADT7462_REG_THERM2_CN    0x0F
#define ADT7462_REG_PIN_CN1      0x10
#define ADT7462_CN1_12V		 	0x00
#define ADT7462_CN1_TACH5	 	0x01
#define ADT7462_CN1_TACH4	 	0x02
#define ADT7462_CN1_TACH3	 	0x04
#define ADT7462_CN1_TACH2	 	0x08
#define ADT7462_CN1_TACH1	 	0x10
#define ADT7462_CN1_DIODE3	 	0x20
#define ADT7462_CN1_DIODE1	 	0x40

#define ADT7462_REG_PIN_CN2      0x11
#define ADT7462_REG_PIN_CN3      0x12
#define ADT7462_REG_PIN_CN4      0x13
#define ADT7462_REG_EASY_CN      0x14
#define ADT7462_EASY_CN_OPTION1      0x01
#define ADT7462_EASY_CN_OPTION2      0x02
#define ADT7462_EASY_CN_OPTION3      0x04
#define ADT7462_EASY_CN_OPTION4      0x08
#define ADT7462_EASY_CN_OPTION5      0x10

#define ADT7462_REG_EDO_EN       0x16
#define ADT7462_REG_ATTN1_EN     0x18
#define ADT7462_REG_ATTN2_EN     0x19
#define ADT7462_REG_EN_AC1       0x1A
#define ADT7462_REG_EN_AC2       0x1B
#define ADT7462_REG_FAN_TEST     0x1C
#define ADT7462_REG_FAN_PRES     0x1D
#define ADT7462_REG_FAN_TEST_EN  0x1E
#define ADT7462_REG_PWM1_CN      0x21
#define ADT7462_REG_PWM2_CN      0x22
#define ADT7462_REG_PWM3_CN      0x23


#define ADT7462_PWM_SPIN	     0x00
#define ADT7462_PWM_SPIN_MASK	     0x07
#define ADT7462_PWM_SPIN_NO_TO       0x00
#define ADT7462_PWM_SPIN_TO_100ms    0x01
#define ADT7462_PWM_SPIN_TO_250ms    0x02
#define ADT7462_PWM_SPIN_TO_400ms    0x03
#define ADT7462_PWM_SPIN_TO_667ms    0x04
#define ADT7462_PWM_SPIN_TO_1sec     0x05
#define ADT7462_PWM_SPIN_TO_2sec     0x06
#define ADT7462_PWM_SPIN_TO_32sec    0x07

#define ADT7462_PWM_SLOW	     0x03
#define ADT7462_PWM_SLOW_MASK	     0x08

#define ADT7462_PWM_BHVR	     0x05
#define ADT7462_PWM_BHVR_MASK	     0xE0
#define ADT7462_PWM_BHVR_LOCAL_TEMP  0x00	
#define ADT7462_PWM_BHVR_RMT1_TEMP   0x20	
#define ADT7462_PWM_BHVR_RMT2_TEMP   0x40	
#define ADT7462_PWM_BHVR_RMT3_TEMP   0x60	
#define ADT7462_PWM_BHVR_OFF 	     0x80	
#define ADT7462_PWM_BHVR_MAX_SPD     0xA0	
#define ADT7462_PWM_BHVR_MAX_SPD1    0xC0	
#define ADT7462_PWM_BHVR_MANUAL      0xE0	

#define ADT7462_PWM_INV 	     0x04
#define ADT7462_PWM_OP_ACTIVE_HI     0x00
#define ADT7462_PWM_OP_ACTIVE_LO     0x10

#define ADT7462_REG_PWM4_CN      0x24
#define ADT7462_REG_PWM1_2_FRQ   0x25
#define ADT7462_PWM_FRQ_PWM1_OFF	 0x01
#define ADT7462_PWM_FRQ_PWM2_OFF	 0x02
#define ADT7462_PWM_FRQ_PWM1_LOW	 0x02
#define ADT7462_PWM_FRQ_PWM2_LOW	 0x05

#define ADT7462_REG_PWM3_4_FRQ   0x26

#define ADT7462_PWM_FRQ_PWM3_OFF	 0x01
#define ADT7462_PWM_FRQ_PWM4_OFF	 0x02
#define ADT7462_PWM_FRQ_PWM3_LOW	 0x02
#define ADT7462_PWM_FRQ_PWM4_LOW	 0x05

#define ADT7462_PWM_FRQ_LOW_11_0	 0x00
#define ADT7462_PWM_FRQ_LOW_14_7	 0x01
#define ADT7462_PWM_FRQ_LOW_22_1	 0x02
#define ADT7462_PWM_FRQ_LOW_29_4	 0x03
#define ADT7462_PWM_FRQ_LOW_35_3	 0x04
#define ADT7462_PWM_FRQ_LOW_44_1	 0x05
#define ADT7462_PWM_FRQ_LOW_58_8	 0x06
#define ADT7462_PWM_FRQ_LOW_88_2	 0x07

#define ADT7462_REG_PWM1_MIN     0x28
#define ADT7462_REG_PWM2_MIN     0x29
#define ADT7462_REG_PWM3_MIN     0x2A
#define ADT7462_REG_PWM4_MIN     0x2B
#define ADT7462_REG_PWM_MAX      0x2C
#define ADT7462_PWM_MAX_DUTY_CYCLE      0xFF
#define ADT7462_PWM_HALF_DUTY_CYCLE     0x80

#define ADT7462_REG_THERM_MASK1  0x30
#define ADT7462_REG_THERM_MASK2  0x31
#define ADT7462_REG_VOLT_MASK1   0x32
#define ADT7462_REG_VOLT_MASK2   0x33
#define ADT7462_REG_FAN_MASK     0x34
#define ADT7462_REG_DIG_MASK     0x35
#define ADT7462_REG_GPIO_MASK    0x36
#define ADT7462_REG_EDO_MASK1    0x37
#define ADT7462_REG_EDO_MASK2    0x38
#define ADT7462_REG_DEV_ID       0x3D
    #define ADT7462_REG_DEV_ID_DEFAULT 0x62
#define ADT7462_REG_CMPY_ID      0x3E
    #define ADT7462_REG_CMPY_ID_DEFAULT 0x41
#define ADT7462_REG_REVISION     0x3F
#define ADT7462_REG_LCL_LOW      0x44
#define ADT7462_REG_RMT1_LOW     0x45
#define ADT7462_REG_RMT2_LOW     0x46
#define ADT7462_REG_RMT3_LOW     0x47
#define ADT7462_REG_LCL_HI       0x48
#define ADT7462_REG_RMT1_HI      0x49
#define ADT7462_REG_RMT2_HI      0x4A
#define ADT7462_REG_RMT3_HI      0x4B
#define ADT7462_REG_LCL_THERM1   0x4C
#define ADT7462_REG_RMT1_THERM1  0x4D
#define ADT7462_REG_RMT2_THERM1  0x4E
#define ADT7462_REG_RMT3_THERM1  0x4F
#define ADT7462_REG_LCL_THERM2   0x50
#define ADT7462_REG_RMT1_THERM2  0x51
#define ADT7462_REG_RMT2_THERM2  0x52
#define ADT7462_REG_RMT3_THERM2  0x53
#define ADT7462_REG_LCL_RMT1_H   0x54
#define ADT7462_REG_RMT2_RMT3_H  0x55
#define ADT7462_REG_LCL_OFFSET   0x56
#define ADT7462_REG_RMT1_OFFSET  0x57
#define ADT7462_REG_RMT2_OFFEST  0x58
#define ADT7462_REG_RMT3_OFFSET  0x59
#define ADT7462_REG_RMT1_OP      0x5A
#define ADT7462_REG_RMT2_OP      0x5B
#define ADT7462_REG_LCL_TMIN     0x5C
#define ADT7462_REG_RMT1_TMIN    0x5D
#define ADT7462_REG_RMT2_TMIN    0x5E
#define ADT7462_REG_RMT3_TMIN    0x5F
#define ADT7462_REG_LCL_TR_H     0x60
#define ADT7462_REG_RMT1_TR_H    0x61
#define ADT7462_REG_RMT2_TR_H    0x62
#define ADT7462_REG_RMT3_TR_H    0x63
#define ADT7462_REG_OP_H         0x64
#define ADT7462_REG_3V3_HI       0x68
#define ADT7462_REG_PIN23_V_HI   0x69
#define ADT7462_REG_PIN24_V_HI   0x6A
#define ADT7462_REG_PIN35_V_HI   0x6B
#define ADT7462_REG_PIN26_V_HI   0x6C
#define ADT7462_REG_12V0_1_LOW   0x6D
#define ADT7462_REG_12V0_2_LOW   0x6E
#define ADT7462_REG_12V0_3_LOW   0x6F
#define ADT7462_REG_3V3_LOW      0x70
#define ADT7462_REG_5V0_LOW      0x71
#define ADT7462_REG_PIN23_V_LOW  0x72
#define ADT7462_REG_PIN24_V_LOW  0x73
#define ADT7462_REG_PIN25_V_LOW  0x74
#define ADT7462_REG_PIN26_V_LOW  0x75
#define ADT7462_REG_1V5_1_V_LOW  0x76
#define ADT7462_REG_1V5_2_V_LOW  0x77
#define ADT7462_REG_TACH1_LIM    0x78
#define ADT7462_REG_TACH2_LIM    0x79
#define ADT7462_REG_TACH3_LIM    0x7A
#define ADT7462_REG_TACH4_LIM    0x7B
#define ADT7462_REG_TACH5_LIM    0x7C
#define ADT7462_REG_TACH6_LIM    0x7D
#define ADT7462_REG_TACH7_LIM    0x7E
#define ADT7462_REG_TACH8_LIM    0x7F
#define ADT7462_REG_THERM1_LIM   0x80
#define ADT7462_REG_THERM2_LIM   0x81
#define ADT7462_REG_LCL_TMP_LSB  0x88
#define ADT7462_REG_LCL_TMP_MSB  0x89
#define ADT7462_REG_RMT1_TMP_LSB 0x8A
#define ADT7462_REG_RMT1_TMP_MSB 0x8B
#define ADT7462_REG_RMT2_TMP_LSB 0x8C
#define ADT7462_REG_RMT2_TMP_MSB 0x8D
#define ADT7462_REG_RMT3_TMP_LSB 0x8E
#define ADT7462_REG_RMT3_TMP_MSB 0x8F
#define ADT7462_REG_PIN23_V      0x90
#define ADT7462_REG_PIN24_V      0x91
#define ADT7462_REG_PIN25_V      0x92
#define ADT7462_REG_PIN26_V      0x93
#define ADT7462_REG_1V5_1_V      0x94
#define ADT7462_REG_1V5_2_V      0x95
#define ADT7462_REG_3V3_V        0x96
#define ADT7462_REG_VID_VAL      0x97
#define ADT7462_REG_TACH1_LSB    0x98
#define ADT7462_REG_TACH1_MSB    0x99
#define ADT7462_REG_TACH2_LSB    0x9A
#define ADT7462_REG_TACH2_MSB    0x9B
#define ADT7462_REG_TACH3_LSB    0x9C
#define ADT7462_REG_TACH3_MSB    0x9D
#define ADT7462_REG_TACH4_LSB    0x9E
#define ADT7462_REG_TACH4_MSB    0x9F
#define ADT7462_REG_TACH5_LSB    0xA2
#define ADT7462_REG_TACH5_MSB    0xA3
#define ADT7462_REG_TACH6_LSB    0xA4
#define ADT7462_REG_TACH6_MSB    0xA5
#define ADT7462_REG_TACH7_LSB    0xA6
#define ADT7462_REG_TACH7_MSB    0xA7
#define ADT7462_REG_TACH8_LSB    0xA8
#define ADT7462_REG_TACH8_MSB    0xA9
#define ADT7462_REG_PWM1         0xAA
#define ADT7462_REG_PWM2         0xAB
#define ADT7462_REG_PWM3         0xAC
#define ADT7462_REG_PWM4         0xAD
#define ADT7462_REG_THERM1_PCT   0xAE
#define ADT7462_REG_THERM2_PCT   0xAF
#define ADT7462_REG_THERM_STAT1  0xB8
#define ADT7462_REG_THERM_STAT2  0xB9
#define ADT7462_REG_THERM_STAT3  0xBA
#define ADT7462_REG_VOLT_STAT1   0xBB
#define ADT7462_REG_VOLT_STAT2   0xBC
#define ADT7462_REG_FAN_STAT     0xBD
#define ADT7462_REG_DIG_STAT     0xBE
#define ADT7462_REG_GPIO_STAT    0xBF
#define ADT7462_REG_THERM_BMC_S1 0xC0
#define ADT7462_REG_THERM_BMC_S2 0xC1
#define ADT7462_REG_THERM_BMC_S3 0xC2
#define ADT7462_REG_VOLT_BMC_S1  0xC3
#define ADT7462_REG_VOLT_BMC_S2  0xC4
#define ADT7462_REG_FAN_BMC_S    0xC5
#define ADT7462_REG_DIG_BMC_S    0xC6
#define ADT7462_REG_GPIO_BMC_S   0xC7
#define ADT7462_MAX_INST_OREGON  2

extern int adt7462_rd ( int fd, uint16_t i2c_addr, uint8_t, uint32_t, uint8_t * );
extern int adt7462_wr ( int fd, uint16_t i2c_addr, uint8_t, uint32_t, uint8_t * );

extern int adt7462_init ( int fd, uint16_t i2c_addr, uint8_t );

extern int adt7462_pwm_rd (int fd, uint16_t i2c_addr, uint8_t reg, uint8_t * );
extern int adt7462_pwm_wr  (int fd, uint16_t i2c_addr, uint8_t reg, uint8_t );

extern int adt7462_tach_rd (int fd, uint16_t i2c_addr, uint8_t reg, uint16_t * );
extern int adt7462_tach_to_rpm ( uint32_t * );

extern int adt7462_to_c   (int );
extern int fan_tach_rd    (int fd, uint16_t i2c_addr, uint8_t reg, uint32_t *speed);

extern int adt7462_gpio_init (int fd, uint16_t i2c_addr, uint8_t gpio,
                        uint8_t output, uint8_t pol, uint8_t int_msk);
extern int adt7462_gpio_rd (int fd, uint16_t i2c_addr, uint8_t gpio, uint8_t *stat);
extern int adt7462_gpio_wr (int fd, uint16_t i2c_addr, uint8_t gpio, uint8_t stat);
extern int adt7462_local_ts_rd (int fd, uint16_t i2c_addr, uint32_t *val);
extern int adt7462_remote_ts_rd (int fd, uint16_t i2c_addr, int channel,
                                 uint32_t *val);

typedef struct _diag_fan_behavior {
    uint32_t duty_cycle;
    uint32_t rpm;
    uint32_t delta;
} diag_fan_behavior_t;

#endif

