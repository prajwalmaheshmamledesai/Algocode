//////////////////////////////////////////////////////////////////
// $Author: pkaplan $
// $Revision: 1.1 $
//
// A test to exercise the MEMIF autotest feature in CNTL memories
//////////////////////////////////////////////////////////////////
#ifndef RED_HW_CNTL_MEMIF_AUTOTEST 
#define RED_HW_CNTL_MEMIF_AUTOTEST 

int red_hw_cntl_memif_autotest (const int seed, const int ss_bitmap, const int cntl_bitmap, unsigned int* addr_error, unsigned  int* exp_data, unsigned  int* got_data) ;

#endif
