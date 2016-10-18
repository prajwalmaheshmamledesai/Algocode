
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <sys/types.h>
#include "red_red_naxislave_define.h"
#include "red_hw_lib.h"

enum { PORTS=5, FRHS=4 };

const static uint32_t sta_mcast_offset[PORTS] = 
        { RED_SS_STA_MCAST0_OFFSET
        , RED_SS_STA_MCAST1_OFFSET
        , RED_SS_STA_MCAST2_OFFSET
        , RED_SS_STA_MCAST3_OFFSET
        , RED_SS_STA_MCAST4_OFFSET
};

const static uint32_t sta_frh_port_offset[FRHS*PORTS] = 
        { RED_SS_STA_FRH0_PORT0_OFFSET
        , RED_SS_STA_FRH1_PORT0_OFFSET
        , RED_SS_STA_FRH2_PORT0_OFFSET
        , RED_SS_STA_FRH3_PORT0_OFFSET

        , RED_SS_STA_FRH0_PORT1_OFFSET
        , RED_SS_STA_FRH1_PORT1_OFFSET
        , RED_SS_STA_FRH2_PORT1_OFFSET
        , RED_SS_STA_FRH3_PORT1_OFFSET

        , RED_SS_STA_FRH0_PORT2_OFFSET
        , RED_SS_STA_FRH1_PORT2_OFFSET
        , RED_SS_STA_FRH2_PORT2_OFFSET
        , RED_SS_STA_FRH3_PORT2_OFFSET

        , RED_SS_STA_FRH0_PORT3_OFFSET
        , RED_SS_STA_FRH1_PORT3_OFFSET
        , RED_SS_STA_FRH2_PORT3_OFFSET
        , RED_SS_STA_FRH3_PORT3_OFFSET

        , RED_SS_STA_FRH0_PORT4_OFFSET
        , RED_SS_STA_FRH1_PORT4_OFFSET
        , RED_SS_STA_FRH2_PORT4_OFFSET
        , RED_SS_STA_FRH3_PORT4_OFFSET
};

const static uint32_t ss_base_addr[] = 
        { RED_SS0_BASE_ADDR
        , RED_SS1_BASE_ADDR
        , RED_SS2_BASE_ADDR
        , RED_SS3_BASE_ADDR
};

static int single_ss_eos_checks(int inst, uint32_t which_ss, uint32_t max_cells, uint8_t free_cell_fifo_check,
		unsigned int* addr_error, unsigned  int* exp_data, unsigned  int* got_data) 
{
        uint32_t port, ss_base = ss_base_addr[which_ss];
	uint32_t read_data, entry, cell, tmp_fc_ptr;
	uint32_t wr_ptr, rd_ptr, ptr_diff;
	uint32_t bmap_sz = (max_cells>>3) + ((max_cells & 0x07) ? 1 : 0);
	uint8_t *pbitmap = malloc(bmap_sz);

	if (!pbitmap) {
		printf(" Malloc for size %d failed\n", bmap_sz);
		return -1;
	} 

	memset(pbitmap, 0, bmap_sz);

        // check busy registers
        for (port=0; port<sizeof(sta_frh_port_offset)/sizeof(uint32_t); ++port) {
                red_reg_rd(inst, BA(ss_base+sta_frh_port_offset[port]), &read_data);
                if (read_data!=0) {
			printf("RED_SS%d_STA_FRH%d_PORT%d busy count = 0x%X\n", 
				which_ss, port/PORTS, port%PORTS, read_data);

			*addr_error = BA(ss_base+sta_frh_port_offset[port]);
			*exp_data   = 0x00;
			*got_data   = read_data;
			return (-2);
                }
        }
        for (port=0; port<sizeof(sta_mcast_offset)/sizeof(uint32_t); ++port) {
                red_reg_rd(inst, BA(ss_base+sta_mcast_offset[port]), &read_data);
                if (read_data!=0) {
                        printf(" ERROR: RED_SS%d_STA_MCAST%d busy count=0x%X",
                                which_ss, port/PORTS, read_data);
			*addr_error = BA(ss_base+sta_mcast_offset[port]);
			*exp_data   = 0x00;
			*got_data   = read_data;
			return (-3);
                }
        }
        
        // read FC read and write pointers, make sure the difference is correct
        red_reg_rd(inst, BA(ss_base+RED_SS_STA_FC_PTR_OFFSET), &tmp_fc_ptr);

        wr_ptr   = tmp_fc_ptr & 0x1FFF; 
        rd_ptr   = (tmp_fc_ptr >> 13) & 0x1FFF;
        ptr_diff = (wr_ptr>rd_ptr) ? wr_ptr-rd_ptr : max_cells+wr_ptr-rd_ptr;

        if (ptr_diff != max_cells) {
		
                printf(" ERROR: RED_SS%d_STA_FC_PTR: wr_ptr=0x%X rd_ptr=0x%X"
			"difference is 0x%X should be 0x%X\n",	
                        which_ss, wr_ptr, rd_ptr, ptr_diff, max_cells);
                *addr_error = BA(ss_base+RED_SS_STA_FC_PTR_OFFSET);
                *exp_data   = max_cells;
                *got_data   = ptr_diff;

		return (-4);
        }

        if (free_cell_fifo_check) {
            // check that the contents of the FC sram are uniq
            red_reg_wr(inst, BA(ss_base+RED_SS_CFG_CNTL_MEMIF_OFFSET), 0); // select free cell fifo

            red_reg_wr(inst, BA(ss_base+RED_SS_DHS_CNTL_MEMIF_ADDR_OFFSET) ,0);
            red_reg_wr(inst, BA(ss_base+RED_SS_DHS_CNTL_MEMIF_CMD_OFFSET),
			(1<<RED_SS_DHS_CNTL_MEMIF_CMD_FLD_AUTORD) |
			(1<<RED_SS_DHS_CNTL_MEMIF_CMD_FLD_AUTOINC));

            for (entry=0; entry<max_cells; ++entry) {
                    red_reg_rd(inst, BA(ss_base+RED_SS_DHS_CNTL_MEMIF_DATA_OFFSET), &cell); 

		    if ((cell >> 3) > bmap_sz) {
			printf(" Invalid Cell 0x%X\n", cell);
			*addr_error = BA(ss_base+RED_SS_DHS_CNTL_MEMIF_DATA_OFFSET); 
			*exp_data   = bmap_sz;
			*got_data   = cell>>3;
			return (-5);
		    }
		    if (pbitmap[cell >> 3] & (0x01 << (cell & 0x07))) {
			printf(" Duplicate Cell 0x%X\n", cell);
			*addr_error = cell; 
			*exp_data   = 0x00;
			*got_data   = cell;
			return (-6);
		    } else {
		    	pbitmap[cell >> 3] |= (0x01 << (cell & 0x07));
		    } 
            }
        }
	free(pbitmap);
	return (0);
}

static void ss_fc_release(int inst, uint8_t which_ss, uint8_t release)
{
	uint32_t data, port, addr;
	uint32_t ss_addr[] = {
				RED_SS0_CFG_WR_PORT0,
				RED_SS1_CFG_WR_PORT0,
				RED_SS2_CFG_WR_PORT0,
				RED_SS3_CFG_WR_PORT0,
			  };

	if (which_ss > 3) {
		printf(" %s:%d - invalid ss%d\n", __func__, __LINE__, which_ss);
		return;
	}

	for (port = 0, addr = ss_addr[which_ss]; port <= 4; 
	     addr += (RED_SS0_CFG_WR_PORT1 - RED_SS0_CFG_WR_PORT0), port++) {

		red_reg_rd(inst, BA(addr), &data); 
		if (release) {
			data &= ~(0x01 << RED_SS_CFG_WR_PORT1_FLD_EN);
		} else {
			data |= (0x01 << RED_SS_CFG_WR_PORT1_FLD_EN);
		}
		red_reg_wr(inst, BA(addr), data); 
	}
}

int redwood_ss_eos_checks(int inst, unsigned int* addr_error, unsigned  int* exp_data, unsigned  int* got_data) 
{
	int rc = 0;
	uint32_t which_ss;
	uint32_t max_cells;

        for (which_ss=0; !rc && which_ss<sizeof(ss_base_addr)/sizeof(uint32_t); ++which_ss) {
		printf ("  SS%d Free list check\n", which_ss);
		ss_fc_release(inst, which_ss, 1);

		red_reg_rd(inst, BA(RED_SS0_CFG_FC + (which_ss*(RED_SS1_CFG_FC-RED_SS0_CFG_FC))), 
				&max_cells);
		max_cells = (max_cells >> 1) & 0x1FFF;
		printf ("  Check Free list max_cells = %d.\n", max_cells);
                rc = single_ss_eos_checks(inst, which_ss, max_cells, 1, addr_error, exp_data, got_data);
		printf ("  SS%d Free list check %s\n", which_ss, rc ? "Failed" : "Passed");
		ss_fc_release(inst, which_ss, 0);
        }

	return (rc);
}

static int single_ss_eos_init(int inst, uint32_t which_ss, uint32_t max_cells, uint8_t free_cell_fifo_init,
				uint32_t fl_start, uint32_t fl_end)
{
        uint32_t port, ss_base = ss_base_addr[which_ss];
	uint32_t read_data, entry, tmp_fc_ptr;
	uint32_t wr_ptr, rd_ptr, ptr_diff;

        // check busy registers
        for (port=0; port<sizeof(sta_frh_port_offset)/sizeof(uint32_t); ++port) {
                red_reg_rd(inst, BA(ss_base+sta_frh_port_offset[port]), &read_data);
                if (read_data!=0) {
			printf("RED_SS%d_STA_FRH%d_PORT%d busy count = 0x%X\n", 
				which_ss, port/PORTS, port%PORTS, read_data);

			return (-2);
                }
        }
        for (port=0; port<sizeof(sta_mcast_offset)/sizeof(uint32_t); ++port) {
                red_reg_rd(inst, BA(ss_base+sta_mcast_offset[port]), &read_data);
                if (read_data!=0) {
                        printf(" ERROR: RED_SS%d_STA_MCAST%d busy count=0x%X",
                                which_ss, port/PORTS, read_data);
			return (-3);
                }
        }
        
        // read FC read and write pointers, make sure the difference is correct
        red_reg_rd(inst, BA(ss_base+RED_SS_STA_FC_PTR_OFFSET), &tmp_fc_ptr);

        wr_ptr   = tmp_fc_ptr & 0x1FFF; 
        rd_ptr   = (tmp_fc_ptr >> 13) & 0x1FFF;
        ptr_diff = (wr_ptr>rd_ptr) ? wr_ptr-rd_ptr : max_cells+wr_ptr-rd_ptr;

        if (ptr_diff != max_cells) {
		
                printf(" ERROR: RED_SS%d_STA_FC_PTR: wr_ptr=0x%X rd_ptr=0x%X"
			"difference is 0x%X should be 0x%X\n",	
                        which_ss, wr_ptr, rd_ptr, ptr_diff, max_cells);
		return (-4);
        }

        if (free_cell_fifo_init) {
            // check that the contents of the FC sram are uniq
            red_reg_wr(inst, BA(ss_base+RED_SS_CFG_CNTL_MEMIF_OFFSET), 0); // select free cell fifo

            red_reg_wr(inst, BA(ss_base+RED_SS_DHS_CNTL_MEMIF_ADDR_OFFSET) ,0);
            red_reg_wr(inst, BA(ss_base+RED_SS_DHS_CNTL_MEMIF_CMD_OFFSET),
			(1<<RED_SS_DHS_CNTL_MEMIF_CMD_FLD_AUTORD) |
			(1<<RED_SS_DHS_CNTL_MEMIF_CMD_FLD_AUTOINC));

            for (entry=0; entry<max_cells; ++entry) {
                    red_reg_wr(inst, BA(ss_base+RED_SS_DHS_CNTL_MEMIF_DATA_OFFSET), entry+fl_start); 
            }
        }
	return (0);
}


int redwood_ss_fl_init(int inst, uint32_t fl_start, uint32_t fl_end)
{
	int rc = 0;
	uint32_t which_ss;
	uint32_t data, max_cells = fl_end-fl_start;

        for (which_ss=0; !rc && which_ss<sizeof(ss_base_addr)/sizeof(uint32_t); ++which_ss) {
		printf ("  RDW%d SS%d Custom Init\n", inst, which_ss);
		ss_fc_release(inst, which_ss, 1);

		red_reg_rd(inst, BA(RED_SS0_CFG_FC + (which_ss*(RED_SS1_CFG_FC-RED_SS0_CFG_FC))), 
				&data);
		data = (data & 1) | (data & (0x01  <<14)) | ((max_cells-1) << 1);
		red_reg_wr(inst, BA(RED_SS0_CFG_FC + (which_ss*(RED_SS1_CFG_FC-RED_SS0_CFG_FC))), 
				data);

		red_reg_wr(inst, BA(RED_SS0_WRO_FC + (which_ss*(RED_SS1_WRO_FC-RED_SS0_WRO_FC))), 
				(max_cells << 13)); 
//				(max_cells << 13) | (max_cells));

		printf ("  Program Free list max_cells = %d.\n", max_cells);
                rc = single_ss_eos_init(inst, which_ss, max_cells, 1, fl_start, fl_end);
		ss_fc_release(inst, which_ss, 0);
        }

	return (rc);
}
