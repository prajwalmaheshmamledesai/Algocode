/********************************************************************
 *
 *      File:   woo_pkt_init.c
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
#include "woo_woo_naxislave_define.h"
#include "woo_ops.h"

#define ASIC_DEST_PORT_NIF(port)	(port & 7)
#define ASIC_DEST_PORT_HIF(port)	(port)
#define ASIC_DEST_PORT_BIF		0x20
#define ASIC_DEST_PORT_CIF		0x21

// Create a BitMask between A & B.
#define BMASK(a,b)	(((0x01 << ((a)-(b)+1))-1) << (b))

#define ASIC_VNTAG_STRIP	0x00
#define ASIC_VNTAG_TAG		0x01
#define ASIC_VNTAG_DEFAULT	0x02
#define ASIC_VNTAG_PASSTHRU	0x03

int asic_config_hif_cif(int inst, int port, int entry)
{
	int rc = 0;
	uint32_t addr, data = 0x00;

	// Program VNTAG Mode
	addr = HI_REG_ADDR(CFG_VNTAG_RX, port);
	rc = asic_rd (inst, addr, &data);
	if (rc) return (rc);

	data |= ASIC_VNTAG_DEFAULT;
	data |= (0x01 << ASIC(HI_CFG_VNTAG_RX_FLD_ALLOW));

	rc = asic_wr (inst, addr, data);
	if (rc) return (rc);

	// Clear Default Dest information & Program Default Dest
	addr = HI_REG_ADDR(CFG_PORT, port);
	rc = asic_rd (inst, addr, &data);
	if (rc) return (rc);

	data |= (0x0F << 4);	// UC_ALLOWED RANGE = allow all
	data |= (0x01 << ASIC(HI_CFG_PORT_FLD_VNTAG_CHK_DIS));
	data &= ~BMASK(22,17);
	data |= (ASIC_DEST_PORT_CIF << 17);
	data |= (0x01 << ASIC(HI_CFG_PORT_FLD_DEFAULT_EN));

	rc = asic_wr (inst, addr, data);
	return (rc);
}

int asic_config_nif_to_cif(int inst, uint8_t nif_port, uint32_t nvif)
{
	int rc = 0;
	uint32_t addr, data=0x00, mask =0;
	uint8_t  boffset = 6 *(nvif&0x3);

	// Program the LookUp to send packets to CIF.
	addr = BA(ASIC(NF_DHS_UCAST_MEMIF_DATA)+((nif_port > 3)? 0x400 : 0x00)+ (nvif>>2));
	mask = 0x3F << boffset;

	rc = asic_rd (inst, addr, &data);
	if (rc) return (rc);

	data &= ~mask;
	data |= (ASIC_DEST_PORT_CIF << boffset);

	rc = asic_wr (inst, addr, data);
	if (rc) return (rc);

	return (0);
}

int asic_config_nif_to_drop(int inst)
{
	int rc = 0;
	uint32_t addr=BA(ASIC(NF_DHS_UCAST_MEMIF_DATA)); 
	uint32_t data=0xFFFFFFFF, cnt =0;

	for (cnt = 0; !rc && (cnt < ASIC(NF_DHS_UCAST_MEMIF_DATA_ARRAYSIZE)); 
		cnt++, addr+=4) {
		rc = asic_wr (inst, addr, data);
	}
	return (rc);
}

int asic_config_nif_to_hif(int inst, uint32_t nvif, uint8_t hvif, uint8_t nifport)
{
	int rc = 0;
	uint32_t addr, data=0, mask;
	uint8_t  boffset = 6 *(nvif&0x3);

	// Program the LookUp to send packets to HIF.
        /*
         * WOO_NF_CFG_NIFx is set such that 0-1k dest VIFS at (woo_core_init.c)
         * NIF0, NIF1, NIF2, NIF3 use offset 0x0.
         * NIF4, NIF5, NIF6, NIF7 use offset 0x400
         */
	addr = BA(ASIC(NF_DHS_UCAST_MEMIF_DATA)+(((nifport&7) > 3)? 0x400 : 0x00)+
                 (nvif>>2));
	mask = 0x3F << boffset;

	rc = asic_rd (inst, addr, &data);
	if (rc) return (rc);

	data &= ~mask;
	data |= (hvif << boffset);

	rc = asic_wr (inst, addr, data);
        printf("NVIF%d to HVIF%d [0x%X] : 0x%X\n", nvif, hvif, addr, data);
	if (rc) return (rc);

	return (0);
}

int asic_config_nif_to_mc_hif(int inst, uint32_t nvif, uint32_t hif_mask)
{
	// Program the LookUp to send packets to CIF.
	asic_wr (inst, BA(ASIC(NF_DHS_MCAST_MEMIF_DATA) + (nvif*2) ), hif_mask);
	asic_wr (inst, BA(ASIC(NF_DHS_MCAST_MEMIF_DATA) + 1 + (nvif*2) ), 0x00);
	return (0);
}


int asic_pif_cfg(uint8_t inst, uint32_t fp_mask, uint32_t bp_mask)
{
	int port, rc = 0;
	uint32_t addr, data = 0x00;
#if defined(PKT_INIT_REMOVE_RB)
	rc = asic_mem_init (inst);
	if (rc) return (rc);
#endif

	rc = asic_config_nif_to_drop(inst);
	if (rc) return (rc);

	// Network to CIF
	for (port = 0; port < ASIC(NI_COUNT); port++) {
		if (!(fp_mask & (0x01 << port))) continue;

		// Program Default VNTAG to CIF. 
		addr = NI_REG_ADDR(CFG_RX_DEFAULT_VNTAG, port);
		data = (ASIC_DEST_PORT_NIF(port) << 16) | ASIC_DEST_PORT_NIF(port);
		rc = asic_wr (inst, addr, data);
		if (rc) return (rc);

		// Program RX VNTAG to take default 
		addr = NI_REG_ADDR(CFG_VNTAG_RX, port);
		rc = asic_rd (inst, addr, &data);
		if (rc) return (rc);

		// STRIP VNTAG and use default
		data &= ~BMASK(1,0);
		data |= ASIC_VNTAG_DEFAULT;
		data |= (0x01 << ASIC(NI_CFG_VNTAG_RX_FLD_ALLOW));

		rc = asic_wr (inst, addr, data);
		if (rc) return (rc);

                // UC Allowed Range. 
                addr = NI_REG_ADDR(CFG_PORT, port);
                rc = asic_rd (inst, addr, &data);
                if (rc) return (rc);
		data |= (0x0F << 5);	// UC_ALLOWED RANGE = allow all
		data |= (0x01 << ASIC(NI_CFG_PORT_FLD_VNTAG_CHK_DIS));
                rc = asic_wr (inst, addr, data);
                if (rc) return (rc);

		// Program the LookUp to send packets to CIF.
		rc = asic_config_nif_to_cif(inst, port, ASIC_DEST_PORT_NIF(port));
                if (rc) return (rc);
	}

	// Host to CIF
	for (port = 0; port < ASIC(HI_COUNT); port++) {
		if (!(bp_mask & (0x01 << port))) continue;

		// Program Default VNTAG to CIF.
		addr = HI_REG_ADDR(CFG_RX_DEFAULT_VNTAG, port);
		data = (ASIC_DEST_PORT_CIF << 16) | ASIC_DEST_PORT_CIF;
		rc = asic_wr (inst, addr, data);
		if (rc) return (rc);

		// Clear Default Dest information & Program Default Dest
		addr = HI_REG_ADDR(CFG_PORT, port);
		rc = asic_rd (inst, addr, &data);
		if (rc) return (rc);

		data |= (0x0F << 4);	// UC_ALLOWED RANGE = allow all
		data |= (0x01 << ASIC(HI_CFG_PORT_FLD_VNTAG_CHK_DIS));
		data &= ~BMASK(22,17);
		data |= (ASIC_DEST_PORT_CIF << 17);
		data |= (0x01 << ASIC(HI_CFG_PORT_FLD_DEFAULT_EN));

		rc = asic_wr (inst, addr, data);
		if (rc) return (rc);

		// Program VNTAG Mode
		addr = HI_REG_ADDR(CFG_VNTAG_RX, port);
		rc = asic_rd (inst, addr, &data);
		if (rc) return (rc);

		// STRIP VNTAG and use default
		data |= ASIC_VNTAG_DEFAULT;
		data |= (0x01 << ASIC(HI_CFG_VNTAG_RX_FLD_ALLOW));

		rc = asic_wr (inst, addr, data);
		if (rc) return (rc);
	}

	return (rc);
}

int asic_hton_cfg(uint8_t inst, uint32_t fp_mask, uint32_t bp_mask)
{
	int port, rc = 0;
	uint32_t addr, data = 0x00;

	// Network to CIF
	rc = asic_config_nif_to_drop(inst);
	if (rc) return (rc);

	for (port = 0; port < ASIC(NI_COUNT); port++) {
		if (!(fp_mask & (0x01 << port))) continue;

		// Program Default VNTAG to CIF. 
		addr = NI_REG_ADDR(CFG_RX_DEFAULT_VNTAG, port);
		data = (ASIC_DEST_PORT_NIF(port) << 16) | ASIC_DEST_PORT_NIF(port);
		rc = asic_wr (inst, addr, data);
		if (rc) return (rc);

		// Program RX VNTAG to take default 
		addr = NI_REG_ADDR(CFG_VNTAG_RX, port);
		rc = asic_rd (inst, addr, &data);
		if (rc) return (rc);

		data &= ~BMASK(1,0);
		data |= ASIC_VNTAG_DEFAULT;
		data |= (0x01 << ASIC(NI_CFG_VNTAG_RX_FLD_ALLOW));

		rc = asic_wr (inst, addr, data);
		if (rc) return (rc);

                // UC Allowed Range. 
                addr = NI_REG_ADDR(CFG_PORT, port);
                rc = asic_rd (inst, addr, &data);
                if (rc) return (rc);
		data |= (0x0F << 5);	// UC_ALLOWED RANGE = allow all
		data |= (0x01 << ASIC(NI_CFG_PORT_FLD_VNTAG_CHK_DIS));
                rc = asic_wr (inst, addr, data);
                if (rc) return (rc);

		// Program the LookUp to send packets to CIF.
		rc = asic_config_nif_to_cif(inst, port, ASIC_DEST_PORT_NIF(port));
                if (rc) return (rc);
	}

	// Host to NIF
	for (port = 0; port < ASIC(HI_COUNT); port++) {
		if (!(bp_mask & (0x01 << port))) continue;

		// Program Default VNTAG to CIF.
		addr = HI_REG_ADDR(CFG_RX_DEFAULT_VNTAG, port);
		data = (ASIC_DEST_PORT_NIF(port&7) << 16) | ASIC_DEST_PORT_NIF(port&7);
		rc = asic_wr (inst, addr, data);
		if (rc) return (rc);

		// Clear Default Dest information & Program Default Dest
		addr = HI_REG_ADDR(CFG_PORT, port);
		rc = asic_rd (inst, addr, &data);
		if (rc) return (rc);

		data |= (0x0F << 4);	// UC_ALLOWED RANGE = allow all
		data |= (0x01 << ASIC(HI_CFG_PORT_FLD_VNTAG_CHK_DIS));
		data &= ~BMASK(22,17);
		data |= (ASIC_DEST_PORT_NIF(port&0x7) << 17);
		data |= (0x01 << ASIC(HI_CFG_PORT_FLD_DEFAULT_EN));

		rc = asic_wr (inst, addr, data);
		if (rc) return (rc);

		// Program VNTAG Mode
		addr = HI_REG_ADDR(CFG_VNTAG_RX, port);
		rc = asic_rd (inst, addr, &data);
		if (rc) return (rc);

		data |= ASIC_VNTAG_DEFAULT;
		data |= (0x01 << ASIC(HI_CFG_VNTAG_RX_FLD_ALLOW));

		rc = asic_wr (inst, addr, data);
		if (rc) return (rc);
	}

	return (rc);
}

int asic_ntoh_cfg(uint8_t inst, uint32_t fp_mask, uint32_t bp_mask)
{
	int port, rc = 0;
	uint32_t addr, data = 0x00;

	// Network to HIF
	rc = asic_config_nif_to_drop(inst);
	if (rc) return (rc);

	for (port = 0; port < ASIC(NI_COUNT); port++) {
		if (!(fp_mask & (0x01 << port))) continue;

		// Program Default VNTAG to HIF. 
		addr = NI_REG_ADDR(CFG_RX_DEFAULT_VNTAG, port);
		data = (ASIC_DEST_PORT_NIF(port) << 16) | ASIC_DEST_PORT_NIF(port);
		rc = asic_wr (inst, addr, data);
		if (rc) return (rc);

		// Program RX VNTAG to take default 
		addr = NI_REG_ADDR(CFG_VNTAG_RX, port);
		rc = asic_rd (inst, addr, &data);
		if (rc) return (rc);

		data &= ~BMASK(1,0);
		data |= ASIC_VNTAG_DEFAULT;
		data |= (0x01 << ASIC(NI_CFG_VNTAG_RX_FLD_ALLOW));

		rc = asic_wr (inst, addr, data);
		if (rc) return (rc);

                // UC Allowed Range. 
                addr = NI_REG_ADDR(CFG_PORT, port);
                rc = asic_rd (inst, addr, &data);
                if (rc) return (rc);
		data |= (0x0F << 5);	// UC_ALLOWED RANGE = allow all
		data |= (0x01 << ASIC(NI_CFG_PORT_FLD_VNTAG_CHK_DIS));
                rc = asic_wr (inst, addr, data);
                if (rc) return (rc);

		// Program the LookUp to send packets to CIF.
		rc = asic_config_nif_to_hif(inst, ASIC_DEST_PORT_NIF(port), 
                                            ASIC_DEST_PORT_HIF(port), port);
                if (rc) return (rc);
	}

	// Host to CIF
	for (port = 0; port < ASIC(HI_COUNT); port++) {
		if (!(bp_mask & (0x01 << port))) continue;

		// Program Default VNTAG to CIF.
		addr = HI_REG_ADDR(CFG_RX_DEFAULT_VNTAG, port);
		data = (ASIC_DEST_PORT_CIF << 16) | ASIC_DEST_PORT_CIF;
		rc = asic_wr (inst, addr, data);
		if (rc) return (rc);

		// Clear Default Dest information & Program Default Dest
		addr = HI_REG_ADDR(CFG_PORT, port);
		rc = asic_rd (inst, addr, &data);
		if (rc) return (rc);

		data |= (0x0F << 4);	// UC_ALLOWED RANGE = allow all
		data |= (0x01 << ASIC(HI_CFG_PORT_FLD_VNTAG_CHK_DIS));
		data &= ~BMASK(22,17);
		data |= (ASIC_DEST_PORT_CIF << 17);
		data |= (0x01 << ASIC(HI_CFG_PORT_FLD_DEFAULT_EN));

		rc = asic_wr (inst, addr, data);
		if (rc) return (rc);

		// Program VNTAG Mode
		addr = HI_REG_ADDR(CFG_VNTAG_RX, port);
		rc = asic_rd (inst, addr, &data);
		if (rc) return (rc);

		data |= ASIC_VNTAG_DEFAULT;
		data |= (0x01 << ASIC(HI_CFG_VNTAG_RX_FLD_ALLOW));

		rc = asic_wr (inst, addr, data);
		if (rc) return (rc);
	}
	return (0);
}

int asic_mcast_cfg(uint8_t inst, uint32_t fp_mask, uint32_t bp_mask)
{
	int port, rc = 0;
	uint32_t addr, data = 0x00;

	// Network to HIF
	rc = asic_config_nif_to_drop(inst);
	if (rc) return (rc);

	for (port = 0; port < ASIC(NI_COUNT); port++) {
		if (!(fp_mask & (0x01 << port))) continue;

		// Program Default VNTAG to HIF. 
		addr = NI_REG_ADDR(CFG_RX_DEFAULT_VNTAG, port);
		data = (0x01 << 30) | (ASIC_DEST_PORT_NIF(port) << 16) | 
			ASIC_DEST_PORT_NIF(port); // Enable Multicast
		rc = asic_wr (inst, addr, data);
		if (rc) return (rc);

		// Program RX VNTAG to take default 
		addr = NI_REG_ADDR(CFG_VNTAG_RX, port);
		rc = asic_rd (inst, addr, &data);
		if (rc) return (rc);

		data &= ~BMASK(1,0);
		data |= ASIC_VNTAG_DEFAULT;
		data |= (0x01 << ASIC(NI_CFG_VNTAG_RX_FLD_ALLOW));

		rc = asic_wr (inst, addr, data);
		if (rc) return (rc);

                // UC Allowed Range. 
                addr = NI_REG_ADDR(CFG_PORT, port);
                rc = asic_rd (inst, addr, &data);
                if (rc) return (rc);
		data |= (0x0F << 5);	// UC_ALLOWED RANGE = allow all
		data |= (0x01 << ASIC(NI_CFG_PORT_FLD_VNTAG_CHK_DIS));
                rc = asic_wr (inst, addr, data);
                if (rc) return (rc);

		// Program the LookUp to send packets to CIF.
		rc = asic_config_nif_to_mc_hif(inst, ASIC_DEST_PORT_NIF(port), bp_mask);
                if (rc) return (rc);
	}

	// Host to CIF
	for (port = 0; port < ASIC(HI_COUNT); port++) {
		if (!(bp_mask & (0x01 << port))) continue;

		// Program Default VNTAG to CIF.
		addr = HI_REG_ADDR(CFG_RX_DEFAULT_VNTAG, port);
		data = (ASIC_DEST_PORT_CIF << 16) | ASIC_DEST_PORT_CIF;
		rc = asic_wr (inst, addr, data);
		if (rc) return (rc);

		// Clear Default Dest information & Program Default Dest
		addr = HI_REG_ADDR(CFG_PORT, port);
		rc = asic_rd (inst, addr, &data);
		if (rc) return (rc);

		data |= (0x0F << 4);	// UC_ALLOWED RANGE = allow all
		data |= (0x01 << ASIC(HI_CFG_PORT_FLD_VNTAG_CHK_DIS));
		data &= ~BMASK(22,17);
		data |= (ASIC_DEST_PORT_CIF << 17);
		data |= (0x01 << ASIC(HI_CFG_PORT_FLD_DEFAULT_EN));

		rc = asic_wr (inst, addr, data);
		if (rc) return (rc);

		// Program VNTAG Mode
		addr = HI_REG_ADDR(CFG_VNTAG_RX, port);
		rc = asic_rd (inst, addr, &data);
		if (rc) return (rc);

		data |= ASIC_VNTAG_DEFAULT;
		data |= (0x01 << ASIC(HI_CFG_VNTAG_RX_FLD_ALLOW));

		rc = asic_wr (inst, addr, data);
		if (rc) return (rc);
	}
	return (0);
}

static int ni_pkt_test_cfg (uint8_t inst, int port)
{
        uint32_t addr, data = 0x00;
        int rc = 0;
	// Program NOT TO STRIP VNTAG 
	addr = NI_REG_ADDR(CFG_VNTAG_TX, port);
	data = ASIC_VNTAG_PASSTHRU; 
	rc = asic_wr(inst, addr, data);
	if (rc) return (rc);
	// Program Default VNTAG to HIF. 
	addr = NI_REG_ADDR(CFG_RX_DEFAULT_VNTAG, port);
	data = (ASIC_DEST_PORT_NIF(port) << 16)| ASIC_DEST_PORT_NIF(port);
	rc = asic_wr (inst, addr, data);
	if (rc) return (rc);

	// Program RX VNTAG to take default 
	addr = NI_REG_ADDR(CFG_VNTAG_RX, port);
	rc = asic_rd (inst, addr, &data);
	if (rc) return (rc);

	data &= ~BMASK(1,0);
	data |= ASIC_VNTAG_PASSTHRU;
	data |= (0x01 << ASIC(NI_CFG_VNTAG_RX_FLD_IS_VNTAG));
	data |= (0x01 << ASIC(NI_CFG_VNTAG_RX_FLD_ALLOW));

	rc = asic_wr (inst, addr, data);
	if (rc) return (rc);

        // UC Allowed Range. 
        addr = NI_REG_ADDR(CFG_PORT, port);
        rc = asic_rd (inst, addr, &data);
        if (rc) return (rc);
	data |= (0x0F << 5);	// UC_ALLOWED RANGE = allow all
	data |= (0x01 << ASIC(NI_CFG_PORT_FLD_VNTAG_CHK_DIS));
        rc = asic_wr (inst, addr, data);
        if (rc) return (rc);
        return 0;
}

static int hi_pkt_test_cfg (uint8_t inst, int port, int destport)
{
        uint32_t addr, data = 0x00;
        int rc = 0;
	// Program Default VNTAG to NIF.
	addr = HI_REG_ADDR(CFG_RX_DEFAULT_VNTAG, port);
	data = (ASIC_DEST_PORT_HIF(port) << 16) | ASIC_DEST_PORT_HIF(port);
	rc = asic_wr (inst, addr, data);
	if (rc) return (rc);

	// Clear Default Dest information & Program Default Dest
	addr = HI_REG_ADDR(CFG_PORT, port);
	rc = asic_rd (inst, addr, &data);
	if (rc) return (rc);

	data |= (0x0F << 4);	// UC_ALLOWED RANGE = allow all
	data |= (0x01 << ASIC(HI_CFG_PORT_FLD_VNTAG_CHK_DIS));
	data &= ~BMASK(22,17);
	data |= (destport << 17);
	data |= (0x01 << ASIC(HI_CFG_PORT_FLD_DEFAULT_EN));

	rc = asic_wr (inst, addr, data);
	if (rc) return (rc);

	// Program RX VNTAG Mode
	addr = HI_REG_ADDR(CFG_VNTAG_RX, port);
	rc = asic_rd (inst, addr, &data);
	if (rc) return (rc);

	data &= ~(0x01 << ASIC(HI_CFG_VNTAG_RX_FLD_IS_VNTAG));
	data |= ASIC_VNTAG_DEFAULT;
	data |= (0x01 << ASIC(HI_CFG_VNTAG_RX_FLD_ALLOW));

	rc = asic_wr (inst, addr, data);

	rc = asic_rd (inst, addr, &data);
	// Program TX VNTAG Mode
	addr = HI_REG_ADDR(CFG_VNTAG_TX, port);
	rc = asic_rd (inst, addr, &data);
	if (rc) return (rc);
	data |= ASIC_VNTAG_PASSTHRU;
	rc = asic_wr (inst, addr, data);
	if (rc) return (rc);
        return (0);
}
/*
 * Snake configurations for self loopback plugs and PHY loopbacks 
 */

int asic_snake_cfg(uint8_t inst, uint32_t fp_mask, uint32_t bp_mask)
{
	int port, rc = 0;

	// NIF to HIF
	rc = asic_config_nif_to_drop(inst);
	if (rc) return (rc);

	for (port = 0; port < ASIC(NI_COUNT); port++) {
		if (!(fp_mask & (0x01 << port))) continue;
                rc = ni_pkt_test_cfg(inst, port);
                if (rc) return (rc);
	}
	for (port = 0; port < ASIC(HI_COUNT); port++) {
		// Program the LookUp to send packets to HIF.
		rc = asic_config_nif_to_hif(inst, ASIC_DEST_PORT_HIF(port), 
                                        ASIC_DEST_PORT_HIF((port+1)&0x1F), port);
		if (rc) return (rc);
	}
	// HIF to NIF
	for (port = 0; port < ASIC(HI_COUNT); port++) {
		if (!(bp_mask & (0x01 << port))) continue;
                rc = hi_pkt_test_cfg(inst, port, (ASIC_DEST_PORT_NIF(port&7)));
                if (rc) return (rc);

	}
	return (0);
}
/*
 * Snake configurations when external cables are connected between different port
 */
int asic_port_to_port_snake_cfg(uint8_t inst, uint32_t fp_mask, uint32_t bp_mask)
{
	int port, rc = 0;

	// NIF to HIF
	rc = asic_config_nif_to_drop(inst);
	if (rc) return (rc);

	for (port = 0; port < ASIC(NI_COUNT); port++) {
		if (!(fp_mask & (0x01 << port))) continue;
                rc = ni_pkt_test_cfg(inst, port);
                if (rc) return (rc);
	}
        // Program NF for NIF to HIF forwarding
	for (port = 0; port < ASIC(HI_COUNT); port++) {
		// Program the LookUp to send packets to HIF.
                if (port & 0x1) {
                    if ((port+1) < ASIC(HI_COUNT)) {
		        rc = asic_config_nif_to_hif(inst, ASIC_DEST_PORT_HIF(port), 
                             ASIC_DEST_PORT_HIF((port+1)&0x1F), port);
                    } else {
		        rc = asic_config_nif_to_hif(inst, ASIC_DEST_PORT_HIF(port), 
                             ASIC_DEST_PORT_HIF((port+1)%32), port);
                    } /* port +1 < ASIC(HI_COUNT) */
                } else {
                    if ((port+3) < ASIC(HI_COUNT)) {
		    rc = asic_config_nif_to_hif(inst, ASIC_DEST_PORT_HIF(port), 
                         ASIC_DEST_PORT_HIF((port+3)&0x1F), port);
                    } else {
		    rc = asic_config_nif_to_hif(inst, ASIC_DEST_PORT_HIF(port), 
                         ASIC_DEST_PORT_HIF((port+3)%32), port);
                    }
                }
		if (rc) return (rc);
	}
	// HIF to NIF
	for (port = 0; port < ASIC(HI_COUNT); port++) {
		if (!(bp_mask & (0x01 << port))) continue;
                if (port & 0x1) {
                    rc = hi_pkt_test_cfg(inst, port, (ASIC_DEST_PORT_NIF(port))-1);
                } else {
                    rc = hi_pkt_test_cfg(inst, port, (ASIC_DEST_PORT_NIF(port))+1);
                }
                if (rc) return (rc);

	}
	return (0);
}

int asic_snt_cfg(uint8_t inst, uint32_t fp_mask, uint32_t bp_mask, 
		 uint8_t sport, uint8_t eport, uint8_t nif_pkt_src)
{
	int port, rc = 0;

	// NIF to HIF
	rc = asic_config_nif_to_drop(inst);
	if (rc) return (rc);

	for (port = 0; port < ASIC(NI_COUNT); port++) {
		if (!(fp_mask & (0x01 << port))) continue;
                rc = ni_pkt_test_cfg(inst, port);
                if (rc) return (rc);
	}

	for (port = sport; port <= eport ; port++) {
		// Program the LookUp to send packets to HIF.
		rc = asic_config_nif_to_hif(inst, ASIC_DEST_PORT_HIF(port), 
				(port == eport)? 
					ASIC_DEST_PORT_HIF(sport) :
					ASIC_DEST_PORT_HIF((port+1)&0x1F), port);
		if (rc) return (rc);
	}


	// HIF to NIF
	for (port = sport; port <= eport; port++) {
		if (!(bp_mask & (0x01 << port))) continue;

                rc = hi_pkt_test_cfg(inst, port, (ASIC_DEST_PORT_NIF(port)));
                if (rc) return (rc);
        }
        return (0);
}

int asic_ext_port_port_snt_cfg(uint8_t inst, uint32_t fp_mask, uint32_t bp_mask, 
		 uint8_t sport, uint8_t eport, uint8_t nif_pkt_src)
{
	int port, rc = 0;

	// NIF to HIF
	rc = asic_config_nif_to_drop(inst);
	if (rc) return (rc);

	for (port = 0; port < ASIC(NI_COUNT); port++) {
		if (!(fp_mask & (0x01 << port))) continue;
                rc = ni_pkt_test_cfg(inst, port);
                if (rc) return (rc);
	}

	for (port = sport; port <= eport ; port++) {
		// Program the LookUp to send packets to HIF.
                if (port & 0x1) {
		    rc = asic_config_nif_to_hif(inst, ASIC_DEST_PORT_HIF(port), 
			 /* ASIC_DEST_PORT_HIF((port+1)&eport), port); */
			((port == eport)?  ASIC_DEST_PORT_HIF(sport) : 
                                   ASIC_DEST_PORT_HIF((port+1)&eport)), port);
                } else {
		    rc = asic_config_nif_to_hif(inst, ASIC_DEST_PORT_HIF(port), 
		/*	 ASIC_DEST_PORT_HIF((port+3)&eport), port); */
			((port +3 > eport)?  ASIC_DEST_PORT_HIF(sport)+1 : 
                                   ASIC_DEST_PORT_HIF((port+3)&0x1F)), port);
                }
		if (rc) return (rc);
	}


	// HIF to NIF
	for (port = sport; port <= eport; port++) {
		if (!(bp_mask & (0x01 << port))) continue;

                if (port & 0x1) {
                    rc = hi_pkt_test_cfg(inst, port, (ASIC_DEST_PORT_NIF(port))-1);
                } else {
                    rc = hi_pkt_test_cfg(inst, port, (ASIC_DEST_PORT_NIF(port))+1);
                }
                if (rc) return (rc);
        }
        if (nif_pkt_src) {
            rc = hi_pkt_test_cfg(inst, 9, 2);
            rc = hi_pkt_test_cfg(inst, 17, 4);
            rc = hi_pkt_test_cfg(inst, 25, 6);
            rc = hi_pkt_test_cfg(inst, 8, 3);
            rc = hi_pkt_test_cfg(inst, 16, 5);
            rc = hi_pkt_test_cfg(inst, 24, 7);
            rc = asic_config_nif_to_hif(inst, 17, 18, 5);
            rc = asic_config_nif_to_hif(inst, 25, 26, 7);
            rc = asic_config_nif_to_hif(inst, 16, 19, 4);
            rc = asic_config_nif_to_hif(inst, 24, 27, 6);
        }
        return (0);
}
