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
*    09/20/2005 Init. Ver.  Minshine's rewriting
*    01/20/2006 Ver.2.0     Minshine
*    02/13/2006 Ver.2.0     Minshine disable cable diag after glink cable diag
*    01/20/2006 Ver. 2.0    Minshine Initial work
*    07/18/2006 Ver. 2.0    Minshine Major algorithm improvement
*    04/12/2007 Ver. 3.0    Minshine Fixed status code 0x0020 issue
******************************************************************************/
#include <stdio.h>
#include <string.h>
#include <sys/types.h>

#include "bcm54980_tdr.h"

#define RDWD_LOG printf

extern int bcm54980_reg_read (int fd, uint8_t bcm_inst, uint8_t port_addr,
                        uint8_t addr, uint16_t *data);
extern int bcm54980_reg_write (int fd, uint8_t bcm_inst, uint8_t port_addr,
                        uint8_t addr, uint16_t data);

extern int16_t turn_on_off_tx(phy_t phy, int16_t txmt_channel, int on);

/* asic_id and portad are encoded in upper/lower 16 bits passed as "phy" to 
* hwg6_core_no_glink and hwg6_core_glink 
*/
int16_t write2Mii( phy_t enc, int16_t reg, int16_t data)
{
    return (bcm54980_reg_write (-1, enc>>13, enc&0xFF, reg, (uint16_t)data));
//    mdio_write_gbe( (enc >>16)/*asic*/, (enc & 0xff)/*portad*/, 0/*dev*/, reg, data );
//   return 0;
}
int16_t readFromMii( phy_t enc, int16_t reg, int16_t *data )
{
     return (bcm54980_reg_read (-1, enc >> 13, enc&0xFF, reg, (uint16_t*)data));
//    *data = mdio_read_gbe( (enc >>16)/*asic*/, (enc & 0xff)/*portad*/, 0/*dev*/, reg );
//    return 0;
}


#define     HWG6_GOOD       0
#define     HWG6_OPEN       1
#define     HWG6_SHORT      2
#define     HWG6_BROKEN     3

// new settings
// 1st pass pwl
#define     OS_T_A_0    ((int16_t) 0x2E01)
#define     OS_T_B_0    ((int16_t) 0x49AC)
#define     OS_T_C_0    ((int16_t) 0x57BB)
#define     OS_T_D_0    ((int16_t) 0x27A6)
#define     OS_T_E_0    ((int16_t) 0x0785)
// for tap <= 10
#define     OS_T_F_0    ((int16_t) 0x0025)
// for 6 <= tap <= 10, 90nm only
#define     OS_T_G_0    ((int16_t) 0x0025)

// 2-5 pass pwl
#define     SS_T_10     ((int16_t) 0x0100)
#define     SS_T_32     ((int16_t) 0x1DBC)
#define     SS_T_54     ((int16_t) 0x1191)
#define     SS_T_76     ((int16_t) 0x08B8)
#define     SS_T_98     ((int16_t) 0x01E8)
// for tap <= 10
#define     SS_T_BA     ((int16_t) 0x0007)
// for 6 <= tap <= 10, 90nm only
#define     SS_T_DC     ((int16_t) 0x0007)

//cable length threshold
//mss TUNED
#define     CL_T_A      ((int16_t) 0x1E1E)
#define     CL_T_B      ((int16_t) 0x231F)
#define     CL_T_C      ((int16_t) 0x131A)
//mss recommends #define     CL_T_A      ((int16_t) 0x100D)
//mss recommends #define     CL_T_B      ((int16_t) 0x1E14)
//mss recommends #define     CL_T_C      ((int16_t) 0x0D0D)

//linked cable length threshold
//mss TUNED
#define     GL_T_A      ((int16_t) 0x1210)
#define     GL_T_B      ((int16_t) 0x1B20)
//mss recommends #define     GL_T_A      ((int16_t) 0x0C07)
//mss recommends #define     GL_T_B      ((int16_t) 0x1214)

// End of new settings

#define     P_2_100_256         ((int32_t) -808)
#define     P_1_100             ((int32_t) 1501)
#define     P_0_100             ((int32_t) -98092)
#define     GLINK_CL_MIN_TOL_M_100          ((int32_t) 300)
#define     GLINK_CL_MAX_TOL_M_FRACTION     ((int32_t) 14)
#define     GLINK_CL_MIN_TOL_A_100          ((int32_t) 600)
#define     GLINK_CL_MAX_TOL_A_FRACTION_UP  ((int32_t) 3)
#define     GLINK_CL_MAX_TOL_A_FRACTION_DOWN    ((int32_t) 4)
#define     GLINK_CL_MIN_TOL_P_100          ((int32_t) 600)
#define     GLINK_CL_MAX_TOL_P_FRACTION     ((int32_t) 10)
//Phase & PGA Cntl (Exp-Reg2Ah)
#define     PH_PGAGAIN_EXP2A    ((int16_t) 0xEEAB)    // 1st pass : 0x2B=43, 2nd pass 0x5D=93
#define     CABLE_LENGTH_EXP27  ((int16_t) 0xD2ED)
/*******************************************************************/
/* this function has 12 block register accesses */
static int16_t
hwg6_init_t (phy_t phy, int16_t use_default)
{
    int16_t   error_flag;
    
    error_flag = GOOD;
    // reset phy
    error_flag |= write2Mii(phy, 0x0, (int16_t) 0x8000);  ///block access A
    wait (phy, 1);

    error_flag |= write2Mii(phy, 0x00, 0x0040);     ///block access B // force gigbit speed
    error_flag |= write2Mii(phy, 0x1e, 0x0080);     ///block access C // manual swap mdix
//mss reg18
    //mss    error_flag |= write2Mii(phy, 0x18, 0x0025);     // enable EC as NEXT (detect both OPEN and SHORT)
//mss reg18
    error_flag |= write2Mii(phy, 0x18, 0x0C00);     ///block access C+ // enable smdsp clk
    // select Exp-2Bh : EC gain control: 1st round = "001"; 2nd round = "000"
    error_flag |= write2Mii(phy, 0x17, 0x0F2B);     ///block access D

    error_flag |= write2Mii(phy, 0x15, 0x0001);     ///block access E
    // select Exp-2Ah : phase tuning & PGA gain setting
    error_flag |= write2Mii(phy, 0x17, 0x0F2A);
//mss reg18
    error_flag |= write2Mii(phy, 0x18, 0x0282);     // turn off Super Isolate mode, Class-A
    error_flag |= write2Mii(phy, 0x15, PH_PGAGAIN_EXP2A);   ///block access F
// patch A240.b - mss - select Exp-27h cable length polynomial - added to force 0m when no cable is plugged in
    error_flag |= write2Mii(phy, 0x17, 0x0F27);
    error_flag |= write2Mii(phy, 0x15, CABLE_LENGTH_EXP27);     ///block access F+
// patch A240.c - mss - select Exp-28h cable diag. misc. control - disable tail search
    error_flag |= write2Mii(phy, 0x17, 0x0F28);
    error_flag |= write2Mii(phy, 0x15, 0x2C05);     ///block access G
    // select Exp-29h : FFE main tap override & AGC gain threshold setting
    error_flag |= write2Mii(phy, 0x17, 0x0F29);     // init the FFE main tap for ECs
    error_flag |= write2Mii(phy, 0x15, 0x0010);     ///block access H // Remember this actually is 0x0100
//mss    error_flag |= write2Mii(phy, 0x15, 0x0002);     // Remember this actually is 0x0020

    if (use_default == 0) {
        /* set good cable length thresholds */
        error_flag |= write2Mii(phy, 0x17, 0x0F1D);     //selec Exp-1D
        error_flag |= write2Mii(phy, 0x15, CL_T_A);     ///block access I
        error_flag |= write2Mii(phy, 0x17, 0x0F1E);     //selec Exp-1E
        error_flag |= write2Mii(phy, 0x15, CL_T_B);     ///block access J
        error_flag |= write2Mii(phy, 0x17, 0x0F1F);     //selec Exp-1F
        error_flag |= write2Mii(phy, 0x15, CL_T_C);     ///block access K
    }
    return error_flag;
}
/*******************************************************************/
/* this function has (8, 10) block register accesses */
static int16_t
hwg6_setup_t (phy_t phy, int16_t mode, int16_t exp10_control, int16_t use_default)
{
    int16_t       error_flag;

    error_flag = GOOD;
    if (mode == 0) {    // OPEN/SHORT and good cable and glink cable lengths
        if (use_default == 0) {
            /* set the 1st round thresholds for EC */
            error_flag |= write2Mii(phy, 0x17, 0x0F14);     ///block access K - continued //select Exp-14h
            error_flag |= write2Mii(phy, 0x15, OS_T_A_0);   ///block access L
            error_flag |= write2Mii(phy, 0x17, 0x0F15);     //select Exp-15h
            error_flag |= write2Mii(phy, 0x15, OS_T_B_0);   ///block access M
            error_flag |= write2Mii(phy, 0x17, 0x0F16);     //select Exp-16h
            error_flag |= write2Mii(phy, 0x15, OS_T_C_0);   ///block access N
            error_flag |= write2Mii(phy, 0x17, 0x0F17);     //select Exp-17h
            error_flag |= write2Mii(phy, 0x15, OS_T_D_0);   ///block access O
            error_flag |= write2Mii(phy, 0x17, 0x0F18);     //select Exp-18h
            error_flag |= write2Mii(phy, 0x15, OS_T_E_0);   ///block access P
            error_flag |= write2Mii(phy, 0x17, 0x0F19);     //select Exp-19h
            error_flag |= write2Mii(phy, 0x15, OS_T_F_0);   ///block access Q
/////////////// This is needed for forward compatibility to 90nm, assuming default value cannot be used
            error_flag |= write2Mii(phy, 0x17, 0x0F2C);     //select Exp-2Ch
            error_flag |= write2Mii(phy, 0x15, OS_T_G_0);  ///block access R
///////////////
        }
    } else {            // NEXT mode setup
        /* set the 1st round thresholds for NEXT */
        error_flag |= write2Mii(phy, 0x17, 0x0F14);     ///block access X //select Exp-14h
        error_flag |= write2Mii(phy, 0x15, SS_T_10);    ///block access Y
        error_flag |= write2Mii(phy, 0x17, 0x0F15);     //select Exp-15h
        error_flag |= write2Mii(phy, 0x15, SS_T_32);    ///block access Z
        error_flag |= write2Mii(phy, 0x17, 0x0F16);     //select Exp-16h
        error_flag |= write2Mii(phy, 0x15, SS_T_54);    ///block access AA
        error_flag |= write2Mii(phy, 0x17, 0x0F17);     //select Exp-17h
        error_flag |= write2Mii(phy, 0x15, SS_T_76);    ///block access AB
        error_flag |= write2Mii(phy, 0x17, 0x0F18);     //select Exp-18h
        error_flag |= write2Mii(phy, 0x15, SS_T_98);    ///block access AC
        error_flag |= write2Mii(phy, 0x17, 0x0F19);     //select Exp-19h
        error_flag |= write2Mii(phy, 0x15, SS_T_BA);    ///block access AD
        error_flag |= write2Mii(phy, 0x17, 0x0F2C);     //select Exp-2Ch
        /* run testmode 4 */
        error_flag |= write2Mii(phy, 0x09, (int16_t) 0x9800);  ///block access AE - /* run testmode 4 */ // testmode 4, Master/manual
        error_flag |= write2Mii(phy, 0x15, SS_T_DC);    ///block access AF // needed for forward compatibility with 90nm
    }
    error_flag |= write2Mii(phy, 0x17, 0x0F10);         // setup control bits and enable cable diag mode
    error_flag |= write2Mii(phy, 0x15, exp10_control);  ///block access S, AG // setup control bits and enable cable diag mode
    return error_flag;
}
/*******************************************************************/
/* this function has (6, 7, 5, 5) or 6 (GLINK up) block register accesses */
static int16_t
hwg6_run (phy_t phy, int16_t exp10_control, int16_t txmt_channel, int16_t error[],
          int16_t fault[NUMCHAN+1][NUMCHAN+1], int16_t length[NUMCHAN+1][NUMCHAN+1],
          int32_t peak_value[NUMCHAN+1][NUMCHAN+1])
{
    int16_t       error_flag, exp10, exp11, exp12, exp13, cd_done;
    int16_t       i;

    error_flag = GOOD;
    if (txmt_channel >= 0) {
        // turn on transmission in txmt_channel only, which has EC, other channels have NEXT
#ifdef  MIN_REG_ACCESS
        error_flag |= power_control_tx_bit_pat(phy, 0x1u << txmt_channel);
#else
        error_flag |= turn_on_off_tx(phy, 0x4u, 0);
        error_flag |= turn_on_off_tx(phy, txmt_channel, 1);
#endif
    }
    error_flag |= write2Mii(phy, 0x17, 0x0F10);     ///block access all continued S, AI, AO, AU, k // start cable diag
    error_flag |= write2Mii(phy, 0x15, exp10_control | 0x0002); ///block access T, AJ, AP, AV, l
#ifdef  NO_POLLING
    // No "finished" checking for NO_POLLING mode
    wait(phy, 40);
    cd_done = 1;
#else
    // Wait in a loop for cable diag to finish
    cd_done = 0;
// patch A240.e - mss - increase loop count (for platform with fast MDIO access)
    for (i=0; i<N_HWG6_WAIT; i++) {
        // check all-finish bit
        error_flag |= readFromMii(phy, 0x15, &exp10);
        if ((exp10 & 0x0200) != 0) {
            cd_done = 1;
            break;
        }
    }
#endif
    // Read Cable Diagnostic Results
    error_flag |= write2Mii(phy, 0x17, 0x0F11);
    error_flag |= readFromMii(phy, 0x15, &exp11);   ///block access U, AK, AQ, AW, m

    if (cd_done == 0) {
        error[txmt_channel+1] = 1;
        error_flag |= ERROR_HWG6_NOT_READY;
    // old version } else if ((exp11 & 0xF000) == 0xF000) {
    } else if ((exp11 & 0xF000) != 0) {
        error[txmt_channel+1] = 1;
        error_flag |= ERROR_HWG6_NO_DETECTION;
    } else {
        error[txmt_channel+1] = 0;
    }

    for (i=0; i<NUMCHAN; i++)   fault[i][txmt_channel+1] = (exp11 >> (i*2)) & 0x0003;
    // Check location report of channels 0, 1, 2, 3
    if (txmt_channel <= 0) {
        error_flag |= write2Mii(phy, 0x17, 0x0F12);     ///block access U+, AK+, m+
        error_flag |= readFromMii(phy, 0x15, &exp12);   ///block access U++, AK++, m++
        length[0][txmt_channel+1] = error[txmt_channel+1] ? BLANK : (exp12 & 0x00FF);
        length[1][txmt_channel+1] = error[txmt_channel+1] ? BLANK : ((exp12 & 0xFF00) >> 8);
    }
    error_flag |= write2Mii(phy, 0x17, 0x0F13); ///block access V, AK+++, AQ+, AW+, m+++
    error_flag |= readFromMii(phy, 0x15, &exp13);   ///block access W, AL, AR, AX, n
    length[2][txmt_channel+1] = error[txmt_channel+1] ? BLANK : (exp13 & 0x00FF);
    length[3][txmt_channel+1] = error[txmt_channel+1] ? BLANK : ((exp13 & 0xFF00) >> 8);

#ifdef BRCM_DBG ////////////////////////
#define     EXP28_VALUE         ((int16_t) 0x2C05)
    error_flag |= write2Mii(phy, 0x17, 0x0F28);
    error_flag |= write2Mii(phy, 0x15, EXP28_VALUE | 0x8000);
    error_flag |= write2Mii(phy, 0x17, 0x0F12);
    error_flag |= readFromMii(phy, 0x15, &exp12);
    error_flag |= write2Mii(phy, 0x17, 0x0F13);
    error_flag |= readFromMii(phy, 0x15, &exp13);

    peak_value[0][txmt_channel+1] = (int32_t) (error[txmt_channel+1] ? BLANK : (exp12 & 0x00FF)) * 256;
    peak_value[1][txmt_channel+1] = (int32_t) (error[txmt_channel+1] ? BLANK : (exp12 >> 8)) * 256;
    peak_value[2][txmt_channel+1] = (int32_t) (error[txmt_channel+1] ? BLANK : (exp13 & 0x00FF)) * 256;
    peak_value[3][txmt_channel+1] = (int32_t) (error[txmt_channel+1] ? BLANK : (exp13 >> 8)) * 256;

    error_flag |= write2Mii(phy, 0x17, 0x0F28);
    error_flag |= write2Mii(phy, 0x15, EXP28_VALUE);

    fault[4][txmt_channel+1] = exp10;   // for debug purpose
    length[4][txmt_channel+1] = exp11;  // for debug purpose
#endif          ////////////////////////

    return error_flag;
}
/*******************************************************************/
static int16_t
improve_accuracy (int16_t len_100[NUMCHAN], int16_t qual[NUMCHAN])
{
    int16_t       txmt_channel, rcvr_channel, counter, match[NUMCHAN][NUMCHAN], lone_channel, best_lone_channel;
    int16_t       found_clique, best_channel_A, best_channel_B, s_best_channel_A, s_best_channel_B;
    int32_t       tol_m_100, tol_p_100, total_sum_100, min_len_100, best_diff_100, s_best_diff_100, diff_100; 
    int32_t       est_len_100, limit_value, best_clique_cost, clique_cost;

    best_channel_A = -1;
    best_channel_B = -1;
    best_diff_100 = 32766;
    s_best_channel_A = -1;
    s_best_channel_B = -1;
    s_best_diff_100 = 32767;
    counter = 0;
    for (txmt_channel=0; txmt_channel<NUMCHAN; txmt_channel++) {
        for (rcvr_channel=txmt_channel+1; rcvr_channel<NUMCHAN; rcvr_channel++) {
            match[txmt_channel][rcvr_channel] = 0;
            match[rcvr_channel][txmt_channel] = 0;
            if ((qual[rcvr_channel] == 0) || (qual[txmt_channel] == 0)) continue;
            min_len_100 = min(len_100[rcvr_channel], len_100[txmt_channel]);
            tol_m_100 = max(GLINK_CL_MIN_TOL_M_100, 
                            (min_len_100+GLINK_CL_MAX_TOL_M_FRACTION/2) / GLINK_CL_MAX_TOL_M_FRACTION);
            diff_100 = abs(len_100[rcvr_channel] - len_100[txmt_channel]);
            if (diff_100 <= tol_m_100) {
                match[txmt_channel][rcvr_channel] = 1;
                match[rcvr_channel][txmt_channel] = 1;
                counter++;
                // Record the best pair-pair match so far
                if (diff_100 < best_diff_100) {
                    s_best_channel_A = best_channel_A;
                    s_best_channel_B = best_channel_B;
                    s_best_diff_100 = best_diff_100;
                    best_channel_A = txmt_channel;
                    best_channel_B = rcvr_channel;
                    best_diff_100 = diff_100;
                } else if (diff_100 < s_best_diff_100) {
                    s_best_channel_A = txmt_channel;
                    s_best_channel_B = rcvr_channel;
                    s_best_diff_100 = diff_100;
                }
            }
        }
    }
    if (counter == (NUMCHAN*(NUMCHAN-1)/2)) {    // full clique
        return NUMCHAN;
    } else {
        // finding (NUMCHAN-1)-clique
        best_lone_channel = -1;
        best_clique_cost = 100000;
        for (lone_channel=0; lone_channel<NUMCHAN; lone_channel++) {
            found_clique = 1;
            clique_cost = 0;
            for (txmt_channel=0; txmt_channel<NUMCHAN; txmt_channel++) {
                if (lone_channel == txmt_channel)   continue;
                for (rcvr_channel=txmt_channel+1; rcvr_channel<NUMCHAN; rcvr_channel++) {
                    if (lone_channel == rcvr_channel)   continue;
                    clique_cost += abs(len_100[rcvr_channel] - len_100[txmt_channel]);
                    if (match[rcvr_channel][txmt_channel] == 0) found_clique = 0;
                }
            }
            if ((found_clique == 1) && (clique_cost < best_clique_cost)) {
                best_lone_channel = lone_channel;
                best_clique_cost = clique_cost;
            }
        }
        if (best_lone_channel != -1) {
            total_sum_100 = 0;
            for (rcvr_channel=0; rcvr_channel<NUMCHAN; rcvr_channel++) {
                if (best_lone_channel == rcvr_channel)   continue;
                total_sum_100 += len_100[rcvr_channel];
            }
            est_len_100 = total_sum_100 / (NUMCHAN-1);
            tol_p_100 = max(GLINK_CL_MIN_TOL_P_100, 
                            (est_len_100+GLINK_CL_MAX_TOL_P_FRACTION/2) / GLINK_CL_MAX_TOL_P_FRACTION);
            if (len_100[best_lone_channel] <= est_len_100) {
                limit_value = max(len_100[best_lone_channel], est_len_100-tol_p_100);
            } else {
                limit_value = min(len_100[best_lone_channel], est_len_100+tol_p_100);
            }
            len_100[best_lone_channel] = (int16_t) ((est_len_100+limit_value+1) / 2);
            return (NUMCHAN-1);
        } else if (best_channel_A != -1) {
            if ((s_best_channel_A != -1)                &&
                (s_best_channel_A != best_channel_A)    &&
                (s_best_channel_A != best_channel_B)    &&
                (s_best_channel_B != best_channel_A)    &&
                (s_best_channel_B != best_channel_B)    &&
                (abs(s_best_diff_100 - best_diff_100) <= 200)) {
                total_sum_100 = 0;
                for (rcvr_channel=0; rcvr_channel<NUMCHAN; rcvr_channel++) {
                    total_sum_100 += len_100[rcvr_channel];
                }
                for (rcvr_channel=0; rcvr_channel<NUMCHAN; rcvr_channel++) {
                    len_100[rcvr_channel] = (int16_t) (total_sum_100 / NUMCHAN);
                }
                return 1;
            } else {
                est_len_100 = (len_100[best_channel_A] + len_100[best_channel_B]+1) / 2;
                tol_p_100 = max(GLINK_CL_MIN_TOL_P_100, 
                                (est_len_100+GLINK_CL_MAX_TOL_P_FRACTION/2) / GLINK_CL_MAX_TOL_P_FRACTION);
                for (rcvr_channel=0; rcvr_channel<NUMCHAN; rcvr_channel++) {
                    if ((rcvr_channel == best_channel_A) || (rcvr_channel == best_channel_B))    continue;
                    if (len_100[rcvr_channel] <= est_len_100) {
                        limit_value = max(len_100[rcvr_channel], est_len_100-tol_p_100);
                    } else {
                        limit_value = min(len_100[rcvr_channel], est_len_100+tol_p_100);
                    }
                    len_100[rcvr_channel] = (int16_t) ((est_len_100+limit_value+1) / 2);
                }
                return 2;
            }
        } else {
            return 0;
        }
    }
    return -1;
}
/*******************************************************************/
static int16_t
hwg6_process_results (int16_t improve_acc, int16_t fault[NUMCHAN+1][NUMCHAN+1],
                      int16_t length[NUMCHAN+1][NUMCHAN+1],
                      int32_t peak_value[NUMCHAN+1][NUMCHAN+1],
                      int16_t faultType[], int16_t distanceCm[],
                      int32_t peak_v[NUMCHAN], int16_t *ptr_crossTalkMap)
{
    int16_t       error_flag;
    int16_t       x_channel, y_channel, txmt_channel, rcvr_channel, found_XT;
    int16_t       len_100[NUMCHAN], qual[NUMCHAN];

    error_flag = GOOD;
    *ptr_crossTalkMap = 0;
    /* Processing the results of all passes */
    for (rcvr_channel=0; rcvr_channel<NUMCHAN; rcvr_channel++) {
        distanceCm[rcvr_channel] = (int16_t) 0x7FFF;
        found_XT = 0;
        for (x_channel=0; x_channel<NUMCHAN; x_channel++) {
            if (x_channel == rcvr_channel)   continue;
            txmt_channel = min (rcvr_channel, x_channel);
            y_channel = max (rcvr_channel, x_channel);

            if ((fault[y_channel][txmt_channel+1] == HWG6_OPEN) ||
                (fault[y_channel][txmt_channel+1] == HWG6_SHORT) ) {
                // y_channel shorted to txmt_channel
                faultType[rcvr_channel] = CD_PIN_SHORT_OR_XT;
                distanceCm[rcvr_channel] = min(distanceCm[rcvr_channel], length[y_channel][txmt_channel+1] * 100);
                if (distanceCm[rcvr_channel] > length[y_channel][txmt_channel+1] * 100) peak_v[rcvr_channel] = peak_value[y_channel][txmt_channel+1];
                *ptr_crossTalkMap |= (0x1 << (x_channel * NUMCHAN + rcvr_channel));
                found_XT = 1;
            }
        }
        if (found_XT==0) {
            switch (fault[rcvr_channel][0]) {
                case HWG6_GOOD:
                    faultType[rcvr_channel] = CD_NO_FAULT;
                    break;
                case HWG6_OPEN:
                    faultType[rcvr_channel] = CD_OPEN;
                    break;
                case HWG6_SHORT:
                    faultType[rcvr_channel] = CD_SHORT;
                    break;
                case HWG6_BROKEN:
                    faultType[rcvr_channel] = CD_PIN_SHORT_OR_XT;
                    error_flag |= ERROR_HWG6_BROKEN;
                    break;
                default:
                    faultType[rcvr_channel] = BLANK;
                    error_flag |= ERROR_HWG6_NO_SUPPORT;
                    break;
            }
            distanceCm[rcvr_channel] = length[rcvr_channel][0] * 100;
            peak_v[rcvr_channel] = peak_value[rcvr_channel][0];
        }
    }

    for (rcvr_channel=0; rcvr_channel<NUMCHAN; rcvr_channel++) {
        len_100[rcvr_channel] = distanceCm[rcvr_channel];
        if (faultType[rcvr_channel] == CD_NO_FAULT) qual[rcvr_channel] = 1;
        else                                        qual[rcvr_channel] = 0;
    }
    if (improve_acc != 0) {
        (void) improve_accuracy (len_100, qual);
        for (rcvr_channel=0; rcvr_channel<NUMCHAN; rcvr_channel++) {
            if (faultType[rcvr_channel] == CD_NO_FAULT)     distanceCm[rcvr_channel] = len_100[rcvr_channel];
        }
    }
    return error_flag;
}
/*******************************************************************/
int16_t
hwg6_core_no_glink (int16_t improve_acc, phy_t phy, int16_t use_default,
                    int16_t faultType[], int16_t distanceCm[],
                    int32_t peak_v[NUMCHAN], int16_t *ptr_crossTalkMap,
                    int16_t error[NUMCHAN+1], int16_t fault[NUMCHAN+1][NUMCHAN+1],
                    int16_t length[NUMCHAN+1][NUMCHAN+1])
{
    int16_t       txmt_channel, error_flag;
//    int16_t       some_value;
    int16_t       exp10_control = 0;
    int32_t       peak_value[NUMCHAN+1][NUMCHAN+1];

    error_flag = GOOD;
// General strategy:
// txmt_channel,  round       functionality
// -------------------------------------------------------------------------
//     -1,        first,      detect Simple Open/Short with smallest gain setting
//     -1,        second-A,   short-between-pairs disabled by exp. reg.10 setting
//     -1,        second-B,   detect good pair length with larger gain setting
//    0-2,        first,      detect cross-talk with smallest gain setting
//    0-2,        second-A,   two-pass disabled by exp. reg.10 setting
//    0-2,        second-B,   two-pass disabled by exp. reg.10 setting
//      3,        all         skipped for minimum reg access
// -------------------------------------------------------------------------
    error_flag |= hwg6_init_t (phy, use_default);
    // txmt_channel is -1 for simple OPEN/SHORT and good pair lengths
    // txmt_channel is 0-2 for all NEXT cross-talk configurations
    for (txmt_channel=-1; txmt_channel<(NUMCHAN-1); txmt_channel++) {
        if (txmt_channel == -1) {
            exp10_control = 0x0089;
            error_flag |= hwg6_setup_t (phy, 0, exp10_control, use_default);
        } else if (txmt_channel == 0) {
            exp10_control = 0x0009;
            error_flag |= hwg6_setup_t (phy, 1, exp10_control, use_default);
        }
        error_flag |= hwg6_run (phy, exp10_control, txmt_channel, error, fault, length, peak_value);
    }
    error_flag |= hwg6_process_results (improve_acc, fault, length, peak_value,
                                        faultType, distanceCm, peak_v, ptr_crossTalkMap);

    error_flag |= write2Mii(phy, 0x0, (int16_t) 0x8000);  ///block access AY // reset phy again
    wait (phy, 1);
//mss // Do maintenance work for DVT - should be deleted for production
//mss reg18
//mss    error_flag |= write2Mii(phy, 0x18, (int16_t) 0x0282); // Turn off Super-isolate, set class A
//mss    error_flag |= write2Mii(phy, 0x18, (int16_t) 0x7007);
//mss    error_flag |= readFromMii(phy, 0x18, &some_value);
//mss    error_flag |= write2Mii(phy, 0x18, some_value | 0x0007 | (int16_t) 0x8000 | 0x0010);

    return error_flag;
}
/*******************************************************************/
/* Input can be any unsigned 16-bit integer value [1-65535] inclusive
   Output is left-shifted 16 bits (scaled up by 65536) as a signed 32-bit integer */
static int32_t
fxp_log_65536 (uint16_t x, int16_t n)
{
    const   int32_t   two_2q = 0x16A0A;
    const   int32_t   ln_two = 0xB172;
    const   int32_t   F[10] = {
                65536/1, 65536/2, 65536/3, 65536/4, 65536/5, 
                65536/6, 65536/7, 65536/8, 65536/9, 65536/10};

    int32_t       t32, z, ys, acc;
    int16_t       BM, i;

    if (n>10)               return (0x80000000);
    // find the leading 1
    t32 = (int32_t) x;
    BM = 15;
    for (i=0; i<16; i++) {
        if (t32 & 0x8000)   break;
        t32 <<= 1;
        BM--;
    }
    if (BM<0)                return (0x80000000);

    t32 = (int32_t) x;
    if ((t32 << (16-BM)) > two_2q)       BM++;

    z = (int32_t) x;
    z = (((int32_t) 1)<<16) - (z << (16-BM));

    ys = z;
    acc = ys;
    for (i=2; i<=n; i++) {
        ys = ((ys>>1) * (z>>1)) >> 14;
        acc += (((ys>>1) * (F[i-1]>>1)) >> 14);
    }
    return (ln_two*BM - acc);
}
/*******************************************************************/
static int16_t
pga_ffe_glink_length (int32_t chip_id_type, phy_t phy, int16_t rcvr_channel, int16_t glink_length_100[],
                      int32_t glink_gain_dB_256[NUMCHAN])
{
    const   int32_t   E = 0x003E;
    const   int32_t   F = 0x08B0;
    const   int16_t   NUMAVG = 8;
    int16_t   error_flag, tap0_value, ffe_value, i;
    int32_t   sum_pga, sum_ffe, pga_dB_256, gain_dB_256;

    error_flag = GOOD;
    sum_pga = 0;
    sum_ffe = 0;
    for (i=0; i<NUMAVG; i++) {
        // tap #0, read PGA gain in dB, set DSP access address, rcvr_channel is actually chip's hardware channel number
        error_flag |= write2Mii(phy, 0x17, (rcvr_channel<<13)); ///block access d0 through d7
        error_flag |= readFromMii(phy, 0x15, &tap0_value);      ///block access e0 through e7
        sum_pga += (tap0_value & 0x007F);

        error_flag |= write2Mii(phy, 0x17, (rcvr_channel<<13) | 0x0100);    ///block access f0 through f7
        error_flag |= readFromMii(phy, 0x15, &ffe_value);       ///block access g0 through g7
        sum_ffe += ffe_value;
    }
    pga_dB_256 = ((sum_pga + 4)>>3) * E;
    gain_dB_256 = (((F>>1) * (fxp_log_65536((uint16_t) ((sum_ffe+4)>>3), 6)>>1)) >> 14) + pga_dB_256;
    glink_length_100[rcvr_channel] = (int16_t) (max(0, 
                        ((((gain_dB_256 * gain_dB_256) >> 16 ) * P_2_100_256) >> 8)
                        + ((gain_dB_256 * P_1_100) >> 8) + P_0_100            ) );
    glink_gain_dB_256[rcvr_channel] = gain_dB_256;
    return error_flag;
}
/*******************************************************************/
static int16_t
qualify_glink_length (int16_t fault[NUMCHAN+1][NUMCHAN+1], int16_t length[NUMCHAN+1][NUMCHAN+1],
                int16_t glink_length_100[NUMCHAN], int16_t qual[NUMCHAN])
{
    int16_t       error_flag, rcvr_channel;
    int32_t       tol_a_100_UP, tol_a_100_DOWN;

    error_flag = GOOD;

    for (rcvr_channel=0; rcvr_channel<NUMCHAN; rcvr_channel++) {
        if (fault[rcvr_channel][0] != HWG6_GOOD)    error_flag |= ERROR_HWG6_GLINK_FAULT;
        tol_a_100_UP = max(GLINK_CL_MIN_TOL_A_100,
            (glink_length_100[GAIN_CHANNEL]+GLINK_CL_MAX_TOL_A_FRACTION_UP/2) / GLINK_CL_MAX_TOL_A_FRACTION_UP);
        tol_a_100_DOWN = max(GLINK_CL_MIN_TOL_A_100,
            (glink_length_100[GAIN_CHANNEL]+GLINK_CL_MAX_TOL_A_FRACTION_DOWN/2) / GLINK_CL_MAX_TOL_A_FRACTION_DOWN);
        if (
            ((length[rcvr_channel][0] * 100) <= (glink_length_100[GAIN_CHANNEL]+tol_a_100_UP)) &&
            ((length[rcvr_channel][0] * 100) >= (glink_length_100[GAIN_CHANNEL]-tol_a_100_DOWN)) ) {
            qual[rcvr_channel] = 1;
        } else {
            qual[rcvr_channel] = 0;
        }
    }
    return error_flag;
}
/*******************************************************************/
int16_t
hwg6_core_glink (int16_t improve_acc, int16_t use_gain_info, phy_t phy, int16_t use_default,
                 int16_t faultType[], int16_t distanceCm[],
                 int32_t peak_v[NUMCHAN],
                 int16_t error[NUMCHAN+1], int16_t fault[NUMCHAN+1][NUMCHAN+1],
                 int16_t length[NUMCHAN+1][NUMCHAN+1], int16_t glink_length_100[NUMCHAN],
                 int32_t glink_gain_dB_256[NUMCHAN], int16_t qual[NUMCHAN])
{
    int16_t       rcvr_channel, error_flag;
    int16_t       reg18;
    int16_t       exp10_control;
    int32_t       peak_value[NUMCHAN+1][NUMCHAN+1];

    error_flag = GOOD;

//mss reg18
    error_flag |= write2Mii(phy, 0x18, 0x7);        ///block access a // set the read pointer to reg18 shadow 0
    error_flag |= readFromMii(phy, 0x18, &reg18);   ///block access b // read reg18 shadow 0 value
    // write reg18 value to enable smdsp clock
    error_flag |= write2Mii(phy, 0x18, (reg18 | 0x0800) & ((int16_t) 0xFFF8));    ///block access c

    if (use_default == 0) {
        error_flag |= write2Mii(phy, 0x17, 0x0F20);     ///block access h //selec Exp-20h
        error_flag |= write2Mii(phy, 0x15, GL_T_A);     ///block access i
        error_flag |= write2Mii(phy, 0x17, 0x0F21);     //selec Exp-21h
        error_flag |= write2Mii(phy, 0x15, GL_T_B);     ///block access j
    }
    error_flag |= write2Mii(phy, 0x17, 0x0F10);     // setup control bits and enable cable diag mode
    exp10_control = 0x0005;
    error_flag |= write2Mii(phy, 0x15, exp10_control);     ///block access k
    error_flag |= hwg6_run (phy, exp10_control, -1, error, fault, length, peak_value);

    for (rcvr_channel=0; rcvr_channel<NUMCHAN; rcvr_channel++) {
        faultType[rcvr_channel] = CD_GLINK;
        distanceCm[rcvr_channel] = length[rcvr_channel][0] * 100;
        peak_v[rcvr_channel] = peak_value[rcvr_channel][0];
    }

    if (improve_acc != 0) {
        if (use_gain_info != 0) {
            error_flag |= pga_ffe_glink_length (TYPE_5468, phy, GAIN_CHANNEL, glink_length_100,
                                                glink_gain_dB_256);
            error_flag |= qualify_glink_length (fault, length, glink_length_100, qual);
        } else {
            for (rcvr_channel=0; rcvr_channel<NUMCHAN; rcvr_channel++)  qual[rcvr_channel] = 1;
        }
        if (improve_accuracy (distanceCm, qual) <= 0) {
            if (use_gain_info != 0) {
                for (rcvr_channel=0; rcvr_channel<NUMCHAN; rcvr_channel++) {
                    distanceCm[rcvr_channel] = glink_length_100[GAIN_CHANNEL];
                }
            }
        }
    }
// patch A240.d - mss - disable cable diag and set back to power-on default
    error_flag |= write2Mii(phy, 0x17, 0x0F10);     ///block access n - continued
    error_flag |= write2Mii(phy, 0x15, 0x0080);     ///block access o 
    // restore reg18 shadow 0
//mss reg18
    error_flag |= write2Mii(phy, 0x18, reg18 & 0xFFF8); ///block access p // single instruction since it's a reg18 write

    return error_flag;
}

char *fault_to_str( int fault )
{
    if (fault == 0) return  "Good     ";
    if (fault == 1) return  "Short/Xt ";
    if (fault == 2) return  "Open     ";
    if (fault == 4) return  "Short    ";
    if (fault == 8) return  "Forced   ";
    if (fault == 16) return "Glink    ";
    return( "        " );
}

void run_cable_diags( int asic_id, int portad, int link_st )
{
    int16_t faultType[NUMCHAN];
    int16_t distanceCm[NUMCHAN];
    int32_t peak_v[NUMCHAN];
    int16_t error[NUMCHAN+1];
    int16_t fault[NUMCHAN+1][NUMCHAN+1];
    int16_t length[NUMCHAN+1][NUMCHAN+1];
    int16_t glink_length_100[NUMCHAN];
    int32_t glink_gain_dB_256[NUMCHAN];
    int16_t qual[NUMCHAN];
    int16_t crossTalkMap;
    int16_t rc;

    if (link_st == 1) {
        rc = hwg6_core_glink(1/*improve_acc*/, 
                             0 /*use_gain_info*/, 
                             (asic_id << 16) | (portad)/*phy*/,
                             0/*use_default*/,
                            faultType, 
                            distanceCm,
                            peak_v,
                            error,
                            fault,
                            length,
                            glink_length_100,
                            glink_gain_dB_256,
                            qual );
    }
    else {
        rc = hwg6_core_no_glink ( 1/*improve_acc*/, 
                                (asic_id << 16) | (portad)/*phy*/, 
                                0 /*use_default*/,
                                faultType, 
                                distanceCm,
                                peak_v,
                                &crossTalkMap,
                                error,
                                fault,
                                length );
    }

    RDWD_LOG("ASIC: %d PHY: %d\n", asic_id, portad);
    if (rc != GOOD) {
        RDWD_LOG("*** Cable diagnostic returned error: %x (%d)\n", rc, rc);
    }

    RDWD_LOG("+-------+------------+----------+\n");
    RDWD_LOG("| Cable |   Status   | Distance |\n");
    RDWD_LOG("| Pair  |            | to Fault |\n");                  
    RDWD_LOG("+-------+------------+----------+\n");
    RDWD_LOG("|  1:0  | %s  | %-4d(cm) |\n", fault_to_str( faultType[0]), distanceCm[0] );
    RDWD_LOG("|  3:2  | %s  | %-4d(cm) |\n", fault_to_str( faultType[1]), distanceCm[1] );
    RDWD_LOG("|  5:4  | %s  | %-4d(cm) |\n", fault_to_str( faultType[2]), distanceCm[2] );
    RDWD_LOG("|  7:6  | %s  | %-4d(cm) |\n", fault_to_str( faultType[3]), distanceCm[3] );
    RDWD_LOG("+-------+------------+----------+\n");
}

            
