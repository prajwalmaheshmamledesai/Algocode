/********************************************************************
 *
 *      File:   redwood_asic.c
 *      Name:   Bruce McLoughlin
 *
 *      Description: ASIC level functions for Redwood USD
 *
 *
 *      Copyright (c) 2001,2002,2003,2004 by Nuova Systems, Inc.
 ********************************************************************/

#include "red_hw_lib.h"
#include "redwood_mdio.h"
#include "redwood_serdes_hw_seq.h"
#include "red_red_naxislave_define.h"
#include "redwood_mac_hw_seq.h"
#include "redwood_portmap.h"

char *rdw_port_names[] = {
	"cif",
	"bif",
	"hif0",
	"hif1",
	"hif2",
	"hif3",
	"hif4",
	"hif5",
	"hif6",
	"hif7",
	"nif0",
	"nif1",
	"nif2",
	"nif3",
	"UNKNOWN",
};

uint32_t dhs_base_addr[8] = 
{
	BA(RED_HI0_DHS_FIN),
	BA(RED_HI1_DHS_FIN),
	BA(RED_HI2_DHS_FIN),
	BA(RED_HI3_DHS_FIN),
	BA(RED_HI4_DHS_FIN),
	BA(RED_HI5_DHS_FIN),
	BA(RED_HI6_DHS_FIN),
	BA(RED_HI7_DHS_FIN),
};

static uint32_t bits_set(uint8_t a, uint8_t b)   
{
	return (((((0x01ull << ((a)+1)) - 1) >> (b)) << (b)));
}


unsigned int asic_read( int asic_id, unsigned int addr )
{
    unsigned int data;
    red_reg_rd(asic_id, addr, &data);
    return(data);
}

void asic_write( int asic_id, unsigned int addr, unsigned int data )
{
    red_reg_wr(asic_id, addr, data);
}

/*****************************************************************
* asic_init_gpio_pins
*****************************************************************/
void asic_init_gpio_pins( int asic_id, unsigned int output_mask )
{
    /* set OE for output pins according to mask/ Max of 24 pins */
    asic_write( asic_id, BA(RED_MISC_GPIO_DATA_OE), (output_mask & 0x00FFFFFF) );

    /* flush the write */
    asic_read(  asic_id, BA(RED_MISC_MISC_VERSION) );
}

void redwood_octphy_unreset(int asic_id)
{
    uint32_t data = 0;
    printf("%d:   Set the gpio_data_out register first\n", asic_id );
    data  = asic_read( asic_id, BA(RED_MISC_GPIO_DATA_OUT) );
    data |= 0x01; //(SFP_PHY_RST_L, KR_PHY_RST_L)
    asic_write( asic_id,  BA(RED_MISC_GPIO_DATA_OUT), data );
}

void redwood_octphy_reset(int asic_id)
{
    uint32_t data = 0;
    printf("%d:   Set the gpio_data_out register first\n", asic_id );
    data  = asic_read( asic_id, BA(RED_MISC_GPIO_DATA_OUT) );
    data &= ~0x01; //(SFP_PHY_RST_L, KR_PHY_RST_L)
    asic_write( asic_id,  BA(RED_MISC_GPIO_DATA_OUT), data );
}

void redwood_bcmsrd_unreset(int asic_id)
{
    uint32_t data = 0;
    data  = asic_read( asic_id, BA(RED_MISC_GPIO_DATA_OUT) );
    if (asic_id < 6) {
       data |= 0x02; //(SFP_PHY_RST_L, KR_PHY_RST_L)
    } else {
       data |= (0x01 << 11); //(SFP_PHY_RST_L, KR_PHY_RST_L)
    }
    asic_write( asic_id,  BA(RED_MISC_GPIO_DATA_OUT), data );
}

void redwood_bcmsrd_reset(int asic_id)
{
    uint32_t data = 0;
    data  = asic_read( asic_id, BA(RED_MISC_GPIO_DATA_OUT) );
    
    if (asic_id < 6) {
       data &= ~0x02; //(SFP_PHY_RST_L, KR_PHY_RST_L)
    } else {
       data &= ~(0x01 << 11); //(SFP_PHY_RST_L, KR_PHY_RST_L)
    }
    asic_write( asic_id,  BA(RED_MISC_GPIO_DATA_OUT), data );
}

void redwood_phy_unreset(int asic_id)
{
    uint32_t data = 0;
    printf("%d:   Set the gpio_data_out register first\n", asic_id );
    data  = asic_read( asic_id, BA(RED_MISC_GPIO_DATA_OUT) );
#ifdef DIAG_FEX
    data |= 0x3; //(SFP_PHY_RST_L, KR_PHY_RST_L)
#else
    data |= 0x300; //(SFP_PHY_RST_L, KR_PHY_RST_L)
#endif
    asic_write( asic_id,  BA(RED_MISC_GPIO_DATA_OUT), data );
}

void redwood_phy_reset(int asic_id)
{
    uint32_t data = 0;
    printf("%d:   Set the gpio_data_out register first\n", asic_id );
    data  = asic_read( asic_id, BA(RED_MISC_GPIO_DATA_OUT) );
#ifdef DIAG_FEX
    data &= ~0x3; //(SFP_PHY_RST_L, KR_PHY_RST_L)
#else
    data &= ~0x300; //(SFP_PHY_RST_L, KR_PHY_RST_L)
#endif
    asic_write( asic_id,  BA(RED_MISC_GPIO_DATA_OUT), data );
}

void redwood_hw_reset (int reset_fd, uint8_t inst)
{
    // Reset the Redwood ASIC itself
    printf("  Redwood-%d set ASIC reset (RDWD_HRESET)\n", inst);
    red_hw_asic_reset(reset_fd, inst);

    usleep(10000); // 10ms

    // reset PLLs
    printf("  Redwood-%d set PLL reset\n", inst);
    red_hw_pll_reset(reset_fd, inst);
    usleep(10000); // 10ms
}

void redwood_hw_unreset (int reset_fd, uint8_t inst)
{
    printf("  Redwood-%d Release PLL (SYS/NI/HI) resets\n", inst);
    // Un-reset PLLs
    red_hw_pll_unreset(reset_fd, inst);

    printf("  Redwood-%d Wait for PLLs to lock\n", inst);
    // Wait 10ms for PLLs to lock
    usleep(10000); // 10ms

    printf("  Redwood-%d Release reset (RDWD_HRESET)\n", inst);
    // Un-Reset the Redwood ASIC itself
    red_hw_asic_unreset(reset_fd, inst);
}

int redwood_get_sfp_mask(int inst, int asic_id, uint8_t *sfp_mask)
{
    uint32_t data, rc;

    rc = red_reg_rd(inst, BA(RED_MISC_GPIO_DATA_IN), &data);
    if(rc) {
        printf("Error: Failed to get SFP MASK\n");
        return (rc);
    }
#ifdef DIAG_FEX
    *sfp_mask = ((((data >> 8) & 0x01) ? 0x00  : 0x01) |
    		 (((data >> 9) & 0x01) ? 0x00  : (0x01 << 1)) |
    		 (((data >> 20) & 0x01) ? 0x00 : (0x01 << 2)) |
    		 (((data >> 21) & 0x01) ? 0x00 : (0x01 << 3)));

#else
    *sfp_mask = ((((data >> 3) & 0x01) ? 0x00  : 0x01) |
                 (((data >> 2) & 0x01) ? 0x00  : 0x02) |
                 (((data >> 1) & 0x01) ? 0x00 :  0x04) |
                 (((data >> 0) & 0x01) ? 0x00 :  0x08));

#endif

    return (rc);
}

void redwood_fex_gpio_init(int asic_id) 
{
    unsigned int data;
    if (asic_id < 6) {
    	asic_init_gpio_pins( asic_id, 0x00000003 );

    	data  = asic_read( asic_id, BA(RED_MISC_GPIO_DATA_OUT) );
	data &= ~0x3; //(SFP_PHY_RST_L, KR_PHY_RST_L)
	asic_write( asic_id,  BA(RED_MISC_GPIO_DATA_OUT), data );
	usleep(1000);
	data |= 0x3; //(SFP_PHY_RST_L, KR_PHY_RST_L)
	asic_write( asic_id,  BA(RED_MISC_GPIO_DATA_OUT), data );
    } else if (asic_id == 6) {

    	asic_init_gpio_pins( asic_id, 0x004C0CC0 );
	data = 0x00;
	asic_write( asic_id,  BA(RED_MISC_GPIO_DATA_OUT), data );
	usleep(1000);
	data = 0x400C00; 
	asic_write( asic_id,  BA(RED_MISC_GPIO_DATA_OUT), data );
    }
}
/***************************************************************
* redwood_bring_out_of_reset
*
* Description:
*
* Un-reset PLLs 
*   RDWD_SYS_PLL_RST_N <- 1
*   RDWD_NI_PLL_RST_N  <- 1
*   RDWD_HI_PLL_RST_N  <- 1
*
* wait for PLLs to lock (10ms)
*
* Un-reset Redwood
*   RDWD_HRESET_L <- 1
*
* Un-reset the PHYs
*   Set the gpio data output register first
*       RED_MISC_GPIO_DATA_OUT |= 0x300 (SFP_PHY_RST_L | KR_PHY_RST_L)
*
*   Then configure GPIO[9:8] as Outputs
*       RED_MISC_GPIO_DATA_OE |= 0x300 (SFP_PHY_RST_L | KR_PHY_RST_L)
*
* Enable NI0/1 MDIO Master OE
*   red_misc_mdio_mdc_oe <- 0x03 (xgbe12 | xgbe33)
*
* Unreset NI0/1 MDIO Master sw_rst
*   red_ni0_mdio_master &= ~(0x10000) (~sw_rst)
*   red_ni1_mdio_master &= ~(0x10000) (~sw_rst)
*
* Load the PHY microcode from SPROM
*   foreach PHY {
*       phy_load_microcode()
*   }
*
* Parameters:
*  asic_id = i2c file descriptor
*
* Returns:
*   nothing
*****************************************************************/
void redwood_bring_out_of_reset( int acc_fd , int reset_fd, int asic_id)
{
    unsigned int data;

    redwood_hw_unreset (reset_fd, asic_id);

//    printf("  Initialize MDIO\n");
    redwood_mdio_init( asic_id );



#ifdef DIAG_FEX
    redwood_fex_gpio_init(asic_id);
#else
    printf("  Release the PHY resets (SFP_PHY_RST_L, KR_PHY_RST_L)\n");
    printf("  Set the gpio_data_out register first\n");
    data  = asic_read( asic_id, BA(RED_MISC_GPIO_DATA_OUT) );
    data |= 0x300; //(SFP_PHY_RST_L, KR_PHY_RST_L)
    asic_write( asic_id,  BA(RED_MISC_GPIO_DATA_OUT), data );
    printf("  Configure GPIO[9:8] as outputs\n");
    asic_init_gpio_pins( asic_id, 0x00000300 );
#endif

    // RED_MISC_CFG_AVAGO
    data = 0x02;
    asic_write( asic_id,  BA(RED_MISC_CFG_AVAGO), data );
    data = 0x00;
    asic_write( asic_id,  BA(RED_MISC_CFG_AVAGO), data );
    data = 0x02;
    asic_write( asic_id,  BA(RED_MISC_CFG_AVAGO), data );
    data = 0x03;
    asic_write( asic_id,  BA(RED_MISC_CFG_AVAGO), data );

    data = asic_read( asic_id,  BA(RED_MISC_CFG_AVAGO));
}

/***************************************************************
* asic_reset_configuration
*
* Description:
* Bring up serdes (required for CLK)
* Reset the MACs
* Un-reset the MACs
* Un-reset all non-PORT related blocks (NF,HR,NR)
* Bring Down all ports again (except CI)
*
* Leaves all PORT related MACs (except CI) in reset
*
* Parameters:
*  asic_id
*
* Returns:
*   nothing
*****************************************************************/
uint8_t tx_pol_inv[12] = {0x00, 0x00, 0x00, 0x00,
			  0x00, 0x00, 0x00, 0x00,
			  0x00, 0x00, 0x00, 0x00};
uint8_t rx_pol_inv[12] = {0x00, 0x00, 0x00, 0x00,
			  0x00, 0x00, 0x00, 0x00,
			  0x00, 0x00, 0x00, 0x00};
	

void asic_reset_configuration( int asic_id )
{
    unsigned int misc_rst;
    int          sbus_port, lane;
    uint8_t	 speed = 0, rc = 0;
    rdw_pmap_t *prdw = NULL;

    /* adjust sbus clk (global) */
    serdes_init_0( asic_id );

    rc = redwood_get_port_information(asic_id, &prdw);
    if (rc || !prdw) {
        printf("Error: %s: redwood_get_port_information asic_id=%d\n", __func__, asic_id);
	return;
    }

    /* for each port, program the serdes */
    for (sbus_port = 0; sbus_port < 12; sbus_port++) {

	rc = redwood_get_port_speed(asic_id,  
			(sbus_port < 8) ? PORT_TYPE_HIF : PORT_TYPE_NIF, 
			(sbus_port < 8) ? sbus_port : sbus_port-8, &speed);
	if (rc) {
		printf("Error getting speed., Default 10G\n");
		speed = MEDIA_TYPE_SFPPLUS;
	}
        serdes_init_1(asic_id, sbus_port, 0, 4, 
			(speed == MEDIA_TYPE_1000BASET) ? 1 : 0 );
        serdes_rx_rdy_wait( asic_id, sbus_port, 0, 4 );
        serdes_init_2(      asic_id, sbus_port, 0, 4 );

        /* handle polarity inversion */
        for (lane = 0; lane < 4; lane++) {
	    rx_pol_inv[sbus_port] = (sbus_port < 8) ? 
					prdw->hif_rx_pol[sbus_port] :
					prdw->nif_rx_pol[sbus_port&0x3];
            if (rx_pol_inv[sbus_port] & (1 << lane)) {
                serdes_invert_polarity_on_rx_lane( asic_id, sbus_port, lane );
	    }

	    tx_pol_inv[sbus_port] = (sbus_port < 8) ? 
					prdw->hif_tx_pol[sbus_port] :
					prdw->nif_tx_pol[sbus_port&0x3];
            if (tx_pol_inv[sbus_port] & (1 << lane)) {
                serdes_invert_polarity_on_tx_lane( asic_id, sbus_port, lane );
	    }
        }
    }


    /* apply sw resets to the MACs (NI and HI) */
    asic_write( asic_id, BA(RED_HI0_SW_RST), ~0 );
    asic_write( asic_id, BA(RED_HI1_SW_RST), ~0 );
    asic_write( asic_id, BA(RED_HI2_SW_RST), ~0 );
    asic_write( asic_id, BA(RED_HI3_SW_RST), ~0 );
    asic_write( asic_id, BA(RED_HI4_SW_RST), ~0 );
    asic_write( asic_id, BA(RED_HI5_SW_RST), ~0 );
    asic_write( asic_id, BA(RED_HI6_SW_RST), ~0 );
    asic_write( asic_id, BA(RED_HI7_SW_RST), ~0 );

    asic_write( asic_id, BA(RED_NI0_SW_RST), ~0 );
    asic_write( asic_id, BA(RED_NI1_SW_RST), ~0 );
    asic_write( asic_id, BA(RED_NI2_SW_RST), ~0 );
    asic_write( asic_id, BA(RED_NI3_SW_RST), ~0 );

    printf("  Redwood-%d Release NF, CI, HR, NR from reset\n", asic_id);

    /* release all sw resets */
    misc_rst = asic_read(  asic_id, BA(RED_MISC_SW_RST) );
    misc_rst &= ~(0x1F000); // un-reset NF, BI, CI, HR, NR
    asic_write( asic_id, BA(RED_MISC_SW_RST), misc_rst);

    asic_read(  asic_id, BA(RED_MISC_MISC_VERSION) );

    sleep(1);

    /* Unreset Mac's */
    asic_write( asic_id, BA(RED_HI0_SW_RST), 0 );
    asic_write( asic_id, BA(RED_HI1_SW_RST), 0 );
    asic_write( asic_id, BA(RED_HI2_SW_RST), 0 );
    asic_write( asic_id, BA(RED_HI3_SW_RST), 0 );
    asic_write( asic_id, BA(RED_HI4_SW_RST), 0 );
    asic_write( asic_id, BA(RED_HI5_SW_RST), 0 );
    asic_write( asic_id, BA(RED_HI6_SW_RST), 0 );
    asic_write( asic_id, BA(RED_HI7_SW_RST), 0 );

    asic_write( asic_id, BA(RED_NI0_SW_RST), 0 );
    asic_write( asic_id, BA(RED_NI1_SW_RST), 0 );
    asic_write( asic_id, BA(RED_NI2_SW_RST), 0 );
    asic_write( asic_id, BA(RED_NI3_SW_RST), 0 );

    /* unmask all non-PORT related error interrupt sources */
//    regmap_unmask_all_error_interrupt_sources( asic_id );
}



/*****************************************************************************
 * From Patricio.
 *
 * Undocumented contents of red_misc_rnaxi_interrupt_read. Each bit 
 * corresponds to potentially multiple interrupt sources.
 */
typedef struct rnaxi_int_to_reg_map_t {
    unsigned int int_register;
    unsigned int bit_mask;
    int          asic_port; // -1 if not port related
} rnaxi_int_to_reg_map_t;

rnaxi_int_to_reg_map_t rnaxi_int_to_reg_map[] = { 
     {RED_MISC_INT_ERR     , 1<<0, -1 }
   , {RED_MISC_INT_GPIO_DATA_IN, 1<<0, -1}
   , {RED_HI0_GBE_INT      , 1<<1, hif0 }
   , {RED_HI0_INTR         , 1<<1, hif0 }
   , {RED_HI1_GBE_INT      , 1<<2, hif1 }
   , {RED_HI1_INTR         , 1<<2, hif1 }
   , {RED_HI2_GBE_INT      , 1<<3, hif2 }
   , {RED_HI2_INTR         , 1<<3, hif2 }
   , {RED_HI3_GBE_INT      , 1<<4, hif3 }
   , {RED_HI3_INTR         , 1<<4, hif3 }
   , {RED_HI4_GBE_INT      , 1<<5, hif4 }
   , {RED_HI4_INTR         , 1<<5, hif4 }
   , {RED_HI5_GBE_INT      , 1<<6, hif5 }
   , {RED_HI5_INTR         , 1<<6, hif5 }
   , {RED_HI6_GBE_INT      , 1<<7, hif6 }
   , {RED_HI6_INTR         , 1<<7, hif6 }
   , {RED_HI7_GBE_INT      , 1<<8, hif7 }
   , {RED_HI7_INTR         , 1<<8, hif7 }
   , {RED_NI0_INTR         , 1<<9, nif0 }
   , {RED_NI1_INTR         , 1<<10, nif1}
   , {RED_NI2_INTR         , 1<<11, nif2}
   , {RED_NI3_INTR         , 1<<12, nif3}
   , {RED_NF_INT_VEC       , 1<<13, -1 }
            
   , {RED_SS0_INT_NORM     , 1<<14, -1 }
   , {RED_SS0_INT_OQ0      , 1<<14, -1 }
   , {RED_SS0_INT_OQ1      , 1<<14, -1 }
   , {RED_SS0_INT_OQ2      , 1<<14, -1 }
   , {RED_SS0_INT_OQ3      , 1<<14, -1 }
   , {RED_SS0_INT_OQ4      , 1<<14, -1 }
   , {RED_SS0_INT_ERR      , 1<<14, -1 }
   , {RED_SS0_INT_ERR_ECC1 , 1<<14, -1 }
   , {RED_SS0_INT_ERR_ECC2 , 1<<14, -1 }
           
   , {RED_SS1_INT_NORM     , 1<<15, -1 }
   , {RED_SS1_INT_OQ0      , 1<<15, -1 }
   , {RED_SS1_INT_OQ1      , 1<<15, -1 }
   , {RED_SS1_INT_OQ2      , 1<<15, -1 }
   , {RED_SS1_INT_OQ3      , 1<<15, -1 }
   , {RED_SS1_INT_OQ4      , 1<<15, -1 }
   , {RED_SS1_INT_ERR      , 1<<15, -1 }
   , {RED_SS1_INT_ERR_ECC1 , 1<<15, -1 }
   , {RED_SS1_INT_ERR_ECC2 , 1<<15, -1 }
   
   , {RED_SS2_INT_NORM     , 1<<16, -1 }
   , {RED_SS2_INT_OQ0      , 1<<16, -1 }
   , {RED_SS2_INT_OQ1      , 1<<16, -1 }
   , {RED_SS2_INT_OQ2      , 1<<16, -1 }
   , {RED_SS2_INT_OQ3      , 1<<16, -1 }
   , {RED_SS2_INT_OQ4      , 1<<16, -1 }
   , {RED_SS2_INT_ERR      , 1<<16, -1 }
   , {RED_SS2_INT_ERR_ECC1 , 1<<16, -1 }
   , {RED_SS2_INT_ERR_ECC2 , 1<<16, -1 }
         
   , {RED_SS3_INT_NORM     , 1<<17, -1 }
   , {RED_SS3_INT_OQ0      , 1<<17, -1 }
   , {RED_SS3_INT_OQ1      , 1<<17, -1 }
   , {RED_SS3_INT_OQ2      , 1<<17, -1 }
   , {RED_SS3_INT_OQ3      , 1<<17, -1 }
   , {RED_SS3_INT_OQ4      , 1<<17, -1 }
   , {RED_SS3_INT_ERR      , 1<<17, -1 }
   , {RED_SS3_INT_ERR_ECC1 , 1<<17, -1 }
   , {RED_SS3_INT_ERR_ECC2 , 1<<17, -1 }
   , {RED_CI0_INT_VEC      , 1<<18, cif}
   , {RED_CI0_INT_ERR_VEC  , 1<<18, cif}
   , {RED_CI0_GBE_INT      , 1<<18, cif}
   , {RED_BI0_GBE_INT      , 1<<19, bif}
   , {RED_BI0_INTR         , 1<<19, bif}
};
 

void asic_handle_interrupts( int asic_id, unsigned int int_bits )
{
    uint32_t i;
    unsigned int active_ints;

    for (i = 0; i < sizeof(rnaxi_int_to_reg_map)/sizeof(rnaxi_int_to_reg_map[0]); i++)
    {
        if (int_bits & rnaxi_int_to_reg_map[i].bit_mask)
        {
            /* read the corresponding interrupt register */
            reg_read( asic_id, BA(rnaxi_int_to_reg_map[i].int_register), &active_ints );
            if (active_ints)
            {
                /* process the interrupt(s) */
                if (rnaxi_int_to_reg_map[i].int_register == RED_MISC_INT_GPIO_DATA_IN) {
                    /* process GPIO interrupts -- SFP presence */
//                    asic_update_gpio_sources( asic_id, active_ints );
                }

                /* clear the interrupt sourc */
                reg_write( asic_id, BA(rnaxi_int_to_reg_map[i].int_register), active_ints );
            }
        }
    }
}


int mac_init_cif (int asic_id)
{
   mac_write(asic_id, BA(RED_CI0_PORT_CFG), 0x803);	// VNTAG mode parsing
   mac_write(asic_id, BA(RED_CI0_NON_FCOE_RX_DEFAULT_VNTAG), 0x10000);
   return (0);
}

#if 0
#define LANE_MAP(a,b,c,d)       (((a)<<6) | ((b) <<4) | ((c) << 2) | ((d)))

int hif_tx_pol[8] = {8, 2, 0, 0, 8, 7, 0, 0};
int hif_rx_pol[8] = {0, 0, 0, 0, 0, 0, 0, 0};

                           /*    MTU  PAUSE  RxLnMap           ipg             */
mac_cfg_t hif_cfg[8] = {
                { -1, 9216, 0xdc, LANE_MAP(0,1,2,3), LANE_MAP(3,2,0,1), 0, 0, 0xc, 0, 1, 0, 0 },
                { -1, 9216, 0xdc, LANE_MAP(0,1,2,3), LANE_MAP(3,1,0,2), 0, 0, 0xc, 0, 1, 0, 0 },
                { -1, 9216, 0xdc, LANE_MAP(0,1,2,3), LANE_MAP(0,1,2,3), 0, 0, 0xc, 0, 1, 0, 0 },
                { -1, 9216, 0xdc, LANE_MAP(0,1,2,3), LANE_MAP(0,1,2,3), 0, 0, 0xc, 0, 1, 0, 0 },
                { -1, 9216, 0xdc, LANE_MAP(0,1,2,3), LANE_MAP(2,0,1,3), 0, 0, 0xc, 0, 1, 0, 0 },
                { -1, 9216, 0xdc, LANE_MAP(0,1,2,3), LANE_MAP(2,0,1,3), 0, 0, 0xc, 0, 1, 0, 0 },
                { -1, 9216, 0xdc, LANE_MAP(0,1,2,3), LANE_MAP(0,1,2,3), 0, 0, 0xc, 0, 1, 0, 0 },
                { -1, 9216, 0xdc, LANE_MAP(0,1,2,3), LANE_MAP(3,1,0,2), 0, 0, 0xc, 0, 1, 0, 0 },
                };

int nif_tx_pol[4] = {0, 3, 8, 4};
int nif_rx_pol[4] = {0, 0, 0, 0};

mac_cfg_t nif_cfg[4] = {
                { -1, 9216, 0xdc, LANE_MAP(3,2,1,0), LANE_MAP(3,2,1,0), 0, 0, 0xc, 0, 1, 0, 0 },
                { -1, 9216, 0xdc, LANE_MAP(3,2,1,0), LANE_MAP(3,2,1,0), 0, 0, 0xc, 0, 1, 0, 0 },
                { -1, 9216, 0xdc, LANE_MAP(3,2,1,0), LANE_MAP(3,2,1,0), 0, 0, 0xc, 0, 1, 0, 0 },
                { -1, 9216, 0xdc, LANE_MAP(3,2,1,0), LANE_MAP(3,2,1,0), 0, 0, 0xc, 0, 1, 0, 0 },
                };
#endif

void redwood_lane_map_set(uint8_t asic_id, uint8_t hif, uint8_t tx, uint8_t port, uint8_t data)
{
	rdw_pmap_t *prdw = NULL;
	mac_cfg_t *pcfg;

	// Get port information here.,
	redwood_get_port_information(asic_id, &prdw);
	if (!prdw) {
		printf("Error: %s: redwood_get_port_information asic_id=%d\n", __func__, asic_id);
		return; 
	}

	pcfg = hif ? prdw->hif_cfg : prdw->nif_cfg;
	if (tx) {
		pcfg[port].tx_lane_map = data;
	} else {
		pcfg[port].rx_lane_map = data;
	}
}

void redwood_polarity_set(uint8_t asic_id, uint8_t hif, uint8_t tx, uint8_t port, uint8_t data)
{
	rdw_pmap_t *prdw = NULL;
	// Get port information here.,
	redwood_get_port_information(asic_id, &prdw);
	if (!prdw) {
		printf("Error: %s: redwood_get_port_information asic_id=%d\n", __func__, asic_id);
		return;
	}
	if (hif) {
		if (tx) {
			prdw->hif_tx_pol[port] = data;
		} else {
			prdw->hif_rx_pol[port] = data;
		}
	} else {
		if (tx) {
			prdw->nif_tx_pol[port] = data;
		} else {
			prdw->nif_rx_pol[port] = data;
		}
	}
}

void redwood_lane_pol_print(uint8_t asic_id, uint8_t hif, uint8_t port)
{
	mac_cfg_t *pcfg;
	rdw_pmap_t *prdw = NULL;
	uint8_t   txmap;
	uint8_t   rxmap;

	// Get port information here.,
	redwood_get_port_information(asic_id, &prdw);
        printf("prdw = %p\n", prdw);
	if (!prdw) {
		printf("Error: %s: redwood_get_port_information asic_id=%d\n", __func__, asic_id);
		return;
	}
	pcfg = hif ? prdw->hif_cfg : prdw->nif_cfg;
	txmap = pcfg[port].tx_lane_map;
	rxmap = pcfg[port].rx_lane_map;

	printf("[%s%d] : Rx(%d,%d,%d,%d Pol=0x%X) Tx(%d,%d,%d,%d, Pol=0x%X)\n",
		hif? "hif" : "nif", port, 
		(rxmap >> 6) & 3,
		(rxmap >> 4) & 3,
		(rxmap >> 2) & 3,
		(rxmap >> 0) & 3,
		(hif ? prdw->hif_rx_pol[port] : prdw->nif_rx_pol[port]),
		(txmap >> 6) & 3,
		(txmap >> 4) & 3,
		(txmap >> 2) & 3,
		(txmap >> 0) & 3,
		(hif ? prdw->hif_tx_pol[port] : prdw->nif_tx_pol[port]));
}

void redwood_lane_pol_print_all(uint8_t asic_id)
{
	int	   port;

	for (port = 0; port < 8; port++) {
		redwood_lane_pol_print(asic_id, 1, port);
	}
	for (port = 0; port < 4; port++) {
		redwood_lane_pol_print(asic_id, 0, port);
	}
}

  /* run POST functions (from diag group -- TBD) */
/***************************************************************
* asic_init 
*
* Description:
* Perform the actual ASIC initialization(s) on an ASIC instance.
*
* Parameters:
*  asic_id
*
* Returns:
*   int, 0=ok, otherwise error
*****************************************************************/
int redwood_core_init(int fd, uint8_t asic_id)
{
  /* Perform Serdes init */
//  printf("  Redwood-%d Configure Serdes\n", asic_id);
//  red_hw_configure_serdes(asic_id, 1, 0, 0xFFFFFFFF, 0xFFFF, 5, 0, 0, 0, 0);

  /* Reset ASIC Configuration to known state. All ports except CI down */
  printf("  Redwood-%d Asic reset configuration\n", asic_id);
  asic_reset_configuration( asic_id );

  /* run the SS init */
  printf("  Redwood-%d ss init\n", asic_id);
  asic_ss_init( asic_id );
  return (0);
}

int redwood_port_init(int fd, uint8_t asic_id)
{
  int rc, port;
  rdw_pmap_t *prdw = NULL;
  uint8_t is_1g = 0, speed;

  /* Set up CIF */
//  printf("  Redwood MAC CIF init\n");
  rc = mac_init_cif( asic_id);
  if (rc) {
	printf("Error: mac_init_cif rc=%d\n", rc);
	return (rc);
  }

  // 100MB for CIF.
//  printf("  Redwood MAC CIF H/W Sequence init\n");
  rc = mac_bring_up_cif_hw_seq(asic_id, 100);
  if (rc) {
	printf("Error: mac_bring_up_cif_hw_seq rc=%d\n", rc);
	return (rc);
  }

  // 1000MB for BIF
//  printf("  Redwood MAC BIF H/W Sequence init\n");
  rc = mac_bring_up_bif_hw_seq(asic_id, 1000);
  if (rc) {
	printf("Error: mac_bring_up_bif_hw_seq rc=%d\n", rc);
	return (rc);
  }

  // Get port information here.,
  rc = redwood_get_port_information(asic_id, &prdw);
  if (rc || !prdw) {
	printf("Error: %s: redwood_get_port_information asic_id=%d\n", __func__, asic_id);
	return (rc);
  }

  // HIF 
  for (port = 0; port < 8; port++) {
	redwood_get_port_speed(asic_id, PORT_TYPE_HIF, port, &speed);
	is_1g = (speed == MEDIA_TYPE_1000BASET) ? 1 : 0;
//  	printf("  Redwood%d MAC HIF%d %s H/W Sequence init\n", 
//			asic_id, port, is_1g? "1G" : "10G");
	if (is_1g) {
	rc = mac_bring_up_hif_1g_hw_seq(asic_id, port, &prdw->hif_cfg[port],
			prdw->hif_rx_pol[port], prdw->hif_tx_pol[port], 0);

	if (rc) return (rc);

	rc = mac_enable_hif_1g_hw_seq (asic_id, port);
	if (rc) return (rc);

	mac_deassert_flush_hif (asic_id, port);

	} else {
	rc = mac_bring_up_hif_hw_seq(asic_id, port, &prdw->hif_cfg[port], 
			prdw->hif_rx_pol[port], prdw->hif_tx_pol[port]);
	}
    	serdes_lower_sig_thresh( asic_id, port, 0, 4 );
  }

  // NIF
  for (port = 0; port < 4; port++) {
//  	printf("  Redwood%d MAC NIF%d H/W Sequence init\n", asic_id, port);
	rc = mac_bring_up_nif_hw_seq(asic_id, port, &prdw->nif_cfg[port], 
			prdw->nif_rx_pol[port], prdw->nif_tx_pol[port]);
   	serdes_lower_sig_thresh( asic_id, 8+port, 0, 4 );
  }

  return( rc );
}

int redwood_init(int fd, uint32_t asic_id)
{
	redwood_core_init(fd, asic_id);

	return (redwood_port_init(fd, asic_id));
}


int redwood_serdes_nif_init(int asic_id, int flag)
{
	int port;
	int rc = 0;
	rdw_pmap_t *prdw = NULL;
  	// Get port information here.,
	rc = redwood_get_port_information(asic_id, &prdw);
	if (rc || !prdw) {
		printf("Error: %s: redwood_get_port_information asic_id=%d\n", __func__, asic_id);
		return (rc);
	}

  	for (port = 0; port < 4; port++) {
//  		printf("  Redwood MAC NIF%d H/W Sequence init\n", port);
		rc = mac_bring_up_nif_hw_seq(asic_id, port, &prdw->nif_cfg[port], 
			prdw->nif_rx_pol[port], prdw->nif_tx_pol[port]);
  	}
	return(rc);
}

int redwood_serdes_hif_init(int asic_id, int flag)
{
	int port;
	int rc = 0;
	rdw_pmap_t *prdw = NULL;

  	// Get port information here.,
	rc = redwood_get_port_information(asic_id, &prdw);
	if (rc || !prdw) {
		printf("Error: %s: redwood_get_port_information asic_id=%d\n", __func__, asic_id);
		return (rc);
	}
	for (port = 0; port < 8; port++) {
		printf("  HIF%d mac program\n", port);
		mac_bring_up_hif_hw_seq(asic_id, port, &prdw->hif_cfg[port], 
				prdw->hif_rx_pol[port], prdw->hif_tx_pol[port]);
	}

	return(rc);
}

int redwood_serdes_hif_port_init(int asic_id, int port)
{
	int rc = 0;
	rdw_pmap_t *prdw = NULL;

  	// Get port information here.,
	rc = redwood_get_port_information(asic_id, &prdw);
	if (rc || !prdw) {
		printf("Error: %s: redwood_get_port_information asic_id=%d\n", __func__, asic_id);
		return (rc);
	}
	printf("  HIF%d mac program\n", port);
	mac_bring_up_hif_hw_seq(asic_id, port, &prdw->hif_cfg[port], 
			prdw->hif_rx_pol[port], prdw->hif_tx_pol[port]);
	return(rc);
}

int redwood_serdes_mac_init(int asic_id, int flag)
{
	int port, rc = 0;
	mac_cfg_t	mac_cfg;
	rdw_pmap_t *prdw = NULL;

	// Get port information here.,
	rc = redwood_get_port_information(asic_id, &prdw);
	if (rc || !prdw) {
		printf("Error: %s: redwood_get_port_information asic_id=%d\n", __func__, asic_id);
		return (rc);
	}

	for (port = 0; port < 8; port++) {
		printf("  HIF%d mac program\n", port);
		mac_bring_up_hif_hw_seq(asic_id, port, &prdw->hif_cfg[port], 
				prdw->hif_rx_pol[port], prdw->hif_tx_pol[port]);
	}

	for (port = 0; port < 4; port++) {
		printf("  NIF%d mac program\n", port);
		mac_cfg = prdw->nif_cfg[port];
		mac_cfg.modbits = mod_tx_lane_map | mod_rx_lane_map;
		mac_nif_hw_seq (asic_id, port, &prdw->nif_cfg[port], 
			prdw->nif_rx_pol[port], prdw->nif_tx_pol[port], flag);
	}

	return(0);
}

#define REDWOOD_HIF_CIF_VECTOR	0x04
#define REDWOOD_HIF_DROP_VECTOR	0x07
#define REDWOOD_HIF_DROP_ALL_VECTOR	0x77777777
void redwood_config_hif_drop(int inst)
{
	uint32_t addr;
	uint8_t hif, cnt;

	for (hif = 0; hif < 8; hif++) {
		addr = BA(RED_HI0_FWD_VECTOR0 + (hif * (RED_HI1_FWD_VECTOR0 - RED_HI0_FWD_VECTOR0)));

		for (cnt = 0; cnt < 32; cnt++) {
        		asic_write( inst, addr, REDWOOD_HIF_DROP_ALL_VECTOR);
			addr += BA(RED_HI0_FWD_VECTOR1 - RED_HI0_FWD_VECTOR0);
		}	
	}
}

void redwood_config_hif_vector(int inst, int src, uint8_t dest, uint8_t entry)
{
	uint32_t	data;
        uint32_t        addr = BA(RED_HI0_FWD_VECTOR0 +
                                (src * (RED_HI1_FWD_VECTOR0 - RED_HI0_FWD_VECTOR0)));

	addr += BA((entry >> 3) * (RED_HI0_FWD_VECTOR1 - RED_HI0_FWD_VECTOR0));

        data = asic_read( inst, addr);
	data &= ~(0x0f << (4* (entry & 0x07)));
	data |= (dest << (4*(entry&0x07)));
        asic_write( inst, addr, data);
}
 
void redwood_config_hif_cif(int inst, int port, int entry)
{
	redwood_config_hif_vector(inst, port, REDWOOD_HIF_CIF_VECTOR, entry);
}

void redwood_config_hif_nif(int inst, uint8_t hif, uint8_t nif, uint8_t entry)
{
	redwood_config_hif_vector(inst, hif, nif, entry);
}

void redwood_config_cif_nif(int inst, int port)
{
        asic_write( inst, BA(RED_CI0_NON_FCOE_RX_DEFAULT_VNTAG), (0x01 << (port+ 16)));
}


int redwood_snake_config(uint8_t asic_id, uint8_t fp_mask, uint8_t bp_mask)
{

    /* program the MAC forwarding entries to set up the following
    *  sequence for simple snake test (no flush testing):
    *   CIF -> NI0
    *   NI0 -> HI0
    *   HI0 -> NI1
    *   NI1 -> HI1
    *   HI1 -> NI2
    *   NI2 -> HI2
    *   HI2 -> NI3
    *   NI3 -> HI3
    *   HI3 -> CIF
    *
    * program the MAC forwarding entries to set up the following
    *  sequence for flush testing:
    *   CIF -> NI0
    *   NI0 -> HI0
    *   HI0 -> NI1
    *   NI1 -> HI1
    *   HI1 -> NI2
    *   NI2 -> HI2
    *   HI2 -> NI3
    *   NI3 -> HI3
    *   HI3 -> NI0 <- causes a forwarding loop (pkt is now "trapped forever")
    */
     asic_write( asic_id, BA(RED_NI0_NON_FCOE_RX_DEFAULT_VNTAG), (1 << 0) );
     asic_write( asic_id, BA(RED_NI1_NON_FCOE_RX_DEFAULT_VNTAG), (1 << 1) );
     asic_write( asic_id, BA(RED_NI2_NON_FCOE_RX_DEFAULT_VNTAG), (1 << 2) );
     asic_write( asic_id, BA(RED_NI3_NON_FCOE_RX_DEFAULT_VNTAG), (1 << 3) );

    /* configure NIx fwd ranges so as not to conflict with default vntag */
    invalidate_fwd_ranges( asic_id, BA(RED_NI0_HIF0_FWD_RANGE) );
    invalidate_fwd_ranges( asic_id, BA(RED_NI1_HIF0_FWD_RANGE) );
    invalidate_fwd_ranges( asic_id, BA(RED_NI2_HIF0_FWD_RANGE) );
    invalidate_fwd_ranges( asic_id, BA(RED_NI3_HIF0_FWD_RANGE) );

    /* program the NIF fwd ranges explicitly FOR default vntag (0) */
     asic_write( asic_id, BA(RED_NI0_HIF0_FWD_RANGE), 0x10000 );
     asic_write( asic_id, BA(RED_NI1_HIF1_FWD_RANGE), 0x10000 ); //0x10000
     asic_write( asic_id, BA(RED_NI2_HIF2_FWD_RANGE), 0x10000 ); //0x10000
     asic_write( asic_id, BA(RED_NI3_HIF3_FWD_RANGE), 0x10000 ); //0x10000

     asic_write( asic_id, BA(RED_HI0_FWD_VECTOR0), 1 );
     asic_write( asic_id, BA(RED_HI1_FWD_VECTOR0), 2 );
     asic_write( asic_id, BA(RED_HI2_FWD_VECTOR0), 3 );
     asic_write( asic_id, BA(RED_HI3_FWD_VECTOR0), 0 /*NI0*/);

     return (0);
}

extern unsigned int hif_ignore_signal_strength_tbl[];
extern unsigned int hif_mac_cfg_reg[];
int redwood_hif_loopback_cfg( int asic_id, int mac_id /*0..7*/ )
{
    unsigned int reg_val;

    asic_write( asic_id,
               BA(hif_ignore_signal_strength_tbl[ mac_id ]),
               ((1 << RED_HI_IGNORE_RX_SIG_STRENGTH_FLD_L0) |
                (1 << RED_HI_IGNORE_RX_SIG_STRENGTH_FLD_L1) |
                (1 << RED_HI_IGNORE_RX_SIG_STRENGTH_FLD_L2) |
                (1 << RED_HI_IGNORE_RX_SIG_STRENGTH_FLD_L3)) );

    /* straigthen out the lane mapping */
    reg_val = asic_read( asic_id, BA(hif_mac_cfg_reg[mac_id]));
    reg_val &= 0x01ffffff;
    reg_val |= bits(31,25,0x64);
    asic_write( asic_id, BA(hif_mac_cfg_reg[mac_id]), reg_val );
    asic_write( asic_id, BA(hif_mac_cfg_reg[mac_id]) + 4, 1 /*upper bit of lane 3*/ );
    reg_val = asic_read(  asic_id, BA(hif_mac_cfg_reg[mac_id]) + 8);
    reg_val &= 0xffffff00;
    reg_val |= 0xe4;
    asic_write(  asic_id, BA(hif_mac_cfg_reg[mac_id]) + 8, reg_val );

    /* un-do any polarity swapping that's already been programmed */
    serdes_normal_polarity_on_tx_lane( asic_id, mac_id, 0 );
    serdes_normal_polarity_on_tx_lane( asic_id, mac_id, 1 );
    serdes_normal_polarity_on_tx_lane( asic_id, mac_id, 2 );
    serdes_normal_polarity_on_tx_lane( asic_id, mac_id, 3 );

    mac_pgm_serdes( asic_id, mac_id, 0, 4, 0,0 /*no polarity swapping*/ );
    return 0;
}

static int redwood_hif_ignore_signal_strength( int asic_id, int mac_id /*0..7*/ )
{
    asic_write( asic_id,
               BA(hif_ignore_signal_strength_tbl[ mac_id ]),
               ((1 << RED_HI_IGNORE_RX_SIG_STRENGTH_FLD_L0) |
                (1 << RED_HI_IGNORE_RX_SIG_STRENGTH_FLD_L1) |
                (1 << RED_HI_IGNORE_RX_SIG_STRENGTH_FLD_L2) |
                (1 << RED_HI_IGNORE_RX_SIG_STRENGTH_FLD_L3)) );
    return (0);
}

extern unsigned int nif_ignore_signal_strength_tbl[];
static int redwood_nif_ignore_signal_strength( int asic_id, int mac_id /*0..3*/ )
{
    asic_write( asic_id,
               BA(nif_ignore_signal_strength_tbl[ mac_id ]),
               ((1 << RED_NI_IGNORE_RX_SIG_STRENGTH_FLD_L0) |
                (1 << RED_NI_IGNORE_RX_SIG_STRENGTH_FLD_L1) |
                (1 << RED_NI_IGNORE_RX_SIG_STRENGTH_FLD_L2) |
                (1 << RED_NI_IGNORE_RX_SIG_STRENGTH_FLD_L3)) );
    return (0);
}

int redwood_ignore_signal_strength(int asic_id)
{
    int mac_id = 0;

    for (mac_id = 0; mac_id < 8; mac_id++) {
	redwood_hif_ignore_signal_strength(asic_id, mac_id);
    }
    for (mac_id = 0; mac_id < 4; mac_id++) {
	redwood_nif_ignore_signal_strength(asic_id, mac_id);
    }

    return (0);
}


int redwood_normal_polarity(int asic_id)
{
    uint8_t mac_id;

    for (mac_id = 0; mac_id < 12; mac_id++) {
    	serdes_normal_polarity_on_tx_lane( asic_id, mac_id, 0 );
    	serdes_normal_polarity_on_tx_lane( asic_id, mac_id, 1 );
    	serdes_normal_polarity_on_tx_lane( asic_id, mac_id, 2 );
	serdes_normal_polarity_on_tx_lane( asic_id, mac_id, 3 );
    }
    return (0);
}

int redwood_rewrite_snake_config(uint8_t asic_id, uint8_t fp_mask, uint8_t bp_mask)
{
   uint32_t addr, hif, reg_val;

    /* program the MAC forwarding entries to set up the following
    *  sequence for simple snake test (no flush testing):
    *   CIF -> NI0
    *   NI0 -> HI0
    *   HI0 -> NI1
    *   NI1 -> HI1
    *   HI1 -> NI2
    *   NI2 -> HI2
    *   HI2 -> NI3
    *   NI3 -> HI3
    *   HI3 -> NI0
    *
    *   NI0 -> HI4
    *   HI4 -> NI1
    *   NI1 -> HI5
    *   HI5 -> NI2
    *   NI2 -> HI6
    *   HI6 -> NI3
    *   NI3 -> HI7
    *   HI7 -> NI0
    */
    asic_write( asic_id, BA(RED_NI0_NON_FCOE_RX_DEFAULT_VNTAG), (1 << 0) );
    asic_write( asic_id, BA(RED_NI1_NON_FCOE_RX_DEFAULT_VNTAG), (1 << 1) );
    asic_write( asic_id, BA(RED_NI2_NON_FCOE_RX_DEFAULT_VNTAG), (1 << 2) );
    asic_write( asic_id, BA(RED_NI3_NON_FCOE_RX_DEFAULT_VNTAG), (1 << 3) );

    /* configure NIx fwd ranges so as not to conflict with default vntag */
    invalidate_fwd_ranges( asic_id, BA(RED_NI0_HIF0_FWD_RANGE) );
    invalidate_fwd_ranges( asic_id, BA(RED_NI1_HIF0_FWD_RANGE) );
    invalidate_fwd_ranges( asic_id, BA(RED_NI2_HIF0_FWD_RANGE) );
    invalidate_fwd_ranges( asic_id, BA(RED_NI3_HIF0_FWD_RANGE) );

    for (hif = 0; hif < 8; hif++) {
	  // Program VIF max
	  addr = BA(RED_HI0_RX_VIF_MAX + hif*(RED_HI1_RX_VIF_MAX-RED_HI0_RX_VIF_MAX));
	  reg_val = 0xFFF;
     	  asic_write(asic_id, addr, reg_val);

	  // program HIF NON FCOE Default vntag.
	  addr = BA(RED_HI0_NON_FCOE_RX_DEFAULT_VNTAG + 
		(hif * (RED_HI1_NON_FCOE_RX_DEFAULT_VNTAG - RED_HI0_NON_FCOE_RX_DEFAULT_VNTAG)));
	  reg_val = (((hif+1)&7) << 16) | ((hif+1)&7);
	  asic_write(asic_id, addr, reg_val);

          // Clear VNTAG mode
	  addr = BA(RED_HI0_PORT_CFG + hif*(RED_HI1_PORT_CFG-RED_HI0_PORT_CFG));
	  reg_val = asic_read(asic_id, addr) &  
			~((0x01 << RED_HI_PORT_CFG_FLD_IS_VNTAG) |
			  (0x01 << RED_HI_PORT_CFG_FLD_RX_VNTAG_PASSTHRU) |
			  (0x01 << RED_HI_PORT_CFG_FLD_TX_VNTAG_PASSTHRU));
     	  asic_write(asic_id, addr, reg_val);
	  // Program Forwarding vector HIF0 - NIF1, HIF1 - NIF2.
	  addr = BA(RED_HI0_FWD_VECTOR0 + (hif * (RED_HI1_FWD_VECTOR0 - RED_HI0_FWD_VECTOR0)));
	  addr += BA((hif >> 3) * (RED_HI0_FWD_VECTOR1 - RED_HI0_FWD_VECTOR0));
	  reg_val = asic_read(asic_id, addr);
	  reg_val |= (((hif+1)&0x03) << (4 * ((hif+1)&0x07)));
	  asic_write(asic_id, addr, reg_val);
     }

     // Program Forwarding range for NIF's
     asic_write( asic_id, BA(RED_NI0_HIF0_FWD_RANGE), 0x10000 );
     asic_write( asic_id, BA(RED_NI1_HIF1_FWD_RANGE), 0x10001 ); 
     asic_write( asic_id, BA(RED_NI2_HIF2_FWD_RANGE), 0x20001 );
     asic_write( asic_id, BA(RED_NI3_HIF3_FWD_RANGE), 0x30001 );

     asic_write( asic_id, BA(RED_NI0_HIF4_FWD_RANGE), 0x40001 );
     asic_write( asic_id, BA(RED_NI1_HIF5_FWD_RANGE), 0x50001 );
     asic_write( asic_id, BA(RED_NI2_HIF6_FWD_RANGE), 0x60001 );
     asic_write( asic_id, BA(RED_NI3_HIF7_FWD_RANGE), 0x70001 );

     return (0);
}


int redwood_pif_config(uint8_t asic_id, uint8_t fp_mask, uint8_t bp_mask)
{
    uint32_t addr, hif, reg_val, nif;

    /* configure NIx fwd ranges so as not to conflict with default vntag */
    invalidate_fwd_ranges( asic_id, BA(RED_NI0_HIF0_FWD_RANGE) );
    invalidate_fwd_ranges( asic_id, BA(RED_NI1_HIF0_FWD_RANGE) );
    invalidate_fwd_ranges( asic_id, BA(RED_NI2_HIF0_FWD_RANGE) );
    invalidate_fwd_ranges( asic_id, BA(RED_NI3_HIF0_FWD_RANGE) );

     /* Configure to drop all */
//     redwood_config_hif_drop(asic_id);

     /* Program Default NIF VNTAGS */
     asic_write( asic_id, BA(RED_NI0_NON_FCOE_RX_DEFAULT_VNTAG), (1 << 0) );
     asic_write( asic_id, BA(RED_NI1_NON_FCOE_RX_DEFAULT_VNTAG), (1 << 1) );
     asic_write( asic_id, BA(RED_NI2_NON_FCOE_RX_DEFAULT_VNTAG), (1 << 2) );
     asic_write( asic_id, BA(RED_NI3_NON_FCOE_RX_DEFAULT_VNTAG), (1 << 3) );

    /* Forward VNTAG range from 0 - 0xFF to CIF */ 
    for(nif = 0; nif < 4; nif++) {
	 addr = BA(RED_NI0_CIF_FWD_RANGE + 
		   (nif * (RED_NI1_CIF_FWD_RANGE - RED_NI0_CIF_FWD_RANGE)));
         asic_write( asic_id, addr, ((0xFF) << 16));
    }

    // Program CIF to forward VNTAG packets. 
     addr = BA(RED_CI0_PORT_CFG);
     reg_val = asic_read(asic_id, addr) | bits_set(RED_CI_PORT_CFG_FLD_NIF_PORTS_VNTAG_MODE); 
     asic_write(asic_id, addr, reg_val);


     for (hif = 0; hif < 8; hif++) {
	  // Program VIF MAX
          addr = BA(RED_HI0_RX_VIF_MAX + hif*(RED_HI1_RX_VIF_MAX-RED_HI0_RX_VIF_MAX));
          reg_val = 0xFFF;
          asic_write(asic_id, addr, reg_val);

          // Program VNTAG mode
	  addr = BA(RED_HI0_PORT_CFG + hif*(RED_HI1_PORT_CFG-RED_HI0_PORT_CFG));
	  reg_val = asic_read(asic_id, addr) | 
			(0x01 << RED_HI_PORT_CFG_FLD_IS_VNTAG) |
			(0x01 << RED_CI_PORT_CFG_FLD_CIF_VNTAG_PRSING_EN) |
			(0x01 << RED_HI_PORT_CFG_FLD_RX_VNTAG_PASSTHRU) |
			(0x01 << RED_HI_PORT_CFG_FLD_TX_VNTAG_PASSTHRU);
     	  asic_write(asic_id, addr, reg_val);

	  // Program Forwarding vector HIF0 - CIF0, HIF1 - CIF0.
//	  redwood_config_hif_cif(asic_id, hif, hif+1);
	  redwood_config_hif_cif(asic_id, hif, (0x01 << hif));
     }

     return (0);
}


int redwood_hton_config(uint8_t asic_id, uint8_t fp_mask, uint8_t bp_mask)
{
    uint32_t addr, hif, reg_val, nif;

    /* configure NIx fwd ranges so as not to conflict with default vntag */
    invalidate_fwd_ranges( asic_id, BA(RED_NI0_HIF0_FWD_RANGE) );
    invalidate_fwd_ranges( asic_id, BA(RED_NI1_HIF0_FWD_RANGE) );
    invalidate_fwd_ranges( asic_id, BA(RED_NI2_HIF0_FWD_RANGE) );
    invalidate_fwd_ranges( asic_id, BA(RED_NI3_HIF0_FWD_RANGE) );

     // Program HIF to drop all packets.
     redwood_config_hif_drop(asic_id);

     /* Program Default NIF VNTAGS */
     asic_write( asic_id, BA(RED_NI0_NON_FCOE_RX_DEFAULT_VNTAG), 0x00 );
     asic_write( asic_id, BA(RED_NI1_NON_FCOE_RX_DEFAULT_VNTAG), 0x00 );
     asic_write( asic_id, BA(RED_NI2_NON_FCOE_RX_DEFAULT_VNTAG), 0x00 );
     asic_write( asic_id, BA(RED_NI3_NON_FCOE_RX_DEFAULT_VNTAG), 0x00 );

    /* Forward VNTAG range from 0 - 0xFF to CIF */ 
    for(nif = 0; nif < 4; nif++) {
	 addr = BA(RED_NI0_CIF_FWD_RANGE + 
		   (nif * (RED_NI1_CIF_FWD_RANGE - RED_NI0_CIF_FWD_RANGE)));
         asic_write( asic_id, addr, ((0xFF) << 16));
    }

    // Program CIF to forward VNTAG packets. 
    addr = BA(RED_CI0_PORT_CFG);
    reg_val = asic_read(asic_id, addr) | 
		bits_set(RED_CI_PORT_CFG_FLD_NIF_PORTS_VNTAG_MODE) |
		(0x01 << RED_CI_PORT_CFG_FLD_CIF_VNTAG_MODE) |
		(0x01 << RED_CI_PORT_CFG_FLD_CIF_VNTAG_PRSING_EN) |
		(0x01 << RED_CI_PORT_CFG_FLD_RX_VNTAG_PASSTHRU) |
		(0x01 << RED_CI_PORT_CFG_FLD_TX_VNTAG_PASSTHRU);
    asic_write(asic_id, addr, reg_val);

     for (hif = 0; hif < 8; hif++) {
	  // Program VIF MAX
          addr = BA(RED_HI0_RX_VIF_MAX + hif*(RED_HI1_RX_VIF_MAX-RED_HI0_RX_VIF_MAX));
          reg_val = 0xFFF;
          asic_write(asic_id, addr, reg_val);

          // Program HIFS in VNTAG mode
	  addr = BA(RED_HI0_PORT_CFG + hif*(RED_HI1_PORT_CFG-RED_HI0_PORT_CFG));
	  reg_val = asic_read(asic_id, addr) | 
			(0x01 << RED_HI_PORT_CFG_FLD_IS_VNTAG) |
			(0x01 << RED_HI_PORT_CFG_FLD_RX_VNTAG_PASSTHRU) |
			(0x01 << RED_HI_PORT_CFG_FLD_TX_VNTAG_PASSTHRU);
     	  asic_write(asic_id, addr, reg_val);

	  // Program Forwarding vector HIF0 - NIF0, HIF1 - NIF1.
	  // Program Forwarding vector HIF2 - NIF2, HIF3 - NIF3.
	  // Program Forwarding vector HIF4 - NIF0, HIF5 - NIF1.
	  // Program Forwarding vector HIF6 - NIF2, HIF7 - NIF3.
	  redwood_config_hif_nif(asic_id, hif, (hif & 0x03), 0x01 << hif);
     }

     return (0);
}


int redwood_ntoh_config(uint8_t asic_id, uint8_t fp_mask, uint8_t bp_mask)
{
    uint32_t addr, hif, reg_val, nif;

    /* configure NIx fwd ranges so as not to conflict with default vntag */
    invalidate_fwd_ranges( asic_id, BA(RED_NI0_HIF0_FWD_RANGE) );
    invalidate_fwd_ranges( asic_id, BA(RED_NI1_HIF0_FWD_RANGE) );
    invalidate_fwd_ranges( asic_id, BA(RED_NI2_HIF0_FWD_RANGE) );
    invalidate_fwd_ranges( asic_id, BA(RED_NI3_HIF0_FWD_RANGE) );

     redwood_config_hif_drop(asic_id);

     /* Program Default NIF VNTAGS */
     asic_write( asic_id, BA(RED_NI0_NON_FCOE_RX_DEFAULT_VNTAG), 0x00);
     asic_write( asic_id, BA(RED_NI1_NON_FCOE_RX_DEFAULT_VNTAG), 0x00);
     asic_write( asic_id, BA(RED_NI2_NON_FCOE_RX_DEFAULT_VNTAG), 0x00); 
     asic_write( asic_id, BA(RED_NI3_NON_FCOE_RX_DEFAULT_VNTAG), 0x00); 

    /* Forward VNTAG range from 0 - 0xFF to CIF */ 
    for(nif = 0; nif < 4; nif++) {
	 addr = BA(RED_NI0_HIF0_FWD_RANGE + 
		   (nif * (RED_NI1_HIF0_FWD_RANGE - RED_NI0_HIF0_FWD_RANGE)));
	 addr+= BA(nif * (RED_NI0_HIF1_FWD_RANGE - RED_NI0_HIF0_FWD_RANGE));
         asic_write( asic_id, addr, ((0xF) << 16));
    }

    // Program CIF to forward VNTAG packets. 
     addr = BA(RED_CI0_PORT_CFG);
     reg_val = asic_read(asic_id, addr) | 
		bits_set(RED_CI_PORT_CFG_FLD_NIF_PORTS_VNTAG_MODE) |
		(0x01 << RED_CI_PORT_CFG_FLD_CIF_VNTAG_MODE) |
		(0x01 << RED_CI_PORT_CFG_FLD_CIF_VNTAG_PRSING_EN) |
		(0x01 << RED_CI_PORT_CFG_FLD_RX_VNTAG_PASSTHRU) |
		(0x01 << RED_CI_PORT_CFG_FLD_TX_VNTAG_PASSTHRU);
     asic_write(asic_id, addr, reg_val);


     for (hif = 0; hif < 8; hif++) {
	  // Program VIF MAX
          addr = BA(RED_HI0_RX_VIF_MAX + hif*(RED_HI1_RX_VIF_MAX-RED_HI0_RX_VIF_MAX));
          reg_val = 0xFFF;
          asic_write(asic_id, addr, reg_val);

          // Program VNTAG mode
	  addr = BA(RED_HI0_PORT_CFG + hif*(RED_HI1_PORT_CFG-RED_HI0_PORT_CFG));
	  reg_val = asic_read(asic_id, addr) | 
			(0x01 << RED_HI_PORT_CFG_FLD_IS_VNTAG) |
			(0x01 << RED_HI_PORT_CFG_FLD_RX_VNTAG_PASSTHRU) |
			(0x01 << RED_HI_PORT_CFG_FLD_TX_VNTAG_PASSTHRU);
     	  asic_write(asic_id, addr, reg_val);

	  // Program Forwarding vector HIF0 - NIF1, HIF1 - NIF2.
	  redwood_config_hif_cif(asic_id, hif, (0x01 << (hif&3)));
     }

     return (0);
}


int redwood_rewrite_config(uint8_t asic_id, uint8_t fp_mask, uint8_t bp_mask)
{
    uint32_t addr, hif, reg_val, nif;
    uint8_t  vntag_rewrite_offset = 0xA0;

    /* configure NIx fwd ranges so as not to conflict with default vntag */
    invalidate_fwd_ranges( asic_id, BA(RED_NI0_HIF0_FWD_RANGE) );
    invalidate_fwd_ranges( asic_id, BA(RED_NI1_HIF0_FWD_RANGE) );
    invalidate_fwd_ranges( asic_id, BA(RED_NI2_HIF0_FWD_RANGE) );
    invalidate_fwd_ranges( asic_id, BA(RED_NI3_HIF0_FWD_RANGE) );

     // Program HIF to drop all packets.
     redwood_config_hif_drop(asic_id);

     /* Program Default NIF VNTAGS */
     asic_write( asic_id, BA(RED_NI0_NON_FCOE_RX_DEFAULT_VNTAG), 0x00 );
     asic_write( asic_id, BA(RED_NI1_NON_FCOE_RX_DEFAULT_VNTAG), 0x00 );
     asic_write( asic_id, BA(RED_NI2_NON_FCOE_RX_DEFAULT_VNTAG), 0x00 );
     asic_write( asic_id, BA(RED_NI3_NON_FCOE_RX_DEFAULT_VNTAG), 0x00 );

    /* Forward VNTAG range from 0 - 0xFF to CIF */ 
    for(nif = 0; nif < 4; nif++) {
	 addr = BA(RED_NI0_CIF_FWD_RANGE + 
		   (nif * (RED_NI1_CIF_FWD_RANGE - RED_NI0_CIF_FWD_RANGE)));
         asic_write( asic_id, addr, ((0xFF) << 16));
    }

    // Program CIF to forward VNTAG packets. 
    addr = BA(RED_CI0_PORT_CFG);
    reg_val = asic_read(asic_id, addr) | 
		bits_set(RED_CI_PORT_CFG_FLD_NIF_PORTS_VNTAG_MODE) |
		(0x01 << RED_CI_PORT_CFG_FLD_CIF_VNTAG_MODE) |
		(0x01 << RED_CI_PORT_CFG_FLD_CIF_VNTAG_PRSING_EN) |
		(0x01 << RED_CI_PORT_CFG_FLD_RX_VNTAG_PASSTHRU) |
		(0x01 << RED_CI_PORT_CFG_FLD_TX_VNTAG_PASSTHRU);
    asic_write(asic_id, addr, reg_val);

     for (hif = 0; hif < 8; hif++) {
	  // Program VIF MAX
          addr = BA(RED_HI0_RX_VIF_MAX + hif*(RED_HI1_RX_VIF_MAX-RED_HI0_RX_VIF_MAX));
          reg_val = 0xFFF;
          asic_write(asic_id, addr, reg_val);

          // Program HIFS in VNTAG mode
	  addr = BA(RED_HI0_PORT_CFG + hif*(RED_HI1_PORT_CFG-RED_HI0_PORT_CFG));
	  reg_val = asic_read(asic_id, addr);
	  reg_val &= ~( (0x01 << RED_HI_PORT_CFG_FLD_IS_VNTAG) |
			(0x01 << RED_HI_PORT_CFG_FLD_RX_VNTAG_PASSTHRU) |
			(0x01 << RED_HI_PORT_CFG_FLD_TX_VNTAG_PASSTHRU));
     	  asic_write(asic_id, addr, reg_val);

          // program HIF NON FCOE Default vntag.
          addr = BA(RED_HI0_NON_FCOE_RX_DEFAULT_VNTAG +
                (hif * (RED_HI1_NON_FCOE_RX_DEFAULT_VNTAG - RED_HI0_NON_FCOE_RX_DEFAULT_VNTAG)));
          reg_val = ((vntag_rewrite_offset | hif) << 16) | (vntag_rewrite_offset | hif);
          asic_write(asic_id, addr, reg_val);


	  // Program Forwarding vector HIF0 - NIF0, HIF1 - NIF1.
	  // Program Forwarding vector HIF2 - NIF2, HIF3 - NIF3.
	  // Program Forwarding vector HIF4 - NIF0, HIF5 - NIF1.
	  // Program Forwarding vector HIF6 - NIF2, HIF7 - NIF3.
	  redwood_config_hif_nif(asic_id, hif, (hif & 0x03), vntag_rewrite_offset | hif);
     }

     return (0);
}


int redwood_port_mesh_config(uint8_t asic_id, uint8_t fp_mask, uint8_t bp_mask)
{
    uint32_t addr, hif, reg_val, nif;

    /* configure NIx fwd ranges so as not to conflict with default vntag */
    invalidate_fwd_ranges( asic_id, BA(RED_NI0_HIF0_FWD_RANGE) );
    invalidate_fwd_ranges( asic_id, BA(RED_NI1_HIF0_FWD_RANGE) );
    invalidate_fwd_ranges( asic_id, BA(RED_NI2_HIF0_FWD_RANGE) );
    invalidate_fwd_ranges( asic_id, BA(RED_NI3_HIF0_FWD_RANGE) );

     // Program HIF to drop all packets.
     redwood_config_hif_drop(asic_id);

     /* Program Default NIF VNTAGS */
     asic_write( asic_id, BA(RED_NI0_NON_FCOE_RX_DEFAULT_VNTAG), 0x00 );
     asic_write( asic_id, BA(RED_NI1_NON_FCOE_RX_DEFAULT_VNTAG), 0x00 );
     asic_write( asic_id, BA(RED_NI2_NON_FCOE_RX_DEFAULT_VNTAG), 0x00 );
     asic_write( asic_id, BA(RED_NI3_NON_FCOE_RX_DEFAULT_VNTAG), 0x00 );

    // Program CIF to forward VNTAG packets. 
    addr = BA(RED_CI0_PORT_CFG);
    reg_val = asic_read(asic_id, addr) | 
		bits_set(RED_CI_PORT_CFG_FLD_NIF_PORTS_VNTAG_MODE) |
		(0x01 << RED_CI_PORT_CFG_FLD_CIF_VNTAG_MODE) |
		(0x01 << RED_CI_PORT_CFG_FLD_CIF_VNTAG_PRSING_EN) |
		(0x01 << RED_CI_PORT_CFG_FLD_RX_VNTAG_PASSTHRU) |
		(0x01 << RED_CI_PORT_CFG_FLD_TX_VNTAG_PASSTHRU);
    asic_write(asic_id, addr, reg_val);

     for (hif = 0; hif < 8; hif++) {
	  // Program VIF MAX
          addr = BA(RED_HI0_RX_VIF_MAX + hif*(RED_HI1_RX_VIF_MAX-RED_HI0_RX_VIF_MAX));
          reg_val = 0xFFF;
          asic_write(asic_id, addr, reg_val);

          // Program HIFS in VNTAG mode
	  addr = BA(RED_HI0_PORT_CFG + hif*(RED_HI1_PORT_CFG-RED_HI0_PORT_CFG));
	  reg_val = asic_read(asic_id, addr) | 
			(0x01 << RED_HI_PORT_CFG_FLD_IS_VNTAG) |
			(0x01 << RED_HI_PORT_CFG_FLD_RX_VNTAG_PASSTHRU) |
			(0x01 << RED_HI_PORT_CFG_FLD_TX_VNTAG_PASSTHRU);
     	  asic_write(asic_id, addr, reg_val);

	  // VNTAG = ((NIF+1) << 4) | (HIF+1)) 
	  for (nif = 0; nif < 4; nif++) {
	  	redwood_config_hif_nif(asic_id, hif, nif, ((nif+1) << 4) | (hif + 1));
	  }
     }

    /* Forward VNTAG range to HIF */ 
    for(nif = 0; nif < 4; nif++) {
	 addr = BA(RED_NI0_HIF0_FWD_RANGE + 
		   (nif * (RED_NI1_HIF0_FWD_RANGE - RED_NI0_HIF0_FWD_RANGE)));

         for (hif = 0; hif < 8; hif++) {
		reg_val = ((nif+1) << 4) | (hif+1);
         	asic_write(asic_id, addr, ((reg_val) << 16) | reg_val);
		addr += BA(RED_NI0_HIF1_FWD_RANGE - RED_NI0_HIF0_FWD_RANGE);
	 }
    }

    /* Clear all the NSPI Timers */
    redwood_nspi_timer_cfg(asic_id, 0x00);

    return (0);
}

void redwood_config_portmesh_nif_cif(int asic_id, uint8_t fp_mask, uint8_t bp_mask)
{
     uint32_t addr, nif;
    for(nif = 0; nif < 4; nif++) {
	 addr = BA(RED_NI0_CIF_FWD_RANGE + 
		   (nif * (RED_NI1_CIF_FWD_RANGE - RED_NI0_CIF_FWD_RANGE)));
         asic_write( asic_id, addr, ((0xFF) << 16));
    }
    /* configure NIx fwd ranges so as not to conflict with default vntag */
    invalidate_fwd_ranges( asic_id, BA(RED_NI0_HIF0_FWD_RANGE) );
    invalidate_fwd_ranges( asic_id, BA(RED_NI1_HIF0_FWD_RANGE) );
    invalidate_fwd_ranges( asic_id, BA(RED_NI2_HIF0_FWD_RANGE) );
    invalidate_fwd_ranges( asic_id, BA(RED_NI3_HIF0_FWD_RANGE) );

}

void redwood_config_portmesh_hif_cif(int asic_id, uint8_t fp_mask, uint8_t bp_mask)
{
     uint8_t hif, nif;
     for (hif = 0; hif < 8; hif++) {
	  // VNTAG = ((NIF+1) << 4) | (HIF+1)) 
	  for (nif = 0; nif < 4; nif++) {
	  	redwood_config_hif_cif(asic_id, hif, ((nif+1) << 4) | (hif + 1));
	  }
     }
}




int redwood_multicast_config(uint8_t asic_id, uint8_t fp_mask, uint8_t bp_mask)
{
    uint32_t addr, hif, reg_val, nif;

    /* configure NIx fwd ranges so as not to conflict with default vntag */
    invalidate_fwd_ranges( asic_id, BA(RED_NI0_HIF0_FWD_RANGE) );
    invalidate_fwd_ranges( asic_id, BA(RED_NI1_HIF0_FWD_RANGE) );
    invalidate_fwd_ranges( asic_id, BA(RED_NI2_HIF0_FWD_RANGE) );
    invalidate_fwd_ranges( asic_id, BA(RED_NI3_HIF0_FWD_RANGE) );

     redwood_config_hif_drop(asic_id);

     /* Program Default NIF VNTAGS */
     asic_write( asic_id, BA(RED_NI0_NON_FCOE_RX_DEFAULT_VNTAG), 0x00);
     asic_write( asic_id, BA(RED_NI1_NON_FCOE_RX_DEFAULT_VNTAG), 0x00);
     asic_write( asic_id, BA(RED_NI2_NON_FCOE_RX_DEFAULT_VNTAG), 0x00); 
     asic_write( asic_id, BA(RED_NI3_NON_FCOE_RX_DEFAULT_VNTAG), 0x00); 

    /* Program Multicast Table to Forward packets to all HIFs */ 
    for(nif = 0; nif < 4; nif++) {
	addr = BA(RED_NF_DHS_MCST_MEMIF_CMD);
	reg_val = (1<<RED_NF_DHS_MCST_MEMIF_CMD_FLD_DIRACC) |
		  (1<<RED_NF_DHS_MCST_MEMIF_CMD_FLD_DIRADDR);
	asic_write(asic_id, addr, reg_val);

        addr = BA(RED_NF_DHS_MCST_MEMIF_DATA +
                ((0x01 << nif) * RED_NF_DHS_MCST_MEMIF_DATA_ARRAYSKIP));
	reg_val = 0xFF;
	asic_write(asic_id, addr, reg_val);
    }

    // Program multicast vector.
    addr = BA(RED_NF_CFG_REG0);
    reg_val = (0xFF << 12) | 0xFF;
    asic_write(asic_id, addr, reg_val);
    
    addr = BA(RED_NF_CFG_REG1);
    reg_val = (0xFF << 12) | 0xFF;
    asic_write(asic_id, addr, reg_val);
    
    // Program CIF to forward VNTAG packets. 
     addr = BA(RED_CI0_PORT_CFG);
     reg_val = asic_read(asic_id, addr) | 
		bits_set(RED_CI_PORT_CFG_FLD_NIF_PORTS_VNTAG_MODE) |
		(0x01 << RED_CI_PORT_CFG_FLD_CIF_VNTAG_MODE) |
		(0x01 << RED_CI_PORT_CFG_FLD_CIF_VNTAG_PRSING_EN) |
		(0x01 << RED_CI_PORT_CFG_FLD_RX_VNTAG_PASSTHRU) |
		(0x01 << RED_CI_PORT_CFG_FLD_TX_VNTAG_PASSTHRU);
     asic_write(asic_id, addr, reg_val);


     for (hif = 0; hif < 8; hif++) {
	  // Program VIF MAX
          addr = BA(RED_HI0_RX_VIF_MAX + hif*(RED_HI1_RX_VIF_MAX-RED_HI0_RX_VIF_MAX));
          reg_val = 0xFFF;
          asic_write(asic_id, addr, reg_val);

          // Program VNTAG mode
	  addr = BA(RED_HI0_PORT_CFG + hif*(RED_HI1_PORT_CFG-RED_HI0_PORT_CFG));
	  reg_val = asic_read(asic_id, addr) | 
			(0x01 << RED_HI_PORT_CFG_FLD_IS_VNTAG) |
			(0x01 << RED_HI_PORT_CFG_FLD_RX_VNTAG_PASSTHRU) |
			(0x01 << RED_HI_PORT_CFG_FLD_TX_VNTAG_PASSTHRU);
     	  asic_write(asic_id, addr, reg_val);

	  // Program Forwarding vector HIF0 - NIF1, HIF1 - NIF2.
	  for (nif = 0; nif < 4; nif++)
			redwood_config_hif_cif(asic_id, hif, (0x01 << nif));
	     }

	     return (0);
}

#define FEX_REV2
#ifdef FEX_REV2
static uint8_t hif_ports[6]  = {0, 1, 3, 4, 5, 7};
static uint8_t hif_ports2[6] = {0, 2, 3, 4, 6, 7};
#define RDW_2NIF_0	1
#define RDW_2NIF_1	4

#else

static uint8_t hif_ports[6]  = {0, 1, 2, 3, 6, 7};
static uint8_t hif_ports2[6] = {0, 1, 5, 4, 6, 7};
#define RDW_2NIF_0	2
#define RDW_2NIF_1	3

#endif

int redwood_system_config(uint8_t asic_id, uint8_t fp_mask, uint8_t bp_mask, uint32_t nif_mask)
{
	uint32_t addr, vntag, reg_val;
	uint8_t inst, hif, nif;

	/* configure NIx fwd ranges so as not to conflict with default vntag */
	invalidate_fwd_ranges( asic_id, BA(RED_NI0_HIF0_FWD_RANGE) );
	invalidate_fwd_ranges( asic_id, BA(RED_NI1_HIF0_FWD_RANGE) );
	invalidate_fwd_ranges( asic_id, BA(RED_NI2_HIF0_FWD_RANGE) );
	invalidate_fwd_ranges( asic_id, BA(RED_NI3_HIF0_FWD_RANGE) );

	// Program HIF to drop all packets.
	redwood_config_hif_drop(asic_id);

	/* Program Default NIF VNTAGS */
	asic_write( asic_id, BA(RED_NI0_NON_FCOE_RX_DEFAULT_VNTAG), 0x00 );
	asic_write( asic_id, BA(RED_NI1_NON_FCOE_RX_DEFAULT_VNTAG), 0x00 );
	asic_write( asic_id, BA(RED_NI2_NON_FCOE_RX_DEFAULT_VNTAG), 0x00 );
	asic_write( asic_id, BA(RED_NI3_NON_FCOE_RX_DEFAULT_VNTAG), 0x00 );

	// Program CIF to forward VNTAG packets. 
	addr = BA(RED_CI0_PORT_CFG);
	reg_val = asic_read(asic_id, addr) | 
		bits_set(RED_CI_PORT_CFG_FLD_NIF_PORTS_VNTAG_MODE) |
		(0x01 << RED_CI_PORT_CFG_FLD_CIF_VNTAG_MODE) |
		(0x01 << RED_CI_PORT_CFG_FLD_CIF_VNTAG_PRSING_EN) |
		(0x01 << RED_CI_PORT_CFG_FLD_RX_VNTAG_PASSTHRU) |
		(0x01 << RED_CI_PORT_CFG_FLD_TX_VNTAG_PASSTHRU);
	asic_write(asic_id, addr, reg_val);

	for (hif = 0; hif < 8; hif++) {
		vntag = (asic_id <<3) | hif;

		// Program VIF MAX
		addr = BA(RED_HI0_RX_VIF_MAX + 
			hif*(RED_HI1_RX_VIF_MAX-RED_HI0_RX_VIF_MAX));
		reg_val = 0xFFF;
		asic_write(asic_id, addr, reg_val);

		// Program HIFS in VNTAG mode
		addr = BA(RED_HI0_PORT_CFG + hif*(RED_HI1_PORT_CFG-RED_HI0_PORT_CFG));
		reg_val = asic_read(asic_id, addr) | 
			  (0x01 << RED_HI_PORT_CFG_FLD_IS_VNTAG) |
			  (0x01 << RED_HI_PORT_CFG_FLD_RX_VNTAG_PASSTHRU) |
			  (0x01 << RED_HI_PORT_CFG_FLD_TX_VNTAG_PASSTHRU);
		asic_write(asic_id, addr, reg_val);
	}
	if (asic_id < 6) {
		for (hif = 0; hif < 8; hif++) {
			vntag = (asic_id <<3) | hif;
			// Program HIF to NIF
			if (((asic_id == RDW_2NIF_0) || (asic_id == RDW_2NIF_1)) && (hif > 3))
				redwood_config_hif_nif(asic_id, hif, 3, vntag);
			else 
				redwood_config_hif_nif(asic_id, hif, 1, vntag);
	
			/* Forward VNTAG range to HIF */ 
			if (((asic_id == RDW_2NIF_0) || (asic_id == RDW_2NIF_1)) && (hif > 3)) {
				addr = BA(RED_NI3_HIF0_FWD_RANGE) + 
			       	  (hif * BA(RED_NI1_HIF1_FWD_RANGE - RED_NI1_HIF0_FWD_RANGE));
				asic_write(asic_id, addr, ((vntag << 16) | vntag));
			} else {
				addr = BA(RED_NI1_HIF0_FWD_RANGE) + 
			       	  (hif * BA(RED_NI1_HIF1_FWD_RANGE - RED_NI1_HIF0_FWD_RANGE));
				asic_write(asic_id, addr, ((vntag << 16) | vntag));
			}
		}
	} else {
		for (inst = 0; inst < 6; inst++) {
			// Program HIF to NIF
			for (hif = 0; hif < 8; hif++) {
				vntag = (inst << 3) | hif;
				if (((inst == RDW_2NIF_0) || (inst == RDW_2NIF_1)) && (hif > 3)) {
					redwood_config_hif_nif(asic_id, hif_ports2[inst], 
							vntag & 3, vntag);
				} else {
					redwood_config_hif_nif(asic_id, hif_ports[inst], 
							vntag & 3, vntag);
				}

			}
		}

		for (nif = 0; nif < 4; nif++) {
			for (inst = 0; inst < 6; inst++) {
				/* Forward VNTAG range to HIF */ 
	                       	addr = BA(RED_NI0_HIF0_FWD_RANGE +
					(nif*(RED_NI1_HIF0_FWD_RANGE - RED_NI0_HIF0_FWD_RANGE)));

				if ((inst == RDW_2NIF_0) || (inst == RDW_2NIF_1)) {
					uint32_t myaddr = addr;
					addr += (hif_ports[inst] * BA(RED_NI0_HIF1_FWD_RANGE - RED_NI0_HIF0_FWD_RANGE));
					asic_write(asic_id, addr, ((((inst<<3)|0x03) << 16) | (inst << 3)));

					myaddr += (hif_ports2[inst] * BA(RED_NI0_HIF1_FWD_RANGE - RED_NI0_HIF0_FWD_RANGE));
					asic_write(asic_id, myaddr, ((((inst<<3)|0x07) << 16) | ((inst << 3) | 4)));
				} else {
					addr += (hif_ports[inst] * BA(RED_NI0_HIF1_FWD_RANGE - RED_NI0_HIF0_FWD_RANGE));
					asic_write(asic_id, addr, ((((inst<<3)|0x07) << 16) | (inst << 3)));
				}
			}
		}
	}

    	redwood_nspi_timer_cfg(asic_id, 0x00);
	return (0);
}

void redwood_config_system_hif_cif(int asic_id, uint8_t fp_mask, uint8_t bp_mask, uint8_t inst)
{
     uint8_t port;

     for (port = (inst<<3); port < ((inst << 3)+8); port++) {
	  // VNTAG = ((NIF+1) << 4) | (HIF+1)) 
		if (((inst == RDW_2NIF_0) || (inst == RDW_2NIF_1)) && ((port&7) > 3)) {
	  		redwood_config_hif_cif(asic_id, hif_ports2[inst], port); 
		} else {
	  		redwood_config_hif_cif(asic_id, hif_ports[inst], port); 
		}
     }
}

static int redwood_snake_config_1g(uint8_t asic_id, uint8_t fp_mask, uint8_t bp_mask)
{
   uint32_t addr, hif, reg_val, vntag;

    /*
     *   For ASIC 0 - 5. 
     *
     *   NI1 -> HI0 -> NI0 -> HI1 -> NI0 -> HI2 -> NI0 -> HI3 ->
     *   NI0 -> HI4 -> NI0 -> HI5 -> NI0 -> HI6 -> NI0 -> HI7 -> NI1
     *
     *   For ASIC 6
     *   NI0 -> HI0 -> NI1 -> HI1 -> NI2 -> HI2 -> NI3 -> HI3 ->
     *   NI1 -> HI4 -> NI2 -> HI5 -> NI3 -> HI6 -> NI1 -> HI7 - NI0
     *
     */
    asic_write( asic_id, BA(RED_NI0_NON_FCOE_RX_DEFAULT_VNTAG), (1 << 0) );
    asic_write( asic_id, BA(RED_NI1_NON_FCOE_RX_DEFAULT_VNTAG), (1 << 1) );
    asic_write( asic_id, BA(RED_NI2_NON_FCOE_RX_DEFAULT_VNTAG), (1 << 2) );
    asic_write( asic_id, BA(RED_NI3_NON_FCOE_RX_DEFAULT_VNTAG), (1 << 3) );

    /* configure NIx fwd ranges so as not to conflict with default vntag */
    invalidate_fwd_ranges( asic_id, BA(RED_NI0_HIF0_FWD_RANGE) );
    invalidate_fwd_ranges( asic_id, BA(RED_NI1_HIF0_FWD_RANGE) );
    invalidate_fwd_ranges( asic_id, BA(RED_NI2_HIF0_FWD_RANGE) );
    invalidate_fwd_ranges( asic_id, BA(RED_NI3_HIF0_FWD_RANGE) );

    // Program CIF to forward VNTAG packets.
    addr = BA(RED_CI0_PORT_CFG);
    reg_val = asic_read(asic_id, addr) |
                bits_set(RED_CI_PORT_CFG_FLD_NIF_PORTS_VNTAG_MODE) |
                (0x01 << RED_CI_PORT_CFG_FLD_CIF_VNTAG_MODE) |
                (0x01 << RED_CI_PORT_CFG_FLD_CIF_VNTAG_PRSING_EN) |
                (0x01 << RED_CI_PORT_CFG_FLD_RX_VNTAG_PASSTHRU) |
                (0x01 << RED_CI_PORT_CFG_FLD_TX_VNTAG_PASSTHRU);
    asic_write(asic_id, addr, reg_val);

    for (hif = 0; hif < 8; hif++) {
	  vntag = (asic_id << 3) | hif;
	  // Program VIF max
	  addr = BA(RED_HI0_RX_VIF_MAX + hif*(RED_HI1_RX_VIF_MAX-RED_HI0_RX_VIF_MAX));
	  reg_val = 0xFFF;
     	  asic_write(asic_id, addr, reg_val);

	  // HIF NON FCOE Default vntag.
	  addr = BA(RED_HI0_NON_FCOE_RX_DEFAULT_VNTAG + 
		(hif * (RED_HI1_NON_FCOE_RX_DEFAULT_VNTAG - RED_HI0_NON_FCOE_RX_DEFAULT_VNTAG)));

	  if ((hif ==7) && (asic_id == 5)) {
	  	reg_val = 0x00;						// Set dest VNTAG back to 0. 
	  } else {
	  	reg_val = ((vntag+1) << 16) | (vntag);  		// Set Destination VNTAG. 
	  }
	  asic_write(asic_id, addr, reg_val);

          // Clear VNTAG mode
	  addr = BA(RED_HI0_PORT_CFG + hif*(RED_HI1_PORT_CFG-RED_HI0_PORT_CFG));
	  reg_val = asic_read(asic_id, addr) &  
			~((0x01 << RED_HI_PORT_CFG_FLD_IS_VNTAG) |
			  (0x01 << RED_HI_PORT_CFG_FLD_RX_VNTAG_PASSTHRU) |
			  (0x01 << RED_HI_PORT_CFG_FLD_TX_VNTAG_PASSTHRU));
     	  asic_write(asic_id, addr, reg_val);

	  // HIFx -> NIF0, HIF7 -> NIF1 
          redwood_config_hif_nif(asic_id, hif, ((hif == 7) ? 1 : 0), 
			((hif ==7) && (asic_id == 5))? 0x00 : vntag);

	  // NIF0 -> HIFx
	  addr = BA(RED_NI0_HIF0_FWD_RANGE) + 
		 BA(hif*(RED_NI0_HIF1_FWD_RANGE-RED_NI0_HIF0_FWD_RANGE));
	  reg_val = ((vntag << 16) | vntag);
     	  asic_write( asic_id, addr, reg_val); 
     }

     // NIF1 -> HIF0
     addr = BA(RED_NI1_HIF0_FWD_RANGE);
     reg_val = ((((asic_id << 3)|7) << 16) | (asic_id << 3));
     asic_write( asic_id, addr, reg_val); 

     return (0);
}


static int redwood_snake_config_10g(uint8_t asic_id, uint8_t fp_mask, uint8_t bp_mask)
{
   uint32_t addr, nif, hif, reg_val, vntag, inst;

    /*
     *   For ASIC 0 - 5. 
     *
     *   NI1 -> HI0 -> NI0 -> HI1 -> NI0 -> HI2 -> NI0 -> HI3 ->
     *   NI0 -> HI4 -> NI0 -> HI5 -> NI0 -> HI6 -> NI0 -> HI7 -> NI1
     *
     *   For ASIC 6
     *   NI0 -> HI0 -> NI1 -> HI1 -> NI2 -> HI2 -> NI3 -> HI3 ->
     *   NI1 -> HI4 -> NI2 -> HI5 -> NI3 -> HI6 -> NI1 -> HI7 - NI0
     *
     */
    asic_write( asic_id, BA(RED_NI0_NON_FCOE_RX_DEFAULT_VNTAG), (1 << 0) );
    asic_write( asic_id, BA(RED_NI1_NON_FCOE_RX_DEFAULT_VNTAG), (1 << 1) );
    asic_write( asic_id, BA(RED_NI2_NON_FCOE_RX_DEFAULT_VNTAG), (1 << 2) );
    asic_write( asic_id, BA(RED_NI3_NON_FCOE_RX_DEFAULT_VNTAG), (1 << 3) );

    /* configure NIx fwd ranges so as not to conflict with default vntag */
    invalidate_fwd_ranges( asic_id, BA(RED_NI0_HIF0_FWD_RANGE) );
    invalidate_fwd_ranges( asic_id, BA(RED_NI1_HIF0_FWD_RANGE) );
    invalidate_fwd_ranges( asic_id, BA(RED_NI2_HIF0_FWD_RANGE) );
    invalidate_fwd_ranges( asic_id, BA(RED_NI3_HIF0_FWD_RANGE) );

    // Program CIF to forward VNTAG packets.
    addr = BA(RED_CI0_PORT_CFG);
    reg_val = asic_read(asic_id, addr) |
                bits_set(RED_CI_PORT_CFG_FLD_NIF_PORTS_VNTAG_MODE) |
                (0x01 << RED_CI_PORT_CFG_FLD_CIF_VNTAG_MODE) |
                (0x01 << RED_CI_PORT_CFG_FLD_CIF_VNTAG_PRSING_EN) |
                (0x01 << RED_CI_PORT_CFG_FLD_RX_VNTAG_PASSTHRU) |
                (0x01 << RED_CI_PORT_CFG_FLD_TX_VNTAG_PASSTHRU);
    asic_write(asic_id, addr, reg_val);

    for (hif = 0; hif < 8; hif++) {
	  vntag = (asic_id << 3) | hif;
	  // Program VIF max
	  addr = BA(RED_HI0_RX_VIF_MAX + hif*(RED_HI1_RX_VIF_MAX-RED_HI0_RX_VIF_MAX));
	  reg_val = 0xFFF;
     	  asic_write(asic_id, addr, reg_val);

	  // Program HIFS in VNTAG mode
	  addr = BA(RED_HI0_PORT_CFG + hif*(RED_HI1_PORT_CFG-RED_HI0_PORT_CFG));
	  reg_val = asic_read(asic_id, addr) | 
			  (0x01 << RED_HI_PORT_CFG_FLD_IS_VNTAG) |
			  (0x01 << RED_HI_PORT_CFG_FLD_RX_VNTAG_PASSTHRU) |
			  (0x01 << RED_HI_PORT_CFG_FLD_TX_VNTAG_PASSTHRU);
		asic_write(asic_id, addr, reg_val);
     }

     for (inst = 0; inst < 6; inst++) { 
	  // HIF -> NIF 
	  for (hif = 0; hif < 8; hif++) {
	       vntag = (inst << 3) | hif; 

	       if ((hif_ports[inst] == 7) && (hif == 7)) {
//                   redwood_config_hif_cif(asic_id, hif_ports[inst], 0x00);
                   redwood_config_hif_nif(asic_id, hif_ports[inst], 0, 0x00);
	       } else {
	           nif = (hif_ports[inst] == 7) ? 0 : ((inst%3)+1);
                   redwood_config_hif_nif(asic_id, hif_ports[inst], nif, vntag);
	       }
	  }
     }

     // NIF -> HIF
     for (nif = 0; nif < 4; nif++) {
     	  for (inst = 0; inst < 6; inst++) { 
	  	addr = BA(RED_NI0_HIF0_FWD_RANGE) + 
		 	BA(nif*(RED_NI1_HIF0_FWD_RANGE-RED_NI0_HIF0_FWD_RANGE));
		addr += BA(hif_ports[inst]*(RED_NI0_HIF1_FWD_RANGE-RED_NI0_HIF0_FWD_RANGE));
	  	reg_val = (((inst <<3)| 0x07) << 16) | (inst << 3);
     	  	asic_write( asic_id, addr, reg_val); 
	  }
     }

     return (0);
}

void redwood_config_syssnake_hif_cif(int asic_id, uint8_t fp_mask, uint8_t bp_mask, uint8_t inst)
{
//     redwood_config_hif_cif(asic_id, 7, 0x00);
}


int redwood_fex_snake_config(uint8_t asic_id, uint8_t fp_mask, uint8_t bp_mask)
{
	if (asic_id < 6) {
		return (redwood_snake_config_1g(asic_id, fp_mask, bp_mask));
	} else {
		return (redwood_snake_config_10g(asic_id, fp_mask, bp_mask));
	}
}

int redwood_err_dump(int inst, uint32_t *pmask)
{
	int port, rc = 0;	

	*pmask = 0x00;
	for (port = cif; port <= nif3; port++) {
		rc = mac_rmon_err_stats_dump(inst, port, (port > 1) ? 36 : 32);
		if (rc)  *pmask |= (0x01 << port);
	}

	return (0);
}

int redwood_get_rev_id (int inst) 
{
    uint32_t data = 0;

    red_reg_rd(inst, BA(RED_MISC_JTAG_ID), &data);

    if (data == 0x1033E57F) {
	return (1);
    } else if (data == 0x2033E57F) {
	return (2);
    }

    return (-1);
}

int redwood_get_die_id (int inst)
{
    uint32_t data = 1;
    red_reg_wr(inst, BA(RED_MISC_TEST__EFUSE), data);
    data = 0;
    red_reg_rd(inst, BA(RED_MISC_TEST__EFUSE_READ_VALID), &data);
    if (data & 0x1) {
        red_reg_rd(inst, BA(RED_MISC_DIE_ID_00), &data);
        printf("Instance %d DIE_ID_00 %x\t", inst, data);
        red_reg_rd(inst, BA(RED_MISC_DIE_ID_01), &data);
        printf("DIE_ID_01 %x\t", data);
        red_reg_rd(inst, BA(RED_MISC_DIE_ID_02), &data);
        printf("DIE_ID_02 %x\n", data);
    }
    return 0;

}

int redwood_packet_drop (int asic_id, uint8_t ena)
{
   uint32_t addr, nif, hif, reg_val, which_ss, port;

    addr = BA(RED_CI0_PORT_CFG);

    // CI
    reg_val = asic_read(asic_id, addr);
    if (ena) {
	reg_val |= (0x01 << RED_CI_PORT_CFG_FLD_TX_ERR_EN);
    } else {
	reg_val &= ~(0x01 << RED_CI_PORT_CFG_FLD_TX_ERR_EN);
    }
    asic_write( asic_id, addr, reg_val); 

    // BI
    addr = BA(RED_BI0_PORT_CFG);
    reg_val = asic_read(asic_id, addr);
    if (ena) {
	reg_val |= (0x01 << RED_BI_PORT_CFG_FLD_TX_ERR_EN);
    } else {
	reg_val &= ~(0x01 << RED_BI_PORT_CFG_FLD_TX_ERR_EN);
    }
    asic_write( asic_id, addr, reg_val); 

    // NI
    for (nif = 0; nif < 4; nif++) {
        addr = BA(RED_NI0_PORT_CFG + (nif *(RED_NI1_PORT_CFG - RED_NI0_PORT_CFG)));
        reg_val = asic_read(asic_id, addr);
        if (ena) {
            reg_val |= (0x01 << RED_NI_PORT_CFG_FLD_TX_ERR_EN);
        } else {
            reg_val &= ~(0x01 << RED_NI_PORT_CFG_FLD_TX_ERR_EN);
        }
        asic_write( asic_id, addr, reg_val); 
    }

    // HI
    for (hif = 0; hif < 4; hif++) {
        addr = BA(RED_HI0_PORT_CFG + (hif *(RED_HI1_PORT_CFG - RED_HI0_PORT_CFG)));
        reg_val = asic_read(asic_id, addr);
        if (ena) {
            reg_val |= (0x01 << RED_HI_PORT_CFG_FLD_TX_ERR_EN);
        } else {
            reg_val &= ~(0x01 << RED_HI_PORT_CFG_FLD_TX_ERR_EN);
        }
        asic_write( asic_id, addr, reg_val); 
    }


    for( which_ss=0; which_ss<4; ++which_ss) {
    	for( port=0; port<5; ++port) {
        	addr = BA(RED_SS0_CFG_WR_PORT0 + (which_ss *(RED_SS1_CFG_WR_PORT0 - RED_SS0_CFG_WR_PORT0)));
        	addr += port * BA(RED_SS0_CFG_WR_PORT1 - RED_SS0_CFG_WR_PORT0);
                reg_val = asic_read(asic_id, addr);
                if (ena) {
                    reg_val |= (0x01 << RED_SS_CFG_WR_PORT0_FLD_ERR_DISC);
                } else {
                    reg_val &= ~(0x01 << RED_SS_CFG_WR_PORT0_FLD_ERR_DISC);
                }
                asic_write( asic_id, addr, reg_val); 
        }
    }
    return (0);
}



int redwood_hif2nif(uint8_t asic_id, uint8_t hi_port, uint8_t ni_port, uint32_t offset)
{
	uint32_t addr, data;

	addr = BA(RED_HI0_RX_VIF_OFFSET + 
			(hi_port*(RED_HI1_RX_VIF_OFFSET-RED_HI0_RX_VIF_OFFSET)));
	data = offset;
	asic_write(asic_id, addr, data);

	// Program FWD Vector for FCOE and NonFCOE
	addr = BA(RED_HI0_FWD_VECTOR0 + 
		(hi_port*(RED_HI1_FWD_VECTOR0 - RED_HI0_FWD_VECTOR0)));
	data = ((ni_port & 7) << 4) | (ni_port & 0x7);
	asic_write(asic_id, addr, data);

	// Default VNTAG for NON-FCOE is 0
	addr = BA(RED_HI0_NON_FCOE_RX_DEFAULT_VNTAG + 
		(hi_port*(RED_HI1_NON_FCOE_RX_DEFAULT_VNTAG-RED_HI0_NON_FCOE_RX_DEFAULT_VNTAG)));
	data = 0x00;
	asic_write(asic_id, addr, data);

	// Set VNTAG Range 0-2
	addr = BA(RED_HI0_RX_VIF_MIN + hi_port*(RED_HI1_RX_VIF_MIN-RED_HI0_RX_VIF_MIN));
	data = 0x00;
	asic_write(asic_id, addr, data);

	addr = BA(RED_HI0_RX_VIF_MAX + hi_port*(RED_HI1_RX_VIF_MAX-RED_HI0_RX_VIF_MAX));
	data = 0x01;
	asic_write(asic_id, addr, data);

	return (0);
}

int redwood_nif2hif(uint8_t asic_id, uint8_t hi_port, uint8_t ni_port, uint32_t offset)
{
	uint32_t addr, data;

	addr = BA(RED_NI0_NON_FCOE_RX_DEFAULT_VNTAG + (ni_port *
		(RED_NI1_NON_FCOE_RX_DEFAULT_VNTAG - RED_NI0_NON_FCOE_RX_DEFAULT_VNTAG)));
	data = 1 << hi_port;
	asic_write(asic_id, addr, data);

	addr = BA(RED_NI0_FCOE_RX_DEFAULT_VNTAG + (ni_port *
		(RED_NI1_FCOE_RX_DEFAULT_VNTAG - RED_NI0_FCOE_RX_DEFAULT_VNTAG)));
	data = 1 << hi_port;
	asic_write(asic_id, addr, data);

	// Invalidate FWD Ranges..,
	invalidate_fwd_ranges(asic_id, BA(RED_NI0_HIF0_FWD_RANGE + (ni_port * 
				(RED_NI1_HIF0_FWD_RANGE-RED_NI0_HIF0_FWD_RANGE))));

	// Set VNtag Ranges to point to HIF.
	addr = BA(RED_NI0_HIF0_FWD_RANGE + 
			(ni_port*(RED_NI0_HIF0_FWD_RANGE-RED_NI0_HIF0_FWD_RANGE)));;

	addr += BA(hi_port*(RED_NI0_HIF1_FWD_RANGE-RED_NI0_HIF0_FWD_RANGE));

	data = ((offset+1)<<16) | offset;
	asic_write(asic_id, addr, data);

	return (0);
}
