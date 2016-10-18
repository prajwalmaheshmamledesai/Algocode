/********************************************************************
 *
 *      File:   redwood_serdes_hw_seq.c
 *      Name:   Bruce McLoughlin
 *
 *      Description: SERDES level functions for Redwood USD
 *
 *
 *      Copyright (c) 2001,2002,2003,2004 by Nuova Systems, Inc.
 ********************************************************************/
#include <stdio.h>
#include <unistd.h>
#include "red_red_naxislave_define.h"
#include "redwood_serdes_hw_seq.h"

#define PREFIX "  Serdes: "

/*************** IO customization ***********************/
#ifdef USD
#include <isan/types.h>
#include "redwood_intf.h"
#include "redwood_access.h"
#define BA(x) ((x) << 2)
#else
#include "red_hw_lib.h" 
#endif
/*************** End of IO customization ****************/
                                                                                                                         

#define SBUS_OFS(x) ((x) << 2)
#define SBUS_READ_SUCCESS 4

// table 53, reg 0xf
#define sbus_rx_rdy_obs 1
                                                                                                                         
// table 52, reg 0x22
#define sbus_rx_en_cntl 1
#define sbus_tx_en_cntl 2
#define sbus_rx_rate_sel_shift 3

#define serdes_write reg_write
#define serdes_read  reg_read                                                                                                                         

/*****************************************************************************
* serdes_init_0
*****************************************************************************/
int serdes_init_0( int asic_id )
{
    unsigned int sbus = BA(RED_MISC_DHS_SBUS);

    // adjust sbus clk
    serdes_write( asic_id, sbus | (0xfe0a << 2), 0x3 );

    // this needs some time ...
    sleep(1);
    return 0;
}

/*****************************************************************************
* serdes_init_1
*****************************************************************************/
int serdes_init_1( int asic_id, int sbus_port, int first_lane, int n_lanes, int is_1g)
{
    int          i, serdes_lane = first_lane;
    unsigned int sbus = BA(RED_MISC_DHS_SBUS);
                                                                                                                          
    /* for all lanes ... */
    for (i = 0; i < n_lanes; i++, serdes_lane = (serdes_lane + 1) % 4) {
        unsigned int sbus_addr = 1 + (sbus_port*4) + serdes_lane;
	if (is_1g) {
        serdes_write( asic_id, sbus | (((sbus_addr << 8) | 0x00) << 2), (0x0A << 1) | 1 );    // set ref_clk
        serdes_write( asic_id, sbus | (((sbus_addr << 8) | 0x36) << 2), 0x02 );               // tx_rate_sel_cntl
        serdes_write( asic_id, sbus | (((sbus_addr << 8) | 0x22) << 2), (0x02 << 3) | 2 | 1 );// tx/rx_en_cntl
	} else {
        serdes_write( asic_id, sbus | (((sbus_addr << 8) | 0x00) << 2), (0x13 << 1) | 1 );    // set ref_clk
        serdes_write( asic_id, sbus | (((sbus_addr << 8) | 0x36) << 2), 0x05 );               // tx_rate_sel_cntl
        serdes_write( asic_id, sbus | (((sbus_addr << 8) | 0x22) << 2), (0x05 << 3) | 2 | 1 );// tx/rx_en_cntl
	}
    }
    return 0;
}
                                                                                                                         
/*****************************************************************************
* serdes_init_2
*****************************************************************************/
int serdes_init_2( int asic_id, int sbus_port, int first_lane, int n_lanes )
{
    int          i, serdes_lane = first_lane;
    unsigned int sbus = BA(RED_MISC_DHS_SBUS);
                                                                                                                          
    /* for all lanes ... */
    for (i = 0; i < n_lanes; i++, serdes_lane = (serdes_lane + 1) % 4) {
        unsigned int sbus_addr = 1 + (sbus_port*4) + serdes_lane;
                                                                                                                          
        serdes_write( asic_id, sbus | (((sbus_addr << 8) | 0x0d) << 2), (1 << 4) | 1 );//tx_output_en
    }
    return 0;
}                                                                                                                         

/*****************************************************************************
* serdes_init_preemp
*****************************************************************************/
int serdes_init_preemp( int asic_id, int sbus_port, int first_lane, int n_lanes , uint8_t preemp)
{
    int          i, serdes_lane = first_lane;
    unsigned int sbus = BA(RED_MISC_DHS_SBUS);
                                                                                                                          
    /* for all lanes ... */
    for (i = 0; i < n_lanes; i++, serdes_lane = (serdes_lane + 1) % 4) {
        unsigned int sbus_addr = 1 + (sbus_port*4) + serdes_lane;
                                                                                                                          
        serdes_write( asic_id, sbus | (((sbus_addr << 8) | 0x0d) << 2), 
			((1 << 4) | (preemp << 1) | 1) );//tx_output_en
    }
    return 0;
}                                                                                                                         
/*****************************************************************************
* serdes_rx_rdy_wait
*
* Utility function to poll for sbus_rx_rdy_obs on 1.4 serdes lanes
* starting with first_lane.
*
* sbus_port is the logical index of the ports' serdes lanes on the SBus.
*
* The mapping is as follows:
*       HI0 = 0
*       HI1 = 1
*       ...
*       HI7 = 7
*       NI0 = 8
*       NI1 = 9
*       NI2 = 10
*       NI3 = 11
*
*  serdes lanes are numbered 0..3
*
* For 10G port,
*   serdes_rx_rdy_wait( asic, xge_port, 0, 4 );
*
* For 1G port,
*   serdes_rx_rdy_wait( asic, gbe_port, 0, 1 );
*
* For HIx in 1G mode only one serdes lane is required. To handle lane
* mapping the starting lane can be incremented modulo the number of lanes
* (maybe this isn't required?).
*
*****************************************************************************/
int serdes_rx_rdy_wait( int asic_id, int sbus_port, int first_lane, int n_lanes )
{
    int i, serdes_lane = first_lane, tries = 100;
    unsigned int data;
    unsigned int sbus = BA(RED_MISC_DHS_SBUS);
                                                                                                                         
    /* wait for all lanes rx_rdy_obs */
    for (i = 0; i < n_lanes; i++, serdes_lane = (serdes_lane + 1) % 4) {
        do {
            unsigned int sbus_addr = 1 + (sbus_port*4) + serdes_lane;

            serdes_read( asic_id, sbus | (((sbus_addr << 8) | 0xf) << 2), &data );
        } while (--tries && !(data & 1) && ((data >> 8) == SBUS_READ_SUCCESS) );
        if ((tries == 0) || !(data & 1) || ((data >> 8) != SBUS_READ_SUCCESS)) break; // exit on error
    }
                                                                                                                         
    // check for SBus error first
    if ((data >> 8) != SBUS_READ_SUCCESS) {
        printf(PREFIX "%d: Port %d: SBus read error <%xh>\n", asic_id, sbus_port, data );
        return 1;
    }
                                                                                                                         
    // then see if data ready set
    if (!(data & 1)) {
        printf(PREFIX " Port %d: Warning: serdes not ready!\n", sbus_port);
        return 2;
    }
//    printf(PREFIX " Port %d: All serdes lanes ready\n", sbus_port);
    return 0;
}

/*****************************************************************************
* serdes_init_all_lanes_1
*
* Uses SBUS broadcast address to program all serdes lanes in one command
*****************************************************************************/
int serdes_init_all_lanes_1( int asic_id )
{
    unsigned int sbus = BA(RED_MISC_DHS_SBUS);
    unsigned int sbus_addr = 0xff; // broadcast address

    serdes_write( asic_id, sbus | (((sbus_addr << 8) | 0x00) << 2), (0x13 << 1) | 1 );    // set ref_clk
    serdes_write( asic_id, sbus | (((sbus_addr << 8) | 0x22) << 2), (0x05 << 3) | 2 | 1 );// tx/rx_en_cntl
    serdes_write( asic_id, sbus | (((sbus_addr << 8) | 0x36) << 2),  0x05 );              // tx_rate_sel_cntl
    return 0;
}

/*****************************************************************************
* serdes_init_all_lanes_2
*
* Uses SBUS broadcast address to program all serdes lanes in one command
*****************************************************************************/
int serdes_init_all_lanes_2( int asic_id )
{
    unsigned int sbus = BA(RED_MISC_DHS_SBUS);
    unsigned int sbus_addr = 0xff; // broadcast address

    serdes_write( asic_id, sbus | (((sbus_addr << 8) | 0x0d) << 2), (1 << 4) | 1 );//tx_output_en
    return 0;
}

/*****************************************************************************
* serdes_enable
*****************************************************************************/
int serdes_enable( int asic_id, int sbus_port, int first_lane, int n_lanes )
{
    int          s, serdes_lane = first_lane;
    unsigned int sbus = BA(RED_MISC_DHS_SBUS);

    /* bring up the serdes */
    for (s = 0; s < n_lanes; s++, serdes_lane = (serdes_lane + 1) % 4) {
        unsigned short sbus_addr = 1 + (sbus_port*4) + serdes_lane;
                                                                                                                         
        serdes_write( asic_id,
                   sbus | SBUS_OFS((sbus_addr << 8) | 0x22 /* avago reg?*/),
                   (sbus_rx_en_cntl | sbus_tx_en_cntl | (5 << sbus_rx_rate_sel_shift)) );
    }
    return 0;
}

/*****************************************************************************
* serdes_disable
*****************************************************************************/
int serdes_disable( int asic_id, int sbus_port, int first_lane, int n_lanes )
{
    int          s, serdes_lane = first_lane;
    unsigned int sbus = BA(RED_MISC_DHS_SBUS);

    /* clear rx_en and tx_en on the serdes */
    for (s = 0; s < n_lanes; s++, serdes_lane = (serdes_lane + 1) % 4) {
        unsigned short sbus_addr = 1 + (sbus_port*4) + serdes_lane;
                                                                                                                         
        serdes_write( asic_id,
                   sbus | SBUS_OFS((sbus_addr << 8) | 0x22 /* avago reg?*/),
                   0 );
    }
    return 0;
}

/*****************************************************************************
* serdes_near_loopback
*****************************************************************************/
int serdes_near_loopback( int asic_id, int sbus_port, int first_lane, int n_lanes )
{
    int          s, serdes_lane = first_lane;
    unsigned int sbus = BA(RED_MISC_DHS_SBUS);
 
    for (s = 0; s < n_lanes; s++, serdes_lane = (serdes_lane + 1) % 4) {
        unsigned short sbus_addr = 1 + (sbus_port*4) + serdes_lane;
  
        serdes_write( asic_id,
                   sbus | SBUS_OFS((sbus_addr << 8) | 0xd /* avago reg*/),
                   ((1 << 7) | (1 << 0)) );
    }
    return 0;
}

/*****************************************************************************
* serdes_lower_sig_thresh
*****************************************************************************/
int serdes_lower_sig_thresh( int asic_id, int sbus_port, int first_lane, int n_lanes )
{
    int          s, serdes_lane = first_lane;
    unsigned int sbus = BA(RED_MISC_DHS_SBUS);
 
    for (s = 0; s < n_lanes; s++, serdes_lane = (serdes_lane + 1) % 4) {
        unsigned short sbus_addr = 1 + (sbus_port*4) + serdes_lane;
  
        serdes_write( asic_id,
                   sbus | SBUS_OFS((sbus_addr << 8) | 0x26 /* avago reg*/),
                   7 );
  
        serdes_write( asic_id,
                   sbus | SBUS_OFS((sbus_addr << 8) | 0x1F /* avago reg*/),
                   0x20 );
    }
    return 0;
}

/*****************************************************************************
* serdes_normal_polarity_on_tx_lane
*****************************************************************************/
int serdes_normal_polarity_on_tx_lane( int asic_id, int sbus_port, int serdes_lane )
{
#define sbus_tx_data_gate            (1 << 0 /*reg=0xa*/)
#define sbus_tx_sel_core_data_cntl   (1 << 1 /*reg=0xa*/)
#define sbus_tx_polarity_inv_en_cntl (1 << 2/*reg=0xb*/)

    unsigned int sbus = BA(RED_MISC_DHS_SBUS);
    unsigned short sbus_addr = 1 + (sbus_port*4) + serdes_lane;
                                                                                                                                                        
    serdes_write( asic_id, 
                  sbus | SBUS_OFS((sbus_addr << 8) | 0xa /* avago reg*/), 
                  sbus_tx_data_gate | sbus_tx_sel_core_data_cntl /*3*/ );

    serdes_write( asic_id, 
                  sbus | SBUS_OFS((sbus_addr << 8) | 0xb /* avago reg*/), 
                  0 /* clear sbus_tx_polarity_inv_en_cntl */);

    return 0;
}

/*****************************************************************************
* serdes_invert_polarity_on_tx_lane
*****************************************************************************/
int serdes_invert_polarity_on_tx_lane( int asic_id, int sbus_port, int serdes_lane )
{
    unsigned int sbus = BA(RED_MISC_DHS_SBUS);
    unsigned short sbus_addr = 1 + (sbus_port*4) + serdes_lane;
                                                                                                                                                        
    serdes_write( asic_id, 
                  sbus | SBUS_OFS((sbus_addr << 8) | 0xa /* avago reg*/), 
                  sbus_tx_data_gate | sbus_tx_sel_core_data_cntl /*3*/ );

    serdes_write( asic_id, 
                  sbus | SBUS_OFS((sbus_addr << 8) | 0xb /* avago reg*/), 
                  sbus_tx_polarity_inv_en_cntl /*4*/);

    return 0;
}

#define sbus_rx_data_gate          (1 << 0 /*reg=0x3*/)
#define sbus_rx_8b10b_align_en_cntl (1 << 3 /*reg=0x3*/)
#define sbus_rx_ib_sig_strength_en_cntl (1 << 3 /*reg=0x6*/)
#define sbus_rx_polarity_inv_en_cntl (1 << 4 /*reg=0x7*/)
#define sbus_rx_reserved_bits        (5 << 1 /*reg=0x9*/)

/*****************************************************************************
* serdes_normal_polarity_on_rx_lane
*****************************************************************************/
int serdes_normal_polarity_on_rx_lane( int asic_id, int sbus_port, int serdes_lane )
{
    unsigned int sbus = BA(RED_MISC_DHS_SBUS);
    unsigned short sbus_addr = 1 + (sbus_port*4) + serdes_lane;

    serdes_write( asic_id,
                  sbus | SBUS_OFS((sbus_addr << 8) | 0x3 /* avago reg*/),
                  sbus_rx_data_gate | sbus_rx_8b10b_align_en_cntl /*9*/ );

    serdes_write( asic_id,
                  sbus | SBUS_OFS((sbus_addr << 8) | 0x7 /* avago reg*/),
                  0 /* ~sbus_rx_polarity_inv_en_cntl */);

    serdes_write( asic_id,
                  sbus | SBUS_OFS((sbus_addr << 8) | 0x6 /* avago reg*/),
                  sbus_rx_ib_sig_strength_en_cntl /*8*/);

    serdes_write( asic_id,
                  sbus | SBUS_OFS((sbus_addr << 8) | 0x9 /* avago reg*/),
                  sbus_rx_reserved_bits /*0xa*/);

    return 0;
}

/*****************************************************************************
* serdes_invert_polarity_on_rx_lane
*****************************************************************************/
int serdes_invert_polarity_on_rx_lane( int asic_id, int sbus_port, int serdes_lane )
{
    unsigned int sbus = BA(RED_MISC_DHS_SBUS);
    unsigned short sbus_addr = 1 + (sbus_port*4) + serdes_lane;
                                                                                                                                                        
    serdes_write( asic_id, 
                  sbus | SBUS_OFS((sbus_addr << 8) | 0x3 /* avago reg*/), 
                  sbus_rx_data_gate | sbus_rx_8b10b_align_en_cntl /*9*/ );

    serdes_write( asic_id, 
                  sbus | SBUS_OFS((sbus_addr << 8) | 0x7 /* avago reg*/), 
                  sbus_rx_polarity_inv_en_cntl /*0x10*/);

    serdes_write( asic_id, 
                  sbus | SBUS_OFS((sbus_addr << 8) | 0x6 /* avago reg*/), 
                  sbus_rx_ib_sig_strength_en_cntl /*8*/);

    serdes_write( asic_id, 
                  sbus | SBUS_OFS((sbus_addr << 8) | 0x9 /* avago reg*/), 
                  sbus_rx_reserved_bits /*0xa*/);

    return 0;
}
                                                                                                                                                        

/*****************************************************************************
* serdes_prbs_test_on_lane_step_1
*
* Initiate PRBS test on a single serdes lane. The caller should've either
* placed the serdes in near loopback mode or set the PHY XS loopback,
* otherwise, expect a failure from step_2.
*****************************************************************************/
int serdes_prbs_test_on_lane_step_1( int asic_id, int sbus_port, int serdes_lane )
{
#define sbus_tx_pattern_gen_en_cntl      (1 << 7 /*reg=0xa*/)
#define sbus_rx_pattern_cmp_en_cntl      (1 << 7 /*reg=0x7*/)
#define sbus_rx_error_monitor_en_cntl    (1 << 7 /*reg=0x4*/)
#define sbus_rx_error_monitor_reset_cntl (1 << 6 /*reg=0x4*/)
#define sbus_rx_error_monitor_gate       (1 << 0 /*reg=0x4*/)
    unsigned int sbus = BA(RED_MISC_DHS_SBUS);
    unsigned short sbus_addr = 1 + (sbus_port*4) + serdes_lane;

    serdes_write( asic_id,
                  sbus | SBUS_OFS((sbus_addr << 8) | 0xa /* avago reg*/),
                  sbus_tx_data_gate /*1, set tx_pattern_gen_sel_cntl & tx_data_sel_cntl to 0*/ );

    serdes_write( asic_id,
                  sbus | SBUS_OFS((sbus_addr << 8) | 0x3 /* avago reg*/),
                  sbus_rx_data_gate /*1*/ );

    serdes_write( asic_id,
                  sbus | SBUS_OFS((sbus_addr << 8) | 0x7 /* avago reg*/),
                  0 /*set the rx_pattern_cmp_sel & rx_sel_cntl to 0*/ );

    serdes_write( asic_id,
                  sbus | SBUS_OFS((sbus_addr << 8) | 0xa /* avago reg*/),
                  sbus_tx_pattern_gen_en_cntl | sbus_tx_data_gate /*0x81*/ );

    serdes_write( asic_id,
                  sbus | SBUS_OFS((sbus_addr << 8) | 0x7 /* avago reg*/),
                  sbus_rx_pattern_cmp_en_cntl /*0x80*/ );

    serdes_write( asic_id,
                  sbus | SBUS_OFS((sbus_addr << 8) | 0x4 /* avago reg*/),
                  (sbus_rx_error_monitor_en_cntl    |
                   sbus_rx_error_monitor_reset_cntl |
                   sbus_rx_error_monitor_gate) /*0xc1*/);

    serdes_write( asic_id,
                  sbus | SBUS_OFS((sbus_addr << 8) | 0x4 /* avago reg*/),
                  (sbus_rx_error_monitor_en_cntl    |
                   sbus_rx_error_monitor_gate) /*0x81*/);
    return 0;
}

/*****************************************************************************
* serdes_prbs_test_on_lane_step_1_1
*****************************************************************************/
int serdes_prbs_test_on_lane_step_1_1( int asic_id, int sbus_port, int serdes_lane )
{
#define sbus_tx_pattern_gen_en_cntl      (1 << 7 /*reg=0xa*/)
#define sbus_rx_pattern_cmp_en_cntl      (1 << 7 /*reg=0x7*/)
#define sbus_rx_error_monitor_en_cntl    (1 << 7 /*reg=0x4*/)
#define sbus_rx_error_monitor_reset_cntl (1 << 6 /*reg=0x4*/)
#define sbus_rx_error_monitor_gate       (1 << 0 /*reg=0x4*/)
    unsigned int sbus = BA(RED_MISC_DHS_SBUS);
    unsigned short sbus_addr = 1 + (sbus_port*4) + serdes_lane;

    serdes_write( asic_id,
                  sbus | SBUS_OFS((sbus_addr << 8) | 0xa /* avago reg*/),
                  sbus_tx_data_gate /*1, set tx_pattern_gen_sel_cntl & tx_data_sel_cntl to 0*/ );

    serdes_write( asic_id,
                  sbus | SBUS_OFS((sbus_addr << 8) | 0x3 /* avago reg*/),
                  sbus_rx_data_gate /*1*/ );

    serdes_write( asic_id,
                  sbus | SBUS_OFS((sbus_addr << 8) | 0x7 /* avago reg*/),
                  0 /*set the rx_pattern_cmp_sel & rx_sel_cntl to 0*/ );

    serdes_write( asic_id,
                  sbus | SBUS_OFS((sbus_addr << 8) | 0x7 /* avago reg*/),
                  sbus_rx_pattern_cmp_en_cntl /*0x80*/ );

    serdes_write( asic_id,
                  sbus | SBUS_OFS((sbus_addr << 8) | 0xa /* avago reg*/),
                  sbus_tx_pattern_gen_en_cntl | sbus_tx_data_gate /*0x81*/ );

    serdes_write( asic_id,
                  sbus | SBUS_OFS((sbus_addr << 8) | 0x4 /* avago reg*/),
                  (sbus_rx_error_monitor_en_cntl    |
                   sbus_rx_error_monitor_reset_cntl |
                   sbus_rx_error_monitor_gate) /*0xc1*/);

    return 0;
}

/*****************************************************************************
* serdes_prbs_test_on_lane_step_1_2
*****************************************************************************/
int serdes_prbs_test_on_lane_step_1_2( int asic_id, int sbus_port, int serdes_lane )
{
#define sbus_tx_pattern_gen_en_cntl      (1 << 7 /*reg=0xa*/)
#define sbus_rx_pattern_cmp_en_cntl      (1 << 7 /*reg=0x7*/)
#define sbus_rx_error_monitor_en_cntl    (1 << 7 /*reg=0x4*/)
#define sbus_rx_error_monitor_reset_cntl (1 << 6 /*reg=0x4*/)
#define sbus_rx_error_monitor_gate       (1 << 0 /*reg=0x4*/)
    unsigned int sbus = BA(RED_MISC_DHS_SBUS);
    unsigned short sbus_addr = 1 + (sbus_port*4) + serdes_lane;

    serdes_write( asic_id,
                  sbus | SBUS_OFS((sbus_addr << 8) | 0x4 /* avago reg*/),
                  (sbus_rx_error_monitor_en_cntl    |
                   sbus_rx_error_monitor_gate) /*0x81*/);
    return 0;
}

/*****************************************************************************
* serdes_prbs_test_on_all_lanes
*
* Initiate PRBS test on ALL serdes lanes. The caller should've either
* placed the serdes in near loopback mode or set the PHY Xaui lane loopback,
* otherwise, expect a failure from step_2.
*****************************************************************************/
int serdes_prbs_test_on_all_lanes( int asic_id )
{
#define sbus_tx_pattern_gen_en_cntl      (1 << 7 /*reg=0xa*/)
#define sbus_rx_pattern_cmp_en_cntl      (1 << 7 /*reg=0x7*/)
#define sbus_rx_error_monitor_en_cntl    (1 << 7 /*reg=0x4*/)
#define sbus_rx_error_monitor_reset_cntl (1 << 6 /*reg=0x4*/)
#define sbus_rx_error_monitor_gate       (1 << 0 /*reg=0x4*/)
    unsigned int sbus = BA(RED_MISC_DHS_SBUS);
    unsigned short sbus_addr = 0xff; // broadcast address

    serdes_write( asic_id,
                  sbus | SBUS_OFS((sbus_addr << 8) | 0xa /* avago reg*/),
                  sbus_tx_data_gate /*1, set tx_pattern_gen_sel_cntl & tx_data_sel_cntl to 0*/ );

    serdes_write( asic_id,
                  sbus | SBUS_OFS((sbus_addr << 8) | 0x3 /* avago reg*/),
                  sbus_rx_data_gate /*1*/ );

    serdes_write( asic_id,
                  sbus | SBUS_OFS((sbus_addr << 8) | 0x7 /* avago reg*/),
                  0 /*set the rx_pattern_cmp_sel & rx_sel_cntl to 0*/ );

    serdes_write( asic_id,
                  sbus | SBUS_OFS((sbus_addr << 8) | 0xa /* avago reg*/),
                  sbus_tx_pattern_gen_en_cntl | sbus_tx_data_gate /*0x81*/ );

    serdes_write( asic_id,
                  sbus | SBUS_OFS((sbus_addr << 8) | 0x7 /* avago reg*/),
                  sbus_rx_pattern_cmp_en_cntl /*0x80*/ );

    serdes_write( asic_id,
                  sbus | SBUS_OFS((sbus_addr << 8) | 0x4 /* avago reg*/),
                  (sbus_rx_error_monitor_en_cntl    |
                   sbus_rx_error_monitor_reset_cntl |
                   sbus_rx_error_monitor_gate) /*0xc1*/);

    serdes_write( asic_id,
                  sbus | SBUS_OFS((sbus_addr << 8) | 0x4 /* avago reg*/),
                  (sbus_rx_error_monitor_en_cntl    |
                   sbus_rx_error_monitor_gate) /*0x81*/);
    return 0;
}

/*****************************************************************************
* serdes_prbs_test_on_all_lanes_1
*
* Initiate PRBS test on ALL serdes lanes. The caller should've either
* placed the serdes in near loopback mode or set the PHY Xaui lane loopback,
* otherwise, expect a failure from step_2.
*****************************************************************************/
int serdes_prbs_test_on_all_lanes_1( int asic_id )
{
#define sbus_tx_pattern_gen_en_cntl      (1 << 7 /*reg=0xa*/)
#define sbus_rx_pattern_cmp_en_cntl      (1 << 7 /*reg=0x7*/)
#define sbus_rx_error_monitor_en_cntl    (1 << 7 /*reg=0x4*/)
#define sbus_rx_error_monitor_reset_cntl (1 << 6 /*reg=0x4*/)
#define sbus_rx_error_monitor_gate       (1 << 0 /*reg=0x4*/)
    unsigned int sbus = BA(RED_MISC_DHS_SBUS);
    unsigned short sbus_addr = 0xff; // broadcast address

    serdes_write( asic_id,
                  sbus | SBUS_OFS((sbus_addr << 8) | 0xa /* avago reg*/),
                  sbus_tx_data_gate /*1, set tx_pattern_gen_sel_cntl & tx_data_sel_cntl to 0*/ );

    serdes_write( asic_id,
                  sbus | SBUS_OFS((sbus_addr << 8) | 0x3 /* avago reg*/),
                  sbus_rx_data_gate /*1*/ );

    serdes_write( asic_id,
                  sbus | SBUS_OFS((sbus_addr << 8) | 0x7 /* avago reg*/),
                  0 /*set the rx_pattern_cmp_sel & rx_sel_cntl to 0*/ );

    serdes_write( asic_id,
                  sbus | SBUS_OFS((sbus_addr << 8) | 0x7 /* avago reg*/),
                  sbus_rx_pattern_cmp_en_cntl /*0x80*/ );

    serdes_write( asic_id,
                  sbus | SBUS_OFS((sbus_addr << 8) | 0xa /* avago reg*/),
                  sbus_tx_pattern_gen_en_cntl | sbus_tx_data_gate /*0x81*/ );

    serdes_write( asic_id,
                  sbus | SBUS_OFS((sbus_addr << 8) | 0x4 /* avago reg*/),
                  (sbus_rx_error_monitor_en_cntl    |
                   sbus_rx_error_monitor_reset_cntl |
                   sbus_rx_error_monitor_gate) /*0xc1*/);
    return 0;
}

int serdes_prbs_test_on_all_lanes_2( int asic_id )
{
    unsigned int sbus = BA(RED_MISC_DHS_SBUS);
    unsigned short sbus_addr = 0xff; // broadcast address

    serdes_write( asic_id,
                  sbus | SBUS_OFS((sbus_addr << 8) | 0x4 /* avago reg*/),
                  (sbus_rx_error_monitor_en_cntl    |
                   sbus_rx_error_monitor_gate) /*0x81*/);
    return 0;
}

/*****************************************************************************
* serdes_prbs_test_on_lane_step_2
*
* Returns: 0 = ok
*         -1 = SBUS read error (FAILED)
*         -2 = PRBS failure    (FAILED)
*****************************************************************************/
int serdes_prbs_test_on_lane_step_2( int asic_id, int sbus_port, int serdes_lane )
{
#define sbus_rx_prbs_failure_obs  (1 << 1 /* reg=0x1*/)
#define sbus_rx_error_occured_obs (1 << 0 /* reg=0x1*/)
#define sbus_rx_error_count_obs_31_24 0x7
#define sbus_rx_error_count_obs_23_16 0x8
#define sbus_rx_error_count_obs_15_8  0x9
#define sbus_rx_error_count_obs_7_0   0xA

    unsigned int data, ec1,ec2,ec3,ec4;
    unsigned int sbus = BA(RED_MISC_DHS_SBUS);
    unsigned short sbus_addr = 1 + (sbus_port*4) + serdes_lane;

    serdes_read( asic_id, sbus | SBUS_OFS((sbus_addr << 8) | 0x1 /* avago reg*/), &data);

    if ((data >> 8) != SBUS_READ_SUCCESS)
        return -1; // read error after test

    /* if an error occurred, see how many of them */
    if (data & (sbus_rx_prbs_failure_obs | sbus_rx_error_occured_obs)) {

        /* read the error counters */
        serdes_read( asic_id, sbus | SBUS_OFS((sbus_addr << 8) | sbus_rx_error_count_obs_7_0),   &ec1);
        serdes_read( asic_id, sbus | SBUS_OFS((sbus_addr << 8) | sbus_rx_error_count_obs_15_8),  &ec2);
        serdes_read( asic_id, sbus | SBUS_OFS((sbus_addr << 8) | sbus_rx_error_count_obs_23_16), &ec3);
        serdes_read( asic_id, sbus | SBUS_OFS((sbus_addr << 8) | sbus_rx_error_count_obs_31_24), &ec4);

        /* mask off the SBUS_READ status */
        ec1 &= 0xff;
        ec2 &= 0xff;
        ec3 &= 0xff;
        ec4 &= 0xff;

        /* error count is the return value (necessarily non-zero) */
        if (ec1 || ec2 || ec3 || ec4) {
            return ( (ec4 << 24) | (ec3 << 16) | (ec2 << 8) | ec1 );
        }
    }

    if (data & sbus_rx_prbs_failure_obs)
        return -2; // PRBS test failure

    if (data & sbus_rx_error_occured_obs)
        return -3; // PRBS test failure

    return 0;
}

/*****************************************************************************
* serdes_teardown_prbs_test_on_all_lanes
*
* Un-configure PRBS test on ALL serdes lanes. The PRBS test setup writes to
* serdes registers 3, 4, 7, and 10 (0xA). Some of these are used to set
* polarity inversion and will need to be re-written from satcfg info (from
* the PORT or MAC level).
* Here we just re-write them to all 0-s to un-set the "gate" that is
* overriding the default (hard-coded inthe RTL) init values.
*****************************************************************************/
int serdes_teardown_prbs_test_on_all_lanes( int asic_id )
{
    unsigned int sbus = BA(RED_MISC_DHS_SBUS);
    unsigned short sbus_addr = 0xff; // broadcast address

    serdes_write( asic_id,
                  sbus | SBUS_OFS((sbus_addr << 8) | 0x3 /* avago reg*/),
                  0 );

    serdes_write( asic_id,
                  sbus | SBUS_OFS((sbus_addr << 8) | 0x4 /* avago reg*/),
                  0 );

    serdes_write( asic_id,
                  sbus | SBUS_OFS((sbus_addr << 8) | 0x7 /* avago reg*/),
                  0 );

    serdes_write( asic_id,
                  sbus | SBUS_OFS((sbus_addr << 8) | 0xa /* avago reg*/),
                  0 );
    return 0;
}

/*****************************************************************************
* serdes_prbs_test_step_3
*****************************************************************************/
int serdes_prbs_test_step_3( int asic_id, int sbus_port, int serdes_lane )
{
    unsigned int sbus = BA(RED_MISC_DHS_SBUS);
    unsigned short sbus_addr = 1 + (sbus_port*4) + serdes_lane;

    serdes_write( asic_id,
                  sbus | SBUS_OFS((sbus_addr << 8) | 0x3 /* avago reg*/),
                  0 );

    serdes_write( asic_id,
                  sbus | SBUS_OFS((sbus_addr << 8) | 0x4 /* avago reg*/),
                  0 );

    serdes_write( asic_id,
                  sbus | SBUS_OFS((sbus_addr << 8) | 0x7 /* avago reg*/),
                  0 );

    serdes_write( asic_id,
                  sbus | SBUS_OFS((sbus_addr << 8) | 0xa /* avago reg*/),
                  0 );
    return 0;
}


/*****************************************************************************
* serdes_strength_test
*****************************************************************************/
int serdes_strength_test( int asic_id, int sbus_port, int serdes_lane,
                          int checks, int *passes, int *failures, int *errors )
{
#define sbus_rx_ib_sig_strength_obs_7_6 (3 << 6 /*reg=0x14*/)
    unsigned int data;
    unsigned int sbus = BA(RED_MISC_DHS_SBUS);
    unsigned short sbus_addr = 1 + (sbus_port*4) + serdes_lane;
    int i;

    *failures = 0;
    *passes   = 0;
    *errors   = 0;

    for (i = 0; i < checks; i++) {
        reg_read( asic_id, sbus | SBUS_OFS((sbus_addr << 8) | 0x14), &data );

        /* check for access error first (before trusting data) */
        if ((data >> 8) != SBUS_READ_SUCCESS) {
            printf("**************** SBus Error: %x\n", data );
            (*errors)++;
        }

        if ((data & sbus_rx_ib_sig_strength_obs_7_6) != sbus_rx_ib_sig_strength_obs_7_6)
            (*failures)++;
        else
            (*passes)++;
    }
    return (*failures);
}

/*****************************************************************************
* serdes_enable_far_loopback_all_lanes
*****************************************************************************/
void serdes_enable_far_loopback_all_lanes( int asic_id )
{
#define sbus_rx_far_loopback_en   (1 << 4 /*reg=0x9*/)
#define sbus_rx_far_loopback_gate (1 << 0 /*reg=0x9*/)
    unsigned int sbus = BA(RED_MISC_DHS_SBUS);
    unsigned short sbus_addr = 0xff; // broadcast address
    serdes_write( asic_id,
                  sbus | SBUS_OFS((sbus_addr << 8) | 0x9 /* avago reg*/),
                  (sbus_rx_far_loopback_en | sbus_rx_far_loopback_gate) /*0x11*/ );
}

/*****************************************************************************
* serdes_disable_far_loopback_all_lanes
*****************************************************************************/
void serdes_disable_far_loopback_all_lanes( int asic_id )
{
    unsigned int sbus = BA(RED_MISC_DHS_SBUS);
    unsigned short sbus_addr = 0xff; // broadcast address

    serdes_write( asic_id,
                  sbus | SBUS_OFS((sbus_addr << 8) | 0x9),
                  (sbus_rx_reserved_bits));
}
