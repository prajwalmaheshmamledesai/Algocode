/////////////////////////////////////////////////////////////////////////////
// $Author: pkaplan $
// $Revision: 1.5 $
//
// common defines 
//
/////////////////////////////////////////////////////////////////////////////
#ifndef RED_HW_GLOBALS
#define RED_HW_GLOBALS

// there are 4 serdes per HI and NI
#define RED_NUM_SERDES_PER_PORT 4

// serdes addresses start at 1 (HI0, serdes0) and end in 48 (NI3, serdes3)
#define RED_SERDES_START_OFFSET 1

// how many HIs in redwood
#define RED_NUM_HI 8

// how many NIs in redwood
#define RED_NUM_NI 4

// how many OQs per SS
#define RED_NUM_SS_OQ 5

// how may SSs per redwood
#define RED_NUM_SS 4

// how may RDARBs per redwood
#define RED_NUM_RDARB 5

// JTAG ID
#define RED_JTAG_ID_REV_B 0x2033e57f
#define RED_JTAG_ID       0x1033e57f

#ifdef RED_VERIFICATION
// helpers to do bitmap operations
static void set_field(unsigned int* variable, const unsigned int field_end_bit, const unsigned int field_start_bit, const unsigned int value) { 
        const unsigned int mask = ((1<<(field_end_bit - field_start_bit +1))-1)<<field_start_bit; 
        *variable = (*variable & ~mask) 
                 | ((value << field_start_bit) & mask) 
                 ; 
}

static void set_bit(unsigned int* variable, const unsigned int field_bit, const unsigned int value) { 
        set_field(variable, field_bit, field_bit, value);
}
#endif

#endif
