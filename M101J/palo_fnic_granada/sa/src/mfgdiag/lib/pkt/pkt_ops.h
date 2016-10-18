/********************************************************************
 *
 *      File:   pkt_ops.h
 *      Name:   Sudharshan Kadari
 *
 *      Description:
 *       Packet lib 
 *
 *
 * Copyright (c) 1985-2007 by Cisco Systems, Inc.
 * All rights reserved.
 *
 *
 *********************************************************************/

#ifndef _PKT_OPS_H_
#define _PKT_OPS_H_

#ifdef DIAG_FEX
   #define MV_SW_INTF	"eth0"
   #define MV_RDW_INTF	"eth0"
#else
#ifdef DIAG_SIM
   #define MV_SW_INTF	"eth1"
   #define MV_RDW_INTF	"eth0"
#else
   #define MV_SW_INTF	"eth0"
   #define MV_RDW_INTF	"eth1"
#endif
#endif

#define MV_RDW_PIO	"pio"

#define CPU_WOO_INTF	"eth0"

#define DIAG_MAX_PACKET_SIZE	1600

extern uint64_t random_generate_40bit_lfsr (uint64_t data);
extern uint8_t pkt_data_generate (const uint8_t *mode, uint8_t data,
                           uint8_t inc);
extern uint32_t pkt_data_generate32 (const uint8_t *mode, uint32_t data,
                           uint32_t inc);
extern int pkt_tx(int fd, int inst,int pkt_size, uint32_t data, uint32_t incr,
           uint8_t *mode, uint8_t type, uint32_t vntag, 
	   uint16_t vlan_tag, int cnt, uint32_t dhs_base);
extern int pkt_rx(int fd, uint8_t *rx_pkt, int pkt_size);
extern void print_pkt(uint8_t *pkt, int pkt_size);
extern int pkt_rx_eth1(uint8_t *rx_pkt, uint32_t psize);
extern int pkt_rx_eth0(uint8_t *rx_pkt, uint32_t psize);



extern int  network_rawsocket_init(const char *int_name);
extern int  network_recv_data(int s, uint8_t *data, size_t *datasize);
extern void network_send_out(int s, void *data, size_t datalen, int fc_sof, int fc_eof);
extern void network_append_crc_32(uint8_t *data, size_t datalen);
extern int network_get_inband_mac_address(uint8_t *mac_addr, char *ifname);


extern int diag_get_first_last_bits_32 (uint32_t mask, uint8_t *first, uint8_t *last);
extern int diag_get_next_bit_32 (uint32_t mask, uint8_t cur_bit, uint8_t *next); 

// Packet queue operations.
extern int pkt_enqueue(int intf_num, uint8_t *pkt, uint32_t len);
extern int pkt_dequeue_head(int intf_num, int all);
extern int pkt_queue_count(int intf_num);
extern void pkt_queue_dump (int intf_num);
extern void pkt_queue_flush (int intf_num);
extern int pkt_rx_proc_init(int intf_num);
extern int pkt_queue_recv (int intf_num);
extern int pkt_shm_rd(char *fname, uint8_t *pkt, uint32_t pcnt, 
        uint32_t max_len);
extern int pkt_shm_dump(char *fname);
extern int pkt_shm_rm(char *fname);

extern uint8_t *diag_get_mac_addr_dst();
extern uint8_t *diag_get_mac_addr_src();
extern void diag_set_mac_addr_dst(uint8_t *mac);
extern void diag_set_mac_addr_src(uint8_t *mac);


#endif // _PKT_OPS_H_
