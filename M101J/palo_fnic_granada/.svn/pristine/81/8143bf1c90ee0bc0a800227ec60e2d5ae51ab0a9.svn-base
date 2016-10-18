/********************************************************************
 *
 *      File:   pkt_que_fext.c
 *      Name:   Raghu Boreda
 *      based on file pkt_que.c
 *
 *      Description:
 *       Receive packets from the intf. 
 *
 *
 * Copyright (c) 1985-2007 by Cisco Systems, Inc.
 * All rights reserved.
 *
 *
 *********************************************************************/

#include <stdio.h>
#include <stdint.h> 
#include <string.h> 
#include <strings.h> 
#include <malloc.h> 
#include <sys/types.h> 
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h> 
#include <errno.h> 
#include <fcntl.h> 
#include "pkt_ge.h"
#include "pkt_ops.h"
#include <sched.h>
#include <sys/ioctl.h>

int pkt_belongs_to_diag (uint8_t *r_mac, uint8_t *s_mac)
{
	return ((0xFFFFFFFFFFFF0000ull & *(uint64_t *)s_mac) == 
	   (0xFFFFFFFFFFFF0000ull & *(uint64_t*)r_mac)) ? 1 : 0;
}

static int pkt_rx_eth(uint8_t intf_num, uint8_t *rx_pkt, uint32_t psize)
{
        int raw_s, rc = 0;

	raw_s = network_rawsocket_init(intf_num ? "eth1" : "eth0");
	if (raw_s < 0) {
		printf("Error: Failed to initialize the socket\n");
	}
	rc = network_recv_data(raw_s, rx_pkt, &psize);
#if 0
		if (pkt_belongs_to_diag(rxpkt+6, s_mac)) {
			if ((psize >= 64) && (psize < DIAG_MAX_PACKET_SIZE)) {
#endif
	if (rc) {
		free(rx_pkt);
		printf("Error: Failed to receive packet\n");
		return (rc);
	} 
        return (psize);

}

int pkt_rx_eth1(uint8_t *rx_pkt, uint32_t psize)
{
	return (pkt_rx_eth(1, rx_pkt, psize));
}
int pkt_rx_eth0(uint8_t *rx_pkt, uint32_t psize)
{
	return (pkt_rx_eth(0, rx_pkt, psize));
}

int pkt_rx_proc_init(int intf_num)
{
	return (0);
}
