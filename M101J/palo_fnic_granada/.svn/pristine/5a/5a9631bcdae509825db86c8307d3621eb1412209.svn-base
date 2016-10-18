/****************************************************************************
*  The information contained in this file is confidential and proprietary to 
*  Broadcom Corporation.  No part of this file may be reproduced or           
*  distributed, in any form or by any means for any purpose, without the      
*  expressed written permission of Broadcom Corporation.                      
*
*  
*  (c) COPYRIGHT 2005 Broadcom Corporation, ALL RIGHTS RESERVED.              
*                                                                            
*  Module Description: 
*                       Simple Software Cable Diagnostic
*  History:
*    10/13/2004 Init. Ver.  Minshine
*    01/28/2005 Ver. 0.8    Minshine
*    03/31/2005 Ver. 0.9    Minshine
*    04/27/2005 Ver. 0.91   Minshine disabled debug capability
*    08/10/2005 Ver. 1.5.8  Minshine New XT (cross-talk) check algorithm
*    08/17/2005 Ver. 1.6    Minshine Added 5414 support, report min. distance
*                                    in case of multiple XT, other improvement
*    01/20/2006 Ver. 2.0    Minshine Initial work
*    07/18/2006 Ver. 2.0    Minshine Major algorithm improvement
*    04/12/2007 Ver. 3.0    Minshine Fixed status code 0x0020 issue
******************************************************************************/
#ifndef __CD
#define __CD
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
/*******************************************************************/
#define     MDIO_R_ACCESS_TIME_US       4  // 13 MHz MDC - Customers should use actual value
#define     MDIO_W_ACCESS_TIME_US       4  // 13 MHz MDC - Customers should use actual value
#define     N_MSE                       50
#define     N_HWG6_WAIT                 6000

#if 0
typedef     char            int8;
typedef     short           int16_t;
typedef     int             int32_t;
#endif

typedef     int32_t           phy_t;      /* Customer definable */

/* STATUS & ERRORS */
#define     GOOD                    0x0000
#define     ERROR_ACCESS            0x0001
#define     ERROR_SOFTWARE_01       0x0002
#define     ERROR_LINK              0x0004
#define     ERROR_NO_SUPPORT        0x0008
#define     ERROR_MALLOC            0x0010
#define     ERROR_EMPTY_ECHO        0x0020
#define     ERROR_EMPTY_NEXT        0x0040
#define     ERROR_READ_ECHO         0x0080
#define     ERROR_READ_NEXT         0x0100
#define     ERROR_SOFTWARE_02       0x0200
#define     ERROR_MII_BUSY          0x0400
#define     ERROR_NO_PEAK           0x0800
#define     ERROR_ACCESS_FILE       0x1000
#define     ERROR_ACCESS_TAP5       0x2000
#define     ERROR_DISTURBANCE       0x4000
// Reusing status codes
#define     ERROR_BUSY_BIT          0x0001
#define     ERROR_SMDSP_CLK         0x0004
#define     ERROR_SOFTWARE_04       0x0008
#define     ERROR_HWG6_NOT_READY    0x0001
#define     ERROR_HWG6_NO_DETECTION 0x0002
#define     ERROR_HWG6_BROKEN       0x0004
#define     ERROR_HWG6_GLINK_FAULT  0x0008
#define     ERROR_HWG6_NO_SUPPORT   0x0010

#define     CD_NO_FAULT         0x0000  // Good cable/PCB signal paths, but no Gigabit link
#define     CD_PIN_SHORT_OR_XT  0x0001  // Pin-short or cross-talk along 2 or more cable/PCB signal paths
#define     CD_OPEN             0x0002  // One or both pins are open for a twisted pair
#define     CD_SHORT            0x0004  // Two pins from the same twisted pair are shorted together
#define     CD_FORCED           0x0008  // Persistent noise present (most likely caused by Forced 10/100) 
#define     CD_GLINK            0x0010  // Gigabit link is up and running
/* Possible future support: #define     CD_HIGH_IMPEDANCE   0x0020
   Possible future support: #define     CD_LOW_IMPEDANCE    0x0040
*/
#define     BLANK               -99

#define     RJ11_NONE           0
#define     RJ11_2              1
#define     RJ11_4              2
/* New SCD Definition */
#define     SCD_GOOD                0
#define     SCD_RJ11_2              1
#define     SCD_RJ11_4              2
#define     SCD_NO_CABLE            3
#define     SCD_REMOTE_OPEN_CABLE   4
#define     SCD_BAD_CABLE           5
#define     SCD_REMOTE_POWER_DOWN   6
#define     SCD_FORCED              7
#define     TAP5_BLOCK_PHYC         0x0800

#define     R16_FREEZE              0x0002
#define     R16_WRITEFILTERTORAM    0x0080
#define     R16_UPPERWORDSEL        0x0400
#define     R16_TAPREADPREFETCH     0x0800
#define     R16_BUSY                0x1000

#define     CONTROL_SWAP_AB         0x0001
#define     CONTROL_SWAP_CD         0x0002
#define     CONTROL_XT              0x0004  /* Check cross-talk, only effective for SW Cable Diag */
#define     CONTROL_HWG6            0x0008  /* Run HW cable diag, only effective for Gen6 core */
#define     CONTROL_BB_HWG6         0x0010  /* Run HW cable diag barebone core (skip FORCED pair check,
                                               skip A/B-C/D-swap compensation), only effective for HWG6 Cable Diag */
#define     CONTROL_GLINK_GAIN_INFO 0x0020  /* Use PGA/FFE info for Gigabit link */
#define     CONTROL_IMPROVE_ACC     0x0040  /* Improve accuracy for good pair length */
/* New control field definition for SCD retry */
#define     CONTROL_F_RETRY         0x1000

#define     EC_SHIFT                0x00001000  // used only for MODE_EC when glink is not up
#define     EC_TAPS_ONLY            0x00000000
#define     EC_MSE_SD               0x00000001
#define     EC_MSE_PGA              0x00000002

#define     MODE_PULSE_MAIN_HYBRID  0
#define     MODE_PULSE_MAIN_ONLY    1
#define     MODE_PULSE_HYBRID_ONLY  2
#define     MODE_EC                 3
#define     MODE_CABLE_DIAG         4

#define     TYPE_5414               0x00000001
#define     TYPE_5464               0x00000002
#define     TYPE_5468               0x00000003
#define     TYPE_5488               0x00000004
#define     TYPE_54980              0x00000005

#define     NUMCHAN             4
#define     ECHOLEN             192
#define     DOWNLOAD_NUMPH      2
#define     MAX_NUMPH           64
#define     START_TAP           4
#define     TAIL_LENGTH         20

#define     P_TAP2CL_1_K        823
#define     P_TAP2CL_0_K_GLINK  -4732
#define     P_TAP2CL_0_K        -4732
#define     GAIN_CHANNEL        3

#define     PER_C_LENGTH        ((DOWNLOAD_NUMPH*2)*NUMCHAN*6 + NUMCHAN*4 + 2*NUMCHAN + NUMCHAN*DOWNLOAD_NUMPH*ECHOLEN)
#define     N_RESERVED          ((NUMCHAN+1)*PER_C_LENGTH + 1 + NUMCHAN*(7+NUMCHAN) + N_MSE*NUMCHAN + 10)

/**************************************************/
//            command       rtdx_cmd[0]  rtdx_cmd[1]  rtdx_cmd[2]
//          regular CD    funtion opcode    phy         control
//-----------------------------------------------------------------
//          advanced CD   command code
#define     CMD_TEST_MDC_MDIO   -1   //               n_loops
#define     CMD_WRITE_REG       -2   //   phy         [regAddr, value]
#define     CMD_READ_REG        -3   //   phy         [regAddr, N/A]
#define     CMD_DUMP_SW_CD      -4   //
#define     CMD_PULSE_REFL      -5   //   phy        [Reserved(15), DF(1), ER(2), PW(3), TC(2), RC(2), PGA(7)]
//-----------------------------------------------------------------
//          Green Mode CD funtion opcode    phy         control
#define     CMD_GG              -6
//          Power On CD funtion opcode    phy         control
#define     CMD_PO              -7
//-----------------------------------------------------------------

/* UNSAFE MACROS -- make sure calls to these have no side effects */
#define abs(a) (((a) >= 0) ? (a) : -(a))
#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) < (b)) ? (a) : (b))

#define RTDX_FUNCTIONOPCODE 0   /* specifies which cable diag. function to perform */
#define RTDX_PHYADDR        1   /* target phy address */
#define RTDX_CONTROL        2   /* controls cable diag. features */

#define RTDX_STATUS         0   /* return status */
#define RTDX_FAULTTYPE_A    1   /* per-pair fault type (or no fault) */
#define RTDX_DISTANCECM_A   5   /* per-pair good cable length or fault location in centi-meter */
#define RTDX_CROSSTALKMAP   9   /* bits 3-0 indicates crosstalk between channel A and other channels */
                                /* bits 7-4 indicates crosstalk between channel B and other channels */
                                /* bits 11-8 indicates crosstalk between channel C and other channels */
                                /* bits 15-12 indicates crosstalk between channel D and other channels */
#define RTDX_MISC_A         10  /* returns per-pair swap, polarity, or skew information */
#define RTDX_MDIO_READ_COUNT    14  /* returns mdio_read_count */
#define RTDX_MDIO_WRITE_COUNT   15  /* returns mdio_write_count */
#define ALL_DATA_SZ         100000 /* in number of int32_t words */

/* the new structure */
typedef struct {
    int16_t     functionOpcode;       /* specifies which cable diag. function to perform */
    phy_t     phyAddr;              /* target phy address */
    int16_t     control;              /* controls cable diag. features */
    int16_t     status;               /* return status */
    int16_t     faultType[NUMCHAN];   /* per-pair fault type (or no fault) */
    int16_t     distanceCm[NUMCHAN];  /* per-pair good cable length or fault location in centi-meter */
    int16_t     crossTalkMap;         /* bits 3-0 indicates crosstalk between channel A and other channels */
                                    /* bits 7-4 indicates crosstalk between channel B and other channels */
                                    /* bits 11-8 indicates crosstalk between channel C and other channels */
                                    /* bits 15-12 indicates crosstalk between channel D and other channels */
    int16_t     misc[NUMCHAN];        /* returns per-pair swap, polarity, or skew information */
} cableDiagStruct;

typedef int32_t (*fp_t)(cableDiagStruct *pStruct);    // function pointer type

typedef struct {
    int16_t       good_mse;
    int16_t       mse_before;
    int16_t       mse_after;
    int16_t       n_noisy_taps;
    int16_t       n_empty_taps;
    int16_t       n_got;
} cableDiagDebug;

typedef struct {
    int16_t       left_index;
    int16_t       right_index;
    int16_t       plus_count;
    int16_t       minus_count;
} overflowDebug;

extern void cdMain(cableDiagStruct *pStruct);
extern int16_t combine_to_int32_t(int32_t chip_id_type, int16_t tap_index, int16_t tap_msb, int16_t tap_lsb, int32_t *ptr_results);
extern int16_t get_chip_id_type(phy_t phy, int32_t *ptr_chip_id_type, int16_t *ptr_use_default);
extern int16_t init_cd(int32_t chip_id_type, phy_t phy);
extern int16_t reconverge_ec(phy_t phy, int16_t stage, int16_t txmt_channel, int16_t tap0_value, int16_t ffe_value);
extern int16_t readFromMii(phy_t phy, int16_t regAddr, int16_t *p_value);
extern int16_t write2Mii(phy_t phy, int16_t regAddr, int16_t value);
extern void wait(phy_t phy, int32_t t_ms);
extern int16_t enable_smdsp_clock(phy_t phy, int read_confirm, int16_t *p_reg18);
extern int16_t check_smdsp_clock(phy_t phy);
extern int16_t readDSP(phy_t phy, int16_t channel, int16_t filter, int16_t tap, int16_t *p_value);
extern int16_t writeDSP(phy_t phy, int16_t channel, int16_t filter, int16_t tap, int16_t value);
extern int16_t power_control_tx_bit_pat(phy_t phy, int16_t bit_pat);
extern int16_t turn_on_off_tx(phy_t phy, int16_t txmt_channel, int on);
extern int32_t getRevisionNumber(cableDiagStruct *pStruct);
extern int32_t getCableDiag(cableDiagStruct *pStruct);
extern int32_t getLinkSpeed(cableDiagStruct *pStruct);
extern int32_t getLinkStatus(cableDiagStruct *pStruct);
extern int32_t getSwap(cableDiagStruct *pStruct);
extern int32_t getPolarity(cableDiagStruct *pStruct);
extern int32_t getSkew(cableDiagStruct *pStruct);
extern int16_t get_hybrid_residual(int32_t rtdx_cmd[]);
extern int16_t test_mdc_mdio(int32_t n_loops, int32_t *p_good_count, int32_t *p_error_count);

extern int16_t
hwg6_core_glink (int16_t improve_acc, int16_t use_gain_info, phy_t phy, int16_t use_default, int16_t faultType[],
                 int16_t distanceCm[], int32_t peak_v[NUMCHAN],
                 int16_t error[NUMCHAN+1], int16_t fault[NUMCHAN+1][NUMCHAN+1],
                 int16_t length[NUMCHAN+1][NUMCHAN+1], int16_t glink_length_100[NUMCHAN],
                 int32_t glink_gain_dB_256[NUMCHAN], int16_t qual[NUMCHAN]);
extern int16_t
hwg6_core_no_glink (int16_t improve_acc, phy_t phy, int16_t use_default, 
                    int16_t faultType[], int16_t distanceCm[], int32_t peak_v[NUMCHAN],
                    int16_t *ptr_crossTalkMap,
                    int16_t error[NUMCHAN+1], int16_t fault[NUMCHAN+1][NUMCHAN+1],
                    int16_t length[NUMCHAN+1][NUMCHAN+1]);

extern int32_t    all_data[];
extern int32_t    mdio_read_count, mdio_write_count;

extern uint16_t mdio_read_gbe( int asic_id, uint8_t portad, uint8_t dev,
                uint16_t reg);
extern void mdio_write_gbe( int asic_id, uint8_t portad, uint8_t dev,
                uint16_t reg, uint16_t data );

#endif

