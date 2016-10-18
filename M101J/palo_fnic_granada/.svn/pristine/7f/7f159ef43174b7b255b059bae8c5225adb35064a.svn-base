/********************************************************************
 *
 *      File:   portola_serdes_avago_65nm.c
 *      Name:   Juan Munoz
 *
 *      Description:
 *      Avago 65nm Serdes driver
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

#define ASIC_SERDES_LOOPBACK_MODE_NONE  0
#define ASIC_SERDES_LOOPBACK_MODE_FAR   1
#define ASIC_SERDES_LOOPBACK_MODE_NEAR  2
#define ASIC_SERDES_LOOPBACK_MODE_PCS   3

#define SBUS_READ_SUCCESS		4
#define MISC_DHS_SBUS_OFFSET		1
#define PRT_SBUS_SERDES_BROADCAST	124	
#define FEX_SERDES_STS_CHK_RETRIES	500

#define PRT_HI_GBE_CFG_IPG_TRANSMIT_TIME_SHIFT  17
#define PRT_HI_GBE_CFG_RX_MIN_SIZE_SHIFT        14
#define PRT_HI_GBE_CFG_RX_MAX_SIZE_SHIFT        0

#define fex_reg_read   asic_rd
#define fex_reg_write  asic_wr

unsigned int portola_sbus_bcast_adr( int asic_id, int avago_reg );
unsigned int portola_sbus_adr( int asic_id, int asic_port, uint8_t is_hi, int dev, int avago_reg );
int serdes_rdy_wait( int asic_id, int asic_port, uint8_t is_hi);
int serdes_cfg_xfi_polarity_avago_65nm(int asic_id, int asic_port,
                                       uint8_t is_hi, asic_port_info_t *ptr);
int serdes_configure_1g( int asic_id, int asic_port, int reset, uint8_t is_hi );

extern int asic_get_xfi_polarity(uint8_t asic_id, uint8_t asic_port,
                          uint8_t *rx_inverted, uint8_t *tx_inverted,
                          asic_port_info_t *ptr);

/*****************************************************************************
* serdes_enable_avago_65nm
*
*****************************************************************************/
int serdes_enable_avago_65nm(int asic_id, int asic_port, uint8_t is_hi,
                             asic_port_info_t *port_info_ptr)
{
    int	port_idx = asic_port;
    uint32_t data;

    DIAG_PRINT(DIAG_PRINT_CONFIG,
               " PRT%d %s%d Enable Avago 65nm\n",
              asic_id, is_hi?"HIF":"NIF", asic_port);

    /* hw ops ... */
    if (is_hi) {
        return 0;
    }
    

    // Serdes Tx/Rx Enable
    asic_cfg_ni_serdes_ena (asic_id, port_idx, 1, 1);

    // Check Serdes Status
    asic_chk_ni_serdes_status (asic_id, port_idx, 
                                        FEX_SERDES_STS_CHK_RETRIES);
    // Remove  HI / NI PCS reset
    asic_cfg_ni_pcs_reset(asic_id, port_idx, 0, 0, 0);

    // Check PCS Status
    asic_chk_ni_pcs_status (asic_id, port_idx, 
                                     FEX_SERDES_STS_CHK_RETRIES);



    // Set en_short_ipg (work-around for PCS problem)
    // printf("Set en_short_ipg (work-around for PCS problem)\n");
    fex_reg_read(asic_id, portola_sbus_adr( asic_id, asic_port, is_hi, 1/*PCS*/, 0x0 ), &data);
    data |= (1 << 3);
    fex_reg_write(asic_id, portola_sbus_adr( asic_id, asic_port, is_hi, 1/*PCS*/, 0x0 ), data);

    sleep(1);
    // Set polarity (must be done after "ready"
    serdes_cfg_xfi_polarity_avago_65nm( asic_id, asic_port, is_hi, port_info_ptr);

    return 0;
}

/*****************************************************************************
* serdes_disable_avago_65nm
*
*****************************************************************************/
int serdes_disable_avago_65nm( int asic_id, int asic_port, uint8_t is_hi)
{
    int	port_idx = asic_port;

    DIAG_PRINT(DIAG_PRINT_CONFIG,
              "  PRT%d %s%d> Disable Avago 65nm \n",
              asic_id, is_hi? "HIF" : "NIF", asic_port);

    /* hw ops ... */
    if (is_hi) return 0;


        // Put  HI / NI PCS reset
        asic_cfg_ni_pcs_reset(asic_id, port_idx, 1, 1, 0);

        // Serdes Tx/Rx Disable
        asic_cfg_ni_serdes_ena (asic_id, port_idx, 0, 0);
        usleep(10);
    return 0;
}
/***************************************************************************
* portola_sbus_controller_adr
***************************************************************************/
static unsigned int portola_sbus_controller_adr (int asic_id, int avago_reg)
{
    int sbus_dev = ASIC(SBUS_CONTROLLER);
    return( BA(ASIC(MISC_DHS_SBUS)) + ((((sbus_dev << 8) + avago_reg) << 2) & 0x3FFFc));
}
/*****************************************************************************
* serdes_bring_up_avago_65nm
*
*****************************************************************************/
void portola_speedup_sbus (int asic_id)
{
    unsigned int ref_clk_divider=4, data=0;
    /* 
     * speed up sbus. note that 127 is mapped to 0xFE by default, see prt_misc.v
     * According to Sheri from Avago the SBUS will not run at speeds greater 
     * than 10Mhz so we use a divide by 16
     * sbus_clk = refclk/divider =156.625Mhz/16 =~ 10Mhz
     * value to be written to sbus serdes controller is log2(16)
     */
    fex_reg_read( asic_id, portola_sbus_controller_adr(asic_id, 0xA), &data);
    if ((data&0xFF) != ref_clk_divider) {
        fex_reg_write(asic_id, portola_sbus_controller_adr(asic_id, 0xA), 
                  ref_clk_divider);
        usleep(100);
    }
    return;
}

int serdes_bring_up_avago_65nm_hif( int asic_id, int asic_port, uint32_t speed)
{
    DIAG_PRINT(DIAG_PRINT_CONFIG,
               "    PRT%d HIF%d Bring up Avago 65nm \n", asic_id, asic_port);

    serdes_configure_1g( asic_id, asic_port, 1/*reset*/ , 1);
    return (0);
}

int serdes_check_avago_65nm_hif (int asic_id, int asic_port, 
                                 asic_port_info_t *port_ptr)
{
    int    rc = 0;
    int retries = 5, not_ready=1;

    while (--retries && not_ready) {
         not_ready = serdes_rdy_wait( asic_id, asic_port, 1);
	 if (not_ready) {
    	 	serdes_configure_1g( asic_id, asic_port, 1/*reset*/ , 1);
         	usleep(100000);
	 }
    }
    if (not_ready)
    	printf("    PRT%d HIF%d Serdes Not Ready \n", asic_id, asic_port);

    /* Set polarity now (for HI) */
    serdes_cfg_xfi_polarity_avago_65nm( asic_id, asic_port, 1, port_ptr);
    return (not_ready? not_ready: rc);
}
    
int serdes_bring_up_avago_65nm_nif( int asic_id, int asic_port)
{
    int    rc = 0, port_idx = asic_port;
    DIAG_PRINT(DIAG_PRINT_CONFIG,
               " PRT%d NIF%d Bring up Avago 65nm \n", asic_id, asic_port);

    // Configure the rate according to our port speed
    asic_cfg_ni_serdes_rate(asic_id, port_idx, 3);
    // NIF only 10G and so let us make sure 8b10b align is cleared.
    asic_cfg_ni_8b10b_align_en( asic_id, port_idx, 0 );
    // Put  HI / NI PCS reset
    rc = asic_cfg_ni_pcs_reset(asic_id, port_idx, 1, 1, 0);

    return (rc);
}
 
/*****************************************************************************
* serdes_bring_dn_avago_65nm
*
*****************************************************************************/
int serdes_bring_dn_avago_65nm( int asic_id, int asic_port, uint8_t is_hi)
{
    int         rc        = 0;

    DIAG_PRINT(DIAG_PRINT_CONFIG," PRT%d %s%d Bring dn Avago 65nm \n",
              asic_id, is_hi? "HIF" : "NIF", asic_port);

    rc = serdes_disable_avago_65nm( asic_id, asic_port, is_hi);
    return rc;
}

/*****************************************************************************
* serdes_set_speed_avago_65nm
*
*****************************************************************************/
int serdes_set_speed_avago_65nm( int asic_id, int asic_port, int speed )
{
    printf("%s:%d NOT IMPLEMENTED\n", __func__, __LINE__);
    return 0;
}

/*****************************************************************************
* serdes_get_state_avago_65nm
*
*****************************************************************************/
int serdes_get_state_avago_65nm( int asic_id, int asic_port, uint8_t is_hi)
{
    int rx_status = 0, tx_status = 0, st = 0;
    unsigned int data;

    /* hw ops ... */
    fex_reg_read( asic_id, portola_sbus_adr( asic_id, asic_port, is_hi, 0/*serdes*/, 0x0 ), &data );
    tx_status = (data >> 3) & 1;
    fex_reg_read( asic_id, portola_sbus_adr( asic_id, asic_port, is_hi, 0/*serdes*/, 0xF ), &data );
    rx_status = (data >> 0) & 1;
    st = tx_status && rx_status;  

    return st;
}

/*****************************************************************************
* serdes_loopback_avago_65nm
*
*****************************************************************************/
#define sbus_rx_far_loopback_en   (1 << 4 /*reg=0x9*/)
#define sbus_rx_far_loopback_res  (5 << 1 /*reg=0x9*/)
#define sbus_rx_far_loopback_gate (1 << 0 /*reg=0x9*/)
int serdes_loopback_avago_65nm( int asic_id, int asic_port, uint8_t is_hi, int mode )
{
    uint32_t addr = 0, data = 0;
    int  port_idx = asic_port;

    DIAG_PRINT(DIAG_PRINT_CONFIG, "  PRT%d %s%d> Loopback Avago 65nm %s\n",
              asic_id, is_hi? "HIF" : "NIF", asic_port,
              mode == ASIC_SERDES_LOOPBACK_MODE_NONE ? "None" : 
              mode == ASIC_SERDES_LOOPBACK_MODE_FAR  ? "Far" : 
              mode == ASIC_SERDES_LOOPBACK_MODE_NEAR ? " Near" : 
              mode == ASIC_SERDES_LOOPBACK_MODE_PCS  ? "PCS" : "** Unknown **");

    /* hw ops ... */
    if (!is_hi) {
        addr = NI_REG_ADDR(CFG_XGMII_CNTL, asic_port);

        /* only used for NI loopbacks */
        fex_reg_read( asic_id, addr, &data);

        /*
         * The intent is to only set the loopback requested and remove any other
         * loopbacks if they were configured.
         * Clear all the loopback bits in the config register.
         */
        data &= ~((0x01 << PRT_NI_CFG_XGMII_CNTL_FLD_FAR_LOOPBACK_EN)   |
                  (0x01 << PRT_NI_CFG_XGMII_CNTL_FLD_NEAR_LOOPBACK_EN)  |
                  (0x01 << PRT_NI_CFG_XGMII_CNTL_FLD_PCS_LOOPBACK));            
    }
    switch (mode) {
        case ASIC_SERDES_LOOPBACK_MODE_NONE:
            /* undo far loopback */
            fex_reg_write(asic_id, portola_sbus_adr( asic_id, asic_port, is_hi, 0, 0x9),
                          sbus_rx_far_loopback_res /*0xa*/ );

            /* undo near loopback */
            fex_reg_write(asic_id, portola_sbus_adr( asic_id, asic_port, is_hi, 0, 0xd), 0);

            /* undo NI PCS loopbacks */
            if (!is_hi) {
                fex_reg_write( asic_id, addr, data);
            }
           if (is_hi) {
                /* undo HI sig_loss_mask */
                addr = HI_REG_ADDR(CFG_SERDES_SIG_LOSS_MASK, port_idx);
                fex_reg_write( asic_id, addr, 0);
            }
        break;

        case ASIC_SERDES_LOOPBACK_MODE_FAR:
            fex_reg_write(asic_id, portola_sbus_adr( asic_id, asic_port, is_hi, 0, 0x9),
                          (sbus_rx_far_loopback_en   |
                           sbus_rx_far_loopback_gate |
                           sbus_rx_far_loopback_res) /*0x1b*/ );
           if (is_hi) {
                addr = HI_REG_ADDR(CFG_SERDES_SIG_LOSS_MASK, port_idx);
                fex_reg_write( asic_id, addr, 3);
            }
        break;
            
        case ASIC_SERDES_LOOPBACK_MODE_NEAR:
            fex_reg_write(asic_id, portola_sbus_adr( asic_id, asic_port, is_hi, 0, 0xd), ((1 << 7) | (1 << 0)));
        break;

        case ASIC_SERDES_LOOPBACK_MODE_PCS:
            if (is_hi){
                printf("PCS loopback not supported on HI ports\n");
            }
            else if (!is_hi) {
                data |= (0x01 << PRT_NI_CFG_XGMII_CNTL_FLD_PCS_LOOPBACK);
                fex_reg_write( asic_id, addr, data);
            }
        break;

        default:
        break;
    }

    return 0;
}

/*****************************************************************************
* serdes_prbs_avago_65nm
*
*****************************************************************************/
static int prt_hw_serdes_prbs_tx_configure (int asic_id, int asic_port)
{
    unsigned int reg_data;
    /* sbus_tx_data_gate, initial set tx_pattern_gen_sel_cntl & 
     * tx_data_sel_cntl to 0
     */
    reg_data = (1<<0); 
    fex_reg_write(asic_id, 
                  portola_sbus_adr(asic_id, asic_port, 0x1, 0, 0xa), 
                  reg_data);
    /*
     * Enable tx_pattern gen
     */
    reg_data = (1<<7) | (1<<0);
    fex_reg_write(asic_id, 
                  portola_sbus_adr(asic_id, asic_port, 0x1, 0, 0xa), 
                  reg_data);
    return 0;
}

static int prt_hw_serdes_prbs_rx_configure (int asic_id, int asic_port)
{
    unsigned int reg_data;
    /* sbus_tx_data_gate, initial set tx_pattern_gen_sel_cntl & 
     * tx_data_sel_cntl to 0
     */
    reg_data = (1<<0); 
    fex_reg_write(asic_id, 
                  portola_sbus_adr(asic_id, asic_port, 0x1, 0, 0x3), 
                  reg_data);
    /*
     * initial set the rx_pattern_cmp_sel & rx_sel_cntl to 0n gen
     */
    reg_data = (0<<0); 
    fex_reg_write(asic_id, 
                  portola_sbus_adr(asic_id, asic_port, 0x1, 0, 0x7), 
                  reg_data);
    /* rx_patter_cmp_en */
    reg_data = (1<<7); 
    fex_reg_write(asic_id, 
                  portola_sbus_adr(asic_id, asic_port, 0x1, 0, 0x7), 
                  reg_data);
    return 0;
}

/*
 * prbs status check
 */
static int prt_hw_serdes_prbs_sta_check (int asic_id, int asic_port)
{
    unsigned int reg_data;

    // check if errors occurred
    fex_reg_read(asic_id, 
                 portola_sbus_adr(asic_id, asic_port, 0x1, 0, 0x1), 
                 &reg_data);
    if ((reg_data&0x3)!=0) {
         printf("SBUS RX PRBS failure occured on node %d\n ", asic_port);
         return 1;
    }
    return 0;
}

static int prt_hw_serdes_prbs_err_reset (int asic_id, int asic_port)
{
    unsigned int reg_data;

    // clear sticky bits
    // sbus_rx_error_monitor_en_cntl: enable error monitoring
    // sbus_rx_error_monitor_reset_cntl: reset error sticky bits    
    // sbus_rx_error_monitor_gate: override pins, use PIOs
    reg_data = (1<<7)  | (1<<6)  | (1<<0);  
    fex_reg_write(asic_id, 
                  portola_sbus_adr(asic_id, asic_port, 0x1, 0, 0x4), 
                  reg_data);

    // remove reset from sticky bits
    // sbus_rx_error_monitor_en_cntl: enable error monitoring
    // sbus_rx_error_monitor_reset_cntl: reset error sticky bits
    // sbus_rx_error_monitor_gate: override pins, use PIOs
    reg_data = (1<<7)  | (0<<6)  | (1<<0);  
    fex_reg_write(asic_id, 
                  portola_sbus_adr(asic_id, asic_port, 0x1, 0, 0x4), 
                  reg_data);
    return 0;

}

int prt_serdes_prbs_test (int asic_id, uint64_t himask, const int wait_time,
                          uint64_t *hifail)
{
    uint32_t hi; 
    int rc = 0;
    uint64_t hi_mask = himask;
    /* SERDES bringup */
    for (hi = 0; !rc && (hi < ASIC(HI_COUNT)); hi++, hi_mask >>=1) {
        if (!(hi_mask & 0x01)) continue;
        rc = serdes_bring_up_avago_65nm_hif(asic_id, hi, 1);
    }
    /* serdes internal loopback */
    hi_mask = himask;
    for (hi = 0; !rc && (hi < ASIC(HI_COUNT)); hi++, hi_mask >>=1) {
        if (!(hi_mask & 0x01)) continue;
        rc = asic_hi_serdes_lpbk(asic_id, hi, 0x1);
    }
    /* Run PRBS test */
    hi_mask = himask;
    for (hi = 0; !rc && (hi < ASIC(HI_COUNT)); hi++, hi_mask >>=1) {
        if (!(hi_mask & 0x01)) continue;
        rc = prt_hw_serdes_prbs_tx_configure(asic_id, hi);
        rc = prt_hw_serdes_prbs_rx_configure(asic_id, hi);
        rc = prt_hw_serdes_prbs_err_reset(asic_id, hi);
    }
    sleep(wait_time);
    hi_mask = himask;
    for (hi = 0; !rc && (hi < ASIC(HI_COUNT)); hi++, hi_mask >>=1) {
        if (!(hi_mask & 0x01)) continue;
        rc = prt_hw_serdes_prbs_sta_check(asic_id, hi);
        if (rc) {
            *hifail |= (0x1 << hi);
            return rc;
        }
    }
    return rc;
}


/***************************************************************************
* portola_sbus_adr
***************************************************************************/
unsigned int portola_sbus_adr( int asic_id, int port, uint8_t is_hi, int dev, int avago_reg )
{
    int sbus_dev;

    if (is_hi) {
        sbus_dev = port + dev + 1;
    }
    else  {
        sbus_dev = 48 + (3*port) + dev + 1;
    }
    return( BA(ASIC(MISC_DHS_SBUS)) + ((((sbus_dev << 8) + avago_reg) << 2) & 0x3FFFc) );
}

/***************************************************************************
* portola_sbus_bcast_adr
***************************************************************************/
unsigned int portola_sbus_bcast_adr( int asic_id, int avago_reg )
{
    int sbus_dev = PRT_SBUS_SERDES_BROADCAST;
    return( BA(ASIC(MISC_DHS_SBUS)) + ((((sbus_dev << 8) + avago_reg) << 2) & 0x3FFFc));
}


/***************************************************************************
* portola_serdes_set_tx_polarity
***************************************************************************/
int portola_serdes_set_tx_polarity( int asic_id, int asic_port, uint8_t is_hi, int invert )
{
//    printf("  %s%d Set Tx Polarity %s\n", is_hi? "HIF": "NIF", 
//		asic_port, invert ? "inverted":"normal");
    fex_reg_write(asic_id, portola_sbus_adr( asic_id, asic_port, is_hi, 0/*serdes*/, 0xA ), 3);
    fex_reg_write(asic_id, portola_sbus_adr( asic_id, asic_port, is_hi, 0/*serdes*/, 0xB ), (invert<<2));
    return 0;
}

/***************************************************************************
* portola_serdes_set_rx_polarity
***************************************************************************/
int portola_serdes_set_rx_polarity( int asic_id, int asic_port, uint8_t is_hi, int invert )
{
//    printf("  %s%d Set Rx Polarity %s\n", is_hi? "HIF": "NIF", 
//		asic_port, invert ? "inverted":"normal");
    fex_reg_write(asic_id, portola_sbus_adr( asic_id, asic_port, is_hi, 0/*serdes*/, 0x7 ), (invert<<4));
    fex_reg_write(asic_id, portola_sbus_adr( asic_id, asic_port, is_hi, 0/*serdes*/, 0x9 ), (5<<1));
    fex_reg_write(asic_id, portola_sbus_adr( asic_id, asic_port, is_hi, 0/*serdes*/, 0x3 ), (1<<0));
    return 0;
}

/*****************************************************************************
* serdes_rdy_wait
*
* Utility function to check for readiness on 1.4 serdes lanes
* starting with first_lane.
*
* Readiness is defined as follows:
*   0xF.rx_rdy_obs = 1
*   0x0.tx_rdy_obs = 1
*   0xD[6:0] != 0
*
* Note: Register 0xD is undocumented but bits 6:0 contain a Tx PLL
*       calibration value. If this value is 0 it indicates the
*       calibration either was not done or failed. Bad in either case.
*
*****************************************************************************/
int serdes_rdy_wait( int asic_id, int asic_port, uint8_t is_hi)
{
#define sbus_rx_rdy_obs            (1 << 0 /*avago reg 0xf*/)
#define sbus_tx_rdy_obs            (1 << 3 /*avago reg 0*/)
#define sbus_valid_calibration_val (0x07 /* 2:0 must be non-zero*/)

    unsigned int data;
    int tries = 100;

    /* STEP 1: wait for rx_rdy_obs */
    do {
        fex_reg_read(asic_id, portola_sbus_adr( asic_id, asic_port, is_hi, 0, 0xf ), &data);

    } while (--tries && !(data & sbus_rx_rdy_obs) && ((data >> 8) == SBUS_READ_SUCCESS) );
    /* ... check for SBus error first */
    if ((data >> 8) != SBUS_READ_SUCCESS) {
        printf( "  PRT%d Port%d SBus read error <%xh>\n", 
                       asic_id, asic_port, data);
        return 1;
    }
    /* ... then see if rx ready set */
    if (!(data & sbus_rx_rdy_obs)) {
        printf( "  PRT%d Port%d: sbus_rx_rdy_obs not set <%xh>\n", 
                       asic_id, asic_port, data);
        return 2;
    }
    /* STEP 2: Once rx_rdy_obs is set tx_rdy_obs ought to be set. Verify it. */
    tries++; /* make sure it doesn't go negative */
    do {
        fex_reg_read(asic_id, portola_sbus_adr( asic_id, asic_port, is_hi, 0, 0x0 ), &data);

    } while (--tries && !(data & sbus_tx_rdy_obs) && ((data >> 8) == SBUS_READ_SUCCESS) );
    /* ... check for SBus error first */
    if ((data >> 8) != SBUS_READ_SUCCESS) {
        printf( "  PRT%d Port%d: SBus read error <%xh>\n", 
                       asic_id, asic_port, data);
        return 3;
    }
    /* ... then see if data ready set */
    if (!(data & sbus_tx_rdy_obs)) {
        printf( "  PRT%d Port%d: Warning: sbus_tx_rdy_obs not set <%xh>\n", 
                       asic_id, asic_port, data);
        return 4;
    }
    /*  STEP 3: Also, check the Tx PLL calibration value and be sure it is not 0. */
    tries++; /* make sure it doesn't go negative */
    do {
        fex_reg_read(asic_id, portola_sbus_adr( asic_id, asic_port, is_hi, 0, 0x10 ), &data);

    } while (--tries && ((data >> 8) == SBUS_READ_SUCCESS) );

    /* ... check for SBus error first */
    if ((data >> 8) != SBUS_READ_SUCCESS) {
        printf( "  PRT%d Port%d: SBus read error <%xh>\n", 
                       asic_id, asic_port, data);
        return 5;
    }
    /* ... then see if data ready set */
    if ((data & sbus_valid_calibration_val)) {
        printf( "  PRT%d Port%d: Warning: serdes not ready (tx PLL calibration[6:0]=0) <%xh>\n", 
                       asic_id, asic_port, data);
        return 6;
    }
    DIAG_PRINT(DIAG_PRINT_CONFIG,
              "    %s%d Serdes ready (data=0x%X)\n", is_hi? "HIF" : "NIF", 
		asic_port, data);
    return 0;
}

/***************************************************************************
* serdes_cfg_xfi_polarity_avago_65nm
***************************************************************************/
int serdes_cfg_xfi_polarity_avago_65nm( int asic_id, int asic_port, uint8_t is_hif,
                                       asic_port_info_t *port_info_ptr)
{
    uint8_t   err=0, rx_inverted=0, tx_inverted=0;

    err = asic_get_xfi_polarity(asic_id, asic_port, &rx_inverted, &tx_inverted,
                                port_info_ptr);
    if (err) {
        printf(" PRT%d Port%d: XFI polarity cannot be determined <err=%x>\n",
                      asic_id, asic_port, err);
        return -1;
    }

    DIAG_PRINT(DIAG_PRINT_CONFIG,
              "  PRT%d %s%d> Configure XFI polarity TxInv=%d RxInv=%d\n",
              asic_id, is_hif? "HIF" : "NIF", asic_port, tx_inverted, rx_inverted);
    portola_serdes_set_tx_polarity( asic_id, asic_port, is_hif, tx_inverted ? 1:0 );
    portola_serdes_set_rx_polarity( asic_id, asic_port, is_hif, rx_inverted ? 1:0 );
    return 0;
}

static int serdes_straighten_xfi_polarity_avago_65nm (int asic_id, 
                                                      int asic_port, 
                                                      uint8_t is_hif)
{
    portola_serdes_set_tx_polarity(asic_id, asic_port, is_hif, 0);
    portola_serdes_set_rx_polarity(asic_id, asic_port, is_hif, 0);
    return 0;
}

/*****************************************************************************
* serdes_configure_1g
*****************************************************************************/
int serdes_configure_1g( int asic_id, int asic_port, int reset, uint8_t is_hi )
{
    if (reset) {
        fex_reg_write(asic_id, portola_sbus_adr( asic_id, asic_port, is_hi, 0, 0x00 ), (1<<31)); //reset
        usleep( 100000 );
    }

    fex_reg_write(asic_id, portola_sbus_adr( asic_id, asic_port, is_hi, 0, 0x00 ),  1 | (3<<1)); // sbus_sbus_clk_gate, ref_sel
    fex_reg_write(asic_id, portola_sbus_adr( asic_id, asic_port, is_hi, 0, 0x22 ), 0x18);
    fex_reg_write(asic_id, portola_sbus_adr( asic_id, asic_port, is_hi, 0, 0x1D ), 0x03);
    fex_reg_write(asic_id, portola_sbus_adr( asic_id, asic_port, is_hi, 0, 0x36 ),  0x66); // tx_rate_sel
    fex_reg_write(asic_id, portola_sbus_adr( asic_id, asic_port, is_hi, 0, 0x3b ),  0x66); // rx_rate_sel

    // reserved bits writes described in [2]
    // printf("reserved bits writes described in [2]\n");
    fex_reg_write(asic_id, portola_sbus_adr( asic_id, asic_port, is_hi, 0, 0x01 ),  0xa);
    fex_reg_write(asic_id, portola_sbus_adr( asic_id, asic_port, is_hi, 0, 0x02 ),  0xf);
    fex_reg_write(asic_id, portola_sbus_adr( asic_id, asic_port, is_hi, 0, 0x13 ),  0x6);
    fex_reg_write(asic_id, portola_sbus_adr( asic_id, asic_port, is_hi, 0, 0x12 ),  0x23);
    fex_reg_write(asic_id, portola_sbus_adr( asic_id, asic_port, is_hi, 0, 0x35 ),  0x7);
    fex_reg_write(asic_id, portola_sbus_adr( asic_id, asic_port, is_hi, 0, 0x3a ),  0x7);

    // normal ref_sel, rate_sel programming
    // printf("normal ref_sel, rate_sel programming\n");
    fex_reg_write(asic_id, portola_sbus_adr( asic_id, asic_port, is_hi, 0, 0x22 ),  3 | 0x18); // tx_en and rx_en assertion, reserved field=3
    return 0;
}

int asic_hi_serdes_lpbk (uint8_t inst, uint8_t port, uint8_t lpbk_en)
{
    if (lpbk_en) {
        serdes_straighten_xfi_polarity_avago_65nm(inst, port, 0x1); 
    }
    return (serdes_loopback_avago_65nm(inst, port, 1, 
			lpbk_en? ASIC_SERDES_LOOPBACK_MODE_NEAR : ASIC_SERDES_LOOPBACK_MODE_NONE));
}

int asic_ni_serdes_lpbk (uint8_t inst, uint8_t port, uint8_t lpbk_en)
{
    if (lpbk_en) {
        serdes_straighten_xfi_polarity_avago_65nm(inst, port, 0); 
    }
    return (serdes_loopback_avago_65nm(inst, port, 0, 
			lpbk_en? ASIC_SERDES_LOOPBACK_MODE_NEAR : ASIC_SERDES_LOOPBACK_MODE_NONE));
}

int asic_hi_serdes_lpbk_with_rst (uint8_t inst, uint8_t port, uint8_t lpbk_en)
{
    int rc = 0;
    uint32_t data;
    uint8_t  tx_data, rx_data;

    rc = serdes_configure_1g(inst, port,  1, 1 );
    if (rc) return (rc);

    if (!lpbk_en) return(rc);

    // Set loopback
    rc = (serdes_loopback_avago_65nm(inst, port, 1,
                        lpbk_en? ASIC_SERDES_LOOPBACK_MODE_NEAR : ASIC_SERDES_LOOPBACK_MODE_NONE));
    if (rc) return (rc);

    usleep(5000);
    // Check Ready 
    fex_reg_read(inst, portola_sbus_adr( inst, port, 1, 0/*PCS*/, 0x0 ), &data);
    tx_data = ((data >> 3) & 1) ? 1 : 0;
    fex_reg_read(inst, portola_sbus_adr( inst, port, 1, 0/*PCS*/, 0xF ), &data);
    rx_data = ((data >> 0) & 1) ? 1 : 0;

    if (!(tx_data & rx_data)) {
        printf("  Error: Asic%d Port%d TxRdy=%d RxRdy=%d\n", inst, port, tx_data, rx_data);
        return (-1);
    }
    return (rc);
}

