/********************************************************************
 *
 *      File:   redwood_portmap.c
 *      Name:   Sudharshan Kadari 
 *
 *      Description: PortMap Definitions. 
 *
 *
 *      Copyright (c) 2001,2002,2003,2004 by Nuova Systems, Inc.
 ********************************************************************/
#include <stdio.h>
#include <sys/types.h>
#include "redwood_portmap.h"

//Redwood 0
int nif_tx_pol[4] = {0, 0, 0, 0};
int nif_rx_pol[4] = {0, 0, 0, 0};
int hif_rx_pol[8] = {1, 0, 1, 0, 1, 1, 0, 0};
int hif_tx_pol[8] = {0, 0, 0, 0, 1, 0, 0, 0};

mac_cfg_t nif_cfg[4] = {
                { -1, 9216, 0xdc, LANE_MAP(3,2,1,0), LANE_MAP(3,2,1,0), 0, 0, 0xc, 0, 1, 0, 0 },
                { -1, 9216, 0xdc, LANE_MAP(3,2,1,0), LANE_MAP(3,2,1,0), 0, 0, 0xc, 0, 1, 0, 0 },
                { -1, 9216, 0xdc, LANE_MAP(3,2,1,0), LANE_MAP(3,2,1,0), 0, 0, 0xc, 0, 1, 0, 0 },
                { -1, 9216, 0xdc, LANE_MAP(3,2,1,0), LANE_MAP(3,2,1,0), 0, 0, 0xc, 0, 1, 0, 0 },
                };
                           /*    MTU  PAUSE  RxLnMap           ipg             */
mac_cfg_t hif_cfg[8] = {
                { -1, 9216, 0xdc, LANE_MAP(3,2,1,0), LANE_MAP(3,2,1,0), 0, 0, 0xc, 0, 1, 0, 0 },
                { -1, 9216, 0xdc, LANE_MAP(3,2,1,0), LANE_MAP(3,2,1,0), 0, 0, 0xc, 0, 1, 0, 0 },
                { -1, 9216, 0xdc, LANE_MAP(3,2,1,0), LANE_MAP(3,2,1,0), 0, 0, 0xc, 0, 1, 0, 0 },
                { -1, 9216, 0xdc, LANE_MAP(3,2,1,0), LANE_MAP(3,2,1,0), 0, 0, 0xc, 0, 1, 0, 0 },
                { -1, 9216, 0xdc, LANE_MAP(3,2,1,0), LANE_MAP(3,2,1,0), 0, 0, 0xc, 0, 1, 0, 0 },
                { -1, 9216, 0xdc, LANE_MAP(3,2,1,0), LANE_MAP(3,2,1,0), 0, 0, 0xc, 0, 1, 0, 0 },
                { -1, 9216, 0xdc, LANE_MAP(3,2,1,0), LANE_MAP(3,2,1,0), 0, 0, 0xc, 0, 1, 0, 0 },
                { -1, 9216, 0xdc, LANE_MAP(3,2,1,0), LANE_MAP(3,2,1,0), 0, 0, 0xc, 0, 1, 0, 0 },
                };

//Redwood 3
int hif_rx_pol_3[8] = {1, 0, 1, 0, 1, 0, 0, 0};

// Redwood 6
int nif_rx_pol_6[4] = {4, 5, 4, 7};
int nif_tx_pol_6[4] = {9, 2, 3, 1};
int hif_rx_pol_6[8] = {3, 3, 5, 9, 0, 0, 0xC, 3};
int hif_tx_pol_6[8] = {0xE, 0, 0xA, 0, 8, 1, 0, 0};

mac_cfg_t nif_cfg_6[4] = {
                { -1, 9216, 0xdc, LANE_MAP(2,1,3,0), LANE_MAP(2,3,1,0), 0, 0, 0xc, 0, 1, 0, 0 },
                { -1, 9216, 0xdc, LANE_MAP(1,2,3,0), LANE_MAP(3,2,0,1), 0, 0, 0xc, 0, 1, 0, 0 },
                { -1, 9216, 0xdc, LANE_MAP(2,1,3,0), LANE_MAP(3,1,0,2), 0, 0, 0xc, 0, 1, 0, 0 },
                { -1, 9216, 0xdc, LANE_MAP(0,2,3,1), LANE_MAP(3,2,1,0), 0, 0, 0xc, 0, 1, 0, 0 },
                };
                           /*    MTU  PAUSE  RxLnMap           ipg             */
mac_cfg_t hif_cfg_6[8] = {
                { -1, 9216, 0xdc, LANE_MAP(3,2,1,0), LANE_MAP(0,1,3,2), 0, 0, 0xc, 0, 1, 0, 0 },
                { -1, 9216, 0xdc, LANE_MAP(3,2,1,0), LANE_MAP(0,1,3,2), 0, 0, 0xc, 0, 1, 0, 0 },
                { -1, 9216, 0xdc, LANE_MAP(0,2,3,1), LANE_MAP(0,2,3,1), 0, 0, 0xc, 0, 1, 0, 0 },
                { -1, 9216, 0xdc, LANE_MAP(2,1,0,3), LANE_MAP(0,3,1,2), 0, 0, 0xc, 0, 1, 0, 0 },
                { -1, 9216, 0xdc, LANE_MAP(3,1,2,0), LANE_MAP(0,3,1,2), 0, 0, 0xc, 0, 1, 0, 0 },
                { -1, 9216, 0xdc, LANE_MAP(3,2,0,1), LANE_MAP(2,1,0,3), 0, 0, 0xc, 0, 1, 0, 0 },
                { -1, 9216, 0xdc, LANE_MAP(0,1,2,3), LANE_MAP(0,2,1,3), 0, 0, 0xc, 0, 1, 0, 0 },
                { -1, 9216, 0xdc, LANE_MAP(3,2,1,0), LANE_MAP(3,1,0,2), 0, 0, 0xc, 0, 1, 0, 0 },
                };


rdw_pmap_t redwood_port_map[] = {
			{nif_tx_pol, nif_rx_pol, hif_tx_pol, hif_rx_pol, nif_cfg, hif_cfg},
			{nif_tx_pol, nif_rx_pol, hif_tx_pol, hif_rx_pol, nif_cfg, hif_cfg},
			{nif_tx_pol, nif_rx_pol, hif_tx_pol, hif_rx_pol, nif_cfg, hif_cfg},
			{nif_tx_pol, nif_rx_pol, hif_tx_pol, hif_rx_pol_3, nif_cfg, hif_cfg},
			{nif_tx_pol, nif_rx_pol, hif_tx_pol, hif_rx_pol, nif_cfg, hif_cfg},
			{nif_tx_pol, nif_rx_pol, hif_tx_pol, hif_rx_pol, nif_cfg, hif_cfg},
			{nif_tx_pol_6, nif_rx_pol_6, hif_tx_pol_6, hif_rx_pol_6, nif_cfg_6, hif_cfg_6},
		};


int hif_rx_pol_6_p1[8] = {3, 3, 0xF, 3, 3, 0xA, 0x0, 3};
int hif_tx_pol_6_p1[8] = {0xE, 0, 0xD, 0, 8, 1, 0, 0};

mac_cfg_t hif_cfg_6_p1[8] = {
                { -1, 9216, 0xdc, LANE_MAP(3,2,1,0), LANE_MAP(0,1,3,2), 0, 0, 0xc, 0, 1, 0, 0 },
                { -1, 9216, 0xdc, LANE_MAP(3,2,1,0), LANE_MAP(0,3,1,2), 0, 0, 0xc, 0, 1, 0, 0 },
                { -1, 9216, 0xdc, LANE_MAP(0,2,1,3), LANE_MAP(0,3,2,1), 0, 0, 0xc, 0, 1, 0, 0 },
                { -1, 9216, 0xdc, LANE_MAP(3,2,0,1), LANE_MAP(0,3,1,2), 0, 0, 0xc, 0, 1, 0, 0 },
                { -1, 9216, 0xdc, LANE_MAP(3,2,1,0), LANE_MAP(0,2,1,3), 0, 0, 0xc, 0, 1, 0, 0 },
                { -1, 9216, 0xdc, LANE_MAP(0,2,1,3), LANE_MAP(2,1,3,0), 0, 0, 0xc, 0, 1, 0, 0 },
                { -1, 9216, 0xdc, LANE_MAP(1,2,0,3), LANE_MAP(2,1,0,3), 0, 0, 0xc, 0, 1, 0, 0 },
                { -1, 9216, 0xdc, LANE_MAP(3,2,1,0), LANE_MAP(3,1,0,2), 0, 0, 0xc, 0, 1, 0, 0 },
                };

rdw_pmap_t redwood_port_map_p1[] = {
			{nif_tx_pol, nif_rx_pol, hif_tx_pol, hif_rx_pol, nif_cfg, hif_cfg},
			{nif_tx_pol, nif_rx_pol, hif_tx_pol, hif_rx_pol, nif_cfg, hif_cfg},
			{nif_tx_pol, nif_rx_pol, hif_tx_pol, hif_rx_pol, nif_cfg, hif_cfg},
			{nif_tx_pol, nif_rx_pol, hif_tx_pol, hif_rx_pol_3, nif_cfg, hif_cfg},
			{nif_tx_pol, nif_rx_pol, hif_tx_pol, hif_rx_pol, nif_cfg, hif_cfg},
			{nif_tx_pol, nif_rx_pol, hif_tx_pol, hif_rx_pol, nif_cfg, hif_cfg},
			{nif_tx_pol_6, nif_rx_pol_6, hif_tx_pol_6_p1, hif_rx_pol_6_p1, nif_cfg_6, hif_cfg_6_p1},
		};

extern int diag_board_revision_get ();
int redwood_get_port_information(uint8_t asic_id, rdw_pmap_t **prdw)
{
	
	if (asic_id > 6) {
		*prdw = NULL;
		return (-1);
	}

	if (diag_board_revision_get() == 2) {
		*prdw = &redwood_port_map_p1[asic_id];
	} else if (diag_board_revision_get() == 1) {
		*prdw = &redwood_port_map[asic_id];
	} else {
		// Default to P1 boards for now.
		*prdw = &redwood_port_map_p1[asic_id];
	}

	return (0);
}

int redwood_lanemap_dump(uint8_t asic_id)
{
	rdw_pmap_t *prdw = NULL;
	int port;
	
	if (asic_id > 6) {
		return (-1);
	}

	if (diag_board_revision_get() == 2) {
		prdw = &redwood_port_map_p1[asic_id];
	} else if (diag_board_revision_get() == 1) {
		prdw = &redwood_port_map[asic_id];
	} else {
		// Default to P1 boards for now.
		prdw = &redwood_port_map_p1[asic_id];
	}

	printf("\n----------------------------------------\n");
	printf("          REDWOOD %1d LANEMAP           \n", asic_id);
	printf("----------------------------------------\n");
	printf("\nNIF : TxLnPol  RxLnPol  RxLnMap  RxLnMap\n");
	for(port = 0; port < 4; port++) {
		printf(" %02d : %1d,%1d,%1d,%1d  %1d,%1d,%1d,%1d  %d,%d,%d,%d  %d,%d,%d,%d\n", port,
			(prdw->nif_tx_pol[port]>>3)&1, 
			(prdw->nif_tx_pol[port]>>2)&1, 
			(prdw->nif_tx_pol[port]>>1)&1, 
			(prdw->nif_tx_pol[port]>>0)&1, 
			(prdw->nif_rx_pol[port]>>3)&1, 
			(prdw->nif_rx_pol[port]>>2)&1, 
			(prdw->nif_rx_pol[port]>>1)&1, 
			(prdw->nif_rx_pol[port]>>0)&1, 
			(prdw->nif_cfg[port].rx_lane_map >> 6)& 0x3,
			(prdw->nif_cfg[port].rx_lane_map >> 4)& 0x3,
			(prdw->nif_cfg[port].rx_lane_map >> 2)& 0x3,
			(prdw->nif_cfg[port].rx_lane_map >> 0)& 0x3,
			(prdw->nif_cfg[port].tx_lane_map >> 6)& 0x3,
			(prdw->nif_cfg[port].tx_lane_map >> 4)& 0x3,
			(prdw->nif_cfg[port].tx_lane_map >> 2)& 0x3,
			(prdw->nif_cfg[port].tx_lane_map >> 0)& 0x3);
	}

	printf("\nHIF : TxLnPol  RxLnPol  RxLnMap  RxLnMap\n");
	for(port = 0; port < 8; port++) {
		printf(" %02d : %1d,%1d,%1d,%1d  %1d,%1d,%1d,%1d  %d,%d,%d,%d  %d,%d,%d,%d\n", port,
			(prdw->hif_tx_pol[port]>>3)&1, 
			(prdw->hif_tx_pol[port]>>2)&1, 
			(prdw->hif_tx_pol[port]>>1)&1, 
			(prdw->hif_tx_pol[port]>>0)&1, 
			(prdw->hif_rx_pol[port]>>3)&1, 
			(prdw->hif_rx_pol[port]>>2)&1, 
			(prdw->hif_rx_pol[port]>>1)&1, 
			(prdw->hif_rx_pol[port]>>0)&1, 
			(prdw->hif_cfg[port].rx_lane_map >> 6)& 0x3,
			(prdw->hif_cfg[port].rx_lane_map >> 4)& 0x3,
			(prdw->hif_cfg[port].rx_lane_map >> 2)& 0x3,
			(prdw->hif_cfg[port].rx_lane_map >> 0)& 0x3,
			(prdw->hif_cfg[port].tx_lane_map >> 6)& 0x3,
			(prdw->hif_cfg[port].tx_lane_map >> 4)& 0x3,
			(prdw->hif_cfg[port].tx_lane_map >> 2)& 0x3,
			(prdw->hif_cfg[port].tx_lane_map >> 0)& 0x3);
	}
	return (0);
}


int redwood_get_port_speed(uint8_t asic_id, uint8_t port_type, uint8_t port_no, uint8_t *speed)
{
	switch (port_type) {
		case	PORT_TYPE_HIF:
			*speed =  (asic_id < 6) ?  MEDIA_TYPE_1000BASET : MEDIA_TYPE_SFPPLUS;
			break;

		case	PORT_TYPE_NIF:
			*speed =  MEDIA_TYPE_SFPPLUS;
			break;

		case	PORT_TYPE_CIF:
		case	PORT_TYPE_BIF:
		default:
			*speed =  MEDIA_TYPE_100BASET;
			break;
	}
	return (0);
}

int redwood_asic_port_to_front_port (uint8_t asic_id, uint8_t asic_port)
{
	return (asic_port);
}
