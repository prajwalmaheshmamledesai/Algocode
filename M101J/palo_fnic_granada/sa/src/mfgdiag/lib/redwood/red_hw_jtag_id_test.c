//////////////////////////////////////////////////////////////////
// $Author: pkaplan $
// $Revision: 1.1 $
//
// A test to read and check the jtag id register
//////////////////////////////////////////////////////////////////
#include "red_hw_error.h"
#include "red_hw_globals.h"
#include "red_red_naxislave_define.h"

#ifdef RED_VERIFICATION
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

/**********************************************************
A test that reads the JTAG ID register and checks that it has the expected value
\param seed used to initialize random generator
\param addr_error returns the address where the first error occurred (word aligned)
\param exp_data returns the data that was expected
\param got_data returns the data that was received
\return 0 on success, an error code otherwise
**********************************************************/
int red_hw_jtag_id_test (int inst, const int seed, unsigned int* addr_error,unsigned  int* exp_data,unsigned  int* got_data) {
        int ret_val;

        *addr_error=RED_MISC_JTAG_ID;
        *exp_data=RED_JTAG_ID;

        if ((ret_val=red_reg_rd(inst, (*addr_error)*4, got_data))!=0) {
            return ret_val;
        }


        if ((*got_data != RED_JTAG_ID) && (*got_data != RED_JTAG_ID_REV_B)) {
            return REDWOOD_ERROR_DEFAULT; /*success*/
        }

        return 0; /*success*/
}
