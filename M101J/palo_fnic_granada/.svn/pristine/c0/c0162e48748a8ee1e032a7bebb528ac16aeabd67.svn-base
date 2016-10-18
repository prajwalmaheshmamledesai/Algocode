/********************************************************************
 *
 *      File:  psu.c
 *      Name:  Matt Strathman
 *
 *      Description: psmi register map
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
@file    psu.c
@author  Matt Strathman -- mstrathman@nuovasystems.com
@brief   psmi register map
************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "psu.h"

static uint32_t ps_reg_psmi_capable[] = {
    PS_REG_THERM_SENS_CFG,
    PS_REG_TSENS_TYPE,
    PS_REG_T1_OFFSET,
    PS_REG_T2_OFFSET,
    PS_REG_T3_OFFSET,
    PS_REG_T4_OFFSET,
    PS_REG_F1F2_SPD_RES,
    PS_REG_F3F4_SPD_RES,
    PS_REG_F1_CFG,
    PS_REG_F2_CFG,
    PS_REG_F3_CFG,
    PS_REG_F4_CFG,
    PS_REG_VOLT_CUR_SENS_CFG,
    PS_REG_FAN_CTRL,
    PS_REG_F1F2_T_ASSC,
    PS_REG_F3F4_T_ASSC,
    PS_REG_SHUT_DOWN_EVT,
    PS_REG_STATUS,
    PS_REG_CTRL,
    PS_REG_WARNING,
};

static uint32_t ps_max_reg_psmi_capable = \
  sizeof ( ps_reg_psmi_capable ) / sizeof ( uint32_t ); 

static uint32_t ps_reg_sensors[] = {
    PS_REG_T1,
    PS_REG_T2,
    PS_REG_T3,
    PS_REG_T4,
    PS_REG_F1_SENS,
    PS_REG_F2_SENS,
    PS_REG_F3_SENS,
    PS_REG_F4_SENS,
    PS_REG_F1_CTRL,
    PS_REG_F2_CTRL,
    PS_REG_F3_CTRL,
    PS_REG_F4_CTRL,
    PS_REG_V1_SENS,
    PS_REG_V2_SENS,
    PS_REG_V3_SENS,
    PS_REG_V4_SENS,
    PS_REG_V5_SENS,
    PS_REG_V6_SENS,
    PS_REG_V7_SENS,
    PS_REG_V8_SENS,
    PS_REG_V9_SENS,
    PS_REG_V10_SENS,
    PS_REG_OUT_C1_SENS,
    PS_REG_OUT_C2_SENS,
    PS_REG_OUT_C3_SENS,
    PS_REG_OUT_C4_SENS,
    PS_REG_OUT_C5_SENS,
    PS_REG_OUT_C6_SENS,
    PS_REG_OUT_C7_SENS,
    PS_REG_OUT_C8_SENS,
    PS_REG_OUT_C9_SENS,
    PS_REG_OUT_C10_SENS,
    PS_REG_IN_C1_SENS,
    PS_REG_PEAK_C1_SENS,
    PS_REG_PEAK_C2_SENS,
    PS_REG_PEAK_C3_SENS,
    PS_REG_PEAK_C4_SENS,
    PS_REG_PEAK_C5_SENS,
    PS_REG_PEAK_C6_SENS,
    PS_REG_PEAK_C7_SENS,
    PS_REG_PEAK_C8_SENS,
    PS_REG_PEAK_C9_SENS,
    PS_REG_PEAK_C10_SENS,
    PS_REG_IN_PEAK_C1_SENS,
};

static uint32_t ps_max_reg_sensors = \
  sizeof ( ps_reg_sensors ) / sizeof ( uint32_t ); 

static uint32_t ps_reg_status[] = {
    PS_REG_SHUTDOWN_EVT_STATUS,
    PS_REG_WARNING_THERMAL,
    PS_REG_WARNING_OUTPUT,
    PS_REG_WARNING_INPUT,
    PS_REG_STATUS_EVT,
    PS_REG_CONTROL,
};

static uint32_t ps_max_reg_status = \
  sizeof ( ps_reg_status ) / sizeof ( uint32_t );

static uint32_t ps_reg_records[] = {
    PS_REG_T1_TARGET,
    PS_REG_T2_TARGET,
    PS_REG_T3_TARGET,
    PS_REG_T4_TARGET,
    PS_REG_F1_MIN,
    PS_REG_F2_MIN,
    PS_REG_F3_MIN,
    PS_REG_F4_MIN,
    PS_REG_SOUND_POWER,
    PS_REG_V1_MAX,
    PS_REG_V1_MIN,
    PS_REG_V2_MAX,
    PS_REG_V2_MIN,
    PS_REG_V3_MAX,
    PS_REG_V3_MIN,
    PS_REG_V4_MAX,
    PS_REG_V4_MIN,
    PS_REG_V5_MAX,
    PS_REG_V5_MIN,
    PS_REG_V6_MAX,
    PS_REG_V6_MIN,
    PS_REG_V7_MAX,
//    PS_REG_V7_MIN,
//    PS_REG_V8_MAX,
//    PS_REG_V8_MIN,
//    PS_REG_V9_MAX,
//    PS_REG_V9_MIN,
//    PS_REG_V10_MAX,
//    PS_REG_V10_MIN,
    PS_REG_FIRST_OUTPUT_C1,
    PS_REG_FIRST_OUTPUT_C2,
    PS_REG_FIRST_OUTPUT_C3,
    PS_REG_FIRST_OUTPUT_C4,
    PS_REG_FIRST_OUTPUT_C5,
    PS_REG_FIRST_OUTPUT_C6,
    PS_REG_FIRST_OUTPUT_C7,
    PS_REG_FIRST_OUTPUT_C8,
    PS_REG_FIRST_OUTPUT_C9,
    PS_REG_FIRST_OUTPUT_C10,
    PS_REG_SECOND_OUTPUT_C1,
    PS_REG_SECOND_OUTPUT_C2,
    PS_REG_SECOND_OUTPUT_C3,
    PS_REG_SECOND_OUTPUT_C4,
    PS_REG_SECOND_OUTPUT_C5,
    PS_REG_SECOND_OUTPUT_C6,
    PS_REG_SECOND_OUTPUT_C7,
    PS_REG_SECOND_OUTPUT_C8,
    PS_REG_SECOND_OUTPUT_C9,
    PS_REG_SECOND_OUTPUT_C10,
    PS_REG_THIRD_OUTPUT_C1,
    PS_REG_THIRD_OUTPUT_C2,
    PS_REG_THIRD_OUTPUT_C3,
    PS_REG_THIRD_OUTPUT_C4,
    PS_REG_THIRD_OUTPUT_C5,
    PS_REG_THIRD_OUTPUT_C6,
    PS_REG_THIRD_OUTPUT_C7,
    PS_REG_THIRD_OUTPUT_C8,
    PS_REG_THIRD_OUTPUT_C9,
    PS_REG_THIRD_OUTPUT_C10,
    PS_REG_FIRST_INPUT_MAX,
    PS_REG_SECOND_INPUT_MAX,
    PS_REG_THIRD_INPUT_MAX,
    PS_REG_FIRST_INPUT_MIN,
    PS_REG_SECOND_INPUT_MIN,
    PS_REG_THIRD_INPUT_MIN,
    PS_REG_FIRST_OUT_PWR_MAX,
    PS_REG_SECOND_OUT_PWR_MAX,
    PS_REG_THIRD_OUT_PWR_MAX,
    PS_REG_COMBO_OUT_LIMIT_1,
    PS_REG_COMBO_OUTPUTS_1,
    PS_REG_COMBO_OUT_LIMIT_2,
    PS_REG_COMBO_OUTPUTS_2,
    PS_REG_OUT_CUR_BW,
    PS_REG_IN_CUR_BW,
    PS_REG_L_OUT_PWR_HI_L,
    PS_REG_M_OUT_PWR_HI_L,
    PS_REG_H_OUT_PWR_HI_L,
    PS_REG_LOAD_EFFNCY_HI_L_1,
    PS_REG_LOAD_EFFNCY__HI_L_2,
    PS_REG_L_OUT_PWR_LO_L,
    PS_REG_M_OUT_PWR_LO_L,
    PS_REG_H_OUT_PWR_LO_L,
    PS_REG_LOAD_EFFNCY_LO_L_1,
    PS_REG_LOAD_EFFNCY_LO_L_2,
    PS_REG_LOAD_SHR_ERR_V1,
    PS_REG_LOAD_SHR_ERR_V2,
    PS_REG_LOAD_SHR_ERR_V3,
    PS_REG_LOAD_SHR_ERR_V4,
    PS_REG_REDUNDANT_CFG,
};

static uint32_t ps_max_reg_records = \
  sizeof ( ps_reg_records ) / sizeof ( uint32_t );


inline uint16_t
ps_reg_field_val ( uint16_t val, uint16_t msk ) 
{
    for ( ; !( msk & 0x1 ); msk>>=1, val>>=1);

    return ( val & msk ); 
}

int
ps_tsens_cnt (int fd, uint32_t ps_inst )
{
    uint16_t val = 0;
    int rc = 0;

    if ( ( rc = psu_rd ( fd, ps_inst,
       PS_REG_THERM_SENS_CFG,  ( uint8_t *)&val ) ) < 0 ) {
        return ( rc );
    }

    return ( ps_reg_field_val ( val, PS_REG_MSK_TSENS_QNT ) );
}

int
ps_fans_cnt (int fd, uint32_t ps_inst )
{
    uint16_t val = 0;
    int rc = 0;

    if ( ( rc = psu_rd ( fd, ps_inst,
       PS_REG_THERM_SENS_CFG,  ( uint8_t *)&val ) ) < 0 ) {
        return ( rc );
    }

    return ( ps_reg_field_val ( val, PS_REG_MSK_FANS_QNT ) );
}

int
ps_fanc_cnt (int fd, uint32_t ps_inst )
{
    uint16_t val = 0;
    int rc = 0;

    if ( ( rc = psu_rd ( fd, ps_inst,
       PS_REG_THERM_SENS_CFG,  ( uint8_t *)&val ) ) < 0 ) {
        return ( rc ); 
    }

    return ( ps_reg_field_val ( val, PS_REG_MSK_FANC_QNT ) );
}

int
ps_vsens_cnt (int fd, uint32_t ps_inst )
{
    uint16_t val = 0;
    int rc = 0;

    if ( ( rc = psu_rd ( fd, ps_inst,
      PS_REG_VOLT_CUR_SENS_CFG,  ( uint8_t *)&val ) ) < 0 ) {
        return ( rc );
    }

    return ( ps_reg_field_val ( val, PS_REG_MSK_PS_OUT_QNT ) );
}

int
ps_out_csens_cnt (int fd, uint32_t ps_inst )
{
    uint16_t val = 0;
    int rc = 0;

    if ( ( rc = psu_rd ( fd, ps_inst,
      PS_REG_VOLT_CUR_SENS_CFG,  ( uint8_t *)&val ) ) < 0 ) {
        return ( rc );
    }

    return ( ps_reg_field_val ( val, PS_REG_MSK_OUT_CUR_SENS ) );
}

int
ps_in_csens_cnt (int fd, uint32_t ps_inst )
{
    uint16_t val = 0;
    int rc = 0;

    if ( ( rc = psu_rd ( fd, ps_inst,
      PS_REG_VOLT_CUR_SENS_CFG,  ( uint8_t *)&val ) ) < 0 ) {
        return ( rc );
    }

    return ( ps_reg_field_val ( val, PS_REG_MSK_IN_CUR_SENS ) );
}

int
ps_peak_csens_enabled (int fd, uint32_t ps_inst )
{
    int rc = 0;
    uint16_t val = 0;
    
    if ( ( rc = psu_rd ( fd, ps_inst, PS_REG_VOLT_CUR_SENS_CFG, 
      ( uint8_t *)&val ) ) < 0 ) {
        return ( rc ); 
    }

    return ( ps_reg_field_val ( val, PS_REG_MSK_PK_CUR_SENS ) );
}

int
ps_show_psmi_header (int fd, uint32_t ps_inst )
{
    int rc = 0;
    uint16_t val = 0;

    if ( ( rc = psu_rd ( fd, ps_inst, PS_REG_DISCOVERY_KEY1_2,  
      ( uint8_t *)&val ) ) < 0 ) {
        return ( rc );
    }

    printf ( "%c%c\n", ( char)val, ( char)(val>>8) );
    if ( ( ( char )val !=  'P' ) && ( ( ( char ) ( val >> 8 ) ) != 'S') ) {
        fprintf ( stderr, "PS%d, Not PSMI Compatable\n", ps_inst ); 
        return ( 1 );
    }
    if ( ( rc = psu_rd ( fd, ps_inst, PS_REG_DISCOVERY_KEY3_4,  
      ( uint8_t *)&val ) ) < 0 ) {
        return ( rc );
    }
    if ( ( ( char )val !=  'M' ) && ( ( ( char ) ( val >> 8 ) ) != 'I') ) {
        fprintf ( stderr, "PS%d, Not PSMI Compatable\n", ps_inst ); 
        return ( 1 );
    }

    if ( ( rc = psu_rd ( fd, ps_inst, PS_REG_PSMI_REV, 
      ( uint8_t *)&val ) ) < 0 ) {
        return ( rc );
    }
    printf ( "( 0x%.02x ) PSMI Rev      : %.02x.%.02x\n", PS_REG_PSMI_REV,
      ps_reg_field_val ( val, PS_REG_MSK_PSMI_MAJOR ),
      ps_reg_field_val ( val, PS_REG_MSK_PSMI_MINOR ) );

    if ( ( rc = psu_rd ( fd, ps_inst, PS_REG_PS_REV, 
      ( uint8_t *)&val ) ) < 0 ) {
        return ( rc );
    }
    printf ( "( 0x%.02x ) PS Rev        : %.02x.%.02x\n", PS_REG_PS_REV,
      ps_reg_field_val ( val, PS_REG_MSK_PS_MAJOR ),
      ps_reg_field_val ( val, PS_REG_MSK_PS_MINOR ) );

    printf ( "( 0x%.02x-0x%.02x ) Vendor ID: ", PS_REG_SUPPLIER_ID0,
      PS_REG_SUPPLIER_ID1 );
    if ( ( rc = psu_rd ( fd, ps_inst, PS_REG_SUPPLIER_ID1, 
      ( uint8_t *)&val ) ) < 0 ) {
        return ( rc );
    }
    printf ( "0x%.02x", val );
    if ( ( rc = psu_rd ( fd, ps_inst, PS_REG_SUPPLIER_ID0, 
      ( uint8_t *)&val ) ) < 0 ) {
        return ( rc );
    }
    printf ( "%.02x\n", val );

    return ( 0 );
}

void
ps_tsens_to_c ( uint16_t *val )
{
   
    *val = ( ( *val & PS_REG_MSK_SIGN ) ? -1 : 1 ) * 
      ps_reg_field_val ( *val, PS_REG_MSK_INT_VAL );

    return;
}

void
ps_show_reg_temp ( uint32_t inst, uint16_t val )
{
    ps_tsens_to_c ( &val );

    printf ( "    T%d                 -> %dC\n", inst + 1, val );

    return; 
}

void
ps_show_reg_tach (  uint32_t inst, uint16_t cfg, uint16_t val )
{

    if ( cfg & ( PS_REG_MSK_F1_CNT_RPM << ( ( inst % 2 ) * 8 ) ) ) {
        printf ( "    F%d                 -> %d COUNTS\n", inst + 1, 
          ps_reg_field_val ( val, 
          ( inst % 2 ) ? PS_REG_MSK_F2_CPM : PS_REG_MSK_F1_CPM ) );
    } else {
        printf ( "    F%d                 -> %d RPM\n", inst + 1, val );
    }

    return;
}

void
ps_show_reg_fan_ctrl ( uint32_t inst, uint16_t cfg,  uint16_t val )
{
    if ( cfg & PS_REG_MSK_CFG ) {
        printf ( "    F%d                 -> %d PERCENT DC\n", inst + 1,
          ps_reg_field_val ( val,
          ( inst % 2 ) ? PS_REG_MSK_F2_CPM : PS_REG_MSK_F1_CPM ) );
    } else {
        printf ( "    F%d                 -> %d RPM\n", inst + 1, val );
    }

    return;
}

void
ps_show_reg_voltage ( uint32_t inst, uint16_t val )
{
    printf ( "    V%d                 -> %c%d.%u V\n", inst + 1,
      ( ps_reg_field_val ( val, PS_REG_MSK_SIGN ) ) ? '-':'+',
      ps_reg_field_val ( val, PS_REG_MSK_INT_127 ),
      ps_reg_field_val ( val, PS_REG_MSK_FRAC_127 ) );

    return;
}

void
ps_show_reg_current ( uint32_t inst, uint16_t val )
{
    printf ( "    C%d                 -> %u.%u A\n", inst + 1, val/64, val%64 );

    return;
}

void
ps_reg_show_thermal_sens_cfg ( uint16_t val )
{
    printf ( "( 0x%.02x ) Temp Sensor Cfg:\n", PS_REG_THERM_SENS_CFG );
    printf ( "    Temp Sensor Cnt    -> 0x%x\n", 
      ps_reg_field_val ( val, PS_REG_MSK_TSENS_QNT ) );
    printf ( "    Fan Sens Cnt       -> 0x%x\n", 
      ps_reg_field_val ( val, PS_REG_MSK_FANS_QNT ) );
    printf ( "    Fan Ctrl Cnt       -> 0x%x\n", 
      ps_reg_field_val ( val, PS_REG_MSK_FANC_QNT ) );
    printf ( "    Fan Ctrl Temp Assc -> 0x%x\n", 
      ps_reg_field_val ( val, PS_REG_MSK_FANT ) );
    printf ( "    Fan Ctrl Assc      -> 0x%x\n", 
      ps_reg_field_val ( val, PS_REG_MSK_CTRL_ASSC ) );
    
    return;
}

void
ps_reg_show_tsens_type ( uint32_t tsens_cnt, uint16_t val )
{
    uint32_t i, type;

    printf ( "( 0x%.02x ) Temp Sensor Types:\n", PS_REG_TSENS_TYPE );

    for ( i = 0; i < 4; i++ ) {
        type =  ps_reg_field_val ( val, PS_REG_MSK_T1 << ( 4 * i ) );
        printf ( "    T%d Type            -> %s\n", i,
          ( ( tsens_cnt <= i ) ? "NA"  :
          ( ( type == 0 ) ? "Internal" :
          ( ( type == 1 ) ? "Inlet"    :
          ( ( type == 2 ) ? "Outlet"   : "Unknown" ) ) ) ) );
    }

    return;
}

void
ps_show_reg_t_offset ( uint32_t inst, uint16_t val )
{
    printf ( "( 0x%.02x ) Temp Sensor Offset T%d:\n", PS_REG_T1_OFFSET + inst, 
      inst + 1 );
    printf ( "    Offset             -> 0x%.04x\n", 
      ps_reg_field_val ( val, PS_REG_MSK_T1_OFFSET ) );
    
    return;
}

void
ps_show_reg_fan_spd_res ( uint32_t inst, uint16_t val )
{
    printf ( "( 0x%.02x ) Fan Speed Resolution:\n", 
      PS_REG_F1F2_SPD_RES + inst );
    printf ( "    F%d Counts/RPM     -> %s\n", ( inst * 2 ) + 1,
      ( ps_reg_field_val ( val, PS_REG_MSK_F1_CNT_RPM ) ) ? "Counts" : "RPM" );
    if ( ps_reg_field_val ( val, PS_REG_MSK_F1_CNT_RPM ) )
        printf ( "    F%d Clck Pulse Mult-> 0x%x\n", ( inst * 2 ) + 1,
          ps_reg_field_val ( val, PS_REG_MSK_F1_CPM ) );
    printf ( "    F%d Counts/RPM     -> %s\n", ( inst * 2 ) + 2,
      ps_reg_field_val ( val, PS_REG_MSK_F2_CNT_RPM ) ? "Counts" : "RPM" );
    if ( ps_reg_field_val ( val, PS_REG_MSK_F1_CNT_RPM ) )
        printf ( "    F%d Clck Pulse Mult-> 0x%x\n", ( inst * 2 ) + 2,
          ps_reg_field_val ( val, PS_REG_MSK_F2_CPM ) );

}

void
ps_show_reg_fan_spd_cfg ( uint32_t inst, uint16_t val )
{
    printf ( "( 0x%.02x ) Fan Speed Control:\n",
      PS_REG_F1_CFG + inst );
    printf ( "    F%d Duty Cycle/RPM -> %s\n", inst + 1,
      ( ps_reg_field_val ( val, PS_REG_MSK_CFG ) ) ? "DC" : "RPM" );
    printf ( "    F%d Max Speed      -> %d\n",  inst + 1,
      ps_reg_field_val ( val, PS_REG_MSK_MAX_SPD ) * 2 );
    
    return;
}

void
ps_show_reg_volt_cur_sens_cfg ( uint16_t val ) 
{
    printf ( "( 0x%.02x ) Voltage/Current Sensor Cfg:\n",
      PS_REG_VOLT_CUR_SENS_CFG );
    printf ( "    Power Supply Output-> %d\n", 
      ps_reg_field_val ( val, PS_REG_MSK_PS_OUT_QNT ) );
    printf ( "    VSensor Support    -> %s\n", 
      ( ps_reg_field_val ( val, PS_REG_MSK_VSENS ) ) ? "YES" : "NO" );
    printf ( "    Out Cur Sensor Cnt -> %d\n", 
      ps_reg_field_val ( val, PS_REG_MSK_OUT_CUR_SENS ) );
    printf ( "    In Cur Sensor Cnt  -> %d\n",
      ps_reg_field_val ( val, PS_REG_MSK_IN_CUR_SENS ) );
    printf ( "    Peak Cur Sensor Cnt-> %d\n",
      ps_reg_field_val ( val, PS_REG_MSK_PK_CUR_SENS ) );
    printf ( "    In VSensor Support -> %s\n",
      ( ps_reg_field_val ( val, PS_REG_MSK_IN_VSENS ) ) ? "YES" : "NO" );

    return;
}

void
ps_show_reg_fan_cnt_assc ( uint32_t fanc_cnt, uint32_t fans_cnt, 
  uint16_t val )
{
    uint32_t i, j;

    printf ( "( 0x%.02x ) Fan Cnt Assc:\n", PS_REG_FAN_CTRL );
    for ( i = 0; i < fanc_cnt; i++, val >>= 4 ) {
        printf ( "    Ctrl%d Assc        ->", i );
        for ( j = 0; j< fans_cnt; j++ ) {
            if ( val & ( 1 << j ) ) {
                printf ( " F%d", j );
            }
        }
        printf ( "\n" );
    }

    return;
}

void
ps_show_reg_tsens_assc ( uint32_t fanc_cnt, uint32_t tsens_cnt, 
  uint16_t val )
{
    return;
}

void
ps_show_reg_shutdown_evt ( uint16_t val )
{
    printf ( "( 0x%.02x ) Shutdown Event Capability:\n", PS_REG_SHUT_DOWN_EVT );
    if ( ps_reg_field_val ( val, PS_REG_MSK_FAIL ) )
        printf ( "    Supports Event     -> Fail\n" );
    if ( ps_reg_field_val ( val, PS_REG_MSK_OVER_CUR ) )
        printf ( "    Supports Event     -> Over Current\n" );
    if ( ps_reg_field_val ( val, PS_REG_MSK_OVER_T ) )
        printf ( "    Supports Event     -> Over Temp\n" );
    if ( ps_reg_field_val ( val, PS_REG_MSK_AC_LOSS ) )
        printf ( "    Supports Event     -> AC Loss\n" );
    if ( ps_reg_field_val ( val, PS_REG_MSK_F1_FAIL ) )
        printf ( "    Supports Event     -> Fan 1 Fail\n" );
    if ( ps_reg_field_val ( val, PS_REG_MSK_F2_FAIL ) )
        printf ( "    Supports Event     -> Fan 2 Fail\n" );
    if ( ps_reg_field_val ( val, PS_REG_MSK_F3_FAIL ) )
        printf ( "    Supports Event     -> Fan 3 Fail\n" );
    if ( ps_reg_field_val ( val, PS_REG_MSK_F4_FAIL ) )
        printf ( "    Supports Event     -> Fan 4 Fail\n" );

    return;
}

void
ps_show_reg_status ( uint16_t val )
{
    printf ( "( 0x%.02x ) Status Capability:\n", PS_REG_STATUS );
    if ( ps_reg_field_val ( val, PS_REG_MSK_F1_OVERRIDE ) )
        printf ( "    Supports           -> F1 Override\n" );
    if ( ps_reg_field_val ( val, PS_REG_MSK_F2_OVERRIDE ) )
        printf ( "    Supports           -> F2 Override\n" );
    if ( ps_reg_field_val ( val, PS_REG_MSK_F3_OVERRIDE ) )
        printf ( "    Supports           -> F3 Override\n" );
    if ( ps_reg_field_val ( val, PS_REG_MSK_F4_OVERRIDE ) )
        printf ( "    Supports           -> F4 Override\n" );
    if ( ps_reg_field_val ( val, PS_REG_MSK_PWOK ) )
        printf ( "    Supports           -> PWOK\n" );
    if ( ps_reg_field_val ( val, PS_REG_MSK_PSON ) )
        printf ( "    Supports           -> PSON\n" );
    if ( ps_reg_field_val ( val, PS_REG_MSK_INTR ) )
        printf ( "    Supports           -> Intr\n" );
    if ( ps_reg_field_val ( val, PS_REG_MSK_REDUDANT ) )
        printf ( "    Supports           -> Redundant\n" );
    if ( ps_reg_field_val ( val, PS_REG_MSK_IN_RANGE ) )
        printf ( "    Supports           -> In Range\n" );

    return;
}

void
ps_show_reg_ctrl ( uint16_t val )
{
    printf ( "( 0x%.02x ) Ctrl Capability:\n", PS_REG_CTRL );
    if ( ps_reg_field_val ( val, PS_REG_MSK_F1_CTRL_MODE ) )
        printf ( "    Flag               -> F1 Ctrl Mode Enabled\n" );
    if ( ps_reg_field_val ( val, PS_REG_MSK_F2_CTRL_MODE ) )
        printf ( "    Flag               -> F2 Ctrl Mode Enabled\n" );
    if ( ps_reg_field_val ( val, PS_REG_MSK_F3_CTRL_MODE ) )
        printf ( "    Flag               -> F3 Ctrl Mode Enabled\n" );
    if ( ps_reg_field_val ( val, PS_REG_MSK_F4_CTRL_MODE ) )
        printf ( "    Flag               -> F4 Ctrl Mode Enabled\n" );
    if ( ps_reg_field_val ( val, PS_REG_MSK_LED_CTRL ) )
        printf ( "    Flag               -> LED Ctrl Enabled\n" );
    if ( ps_reg_field_val ( val, PS_REG_MSK_PS_REC ) )
        printf ( "    Flag               -> Capability Records Enabled\n" );
    if ( ps_reg_field_val ( val, PS_REG_MSK_V_PSMI_DEV ) )
        printf ( "    Flag               -> PSMI Vendor Spec Supported\n" );
    if ( ps_reg_field_val ( val, PS_REG_MSK_CSTM_FTR ) )
        printf ( "    Flag               -> Custom Features Supported\n" );

    return;
}

void
ps_show_reg_warning ( uint16_t val )
{
    printf ( "( 0x%.02x ) Warning Capability:\n", PS_REG_WARNING );
    printf ( "    Ctrl Temp Warn     -> %d\n",
       ps_reg_field_val ( val, PS_REG_MSK_CTRL_TEMP ) );
    printf ( "    Ambient Temp Warn  -> %d\n",
       ps_reg_field_val ( val, PS_REG_MSK_ATEMP ) );
    printf ( "    Fan Failure Warn   -> %d\n",
       ps_reg_field_val ( val, PS_REG_MSK_F_FAIL ) );
    printf ( "    Output Current Warn-> %d\n",
       ps_reg_field_val ( val, PS_REG_MSK_OUT_CUR ) );
    printf ( "    Input Voltage Warn -> %d\n",
       ps_reg_field_val ( val, PS_REG_MSK_IN_V ) );
    printf ( "    Input Current Warn -> %d\n",
       ps_reg_field_val ( val, PS_REG_MSK_IN_CUR ) );
    
    return;
}

/*********************************************/
/*              Status/Warning               */
/*********************************************/
void
ps_show_reg_shutdown_evt_status ( uint16_t val )
{
    printf ( "( 0x%.02x ) Shutdown Event Status:\n", 
      PS_REG_SHUTDOWN_EVT_STATUS );
    if ( ps_reg_field_val ( val, PS_REG_MSK_FAIL ) )
        printf ( "    Event              -> Fail\n" );
    if ( ps_reg_field_val ( val, PS_REG_MSK_OVER_CUR ) )
        printf ( "    Event              -> Over Current\n" );
    if ( ps_reg_field_val ( val, PS_REG_MSK_OVER_T ) )
        printf ( "    Event              -> Over Temp\n" );
    if ( ps_reg_field_val ( val, PS_REG_MSK_AC_LOSS ) )
        printf ( "    Event              -> AC Loss\n" );
    if ( ps_reg_field_val ( val, PS_REG_MSK_F1_FAIL ) )
        printf ( "    Event              -> Fan 1 Fail\n" );
    if ( ps_reg_field_val ( val, PS_REG_MSK_F2_FAIL ) )
        printf ( "    Event              -> Fan 2 Fail\n" );
    if ( ps_reg_field_val ( val, PS_REG_MSK_F3_FAIL ) )
        printf ( "    Event              -> Fan 3 Fail\n" );
    if ( ps_reg_field_val ( val, PS_REG_MSK_F4_FAIL ) )
        printf ( "    Event              -> Fan 4 Fail\n" );

    return;
}

void
ps_show_reg_warning_thermal ( uint16_t val )
{
    printf ( "( 0x%.02x ) Thermal Warning:\n", 
      PS_REG_WARNING_THERMAL );
    if ( ps_reg_field_val ( val, PS_REG_MSK_T1_WARNING ) )
        printf ( "    Warning            -> T1\n" );
    if ( ps_reg_field_val ( val, PS_REG_MSK_T2_WARNING ) )
        printf ( "    Warning            -> T2\n" );
    if ( ps_reg_field_val ( val, PS_REG_MSK_T3_WARNING ) )
        printf ( "    Warning            -> T3\n" );
    if ( ps_reg_field_val ( val, PS_REG_MSK_T4_WARNING ) )
        printf ( "    Warning            -> T4\n" );

    if ( ps_reg_field_val ( val, PS_REG_MSK_F1_WARNING ) )
        printf ( "    Warning            -> F1\n" );
    if ( ps_reg_field_val ( val, PS_REG_MSK_F2_WARNING ) )
        printf ( "    Warning            -> F2\n" );
    if ( ps_reg_field_val ( val, PS_REG_MSK_F3_WARNING ) )
        printf ( "    Warning            -> F3\n" );
    if ( ps_reg_field_val ( val, PS_REG_MSK_F4_WARNING ) )
        printf ( "    Warning            -> F4\n" );
    
    return;
}

void
ps_show_reg_warning_output ( uint16_t val )
{
    printf ( "( 0x%.02x ) Output Current Warning:\n",      
      PS_REG_WARNING_OUTPUT );
    if ( ps_reg_field_val ( val, PS_REG_MSK_C1_WARNING ) )
        printf ( "    Warning            -> C1\n" );
    if ( ps_reg_field_val ( val, PS_REG_MSK_C2_WARNING ) )
        printf ( "    Warning            -> C2\n" );
    if ( ps_reg_field_val ( val, PS_REG_MSK_C3_WARNING ) )
        printf ( "    Warning            -> C3\n" );
    if ( ps_reg_field_val ( val, PS_REG_MSK_C4_WARNING ) )
        printf ( "    Warning            -> C4\n" );
    if ( ps_reg_field_val ( val, PS_REG_MSK_C5_WARNING ) )
        printf ( "    Warning            -> C5\n" );
    if ( ps_reg_field_val ( val, PS_REG_MSK_C6_WARNING ) )
        printf ( "    Warning            -> C6\n" );
    if ( ps_reg_field_val ( val, PS_REG_MSK_C7_WARNING ) )
        printf ( "    Warning            -> C7\n" );
    if ( ps_reg_field_val ( val, PS_REG_MSK_C8_WARNING ) )
        printf ( "    Warning            -> C8\n" );
    if ( ps_reg_field_val ( val, PS_REG_MSK_C9_WARNING ) )
        printf ( "    Warning            -> C9\n" );
    if ( ps_reg_field_val ( val, PS_REG_MSK_C10_WARNING ) )
        printf ( "    Warning            -> C10\n" );
    
    return;
}

void
ps_show_reg_warning_input ( uint16_t val )
{
    printf ( "( 0x%.02x ) Input Current Warning:\n",
      PS_REG_WARNING_INPUT );
    if ( ps_reg_field_val ( val, PS_REG_MSK_OVER_CUR_WARN ) )
        printf ( "    Warning            -> Over Current\n" );
    if ( ps_reg_field_val ( val, PS_REG_MSK_UNDER_V_WARN ) )
        printf ( "    Warning            -> Under Current\n" );

    return;
}

void
ps_show_reg_status_event ( uint16_t val )
{
    printf ( "( 0x%.02x ) Status Event:\n",
      PS_REG_STATUS_EVT );
    if ( ps_reg_field_val ( val, PS_REG_MSK_RPM1_OVERRIDE ) )
        printf ( "    Event              -> RPM1 Override\n" );
    if ( ps_reg_field_val ( val, PS_REG_MSK_RPM2_OVERRIDE ) )
        printf ( "    Event              -> RPM2 Override\n" );
    if ( ps_reg_field_val ( val, PS_REG_MSK_RPM3_OVERRIDE ) )
        printf ( "    Event              -> RPM3 Override\n" );
    if ( ps_reg_field_val ( val, PS_REG_MSK_RPM4_OVERRIDE ) )
        printf ( "    Event              -> RPM4 Override\n" );
   
    if ( ps_reg_field_val ( val, PS_REG_MSK_PWOK_EVT ) )
        printf ( "    Event              -> PWOK\n" );
    if ( ps_reg_field_val ( val, PS_REG_MSK_PSON_EVT ) )
        printf ( "    Event              -> PSON\n" );

    if ( ps_reg_field_val ( val, PS_REG_MSK_INTERRUPT_EVT ) )
        printf ( "    Event              -> Interrupt\n" );

    if ( ps_reg_field_val ( val, PS_REG_MSK_REDUNDANCY ) )
        printf ( "    Event              -> Redundant\n" );
    
    printf ( "    Range              -> %u\n", 
      ps_reg_field_val ( val, PS_REG_MSK_OP_RANGE ) );

    return;
}

void
ps_show_reg_control ( uint16_t val )
{
    printf ( "( 0x%.02x ) Control Event:\n",
      PS_REG_CONTROL );
    printf ( "    F1 Ctrl Mode       -> %sabled\n",
      ( ps_reg_field_val ( val, PS_REG_MSK_F1_CTRL_MODE ) ) ? "En":"Dis" );
    printf ( "    F2 Ctrl Mode       -> %sabled\n",
      ( ps_reg_field_val ( val, PS_REG_MSK_F2_CTRL_MODE ) ) ? "En":"Dis" );
    printf ( "    F3 Ctrl Mode       -> %sabled\n",
      ( ps_reg_field_val ( val, PS_REG_MSK_F3_CTRL_MODE ) ) ? "En":"Dis" );
    printf ( "    F4 Ctrl Mode       -> %sabled\n",
      ( ps_reg_field_val ( val, PS_REG_MSK_F4_CTRL_MODE ) ) ? "En":"Dis" );
    
    printf ( "    Shutdown LED       -> %s Control\n",
      ( ps_reg_field_val ( val, PS_REG_MSK_SHUTDOWN_LED ) ) ? "System":"PS" );
    printf ( "    Warning  LED       -> %s Control\n",
      ( ps_reg_field_val ( val, PS_REG_MSK_WARNING_LED ) ) ? "System":"PS" );

    printf ( "    Shutdown LED Intr  -> %sabled\n",
      ( ps_reg_field_val ( val, PS_REG_MSK_SHUTDOWN_EVT_EN ) ) ? "En":"Dis" );
    printf ( "    Warning  LED Intr  -> %sabled\n",
      ( ps_reg_field_val ( val, PS_REG_MSK_WARNING_EVT_EN ) ) ? "En":"Dis" );
    
    return;
}

/*********************************************/
/*            Capability Records             */
/*********************************************/
void
ps_show_reg_max_temp ( uint32_t inst, uint16_t val )
{
    printf ( "    T%d                 -> %d\n", inst + 1, ( short ) val >> 6 );

    return;
}

void
ps_show_reg_min_rpm ( uint32_t inst, uint16_t val )
{
    printf ( "    F%d                 -> %d\n", inst + 1, val );              

    return;
}

void
ps_show_reg_sound_power ( uint16_t val )
{
    printf ( "( 0x%.02x ) Sound Power Capability:\n", PS_REG_SOUND_POWER );
    printf ( "    Max                -> %d\n", val & 0xFF );
    printf ( "    Min                -> %d\n", val >> 0x8 ); 

    return;
}

void
ps_show_reg_max_voltage ( uint32_t inst, uint16_t val )
{
    ps_show_reg_voltage ( inst, val );
    return;   
}

void
ps_show_reg_min_voltage ( uint32_t inst, uint16_t val )
{
    ps_show_reg_voltage ( inst, val );
    return;
}

void
ps_show_reg_out_current_limit ( uint32_t inst, uint16_t val )
{
    ps_show_reg_current ( inst, val );
    return;
}

int
ps_show_reg (int fd, uint32_t ps_inst, uint32_t reg, uint32_t verbose )
{
    int inst;

    uint16_t val = 0, cfg = 0;
    int rc = 0;

    if ( ( rc = psu_rd ( fd, ps_inst,
       reg,  ( uint8_t *)&val ) ) < 0 ) {
        fprintf ( stderr, "Err: PSU %d, Read Failed\n", ps_inst + 1 );
        return ( rc );
    }

    if ( verbose ) {
        switch ( reg ) {
        case PS_REG_THERM_SENS_CFG:
            ps_reg_show_thermal_sens_cfg ( val );
            break;
        case PS_REG_TSENS_TYPE:
            ps_reg_show_tsens_type ( ps_tsens_cnt (fd, ps_inst ), val );
            break;
        case PS_REG_T1_OFFSET:
        case PS_REG_T2_OFFSET:
        case PS_REG_T3_OFFSET:
        case PS_REG_T4_OFFSET:
            inst = ( reg - PS_REG_T1_OFFSET );
            if ( inst < ps_tsens_cnt (fd, ps_inst ) )
                ps_show_reg_t_offset ( inst, val );
            break;
        case PS_REG_F1F2_SPD_RES:
        case PS_REG_F3F4_SPD_RES:
            inst = ( reg - PS_REG_F1F2_SPD_RES );
            if ( inst < ( ( ps_fanc_cnt (fd, ps_inst ) / 2 ) + 1 ) )
                ps_show_reg_fan_spd_res ( inst, val );
            break;
        case PS_REG_F1_CFG:
        case PS_REG_F2_CFG:
        case PS_REG_F3_CFG:
        case PS_REG_F4_CFG:
            inst = ( reg - PS_REG_F1_CFG );
            if ( inst < ps_fans_cnt (fd, ps_inst) )
                ps_show_reg_fan_spd_cfg ( inst, val );
            break;
        case PS_REG_VOLT_CUR_SENS_CFG:
            ps_show_reg_volt_cur_sens_cfg ( val );
            break;
        case PS_REG_FAN_CTRL:
            //if ( fans_assc )
            //    ps_show_reg_fan_cnt_assc ( fanc_cnt, fans_cnt, val );
            break;
        case PS_REG_F1F2_T_ASSC:
        case PS_REG_F3F4_T_ASSC:
            //if ( tsens_assc )
            //    ps_show_reg_tsens_assc ( fanc_cnt, tsens_cnt, val );
            break;
        case PS_REG_SHUT_DOWN_EVT:
            ps_show_reg_shutdown_evt ( val );
            break;
        case PS_REG_STATUS:
            ps_show_reg_status ( val );
            break;
        case PS_REG_CTRL:
            ps_show_reg_ctrl ( val );
            break;
        case PS_REG_WARNING:
            ps_show_reg_warning ( val );
            break;
        case PS_REG_T1:
            printf ( "Temp Sensor Readings:\n" );
        case PS_REG_T2:
        case PS_REG_T3:
        case PS_REG_T4:
            inst = ( reg - PS_REG_T1 );
            if ( inst < ps_tsens_cnt (fd, ps_inst ) )
                ps_show_reg_temp ( inst, val );
            break;
        case PS_REG_F1_SENS:
            printf ( "Fan Tach Readings:\n" );
        case PS_REG_F2_SENS:
        case PS_REG_F3_SENS:
        case PS_REG_F4_SENS:
            inst = ( reg - PS_REG_F1_SENS );
            if ( inst < ps_fans_cnt (fd, ps_inst ) ) {
                if ( ( rc = psu_rd ( fd, ps_inst,
                  ( ( inst < 2 ) ? PS_REG_F1F2_SPD_RES : 
                  PS_REG_F3F4_SPD_RES ),  ( uint8_t *)&cfg ) ) < 0 ) {
                    return ( rc );
                } 
                ps_show_reg_tach ( inst, cfg, val );
            }
            break;
        case PS_REG_F1_CTRL:
            printf ( "Fan PWM Readings:\n" );
        case PS_REG_F2_CTRL:
        case PS_REG_F3_CTRL:
        case PS_REG_F4_CTRL:
            inst = ( reg - PS_REG_F1_CTRL );
            if ( inst < ps_fans_cnt (fd, ps_inst ) ) {
                if ( ( rc = psu_rd ( fd, ps_inst, inst + PS_REG_F1_CFG, 
                   ( uint8_t *)&cfg ) ) < 0 ) {
                    return ( rc );
                } 
                ps_show_reg_fan_ctrl ( inst, cfg, val );
            }
            break;
        case PS_REG_V1_SENS:
            printf ( "Voltage Sensor Readings:\n" );
        case PS_REG_V2_SENS:
        case PS_REG_V3_SENS:
        case PS_REG_V4_SENS:
        case PS_REG_V5_SENS:
        case PS_REG_V6_SENS:
        case PS_REG_V7_SENS:
        case PS_REG_V8_SENS:
        case PS_REG_V9_SENS:
        case PS_REG_V10_SENS:
            inst = ( reg - PS_REG_V1_SENS );
            if ( inst < ps_vsens_cnt (fd, ps_inst ) ) {
                ps_show_reg_voltage ( inst, val );
            }
            break;
        case PS_REG_OUT_C1_SENS:
            printf ( "Output Current Sensor Readings:\n" );
        case PS_REG_OUT_C2_SENS:
        case PS_REG_OUT_C3_SENS:
        case PS_REG_OUT_C4_SENS:
        case PS_REG_OUT_C5_SENS:
        case PS_REG_OUT_C6_SENS:
        case PS_REG_OUT_C7_SENS:
        case PS_REG_OUT_C8_SENS:
        case PS_REG_OUT_C9_SENS:
        case PS_REG_OUT_C10_SENS:
            inst = reg - PS_REG_OUT_C1_SENS;
            if ( inst < ps_out_csens_cnt (fd, ps_inst ) ) {
                ps_show_reg_current ( inst, val );
            }
            break;
        case PS_REG_IN_C1_SENS:
            printf ( "Input  Current Sensor Readings:\n" );
            inst = reg - PS_REG_IN_C1_SENS;
            if ( inst < ps_in_csens_cnt (fd, ps_inst ) ) {
                ps_show_reg_current ( inst, val );
            }
            break;
        case PS_REG_PEAK_C1_SENS:
            printf ( "Peak Out Current Sensor Readngs:\n" );
        case PS_REG_PEAK_C2_SENS:
        case PS_REG_PEAK_C3_SENS:
        case PS_REG_PEAK_C4_SENS:
        case PS_REG_PEAK_C5_SENS:
        case PS_REG_PEAK_C6_SENS:
        case PS_REG_PEAK_C7_SENS:
        case PS_REG_PEAK_C8_SENS:
        case PS_REG_PEAK_C9_SENS:
        case PS_REG_PEAK_C10_SENS:
            inst = reg - PS_REG_PEAK_C1_SENS;
            if ( ps_peak_csens_enabled (fd, ps_inst ) == 1 ) {
                if ( inst < ps_out_csens_cnt (fd, ps_inst ) ) {
                    ps_show_reg_current ( inst, val );
                }
            }
            break;
        case PS_REG_IN_PEAK_C1_SENS:
            printf ( "Peak In Current Sensor Readngs:\n" );
            inst = reg - PS_REG_IN_PEAK_C1_SENS;
            if ( ps_peak_csens_enabled (fd, ps_inst ) == 1 ) {
                if ( inst < ps_in_csens_cnt (fd, ps_inst ) ) {
                    ps_show_reg_current ( inst, val );
                }
            }
            break;
        case PS_REG_SHUTDOWN_EVT_STATUS:
            ps_show_reg_shutdown_evt_status ( val );
            break;
        case PS_REG_WARNING_THERMAL:
            ps_show_reg_warning_thermal ( val );
            break;
        case PS_REG_WARNING_OUTPUT:
            ps_show_reg_warning_output ( val );
            break;
        case PS_REG_WARNING_INPUT:
            ps_show_reg_warning_input ( val );
            break;
        case PS_REG_STATUS_EVT:
            ps_show_reg_status_event ( val );
            break;
        case PS_REG_CONTROL:
            ps_show_reg_control ( val );
            break;
        case PS_REG_T1_TARGET:
            printf ( "Temperature Target:\n" );
        case PS_REG_T2_TARGET:
        case PS_REG_T3_TARGET:
        case PS_REG_T4_TARGET:
            inst = ( reg - PS_REG_T1_TARGET );
            if ( inst < ps_tsens_cnt (fd, ps_inst ) ) {
                ps_show_reg_max_temp ( inst, val );
            }
            break;
        case PS_REG_F1_MIN:
            printf ( "Fan Max RPM:\n" );
        case PS_REG_F2_MIN:
        case PS_REG_F3_MIN:
        case PS_REG_F4_MIN:
            inst = ( reg - PS_REG_F1_MIN );
            if ( inst < ps_fans_cnt (fd, ps_inst ) ) {
                ps_show_reg_min_rpm ( inst, val );
            }
            break;
        case PS_REG_SOUND_POWER:
            ps_show_reg_sound_power ( val );
            break;
        case PS_REG_V1_MAX:
            printf ( "Voltage Max/Min:\n" );
        case PS_REG_V1_MIN:
        case PS_REG_V2_MAX:
        case PS_REG_V2_MIN:
        case PS_REG_V3_MAX:
        case PS_REG_V3_MIN:
        case PS_REG_V4_MAX:
        case PS_REG_V4_MIN:
        case PS_REG_V5_MAX:
        case PS_REG_V5_MIN:
        case PS_REG_V6_MAX:
        case PS_REG_V6_MIN:
        case PS_REG_V7_MAX:
//        case PS_REG_V7_MIN:
//        case PS_REG_V8_MAX:
//        case PS_REG_V8_MIN:
//        case PS_REG_V9_MAX:
//        case PS_REG_V9_MIN:
//        case PS_REG_V10_MAX:
//        case PS_REG_V10_MIN:
            inst = ( reg - PS_REG_V1_MAX );
            if ( ( inst / 2 ) < ps_vsens_cnt (fd, ps_inst ) ) {
                if ( inst % 2 )
                    ps_show_reg_min_voltage ( inst / 2, val );
                else
                    ps_show_reg_max_voltage ( inst / 2, val );
            }
            break;
        case PS_REG_FIRST_OUTPUT_C1:
            printf ( "Output Current Limits First Setting:\n" );
        case PS_REG_FIRST_OUTPUT_C2:
        case PS_REG_FIRST_OUTPUT_C3:
        case PS_REG_FIRST_OUTPUT_C4:
        case PS_REG_FIRST_OUTPUT_C5:
        case PS_REG_FIRST_OUTPUT_C6:
        case PS_REG_FIRST_OUTPUT_C7:
        case PS_REG_FIRST_OUTPUT_C8:
        case PS_REG_FIRST_OUTPUT_C9:
        case PS_REG_FIRST_OUTPUT_C10:
            inst = ( reg - PS_REG_FIRST_OUTPUT_C1 );
            if ( inst < ps_out_csens_cnt (fd, ps_inst ) ) {
                ps_show_reg_out_current_limit ( inst, val );
            }
            break;
        case PS_REG_SECOND_OUTPUT_C1:
            printf ( "Output Current Limits Second Setting:\n" );
        case PS_REG_SECOND_OUTPUT_C2:
        case PS_REG_SECOND_OUTPUT_C3:
        case PS_REG_SECOND_OUTPUT_C4:
        case PS_REG_SECOND_OUTPUT_C5:
        case PS_REG_SECOND_OUTPUT_C6:
        case PS_REG_SECOND_OUTPUT_C7:
        case PS_REG_SECOND_OUTPUT_C8:
        case PS_REG_SECOND_OUTPUT_C9:
        case PS_REG_SECOND_OUTPUT_C10:
            inst = ( reg - PS_REG_SECOND_OUTPUT_C1 );
            if ( inst < ps_out_csens_cnt (fd, ps_inst ) ) {
                ps_show_reg_out_current_limit ( inst, val );
            }
            break;
        case PS_REG_THIRD_OUTPUT_C1:
            printf ( "Output Current Limits Third Setting:\n" );
        case PS_REG_THIRD_OUTPUT_C2:
        case PS_REG_THIRD_OUTPUT_C3:
        case PS_REG_THIRD_OUTPUT_C4:
        case PS_REG_THIRD_OUTPUT_C5:
        case PS_REG_THIRD_OUTPUT_C6:
        case PS_REG_THIRD_OUTPUT_C7:
        case PS_REG_THIRD_OUTPUT_C8:
        case PS_REG_THIRD_OUTPUT_C9:
        case PS_REG_THIRD_OUTPUT_C10:
            inst = ( reg - PS_REG_THIRD_OUTPUT_C1 );
            if ( inst < ps_out_csens_cnt (fd, ps_inst ) ) {
                ps_show_reg_out_current_limit ( inst, val );
            }
            break;
        case PS_REG_FIRST_INPUT_MAX:
            printf ( "Input Min Current Limit (First/Second/Third):\n" );
        case PS_REG_SECOND_INPUT_MAX:
        case PS_REG_THIRD_INPUT_MAX:
            inst = ( reg - PS_REG_FIRST_INPUT_MAX );
            if ( ( inst / 3 ) < ps_in_csens_cnt (fd, ps_inst ) ) {
                ps_show_reg_current ( inst / 3, val );
            }
            break;
        case PS_REG_FIRST_INPUT_MIN:
            printf ( "Input Min Voltage (First/Second/Third):\n" );
        case PS_REG_SECOND_INPUT_MIN:
        case PS_REG_THIRD_INPUT_MIN:
            inst = ( reg - PS_REG_FIRST_INPUT_MIN );
            if ( ( inst / 3 ) < ps_in_csens_cnt (fd, ps_inst ) ) {
                printf ( "    V%d                 -> %d V\n", 
                  ( inst / 3 ) + 1, val / 32 );
            }
            break;
        case PS_REG_FIRST_OUT_PWR_MAX:
            printf ( "Output Power Max Limit (First/Second/Third):\n" );
        case PS_REG_SECOND_OUT_PWR_MAX:
        case PS_REG_THIRD_OUT_PWR_MAX:
            printf ( "                       -> %d W\n", val );
            break;
        case PS_REG_COMBO_OUT_LIMIT_1:
            printf ( "Combo Power Output 1: \n" );
            printf ( "                       -> %d W\n", val );
            break;
        case PS_REG_COMBO_OUTPUTS_1:
            printf ( "Combo Power Outputs 1: \n                       -> " );
            for ( ; val; val >>= 4 ) {
                if ( val & 0xF )
                    printf ( "V%d ", val & 0xF );
            }
            printf ( "\n" );
            break;
        case PS_REG_COMBO_OUT_LIMIT_2:
            printf ( "Combo Power Output 2: \n" );
            printf ( "                       -> %d W\n", val );
            break;
        case PS_REG_COMBO_OUTPUTS_2:
            printf ( "Combo Power Outputs 2: \n                       -> " );
            for ( ; val; val >>= 4 ) {
                if ( val & 0xF )
                    printf ( "V%d ", val & 0xF );
            }
            printf ( "\n" );
            break;
        case PS_REG_OUT_CUR_BW:
            printf ( "Output Current Bandwidth: \n" );
            printf ( "                      -> %d.%d Hz\n",val / 10, val % 10 );
            break;
        case PS_REG_IN_CUR_BW:
            printf ( "Input Current Bandwidth: \n" );
            printf ( "                      -> %d.%d Hz\n",val / 10, val % 10 );
            break;
        case PS_REG_L_OUT_PWR_HI_L:
            printf ( "Output Power High Line: \n" );
            printf ( "    Light Load        -> %d W\n", val );
            break;
        case PS_REG_M_OUT_PWR_HI_L:
            printf ( "    Mid-Load          -> %d W\n", val );
            break; 
        case PS_REG_H_OUT_PWR_HI_L:
            printf ( "    Max Load          -> %d W\n", val );
            break;
        case PS_REG_LOAD_EFFNCY_HI_L_1:
            printf ( "Ouput Power High Efficiency: \n" );
            printf ( "    Light Load        -> %d\n",(((val&0xFF)*100)/0x100) );
            printf ( "    Mid-Load          -> %d\n",(((val>>8)*100)/0x100) );
            break;
        case PS_REG_LOAD_EFFNCY__HI_L_2:
            printf ( "    Max Load          -> %d\n",(((val&0xFF)*100)/0x100) );
            break;
        case PS_REG_L_OUT_PWR_LO_L:
            printf ( "Output Power Low Line: \n" );
            printf ( "    Light Load        -> %d W\n", val );
            break;
        case PS_REG_M_OUT_PWR_LO_L:
            printf ( "    Mid-Load          -> %d W\n", val );
            break; 
        case PS_REG_H_OUT_PWR_LO_L:
            printf ( "    Max Load          -> %d W\n", val );
            break;
        case PS_REG_LOAD_EFFNCY_LO_L_1:
            printf ( "Ouput Power High Efficiency: \n" );
            printf ( "    Light Load        -> %d\n",(((val&0xFF)*100)/0x100) );
            printf ( "    Mid-Load          -> %d\n",(((val>>8)*100)/0x100) );
            break;
        case PS_REG_LOAD_EFFNCY_LO_L_2:
            printf ( "    Max Load          -> %d\n",(((val&0xFF)*100)/0x100) );
            break;
        case PS_REG_LOAD_SHR_ERR_V1:
        case PS_REG_LOAD_SHR_ERR_V2:
        case PS_REG_LOAD_SHR_ERR_V3:
        case PS_REG_LOAD_SHR_ERR_V4:
            break;
        default:
            break;
        }
    } else {
        printf ( "%.02x: 0x%.04x\n", reg, val );
    }

    return ( 0 );
}

int
psu_util_show (int fd, uint32_t ps_inst, uint32_t cmd, uint32_t verbose ) 
{
    uint32_t i;
    int rc;

    if ( !psu_present (fd, ps_inst ) ) {
        printf ( "PS0 Not Present\n" );
        return ( 1 );
    }

    if ( verbose && ( ( cmd == PSU_CMD_SHOW_ALL) || 
      ( cmd == PSU_CMD_SHOW_PSMI_HEADER ) ) ) {
        printf ( "-- PSMI Header --\n" );
        if ( ps_show_psmi_header (fd, ps_inst ) ) {
            return ( 1 );
        }
    }

    if ( ( cmd == PSU_CMD_SHOW_ALL) || ( cmd == PSU_CMD_SHOW_CAPABILITY ) )
    {
        printf ( "-- PSMI Capability Registers --\n" );
        for ( i = 0; i < ps_max_reg_psmi_capable; i++ ) {
            if ( ( rc = ps_show_reg (fd, ps_inst, ps_reg_psmi_capable[i], 
              verbose ) ) < 0 ) {
                return ( rc );
            }
        }
    }
    if ( ( cmd == PSU_CMD_SHOW_ALL) || ( cmd == PSU_CMD_SHOW_SENSORS ) )
    {
        printf( "\n-- Sensor Registers --\n" );
        for ( i = 0; i < ps_max_reg_sensors; i++ ) {
            if ( ( rc = ps_show_reg (fd, ps_inst, ps_reg_sensors[i], 
              verbose ) ) < 0 ) {
                return ( rc );
            }
        }
    }    
    if ( ( cmd == PSU_CMD_SHOW_ALL) || ( cmd == PSU_CMD_SHOW_STATUS ) )
    {
        printf( "\n-- Status Registers --\n" );
        for ( i = 0; i < ps_max_reg_status; i++ ) {
            if ( ( rc = ps_show_reg (fd, ps_inst, ps_reg_status[i], 
              verbose ) ) < 0 ) {
                return ( rc );
            }
        }
    } 
    if ( ( cmd == PSU_CMD_SHOW_ALL) || ( cmd == PSU_CMD_SHOW_RECORDS ) )
    {
        printf( "\n-- Capability Records Registers --\n" );
        for ( i = 0; i < ps_max_reg_records; i++ ) {
            if ( ( rc = ps_show_reg (fd, ps_inst, ps_reg_records[i],
              verbose ) ) < 0 ) {
                return ( rc );
            }
        }
    }
    return ( 0 );
}

int
psu_util_get_psmi_revision (int fd, uint32_t ps_inst, uint32_t *major, 
  uint32_t *minor )
{
    int rc = 0;
    uint16_t val = 0;    

    if ( ( rc = psu_rd ( fd, ps_inst, PS_REG_DISCOVERY_KEY1_2, 
      ( uint8_t *)&val ) ) < 0 ) {
        return ( rc );
    }

    if ( ( ( char )val !=  'P' ) && ( ( ( char ) ( val >> 8 ) ) != 'S') ) {
        fprintf ( stderr, "PS%d, Not PSMI Compatable\n", ps_inst );
        return ( 1 );
    }
    if ( ( rc = psu_rd ( fd, ps_inst, PS_REG_DISCOVERY_KEY3_4, 
      ( uint8_t *)&val ) ) < 0 ) {
        return ( rc );
    }
    if ( ( ( char )val !=  'M' ) && ( ( ( char ) ( val >> 8 ) ) != 'I') ) {
        fprintf ( stderr, "PS%d, Not PSMI Compatable\n", ps_inst );
        return ( 1 );
    }
    if ( ( rc = psu_rd ( fd, ps_inst, PS_REG_PSMI_REV, 
      ( uint8_t *)&val ) ) < 0 ) {
        return ( rc );
    }

    *major = ps_reg_field_val ( val, PS_REG_MSK_PSMI_MAJOR );
    *minor = ps_reg_field_val ( val, PS_REG_MSK_PSMI_MINOR );

    return ( 0 );
}


int psu_ui (int fd, int argc, char *argv[])
{
        int             len = 0;
        uint32_t        ps_inst;
        uint32_t        verbose = 0;
        psu_cmd_t       cmd = PSU_CMD_SHOW_ALL;
        uint8_t         offset;
        uint16_t        data;

        if (argc < 3) return (-1);

        argv++;
        argc--;

        // Get the instance (0-3).
        sscanf(*argv, "%d", &ps_inst);
        assert(ps_inst < 4);

        // Check if show is specified.
        argv++;
        argc--;
        len = strlen ( *argv );
        if ( strncmp ( *argv, "show", len ) == 0 ) {
                argv++;
                argc--;
                // Get command if specified.
                if ( argc ) {
                        len = strlen ( *argv );
                        if ( strncmp ( *argv, "sensors", len ) == 0 ) {
                                cmd = PSU_CMD_SHOW_SENSORS;
                        } else if ( strncmp ( *argv, "header", len ) == 0 ) {
                                cmd = PSU_CMD_SHOW_PSMI_HEADER;
                        } else if ( strncmp ( *argv, "capability", len ) == 0 ) {
                                cmd = PSU_CMD_SHOW_CAPABILITY;
                        } else if ( strncmp ( *argv, "status", len ) == 0 ) {
                                cmd = PSU_CMD_SHOW_STATUS;
                        } else if ( strncmp ( *argv, "records", len ) == 0 ) {
                                cmd = PSU_CMD_SHOW_RECORDS;
                        } else if ( strncmp ( *argv, "verbose", len ) == 0 ) {
                                verbose = 1;
                        }

                        argv++;
                        argc--;
                }

                // Get verbose
                if ( argc ) {
                        len = strlen ( *argv );
                        if ( strncmp ( *argv, "verbose", len ) == 0 ) {
                                verbose = 1;
                        }
                }

                return (psu_util_show (fd, ps_inst, cmd, verbose));
        } else  if (strncmp (*argv, "w", strlen(*argv)) == 0 ) {
                argv++;
                argc--;
                if (!argc) return (-1);
                offset = strtoul(*argv, NULL, 0);
                argv++;
                argc--;
                if (!argc) return (-1);
                data = strtoul(*argv, NULL, 0);

                printf("[PSU%d 0x%X ] : 0x%X\n", ps_inst, offset, data);
                return (psu_wr(fd, ps_inst, offset, data));             
        } else  if (strncmp (*argv, "r", strlen(*argv)) == 0 ) {
                argv++;
                argc--;
                offset = strtoul(*argv, NULL, 0);
                if (!psu_rd(fd, ps_inst, offset, (uint8_t*)&data)) {
                        printf("[PSU%d 0x%X ] : 0x%X\n", ps_inst, offset, data);
                } else {
                        printf("Rd [PSU%d 0x%X ] : Err\n", ps_inst, offset);
                }
        } else {
                return (-1);
        }

        return (0);
}
