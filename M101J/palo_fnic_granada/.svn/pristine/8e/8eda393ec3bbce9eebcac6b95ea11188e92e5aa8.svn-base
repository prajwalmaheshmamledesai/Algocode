/////////////////////////////////////////////////////////////////////////////
// $Author: pkaplan $
// $Revision: 1.3 $
//
// sbus defines
//
// references:
// [1] AVAGO 90 nm SBus Specification
/////////////////////////////////////////////////////////////////////////////
#ifndef RED_SBUS_DEFINES
#define RED_SBUS_DEFINES

#define RED_SBUS_BROADCAST  0xff // [1, page 8]
#define RED_SBUS_CONTROLLER 0xfe // [1, page 8]

// read return codes
#define RED_SBUS_READ_SUCCESS 4 // [1, table 3]
#define RED_SBUS_READ_FAILURE 6 // [1, table 3]

// how to create a byte aligned rnaxi address
#define RED_SBUS_ADDR(node, reg) \
                (RED_MISC_DHS_SBUS | ((node)<<8) | (reg))<<2
#endif
