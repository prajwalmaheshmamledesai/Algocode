//////////////////////////////////////////////////////////////////
// $Author: pkaplan $
// $Revision: 1.2 $
//
// A test to check read-only-ness and accessibility of the OQ DHS in SS. 
// Test assumes out of reset condition (all registers return 0)
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
#define RED_SRAND(seed) srand(seed)  
#define RED_RAND rand()
#endif

////////////////////////////////////////////////////////////////////////
// externally defined symbols
////////////////////////////////////////////////////////////////////////
extern int red_reg_wr(int inst, unsigned int address, unsigned int  data);  // address is byte aligned
extern int red_reg_rd(int inst, unsigned int address, unsigned int* data); // address is byte aligned

typedef struct {
        unsigned int num_rows; 
        unsigned int num_columns;  // words
        unsigned int width; // bits
} oq_entry_t;

const static oq_entry_t oq_table[] = 
        { { RED_SS_DHS_OQ0_STATUS_ARRAYSIZE, RED_SS_DHS_OQ0_STATUS_ARRAYSKIP, RED_SS_DHS_OQ0_STATUS_SZ }
        , { RED_SS_DHS_OQ1_STATUS_ARRAYSIZE, RED_SS_DHS_OQ1_STATUS_ARRAYSKIP, RED_SS_DHS_OQ1_STATUS_SZ }
        , { RED_SS_DHS_OQ2_STATUS_ARRAYSIZE, RED_SS_DHS_OQ2_STATUS_ARRAYSKIP, RED_SS_DHS_OQ2_STATUS_SZ }
        , { RED_SS_DHS_OQ3_STATUS_ARRAYSIZE, RED_SS_DHS_OQ3_STATUS_ARRAYSKIP, RED_SS_DHS_OQ3_STATUS_SZ }
        , { RED_SS_DHS_OQ4_STATUS_ARRAYSIZE, RED_SS_DHS_OQ4_STATUS_ARRAYSKIP, RED_SS_DHS_OQ4_STATUS_SZ }
};

#define DHS_ENTRY(ss,oq) \
        RED_SS##ss##_DHS_OQ##oq##_STATUS

const static unsigned int dhs_table[RED_NUM_SS_OQ*RED_NUM_SS] = 
                // ss oq
        { DHS_ENTRY(0,0)
        , DHS_ENTRY(1,0)
        , DHS_ENTRY(2,0)
        , DHS_ENTRY(3,0)

        , DHS_ENTRY(0,1)
        , DHS_ENTRY(1,1)
        , DHS_ENTRY(2,1)
        , DHS_ENTRY(3,1)

        , DHS_ENTRY(0,2)
        , DHS_ENTRY(1,2)
        , DHS_ENTRY(2,2)
        , DHS_ENTRY(3,2)

        , DHS_ENTRY(0,3)
        , DHS_ENTRY(1,3)
        , DHS_ENTRY(2,3)
        , DHS_ENTRY(3,3)

        , DHS_ENTRY(0,4)
        , DHS_ENTRY(1,4)
        , DHS_ENTRY(2,4)
        , DHS_ENTRY(3,4)
};
// helper to retrieve a register word address based on ss and oq
static int get_dhs_oq_status_reg(const unsigned int which_ss, const unsigned int which_oq) {
        if (which_ss>=RED_NUM_SS) {
                return -1;
        }
        if (which_oq>=RED_NUM_SS_OQ) {
                return -1;
        }
        return dhs_table[which_oq*RED_NUM_SS +  which_ss];
}

/**********************************************************
A test that scrubs the OQ DHS in SS
writes a bunch of random values to all memories behind the DHS
read back those values
\param seed used to initialize random generator
\param ss_bitmap a bit per SS  (bit 0=ss0, bit 1=ss1 ...)
\param oq_bitmap a bit per function  (bit 0=oq0, bit 1=oq1 ...)
\param addr_error returns the address where the first error occurred (byte aligned)
\param exp_data returns the data that was expected
\param got_data returns the data that was received
\return 0 on success, an error code otherwise
**********************************************************/
int red_hw_ss_oq_dhs_test (int inst, const int seed, const int ss_bitmap, const int oq_bitmap, unsigned int* addr_error, unsigned  int* exp_data, unsigned  int* got_data) {
        int ret_val;
        unsigned int row, column;
	unsigned int which_ss, which_oq;

        {   RED_SRAND(seed); // fill-up phase, seed random generator 
            for( which_ss=0; which_ss<RED_NUM_SS; ++which_ss) {

                if ( !(ss_bitmap&(1<<which_ss))) {
                        continue;
                }


                for (which_oq=0; which_oq<RED_NUM_SS_OQ; ++which_oq) {

                    if ( !(oq_bitmap&(1<<which_oq))) {
                            continue;
                    }

                    const int base_word_addr = get_dhs_oq_status_reg(which_ss, which_oq);
                
                    if (base_word_addr==-1) { // sanity check
                        return REDWOOD_ERROR_DEFAULT;
                    }

                    // write a bunch of garbage into the device
                    for (row=0; row<oq_table[which_oq].num_rows; ++row) {
                        for (column=0; column<oq_table[which_oq].num_columns; ++column) { 
                            *addr_error = base_word_addr 
                                        + row*oq_table[which_oq].num_columns 
                                        + column; // words 
                            *exp_data = RED_RAND;
                            #ifdef RED_VERIFICATION
                                    NU_LOG << " filling addr " << *addr_error << " row " << row << " column " << column << endl;
                            #endif
                            if ((ret_val=red_reg_wr(inst, (*addr_error)*4, *exp_data))!=0) {
                                return ret_val;
                            }
                        } 
                    }
                }
            }
        }
        {   RED_SRAND(seed); //read & check phase: seed random generator again 
            for( which_ss=0; which_ss<RED_NUM_SS; ++which_ss) {

                if ( !(ss_bitmap&(1<<which_ss))) {
                        continue;
                }

                for (which_oq=0; which_oq<RED_NUM_SS_OQ; ++which_oq) {

                    if ( !(oq_bitmap&(1<<which_oq))) {
                            continue;
                    }

                    const int base_word_addr = get_dhs_oq_status_reg(which_ss, which_oq);

                    if (base_word_addr==-1) { // sanity check
                        return REDWOOD_ERROR_DEFAULT;
                    }

                    for (row=0; row<oq_table[which_oq].num_rows; ++row) {
                        for (column=0; column<oq_table[which_oq].num_columns; ++column) { 
                            *addr_error = base_word_addr
                                        + row*oq_table[which_oq].num_columns 
                                        + column;  // words
                            *exp_data = 0;// read only
                            #ifdef RED_VERIFICATION
                                    NU_LOG << " checking addr " << *addr_error << " row " << row << " column " << column << endl;
                            #endif
                            if ((ret_val=red_reg_rd(inst, (*addr_error)*4, got_data))!=0) {
                                return ret_val;
                            }
                            if (*got_data!=*exp_data) { 
                                return -1;
                            }
                        }
                    } 
                }
            }
        }
        return 0; /*success*/
}
