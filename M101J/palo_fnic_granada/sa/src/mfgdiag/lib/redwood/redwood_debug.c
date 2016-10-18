
#include "red_hw_lib.h"
#include "redwood_ss_stats.h"
#include "redwood_mac_rmon_stats.h"
#include "red_red_naxislave_define.h"

extern int verbose;
unsigned int ss_dhs_stats[4] = {
    RED_SS0_DHS_STATS,
    RED_SS1_DHS_STATS,
    RED_SS2_DHS_STATS,
    RED_SS3_DHS_STATS,
};

void redwood_ss_stats_dump(int asic_id, int ssx, uint8_t portmask)
{
    int i, j;
    unsigned int reg;
    unsigned int rd_ctr[12], wr_ctr[12];

    if ((ssx < 0) || (ssx > 3)) {
        return;
    }

    printf("SS%d statistics:\n", ssx);
    reg = ss_dhs_stats[ssx];

    /* read in the write port stats */
    for (i = 0; i < NUM_PORTS; i++) {
	if (!((0x01 << i) & portmask))
		continue;

	printf("\nDUMP SS %d Port %d\n", ssx, i);
	printf("------------------------------------------------------------------------\n");
        for (j = 0; j < 12; j++) {

            redwood_read_register_internal( asic_id,
                                            (BA(reg + (i*16) + j)),
                                            &wr_ctr[j],
                                            __FILE__, __LINE__, 1);

	    if (j < 11) {
		redwood_read_register_internal( asic_id,
			(BA(reg + (NUM_PORTS*16) + (i*16) + j)),
                        &rd_ctr[j], __FILE__, __LINE__, 1);
	    }
        }

        for (j = 0; j < 12; j++) {
            printf("%s%d: %d\t", wr_port_stat_names[j], i, wr_ctr[j]);
	    if (j < 11)
            	printf("%s%d: %d\n", rd_port_stat_names[j], i, rd_ctr[j]);
        }
	printf("\n");
    }
}

#define RED_HI_MAC_PCS_STATUS_CG_ALIGN  (0x0F << 10)
#define RED_HI_MAC_PCS_STATUS_SYNC  (0x0F << 14)
int redwood_10G_mac_status_get (int inst, uint8_t hif, uint8_t port, uint8_t *link, uint8_t *fault)
{
        uint32_t macdata, pcsdata, mask, addr;
        int      rc = 0;

        addr = hif ? BA(RED_HI0_MAC_STATUS) : BA(RED_NI0_MAC_STATUS);
        addr += (port * (BA(RED_HI1_MAC_STATUS) - BA(RED_HI0_MAC_STATUS)));

        rc = red_reg_rd (inst, addr, &macdata);
        if (rc) return (rc);

        addr = hif ? BA(RED_HI0_MAC_PCS_STATUS) : BA(RED_NI0_MAC_PCS_STATUS);
        addr += (port * (BA(RED_HI1_MAC_PCS_STATUS) - BA(RED_HI0_MAC_PCS_STATUS)));

        rc = red_reg_rd (inst, addr, &pcsdata);
        if (rc) return (rc);


        mask =  (0x01 << RED_HI_MAC_STATUS_FLD_LOC_FAULT) |
                (0x01 << RED_HI_MAC_STATUS_FLD_REM_FAULT);
        *fault = ((mask & macdata)) ? (macdata & mask) : 0;


        mask =  (0x01 << RED_HI_MAC_PCS_STATUS_FLD_ALIGN_DONE) |
                RED_HI_MAC_PCS_STATUS_CG_ALIGN |
                RED_HI_MAC_PCS_STATUS_SYNC; 

        *link = ((mask & pcsdata) == mask) ? 1 : 0;

	if ((*link == 0) || *fault) {
		printf("  Redwood%d %s%d [0x%X] : [PCS = 0x%X MAC = 0x%X]\n", 
			inst, hif ? "HIF" : "NIF",
			port, addr, pcsdata, macdata);

		printf("  Redwood%d %s%d: AlignDone=%d Align=0x%X, Sync=0x%X\n",
			inst, hif ? "HIF" : "NIF", port,
			(pcsdata & (0x01 << RED_HI_MAC_PCS_STATUS_FLD_ALIGN_DONE))? 1:0,
			((pcsdata >> 10) & 0x0F), 
			((pcsdata >> 14) & 0x0F));
	}
	*fault=0;
        return (rc);
}

int redwood_1G_rx_sync_get(int inst, uint8_t port, uint8_t *link)
{
        uint32_t data, addr;
        int      rc = 0;

        addr = BA(RED_HI0_GBE_STA) + (port * (BA(RED_HI1_GBE_STA - RED_HI0_GBE_STA)));

        rc = red_reg_rd (inst, addr, &data);
        if (rc) return (rc);

	*link = (data & (1 << RED_HI_GBE_STA_FLD_RX_SYNCHRONIZED)) ? 1 : 0;
	return (rc);
}


int redwood_gbe_mac_status_get(int inst, uint8_t cpuif, uint8_t *plink)
{
        int      rc = 0;
        uint32_t data, addr;

        addr = cpuif ? BA(RED_CI0_GBE_STA) : BA(RED_BI0_GBE_STA);
        rc = red_reg_rd (inst, addr, &data);  
        if (rc) return (rc);

        *plink = (data & RED_CI_GBE_STA_FLD_RX_SYNCHRONIZED) ? 1 : 0;

//	printf("%s [MAC = 0x%X]\n", cpuif ? "CIF" : "BIF", data);
        return (rc);
}


void ss_fc_rd_port_release(int inst, uint8_t which_ss, uint8_t release)
{
	uint32_t data, port;
	uint32_t ss_addr[][5] = {
	{RED_SS0_CFG_RDCH0,
	 RED_SS0_CFG_RDCH1,
	 RED_SS0_CFG_RDCH2,
	 RED_SS0_CFG_RDCH3,
	 RED_SS0_CFG_RDCH4},

	{RED_SS1_CFG_RDCH0,
	 RED_SS1_CFG_RDCH1,
	 RED_SS1_CFG_RDCH2,
	 RED_SS1_CFG_RDCH3,
	 RED_SS1_CFG_RDCH4},

	{RED_SS2_CFG_RDCH0,
	 RED_SS2_CFG_RDCH1,
	 RED_SS2_CFG_RDCH2,
	 RED_SS2_CFG_RDCH3,
	 RED_SS2_CFG_RDCH4},

	{RED_SS3_CFG_RDCH0,
	 RED_SS3_CFG_RDCH1,
	 RED_SS3_CFG_RDCH2,
	 RED_SS3_CFG_RDCH3,
	 RED_SS3_CFG_RDCH4},

			  };

	if (which_ss > 3) {
		printf(" %s:%d - invalid ss%d\n", __func__, __LINE__, which_ss);
		return;
	}

	for (port = 0; port <= 4; port++) {

		red_reg_rd(inst, BA(ss_addr[which_ss][port]), &data); 
		if (release) {
			data &= ~0x0F;
		} else {
			data |= 0x0F;
		}
		red_reg_wr(inst, BA(ss_addr[which_ss][port]), data); 
	}
}

void ss_fc_rd_port_release_all(int inst, uint8_t release)
{
    uint8_t ss;
    for(ss = 0; ss < 4; ss++) {    
	ss_fc_rd_port_release(inst, ss, release);
    }
}

int redwood_nspi_timer_cfg(int inst, uint32_t data)
{
	int rc = 0;
	uint32_t cnt;
	uint32_t addr[] = {
				RED_CI0_CFG_EG_NSPI_TIMER,
				RED_BI0_CFG_EG_NSPI_TIMER,
				RED_HI0_CFG_EG_NSPI_TIMER,
				RED_HI1_CFG_EG_NSPI_TIMER,
				RED_HI2_CFG_EG_NSPI_TIMER,
				RED_HI3_CFG_EG_NSPI_TIMER,
				RED_HI4_CFG_EG_NSPI_TIMER,
				RED_HI5_CFG_EG_NSPI_TIMER,
				RED_HI6_CFG_EG_NSPI_TIMER,
				RED_HI7_CFG_EG_NSPI_TIMER,
				RED_NI0_CFG_EG_NSPI_TIMER,
				RED_NI1_CFG_EG_NSPI_TIMER,
				RED_NI2_CFG_EG_NSPI_TIMER,
				RED_NI3_CFG_EG_NSPI_TIMER,
			  };

	for (cnt = 0; cnt < sizeof(addr)/sizeof(uint32_t); cnt++) {
		red_reg_wr(inst, BA(addr[cnt]), data); 
	}
	return (rc);
}

static int redwood_dump_regs(int inst, uint32_t *pregs, uint32_t count, uint8_t all_flag)
{
	uint32_t data, cnt;
	char* pname;

	for (cnt = 0; cnt < count; cnt++) {
		red_reg_rd(inst, pregs[cnt], &data); 
		if ((all_flag) || (!all_flag && data)) {
			pname = redwood_reg_addr_to_name(pregs[cnt]);
			printf("  %-48s [0x%08X] : 0x%08X\n", 
				pname ? pname : "*** ANONYMOUS REGISTER *** ", pregs[cnt], data);
		}
	}

	return (0);
}

int redwood_dump_intr_regs(int inst, uint32_t flag)
{
	uint32_t intr_regs[] = {
        BA(RED_MISC_INT_GPIO_DATA_IN),
        BA(RED_MISC_INT_ERR),
        BA(RED_MISC_RNAXI_INTERRUPT_READ),
        BA(RED_NF_INT_VEC),
        BA(RED_CI0_INT_VEC),
        BA(RED_CI0_INT_ERR_VEC),
        BA(RED_CI0_GBE_INT),
        BA(RED_BI0_GBE_INT),
        BA(RED_BI0_INTR),
        BA(RED_HI0_GBE_INT),
        BA(RED_HI1_GBE_INT),
        BA(RED_HI2_GBE_INT),
        BA(RED_HI3_GBE_INT),
        BA(RED_HI4_GBE_INT),
        BA(RED_HI5_GBE_INT),
        BA(RED_HI6_GBE_INT),
        BA(RED_HI7_GBE_INT),
        BA(RED_HI0_INTR),
        BA(RED_HI1_INTR),
        BA(RED_HI2_INTR),
        BA(RED_HI3_INTR),
        BA(RED_HI4_INTR),
        BA(RED_HI5_INTR),
        BA(RED_HI6_INTR),
        BA(RED_HI7_INTR),
        BA(RED_NI0_INTR),
        BA(RED_NI1_INTR),
        BA(RED_NI2_INTR),
        BA(RED_NI3_INTR),
        BA(RED_SS0_INT_OQ0),
        BA(RED_SS1_INT_OQ0),
        BA(RED_SS2_INT_OQ0),
        BA(RED_SS3_INT_OQ0),
        BA(RED_SS0_INT_OQ1),
        BA(RED_SS1_INT_OQ1),
        BA(RED_SS2_INT_OQ1),
        BA(RED_SS3_INT_OQ1),
        BA(RED_SS0_INT_OQ2),
        BA(RED_SS1_INT_OQ2),
        BA(RED_SS2_INT_OQ2),
        BA(RED_SS3_INT_OQ2),
        BA(RED_SS0_INT_OQ3),
        BA(RED_SS1_INT_OQ3),
        BA(RED_SS2_INT_OQ3),
        BA(RED_SS3_INT_OQ3),
        BA(RED_SS0_INT_OQ4),
        BA(RED_SS1_INT_OQ4),
        BA(RED_SS2_INT_OQ4),
        BA(RED_SS3_INT_OQ4),
        BA(RED_SS0_INT_NORM),
        BA(RED_SS1_INT_NORM),
        BA(RED_SS2_INT_NORM),
        BA(RED_SS3_INT_NORM),
        BA(RED_SS0_INT_ERR_ECC1),
        BA(RED_SS1_INT_ERR_ECC1),
        BA(RED_SS2_INT_ERR_ECC1),
        BA(RED_SS3_INT_ERR_ECC1),
        BA(RED_SS0_INT_ERR_ECC2),
        BA(RED_SS1_INT_ERR_ECC2),
        BA(RED_SS2_INT_ERR_ECC2),
        BA(RED_SS3_INT_ERR_ECC2),
        BA(RED_SS0_INT_ERR),
        BA(RED_SS1_INT_ERR),
        BA(RED_SS2_INT_ERR),
        BA(RED_SS3_INT_ERR),
	};
	return (redwood_dump_regs(inst, intr_regs, sizeof(intr_regs)/sizeof(uint32_t), flag));
}


int redwood_dump_intr_mask_regs(int inst, uint32_t flag)
{
	uint32_t intr_mask_regs[] = {
        BA(RED_MISC_INT_GPIO_DATA_IN_MSK),
        BA(RED_MISC_INT_ERR_MSK),
        BA(RED_NF_INT_VEC_MSK),
        BA(RED_CI0_INT_VEC_MSK),
        BA(RED_CI0_INT_ERR_VEC_MSK),
        BA(RED_CI0_GBE_INT_MSK),
        BA(RED_BI0_GBE_INT_MSK),
        BA(RED_BI0_INTR_MSK),
        BA(RED_HI0_GBE_INT_MSK),
        BA(RED_HI1_GBE_INT_MSK),
        BA(RED_HI2_GBE_INT_MSK),
        BA(RED_HI3_GBE_INT_MSK),
        BA(RED_HI4_GBE_INT_MSK),
        BA(RED_HI5_GBE_INT_MSK),
        BA(RED_HI6_GBE_INT_MSK),
        BA(RED_HI7_GBE_INT_MSK),
        BA(RED_HI0_INTR_MSK),
        BA(RED_HI1_INTR_MSK),
        BA(RED_HI2_INTR_MSK),
        BA(RED_HI3_INTR_MSK),
        BA(RED_HI4_INTR_MSK),
        BA(RED_HI5_INTR_MSK),
        BA(RED_HI6_INTR_MSK),
        BA(RED_HI7_INTR_MSK),
        BA(RED_NI0_INTR_MSK),
        BA(RED_NI1_INTR_MSK),
        BA(RED_NI2_INTR_MSK),
        BA(RED_NI3_INTR_MSK),
        BA(RED_SS0_INT_OQ0_MSK),
        BA(RED_SS1_INT_OQ0_MSK),
        BA(RED_SS2_INT_OQ0_MSK),
        BA(RED_SS3_INT_OQ0_MSK),
        BA(RED_SS0_INT_OQ1_MSK),
        BA(RED_SS1_INT_OQ1_MSK),
        BA(RED_SS2_INT_OQ1_MSK),
        BA(RED_SS3_INT_OQ1_MSK),
        BA(RED_SS0_INT_OQ2_MSK),
        BA(RED_SS1_INT_OQ2_MSK),
        BA(RED_SS2_INT_OQ2_MSK),
        BA(RED_SS3_INT_OQ2_MSK),
        BA(RED_SS0_INT_OQ3_MSK),
        BA(RED_SS1_INT_OQ3_MSK),
        BA(RED_SS2_INT_OQ3_MSK),
        BA(RED_SS3_INT_OQ3_MSK),
        BA(RED_SS0_INT_OQ4_MSK),
        BA(RED_SS1_INT_OQ4_MSK),
        BA(RED_SS2_INT_OQ4_MSK),
        BA(RED_SS3_INT_OQ4_MSK),
        BA(RED_SS0_INT_NORM_MSK),
        BA(RED_SS1_INT_NORM_MSK),
        BA(RED_SS2_INT_NORM_MSK),
        BA(RED_SS3_INT_NORM_MSK),
        BA(RED_SS0_INT_ERR_ECC1_MSK),
        BA(RED_SS1_INT_ERR_ECC1_MSK),
        BA(RED_SS2_INT_ERR_ECC1_MSK),
        BA(RED_SS3_INT_ERR_ECC1_MSK),
        BA(RED_SS0_INT_ERR_ECC2_MSK),
        BA(RED_SS1_INT_ERR_ECC2_MSK),
        BA(RED_SS2_INT_ERR_ECC2_MSK),
        BA(RED_SS3_INT_ERR_ECC2_MSK),
        BA(RED_SS0_INT_ERR_MSK),
        BA(RED_SS1_INT_ERR_MSK),
        BA(RED_SS2_INT_ERR_MSK),
        BA(RED_SS3_INT_ERR_MSK),
	};

	return (redwood_dump_regs(inst, intr_mask_regs, sizeof(intr_mask_regs)/sizeof(uint32_t), flag));
}


int redwood_dump_drop_regs(int inst, uint32_t flag)
{
	uint32_t drop_regs[] = {
                BA(RED_CI0_CNT_TX_RUNT_PKT_DROP),
                BA(RED_CI0_CNT_RX_RUNT_PKT_DROP),
                BA(RED_CI0_CNT_SWITCH_DROP),
                BA(RED_CI0_CNT_HOST_MC_DROP),
                BA(RED_CI0_CNT_HOST_UC_DROP),
                BA(RED_BI0_CNT_RX_MC_DROP),
                BA(RED_BI0_CNT_RX_FWD_DROP),
                BA(RED_BI0_CNT_RX_SRC_VIF_OUT_OF_RANGE_DROP),
                BA(RED_BI0_CNT_TX_LB_DROP),
                BA(RED_BI0_CNT_RX_ALLOW_VNTAG_DROP),
                BA(RED_BI0_CNT_RX_ECHANNEL_DROP),
                BA(RED_BI0_CNT_RX_RUNT_PKT_DROP),
                BA(RED_HI0_CNT_RX_MC_DROP),
                BA(RED_HI1_CNT_RX_MC_DROP),
                BA(RED_HI2_CNT_RX_MC_DROP),
                BA(RED_HI3_CNT_RX_MC_DROP),
                BA(RED_HI4_CNT_RX_MC_DROP),
                BA(RED_HI5_CNT_RX_MC_DROP),
                BA(RED_HI6_CNT_RX_MC_DROP),
                BA(RED_HI7_CNT_RX_MC_DROP),
                BA(RED_HI0_CNT_RX_FWD_DROP),
                BA(RED_HI1_CNT_RX_FWD_DROP),
                BA(RED_HI2_CNT_RX_FWD_DROP),
                BA(RED_HI3_CNT_RX_FWD_DROP),
                BA(RED_HI4_CNT_RX_FWD_DROP),
                BA(RED_HI5_CNT_RX_FWD_DROP),
                BA(RED_HI6_CNT_RX_FWD_DROP),
                BA(RED_HI7_CNT_RX_FWD_DROP),
                BA(RED_HI0_CNT_RX_SRC_VIF_OUT_OF_RANGE_DROP),
                BA(RED_HI1_CNT_RX_SRC_VIF_OUT_OF_RANGE_DROP),
                BA(RED_HI2_CNT_RX_SRC_VIF_OUT_OF_RANGE_DROP),
                BA(RED_HI3_CNT_RX_SRC_VIF_OUT_OF_RANGE_DROP),
                BA(RED_HI4_CNT_RX_SRC_VIF_OUT_OF_RANGE_DROP),
                BA(RED_HI5_CNT_RX_SRC_VIF_OUT_OF_RANGE_DROP),
                BA(RED_HI6_CNT_RX_SRC_VIF_OUT_OF_RANGE_DROP),
                BA(RED_HI7_CNT_RX_SRC_VIF_OUT_OF_RANGE_DROP),
                BA(RED_HI0_CNT_TX_LB_DROP),
                BA(RED_HI1_CNT_TX_LB_DROP),
                BA(RED_HI2_CNT_TX_LB_DROP),
                BA(RED_HI3_CNT_TX_LB_DROP),
                BA(RED_HI4_CNT_TX_LB_DROP),
                BA(RED_HI5_CNT_TX_LB_DROP),
                BA(RED_HI6_CNT_TX_LB_DROP),
                BA(RED_HI7_CNT_TX_LB_DROP),
                BA(RED_HI0_CNT_RX_ALLOW_VNTAG_DROP),
                BA(RED_HI1_CNT_RX_ALLOW_VNTAG_DROP),
                BA(RED_HI2_CNT_RX_ALLOW_VNTAG_DROP),
                BA(RED_HI3_CNT_RX_ALLOW_VNTAG_DROP),
                BA(RED_HI4_CNT_RX_ALLOW_VNTAG_DROP),
                BA(RED_HI5_CNT_RX_ALLOW_VNTAG_DROP),
                BA(RED_HI6_CNT_RX_ALLOW_VNTAG_DROP),
                BA(RED_HI7_CNT_RX_ALLOW_VNTAG_DROP),
                BA(RED_HI0_CNT_RX_ECHANNEL_DROP),
                BA(RED_HI1_CNT_RX_ECHANNEL_DROP),
                BA(RED_HI2_CNT_RX_ECHANNEL_DROP),
                BA(RED_HI3_CNT_RX_ECHANNEL_DROP),
                BA(RED_HI4_CNT_RX_ECHANNEL_DROP),
                BA(RED_HI5_CNT_RX_ECHANNEL_DROP),
                BA(RED_HI6_CNT_RX_ECHANNEL_DROP),
                BA(RED_HI7_CNT_RX_ECHANNEL_DROP),
                BA(RED_HI0_CNT_RX_RUNT_PKT_DROP),
                BA(RED_HI1_CNT_RX_RUNT_PKT_DROP),
                BA(RED_HI2_CNT_RX_RUNT_PKT_DROP),
                BA(RED_HI3_CNT_RX_RUNT_PKT_DROP),
                BA(RED_HI4_CNT_RX_RUNT_PKT_DROP),
                BA(RED_HI5_CNT_RX_RUNT_PKT_DROP),
                BA(RED_HI6_CNT_RX_RUNT_PKT_DROP),
                BA(RED_HI7_CNT_RX_RUNT_PKT_DROP),
                BA(RED_NI0_CNT_RX_MC_DROP),
                BA(RED_NI1_CNT_RX_MC_DROP),
                BA(RED_NI2_CNT_RX_MC_DROP),
                BA(RED_NI3_CNT_RX_MC_DROP),
                BA(RED_NI0_CNT_MCST_FWD_TABLE_MISS_DROP),
                BA(RED_NI1_CNT_MCST_FWD_TABLE_MISS_DROP),
                BA(RED_NI2_CNT_MCST_FWD_TABLE_MISS_DROP),
                BA(RED_NI3_CNT_MCST_FWD_TABLE_MISS_DROP),
                BA(RED_NI0_CNT_MCST_FWD_TABLE_EMPTY_DROP),
                BA(RED_NI1_CNT_MCST_FWD_TABLE_EMPTY_DROP),
                BA(RED_NI2_CNT_MCST_FWD_TABLE_EMPTY_DROP),
                BA(RED_NI3_CNT_MCST_FWD_TABLE_EMPTY_DROP),
                BA(RED_NI0_CNT_UCST_RANGE_CHECK_DROP),
                BA(RED_NI1_CNT_UCST_RANGE_CHECK_DROP),
                BA(RED_NI2_CNT_UCST_RANGE_CHECK_DROP),
                BA(RED_NI3_CNT_UCST_RANGE_CHECK_DROP),
                BA(RED_NI0_CNT_EXPECTED_DROP),
                BA(RED_NI1_CNT_EXPECTED_DROP),
                BA(RED_NI2_CNT_EXPECTED_DROP),
                BA(RED_NI3_CNT_EXPECTED_DROP),
                BA(RED_SS0_CFG_DDROP),
                BA(RED_SS1_CFG_DDROP),
                BA(RED_SS2_CFG_DDROP),
                BA(RED_SS3_CFG_DDROP),
                BA(RED_SS0_DHS_DDROP),
                BA(RED_SS1_DHS_DDROP),
                BA(RED_SS2_DHS_DDROP),
                BA(RED_SS3_DHS_DDROP),
		};

	return (redwood_dump_regs(inst, drop_regs, sizeof(drop_regs)/sizeof(uint32_t), flag));
}


int redwood_hi_fin_enable(int inst, uint8_t hif, uint8_t ena)
{
	uint32_t addr, data;

	addr = BA(RED_HI0_CFG_DEBUG_PORT + 
	       (hif *(RED_HI1_CFG_DEBUG_PORT - RED_HI0_CFG_DEBUG_PORT)));

	red_reg_rd(inst, addr, &data); 
	if (ena) {
		data |= (0x01 << RED_HI_CFG_DEBUG_PORT_FLD_FIN_ENABLE);
	} else {
		data &= ~(0x01 << RED_HI_CFG_DEBUG_PORT_FLD_FIN_ENABLE);
	}

	red_reg_wr(inst, addr, data); 
	return (0);
}

int redwood_pause_enable(int inst)
{
	uint32_t cnt, count;

	uint32_t addr[][2] =  {
		{BA(RED_HI0_FLOW_CONTROL_Q2COS_MAP), 0xffffffff},
		{BA(RED_HI1_FLOW_CONTROL_Q2COS_MAP), 0xffffffff},
		{BA(RED_HI2_FLOW_CONTROL_Q2COS_MAP), 0xffffffff},
		{BA(RED_HI3_FLOW_CONTROL_Q2COS_MAP), 0xffffffff},
		{BA(RED_HI4_FLOW_CONTROL_Q2COS_MAP), 0xffffffff},
		{BA(RED_HI5_FLOW_CONTROL_Q2COS_MAP), 0xffffffff},
		{BA(RED_HI6_FLOW_CONTROL_Q2COS_MAP), 0xffffffff},
		{BA(RED_HI7_FLOW_CONTROL_Q2COS_MAP), 0xffffffff},
		{BA(RED_HI0_FLOW_CONTROL_COS2Q_MAP), 0xffffffff},
		{BA(RED_HI1_FLOW_CONTROL_COS2Q_MAP), 0xffffffff},
		{BA(RED_HI2_FLOW_CONTROL_COS2Q_MAP), 0xffffffff},
		{BA(RED_HI3_FLOW_CONTROL_COS2Q_MAP), 0xffffffff},
		{BA(RED_HI4_FLOW_CONTROL_COS2Q_MAP), 0xffffffff},
		{BA(RED_HI5_FLOW_CONTROL_COS2Q_MAP), 0xffffffff},
		{BA(RED_HI6_FLOW_CONTROL_COS2Q_MAP), 0xffffffff},
		{BA(RED_HI7_FLOW_CONTROL_COS2Q_MAP), 0xffffffff},
		{BA(RED_NI0_FLOW_CONTROL_Q2COS_MAP), 0xffffffff},
		{BA(RED_NI1_FLOW_CONTROL_Q2COS_MAP), 0xffffffff},
		{BA(RED_NI2_FLOW_CONTROL_Q2COS_MAP), 0xffffffff},
		{BA(RED_NI3_FLOW_CONTROL_Q2COS_MAP), 0xffffffff},
		{BA(RED_HI0_GBE_CFG_PAUSE), 0x201},
		{BA(RED_HI1_GBE_CFG_PAUSE), 0x201},
		{BA(RED_HI2_GBE_CFG_PAUSE), 0x201},
		{BA(RED_HI3_GBE_CFG_PAUSE), 0x201},
		{BA(RED_HI4_GBE_CFG_PAUSE), 0x201},
		{BA(RED_HI5_GBE_CFG_PAUSE), 0x201},
		{BA(RED_HI6_GBE_CFG_PAUSE), 0x201},
		{BA(RED_HI7_GBE_CFG_PAUSE), 0x201},
	};

	count = sizeof(addr)/(2 * sizeof(uint32_t));

	for (cnt = 0; cnt < count; cnt++) {
		red_reg_wr(inst, addr[cnt][0], addr[cnt][1]); 
	}
	return (0);
}

int redwood_rmon_clear(int inst)
{
	uint32_t cnt, count; 
	uint32_t addr[] = {
		BA(RED_NI0_SW_RST),
		BA(RED_NI1_SW_RST),
		BA(RED_NI2_SW_RST),
		BA(RED_NI3_SW_RST),
		BA(RED_HI0_SW_RST),
		BA(RED_HI1_SW_RST),
		BA(RED_HI2_SW_RST),
		BA(RED_HI3_SW_RST),
		BA(RED_HI4_SW_RST),
		BA(RED_HI5_SW_RST),
		BA(RED_HI6_SW_RST),
		BA(RED_HI7_SW_RST),
	};

	count = sizeof(addr)/(sizeof(uint32_t));
	for (cnt = 0; cnt < count; cnt++) {
		red_reg_wr(inst, addr[cnt], 0x20000); 
	}
	for (cnt = 0; cnt < count; cnt++) {
		red_reg_wr(inst, addr[cnt], 0x00000); 
	}
	return (0);	
}
