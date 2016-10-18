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
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include "bcm54980_tdr.h"

/**************************************************/
int16_t
enable_smdsp_clock(phy_t phy, int read_confirm, int16_t *p_reg18)
{
    int16_t   error_flag, reg18_new;

    error_flag = GOOD;

    error_flag |= write2Mii(phy, 0x18, 0x7);        // set the read pointer to reg18 shadow 0
    error_flag |= readFromMii(phy, 0x18, p_reg18);   // read reg18 shadow 0 value
    // write reg18 value to enable smdsp clock
    error_flag |= write2Mii(phy, 0x18, (*p_reg18 | 0x0800) & ((int16_t) 0xFFF8));
    if (read_confirm !=0) {
        error_flag |= readFromMii(phy, 0x18, &reg18_new);   // read reg18 shadow 0 value
        if ((reg18_new & 0x0800) == 0)  error_flag |= ERROR_SOFTWARE_04;
    }
    return error_flag;
}
/**************************************************/
int16_t
check_smdsp_clock(phy_t phy)
{
    int16_t   error_flag, reg18;

    error_flag = GOOD;

    error_flag |= write2Mii(phy, 0x18, 0x7);        // set the read pointer to reg18 shadow 0
    error_flag |= readFromMii(phy, 0x18, &reg18);   // read reg18 shadow 0 value
    if ((reg18 & 0x0800) == 0)  error_flag |= ERROR_SMDSP_CLK;
    return error_flag;
}
/**************************************************/
int16_t
readMiscReg(phy_t phy, int16_t channel, int16_t tap, int16_t *p_value)
{
    int16_t       error_flag;
    int16_t       address;

    error_flag = GOOD;
    address = ((channel & 0x0003) << 13) | (tap & 0x00FF);
    error_flag |= write2Mii(phy, 0x17, address);
    error_flag |= readFromMii(phy, 0x15, p_value);
    return error_flag;
}
/**************************************************/
int16_t
readDSP(phy_t phy, int16_t channel, int16_t filter, int16_t tap, int16_t *p_value)
{
    int16_t       error_flag;
    int16_t       address;

    error_flag = GOOD;
    address = ((channel & 0x0007) << 13) | ((filter & 0x001F) << 8) | (tap & 0x00FF);
    error_flag |= write2Mii(phy, 0x17, address);
    error_flag |= readFromMii(phy, 0x15, p_value);
    return error_flag;
}
/**************************************************/
int16_t
writeDSP(phy_t phy, int16_t channel, int16_t filter, int16_t tap, int16_t value)
{
    int16_t       error_flag;
    int16_t       address;

    error_flag = GOOD;
    address = ((channel & 0x0007) << 13) | ((filter & 0x001F) << 8) | (tap & 0x00FF);
    error_flag |= write2Mii(phy, 0x17, address);
    error_flag |= write2Mii(phy, 0x15, value);
    return error_flag;
}
/**************************************************/
int16_t
power_control_tx_bit_pat(phy_t phy, int16_t bit_pat)
{
    int16_t       error_flag=GOOD;

    error_flag |= write2Mii(phy, 0x17, 0x0FFF);
    error_flag |= write2Mii(phy, 0x15, 0x2000u | ((bit_pat<<4) & 0x00F0u));

    return error_flag;
}
/**************************************************/
int16_t
turn_on_off_tx(phy_t phy, int16_t txmt_channel, int on)
{
    int16_t       error_flag=GOOD;
    // Misc. Tap 5 : turn on/off transmission for all or selected channel, slice to 0
    error_flag |= write2Mii(phy, 0x17, ((txmt_channel & 0x7u)<<13) | 0x0005u);
    error_flag |= write2Mii(phy, 0x15, (on ? 0x2000 : 0x3000));

    return error_flag;
}
/**************************************************/
int32_t
getLinkSpeed(cableDiagStruct *pStruct)
{   // This routine is no longer supported
    int16_t       getReg, linkStatus;
    phy_t       phyAddr;
    int16_t       error_flag;

    error_flag = GOOD;
    phyAddr = pStruct->phyAddr;
    
    error_flag |= readFromMii(phyAddr, 0x1A, &getReg);
    error_flag |= readFromMii(phyAddr, 0x11, &linkStatus);

    wait(phyAddr, 4000);                                                // wait for link
    error_flag |= readFromMii(phyAddr, 0x11, &linkStatus);
    linkStatus &= 0x0FFE;  // read again to clear stale status
    error_flag |= readFromMii(phyAddr, 0x1A, &getReg);          // check interrupts
    if (getReg & 6) {
        linkStatus = 0;
    }

    if (linkStatus == 0x0F00) {                         // valid only if linkStatus is 0x0F00
        error_flag |= readFromMii(phyAddr, 0x19, &getReg);
        pStruct->misc[0] = (getReg >> 8) & 0x7;
        return error_flag;
    } else {
        pStruct->misc[0] = 0;
        return ERROR_LINK;                  
    }
}
/**************************************************/
int32_t
getLinkStatus(cableDiagStruct *pStruct)
{   // This routine is no longer supported
    int16_t     getReg, linkStatus;
    phy_t     phyAddr;
    int16_t     error_flag;

    error_flag = GOOD;
    phyAddr = pStruct->phyAddr;

    error_flag |= readFromMii(phyAddr, 0x1A, &getReg);
    error_flag |= readFromMii(phyAddr, 0x11, &linkStatus);

    wait(phyAddr, 4000);                                                // wait for link
    error_flag |= readFromMii(phyAddr, 0x11, &linkStatus);
    linkStatus &= 0x0FFE;  // read again to clear stale status
    error_flag |= readFromMii(phyAddr, 0x1A, &getReg);          // check interrupts
    if (getReg & 6) {
        linkStatus = 0;
    }
    pStruct->misc[0] = (linkStatus >> 8) & 0x000F;

    return error_flag;              
}
/**************************************************/
int32_t
getSwap(cableDiagStruct *pStruct)
{
    int16_t       wData, wData1;
    int16_t       swapCD, ch;
    int16_t       error_flag;
    int16_t       mdix;

    error_flag = GOOD;
    error_flag |= readFromMii(pStruct->phyAddr, 0x11, &mdix);       // read MDIX
    mdix &= 0x2000;

    error_flag |= write2Mii(pStruct->phyAddr, 0x18, 7);
    error_flag |= readFromMii(pStruct->phyAddr, 0x18, &wData);
    wData |= 0x0800;
    error_flag |= write2Mii(pStruct->phyAddr, 0x18, wData);

    for (ch=0; ch<NUMCHAN; ch++) {
        error_flag |= readMiscReg(pStruct->phyAddr, ch, 0x05, &wData);
        if (wData & 0x0020) {
//          swapAB = (wData & 0x01);                        // swap pair AB 
            swapCD = ((wData >> 1) & 0x01);                 // swap pair CD 

            wData1 = (wData >> 3) & 0x03;
            switch (wData1) {                               // set pair select 
                case 0:                 // ch a
                    if (mdix)           pStruct->misc[ch] = (0x0A);
                    else                pStruct->misc[ch] = (0x0B); // swap AB
                    break;
                case 1:                 // ch b
                    if (mdix)           pStruct->misc[ch] = (0x0B);
                    else                pStruct->misc[ch] = (0x0A); // swap AB
                    break;
                case 2:                 // ch c
                    if (mdix) {
                        if (!swapCD)    pStruct->misc[ch] = (0x0C);
                        else            pStruct->misc[ch] = (0x0D); // swap CD
                    } else {
                        if (swapCD)     pStruct->misc[ch] = (0x0C);
                        else            pStruct->misc[ch] = (0x0D); // swap CD
                    }
                   break;
                case 3:                 // ch d
                    if (mdix) {
                        if (!swapCD)    pStruct->misc[ch] = (0x0D);
                        else            pStruct->misc[ch] = (0x0C); // swap CD
                    } else {
                        if (swapCD)     pStruct->misc[ch] = (0x0D);
                        else            pStruct->misc[ch] = (0x0C); // swap CD
                    }
                   break;
                default:
                   break;
            }
        } else {// no valid data
            pStruct->misc[ch] = -1;
        }
    }
    return error_flag;              
}
/**************************************************/
int32_t
getPolarity(cableDiagStruct *pStruct)
{
    int16_t       wData;
    int16_t       ch;
    int16_t       error_flag;
    int16_t       mdix;

    error_flag = GOOD;

    error_flag |= readFromMii(pStruct->phyAddr, 0x11, &mdix);       // read MDIX
    mdix &= 0x2000;

    error_flag |= write2Mii(pStruct->phyAddr, 0x18, 7);
    error_flag |= readFromMii(pStruct->phyAddr, 0x18, &wData);
    wData |= 0x800;
    error_flag |= write2Mii(pStruct->phyAddr, 0x18, wData);

    for (ch=0; ch<NUMCHAN; ch++) {
        error_flag |= readMiscReg(pStruct->phyAddr, ch, 0x05, &wData);
        if (wData & 0x0020) pStruct->misc[ch] = ((wData >> 2) & 0x01);  // polarity
        else                pStruct->misc[ch] = -1;                     // no valid data
    }
    if (mdix == 0) {
        //   if MDIX is set, we need to swap the polarity values between pairs
        //   0<->1 and 2<->3.  This scenario would correctly detect the cable
        //   pair polarity if the cable trace between the RJ45 connector to the
        //   phy is polarity correct on both the local and remote phys.  This
        //   swapping only works with straight cable.
        wData = pStruct->misc[0];
        pStruct->misc[0] = pStruct->misc[1];
        pStruct->misc[1] = wData;

        wData = pStruct->misc[2];
        pStruct->misc[2] = pStruct->misc[3];
        pStruct->misc[3] = wData;
    }
    return error_flag;
}
/**************************************************/
int32_t
getSkew(cableDiagStruct *pStruct)
{
    int16_t     wData;
    int16_t     ch;
    int16_t     error_flag;

    error_flag = GOOD;
    error_flag |= write2Mii(pStruct->phyAddr, 0x18, 7);
    error_flag |= readFromMii(pStruct->phyAddr, 0x18, &wData);
    wData |= 0x0800;
    error_flag |= write2Mii(pStruct->phyAddr, 0x18, wData);

    for (ch=0; ch<NUMCHAN; ch++) {
        error_flag |= readMiscReg(pStruct->phyAddr, ch, 0x05, &wData);
        if (wData & 0x0020) pStruct->misc[ch] =((wData >> 6) & 0x0F);  // pair Skew
        else                pStruct->misc[ch] = -1;                    // no valid data
    }
    return error_flag;
}

