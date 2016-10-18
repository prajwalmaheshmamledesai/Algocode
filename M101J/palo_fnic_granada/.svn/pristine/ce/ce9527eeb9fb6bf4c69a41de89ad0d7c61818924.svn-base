/********************************************************************
 *
 *      File:   portola_mac_hw_dover.c
 *      Name:   Juan Munoz
 *
 *      Description:
 *      MAC dispatch layer. Appropriate driver HW layer function
 *      called thru function table.
 *
 *      Copyright (c) 2009 by Cisco Systems, Inc.
 *      All rights reserved.
 *
 ********************************************************************/
#include <stdio.h> 
#include <stdint.h> 
#include <unistd.h> 
#include "woo_ops.h"
#include "diag_main.h"

#define PRT_HI_GBE_CFG_IPG_TRANSMIT_TIME_SHIFT	17
#define PRT_HI_GBE_CFG_RX_MIN_SIZE_SHIFT	14
#define PRT_HI_GBE_CFG_RX_MAX_SIZE_SHIFT	0

#define mac_rd	asic_rd
#define mac_wr	asic_wr

// fwd refs

int mac_disable_dover( int asic_id, int asic_port);
int mac_flush_dover( int asic_id, int asic_port);
int mac_un_flush_dover( int asic_id, int asic_port);

/*****************************************************************************
* wait_100_sysclks
*****************************************************************************/
void wait_100_sysclks( int asic_id, int asic_port)
{
    /*  wait for clocks to propagate:
    * Each sysClk is 8 ns long, so this is about 1 us -- Patricio
    * our access time is about 3us, so no-op */
//    printf("Bring up [dover] delay 100 CLKs\n");
    usleep(100);
}

/*****************************************************************************
* mac_bring_up_dover
*
*****************************************************************************/
int asic_mac_bring_up_dover( int asic_id, int asic_port, int cfgd_speed)
{
    unsigned int data;
    int speed_setting = 2 /*Ge*/;

    DIAG_PRINT(DIAG_PRINT_CONFIG,
               "  PRT%d HIF%d Bring up to %dMbps\n", 
              asic_id, asic_port, cfgd_speed);

       mac_wr( asic_id, HI_REG_ADDR(CFG_DEBUG_PORT, asic_port),      0x300000 );
//        mac_wr( asic_id, HI_REG_ADDR(GBE_CFG_PAUSE_FC_LIMIT, asic_port), 0x100 );
        mac_wr( asic_id, HI_REG_ADDR(GBE_CFG_MII_MDC_PERIOD, asic_port),    60 );

        /* For dover MAC, the TX_IPG should always be configured 4 less than actual
        * as it always adds 4 extra IPG bytes.*/
        mac_wr( asic_id, HI_REG_ADDR(GBE_CFG, asic_port),
                ((1 << PRT_HI_GBE_CFG_FLD_RX_LINK_RST)   |
                 (1 << PRT_HI_GBE_CFG_FLD_RX_CRC_CHK_EN) |
                 (8 << PRT_HI_GBE_CFG_IPG_TRANSMIT_TIME_SHIFT )) );
        wait_100_sysclks(asic_id, asic_port);

        /* release link_rst */
        mac_wr( asic_id, HI_REG_ADDR(GBE_CFG, asic_port),
                ((1 << PRT_HI_GBE_CFG_FLD_TX_FORCE_XMIT_DATA) | // XXX done for dover in new mac, really needed???
                 (1 << PRT_HI_GBE_CFG_FLD_RX_CRC_CHK_EN)      |
                 (8 << PRT_HI_GBE_CFG_IPG_TRANSMIT_TIME_SHIFT )) );

        /* assert tx/rx sw_rst */
        mac_wr( asic_id, HI_REG_ADDR(GBE_CFG, asic_port),
                ((1 << PRT_HI_GBE_CFG_FLD_RX_SW_RST)          |
                 (1 << PRT_HI_GBE_CFG_FLD_TX_SW_RST)          |
                 (1 << PRT_HI_GBE_CFG_FLD_TX_FORCE_XMIT_DATA) | // XXX done for dover in new mac, really needed???
                 (1 << PRT_HI_GBE_CFG_FLD_RX_CRC_CHK_EN)      |
                 (8 << PRT_HI_GBE_CFG_IPG_TRANSMIT_TIME_SHIFT )) );
        
        /* reset PCS including both RX and TX */
        mac_wr( asic_id, HI_REG_ADDR(CFG_PCS_RESET, asic_port), 1 );
        wait_100_sysclks(asic_id, asic_port);
    
        /* default to SGMII mode as requested by Yu */
        mac_wr( asic_id, HI_REG_ADDR(CFG_GMII_MODE, asic_port), 1 );

        /* see what speed we are currently at */
        if (cfgd_speed == 1000) {
            speed_setting = 2; /*Ge*/
        } else if (cfgd_speed == 100) {
            speed_setting = 1; /*100Mb*/
        } else if (cfgd_speed == 10){
            speed_setting = 0; /*10Mb*/
        }
        mac_rd( asic_id, HI_REG_ADDR(CFG_PCS_LINK, asic_port), &data );
        mac_wr( asic_id, HI_REG_ADDR(CFG_PCS_LINK, asic_port), 
			((data & ~3) | speed_setting));
        
        /* The tx align procedure per Yu Fang's guideline
         *
         * By default, I think the firmware left 'cfg_pcs_tx_mac_align/en'
         * set all the time. This is not the intended way to use.
         * The correct way should be:
         * assert 'cfg_pcs_reset'
         * set 'cfg_pcs_link/speed' to desired speed
         * optionally set 'cfg_pcs_tx_mac_align/en' to 1 with desired value for
         *'cfg_pcs_tx_mac_align/phase'
         * de-assert 'cfg_pcs_reset'
         * set 'cfg_pcs_tx_mac_align/en' to 0
         * enable traffic
         */
        // assert the TX align.
        mac_wr(asic_id, HI_REG_ADDR(CFG_PCS_TX_MAC_ALIGN, asic_port), 1);
        
        /* remove PCS reset */
        mac_wr( asic_id, HI_REG_ADDR(CFG_PCS_RESET, asic_port), 0 );
        wait_100_sysclks(asic_id, asic_port);

        // de-assert the TX align
        mac_wr(asic_id, HI_REG_ADDR(CFG_PCS_TX_MAC_ALIGN, asic_port), 0);

        /* remove tx/rx sw_rst */
        mac_wr( asic_id, HI_REG_ADDR(GBE_CFG, asic_port),
                ( (1 << PRT_HI_GBE_CFG_FLD_TX_FORCE_XMIT_DATA) | // XXX done for dover in new mac, really needed???
                 (1 << PRT_HI_GBE_CFG_FLD_RX_CRC_CHK_EN)      |
                 (8 << PRT_HI_GBE_CFG_IPG_TRANSMIT_TIME_SHIFT )) );
        wait_100_sysclks(asic_id, asic_port);

        /* Set MTU. Also enable Rx flush */
        mac_wr( asic_id, HI_REG_ADDR(GBE_CFG_RX, asic_port),
                ( (64       << PRT_HI_GBE_CFG_RX_MIN_SIZE_SHIFT) |
                 ((9216+6) << PRT_HI_GBE_CFG_RX_MAX_SIZE_SHIFT)));

        /* Enable tx flush */
        mac_wr( asic_id, HI_REG_ADDR(GBE_CFG_TX_FLUSH_EN, asic_port), 0 );
#if 0
        /* This is REQUIRED to "fix" a bug in the 1G (Dover) MAC having to do with
        * not getting line rate on 65-byte packets. */
        mac_wr( asic_id, HI_REG_ADDR(CFG_SPARE, asic_port), 4 );
#endif
        mac_wr( asic_id, HI_REG_ADDR(GBE_CFG_PAUSE_SAMPLE_TIMER, asic_port), 0xfc000);
       
        /* sig_loss mask needs to be set for Rx packet reception */
        mac_wr( asic_id, HI_REG_ADDR(CFG_SERDES_SIG_LOSS_MASK, asic_port), 3);
 
        /* clear any interrupts generated by the bring-up sequence */
        mac_wr( asic_id, HI_REG_ADDR(GBE_INT, asic_port), -1 );

	// Clear Status
        mac_wr(asic_id, HI_REG_ADDR(CFG_PCS_RX_STATUS_CLEAR, asic_port), 0x01);
        mac_wr(asic_id, HI_REG_ADDR(CFG_PCS_RX_STATUS_CLEAR, asic_port), 0x00);

        /* expect status to be link up at end of sequence(?) */
        mac_rd(asic_id, HI_REG_ADDR(STA_PCS_RX_STATUS, asic_port), &data);
        if ((data & 0x18) != 0x18) {
            printf("  Warn: Port%d PCS not yet ready. PRT_HI_STA_PCS_RX_STATUS: %x\n", 
			asic_port, data);
        }
    return 0;
}

/*****************************************************************************
* mac_enable_dover
*
*****************************************************************************/
int mac_enable_dover( int asic_id, int asic_port)
{

    DIAG_PRINT(DIAG_PRINT_CONFIG,
               "  PRT%d HIF%d> Enable [Dover]\n", asic_id, asic_port);
    mac_un_flush_dover( asic_id, asic_port);
    return 0;
}

/*****************************************************************************
* mac_disable_dover
*
*****************************************************************************/
int mac_disable_dover( int asic_id, int asic_port)
{

    DIAG_PRINT(DIAG_PRINT_CONFIG,
               "  PRT%d HIF%d> Disable \n", asic_id, asic_port);
    mac_flush_dover( asic_id, asic_port);
    return 0;
}

/*****************************************************************************
* mac_flush_dover
*
*****************************************************************************/
int mac_flush_dover( int asic_id, int asic_port)
{
    unsigned int data;

    DIAG_PRINT(DIAG_PRINT_CONFIG,
               "  PRT%d HIF%d> Flush [Dover] \n", asic_id, asic_port);

    /* Enable tx flush */
    mac_wr( asic_id, HI_REG_ADDR(GBE_CFG_TX_FLUSH_EN, asic_port), 1 );

    /* enable Rx flush */
    mac_rd( asic_id, HI_REG_ADDR(GBE_CFG_RX, asic_port), &data);
    data |= (1<<PRT_HI_GBE_CFG_RX_FLD_FLUSH_EN);
    mac_wr( asic_id, HI_REG_ADDR(GBE_CFG_RX, asic_port), data );
    /* hw ops ... */
    return 0;
}

/*****************************************************************************
* mac_un_flush_dover
*
*****************************************************************************/
int mac_un_flush_dover( int asic_id, int asic_port)
{
    unsigned int data;

    printf("  PRT%d HIF%d Un-Flush [Dover]\n",
              asic_id, asic_port);


    /* disable Rx flush */
    mac_rd( asic_id, HI_REG_ADDR(GBE_CFG_RX, asic_port), &data);
    data &= ~((1<<PRT_HI_GBE_CFG_RX_FLD_FLUSH_EN));
    mac_wr( asic_id, HI_REG_ADDR(GBE_CFG_RX, asic_port), data );

    /* Disable tx flush */
    mac_wr( asic_id, HI_REG_ADDR(GBE_CFG_TX_FLUSH_EN, asic_port), 0 );
    return 0;
}

/*****************************************************************************
* mac_set_rx_link_pause_dover
*
*****************************************************************************/
int mac_set_rx_link_pause_dover( int asic_id, int asic_port, int st )
{
    unsigned int data;

    printf("  PRT%d HIF%d 802.3 Link-Pause Rx [Dover]: %s\n",
              asic_id, asic_port, st ? "Enbl":"Dsbl");

    /* hw ops ... */
    if (st) { /* enable rx link-Pause */
        mac_rd(asic_id, HI_REG_ADDR(GBE_CFG_PAUSE, asic_port), &data);
        data &= ~(0xff << 10);
        data |= 0x80200;
        mac_wr(asic_id, HI_REG_ADDR(GBE_CFG_PAUSE, asic_port), data);
    }
    else {
        mac_rd(asic_id, HI_REG_ADDR(GBE_CFG_PAUSE, asic_port), &data);
        data &= ~(1 << 9);
        mac_wr(asic_id, HI_REG_ADDR(GBE_CFG_PAUSE, asic_port), data);
    }

    mac_rd(asic_id, HI_REG_ADDR(GBE_CFG_RMA, asic_port), &data);
    data &= ~(1 << 2); // disable MAC control frame receive
    mac_wr(asic_id, HI_REG_ADDR(GBE_CFG_RMA, asic_port), data );

    mac_wr( asic_id, HI_REG_ADDR(GBE_CFG_PAUSE_CFC_OPCODE, asic_port), 1);
    
    return 0;
}

/*****************************************************************************
* mac_set_tx_link_pause_dover
*
*****************************************************************************/
int mac_set_tx_link_pause_dover( int asic_id, int asic_port, int st )
{
    unsigned int data;

    printf("  PRT%d HIF%d 802.3 Link-Pause Tx [Dover]: %s\n",
              asic_id, asic_port, st ? "Enbl":"Dsbl");

    /* hw ops ... */
    if (st) { /* enable tx link-Pause */
        mac_rd(asic_id, HI_REG_ADDR(GBE_CFG_PAUSE, asic_port), &data);
        data &= ~(0xff << 1);
        data |= 0x80001;
        mac_wr(asic_id, HI_REG_ADDR(GBE_CFG_PAUSE, asic_port), data);
    }
    else {
        mac_rd(asic_id, HI_REG_ADDR(GBE_CFG_PAUSE, asic_port), &data);
        data &= ~(1 << 0);
        mac_wr(asic_id, HI_REG_ADDR(GBE_CFG_PAUSE, asic_port), data);
    }

    mac_rd(asic_id, HI_REG_ADDR(GBE_CFG_RMA, asic_port), &data);
    data &= ~(1 << 2); // disable MAC control frame receive
    mac_wr(asic_id, HI_REG_ADDR(GBE_CFG_RMA, asic_port), data );

    mac_wr( asic_id, HI_REG_ADDR(GBE_CFG_PAUSE_CFC_OPCODE, asic_port), 1);

    return 0;
}

/*****************************************************************************
* mac_set_speed_dover
*
* based on verif code:
*
* int prt_hw_sgmii_change_speed(const unsigned int which_hi, const unsigned int megabits)
*
*****************************************************************************/
int mac_set_speed_dover( int asic_id, int asic_port, int speed )
{
    unsigned int gbe_cfg_rx; // remember old state
    unsigned int gbe_cfg; // remember old state
    unsigned int flush; // Store temporary read values 

    DIAG_PRINT(DIAG_PRINT_CONFIG, "  PRT%d HIF%d Set Speed [Dover]: %s <%d>\n",
              asic_id, asic_port,
              (speed == 10    ? "10Mb"  :
               speed == 100   ? "100Mb" :
               speed == 1000  ? "1Gb"   :
               speed == 10000 ? "10Gb"  : "Invalid"),
              speed );

    /* hw ops ... */

    // set MAC in RX FLUSH mode
    mac_rd(asic_id, HI_REG_ADDR(GBE_CFG_RX, asic_port), &gbe_cfg_rx);
    mac_wr(asic_id, HI_REG_ADDR(GBE_CFG_RX, asic_port), gbe_cfg_rx | (1<<PRT_HI_GBE_CFG_RX_FLD_FLUSH_EN));

    // set MAC in TX FLUSH mode
    mac_wr(asic_id, HI_REG_ADDR(GBE_CFG_TX_FLUSH_EN, asic_port), 1);

    // wait for a jumbo frame time at current speeds. worst case is 10 mb/s. A jumbo frame takes 100ns/bit * 10000bytes * 8bits/byte = 8ms
    usleep(8000);

    // software reset mac
    mac_rd(asic_id, HI_REG_ADDR(GBE_CFG, asic_port), &gbe_cfg);
    mac_wr(asic_id, HI_REG_ADDR(GBE_CFG, asic_port), gbe_cfg | (1<<PRT_HI_GBE_CFG_FLD_RX_SW_RST) | (1<<PRT_HI_GBE_CFG_FLD_TX_SW_RST));

    // reset pcs
    mac_wr(asic_id, HI_REG_ADDR(CFG_PCS_RESET, asic_port),  ~0);

    // update pcs speed
    switch (speed) {
        case 10  : mac_wr(asic_id, HI_REG_ADDR(CFG_PCS_LINK, asic_port), (2<<2) | 0); break;
        case 100 : mac_wr(asic_id, HI_REG_ADDR(CFG_PCS_LINK, asic_port), (2<<2) | 1); break;
        case 1000: mac_wr(asic_id, HI_REG_ADDR(CFG_PCS_LINK, asic_port), (2<<2) | 2); break;
    }
    /* The tx align procedure per Yu Fang's guideline
     *
     * By default, I think the firmware left 'cfg_pcs_tx_mac_align/en'
     * set all the time. This is not the intended way to use.
     * The correct way should be:
     * assert 'cfg_pcs_reset'
     * set 'cfg_pcs_link/speed' to desired speed
     * optionally set 'cfg_pcs_tx_mac_align/en' to 1 with desired value for
     *'cfg_pcs_tx_mac_align/phase'
     * de-assert 'cfg_pcs_reset'
     * set 'cfg_pcs_tx_mac_align/en' to 0
     * enable traffic
     */
    // assert the TX align.
    mac_wr( asic_id, HI_REG_ADDR(CFG_PCS_TX_MAC_ALIGN, asic_port), 1 );

    // make sure reset has been released before proceeding (read to flush)
    mac_rd(asic_id, HI_REG_ADDR(CFG_PCS_RESET, asic_port), &flush);
    usleep(100);

    // unreset PCS
    mac_wr(asic_id, HI_REG_ADDR(CFG_PCS_RESET, asic_port), 0);

    // unreset MAC
    mac_wr(asic_id, HI_REG_ADDR(GBE_CFG, asic_port), gbe_cfg );

    // unflush MAC tx
    mac_wr(asic_id, HI_REG_ADDR(GBE_CFG_TX_FLUSH_EN, asic_port), 0);

    // unflush MAC rx
    mac_wr(asic_id, HI_REG_ADDR(GBE_CFG_RX, asic_port), gbe_cfg_rx);

    return 0;
}

/*****************************************************************************
* mac_disable_interrupts_dover
*
*****************************************************************************/
int mac_disable_interrupts_dover( int asic_id, int asic_port, int na )
{
    printf(" PRT%d HIF%d> Disable Interrupts [Dover]\n",
              asic_id, asic_port);

    /* hw ops ... */
    mac_wr(asic_id, HI_REG_ADDR(INT_ERR_MSK, asic_port), 0 );
    mac_wr(asic_id, HI_REG_ADDR(INT_NORM_MSK, asic_port), 0 );
    mac_wr(asic_id, HI_REG_ADDR(INT_SERDES_MSK, asic_port), 0 );
    mac_wr(asic_id, HI_REG_ADDR(INT_PCS_RX_MSK, asic_port), 0 );
    mac_wr(asic_id, HI_REG_ADDR(GBE_INT_MSK, asic_port), 0 );
    return 0;
}

/*****************************************************************************
* mac_enable_interrupts_dover
*
*****************************************************************************/
int mac_enable_interrupts_dover( int asic_id, int asic_port, int na )
{

    printf("  PRT%d HIF%d Enable Interrupts [Dover]\n",
              asic_id, asic_port);

    /* hw ops ... */

    mac_wr(asic_id, HI_REG_ADDR(INT_ERR_MSK, asic_port), ~0 );
    mac_wr(asic_id, HI_REG_ADDR(INT_NORM_MSK, asic_port), ~0 );
    mac_wr(asic_id, HI_REG_ADDR(INT_SERDES_MSK, asic_port), ~0 );
    mac_wr(asic_id, HI_REG_ADDR(INT_PCS_RX_MSK, asic_port), ~0 );
    mac_wr(asic_id, HI_REG_ADDR(GBE_INT_MSK, asic_port), ~0 );
    return 0;
}

/*****************************************************************************
* mac_clear_interrupts_dover
*
*****************************************************************************/
int mac_clear_interrupts_dover( int asic_id, int asic_port, int na )
{

    printf("  PRT%d HIF%d Clear Interrupts [Dover]\n",
              asic_id, asic_port);

    /* hw ops ... */
    mac_wr(asic_id, HI_REG_ADDR(INT_ERR_MSK, asic_port), ~0 );
    mac_wr(asic_id, HI_REG_ADDR(INT_NORM_MSK, asic_port), ~0 );
    mac_wr(asic_id, HI_REG_ADDR(INT_SERDES_MSK, asic_port), ~0 );
    mac_wr(asic_id, HI_REG_ADDR(INT_PCS_RX_MSK, asic_port), ~0 );
    mac_wr(asic_id, HI_REG_ADDR(GBE_INT_MSK, asic_port), ~0 );

    return 0;
}
