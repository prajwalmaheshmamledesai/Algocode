/********************************************************************
 *
 *      File:   pkt_fc.c
 *      Name:   Sudharshan Kadari
 *
 *      Description:
 *       FC Packet Generation 
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
#include "pkt_fc.h"
#include "pkt_ops.h"

/**
 * pkt_fc_compute_crc - Compute 32 bit CRC for the Packet.
 * @pkt: Pinter to the packet data.
 * @size: Size of the packet on which CRC should be computed.
 * 
 * compute_crc :  This routine is used to compute the 32 bit CRC for
 *                FC packets. CRC includes the FC header and Payload
 *                only. SOF, EOF and CRC portion of the packet are not
 *                used to compute CRC.                           
 *
 */ 
uint32_t pkt_fc_compute_crc (const uint8_t *pkt, const uint32_t size)
{
   uint32_t bitnum, pktcrc, crcResult = 0xFFFFFFFF;
   uint32_t offset, crcXorFactor[] = {0x00, FC_CRC_POLYNOMIAL};
   uint8_t  crc_val[4]; 

   bzero (crc_val, sizeof(crc_val));
   for (offset = 0; offset < size; offset++)
   {
       for (bitnum = 0; bitnum < 8; bitnum++)
       {
           uint32_t msbResult = (crcResult & 0x80000000) ? 1 : 0;
           uint32_t msbByte   = (pkt[offset] >> bitnum) & 0x01;
           crcResult <<= 1;
           crcResult ^= crcXorFactor[msbResult ^ msbByte];

       }
   }

   crcResult = ~crcResult;

   for (offset = 0; offset < 4; offset ++)
   {
       for (bitnum = 0; bitnum < 8; bitnum++)
       {
           crc_val[offset] <<= 1;
           crc_val[offset] |= crcResult & 0x01;
           crcResult >>= 1;
       }
   }

   pktcrc = (crc_val[3] << 24) | (crc_val[2] << 16) |
            (crc_val[1] << 8) | crc_val[0];

   return (pktcrc);
}

/** 
 * pkt_fc_generate - Generate the Fibre Channal Packet. 
 * @pkt: Buffer pointer to generate the packet.
 * @len: Length of the packet.
 * @info: Packet Specific Info.
 */ 

uint32_t pkt_fc_generate (uint8_t *pkt, uint32_t len, void *info)
{
   uint8_t curdata, *pfchdr, *cur_pkt = pkt;
   fc_info_t *fcinfo =  (fc_info_t *)info;
   uint32_t crc, remlen = len;
   uint32_t payload_len, count;
   uint32_t data32, *pdata;

   /*
    * Append FC header
    */
   memcpy (cur_pkt, (uint32_t *)&(fcinfo->sof), FC_SOF_SIZE);
   remlen  -= FC_SOF_SIZE;
   cur_pkt += FC_SOF_SIZE;

   pfchdr = cur_pkt;
   memcpy (cur_pkt, fcinfo->fc_hdr, sizeof(fc_pkt_hdr_t));
   remlen  -= sizeof(fc_pkt_hdr_t);
   cur_pkt += sizeof(fc_pkt_hdr_t);

   /*
    * Create the Payload
    */
   payload_len = remlen - FC_CRC_SIZE - FC_EOF_SIZE;

   /* 
    * This supports a data modification of either 32 bit
    * 0r 8 bit 
    */ 
   if (fcinfo->data_sz == sizeof(uint32_t)) {

      data32 = fcinfo->data;
      pdata  = (uint32_t*)cur_pkt;

      for (count = 0; count < payload_len; pdata++, count += sizeof(uint32_t))
      {
         *pdata = data32; 
         data32 = pkt_data_generate32 (fcinfo->mode, 
                                       data32, 
                                       fcinfo->incr);
      }
   } else {
      curdata     = fcinfo->data;

      for (count = 0; count < payload_len; count++)
      {
         cur_pkt[count] = curdata;
         curdata        = pkt_data_generate (fcinfo->mode, 
                                             curdata, 
                                             fcinfo->incr);
      }
   } 

   cur_pkt += remlen - FC_CRC_SIZE - FC_EOF_SIZE;

   /*
    * CRC includes the FC header and payload only. SOF and EOF are
    * not included in the CRC computation.
    */
   crc = pkt_fc_compute_crc (pfchdr, len - FC_CRC_SIZE - 
                                     FC_SOF_SIZE - FC_EOF_SIZE);

   /* 
    * Memcpy is used instead of direct assignments to avoid
    * exception due to misalignments.
    */
   /* Remove CRC for now */
   crc = 0x00;

   memcpy(cur_pkt, &crc, FC_CRC_SIZE);
   cur_pkt              += FC_CRC_SIZE;
   memcpy(cur_pkt, &fcinfo->eof, FC_CRC_SIZE);

   return (0);
}
