//////////////////////////////////////////////////////////////////
// $Author: pkaplan $
// $Revision: 1.2 $
//
// Use MEMIF's autotest feature to exercise SS's control memories
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
#include <stdlib.h>
#include "red_hw_lib.h"
#define RED_SRAND(seed) srand(seed)  
#define RED_RAND rand()
#endif


////////////////////////////////////////////////////////////////////////
// externally defined symbols
////////////////////////////////////////////////////////////////////////

/**********************************************************
A test that uses MEMIF's autotest feature to scrub the CNTL memories in SS
writes a bunch of random values to all memories behind the DHS
read back those values
\param seed used to initialize random generator
\param ss_bitmap a bit per SS  (bit 0=ss0, bit 1=ss1 ...)
\param cntl_bitmap a bit per memory  (bit 0=free cell, bit 1=frame list ...)
\param addr_error returns the address where the first error occurred (byte aligned)
\param exp_data returns the data that was expected
\param got_data returns the data that was received
\return 0 on success, an error code otherwise
**********************************************************/
int red_hw_cntl_memif_autotest (int inst, const int seed, const int ss_bitmap, const int cntl_bitmap, unsigned int* addr_error, unsigned  int* exp_data, unsigned  int* got_data) {
        int ret_val;
        unsigned int column, entry;
	unsigned int which_ss;
        const unsigned int NUM_COLUMNS = (RED_SS_DHS_CNTL_MEMIF_DATA_SZ+31)/32;
        const static unsigned int ERR_MASK  =(1<<RED_SS_DHS_CNTL_MEMIF_STA_FLD_AUTOVRFY_ERR);
        const static unsigned int DONE_MASK =(1<<RED_SS_DHS_CNTL_MEMIF_STA_FLD_AUTOTEST_DONE);

        typedef struct {
                char* name;
                unsigned int id;
                unsigned int width; // bits
                unsigned int depth;
        } table_entry;
        const unsigned int MAX_DEPTH=4800;
        const unsigned int DATA_WIDTH=32;
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

//        const static unsigned int DHS_CNTL_MEMIF_ADDR[] =
//                { RED_SS0_DHS_CNTL_MEMIF_ADDR
//                , RED_SS1_DHS_CNTL_MEMIF_ADDR
//                , RED_SS2_DHS_CNTL_MEMIF_ADDR
//                , RED_SS3_DHS_CNTL_MEMIF_ADDR
//        };

        const static unsigned int DHS_CNTL_MEMIF_DATA[] =
                { RED_SS0_DHS_CNTL_MEMIF_DATA
                , RED_SS1_DHS_CNTL_MEMIF_DATA
                , RED_SS2_DHS_CNTL_MEMIF_DATA
                , RED_SS3_DHS_CNTL_MEMIF_DATA
        };

        const static unsigned int DHS_CNTL_MEMIF_STA[] =
                { RED_SS0_DHS_CNTL_MEMIF_STA
                , RED_SS1_DHS_CNTL_MEMIF_STA
                , RED_SS2_DHS_CNTL_MEMIF_STA
                , RED_SS3_DHS_CNTL_MEMIF_STA
        };

        RED_SRAND(seed); //read & check phase: seed random generator again 

        for (which_ss=0; which_ss<RED_NUM_SS; ++which_ss) {

            if (!(ss_bitmap & (1<<which_ss))) {
                   continue;
            }

            for (entry=0; entry<(sizeof(table)/sizeof(table_entry)); ++entry) {

                if ( !(cntl_bitmap&(1<<entry))) {
                        continue;
                }
                int tries=50000;

                // setup the data 
                for (column=0; column<NUM_COLUMNS; ++column) {
                        *addr_error = DHS_CNTL_MEMIF_DATA[which_ss]+column;
                        *exp_data = RED_RAND; 
                        if ((ret_val=red_reg_wr(inst, (*addr_error)*4, *exp_data))!=0) {
                                return ret_val;
                        }
                }

                // set the id in the CFG register to point to the desired data structure
                *addr_error=CFG_CNTL_MEMIF[which_ss];
                *exp_data =0;
                set_field(exp_data, RED_SS_CFG_CNTL_MEMIF_FLD_COL_SEL, table[entry].id);
                set_field(exp_data, RED_SS_CFG_CNTL_MEMIF_FLD_END, table[entry].depth-1);
                set_bit  (exp_data, RED_SS_CFG_CNTL_MEMIF_FLD_ECC_BYPASS, 1);

                if ((ret_val=red_reg_wr (inst, (*addr_error)*4 , *exp_data))!=0) {
                        return ret_val;
                }

                *addr_error=DHS_CNTL_MEMIF_CMD[which_ss];
                *exp_data =0;
                set_bit  (exp_data, RED_SS_DHS_CNTL_MEMIF_CMD_FLD_AUTOTEST, 1);
                set_field(exp_data, RED_SS_DHS_CNTL_MEMIF_CMD_FLD_UNUSED_BITS, DATA_WIDTH-table[entry].width);

                if ((ret_val=red_reg_wr(inst, (*addr_error)*4 // byte aligned
                                                   , *exp_data
                                        ))!=0) {
                        return ret_val;
                }

                // poll for completion or error
                *addr_error = DHS_CNTL_MEMIF_STA[which_ss];
                do { 
                    if ((ret_val=red_reg_rd(inst, (*addr_error)*4, got_data))!=0) {
                        return ret_val;
                    }
                } while (!((*got_data) & ( ERR_MASK | DONE_MASK)) && tries--);

                if (tries<1) {
                        return REDWOOD_ERROR_MEMIF_AUTOTEST_TIMEOUT;  
                }

                // check for error
                if ((*got_data) & ERR_MASK) {
                        // NOTE: bug workaround: the STA read is really read+clear and
                        // there's a 2 cycle window where the error bit is set and the done bit is not
                        // so we need to check that we really are done when we read the error bit
                        for (tries=5; tries && (!((*got_data) & DONE_MASK)); --tries) {
                            if ((ret_val=red_reg_rd(inst, (*addr_error)*4, got_data))!=0) {
                                return ret_val;
                            }
                        }
                        if (!tries) {
                            return REDWOOD_ERROR_MEMIF_AUTOTEST_TIMEOUT;  
                        }
                        return REDWOOD_ERROR_MEMIF_AUTOVRFY_ERR;
                }
           }
        }

        return 0; /*success*/
}
