/////////////////////////////////////////////////////////////////////////////
// $Author: pkaplan $
// $Revision: 1.9 $
//
// serdes configuration library header file
//
/////////////////////////////////////////////////////////////////////////////
#ifndef RED_HW_SERDES
#define RED_HW_SERDES
/**
 Serdes configuration procedure

 \param gbe_mode HI ports are Gigabit Ethernet ports
 \param pll_bypass the clock reference to redwood bypasses the PLL
 \param hi_mask which HI instances to configure, one bit per serdes (there are 32 of them), HI0 serdes 0 is bit 0. To broadcast use -1
 \param ni_mask which NI instances to configure, one bit per serdes (there are 16 of them), NI0 serdes 0 is bit 0. To broadcast use -1
 \param MAX_TRIES number of polling for readiness iterations before giving up
 \return 0 on success, otherwise a code defined in red_hw_error.h

 Depending on PLL bypass, GBE or XGBE modes, serdes use different parameters
 For reference, here is the table of clock we use
 mode                       clock freq (MHz)
 ---------------            ----------------
 1G PLL bypass              125
 1G PLL                     62.5
 10G PLL bypass             312.5
 10G PLL                    156.25
*/
int red_hw_configure_serdes(int inst,const int gbe_mode, const int pll_bypass, const int hi_mask, const short ni_mask, const int max_tries, const int hi_polarity_rx_inv_mask, const int hi_polarity_tx_inv_mask, const short ni_polarity_rx_inv_mask, const short ni_polarity_tx_inv_mask) ;
int red_hw_serdes_near_loopback(int inst,const int serdes_sbus_address, const int enable) ;
int red_hw_serdes_prbs_test(int inst,const int serdes_sbus_address, const int wait_time);
int red_hw_serdes_far_loopback(int inst,const int serdes_sbus_address, const int enable) ;
int red_hw_serdes_rx_rdy_test(int inst,const int hi_mask, const short ni_mask, unsigned int* addr_error, unsigned int* exp_data, unsigned int* got_data);
int red_hw_serdes_access_test(int inst,const int hi_mask, const short ni_mask, unsigned int* addr_error, unsigned int* exp_data, unsigned int* got_data);
#endif
