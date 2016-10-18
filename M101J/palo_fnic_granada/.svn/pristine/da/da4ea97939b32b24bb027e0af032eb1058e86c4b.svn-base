/********************************************************************
 *
 *      File:   pkt_ge.c
 *      Name:   Sudharshan Kadari
 *
 *      Description:
 *       IP Packet generation code 
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
#include "pkt_ge.h"
#include "pkt_ops.h"

typedef uint8_t* (*PGEN_PTR)(uint8_t *, uint32_t *, void *);

uint8_t* append_vlan_tag     (uint8_t *pkt, uint32_t *rlen, void *hdr);

/**
 * append_mac_header - Append the Mac Header to Packet.
 * @pkt: pointer to the packet buffer. 
 * @remlen: remaining length of the packet.
 * @header: Pointer to Mac Header. 
 */

static uint8_t* append_mac_header (uint8_t *pkt, uint32_t *remlen, void *header)
{
   mac_header_t *mac = (mac_header_t *) header;

   memcpy(pkt, mac->dst, MAC_ADDRESS_SIZE);
   pkt += MAC_ADDRESS_SIZE;
   memcpy(pkt, mac->src, MAC_ADDRESS_SIZE);
   pkt += MAC_ADDRESS_SIZE;

   *remlen -= 2 * MAC_ADDRESS_SIZE;
   return (pkt);
}

/**
 * append_vlan_tag - Append VLAN Tag to the ethernet Packet.
 * @pkt: Pointer to Packet buffer.
 * @remlen: Pointer to remaining Length of Packet.
 * @vid: Holds the vlan Id.
 *
 * This function appends the VLAN TAG information to
 * the packet for IPV4packets. 
 *
 */
uint8_t* append_vlan_tag (uint8_t *pkt, uint32_t *remlen, void *vid)
{
   uint16_t vlan = *(uint16_t *)vid;

   *pkt++   = IPV4_VLAN_TAG_HI; 
   *pkt++   = IPV4_VLAN_TAG_LO; 
   *pkt++   = (vlan >> 8) & 0x0F ;
   *pkt++   = vlan & 0xFF;

   *remlen -= IPV4_VLAN_TAG_SIZE; 

   return (pkt);
}

uint8_t* append_vntag (uint8_t *pkt, uint32_t *remlen, void *ptag)
{
   uint32_t tag = *(uint32_t *)ptag;

   *pkt++   = IPV4_VNTAG_HI; 
   *pkt++   = IPV4_VNTAG_LO; 
   *pkt++   = (tag >> 24) & 0xFF ;
   *pkt++   = (tag >> 16) & 0xFF ;
   *pkt++   = (tag >> 8)  & 0xFF ;
   *pkt++   =  tag & 0xFF;

   *remlen -= IPV4_VNTAG_SIZE; 

   return (pkt);
}

/** 
 * append_arpa_encap - Append ARPA Encapsulation. 
 * @pkt: Pointer to Packet buffer.
 * @remlen: Pointer to remaining Length of Packet.
 * @h: Not Used. 
 *
 * This function appends the arpa encapsulation to 
 * the packet for IPV4-ARPA packets.
 *
 */

static uint8_t* append_arpa_encap (uint8_t *pkt, uint32_t *remlen, void *h)
{

   *pkt++   = IPV4_ARPA_ETHER_HI; 
   *pkt++   = IPV4_ARPA_ETHER_LO; 
   *remlen -= sizeof (uint16_t);

   return (pkt);
}

/**
 * append_sap_encap - Append SAP Encapsulation. 
 * @pkt: Pointer to Packet buffer.
 * @remlen: Pointer to remaining Length of Packet.
 * @h: Not Used. 
 * 
 * This function appends the SAP encapsulation for 
 * the IPv4 packets. 
 *
 */

static uint8_t* append_sap_encap (uint8_t *pkt, uint32_t *remlen, void *h)
{
   sap_encap_t sap;
   
   sap.length_hi  = ((*remlen - 2) >> 8)& 0xFF ;
   sap.length_lo  = (*remlen - 2) & 0xFF ;
   sap.dsap       = IPV4_SAP_DSAP;
   sap.ssap       = IPV4_SAP_SSAP;
   sap.control    = IPV4_SAP_CONTROL;

   memcpy(pkt, &sap, sizeof(sap));
   *remlen -= sizeof(sap);

   return (pkt += sizeof(sap));
}

/* 
 * append_snap_encap - Append SNAP Encapsulation. 
 * @pkt: Pointer to Packet buffer.
 * @remlen: Pointer to remaining Length of Packet.
 * @h: Not Used. 
 * 
 * This function appends the SNAP encapsulation for
 * the IPv4 packets.
 */

static uint8_t* append_snap_encap (uint8_t *pkt, uint32_t *remlen, void *h)
{
   snap_encap_t snap;

   snap.length_hi       = ((*remlen - 2) >> 8)& 0xFF ;
   snap.length_lo       = (*remlen - 2) & 0xFF ;
   snap.dsap            = IPV4_SNAP_DSAP; 
   snap.ssap            = IPV4_SNAP_SSAP; 
   snap.control         = IPV4_SNAP_CONTROL; 

   snap.organization[0] = IPV4_SNAP_ORG_0;
   snap.organization[1] = IPV4_SNAP_ORG_1;
   snap.organization[2] = IPV4_SNAP_ORG_2;

   snap.ether_type_hi   = IPV4_SNAP_ETHER_HI; 
   snap.ether_type_lo   = IPV4_SNAP_ETHER_LO; 

   memcpy(pkt, &snap, sizeof(snap));
   *remlen -= sizeof(snap);

   return (pkt += sizeof(snap));
}


/**
 * compute_ip_checksum - Compute Ip Check SUM 
 * @pkt: Pointer to Packet buffer.
 * @nbytes: Length of packet. 
 *
 * This function computes the IP check sum of  the
 * IP header, in the IPV4 packets.
 *
 */

static uint16_t compute_ip_checksum (uint16_t *pkt, uint16_t nbytes)
{
   int      sum = 0;
   uint16_t answer = 0;
   uint16_t data_hold = 0;
   uint8_t  *pdata = (uint8_t *)pkt;
   
   while (nbytes > 1)
   {
      data_hold  = *pdata++ << 8; 
      data_hold |= *pdata++;
      sum += data_hold;
      nbytes -= 2;
   }   

   if (nbytes == 1)
   {
      *((uint8_t *) &answer) = *(uint8_t *) pkt;
      sum += answer;
   }

   sum    = (sum >> 16) + (sum & 0xFFFF);
   sum   += (sum >> 16);
   answer = ~sum; 

   return (answer);
}

/** 
 * append_ip_header - Append IP Header 
 * @pkt: Pointer to Packet buffer.
 * @remlen: Pointer to remaining Length of Packet.
 * @info: Packet Specific Information. 
 *
 * This function appends the IP header to the IPv4
 * packets.
 *
 */

static uint8_t* append_ip_header (uint8_t *pkt, uint32_t *remlen, 
                                  ip_info_t *info)
{
   ip_header_t  lcl_hdr;
   ip_header_t *iphdr  = &lcl_hdr; 
   ip_info_t   *ipinfo = info;
   uint16_t   hlen   = IPV4_HEADER_SIZE + ipinfo->option_length;

   iphdr->version          = IPV4_VERSION;
   iphdr->header_length    = hlen/4;
   iphdr->type_of_service  = 0x00;
   iphdr->total_length     = ipinfo->total_length;


   iphdr->identification   = IPV4_IDENTIFICATION;
   iphdr->flags            = 0x00;
   iphdr->fragment_offset  = 0x00;

   iphdr->time_to_live     = 0xFF;
   iphdr->protocol         = ipinfo->protocol;
   iphdr->header_check_sum = 0x00; 

   iphdr->source_ip        = ipinfo->source;
   iphdr->destination_ip   = ipinfo->destination;


   memcpy(pkt, iphdr, IPV4_HEADER_SIZE);

   if (ipinfo->option_length)
   {
      memcpy(pkt + IPV4_HEADER_SIZE, ipinfo->options,  ipinfo->option_length);
   }

   iphdr->header_check_sum = compute_ip_checksum ((uint16_t*)pkt, hlen);
   memcpy(pkt, iphdr, IPV4_HEADER_SIZE);

   *remlen -=  hlen; 

   return (pkt += hlen); 
}

/** 
 * append_l4_header - Append Layer-4 Header 
 * @pkt: Pointer to Packet buffer.
 * @remlen: Pointer to remaining Length of Packet.
 * @info: Packet Specific Information. 
 *
 * This function appends the source/destination port
 * numbers (Layer-4 information) to the packet.
 *
 */

static uint8_t* append_l4_header (uint8_t *pkt, uint32_t *remlen, 
                                  const ip_info_t *info)
{

   *pkt++ = (info->l4_src_port >> 8) & 0xFF;
   *pkt++ =  info->l4_src_port & 0xFF;

   *pkt++ = (info->l4_dst_port >> 8) & 0xFF;
   *pkt++ =  info->l4_dst_port & 0xFF;

   *remlen -= IPV4_L4_PORT_SIZE;
   return (pkt);
}


/** 
 * pkt_ipv4_generate - Generate IP Packet 
 * @pkt: Pointer to Packet buffer.
 * @len: Length of Packet.
 * @info: Packet Specific Information. 
 *
 * This function generates the ipv4 packets. The
 * function includes appending the following
 * information to the packet.
 *
 * Append MAC header.
 * Append VLAN TAG.
 * Append ARPA/SAP/SNAP encapsulation. 
 * Append IP header (Options if included).
 * Compute IP checksum for IP header.
 * Append L4 information.
 * Append the packet data.
 */

uint32_t pkt_ipv4_generate (uint8_t *pkt, uint32_t len, ip_info_t *info)
{
   volatile ip_info_t *ipinfo =  (ip_info_t *)info;

   uint32_t  count;
   uint32_t *rlen, remlen = len;
   uint8_t  *cur_pkt = pkt;
   uint8_t   curdata = ipinfo->data;

   rlen = &remlen;

   /* Append MAC Header */

   cur_pkt = (uint8_t *)append_mac_header (cur_pkt, rlen, ipinfo->mac_hdr);

   /* Append VLAN Tag to the Packet */
   if (info->tag_type == VNTAG) {
   	cur_pkt = (uint8_t *) append_vntag (cur_pkt, rlen, (void *)&ipinfo->vntag);
   } else {
   	cur_pkt = (uint8_t *) append_vlan_tag (cur_pkt, rlen, (void *)&ipinfo->vlan);
   }

   /* Append Encapsulation Header ARPA/SAP/SNAP */
   switch (ipinfo->encap_type)
   {
      case ARPA_ENCAP:
         cur_pkt = (uint8_t *) append_arpa_encap (cur_pkt, rlen, NULL);
         break;

      case SAP_ENCAP:
         cur_pkt = (uint8_t *) append_sap_encap (cur_pkt, rlen, NULL);
         break;

      case SNAP_ENCAP:
         cur_pkt = (uint8_t *)append_snap_encap (cur_pkt, rlen, NULL);
         break;

      default:
         break;
   }

   ipinfo->total_length = remlen; 

   /* Append IP Header */
   cur_pkt = (uint8_t *) append_ip_header (cur_pkt, rlen, 
                                           (ip_info_t*) ipinfo);

   /* Append L4 Header */
   cur_pkt = (uint8_t *) append_l4_header (cur_pkt, rlen, 
                                           (ip_info_t*) ipinfo);

   /* Append Datagram */
   for (count = 0; count < *rlen; count++)
   {
      cur_pkt[count] = curdata;
      curdata        = pkt_data_generate (ipinfo->mode,
                                          curdata,
                                          ipinfo->incr);
   }

   return (0);
}
