/********************************************************************
 *
 *      File:   woo_asic.c
 *      Name:   Sudharshan Kadari 
 *
 *      Description: ASIC level functions for Redwood USD
 *
 *
 *      Copyright (c) 1984-2009 by Cisco Systems, Inc.
 ********************************************************************/
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include "woo_ops.h"
#include "diag_main.h"

#define ASIC_MAX_PKT_WORD_SZ 	380	//1520 bytes
#define ASIC_MAX_PKT_BYTE_SZ 	1518	//1518 bytes
#define ASIC_MIN_PKT_BYTE_SZ 	64	//1518 bytes

#if defined(DIAG_PORTOLA)

static int asic_cfg_mac_1G(uint8_t inst, uint8_t hi, uint8_t port, 
                           uint32_t gbe_mode)
{
	int rc = 0;
        if (gbe_mode == 0) {
            rc = asic_mac_bring_up_dover( inst, port, 1000);
        } else if(gbe_mode == 1) {
            rc = asic_mac_bring_up_dover( inst, port, 100);
            rc = mac_set_speed_dover( inst, port, 100);
        }
	return (rc);
}
#endif

#ifndef DIAG_PORTOLA
static int asic_hi_1g_set (uint8_t inst)
{
	int cnt = 0, rc=0;
	uint32_t reg_addr;
	DIAG_PRINT(DIAG_PRINT_CONFIG, "  ASIC-%d SET HI in GE Mode\n", inst);
	for (cnt = 0; cnt < ASIC(HI_COUNT); cnt++) {
		reg_addr = HI_REG_ADDR(CFG_PORT, cnt);
		rc = asic_bset (inst, reg_addr, ASIC(HI_CFG_PORT_FLD_1G_MODE)); 
		if (rc) return (rc);
	}
	return (rc);
}
/*
 * gbe_mode = 0 => 10G mode
 * gbe_mode = 1 => 1G mode
 */
static int asic_cfg_hi_mac_10G(uint8_t inst, uint8_t port, uint32_t gbe_mode)
{
	int		rc = 0;
	uint32_t	reg_addr, reg_data;
         
        if (gbe_mode) {
            asic_hi_1g_set(inst);
        }
        asic_cfg_hi_serdes_sig_loss_mask(inst, port, gbe_mode);

	// Enable MAC Core Clock.
	reg_addr = HI_REG_ADDR(CT_CFG_CONTROL, port); 
	rc = asic_rd(inst, reg_addr, &reg_data);
	if (rc) return (rc);

	reg_data |= (0x01 << ASIC(HI_CT_CFG_CONTROL_FLD_SW_RST));
	reg_data |= (0x01 << ASIC(HI_CT_CFG_CONTROL_FLD_XE_ENABLE_CLK));
	reg_data |= (gbe_mode ? (0x01 << ASIC(HI_CT_CFG_CONTROL_FLD_GE_ENABLE_CLK))
                                 : 0x00);

	rc = asic_wr(inst, reg_addr, reg_data);
	if (rc) return (rc);
	usleep(50);

	// Mini Slave reset
	reg_data &= ~(0x01 << ASIC(HI_CT_CFG_CONTROL_FLD_XE_MINISLAVE_RST));
	reg_data &= ~(gbe_mode ? 
                     (0x01 << ASIC(HI_CT_CFG_CONTROL_FLD_GE_MINISLAVE_RST)) : 0x00);

	rc = asic_wr(inst, reg_addr, reg_data);
	if (rc) return (rc);
	usleep(50);

	if (gbe_mode) {
	    reg_addr = HI_REG_ADDR(GE_CFG, port);
	    rc = asic_bset(inst, reg_addr, ASIC(HI_GE_CFG_FLD_RX_LINK_RST));
	    if (rc) return (rc);
            
	    reg_addr = HI_REG_ADDR(XE_CFG_XG_CONTROL, port);
	    rc = asic_bset(inst, reg_addr, ASIC(HI_XE_CFG_XG_CONTROL_FLD_GBE_MODE));
	    if (rc) return (rc);
	} else {
	    reg_addr = HI_REG_ADDR(XE_CFG_XG_CONTROL, port);
	    rc = asic_bclr(inst, reg_addr, ASIC(HI_XE_CFG_XG_CONTROL_FLD_GBE_MODE));
        }
	// CT CFG MUX
	reg_data = ASIC(HI_CT_CFG_MUX_RX_DEF);
	reg_data |= (ASIC_MAX_PKT_WORD_SZ << 13);
	reg_data |= (0x01 << 3);			// DST Select.
	reg_data |= (0x01 << ASIC(HI_CT_CFG_MUX_RX_FLD_ENABLE));
	reg_addr = HI_REG_ADDR(CT_CFG_MUX_RX, port);

	rc = asic_wr(inst, reg_addr, reg_data);
	if (rc) return (rc);

	// HI_XE_CFG_XG_STATS_DEF
	reg_addr = HI_REG_ADDR(XE_CFG_XG_STATS, port); 
	reg_data = (ASIC_MAX_PKT_BYTE_SZ << 8) | ASIC_MIN_PKT_BYTE_SZ;
	reg_data |= (0x01 << ASIC(HI_XE_CFG_XG_STATS_FLD_RX_CHECK_LEN_FIELD)); 
	rc = asic_wr(inst, reg_addr, reg_data);
	if (rc) return (rc);

	// HI_CT_CFG_MUX_TX_DEF
	reg_addr = HI_REG_ADDR(CT_CFG_MUX_TX, port);
	reg_data = ASIC(HI_CT_CFG_MUX_TX_DEF);
	reg_data |= (0x05 << 26);			// Start Threshold
	reg_data |= (0x01 << ASIC(HI_CT_CFG_MUX_TX_FLD_ENABLE)); 
	rc = asic_wr(inst, reg_addr, reg_data);
	if (rc) return (rc);

	// release sw reset	
	reg_addr = HI_REG_ADDR(CT_CFG_CONTROL, port);
	rc = asic_bclr(inst, reg_addr, ASIC(HI_CT_CFG_CONTROL_FLD_SW_RST));
        usleep(500);

	if (gbe_mode) {
	    reg_addr = HI_REG_ADDR(GE_CFG, port);
	    rc = asic_rd(inst, reg_addr, &reg_data); 
	    if (rc) return (rc);
            
	    reg_data |= (0x01 << ASIC(HI_GE_CFG_FLD_TX_FORCE_XMIT_DATA));
	    reg_data &= ~(0x01 << ASIC(HI_GE_CFG_FLD_RX_LINK_RST));
	    reg_data &= ~(0x3FF << 3); 	// Clear Transmit Time
	    reg_data |= (8 << 3);		// Set Transmit time.
	    reg_data |= (0x01 << ASIC(HI_GE_CFG_FLD_TX_IPG_ADJUST));
            
	    rc = asic_wr(inst, reg_addr, reg_data);
	    if (rc) return (rc);
	}

	// Enable MAC Operation.
	reg_addr = HI_REG_ADDR(XE_CFG_XG_CONTROL, port);
	rc = asic_rd(inst, reg_addr, &reg_data);
	if (rc) return (rc);
	reg_data &= ~(0xF << 8);			// clear IPG
	/* reg_data |= (0x3 << 8); // set TX IPG to 3 */
	reg_data |= (0x01 << ASIC(HI_XE_CFG_XG_CONTROL_FLD_TX_ENABLE));
	reg_data |= (0x01 << ASIC(HI_XE_CFG_XG_CONTROL_FLD_RX_ENABLE));
        /*
         * QA found that this needs to be set to ignore without which raw
         * Ethernet packets would drop as RXINRANGE Errors
         */
	reg_data |= (0x01 << ASIC(HI_XE_CFG_XG_CONTROL_FLD_RX_IGNORE_LEN_FIELD_ERR));
	reg_data |= (0x01 << ASIC(HI_XE_CFG_XG_CONTROL_FLD_RX_IGNORE_OVERSIZE_ERR));
#if defined(DIAG_BROCKWAY)
	reg_data |= (0x01 << ASIC(HI_XE_CFG_XG_CONTROL_FLD_RX_STRICT_SOP));
#else
	reg_data &= ~(0x01 << ASIC(HI_XE_CFG_XG_CONTROL_FLD_RX_STRICT_SOP));
#endif
	rc = asic_wr(inst, reg_addr, reg_data);
	// By default COS2Q / Q2COS is 1 to 1
	reg_data = 0xFAC688;
	reg_addr = HI_REG_ADDR(CFG_RX_COS_MAP, port);
	rc = asic_wr (inst, reg_addr, reg_data);
	if(rc) return (rc);

	reg_addr = HI_REG_ADDR(CFG_TX_COS_MAP, port);
	rc = asic_wr (inst, reg_addr, reg_data);
	if(rc) return (rc);

	reg_addr = HI_REG_ADDR(CFG_RX_COS2Q_MAP, port);
	rc = asic_wr (inst, reg_addr, reg_data);
	return (rc);
}
#endif


int asic_cfg_ni_mac_10G(uint8_t inst, uint8_t port)
{
	uint32_t	reg_addr, reg_data;
	int		rc = 0;

	// Enable MAC Core Clock.
	reg_addr = NI_REG_ADDR(CT_CFG_CONTROL, port);
	rc = asic_rd(inst, reg_addr, &reg_data);
	if (rc) return (rc);

	reg_data |= (0x01 << ASIC(NI_CT_CFG_CONTROL_FLD_SW_RST));
	reg_data |= (0x01 << ASIC(NI_CT_CFG_CONTROL_FLD_XE_ENABLE_CLK));

	rc = asic_wr(inst, reg_addr, reg_data);
	if (rc) return (rc);
	usleep(50);

	// Mini Slave reset
	reg_data &= ~(0x01 << ASIC(NI_CT_CFG_CONTROL_FLD_XE_MINISLAVE_RST));

	rc = asic_wr(inst, reg_addr, reg_data);
	if (rc) return (rc);
	usleep(50);

	// CT CFG MUX
	reg_data = ASIC(NI_CT_CFG_MUX_RX_DEF);
	reg_data |= (ASIC_MAX_PKT_WORD_SZ << 13);
	reg_data |= (0x01 << 3);			// DST Select.
	reg_data |= (0x01 << ASIC(NI_CT_CFG_MUX_RX_FLD_ENABLE));
	reg_addr = NI_REG_ADDR(CT_CFG_MUX_RX, port);

	rc = asic_wr(inst, reg_addr, reg_data);
	if (rc) return (rc);

	// NI_XE_CFG_XG_STATS_DEF
	reg_addr = NI_REG_ADDR(XE_CFG_XG_STATS, port);
	reg_data = (ASIC_MAX_PKT_BYTE_SZ << 8) | ASIC_MIN_PKT_BYTE_SZ;
	reg_data |= (0x01 << ASIC(NI_XE_CFG_XG_STATS_FLD_RX_CHECK_LEN_FIELD)); 
	rc = asic_wr(inst, reg_addr, reg_data);
	if (rc) return (rc);

	// NI_CT_CFG_MUX_TX_DEF
	reg_addr = NI_REG_ADDR(CT_CFG_MUX_TX, port);
	reg_data = ASIC(NI_CT_CFG_MUX_TX_DEF);
	reg_data |= (0x05 << 26);			// Start Threshold
	reg_data |= (0x01 << ASIC(NI_CT_CFG_MUX_TX_FLD_ENABLE)); 
	rc = asic_wr(inst, reg_addr, reg_data);
	if (rc) return (rc);

	// release sw reset	
	reg_addr = NI_REG_ADDR(CT_CFG_CONTROL, port);
	rc = asic_bclr(inst, reg_addr, ASIC(NI_CT_CFG_CONTROL_FLD_SW_RST));


	// Enable MAC Operation.
	reg_addr = NI_REG_ADDR(XE_CFG_XG_CONTROL, port);
	rc = asic_rd(inst, reg_addr, &reg_data);
	if (rc) return (rc);
	reg_data &= ~(0xF << 8);			// clear IPG
	reg_data |= (0x01 << ASIC(NI_XE_CFG_XG_CONTROL_FLD_TX_ENABLE));
	reg_data |= (0x01 << ASIC(NI_XE_CFG_XG_CONTROL_FLD_RX_ENABLE));
	reg_data &= ~(0x01 << ASIC(NI_XE_CFG_XG_CONTROL_FLD_RX_IGNORE_LEN_FIELD_ERR));
	reg_data &= ~(0x01 << ASIC(NI_XE_CFG_XG_CONTROL_FLD_RX_IGNORE_OVERSIZE_ERR));
#if defined(DIAG_BROCKWAY)
	reg_data |= (0x01 << ASIC(NI_XE_CFG_XG_CONTROL_FLD_RX_STRICT_SOP));
#else
	reg_data &= ~(0x01 << ASIC(NI_XE_CFG_XG_CONTROL_FLD_RX_STRICT_SOP));
#endif
	rc = asic_wr(inst, reg_addr, reg_data);

	// By default COS2Q / Q2COS is 1 to 1
	reg_data = 0xFAC688;
	reg_addr = NI_REG_ADDR(CFG_RX_COS_MAP, port);
	rc = asic_wr (inst, reg_addr, reg_data);
	if(rc) return (rc);

	reg_addr = NI_REG_ADDR(CFG_TX_COS_MAP, port);
	rc = asic_wr (inst, reg_addr, reg_data);
	if(rc) return (rc);

	reg_addr = NI_REG_ADDR(CFG_RX_COS2Q_MAP, port);
	rc = asic_wr (inst, reg_addr, reg_data);
	return (rc);
}

static int asic_cfg_hi_mac(uint8_t inst, uint8_t hif, uint32_t gbe_mode)
{
        // gigabit mode configuration
        int rc = 0 ;

#ifdef DIAG_PORTOLA
	rc = asic_cfg_mac_1G(inst, 1, hif, gbe_mode);
#else
        rc = asic_cfg_hi_mac_10G(inst, hif, gbe_mode);
#endif
	return (rc);
}

int asic_cfg_tx_flush(uint8_t inst, uint8_t is_hif, uint8_t port, uint8_t ena)
{
	uint32_t reg_addr = NI_REG_ADDR(XE_CFG_XG_CONTROL, port); 

	if (is_hif) {
#ifdef DIAG_PORTOLA
		printf(" %s NOT IMPLEMENTED\n", __func__);
#else
		reg_addr = HI_REG_ADDR(XE_CFG_XG_CONTROL, port);
#endif
	}
        return ((ena? asic_bset : asic_bclr) (inst, reg_addr, 
		ASIC(NI_XE_CFG_XG_CONTROL_FLD_TX_FLUSH_ENABLE)));
}

int asic_cfg_fin_fout(uint8_t inst, uint8_t ptype, uint8_t port, uint8_t fin, uint8_t fout)
{
	int rc = 0;
	uint32_t reg_addr, data;

	switch(ptype) {
		case	ASIC_PTYPE_HI:
			reg_addr = HI_REG_ADDR(CFG_DEBUG_PORT, port);
			break;
		case	ASIC_PTYPE_NI:
			reg_addr = NI_REG_ADDR(CFG_DEBUG_PORT, port);
			break;

#if 0
		case	ASIC_PTYPE_CI:
			break;
#endif

		case	ASIC_PTYPE_BI:
			reg_addr = BA(ASIC(BI_CFG_DEBUG_PORT));
			break;

		default:
			printf("  Error: Invalid port type %d\n", ptype);
			return (-1);
			break;

	}
        rc = asic_rd(inst, reg_addr, &data);
	if (rc) return (rc);

        if (fin) {
                data |= (0x01 << ASIC(HI_CFG_DEBUG_PORT_FLD_FIN_ENABLE));
        } else {
                data &= ~(0x01 << ASIC(HI_CFG_DEBUG_PORT_FLD_FIN_ENABLE));
        }
        if (fout) {
                data |= (0x01 << ASIC(HI_CFG_DEBUG_PORT_FLD_FOUT_ENABLE));
        } else {
                data &= ~(0x01 << ASIC(HI_CFG_DEBUG_PORT_FLD_FOUT_ENABLE));
        }

        rc = asic_wr(inst, reg_addr, data);
	return (rc);
}

static int asic_cfg_hif_mac(uint8_t inst, uint64_t hif_mask, uint32_t gbe_mode)
{
	int rc = 0, hif;

	for (hif = 0; hif < ASIC(HI_COUNT); hif++, hif_mask>>=1) {
		if (!(hif_mask & 1)) continue;
		rc = asic_cfg_hi_mac(inst, hif, gbe_mode);
	}
	return (rc);
}

int asic_cfg_ni_mac (uint8_t inst, uint8_t nif, uint8_t fin_fout)
{
	int	 rc = 0;

        rc = asic_cfg_ni_mac_10G(inst, nif);
	if (rc) return (rc);

	if (fin_fout) {
		rc = asic_cfg_fin_fout(inst, ASIC_PTYPE_NI, nif, 1, 1);
		if (rc) return (rc);

		rc = asic_cfg_tx_flush(inst, 0, nif, 1);
		if (rc) return (rc);
	}	
	return (rc);
}

int asic_cfg_nif_mac(uint8_t inst, uint64_t nif_mask)
{
	int rc = 0, nif;

	for (nif = 0; nif < ASIC(NI_COUNT); nif++, nif_mask>>=1) {
		if (!(nif_mask & 1)) continue;
		rc = asic_cfg_ni_mac(inst, nif, 0);
	}
	return (rc);
}

int asic_cfg_mac_mii (uint8_t inst, uint8_t is_bi)
{
	uint32_t reg_addr, reg_data;
	int	 rc = 0;

	if (!is_bi) {
		reg_data = 0x20 * 4 * 10 ;  // CI is in MII 
		rc = asic_wr (inst, BA(ASIC(CI_CFG_EG_NSPI_TIMER)), reg_data);
		if (rc) return (rc);
	}

	reg_addr = is_bi? BA(ASIC(BI_GBE_CFG_PAUSE_FC_LIMIT)) : 
			  BA(ASIC(CI_GBE_CFG_PAUSE_FC_LIMIT));
	reg_data = 0x200;
	rc = asic_wr (inst, reg_addr, reg_data);
	if (rc) return (rc);

	reg_addr = is_bi? BA(ASIC(BI_GBE_CFG_MII_MDC_PERIOD)) : 
			  BA(ASIC(CI_GBE_CFG_MII_MDC_PERIOD));
	reg_data = 60;
	rc = asic_wr (inst, reg_addr, reg_data);
	if (rc) return (rc);

	reg_addr = is_bi? BA(ASIC(BI_GBE_CFG)) : 
			  BA(ASIC(CI_GBE_CFG));
	rc = asic_rd(inst, reg_addr, &reg_data);
	if (rc) return (rc);

	reg_data |= (0x01 << ASIC(CI_GBE_CFG_FLD_RX_LINK_RST));
	reg_data |= (0x01 << ASIC(CI_GBE_CFG_FLD_RX_SW_RST));
	reg_data |= (0x01 << ASIC(CI_GBE_CFG_FLD_TX_SW_RST));
	reg_data |= (0x01 << ASIC(CI_GBE_CFG_FLD_RX_CRC_CHK_EN));
	reg_data &= ~(0x01 << ASIC(CI_GBE_CFG_FLD_PAUSE_TX_EN));
	reg_data &= ~(0x3FF << 17);
	reg_data |= (8 << 17);

	rc = asic_wr (inst, reg_addr, reg_data);
	if (rc) return (rc);

	usleep (100);
	reg_data &= ~(0x01 << ASIC(CI_GBE_CFG_FLD_RX_SW_RST));
	reg_data &= ~(0x01 << ASIC(CI_GBE_CFG_FLD_TX_SW_RST));
	rc = asic_wr (inst, reg_addr, reg_data);
	if (rc) return (rc);

	usleep (100);
	reg_data &= ~(0x01 << ASIC(CI_GBE_CFG_FLD_RX_LINK_RST));
	rc = asic_wr (inst, reg_addr, reg_data);
	if (rc) return (rc);

	reg_addr = is_bi? BA(ASIC(BI_GBE_CFG_RX)) : 
			  BA(ASIC(CI_GBE_CFG_RX));
	reg_data = ASIC(MAX_PKT_SIZE) | (ASIC(MIN_PKT_SIZE) << 14);
	rc = asic_wr (inst, reg_addr, reg_data);
	if (rc) return (rc);

	reg_addr = is_bi? BA(ASIC(BI_GBE_INT)) : 
			  BA(ASIC(CI_GBE_INT));
	reg_data = 0xFFFFFFFF; 
	rc = asic_wr (inst, reg_addr, reg_data);
	if (rc) return (rc);

	// By default COS2Q / Q2COS is 1 to 1
	reg_data = 0xFAC688;
	reg_addr = is_bi? BA(ASIC(BI_CFG_RX_COS_MAP)) :
			  BA(ASIC(CI_CFG_RX_COS_MAP));
	rc = asic_wr (inst, reg_addr, reg_data);
	if(rc) return (rc);

	reg_addr = is_bi? BA(ASIC(BI_CFG_TX_COS_MAP)) :
			  BA(ASIC(CI_CFG_TX_COS_MAP));
	rc = asic_wr (inst, reg_addr, reg_data);
	if(rc) return (rc);

	reg_addr = is_bi? BA(ASIC(BI_CFG_RX_COS2Q_MAP)) :
			  BA(ASIC(CI_CFG_RX_COS2Q_MAP));
	rc = asic_wr (inst, reg_addr, reg_data);
	return (rc);
}

int asic_cfg_bi_mac (uint8_t inst, uint8_t bifport, uint8_t fin_fout)
{
	int	 rc = 0;

	rc = asic_cfg_mac_mii (inst, 1);

	if (!rc && fin_fout) {
		rc = asic_cfg_fin_fout(inst, ASIC_PTYPE_BI, bifport, 1, 1);
		if (rc) return (rc);

		rc = asic_cfg_tx_flush(inst, 0, bifport, 1);
		if (rc) return (rc);
	}	
	return (rc);
}


int asic_cfg_bif_mac(uint8_t inst, uint64_t bif_mask)
{
	int rc = 0, bifport;

	for (bifport = 0; bifport < ASIC(BI_COUNT); bifport++, bif_mask>>=1) {
		if (!(bif_mask & 1)) continue;
		rc = asic_cfg_bi_mac(inst, bifport, 0);
	}

	return (rc);
}

int asic_cfg_ci_mac (uint8_t inst, uint8_t cifport, uint8_t fin_fout)
{
	int	 rc = 0;

	rc = asic_cfg_mac_mii (inst, 0);
	if (fin_fout) {
		rc = asic_cfg_fin_fout(inst, ASIC_PTYPE_CI, cifport, 0, 1);
		if (rc) return (rc);

		rc = asic_cfg_tx_flush(inst, 0, cifport, 1);
		if (rc) return (rc);
	}	
	return (rc);
}


int asic_cfg_cif_mac(uint8_t inst, uint64_t cif_mask)
{
	int rc = 0, cifport;

	for (cifport = 0; cifport < ASIC(HI_COUNT); cifport++, cif_mask>>=1) {
		if (!(cif_mask & 1)) continue;
		rc = asic_cfg_ci_mac(inst, cifport, 0);
	}

	return (rc);
}

int asic_cfg_ss (uint8_t inst, uint8_t which_ss, uint8_t raminit, uint8_t gbeMode)
{
	uint32_t reg_addr, reg_data, data;
	int	 rc = 0, port, cnt;

	reg_addr = SS_REG_ADDR(CFG_FC, which_ss); 
	reg_data = ((0x01 << ASIC(SS_CFG_FC_FLD_MC_INIT)) |
		    (0x01 << ASIC(SS_CFG_FC_FLD_MM_INIT)) |
		    (0x01 << ASIC(SS_CFG_FC_FLD_INIT)) |
		    ((which_ss<6)? (ASIC_SS_CFG_FC_SIZE<< 1) :
				(ASIC_SS_CFG_FC_SIZE_1<<1))) ;
	rc = asic_wr(inst, reg_addr, reg_data);
	if (rc) return (rc);


	for (port = 0; port < ASIC(SS_PORT_COUNT); port++)  {

		// CFG_WR_PORT
		reg_addr = SS_REG_ADDR(CFG_WR_PORT0, which_ss);
		reg_addr+= (port * BA(ASIC(SS0_CFG_WR_PORT1) - ASIC(SS0_CFG_WR_PORT0)));

		data =  (((port == 8) && (which_ss <=  (1+ASIC(SS_COUNT)/2))) ||
			 ((which_ss >= ASIC(SS_COUNT)/2) && gbeMode)) ? 0x00 : 0xFF;

#ifdef DIAG_PORTOLA
		if (which_ss >= (ASIC(SS_COUNT) >> 1))
			data = 0;
#endif
		rc = asic_rd (inst, reg_addr, &reg_data);
		if (rc) return (rc);

		reg_data &= ~((0xFF << 2) | (0x3FFF << 10));	// Clear the bits
		reg_data |= ((data << 2) | (0x80 << 10));
		reg_data &= ~(0x01 << ASIC(SS_CFG_WR_PORT0_FLD_TD_EN));

		rc = asic_wr (inst, reg_addr, reg_data);
		if (rc) return (rc);

		// CFG_CLASS
		reg_addr = SS_REG_ADDR(CFG_CLASS_MAX0, which_ss);
		reg_addr+= (port * BA(ASIC(SS0_CFG_CLASS_MAX1)-ASIC(SS0_CFG_CLASS_MAX0)));

		reg_data = ((ASIC(SS_MAX_CELLS) << 0)  | (ASIC(SS_MAX_CELLS) << 8) | 
			    (ASIC(SS_MAX_CELLS) << 16) | (ASIC(SS_MAX_CELLS) << 24));
		rc = asic_wr (inst, reg_addr, reg_data);
		if (rc) return (rc);

		// CFG_CLASS47
		reg_addr = SS_REG_ADDR(CFG_CLASS47_MAX0, which_ss);
		reg_addr+= (port * BA(ASIC(SS0_CFG_CLASS47_MAX1)-ASIC(SS0_CFG_CLASS47_MAX0)));
		reg_data = ((ASIC(SS_MAX_CELLS) << 0)  | (ASIC(SS_MAX_CELLS) << 8) | 
			    (ASIC(SS_MAX_CELLS) << 16) | (ASIC(SS_MAX_CELLS) << 24));
		rc = asic_wr (inst, reg_addr, reg_data);
		if (rc) return (rc);

#ifdef DIAG_PORTOLA 	
        if (which_ss < 6) {
            // FRH  (0 | (1 << 3) | (2 << 6) | (3 << 9) | (4 << 12)
            //     | (5 << 15)| (6 << 18)| (7 << 21))
            reg_data = 0xFAC688;
            reg_addr = SS_REG_ADDR(CFG_RESRC0_MAP, which_ss);
            rc = asic_wr(inst, reg_addr, reg_data);
            // FRH INFO
            for (cnt=0; cnt < ASIC(SS_CLASS_COUNT); cnt++) {
		reg_addr = SS_REG_ADDR(CFG_FRH0_INFO0, which_ss);
		reg_addr += (port*BA(ASIC(SS0_CFG_FRH0_INFO1)-ASIC(SS0_CFG_FRH0_INFO0)));
		reg_addr += (cnt* BA(ASIC(SS0_CFG_FRH1_INFO1)-ASIC(SS0_CFG_FRH0_INFO0)));
                reg_data = ((0x01 << ASIC(SS_CFG_FRH0_INFO0_FLD_ACTION)) |
                            (0x2 << 24)      |  // XON
                            (0x4 << 16)      |  // XOFF
                            ((0x01 << cnt) << 8) |  // MEMBERS
                            (0x0E));            // Total Cells

                rc = asic_wr(inst, reg_addr, reg_data);
            }

	    // Program Total Cells and Threshold
	    reg_addr = SS_REG_ADDR(CFG_GLB_PAUSE0, which_ss);
	    reg_addr+= (port * BA(ASIC(SS0_CFG_GLB_PAUSE1)-ASIC(SS0_CFG_GLB_PAUSE0)));
	    reg_data = (0xFF << 10) | (0xF0); // total cells, threshold
	    rc = asic_wr (inst, reg_addr, reg_data);
	    if (rc) return (rc);

	    reg_addr = SS_REG_ADDR(CFG_GLB_PAUSE_FRH0, which_ss); 
	    reg_addr+= BA(port*(ASIC(SS0_CFG_GLB_PAUSE_FRH1) - ASIC(SS0_CFG_GLB_PAUSE_FRH0)));
	    reg_data = 0x00;
	    rc = asic_wr (inst, reg_addr, reg_data);
	    if (rc) return (rc);
        } else {
            reg_data = 0x00;
            reg_addr = SS_REG_ADDR(CFG_RESRC0_MAP, which_ss);
            rc = asic_wr(inst, reg_addr, reg_data);
            // FRH INFO
            for (cnt=0; cnt < ASIC(SS_CLASS_COUNT); cnt++) {
		reg_addr = SS_REG_ADDR(CFG_FRH0_INFO0, which_ss);
		reg_addr += (port*BA(ASIC(SS0_CFG_FRH0_INFO1)-ASIC(SS0_CFG_FRH0_INFO0)));
		reg_addr += (cnt* BA(ASIC(SS0_CFG_FRH1_INFO1)-ASIC(SS0_CFG_FRH0_INFO0)));

                reg_data = cnt? 0x00 :
			   ((0x01 << 28) |
                            (0x0F << 24) |  // XON
                            (0x17 << 16) |  // XOFF
                            (0xFF << 8)  |  // MEMBERS
                            (0x2A));        // Total Cells

                rc = asic_wr(inst, reg_addr, reg_data);
            }

	    // Program Total Cells and Threshold
	    reg_addr = SS_REG_ADDR(CFG_GLB_PAUSE0, which_ss);
	    reg_addr+= (port * BA(ASIC(SS0_CFG_GLB_PAUSE1)-ASIC(SS0_CFG_GLB_PAUSE0)));
	    reg_data = 0x00; // total cells, threshold
	    rc = asic_wr (inst, reg_addr, reg_data);
	    if (rc) return (rc);

	    reg_addr = SS_REG_ADDR(CFG_GLB_PAUSE_FRH0, which_ss); 
	    reg_addr+= BA(port*(ASIC(SS0_CFG_GLB_PAUSE_FRH1) - ASIC(SS0_CFG_GLB_PAUSE_FRH0)));
	    reg_data = 0x00;
	    rc = asic_wr (inst, reg_addr, reg_data);
	    if (rc) return (rc);
	}

#else
		// FRH  (0 | (1 << 3) | (2 << 6) | (3 << 9) | (4 << 12)
		//	   | (5 << 15)| (6 << 18)| (7 << 21))
		reg_data = 0xFAC688;	
		reg_addr = SS_REG_ADDR(CFG_RESRC0_MAP, which_ss);
		reg_addr+= (port * (BA(ASIC(SS0_CFG_RESRC1_MAP)-ASIC(SS0_CFG_RESRC0_MAP))));
		rc = asic_wr (inst, reg_addr, reg_data);
		if (rc) return (rc);

		// FRH INFO
		for (cnt=0; cnt < ASIC(SS_CLASS_COUNT); cnt++) {
			reg_addr = SS_REG_ADDR(CFG_FRH0_INFO0, which_ss);
			reg_addr += (port*BA(ASIC(SS0_CFG_FRH0_INFO1)-ASIC(SS0_CFG_FRH0_INFO0)));
			reg_addr += (cnt* BA(ASIC(SS0_CFG_FRH1_INFO1)-ASIC(SS0_CFG_FRH0_INFO0)));
			reg_data = ((0x01 << ASIC(SS_CFG_FRH0_INFO0_FLD_ACTION)) |
				    (0x2 << 24) 	 |	// XON
				    (0x4 << 16) 	 |	// XOFF
				    ((0x01 << cnt) << 8) |	// MEMBERS
				    (0x0E));			// Total Cells

			rc = asic_wr (inst, reg_addr, reg_data);
			if (rc) return (rc);
		}

		// Program Total Cells and Threshold
		reg_addr = SS_REG_ADDR(CFG_GLB_PAUSE0, which_ss);
		reg_addr+= (port * BA(ASIC(SS0_CFG_GLB_PAUSE1)-ASIC(SS0_CFG_GLB_PAUSE0)));
		reg_data = (0xFF << 10) | (0xF0); // total cells, threshold
		rc = asic_wr (inst, reg_addr, reg_data);
		if (rc) return (rc);

		reg_addr = SS_REG_ADDR(CFG_GLB_PAUSE_FRH0, which_ss); 
		reg_addr+= BA(port*(ASIC(SS0_CFG_GLB_PAUSE_FRH1) - ASIC(SS0_CFG_GLB_PAUSE_FRH0)));
		reg_data = 0x00;
		rc = asic_wr (inst, reg_addr, reg_data);
		if (rc) return (rc);
#endif
		reg_addr = SS_REG_ADDR(CFG_MCAST0,  which_ss);
		reg_addr+= BA(port * (ASIC(SS0_CFG_MCAST1)-ASIC(SS0_CFG_MCAST0))); 
		reg_data = 0xFF;		// Drop Threshold, Drop ena=0;
		rc = asic_wr (inst, reg_addr, reg_data);
		if (rc) return (rc);
	}	

	for (cnt = 0; cnt < ASIC(DD_COUNT); cnt++) {
		reg_addr = SS_REG_ADDR(CFG_DD_CNTL0, which_ss);
		reg_addr+= BA(cnt * (ASIC(SS0_CFG_DD_CNTL1)-ASIC(SS0_CFG_DD_CNTL0)));

		//           PQ_DEC      PQ_INC      HDTD_OFF   CHARGE_THRESHOD
		reg_data =  (1 << 24) | (1 << 16) | (2 << 8) | (4); 
		rc = asic_wr (inst, reg_addr, reg_data);
		if (rc) return (rc);

		reg_addr = SS_REG_ADDR(CFG_PQ_CNTL0, which_ss);
		reg_addr+= BA(cnt * (ASIC(SS0_CFG_PQ_CNTL1)-ASIC(SS0_CFG_PQ_CNTL0)));
		reg_data = ASIC(SS_CFG_PQ_CNTL0_DEF);
		rc = asic_wr (inst, reg_addr, reg_data);
		if (rc) return (rc);
	}

	reg_addr = SS_REG_ADDR(CFG_TD, which_ss);
	reg_data = 0x01;
	rc = asic_wr (inst, reg_addr, reg_data);
	if (rc) return (rc);

	reg_addr = SS_REG_ADDR(CFG_DD_TICK, which_ss);
	reg_data = 0x01;
	rc = asic_wr (inst, reg_addr, reg_data);
	if (rc) return (rc);

	for (cnt = 0; cnt < ASIC(SS_COUNT); cnt++) {
		reg_addr = SS_REG_ADDR(CFG_RD0_DD, which_ss);
		reg_addr+= BA(cnt * (ASIC(SS0_CFG_RD1_DD)-ASIC(SS0_CFG_RD0_DD)));
		reg_data = 0xFAC688 << 16;
		rc = asic_wr (inst, reg_addr, reg_data);
		if (rc) return (rc);

		reg_addr = SS_REG_ADDR(CFG_RD0_PQ, which_ss);
		reg_addr+= BA(cnt * (ASIC(SS0_CFG_RD1_PQ)-ASIC(SS0_CFG_RD0_PQ)));
		reg_data = 0x00;
		rc = asic_wr (inst, reg_addr, reg_data);
		if (rc) return (rc);
	}

	return (rc);
}

int asic_cfg_pg (uint8_t inst, uint8_t port, uint8_t is_hr)
{
	int rc = 0, cnt;
	uint32_t reg_data, reg_addr;

	reg_addr = is_hr ? 
		BA(ASIC(HR_DHS_ARB0) + ASIC(HR_DHS_ARB0__ENUM_PG_CNTL) + 
			(port * (ASIC(HR_DHS_ARB1)-ASIC(HR_DHS_ARB0)))) :
		BA(ASIC(NR_DHS_ARB0) + ASIC(NR_DHS_ARB0__ENUM_PG_CNTL) + 
			(port * (ASIC(NR_DHS_ARB1)-ASIC(NR_DHS_ARB0))));
	reg_data = (0x01 << 9);
	rc = asic_wr (inst, reg_addr, reg_data);
	if (rc) return (rc);

#ifdef _CHECK_
	reg_data = (quanta & 0x1F) | ((ipg & 0xF) << 5);
#else
	reg_data = 0x00; 
#endif
	rc = asic_wr (inst, reg_addr, reg_data);
	if (rc) return (rc);


	// Program Group parameters.
	for (cnt = 0; cnt < 4; cnt++) {
		reg_addr = is_hr ? 
			BA(ASIC(HR_DHS_ARB0) + ASIC(HR_DHS_ARB0__ENUM_GRP_DEC01) + cnt + 
				(port * (ASIC(HR_DHS_ARB1)-ASIC(HR_DHS_ARB0)))) :
			BA(ASIC(NR_DHS_ARB0) + ASIC(NR_DHS_ARB0__ENUM_GRP_DEC01) + cnt + 
				(port * (ASIC(NR_DHS_ARB1)-ASIC(NR_DHS_ARB0))));
			reg_data = 0x00; 
#ifdef _CHECK_
	reg_data = (grp_dec0 & 0xFF) | ((grp_dec1 & 0xFF) << 8);
#endif
			rc = asic_wr (inst, reg_addr, reg_data);
			if (rc) return (rc);
	}

	// Program Queue parameters.
	for (cnt = 0; cnt < 8; cnt++) {
		reg_addr = is_hr ? 
			BA(ASIC(HR_DHS_ARB0) + ASIC(HR_DHS_ARB0__ENUM_QUE_DEC0) + cnt + 
				(port * (ASIC(HR_DHS_ARB1)-ASIC(HR_DHS_ARB0)))) :
			BA(ASIC(NR_DHS_ARB0) + ASIC(NR_DHS_ARB0__ENUM_QUE_DEC0) + cnt + 
				(port * (ASIC(NR_DHS_ARB1)-ASIC(NR_DHS_ARB0))));
			reg_data = 0x00; 
#ifdef _CHECK_
	reg_data =
#endif
			rc = asic_wr (inst, reg_addr, reg_data);
			if (rc) return (rc);
	}
	return (rc);	
}

int asic_cfg_nr_check(uint8_t inst)
{
	int rc = 0, cnt, oq, tcnt;
	uint32_t reg_addr, data;
	uint32_t nr_oq_reg_addr[] = {
				ASIC(NR_CFG_OQ0_PORT0),		
				ASIC(NR_CFG_OQ1_PORT0),		
				ASIC(NR_CFG_OQ2_PORT0),		
				ASIC(NR_CFG_OQ3_PORT0),		
#ifdef DIAG_PORTOLA
				ASIC(NR_CFG_OQ4_PORT0),		
				ASIC(NR_CFG_OQ5_PORT0),		
#endif
				};

	for (oq = 0; oq < ASIC(OQ_COUNT); oq++) {

		reg_addr = BA(nr_oq_reg_addr[oq]);

		tcnt = (oq < 2) ? ASIC(OQ_PORT_COUNT) : (ASIC(OQ_PORT_COUNT)-1);
		for (cnt = 0; !rc && cnt < tcnt; cnt++) {
			rc = asic_rd (inst, reg_addr, &data); // Queue Enable.
			reg_addr += BA(ASIC(NR_CFG_OQ0_PORT1) - ASIC(NR_CFG_OQ0_PORT0));
		}
	}
	return (0);
}

int asic_cfg_hr_nr(uint8_t inst)
{
	int rc = 0, ss, cnt, oq, tcnt;
	uint32_t reg_addr;
	uint32_t hr_oq_reg_addr[] = {
				ASIC(HR_CFG_OQ0_PORT0),		
				ASIC(HR_CFG_OQ1_PORT0),		
				ASIC(HR_CFG_OQ2_PORT0),		
				ASIC(HR_CFG_OQ3_PORT0),		
#ifdef DIAG_PORTOLA
				ASIC(HR_CFG_OQ4_PORT0),		
				ASIC(HR_CFG_OQ5_PORT0),		
#endif
				};

	uint32_t nr_oq_reg_addr[] = {
				ASIC(NR_CFG_OQ0_PORT0),		
				ASIC(NR_CFG_OQ1_PORT0),		
				ASIC(NR_CFG_OQ2_PORT0),		
				ASIC(NR_CFG_OQ3_PORT0),		
#ifdef DIAG_PORTOLA
				ASIC(NR_CFG_OQ4_PORT0),		
				ASIC(NR_CFG_OQ5_PORT0),		
#endif
				};

	for (ss=0; !rc && ss < ASIC(SS_COUNT); ss++) {
		rc = asic_cfg_ss (inst, ss, 1, 0);
	}

	for (oq = 0; oq < ASIC(OQ_COUNT); oq++) {
		reg_addr = BA(hr_oq_reg_addr[oq]);
		for (cnt = 0; !rc && cnt < ASIC(OQ_PORT_COUNT); cnt++) {
			rc = asic_wr (inst, reg_addr, 0xFF); // Queue Enable.
			reg_addr += BA(ASIC(HR_CFG_OQ0_PORT1) - ASIC(HR_CFG_OQ0_PORT0));
		}

		reg_addr = BA(nr_oq_reg_addr[oq]);
		tcnt = (oq < 2) ? ASIC(OQ_PORT_COUNT) : (ASIC(OQ_PORT_COUNT)-1);
		for (cnt = 0; !rc && cnt < tcnt; cnt++) {
			rc = asic_wr (inst, reg_addr, 0xFF); // Queue Enable.
			reg_addr += BA(ASIC(NR_CFG_OQ0_PORT1) - ASIC(NR_CFG_OQ0_PORT0));
		}
	}

#ifdef DIAG_PORTOLA
// Removed for woodside as default values are good.

	rc = asic_wr (inst, BA(ASIC(NR_CFG_ARB)), 0xAA);
	if (rc) return (rc);

	rc = asic_wr (inst, BA(ASIC(HR_CFG_ARB_OK_DLY)), 0xAA);
	if (rc) return (rc);
#endif

	return (rc); 
}

int asic_init_hf_nf_mem (uint8_t inst)
{
	int rc = 0, cnt;
	uint32_t reg_data, reg_addr, entry;

	// Initialize the Unicast and Multicast Memory
	rc = asic_wr(inst, BA(ASIC(NF_CFG_MEM)), 0x01);
	if (rc) return (rc);

	sleep(1);
	for (cnt = 0;  !rc && cnt < ASIC(HF_COUNT); cnt++) {
		reg_addr = BA(ASIC(HF0_CFG_MEM)+
				(cnt*(ASIC(HF1_CFG_MEM)-ASIC(HF0_CFG_MEM))));
		reg_data = 0x01 << ASIC(HF_CFG_MEM_FLD_INIT);
		rc = asic_wr (inst, reg_addr, reg_data);
		if (rc) return (rc);

		sleep(1);

		// Program SVIF to allow.
		DIAG_PRINT(DIAG_PRINT_CONFIG,
                           "    Woo-%d Config HF%d SVIF\n", inst, cnt);
		reg_addr = BA(ASIC(HF0_DHS_SVIF_MEM) + 
			(cnt * (ASIC(HF1_DHS_SVIF_MEM)-ASIC(HF0_DHS_SVIF_MEM))));
		reg_data = 0x0F;		// Field OK.
		for (entry = 0; !rc && (entry < ASIC(HF_DHS_SVIF_MEM_ARRAYSIZE)); 
			entry++, reg_addr+=4) {
			rc = asic_wr (inst, reg_addr, reg_data);
		}

		// Program HF Segment mapping HF0-HF3(3,2,1,0) HF4-HF7(7,6,5,4)
		reg_addr = BA(ASIC(HF0_CFG_HIF0)+
				(cnt*(ASIC(HF1_CFG_HIF0)-ASIC(HF0_CFG_HIF0))));
		for (entry = 0; entry < (ASIC(HI_COUNT)>>2); entry++) {
			reg_data = (entry > 3)? 0x0FAC : 0x0688;
			asic_wr (inst, reg_addr, reg_data);
			reg_addr += BA(ASIC(HF0_CFG_HIF1)-ASIC(HF0_CFG_HIF0));
		}
	}

	reg_addr = BA(ASIC(NF_DHS_UCAST_MEMIF_DATA));
	reg_data = 0x00;
	for (entry = 0; entry < ASIC(NF_DHS_UCAST_MEMIF_DATA_ARRAYSIZE); entry++, reg_addr+=4) {
		asic_wr (inst, reg_addr, reg_data);
	}

	// Program NF Segment mapping NF0-NF3(3,2,1,0) NF4-NF7(7,6,5,4)
	reg_addr = BA(ASIC(NF_CFG_NIF0));
	for (entry = 0; entry < ASIC(NI_COUNT); entry++) {
		reg_data = (entry > 3)? 0x0FAC : 0x0688;
		asic_wr (inst, reg_addr, reg_data);
		reg_addr += BA(ASIC(NF_CFG_NIF1)-ASIC(NF_CFG_NIF0));
	}
	if (rc) return (rc);

	return (rc);	
}

int asic_hf_nf_init_check(uint8_t inst)
{
	int rc = 0;
	uint8_t  init_flag = 0x00;
	uint32_t reg_data, timeout = 1000;

	do {
		if (!(init_flag & 0x01)) {
			rc = asic_rd (inst, BA(ASIC(NF_INT_NORM)), &reg_data);
			if (rc) return (rc);
			if (reg_data & (0x01 << ASIC(NF_INT_NORM_FLD_MEM_INIT_DONE))) {
				init_flag |= (0x01 << 0);
			}
		}

		if (!(init_flag & 0x02)) {
			rc = asic_rd (inst, BA(ASIC(HF0_INT_NORM)), &reg_data);
			if (rc) return (rc);
			if (reg_data & (0x01 << ASIC(HF_INT_NORM_FLD_MEM_INIT_DONE))) {
				init_flag |= (0x01 << 1);
			}
		}

		if (!(init_flag & 0x04)) {
			rc = asic_rd (inst, BA(ASIC(HF1_INT_NORM)), &reg_data);
			if (rc) return (rc);
			if (reg_data & (0x01 << ASIC(HF_INT_NORM_FLD_MEM_INIT_DONE))) {
				init_flag |= (0x01 << 2);
			}
		}

		if (!(init_flag & 0x08)) {
			rc = asic_rd (inst, BA(ASIC(HF2_INT_NORM)), &reg_data);
			if (rc) return (rc);
			if (reg_data & (0x01 << ASIC(HF_INT_NORM_FLD_MEM_INIT_DONE))) {
				init_flag |= (0x01 << 3);
			}
		}

		if (!(init_flag & 0x10)) {
			rc = asic_rd (inst, BA(ASIC(HF3_INT_NORM)), &reg_data);
			if (rc) return (rc);
			if (reg_data & (0x01 << ASIC(HF_INT_NORM_FLD_MEM_INIT_DONE))) {
				init_flag |= (0x01 << 4);
			}
		}
#ifdef DIAG_PORTOLA
		if (!(init_flag & 0x20)) {
			rc = asic_rd (inst, BA(ASIC(HF4_INT_NORM)), &reg_data);
			if (rc) return (rc);
			if (reg_data & (0x01 << ASIC(HF_INT_NORM_FLD_MEM_INIT_DONE))) {
				init_flag |= (0x01 << 5);
			}
		}

		if (!(init_flag & 0x40)) {
			rc = asic_rd (inst, BA(ASIC(HF5_INT_NORM)), &reg_data);
			if (rc) return (rc);
			if (reg_data & (0x01 << ASIC(HF_INT_NORM_FLD_MEM_INIT_DONE))) {
				init_flag |= (0x01 << 6);
			}
		}
#endif
	} while (--timeout && (init_flag != ASIC(HF_INIT)));

	if (init_flag != ASIC(HF_INIT)) {
		if (!(init_flag & 0x01)) 
			printf("  Error:  Woo%d NF Mem init \n", inst);
		
		if (!(init_flag & 0x02)) 
			printf("  Error:  Woo%d HF0 Mem init \n", inst);
		
		if (!(init_flag & 0x04)) 
			printf("  Error:  Woo%d HF1 Mem init \n", inst);
		
		if (!(init_flag & 0x08)) 
			printf("  Error:  Woo%d HF2 Mem init \n", inst);
		
		if (!(init_flag & 0x10)) 
			printf("  Error:  Woo%d HF3 Mem init \n", inst);
#ifdef DIAG_PORTOLA
		if (!(init_flag & 0x20)) 
			printf("  Error:  Woo%d HF4 Mem init \n", inst);
		
		if (!(init_flag & 0x40)) 
			printf("  Error:  Woo%d HF5 Mem init \n", inst);
#endif
	}	
	return ((init_flag != ASIC(HF_INIT))? -1 : rc);
}

int asic_hr_nr_init_check(uint8_t inst)
{
	int rc = 0;
	uint8_t  cnt, init_flag = 0x00;
	uint32_t reg_data, reg_addr, timeout = 200;

	do {
		for (cnt = 0; cnt < ASIC(SS_COUNT); cnt++) {
			if (init_flag & (0x01 << cnt)) continue;

			reg_addr = SS_REG_ADDR(INT_NORM, cnt);
			rc = asic_rd (inst, reg_addr, &reg_data);
			if (rc) return (rc);

			if ((reg_data & 0xF) == 0xF)
				init_flag |= (0x01 << cnt);
		}

		usleep(1000);
	} while (--timeout && (init_flag != 0xFF));

	return ((init_flag != 0xFF)? -1 : rc);
}

int asic_ss_ena (uint8_t inst)
{
	int rc = 0;
	uint8_t  cnt, port;
	uint32_t reg_data, reg_addr;

	for (cnt = 0; !rc && (cnt < ASIC(SS_COUNT)); cnt++) {
		for (port = 0; !rc && (port < ASIC(SS_PORT_COUNT)); port++) {
			reg_addr = SS_REG_ADDR(CFG_WR_PORT0, cnt);
			reg_addr+= (port * BA(ASIC(SS0_CFG_WR_PORT1) - ASIC(SS0_CFG_WR_PORT0)));

			rc = asic_rd (inst, reg_addr, &reg_data);
			if (rc) return (rc);

			reg_data |= ((0x01 << ASIC(SS_CFG_WR_PORT0_FLD_EN)) |
				     (0x01 << ASIC(SS_CFG_WR_PORT0_FLD_EM_STOP_EN)));

			rc = asic_wr (inst, reg_addr, reg_data);
			if (rc) return (rc);
		}
	}
	return (rc);
}
int asic_ss_dis (uint8_t inst)
{
	int rc = 0;
	uint8_t  cnt, port;
	uint32_t reg_data, reg_addr;

	for (cnt = 0; !rc && (cnt < ASIC(SS_COUNT)); cnt++) {
		for (port = 0; !rc && (port < ASIC(SS_PORT_COUNT)); port++) {
			reg_addr = SS_REG_ADDR(CFG_WR_PORT0, cnt);
			reg_addr+= (port * BA(ASIC(SS0_CFG_WR_PORT1) - ASIC(SS0_CFG_WR_PORT0)));

			rc = asic_rd (inst, reg_addr, &reg_data);
			if (rc) return (rc);

			reg_data &= ~(0x01 << ASIC(SS_CFG_WR_PORT0_FLD_EN));

			rc = asic_wr (inst, reg_addr, reg_data);
			if (rc) return (rc);
		}
	}
	return (rc);
}

int asic_ss_ena_check (uint8_t inst)
{
	int rc = 0;
	uint8_t  cnt, port, err=0;
	uint32_t reg_data, reg_addr;

	for (cnt = 0; !rc && (cnt < ASIC(SS_COUNT)); cnt++) {
		for (port = 0; !rc && (port < ASIC(SS_PORT_COUNT)); port++) {
			reg_addr = SS_REG_ADDR(CFG_WR_PORT0, cnt);
			reg_addr+= (port * BA(ASIC(SS0_CFG_WR_PORT1) - ASIC(SS0_CFG_WR_PORT0)));

			rc = asic_rd (inst, reg_addr, &reg_data);
			if (rc) return (rc);

			if (!(reg_data & (0x01 << ASIC(SS_CFG_WR_PORT0_FLD_EN)))) {
				printf("  ASIC%d SS%d Port%d Init Failed\n", inst, cnt, port);
				err = -1;
			}
		}
	}
	return (err? err : rc);
}


int asic_core_init (uint8_t inst, uint64_t hi_mask, uint64_t ni_mask,
                    uint32_t hif_speed)
{
        int rc = 0;

	DIAG_PRINT(DIAG_PRINT_CONFIG, "  Woo-%d Config HR/NR\n", inst);
        rc = asic_cfg_hr_nr(inst);
        if (rc) return (rc);

	DIAG_PRINT(DIAG_PRINT_CONFIG, "  Woo-%d Config HF/NF\n", inst);
        rc = asic_init_hf_nf_mem(inst);
        if (rc) return (rc);

	// Configure MAC.
	DIAG_PRINT(DIAG_PRINT_CONFIG, "  Woo-%d Config HIF\n", inst);
        rc = asic_cfg_hif_mac(inst, hi_mask, hif_speed);
        if (rc) return (rc);

	DIAG_PRINT(DIAG_PRINT_CONFIG, "  Woo-%d Config NIF\n", inst);
        rc = asic_cfg_nif_mac(inst, ni_mask);
        if (rc) return (rc);

	DIAG_PRINT(DIAG_PRINT_CONFIG, "  Woo-%d Config BIF\n", inst);
        rc = asic_cfg_bif_mac(inst, 0x01);
        if (rc) return (rc);

	DIAG_PRINT(DIAG_PRINT_CONFIG, "  Woo-%d Config CIF\n", inst);
        rc = asic_cfg_cif_mac(inst, 0x01);
        if (rc) return (rc);

	DIAG_PRINT(DIAG_PRINT_CONFIG, "  Woo-%d Check HF/NF\n", inst);
	rc = asic_hf_nf_init_check(inst);
	if (rc) return (rc);

	DIAG_PRINT(DIAG_PRINT_CONFIG, "  Woo-%d Check HR/NR\n", inst);
	rc = asic_hr_nr_init_check(inst);
	if (rc) return (rc);

	DIAG_PRINT(DIAG_PRINT_CONFIG, "  Woo-%d SS Enable\n", inst);
	rc = asic_ss_ena(inst);
	if (rc) return (rc);

	DIAG_PRINT(DIAG_PRINT_CONFIG, "  Woo-%d SS Check\n", inst);
	rc = asic_ss_ena_check(inst);
	if (rc) return (rc);

        return (rc);
}

static int asic_fuse_poll_for_cmplt(uint8_t inst, char *cmplt_str)
{
    int         rc = 0, fuse_retries = 100, poll_delay = 100;
    uint32_t    data;

    do {
#if defined(DIAG_WOODSIDE)
    //
    // Poll the pass bit and fail bit both in
    // Register :   woo_misc_sta_spare
    // Fields   :   Pass [0]
    //          :   Fail [1]
        rc = asic_rd( inst, BA(ASIC(MISC_STA_SPARE)), &data );
        if (rc) return (rc);

#elif defined(DIAG_PORTOLA)
    //
    // Poll the pass bit and fail bit both in
    // Register :   woo_misc_sram_fuse
    // Fields   :   Pass [1]
    //          :   Fail [0]
        rc = asic_rd( inst, BA(ASIC(MISC_SRAM_FUSE)), &data );
        if (rc) return (rc);
#endif
        if ( data & 0x03 ) {
            break;
        }
        usleep(poll_delay);
    } while (--fuse_retries);

    //
    // if fail is detected here, crib and quit.
    // if we pass go tp next step.
    //
#if defined(DIAG_WOODSIDE)
    if ((data &0x3) == 0x1) {
        DIAG_PRINT(DIAG_PRINT_CONFIG, "%s completed ok\n", cmplt_str);
    }
    else if ((data &0x3) == 0x2) {
        printf("%s  completed with error\n", cmplt_str);
    } else {
        printf("%s didn't complete %x\n", cmplt_str, (data &0x3));
        return 1;
    }
#elif defined(DIAG_PORTOLA)
    if (data & (0x1 << ASIC(MISC_SRAM_FUSE_FLD_DONE_PASS))) {
        DIAG_PRINT(DIAG_PRINT_CONFIG, "%s completed ok\n", cmplt_str);
    }
    else if (data & (0x1 << ASIC(MISC_SRAM_FUSE_FLD_DONE_FAIL))) {
        printf("%s  completed with error\n", cmplt_str);
    }
    else {
        printf("%s didn't complete\n", cmplt_str);
        return 1;
    }
#endif
    return 0;
}

int asic_fuse_load (uint8_t inst)
{
    int         rc = 0;

    //Load the data from the eFUSE RAM into the REI(RAM-eFuse interface) module
    rc = asic_wr (inst, BA(ASIC(MISC_CORE_SRAM)), 0x0 /*out of rst*/);
    if (rc) return (rc);

    rc = asic_wr (inst, BA(ASIC(MISC_CORE_SRAM)), 0x4 /*FUSE_RUN*/);
    if (rc) return (rc);
    rc = asic_fuse_poll_for_cmplt(inst, "  Woo-0 eFuse value loading to REI ");
    if (rc) return (rc);

    //Disable the FUSE RUN
    rc = asic_wr (inst, BA(ASIC(MISC_CORE_SRAM)), 0x0 /*out of rst*/);
    if (rc) return (rc);

    DIAG_PRINT(DIAG_PRINT_CONFIG, 
              "  Woo-0 Loading efuse info from REI block to RAM\n");
    rc = asic_wr (inst, BA(ASIC(MISC_CORE_SRAM)), 0x2 /*FUSE_MODE*/);
    if (rc) return (rc);

    rc = asic_wr (inst, BA(ASIC(MISC_CORE_SRAM)), 0x6 /*FUSE_MODE | FUSE_RUN*/);
    if (rc) return (rc);
    rc = asic_fuse_poll_for_cmplt(inst, "  Woo-0 eFuse value loading to RAM ");
    if (rc) return (rc);

    //Disable the FUSE RUN and leave the Reset deasserted
    rc = asic_wr (inst, BA(ASIC(MISC_CORE_SRAM)), 0x2 /*FUSE_MODE*/);
    if (rc) return (rc);

    rc = asic_wr (inst, BA(ASIC(MISC_CORE_SRAM)), 0x0 /*out of rst*/);
    if (rc) return (rc);
    return 0;
}

