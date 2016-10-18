/********************************************************************
 *
 *      File:   redwood_mac_rmon.c
 *      Name:   Bruce McLoughlin
 *
 *      Description: MAC RMON stats functions for Redwood USD
 *
 *
 *      Copyright (c) 2001,2002,2003,2004 by Nuova Systems, Inc.
 ********************************************************************/

#include "red_hw_lib.h"
#include "redwood_mac_rmon_stats.h"
#include "red_red_naxislave_define.h"

typedef enum _rdw_mac_stats_ {
    RDW_STAT_TX_PKT_LT64	= 0   
  , RDW_STAT_TX_PKT_64     
  , RDW_STAT_TX_PKT_65     
  , RDW_STAT_TX_PKT_128    
  , RDW_STAT_TX_PKT_256    
  , RDW_STAT_TX_PKT_512    
  , RDW_STAT_TX_PKT_1024   
  , RDW_STAT_TX_PKT_1519   
  , RDW_STAT_TX_PKT_2048   
  , RDW_STAT_TX_PKT_4096   
  , RDW_STAT_TX_PKT_8192   
  , RDW_STAT_TX_PKT_GT9216 
  , RDW_STAT_TX_PKTTOTAL   
  , RDW_STAT_TX_OCTETS     
  , RDW_STAT_TX_PKTOK      
  , RDW_STAT_TX_UCAST      
  , RDW_STAT_TX_MCAST      
  , RDW_STAT_TX_BCAST      
  , RDW_STAT_TX_VLAN       
  , RDW_STAT_TX_PAUSE      
  , RDW_STAT_TX_USER_PAUSE 
  , RDW_STAT_TX_FRM_ERROR  
  , RDW_STAT_RX_PKT_LT64   
  , RDW_STAT_RX_PKT_64     
  , RDW_STAT_RX_PKT_65     
  , RDW_STAT_RX_PKT_128    
  , RDW_STAT_RX_PKT_256    
  , RDW_STAT_RX_PKT_512    
  , RDW_STAT_RX_PKT_1024   
  , RDW_STAT_RX_PKT_1519   
  , RDW_STAT_RX_PKT_2048   
  , RDW_STAT_RX_PKT_4096   
  , RDW_STAT_RX_PKT_8192   
  , RDW_STAT_RX_PKT_GT9216 
  , RDW_STAT_RX_PKTTOTAL   
  , RDW_STAT_RX_OCTETS     
  , RDW_STAT_RX_PKTOK      
  , RDW_STAT_RX_UCAST      
  , RDW_STAT_RX_MCAST      
  , RDW_STAT_RX_BCAST      
  , RDW_STAT_RX_VLAN       
  , RDW_STAT_RX_OVERSIZE   
  , RDW_STAT_RX_TOOLONG    
  , RDW_STAT_RX_DISCARD    
  , RDW_STAT_RX_UNDERSIZE  
  , RDW_STAT_RX_FRAGMENT   
  , RDW_STAT_RX_CRCERR     
  , RDW_STAT_RX_INRANGEERR 
  , RDW_STAT_RX_JABBER     
  , RDW_STAT_RX_PAUSE      
  , RDW_STAT_RX_USER_PAUSE 
  , RDW_STAT_TX_OCTETSOK   
  , RDW_STAT_RX_OCTETSOK   
  , RDW_STATS_END
} rdw_mac_stats_t;

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
  , "STAT_RX_INRANGEERR "
  , "STAT_RX_JABBER     "
  , "STAT_RX_PAUSE      "
  , "STAT_RX_USER_PAUSE "


  , "STAT_TX_OCTETSOK   "
  , "STAT_RX_OCTETSOK   "
  , "THE_END            "
  };


unsigned int rmon_base[] = {
    RED_CI0_DHS_MAC_STAT,
    RED_BI0_DHS_MAC_STAT,
    RED_HI0_DHS_MAC_STAT,
    RED_HI1_DHS_MAC_STAT,
    RED_HI2_DHS_MAC_STAT,
    RED_HI3_DHS_MAC_STAT,
    RED_HI4_DHS_MAC_STAT,
    RED_HI5_DHS_MAC_STAT,
    RED_HI6_DHS_MAC_STAT,
    RED_HI7_DHS_MAC_STAT,
    RED_NI0_DHS_MAC_STAT,
    RED_NI1_DHS_MAC_STAT,
    RED_NI2_DHS_MAC_STAT,
    RED_NI3_DHS_MAC_STAT,
};


unsigned int rmon_upper[] = {
    RED_CI0_STA_STAT_UPPER,
    RED_BI0_STA_STAT_UPPER,
    RED_HI0_STA_STAT_UPPER,
    RED_HI1_STA_STAT_UPPER,
    RED_HI2_STA_STAT_UPPER,
    RED_HI3_STA_STAT_UPPER,
    RED_HI4_STA_STAT_UPPER,
    RED_HI5_STA_STAT_UPPER,
    RED_HI6_STA_STAT_UPPER,
    RED_HI7_STA_STAT_UPPER,
    RED_NI0_STA_STAT_UPPER,
    RED_NI1_STA_STAT_UPPER,
    RED_NI2_STA_STAT_UPPER,
    RED_NI3_STA_STAT_UPPER,
};

/* read RMON stats from Redwood. make copy of previous stats */
void mac_rmon_stats_dump( int asic_id,
                          int port,
                          int width)
{
    unsigned int rmon_reg = BA(rmon_base[port]);
    unsigned int data, datau;
    int stride = (width == 32) ? 4 : 8;
    int i;

    for (i = 0; i < RMON_STAT_NUM; i++, rmon_reg += stride) {
        redwood_read_register_internal(  asic_id,
                                         rmon_reg,
                                         &data,
                                         __FILE__,
                                         __LINE__,
                                         1 /* for periodic accesses */ );
        if (width == 36) {
            redwood_read_register_internal(  asic_id,
                                             BA(rmon_upper[port]),
                                             &datau,
                                             __FILE__,
                                             __LINE__,
                                             1 /* for periodic accesses */ );
	    printf("[%s] : 0x%02X%08X\n", strMacStats[i], datau, data);
        }
        else {
	    printf("[%s] : 0x%08X\n", strMacStats[i], data);
        }
    }
}

/* Clear RMON stats from Redwood. make copy of previous stats */
void mac_rmon_stats_clear( int asic_id,
                          int port,
                          int width)
{
    unsigned int rmon_reg = BA(rmon_base[port]);
    unsigned int data = 0;
    int stride = (width == 32) ? 4 : 8;
    int i;

    for (i = 0; i < RMON_STAT_NUM; i++, rmon_reg += stride) {
        redwood_write_register_internal(  asic_id,
                                         rmon_reg,
                                         data,
                                         __FILE__,
                                         __LINE__,
                                         1 /* for periodic accesses */ );
        if (width == 36) {
            redwood_write_register_internal(  asic_id,
                                             BA(rmon_upper[port]),
                                             data,
                                             __FILE__,
                                             __LINE__,
                                             1 /* for periodic accesses */ );
        }
    }
}

void mac_rmon_stats_clear_all (int asic_id)
{
    unsigned int port;

    for (port = cif; port <= hif7; port++) {
	mac_rmon_stats_clear(asic_id, port, (port > 1) ? 36 : 32);
    }
}


/* read RMON stats from Redwood. make copy of previous stats */
void mac_rmon_stats_get(int  asic_id,
                        int  port,
                        int  width, 
			uint32_t *tx_cnt, 
			uint32_t *rx_cnt)
{
    unsigned int r_base = BA(rmon_base[port]);
    unsigned int rmon_reg;
    int stride = (width == 32) ? 4 : 8;
   
    rmon_reg = r_base + (stride * RDW_STAT_TX_UCAST);

    redwood_read_register_internal(asic_id,
                                   rmon_reg,
                                   tx_cnt,
                                   __FILE__,
                                   __LINE__,
                                   1 /* for periodic accesses */ );

    rmon_reg = r_base + (stride * RDW_STAT_RX_UCAST);
    redwood_read_register_internal(asic_id,
                                   rmon_reg,
                                   rx_cnt,
                                   __FILE__,
                                   __LINE__,
                                   1 /* for periodic accesses */ );
}

void mac_rmon_cif_stats_get(int  asic_id,
			uint32_t *tx_cnt, 
			uint32_t *rx_cnt)
{
	mac_rmon_stats_get(asic_id, cif, 32, tx_cnt, rx_cnt);
}

extern char *rdw_port_names[];
int mac_rmon_err_stats_dump (int asic_id, int port, int width)
{
	uint32_t data, cnt, count;
	uint32_t addr[] = {RDW_STAT_TX_PKT_LT64
			, RDW_STAT_TX_FRM_ERROR  
			, RDW_STAT_RX_PKT_LT64   
			, RDW_STAT_RX_OVERSIZE   
			, RDW_STAT_RX_TOOLONG    
			, RDW_STAT_RX_DISCARD    
			, RDW_STAT_RX_UNDERSIZE  
			, RDW_STAT_RX_FRAGMENT   
			, RDW_STAT_RX_CRCERR     
			, RDW_STAT_RX_INRANGEERR 
			, RDW_STAT_RX_JABBER};

    unsigned int r_base = BA(rmon_base[port]);
    unsigned int rmon_reg, err = 0;
    int stride = (width == 32) ? 4 : 8;
   
	count = sizeof(addr)/sizeof(uint32_t);

	for (cnt = 0; cnt < count; cnt++) {
    		rmon_reg = r_base + (stride * addr[cnt]);
    		redwood_read_register_internal(asic_id,
                                   rmon_reg,
                                   &data,
                                   __FILE__,
                                   __LINE__,
                                   1 /* for periodic accesses */ );

		if (data) {
			err = 1;
			printf(" Redwood%d %s %s : [0x%X]\n", 
					asic_id, rdw_port_names[port], 
					strMacStats[addr[cnt]], data);
		}
	}
	return (err);	
}
