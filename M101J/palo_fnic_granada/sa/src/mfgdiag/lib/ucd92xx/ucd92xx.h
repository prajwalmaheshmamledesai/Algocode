/********************************************************************
 *
 *      File:   ucd92xx.h
 *      Name:   Sudharshan Kadari
 *
 *      Description:
 *      ucd92xx header file. 
 *
 *
 * Copyright (c) 1985-2009 by Cisco Systems, Inc.
 * All rights reserved.
 *
 *
 *********************************************************************/

#ifndef _UCD92XX_H_
#define _UCD92XX_H_

#define PMBUS_PAGE			0x00
#define PMBUS_OPERATION			0x01
	#define PMBUS_OPERATION_IMMEDIATE_OFF	0x00
	#define PMBUS_OPERATION_SOFT_OFF	0x40
	#define PMBUS_OPERATION_ON		0x80
	#define PMBUS_OPERATION_MARGIN_LOW	0x94
	#define PMBUS_OPERATION_MARGIN_LOW_FLT	0x98
	#define PMBUS_OPERATION_MARGIN_HI	0xA4
	#define PMBUS_OPERATION_MARGIN_HI_FLT	0xA8

#define PMBUS_ON_OFF_CONFIG		0x02
	#define PMBUS_CONFIG_OPERATION_CTRL	0x10
	#define PMBUS_CONFIG_SERIAL_CTRL	0x08
	#define PMBUS_CONFIG_CONTROL_CTRL	0x04
	#define PMBUS_CONFIG_POLARITY_CTRL	0x02
	#define PMBUS_CONFIG_DELAY_CTRL		0x01

#define PMBUS_CLEAR_FAULTS		0x03
#define PMBUS_PHASE			0x04

#define PMBUS_WRITE_PROTECT		0x10
	#define PMBUS_WRITE_PROTECT_WP_ONLY		0x80
	#define PMBUS_WRITE_PROTECT_WP_OP_PG		0x40
	#define PMBUS_WRITE_PROTECT_WP_OP_PG_CFG_VOUT	0x20
	#define PMBUS_WRITE_PROTECT_WP_ALL		0x01
	
#define PMBUS_STORE_DEFAULT_ALL		0x11
#define PMBUS_RESTORE_DEFAULT_ALL	0x12
#define PMBUS_STORE_DEFAULT_CODE	0x13
#define PMBUS_RESTORE_DEFAULT_CODE	0x14
#define PMBUS_STORE_USER_ALL		0x15
#define PMBUS_RESTORE_USER_ALL		0x16
#define PMBUS_STORE_USER_CODE		0x17
#define PMBUS_RESTORE_USER_CODE		0x18
#define PMBUS_CAPABILITY		0x19
#define PMBUS_QUERY			0x1A

#define PMBUS_VOUT_MODE			0x20
#define PMBUS_VOUT_COMMAND		0x21
#define PMBUS_VOUT_TRIM			0x22
#define PMBUS_VOUT_CAL_OFFSET		0x23
#define PMBUS_VOUT_MAX			0x24
#define PMBUS_VOUT_MARGIN_HI		0x25
#define PMBUS_VOUT_MARGIN_LO		0x26
#define PMBUS_VOUT_TRANSITION_RATE	0x27
#define PMBUS_VOUT_DROOP		0x28
#define PMBUS_VOUT_SCALE_LOOP		0x29
#define PMBUS_VOUT_SCALE_MONITOR	0x2A

#define PMBUS_COEFFICIENTS		0x30
#define PMBUS_POUT_MAX			0x31
#define PMBUS_MAX_DUTY			0x32
#define PMBUS_FREQUENCY_SWITCH		0x33
#define PMBUS_VIN_ON			0x35
#define PMBUS_VIN_OFF			0x36
#define PMBUS_INTERLEAVE		0x37
#define PMBUS_IOUT_CAL_GAIN		0x38
#define PMBUS_IOUT_CAL_OFFSET		0x39
#define PMBUS_FAN_CONFIG_1_2		0x3A
#define PMBUS_FAN_CONFIG_1_2_FLD_FAN1_INSTALLED		7
#define PMBUS_FAN_CONFIG_1_2_FLD_FAN1_RPM		6
#define PMBUS_FAN_CONFIG_1_2_FLD_FAN1_PULSE_PER_REV_E	5
#define PMBUS_FAN_CONFIG_1_2_FLD_FAN1_PULSE_PER_REV_S	4

#define PMBUS_FAN_COMMAND_1		0x3B
#define PMBUS_FAN_COMMAND_2		0x3C
#define PMBUS_FAN_CONFIG_3_4		0x3D
#define PMBUS_FAN_COMMAND_3		0x3E
#define PMBUS_FAN_COMMAND_4		0x3F

#define PMBUS_VOUT_OV_FAULT_LIMIT	0x40
#define PMBUS_VOUT_OV_FAULT_RESPONSE	0x41
#define PMBUS_VOUT_OV_WARN_LIMIT	0x42
#define PMBUS_VOUT_UV_WARN_LIMIT	0x43
#define PMBUS_VOUT_UV_FAULT_LIMIT	0x44
#define PMBUS_VOUT_UV_FAULT_RESPONSE	0x45
#define PMBUS_VOUT_OC_FAULT_LIMIT	0x46
#define PMBUS_VOUT_OC_FAULT_RESPONSE	0x47
#define PMBUS_VOUT_OC_LV_FAULT_LIMIT	0x48
#define PMBUS_VOUT_OC_LV_FAULT_RESPONSE	0x49
#define PMBUS_IOUT_OC_WARN_LIMIT	0x4A
#define PMBUS_IOUT_UC_FAULT_LIMIT	0x4B
#define PMBUS_IOUT_UC_FAULT_RESPONSE	0x4C
#define PMBUS_VOUT_OT_FAULT_LIMIT	0x4F

#define PMBUS_VOUT_OT_FAULT_RESPONSE	0x50
#define PMBUS_VOUT_OT_WARN_LIMIT	0x51
#define PMBUS_VOUT_UT_WARN_LIMIT	0x52
#define PMBUS_VOUT_UT_FAULT_LIMIT	0x53
#define PMBUS_VOUT_UT_FAULT_RESPONSE	0x54
#define PMBUS_VIN_OV_FAULT_LIMIT	0x55
#define PMBUS_VIN_OV_FAULT_RESPONSE	0x56
#define PMBUS_VIN_OV_WARN_LIMIT		0x57
#define PMBUS_VIN_UV_WARN_LIMIT		0x58
#define PMBUS_VIN_UV_FAULT_LIMIT	0x59
#define PMBUS_VIN_UV_FAULT_RESPONSE	0x5A
#define PMBUS_IIN_OC_FAULT_LIMIT	0x5B
#define PMBUS_IIN_OC_FAULT_RESPONSE	0x5C
#define PMBUS_IIN_OC_WARN_LIMIT		0x5D
#define PMBUS_POWER_GOOD_ON		0x5E
#define PMBUS_POWER_GOOD_OFF		0x5F

#define PMBUS_TON_DELAY			0x60
#define PMBUS_TON_RISE			0x61
#define PMBUS_TON_MAX_FAULT_LIMIT	0x62
#define PMBUS_TON_MAX_FAULT_RESPONSE	0x63
#define PMBUS_TOFF_DELAY		0x64
#define PMBUS_TOFF_FALL			0x65
#define PMBUS_TOFF_MAX_WARN_LIMIT	0x66
#define PMBUS_POUT_OP_FAULT_LIMIT	0x68
#define PMBUS_POUT_OP_FAULT_RESPONSE	0x69
#define PMBUS_POUT_OP_WARN_LIMIT	0x6A
#define PMBUS_PIN_OP_WARN_LIMIT		0x6B

#define PMBUS_STATUS_BYTE		0x78
	#define PMBUS_STATUS_BYTE_BUSY		0x80
	#define PMBUS_STATUS_BYTE_OFF		0x40
	#define PMBUS_STATUS_BYTE_VOUT_OV	0x20
	#define PMBUS_STATUS_BYTE_IOUT_OC	0x10
	#define PMBUS_STATUS_BYTE_VIN_UV	0x08
	#define PMBUS_STATUS_BYTE_TEMPERATURE	0x04
	#define PMBUS_STATUS_BYTE_CML		0x02
	#define PMBUS_STATUS_BYTE_OTHER		0x01

#define PMBUS_STATUS_WORD		0x79
	#define PMBUS_STATUS_HI_BYTE_VOUT_FLT	0x80
	#define PMBUS_STATUS_HI_BYTE_IOUT_FLT	0x40
	#define PMBUS_STATUS_HI_BYTE_INC_FLT	0x20
	#define PMBUS_STATUS_HI_BYTE_MFR_FLT	0x10
	#define PMBUS_STATUS_HI_BYTE_PWR_GOOD	0x08
	#define PMBUS_STATUS_HI_BYTE_OTHER	0x01

#define PMBUS_STATUS_VOUT		0x7A
#define PMBUS_STATUS_IOUT		0x7B
#define PMBUS_STATUS_INPUT		0x7C
#define PMBUS_STATUS_TEMPERATURE	0x7D
#define PMBUS_STATUS_CML		0x7E
#define PMBUS_STATUS_OTHER		0x7F

#define PMBUS_STATUS_MFR_SPECIFIC	0x80
#define PMBUS_STATUS_FAN_1_2		0x81
#define PMBUS_STATUS_FAN_3_4		0x82
#define PMBUS_READ_VIN			0x88
#define PMBUS_READ_IIN			0x89
#define PMBUS_READ_VCAP			0x8A
#define PMBUS_READ_VOUT			0x8B
#define PMBUS_READ_IOUT			0x8C
#define PMBUS_READ_TEMPERATURE_1	0x8D
#define PMBUS_READ_TEMPERATURE_2	0x8E
#define PMBUS_READ_TEMPERATURE_3	0x8F

#define PMBUS_READ_FAN_SPEED_1		0x90
#define PMBUS_READ_FAN_SPEED_2		0x91
#define PMBUS_READ_FAN_SPEED_3		0x92
#define PMBUS_READ_FAN_SPEED_4		0x93
#define PMBUS_READ_DUTY_CYCLE		0x94
#define PMBUS_READ_FREQUENCY		0x95
#define PMBUS_READ_POUT			0x96
#define PMBUS_READ_PIN			0x97
#define PMBUS_PMBUS_REVISION		0x98
#define PMBUS_MFR_ID			0x99
#define PMBUS_MFR_MODEL			0x9A
#define PMBUS_MFR_REVISION		0x9B
#define PMBUS_MFR_LOCATION		0x9C
#define PMBUS_MFR_DATE			0x9D
#define PMBUS_MFR_SERIAL		0x9E

#define PMBUS_MFR_VIN_MIN		0xA0
#define PMBUS_MFR_VIN_MAX		0xA1
#define PMBUS_MFR_IIN_MAX		0xA2
#define PMBUS_MFR_PIN_MAX		0xA3
#define PMBUS_MFR_VOUT_MIN		0xA4
#define PMBUS_MFR_VOUT_MAX		0xA5
#define PMBUS_MFR_IOUT_MAX		0xA6
#define PMBUS_MFR_POUT_MAX		0xA7
#define PMBUS_MFR_TAMBIENT_MAX		0xA8
#define PMBUS_MFR_TAMBIENT_MIN		0xA9

#define PMBUS_USER_DATA_00		0xB0
#define PMBUS_USER_DATA_01		0xB1
#define PMBUS_USER_DATA_02		0xB2
#define PMBUS_USER_DATA_03		0xB3
#define PMBUS_USER_DATA_04		0xB4
#define PMBUS_USER_DATA_05		0xB5
#define PMBUS_USER_DATA_06		0xB6
#define PMBUS_USER_DATA_07		0xB7
#define PMBUS_USER_DATA_08		0xB8
#define PMBUS_USER_DATA_09		0xB9
#define PMBUS_USER_DATA_10		0xBA
#define PMBUS_USER_DATA_11		0xBB
#define PMBUS_USER_DATA_12		0xBC
#define PMBUS_USER_DATA_13		0xBD
#define PMBUS_USER_DATA_14		0xBE
#define PMBUS_USER_DATA_15		0xBF

#define PMBUS_SEQ_TIMEOUT		0xD0
#define PMBUS_VOUT_CAL_MONITOR		0xD1
#define PMBUS_PHASE_INFO		0xD2
#define PMBUS_VIN_SCALE_MONITOR		0xD3
#define PMBUS_CLA_BLANK			0xD4
#define PMBUS_CLA_GAINS			0xD5
#define PMBUS_PAGE_ISOLATED		0xD6
#define PMBUS_EADC_SAMPLE_TRIGGER	0xD7
#define PMBUS_ACTIVATE_CLA_BLANK	0xD8
#define PMBUS_ROM_MODE			0xD9
#define PMBUS_USER_RAM_00		0xDA
#define PMBUS_SOFT_RESET		0xDB
#define PMBUS_IIN_SCALE_MONITOR		0xDC
#define PMBUS_THERMAL_COEF		0xDD
#define PMBUS_PHASE_ENABLE		0xDE
#define PMBUS_DRIVER_MIN_PULSE		0xDF
#define PMBUS_MIN_DUTY			0xE0
#define PMBUS_SYNC_IN_OUT		0xE1
#define PMBUS_PARM_INFO			0xE2
#define PMBUS_PARM_VALUE		0xE3
#define PMBUS_TEMPERATURE_CAL_GAIN	0xE4
#define PMBUS_TEMPERATURE_CAL_OFFSET	0xE5
#define PMBUS_TRACKING_SOURCE		0xE6
#define PMBUS_TRACKING_SCALE_MONITOR	0xE7
#define PMBUS_FAN_SPEED_FAULT_LIMIT	0xE8
#define PMBUS_LOGGED_PEAKS		0xE9
#define PMBUS_LOGGED_FAULTS		0xEA
#define PMBUS_LIGHT_LOAD_LIMIT_HIGH	0xEB
#define PMBUS_DEVICE_ID_0		0xEC
#define PMBUS_LIGHT_LOAD_CONFIG		0xED
#define PMBUS_PREBIAS_OFFSET		0xED
#define PMBUS_PREBIAS_GAIN		0xEF
#define PMBUS_EXECUTE_FLASH		0xF0
#define PMBUS_MFR_SETUP_PASSWORD	0xF1
#define PMBUS_DISABLE_SECURITY		0xF2
#define PMBUS_GPIO_SEQ_CONFIG		0xF3
#define PMBUS_MFR_SECURITY_BIT_MASK	0xF4
#define PMBUS_TEMP_BALANCE_IMIN		0xF5
#define PMBUS_LIGHT_LOAD_LIMIT_LOW	0xF6
#define PMBUS_FAST_OC_FAULT_LIMIT	0xF7
#define PMBUS_MFR_SPECIFIC_40		0xF8
#define PMBUS_MFR_SPECIFIC_41		0xF9
#define PMBUS_MFR_SPECIFIC_42		0xFA
#define PMBUS_MFR_SPECIFIC_43		0xFB
#define PMBUS_EADC_TRIM			0xFC
#define PMBUS_DEVICE_ID_1		0xFD
#define PMBUS_MFR_SPECIFIC_EXT_CMD	0xFE
#define PMBUS_PMBUS_EXT_CMD		0xFF
#endif // _UCD92XX_H_
