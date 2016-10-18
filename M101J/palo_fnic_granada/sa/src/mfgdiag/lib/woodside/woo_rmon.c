/********************************************************************
 *
 *      File:   woo_rmon.c
 *      Name:   Sudarshan Kadari
 *
 *      Description: MAC RMON stats functions for Woodside USD
 *
 *
 *      Copyright (c) 2001-2009 by Cisco Systems, Inc.
 ********************************************************************/

#include <stdio.h> 
#include <stdint.h> 
#include "woo_ops.h"

typedef enum _asic_mac_stats_ {
    WOO_STAT_TX_PKT_LT64	= 0   
  , WOO_STAT_TX_PKT_64     
  , WOO_STAT_TX_PKT_65     
  , WOO_STAT_TX_PKT_128    
  , WOO_STAT_TX_PKT_256    
  , WOO_STAT_TX_PKT_512    
  , WOO_STAT_TX_PKT_1024   
  , WOO_STAT_TX_PKT_1519   
  , WOO_STAT_TX_PKT_2048   
  , WOO_STAT_TX_PKT_4096   
  , WOO_STAT_TX_PKT_8192   
  , WOO_STAT_TX_PKT_GT9216 
  , WOO_STAT_TX_PKTTOTAL   
  , WOO_STAT_TX_OCTETS     
  , WOO_STAT_TX_PKTOK      
  , WOO_STAT_TX_UCAST      
  , WOO_STAT_TX_MCAST      
  , WOO_STAT_TX_BCAST      
  , WOO_STAT_TX_VLAN       
  , WOO_STAT_TX_PAUSE      
  , WOO_STAT_TX_USER_PAUSE 
  , WOO_STAT_TX_FRM_ERROR  
  , WOO_STAT_RX_PKT_LT64   
  , WOO_STAT_RX_PKT_64     
  , WOO_STAT_RX_PKT_65     
  , WOO_STAT_RX_PKT_128    
  , WOO_STAT_RX_PKT_256    
  , WOO_STAT_RX_PKT_512    
  , WOO_STAT_RX_PKT_1024   
  , WOO_STAT_RX_PKT_1519   
  , WOO_STAT_RX_PKT_2048   
  , WOO_STAT_RX_PKT_4096   
  , WOO_STAT_RX_PKT_8192   
  , WOO_STAT_RX_PKT_GT9216 
  , WOO_STAT_RX_PKTTOTAL   
  , WOO_STAT_RX_OCTETS     
  , WOO_STAT_RX_PKTOK      
  , WOO_STAT_RX_UCAST      
  , WOO_STAT_RX_MCAST      
  , WOO_STAT_RX_BCAST      
  , WOO_STAT_RX_VLAN       
  , WOO_STAT_RX_OVERSIZE   
  , WOO_STAT_RX_TOOLONG    
  , WOO_STAT_RX_DISCARD    
  , WOO_STAT_RX_UNDERSIZE  
  , WOO_STAT_RX_FRAGMENT   
  , WOO_STAT_RX_CRCERR     
  , WOO_STAT_RX_CRCERR_STOMPED     
  , WOO_STAT_RX_INRANGEERR 
  , WOO_STAT_RX_JABBER     
  , WOO_STAT_RX_PAUSE      
  , WOO_STAT_RX_USER_PAUSE 
  , WOO_STAT_TX_OCTETSOK   
  , WOO_STAT_RX_OCTETSOK   
  , WOO_STATS_END
} asic_mac_stats_t;

char *strMacStats[]  = {
    "STAT_TX_PKT_LT64   "
  , "STAT_TX_PKT_64     "
  , "STAT_TX_PKT_65     "
  , "STAT_TX_PKT_128    "
  , "STAT_TX_PKT_256    "
  , "STAT_TX_PKT_512    "
  , "STAT_TX_PKT_1024   "
  , "STAT_TX_PKT_1519   "
  , "STAT_TX_PKT_2048   "
  , "STAT_TX_PKT_4096   "
  , "STAT_TX_PKT_8192   "
  , "STAT_TX_PKT_GT9216 "
  , "STAT_TX_PKTTOTAL   "
  , "STAT_TX_OCTETS     "
  , "STAT_TX_PKTOK      "
  , "STAT_TX_UCAST      "
  , "STAT_TX_MCAST      "
  , "STAT_TX_BCAST      "
  , "STAT_TX_VLAN       "
  , "STAT_TX_PAUSE      "
  , "STAT_TX_USER_PAUSE "
  , "STAT_TX_FRM_ERROR  "
  
  , "STAT_RX_PKT_LT64   "
  , "STAT_RX_PKT_64     "
  , "STAT_RX_PKT_65     "
  , "STAT_RX_PKT_128    "
  , "STAT_RX_PKT_256    "
  , "STAT_RX_PKT_512    "
  , "STAT_RX_PKT_1024   "
  , "STAT_RX_PKT_1519   "
  , "STAT_RX_PKT_2048   "
  , "STAT_RX_PKT_4096   "
  , "STAT_RX_PKT_8192   "
  , "STAT_RX_PKT_GT9216 "
  , "STAT_RX_PKTTOTAL   "
  , "STAT_RX_OCTETS     "
  , "STAT_RX_PKTOK      "
  , "STAT_RX_UCAST      "
  , "STAT_RX_MCAST      "
  , "STAT_RX_BCAST      "
  , "STAT_RX_VLAN       "
  , "STAT_RX_OVERSIZE   "
  , "STAT_RX_TOOLONG    "
  , "STAT_RX_DISCARD    "
  , "STAT_RX_UNDERSIZE  "
  , "STAT_RX_FRAGMENT   "
  , "STAT_RX_CRCERR     "
  , "STAT_RX_CRCERR_STOMPED"
  , "STAT_RX_INRANGEERR "
  , "STAT_RX_JABBER     "
  , "STAT_RX_PAUSE      "
  , "STAT_RX_USER_PAUSE "

  , "STAT_TX_OCTETSOK   "
  , "STAT_RX_OCTETSOK   "
  , "THE_END            "
  };


unsigned int rmon_base[] = {
#ifdef DIAG_PORTOLA
    ASIC(CI_DHS_MAC_STAT),
    ASIC(BI_DHS_MAC_STAT),
    ASIC(HI0_DHS_MAC_STAT),
    ASIC(HI1_DHS_MAC_STAT),
    ASIC(HI2_DHS_MAC_STAT),
    ASIC(HI3_DHS_MAC_STAT),
    ASIC(HI4_DHS_MAC_STAT),
    ASIC(HI5_DHS_MAC_STAT),
    ASIC(HI6_DHS_MAC_STAT),
    ASIC(HI7_DHS_MAC_STAT),
    ASIC(HI8_DHS_MAC_STAT),
    ASIC(HI9_DHS_MAC_STAT),
    ASIC(HI10_DHS_MAC_STAT),
    ASIC(HI11_DHS_MAC_STAT),
    ASIC(HI12_DHS_MAC_STAT),
    ASIC(HI13_DHS_MAC_STAT),
    ASIC(HI14_DHS_MAC_STAT),
    ASIC(HI15_DHS_MAC_STAT),
    ASIC(HI16_DHS_MAC_STAT),
    ASIC(HI17_DHS_MAC_STAT),
    ASIC(HI18_DHS_MAC_STAT),
    ASIC(HI19_DHS_MAC_STAT),
    ASIC(HI20_DHS_MAC_STAT),
    ASIC(HI21_DHS_MAC_STAT),
    ASIC(HI22_DHS_MAC_STAT),
    ASIC(HI23_DHS_MAC_STAT),
    ASIC(HI24_DHS_MAC_STAT),
    ASIC(HI25_DHS_MAC_STAT),
    ASIC(HI26_DHS_MAC_STAT),
    ASIC(HI27_DHS_MAC_STAT),
    ASIC(HI28_DHS_MAC_STAT),
    ASIC(HI29_DHS_MAC_STAT),
    ASIC(HI30_DHS_MAC_STAT),
    ASIC(HI31_DHS_MAC_STAT),
    ASIC(HI32_DHS_MAC_STAT),
    ASIC(HI33_DHS_MAC_STAT),
    ASIC(HI34_DHS_MAC_STAT),
    ASIC(HI35_DHS_MAC_STAT),
    ASIC(HI36_DHS_MAC_STAT),
    ASIC(HI37_DHS_MAC_STAT),
    ASIC(HI38_DHS_MAC_STAT),
    ASIC(HI39_DHS_MAC_STAT),
    ASIC(HI40_DHS_MAC_STAT),
    ASIC(HI41_DHS_MAC_STAT),
    ASIC(HI42_DHS_MAC_STAT),
    ASIC(HI43_DHS_MAC_STAT),
    ASIC(HI44_DHS_MAC_STAT),
    ASIC(HI45_DHS_MAC_STAT),
    ASIC(HI46_DHS_MAC_STAT),
    ASIC(HI47_DHS_MAC_STAT),
    ASIC(NI0_XE_DHS_STAT),
    ASIC(NI1_XE_DHS_STAT),
    ASIC(NI2_XE_DHS_STAT),
    ASIC(NI3_XE_DHS_STAT),
#else
    ASIC(CI_DHS_MAC_STAT),
    ASIC(BI_DHS_MAC_STAT),
    ASIC(HI0_XE_DHS_STAT),
    ASIC(HI1_XE_DHS_STAT),
    ASIC(HI2_XE_DHS_STAT),
    ASIC(HI3_XE_DHS_STAT),
    ASIC(HI4_XE_DHS_STAT),
    ASIC(HI5_XE_DHS_STAT),
    ASIC(HI6_XE_DHS_STAT),
    ASIC(HI7_XE_DHS_STAT),
    ASIC(HI8_XE_DHS_STAT),
    ASIC(HI9_XE_DHS_STAT),
    ASIC(HI10_XE_DHS_STAT),
    ASIC(HI11_XE_DHS_STAT),
    ASIC(HI12_XE_DHS_STAT),
    ASIC(HI13_XE_DHS_STAT),
    ASIC(HI14_XE_DHS_STAT),
    ASIC(HI15_XE_DHS_STAT),
    ASIC(HI16_XE_DHS_STAT),
    ASIC(HI17_XE_DHS_STAT),
    ASIC(HI18_XE_DHS_STAT),
    ASIC(HI19_XE_DHS_STAT),
    ASIC(HI20_XE_DHS_STAT),
    ASIC(HI21_XE_DHS_STAT),
    ASIC(HI22_XE_DHS_STAT),
    ASIC(HI23_XE_DHS_STAT),
    ASIC(HI24_XE_DHS_STAT),
    ASIC(HI25_XE_DHS_STAT),
    ASIC(HI26_XE_DHS_STAT),
    ASIC(HI27_XE_DHS_STAT),
    ASIC(HI28_XE_DHS_STAT),
    ASIC(HI29_XE_DHS_STAT),
    ASIC(HI30_XE_DHS_STAT),
    ASIC(HI31_XE_DHS_STAT),
    ASIC(NI0_XE_DHS_STAT),
    ASIC(NI1_XE_DHS_STAT),
    ASIC(NI2_XE_DHS_STAT),
    ASIC(NI3_XE_DHS_STAT),
    ASIC(NI4_XE_DHS_STAT),
    ASIC(NI5_XE_DHS_STAT),
    ASIC(NI6_XE_DHS_STAT),
    ASIC(NI7_XE_DHS_STAT),
#endif
};


/* read RMON stats from Woodside. make copy of previous stats */
void asic_rmon_stats_dump( int asic_id,
                          int port,
                          int width)
{
    unsigned int rmon_reg = BA(rmon_base[port]);
    unsigned int data, datau;
    int stride = (width == 32) ? 4 : 8;
    int i;

    for (i = 0; i < WOO_STATS_END; i++, rmon_reg += stride) {
        asic_rd(asic_id, rmon_reg, &data);
        if (width > 32) {
            asic_rd(asic_id, rmon_reg+4, &datau);
	    printf("[0x%08X: %s] : 0x%02X%08X\n", rmon_reg, strMacStats[i], datau, data);
        } else {
	    if (i == WOO_STAT_RX_CRCERR_STOMPED) i++;
	    printf("[0x%08X: %s] : 0x%08X\n", rmon_reg, strMacStats[i], data);
        }
    }
}

void asic_rmon_brief_stats_dump( int asic_id,
                                 int port,
                                 int width)
{
    unsigned int rmon_reg = BA(rmon_base[port]);
    unsigned int tx_pkttotal_l, tx_pkttotal_h, tx_octetsok_l, tx_octetsok_h;
    unsigned int rx_pkttotal_l, rx_pkttotal_h, rx_octetsok_l, rx_octetsok_h;
    int stride = (width == 32) ? 4 : 8;
    int i = 0;

    while (i < WOO_STAT_TX_PKTTOTAL) {
      if ((width == 32) && (i == WOO_STAT_RX_CRCERR_STOMPED)) {
          i=i+1; 
      }
      i = i +1;
      rmon_reg = rmon_reg+stride;
    } 

    asic_rd(asic_id, rmon_reg, &tx_pkttotal_l);
    if (width > 32) {
        asic_rd(asic_id, rmon_reg+4, &tx_pkttotal_h);
    }

    while (i < WOO_STAT_RX_PKTTOTAL) {
      if ((width == 32) && (i == WOO_STAT_RX_CRCERR_STOMPED)) {
          i=i+1; 
      }
      i = i +1;
      rmon_reg = rmon_reg+stride;
    } 
    asic_rd(asic_id, rmon_reg, &rx_pkttotal_l);
    if (width > 32) {
        asic_rd(asic_id, rmon_reg+4, &rx_pkttotal_h);
    }

    while (i < WOO_STAT_TX_OCTETSOK) {
      if ((width == 32) && (i == WOO_STAT_RX_CRCERR_STOMPED)) {
          i=i+1; 
      }
      i = i +1;
      rmon_reg = rmon_reg+stride;
    } 

    asic_rd(asic_id, rmon_reg, &tx_octetsok_l);
    if (width > 32) {
        asic_rd(asic_id, rmon_reg+4, &tx_octetsok_h);
    }
    /* increment rmon_reg to point to receive octets*/
    rmon_reg = rmon_reg+stride;
    asic_rd(asic_id, rmon_reg, &rx_octetsok_l);
    if (width > 32) {
        asic_rd(asic_id, rmon_reg+4, &rx_octetsok_h);
    }

    if (width > 32) {
        printf("[%s] : 0x%02X%08X/0x%02X%08X\t0x%02X%08X/0x%02X%08X\n", 
               asic_port_to_name(port),
               tx_pkttotal_h, tx_pkttotal_l, tx_octetsok_h, tx_octetsok_l,
               rx_pkttotal_h, rx_pkttotal_l, rx_octetsok_h, rx_octetsok_l);
    } else {
        printf("[%s] : 0x%08X/0x%08X\t0x%08X/0x%08X\n", 
               asic_port_to_name(port),
               tx_pkttotal_l, tx_octetsok_l,
               rx_pkttotal_l, rx_octetsok_l);
    }
}

/* Clear RMON stats from Woodside. make copy of previous stats */
void asic_rmon_stats_clear( int asic_id,
                          int port,
                          int width)
{
    unsigned int rmon_reg = BA(rmon_base[port]);
    unsigned int data = 0;
    int stride = (width == 32) ? 4 : 8;
    int count = (width == 32) ? WOO_STATS_END-1 : WOO_STATS_END;
    int i;

    for (i = 0; i < count; i++, rmon_reg += stride) {
        asic_wr(asic_id, rmon_reg, data);
        if (width > 32) {
            asic_wr( asic_id, rmon_reg+4, data);
        }
    }
}

void asic_rmon_stats_clear_all (int asic_id)
{
    unsigned int port;

    printf("  Woo-%d Stats clear\n", asic_id);
    for (port = cif; port <= ASIC(LAST_NIF); port++) {
	asic_rmon_stats_clear(asic_id, port, (port >= ASIC(FIRST_10G)) ? 36 : 32);
    }
}

int asic_rmon_clear(int asic_id)
{
	asic_rmon_stats_clear_all (asic_id);
	return (0);
}


/* read RMON stats from Woodside. make copy of previous stats */
void asic_rmon_stats_get(int  asic_id,
                        int  port,
                        int  width, 
			uint32_t *tx_cnt, 
			uint32_t *rx_cnt)
{
    unsigned int r_base = BA(rmon_base[port]);
    unsigned int rmon_reg;
    int stride = (width == 32) ? 4 : 8;
   
    rmon_reg = r_base + (stride * WOO_STAT_TX_UCAST);
    asic_rd(asic_id, rmon_reg, tx_cnt);
    rmon_reg = r_base + (stride * WOO_STAT_RX_UCAST);
    asic_rd(asic_id, rmon_reg, rx_cnt);
}

void asic_rmon_cif_stats_get(int  asic_id,
			uint32_t *tx_cnt, 
			uint32_t *rx_cnt)
{
	asic_rmon_stats_get(asic_id, cif, 32, tx_cnt, rx_cnt);
}

int asic_rmon_err_stats_dump (int asic_id, int port, int width)
{
	uint32_t data, cnt, count;
	uint32_t addr[] = {WOO_STAT_TX_PKT_LT64
			, WOO_STAT_TX_FRM_ERROR  
			, WOO_STAT_RX_PKT_LT64   
			, WOO_STAT_RX_OVERSIZE   
			, WOO_STAT_RX_TOOLONG    
			, WOO_STAT_RX_DISCARD    
			, WOO_STAT_RX_UNDERSIZE  
			, WOO_STAT_RX_FRAGMENT   
			, WOO_STAT_RX_CRCERR     
			, WOO_STAT_RX_INRANGEERR 
			, WOO_STAT_RX_JABBER
			, WOO_STAT_RX_CRCERR_STOMPED};

    unsigned int r_base = BA(rmon_base[port]);
    unsigned int rmon_reg, err = 0;
    int stride = (width == 32) ? 4 : 8;
   
    count = sizeof(addr)/sizeof(uint32_t);

    // STOPMED REG is no available for CI/BI
    if (width == 32) count--; 	
    for (cnt = 0; cnt < count; cnt++) {
        rmon_reg = r_base + (stride * addr[cnt]);
        asic_rd(asic_id, rmon_reg, &data);
	if (data) {
	    err = 1;
	    printf(" Woo-%d %s %s : [0x%X]\n", 
				asic_id, asic_port_to_name(port), 
				strMacStats[addr[cnt]], data);
	}
   }
   return (err);	
}

int asic_err_dump(int inst, uint32_t *pmask)
{
        int port, rc = 0;

        *pmask = 0x00;
        for (port = cif; port <= ASIC(LAST_NIF); port++) {
                rc = asic_rmon_err_stats_dump(inst, port, (port >= ASIC(FIRST_10G)) ? 36 : 32);
                if (rc)  *pmask |= (0x01 << port);
        }
        return (0);
}

