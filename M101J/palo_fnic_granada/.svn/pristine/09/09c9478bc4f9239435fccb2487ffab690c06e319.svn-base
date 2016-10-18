/********************************************************************
 *
 *      File:   pkt_fc.h
 *      Name:   Sudharshan Kadari
 *
 *      Description:
 *       FC Packet generation code. 
 *
 *
 * Copyright (c) 1985-2007 by Cisco Systems, Inc.
 * All rights reserved.
 *
 *
 *********************************************************************/

#ifndef _FC_PKT_H_
#define _FC_PHT_H_

#define   FC_CRC_POLYNOMIAL      0x04c11db7

#define   UNKNOWN_FC_HEADER      0x0000
#define   FC_SOF_HEADER          0x0001
#define   FC_10G_SOF_HEADER      0x0002
#define   FC_BASIC_HEADER        0x0004
#define   FC_NETWORK_HEADER      0x0008
#define   FC_ASSOCIATION_HEADER  0x0010
#define   FC_DEVICE_HEADER       0x0020
#define   FC_LUN_HEADER          0x0040
#define   FC_ERROR               0x8000


#define  FC_CLASS_1              0x0001
#define  FC_CLASS_2              0x0002
#define  FC_CLASS_3              0x0003
#define  FC_CLASS_4              0x0004
#define  FC_CLASS_F              0x000F
#define  FC_SOF_CONNECT          0x0100
#define  FC_SOF_INITIATE         0x0200
#define  FC_SOF_NORMAL           0x0400
#define  FC_SOF_ACTIVATE         0x0800
#define  FC_SOF_FABRIC           0x1000


/* R_CTL field definitions */
#define  FC_RCTL(a, b)           ((a << 4) | b)
/* Routing bits */
#define  FC4_DEVICE_DATA         0x00
#define  EXTENDED_LINK_DATA      0x02
#define  FC4_LINK_DATA           0x03
#define  VIDEO_DATA              0x04
#define  BASIC_LINK_DATA         0x0C

/* r_ctl device data info category */
#define  UNCATEGORIZED_INFO      0x00
#define  SOLICITED_DATA          0x01
#define  UNSOLICITED_CONTROL     0x02
#define  SOLOCITED_CONTROL       0x03
#define  UNSOLOCITED_DATA        0x04
#define  DATA_DESCRIPTOR         0x05
#define  UNSOLICITED_COMMAND     0x06
#define  COMMAND_STATUS          0x07

/* r_ctl Extended Link Data info category */
#define  REQUEST                 0x02
#define  REPLY                   0x03

/* r_ctl Basic Link Data info category */
#define  NO_OPERATION            0x00
#define  ABORT_SEQUENCE          0x01
#define  REMOVE_CONNECTION       0x02
#define  BASIC_ACCEPT            0x04
#define  BASIC_REJECT            0x05
#define  PREEMPTED               0x06

/* r_ctl Link Control info category */
#define  ACKNOWLEDGE_1           0x00
#define  ACKNOWLEDGE_N           0x01
#define  N_PORT_REJECT           0x02
#define  FABRIC_REJECT           0x03
#define  N_PORT_BUSY             0x04
#define  F_BSY_DATA_FRAME        0x05
#define  F_BSY_LINK_CONTROL      0x06
#define  LINK_CREDIT_RESET       0x07
#define  NOTIFY                  0x08
#define  VENDOR_UNIQE            0x09

/* FC SOF and EOF defines defines */
#define  FC_SOF_SIZE             4
#define  FC_CRC_SIZE             4
#define  FC_EOF_SIZE             4

#define FC_FD_CONTROL            0xBC
#define FC_10G_SOF_CONTROL       0xFB 
#define FC_10G_EOF_CONTROL       0xFD

#define  SOF_CONNECT             0x001010
#define  SOF_INITIATE            0x005050
#define  SOF_NORMAL              0x003030

#define  SOF_CLASS_1             0xB50707
#define  SOF_CLASS_2             0xB50505
#define  SOF_CLASS_3             0xB50606
#define  SOF_CLASS_4             0xB50909
#define  SOF_CLASS_F             0xB50808

#define  EOF_ABORT               0x00F0F0
#define  EOF_TERMINATE           0x005050
#define  EOF_NORMAL              0x00D0D0

#define  EOF_NEG                 0x950505
#define  EOF_POS                 0xB50505

#define  SOF_CONNECT_CLASS_1     0xB51717
#define  SOF_INITIATE_CLASS_1    0xB55757
#define  SOF_NORMAL_CLASS_1      0xB53737

#define  SOF_INITIATE_CLASS_2    0xB55555
#define  SOF_NORMAL_CLASS_2      0xB53535

#define  SOF_INITIATE_CLASS_3    0xB55656
#define  SOF_NORMAL_CLASS_3      0xB53636

#define  SOF_CONNECT_CLASS_4     0xB51919
#define  SOF_INITIATE_CLASS_4    0xB55959
#define  SOF_NORMAL_CLASS_4      0xB53939

#define  SOF_FRAME_FABRIC_F      0xB55858


#define  EOF_NORMAL_NEG          0x95D5D5
#define  EOF_NORMAL_POS          0xB5D5D5

#define  EOF_TERMINATE_NEG       0x957575
#define  EOF_TERMINATE_POS       0xB57575

#define  EOF_ABORT_NEG           0x95F5F5
#define  EOF_ABORT_POS           0xB5F5F5

#define  EOF_DTI_NEG             0x8A9595
#define  EOF_DTI_POS             0xAA9595

#define  EOF_NI_NEG              0x8AD5D5
#define  EOF_NI_POS              0xAAD5D5

#define  EOF_FRT_NEG             0x959999
#define  EOF_FRT_POS             0xB59999

#define  EOF_FRTI_NEG            0x8A9999
#define  EOF_FRTI_POS            0xAA9999

#define  EOF_DT_NEG              0x959595
#define  EOF_DT_POS              0xB59595


typedef struct _fc_pkt_hdr_s
{
   uint32_t sof       : 32;

   uint32_t r_ctl     : 8;
   uint32_t dst_id    : 24;

   uint32_t cs_ctl    : 8;
   uint32_t src_id    : 24;

   uint32_t type      : 8;
   uint32_t f_ctl     : 24;

   uint32_t seq_id    : 8;
   uint32_t df_ctl    : 8;
   uint32_t seq_cnt   : 16;

   uint32_t ox_id     : 16;
   uint32_t rx_id     : 16;

   uint32_t parameter : 32;

} fc_pkt_hdr_t;


typedef struct fc_info_s
{
   fc_pkt_hdr_t   *fc_hdr;
   uint32_t        flags;
   uint32_t        sof;
   uint32_t        eof;
   char*           mode;
   uint32_t        incr;
   uint32_t        data;
   uint32_t        data_sz;

} fc_info_t;


extern uint32_t generate_fc_pkt (uint8_t *pkt, uint32_t len, void *info);
extern uint32_t compute_crc (uint8_t *pkt, uint32_t size);

#endif /* _FC_PHT_H_ */



