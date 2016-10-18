/********************************************************************
 *
 *      File:  psu.h
 *      Name:  Matt Strathman
 *
 *      Description: psu private include file
 *
 * Copyright (c) 2007 by Cisco Systems, Inc.
 * All rights reserved.
 *  
 *  
 * $Id:$
 * $Source:$
 * $Author:$
 * ---------------------------------------------------------------
 **********************************************************************/
/**********************************************************************
@file    psu.h
@author  Matt Strathman -- mstrathman@nuovasystems.com
@brief   psu private include file
************************************************************************/
#ifndef __PSU__H_
#define __PSU__H_
#include "psu_api.h"

/*********************************************/
/*            PSMI Capabilities              */
/*********************************************/
#define PS_REG_THERM_SENS_CFG    0x06
    #define PS_REG_MSK_TSENS_QNT    0x0007   
    #define PS_REG_MSK_FANS_QNT     0x0038
    #define PS_REG_MSK_FANC_QNT     0x01C0
    #define PS_REG_MSK_FANT         0x0200
    #define PS_REG_MSK_CTRL_ASSC    0x0400

#define PS_REG_TSENS_TYPE        0x07
    #define PS_REG_MSK_T1           0x000F
    #define PS_REG_MSK_T2           0x00F0
    #define PS_REG_MSK_T3           0x0F00
    #define PS_REG_MSK_T4           0xF000

#define PS_REG_T1_OFFSET         0x08
    #define PS_REG_MSK_T1_OFFSET    0xFFFF

#define PS_REG_T2_OFFSET         0x09
    #define PS_REG_MSK_T2_OFFSET    0xFFFF

#define PS_REG_T3_OFFSET         0x0A
    #define PS_REG_MSK_T3_OFFSET    0xFFFF

#define PS_REG_T4_OFFSET         0x0B
    #define PS_REG_MSK_T4_OFFSET    0xFFFF

#define PS_REG_F1F2_SPD_RES      0x0C
    #define PS_REG_MSK_F1_CNT_RPM   0x0001
    #define PS_REG_MSK_F1_CPM       0x00FE
    #define PS_REG_MSK_F2_CNT_RPM   0x0100
    #define PS_REG_MSK_F2_CPM       0xFE00

#define PS_REG_F3F4_SPD_RES      0x0D
    #define PS_REG_MSK_F3_CNT_RPM   0x0001
    #define PS_REG_MSK_F3_CPM       0x00FE
    #define PS_REG_MSK_F4_CNT_RPM   0x0100
    #define PS_REG_MSK_F4_CPM       0xFE00

#define PS_REG_F1_CFG            0x0E
#define PS_REG_F2_CFG            0x0F
#define PS_REG_F3_CFG            0x10
#define PS_REG_F4_CFG            0x11
    #define PS_REG_MSK_CFG       0x0001
    #define PS_REG_MSK_MAX_SPD   0xFFFE
 
#define PS_REG_VOLT_CUR_SENS_CFG 0x12
    #define PS_REG_MSK_PS_OUT_QNT   0x000F
    #define PS_REG_MSK_VSENS        0x0010
    #define PS_REG_MSK_OUT_CUR_SENS 0x01E0
    #define PS_REG_MSK_IN_CUR_SENS  0x0200
    #define PS_REG_MSK_PK_CUR_SENS  0x0400
    #define PS_REG_MSK_IN_VSENS     0x0800

#define PS_REG_FAN_CTRL          0x13
    #define PS_REG_MSK_F1_CTRL      0x000F
    #define PS_REG_MSK_F2_CTRL      0x00F0
    #define PS_REG_MSK_F3_CTRL      0x0F00
    #define PS_REG_MSK_F4_CTRL      0xF000

#define PS_REG_F1F2_T_ASSC       0x14
    #define PS_REG_MSK_F1_T_ASSC    0x00FF
    #define PS_REG_MSK_F2_T_ASSC    0xFF00

#define PS_REG_F3F4_T_ASSC       0x15
    #define PS_REG_MSK_F3_T_ASSC    0x00FF
    #define PS_REG_MSK_F4_T_ASSC    0xFF00

#define PS_REG_SHUT_DOWN_EVT     0x16
    #define PS_REG_MSK_FAIL         0x0001
    #define PS_REG_MSK_OVER_CUR     0x0002
    #define PS_REG_MSK_OVER_T       0x0004
    #define PS_REG_MSK_AC_LOSS      0x0008
    #define PS_REG_MSK_F1_FAIL      0x0010
    #define PS_REG_MSK_F2_FAIL      0x0020
    #define PS_REG_MSK_F3_FAIL      0x0040
    #define PS_REG_MSK_F4_FAIL      0x0080

#define PS_REG_STATUS            0x17
    #define PS_REG_MSK_F1_OVERRIDE  0x0001
    #define PS_REG_MSK_F2_OVERRIDE  0x0002
    #define PS_REG_MSK_F3_OVERRIDE  0x0004
    #define PS_REG_MSK_F4_OVERRIDE  0x0008
    #define PS_REG_MSK_PWOK         0x0010
    #define PS_REG_MSK_PSON         0x0020
    #define PS_REG_MSK_INTR         0x0040
    #define PS_REG_MSK_REDUDANT     0x0080
    #define PS_REG_MSK_IN_RANGE     0x0100

#define PS_REG_CTRL              0x18
    #define PS_REG_MSK_F1_CTRL_MODE 0x0001
    #define PS_REG_MSK_F2_CTRL_MODE 0x0002
    #define PS_REG_MSK_F3_CTRL_MODE 0x0004
    #define PS_REG_MSK_F4_CTRL_MODE 0x0008
    #define PS_REG_MSK_LED_CTRL     0x0010
    #define PS_REG_MSK_PS_REC       0x0100
    #define PS_REG_MSK_V_PSMI_DEV   0x0200
    #define PS_REG_MSK_CSTM_FTR     0x0400

#define PS_REG_WARNING           0x19
    #define PS_REG_MSK_CTRL_TEMP    0x0003
    #define PS_REG_MSK_ATEMP        0x000C
    #define PS_REG_MSK_F_FAIL       0x0070
    #define PS_REG_MSK_OUT_CUR      0x0780
    #define PS_REG_MSK_IN_V         0x0800
    #define PS_REG_MSK_IN_CUR       0x1000

/*********************************************/
/*               PSMI Header                 */
/*********************************************/
#define PS_REG_DISCOVERY_KEY1_2  0x3E
#define PS_REG_DISCOVERY_KEY3_4  0x3F
    #define PS_REG_MSK_KEY1         0x00FF
    #define PS_REG_MAK_KEY2         0xFF00

#define PS_REG_PSMI_REV          0x40
    #define PS_REG_MSK_PSMI_MAJOR   0x00FF
    #define PS_REG_MSK_PSMI_MINOR   0xFF00

#define PS_REG_PS_REV            0x41
    #define PS_REG_MSK_PS_MAJOR     0x00FF
    #define PS_REG_MSK_PS_MINOR     0xFF00

#define PS_REG_SUPPLIER_ID0      0x42
#define PS_REG_SUPPLIER_ID1      0x43

/*********************************************/
/*               Sensor Data                 */
/*********************************************/
#define PS_REG_T1                0x00
#define PS_REG_T2                0x01
#define PS_REG_T3                0x02
#define PS_REG_T4                0x03
    #define PS_REG_MSK_FRAC_VAL     0x003F
    #define PS_REG_MSK_INT_VAL      0x7FC0
    #define PS_REG_MSK_SIGN         0x8000

#define PS_REG_F1_SENS           0x20
#define PS_REG_F2_SENS           0x21
#define PS_REG_F3_SENS           0x22
#define PS_REG_F4_SENS           0x23

#define PS_REG_F1_CTRL           0x24
#define PS_REG_F2_CTRL           0x25
#define PS_REG_F3_CTRL           0x26
#define PS_REG_F4_CTRL           0x27

#define PS_REG_V1_SENS           0x28
#define PS_REG_V2_SENS           0x29
#define PS_REG_V3_SENS           0x2A
#define PS_REG_V4_SENS           0x2B
#define PS_REG_V5_SENS           0x2C
#define PS_REG_V6_SENS           0x2D
#define PS_REG_V7_SENS           0x2E
#define PS_REG_V8_SENS           0x2F
#define PS_REG_V9_SENS           0x30
#define PS_REG_V10_SENS          0x31
    #define PS_REG_MSK_FRAC_127     0x00FF
    #define PS_REG_MSK_INT_127      0x7F00
    #define PS_REG_MSK_FRAC_512     0x001F
    #define PS_REG_MSK_INT_512      0x7FE0

#define PS_REG_OUT_C1_SENS       0x33
#define PS_REG_OUT_C2_SENS       0x34
#define PS_REG_OUT_C3_SENS       0x35
#define PS_REG_OUT_C4_SENS       0x36
#define PS_REG_OUT_C5_SENS       0x37
#define PS_REG_OUT_C6_SENS       0x38
#define PS_REG_OUT_C7_SENS       0x39
#define PS_REG_OUT_C8_SENS       0x3A
#define PS_REG_OUT_C9_SENS       0x3B
#define PS_REG_OUT_C10_SENS      0x3C

#define PS_REG_IN_C1_SENS        0x3D

#define PS_REG_PEAK_C1_SENS      0x53
#define PS_REG_PEAK_C2_SENS      0x54
#define PS_REG_PEAK_C3_SENS      0x55
#define PS_REG_PEAK_C4_SENS      0x56
#define PS_REG_PEAK_C5_SENS      0x57
#define PS_REG_PEAK_C6_SENS      0x58
#define PS_REG_PEAK_C7_SENS      0x59
#define PS_REG_PEAK_C8_SENS      0x5A
#define PS_REG_PEAK_C9_SENS      0x5B
#define PS_REG_PEAK_C10_SENS     0x5C

#define PS_REG_IN_PEAK_C1_SENS   0x5D

/*********************************************/
/*          Status/Warning/Control           */
/*********************************************/
#define PS_REG_SHUTDOWN_EVT_STATUS 0x5E
#define PS_REG_WARNING_THERMAL     0x5F
    #define PS_REG_MSK_T1_WARNING     0x0001
    #define PS_REG_MSK_T2_WARNING     0x0002
    #define PS_REG_MSK_T3_WARNING     0x0004
    #define PS_REG_MSK_T4_WARNING     0x0008
    #define PS_REG_MSK_F1_WARNING     0x0010
    #define PS_REG_MSK_F2_WARNING     0x0020
    #define PS_REG_MSK_F3_WARNING     0x0040
    #define PS_REG_MSK_F4_WARNING     0x0080

#define PS_REG_WARNING_OUTPUT      0x60
    #define PS_REG_MSK_C1_WARNING     0x0001
    #define PS_REG_MSK_C2_WARNING     0x0002
    #define PS_REG_MSK_C3_WARNING     0x0004
    #define PS_REG_MSK_C4_WARNING     0x0008
    #define PS_REG_MSK_C5_WARNING     0x0010
    #define PS_REG_MSK_C6_WARNING     0x0020
    #define PS_REG_MSK_C7_WARNING     0x0040
    #define PS_REG_MSK_C8_WARNING     0x0080
    #define PS_REG_MSK_C9_WARNING     0x0100
    #define PS_REG_MSK_C10_WARNING    0x0200

#define PS_REG_WARNING_INPUT       0x61
    #define PS_REG_MSK_OVER_CUR_WARN  0x0001
    #define PS_REG_MSK_UNDER_V_WARN   0x0008

#define PS_REG_STATUS_EVT          0x62
    #define PS_REG_MSK_RPM1_OVERRIDE  0x0002
    #define PS_REG_MSK_RPM2_OVERRIDE  0x0004
    #define PS_REG_MSK_RPM3_OVERRIDE  0x0008
    #define PS_REG_MSK_RPM4_OVERRIDE  0x0010
    #define PS_REG_MSK_PWOK_EVT       0x0020
    #define PS_REG_MSK_PSON_EVT       0x0040
    #define PS_REG_MSK_INTERRUPT_EVT  0x0080
    #define PS_REG_MSK_OP_RANGE       0x0300
    #define PS_REG_MSK_REDUNDANCY     0x0400

#define PS_REG_CONTROL             0x63 
    #define PS_REG_MSK_SHUTDOWN_LED    0x0010
    #define PS_REG_MSK_WARNING_LED     0x0020
    #define PS_REG_MSK_SHUTDOWN_EVT_EN 0x0040
    #define PS_REG_MSK_WARNING_EVT_EN  0x0080

/*********************************************/
/*            Capability Records             */
/*********************************************/
/* Max Temperature */
#define PS_REG_T1_TARGET         0x64
#define PS_REG_T2_TARGET         0x65
#define PS_REG_T3_TARGET         0x66
#define PS_REG_T4_TARGET         0x67

/* Fan Operatin Minimums */
#define PS_REG_F1_MIN            0x68
#define PS_REG_F2_MIN            0x69
#define PS_REG_F3_MIN            0x6A
#define PS_REG_F4_MIN            0x6B

/* Sound Power Capability */
#define PS_REG_SOUND_POWER       0x6C
    #define PS_REG_MSK_MAX_FAN_SOUND 0x00FF
    #define PS_REG_MSK_MIN_FAN_SOUND 0xFF00

/* Voltage Ouput Limits */
#define PS_REG_V1_MAX             0x6D
#define PS_REG_V1_MIN             0x6E
#define PS_REG_V2_MAX             0x6F
#define PS_REG_V2_MIN             0x70
#define PS_REG_V3_MAX             0x71
#define PS_REG_V3_MIN             0x72
#define PS_REG_V4_MAX             0x73
#define PS_REG_V4_MIN             0x74
#define PS_REG_V5_MAX             0x75
#define PS_REG_V5_MIN             0x76
#define PS_REG_V6_MAX             0x77
#define PS_REG_V6_MIN             0x78
#define PS_REG_V7_MAX             0x79

//#define PS_REG_V7_MIN             0x7A
//#define PS_REG_V8_MAX             0x7B
//#define PS_REG_V8_MIN             0x7C
//#define PS_REG_V9_MAX             0x7D
//#define PS_REG_V9_MIN             0x7E
//#define PS_REG_V10_MAX            0x7F
//#define PS_REG_V10_MIN            0x80

/* Current Ouput Capability */
#define PS_REG_FIRST_OUTPUT_C1    0x7A
#define PS_REG_FIRST_OUTPUT_C2    0x7B
#define PS_REG_FIRST_OUTPUT_C3    0x7C
#define PS_REG_FIRST_OUTPUT_C4    0x7D
#define PS_REG_FIRST_OUTPUT_C5    0x7E
#define PS_REG_FIRST_OUTPUT_C6    0x7F
#define PS_REG_FIRST_OUTPUT_C7    0x80
#define PS_REG_FIRST_OUTPUT_C8    0x81
#define PS_REG_FIRST_OUTPUT_C9    0x82
#define PS_REG_FIRST_OUTPUT_C10   0x83

#define PS_REG_SECOND_OUTPUT_C1   0x84
#define PS_REG_SECOND_OUTPUT_C2   0x85
#define PS_REG_SECOND_OUTPUT_C3   0x86
#define PS_REG_SECOND_OUTPUT_C4   0x87
#define PS_REG_SECOND_OUTPUT_C5   0x88
#define PS_REG_SECOND_OUTPUT_C6   0x89
#define PS_REG_SECOND_OUTPUT_C7   0x8A
#define PS_REG_SECOND_OUTPUT_C8   0x8B
#define PS_REG_SECOND_OUTPUT_C9   0x8C
#define PS_REG_SECOND_OUTPUT_C10  0x8D

#define PS_REG_THIRD_OUTPUT_C1    0x8E
#define PS_REG_THIRD_OUTPUT_C2    0x8F
#define PS_REG_THIRD_OUTPUT_C3    0x90
#define PS_REG_THIRD_OUTPUT_C4    0x91
#define PS_REG_THIRD_OUTPUT_C5    0x92
#define PS_REG_THIRD_OUTPUT_C6    0x93
#define PS_REG_THIRD_OUTPUT_C7    0x94
#define PS_REG_THIRD_OUTPUT_C8    0x95
#define PS_REG_THIRD_OUTPUT_C9    0x96
#define PS_REG_THIRD_OUTPUT_C10   0x97

/* Current Input Capability */
#define PS_REG_FIRST_INPUT_MAX    0x98
#define PS_REG_SECOND_INPUT_MAX   0x99
#define PS_REG_THIRD_INPUT_MAX    0x9A

#define PS_REG_FIRST_INPUT_MIN    0x9B
#define PS_REG_SECOND_INPUT_MIN   0x9C
#define PS_REG_THIRD_INPUT_MIN    0x9D

/* Total Output Power */
#define PS_REG_FIRST_OUT_PWR_MAX  0x9E
#define PS_REG_SECOND_OUT_PWR_MAX 0x9F
#define PS_REG_THIRD_OUT_PWR_MAX  0xA0

/* Combined Power Limits */
#define PS_REG_COMBO_OUT_LIMIT_1  0xA1
#define PS_REG_COMBO_OUTPUTS_1    0xA2
#define PS_REG_COMBO_OUT_LIMIT_2  0xA3
#define PS_REG_COMBO_OUTPUTS_2    0xA4

/* Current Sensor Bandwidths */
#define PS_REG_OUT_CUR_BW         0xA5
#define PS_REG_IN_CUR_BW          0xA6

/* Power Supply Efficiency Curve */
#define PS_REG_L_OUT_PWR_HI_L     0xA7
#define PS_REG_M_OUT_PWR_HI_L     0xA8
#define PS_REG_H_OUT_PWR_HI_L     0xA9
#define PS_REG_LOAD_EFFNCY_HI_L_1 0xAA
    #define PS_REG_MSK_L_LOAD_EFFNCY 0x00FF
    #define PS_REG_MSK_M_LOAD_EFFNCY 0xFF00
#define PS_REG_LOAD_EFFNCY__HI_L_2 0xAB
    #define PS_REG_MSK_H_LOAD_EFFNCY 0x00FF

#define PS_REG_L_OUT_PWR_LO_L     0xAC
#define PS_REG_M_OUT_PWR_LO_L     0xAD
#define PS_REG_H_OUT_PWR_LO_L     0xAE
#define PS_REG_LOAD_EFFNCY_LO_L_1 0xAF
#define PS_REG_LOAD_EFFNCY_LO_L_2 0xB0

/* Load Share Error */
#define PS_REG_LOAD_SHR_ERR_V1    0xB1
#define PS_REG_LOAD_SHR_ERR_V2    0xB2
#define PS_REG_LOAD_SHR_ERR_V3    0xB3
#define PS_REG_LOAD_SHR_ERR_V4    0xB4

#define PS_REG_REDUNDANT_CFG      0xB5
    #define PS_REG_MSK_MAX_PS_QUANT  0x00FF
    #define PS_REG_MSK_MIN_PS_QUANT  0xFF00

#ifdef cplusplus
extern "C" {
#endif

extern int
psu_get_min_inst_eugene ( void );

extern int
psu_get_max_inst_eugene ( void );

#ifdef cplusplus
}
#endif

#endif

