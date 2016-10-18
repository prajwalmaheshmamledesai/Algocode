//////////////////////////////////////////////////////////////////
// $Author: pkaplan $
// $Revision: 1.3 $
//
// A test to scrub the ctrl DHS memories in SS
//////////////////////////////////////////////////////////////////
#include "red_hw_error.h"
#include "red_hw_globals.h"
#include "red_red_naxislave_define.h"

#ifdef RED_VERIFICATION
#include "msgFormat.h"
#include "Knob.h"
#define RED_SRAND(seed) RandKnob my_rand(#seed)
#define RED_RAND my_rand.eval()
#else
#include <stdio.h>
#include <stdlib.h>
#define RED_SRAND(seed) srand(seed)  
#define RED_RAND rand()
#endif

////////////////////////////////////////////////////////////////////////
// externally defined symbols
////////////////////////////////////////////////////////////////////////
extern int red_reg_wr(int inst, unsigned int address, unsigned int  data);  // address is byte aligned
extern int red_reg_rd(int inst, unsigned int address, unsigned int* data); // address is byte aligned

/**********************************************************
A test that scrubs the ctrl DHS memories in SS
writes a bunch of random values to all memories behind the DHS
read back those values
\param seed used to initialize random generator
\param ss_bitmap a bit per SS  (bit 0=ss0, bit 1=ss1 ...)
\param cntl_bitmap a bit per function  (bit 0=frame cell fifo, bit 1=frame list ...)
\param addr_error returns the address where the first error occurred (byte aligned)
\param exp_data returns the data that was expected
\param got_data returns the data that was received
\return 0 on success, an error code otherwise
**********************************************************/
int red_hw_cntl_dhs_test (int inst, const int seed, const int ss_bitmap, const int cntl_bitmap, unsigned int* addr_error, unsigned  int* exp_data, unsigned  int* got_data) {
        typedef struct {
                char* name;
                unsigned int id;
                unsigned int width; // bits
                unsigned int depth;
        } table_entry;
        const unsigned int MAX_DEPTH=4800;
        const table_entry table[] = 
                   // name               id      width  depth
                { { "FREE_CELL_FIFO"    , 0     , 13    ,MAX_DEPTH }
                , { "FRAME_LIST"        , 1     , 13    ,MAX_DEPTH }
                , { "DESC"              , 2     , 26    ,MAX_DEPTH }
                , { "MULTICAST"         , 3     , 4     ,MAX_DEPTH }
                , { "OQ0"               , 4     , 13    ,MAX_DEPTH }
                , { "OQ1"               , 5     , 13    ,MAX_DEPTH }
                , { "OQ2"               , 6     , 13    ,MAX_DEPTH }
                , { "OQ3"               , 7     , 13    ,MAX_DEPTH }
                , { "OQ4"               , 8     , 13    ,MAX_DEPTH }
        };
        const static unsigned int DHS_CNTL_MEMIF_CMD[] = 
                { RED_SS0_DHS_CNTL_MEMIF_CMD
                , RED_SS1_DHS_CNTL_MEMIF_CMD
                , RED_SS2_DHS_CNTL_MEMIF_CMD
                , RED_SS3_DHS_CNTL_MEMIF_CMD
        };
        const static unsigned int CFG_CNTL_MEMIF[] =
                { RED_SS0_CFG_CNTL_MEMIF
                , RED_SS1_CFG_CNTL_MEMIF
                , RED_SS2_CFG_CNTL_MEMIF
                , RED_SS3_CFG_CNTL_MEMIF
        };

        const static unsigned int DHS_CNTL_MEMIF_ADDR[] =
                { RED_SS0_DHS_CNTL_MEMIF_ADDR
                , RED_SS1_DHS_CNTL_MEMIF_ADDR
                , RED_SS2_DHS_CNTL_MEMIF_ADDR
                , RED_SS3_DHS_CNTL_MEMIF_ADDR
        };

        const static unsigned int DHS_CNTL_MEMIF_DATA[] =
                { RED_SS0_DHS_CNTL_MEMIF_DATA
                , RED_SS1_DHS_CNTL_MEMIF_DATA
                , RED_SS2_DHS_CNTL_MEMIF_DATA
                , RED_SS3_DHS_CNTL_MEMIF_DATA
        };
        
        int ret_val;

        const unsigned int COLUMNS = (RED_SS_DHS_CNTL_MEMIF_DATA_SZ+31)/32;


        unsigned int row, column, which_ss=0;
	unsigned int entry;
        {   RED_SRAND(seed); //seed random generator again 
            for( which_ss=0; which_ss<RED_NUM_SS; ++which_ss) {
                if ( !(ss_bitmap&(1<<which_ss))) {
                        continue;
                }
                *addr_error=DHS_CNTL_MEMIF_CMD[which_ss];
                if ((ret_val=red_reg_wr(inst,            (*addr_error)*4 // byte aligned
                                                   , (1<<12) // autowrite
                                                   | (1<<RED_SS_DHS_CNTL_MEMIF_CMD_FLD_DIRACC)
                                                   | (1<<RED_SS_DHS_CNTL_MEMIF_CMD_FLD_AUTOINC)
                                        ))!=0) {
                        return ret_val;
                }
                // first write all the tables
                for (entry=0; entry<(sizeof(table)/sizeof(table_entry)); ++entry) {
                    if ( !(cntl_bitmap&(1<<entry))) {
                            continue;
                    }
                    // set the id in the CFG register to point to the desired data structure
                    *addr_error=CFG_CNTL_MEMIF[which_ss];
                    if ((ret_val=red_reg_wr(inst, (*addr_error)*4, table[entry].id))!=0) {
                        return ret_val;
                    }
                    const unsigned int MASK = (1<<table[entry].width)-1;

                    // reset the starting address
                    *addr_error=DHS_CNTL_MEMIF_ADDR[which_ss];
                    if ((ret_val=red_reg_wr(inst, (*addr_error)*4, 0))!=0) {
                        return ret_val;
                    }

                    // write a bunch of garbage into the device
                    for (row=0; row<table[entry].depth; ++row) {
                        #ifdef RED_VERIFICATION
                                NU_LOG << " filling device " << table[entry].id << " row " << row << endl;
                        #endif
                        for (column=0; column<COLUMNS; ++column) { 
                            *addr_error=DHS_CNTL_MEMIF_DATA[which_ss]+column; // words 
                            *exp_data = RED_RAND & MASK; 
                            if ((ret_val=red_reg_wr(inst, (*addr_error)*4, *exp_data))!=0) {
                                return ret_val;
                            }
                        } 
                    }
                }
            }
        }
        {   RED_SRAND(seed); //seed random generator again 
            for( which_ss=0; which_ss<RED_NUM_SS; ++which_ss) {
                if ( !(ss_bitmap&(1<<which_ss))) {
                        continue;
                }
                // now read all the tables
                for (entry=0; entry<(sizeof(table)/sizeof(table_entry)); ++entry) {

                    if ( !(cntl_bitmap&(1<<entry))) {
                            continue;
                    }

                    // set the id in the CFG register to point to the desired data structure
                    *addr_error=CFG_CNTL_MEMIF[which_ss];
                    if ((ret_val=red_reg_wr(inst, (*addr_error)*4, table[entry].id))!=0) {
                        return ret_val;
                    }

                    // reset the starting address
                    *addr_error=DHS_CNTL_MEMIF_ADDR[which_ss];
                    if ((ret_val=red_reg_wr(inst, (*addr_error)*4, 0))!=0) {
                        return ret_val;
                    }

                    const unsigned int MASK = (1<<table[entry].width)-1;

                    for (row=0; row<table[entry].depth; ++row) {
                        #ifdef RED_VERIFICATION
                                NU_LOG << " checking device " << table[entry].id << " row " << row << endl;
                        #endif
                        for (column=0; column<COLUMNS;++column) { 
                            *addr_error=DHS_CNTL_MEMIF_DATA[which_ss]+column;  // words
                            *exp_data = RED_RAND & MASK; 
                            if ((ret_val=red_reg_rd(inst, *addr_error*4, got_data))!=0) {
                                return ret_val;
                            }
                            if (*got_data!=*exp_data) { 
				printf("[0x%X] : 0x%X 0x%X row=%d column=%d\n",
					*addr_error << 2, *exp_data, *got_data, row, column);
                                *addr_error=DHS_CNTL_MEMIF_DATA[which_ss]+row*COLUMNS+column;  // words
                                return -1;
                            }
                        }
                    } 
                }
            }
        }
        return 0; /*success*/
}
