/********************************************************************
*
*      File:   redwood_serdes_hw_seq.h
*      Name:   Bruce McLoughlin
*
*      Description: Stripped down SERDES programming sequences
*      that can be shared between USD and Diags
*
*
*      Copyright (c) 2001,2002,2003,2004 by Nuova Systems, Inc.
********************************************************************/
#ifndef SERDES_HW_SEQ_H_INCLUDED
#define SERDES_HW_SEQ_H_INCLUDED


/*****************************************************************************
* serdes_init_0
*****************************************************************************/
int serdes_init_0( int asic_id );

/*****************************************************************************
* serdes_init_1
*****************************************************************************/
int serdes_init_1( int asic_id, int sbus_port, int first_lane, int n_lanes, int is_1g);


/*****************************************************************************
* serdes_init_2
*****************************************************************************/
int serdes_init_2( int asic_id, int sbus_port, int first_lane, int n_lanes  );

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
int serdes_rx_rdy_wait( int asic_id, int sbus_port, int first_lane, int n_lanes );

/*****************************************************************************
* serdes_enable
*****************************************************************************/
int serdes_enable( int asic_id, int sbus_port, int first_lane, int n_lanes );

/*****************************************************************************
* serdes_disable
*****************************************************************************/
int serdes_disable( int asic_id, int sbus_port, int first_lane, int n_lanes );

/*****************************************************************************
* serdes_lower_sig_thresh
*****************************************************************************/
int serdes_lower_sig_thresh( int asic_id, int sbus_port, int first_lane, int n_lanes );

/*****************************************************************************
* serdes_near_loopback
*****************************************************************************/
int serdes_near_loopback( int asic_id, int sbus_port, int first_lane, int n_lanes );

/*****************************************************************************
* serdes_normal_polarity_on_tx_lane
*****************************************************************************/
int serdes_normal_polarity_on_tx_lane( int asic_id, int sbus_port, int serdes_lane );
int serdes_normal_polarity_on_rx_lane( int asic_id, int sbus_port, int serdes_lane );

/*****************************************************************************
* serdes_invert_polarity_on_tx_lane
*****************************************************************************/
int serdes_invert_polarity_on_tx_lane( int asic_id, int sbus_port, int serdes_lane );

/*****************************************************************************
* serdes_invert_polarity_on_rx_lane
*****************************************************************************/
int serdes_invert_polarity_on_rx_lane( int asic_id, int sbus_port, int serdes_lane );


#endif /*SERDES_HW_SEQ_H_INCLUDED*/

