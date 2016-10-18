/////////////////////////////////////////////////////////////////////////////
// $Author: pkaplan $
// $Revision: 1.2 $
//
// reset routines
//
/////////////////////////////////////////////////////////////////////////////
#include "red_red_naxislave_define.h"
#ifdef RED_VERIFICATION
#include "libutilTB.h"
#include "cmdOpts.h"
#else 
#include "red_hw_lib.h" 
#endif

////////////////////////////////////////////////////////////////////////
// externally defined symbols
////////////////////////////////////////////////////////////////////////
extern int red_reg_wr(int inst, unsigned int address, unsigned int  data);  // address is 32 bit aligned
extern int red_reg_rd(int inst, unsigned int address, unsigned int* data); // address is 32 bit aligned


/**
 software reset routine
 \param mask one bit per port, HI0 is bit 0, HI7 is bit 7, NI0 is bit 8, NI3 is bit 11
 \return 0 on success
*/
int red_hw_software_reset(int inst, const int MASK) {
    int index;
    int ret_val;
    unsigned int read_data;
    int sleep_time = 3*256; // 256 cycles * 3ns/cycle [ns]
    #ifndef RED_VERIFICATION
    sleep_time = (sleep_time + 999)/1000; // round up to microseconds for software
    #endif
    // table of SW reset registers
    typedef struct { 
         int addr; 
         int offset_10b20b; 
    } addr_table_t;
    const addr_table_t addr_table[] = 
        { { RED_HI0_SW_RST, RED_HI_SW_RST_FLD_RESET_10B20B }
        , { RED_HI1_SW_RST, RED_HI_SW_RST_FLD_RESET_10B20B }
        , { RED_HI2_SW_RST, RED_HI_SW_RST_FLD_RESET_10B20B }
        , { RED_HI3_SW_RST, RED_HI_SW_RST_FLD_RESET_10B20B }
        , { RED_HI4_SW_RST, RED_HI_SW_RST_FLD_RESET_10B20B }
        , { RED_HI5_SW_RST, RED_HI_SW_RST_FLD_RESET_10B20B }
        , { RED_HI6_SW_RST, RED_HI_SW_RST_FLD_RESET_10B20B }
        , { RED_HI7_SW_RST, RED_HI_SW_RST_FLD_RESET_10B20B }
        , { RED_NI0_SW_RST, RED_NI_SW_RST_FLD_RESET_10B20B }
        , { RED_NI1_SW_RST, RED_NI_SW_RST_FLD_RESET_10B20B }
        , { RED_NI2_SW_RST, RED_NI_SW_RST_FLD_RESET_10B20B }
        , { RED_NI3_SW_RST, RED_NI_SW_RST_FLD_RESET_10B20B }
    };
    const int TABLE_SIZE =sizeof(addr_table)/sizeof(addr_table_t); 

    // wait for internal counters to propagate
    // once the redwood input pin H_RESET is deasserted
    // this is probably only relevant for simulation, but it does not hurt
    // in real silicon
    RED_SLEEP(sleep_time);

    // set all bits in the software reset registers with their MASK bit set to 1
    for (index=0; index<TABLE_SIZE; ++index) {
        if ((MASK>>index)&1) {
            const int write_value = 0xffffffff;
            if ((ret_val=red_reg_wr(inst, addr_table[index].addr<<2, write_value))!=0) {
                    return ret_val;
            }
        }
    }

    if ((ret_val=red_reg_rd(inst, RED_HI0_SW_RST<<2, &read_data))!=0) { // flush writes
            return ret_val;
    }

    // set 10b20b gearbox's reset to 0, but keep all other resets high
    for (index = 0; index < TABLE_SIZE; ++index) {
        if ((MASK>>index)&1) {
            const int write_value = ~(1<<addr_table[index].offset_10b20b);
            if ((ret_val=red_reg_wr(inst, addr_table[index].addr<<2, write_value))!=0) {
                    return ret_val;
            }
        }
    }

    // big flush, 3 is an arbitrary number to allow the reset of the gearbox
    for (index=0; index<3; ++index) {
        if ((ret_val=red_reg_rd(inst, RED_HI0_SW_RST<<2, &read_data))!=0) { // flush writes
                return ret_val;
        }
    }

    // release all reset bits
    for (index = 0; index<TABLE_SIZE; ++index) {
        if ((MASK>>index)&1) {
            const int write_value = 0;   
            if ((ret_val=red_reg_wr(inst, addr_table[index].addr<<2, write_value))!=0) {
                    return ret_val;
            }
        }
    }

    // release misc SW resets
    if ((ret_val=red_reg_wr(inst, RED_MISC_SW_RST<<2, 0))!=0) {
            return ret_val;
    }
    // flush writes
    ret_val=red_reg_rd(inst, RED_MISC_SW_RST<<2, &read_data);
    return ret_val;
}
