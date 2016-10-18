/********************************************************************
*
*      File:   redwood_mac_hw_seq.c
*      Name:   Bruce McLoughlin
*
*      Description: Stripped down MAC programming sequences
*      that can be shared between USD and Diags
*
*      Note: You'll need to define the following for your
*      platform:
*
*      reg_read( asic-id, register-addr, *data )
*      reg_write(asic-id, register-addr,  data )
*
*       CAUTION:
*           The "..._hw_seq.c" files use red_red_naxislave_define.h
*           while the rest of the USD code uses red_red_naxislave.h.
*           The two files are just different sets of #defines
*           generated from the rtdb file. The "_define" file uses
*           "Word addresses" whereas the other uses "byte addresses".
*           So, all the addresses used here need to be left-shifted
*           by two bits to generate the corresponding byte-
*           address for presentation on the NAXI bus.
*
*      Copyright (c) 2001,2002,2003,2004 by Nuova Systems, Inc.
********************************************************************/
#include <stdio.h>
#include <unistd.h>
#include "red_red_naxislave_define.h"
#include "redwood_mac_hw_seq.h"
#include "redwood_serdes_hw_seq.h"

#define DO_STEP(x, y)	1

unsigned int ss_wr_port_to_set[] = {
    RED_SS2_CFG_WR_PORT0,
    RED_SS2_CFG_WR_PORT1,
    RED_SS2_CFG_WR_PORT2,
    RED_SS2_CFG_WR_PORT3,
    RED_SS2_CFG_WR_PORT0,
    RED_SS2_CFG_WR_PORT1,
    RED_SS2_CFG_WR_PORT2,
    RED_SS2_CFG_WR_PORT3
};


extern int mac_disable_hif_1g_hw_seq( int asic_id, int mac_id /*0..7*/ );

typedef enum satcfg_port_type_e
{
    SATCFG_PORT_TYPE_CIF = 0,           // cpu interface.
    SATCFG_PORT_TYPE_BIF,               // bmc interface.
    SATCFG_PORT_TYPE_NIF,               // network interface.
    SATCFG_PORT_TYPE_HIF,               // host interface.
    SATCFG_PORT_TYPE_MAX
} satcfg_port_type_t;


/*************** IO customization ***********************/
#ifdef USD
#include <isan/types.h>
#include "redwood_intf.h"
#include "redwood_access.h"
int mac_write( unsigned int asic, unsigned int reg, unsigned int data );
int mac_read( unsigned int asic, unsigned int reg, unsigned int *data );
#define BA(x) ((x) << 2)
#else
// your reg_read/write definitions here
#include "red_hw_lib.h"
#endif
/*************** End of IO customization ****************/

#include "redwood_mac_hw_qos_defaults.h"

// from red_red_naxislave.h
#define RED_HI_BLKSZ 0x00008000
#define MAC_OFFSET( port ) (port * RED_HI_BLKSZ )
#define SBUS_OFS(x) ((x) << 2)
/* The _define.h file gives bit positions, not "masks" for the fields
* in a register, so we need to construct the mask value
*/
#define BIT(x) (1 << (x))

// table 53, reg 0xf
#define sbus_rx_rdy_obs 1

// table 52, reg 0x22
#define sbus_rx_en_cntl 1
#define sbus_tx_en_cntl 2
#define sbus_rx_rate_sel_shift 3


static unsigned int misc_version; // used just for flushing PIOs
#define mac_flush_pio( asic ) mac_read( asic, BA(RED_MISC_MISC_VERSION), &misc_version )

extern int port_is_hif( int asic_port );

/***************************************************************
* mac_config
***************************************************************/
int mac_config( int asic_id,
                mac_cfg_t *p_cfg, int ofs /*of MAC regs from MAC0 */,
                unsigned int mac_code_pause_sub /*reg*/,
                unsigned int mac_frm_len_max /*reg*/,
                unsigned int mac_ext_lpbak /*reg*/,
                unsigned int mac_pkt_lpbak /*reg*/,
                unsigned int mac_cfg /*reg*/,
                unsigned int mac_cfg_ext /*reg*/ )
{
    unsigned int data, mac_cfg_data, mac_cfg_ext_data;

    if (!p_cfg)          return 0; // ok to pass
    if (!p_cfg->modbits) return 0; // probably a bug, but for now..

    /* pre-read the mac_cfg regs we may need to write */
    mac_read( asic_id, mac_cfg     + ofs, &mac_cfg_data );
    mac_read( asic_id, mac_cfg_ext + ofs, &mac_cfg_ext_data );

    if (p_cfg->modbits & mod_mtu) {
        data = bits(14, 0, p_cfg->mtu );
        mac_write( asic_id, mac_frm_len_max + ofs, data);
    }
    if (p_cfg->modbits & mod_pause_sub_code) {
        data = bits(7, 0, p_cfg->pause_sub_code );
        mac_write( asic_id, mac_code_pause_sub + ofs, p_cfg->pause_sub_code ); 
    }
    if (p_cfg->modbits & mod_rx_lane_map) {
        mac_cfg_data &= ~( bits(31, 25, -1) );
        mac_cfg_data |=    bits(31, 25, p_cfg->rx_lane_map);
        /* the upper bit of the lane map is in the next location.
        *  It defaults to "1" (for straight-thru mapping).
        * So, if lane 3 maps to either lane 0 or one, the upper
        * bit needs changing.
        */
       mac_write( asic_id, mac_cfg + 4 + ofs, p_cfg->rx_lane_map >> 7 ); 

    }
    if (p_cfg->modbits & mod_tx_lane_map) {
        mac_cfg_ext_data &= ~( bits(7, 0, -1) );
        mac_cfg_ext_data |=    bits(7, 0, p_cfg->tx_lane_map);
    }
    if (p_cfg->modbits & mod_ext_loopback) {
        data = bits(3, 0, p_cfg->ext_loopback);
        mac_write( asic_id, mac_ext_lpbak + ofs, data );
    }
    if (p_cfg->modbits & mod_pkt_loopback) {
        data = bits(0, 0, p_cfg->pkt_loopback);
        mac_write( asic_id, mac_pkt_lpbak + ofs, data );
    }
    if (p_cfg->modbits & mod_ipg) {
        mac_cfg_data &= ~( bits(6, 0, -1) );
        mac_cfg_data |=    bits(6, 0, p_cfg->ipg);
    }
    if (p_cfg->modbits & mod_pad_en) {
        mac_cfg_data &= ~( bits(11, 11, -1) );
        mac_cfg_data |=    bits(11, 11, p_cfg->pad_en);
    }
    if (p_cfg->modbits & mod_cmd_frm_en) {
        mac_cfg_data &= ~( bits(16, 16, -1) );
        mac_cfg_data |=    bits(16, 16, p_cfg->cmd_frm_en);
    }
    if (p_cfg->modbits & mod_rx_scramble_en) {
        mac_cfg_data &= ~( bits(23, 23, -1) );
        mac_cfg_data |=    bits(23, 23, p_cfg->rx_scramble_en);
    }
    if (p_cfg->modbits & mod_tx_scramble_en) {
        mac_cfg_data &= ~( bits(21, 21, -1) );
        mac_cfg_data |=    bits(21, 21, p_cfg->tx_scramble_en);
    }

    mac_write( asic_id, mac_cfg + ofs, mac_cfg_data );
    mac_write( asic_id, mac_cfg_ext + ofs, mac_cfg_ext_data );

    return 0;
}


/***************************************************************
* mac_bring_up_bif_hw_seq
*
* Bring up the BIF MAC/PHY
*
* Note: This sequence is translated from RTL in:
*
* asic/src/redwood/verif/top/tests/SCE_Config.h:cfgMAC4MII
*
* mode,  1000 (only)
*
* returns: 0 == OK
*          else, error code
**************************************************************/
int mac_bring_up_bif_hw_seq( int asic_id, int mode )
{
    unsigned int nspi_timer, pause_fc_limit;
    boolean fcoe;
    
    if (mode == 1000) {
        nspi_timer     = 0x20*4;
        pause_fc_limit = 0x100;
    }
    else if (mode == 100) {
        nspi_timer     = 0x20*4*10;
        pause_fc_limit = 0x200;
    }
    else {
        nspi_timer     = 0x20*4*100;
        pause_fc_limit = 0x200;
    }

    mac_write( asic_id, BA(RED_BI0_CFG_EG_NSPI_TIMER),      nspi_timer );
    mac_write( asic_id, BA(RED_BI0_GBE_CFG_PAUSE_FC_LIMIT), pause_fc_limit );

    mac_write( asic_id, BA(RED_BI0_GBE_CFG_MII_MDC_PERIOD), 60 );

    mac_write( asic_id, BA(RED_BI0_GBE_CFG),
               (bits(RED_BI_GBE_CFG_FLD_RX_LINK_RST,  RED_BI_GBE_CFG_FLD_RX_LINK_RST,   1) |
                bits(RED_BI_GBE_CFG_FLD_RX_CRC_CHK_EN,RED_BI_GBE_CFG_FLD_RX_CRC_CHK_EN, 1) |
                bits(RED_BI_GBE_CFG_FLD_PAUSE_TX_EN,  RED_BI_GBE_CFG_FLD_PAUSE_TX_EN,   0) |
                bits(26,17/*RED_BI_GBE_CFG_FLD_IPG_TRANSMIT_TIME*/, 12)) );
    
    // usleep(100);
    
    mac_flush_pio( asic_id );

    mac_write( asic_id, BA(RED_BI0_GBE_CFG),
               (bits(RED_BI_GBE_CFG_FLD_RX_LINK_RST, RED_BI_GBE_CFG_FLD_RX_LINK_RST, 0)) ); 


    // Set Pause to default (disabled)
    // BI in GBE mode
    // mac_write( asic_id, BA(RED_BI0_CFG_PAUSE), RED_BI_CFG_PAUSE_DEF );
    mac_write( asic_id, BA(RED_BI0_GBE_CFG_PAUSE), RED_BI_GBE_CFG_PAUSE_DEF);

    // Setup RX COS_MAP
    fcoe = FALSE;
    mac_write( asic_id, BA(RED_BI0_NON_FCOE_RX_COS_MAP_00),
               qc_mac_hw_rx_cos_map[fcoe][SATCFG_PORT_TYPE_BIF][0] );
    mac_write( asic_id, BA(RED_BI0_NON_FCOE_RX_COS_MAP_01),
               qc_mac_hw_rx_cos_map[fcoe][SATCFG_PORT_TYPE_BIF][1] );
    fcoe = TRUE;
    mac_write( asic_id, BA(RED_BI0_FCOE_RX_COS_MAP_00),
               qc_mac_hw_rx_cos_map[fcoe][SATCFG_PORT_TYPE_BIF][0] );
    mac_write( asic_id, BA(RED_BI0_FCOE_RX_COS_MAP_01),
               qc_mac_hw_rx_cos_map[fcoe][SATCFG_PORT_TYPE_BIF][1] );

    // Setup RX COS2Q
    fcoe = FALSE;
    mac_write( asic_id, BA(RED_BI0_NON_FCOE_COS2Q_MAP),
               qc_mac_hw_cos2q_map[fcoe][SATCFG_PORT_TYPE_BIF][0] );
    fcoe = TRUE;
    mac_write( asic_id, BA(RED_BI0_FCOE_COS2Q_MAP),
               qc_mac_hw_cos2q_map[fcoe][SATCFG_PORT_TYPE_BIF][0] );

    // Setup TX COS_MAP
    fcoe = FALSE;
    mac_write( asic_id, BA(RED_BI0_NON_FCOE_TX_COS_MAP_00),
               qc_mac_hw_tx_cos_map[fcoe][SATCFG_PORT_TYPE_BIF][0] );
    mac_write( asic_id, BA(RED_BI0_NON_FCOE_TX_COS_MAP_01),
               qc_mac_hw_tx_cos_map[fcoe][SATCFG_PORT_TYPE_BIF][1] );
    fcoe = TRUE;
    mac_write( asic_id, BA(RED_BI0_FCOE_TX_COS_MAP_00),
               qc_mac_hw_tx_cos_map[fcoe][SATCFG_PORT_TYPE_BIF][0] );
    mac_write( asic_id, BA(RED_BI0_FCOE_TX_COS_MAP_01),
               qc_mac_hw_tx_cos_map[fcoe][SATCFG_PORT_TYPE_BIF][1] );
    
    // Setup TX FLOW_CTRL_COS2Q
    mac_write ( asic_id, BA(RED_BI0_FLOW_CONTROL_COS2Q_MAP),
                qc_mac_hw_flow_control_cos2q_map[SATCFG_PORT_TYPE_BIF][0] );
    mac_write ( asic_id, BA(RED_BI0_FLOW_CONTROL_Q2COS_MAP),
                qc_mac_hw_q2cos_map[SATCFG_PORT_TYPE_BIF][0] );

    mac_write( asic_id, BA(RED_BI0_GBE_CFG_RX),
               (bits(13,0 /*RED_BI_GBE_CFG_RX_FLD_MAX_SIZE*/, 9216) |
                bits(20,14/*RED_BI_GBE_CFG_RX_FLD_MIN_SIZE*/,   64)) );

    mac_write( asic_id, BA(RED_BI0_GBE_INT), 0xffffffff );

    return 0;
}

/***************************************************************
* mac_bring_down_bif_hw_seq
***************************************************************/
int mac_bring_down_bif_hw_seq( int asic_id )
{
    unsigned int data;
    
    mac_read( asic_id, BA(RED_BI0_GBE_CFG), &data );

    data |=  (bits(RED_BI_GBE_CFG_FLD_RX_LINK_RST,RED_BI_GBE_CFG_FLD_RX_LINK_RST, 1) |
              bits(RED_BI_GBE_CFG_FLD_TX_SW_RST,  RED_BI_GBE_CFG_FLD_TX_SW_RST,   1) |
              bits(RED_BI_GBE_CFG_FLD_RX_SW_RST,  RED_BI_GBE_CFG_FLD_RX_SW_RST,   1));

    mac_write( asic_id, BA(RED_BI0_GBE_CFG), data );
    mac_flush_pio( asic_id );
    return 0;
}

/***************************************************************
* mac_bring_down_cif_hw_seq
***************************************************************/
int mac_bring_down_cif_hw_seq( int asic_id )
{
    unsigned int data;
    
    mac_read( asic_id, BA(RED_CI0_GBE_CFG), &data );

    data |=  (bits(RED_CI_GBE_CFG_FLD_RX_LINK_RST,RED_CI_GBE_CFG_FLD_RX_LINK_RST, 1) |
              bits(RED_CI_GBE_CFG_FLD_TX_SW_RST,  RED_CI_GBE_CFG_FLD_TX_SW_RST,   1) |
              bits(RED_CI_GBE_CFG_FLD_RX_SW_RST,  RED_CI_GBE_CFG_FLD_RX_SW_RST,   1));

    mac_write( asic_id, BA(RED_CI0_GBE_CFG), data );
    mac_flush_pio( asic_id );
    return 0;
}

/***************************************************************
* mac_bring_up_cif_hw_seq
*
* Bring up the CIF MAC/PHY
*
* Note: This sequence is translated from RTL in:
*
* asic/src/redwood/verif/top/tests/SCE_Config.h:cfgMAC4MII
*
* mode,  { 1000, 100, 10 }
*
* (note: must be 100Mb for Saratoga)
*
* returns: 0 == OK
*          else, error code
**************************************************************/
int mac_bring_up_cif_hw_seq( int asic_id, int mode )
{
    unsigned int nspi_timer, pause_fc_limit;
    boolean fcoe;
    
    if (mode == 1000) {
        nspi_timer     = 0x20*4;
        pause_fc_limit = 0x100;
    }
    else if (mode == 100) {
        nspi_timer     = 0x20*4*10;
        pause_fc_limit = 0x200;
    }
    else {
        nspi_timer     = 0x20*4*100;
        pause_fc_limit = 0x200;
    }

    mac_write( asic_id, BA(RED_CI0_CFG_EG_NSPI_TIMER),      nspi_timer );
    mac_write( asic_id, BA(RED_CI0_GBE_CFG_PAUSE_FC_LIMIT), pause_fc_limit );

    mac_write( asic_id, BA(RED_CI0_GBE_CFG_MII_MDC_PERIOD), 60 );

    mac_write( asic_id, BA(RED_CI0_GBE_CFG),
               (bits(RED_CI_GBE_CFG_FLD_RX_LINK_RST,  RED_CI_GBE_CFG_FLD_RX_LINK_RST,   1) |
                bits(RED_CI_GBE_CFG_FLD_RX_CRC_CHK_EN,RED_CI_GBE_CFG_FLD_RX_CRC_CHK_EN, 1) |
                bits(RED_CI_GBE_CFG_FLD_PAUSE_TX_EN,  RED_CI_GBE_CFG_FLD_PAUSE_TX_EN,   0) |
                bits(26,17/*RED_CI_GBE_CFG_FLD_IPG_TRANSMIT_TIME*/, 12)) );
    
    usleep(100);

    mac_flush_pio( asic_id );

    mac_write( asic_id, BA(RED_CI0_GBE_CFG), bits(26,17/*RED_CI_GBE_CFG_FLD_IPG_TRANSMIT_TIME*/, 12)); 


    // Set Pause to default (disabled)
    // CI in GBE mode
    mac_write( asic_id, BA(RED_CI0_GBE_CFG_PAUSE), RED_CI_GBE_CFG_PAUSE_DEF);

    // Setup RX COS_MAP
    fcoe = FALSE;
    mac_write( asic_id, BA(RED_CI0_NON_FCOE_RX_COS_MAP_00),
               qc_mac_hw_rx_cos_map[fcoe][SATCFG_PORT_TYPE_CIF][0] );
    mac_write( asic_id, BA(RED_CI0_NON_FCOE_RX_COS_MAP_01),
               qc_mac_hw_rx_cos_map[fcoe][SATCFG_PORT_TYPE_CIF][1] );
    fcoe = TRUE;
    mac_write( asic_id, BA(RED_CI0_FCOE_RX_COS_MAP_00),
               qc_mac_hw_rx_cos_map[fcoe][SATCFG_PORT_TYPE_CIF][0] );
    mac_write( asic_id, BA(RED_CI0_FCOE_RX_COS_MAP_01),
               qc_mac_hw_rx_cos_map[fcoe][SATCFG_PORT_TYPE_CIF][1] );

    // Setup RX COS2Q
    fcoe = FALSE;
    mac_write( asic_id, BA(RED_CI0_NON_FCOE_COS2Q_MAP),
               qc_mac_hw_cos2q_map[fcoe][SATCFG_PORT_TYPE_CIF][0] );
    fcoe = TRUE;
    mac_write( asic_id, BA(RED_CI0_FCOE_COS2Q_MAP),
               qc_mac_hw_cos2q_map[fcoe][SATCFG_PORT_TYPE_CIF][0] );

    // Setup TX COS_MAP
    fcoe = FALSE;
    mac_write( asic_id, BA(RED_CI0_NON_FCOE_TX_COS_MAP_00),
               qc_mac_hw_tx_cos_map[fcoe][SATCFG_PORT_TYPE_CIF][0] );
    mac_write( asic_id, BA(RED_CI0_NON_FCOE_TX_COS_MAP_01),
               qc_mac_hw_tx_cos_map[fcoe][SATCFG_PORT_TYPE_CIF][1] );
    fcoe = TRUE;
    mac_write( asic_id, BA(RED_CI0_FCOE_TX_COS_MAP_00),
               qc_mac_hw_tx_cos_map[fcoe][SATCFG_PORT_TYPE_CIF][0] );
    mac_write( asic_id, BA(RED_CI0_FCOE_TX_COS_MAP_01),
               qc_mac_hw_tx_cos_map[fcoe][SATCFG_PORT_TYPE_CIF][1] );
    
    // Setup TX FLOW_CTRL_COS2Q
    mac_write ( asic_id, BA(RED_CI0_FLOW_CONTROL_COS2Q_MAP),
                qc_mac_hw_flow_control_cos2q_map[SATCFG_PORT_TYPE_CIF][0] );
    mac_write ( asic_id, BA(RED_CI0_FLOW_CONTROL_Q2COS_MAP),
                qc_mac_hw_q2cos_map[SATCFG_PORT_TYPE_CIF][0] );

    mac_write( asic_id, BA(RED_CI0_GBE_CFG_RX),
               (bits(13,0 /*RED_CI_GBE_CFG_RX_FLD_MAX_SIZE*/, 9216) |
                bits(20,14/*RED_CI_GBE_CFG_RX_FLD_MIN_SIZE*/,   64)) );

    mac_write( asic_id, BA(RED_CI0_GBE_INT), 0xffffffff );

    return 0;
}

void mac_pgm_serdes( int asic_id, int mac_id, int first_lane, int num_lanes,
                     int rx_polarity, int tx_polarity )
{
    int lane, n;
    int is_1g = 0;

    serdes_init_1(      asic_id, mac_id, first_lane, num_lanes, is_1g );
    serdes_rx_rdy_wait( asic_id, mac_id, first_lane, num_lanes );
    serdes_init_2(      asic_id, mac_id, first_lane, num_lanes );

    serdes_lower_sig_thresh( asic_id, mac_id, first_lane, num_lanes );


    /* fix serdes polarity issues. Due to board layout some lanes
    *  polarity may get swapped */
    for (n = 0, lane = first_lane; n < num_lanes; n++, lane = ++lane % num_lanes) {

        if (rx_polarity & (1 << ((first_lane + n) % num_lanes)))
            serdes_invert_polarity_on_rx_lane( asic_id, mac_id, lane );

        if (tx_polarity & (1 << ((first_lane + n) % num_lanes)))
            serdes_invert_polarity_on_tx_lane( asic_id, mac_id, lane );
    }
}

unsigned int ss_rdch_to_flush[] = {
    RED_SS0_CFG_RDCH0,
    RED_SS0_CFG_RDCH1,
    RED_SS0_CFG_RDCH2,
    RED_SS0_CFG_RDCH3,
    RED_SS0_CFG_RDCH0,
    RED_SS0_CFG_RDCH1,
    RED_SS0_CFG_RDCH2,
    RED_SS0_CFG_RDCH3
};

unsigned int hi_sw_rst_tbl[] = {
    RED_HI0_SW_RST,
    RED_HI1_SW_RST,
    RED_HI2_SW_RST,
    RED_HI3_SW_RST,
    RED_HI4_SW_RST,
    RED_HI5_SW_RST,
    RED_HI6_SW_RST,
    RED_HI7_SW_RST
};

unsigned int ni_sw_rst_tbl[] = {
    RED_NI0_SW_RST,
    RED_NI1_SW_RST,
    RED_NI2_SW_RST,
    RED_NI3_SW_RST
};


/***************************************************************
* mac_set_ss_wr_port_in_store_n_fwd_for_1g_hif
***************************************************************/
void mac_set_ss_wr_port_in_store_n_fwd_for_1g_hif( int asic_id, int mac_id /*0..7*/ )
{
    unsigned int reg_val;
    unsigned int ss;

    /* Set the associated SS block to read-port-flush */
    ss = BA(ss_wr_port_to_set[ mac_id ]) + ((mac_id/4) * 0x10000);

    mac_read(  asic_id, ss, &reg_val );
    reg_val &= ~(0xF << 2); // ct=0
    mac_write( asic_id, ss, reg_val );
}

/***************************************************************
* mac_bring_up_hif_1g_hw_seq
*
* Bring up the specified MAC/PHY
*
* Note: This sequence is translated from RTL test code in:
*
* asic/src/redwood/verif/top/tests/sce_pk_fw.h
*
* It adds the MAC configuration parameters that are used in
* SCE_Config.h::cfgMACFor10G
*
* returns: 0 == OK
*          else, error code
**************************************************************/
int mac_bring_up_hif_1g_hw_seq( int asic_id,
                                int mac_id /*0..7*/,
                                mac_cfg_t *p_cfg,
                                int rx_polarity, int tx_polarity,
                                int steps )
{
    unsigned int nspi_timer, pause_fc_limit, data;
    unsigned int ofs = MAC_OFFSET( mac_id );
    int err;

    if (DO_STEP(0, steps)) {

        /* release soft reset
        * MAC must be in (MAC) reset before releasing soft reset. This should
        * be guaranteed by either the ASIC bring-up sequence or the previous
        * MAC bring-down sequence.
        */
        mac_read( asic_id,  BA(RED_MISC_SW_RST), &data );
        data &= ~( BIT( mac_id )); // logical HIF number
        mac_write( asic_id, BA(RED_MISC_SW_RST), data );
        mac_flush_pio( asic_id );

        /* confire wr_port for store and forward mode */
        mac_set_ss_wr_port_in_store_n_fwd_for_1g_hif( asic_id, mac_id );

        // enable clock for the 1GE (there is clock gating logic in 1GE IP)
        mac_write( asic_id, BA(RED_HI0_GBE_CFG_GBE) + ofs, 1 );

        // reset mac using sw_rst pin to the 1GE IP
        mac_read( asic_id, BA(RED_HI0_SW_RST) + ofs, &data );
        data |= (1 << 17); // mac reset, resets rx and tx mac
        data |= (1 << 2); // mac reset, resets rx and tx mac
        mac_write( asic_id, BA(RED_HI0_SW_RST)  + ofs, data );
        mac_flush_pio( asic_id );
        usleep(10);

        /* clean up any wierd ints this may have caused */
        mac_write( asic_id, BA(RED_HI0_GBE_INT) + ofs, 0xffffffff );

        // clear mac reset
        data &= ~(1 << 2);
        data &= ~(1 << 17);
        mac_write( asic_id, BA(RED_HI0_SW_RST)  + ofs, data );
        mac_flush_pio( asic_id );

        data  = RED_HI_GBE_CFG_DEF;
        data |= (bits(RED_HI_GBE_CFG_FLD_RX_LINK_RST,   RED_HI_GBE_CFG_FLD_RX_LINK_RST,   1) |
                 bits(RED_HI_GBE_CFG_FLD_RX_CRC_CHK_EN, RED_HI_GBE_CFG_FLD_RX_CRC_CHK_EN, 1) |
                 bits(26,17/*RED_HI_GBE_CFG_FLD_IPG_TRANSMIT_TIME*/, 12)                     |
                 bits(15,15/*RED_HI_GBE_CFG_FLD_PAUSE_TX_EN*/, 1));

        /* set rx_link_rst */
        mac_write( asic_id, BA(RED_HI0_GBE_CFG) + ofs, data );
        mac_flush_pio( asic_id );
    }

    if (DO_STEP(1, steps)) {

        //usleep(100);
    }

    if (DO_STEP(2, steps)) {

        /****************************
        *  clear: rx_link_rst.
        ****************************/
        mac_read(  asic_id, BA(RED_HI0_GBE_CFG) + ofs, &data );
        data &= ~(bits(RED_HI_GBE_CFG_FLD_RX_LINK_RST,   RED_HI_GBE_CFG_FLD_RX_LINK_RST,   1));
        mac_write( asic_id, BA(RED_HI0_GBE_CFG) + ofs, data );

        data |= bits( 3, 3/*FLD_TX_FORCE_XMIT_DATA*/,1);
        mac_write( asic_id, BA(RED_HI0_GBE_CFG) + ofs, data );

        /* enable RX flush */
        mac_write( asic_id, BA(RED_HI0_GBE_CFG_RX) + ofs,
                   (bits(21,21,/*RED_HI_GBE_CFG_RX_FLD_FLUSH_EN*/ 1)    |
                    bits(13,0 /*RED_HI_GBE_CFG_RX_FLD_MAX_SIZE*/, 9216) |
                    bits(20,14/*RED_HI_GBE_CFG_RX_FLD_MIN_SIZE*/,   64)) );

        /* Enable tx flush */
        mac_write( asic_id, BA(RED_HI0_GBE_CFG_TX_FLUSH_EN) + ofs, 1 );

        // Set Pause to default (disabled)
        // HI in GBE mode
        mac_write( asic_id, BA(RED_HI0_GBE_CFG_PAUSE) + ofs, RED_HI_GBE_CFG_PAUSE_DEF);

        /* set up for 1G (only) */
        nspi_timer     = 0x140; //0x20*4; // may need to be 0x140 (SCE_Config::setupTrafficForOneHIF)
        pause_fc_limit = 0x100;

        mac_write( asic_id, BA(RED_HI0_CFG_EG_NSPI_TIMER)      + ofs, nspi_timer );
        mac_write( asic_id, BA(RED_HI0_GBE_CFG_PAUSE_FC_LIMIT) + ofs, pause_fc_limit );
        //mac_write( asic_id, BA(RED_HI0_GBE_CFG_MII_MDC_PERIOD) + ofs, 60 );

        /* Set up lane map.
        * Also fills in the "extra" MAC config info that is found in
        * SCE_Config.h */
        if ((err = mac_config( asic_id, p_cfg, ofs,
                               BA(RED_HI0_CFG_CODE_PAUSE),
                               BA(RED_HI0_MAC_FRM_LENGTH_MAX),
                               BA(RED_HI0_MAC_EXT_LPBK),
                               BA(RED_HI0_PKT_LPBK),
                               BA(RED_HI0_MAC_CFG),
                               BA(RED_HI0_MAC_CFG_EXT) )) != 0)  return err;
        /* cleanup any ints caused by config  */
        mac_write( asic_id, BA(RED_HI0_GBE_INT) + ofs, 0xffffffff );
        mac_flush_pio( asic_id );
    }

    return 0;
}

/***************************************************************
* mac_bring_up_hif_hw_seq
*
* Bring up the specified MAC/PHY
*
* Note: This sequence is translated from RTL in:
*
* asic/src/redwood/verif/top/tests/sce_pk_fw.h
*
* It adds the MAC configuration parameters that are used in 
* SCE_Config.h::cfgMACFor10G
*
* returns: 0 == OK
*          else, error code
**************************************************************/
int mac_bring_up_hif_hw_seq( int asic_id, 
                             int mac_id /*0..7*/,
                             mac_cfg_t *p_cfg,
                             int rx_polarity, int tx_polarity )
{
    unsigned int reg_val, ss, ofs = MAC_OFFSET( mac_id );
    boolean fcoe;
    int err;

    /* program the serdes first, to get a clk to the MAC */
    mac_pgm_serdes( asic_id, mac_id, 0, 4, rx_polarity, tx_polarity );

    /* Make sure the MAC is in reset */
    mac_write( asic_id, BA(RED_HI0_SW_RST) + ofs, ~0 );

    /* Assumption is the MAC is in reset at this point. This function
    * fills in the "extra" MAC config info tht is found in
    * SCE_Config.h */
    if ((err = mac_config( asic_id, p_cfg, ofs,
                           BA(RED_HI0_CFG_CODE_PAUSE),
                           BA(RED_HI0_MAC_FRM_LENGTH_MAX),
                           BA(RED_HI0_MAC_EXT_LPBK),
                           BA(RED_HI0_PKT_LPBK),
                           BA(RED_HI0_MAC_CFG),
                           BA(RED_HI0_MAC_CFG_EXT) )) != 0)  return err;
    
    /* release the 10b/20b converter */
    mac_read(  asic_id, BA(RED_HI0_SW_RST) + ofs, &reg_val );
    reg_val &= ~( BIT( RED_HI_SW_RST_FLD_RESET_10B20B ));
    mac_write( asic_id, BA(RED_HI0_SW_RST) + ofs, reg_val );

    /*  wait for some time for clk to arrive at the mac */
    mac_flush_pio( asic_id );
    mac_flush_pio( asic_id );
    mac_flush_pio( asic_id );

    /* release soft reset 
    * MAC must be in (MAC) reset before releasing soft reset. This should
    * be guaranteed by either the ASIC bring-up sequence or the previous
    * MAC bring-down sequence.
    */
    mac_read( asic_id,  BA(RED_MISC_SW_RST), &reg_val );
    reg_val &= ~( BIT( mac_id )); // logical HIF number
    mac_write( asic_id, BA(RED_MISC_SW_RST), reg_val );
    mac_flush_pio( asic_id );


    // Set Pause to default (disabled)
    // HI in XGBE mode
    // mac_write( asic_id, BA(RED_HI0_GBE_CFG_PAUSE) + ofs, RED_HI_GBE_CFG_PAUSE_DEF);
    mac_write( asic_id, BA(RED_HI0_CFG_PAUSE) + ofs, RED_HI_CFG_PAUSE_DEF | 0x802 /*802.3x pause*/);

    // Setup RX COS_MAP
    fcoe = FALSE;
    mac_write( asic_id, BA(RED_HI0_NON_FCOE_RX_COS_MAP_00) + ofs,
               qc_mac_hw_rx_cos_map[fcoe][SATCFG_PORT_TYPE_HIF][2 * mac_id + 0] );
    mac_write( asic_id, BA(RED_HI0_NON_FCOE_RX_COS_MAP_01) + ofs,
               qc_mac_hw_rx_cos_map[fcoe][SATCFG_PORT_TYPE_HIF][2 * mac_id + 1] );
    fcoe = TRUE;
    mac_write( asic_id, BA(RED_HI0_FCOE_RX_COS_MAP_00) + ofs,
               qc_mac_hw_rx_cos_map[fcoe][SATCFG_PORT_TYPE_HIF][2 * mac_id + 0] );
    mac_write( asic_id, BA(RED_HI0_FCOE_RX_COS_MAP_01) + ofs,
               qc_mac_hw_rx_cos_map[fcoe][SATCFG_PORT_TYPE_HIF][2 * mac_id + 1] );

    // Setup RX COS2Q
    fcoe = FALSE;
    mac_write( asic_id, BA(RED_HI0_NON_FCOE_COS2Q_MAP) + ofs,
               qc_mac_hw_cos2q_map[fcoe][SATCFG_PORT_TYPE_HIF][mac_id] );
    fcoe = TRUE;
    mac_write( asic_id, BA(RED_HI0_FCOE_COS2Q_MAP) + ofs,
               qc_mac_hw_cos2q_map[fcoe][SATCFG_PORT_TYPE_HIF][mac_id] );

    // Setup TX COS_MAP
    fcoe = FALSE;
    mac_write( asic_id, BA(RED_HI0_NON_FCOE_TX_COS_MAP_00) + ofs,
               qc_mac_hw_tx_cos_map[fcoe][SATCFG_PORT_TYPE_HIF][2 * mac_id + 0] );
    mac_write( asic_id, BA(RED_HI0_NON_FCOE_TX_COS_MAP_01) + ofs,
               qc_mac_hw_tx_cos_map[fcoe][SATCFG_PORT_TYPE_HIF][2 * mac_id + 1] );
    fcoe = TRUE;
    mac_write( asic_id, BA(RED_HI0_FCOE_TX_COS_MAP_00) + ofs,
               qc_mac_hw_tx_cos_map[fcoe][SATCFG_PORT_TYPE_HIF][2 * mac_id + 0] );
    mac_write( asic_id, BA(RED_HI0_FCOE_TX_COS_MAP_01) + ofs,
               qc_mac_hw_tx_cos_map[fcoe][SATCFG_PORT_TYPE_HIF][2 * mac_id + 1] );
    
    // Setup TX FLOW_CTRL_COS2Q
    mac_write ( asic_id, BA(RED_HI0_FLOW_CONTROL_COS2Q_MAP) + ofs,
                qc_mac_hw_flow_control_cos2q_map[SATCFG_PORT_TYPE_HIF][mac_id] );
    mac_write ( asic_id, BA(RED_HI0_FLOW_CONTROL_Q2COS_MAP) + ofs,
                qc_mac_hw_q2cos_map[SATCFG_PORT_TYPE_HIF][mac_id] );
                

    /* re-enable rx */
    mac_read( asic_id,  BA(RED_HI0_MAC_CFG) + ofs, &reg_val );
    reg_val |= BIT(RED_HI_MAC_CFG_00_FLD_ENABLE_RX);
    reg_val |= BIT(RED_HI_MAC_CFG_00_FLD_ENABLE_TX);
    mac_write( asic_id, BA(RED_HI0_MAC_CFG) + ofs, reg_val );
    
    /* deassert flush and disable "inrange" chk */
    mac_read(  asic_id, BA(RED_HI0_MAC_CFG_EXT) + ofs, &reg_val);
    reg_val &= ~( BIT(RED_HI_MAC_CFG_EXT_FLD_TX_FLUSH) );
    reg_val &= ~( BIT(RED_HI_MAC_CFG_EXT_FLD_RX_INRANGE_CHK_EN) );
    mac_write( asic_id, BA(RED_HI0_MAC_CFG_EXT) + ofs, reg_val);
    mac_flush_pio( asic_id );

    /* release MAC reset (note; MUST be after releasing "soft reset") */
    mac_write( asic_id, BA(RED_HI0_SW_RST) + ofs, 0 );
  
    /* De-assert the associated SS block read-port-flush */
    ss = BA(ss_rdch_to_flush[ mac_id ]) + ((mac_id/4) * 0x10000);
    mac_read(  asic_id, ss, &reg_val );
    reg_val &= ~0x00F0; // RED_SS_CFG_RDCHx_FLD_QUE_FLUSH
    mac_write( asic_id, ss, reg_val );

    return 0;
}

/***************************************************************
* mac_enable_hif_hw_seq
*
* Enable the specified MAC/PHY
**************************************************************/
int mac_enable_hif_hw_seq( int asic_id, int mac_id /*0..7*/ )
{
    unsigned int reg_val, ofs = MAC_OFFSET( mac_id );

    /* re-enable tx/rx */
    mac_read( asic_id,  BA(RED_HI0_MAC_CFG) + ofs, &reg_val );
    reg_val |= BIT(RED_HI_MAC_CFG_00_FLD_ENABLE_RX);
    reg_val |= BIT(RED_HI_MAC_CFG_00_FLD_ENABLE_TX);
    mac_write( asic_id, BA(RED_HI0_MAC_CFG) + ofs, reg_val );

    /* flush pio */
    mac_flush_pio( asic_id );

    return 0;
}

/***************************************************************
* mac_enable_hif_1g_hw_seq
*
* Enable the specified MAC/PHY
**************************************************************/
int mac_enable_hif_1g_hw_seq( int asic_id, int mac_id /*0..7*/ )
{
    unsigned int reg_val, ofs = MAC_OFFSET( mac_id );

    /* Remove rx flush */
    mac_read( asic_id,  BA(RED_HI0_GBE_CFG_RX) + ofs, &reg_val );
    reg_val &= ~(bits(21,21,/*RED_HI_GBE_CFG_RX_FLD_FLUSH_EN*/ 1));
    mac_write( asic_id, BA(RED_HI0_GBE_CFG_RX) + ofs, reg_val );

    /* Remove tx flush */
    mac_write( asic_id, BA(RED_HI0_GBE_CFG_TX_FLUSH_EN) + ofs, 0 );

    /* flush pio */
    mac_flush_pio( asic_id );

    return 0;
}


/***************************************************************
* mac_assert_flush_hif_ss
***************************************************************/
void mac_assert_flush_hif_ss( int asic_id, int mac_id /*0..7*/ )
{
    unsigned int reg_val;
    unsigned int ss;

    /* Set the associated SS block to read-port-flush */
    ss = BA(ss_rdch_to_flush[ mac_id ]) + ((mac_id/4) * 0x10000);

    mac_read(  asic_id, ss, &reg_val );
    reg_val |= 0x00F0; // RED_SS_CFG_RDCHx_FLD_QUE_FLUSH
    mac_write( asic_id, ss, reg_val );
}


/***************************************************************
* mac_assert_flush_hif
*
* Assert flush on both the MAC and the associated SS (ss0 for
* HI0-3, ss1 for HI4-7).
*
* This function ha sbeen abstracted from the MAC bring-down
* sequence because it is also required when a PORT link state
* becomes operationally "Down". Flush must be asserted when the
* link state goes down to minimize HOL blocking in the SS.
*
**************************************************************/
void mac_assert_flush_hif( int asic_id, int mac_id /*0..7*/ )
{
    unsigned int reg_val, ofs = MAC_OFFSET( mac_id );

    /* enable tx flush */
    mac_read(  asic_id, BA(RED_HI0_MAC_CFG_EXT) + ofs, &reg_val );
    reg_val |= BIT(RED_HI_MAC_CFG_EXT_FLD_TX_FLUSH);
    mac_write( asic_id, BA(RED_HI0_MAC_CFG_EXT) + ofs, reg_val );

    /* Set the associated SS block to read-port-flush */
    mac_assert_flush_hif_ss( asic_id, mac_id );

    /* flush pio */
    mac_flush_pio( asic_id );
}

/***************************************************************
* mac_deassert_flush_hif_ss
***************************************************************/
void mac_deassert_flush_hif_ss( int asic_id, int mac_id /*0..7*/ )
{
    unsigned int reg_val;
    unsigned int ss;

    /* Clear the associated SS block to read-port-flush */
    ss = BA(ss_rdch_to_flush[ mac_id ]) + ((mac_id/4) * 0x10000);

    mac_read(  asic_id, ss, &reg_val );
    reg_val &= ~0x00F0; // RED_SS_CFG_RDCHx_FLD_QUE_FLUSH
    mac_write( asic_id, ss, reg_val );
}

/***************************************************************
* mac_deassert_flush_hif
*
* Deassert flush on both the MAC and the associated SS (ss0 for
* HI0-3, ss1 for HI4-7).
*
* This function ha sbeen abstracted from the MAC bring-down
* sequence because it is also required when a PORT link state
* becomes operationally "Down". Flush must be asserted when the
* link state goes down to minimize HOL blocking in the SS.
*
**************************************************************/
void mac_deassert_flush_hif( int asic_id, int mac_id /*0..7*/ )
{
    unsigned int reg_val, ofs = MAC_OFFSET( mac_id );

    /* Clear the associated SS block to read-port-flush */
    mac_deassert_flush_hif_ss( asic_id, mac_id );

    /* disable tx flush */
    mac_read(  asic_id, BA(RED_HI0_MAC_CFG_EXT) + ofs, &reg_val );
    reg_val &= ~BIT(RED_HI_MAC_CFG_EXT_FLD_TX_FLUSH);
    mac_write( asic_id, BA(RED_HI0_MAC_CFG_EXT) + ofs, reg_val );

    /* flush pio */
    mac_flush_pio( asic_id );
}

/***************************************************************
* mac_assert_flush_nif_ss
***************************************************************/
void mac_assert_flush_nif_ss( int asic_id, int mac_id /*0..3*/ )
{
    unsigned int reg_val;
    unsigned int ss2, ss3;

    /* assert flush on the two associated SS blocks */
    ss2 = BA(ss_rdch_to_flush[ mac_id ]) + 0x20000; //ss2
    ss3 = BA(ss_rdch_to_flush[ mac_id ]) + 0x30000; //ss3

    mac_read(  asic_id, ss2, &reg_val );
    reg_val |= 0x00F0; // RED_SS_CFG_RDCHx_FLD_QUE_FLUSH
    mac_write( asic_id, ss2, reg_val );

    mac_read(  asic_id, ss3, &reg_val );
    reg_val |= 0x00F0; // RED_SS_CFG_RDCHx_FLD_QUE_FLUSH
    mac_write( asic_id, ss3, reg_val );
}

/***************************************************************
* mac_assert_flush_nif
*
* Assert flush on both the MAC and the associated SS's (ss2
* and ss3 for NI).
*
* This function ha sbeen abstracted from the MAC bring-down
* sequence because it is also required when a PORT link state
* becomes operationally "Down". Flush must be asserted when the
* link state goes down to minimize HOL blocking in the SS.
*
**************************************************************/
void mac_assert_flush_nif( int asic_id, int mac_id /*0..3*/ )
{
    unsigned int reg_val, ofs = MAC_OFFSET( mac_id );

    /* deassert tx flush on mac and disable "inrange" chk */
    mac_read(  asic_id, BA(RED_NI0_MAC_CFG_EXT) + ofs, &reg_val );
    reg_val |= BIT(RED_NI_MAC_CFG_EXT_FLD_TX_FLUSH);
    mac_write( asic_id, BA(RED_NI0_MAC_CFG_EXT) + ofs, reg_val );

    /* assert flush on the two associated SS blocks */
    mac_assert_flush_nif_ss( asic_id, mac_id );

    /* flush pio */
    mac_flush_pio( asic_id );
}

/***************************************************************
* mac_deassert_flush_nif_ss
***************************************************************/
void mac_deassert_flush_nif_ss( int asic_id, int mac_id /*0..3*/ )
{
    unsigned int reg_val;
    unsigned int ss1, ss2;

    /* deassert flush on the two associated SS blocks */
    ss1 = BA(ss_rdch_to_flush[ mac_id ]) + 0x20000; //ss2
    ss2 = BA(ss_rdch_to_flush[ mac_id ]) + 0x30000; //ss3

    mac_read( asic_id, ss1, &reg_val );
    reg_val &= ~0x00F0; // RED_SS_CFG_RDCHx_FLD_QUE_FLUSH
    mac_write( asic_id, ss1, reg_val );

    mac_read( asic_id, ss2, &reg_val );
    reg_val &= ~0x00F0; // RED_SS_CFG_RDCHx_FLD_QUE_FLUSH
    mac_write( asic_id, ss2, reg_val );
}

/***************************************************************
* mac_deassert_flush_nif
*
* Deassert flush on both the MAC and the associated SS's (ss2
* and ss3 for NI).
*
* This function ha sbeen abstracted from the MAC bring-up
* sequence because it is also required when a PORT link state
* becomes operationally "Up" (Flush must be asserted when the
* link state goes down to minimize HOL blocking in the SS).
*
**************************************************************/
void mac_deassert_flush_nif( int asic_id, int mac_id /*0..3*/ )
{
    unsigned int reg_val, ofs = MAC_OFFSET( mac_id );

    /* deassert flush on the two associated SS blocks */
    mac_deassert_flush_nif_ss( asic_id, mac_id );

    /* deassert tx flush on mac and disable "inrange" chk */
    mac_read(  asic_id, BA(RED_NI0_MAC_CFG_EXT) + ofs, &reg_val );
    reg_val &= ~( BIT(RED_NI_MAC_CFG_EXT_FLD_TX_FLUSH) );
    mac_write( asic_id, BA(RED_NI0_MAC_CFG_EXT) + ofs, reg_val );

    /* flush pio */
    mac_flush_pio( asic_id );
}


/***************************************************************
* mac_bring_up_nif_hw_seq
*
* Bring up the specified MAC/PHY
*
* Note: This sequence is translated from RTL in:
*
* asic/src/redwood/verif/top/tests/sce_pk_fw.h
*
* It adds the MAC configuration parameters that are used in 
* SCE_Config.h::cfgMACFor10G
*
**************************************************************/
int mac_bring_up_nif_hw_seq( int asic_id, 
                             int mac_id /*0..7*/,
                             mac_cfg_t *p_cfg, 
                             int rx_polarity, int tx_polarity )
{
    unsigned int reg_val, ofs = MAC_OFFSET( mac_id );
    unsigned int ss1, ss2;
    boolean fcoe;
    int err;

    /* Program the serdes first to get a clk to the MAC */
    mac_pgm_serdes( asic_id, 8+mac_id, 0, 4, rx_polarity, tx_polarity );

    /* Make sure the MAC is in reset */
    mac_write( asic_id, BA(RED_NI0_SW_RST) + ofs, ~0 );

    /* Assumption is the MAC is in reset at this point. This function
    * fills in the "extra" MAC config info tht is found in
    * SCE_Config.h */
    if ((err = mac_config( asic_id, p_cfg, ofs,
                           BA(RED_NI0_CFG_CODE_PAUSE),
                           BA(RED_NI0_MAC_FRM_LENGTH_MAX),
                           BA(RED_NI0_MAC_EXT_LPBK),
                           BA(RED_NI0_PKT_LPBK),
                           BA(RED_NI0_MAC_CFG),
                           BA(RED_NI0_MAC_CFG_EXT) )) != 0)  return err;

    /* release the 10b/20b converter */
    mac_read(  asic_id, BA(RED_NI0_SW_RST) + ofs, &reg_val );
    reg_val &= ~( BIT( RED_NI_SW_RST_FLD_RESET_10B20B ));
    mac_write( asic_id, BA(RED_NI0_SW_RST) + ofs, reg_val );
    
    /* wait for some time for clk to arrive at the mac */
    //usleep(300);
    mac_flush_pio( asic_id );
    mac_flush_pio( asic_id );
    mac_flush_pio( asic_id );
    
    /* Release soft reset
    * MAC must be in (MAC) reset before releasing soft reset. This should
    * be guaranteed by either the ASIC bring-up sequence or the previous
    * MAC bring-down sequence.
    */
    mac_read(  asic_id, BA(RED_MISC_SW_RST), &reg_val );
    reg_val &= ~( BIT( (8+mac_id) )); // logical NIF number + 8 HIFs
    mac_write( asic_id, BA(RED_MISC_SW_RST), reg_val );


    // Set Pause to default (disabled)
    // NI in XGBE mode
    mac_write( asic_id, BA(RED_NI0_CFG_PAUSE) + ofs, RED_NI_CFG_PAUSE_DEF | 0x802 /*802.3x pause*/);
    
    // Setup RX COS2Q
    fcoe = FALSE;
    mac_write( asic_id, BA(RED_NI0_NON_FCOE_COS2Q_MAP) + ofs,
               qc_mac_hw_cos2q_map[fcoe][SATCFG_PORT_TYPE_NIF][mac_id] );
    fcoe = TRUE;
    mac_write( asic_id, BA(RED_NI0_FCOE_COS2Q_MAP) + ofs,
               qc_mac_hw_cos2q_map[fcoe][SATCFG_PORT_TYPE_NIF][mac_id] );

    // Setup TX FLOW_CTRL_COS2Q
    mac_write ( asic_id, BA(RED_NI0_FLOW_CONTROL_COS2Q_MAP) + ofs,
                qc_mac_hw_flow_control_cos2q_map[SATCFG_PORT_TYPE_NIF][mac_id] );
    mac_write ( asic_id, BA(RED_NI0_FLOW_CONTROL_Q2COS_MAP) + ofs,
                qc_mac_hw_q2cos_map[SATCFG_PORT_TYPE_NIF][mac_id] );
                
    /* enable Rx */
    mac_read(  asic_id, BA(RED_NI0_MAC_CFG) + ofs, &reg_val );
    reg_val |= BIT(RED_NI_MAC_CFG_00_FLD_ENABLE_RX);
    reg_val |= BIT(RED_NI_MAC_CFG_00_FLD_ENABLE_TX);
    mac_write( asic_id, BA(RED_NI0_MAC_CFG) + ofs, reg_val );

    /* deassert tx flush on mac and disable "inrange" chk */
    mac_read(  asic_id, BA(RED_NI0_MAC_CFG_EXT) + ofs, &reg_val );
    reg_val &= ~( BIT(RED_NI_MAC_CFG_EXT_FLD_TX_FLUSH) );
    reg_val &= ~( BIT(RED_NI_MAC_CFG_EXT_FLD_RX_INRANGE_CHK_EN) );
    mac_write( asic_id, BA(RED_NI0_MAC_CFG_EXT) + ofs, reg_val );

    /* release MAC reset (note; MUST be after releasing "soft reset") */
    mac_write( asic_id, BA(RED_NI0_SW_RST) + ofs, 0 );

    /* deassert flush on the two associated SS blocks */
    ss1 = BA(ss_rdch_to_flush[ mac_id ]) + 0x20000; //ss2
    ss2 = BA(ss_rdch_to_flush[ mac_id ]) + 0x30000; //ss3

    mac_read( asic_id, ss1, &reg_val );
    reg_val &= ~0x00F0; // RED_SS_CFG_RDCHx_FLD_QUE_FLUSH
    mac_write( asic_id, ss1, reg_val );
    
    mac_read( asic_id, ss2, &reg_val );
    reg_val &= ~0x00F0; // RED_SS_CFG_RDCHx_FLD_QUE_FLUSH
    mac_write( asic_id, ss2, reg_val );
    
    /* flush pio */
//    mac_read( asic_id, BA(RED_NI0_MAC_CFG) + ofs, &reg_val );
    mac_flush_pio( asic_id );

    return 0;
}


/***************************************************************
* mac_enable_nif_hw_seq
*
* Enable the specified MAC/PHY
**************************************************************/
int mac_enable_nif_hw_seq( int asic_id, int mac_id /*0..7*/ )
{
    unsigned int reg_val, ofs = MAC_OFFSET( mac_id );

    /* enable Rx/Tx */
    mac_read(  asic_id, BA(RED_NI0_MAC_CFG) + ofs, &reg_val );
    reg_val |= BIT(RED_NI_MAC_CFG_00_FLD_ENABLE_RX);
    reg_val |= BIT(RED_NI_MAC_CFG_00_FLD_ENABLE_TX);
    mac_write( asic_id, BA(RED_NI0_MAC_CFG) + ofs, reg_val );

    /* flush pio */
    mac_flush_pio( asic_id );

    return 0;
}

/***************************************************************
* mac_bring_down_hif_hw_seq
*
* Bring down the specified MAC/PHY.
*
* Note: This sequence is translated from RTL in:
* 
* asic/src/redwood/verif/top/tests/sce_pk_fw.h
*
**************************************************************/
int mac_bring_down_hif_hw_seq( int asic_id, int mac_id )
{
    unsigned int reg_val, ss, ofs = MAC_OFFSET( mac_id );

    /* disable Rx */
    mac_read( asic_id, BA(RED_HI0_MAC_CFG) + ofs, &reg_val );
    reg_val &= ~( BIT(RED_HI_MAC_CFG_00_FLD_ENABLE_RX) );
    mac_write( asic_id, BA(RED_HI0_MAC_CFG) + ofs, reg_val );
    
    /* enable tx flush */
    mac_read(  asic_id, BA(RED_HI0_MAC_CFG_EXT) + ofs, &reg_val );
    reg_val |= BIT(RED_HI_MAC_CFG_EXT_FLD_TX_FLUSH);
    mac_write( asic_id, BA(RED_HI0_MAC_CFG_EXT) + ofs, reg_val );

    /* Set the associated SS block to read-port-flush */
    ss = BA(ss_rdch_to_flush[ mac_id ]) + ((mac_id/4) * 0x10000);
 
    mac_read(  asic_id, ss, &reg_val );
    reg_val |= 0x00F0; // RED_SS_CFG_RDCHx_FLD_QUE_FLUSH
    mac_write( asic_id, ss, reg_val );

    /* delay for MAX_MTU*1 core clks */
//    usleep( 9200 ); // FIXME
    mac_flush_pio( asic_id );
    mac_flush_pio( asic_id );
    mac_flush_pio( asic_id );

    /* SW reset the whole port logic (but NOT the 10b/20b converter so the reset can be 
     * clocked in) 
     */
    mac_read(  asic_id, BA(RED_MISC_SW_RST), &reg_val );
    reg_val |= ( BIT( mac_id )); // logical HIF number 
    mac_write( asic_id, BA(RED_MISC_SW_RST), reg_val );

    /* wait some time for clk to arrive at the MAC */
//    usleep(300);
    mac_flush_pio( asic_id );
    mac_flush_pio( asic_id );
    mac_flush_pio( asic_id );
    
    /* reset the 10b/20b converter */
    mac_write( asic_id, BA(RED_HI0_SW_RST) + ofs, ~0 );
//    mac_read(  asic_id, BA(RED_HI0_SW_RST) + ofs, &reg_val ); // flush the write
    mac_flush_pio( asic_id );

    /* bring down the serdes */
    serdes_disable( asic_id, mac_id, 0, 4 );

    return 0;   
}

/***************************************************************
* mac_bring_down_hif_1g_hw_seq
*
* Bring down the specified MAC/PHY.
*
* Note: This sequence is translated from RTL in:
*
* asic/src/redwood/verif/top/tests/sce_pk_fw.h
*
* Sequence is borken into individually executable steps so
* the operation can be parallelized over all HIFs by
* executing the steps one at a time with the delays shared
* by all ports.
**************************************************************/
int mac_bring_down_hif_1g_hw_seq( int asic_id, int mac_id, int steps )
{
    unsigned int reg_val, ofs = MAC_OFFSET( mac_id );

    if (DO_STEP(0, steps)) {

        /* disable first, this sets MAC/SS to flush */
        mac_disable_hif_1g_hw_seq( asic_id, mac_id );
    }

    /*****************************************************
    *  "drain any ingress packet (up to max frame size)."
    *
    * The I2C accesses take about 400us each. This should be plenty of time.
    * If these speed up significantly we may need to add a real delay here.
    *****************************************************/

    if (DO_STEP(1, steps)) {

        /* SW reset the whole port logic (but NOT the 10b/20b converter so the reset can be
         * clocked in)
         */
        mac_read(  asic_id, BA(RED_MISC_SW_RST), &reg_val );
        reg_val |= ( BIT( mac_id )); // logical HIF number
        mac_write( asic_id, BA(RED_MISC_SW_RST), reg_val );
        mac_flush_pio( asic_id );
    }

    /********************************************************
    *  "wait for some time for clocks to arrive to the mac"
    *
    * The I2C accesses take about 400us each. This should be plenty of time.
    * If these speed up significantly we may need to add a real delay here.
    ********************************************************/

    if (DO_STEP(2, steps)) {

        /* reset the 10b/20b converter */
        mac_write( asic_id, BA(RED_HI0_SW_RST) + ofs, ~0 );
        mac_flush_pio( asic_id );
    }

    return 0;
}

/***************************************************************
* mac_disable_hif_hw_seq
*
* Disable the specified MAC/PHY.
**************************************************************/
int mac_disable_hif_hw_seq( int asic_id, int mac_id )
{
    unsigned int reg_val, ofs = MAC_OFFSET( mac_id );

    /* disable Rx */
    mac_read( asic_id, BA(RED_HI0_MAC_CFG) + ofs, &reg_val );
    reg_val &= ~( BIT(RED_HI_MAC_CFG_00_FLD_ENABLE_RX) );
    mac_write( asic_id, BA(RED_HI0_MAC_CFG) + ofs, reg_val );

    /* enable tx flush */
    mac_read(  asic_id, BA(RED_HI0_MAC_CFG_EXT) + ofs, &reg_val );
    reg_val |= BIT(RED_HI_MAC_CFG_EXT_FLD_TX_FLUSH);
    mac_write( asic_id, BA(RED_HI0_MAC_CFG_EXT) + ofs, reg_val );

    /* Set the associated SS block to read-port-flush */
    mac_assert_flush_hif_ss( asic_id, mac_id );

    mac_flush_pio( asic_id );

    return 0;
}

/***************************************************************
* mac_disable_hif_1g_hw_seq
*
* Enable the specified MAC/PHY
**************************************************************/
int mac_disable_hif_1g_hw_seq( int asic_id, int mac_id /*0..7*/ )
{
    unsigned int reg_val, ofs = MAC_OFFSET( mac_id );

    /* Set rx flush */
    mac_read( asic_id,  BA(RED_HI0_GBE_CFG_RX) + ofs, &reg_val );
    reg_val |= bits(21,21,/*RED_HI_GBE_CFG_RX_FLD_FLUSH_EN*/ 1);
    mac_write( asic_id, BA(RED_HI0_GBE_CFG_RX) + ofs, reg_val );

    /* Set tx flush */
    mac_write( asic_id, BA(RED_HI0_GBE_CFG_TX_FLUSH_EN) + ofs, 1 );

    /* Set the associated SS block to read-port-flush */
    mac_assert_flush_hif_ss( asic_id, mac_id );

    return 0;
}

/***************************************************************
* mac_bring_down_nif_hw_seq
*
* Bring down the specified MAC/PHY.
*
* Note: This sequence is translated from RTL in:
* 
* asic/src/redwood/verif/top/tests/sce_pk_fw.h
*
**************************************************************/
int mac_bring_down_nif_hw_seq( int asic_id, int mac_id )
{
    unsigned int reg_val, ofs = MAC_OFFSET( mac_id );
    unsigned int ss1, ss2;

    /* disable Rx */
    mac_read(  asic_id, BA(RED_NI0_MAC_CFG) + ofs, &reg_val );
    reg_val &= ~( BIT(RED_NI_MAC_CFG_00_FLD_ENABLE_RX) );
    mac_write( asic_id, BA(RED_NI0_MAC_CFG) + ofs, reg_val );
    
    /* enable tx flush */
    mac_read(  asic_id, BA(RED_NI0_MAC_CFG_EXT) + ofs, &reg_val );
    reg_val |= BIT(RED_NI_MAC_CFG_EXT_FLD_TX_FLUSH);
    mac_write( asic_id, BA(RED_NI0_MAC_CFG_EXT) + ofs, reg_val );

    /* Set the two associated SS blocks to read-port-flush */
    ss1 = BA(ss_rdch_to_flush[ mac_id ]) + 0x20000; //ss2
    ss2 = BA(ss_rdch_to_flush[ mac_id ]) + 0x30000; //ss3

    mac_read(  asic_id, ss1, &reg_val );
    reg_val |= 0x00F0; // RED_SS_CFG_RDCHx_FLD_QUE_FLUSH
    mac_write( asic_id, ss1, reg_val );

    mac_read(  asic_id, ss2, &reg_val );
    reg_val |= 0x00F0; // RED_SS_CFG_RDCHx_FLD_QUE_FLUSH
    mac_write( asic_id, ss2, reg_val );

    /* delay for MAX_MTU*1 core clks */
//    usleep( 9200 ); // FIXME
    mac_flush_pio( asic_id );
    mac_flush_pio( asic_id );
    mac_flush_pio( asic_id );

    /* SW reset the whole port logic (but NOT the 10b/20b converter so the reset can be 
     * clocked in) 
     * */
    mac_read(  asic_id, BA(RED_MISC_SW_RST), &reg_val );
    reg_val |= ( BIT( (8+mac_id) )); // logical NIF number + 8 HIFs
    mac_write( asic_id, BA(RED_MISC_SW_RST), reg_val );
//    mac_read(  asic_id, BA(RED_MISC_SW_RST), &reg_val ); // flush the write

    /* wait some time for clk to arrive at the MAC */
//    usleep(300);
    mac_flush_pio( asic_id );
    mac_flush_pio( asic_id );
    mac_flush_pio( asic_id );
    
    /* reset the 10b/20b converter */
    mac_write( asic_id, BA(RED_NI0_SW_RST) + ofs, ~0 );
//    mac_read(  asic_id, BA(RED_NI0_SW_RST) + ofs, &reg_val ); // flush the write
    mac_flush_pio( asic_id );

    /* bring down the serdes */
    serdes_disable( asic_id, 8+mac_id, 0, 4 );

    return 0;   
}

/***************************************************************
* mac_disable_nif_hw_seq
*
* Disable the specified MAC/PHY.
**************************************************************/
int mac_disable_nif_hw_seq( int asic_id, int mac_id )
{
    unsigned int reg_val, ofs = MAC_OFFSET( mac_id );
    unsigned int ss1, ss2;

    /* disable Rx */
    mac_read(  asic_id, BA(RED_NI0_MAC_CFG) + ofs, &reg_val );
    reg_val &= ~( BIT(RED_NI_MAC_CFG_00_FLD_ENABLE_RX) );
    mac_write( asic_id, BA(RED_NI0_MAC_CFG) + ofs, reg_val );

    /* enable tx flush */
    mac_read(  asic_id, BA(RED_NI0_MAC_CFG_EXT) + ofs, &reg_val );
    reg_val |= BIT(RED_NI_MAC_CFG_EXT_FLD_TX_FLUSH);
    mac_write( asic_id, BA(RED_NI0_MAC_CFG_EXT) + ofs, reg_val );

    /* Set the two associated SS blocks to read-port-flush */
    ss1 = BA(ss_rdch_to_flush[ mac_id ]) + 0x20000; //ss2
    ss2 = BA(ss_rdch_to_flush[ mac_id ]) + 0x30000; //ss3

    mac_read(  asic_id, ss1, &reg_val );
    reg_val |= 0x00F0; // RED_SS_CFG_RDCHx_FLD_QUE_FLUSH
    mac_write( asic_id, ss1, reg_val );

    mac_read(  asic_id, ss2, &reg_val );
    reg_val |= 0x00F0; // RED_SS_CFG_RDCHx_FLD_QUE_FLUSH
    mac_write( asic_id, ss2, reg_val );

    return 0;
}

unsigned int nif_ignore_signal_strength_tbl[] = {
    RED_NI0_IGNORE_RX_SIG_STRENGTH,
    RED_NI1_IGNORE_RX_SIG_STRENGTH,
    RED_NI2_IGNORE_RX_SIG_STRENGTH,
    RED_NI3_IGNORE_RX_SIG_STRENGTH
};

unsigned int nif_mac_cfg_reg[] = {
    RED_NI0_MAC_CFG,
    RED_NI1_MAC_CFG,
    RED_NI2_MAC_CFG,
    RED_NI3_MAC_CFG
};

void mac_ignore_signal_strength_nif( int asic_id, int mac_id /*0..3*/ )
{
    mac_write( asic_id,
               BA(nif_ignore_signal_strength_tbl[ mac_id ]),
               ((1 << RED_NI_IGNORE_RX_SIG_STRENGTH_FLD_L0) |
                (1 << RED_NI_IGNORE_RX_SIG_STRENGTH_FLD_L1) |
                (1 << RED_NI_IGNORE_RX_SIG_STRENGTH_FLD_L2) |
                (1 << RED_NI_IGNORE_RX_SIG_STRENGTH_FLD_L3)) );
}


unsigned int hif_ignore_signal_strength_tbl[] = {
    RED_HI0_IGNORE_RX_SIG_STRENGTH,
    RED_HI1_IGNORE_RX_SIG_STRENGTH,
    RED_HI2_IGNORE_RX_SIG_STRENGTH,
    RED_HI3_IGNORE_RX_SIG_STRENGTH,
    RED_HI4_IGNORE_RX_SIG_STRENGTH,
    RED_HI5_IGNORE_RX_SIG_STRENGTH,
    RED_HI6_IGNORE_RX_SIG_STRENGTH,
    RED_HI7_IGNORE_RX_SIG_STRENGTH
};

void mac_ignore_signal_strength_hif( int asic_id, int mac_id /*0..7*/ )
{
    mac_write( asic_id,
               BA(hif_ignore_signal_strength_tbl[ mac_id ]),
               ((1 << RED_HI_IGNORE_RX_SIG_STRENGTH_FLD_L0) |
                (1 << RED_HI_IGNORE_RX_SIG_STRENGTH_FLD_L1) |
                (1 << RED_HI_IGNORE_RX_SIG_STRENGTH_FLD_L2) |
                (1 << RED_HI_IGNORE_RX_SIG_STRENGTH_FLD_L3)) );
}

int mac_loopback_nif( int asic_id, int mac_id /*0..3*/ )
{
    unsigned int reg_val;

    mac_write( asic_id,
               BA(nif_ignore_signal_strength_tbl[ mac_id ]),
               ((1 << RED_NI_IGNORE_RX_SIG_STRENGTH_FLD_L0) |
                (1 << RED_NI_IGNORE_RX_SIG_STRENGTH_FLD_L1) |
                (1 << RED_NI_IGNORE_RX_SIG_STRENGTH_FLD_L2) |
                (1 << RED_NI_IGNORE_RX_SIG_STRENGTH_FLD_L3)) );

    /* straigthen out the lane mapping */
    mac_read( asic_id, BA(nif_mac_cfg_reg[mac_id]), &reg_val );
    reg_val &= 0x01ffffff;
    reg_val |= bits(31,25,0x64);
    mac_write( asic_id, BA(nif_mac_cfg_reg[mac_id]), reg_val );
    mac_write( asic_id, BA(nif_mac_cfg_reg[mac_id]) + 4, 1 /*upper bit of lane 3*/ );
    mac_read(  asic_id, BA(nif_mac_cfg_reg[mac_id]) + 8, &reg_val );
    reg_val &= 0xffffff00;
    reg_val |= 0xe4;
    mac_write(  asic_id, BA(nif_mac_cfg_reg[mac_id]) + 8, reg_val );

    /* un-do any polarity swapping that's already been programmed */
    serdes_normal_polarity_on_tx_lane( asic_id, 8+mac_id, 0 );
    serdes_normal_polarity_on_tx_lane( asic_id, 8+mac_id, 1 );
    serdes_normal_polarity_on_tx_lane( asic_id, 8+mac_id, 2 );
    serdes_normal_polarity_on_tx_lane( asic_id, 8+mac_id, 3 );

    /* un-do any polarity swapping that's already been programmed */
    serdes_normal_polarity_on_rx_lane( asic_id, 8+mac_id, 0 );
    serdes_normal_polarity_on_rx_lane( asic_id, 8+mac_id, 1 );
    serdes_normal_polarity_on_rx_lane( asic_id, 8+mac_id, 2 );
    serdes_normal_polarity_on_rx_lane( asic_id, 8+mac_id, 3 );

    mac_pgm_serdes( asic_id, 8+mac_id, 0, 4, 0,0 /*no polarity swapping*/ );
    serdes_near_loopback( asic_id, 8+mac_id, 0, 4 );

    return 0;
}

unsigned int hif_mac_cfg_reg[] = {
    RED_HI0_MAC_CFG,
    RED_HI1_MAC_CFG,
    RED_HI2_MAC_CFG,
    RED_HI3_MAC_CFG,
    RED_HI4_MAC_CFG,
    RED_HI5_MAC_CFG,
    RED_HI6_MAC_CFG,
    RED_HI7_MAC_CFG
};

int mac_loopback_hif( int asic_id, int mac_id /*0..7*/ )
{
    unsigned int reg_val;

    mac_write( asic_id,
               BA(hif_ignore_signal_strength_tbl[ mac_id ]),
               ((1 << RED_HI_IGNORE_RX_SIG_STRENGTH_FLD_L0) |
                (1 << RED_HI_IGNORE_RX_SIG_STRENGTH_FLD_L1) |
                (1 << RED_HI_IGNORE_RX_SIG_STRENGTH_FLD_L2) |
                (1 << RED_HI_IGNORE_RX_SIG_STRENGTH_FLD_L3)) );

    /* straigthen out the lane mapping */
    mac_read( asic_id, BA(hif_mac_cfg_reg[mac_id]), &reg_val );
    reg_val &= 0x01ffffff;
    reg_val |= bits(31,25,0x64);
    mac_write( asic_id, BA(hif_mac_cfg_reg[mac_id]), reg_val );
    mac_write( asic_id, BA(hif_mac_cfg_reg[mac_id]) + 4, 1 /*upper bit of lane 3*/ );
    mac_read(  asic_id, BA(hif_mac_cfg_reg[mac_id]) + 8, &reg_val );
    reg_val &= 0xffffff00;
    reg_val |= 0xe4;
    mac_write(  asic_id, BA(hif_mac_cfg_reg[mac_id]) + 8, reg_val );

    /* un-do any polarity swapping that's already been programmed */
    serdes_normal_polarity_on_tx_lane( asic_id, mac_id, 0 );
    serdes_normal_polarity_on_tx_lane( asic_id, mac_id, 1 );
    serdes_normal_polarity_on_tx_lane( asic_id, mac_id, 2 );
    serdes_normal_polarity_on_tx_lane( asic_id, mac_id, 3 );

    /* un-do any polarity swapping that's already been programmed */
    serdes_normal_polarity_on_rx_lane( asic_id, mac_id, 0 );
    serdes_normal_polarity_on_rx_lane( asic_id, mac_id, 1 );
    serdes_normal_polarity_on_rx_lane( asic_id, mac_id, 2 );
    serdes_normal_polarity_on_rx_lane( asic_id, mac_id, 3 );

    mac_pgm_serdes( asic_id, mac_id, 0, 4, 0,0 /*no polarity swapping*/ );
    serdes_near_loopback( asic_id, mac_id, 0, 4 );

    return 0;
}

/**********************************************************************
* fwd ranges are consecutive 32-bit addresses from base as follows:
*
*   Bits:   0 - 7,  8,   9    10
*           HI0-7, BIF, CIF, Drop
*/
void invalidate_fwd_ranges( int asic_id, unsigned int base )
{
    int i;

    for (i = 0; i < 11; i++) {
        mac_write( asic_id, base + (4*i), 0x3fff3fff );
    }
}

unsigned int hif_fwd_vector_for_lpbk[] = {
    RED_HI0_FWD_VECTOR0, RED_HI1_FWD_VECTOR0, RED_HI2_FWD_VECTOR0, RED_HI3_FWD_VECTOR0,
    RED_HI4_FWD_VECTOR0, RED_HI5_FWD_VECTOR0, RED_HI6_FWD_VECTOR0, RED_HI7_FWD_VECTOR0
};

unsigned int hif_vif_min_for_lpbk[] = {
    RED_HI0_RX_VIF_MIN, RED_HI1_RX_VIF_MIN, RED_HI2_RX_VIF_MIN, RED_HI3_RX_VIF_MIN,
    RED_HI4_RX_VIF_MIN, RED_HI5_RX_VIF_MIN, RED_HI6_RX_VIF_MIN, RED_HI7_RX_VIF_MIN
};

void mac_setup_lpbk_forwarding_for_port( int asic_id, int asic_port )
{
    if (port_is_hif( asic_port )) {
        unsigned int fwd_reg = BA(hif_fwd_vector_for_lpbk[ asic_port - 2 ]);
        unsigned int vif_reg = BA(hif_vif_min_for_lpbk[    asic_port - 2 ]);

        mac_write( asic_id, fwd_reg, 0x77777774 );
        mac_write( asic_id, vif_reg, 0 );
    }
}

void mac_teardown_lpbk_forwarding_for_port( int asic_id, int asic_port )
{
    if (port_is_hif( asic_port )) {
        unsigned int fwd_reg = BA(hif_fwd_vector_for_lpbk[ asic_port - 2 ]);
        unsigned int vif_reg = BA(hif_vif_min_for_lpbk[    asic_port - 2 ]);

        mac_write( asic_id, fwd_reg, 0x77777777 );
        mac_write( asic_id, vif_reg, 0xfff );
    }
}

void mac_setup_lpbk_forwarding( int asic_id )
{
     mac_write( asic_id, BA(RED_NI0_NON_FCOE_RX_DEFAULT_VNTAG), (1 << 9) );
     mac_write( asic_id, BA(RED_NI1_NON_FCOE_RX_DEFAULT_VNTAG), (1 << 9) );
     mac_write( asic_id, BA(RED_NI2_NON_FCOE_RX_DEFAULT_VNTAG), (1 << 9) );
     mac_write( asic_id, BA(RED_NI3_NON_FCOE_RX_DEFAULT_VNTAG), (1 << 9) );

    /* configure NIx fwd ranges so as not to conflict with default vntag */
    invalidate_fwd_ranges( asic_id, BA(RED_NI0_HIF0_FWD_RANGE) );
    invalidate_fwd_ranges( asic_id, BA(RED_NI1_HIF0_FWD_RANGE) );
    invalidate_fwd_ranges( asic_id, BA(RED_NI2_HIF0_FWD_RANGE) );
    invalidate_fwd_ranges( asic_id, BA(RED_NI3_HIF0_FWD_RANGE) );

    /* program the NIF fwd ranges explicitly FOR default vntag (0) */
     mac_write( asic_id, BA(RED_NI0_CIF_FWD_RANGE), 0x3fff0000 );
     mac_write( asic_id, BA(RED_NI1_CIF_FWD_RANGE), 0x3fff0000 ); //0x10000
     mac_write( asic_id, BA(RED_NI2_CIF_FWD_RANGE), 0x3fff0000 ); //0x10000
     mac_write( asic_id, BA(RED_NI3_CIF_FWD_RANGE), 0x3fff0000 ); //0x10000

     mac_write( asic_id, BA(RED_HI0_FWD_VECTOR0), 0x77777774 );
     mac_write( asic_id, BA(RED_HI1_FWD_VECTOR0), 0x77777774 );
     mac_write( asic_id, BA(RED_HI2_FWD_VECTOR0), 0x77777774 );
     mac_write( asic_id, BA(RED_HI3_FWD_VECTOR0), 0x77777774 );
     mac_write( asic_id, BA(RED_HI4_FWD_VECTOR0), 0x77777774 );
     mac_write( asic_id, BA(RED_HI5_FWD_VECTOR0), 0x77777774 );
     mac_write( asic_id, BA(RED_HI6_FWD_VECTOR0), 0x77777774 );
     mac_write( asic_id, BA(RED_HI7_FWD_VECTOR0), 0x77777774 );
}

void mac_teardown_lpbk_forwarding( int asic_id )
{
     mac_write( asic_id, BA(RED_NI0_NON_FCOE_RX_DEFAULT_VNTAG), 0 );
     mac_write( asic_id, BA(RED_NI1_NON_FCOE_RX_DEFAULT_VNTAG), 0 );
     mac_write( asic_id, BA(RED_NI2_NON_FCOE_RX_DEFAULT_VNTAG), 0 );
     mac_write( asic_id, BA(RED_NI3_NON_FCOE_RX_DEFAULT_VNTAG), 0 );

    /* configure NIx fwd ranges so as not to conflict with default vntag */
    invalidate_fwd_ranges( asic_id, BA(RED_NI0_HIF0_FWD_RANGE) );
    invalidate_fwd_ranges( asic_id, BA(RED_NI1_HIF0_FWD_RANGE) );
    invalidate_fwd_ranges( asic_id, BA(RED_NI2_HIF0_FWD_RANGE) );
    invalidate_fwd_ranges( asic_id, BA(RED_NI3_HIF0_FWD_RANGE) );

     mac_write( asic_id, BA(RED_HI0_FWD_VECTOR0), 0x77777777 );
     mac_write( asic_id, BA(RED_HI1_FWD_VECTOR0), 0x77777777 );
     mac_write( asic_id, BA(RED_HI2_FWD_VECTOR0), 0x77777777 );
     mac_write( asic_id, BA(RED_HI3_FWD_VECTOR0), 0x77777777 );
     mac_write( asic_id, BA(RED_HI4_FWD_VECTOR0), 0x77777777 );
     mac_write( asic_id, BA(RED_HI5_FWD_VECTOR0), 0x77777777 );
     mac_write( asic_id, BA(RED_HI6_FWD_VECTOR0), 0x77777777 );
     mac_write( asic_id, BA(RED_HI7_FWD_VECTOR0), 0x77777777 );
}


void mac_setup_snake_test(int asic_id, int flush)
{
    /* program the MAC forwarding entries to set up the following
    *  sequence for simple snake test (no flush testing):
    *   CIF -> NI0
    *   NI0 -> HI0
    *   HI0 -> NI1
    *   NI1 -> HI1
    *   HI1 -> NI2
    *   NI2 -> HI2
    *   HI2 -> NI3
    *   NI3 -> HI3
    *   HI3 -> CIF
    *
    * program the MAC forwarding entries to set up the following
    *  sequence for flush testing:
    *   CIF -> NI0
    *   NI0 -> HI0
    *   HI0 -> NI1
    *   NI1 -> HI1
    *   HI1 -> NI2
    *   NI2 -> HI2
    *   HI2 -> NI3
    *   NI3 -> HI3
    *   HI3 -> NI0 <- causes a forwarding loop (pkt is now "trapped forever")
    */
     mac_write( asic_id, BA(RED_NI0_NON_FCOE_RX_DEFAULT_VNTAG), (1 << 0) );
     mac_write( asic_id, BA(RED_NI1_NON_FCOE_RX_DEFAULT_VNTAG), (1 << 1) );
     mac_write( asic_id, BA(RED_NI2_NON_FCOE_RX_DEFAULT_VNTAG), (1 << 2) );
     mac_write( asic_id, BA(RED_NI3_NON_FCOE_RX_DEFAULT_VNTAG), (1 << 3) );

    /* configure NIx fwd ranges so as not to conflict with default vntag */
    invalidate_fwd_ranges( asic_id, BA(RED_NI0_HIF0_FWD_RANGE) );
    invalidate_fwd_ranges( asic_id, BA(RED_NI1_HIF0_FWD_RANGE) );
    invalidate_fwd_ranges( asic_id, BA(RED_NI2_HIF0_FWD_RANGE) );
    invalidate_fwd_ranges( asic_id, BA(RED_NI3_HIF0_FWD_RANGE) );

    /* program the NIF fwd ranges explicitly FOR default vntag (0) */
     mac_write( asic_id, BA(RED_NI0_HIF0_FWD_RANGE), 0x10000 );
     mac_write( asic_id, BA(RED_NI1_HIF1_FWD_RANGE), 0x10000 ); //0x10000
     mac_write( asic_id, BA(RED_NI2_HIF2_FWD_RANGE), 0x10000 ); //0x10000
     mac_write( asic_id, BA(RED_NI3_HIF3_FWD_RANGE), 0x10000 ); //0x10000

     mac_write( asic_id, BA(RED_HI0_FWD_VECTOR0), 1 );
     mac_write( asic_id, BA(RED_HI1_FWD_VECTOR0), 2 );
     mac_write( asic_id, BA(RED_HI2_FWD_VECTOR0), 3 );
     if (!flush)
        mac_write( asic_id, BA(RED_HI3_FWD_VECTOR0), 4 /*CI0*/ );
     else
        mac_write( asic_id, BA(RED_HI3_FWD_VECTOR0), 0 /*NI0*/);

    /* set serdes in "near loopback" mode */

     mac_loopback_nif( asic_id, 0 );
     mac_loopback_nif( asic_id, 1 );
     mac_loopback_nif( asic_id, 2 );
     mac_loopback_nif( asic_id, 3 );

     mac_loopback_hif( asic_id, 0 );
     mac_loopback_hif( asic_id, 1 );
     mac_loopback_hif( asic_id, 2 );
     mac_loopback_hif( asic_id, 3 );

}

void mac_setup_ixia_snake_test(int asic_id)
{
    /* program the MAC forwarding entries to set up the following
    *  sequence for simple snake test (no flush testing):
    *   IXIA-> NI0
    *   NI0 -> HI0
    *   HI0 -> NI1
    *   NI1 -> HI1
    *   HI1 -> NI2
    *   NI2 -> HI2
    *   HI2 -> NI3
    *   NI3 -> HI3
    *   HI3 -> NI0 -> IXIA
    */
     mac_write( asic_id, BA(RED_NI0_NON_FCOE_RX_DEFAULT_VNTAG), (1 << 0) );
     mac_write( asic_id, BA(RED_NI1_NON_FCOE_RX_DEFAULT_VNTAG), (1 << 1) );
     mac_write( asic_id, BA(RED_NI2_NON_FCOE_RX_DEFAULT_VNTAG), (1 << 2) );
     mac_write( asic_id, BA(RED_NI3_NON_FCOE_RX_DEFAULT_VNTAG), (1 << 3) );

    /* configure NIx fwd ranges so as not to conflict with default vntag */
    invalidate_fwd_ranges( asic_id, BA(RED_NI0_HIF0_FWD_RANGE) );
    invalidate_fwd_ranges( asic_id, BA(RED_NI1_HIF0_FWD_RANGE) );
    invalidate_fwd_ranges( asic_id, BA(RED_NI2_HIF0_FWD_RANGE) );
    invalidate_fwd_ranges( asic_id, BA(RED_NI3_HIF0_FWD_RANGE) );

    /* program the NIF fwd ranges explicitly FOR default vntag (0) */
     mac_write( asic_id, BA(RED_NI0_HIF0_FWD_RANGE), 0x10000 );
     mac_write( asic_id, BA(RED_NI1_HIF1_FWD_RANGE), 0x10000 ); //0x10000
     mac_write( asic_id, BA(RED_NI2_HIF2_FWD_RANGE), 0x10000 ); //0x10000
     mac_write( asic_id, BA(RED_NI3_HIF3_FWD_RANGE), 0x10000 ); //0x10000

     mac_write( asic_id, BA(RED_HI0_FWD_VECTOR0), 1 );
     mac_write( asic_id, BA(RED_HI1_FWD_VECTOR0), 2 );
     mac_write( asic_id, BA(RED_HI2_FWD_VECTOR0), 3 );
     mac_write( asic_id, BA(RED_HI3_FWD_VECTOR0), 0 /*NI0*/);

     mac_write( asic_id, BA(RED_HI0_RX_VIF_MIN), 0 );
     mac_write( asic_id, BA(RED_HI1_RX_VIF_MIN), 0 );
     mac_write( asic_id, BA(RED_HI2_RX_VIF_MIN), 0 );
     mac_write( asic_id, BA(RED_HI3_RX_VIF_MIN), 0 );

    /* set serdes in "near loopback" mode */
     mac_loopback_nif( asic_id, 1 );
     mac_loopback_nif( asic_id, 2 );
     mac_loopback_nif( asic_id, 3 );

     mac_loopback_hif( asic_id, 0 );
     mac_loopback_hif( asic_id, 1 );
     mac_loopback_hif( asic_id, 2 );
     mac_loopback_hif( asic_id, 3 );
}



/***************************************************************
* mac_nif_hw_seq
*
* Bring up the specified MAC/PHY
*
* Note: This sequence is translated from RTL in:
*
* asic/src/redwood/verif/top/tests/sce_pk_fw.h
*
* It adds the MAC configuration parameters that are used in 
* SCE_Config.h::cfgMACFor10G
*
**************************************************************/
int mac_nif_hw_seq( int asic_id, 
                    int mac_id /*0..7*/,
                    mac_cfg_t *p_cfg, 
                    int rx_polarity, int tx_polarity, int flag )
{
    unsigned int ofs = MAC_OFFSET( mac_id );
    int err;

    /* Program the serdes first to get a clk to the MAC */
    if (!flag) {
    mac_pgm_serdes( asic_id, 8+mac_id, 0, 4, rx_polarity, tx_polarity );
    } else {
    /* Assumption is the MAC is in reset at this point. This function
    * fills in the "extra" MAC config info tht is found in
    * SCE_Config.h */
    if ((err = mac_config( asic_id, p_cfg, ofs,
                           BA(RED_NI0_CFG_CODE_PAUSE),
                           BA(RED_NI0_MAC_FRM_LENGTH_MAX),
                           BA(RED_NI0_MAC_EXT_LPBK),
                           BA(RED_NI0_PKT_LPBK),
                           BA(RED_NI0_MAC_CFG),
                           BA(RED_NI0_MAC_CFG_EXT) )) != 0)  return err;
    }
    return 0;
}
