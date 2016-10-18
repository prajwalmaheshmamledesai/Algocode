/////////////////////////////////////////////////////////////////////////////
// $Author: pkaplan $
// $Revision: 1.1 $
//
// reset routines
//
/////////////////////////////////////////////////////////////////////////////
#ifndef RED_HW_RESET
#define RED_HW_RESET
/**
 software reset routine
 \param mask one bit per port, HI0 is bit 0, HI7 is bit 7, NI0 is bit 8, NI3 is bit 11
 \return 0 on success
*/
int red_hw_software_reset(int inst, const int MASK);
#endif
