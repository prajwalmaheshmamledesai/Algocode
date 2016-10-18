/********************************************************************
 *
 *      File:   pkt_ge.h 
 *      Name:   Sudharshan Kadari
 *
 *      Description:
 *       IP Packet generation header file 
 *
 *
 * Copyright (c) 1985-2007 by Cisco Systems, Inc.
 * All rights reserved.
 *
 *
 *********************************************************************/

#ifndef _GE_PKT_H_
#define _GE_PHT_H_

#define MAC_ADDRESS_SIZE    6
#define MAC_HEADER_SIZE     12               /* (2*MAC_ADDRESS_SIZE) */ 

#define IPV4_ARPA_ETHER     0x0800
#define IPV4_ARPA_ETHER_HI  0x08
#define IPV4_ARPA_ETHER_LO  0x00

#define IPV4_SAP_DSAP       0x06
#define IPV4_SAP_SSAP       0x06
#define IPV4_SAP_CONTROL    0x03

#define IPV4_SNAP_DSAP      0xAA
#define IPV4_SNAP_SSAP      0xAA
#define IPV4_SNAP_CONTROL   0x03

#define IPV4_SNAP_ETHER     0x0800
#define IPV4_SNAP_ETHER_HI  0x08
#define IPV4_SNAP_ETHER_LO  0x00

#define IPV4_SNAP_ORG_0     0x00
#define IPV4_SNAP_ORG_1     0x00
#define IPV4_SNAP_ORG_2     0x00

#define IPV4_HEADER_SIZE    0x14
#define IPV4_IDENTIFICATION 0x0001
#define IPV4_VERSION        0x04

#define IPV4_L4_PORT_SIZE   0x04
#define IPV4_UDP_PROTOCOL   0x11
#define IPV4_TCP_PROTOCOL   0x06
#define IPV4_ICMP_PROTOCOL  0x01
#define IPV4_IGMP_PROTOCOL  0x02

#define IPV4_VLAN_TAG       0x8100
#define IPV4_VLAN_TAG_HI    0x81
#define IPV4_VLAN_TAG_LO    0x00
#define IPV4_VLAN_TAG_SIZE  0x04

#define IPV4_VNTAG	    0x564E 
#define IPV4_VNTAG_HI	    0x56
#define IPV4_VNTAG_LO	    0x4E
#define IPV4_VNTAG_SIZE     0x06

typedef enum
{
   UNKNOWN_ENCAP  = 0,
   ARPA_ENCAP     = 1,
   SAP_ENCAP      = 2,
   SNAP_ENCAP     = 3,
   RAW_ENCAP      = 4
} encap_type_t;

typedef enum
{
   VLAN_TAG  = 0,
   VNTAG     = 1,
} tag_type_t;

typedef struct
{
   uint8_t  dst[MAC_ADDRESS_SIZE];
   uint8_t  src[MAC_ADDRESS_SIZE];
} mac_header_t;

typedef struct
{
   uint32_t tagpid   : 16; 
   uint32_t prior    : 3; 
   uint32_t tokring  : 1; 
   uint32_t vlan     : 12; 
}__attribute__((packed)) vlan_tag_t;

typedef struct
{
   uint8_t    length_hi; 
   uint8_t    length_lo;
   uint8_t    dsap;
   uint8_t    ssap;
   uint8_t    control;
} __attribute__((packed)) sap_encap_t;

typedef struct
{
   uint8_t   length_hi;
   uint8_t   length_lo;
   uint8_t   dsap;
   uint8_t   ssap;
   uint8_t   control;
   uint8_t   organization[3];
   uint8_t   ether_type_hi;
   uint8_t   ether_type_lo;
} __attribute__((packed)) snap_encap_t;

typedef struct
{
   uint16_t ethertype;
} __attribute__((packed)) arpa_encap_t;


typedef struct
{
   uint16_t type;
} __attribute__((packed)) raw_encap_t;


typedef struct
{
   uint8_t  version          : 4;
   uint8_t  header_length    : 4;
   uint8_t  type_of_service  : 8;
   uint16_t total_length     : 16;
   
   uint16_t identification   : 16;
   uint16_t flags            : 3;
   uint16_t fragment_offset  : 13;

   uint8_t  time_to_live     : 8;
   uint8_t  protocol         : 8;
   uint16_t header_check_sum : 16;

   uint32_t source_ip        : 32;
   uint32_t destination_ip   : 32;

   uint32_t l4_src_port      : 16;
   uint32_t l4_dst_port      : 16;  
} __attribute__((packed)) ip_header_t;


extern uint32_t generate_ipv4_pkt (uint8_t *pkt, uint32_t len, void *info);

typedef struct _ip_info_
{
        uint32_t        source;
        uint32_t        destination;
        uint32_t        total_length;
        uint32_t        protocol;
        uint32_t        encap_type;
        uint32_t        tag_type;
        char*           mode;
        uint32_t        incr;
        uint32_t        data;
        mac_header_t    *mac_hdr;
        uint16_t        l4_src_port;
        uint16_t        l4_dst_port;
        uint32_t        option_length;
        uint16_t        vlan;
        uint32_t        vntag;
        char*           options;
} ip_info_t;

#define MK_VNTAG(dir, mcast, loop, dst, src)	\
	(((dir)&1) << 31) | (((mcast)&1) << 30) | 	\
	(((dst)&0x3FFF) << 16) | (((loop)&1) << 15) | (((src)&0xfff))

#endif /* _GE_PHT_H_ */
