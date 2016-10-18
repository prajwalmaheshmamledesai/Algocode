/********************************************************************
*
*      File:   redwood_mac_hw_seq.h
*      Name:   Bruce McLoughlin
*
*      Description: Stripped down MAC programming sequences
*      that can be shared between USD and Diags
*
*
*      Copyright (c) 2001,2002,2003,2004 by Nuova Systems, Inc.
********************************************************************/
#ifndef MAC_HW_SEQ_H_INCLUDED
#define MAC_HW_SEQ_H_INCLUDED

typedef enum {

    mod_mtu = (1 << 0),
    mod_pause_sub_code = (1 << 1),
    mod_rx_lane_map    = (1 << 2),
    mod_tx_lane_map    = (1 << 3),
    mod_ext_loopback   = (1 << 4),
    mod_pkt_loopback   = (1 << 5),
    mod_ipg            = (1 << 6),
    mod_pad_en         = (1 << 7),
    mod_cmd_frm_en     = (1 << 8),
    mod_rx_scramble_en = (1 << 9),
    mod_tx_scramble_en = (1 << 10),
    mod_all            = (0xFFFFFFFF)

} mac_cfg_modbits_t;
 
typedef struct mac_cfg_t {

    mac_cfg_modbits_t modbits;   // one bit per field (below) or ALL
    int          mtu;            // (9200?)
    unsigned int pause_sub_code; // (0xdc -- default)
    unsigned int rx_lane_map;    // 0xe4
    unsigned int tx_lane_map;    // 0xe4
    int          ext_loopback;   // (serdes) (0)
    int          pkt_loopback;   // (mac) (0)
    int          ipg;            // (0xc -- default)
    int          pad_en;         // (0)
    int          cmd_frm_en;     // (1)
    int          rx_scramble_en; // (0)
    int          tx_scramble_en; // (0)

} mac_cfg_t;

int mac_bring_up_hif_hw_seq( int asic_id, int mac_id /*0..7*/, mac_cfg_t *p_cfg,
                             int rx_polarity, int tx_polarity );
int mac_bring_up_nif_hw_seq( int asic_id, int mac_id /*0..7*/, mac_cfg_t *p_cfg,
                             int rx_polarity, int tx_polarity );
int mac_bring_down_hif_hw_seq( int asic_id, int mac_id );
int mac_bring_down_nif_hw_seq( int asic_id, int mac_id );
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
* returns: 0 == OK
*          else, error code
**************************************************************/
int mac_bring_up_cif_hw_seq( int asic_id, int mode );

/***************************************************************
* mac_bring_down_cif_hw_seq
***************************************************************/
int mac_bring_down_cif_hw_seq( int asic_id );

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
int mac_bring_up_bif_hw_seq( int asic_id, int mode );

/***************************************************************
* mac_bring_down_bif_hw_seq
***************************************************************/
int mac_bring_down_bif_hw_seq( int asic_id );

void mac_setup_snake_test(int asic_id, int flush);
void mac_setup_ixia_snake_test(int asic_id);

int mac_nif_hw_seq( int asic_id, int mac_id /*0..7*/, mac_cfg_t *p_cfg,
                             int rx_polarity, int tx_polarity , int flag);
extern int mac_bring_up_hif_1g_hw_seq( int asic_id, int mac_id /*0..7*/,
                mac_cfg_t *p_cfg, int rx_polarity, int tx_polarity, int steps );

#endif /*MAC_HW_SEQ_H_INCLUDED*/

