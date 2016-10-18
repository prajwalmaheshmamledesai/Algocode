//////////////////////////////////////////////////////////////////
// $Author: pkaplan $
// $Revision: 1.2 $
//
// A test to scrub MAC STAT DHS 
//////////////////////////////////////////////////////////////////
#include "red_hw_fcoe_cnt_dhs_test.h"
#include "red_hw_error.h"
#include "red_red_naxislave_define.h"

#ifdef RED_VERIFICATION
#include "msgFormat.h"
#include "Knob.h"
#define RED_SRAND(seed) RandKnob my_rand(#seed)
#define RED_RAND my_rand.eval()
#else
#include <stdlib.h>
#define RED_SRAND(seed) srand(seed)  
#define RED_RAND rand()
#endif

////////////////////////////////////////////////////////////////////////
// externally defined symbols
////////////////////////////////////////////////////////////////////////
extern int red_reg_wr(int inst, unsigned int address, unsigned int  data);  // address is byte aligned
extern int red_reg_rd(int inst, unsigned int address, unsigned int* data); // address is byte aligned

//////////////////////////////////////////////////////
// Although the DHS specifies a width of 36 bits
// the implementation only has 36 bits for octet counts
// which fall in the even rows.
// Odd rows are packet counts which are 32 bits wide
//////////////////////////////////////////////////////
const static unsigned int MASK_TABLE[] = 
        { ~0, 0xf // even row: 36 bits
        , ~0, 0   // odd row: 32 bits
};


// NI is asymmetrical: only RX has fcoe counters. Here we define symbos to easy our table driven approach belox
#define RED_NI0_DHS_TX_FCOE_CNT ~0
#define RED_NI1_DHS_TX_FCOE_CNT ~0
#define RED_NI2_DHS_TX_FCOE_CNT ~0
#define RED_NI3_DHS_TX_FCOE_CNT ~0
#define RED_NI_DHS_TX_FCOE_CNT_ARRAYSIZE ~0
#define RED_NI_DHS_TX_FCOE_CNT_ARRAYSKIP  ~0
#define RED_NI_DHS_TX_FCOE_CNT_SZ  ~0

/**********************************************************
A test that scrubs the FCOE CNT DHS
writes a bunch of random values to all memories behind the DHS
read back those values
\param seed used to initialize random generator
\param dhs_bitmap a bit per DHS: 0=HI0 RX, 1=HI0 TX, 2=H1 RX, 3=HI1 TX ... 16=NI0 RX, 17=NI1 RX, 24=BI RX, 25=BI TX, 26=CI RX, 27=CI TX
\param addr_error returns the address where the first error occurred (byte aligned)
\param exp_data returns the data that was expected
\param got_data returns the data that was received
\return 0 on success, an error code otherwise
**********************************************************/
int red_hw_fcoe_cnt_dhs_test (int inst, const int seed, const int dhs_bitmap, unsigned int* addr_error, unsigned  int* exp_data, unsigned  int* got_data) {
        enum    { HI=0
                , NI=1
                , BI=2
                , CI=3
        };

        typedef struct {
                unsigned int num_rows;
                unsigned int num_cols;
        } block_table_entry;

        enum    { RX=0
                , TX=4
        };

        const block_table_entry block_table[] = 
                        // num_rows                     num_cols                        
                { { RED_HI_DHS_RX_FCOE_CNT_ARRAYSIZE, RED_HI_DHS_RX_FCOE_CNT_ARRAYSKIP }
                , { RED_NI_DHS_RX_FCOE_CNT_ARRAYSIZE, RED_NI_DHS_RX_FCOE_CNT_ARRAYSKIP }
                , { RED_BI_DHS_RX_FCOE_CNT_ARRAYSIZE, RED_BI_DHS_RX_FCOE_CNT_ARRAYSKIP }
                , { RED_CI_DHS_RX_FCOE_CNT_ARRAYSIZE, RED_CI_DHS_RX_FCOE_CNT_ARRAYSKIP }
                , { RED_HI_DHS_TX_FCOE_CNT_ARRAYSIZE, RED_HI_DHS_TX_FCOE_CNT_ARRAYSKIP }
                , { RED_NI_DHS_TX_FCOE_CNT_ARRAYSIZE, RED_NI_DHS_TX_FCOE_CNT_ARRAYSKIP }
                , { RED_BI_DHS_TX_FCOE_CNT_ARRAYSIZE, RED_BI_DHS_TX_FCOE_CNT_ARRAYSKIP }
                , { RED_CI_DHS_TX_FCOE_CNT_ARRAYSIZE, RED_CI_DHS_TX_FCOE_CNT_ARRAYSKIP }
        };

        typedef struct {
                char* name;
                unsigned int base;
                unsigned int block; 
                unsigned int direction;
        } dhs_table_entry;

        #define ENTRY(name,number) \
                 { #name#number , RED_##name##number##_DHS_RX_FCOE_CNT, name, RX } \
                ,{ #name#number , RED_##name##number##_DHS_TX_FCOE_CNT, name, TX } 

        const dhs_table_entry dhs_table[] = 
                { ENTRY(HI,0)
                , ENTRY(HI,1)
                , ENTRY(HI,2)
                , ENTRY(HI,3)
                , ENTRY(HI,4)
                , ENTRY(HI,5)
                , ENTRY(HI,6)
                , ENTRY(HI,7)

                , ENTRY(NI,0)
                , ENTRY(NI,1)
                , ENTRY(NI,2)
                , ENTRY(NI,3)

                , ENTRY(BI,0)

                , ENTRY(CI,0)
        };

        int ret_val;

        unsigned int row, column, which_dhs=0;

        {   RED_SRAND(seed); //seed random generator again 
            for(which_dhs=0; which_dhs<sizeof(dhs_table)/sizeof(dhs_table_entry); ++which_dhs) {

                if ( !(dhs_bitmap&(1<<which_dhs))) {
                        continue;
                }

                if (dhs_table[which_dhs].base == (unsigned int) ~0) {
                        continue; // dummy entry in table
                }


                const unsigned int which_block  = dhs_table[which_dhs].block
                                                + dhs_table[which_dhs].direction
                                                ;

                // write a bunch of garbage into the device
                for (row=0; row<block_table[which_block].num_rows; ++row) {
                            #ifdef RED_VERIFICATION
                                    NU_LOG << " filling device " << dhs_table[which_dhs].name << " row " << row << endl;
                            #endif
                            for (column=0; column<block_table[which_block].num_cols; ++column) { 
                                *addr_error = dhs_table[which_dhs].base
                                            + block_table[which_block].num_cols*row
                                            + column; // words 
                                *exp_data = RED_RAND;
                                if ((ret_val=red_reg_wr(inst, *addr_error*4, *exp_data))!=0) {
                                    return ret_val;
                                }
                            }
                }
            }
         }
         {  RED_SRAND(seed); //seed random generator again 
            for(which_dhs=0; which_dhs<sizeof(dhs_table)/sizeof(dhs_table_entry); ++which_dhs) {

                if ( !(dhs_bitmap&(1<<which_dhs))) {
                        continue;
                }

                if (dhs_table[which_dhs].base == (unsigned int)~0) {
                        continue; // dummy entry in table
                }

                const unsigned int which_block = dhs_table[which_dhs].block;

                for (row=0; row<block_table[which_block].num_rows; ++row) {
                    #ifdef RED_VERIFICATION
                            NU_LOG << " checking device " << dhs_table[which_dhs].name << " row " << row << " num_cols=" 
                                   << block_table[which_block].num_cols
                                   << endl;
                    #endif
                    for (column=0; column<block_table[which_block].num_cols; ++column) {
                        *addr_error = dhs_table[which_dhs].base
                                    + block_table[which_block].num_cols * row
                                    + column; // words 
                        *exp_data = RED_RAND & MASK_TABLE[(row&1)*2+column]; 

                        if ((ret_val=red_reg_rd(inst, (*addr_error)*4, got_data))!=0) {
                            return ret_val;
                        }

                        if ((*got_data) != (*exp_data)) { 
                            return REDWOOD_ERROR_DEFAULT;
                        }

                    }
                } 
            }
        }
        return 0; /*success*/
}
