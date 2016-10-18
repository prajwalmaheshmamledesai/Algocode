/********************************************************************
 *
 *      File:   pkt_ops.c
 *      Name:   Sudharshan Kadari
 *
 *      Description:
 *       Generic Packet operation library 
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
#include <unistd.h> 
#include "pkt_ge.h"
#include "pkt_ops.h"

#define BIT(x)	(0x01ull << (x))

extern int pkt_ipv4_generate(uint8_t *tx_pkt, uint32_t pkt_size, ip_info_t *ip_info);

/**
 * random_generate_40bit_lfsr - use LFSR to return a random number. 
 * @data: seed data.
 */
uint64_t random_generate_40bit_lfsr (uint64_t data)
{  
	uint64_t new_data;
	uint8_t  bit39, bit37, bit20, bit18, bit0;
   
	new_data = (data << 1); 
   
	bit39 = (BIT(39) & data) ? 1 : 0;
	bit37 = (BIT(37) & data) ? 1 : 0;
	bit20 = (BIT(20) & data) ? 1 : 0;
	bit18 = (BIT(18) & data) ? 1 : 0;

	bit0 = bit39 ^ bit37 ^ bit20 ^ bit18;
	new_data |= bit0;
   
	return (new_data & 0xFFFFFFFFFFull);
}  

/**
 * random_generate_8bit_lfsr - use LFSR to return a random number.
 * @data: seed data.
 */
static uint8_t random_generate_8bit_lfsr (uint8_t data)
{
	uint16_t new_data;
	uint8_t  bit7, bit5, bit4, bit3, bit0;

	new_data = (data << 1);

	bit7 = (BIT(7) & data) ? 1 : 0;
	bit5 = (BIT(5) & data) ? 1 : 0;
	bit4 = (BIT(4) & data) ? 1 : 0;
	bit3 = (BIT(3) & data) ? 1 : 0;

	bit0 = bit7 ^ bit5 ^ bit4 ^ bit3;
	new_data |= bit0;

	return (new_data & 0xFF);
}

/**
 * random_generate_32bit_lfsr - use LFSR to return a random number.
 * @data: seed data.
 */
static uint32_t random_generate_32bit_lfsr (uint32_t data)
{
   uint64_t new_data;
   uint8_t  bit31, bit21, bit1, bit0, lbit;

   new_data = (data << 1);

   bit31 = (BIT(31) & data) ? 1 : 0;
   bit21 = (BIT(21) & data) ? 1 : 0;
   bit1  = (BIT(1) & data) ? 1 : 0;
   bit0  = (BIT(0) & data) ? 1 : 0;

   lbit = bit31 ^ bit21 ^ bit1 ^ bit0;
   new_data |= lbit;

   return ((uint32_t)new_data);
}

/**
 * diag_get_first_last_bits 
 */
int diag_get_first_last_bits_32 (uint32_t mask, uint8_t *first, uint8_t *last)
{
	int flag = 0, cnt = 0;	

	while(mask) {
		if (mask & 0x01) {
			if (!flag) {
				*first = cnt;
				*last = cnt;
				flag = 1;
			} else {
				*last = cnt;
			}	
		}	
		mask >>= 1;
		cnt++;
	}

	return (flag ? 0 : -1);
}

/**
 * diag_get_next_bit (uint32_t mask, uint8_t cur_bit, uint8_t *next)  
 */
int diag_get_next_bit_32 (uint32_t mask, uint8_t cur_bit, uint8_t *next)  
{
	int cnt = 0;	
	uint32_t bmask = 0x01 << (cur_bit+1);

	for (cnt = cur_bit+1; cnt < 32; cnt++) {
		if (mask & bmask) {
			*next = cnt;
			return (0);
		}
		bmask <<= 1;
	}

	return (-1);
}

/**
 * pkt_data_generate - Generate Next Data
 * @mode: Mode of data increment.                                                       
 * @data: Seed Data.
 * @inc: increment.
 *
 * This function computes the new data based on the mode 
 * passed in.                                             
 */

uint8_t pkt_data_generate (const uint8_t *mode, uint8_t data,
                           uint8_t inc)
{
	uint8_t newdata;

	switch (*mode)
	{
		case 'I':
		case 'i':
			newdata = data + inc;
			break; 
		case 'C':
		case 'c':
			newdata = ~data;
			break;

		case 'R':
		case 'r':
			newdata = random_generate_8bit_lfsr(data); 
			break;

		default:
			newdata = data;
			break;
	}

	return (newdata);
}

/**
 * pkt_data_generate32 - Generate Next Data
 * @mode: Mode of data increment.                                                       
 * @data: Seed Data.
 * @inc: increment.
 *
 * This function computes the new data based on the mode 
 * passed in.                                             
 */

uint32_t pkt_data_generate32 (const uint8_t *mode, uint32_t data,
                           uint32_t inc)
{
	uint32_t newdata;

	switch (*mode)
	{
		case 'I':
		case 'i':
			newdata = data + inc;
			break;

		case 'C':
		case 'c':
			newdata = ~data;
			break;

		case 'R':
		case 'r':
			newdata = random_generate_32bit_lfsr(data); 
			break;

		default:
			newdata = data;
			break;
	}

	return (newdata);
}

/*
ARPA Packet
   00 01 0e d2 00 00 00 01 0e d2 00 02 08 00 45 00
   00 32 00 01 00 00 ff 11 ba 34 00 c2 00 02 00 c2
   00 00 00 02 00 00 55 55 55 55 55 55 55 55 55 55
   55 55 55 55 55 55 55 55 55 55 55 55 55 55 55 55

SAP Packet

   00 01 0e d2 00 00 00 01 0e d2 00 02 00 32 06 06
   03 45 00 00 2f 00 01 00 00 ff 11 ba 37 00 c2 00
   02 00 c2 00 00 00 02 00 00 55 55 55 55 55 55 55
   55 55 55 55 55 55 55 55 55 55 55 55 55 55 55 55


SNAP Packet

   00 01 0e d2 00 00 00 01 0e d2 00 02 00 32 aa aa
   03 00 00 00 08 00 45 00 00 2a 00 01 00 00 ff 11
   ba 3c 00 c2 00 02 00 c2 00 00 00 02 00 00 55 55
   55 55 55 55 55 55 55 55 55 55 55 55 55 55 55 55

*/
extern int diag_debug;
void print_pkt(uint8_t *pkt, int pkt_size)
{
	int cnt;
	if (diag_debug) {
		for (cnt = 0; cnt < pkt_size; cnt++) {
			if (!(cnt & 0x0F))
				printf("\n");
			printf("%02x ", pkt[cnt]);
		}
	}
}

uint8_t mac_addr_dst[6] = {0x00, 0xAB, 0xCD, 0xEF, 0x12, 0x34};
uint8_t mac_addr_src[6] = {0x00, 0x04, 0x9F, 0x00, 0x39, 0x8E};

uint8_t *diag_get_mac_addr_dst()
{
	return (mac_addr_dst);
}

uint8_t *diag_get_mac_addr_src()
{
	return (mac_addr_src);
}
void diag_set_mac_addr_dst(uint8_t *mac)
{
	mac_addr_dst[0] = mac[0];
	mac_addr_dst[1] = mac[1];
	mac_addr_dst[2] = mac[2];
	mac_addr_dst[3] = mac[3];
	mac_addr_dst[4] = mac[4];
	mac_addr_dst[5] = mac[5];
}

void diag_set_mac_addr_src(uint8_t *mac)
{
	mac_addr_src[0] = mac[0];
	mac_addr_src[1] = mac[1];
	mac_addr_src[2] = mac[2];
	mac_addr_src[3] = mac[3];
	mac_addr_src[4] = mac[4];
	mac_addr_src[5] = mac[5];
}

static int diag_ge_pkt_gen (char *tx_pkt, int pkt_size, uint32_t data, 
		     uint32_t incr, uint8_t* mode, uint8_t tag_type,
			uint32_t vntag, uint16_t vlan_tag)
{
//static int tx_cnt = 0;
	int	rc = 0;
	mac_header_t mac_hdr;
	ip_info_t    ip_info;

	bzero (&mac_hdr, sizeof(mac_hdr));
	bzero (&ip_info, sizeof(ip_info));

//	network_get_inband_mac_address(mac_hdr.dst, "eth1");
//	network_get_inband_mac_address(mac_hdr.src, "eth1");
	mac_hdr.dst[0] = mac_addr_dst[0];
	mac_hdr.dst[1] = mac_addr_dst[1];
	mac_hdr.dst[2] = mac_addr_dst[2];
	mac_hdr.dst[3] = mac_addr_dst[3];
	mac_hdr.dst[4] = mac_addr_dst[4];
	mac_hdr.dst[5] = mac_addr_dst[5];

	mac_hdr.src[0] = mac_addr_src[0];
	mac_hdr.src[1] = mac_addr_src[1];
	mac_hdr.src[2] = mac_addr_src[2];
	mac_hdr.src[3] = mac_addr_src[3];
	mac_hdr.src[4] = mac_addr_src[4];
	mac_hdr.src[5] = mac_addr_src[5];

	ip_info.source		= 0xAC100CF7;
	ip_info.destination	= 0xAC100CF7;
	ip_info.total_length	= pkt_size;
	ip_info.protocol	= IPV4_UDP_PROTOCOL;
	ip_info.encap_type	= ARPA_ENCAP;
        ip_info.mode		= mode;
	ip_info.incr		= incr;
	ip_info.data		= data;
	ip_info.mac_hdr		= &mac_hdr;
	ip_info.l4_src_port	= 0x2222;
	ip_info.l4_dst_port	= 0x1111;
	ip_info.option_length	= 0;
//	ip_info.vlan		= 0xFC4;
	ip_info.vlan		= vlan_tag;
	ip_info.options		= NULL;
	ip_info.tag_type	= tag_type;
	ip_info.vntag		= vntag;

	pkt_ipv4_generate(tx_pkt, pkt_size, &ip_info);

	return (rc);
}
//#if defined(DIAG_FEXT) || defined (DIAG_SANDIEGO) || defined (DIAG_LA)
//#else
//extern int red_hw_frame_input_dhs_handler(int inst, 
//				  const unsigned int THE_DHS_ADDR
//                                , const unsigned char* PktPtr
//                                , const unsigned int PktSize
//                                , const unsigned int BPF //bytes per flit
//                                , const int is_big_endian // true if non-zero
//                                , const int inject_error
//                                );
//#endif
int pkt_tx(int raw_s, int inst, int pkt_size, uint32_t data, uint32_t incr,
	   uint8_t *mode, uint8_t tag_type, uint32_t vntag,
	   uint16_t vlan_tag, int pkt_cnt, uint32_t dhs_base)
{
	int cnt;
	char *tx_pkt = NULL;

//#ifndef DIAG_FEXT
//	uint8_t is_1g = 0;
//
//#ifdef DIAG_FEX
//	is_1g = (inst<6) ? 1 : 0;
//#endif
//#endif
	tx_pkt = malloc(pkt_size);
	if (!tx_pkt) {
		printf("Failed to Allocate tx pkt pkt_size %d\n", pkt_size);
		return (-1);
	}

	if ((diag_ge_pkt_gen (tx_pkt, pkt_size, data, incr, mode, tag_type, vntag, vlan_tag))) {
		printf("Failed to generate eth packet\n");
		free(tx_pkt);
		return (-1);
	}

	network_append_crc_32((uint8_t*)tx_pkt, pkt_size-4);

	for (cnt = 0; cnt < pkt_cnt; cnt++) {
		if (!dhs_base) {
			network_send_out( raw_s, tx_pkt, pkt_size, 0, 0 );
		} else {
//#if defined(DIAG_FEXT) || defined (DIAG_SANDIEGO) || defined (DIAG_LA)
//#else
//			red_hw_frame_input_dhs_handler(inst, dhs_base, tx_pkt, 
//				pkt_size, 8, is_1g? 0 : 1,0);
//#endif
		}
	}
	free(tx_pkt);

	return (0);
}
