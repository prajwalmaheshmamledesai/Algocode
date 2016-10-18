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
int nif_tx_pol[4] = {0, 3, 8, 4};
int nif_rx_pol[4] = {0, 0, 0, 0};
int hif_tx_pol[8] = {8, 2, 0, 0, 8, 7, 0, 0};
int hif_rx_pol[8] = {0, 0, 0, 0, 0, 0, 0, 0};


mac_cfg_t nif_cfg[4] = {
                { -1, 9216, 0xdc, LANE_MAP(3,2,1,0), LANE_MAP(3,2,1,0), 0, 0, 0xc, 0, 1, 0, 0 },
                { -1, 9216, 0xdc, LANE_MAP(3,2,1,0), LANE_MAP(3,2,1,0), 0, 0, 0xc, 0, 1, 0, 0 },
                { -1, 9216, 0xdc, LANE_MAP(3,2,1,0), LANE_MAP(3,2,1,0), 0, 0, 0xc, 0, 1, 0, 0 },
                { -1, 9216, 0xdc, LANE_MAP(3,2,1,0), LANE_MAP(3,2,1,0), 0, 0, 0xc, 0, 1, 0, 0 },
                };
                           /*    MTU  PAUSE  RxLnMap           ipg             */
mac_cfg_t hif_cfg[8] = {
                { -1, 9216, 0xdc, LANE_MAP(0,1,2,3), LANE_MAP(3,2,0,1), 0, 0, 0xc, 0, 1, 0, 0 },
                { -1, 9216, 0xdc, LANE_MAP(0,1,2,3), LANE_MAP(3,1,0,2), 0, 0, 0xc, 0, 1, 0, 0 },
                { -1, 9216, 0xdc, LANE_MAP(0,1,2,3), LANE_MAP(0,1,2,3), 0, 0, 0xc, 0, 1, 0, 0 },
                { -1, 9216, 0xdc, LANE_MAP(0,1,2,3), LANE_MAP(0,1,2,3), 0, 0, 0xc, 0, 1, 0, 0 },
                { -1, 9216, 0xdc, LANE_MAP(0,1,2,3), LANE_MAP(2,0,1,3), 0, 0, 0xc, 0, 1, 0, 0 },
                { -1, 9216, 0xdc, LANE_MAP(0,1,2,3), LANE_MAP(2,0,1,3), 0, 0, 0xc, 0, 1, 0, 0 },
                { -1, 9216, 0xdc, LANE_MAP(0,1,2,3), LANE_MAP(0,1,2,3), 0, 0, 0xc, 0, 1, 0, 0 },
                { -1, 9216, 0xdc, LANE_MAP(0,1,2,3), LANE_MAP(3,1,0,2), 0, 0, 0xc, 0, 1, 0, 0 },
                };

rdw_pmap_t redwood_port_map[] = {
			{nif_tx_pol, nif_rx_pol, hif_tx_pol, hif_rx_pol, nif_cfg, hif_cfg},
		};


int redwood_get_port_information(uint8_t asic_id, rdw_pmap_t **prdw)
{
	if (asic_id) {
		prdw = NULL;
		return (-1);
	}

	*prdw = &redwood_port_map[asic_id];
	return (0);
}

int redwood_get_port_speed(uint8_t asic_id, uint8_t port_type, uint8_t port_no, uint8_t *speed)
{
	if ((port_type == PORT_TYPE_HIF) ||
	    (port_type == PORT_TYPE_NIF))
		*speed = MEDIA_TYPE_SFPPLUS;
	else 
		*speed = MEDIA_TYPE_100BASET;
	return (0);
}


int redwood_lanemap_dump(uint8_t asic_id)
{
	rdw_pmap_t *prdw = &redwood_port_map[0];
	int port;
	
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
