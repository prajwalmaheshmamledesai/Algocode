//////////////////////////////////////////////////////////////////
// $Author: pkaplan $
// $Revision: 1.2 $
//
// A test to scrub MAC STAT DHS 
//////////////////////////////////////////////////////////////////
#include "red_hw_mac_stat_dhs_test.h"
#include "red_hw_error.h"
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

//////////////////////////////////////////////////////
// helper to construct a mask given a width
//////////////////////////////////////////////////////
static void red_hw_mac_stat_dhs_test_make_mask (unsigned int* mask, int mask_bits) {
	int column;
        for (column=0; mask_bits>=0; ++column) {
                    mask[column] = (mask_bits>=32) ? ~0 : (1<<mask_bits)-1;
                    mask_bits -= 32;
#ifdef RED_VERIFICATION
                    NU_LOG << " column="<<column << " mask="<< hex << mask[column] << dec << endl;
#endif
        }
}

/**********************************************************
A test that scrubs the MAC stat DHS
writes a bunch of random values to all memories behind the DHS
read back those values
\param seed used to initialize random generator
\param dhs_bitmap a bit per DHS: 0-7 are HI0-7, 8-11 are NI0-3, 12 is BI and 13 is CI
\param addr_error returns the address where the first error occurred (byte aligned)
\param exp_data returns the data that was expected
\param got_data returns the data that was received
\return 0 on success, an error code otherwise
**********************************************************/
int red_hw_mac_stat_dhs_test (int inst, const int seed, const int dhs_bitmap, unsigned int* addr_error, unsigned  int* exp_data, unsigned  int* got_data) {
        enum    { HI=0
                , NI=1
                , BI=2
                , CI=3
        };

        typedef struct {
                unsigned int num_rows;
                unsigned int num_cols;
                int width;
        } block_table_entry;

        // NOTE: the order of this table must match the enum above
        const block_table_entry block_table[] = 
                        // num_rows                     num_cols                        width
                { { RED_HI_DHS_MAC_STAT_ARRAYSIZE, RED_HI_DHS_MAC_STAT_ARRAYSKIP, RED_HI_DHS_MAC_STAT_SZ }
                , { RED_NI_DHS_MAC_STAT_ARRAYSIZE, RED_NI_DHS_MAC_STAT_ARRAYSKIP, RED_NI_DHS_MAC_STAT_SZ }
                , { RED_BI_DHS_MAC_STAT_ARRAYSIZE, RED_BI_DHS_MAC_STAT_ARRAYSKIP, RED_BI_DHS_MAC_STAT_SZ }
                , { RED_CI_DHS_MAC_STAT_ARRAYSIZE, RED_CI_DHS_MAC_STAT_ARRAYSKIP, RED_CI_DHS_MAC_STAT_SZ }
        };

        typedef struct {
                char* name;
                unsigned int base;
                unsigned int block; // bits
        } dhs_table_entry;

        #define ENTRY(name,number) \
                { #name#number , RED_##name##number##_DHS_MAC_STAT, name }

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

                const unsigned int which_block = dhs_table[which_dhs].block;

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

                const unsigned int which_block = dhs_table[which_dhs].block;

                // construct the mask for this DHS
                unsigned int mask[block_table[which_block].num_cols];
                red_hw_mac_stat_dhs_test_make_mask (mask, block_table[which_block].width);

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
                        *exp_data = RED_RAND & mask[column]; 

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
