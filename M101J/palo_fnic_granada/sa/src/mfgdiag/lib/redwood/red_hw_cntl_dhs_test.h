//////////////////////////////////////////////////////////////////
// $Author: pkaplan $
// $Revision: 1.1 $
//
// A test to scrub the ctrl DHS memories in SS
//////////////////////////////////////////////////////////////////
#ifndef RED_HW_CNTL_DHS_TEST 
#define RED_HW_CNTL_DHS_TEST 

int red_hw_cntl_dhs_test (int inst, const int seed, const int ss_bitmap, const int cntl_bitmap, unsigned int* addr_error, unsigned  int* exp_data, unsigned  int* got_data) ;

#endif
