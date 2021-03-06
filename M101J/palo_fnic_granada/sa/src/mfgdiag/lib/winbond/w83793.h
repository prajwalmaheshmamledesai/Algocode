/********************************************************************
 * File:   w83793.h
 * Name:   Sudharshan Kadari 
 * Description: register defines of W83793
 * Copyright (c) 2009 by Cisco Systems, Inc.
 * All rights reserved.
 *********************************************************************/
#ifndef _W83793_H_
#define _W83793_H_

#define W83793_REG_BANKSEL		0x00
#define W83793_BANK0_WD_LOCK		0x01
#define W83793_BANK0_WD_ENABLE		0x02
#define W83793_BANK0_WD_STATUS		0x03
#define W83793_BANK0_WD_TIMER		0x04
#define W83793_BANK0_VID_INA		0x05
#define W83793_BANK0_VID_INB		0x06
#define W83793_BANK0_VID_LATCHA		0x07
#define W83793_BANK0_VID_LATCHB		0x08
#define W83793_BANK0_I2C_ADDR		0x0b
#define W83793_BANK0_I2C_SUBADDR	0x0c
#define W83793_BANK0_VENDOR_ID		0x0d
#define W83793_BANK0_CHIP_ID		0x0e
#define W83793_BANK0_DEVICE_ID		0x0f

#define W83793_BANK0_VENDOR_ID_DFLT	0xA3
#define W83793_BANK0_CHIP_ID_DFLT	0x7B
#define W83793_BANK0_DEVICE_ID_DFLT	0x12

#define W83793_BANK0_VOLT_CORE_A	0x10
#define W83793_BANK0_VOLT_CORE_B	0x11
#define W83793_BANK0_VTT_READ		0x12
#define W83793_BANK0_VSENS1_READ	0x14
#define W83793_BANK0_VSENS2_READ	0x15
#define W83793_BANK0_3VSEN_READ		0x16
#define W83793_BANK0_12VSEN_READ	0x17
#define W83793_BANK0_5VDD_READ		0x18
#define W83793_BANK0_5VSB_READ		0x19
#define W83793_BANK0_VBAT_READ		0x1A
#define W83793_BANK0_VIN_LOW_BITS	0x1B
#define W83793_BANK0_TD1_READ		0x1C
#define W83793_BANK0_TD2_READ		0x1D
#define W83793_BANK0_TD3_READ		0x1E
#define W83793_BANK0_TD4_READ		0x1F

#define W83793_BANK0_TR1_READ		0x20
#define W83793_BANK0_TR2_READ		0x21
#define W83793_BANK0_TEMP_LOW_BITS	0x22
#define W83793_BANK0_FAN1_COUNT_HI	0x23
#define W83793_BANK0_FAN1_COUNT_LO	0x24
#define W83793_BANK0_FAN2_COUNT_HI	0x25
#define W83793_BANK0_FAN2_COUNT_LO	0x26
#define W83793_BANK0_FAN3_COUNT_HI	0x27
#define W83793_BANK0_FAN3_COUNT_LO	0x28
#define W83793_BANK0_FAN4_COUNT_HI	0x29
#define W83793_BANK0_FAN4_COUNT_LO	0x2A
#define W83793_BANK0_FAN5_COUNT_HI	0x2B
#define W83793_BANK0_FAN5_COUNT_LO	0x2C
#define W83793_BANK0_FAN6_COUNT_HI	0x2D
#define W83793_BANK0_FAN6_COUNT_LO	0x2E
#define W83793_BANK0_FAN7_COUNT_HI	0x2F
#define W83793_BANK0_FAN7_COUNT_LO	0x30
#define W83793_BANK0_FAN8_COUNT_HI	0x31
#define W83793_BANK0_FAN8_COUNT_LO	0x32
#define W83793_BANK0_FAN9_COUNT_HI	0x33
#define W83793_BANK0_FAN9_COUNT_LO	0x34
#define W83793_BANK0_FAN10_COUNT_HI	0x35
#define W83793_BANK0_FAN10_COUNT_LO	0x36
#define W83793_BANK0_FAN11_COUNT_HI	0x37
#define W83793_BANK0_FAN11_COUNT_LO	0x38	// Reserved May Be
#define W83793_BANK0_FAN12_COUNT_HI	0x39	// Reserved May Be
#define W83793_BANK0_FAN12_COUNT_LO	0x3A

#define W83793_BANK0_CONFIG		0x40
#define W83793_BANK0_IRQ_STATUS1	0x41
#define W83793_BANK0_IRQ_STATUS2	0x42
#define W83793_BANK0_IRQ_STATUS3	0x43
#define W83793_BANK0_IRQ_STATUS4	0x44
#define W83793_BANK0_IRQ_STATUS5	0x45

#define W83793_BANK0_IRQ_MASK1		0x46
#define W83793_BANK0_IRQ_MASK2		0x47
#define W83793_BANK0_IRQ_MASK3		0x48
#define W83793_BANK0_IRQ_MASK4		0x49
#define W83793_BANK0_IRQ_MASK5		0x4A

#define W83793_BANK0_REAL_STS1		0x4B
#define W83793_BANK0_REAL_STS2		0x4C
#define W83793_BANK0_REAL_STS3		0x4D
#define W83793_BANK0_REAL_STS4		0x4E
#define W83793_BANK0_REAL_STS5		0x4F
#define W83793_BANK0_IRQ_CTRL		0x50
#define W83793_BANK0_OVT_CTRL		0x51
#define W83793_BANK0_OVT_BEEP		0x52
#define W83793_BANK0_BEEP_CTRL1		0x53
#define W83793_BANK0_BEEP_CTRL2		0x54
#define W83793_BANK0_BEEP_CTRL3		0x55
#define W83793_BANK0_BEEP_CTRL4		0x56
#define W83793_BANK0_BEEP_CTRL5		0x57
#define W83793_BANK0_MFC		0x58
#define W83793_BANK0_VID_CTRL		0x59
#define W83793_BANK0_TD1_CONFIG		0x5A
#define W83793_BANK0_TD2_CONFIG		0x5B
#define W83793_BANK0_FANIN_CTRL		0x5C
#define W83793_BANK0_FANIN_SEL		0x5D
#define W83793_BANK0_TD_MODE_SEL	0x5E
#define W83793_BANK0_TR_MODE_SEL	0x5F


#define W83793_BANK0_VOLT_CORE_A_HI	0x60
#define W83793_BANK0_VOLT_CORE_A_LO	0x61
#define W83793_BANK0_VOLT_CORE_B_HI	0x62
#define W83793_BANK0_VOLT_CORE_B_LO	0x63
#define W83793_BANK0_VTT_HI		0x64
#define W83793_BANK0_VTT_LO		0x65
#define W83793_BANK0_HI_LIMIT_LO_BITS	0x68
#define W83793_BANK0_LO_LIMIT_LO_BITS	0x69
#define W83793_BANK0_VSENS1_HI		0x6A
#define W83793_BANK0_VSENS1_LO		0x6B
#define W83793_BANK0_VSENS2_HI		0x6C
#define W83793_BANK0_VSENS2_LO		0x6D
#define W83793_BANK0_3VSEN_HI		0x6E
#define W83793_BANK0_3VSEN_LO		0x6F
#define W83793_BANK0_12VSEN_HI		0x70
#define W83793_BANK0_12VSEN_LO		0x71
#define W83793_BANK0_5VDD_HI		0x72
#define W83793_BANK0_5VDD_LO		0x73
#define W83793_BANK0_5VSB_HI		0x74
#define W83793_BANK0_5VSB_LO		0x75
#define W83793_BANK0_VBAT_HI		0x76
#define W83793_BANK0_VBAT_LO		0x77
#define W83793_BANK0_TD1_CRIT           0x78
#define W83793_BANK0_TD1_CRIT_HYST      0x79
#define W83793_BANK0_TD1_WARN           0x7A
#define W83793_BANK0_TD1_WARN_HYST      0x7B
#define W83793_BANK0_TD2_CRIT           0x7C
#define W83793_BANK0_TD2_CRIT_HYST      0x7D
#define W83793_BANK0_TD2_WARN           0x7E
#define W83793_BANK0_TD2_WARN_HYST      0x7F
#define W83793_BANK0_TD3_CRIT           0x80
#define W83793_BANK0_TD3_CRIT_HYST      0x81
#define W83793_BANK0_TD3_WARN           0x82
#define W83793_BANK0_TD3_WARN_HYST      0x83
#define W83793_BANK0_TD4_CRIT           0x84
#define W83793_BANK0_TD4_CRIT_HYST      0x85
#define W83793_BANK0_TD4_WARN           0x86
#define W83793_BANK0_TD4_WARN_HYST      0x87
#define W83793_BANK0_TR1_CRIT           0x88
#define W83793_BANK0_TR1_CRIT_HYST      0x89
#define W83793_BANK0_TR1_WARN           0x8A
#define W83793_BANK0_TR1_WARN_HYST      0x8B
#define W83793_BANK0_TR2_CRIT           0x8C
#define W83793_BANK0_TR2_CRIT_HYST      0x8D
#define W83793_BANK0_TR2_WARN           0x8E
#define W83793_BANK0_TR2_WARN_HYST      0x8F

#define W83793_BANK0_FAN1_HI		0x90
#define W83793_BANK0_FAN1_LO		0x91
#define W83793_BANK0_FAN2_HI		0x92
#define W83793_BANK0_FAN2_LO		0x93
#define W83793_BANK0_FAN3_HI		0x94
#define W83793_BANK0_FAN3_LO		0x95
#define W83793_BANK0_FAN4_HI		0x96
#define W83793_BANK0_FAN4_LO		0x97
#define W83793_BANK0_FAN5_HI		0x98
#define W83793_BANK0_FAN5_LO		0x99
#define W83793_BANK0_FAN6_HI		0x9A
#define W83793_BANK0_FAN6_LO		0x9B
#define W83793_BANK0_FAN7_HI		0x9C
#define W83793_BANK0_FAN7_LO		0x9D
#define W83793_BANK0_FAN8_HI		0x9E
#define W83793_BANK0_FAN8_LO		0x9F
#define W83793_BANK0_FAN9_HI		0xA0
#define W83793_BANK0_FAN9_LO		0xA1
#define W83793_BANK0_FAN10_HI		0xA2
#define W83793_BANK0_FAN10_LO		0xA3
#define W83793_BANK0_FAN11_HI		0xA4
#define W83793_BANK0_FAN11_LO		0xA5
#define W83793_BANK0_FAN12_HI		0xA6
#define W83793_BANK0_FAN12_LO		0xA7
#define W83793_BANK0_TD1_TEMP_OFFSET	0xA8
#define W83793_BANK0_TD2_TEMP_OFFSET	0xA9
#define W83793_BANK0_TD3_TEMP_OFFSET	0xAA
#define W83793_BANK0_TD4_TEMP_OFFSET	0xAB
#define W83793_BANK0_TR1_TEMP_OFFSET	0xAC
#define W83793_BANK0_TR2_TEMP_OFFSET	0xAD

#define W83793_BANK0_FAN_OUT_STYLE1	0xB0
#define W83793_BANK0_FAN_OUT_STYLE2	0xB1
#define W83793_BANK0_FAN_DEFAULT_SPEED	0xB2

#define W83793_BANK0_FAN_DUTY_1		0xB3
#define W83793_BANK0_FAN_DUTY_2		0xB4
#define W83793_BANK0_FAN_DUTY_3		0xB5
#define W83793_BANK0_FAN_DUTY_4		0xB6
#define W83793_BANK0_FAN_DUTY_5		0xB7
#define W83793_BANK0_FAN_DUTY_6		0xB8
#define W83793_BANK0_FAN_DUTY_7		0xB9
#define W83793_BANK0_FAN_DUTY_8		0xBA
#define W83793_BANK0_FAN_PRESCALE_1	0xBB
#define W83793_BANK0_FAN_PRESCALE_2	0xBC
#define W83793_BANK0_FAN_PRESCALE_3	0xBD
#define W83793_BANK0_FAN_PRESCALE_4	0xBE
#define W83793_BANK0_FAN_PRESCALE_5	0xBF
#define W83793_BANK0_FAN_PRESCALE_6	0xC0
#define W83793_BANK0_FAN_PRESCALE_7	0xC1
#define W83793_BANK0_FAN_PRESCALE_8	0xC2
#define W83793_BANK0_STEP_UP_TIME	0xC3
#define W83793_BANK0_STEP_DOWN_TIME	0xC4
#define W83793_BANK0_CRIT_TEMP		0xC5

#define W83793_BANK0_PECI_AGENT_CFG	0xD0
#define W83793_BANK0_PECI_AGENT1_TCTRL	0xD1
#define W83793_BANK0_PECI_AGENT2_TCTRL	0xD2
#define W83793_BANK0_PECI_AGENT3_TCTRL	0xD3
#define W83793_BANK0_PECI_AGENT4_TCTRL	0xD4
#define W83793_BANK0_PECI_RETURN_DOMAIN	0xD5
#define W83793_BANK0_PECI_WARN_FLAGS	0xD6

#define W83793_BANK0_PECI_AGENT1_REL_TEMP_HI	0xD8
#define W83793_BANK0_PECI_AGENT1_REL_TEMP_LO	0xD9
#define W83793_BANK0_PECI_AGENT2_REL_TEMP_HI	0xDA
#define W83793_BANK0_PECI_AGENT2_REL_TEMP_LO	0xDB
#define W83793_BANK0_PECI_AGENT3_REL_TEMP_HI	0xDC
#define W83793_BANK0_PECI_AGENT3_REL_TEMP_LO	0xDD
#define W83793_BANK0_PECI_AGENT4_REL_TEMP_HI	0xDE
#define W83793_BANK0_PECI_AGENT4_REL_TEMP_LO	0xDF

extern int hwmon_wr(int fd, uint8_t i2c_bus, uint16_t i2c_addr, uint8_t offset, uint8_t val );
extern int hwmon_rd(int fd, uint8_t i2c_bus, uint16_t i2c_addr, uint8_t offset, uint8_t *val );

#endif // _W83793_H_
