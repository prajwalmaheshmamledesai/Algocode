/********************************************************************
 *
 *      File:   woo_serdes.c
 *      Name:   Sudharshan Kadari 
 *
 *      Description: ASIC level functions for Woodside serdes
 *
 *
 *      Copyright (c) 1984-2009 by Cisco Systems, Inc.
 ********************************************************************/
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include "woo_woo_naxislave_define.h"
#include "woo_ops.h"

#if defined(DIAG_WOODSIDE)
void woo_speedup_sbus (uint8_t inst)
{
    int deviceId, rc=0;
    unsigned int ref_clk_divider=3, data=0;
    /*
     * Writing to the SBUS controller clock divider register.
     * Writing to this register in the non realsbus build shouldn't make 
     * any difference(in the generate clock frequency and also the register 
     * value itself.)
     * Writing to this register in the regular top build should cause the sbus 
     * operating to go up from 1.288Ghz(156.25/128) to 19.5Ghz(156.25/8).
     */
    deviceId = ASIC(SBUS_CONTROLLER); /* SBUS Controller */
    //Read & check
    rc = asic_rd(inst, ASIC_SBUS_ADDR(deviceId, 0xa), &data);
    //Write
    if ((data&0xFF) != ref_clk_divider) {
        rc = asic_wr(inst, ASIC_SBUS_ADDR(deviceId, 0xa), ref_clk_divider);
        usleep(100);
    }
    return;
}
#endif

int asic_cfg_hi_pcs_short_ipg (uint8_t inst, uint8_t hi)
{
	uint32_t dev_id = ASIC_HI_SERDES_SBUS_DEVID(hi);
	return(asic_wr (inst, ASIC_PCS_ADDR(dev_id, 0), 0x08));
}

int asic_cfg_ni_pcs_short_ipg (uint8_t inst, uint8_t ni)
{
	uint32_t dev_id = ASIC_NI_SERDES_SBUS_DEVID(ni);
	return(asic_wr (inst, ASIC_PCS_ADDR(dev_id, 0), 0x08));
}

/// FIX FIX HIMASK IS PASSED _ NOT HI..,
/// FIX FIX ADD NI MASK ALso 
int asic_cfg_hi_serdes_rate(uint8_t inst, uint8_t hi, uint8_t serial_rate)
{
	int rc = 0;
#ifndef DIAG_PORTOLA
	uint32_t reg_data, reg_addr, rate_sel, ref_sel;

  //Serial_rate      tx/rx_rate_sel[6:0]     ref_sel[5:0]
  // 10.3125Gbps(0)    0x40                    0x1B
  // 3.125 Gbps(1)     0x10                    0x0B
  // 1.25  Gbps(2)     0x63                    0x13

	switch(serial_rate) {
		case	2:			// AN Mode.
			rate_sel = 0x10;
			ref_sel  = 0x0B;
			break;

		case	1:			//1G
			rate_sel = 0x63;
			ref_sel  = 0x13;
			break;

		default:			// 10G
			rate_sel = 0x40;
			ref_sel  = 0x1B;
			break;
	}

	reg_addr = HI_REG_ADDR(CFG_XGMII_CNTL_00, hi);

	rc = asic_rd (inst, reg_addr, &reg_data);	
	if (rc) return (rc);

	// Clear rate_sel and ref_sel
	reg_data &= ~((0x3F << 8) | (0x7F << 15)); 
	reg_data |= (ref_sel << 8) | (rate_sel << 15);

	rc = asic_wr (inst, reg_addr, reg_data);	
	if (rc) return (rc);
#endif // DIAG_PORTOLA

	return (rc);
}

int asic_cfg_ni_serdes_rate(uint8_t inst, uint8_t ni, uint8_t serial_rate)
{
	int rc = 0;
	uint32_t reg_data, reg_addr, rate_sel, ref_sel;

	switch(serial_rate) {
		case	2:
			rate_sel = 0x10;
			ref_sel  = 0x0B;
			break;

		case	1:
			rate_sel = 0x63;
			ref_sel  = 0x13;
			break;

		default:
			rate_sel = 0x40;
			ref_sel  = 0x1B;
			break;
	}

	reg_addr = NI_REG_ADDR(CFG_XGMII_CNTL_00, ni);

	rc = asic_rd (inst, reg_addr, &reg_data);	
	if (rc) return (rc);

	// Clear rate_sel and ref_sel
	reg_data &= ~((0x3F << 8) | (0x7F << 15)); 
	reg_data |= (ref_sel << 8) | (rate_sel << 15);

	rc = asic_wr (inst, reg_addr, reg_data);	
	return (rc);
}

int asic_cfg_serdes_rate (uint8_t inst, uint64_t hi_mask, uint64_t ni_mask,
                          uint8_t hif_rate, uint8_t nif_rate)
{
	int port, rc = 0;
        for (port = 0; !rc && (port < ASIC(HI_COUNT)); port++, hi_mask >>=1) {
                if (!(hi_mask & 0x01)) continue;

	/*	rc = asic_cfg_hi_pcs_short_ipg(inst, port);
		if (!rc) */ 
                rc = asic_cfg_hi_serdes_rate(inst, port, hif_rate);
	}

        for (port = 0; !rc && port < ASIC(NI_COUNT); port++, ni_mask >>=1) {
                if (!(ni_mask & 0x01)) continue;
	/*	rc = asic_cfg_ni_pcs_short_ipg(inst, port);
		if (!rc) */ 
                rc = asic_cfg_ni_serdes_rate(inst, port, nif_rate);
	}
	return (rc);
}

int asic_cfg_hi_serdes_sig_loss_mask(uint8_t inst, uint8_t hi, uint8_t gbe_mode)
{
    int rc = 0;
    uint32_t reg_data, reg_addr;

    reg_addr = HI_REG_ADDR(CFG_SERDES_SIG_LOSS_MASK, hi);
    reg_data = gbe_mode ? 0x3 : 0x0;

    rc = asic_wr (inst, reg_addr, reg_data);
    return (rc);
}

int asic_cfg_hi_8b10b_align_en(uint8_t inst, uint8_t hi, uint8_t ena)
{
	int rc = 0;
#ifndef DIAG_PORTOLA
	uint32_t reg_addr = HI_REG_ADDR(CFG_XGMII_CNTL_01, hi);

	rc = (ena ? asic_bset : asic_bclr)
		 (inst, reg_addr, ASIC(HI_CFG_XGMII_CNTL_01_FLD_RX_8B10B_ALIGN_EN));
#endif // DIAG_PORTOLA
	return (rc);
}

int asic_cfg_ni_8b10b_align_en(uint8_t inst, uint8_t ni, uint8_t ena)
{
	int rc = 0;
	uint32_t reg_addr = NI_REG_ADDR(CFG_XGMII_CNTL_01, ni);

	rc = (ena ? asic_bset : asic_bclr) 
		(inst, reg_addr, ASIC(NI_CFG_XGMII_CNTL_01_FLD_RX_8B10B_ALIGN_EN));
	return (rc);
}


int asic_cfg_8b10b_align_en(uint8_t inst, uint64_t hi_mask, 
		uint64_t ni_mask, uint8_t ena)
{
	int rc = 0, port;

	for (port = 0; !rc && (port < ASIC(HI_COUNT)); port++, hi_mask >>=1) {
		if (!(hi_mask & 0x01)) continue;
		rc = asic_cfg_hi_8b10b_align_en(inst, port, ena);
	}

	for (port = 0; !rc && (port < ASIC(NI_COUNT)); port++, ni_mask >>=1) {
		if (!(ni_mask & 0x01)) continue;
		rc = asic_cfg_ni_8b10b_align_en(inst, port, ena);
	}

	return (rc);
}

int asic_cfg_sbus_pcs_reset(uint8_t inst, uint8_t rx_rst, uint8_t tx_rst, uint8_t bypass)
{
	int rc = 0, dev_id = 125;

	rc = asic_wr (inst, ASIC_SBUS_ADDR(dev_id, 0), 
		((tx_rst&0x01)<<2) | ((rx_rst&0x01) << 1) | 0x01); 
	if (rc) return (rc);

	rc = asic_wr (inst, ASIC_SBUS_ADDR(dev_id, 1), (((bypass&0x01) << 1) | 0x01)); 
	return (rc);
}

int asic_cfg_hi_pcs_reset(uint8_t inst, uint8_t hi, uint8_t rx_rst, uint8_t tx_rst, uint8_t bypass)
{
	int rc = 0;
#ifndef DIAG_PORTOLA
	uint32_t reg_data, reg_addr = HI_REG_ADDR(CFG_XGMII_CNTL_00, hi);

	rc = asic_rd (inst, reg_addr, &reg_data);	
	if (rc) return (rc);

	if (rx_rst) {
		reg_data |= 0x01 << ASIC(HI_CFG_XGMII_CNTL_FLD_PCS_RX_RESET);
	} else {
		reg_data &= ~(0x01 << ASIC(HI_CFG_XGMII_CNTL_FLD_PCS_RX_RESET));
	}
	if (tx_rst) {
		reg_data |= 0x01 << ASIC(HI_CFG_XGMII_CNTL_FLD_PCS_TX_RESET);
	} else {
		reg_data &= ~(0x01 << ASIC(HI_CFG_XGMII_CNTL_FLD_PCS_TX_RESET));
	}
	if (bypass) {
		reg_data |= 0x01 << ASIC(HI_CFG_XGMII_CNTL_FLD_PCS_BYPASS);
	} else {
		reg_data &= ~(0x01 << ASIC(HI_CFG_XGMII_CNTL_FLD_PCS_BYPASS));
	}

	rc = asic_wr (inst, reg_addr, reg_data);	
#endif
	return (rc);
}


int asic_cfg_ni_pcs_reset(uint8_t inst, uint8_t ni, uint8_t rx_rst, uint8_t tx_rst, 
			uint8_t bypass)
{
	int rc = 0;
	uint32_t reg_data, reg_addr = NI_REG_ADDR(CFG_XGMII_CNTL, ni);

	rc = asic_rd (inst, reg_addr, &reg_data);	
	if (rc) return (rc);

	if (rx_rst) {
		reg_data |= 0x01 << ASIC(NI_CFG_XGMII_CNTL_FLD_PCS_RX_RESET);
	} else {
		reg_data &= ~(0x01 << ASIC(NI_CFG_XGMII_CNTL_FLD_PCS_RX_RESET));
	}
	if (tx_rst) {
		reg_data |= 0x01 << ASIC(NI_CFG_XGMII_CNTL_FLD_PCS_TX_RESET);
	} else {
		reg_data &= ~(0x01 << ASIC(NI_CFG_XGMII_CNTL_FLD_PCS_TX_RESET));
	}
	if (bypass) {
		reg_data |= 0x01 << ASIC(NI_CFG_XGMII_CNTL_FLD_PCS_BYPASS);
	} else {
		reg_data &= ~(0x01 << ASIC(NI_CFG_XGMII_CNTL_FLD_PCS_BYPASS));
	}

	rc = asic_wr (inst, reg_addr, reg_data);	
	return (rc);
}

int asic_cfg_pcs_reset(uint8_t inst, uint64_t hi_mask, 
			uint64_t ni_mask,
			uint8_t rx_rst, uint8_t tx_rst, 
			uint8_t bypass) 
{
	int rc = 0, port;

	for (port = 0; !rc && (port < ASIC(HI_COUNT)); port++, hi_mask >>=1) {
		if (!(hi_mask & 0x01)) continue;
		rc = asic_cfg_hi_pcs_reset(inst, port, rx_rst, tx_rst, bypass);
	}

	for (port = 0; !rc && (port < ASIC(NI_COUNT)); port++, ni_mask >>=1) {
		if (!(ni_mask & 0x01)) continue;
		rc = asic_cfg_ni_pcs_reset(inst, port, rx_rst, tx_rst, bypass);
	}
	return (rc);
}

#if defined(DIAG_BROCKWAY)
typedef struct _brockway_port_info_ {
    uint8_t hif_id;
    uint8_t xfi_rx_inv;
    uint8_t xfi_tx_inv;
} brockway_port_info_t;
/* XFI Port pairs are inverted for some HIF ports on Brockway */
brockway_port_info_t brockway_hi_info[] =
{
/* hif_id xfi_rx_inv xfi_tx_inv */
 { 0,  1, 1 },
 { 1,  0, 0 },
 { 2,  1, 1 },
 { 3,  0, 0 },
 { 4,  1, 1 },
 { 5,  0, 0 },
 { 6,  1, 1 },
 { 7,  0, 0 },
 { 8,  1, 0 },
 { 9,  1, 1 },
 {10,  0, 1 },
 {11,  0, 0 },
 {12,  1, 0 },
 {13,  0, 1 },
 {14,  1, 1 },
 {15,  0, 0 },
 {16,  1, 0 },
 {17,  0, 1 },
 {18,  1, 1 },
 {19,  0, 1 },
 {20,  1, 0 },
 {21,  0, 1 },
 {22,  0, 0 },
 {23,  0, 0 },
 {24,  0, 0 },
 {25,  0, 1 },
 {26,  1, 1 },
 {27,  0, 0 },
 {28,  1, 0 },
 {29,  0, 0 },
 {30,  1, 1 },
 {31,  0, 0 },
};

static int brockway_get_xfi_polarity (int port, uint8_t *rx_inv, uint8_t *tx_inv)
{
    if (port > ASIC(HI_COUNT)) return (-1);
    *rx_inv = brockway_hi_info[port].xfi_rx_inv;
    *tx_inv = brockway_hi_info[port].xfi_tx_inv;
    return (0);
}
#endif

int asic_cfg_hi_serdes_pol_inv(uint8_t inst, uint8_t hi, uint8_t rx_pol_inv, uint8_t tx_pol_inv)
{
	int rc = 0;
#ifndef DIAG_PORTOLA
	uint32_t reg_addr = HI_REG_ADDR(CFG_XGMII_CNTL_00, hi);

	rc = (rx_pol_inv? asic_bset : asic_bclr)
		(inst, reg_addr, ASIC(HI_CFG_XGMII_CNTL_FLD_RX_POLARITY_INV_EN));	
	if (rc) return (rc);

	reg_addr = HI_REG_ADDR(CFG_XGMII_CNTL_01, hi);

	rc = (tx_pol_inv ? asic_bset : asic_bclr)
		(inst, reg_addr, ASIC(HI_CFG_XGMII_CNTL_01_FLD_AN_TX_POLARITY_INV_EN));	
#endif
	return (rc);
}

int asic_cfg_ni_serdes_pol_inv(uint8_t inst, uint8_t ni, uint8_t rx_pol_inv, uint8_t tx_pol_inv)
{
	int rc = 0;
	uint32_t reg_addr = NI_REG_ADDR(CFG_XGMII_CNTL_00, ni);

	rc = (rx_pol_inv ? asic_bset : asic_bclr)
		(inst, reg_addr, ASIC(NI_CFG_XGMII_CNTL_FLD_RX_POLARITY_INV_EN));	

	reg_addr = NI_REG_ADDR(CFG_XGMII_CNTL_01, ni);

	rc = (tx_pol_inv ? asic_bset : asic_bclr)
		(inst, reg_addr, ASIC(NI_CFG_XGMII_CNTL_01_FLD_AN_TX_POLARITY_INV_EN));	
	return (rc);
}

int asic_cfg_serdes_pol_inv(uint8_t inst, uint64_t hi_mask, uint64_t ni_mask) 
{
	int rc = 0, port;
        uint8_t rx_pol_inv = 0, tx_pol_inv = 0;

	for (port = 0; !rc && (port < ASIC(HI_COUNT)); port++, hi_mask >>=1) {
		if (!(hi_mask & 0x01)) continue;
#if defined(DIAG_BROCKWAY)
                brockway_get_xfi_polarity(port, &rx_pol_inv, &tx_pol_inv);
#endif
		rc = asic_cfg_hi_serdes_pol_inv(inst, port, rx_pol_inv, tx_pol_inv);
	}

	for (port = 0; !rc && (port < ASIC(NI_COUNT)); port++, ni_mask >>=1) {
		if (!(ni_mask & 0x01)) continue;
		rc = asic_cfg_ni_serdes_pol_inv(inst, port, rx_pol_inv, tx_pol_inv);
	}
	return (rc);
}


int asic_cfg_hi_serdes_ena (uint8_t inst, uint8_t hi, uint8_t rx_en, uint8_t tx_en)
{
	int rc = 0;
#ifndef DIAG_PORTOLA
	uint32_t reg_data, reg_addr = HI_REG_ADDR(CFG_XGMII_CNTL_00, hi);

	rc = asic_rd (inst, reg_addr, &reg_data);	
	if (rc) return (rc);

	reg_data &= ~((0x01 << ASIC(HI_CFG_XGMII_CNTL_FLD_RX_EN)) |
		  (0x01 << ASIC(HI_CFG_XGMII_CNTL_FLD_TX_EN)));	
	reg_data |=  ((rx_en << ASIC(HI_CFG_XGMII_CNTL_FLD_RX_EN)) |
		  (tx_en << ASIC(HI_CFG_XGMII_CNTL_FLD_TX_EN)));	

	rc = asic_wr (inst, reg_addr, reg_data);	
#endif
	return (rc);
}

int asic_cfg_ni_serdes_ena (uint8_t inst, uint8_t ni, uint8_t rx_en, uint8_t tx_en)
{
	int rc = 0;
	uint32_t reg_data, reg_addr = NI_REG_ADDR(CFG_XGMII_CNTL_00, ni);

	rc = asic_rd (inst, reg_addr, &reg_data);	
	if (rc) return (rc);

	reg_data &= ~((0x01 << ASIC(NI_CFG_XGMII_CNTL_FLD_RX_EN)) |
		  (0x01 << ASIC(NI_CFG_XGMII_CNTL_FLD_TX_EN)));	
	reg_data |=  ((rx_en << ASIC(NI_CFG_XGMII_CNTL_FLD_RX_EN)) |
		  (tx_en << ASIC(NI_CFG_XGMII_CNTL_FLD_TX_EN)));	

	rc = asic_wr (inst, reg_addr, reg_data);	
	return (rc);
}

int asic_cfg_sbus_serdes_ena (uint8_t inst, uint8_t rx_en, uint8_t tx_en)
{
	int rc = 0, dev_id = 124;

	rc = asic_wr (inst, ASIC_SBUS_ADDR(dev_id, 59), 0x40);
	if (rc) return (rc);

	rc = asic_wr (inst, ASIC_SBUS_ADDR(dev_id, 54), 0x40);
	if (rc) return (rc);

	rc = asic_wr (inst, ASIC_SBUS_ADDR(dev_id, 29), 0x01);
	if (rc) return (rc);

	rc = asic_wr (inst, ASIC_SBUS_ADDR(dev_id, 34), 0x03);
	if (rc) return (rc);

	rc = asic_wr (inst, ASIC_SBUS_ADDR(dev_id, 0), (0x1B << 1) | 0x01);
	return (rc);
}

int asic_cfg_serdes_ena (uint8_t inst, uint64_t hi_mask, uint64_t ni_mask, 
			   uint8_t rx_en, uint8_t tx_en)
{
	int rc = 0, port;
	for (port = 0; !rc && (port < ASIC(HI_COUNT)); port++, hi_mask >>=1) {
		if (!(hi_mask & 0x01)) continue;
		rc = asic_cfg_hi_serdes_ena(inst, port, rx_en, tx_en);
	}

	for (port = 0; !rc && (port < ASIC(NI_COUNT)); port++, ni_mask >>=1) {
		if (!(ni_mask & 0x01)) continue;
		rc = asic_cfg_ni_serdes_ena(inst, port, rx_en, tx_en);
	}
	return (rc);
}

int asic_chk_hi_serdes_status (uint8_t inst, uint8_t hi, int max_tries)
{
	int rc = 0;
#ifndef DIAG_PORTOLA
	uint32_t reg_data, reg_addr, port_rdy = 0, poll = max_tries;

	reg_addr = HI_REG_ADDR(STA_XGMII_STATUS, hi);

	do { 
		rc = asic_rd (inst, reg_addr, &reg_data);	
		if (rc) return (rc);

		if ((reg_data & (0x01 << ASIC(HI_STA_XGMII_STATUS_FLD_RX_RDY))) &&
		    (reg_data & (0x01 << ASIC(HI_STA_XGMII_STATUS_FLD_TX_RDY)))) {
			port_rdy = 1;
		}
		usleep(10);	
	} while (--poll && !port_rdy);

	if (!port_rdy) {
		printf("  HI%d Port Tx %s Rdy, Rx %s Rdy\n", hi,
		   (reg_data & (0x01 << ASIC(HI_STA_XGMII_STATUS_FLD_TX_RDY))) ? "Not": "",
		   (reg_data & (0x01 << ASIC(HI_STA_XGMII_STATUS_FLD_RX_RDY))) ? "Not": "");
	}
#endif
	return (rc);
}

int asic_chk_ni_serdes_status (uint8_t inst, uint8_t ni, int max_tries)
{
	int rc = 0;
	uint32_t reg_data, reg_addr, port_rdy = 0, poll = max_tries;

	reg_addr = NI_REG_ADDR(STA_XGMII_STATUS, ni);

	do { 
		rc = asic_rd (inst, reg_addr, &reg_data);	
		if (rc) return (rc);

		if ((reg_data & (0x01 << ASIC(NI_STA_XGMII_STATUS_FLD_RX_RDY))) &&
		    (reg_data & (0x01 << ASIC(NI_STA_XGMII_STATUS_FLD_TX_RDY)))) {
			port_rdy = 1;
		}
		usleep(10);	
	} while (--poll && !port_rdy);

	if (!port_rdy) {
		printf("  NI%d Port Tx %s Rdy, Rx %s Rdy\n", ni,
		   (reg_data & (0x01 << ASIC(NI_STA_XGMII_STATUS_FLD_TX_RDY))) ? "Not": "",
		   (reg_data & (0x01 << ASIC(NI_STA_XGMII_STATUS_FLD_RX_RDY))) ? "Not": "");
		rc = -1;
	}
	return (rc);
}

int asic_chk_serdes_status (uint8_t inst, uint64_t hi_mask, uint64_t ni_mask, int max_tries,
				uint64_t *hi_fail, uint64_t *ni_fail)
{
	int rc = 0, port;

	*hi_fail = 0x00ull;
	*ni_fail = 0x00ull;

	for (port = 0; !rc && (port < ASIC(HI_COUNT)); port++, hi_mask >>=1) {
		if (!(hi_mask & 0x01)) continue;
		rc = asic_chk_hi_serdes_status (inst, port, max_tries);
		if (rc) (*hi_fail |= (0x01 << port));
	}

	for (port = 0; !rc && (port < ASIC(NI_COUNT)); port++, ni_mask >>=1) {
		if (!(ni_mask & 0x01)) continue;
		rc = asic_chk_ni_serdes_status (inst, port, max_tries);
		if (rc) (*ni_fail |= (0x01 << port));
	}
	return ((*hi_fail || *ni_fail)? -1 : 0);
}


int asic_chk_hi_pcs_status (uint8_t inst, uint8_t hi, int max_tries)
{
#if defined(DIAG_WOODSIDE)
	int rc = 0; 
	uint32_t poll=max_tries, reg_data, reg_addr, port_rdy = 0;

	reg_addr = HI_REG_ADDR(STA_XGMII_STATUS, hi);

	do { 
		rc = asic_rd (inst, reg_addr, &reg_data);	
		if (rc) return (rc);

		if ((reg_data & (0x01 << ASIC(HI_STA_XGMII_STATUS_FLD_PCS_STATUS))) &&
		    (reg_data & (0x01 << ASIC(HI_STA_XGMII_STATUS_FLD_RX_RDY))) &&
		    (reg_data & (0x01 << ASIC(HI_STA_XGMII_STATUS_FLD_TX_RDY)))) {
			port_rdy = 1;
		} else {
			usleep(10);	
		}
	} while (--poll && !port_rdy);

	if (!port_rdy) {
		printf("  HI%d Port Tx %s Rdy, Rx %s Rdy Pcs %s up\n", hi,
		   (reg_data & (0x01 << ASIC(HI_STA_XGMII_STATUS_FLD_TX_RDY))) ? "": "Not",
		   (reg_data & (0x01 << ASIC(HI_STA_XGMII_STATUS_FLD_RX_RDY))) ? "":"Not",
		   (reg_data & (0x01 << ASIC(HI_STA_XGMII_STATUS_FLD_PCS_STATUS))) ? "":"Not");
	}
	return (!port_rdy);
#else /* FOR PORTOLA */
	return (0);
#endif
}

int asic_chk_ni_pcs_status (uint8_t inst, uint8_t ni, int max_tries)
{
	int rc = 0;
	uint32_t poll=max_tries, reg_data, reg_addr, port_rdy = 0;

	reg_addr = NI_REG_ADDR(STA_XGMII_STATUS, ni);

	do { 
		rc = asic_rd (inst, reg_addr, &reg_data);	
		if (rc) return (rc);

		if ((reg_data & (0x01 << ASIC(NI_STA_XGMII_STATUS_FLD_PCS_STATUS))) &&
		    (reg_data & (0x01 << ASIC(NI_STA_XGMII_STATUS_FLD_RX_RDY))) &&
		    (reg_data & (0x01 << ASIC(NI_STA_XGMII_STATUS_FLD_TX_RDY)))) {
			port_rdy = 1;
		} else { 
			usleep(10);	
		}
	} while (--poll && !port_rdy);

	if (!port_rdy) {
		printf("data x%x  NI%d Port Tx %s Rdy, Rx %s Rdy, Pcs %s Up\n", 
                       reg_data, ni,
		   (reg_data & (0x01 << ASIC(NI_STA_XGMII_STATUS_FLD_TX_RDY))) ? "":"Not",
		   (reg_data & (0x01 << ASIC(NI_STA_XGMII_STATUS_FLD_RX_RDY))) ? "":"Not",
		   (reg_data & (0x01 << ASIC(NI_STA_XGMII_STATUS_FLD_PCS_STATUS))) ? "":"Not");
	}
	return(!port_rdy);
}

int asic_chk_pcs_status (uint8_t inst, uint64_t hi_mask, uint64_t ni_mask, int max_tries,
			   uint64_t *hi_fail, uint64_t *ni_fail)
{
	int rc = 0, port;

	*hi_fail = 0x00ull;
	*ni_fail = 0x00ull;

	for (port = 0; !rc && (port < ASIC(HI_COUNT)); port++, hi_mask >>=1) {
		if (!(hi_mask & 0x01)) continue;
		rc = asic_chk_hi_pcs_status (inst, port, max_tries);
		if (rc) (*hi_fail |= (0x01 << port));
	}

	for (port = 0; !rc && (port < ASIC(NI_COUNT)); port++, ni_mask >>=1) {
		if (!(ni_mask & 0x01)) continue;
		rc = asic_chk_ni_pcs_status (inst, port, max_tries);
		if (rc) (*ni_fail |= (0x01 << port));
	}
	return ((*hi_fail || *ni_fail)? -1 : 0);
}




int asic_cfg_hi_an_timers (uint8_t inst, uint8_t hi, int timer_val, uint8_t blink)
{
	int rc = 0;
	uint32_t dev_id;

	dev_id = (hi+1) * 3;
	rc = asic_wr (inst, ASIC_SBUS_ADDR(dev_id, 17), blink? 0x05 : 0x01);
	if (rc) return (rc);

	rc = asic_wr (inst, ASIC_SBUS_ADDR(dev_id, 18), (timer_val & 0xFF));
	return (rc);
}

int asic_cfg_ni_an_timers (uint8_t inst, uint8_t ni, int timer_val, uint8_t blink)
{
	int rc = 0;
	uint32_t dev_id;

	dev_id = 96+ ((ni+1) * 3);
	rc = asic_wr (inst, ASIC_SBUS_ADDR(dev_id, 17), blink? 0x05 : 0x01);
	if (rc) return (rc);

	rc = asic_wr (inst, ASIC_SBUS_ADDR(dev_id, 18), (timer_val & 0xFF));
	return (rc);
}

int asic_cfg_an_timers (uint8_t inst, uint64_t hi_mask, uint64_t ni_mask,
			  int timer_val, uint8_t blink)
{
	int rc = 0, port; 

	for (port = 0; !rc && (port < ASIC(HI_COUNT)); port++, hi_mask >>=1) {
		if (!(hi_mask & 0x01)) continue;
		rc = asic_cfg_hi_an_timers (inst, port, timer_val, blink);
	}

	for (port = 0; !rc && (port < ASIC(NI_COUNT)); port++, ni_mask >>=1) {
		if (!(ni_mask & 0x01)) continue;
		rc = asic_cfg_ni_an_timers (inst, port, timer_val, blink);
	}

	return (rc);
}

int asic_cfg_hi_an_enable (uint8_t inst, uint8_t hi, uint8_t ena)
{
	int rc = 0;
#ifndef DIAG_PORTOLA
	uint32_t reg_addr, dev_id;

	reg_addr = HI_REG_ADDR(CFG_XGMII_CNTL_01, hi);

	rc = (ena?asic_bset:asic_bclr)(inst, reg_addr, ASIC(HI_CFG_XGMII_CNTL_01_FLD_AN_POWER_ON));

	dev_id = (hi+1) * 3;
	if (!rc) asic_wr (inst, ASIC_SBUS_ADDR(dev_id, 1), ena? 0x01 : 0x02);
#endif
	return (rc);
}

int asic_cfg_ni_an_enable (uint8_t inst, uint8_t ni, uint8_t ena)
{
	int rc = 0;
	uint32_t reg_addr, dev_id;

	reg_addr = NI_REG_ADDR(CFG_XGMII_CNTL_01, ni);

	rc = (ena?asic_bset:asic_bclr)(inst, reg_addr, ASIC(NI_CFG_XGMII_CNTL_01_FLD_AN_POWER_ON));

	dev_id = 96+((ni+1) * 3);
	return(rc? rc : asic_wr (inst, ASIC_SBUS_ADDR(dev_id, 1), ena? 0x01 : 0x02));
}

int asic_cfg_an_enable (uint8_t inst, uint64_t hi_mask, uint64_t ni_mask,
			  uint8_t ena)
{
	int rc = 0, port;

	for (port = 0; !rc && (port < ASIC(HI_COUNT)); port++, hi_mask >>=1) {
		if (!(hi_mask & 0x01)) continue;
		rc = asic_cfg_hi_an_enable (inst, port, ena);
	}

	for (port = 0; !rc && (port < ASIC(NI_COUNT)); port++, ni_mask >>=1) {
		if (!(ni_mask & 0x01)) continue;
		rc = asic_cfg_ni_an_enable (inst, port, ena);
	}

	return (rc);
}

int asic_cfg_hi_an_bpage (uint8_t inst, uint8_t hi, uint32_t hi_word, uint32_t lo_word, 
			uint32_t cfg_flds_sel_fec)
{
	int rc = 0;
	uint32_t dev_id;

	dev_id = (hi+1) * 3;

	rc = asic_wr (inst, ASIC_SBUS_ADDR(dev_id, 7), (cfg_flds_sel_fec==1)? (
				(lo_word&0xE0) | (hi&0x1F)): (lo_word&0xFF));
	if (rc) return (rc);

	rc = asic_wr (inst, ASIC_SBUS_ADDR(dev_id, 6), (lo_word >> 8) & 0xFF); 
	if (rc) return (rc);

	rc = asic_wr (inst, ASIC_SBUS_ADDR(dev_id, 5), (lo_word >> 16) & 0xFF); 
	if (rc) return (rc);

	rc = asic_wr (inst, ASIC_SBUS_ADDR(dev_id, 4), (lo_word >> 24) & 0xFF); 
	if (rc) return (rc);

	rc = asic_wr (inst, ASIC_SBUS_ADDR(dev_id, 3), (hi_word & 0xFF)); 
	if (rc) return (rc);

	rc = asic_wr (inst, ASIC_SBUS_ADDR(dev_id, 2), (cfg_flds_sel_fec == 1) ? 
			(((hi_word >> 8) & 0x3F) | ((hi&0x3) << 6)) :
			(hi_word >> 8) & 0xFF); 
	return (rc);
}

int asic_cfg_ni_an_bpage (uint8_t inst, uint8_t ni, uint32_t hi_word, uint32_t lo_word, 
			uint32_t cfg_flds_sel_fec)
{
	int rc = 0;
	uint32_t dev_id = 96 + ((ni+1) * 3);

	rc = asic_wr (inst, ASIC_SBUS_ADDR(dev_id, 7), (cfg_flds_sel_fec==1)? (
				(lo_word&0xE0) | (ni&0x1F)): (lo_word&0xFF));
	if (rc) return (rc);

	rc = asic_wr (inst, ASIC_SBUS_ADDR(dev_id, 6), (lo_word >> 8) & 0xFF); 
	if (rc) return (rc);

	rc = asic_wr (inst, ASIC_SBUS_ADDR(dev_id, 5), (lo_word >> 16) & 0xFF); 
	if (rc) return (rc);

	rc = asic_wr (inst, ASIC_SBUS_ADDR(dev_id, 4), (lo_word >> 24) & 0xFF); 
	if (rc) return (rc);

	rc = asic_wr (inst, ASIC_SBUS_ADDR(dev_id, 3), (hi_word & 0xFF)); 
	if (rc) return (rc);

	rc = asic_wr (inst, ASIC_SBUS_ADDR(dev_id, 2), (cfg_flds_sel_fec == 1) ? 
			(((hi_word >> 8) & 0x3F) | ((ni&0x3) << 6)) :
			(hi_word >> 8) & 0xFF); 
	return (rc);
}

int asic_cfg_an_bpage (uint8_t inst, uint64_t hi_mask, uint64_t ni_mask,
			uint32_t hi_word, uint32_t lo_word, uint32_t cfg_flds)
{
	int rc = 0, port;

	for (port = 0; !rc && (port < ASIC(HI_COUNT)); port++, hi_mask >>=1) {
		if (!(hi_mask & 0x01)) continue;
		rc = asic_cfg_hi_an_bpage (inst, port, hi_word, lo_word, cfg_flds);
	}

	for (port = 0; !rc && (port < ASIC(NI_COUNT)); port++, ni_mask >>=1) {
		if (!(ni_mask & 0x01)) continue;
		rc = asic_cfg_ni_an_bpage (inst, port, hi_word, lo_word, cfg_flds);
	}

	return (rc);
}


int asic_cfg_hi_an_nxtpage (uint8_t inst, uint8_t hi, uint32_t hi_word, uint32_t lo_word)
{
	int rc = 0;
	uint32_t dev_id = (hi+1) * 3;

	rc = asic_wr (inst, ASIC_SBUS_ADDR(dev_id, 13), lo_word & 0xFF); 
	if (rc) return (rc);

	rc = asic_wr (inst, ASIC_SBUS_ADDR(dev_id, 12), (lo_word >> 8) & 0xFF); 
	if (rc) return (rc);

	rc = asic_wr (inst, ASIC_SBUS_ADDR(dev_id, 11), (lo_word >> 16) & 0xFF); 
	if (rc) return (rc);

	rc = asic_wr (inst, ASIC_SBUS_ADDR(dev_id, 10), (lo_word >> 24) & 0xFF); 
	if (rc) return (rc);

	rc = asic_wr (inst, ASIC_SBUS_ADDR(dev_id, 9), (hi_word & 0xFF)); 
	if (rc) return (rc);

	rc = asic_wr (inst, ASIC_SBUS_ADDR(dev_id, 8), (hi_word >> 8) & 0xFF); 
	return (rc);
}

int asic_cfg_ni_an_nxtpage (uint8_t inst, uint8_t ni, uint32_t hi_word, uint32_t lo_word)
{
	int rc = 0;
	uint32_t dev_id = 96 + ((ni+1) * 3);

	rc = asic_wr (inst, ASIC_SBUS_ADDR(dev_id, 13), lo_word & 0xFF); 
	if (rc) return (rc);

	rc = asic_wr (inst, ASIC_SBUS_ADDR(dev_id, 12), (lo_word >> 8) & 0xFF); 
	if (rc) return (rc);

	rc = asic_wr (inst, ASIC_SBUS_ADDR(dev_id, 11), (lo_word >> 16) & 0xFF); 
	if (rc) return (rc);

	rc = asic_wr (inst, ASIC_SBUS_ADDR(dev_id, 10), (lo_word >> 24) & 0xFF); 
	if (rc) return (rc);

	rc = asic_wr (inst, ASIC_SBUS_ADDR(dev_id, 9), (hi_word & 0xFF)); 
	if (rc) return (rc);

	rc = asic_wr (inst, ASIC_SBUS_ADDR(dev_id, 8), (hi_word >> 8) & 0xFF); 
	return (rc);
}

int asic_cfg_an_nxtpage (uint8_t inst, uint64_t hi_mask, uint64_t ni_mask,
			uint32_t hi_word, uint32_t lo_word)
{
	int rc = 0, port;

	for (port = 0; !rc && (port < ASIC(HI_COUNT)); port++, hi_mask >>=1) {
		if (!(hi_mask & 0x01)) continue;
		rc = asic_cfg_hi_an_nxtpage (inst, port, hi_word, lo_word);
	}

	for (port = 0; !rc && (port < ASIC(NI_COUNT)); port++, ni_mask >>=1) {
		if (!(ni_mask & 0x01)) continue;
		rc = asic_cfg_ni_an_nxtpage (inst, port, hi_word, lo_word);
	}
	return (rc);
}

int asic_cfg_hi_an_nxtpage_tgl (uint8_t inst, uint8_t hi)
{
	int rc = 0;
	uint32_t dev_id = (hi+1) * 3;

	rc = asic_wr (inst, ASIC_SBUS_ADDR(dev_id, 1), 1);
	if (rc) return (rc);

	return (asic_wr (inst, ASIC_SBUS_ADDR(dev_id, 1), 9));
}

int asic_cfg_ni_an_nxtpage_tgl (uint8_t inst, uint8_t ni)
{
	int rc = 0;
	uint32_t dev_id = 96 + ((ni+1) * 3);

	rc = asic_wr (inst, ASIC_SBUS_ADDR(dev_id, 1), 1);
	if (rc) return (rc);

	return (asic_wr (inst, ASIC_SBUS_ADDR(dev_id, 1), 9));
}

int asic_cfg_an_nxtpage_tgl (uint8_t inst, uint64_t hi_mask, uint64_t ni_mask)
{
	int rc = 0, port;

	for (port = 0; !rc && (port < ASIC(HI_COUNT)); port++, hi_mask >>=1) {
		if (!(hi_mask & 0x01)) continue;
		rc = asic_cfg_hi_an_nxtpage_tgl (inst, port);
	}

	for (port = 0; !rc && (port < ASIC(NI_COUNT)); port++, ni_mask >>=1) {
		if (!(ni_mask & 0x01)) continue;
		rc = asic_cfg_ni_an_nxtpage_tgl (inst, port);
	}
	return (rc);
}


int asic_cfg_hi_an_rate (uint8_t inst, uint8_t hi, uint8_t rate)
{
	int rc = 0;
#ifndef DIAG_PORTOLA
	uint32_t reg_data, reg_addr = HI_REG_ADDR(CFG_XGMII_CNTL_01, hi);

	rc = asic_rd(inst, reg_addr, &reg_data); 
	if (rc) return (rc);

	reg_data &= ~(0x3 << 4);
	reg_data |= ((rate&0x3) << 4);

	rc = asic_wr(inst, reg_addr, reg_data); 
#endif
	return (rc);
}

int asic_cfg_ni_an_rate (uint8_t inst, uint8_t ni, uint8_t rate)
{
	int rc = 0;
	uint32_t reg_data, reg_addr;

	reg_addr = NI_REG_ADDR(CFG_XGMII_CNTL_01, ni);

	rc = asic_rd(inst, reg_addr, &reg_data); 
	if (rc) return (rc);

	reg_data &= ~(0x3 << 4);
	reg_data |= ((rate&0x3) << 4);

	rc = asic_wr(inst, reg_addr, reg_data); 
	return (rc);
}

int asic_cfg_an_rate (uint8_t inst, uint64_t hi_mask, uint64_t ni_mask,
			  uint8_t rate)
{
	int rc = 0, port;

	for (port = 0; !rc && (port < ASIC(HI_COUNT)); port++, hi_mask >>=1) {
		if (!(hi_mask & 0x01)) continue;
		rc = asic_cfg_hi_an_rate (inst, port, rate);
	}

	for (port = 0; !rc && (port < ASIC(NI_COUNT)); port++, ni_mask >>=1) {
		if (!(ni_mask & 0x01)) continue;
		rc = asic_cfg_ni_an_rate (inst, port, rate);
	}
	return (rc);
}


int asic_cfg_hi_an_link_status (uint8_t inst, uint8_t hi, uint8_t an_linkstat)
{
	int rc = 0;
#ifndef DIAG_PORTOLA
	uint32_t reg_data, reg_addr;

	reg_addr = HI_REG_ADDR(CFG_XGMII_CNTL_01, hi);

	rc = asic_rd(inst, reg_addr, &reg_data); 
	if (rc) return (rc);

	reg_data &= ~((0x01 << ASIC(HI_CFG_XGMII_CNTL_01_FLD_AN_LINK_STATUS_KX)) |
		      (0x01 << ASIC(HI_CFG_XGMII_CNTL_01_FLD_AN_LINK_STATUS_KX4)) |
		      (0x01 << ASIC(HI_CFG_XGMII_CNTL_01_FLD_AN_LINK_STATUS_KR)));

	switch(an_linkstat) {	
		case	0:
			reg_data |= (0x01 << ASIC(HI_CFG_XGMII_CNTL_01_FLD_AN_LINK_STATUS_KX));
			break;
		case	1:
			reg_data |= (0x01 << ASIC(HI_CFG_XGMII_CNTL_01_FLD_AN_LINK_STATUS_KX4));
			break;
		default:
			reg_data |= (0x01 << ASIC(HI_CFG_XGMII_CNTL_01_FLD_AN_LINK_STATUS_KR));
			break;
	}
	rc = asic_wr(inst, reg_addr, reg_data); 
#endif
	return (rc);
}
int asic_cfg_ni_an_link_status (uint8_t inst, uint8_t ni, uint8_t an_linkstat)
{
	int rc = 0;
	uint32_t reg_data, reg_addr = NI_REG_ADDR(CFG_XGMII_CNTL_01, ni);

	rc = asic_rd(inst, reg_addr, &reg_data); 
	if (rc) return (rc);

	reg_data &= ~((0x01 << ASIC(NI_CFG_XGMII_CNTL_01_FLD_AN_LINK_STATUS_KX)) |
		      (0x01 << ASIC(NI_CFG_XGMII_CNTL_01_FLD_AN_LINK_STATUS_KX4)) |
		      (0x01 << ASIC(NI_CFG_XGMII_CNTL_01_FLD_AN_LINK_STATUS_KR)));

	switch(an_linkstat) {	
		case	0:
			reg_data |= (0x01 << ASIC(NI_CFG_XGMII_CNTL_01_FLD_AN_LINK_STATUS_KX));
			break;
		case	1:
			reg_data |= (0x01 << ASIC(NI_CFG_XGMII_CNTL_01_FLD_AN_LINK_STATUS_KX4));
			break;
		default:
			reg_data |= (0x01 << ASIC(NI_CFG_XGMII_CNTL_01_FLD_AN_LINK_STATUS_KR));
			break;
	}
	return (asic_wr(inst, reg_addr, reg_data)); 
}

int asic_cfg_an_link_status (uint8_t inst, uint64_t hi_mask, uint64_t ni_mask,
			  uint8_t an_linkstat)
{
	int rc = 0, port;

	for (port = 0; !rc && (port < ASIC(HI_COUNT)); port++, hi_mask >>=1) {
		if (!(hi_mask & 0x01)) continue;
		rc = asic_cfg_hi_an_link_status (inst, port, an_linkstat);
	}

	for (port = 0; !rc && (port < ASIC(NI_COUNT)); port++, ni_mask >>=1) {
		if (!(ni_mask & 0x01)) continue;
		rc = asic_cfg_ni_an_link_status (inst, port, an_linkstat);
	}
	return (rc);
}

static int asic_cfg_port_pcs_test_seeds (uint8_t inst, uint8_t port, 
                                         uint8_t hi, uint8_t ena_a, 
                                         uint8_t ena_b, uint32_t seeda_hi, 
                                         uint32_t seeda_lo, uint32_t seedb_hi, 
			                 uint32_t seedb_lo)
{
	int rc = 0;
	uint32_t dev_id, cnt, reg_data;

#if defined(DIAG_WOODSIDE)
	dev_id = 2 + (port*3) + (hi ? 0 : 96); 
#elif defined(DIAG_PORTOLA)
        if (hi) {
	    return 0 ; 
        } else {
	    dev_id = 2 + (port*3) + 48; 
        }
#endif

	if (ena_a) {
		for (cnt = 0; !rc && cnt < 4; cnt++) {
			rc = asic_wr (inst, ASIC_SBUS_ADDR(dev_id, 8+cnt), 
					(seeda_lo >> (cnt*8)) & 0xFF);
		}
		for (cnt = 0; !rc && cnt < 3; cnt++) {
			rc = asic_wr (inst, ASIC_SBUS_ADDR(dev_id, 12+cnt), 
				(seeda_hi >> (cnt*8)) & 0xFF);
		}
		if (rc) return (rc);

		reg_data = ((seeda_hi >> 24) & 0x3) | 
			   (ena_b ? 0x00 : (((seedb_hi >> 24) & 0x3) << 2));
		rc = asic_wr (inst, ASIC_SBUS_ADDR(dev_id, 15), reg_data); 
	}

	if (ena_b) {
		for (cnt = 0; !rc && cnt < 4; cnt++) {
			rc = asic_wr (inst, ASIC_SBUS_ADDR(dev_id, 16+cnt), 
				(seedb_lo >> (cnt*8)) & 0xFF);
		}
		for (cnt = 0; !rc && cnt < 3; cnt++) {
			rc = asic_wr (inst, ASIC_SBUS_ADDR(dev_id, 20+cnt), 
				(seedb_hi >> (cnt*8)) & 0xFF);
		}
		if (rc) return (rc);
	}
	return (rc);	
}

int asic_cfg_pcs_test_seeds (uint8_t inst, uint64_t hi_mask, uint64_t ni_mask,
				uint8_t ena_a, uint8_t ena_b, uint32_t seeda_hi,
				uint32_t seeda_lo, uint32_t seedb_hi, uint32_t seedb_lo)
{
	int rc = 0, port;

	for (port = 0; !rc && (port < ASIC(HI_COUNT)); port++, hi_mask >>=1) {
		if (!(hi_mask & 0x01)) continue;
		rc = asic_cfg_port_pcs_test_seeds (inst, port, 1, ena_a, ena_b, 
			     seeda_hi, seeda_lo, seedb_hi, seedb_lo);
	}

	for (port = 0; !rc && (port < ASIC(NI_COUNT)); port++, ni_mask >>=1) {
		if (!(ni_mask & 0x01)) continue;
		rc = asic_cfg_port_pcs_test_seeds (inst, port, 0, ena_a, ena_b, 
			     seeda_hi, seeda_lo, seedb_hi, seedb_lo);
	}
	return (rc);
}


static int asic_cfg_hi_pcs_tx_testmode (uint8_t inst, uint8_t hi, uint32_t tst_cntl, uint32_t pattern_sel)
{
	int rc = 0;
#ifndef DIAG_PORTOLA
	uint32_t reg_data;

	reg_data = ((tst_cntl&1) << 4) | ((pattern_sel&0x1)<<5);
	rc = asic_wr (inst, ASIC_SBUS_ADDR((2 + (hi*3)), 1), reg_data);
#endif
	return (rc);
}

static int asic_cfg_ni_pcs_tx_testmode (uint8_t inst, uint8_t ni, uint32_t tst_cntl, uint32_t pattern_sel)
{
	int rc = 0;
	uint32_t reg_data, dev_id;
#if defined(DIAG_WOODSIDE)
	dev_id = 96 + 2 + (ni * 3);
#elif defined(DIAG_PORTOLA)
	dev_id = 48 + 2 + (ni * 3);
#endif
	reg_data = ((tst_cntl&1) << 4) | ((pattern_sel&0x1)<<5);
	rc = asic_wr (inst, ASIC_SBUS_ADDR(dev_id, 1), reg_data);
	return (rc);
}

int asic_cfg_pcs_tx_testmode (uint8_t inst, uint64_t hi_mask, uint64_t ni_mask,
				  uint32_t tst_cntl, uint32_t pattern_sel)
{
	int rc = 0, port;

	for (port = 0; !rc && (port < ASIC(HI_COUNT)); port++, hi_mask >>=1) {
		if (!(hi_mask & 0x01)) continue;
		rc = asic_cfg_hi_pcs_tx_testmode(inst, port, tst_cntl, pattern_sel);
	}

	for (port = 0; !rc && (port < ASIC(NI_COUNT)); port++, ni_mask >>=1) {
		if (!(ni_mask & 0x01)) continue;
		rc = asic_cfg_ni_pcs_tx_testmode(inst, port, tst_cntl, pattern_sel);
	}
	return (rc);
}

static int asic_cfg_hi_pcs_rx_testmode (uint8_t inst, uint8_t hi, uint32_t tst_cntl, uint32_t pattern_sel)
{
#if defined(DIAG_WOODSIDE)
	uint32_t reg_data = (0x4 | (tst_cntl&1)  | ((pattern_sel&0x1)<<1));
	uint32_t dev_id = 2 + (hi*3);
	return (asic_wr (inst, ASIC_SBUS_ADDR(dev_id, 2), reg_data));
#elif defined(DIAG_PORTOLA)
        return 0;
#endif
}

static int asic_cfg_ni_pcs_rx_testmode (uint8_t inst, uint8_t ni, uint32_t tst_cntl, uint32_t pattern_sel)
{
	uint32_t dev_id, reg_data = 0x4 | (tst_cntl&1)  | ((pattern_sel&0x1)<<1);
	dev_id =  ASIC_NI_PCS_SBUS_DEVID(ni);
	return (asic_wr (inst, ASIC_SBUS_ADDR(dev_id, 2), reg_data));
}

int asic_cfg_pcs_rx_testmode (uint8_t inst, uint64_t hi_mask, uint64_t ni_mask,
				  uint32_t tst_cntl, uint32_t pattern_sel)
{
	int rc = 0, port;

	for (port = 0; !rc && (port < ASIC(HI_COUNT)); port++, hi_mask >>=1) {
		if (!(hi_mask & 0x01)) continue;
		rc = asic_cfg_hi_pcs_rx_testmode(inst, port, tst_cntl, pattern_sel);
	}

	for (port = 0; !rc && (port < ASIC(NI_COUNT)); port++, ni_mask >>=1) {
		if (!(ni_mask & 0x01)) continue;
		rc = asic_cfg_ni_pcs_rx_testmode(inst, port, tst_cntl, pattern_sel);
	}
	return (rc);
}


int asic_cfg_hi_serdes_tx_prbs (uint8_t inst, uint8_t hi, int tx_prbs_en, int tx_prbs_sel)
{
	int rc = 0;
#ifndef DIAG_PORTOLA
	uint32_t reg_data, reg_addr = HI_REG_ADDR(CFG_XGMII_CNTL_01, hi);

	rc = asic_rd (inst, reg_addr, &reg_data);
	if (rc) return (rc);

	reg_data &= ~(0x3 << 1);		// Clear pattern bits
	reg_data |= ((tx_prbs_sel&3) << 1);

	rc = asic_wr (inst, reg_addr, reg_data);
	if (rc) return (rc);

	reg_data &= ~(1 << ASIC(HI_CFG_XGMII_CNTL_01_FLD_AN_TX_PATTERN_GEN_EN));
	reg_data |= (tx_prbs_en << ASIC(HI_CFG_XGMII_CNTL_01_FLD_AN_TX_PATTERN_GEN_EN));

	rc = asic_wr (inst, reg_addr, reg_data);
#endif
	return (rc);
}

int asic_cfg_ni_serdes_tx_prbs (uint8_t inst, uint8_t ni, int tx_prbs_en, int tx_prbs_sel)
{
	int rc = 0;
	uint32_t reg_data, reg_addr = NI_REG_ADDR(CFG_XGMII_CNTL_01, ni);

	rc = asic_rd (inst, reg_addr, &reg_data);
	if (rc) return (rc);

	reg_data &= ~(0x3 << 1);		// Clear pattern bits
	reg_data |= ((tx_prbs_sel&3) << 1);

	rc = asic_wr (inst, reg_addr, reg_data);
	if (rc) return (rc);

	reg_data &= ~(1 << ASIC(NI_CFG_XGMII_CNTL_01_FLD_AN_TX_PATTERN_GEN_EN));
	reg_data |= (tx_prbs_en << ASIC(NI_CFG_XGMII_CNTL_01_FLD_AN_TX_PATTERN_GEN_EN));

	rc = asic_wr (inst, reg_addr, reg_data);
	return (rc);
}

int asic_cfg_serdes_tx_prbs (uint8_t inst, uint64_t hi_mask, uint64_t ni_mask,
			  int tx_prbs_en, int tx_prbs_sel)
{
        int rc = 0, port;

        for (port = 0; !rc && (port < ASIC(HI_COUNT)); port++, hi_mask >>=1) {
                if (!(hi_mask & 0x01)) continue;
                rc = asic_cfg_hi_serdes_tx_prbs(inst, port, tx_prbs_en, tx_prbs_sel);
        }

        for (port = 0; !rc && (port < ASIC(NI_COUNT)); port++, ni_mask >>=1) {
                if (!(ni_mask & 0x01)) continue;
                rc = asic_cfg_ni_serdes_tx_prbs(inst, port, tx_prbs_en, tx_prbs_sel);
        }
        return (rc);
}

int asic_cfg_hi_serdes_rx_prbs (uint8_t inst, uint8_t hi, int rx_prbs_en, int rx_prbs_sel)
{
	int rc = 0;
#ifndef DIAG_PORTOLA
	uint32_t reg_data, reg_addr = HI_REG_ADDR(CFG_XGMII_CNTL_00, hi);

	rc = asic_rd (inst, reg_addr, &reg_data);
	if (rc) return (rc);

	reg_data &= ~(0x3 << 23);		// Clear pattern bits
	reg_data |= ((rx_prbs_sel&3) << 23);

	rc = asic_wr (inst, reg_addr, reg_data);
	if (rc) return (rc);

	reg_data &= ~(1 << ASIC(HI_CFG_XGMII_CNTL_FLD_RX_PATTERN_CMP_EN));
	reg_data |= (rx_prbs_en << ASIC(HI_CFG_XGMII_CNTL_FLD_RX_PATTERN_CMP_EN));

	rc = asic_wr (inst, reg_addr, reg_data);
#endif
	return (rc);
}

int asic_cfg_ni_serdes_rx_prbs (uint8_t inst, uint8_t ni, int rx_prbs_en, int rx_prbs_sel)
{
	int rc = 0;
	uint32_t reg_data, reg_addr = NI_REG_ADDR(CFG_XGMII_CNTL_00, ni);

	rc = asic_rd (inst, reg_addr, &reg_data);
	if (rc) return (rc);

	reg_data &= ~(0x3 << 23);		// Clear pattern bits
	reg_data |= ((rx_prbs_sel&3) << 23);

	rc = asic_wr (inst, reg_addr, reg_data);
	if (rc) return (rc);

	reg_data &= ~(1 << ASIC(NI_CFG_XGMII_CNTL_FLD_RX_PATTERN_CMP_EN));
	reg_data |= (rx_prbs_en << ASIC(NI_CFG_XGMII_CNTL_FLD_RX_PATTERN_CMP_EN));

	rc = asic_wr (inst, reg_addr, reg_data);
	return (rc);
}

int asic_cfg_serdes_rx_prbs (uint8_t inst, uint64_t hi_mask, uint64_t ni_mask,
			  int rx_prbs_en, int rx_prbs_sel)
{
        int rc = 0, port;

        for (port = 0; !rc && (port < ASIC(HI_COUNT)); port++, hi_mask >>=1) {
                if (!(hi_mask & 0x01)) continue;
                rc = asic_cfg_hi_serdes_rx_prbs(inst, port, rx_prbs_en, rx_prbs_sel);
        }

        for (port = 0; !rc && (port < ASIC(NI_COUNT)); port++, ni_mask >>=1) {
                if (!(ni_mask & 0x01)) continue;
                rc = asic_cfg_ni_serdes_rx_prbs(inst, port, rx_prbs_en, rx_prbs_sel);
        }
        return (rc);
}


int asic_chk_hi_an_state (uint8_t inst, uint8_t hi)
{
	int rc = 0;
	uint32_t reg_data, dev_id = (hi+1) * 3;

	rc = asic_wr (inst, ASIC_SBUS_ADDR(dev_id, 15), 0x00);
	if (rc) return (rc);

	rc = asic_wr (inst, ASIC_SBUS_ADDR(dev_id, 15), 0x01);
	if (rc) return (rc);

	rc = asic_rd (inst, ASIC_SBUS_ADDR(dev_id, 15), &reg_data);
	if (rc) return (rc);

	printf("  HI%d An Arb State 0x%X\n", hi, reg_data);

	rc = asic_rd (inst, ASIC_SBUS_ADDR(dev_id, 14), &reg_data);
	if (rc) return (rc);
	printf("  HI%d An State Variable 0x%X\n", hi, reg_data);

	rc = asic_rd (inst, ASIC_SBUS_ADDR(dev_id, 16), &reg_data);
	if (rc) return (rc);
	printf("  HI%d An Rx & Tx State 0x%X\n", hi, reg_data);
	return (rc);
}

int asic_chk_ni_an_state (uint8_t inst, uint8_t ni)
{
	int	 rc = 0;
	uint32_t reg_data, dev_id = 96 + ((ni+1) * 3);

	rc = asic_wr (inst, ASIC_SBUS_ADDR(dev_id, 15), 0x00);
	if (rc) return (rc);

	rc = asic_wr (inst, ASIC_SBUS_ADDR(dev_id, 15), 0x01);
	if (rc) return (rc);

	rc = asic_rd (inst, ASIC_SBUS_ADDR(dev_id, 15), &reg_data);
	if (rc) return (rc);

	printf("  NI%d An Arb State 0x%X\n", ni, reg_data);

	rc = asic_rd (inst, ASIC_SBUS_ADDR(dev_id, 14), &reg_data);
	if (rc) return (rc);
	printf("  NI%d An State Variable 0x%X\n", ni, reg_data);

	rc = asic_rd (inst, ASIC_SBUS_ADDR(dev_id, 16), &reg_data);
	if (rc) return (rc);
	printf("  NI%d An Rx & Tx State 0x%X\n", ni, reg_data);

	return (rc);
}

int asic_chk_an_state (uint8_t inst, uint64_t hi_mask, uint64_t ni_mask)
{
        int rc = 0, port;

        for (port = 0; !rc && (port < ASIC(HI_COUNT)); port++, hi_mask >>=1) {
                if (!(hi_mask & 0x01)) continue;
                rc = asic_chk_hi_an_state(inst, port);
        }

        for (port = 0; !rc && (port < ASIC(NI_COUNT)); port++, ni_mask >>=1) {
                if (!(ni_mask & 0x01)) continue;
                rc = asic_chk_ni_an_state(inst, port);
        }
        return (rc);
}

int asic_chk_hi_an_rx_page (uint8_t inst, uint8_t hi, int chk_flds_sel_fec)
{
	int rc = 0;
	uint32_t reg_data, dev_id, reg_id, cmp_val;

	dev_id = (hi+1) * 3;

	for (reg_id = 7; reg_id > 1; reg_id--){
		rc = asic_rd (inst, ASIC_SBUS_ADDR(dev_id, reg_id), &reg_data);
		if (rc) return (rc);

		if (chk_flds_sel_fec == 1) {
			cmp_val = (hi&1) ? (hi-1) : (hi+1);
			if (reg_id == 7) {
				if ((reg_data & 0x1F) != (cmp_val & 0x1F)) {
					printf("  Error: HI%d Reg%d Rx Page Exp : 0x%X, Rd 0x%X\n", 
							hi, reg_id, cmp_val&0x1F, reg_data&0x1F);
					return (-1);
				}
			} else if (reg_id == 2) {
				if (((reg_data>>6) & 0x03) != (cmp_val & 0x03)) {
						printf("  Error: HI%d Reg%d Rx Page Exp : 0x%X, Rd 0x%X\n", 
						hi, reg_id, cmp_val&0x1F, reg_data&0x1F);
					return (-1);
				}
			}
		}
	}
	return (rc);
}

int asic_chk_ni_an_rx_page (uint8_t inst, uint8_t ni, int chk_flds_sel_fec)
{
	int rc = 0;
	uint32_t reg_data, dev_id, reg_id, cmp_val;

	dev_id = 96 + ((ni+1) * 3);
	for (reg_id = 7; reg_id > 1; reg_id--){
		rc = asic_rd (inst, ASIC_SBUS_ADDR(dev_id, reg_id), &reg_data);
		if (rc) return (rc);

		if (chk_flds_sel_fec == 1) {
			cmp_val = (ni&1) ? (ni-1) : (ni+1);
			if (reg_id == 7) {
				if ((reg_data & 0x1F) != (cmp_val & 0x1F)) {
					printf("  Error: HI%d Reg%d Rx Page Exp : 0x%X, Rd 0x%X\n", 
							ni, reg_id, cmp_val&0x1F, reg_data&0x1F);
					return (-1);
				}
			} else if (reg_id == 2) {
				if (((reg_data>>6) & 0x03) != (cmp_val & 0x03)) {
						printf("  Error: HI%d Reg%d Rx Page Exp : 0x%X, Rd 0x%X\n", 
						ni, reg_id, cmp_val&0x1F, reg_data&0x1F);
					return (-1);
				}
			}
		}
	}
	return (rc);
}

int asic_chk_an_rx_page (uint8_t inst, uint64_t hi_mask, uint64_t ni_mask,
			  int chk_flds_sel_fec)
{
        int rc = 0, port;

        for (port = 0; !rc && (port < ASIC(HI_COUNT)); port++, hi_mask >>=1) {
                if (!(hi_mask & 0x01)) continue;
                rc = asic_chk_hi_an_rx_page(inst, port, chk_flds_sel_fec);
        }

        for (port = 0; !rc && (port < ASIC(NI_COUNT)); port++, ni_mask >>=1) {
                if (!(ni_mask & 0x01)) continue;
                rc = asic_chk_ni_an_rx_page(inst, port, chk_flds_sel_fec);
        }
        return (rc);
}

int asic_chk_hi_an_nxt_page (uint8_t inst, uint8_t hi)
{
	int rc = 0;
	uint32_t reg_data, dev_id, reg_id;

	dev_id = (hi+1) * 3;

	for (reg_id = 13; reg_id > 7; reg_id--){
		rc = asic_rd (inst, ASIC_SBUS_ADDR(dev_id, reg_id), &reg_data);
		if (rc) return (rc);
		printf("  HI%d Reg%d  : 0x%X\n", hi, reg_id, reg_data);
	}
	return (rc);
}

int asic_chk_ni_an_nxt_page (uint8_t inst, uint8_t ni)
{
	int rc = 0;
	uint32_t reg_data, dev_id, reg_id;

	dev_id = 96 + ((ni+1) * 3);
	for (reg_id = 13; reg_id > 7; reg_id--){
		rc = asic_rd (inst, ASIC_SBUS_ADDR(dev_id, reg_id), &reg_data);
		if (rc) return (rc);
		printf("  NI%d Reg%d  : 0x%X\n", ni, reg_id, reg_data);
	}

	return (rc);
}

int asic_chk_an_nxt_page (uint8_t inst, uint64_t hi_mask, uint64_t ni_mask)
{
        int rc = 0, port;

        for (port = 0; !rc && (port < ASIC(HI_COUNT)); port++, hi_mask >>=1) {
                if (!(hi_mask & 0x01)) continue;
                rc = asic_chk_hi_an_nxt_page(inst, port);
        }

        for (port = 0; !rc && (port < ASIC(NI_COUNT)); port++, ni_mask >>=1) {
                if (!(ni_mask & 0x01)) continue;
                rc = asic_chk_ni_an_nxt_page(inst, port);
        }
        return (rc);
}


int asic_chk_hi_an_rxpage_avil (uint8_t inst, uint8_t hi, uint8_t nxt_pg, int max_tries)
{
	int rc = 0;
#ifndef DIAG_PORTOLA
	uint32_t reg_data, reg_addr, pg_avail = 0;
	uint32_t pg_chk = nxt_pg? (0x01 << ASIC(HI_STA_XGMII_STATUS_FLD_AN_MR_NEXT_PAGE_RX)):
				  (0x01 << ASIC(HI_STA_XGMII_STATUS_FLD_AN_MR_BASE_PAGE_RX));
	pg_chk = nxt_pg? (0x01 << ASIC(HI_STA_XGMII_STATUS_FLD_AN_MR_NEXT_PAGE_RX)):
			  (0x01 << ASIC(HI_STA_XGMII_STATUS_FLD_AN_MR_BASE_PAGE_RX));
	pg_avail=0;
	reg_addr = HI_REG_ADDR(STA_XGMII_STATUS, hi);

	while (!pg_avail && max_tries--) {
		rc = asic_rd (inst, reg_addr, &reg_data);	
		if (rc) return (rc);

		pg_avail = (reg_data & pg_chk) ? 1 : 0;
	} 

	if (!pg_avail) {
		printf("  HI%d Rx Pg Avail not asserted\n", hi);
		return (-1);
	} 
#endif
	return (rc);
}

int asic_chk_ni_an_rxpage_avil (uint8_t inst, uint8_t ni, uint8_t nxt_pg, int max_tries)
{
	int rc = 0;
	uint32_t pg_chk, reg_data, reg_addr, pg_avail = 0;

	reg_addr = NI_REG_ADDR(STA_XGMII_STATUS, ni);
	pg_chk = nxt_pg? (0x01 << ASIC(NI_STA_XGMII_STATUS_FLD_AN_MR_NEXT_PAGE_RX)):
			  (0x01 << ASIC(NI_STA_XGMII_STATUS_FLD_AN_MR_BASE_PAGE_RX));

	while (!pg_avail && max_tries--) {
		rc = asic_rd (inst, reg_addr, &reg_data);	
		if (rc) return (rc);

		pg_avail = (reg_data & pg_chk) ? 1 : 0;
	}

	if (!pg_avail) {
		printf("  NI%d Rx Pg Avail not asserted\n", ni);
		return (-1);
	} 
	return (rc);
}

int asic_chk_an_rxpage_avil (uint8_t inst, uint64_t hi_mask, uint64_t ni_mask, 
			    uint8_t nxt_pg, int max_tries)
{
        int rc = 0, port;

        for (port = 0; !rc && (port < ASIC(HI_COUNT)); port++, hi_mask >>=1) {
                if (!(hi_mask & 0x01)) continue;
                rc = asic_chk_hi_an_rxpage_avil(inst, port, nxt_pg, max_tries);
        }

        for (port = 0; !rc && (port < ASIC(NI_COUNT)); port++, ni_mask >>=1) {
                if (!(ni_mask & 0x01)) continue;
                rc = asic_chk_ni_an_rxpage_avil(inst, port, nxt_pg, max_tries);
        }
        return (rc);
}

int asic_chk_hi_an_link_good (uint8_t inst, uint8_t hi)
{
	int rc = 0;
#ifndef DIAG_PORTOLA
	uint32_t reg_data, reg_addr = HI_REG_ADDR(STA_XGMII_STATUS, hi);

	rc = asic_rd (inst, reg_addr, &reg_data);	
	if (rc) return (rc);

	if (reg_data & (0x01 << ASIC(HI_STA_XGMII_STATUS_FLD_AN_LINK_GOOD))) {
		printf("  HI%d Link Up\n", hi);
	} else {
		printf("  HI%d Link Down\n", hi);
		rc = -1;
	}
#endif
	return (rc);
}

int asic_chk_ni_an_link_good (uint8_t inst, uint8_t ni)
{
	int rc = 0;
	uint32_t reg_data, reg_addr = NI_REG_ADDR(STA_XGMII_STATUS, ni);

	rc = asic_rd (inst, reg_addr, &reg_data);	
	if (rc) return (rc);

	if (reg_data & (0x01 << ASIC(NI_STA_XGMII_STATUS_FLD_AN_LINK_GOOD))) {
		printf("  NI%d Link Up\n", ni);
	} else {
		printf("  NI%d Link Down\n", ni);
		rc = -1;
	}
	return (rc);
}

int asic_chk_an_link_good (uint8_t inst, uint64_t hi_mask, uint64_t ni_mask)
{
        int rc = 0, port;

        for (port = 0; !rc && (port < ASIC(HI_COUNT)); port++, hi_mask >>=1) {
                if (!(hi_mask & 0x01)) continue;
                rc = asic_chk_hi_an_link_good(inst, port);
        }

        for (port = 0; !rc && (port < ASIC(NI_COUNT)); port++, ni_mask >>=1) {
                if (!(ni_mask & 0x01)) continue;
                rc = asic_chk_ni_an_link_good(inst, port);
        }
        return (rc);
}

int asic_chk_hi_an_complete (uint8_t inst, uint8_t hi)
{
	int rc = 0;
#ifndef DIAG_PORTOLA
	uint32_t reg_data, reg_addr = HI_REG_ADDR(STA_XGMII_STATUS, hi);

	rc = asic_rd (inst, reg_addr, &reg_data);	
	if (rc) return (rc);

	if (reg_data & (0x01 << ASIC(HI_STA_XGMII_STATUS_FLD_AN_MR_AUTONEG_COMPLETE))) {
		printf("  HI%d AN Complete\n", hi);
	} else {
		printf("  HI%d AN Not Complete\n", hi);
		rc = -1;
	}
#endif
	return (rc);
}

int asic_chk_ni_an_complete (uint8_t inst, uint8_t ni)
{
	int rc = 0;
	uint32_t reg_data, reg_addr = NI_REG_ADDR(STA_XGMII_STATUS, ni);

	rc = asic_rd (inst, reg_addr, &reg_data);	
	if (rc) return (rc);

	if (reg_data & (0x01 << ASIC(NI_STA_XGMII_STATUS_FLD_AN_MR_AUTONEG_COMPLETE))) {
		printf("  NI%d AN Complete\n", ni);
	} else {
		printf("  NI%d AN Not Complete\n", ni);
		rc = -1;
	}
	return (rc);
}

int asic_chk_an_complete (uint8_t inst, uint64_t hi_mask, uint64_t ni_mask)
{
        int rc = 0, port;

        for (port = 0; !rc && (port < ASIC(HI_COUNT)); port++, hi_mask >>=1) {
                if (!(hi_mask & 0x01)) continue;
                rc = asic_chk_hi_an_complete(inst, port);
        }

        for (port = 0; !rc && (port < ASIC(NI_COUNT)); port++, ni_mask >>=1) {
                if (!(ni_mask & 0x01)) continue;
                rc = asic_chk_ni_an_complete(inst, port);
        }
        return (rc);
}


int asic_chk_hi_an_hcd (uint8_t inst, uint8_t hi, uint8_t exp_hcd)
{
	int rc = 0;
#ifndef DIAG_PORTOLA
	uint32_t reg_data, reg_addr = HI_REG_ADDR(STA_XGMII_STATUS, hi);

	rc = asic_rd (inst, reg_addr, &reg_data);	
	if (rc) return (rc);

	reg_data >>= 14;
	if ((reg_data & 3) != exp_hcd) {
		printf("  Hi%d HCD mismatch (exp=%d, rd=%d)\n", hi, exp_hcd, reg_data&3);
		rc = -1;
	}	
#endif
	return (rc);
}

int asic_chk_ni_an_hcd (uint8_t inst, uint8_t ni, uint8_t exp_hcd)
{
	int rc = 0;
	uint32_t reg_data, reg_addr = NI_REG_ADDR(STA_XGMII_STATUS, ni);

	rc = asic_rd (inst, reg_addr, &reg_data);	
	if (rc) return (rc);

	reg_data >>= 14;
	if ((reg_data & 3) != exp_hcd) {
		printf("  Ni%d HCD mismatch (exp=%d, rd=%d)\n", ni, exp_hcd, reg_data&3);
		rc = -1;
	}

	return (rc);
}

int asic_chk_an_hcd (uint8_t inst, uint64_t hi_mask, uint64_t ni_mask, uint8_t exp_hcd)
{
        int rc = 0, port;

        for (port = 0; !rc && (port < ASIC(HI_COUNT)); port++, hi_mask >>=1) {
                if (!(hi_mask & 0x01)) continue;
                rc = asic_chk_hi_an_hcd(inst, port, exp_hcd);
        }

        for (port = 0; !rc && (port < ASIC(NI_COUNT)); port++, ni_mask >>=1) {
                if (!(ni_mask & 0x01)) continue;
                rc = asic_chk_ni_an_hcd(inst, port, exp_hcd);
        }
        return (rc);
}


static int asic_chk_hi_pcs_test_err_count (uint8_t inst, uint8_t hi)
{
#if defined(DIAG_WOODSIDE)
	int rc = 0;
	uint32_t reg_data_lo, reg_data_hi, dev_id,count=0;

	dev_id = 2 + (hi * 3);
	rc = asic_rd (inst, ASIC_SBUS_ADDR(dev_id, 246), &reg_data_lo);
	if (rc) return (rc);

	rc = asic_rd (inst, ASIC_SBUS_ADDR(dev_id, 245), &reg_data_hi);
	if (rc) return (rc);
        count = ((reg_data_hi & 0xff) << 8) | (reg_data_lo & 0xff);

        if (count > 0) {
	    printf("  HI%d Test Err Count = 0x%04X\n", hi, count);
        }
	return (count);
#elif defined(DIAG_PORTOLA)
        return 0;
#endif
}

static int asic_chk_ni_pcs_test_err_count (uint8_t inst, uint8_t ni)
{
	int rc = 0;
	uint32_t reg_data_lo, reg_data_hi, dev_id, count=0;

	dev_id = ASIC_NI_PCS_SBUS_DEVID(ni);
	rc = asic_rd (inst, ASIC_SBUS_ADDR(dev_id, 246), &reg_data_lo);
	if (rc) return (rc);

	rc = asic_rd (inst, ASIC_SBUS_ADDR(dev_id, 245), &reg_data_hi);
	if (rc) return (rc);

        count = ((reg_data_hi & 0xff) << 8) | (reg_data_lo & 0xff);
        
        if (count > 0) {
	    printf("  NI%d Test Err Count = 0x%04X\n", ni,count);
        }
	return (count);
}

int asic_chk_pcs_test_err_count (uint8_t inst, uint64_t hi_mask, 
                                 uint64_t ni_mask, uint64_t *hi_fail,
                                 uint64_t *ni_fail)
{
        int rc = 0, port;
        *hi_fail = 0x00ull;
        *ni_fail = 0x00ull;

        for (port = 0; !rc && (port < ASIC(HI_COUNT)); port++, hi_mask >>=1) {
                if (!(hi_mask & 0x01)) continue;
                rc = asic_chk_hi_pcs_test_err_count(inst, port);
                if (rc) {
                  *hi_fail |= (0x1 << port);
                }
        }

        for (port = 0; !rc && (port < ASIC(NI_COUNT)); port++, ni_mask >>=1) {
                if (!(ni_mask & 0x01)) continue;
                rc = asic_chk_ni_pcs_test_err_count(inst, port);
                if (rc) {
                  *ni_fail |= (0x1 << port);
                }
        }
	return ((*hi_fail || *ni_fail) ? 1 : 0);
}

int asic_chk_hi_serdes_prbs (uint8_t inst, uint8_t hi, uint8_t exp_pattern)
{
	int rc = 0;
#ifndef DIAG_PORTOLA
	uint32_t reg_data, reg_addr = HI_REG_ADDR(STA_XGMII_STATUS, hi);

	rc = asic_rd (inst, reg_addr, &reg_data);	
	if (rc) return (rc);

	reg_data >>= 6;
	if ((reg_data & 3) != exp_pattern) {
		printf("  Hi%d HCD Pattern (exp=%d, rd=%d)\n", hi, exp_pattern, reg_data&3);
		rc = -1;
	}
#endif
	return (rc);
}

int asic_chk_ni_serdes_prbs (uint8_t inst, uint8_t ni, uint8_t exp_pattern)
{
	int rc = 0;
	uint32_t reg_data, reg_addr = NI_REG_ADDR(STA_XGMII_STATUS, ni);

	rc = asic_rd (inst, reg_addr, &reg_data);	
	if (rc) return (rc);

	reg_data >>= 6;
	if ((reg_data & 3) != exp_pattern) {
		printf("  Hi%d HCD Pattern (exp=%d, rd=%d)\n", ni, exp_pattern, reg_data&3);
		rc = -1;
	}	

	return (rc);
}

int asic_chk_serdes_prbs (uint8_t inst, uint64_t hi_mask, uint64_t ni_mask, uint8_t exp_pattern)
{
        int rc = 0, port;

        for (port = 0; !rc && (port < ASIC(HI_COUNT)); port++, hi_mask >>=1) {
                if (!(hi_mask & 0x01)) continue;
                rc = asic_chk_hi_serdes_prbs(inst, port, exp_pattern);
        }

        for (port = 0; !rc && (port < ASIC(NI_COUNT)); port++, ni_mask >>=1) {
                if (!(ni_mask & 0x01)) continue;
                rc = asic_chk_ni_serdes_prbs(inst, port, exp_pattern);
        }
        return (rc);
}


int asic_mac_hi_ext_lpbk (uint8_t inst, uint8_t hi, uint8_t lpbk_en)
{
	int rc = 0;
#ifndef DIAG_PORTOLA
	uint32_t reg_addr = HI_REG_ADDR(XE_CFG_XG_CONTROL, hi);

	rc = (lpbk_en ? asic_bset : asic_bclr) 
		(inst, reg_addr, ASIC(HI_XE_CFG_XG_CONTROL_FLD_XGMII_RX2TX));
#endif
	return (rc);
}

int asic_mac_ni_ext_lpbk (uint8_t inst, uint8_t ni, uint8_t lpbk_en)
{
	uint32_t reg_addr = NI_REG_ADDR(XE_CFG_XG_CONTROL, ni);

	return ((lpbk_en? asic_bset : asic_bclr) 
			(inst, reg_addr, ASIC(NI_XE_CFG_XG_CONTROL_FLD_XGMII_RX2TX)));
}

int asic_mac_ext_lpbk (uint8_t inst, uint64_t hi_mask, uint64_t ni_mask, uint8_t lpbk_en)
{
        int rc = 0, port;

        for (port = 0; !rc && (port < ASIC(HI_COUNT)); port++, hi_mask >>=1) {
                if (!(hi_mask & 0x01)) continue;
                rc = asic_mac_hi_ext_lpbk(inst, port, lpbk_en);
        }

        for (port = 0; !rc && (port < ASIC(NI_COUNT)); port++, ni_mask >>=1) {
                if (!(ni_mask & 0x01)) continue;
                rc = asic_mac_ni_ext_lpbk(inst, port, lpbk_en);
        }
        return (rc);
}

int asic_hi_mac_lpbk (uint8_t inst, uint8_t hi, uint8_t lpbk_en)
{
	int rc = 0;
#ifndef DIAG_PORTOLA
	uint32_t reg_addr = HI_REG_ADDR(XE_CFG_XG_CONTROL, hi);

	rc = (lpbk_en? asic_bset : asic_bclr)
			(inst, reg_addr, ASIC(HI_XE_CFG_XG_CONTROL_FLD_XGMII_TX2RX));
#endif
	return (rc);
}

int asic_ni_mac_lpbk (uint8_t inst, uint8_t ni, uint8_t lpbk_en)
{
	uint32_t reg_addr = NI_REG_ADDR(XE_CFG_XG_CONTROL, ni);

	return (lpbk_en? asic_bset : asic_bclr) 
			(inst, reg_addr, ASIC(NI_XE_CFG_XG_CONTROL_FLD_XGMII_TX2RX));
}

int asic_mac_lpbk (uint8_t inst, uint64_t hi_mask, uint64_t ni_mask, uint8_t lpbk_en)
{
        int rc = 0, port;

        for (port = 0; !rc && (port < ASIC(HI_COUNT)); port++, hi_mask >>=1) {
                if (!(hi_mask & 0x01)) continue;
                rc = asic_hi_mac_lpbk(inst, port, lpbk_en);
        }

        for (port = 0; !rc && (port < ASIC(NI_COUNT)); port++, ni_mask >>=1) {
                if (!(ni_mask & 0x01)) continue;
                rc = asic_ni_mac_lpbk(inst, port, lpbk_en);
        }
        return (rc);
}

int asic_hi_pcs_ext_lpbk (uint8_t inst, uint8_t hi, uint8_t lpbk_en)
{
	int rc = 0;
#ifndef DIAG_PORTOLA
	uint32_t dev_id, reg_addr, reg_data;

	reg_addr = HI_REG_ADDR(CFG_XGMII_CNTL, hi);

	rc = asic_rd (inst, reg_addr, &reg_data); 
	if (rc) return (rc);

	reg_data |= ((0x01 << ASIC(HI_CFG_XGMII_CNTL_FLD_PCS_RX_RESET)) |
		     (0x01 << ASIC(HI_CFG_XGMII_CNTL_FLD_PCS_TX_RESET)));

	rc = asic_wr (inst, reg_addr, reg_data); 
	if (rc) return (rc);

	dev_id = 2 + (hi*3);
	rc = asic_wr (inst, ASIC_SBUS_ADDR(dev_id, 1), 8);
	usleep(1000);

	reg_data &= ~((0x01 << ASIC(HI_CFG_XGMII_CNTL_FLD_PCS_RX_RESET)) |
		       (0x01 << ASIC(HI_CFG_XGMII_CNTL_FLD_PCS_TX_RESET)));

	rc = asic_wr (inst, reg_addr, reg_data); 
	if (rc) return (rc);
#endif
	return (rc);
}

int asic_ni_pcs_ext_lpbk (uint8_t inst, uint8_t ni, uint8_t lpbk_en)
{
	int rc = 0;
	uint32_t reg_addr, reg_data, dev_id;

	reg_addr = NI_REG_ADDR(CFG_XGMII_CNTL, ni);

	rc = asic_rd (inst, reg_addr, &reg_data); 
	if (rc) return (rc);

	reg_data |= (0x01 << ASIC(NI_CFG_XGMII_CNTL_FLD_PCS_RX_RESET)) |
		     (0x01 << ASIC(NI_CFG_XGMII_CNTL_FLD_PCS_TX_RESET));

	rc = asic_wr (inst, reg_addr, reg_data); 
	if (rc) return (rc);

	dev_id = 96 + 2 + (ni*3);
	rc = asic_wr (inst, ASIC_SBUS_ADDR(dev_id, 1), 8);
	usleep(1000);

	reg_data &= ~((0x01 << ASIC(NI_CFG_XGMII_CNTL_FLD_PCS_RX_RESET)) |
		       (0x01 << ASIC(NI_CFG_XGMII_CNTL_FLD_PCS_TX_RESET)));

	rc = asic_wr (inst, reg_addr, reg_data); 
	return (rc);
}

int asic_pcs_ext_lpbk (uint8_t inst, uint64_t hi_mask, uint64_t ni_mask, uint8_t lpbk_en)
{
        int rc = 0, port;

        for (port = 0; !rc && (port < ASIC(HI_COUNT)); port++, hi_mask >>=1) {
                if (!(hi_mask & 0x01)) continue;
                rc = asic_hi_pcs_ext_lpbk(inst, port, lpbk_en);
        }

        for (port = 0; !rc && (port < ASIC(NI_COUNT)); port++, ni_mask >>=1) {
                if (!(ni_mask & 0x01)) continue;
                rc = asic_ni_pcs_ext_lpbk(inst, port, lpbk_en);
        }
        return (rc);
}


int asic_hi_pcs_lpbk (uint8_t inst, uint8_t hi, uint8_t lpbk_en)
{
	int rc = 0;
#ifndef DIAG_PORTOLA
	uint32_t reg_addr = HI_REG_ADDR(CFG_XGMII_CNTL, hi);

	rc = (lpbk_en ? asic_bset : asic_bclr) 
			(inst, reg_addr, ASIC(HI_CFG_XGMII_CNTL_FLD_PCS_LOOPBACK)); 
#endif
	return (rc);
}

int asic_ni_pcs_lpbk (uint8_t inst, uint8_t ni, uint8_t lpbk_en)
{
	
	uint32_t reg_addr = NI_REG_ADDR(CFG_XGMII_CNTL, ni);
	return ((lpbk_en? asic_bset : asic_bclr) 
			(inst, reg_addr, ASIC(NI_CFG_XGMII_CNTL_FLD_PCS_LOOPBACK))); 
}

int asic_pcs_lpbk (uint8_t inst, uint64_t hi_mask, uint64_t ni_mask, uint8_t lpbk_en)
{
        int rc = 0, port;

        for (port = 0; !rc && (port < ASIC(HI_COUNT)); port++, hi_mask >>=1) {
                if (!(hi_mask & 0x01)) continue;
                rc = asic_hi_pcs_lpbk(inst, port, lpbk_en);
        }

        for (port = 0; !rc && (port < ASIC(NI_COUNT)); port++, ni_mask >>=1) {
                if (!(ni_mask & 0x01)) continue;
                rc = asic_ni_pcs_lpbk(inst, port, lpbk_en);
        }
        return (rc);
}

int asic_hi_serdes_ext_lpbk (uint8_t inst, uint8_t hi, uint8_t lpbk_en)
{
	int rc = 0;
#ifndef DIAG_PORTOLA
	uint32_t reg_addr = HI_REG_ADDR(CFG_XGMII_CNTL, hi);

	rc = (lpbk_en ? asic_bset : asic_bclr) 
			(inst, reg_addr, ASIC(HI_CFG_XGMII_CNTL_FLD_FAR_LOOPBACK_EN)); 
#endif
	return (rc);
}

int asic_ni_serdes_ext_lpbk (uint8_t inst, uint8_t ni, uint8_t lpbk_en)
{
	uint32_t reg_addr = NI_REG_ADDR(CFG_XGMII_CNTL, ni);
	return ((lpbk_en ? asic_bset : asic_bclr) 
			(inst, reg_addr, ASIC(NI_CFG_XGMII_CNTL_FLD_FAR_LOOPBACK_EN))); 
}

int asic_serdes_ext_lpbk (uint8_t inst, uint64_t hi_mask, uint64_t ni_mask, uint8_t lpbk_en)
{
        int rc = 0, port;

        for (port = 0; !rc && (port < ASIC(HI_COUNT)); port++, hi_mask >>=1) {
                if (!(hi_mask & 0x01)) continue;
                rc = asic_hi_serdes_ext_lpbk(inst, port, lpbk_en);
        }

        for (port = 0; !rc && (port < ASIC(NI_COUNT)); port++, ni_mask >>=1) {
                if (!(ni_mask & 0x01)) continue;
                rc = asic_ni_serdes_ext_lpbk(inst, port, lpbk_en);
        }
        return (rc);
}



#ifndef DIAG_PORTOLA
int asic_hi_serdes_lpbk (uint8_t inst, uint8_t hi, uint8_t lpbk_en)
{
	int rc = 0;
	uint32_t reg_addr = HI_REG_ADDR(CFG_XGMII_CNTL, hi);

	rc = (lpbk_en ? asic_bset : asic_bclr)
		(inst, reg_addr, ASIC(HI_CFG_XGMII_CNTL_FLD_NEAR_LOOPBACK_EN)); 
	return (rc);
}

int asic_ni_serdes_lpbk (uint8_t inst, uint8_t ni, uint8_t lpbk_en)
{
	uint32_t reg_addr = NI_REG_ADDR(CFG_XGMII_CNTL, ni);

	return((lpbk_en? asic_bset : asic_bclr) 
			(inst, reg_addr, ASIC(NI_CFG_XGMII_CNTL_FLD_NEAR_LOOPBACK_EN))); 
}
#endif

int asic_serdes_lpbk (uint8_t inst, uint64_t hi_mask, uint64_t ni_mask, uint8_t lpbk_en)
{
        int rc = 0, port;

        for (port = 0; !rc && (port < ASIC(HI_COUNT)); port++, hi_mask >>=1) {
                if (!(hi_mask & 0x01)) continue;
                rc = asic_hi_serdes_lpbk(inst, port, lpbk_en);
        }

        for (port = 0; !rc && (port < ASIC(NI_COUNT)); port++, ni_mask >>=1) {
                if (!(ni_mask & 0x01)) continue;
                rc = asic_ni_serdes_lpbk(inst, port, lpbk_en);
        }
        return (rc);
}

int asic_hi_serdes_sbus_ext_lpbk (uint8_t inst, uint8_t hi, uint8_t lpbk_en)
{
	int rc = 0;
#ifndef DIAG_PORTOLA
	printf("REVISIT %s \n", __func__);
	rc = asic_wr (inst, ASIC_SBUS_ADDR(((hi*3)+1), 9), 0x11);
#endif
	return (rc);
}

int asic_ni_serdes_sbus_ext_lpbk (uint8_t inst, uint8_t ni, uint8_t lpbk_en)
{
	
	uint32_t dev_id = 97 + (ni*3)+1;
	printf("REVISIT %s \n", __func__);
	return (asic_wr (inst, ASIC_SBUS_ADDR(dev_id, 9), 0x11));
}

int asic_serdes_sbus_ext_lpbk (uint8_t inst, uint64_t hi_mask, uint64_t ni_mask, uint8_t lpbk_en)
{
        int rc = 0, port;

        for (port = 0; !rc && (port < ASIC(HI_COUNT)); port++, hi_mask >>=1) {
                if (!(hi_mask & 0x01)) continue;
                rc = asic_hi_serdes_sbus_ext_lpbk(inst, port, lpbk_en);
        }

        for (port = 0; !rc && (port < ASIC(NI_COUNT)); port++, ni_mask >>=1) {
                if (!(ni_mask & 0x01)) continue;
                rc = asic_ni_serdes_sbus_ext_lpbk(inst, port, lpbk_en);
        }
        return (rc);
}



int asic_hi_xgmac_rx_tx_rst (uint8_t inst, uint8_t hi, uint8_t rx_rst, uint8_t tx_rst)
{
	int rc = 0;
#ifndef DIAG_PORTOLA
	uint32_t reg_addr, reg_data;

	reg_addr = HI_REG_ADDR(XE_CFG_XG_CONTROL, hi);

	rc = asic_rd (inst, reg_addr, &reg_data);
	if (rc) return (rc);

	if (rx_rst) {
		reg_data |= ASIC(HI_XE_CFG_XG_CONTROL_FLD_RX_SW_RST);
	} else {
		reg_data &= ~ASIC(HI_XE_CFG_XG_CONTROL_FLD_RX_SW_RST);
	}
	if (tx_rst) {
		reg_data |= ASIC(HI_XE_CFG_XG_CONTROL_FLD_TX_SW_RST);
	} else {
		reg_data &= ~ASIC(HI_XE_CFG_XG_CONTROL_FLD_TX_SW_RST);
	}
	rc = asic_wr (inst, reg_addr, reg_data);
#endif
	return (rc);
}

int asic_ni_xgmac_rx_tx_rst (uint8_t inst, uint8_t ni, uint8_t rx_rst, uint8_t tx_rst)
{
	int rc = 0;
	uint32_t reg_addr, reg_data;

	reg_addr = NI_REG_ADDR(XE_CFG_XG_CONTROL, ni);

	rc = asic_rd (inst, reg_addr, &reg_data);
	if (rc) return (rc);

	if (rx_rst) {
		reg_data |= ASIC(NI_XE_CFG_XG_CONTROL_FLD_RX_SW_RST);
	} else {
		reg_data &= ~ASIC(NI_XE_CFG_XG_CONTROL_FLD_RX_SW_RST);
	}
	if (tx_rst) {
		reg_data |= ASIC(NI_XE_CFG_XG_CONTROL_FLD_TX_SW_RST);
	} else {
		reg_data &= ~ASIC(NI_XE_CFG_XG_CONTROL_FLD_TX_SW_RST);
	}
	rc = asic_wr (inst, reg_addr, reg_data);
	return (rc);
}

int asic_xgmac_rx_tx_rst (uint8_t inst, uint64_t hi_mask, uint64_t ni_mask, 
			   uint8_t rx_rst, uint8_t tx_rst)
{
        int rc = 0, port;

        for (port = 0; !rc && (port < ASIC(HI_COUNT)); port++, hi_mask >>=1) {
                if (!(hi_mask & 0x01)) continue;
                rc = asic_hi_xgmac_rx_tx_rst(inst, port, rx_rst, tx_rst);
        }

        for (port = 0; !rc && (port < ASIC(NI_COUNT)); port++, ni_mask >>=1) {
                if (!(ni_mask & 0x01)) continue;
                rc = asic_ni_xgmac_rx_tx_rst(inst, port, rx_rst, tx_rst);
        }
        return (rc);
}

int asic_hi_xgmac_sw_rst (uint8_t inst, uint8_t hi, uint8_t sw_rst)
{
	int rc = 0;
#ifndef DIAG_PORTOLA
	uint32_t reg_addr = HI_REG_ADDR(CT_CFG_CONTROL, hi);

	rc = (sw_rst? asic_bset : asic_bclr) 
			(inst, reg_addr, ASIC(HI_CT_CFG_CONTROL_FLD_XE_SW_RST)); 
#endif
	return (rc);
}

int asic_ni_xgmac_sw_rst (uint8_t inst, uint8_t ni, uint8_t sw_rst)
{
	
	uint32_t reg_addr = NI_REG_ADDR(CT_CFG_CONTROL, ni);

	return (sw_rst ? asic_bset : asic_bclr) (inst, reg_addr, ASIC(NI_CT_CFG_CONTROL_FLD_XE_SW_RST)); 
}

int asic_xgmac_sw_rst (uint8_t inst, uint64_t hi_mask, uint64_t ni_mask, uint8_t sw_rst)
{
        int rc = 0, port;

        for (port = 0; !rc && (port < ASIC(HI_COUNT)); port++, hi_mask >>=1) {
                if (!(hi_mask & 0x01)) continue;
                rc = asic_hi_xgmac_sw_rst(inst, port, sw_rst);
        }

        for (port = 0; !rc && (port < ASIC(NI_COUNT)); port++, ni_mask >>=1) {
                if (!(ni_mask & 0x01)) continue;
                rc = asic_ni_xgmac_sw_rst(inst, port, sw_rst);
        }
        return (rc);
}


int asic_hi_xgmac_tx_flush (uint8_t inst, uint8_t hi, uint8_t tx_flush)
{
	int rc = 0;
#ifndef DIAG_PORTOLA
	uint32_t reg_addr = HI_REG_ADDR(XE_CFG_XG_CONTROL, hi);

	rc = (tx_flush? asic_bset : asic_bclr) 
		(inst, reg_addr, ASIC(HI_XE_CFG_XG_CONTROL_FLD_TX_FLUSH_ENABLE)); 
#endif
	return (rc);
}

int asic_ni_xgmac_tx_flush (uint8_t inst, uint8_t ni, uint8_t tx_flush)
{
	uint32_t reg_addr = NI_REG_ADDR(XE_CFG_XG_CONTROL, ni);

	return ((tx_flush? asic_bset : asic_bclr) 
			(inst, reg_addr, ASIC(NI_XE_CFG_XG_CONTROL_FLD_TX_FLUSH_ENABLE))); 
}

int asic_xgmac_tx_flush (uint8_t inst, uint64_t hi_mask, uint64_t ni_mask, uint8_t tx_flush)
{
        int rc = 0, port;

        for (port = 0; !rc && (port < ASIC(HI_COUNT)); port++, hi_mask >>=1) {
                if (!(hi_mask & 0x01)) continue;
                rc = asic_hi_xgmac_tx_flush(inst, port, tx_flush);
        }

        for (port = 0; !rc && (port < ASIC(NI_COUNT)); port++, ni_mask >>=1) {
                if (!(ni_mask & 0x01)) continue;
                rc = asic_ni_xgmac_tx_flush(inst, port, tx_flush);
        }
        return (rc);
}


int asic_1gmac_tx_flush (uint8_t inst, uint64_t hi_mask, uint8_t tx_flush)
{
	int rc = 0;
#ifndef DIAG_PORTOLA
	uint32_t hi, reg_addr;
	for (hi = 0; hi < ASIC(HI_COUNT); hi++, hi_mask >>=1) {
		if (!(hi_mask & 0x01)) continue;

		reg_addr = HI_REG_ADDR(XE_CFG_XG_CONTROL, hi);

		rc = (tx_flush? asic_bset : asic_bclr) 
			(inst, reg_addr, ASIC(HI_XE_CFG_XG_CONTROL_FLD_TX_FLUSH_ENABLE)); 
	}
#endif

	return (rc);
}

int asic_1gmac_rx_tx_rst (uint8_t inst, uint64_t hi_mask, uint8_t rx_rst, uint8_t tx_rst)
{
	int rc = 0;
#ifndef DIAG_PORTOLA
	uint32_t hi, reg_addr, reg_data;
	for (hi = 0; hi < ASIC(HI_COUNT); hi++, hi_mask >>=1) {
		if (!(hi_mask & 0x01)) continue;

		reg_addr = HI_REG_ADDR(GE_CFG, hi);

		rc = asic_rd (inst, reg_addr, &reg_data); 
		if (rc) return (rc);
		if (tx_rst) {
			reg_data |= ASIC(HI_GE_CFG_FLD_TX_SW_RST); 
		} else {
			reg_data &= ~ASIC(HI_GE_CFG_FLD_TX_SW_RST); 
		}
		if (rx_rst) {
			reg_data |= ASIC(HI_GE_CFG_FLD_RX_SW_RST); 
		} else {
			reg_data &= ~ASIC(HI_GE_CFG_FLD_RX_SW_RST); 
		}
		rc = asic_wr (inst, reg_addr, reg_data); 
		if (rc) return (rc);
	}
#endif
	return (rc);
}


int asic_1gmac_sw_rst (uint8_t inst, uint64_t hi_mask, uint8_t sw_rst)
{
	int rc = 0;
#ifndef DIAG_PORTOLA
	uint32_t hi, reg_addr;
	for (hi = 0; hi < ASIC(HI_COUNT); hi++, hi_mask >>=1) {
		if (!(hi_mask & 0x01)) continue;

		reg_addr = HI_REG_ADDR(CT_CFG_CONTROL, hi);

		rc = (sw_rst ? asic_bset : asic_bclr) 
			(inst, reg_addr, ASIC(HI_CT_CFG_CONTROL_FLD_GE_SW_RST)); 
	}
#endif
	return (rc);
}

int asic_hi_ip_mac_mux_rx_tx_en (uint8_t inst, uint8_t hi, uint8_t rx_en, uint8_t tx_en)
{
	int rc = 0;
#ifndef DIAG_PORTOLA
	uint32_t reg_addr, reg_data;

	reg_addr = HI_REG_ADDR(CT_CFG_MUX_RX, hi);
	rc = asic_rd (inst, reg_addr, &reg_data);
	if (rc) return (rc);

	if (rx_en) {
		reg_data |= ASIC(HI_CT_CFG_MUX_RX_FLD_ENABLE);
	} else {
		reg_data &= ~ASIC(HI_CT_CFG_MUX_RX_FLD_ENABLE);
	}
	rc = asic_wr (inst, reg_addr, reg_data);
	if (rc) return (rc);

	reg_addr = BA(ASIC(HI0_CT_CFG_MUX_TX) + hi*(ASIC(HI1_CT_CFG_MUX_TX) - 
		       ASIC(HI0_CT_CFG_MUX_TX)));
	rc = asic_rd (inst, reg_addr, &reg_data);
	if (rc) return (rc);

	if (tx_en) {
		reg_data |= ASIC(HI_CT_CFG_MUX_TX_FLD_ENABLE);
	} else {
		reg_data &= ~ASIC(HI_CT_CFG_MUX_TX_FLD_ENABLE);
	}
	rc = asic_wr (inst, reg_addr, reg_data);
#endif
	return (rc);
}

int asic_ni_ip_mac_mux_rx_tx_en (uint8_t inst, uint8_t ni, uint8_t rx_en, uint8_t tx_en)
{
	int rc = 0;
	uint32_t reg_addr, reg_data;

	reg_addr = NI_REG_ADDR(CT_CFG_MUX_RX, ni);

	rc = asic_rd (inst, reg_addr, &reg_data);
	if (rc) return (rc);

	if (rx_en) {
		reg_data |= ASIC(NI_CT_CFG_MUX_RX_FLD_ENABLE);
	} else {
		reg_data &= ~ASIC(NI_CT_CFG_MUX_RX_FLD_ENABLE);
	}
	rc = asic_wr (inst, reg_addr, reg_data);
	if (rc) return (rc);

	reg_addr = NI_REG_ADDR(CT_CFG_MUX_TX, ni);
	rc = asic_rd (inst, reg_addr, &reg_data);
	if (rc) return (rc);

	if (tx_en) {
		reg_data |= ASIC(NI_CT_CFG_MUX_TX_FLD_ENABLE);
	} else {
		reg_data &= ~ASIC(NI_CT_CFG_MUX_TX_FLD_ENABLE);
	}
	rc = asic_wr (inst, reg_addr, reg_data);
	return (rc);
}

int asic_ip_mac_mux_rx_tx_en (uint8_t inst, uint64_t hi_mask, uint64_t ni_mask, 
			   uint8_t rx_en, uint8_t tx_en)
{
        int rc = 0, port;

        for (port = 0; !rc && (port < ASIC(HI_COUNT)); port++, hi_mask >>=1) {
                if (!(hi_mask & 0x01)) continue;
                rc = asic_hi_ip_mac_mux_rx_tx_en(inst, port, rx_en, tx_en);
        }

        for (port = 0; !rc && (port < ASIC(NI_COUNT)); port++, ni_mask >>=1) {
                if (!(ni_mask & 0x01)) continue;
                rc = asic_ni_ip_mac_mux_rx_tx_en(inst, port, rx_en, tx_en);
        }
        return (rc);
}

int asic_ip_mac_sw_rst (uint8_t inst, uint64_t hi_mask, uint8_t sw_rst)
{
	int rc = 0;
#ifndef DIAG_PORTOLA
	uint32_t hi, reg_addr;
	for (hi = 0; !rc && (hi < ASIC(HI_COUNT)); hi++, hi_mask >>=1) {
		if (!(hi_mask & 0x01)) continue;

		reg_addr = HI_REG_ADDR(CT_CFG_CONTROL, hi);

		rc = (sw_rst? asic_bset : asic_bclr) 
			(inst, reg_addr, ASIC(HI_CT_CFG_CONTROL_FLD_SW_RST)); 
	}
#endif
	return (rc);
}


int asic_serdes_rd (uint8_t inst, uint32_t dev_id, uint32_t addr, uint32_t *data)
{
	return (asic_rd(inst, ASIC_SBUS_ADDR(dev_id, addr), data));
}

int asic_serdes_wr (uint8_t inst, uint32_t dev_id, uint32_t addr, uint32_t data)
{
	return (asic_wr(inst, ASIC_SBUS_ADDR(dev_id, addr), data));
}

/*
 * programmed serdes from data given by SI team
 */
int asic_serdes_si_config (uint8_t inst, uint64_t hi_mask, uint64_t ni_mask)
{
    int rc=0, port;
    unsigned int index;
    /* Commands to turn off Default DFE on Receive */
    static unsigned int addr_val[][2] = {
        {0x26,0x07},
        {0x1f,0x40},
        {0x20,0x00},
        {0x21,0x00},
        {0x2e,0x10},
        {0x23,0x40},
        {0x24,0x00},
        {0x25,0x00},
        {0x2f,0x10},
    };
#if defined(DIAG_BROCKWAY)
    /* Serdes values given by SI team for serdes transmit */
    static unsigned int hi_reg3e[] = {
       0xab ,0xab ,0xc3 ,0xb3 ,0xab ,0xa3,0xab,0x9b,
       0xa7 ,0x87 ,0xa7 ,0x97 ,0x93 ,0x83,0xa3,0x83,
       0x83 ,0x63 ,0x97 ,0xa3 ,0x73 ,0x63,0xa3,0x93,
       0x53 ,0x63 ,0x93 ,0x83 ,0x93 ,0x93,0x93,0x93,
    };
    static unsigned int ni_reg3e[] = {
       0xbb ,0xab ,0xbb ,0xab ,0xab ,0xab,0xbb,0xab
    }; 
    static unsigned int hi_reg3d[] = {
       0x0 ,0x0 ,0x0 ,0x0 ,0x0 ,0x0 ,0x0 ,0x0,
       0x0 ,0x0 ,0x0 ,0x0 ,0x0 ,0x0 ,0x0 ,0x0,
       0x0 ,0x0 ,0x0 ,0x0 ,0x0 ,0x10 ,0x0 ,0x0,
       0x0 ,0x10 ,0x0 ,0x0 ,0x4 ,0x0 ,0x0 ,0x0,
    };
    static unsigned int ni_reg3d[] = {
       0x0 ,0x0 ,0x0 ,0x0 ,0x0 ,0x0 ,0x0 ,0x0,
    }; 
#else
    /* Serdes values given by SI team for serdes transmit */
    static unsigned int hi_reg3d[] = {
       0x0 ,0x0 ,0x0 ,0x0 ,0x0 ,0x20,0x20,0x20,0x24,0x24,
       0x28,0x28,0x30,0x30,0x34,0x34,0x38,0x38,0x38,0x3c,
       0x3c,0x3c,0x3c,0x3c,0x34,0x38,0x34,0x34,0x38,0x34,
       0x30,0x20
    };
    static unsigned int ni_reg3d[] = {
       0x20,0x20,0x14,0x0c,0x00,0x04,0x00,0x00
    }; 
    static unsigned int hi_reg3e[] = {
       0x73,0x73,0x73,0x63,0x63,0x53,0x53,0x53,0x33,0x33,
       0x33,0x33,0x23,0x23,0x33,0x33,0x13,0x23,0x23,0x23,
       0x03,0x23,0x23,0x23,0x23,0x33,0x33,0x33,0x33,0x43,
       0x43,0x53};
    static unsigned int ni_reg3e[] = {
       0x53,0x63,0x63,0x63,0x83,0x93,0x93,0x93
    }; 
#endif
    for (port = 0; !rc && (port < ASIC(HI_COUNT)); port++, hi_mask >>=1) {
        if (!(hi_mask & 0x01)) continue;
        /* dev_id == port * 3 + 1 
         * addr == 0x3D, 0x3E
         */
//        printf("reg3d 0x%x 0x%x\n",
 //              ASIC_SBUS_ADDR((3*port)+1,0x3d), hi_reg3d[port]);
        rc = asic_wr(inst, ASIC_SBUS_ADDR((3*port)+1,0x3d), hi_reg3d[port]);
        rc = asic_wr(inst, ASIC_SBUS_ADDR((3*port)+1,0x3e), hi_reg3e[port]);
        rc = asic_wr(inst, ASIC_SBUS_ADDR((3*port)+1,0x41), 0x00);
        for (index = 0; index < (sizeof(addr_val)/(2*sizeof(unsigned int)));
             index++) {
             rc = asic_wr(inst, ASIC_SBUS_ADDR((3*port)+1,addr_val[index][0]), 
                          addr_val[index][1]);
        }
    }

    for (port = 0; !rc && (port < ASIC(NI_COUNT)); port++, ni_mask >>=1) {
        if (!(ni_mask & 0x01)) continue;
        rc = asic_wr(inst, ASIC_SBUS_ADDR((3*(32+port))+1,0x3d), 
                     ni_reg3d[port]);
        rc = asic_wr(inst, ASIC_SBUS_ADDR((3*(32+port))+1,0x3e), 
                     ni_reg3e[port]);
        rc = asic_wr(inst, ASIC_SBUS_ADDR((3*(32+port))+1,0x41), 
                     0x0);
        for (index = 0; index < (sizeof(addr_val)/(2*sizeof(unsigned int)));
             index++) {
             rc = asic_wr(inst, ASIC_SBUS_ADDR((3*(32+port))+1,
                                  addr_val[index][0]), addr_val[index][1]);
        }
    }
    return (rc);
}

int asic_pcs_prbs_test (uint8_t inst, uint64_t hi_mask, uint64_t ni_mask, 
                        const int wait_time, const int pattern_sel, 
                        uint64_t *hi_fail, uint64_t *ni_fail)
{
    int ret_val = 0;
    uint32_t seeda_lo, seedb_lo; //lower 32-bits of the seeds
    uint32_t seeda_hi, seedb_hi; //upper 26-bits of the seeds

    seeda_hi = 0x0;
    seeda_lo = 0x0;
    seedb_hi = 0xabababab;
    seedb_lo = 0xabababab;
    /* Enable SERDES */
    ret_val = asic_cfg_pcs_reset(inst, hi_mask, ni_mask, 1, 1, 0);
    if (ret_val) {
        printf("PCS in reset for HI/NI Error\n");
        return (ret_val);
    }

    ret_val = asic_cfg_serdes_ena(inst, hi_mask, ni_mask, 0x1, 0x1);
    if (ret_val) {
        printf("Serdes Enable Error\n");
        return (ret_val);
    }

    /* remove HI/NI out of reset */

    ret_val = asic_cfg_pcs_reset(inst, hi_mask, ni_mask, 0, 0, 0);
    if (ret_val) {
        printf("PCS out of reset for HI/NI Error\n");
        return (ret_val);
    }

    ret_val = asic_serdes_lpbk(inst, hi_mask, ni_mask, 0x1);
    if (ret_val) {
        printf("Serdes Loopback Error\n");
        return (ret_val);
    }
    ret_val = asic_chk_pcs_status(inst, hi_mask, ni_mask, 1700, hi_fail, 
                                  ni_fail);
    if (ret_val) {
        printf("PCS Status Check Error\n");
        return (ret_val);
    }
    /*
     * seeda_en = 0; seedb_en = 1;
     */
    ret_val = asic_cfg_pcs_test_seeds(inst, hi_mask, ni_mask, 0x0, 0x1, 
                                      seeda_hi, seeda_lo, seedb_hi, seedb_lo);
    if (ret_val) {
        printf("PCS Test Seeds Error\n");
        return (ret_val);
    }
    ret_val = asic_cfg_pcs_tx_testmode(inst, hi_mask, ni_mask, 0x1, 
                                       pattern_sel);
    if (ret_val) {
        printf("PCS TX Test mode Error\n");
        return (ret_val);
    }

    ret_val = asic_cfg_pcs_rx_testmode(inst, hi_mask, ni_mask, 0x1, 
                                       pattern_sel);
    if (ret_val) {
        printf("PCS TX Test mode Error\n");
        return (ret_val);
    }
    ret_val = asic_chk_pcs_test_err_count(inst, hi_mask, ni_mask, hi_fail,
                                          ni_fail);
    if (ret_val) {
        printf("Error in PCS Test Error count\n");
        return (ret_val);
    }
    return (ret_val);
}
