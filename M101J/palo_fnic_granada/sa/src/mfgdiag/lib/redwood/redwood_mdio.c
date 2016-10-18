/********************************************************************
 *
 *      File:   redwood_mdio.c
 *      Name:   Bruce McLoughlin
 *
 *      Description: MDIO level functions for Redwood USD
 *
 *      Copyright (c) 2001,2002,2003,2004 by Nuova Systems, Inc.
 ********************************************************************/
#include <stdio.h>
#include <time.h>
#include <assert.h>
#ifdef USD
#include "isan/fsm_utils_intf.h"
#include "isan/ethpm_defs.h"
#include "isan/redwood_intf.h"
#include "redwood_private.h"
#include "redwood.h"
#include "redwood_access.h"
#include "redwood_phy.h"
#include "redwood_mac.h"
#include "red_red_naxislave_define.h"


//#include "red_red_naxislave.h"
#define BA(x) ((x) << 2)
#define mdio_reg_read( a,r,d) redwood_read_register_internal( a,r,d, __FILE__, __LINE__, p_ctx->log_mdios ? 0:1);
#define mdio_reg_write(a,r,d) redwood_write_register_internal(a,r,d, __FILE__, __LINE__, p_ctx->log_mdios ? 0:1 );
#else
#include <sys/types.h>
#include "red_hw_lib.h"
#include "red_red_naxislave_define.h"
#define mdio_reg_read( a,r,d) reg_read( a,r,d)
#define mdio_reg_write(a,r,d) reg_write(a,r,d)
#endif

#include "redwood_mdio.h"

#define PREFIX " |   |     |    |   + MDIO: "

int skip_gbe_mdio_preamble = 0;

/***************************************************************
* mdio_init_master
***************************************************************/
void mdio_init_master_clause_45( int asic_id, unsigned int mdio_master )
{
    unsigned int unused;

    // reset the MDIO logic
    mdio_reg_write( asic_id, mdio_master, ~0 );
    mdio_reg_read(  asic_id, mdio_master, &unused );

    // 16404 == RED_NI_MDIO_MASTER_DEF
    mdio_reg_write( asic_id, mdio_master, RED_NI_MDIO_MASTER_DEF & ~(1UL << RED_NI_MDIO_MASTER_FLD_SW_RST) );

}


/***************************************************************
* mdio_init_master_clause_22
***************************************************************/
void mdio_init_master_clause_22( int asic_id, unsigned int mdio_master )
{
    unsigned int data, unused;

    // reset the MDIO logic
    mdio_reg_read(  asic_id, mdio_master, &data );
    data |= (1 << 16);
    mdio_reg_write( asic_id, mdio_master, data );

    // flush
    mdio_reg_read(  asic_id, mdio_master, &unused );

    // un-reset
    data &= ~(1 << 16);
    mdio_reg_write( asic_id, mdio_master, data );

    // flush
    mdio_reg_read(  asic_id, mdio_master, &unused );

    // use default config for now
}


/***************************************************************
* redwood_mdio_init
*
*   Initialize the MDIO master module in Redwood (if not
*   done already).
*
*   This function is called at ASIC bringup time because
*   MDIO is required to load the PHY micro-code. So, here
*   we enable both 1.2V and 3.3V MDIO masters, even though
*   only one is physically connected on the board.
*
* Note: The "redwood" prefix is necessary due to a name
*       conflict in shared code.
**************************************************************/
void redwood_mdio_init( int asic_id )
{
    // enable all MDIO clocks
    mdio_reg_write( asic_id, BA(RED_MISC_MDIO_MDC_OE), ~0 );

    // reset the MDIO logic in both 1.2v and 3.3v masters
    mdio_init_master_clause_45( asic_id, BA(RED_NI0_MDIO_MASTER) );
    mdio_init_master_clause_45( asic_id, BA(RED_NI1_MDIO_MASTER) );

    mdio_init_master_clause_22( asic_id, BA(RED_CI0_GBE_CFG_MII_MDIO) );
}

/***************************************************************
* mdio_wait_for_not_busy
**************************************************************/
void mdio_wait_for_not_busy(int asic_id, unsigned int mdio_reg_base )
{
    unsigned int sts;
    int tries = 1000;

#ifdef SAT_NUOVA_SIM
    /******************************************************
    * TEMP: this takes an immense amount of time in the rtl
    * so just pretend to wait for now.  */
    tries = 2;
#endif

    do {
        /* read STATUS thru MDIO DHS array */
        mdio_reg_read( asic_id, mdio_reg_base + MDIO_STATUS, &sts );

    } while ((--tries) &&
             (sts & MDIO_STATUS_BUSY) &&
            !(sts & MDIO_STATUS_ERROR) );

    if (sts & MDIO_STATUS_ERROR)
        printf( PREFIX " Busy wait ERROR <mdio sts: %xh>\n", sts);
    else if (sts & MDIO_STATUS_BUSY)
        printf( PREFIX " Busy wait TIMEDOUT <mdio sts: %xh>\n", sts);
}

/***************************************************************
* mdio_read_thru_redwood
**************************************************************/
uint16_t mdio_read_thru_redwood(int          asic_id,
                                unsigned int mdio_reg_base,
                                uint8_t      portad, 
                                uint8_t      dev, 
                                uint16_t     reg )
{
    unsigned int data;
    unsigned int dev_port_addr = (portad << MDIO_COMMAND_PORT_SHIFT) | dev;

    /* wait until its not BUSY */
    mdio_wait_for_not_busy( asic_id, mdio_reg_base );

    /* write COMMAND w/phy dev/port, bits[15:14]=0 */
    mdio_reg_write( asic_id, mdio_reg_base + MDIO_COMMAND, dev_port_addr );

    /* write REGADDR (triggers an MDIO access) */
    mdio_reg_write( asic_id, mdio_reg_base + MDIO_REGADDR, reg );

    /* wait until its not BUSY (again) */
    mdio_wait_for_not_busy( asic_id, mdio_reg_base );

    /* write COMMAND w/phy dev/port bits[15:14] = 10b (triggers the MDIO access) */
    mdio_reg_write( asic_id, mdio_reg_base + MDIO_COMMAND, MDIO_COMMAND_READ | dev_port_addr );

    /* wait until its not BUSY (again) */
    mdio_wait_for_not_busy( asic_id, mdio_reg_base );

    /* read the DATA */
    mdio_reg_read( asic_id, mdio_reg_base + MDIO_DATA, &data );

    return( (uint16_t) data );
}

/***************************************************************
* mdio_write_thru_redwood
**************************************************************/
void mdio_write_thru_redwood( int asic_id,
                              unsigned int mdio_reg_base,
                              uint8_t portad, 
                              uint8_t dev, 
                              uint16_t reg, 
                              uint16_t data )
{
    unsigned int dev_port_addr = (portad << MDIO_COMMAND_PORT_SHIFT) | dev;

    /* wait until its not BUSY */
    mdio_wait_for_not_busy( asic_id, mdio_reg_base );

    /* write COMMAND w/phy dev/port, bits[15:14]=0 */
    mdio_reg_write( asic_id, mdio_reg_base + MDIO_COMMAND, dev_port_addr );

    /* write REGADDR (triggers an MDIO access) */
    mdio_reg_write( asic_id, mdio_reg_base + MDIO_REGADDR, reg );

    /* wait until its not BUSY (again) */
    mdio_wait_for_not_busy( asic_id, mdio_reg_base );

    /* write DATA (triggers the MDIO access) */
    mdio_reg_write( asic_id, mdio_reg_base + MDIO_DATA, data );

    /* wait until its not BUSY (again) */
    mdio_wait_for_not_busy( asic_id, mdio_reg_base );
}

/***************************************************************
* mdio_flush_gbe
**************************************************************/
void mdio_flush_gbe( int asic_id, unsigned int mii_reg )
{
    unsigned int unused;

    mdio_reg_read( asic_id, BA(RED_CI0_GBE_CFG_MII), &unused);
    mdio_reg_read( asic_id, BA(RED_CI0_GBE_CFG_MII), &unused);
}

/***************************************************************
* mdio_read_gbe
**************************************************************/
uint16_t mdio_read_gbe( int asic_id, uint8_t portad, uint8_t dev, uint16_t reg)
{
#ifdef SAT_NUOVA_SIM
    return( mdio_read( portad, dev, reg ) );
#else
    // FEX-TBD
    unsigned int ctrl_word, mdio_sts, data;
    int tout = 10;

    /* make sure MDIO master is ready for an op */
    /* Note: This can be removed once the GBE MDIO core has been verified */
    mdio_reg_read( asic_id, BA(RED_CI0_GBE_STA_MII), &mdio_sts);
    if (mdio_sts & (1 /*busy*/ << 16)) { // still busy, problem
        printf(" Clause 22 MDIO master Busy before read"
		" (portad: %d, reg: %d)\n", portad, reg);
        return( 0xffff );
    }

    /* make sur "go" bit is clear to begin with */
    /* Note: This can be removed once the GBE MDIO core has been verified */
    mdio_reg_read( asic_id, BA(RED_CI0_GBE_CFG_MII), &ctrl_word);
    if (ctrl_word & (1 << 28)) { // "go" already set, problem
        printf(" Clause 22 MDIO master go-bit set before read"
		"  (portad: %d, reg: %d)\n", portad, reg);
        mdio_reg_write( asic_id, BA(RED_CI0_GBE_CFG_MII), 0);
	/* See "flush note:" comment below */
        mdio_flush_gbe( asic_id, BA(RED_CI0_GBE_CFG_MII)); 
        mdio_reg_read(  asic_id, BA(RED_CI0_GBE_CFG_MII), &ctrl_word);
        if (ctrl_word & (1 << 28)) { // "go" already set, problem
            printf(" Clause 22 MDIO master go-bit unclearable"
		   " (portad: %d, reg: %d)\n", portad, reg);
            return( 0xffff );
        }
    }

    ctrl_word = (portad     << 21) |
                (reg        << 16) |
                (1 /*read*/ << 26) |
                (skip_gbe_mdio_preamble << 27) |
                (0 /*go*/   << 28);

    /* issue ctrl word with "go" bit set. Should trigger MDIO read op */
    mdio_reg_write( asic_id, BA(RED_CI0_GBE_CFG_MII), ctrl_word);

    ctrl_word |=(1 /*go*/   << 28);

    /* issue ctrl word with "go" bit set. Should trigger MDIO read op */
    mdio_reg_write( asic_id, BA(RED_CI0_GBE_CFG_MII), ctrl_word);


    /* wait for done */
    do {
        mdio_reg_read( asic_id, BA(RED_CI0_GBE_STA_MII), &mdio_sts);
    } while ((mdio_sts & (1 /*busy*/ << 16)) && (--tout > 0));

    /* see if read ok or timeout */
    if (mdio_sts & (1 /*busy*/ << 16)) { // still busy, problem
        printf(" Clause 22 MDIO master Busy wait READ TIMEOUT"
		" (portad: %d, reg: %d\n", portad, reg);
        data = 0xffff;
    }
    else {
        /* get the data read from the PHY */
        data = mdio_sts & 0xFFFF; // only 16 bits of data (rest is status)
    }

    /* clear the "go" bit for next time */
    mdio_reg_write( asic_id, BA(RED_CI0_GBE_CFG_MII), 0 );
    /* FLUSH NOTE:
    *
    * The following flushes are needed to avoid the following 
    * race condition:
    * If the Write(CFG_MII,0) and the Write(CFG_MII,some_data_go_is_set) 
    * for the next operation
    * happen back to back in rnaxi, then the pulse that needs to cross 
    * the async boundary is too short
    * and it's missed by the slower destination clock.
    * The gbe_wrapper logic does a posedge detection to trigger 
    * the state machine:
    * CFG_mii_go_d1 <= #1 CFG_mii_go;
    * mii_go_pulse <= #1 CFG_mii_go & ~CFG_mii_go_d1;
    * As a result, the go is never seen, busy is never set,
    * and we get garbage back in the read(CFG_STA), regardless of 
    * how many reads you make after the write. */
    mdio_flush_gbe( asic_id, BA(RED_CI0_GBE_CFG_MII));

//    printf("MDIO (gbe) Read  portad: %d, reg: %xh == %xh\n", 
//		portad, reg, data );

    return data;
#endif
}
                                                                                                         
/***************************************************************
* mdio_read_1_2v
**************************************************************/
uint16_t mdio_read_1_2v( int asic_id, uint8_t portad, uint8_t dev, uint16_t reg)
{
#ifdef SAT_NUOVA_SIM
    return( mdio_read( portad, dev, reg ) );
#else
    return( mdio_read_thru_redwood( asic_id,
                            BA(RED_NI0_DHS_MDIO), // 1.2v goes thru NI0
                            portad,
                            dev,
                            reg ) );  
#endif
}
                                                                                                         
/***************************************************************
* mdio_read_3_3v
**************************************************************/
uint16_t mdio_read_3_3v( int asic_id, uint8_t portad, uint8_t dev, uint16_t reg)
{
#ifdef SAT_NUOVA_SIM
    return( mdio_read( portad, dev, reg ) );
#else
    return( mdio_read_thru_redwood( asic_id,
                            BA(RED_NI1_DHS_MDIO), // 3.3v goes thru NI1
                            portad,
                            dev,
                            reg ) );  
#endif
}
                                                                                                         
/***************************************************************
* mdio_write_gbe
**************************************************************/
void mdio_write_gbe( int asic_id, uint8_t portad, uint8_t dev, uint16_t reg, uint16_t data )
{
#ifdef SAT_NUOVA_SIM
    mdio_write( portad, dev, reg, data );
#else
    // FEX-TBD
    unsigned int ctrl_word, mdio_sts;
    int tout = 10;

//    printf("MDIO (gbe) Write portad: %d, reg: %xh = %xh\n", 
//		portad, reg, data );

    /* make sure MDIO master is ready for an op */
    /* Note: This can be removed once the GBE MDIO core has been verified */
    mdio_reg_read( asic_id, BA(RED_CI0_GBE_STA_MII), &mdio_sts);
    if (mdio_sts & (1 /*busy*/ << 16)) { // still busy, problem
        printf(" Clause 22 MDIO master Busy before write"
		" (portad: %d, reg: %d, data: %04x)\n", portad, reg, data);
        return;
    }

    /* make sur "go" bit is clear to begin with */
    /* Note: This can be removed once the GBE MDIO core has been verified */
    mdio_reg_read( asic_id, BA(RED_CI0_GBE_CFG_MII), &ctrl_word);
    if (ctrl_word & (1 << 28)) { // "go" already set, problem
        printf(" Clause 22 MDIO master go-bit set before write"
		" (portad: %d, reg: %d, data: %04x)\n",
                 portad, reg, data);
        mdio_reg_write( asic_id, BA(RED_CI0_GBE_CFG_MII), 0);
	/* See "flush note:" comment below */
        mdio_flush_gbe( asic_id, BA(RED_CI0_GBE_CFG_MII)); 
        mdio_reg_read(  asic_id, BA(RED_CI0_GBE_CFG_MII), &ctrl_word);
        if (ctrl_word & (1 << 28)) { // "go" already set, problem
            printf(" Clause 22 MDIO master go-bit unclearable"
		   " (portad: %d, reg: %d, data: %04x)\n",
                     portad, reg, data);
            return;
        }
    }

    ctrl_word = (data & 0xFFFF)    |
                (portad     << 21) |
                (reg        << 16) |
                (0 /*write*/<< 26) |
                (skip_gbe_mdio_preamble << 27) |
                (0 /*go*/   << 28);

    /* issue ctrl word with "go" bit set. Should trigger MDIO read op */
     mdio_reg_write( asic_id, BA(RED_CI0_GBE_CFG_MII), ctrl_word);

    ctrl_word |= (1 /*go*/   << 28);
    /* issue ctrl word with "go" bit set. Should trigger MDIO read op */
     mdio_reg_write( asic_id, BA(RED_CI0_GBE_CFG_MII), ctrl_word);

    /* wait for done */
    do {
        mdio_reg_read( asic_id, BA(RED_CI0_GBE_STA_MII), &mdio_sts);
    } while ((mdio_sts & (1 /*busy*/ << 16)) && (--tout > 0));

    /* see if write ok or timeout */
    if (mdio_sts & (1 /*busy*/ << 16)) { // still busy, problem
        printf(" Clause 22 MDIO master Busy wait WRITE TIMEOUT"
	       " (portad: %d, reg: %d, data: %04x)\n",
                 portad, reg, data);
        data = 0xffff;
    }
    /* clear the "go" bit for next time */
    mdio_reg_write( asic_id, BA(RED_CI0_GBE_CFG_MII), 0 );

    /* FLUSH NOTE:
    *
    * The following flushes are needed to avoid the following 
    * race condition:
    * If the Write(CFG_MII,0) and the Write(CFG_MII,some_data_go_is_set) 
    * for the next operation
    * happen back to back in rnaxi, then the pulse that needs to cross 
    * the async boundary is too short
    * and it's missed by the slower destination clock.
    * The gbe_wrapper logic does a posedge detection to trigger 
    * the state machine:
    * CFG_mii_go_d1 <= #1 CFG_mii_go;
    * mii_go_pulse <= #1 CFG_mii_go & ~CFG_mii_go_d1;
    * As a result, the go is never seen, busy is never set,
    * and we get garbage back in the read(CFG_STA), regardless of 
    * how many reads you make after the write. */
    mdio_flush_gbe( asic_id, BA(RED_CI0_GBE_CFG_MII));
    return;

#endif

}
                                                                                                         
/***************************************************************
* mdio_write_1_2v
**************************************************************/
void mdio_write_1_2v( int asic_id, uint8_t portad, uint8_t dev, uint16_t reg, uint16_t data )
{
#ifdef SAT_NUOVA_SIM
    mdio_write( portad, dev, reg, data );
#else
     mdio_write_thru_redwood( asic_id,
                              BA(RED_NI0_DHS_MDIO), // 1.2v goes thru NI0
                              portad,
                              dev,
                              reg,
                              data );
#endif
}
                                                                                                         
/***************************************************************
* mdio_write_3_3v
**************************************************************/
void mdio_write_3_3v( int asic_id, uint8_t portad, uint8_t dev, uint16_t reg, uint16_t data )
{
#ifdef SAT_NUOVA_SIM
    mdio_write( portad, dev, reg,data );
#else
     mdio_write_thru_redwood( asic_id,
                              BA(RED_NI1_DHS_MDIO), // 3.3v goes thru NI1
                              portad,
                              dev,
                              reg,
                              data );
#endif
}


#ifdef USD
/*************************************************************
* mdio_set_logging
*************************************************************/
void mdio_set_logging( int on_or_off )
{
    p_ctx->log_mdios = on_or_off;
}

/*************************************************************
* mdio_logging_enabled
*************************************************************/
int mdio_logging_enabled(void)
{
    return( p_ctx->log_mdios );
}


#endif
