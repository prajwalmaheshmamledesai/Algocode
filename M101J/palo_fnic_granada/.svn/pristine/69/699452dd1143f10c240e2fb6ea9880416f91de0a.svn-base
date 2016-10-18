/********************************************************************
*
*      File:   redwood_mac_hw_qod_defaults.h
*      Name:   Richard Tang
*
*      Description: Stripped down QoS MAC programming defaults
*      that can be shared between USD and Diags.
*
*      The programming can reference
*          platform/nuova/satellite/qosctrl/qosctrl_redwd_primitives.c
*      for details
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

#define NON_FCOE_OR_FCOE  2
#define QC_MAX_PORTS_PERTYPE 8

const uint32_t
qc_mac_hw_default_rx_cos[NON_FCOE_OR_FCOE][SATCFG_PORT_TYPE_MAX][QC_MAX_PORTS_PERTYPE] = {
    { // non_fcoe
        [SATCFG_PORT_TYPE_CIF] = {0},

        [SATCFG_PORT_TYPE_BIF] = {0},

        [SATCFG_PORT_TYPE_NIF] = {0,
                                  0,
                                  0,
                                  0},

        [SATCFG_PORT_TYPE_HIF] = {0,
                                  0,
                                  0,
                                  0,
                                  0,
                                  0,
                                  0,
                                  0},
    },

    { // fcoe
        [SATCFG_PORT_TYPE_CIF] = {0},

        [SATCFG_PORT_TYPE_BIF] = {0},

        [SATCFG_PORT_TYPE_NIF] = {0,
                                  0,
                                  0,
                                  0},

        [SATCFG_PORT_TYPE_HIF] = {0,
                                  0,
                                  0,
                                  0,
                                  0,
                                  0,
                                  0,
                                  0},
    },    
};


const uint32_t
qc_mac_hw_default_tx_cos[NON_FCOE_OR_FCOE][SATCFG_PORT_TYPE_MAX][QC_MAX_PORTS_PERTYPE] = {
    { // non_fcoe
        [SATCFG_PORT_TYPE_CIF] = {0},

        [SATCFG_PORT_TYPE_BIF] = {0},

        [SATCFG_PORT_TYPE_NIF] = {0,
                                  0,
                                  0,
                                  0},

        [SATCFG_PORT_TYPE_HIF] = {0,
                                  0,
                                  0,
                                  0,
                                  0,
                                  0,
                                  0,
                                  0},
    },

    { // fcoe
        [SATCFG_PORT_TYPE_CIF] = {0},

        [SATCFG_PORT_TYPE_BIF] = {0},

        [SATCFG_PORT_TYPE_NIF] = {0,
                                  0,
                                  0,
                                  0},

        [SATCFG_PORT_TYPE_HIF] = {0,
                                  0,
                                  0,
                                  0,
                                  0,
                                  0,
                                  0,
                                  0},
    },    
};


const uint32_t // COS maps are 64 bit, thus *2 for Hi and Lo
qc_mac_hw_rx_cos_map[NON_FCOE_OR_FCOE][SATCFG_PORT_TYPE_MAX][QC_MAX_PORTS_PERTYPE*2] = {
    { // non_fcoe
        [SATCFG_PORT_TYPE_CIF] = {0x76543210,
                                  0xfedcba98},

        [SATCFG_PORT_TYPE_BIF] = {0x76543210,
                                  0xfedcba98},

        [SATCFG_PORT_TYPE_NIF] = {0, 0, 0, 0,
                                  0, 0, 0, 0}, // Invalid

        [SATCFG_PORT_TYPE_HIF] = {0x76543210,
                                  0xfedcba98,
                                  0x76543210,
                                  0xfedcba98,
                                  0x76543210,
                                  0xfedcba98,
                                  0x76543210,
                                  0xfedcba98,
                                  0x76543210,
                                  0xfedcba98,
                                  0x76543210,
                                  0xfedcba98,
                                  0x76543210,
                                  0xfedcba98,
                                  0x76543210,
                                  0xfedcba98,},
    },

    { // fcoe
        [SATCFG_PORT_TYPE_CIF] = {0x76543210,
                                  0xfedcba98},

        [SATCFG_PORT_TYPE_BIF] = {0x76543210,
                                  0xfedcba98},

        [SATCFG_PORT_TYPE_NIF] = {0, 0, 0, 0,
                                  0, 0, 0, 0}, // Invalid

        [SATCFG_PORT_TYPE_HIF] = {0x76543210,
                                  0xfedcba98,
                                  0x76543210,
                                  0xfedcba98,
                                  0x76543210,
                                  0xfedcba98,
                                  0x76543210,
                                  0xfedcba98,
                                  0x76543210,
                                  0xfedcba98,
                                  0x76543210,
                                  0xfedcba98,
                                  0x76543210,
                                  0xfedcba98,
                                  0x76543210,
                                  0xfedcba98,},
    },    
};


const uint32_t // COS maps are 64 bit, thus *2 for Hi and Lo
qc_mac_hw_tx_cos_map[NON_FCOE_OR_FCOE][SATCFG_PORT_TYPE_MAX][QC_MAX_PORTS_PERTYPE*2] = {
    { // non_fcoe
        [SATCFG_PORT_TYPE_CIF] = {0x76543210,
                                  0xfedcba98},

        [SATCFG_PORT_TYPE_BIF] = {0x76543210,
                                  0xfedcba98},

        [SATCFG_PORT_TYPE_NIF] = {0, 0, 0, 0,
                                  0, 0, 0, 0}, // Invalid

        [SATCFG_PORT_TYPE_HIF] = {0x76543210,
                                  0xfedcba98,
                                  0x76543210,
                                  0xfedcba98,
                                  0x76543210,
                                  0xfedcba98,
                                  0x76543210,
                                  0xfedcba98,
                                  0x76543210,
                                  0xfedcba98,
                                  0x76543210,
                                  0xfedcba98,
                                  0x76543210,
                                  0xfedcba98,
                                  0x76543210,
                                  0xfedcba98,},
    },

    { // fcoe
        [SATCFG_PORT_TYPE_CIF] = {0x76543210,
                                  0xfedcba98},

        [SATCFG_PORT_TYPE_BIF] = {0x76543210,
                                  0xfedcba98},

        [SATCFG_PORT_TYPE_NIF] = {0, 0, 0, 0,
                                  0, 0, 0, 0}, // Invalid

        [SATCFG_PORT_TYPE_HIF] = {0x76543210,
                                  0xfedcba98,
                                  0x76543210,
                                  0xfedcba98,
                                  0x76543210,
                                  0xfedcba98,
                                  0x76543210,
                                  0xfedcba98,
                                  0x76543210,
                                  0xfedcba98,
                                  0x76543210,
                                  0xfedcba98,
                                  0x76543210,
                                  0xfedcba98,
                                  0x76543210,
                                  0xfedcba98,},
    },    
};

const uint32_t
qc_mac_hw_cos2q_map[NON_FCOE_OR_FCOE][SATCFG_PORT_TYPE_MAX][QC_MAX_PORTS_PERTYPE] = {
    { // non_fcoe
        [SATCFG_PORT_TYPE_CIF] = {0x00000000},

        [SATCFG_PORT_TYPE_BIF] = {0x00000000},

        [SATCFG_PORT_TYPE_NIF] = {0x00000000,
                                  0x00000000,
                                  0x00000000,
                                  0x00000000},

        [SATCFG_PORT_TYPE_HIF] = {0x00000000,
                                  0x00000000,
                                  0x00000000,
                                  0x00000000,
                                  0x00000000,
                                  0x00000000,
                                  0x00000000,
                                  0x00000000},
    },

    { // fcoe
        [SATCFG_PORT_TYPE_CIF] = {0x00000000},

        [SATCFG_PORT_TYPE_BIF] = {0x00000000},

        [SATCFG_PORT_TYPE_NIF] = {0x00000000,
                                  0x00000000,
                                  0x00000000,
                                  0x00000000},

        [SATCFG_PORT_TYPE_HIF] = {0x00000000,
                                  0x00000000,
                                  0x00000000,
                                  0x00000000,
                                  0x00000000,
                                  0x00000000,
                                  0x00000000,
                                  0x00000000},
    },    
};


const uint32_t
qc_mac_hw_flow_control_cos2q_map[SATCFG_PORT_TYPE_MAX][QC_MAX_PORTS_PERTYPE] = {
    [SATCFG_PORT_TYPE_CIF] = {0x11111111},

    [SATCFG_PORT_TYPE_BIF] = {0x11111111},

    [SATCFG_PORT_TYPE_NIF] = {0x11111111,
                              0x11111111,
                              0x11111111,
                              0x11111111},

    [SATCFG_PORT_TYPE_HIF] = {0x11111111,
                              0x11111111,
                              0x11111111,
                              0x11111111,
                              0x11111111,
                              0x11111111,
                              0x11111111,
                              0x11111111},
};


const uint32_t
qc_mac_hw_q2cos_map[SATCFG_PORT_TYPE_MAX][QC_MAX_PORTS_PERTYPE] = {
    [SATCFG_PORT_TYPE_CIF] = {0x000000ff},

    [SATCFG_PORT_TYPE_BIF] = {0x000000ff},

    [SATCFG_PORT_TYPE_NIF] = {0x000000ff,
                              0x000000ff,
                              0x000000ff,
                              0x000000ff},

    [SATCFG_PORT_TYPE_HIF] = {0x000000ff,
                              0x000000ff,
                              0x000000ff,
                              0x000000ff,
                              0x000000ff,
                              0x000000ff,
                              0x000000ff,
                              0x000000ff},
};
