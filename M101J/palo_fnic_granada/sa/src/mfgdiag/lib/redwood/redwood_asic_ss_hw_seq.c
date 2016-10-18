/********************************************************************
*
*      File:   redwood_asic_ss_hw_seq.c
*      Name:   Bruce McLoughlin
*
*      Description: Stripped down SS programming sequences
*      that can be shared between USD and Diags
*
*      Note: You'll need to define the following for your
*      platform:
*
*      reg_read( asic-id, register-addr, *data )
*      reg_write(asic-id, register-addr,  data )
*      bits( hi_pos, lo_pos, val )
*
*       CAUTION:
*           The "..._hw_seq.c" files use red_red_naxislave_define.h
*           while the rest of the USD code uses red_red_naxislave.h.
*           The two files are just different sets of #defines
*           generated from the rtdb file. The "_define" file uses
*           "Word addresses" whereas the other uses "byte addresses".
*           So, all the addresses used here need to be left-shifted
*           by two bits to generate the corresponding byte-
*           address for presentation on the NAXI bus.
*
*      Copyright (c) 2001,2002,2003,2004 by Nuova Systems, Inc.
********************************************************************/
#include <stdio.h>
#include <unistd.h>
#include "red_red_naxislave_define.h"

#define REDWD_PREFETCH_RSV  16

/*************** IO customization ***********************/

#ifdef USD
#include <isan/types.h>
#include "redwood_intf.h"
#include "redwood_access.h"
/* construct a "byte address" from a "word address" */
#define BA(x) ((x) << 2)
#else
#include "red_hw_lib.h"
#endif 
/*************** End of IO customization ****************/


/* distance between SS blocks registers */
#define SS_DISTANCE 0x10000



/* index is ssx*5 + port */
unsigned int ssx_cfg_class_regs[] = {

  RED_SS0_CFG_CLASS_MAX0,
  RED_SS0_CFG_CLASS_MAX1,
  RED_SS0_CFG_CLASS_MAX2,
  RED_SS0_CFG_CLASS_MAX3,
  RED_SS0_CFG_CLASS_MAX4
};

#define red_ss_cfg_class_max(asic, ssx, port, cells0, cells1, cells2, cells3) \
    reg_write(  asic,                                           \
                BA(ssx_cfg_class_regs[ port ]) + (ssx*SS_DISTANCE), \
                (bits(6,0,   cells0) |                          \
                 bits(14,8,  cells1) |                          \
                 bits(22,16, cells2) |                          \
                 bits(30,24, cells3)) );

int ss_init_class_max( int asic_id )
{
    int ssx, port;
    unsigned int cells0, cells1, cells2, cells3;

    cells0 = (9216 + 79)/80;  // QoS BringUp Defaults to same jumbo 
    cells1 = (9216 + 79)/80;  // QoS BringUp Defaults to same jumbo (unused)
    cells2 = (9216 + 79)/80;  // QoS BringUp Defaults to same jumbo (unused)
    cells3 = (9216 + 79)/80;  // QoS BringUp Defaults to same jumbo (unused)

    for (ssx = 0; ssx < 4; ssx++) {
        for (port = 0; port < 5; port ++) {
            red_ss_cfg_class_max( asic_id, ssx, port, cells0, cells1, cells2, cells3);
        }
    }
    return 0;
}

unsigned int ssx_cfg_resrc_regs[] = {

  RED_SS0_CFG_RESRC0,
  RED_SS0_CFG_RESRC1,
  RED_SS0_CFG_RESRC2,
  RED_SS0_CFG_RESRC3,
  RED_SS0_CFG_RESRC4

};

int ss_init_resrc( int asic_id )
{
    int ssx, port;
    unsigned int resrc[5];

    resrc[0] = (0<<0)|(0<<2)|(0<<4)|(0<<6);
    resrc[1] = (0<<0)|(0<<2)|(0<<4)|(0<<6);
    resrc[2] = (0<<0)|(0<<2)|(0<<4)|(0<<6);
    resrc[3] = (0<<0)|(0<<2)|(0<<4)|(0<<6);
    resrc[4] = (0<<0)|(0<<2)|(0<<4)|(0<<6);

    for (ssx = 0; ssx < 4; ssx++) {
        for (port = 0; port < 5; port ++) {
            reg_write(  asic_id, 
                        BA(ssx_cfg_resrc_regs[ port ]) + (ssx*SS_DISTANCE), 
                        resrc[port] );
        }
    }
    return 0;
}

unsigned int ssx_cfg_frh_info_regs[] = {
   RED_SS0_CFG_FRH0_INFO0,
   RED_SS0_CFG_FRH0_INFO1,
   RED_SS0_CFG_FRH0_INFO2,
   RED_SS0_CFG_FRH0_INFO3,
   RED_SS0_CFG_FRH0_INFO4,
   RED_SS0_CFG_FRH1_INFO0,
   RED_SS0_CFG_FRH1_INFO1,
   RED_SS0_CFG_FRH1_INFO2,
   RED_SS0_CFG_FRH1_INFO3,
   RED_SS0_CFG_FRH1_INFO4,
   RED_SS0_CFG_FRH2_INFO0,
   RED_SS0_CFG_FRH2_INFO1,
   RED_SS0_CFG_FRH2_INFO2,
   RED_SS0_CFG_FRH2_INFO3,
   RED_SS0_CFG_FRH2_INFO4,
   RED_SS0_CFG_FRH3_INFO0,
   RED_SS0_CFG_FRH3_INFO1,
   RED_SS0_CFG_FRH3_INFO2,
   RED_SS0_CFG_FRH3_INFO3,
   RED_SS0_CFG_FRH3_INFO4
};

#define red_ss_cfg_frh_info( a, ssx, frh, port, action, xon, xoff, mbrs, cells) \
    reg_write(  a,                                           \
                BA(ssx_cfg_frh_info_regs[ frh*5 + port ]) + (ssx*SS_DISTANCE),  \
                (bits(24,24, action) | \
                 bits(23,20, xon)    | \
                 bits(19,12, (xoff)) | \
                 bits(11,8, mbrs)    | \
                 bits(7,0, cells)) );  \


int ss_init_frh_info( int asic_id )
{
    int ssx;

    for (ssx = 0; ssx < 4; ssx++) {

        /************************************************************
         *                                                          *
         * QoS Defaults for BringUP : 71*4 + 15 + 1 = 300 pauses    *
         *                                                          *
         *                                                        c *
         *                                     p        x      m  e *
         *                                 f   o  a  x  o      b  l *
         *                                 r   r  c  o  f      r  l *
         *                                 h   t  t  n  f      s  s */
        red_ss_cfg_frh_info( asic_id, ssx, 0 , 0, 1, 19,39,  0xf, 71);
        red_ss_cfg_frh_info( asic_id, ssx, 0 , 1, 1, 19,39,  0xf, 71);
        red_ss_cfg_frh_info( asic_id, ssx, 0 , 2, 1, 19,39,  0xf, 71);
        red_ss_cfg_frh_info( asic_id, ssx, 0 , 3, 1, 19,39,  0xf, 71);
        /* port 4 has only one frh with all classes mapped to it */
        red_ss_cfg_frh_info( asic_id, ssx, 0 , 4, 1, 3, 6,   0xf, 15);

        /* frh1 not used for bringup */
        red_ss_cfg_frh_info( asic_id, ssx, 1 , 0, 1, 0, 0,     0, 0); // no mbrs
        red_ss_cfg_frh_info( asic_id, ssx, 1 , 1, 1, 0, 0,     0, 0); // no mbrs
        red_ss_cfg_frh_info( asic_id, ssx, 1 , 2, 1, 0, 0,     0, 0); // no mbrs
        red_ss_cfg_frh_info( asic_id, ssx, 1 , 3, 1, 0, 0,     0, 0); // no mbrs
        red_ss_cfg_frh_info( asic_id, ssx, 1 , 4, 1, 0, 0,     0, 0); // no mbrs

        /* frh2 not used for bringup */
        red_ss_cfg_frh_info( asic_id, ssx, 2 , 0, 1, 0, 0,     0, 0); // no mbrs
        red_ss_cfg_frh_info( asic_id, ssx, 2 , 1, 1, 0, 0,     0, 0); // no mbrs
        red_ss_cfg_frh_info( asic_id, ssx, 2 , 2, 1, 0, 0,     0, 0); // no mbrs
        red_ss_cfg_frh_info( asic_id, ssx, 2 , 3, 1, 0, 0,     0, 0); // no mbrs
        red_ss_cfg_frh_info( asic_id, ssx, 2 , 4, 1, 0, 0,     0, 0); // no mbrs

        /* frh3 not used */
        red_ss_cfg_frh_info( asic_id, ssx, 3 , 0, 1, 0, 0,     0, 0); // no mbrs
        red_ss_cfg_frh_info( asic_id, ssx, 3 , 1, 1, 0, 0,     0, 0); // no mbrs
        red_ss_cfg_frh_info( asic_id, ssx, 3 , 2, 1, 0, 0,     0, 0); // no mbrs
        red_ss_cfg_frh_info( asic_id, ssx, 3 , 3, 1, 0, 0,     0, 0); // no mbrs
        red_ss_cfg_frh_info( asic_id, ssx, 3 , 4, 1, 0, 0,     0, 0); // no mbrs
    }
    return 0;
}


unsigned int ss_cfg_glb_pause_regs[] = {

  RED_SS0_CFG_GLB_PAUSE0,
  RED_SS0_CFG_GLB_PAUSE1,
  RED_SS0_CFG_GLB_PAUSE2,
  RED_SS0_CFG_GLB_PAUSE3,
  RED_SS0_CFG_GLB_PAUSE4

};

#define red_ss_cfg_glb_pause( asic, ssx, port, frh_map, classes, xoff_thresh, cells ) \
    reg_write(  asic,                                           \
                BA(ss_cfg_glb_pause_regs[ port ]) + (ssx*SS_DISTANCE),  \
                (bits(3,0,   frh_map)     | \
                 bits(7,4,   classes)     | \
                 bits(15,8,  xoff_thresh) | \
                 bits(23,16, cells )) );

#define PAUSE_QUANTA 16
#define PORTS         5
#define CELL_SIZE    80
#define MAX_PKT_SIZE 9200
#define MAX_CELLS    4800
#define GLOBAL_CELLS (MAX_CELLS/PORTS/PAUSE_QUANTA)
#define MAX_PKT_CELLS (MAX_PKT_SIZE/CELL_SIZE)

int ss_cfg_glb_pause( int asic_id )
{
    int ssx, port;
    unsigned int thresh;

    thresh = GLOBAL_CELLS-(MAX_PKT_CELLS/16)-1;

    for (ssx = 0; ssx < 4; ssx++) {
        for (port = 0; port < 5; port ++) {
            red_ss_cfg_glb_pause( asic_id, ssx, port, 0, 0, thresh, GLOBAL_CELLS );
        }
    }
    return 0;
}


unsigned int ss_cfg_mcast_regs[] = {

    RED_SS0_CFG_MCAST0,
    RED_SS0_CFG_MCAST1,
    RED_SS0_CFG_MCAST2,
    RED_SS0_CFG_MCAST3,
    RED_SS0_CFG_MCAST4
};

#define  red_ss_cfg_mcast( asic, ssx, port, drop_en, drop_thresh ) \
    reg_write(  asic,                                           \
                BA(ss_cfg_mcast_regs[ port ]) + (ssx*SS_DISTANCE), \
                (bits(7,0, drop_thresh )  | \
                 bits(12,8, drop_en )) ); \

int ss_cfg_mcast( int asic_id )
{
    int ssx, port;

    for (ssx = 0; ssx < 4; ssx++) {
        for (port = 0; port < 5; port ++) {
            red_ss_cfg_mcast( asic_id, ssx, port, 0, 255 );
        }
    }
    return 0;
}

uint32_t rdw_fl_start = 0;
uint32_t rdw_fl_end = 0;
extern int redwood_ss_fl_init(int inst, uint32_t fl_start, uint32_t fl_end);
int ss_cfg_free_cells( int asic_id )
{
    int ssx;

    for (ssx = 0; ssx < 4; ssx++) {
#if 0
        // TEST
        reg_write(  asic_id,
                    BA(RED_SS0_CFG_FC) + (ssx*SS_DISTANCE),
                    (bits(0, 0, 0)              |
                     bits(13, 1, (MAX_CELLS-1)) | // be sure to use one less!!
                     bits(14, 14, 0)) );
        usleep(100);
        // END TEST
#endif /*0*/

        reg_write(  asic_id,
                    BA(RED_SS0_CFG_FC) + (ssx*SS_DISTANCE),
                    (bits(0, 0, 1)              |
                     bits(13, 1, (MAX_CELLS-1)) | // be sure to use one less!!
                     bits(14, 14, 1)) );

    }
    if (rdw_fl_start != rdw_fl_end) {
       redwood_ss_fl_init(asic_id, rdw_fl_start, rdw_fl_end);
    }
    return 0;
}

int ss_cfg_timeouts( int asic_id )
{
    int ssx;

    for (ssx = 0; ssx < 4; ssx++) {
        reg_write(  asic_id, BA(RED_SS0_CFG_FRM_TO) + (ssx*SS_DISTANCE), 0 );
        reg_write(  asic_id, BA(RED_SS0_CFG_REQ_TO) + (ssx*SS_DISTANCE), 0 );
    }
    return 0;
}

unsigned int ss_cfg_rdch_regs[] = {
  RED_SS0_CFG_RDCH0,
  RED_SS0_CFG_RDCH1,
  RED_SS0_CFG_RDCH2,
  RED_SS0_CFG_RDCH3,
  RED_SS0_CFG_RDCH4
};

int ss_cfg_rdch( int asic_id )
{
    int ssx, port;

    for (ssx = 0; ssx < 4; ssx++) {
        for (port = 0; port < 5; port ++) {
            reg_write(  asic_id, 
                        BA(ss_cfg_rdch_regs[ port ]) + (ssx*SS_DISTANCE), 
                        bits(3,0,0xf) );
        }
    }
    return 0;
}

int ss_wait_for_init_done( int asic_id )
{
    unsigned int dones = 0;
    int ssx;
#ifdef SAT_NUOVA_SIM
    int tries = 2; // will never work on sim anyhow
#else
    int tries = 1000;
#endif /*SAT_NUOVA_SIM*/

    for (ssx = 0; ssx < 4; ssx++) {
        do {
            reg_read( asic_id, BA(RED_SS0_INT_NORM) + (ssx*SS_DISTANCE), &dones );
            usleep(500);
        } while ( ((dones & 7) != 7) && (tries--) );

        /* clear the interrupt bits */
        reg_write( asic_id,  BA(RED_SS0_INT_NORM) + (ssx*SS_DISTANCE), (dones & 7) );

        if  ((dones & 7) != 7)
            return 1; // may not be initialized
    }
    return 0;
}

unsigned int ss_cfg_write_port_regs[] = {
    RED_SS0_CFG_WR_PORT0,
    RED_SS0_CFG_WR_PORT1,
    RED_SS0_CFG_WR_PORT2,
    RED_SS0_CFG_WR_PORT3,
    RED_SS0_CFG_WR_PORT4
};
#define red_ss_cfg_wr_port( asic, ssx, port, en, err_disc, ct, ct_min, em_stop_en )\
    reg_write(  asic,\
                BA(ss_cfg_write_port_regs[ port ]) + (ssx*SS_DISTANCE),\
                (bits(0,0, en)        |\
                 bits(1,1, err_disc ) |\
                 bits(5,2, ct )       |\
                 bits(19,6,ct_min )   |\
                 bits(20,20,em_stop_en )) );


int ss_cfg_write_ports( int asic_id, int enable /* or not */ )
{
    int ssx, port;

    for (ssx = 0; ssx < 4; ssx++) {
        for (port = 0; port < 5; port ++) {
            //red_ss_cfg_wr_port( asic_id, ssx, port, 1, 1, 0xf, 80, 0 );
            red_ss_cfg_wr_port( asic_id, ssx, port, enable, 1, 0x0/*ct disabled*/, 80, 0 );
        }
    }
    return 0;
}

typedef enum {
    pg_cntl = 0,
    grp_dec01,
    grp_dec23,
    grp_dec45,
    grp_dec67,
                                                                                                             
    que_dec0 = 8,
    que_dec1,
    que_dec2,
    que_dec3,
    que_dec4,
    que_dec5,
    que_dec6,
    que_dec7,
                                                                                                             
    que_sta0,
    que_sta1,
    que_sta2,
    que_sta3,
    que_sta4,
    que_sta5,
    que_sta6,
    que_sta7
                                                                                                             
} rdarb_dhs_offset_t;

#define red_ss_rdarb_pg_cntl( asic, ssx, port, quanta, ipg_adj, clr_cr) \
    reg_write( asic, \
               BA(RED_SS0_DHS_SS_RDARB) + (ssx*SS_DISTANCE) + (port*4*0x20), \
               (bits(4, 0, quanta) | \
                bits(8, 5, ipg_adj)| \
                bits(9, 9, clr_cr)) );


#define red_ss_rdarb_que_dec( asic, ssx, port, grp, que, dec, lsp, gsp ) \
    reg_write( asic, \
               BA(RED_SS0_DHS_SS_RDARB) + (ssx*SS_DISTANCE) + (port*4*0x20) + (4*(que_dec0 + que)), \
               (bits(7, 0, dec)   | \
                bits(10, 8, grp)  | \
                bits(11, 11, lsp) | \
                bits(12, 12, gsp)) );

int ss_cfg_rdarb( int asic_id )
{
    int ssx, port;

    for (ssx = 0; ssx < 3 /*SS3 not used*/; ssx++) {
        for (port = 0; port < 5; port++) { // QoS BringUp, treat ports same as CIF/BIF
            /* PG_CNTL */
            red_ss_rdarb_pg_cntl( asic_id, ssx, port, 16, 0, 0 );

            /* GRP_DEC for CIF/BIF -- only grp01 programmed different from default */
            reg_write( asic_id, BA(RED_SS0_DHS_SS_RDARB) + (ssx*SS_DISTANCE) +
                       (port*4*0x20) + (4*grp_dec01), 0xff01 );

            /* QUE_DEC */
            switch (ssx) {
            case 0:
                /* SS0                                    g  q  d */
                red_ss_rdarb_que_dec( asic_id, ssx, port, 0, 0,   0, 0, 0 );   
                break;

            case 1:
                /* SS1                                    g  q  d */
                red_ss_rdarb_que_dec( asic_id, ssx, port, 0, 0,   0, 0, 0 );   
                break;

            case 2:
                /* SS2                                    g  q  d */
                red_ss_rdarb_que_dec( asic_id, ssx, port, 0, 0, 128, 0, 0 );   
                red_ss_rdarb_que_dec( asic_id, ssx, port, 0, 4, 128, 0, 0 );   
                break;
            }
        }
    }

    // By now all 5 ports on all 4 SSXs are programmed on RDARB
    
    return 0;
}

int ss_enable_frame_timeouts( int asic_id )
{
  /* enable frame timeout processing */
  reg_write( asic_id, BA(RED_SS0_CFG_FRM_TO), 0x06ff ); //flush+en+us+maxtime
  reg_write( asic_id, BA(RED_SS1_CFG_FRM_TO), 0x06ff ); //flush+en+us+maxtime
  reg_write( asic_id, BA(RED_SS2_CFG_FRM_TO), 0x06ff ); //flush+en+us+maxtime
  reg_write( asic_id, BA(RED_SS3_CFG_FRM_TO), 0x06ff ); //flush+en+us+maxtime

  return 0;
}

/* overall SS init */
int asic_ss_init( int asic_id )
{
    int err;

//    /* disable+discard on write ports */
//    if ((err = ss_cfg_write_ports( asic_id, 0 )) != 0) return err;

    if ((err = ss_init_class_max( asic_id ))     != 0) return err;
    if ((err = ss_init_resrc( asic_id ))         != 0) return err;
    if ((err = ss_init_frh_info( asic_id ))      != 0) return err;
    if ((err = ss_cfg_glb_pause( asic_id ))      != 0) return err;
    if ((err = ss_cfg_mcast( asic_id ))          != 0) return err;
    if ((err = ss_cfg_free_cells( asic_id ))     != 0) return err;
    if ((err = ss_cfg_timeouts( asic_id ))       != 0) return err;
//    if ((err = ss_cfg_rdarb( asic_id ))          != 0) return err;
    if ((err = ss_cfg_rdch( asic_id ))           != 0) return err;
    if ((err = ss_wait_for_init_done( asic_id )) != 0) {
        printf("SS Init failed to complete\n");
        return err;
    }
    if ((err = ss_cfg_write_ports( asic_id, 1 ))    != 0) return err;

// Per Subbarao, we should NOT be enabling frm_to
//
//    if ((err = ss_enable_frame_timeouts( asic_id )) != 0) return err;

    return 0;
}

