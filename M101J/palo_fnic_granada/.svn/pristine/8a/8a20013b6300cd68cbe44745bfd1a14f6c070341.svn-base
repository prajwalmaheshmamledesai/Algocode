//////////////////////////////////////////////////////////////////
// $Author: pkaplan $
// $Revision: 1.2 $
//
// A test to scrub bulk memory DHS 
//////////////////////////////////////////////////////////////////
#include "red_hw_bulk_dhs_test.h"
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
static unsigned int NUM_COLUMNS= (RED_SS_DHS_BULK_MEMIF_DATA_SZ+31)/32;

static unsigned int SS_ECC2_ERR[] = 
                { RED_SS0_INT_ERR_ECC2
                , RED_SS1_INT_ERR_ECC2
                , RED_SS2_INT_ERR_ECC2
                , RED_SS3_INT_ERR_ECC2
        };

static unsigned int SS_ECC1_ERR[] = 
                { RED_SS0_INT_ERR_ECC1
                , RED_SS1_INT_ERR_ECC1
                , RED_SS2_INT_ERR_ECC1
                , RED_SS3_INT_ERR_ECC1
        };

static unsigned int CFG_BULK_MEMIF[] = 
                { RED_SS0_CFG_BULK_MEMIF
                , RED_SS1_CFG_BULK_MEMIF
                , RED_SS2_CFG_BULK_MEMIF
                , RED_SS3_CFG_BULK_MEMIF
        };

static unsigned int DHS_BULK_MEMIF_CMD[] = 
                { RED_SS0_DHS_BULK_MEMIF_CMD
                , RED_SS1_DHS_BULK_MEMIF_CMD
                , RED_SS2_DHS_BULK_MEMIF_CMD
                , RED_SS3_DHS_BULK_MEMIF_CMD
        };

static unsigned int DHS_BULK_MEMIF_DATA[] = 
                { RED_SS0_DHS_BULK_MEMIF_DATA
                , RED_SS1_DHS_BULK_MEMIF_DATA
                , RED_SS2_DHS_BULK_MEMIF_DATA
                , RED_SS3_DHS_BULK_MEMIF_DATA
        };
static unsigned int DHS_BULK_MEMIF_ADDR[] = 
                { RED_SS0_DHS_BULK_MEMIF_ADDR
                , RED_SS1_DHS_BULK_MEMIF_ADDR
                , RED_SS2_DHS_BULK_MEMIF_ADDR
                , RED_SS3_DHS_BULK_MEMIF_ADDR
        };

/**********************************************************
A test that scrubs the bulk DHS memories
writes a bunch of random values to all memories behind the DHS
read back those values
\param seed used to initialize random generator
\param ss_bitmap a bit per SS: bit0=ss0, bit1=ss1 ...
\param dw_bitmap a bit per DW: bit0=word0, bit1=word 1 ... bit9=word 9
\param addr_error returns the address where the first error occurred (byte aligned)
\param exp_data returns the data that was expected
\param got_data returns the data that was received
\return 0 on success, an error code otherwise
**********************************************************/
int red_hw_bulk_dhs_test (int inst, const int seed, const unsigned int ss_bitmap, const unsigned int dw_bitmap, unsigned int* addr_error, unsigned  int* exp_data, unsigned  int* got_data) {
        int ret_val;

        unsigned int row, column, dw, which_ss;
        unsigned int mask[NUM_COLUMNS];
        const unsigned int NUM_ROWS= 4800; // max number of cells
        const unsigned int NUM_DWS = 10; // 80 bytes per cell (8 bytes per column x 10 columns)


        // create mask
        for (column=0; column<NUM_COLUMNS; ++column) {
                mask[column] = (column==(NUM_COLUMNS-1)) ? (1<<(RED_SS_DHS_BULK_MEMIF_DATA_SZ%32))-1 : ~0;
        }

        {   RED_SRAND(seed); //seed random generator 

            for (which_ss=0; which_ss<4; ++which_ss) {

                if ((ss_bitmap & (1<<which_ss))==0) {
                        continue;
                }

                // configure the MEMIF mode
                *addr_error = DHS_BULK_MEMIF_CMD[which_ss] ;
                *exp_data= (1<<12) // auto-write. this is undocumented: when the last column of a row is written the row is commited
                         | (1<<RED_SS_DHS_BULK_MEMIF_CMD_FLD_AUTOINC) 
                         | (1<<RED_SS_DHS_BULK_MEMIF_CMD_FLD_DIRACC) 
                         ;
                if ((ret_val=red_reg_wr(inst, (*addr_error)*4, *exp_data))!=0) {
                    return ret_val;
                }
            
                for (dw=0; dw<NUM_DWS; ++dw) {

                    if ((dw_bitmap & (1<<dw))==0) {
                            continue;
                    }

                    // choose the column, bypass ECC
                    *addr_error = CFG_BULK_MEMIF[which_ss];
                    *exp_data   = dw
                                | (1<<RED_SS_CFG_BULK_MEMIF_FLD_ECC_BYPASS)
                                ;
                    if ((ret_val=red_reg_wr(inst, (*addr_error)*4, *exp_data))!=0) {
                        return ret_val;
                    }

                    // reset row
                    *addr_error = DHS_BULK_MEMIF_ADDR[which_ss] ;
                    *exp_data= 0;
                    if ((ret_val=red_reg_wr(inst, (*addr_error)*4, *exp_data))!=0) {
                        return ret_val;
                    }

                    // write a bunch of garbage into the device
                    for (row=0; row<NUM_ROWS; ++row) {
                        #ifdef RED_VERIFICATION
                                NU_LOG << " writing row " << row << " word="<<dw << " ss="<<which_ss << endl;
                        #endif
                        for (column=0; column<NUM_COLUMNS; ++column) { 

                            *addr_error = DHS_BULK_MEMIF_DATA[which_ss]
                                        + column; // words 

                            *exp_data = RED_RAND;

                            if ((ret_val=red_reg_wr(inst, *addr_error*4, *exp_data))!=0) {
                                return ret_val;
                            }
                        }
                    }
		    printf("  Wr SS%d DW%d done\n", which_ss, dw);
                }
            }
        }

        // now read back what was written
        {   RED_SRAND(seed); //seed random generator again 

            for (which_ss=0; which_ss<4; ++which_ss) {

                if ((ss_bitmap & (1<<which_ss))==0) {
                        continue;
                }

                for (dw=0; dw<NUM_DWS; ++dw) {

                    if ((dw_bitmap & (1<<dw))==0) {
                            continue;
                    }

                    // configure the MEMIF mode
                    *addr_error = DHS_BULK_MEMIF_CMD[which_ss] ;
                    *exp_data= (1<<RED_SS_DHS_BULK_MEMIF_CMD_FLD_DIRACC);
                    if ((ret_val=red_reg_wr(inst, (*addr_error)*4, *exp_data))!=0) {
                        return ret_val;
                    }

                    // choose the column, bypass ECC
                    *addr_error = CFG_BULK_MEMIF[which_ss];
                    *exp_data   = dw
                                | (1<<RED_SS_CFG_BULK_MEMIF_FLD_ECC_BYPASS)
                                ;
                    if ((ret_val=red_reg_wr(inst, (*addr_error)*4, *exp_data))!=0) {
                        return ret_val;
                    }

                    // write a bunch of garbage into the device
                    for (row=0; row<NUM_ROWS; ++row) {

                        *addr_error = DHS_BULK_MEMIF_ADDR[which_ss] ;
                        *exp_data= row;
                        if ((ret_val=red_reg_wr(inst, (*addr_error)*4, *exp_data))!=0) {
                            return ret_val;
                        }

                        for (column=0; column<NUM_COLUMNS; ++column) { 
                            #ifdef RED_VERIFICATION
                                    NU_LOG << " checking row " << row << " word="<<dw << " ss="<<which_ss << " columns="<<column << endl;
                            #endif

                            *addr_error = DHS_BULK_MEMIF_DATA[which_ss]
                                        + column; // words 

                            *exp_data = RED_RAND & mask[column];

                            if ((ret_val=red_reg_rd(inst, (*addr_error)*4, got_data))!=0) {
                                return ret_val;
                            }
                            if ((*got_data)!=(*exp_data)) {
                                *addr_error = DHS_BULK_MEMIF_DATA[which_ss]
                                            + row * NUM_COLUMNS
                                            + column; // words 
                                return REDWOOD_ERROR_DEFAULT;
                            }
                        }
                    }
	 	    printf("  Rd SS%d DW%d done\n", which_ss, dw);
                }
            }
         }
        return 0; /*success*/
}



extern int diag_get_verbose();
int red_hw_bulk_dhs_write (int inst, uint8_t which_ss, uint32_t start, uint32_t data, uint32_t len)
{
	int rc = 0;
	uint32_t maddr, mdata; 
	uint32_t rdcnt, mloc;

	// Write Data.
	for (rdcnt = 0; rdcnt < 3; rdcnt++) {
		maddr = ((DHS_BULK_MEMIF_DATA[which_ss]+rdcnt) << 2);
        	if ((rc=red_reg_wr(inst, maddr, data))!=0) {
			return rc;
		}
	}

	for (mloc=start; mloc < (start+len); mloc++) {
		if ((mloc&0x7F) == 0x00)
			printf(" BULK Wr SS%d Loc=0x%X\n", which_ss, mloc);
		// Write mem pointer
		maddr = (DHS_BULK_MEMIF_ADDR[which_ss] << 2);
		mdata = mloc;
		if ((rc=red_reg_wr(inst, maddr, mdata))!=0) {
			return rc;
        	}

		// Immediate Write. 
		maddr = (DHS_BULK_MEMIF_CMD[which_ss]<<2);
		mdata = 0x8;
        	if ((rc=red_reg_wr(inst, maddr, mdata))!=0) {
            		return rc;
        	}
	}

	return (rc);
}

int red_hw_bulk_dhs_read (int inst, uint8_t which_ss, uint32_t mloc)
{
	int rc = 0;
	uint32_t maddr, mdata; 

	// Write mem pointer
	maddr = (DHS_BULK_MEMIF_ADDR[which_ss] << 2);
	mdata = mloc;
        if ((rc=red_reg_wr(inst, maddr, mdata))!=0) {
            return rc;
        }

	// Immediate Read. 
	maddr = (DHS_BULK_MEMIF_CMD[which_ss]<<2);
	mdata = 0x10;
        if ((rc=red_reg_wr(inst, maddr, mdata))!=0) {
            return rc;
        }

	if (diag_get_verbose())
	{
	uint32_t data[3], rdcnt;
	for (rdcnt = 0; rdcnt < 3; rdcnt++) {
		maddr = ((DHS_BULK_MEMIF_DATA[which_ss]+rdcnt) << 2);
        	if ((rc=red_reg_rd(inst, maddr, &data[rdcnt]))!=0) {
			return rc;
		}
	}
	printf("[0x%X] : 0x%08X-%08X-%08X\n", mloc, data[0], data[1], data[2]);
	}
	return (rc);
}

int red_hw_bulk_dhs_rd_diag (int inst, uint32_t start, uint32_t slen, uint8_t which_ss, uint8_t which_col)
{
	int rc = 0;
	uint32_t cnt0, cnt1, maddr, mdata, ecc1, ecc2;

	maddr = (CFG_BULK_MEMIF[which_ss]<<2);
	mdata = (which_col);
        if ((rc=red_reg_wr(inst, maddr, mdata))!=0) {
            return rc;
        }

	for (cnt0 = start; cnt0 < (start+slen); cnt0++) {
		printf("  RED-%d Bulk %X Rd\n", inst, cnt0);
		for (cnt1 = start; cnt1 < (start+slen); cnt1++) {
			if ((cnt1 & 0x7F) == 0) {
				printf("      RED-%d Bulk %X Follow\n", inst, cnt1);
			}
			rc = red_hw_bulk_dhs_read (inst, which_ss, cnt0);
			if (rc) return (rc);

			rc = red_hw_bulk_dhs_read (inst, which_ss, cnt1);
			if (rc) return (rc);

			maddr = (SS_ECC1_ERR[which_ss]<<2);
        		if ((rc=red_reg_rd(inst, maddr, &ecc1))!=0) {
            			return rc;
        		}

			maddr = (SS_ECC2_ERR[which_ss]<<2);
        		if ((rc=red_reg_rd(inst, maddr, &ecc2))!=0) {
            			return rc;
        		}

			if (ecc1 || ecc2) {
				printf(" Rd 0x%X Followed by Rd 0x%X => ECC1=0x%X, ECC2=0x%X\n",
						cnt1, cnt0, ecc1, ecc2);

				maddr = (SS_ECC1_ERR[which_ss]<<2);
	        		if ((rc=red_reg_wr(inst, maddr, ecc1))!=0) {
       		     			return rc;
       		 		}

				maddr = (SS_ECC2_ERR[which_ss]<<2);
       		 		if ((rc=red_reg_wr(inst, maddr, ecc2))!=0) {
       		     			return rc;
       		 		}
			}
		}
	}
	return (rc);
}

int red_hw_bulk_dhs_wr_diag (int inst, uint32_t start, uint32_t slen, uint8_t which_ss, 
				uint8_t which_col, uint32_t data)
{
	int rc = 0;
	uint32_t maddr, mdata;

	// Select  column
	maddr = (CFG_BULK_MEMIF[which_ss]<<2);
	mdata = (which_col);
        if ((rc=red_reg_wr(inst, maddr, mdata))!=0) {
            return rc;
        }

	rc = red_hw_bulk_dhs_write (inst, which_ss, start, data, slen);
	if (rc) return (rc);

	return (rc);
}
