#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include "bcm84823_lib.h"
#include "bcm84823.h"
#include "diag_main.h"

/* Mode of operation */
#define modeAUTO    0
#define mode1G      1
#define mode10G    10

/****************************************************************************
* Link-pause advertisement map
* This table converts {tx-pause-enbl,rx-pause-enbl} to {PS1,PS2}
*
*    Tx  Rx   PS1  PS2     Mode
*   +---+---+----+----+-----------------------+
*   | 0 | 0 |  0 |  0 |  None                 |
*   | 0 | 1 |  1 |  1 |  Asymmetric twds local|
*   | 1 | 0 |  1 |  0 |  Asymmetric twds LP   |
*   | 1 | 1 |  0 |  1 |  Symmetric            |
*   +---+---+----+----+-----------------------+
*
* table is indexed by tx-pause-enbl*2 + rx-pause-enbl
*****************************************************************************/
#define link_pause_map_bcm84823_index( tx_enbl, rx_enbl ) ((tx_enbl*2) + (rx_enbl))

/* IEEE PS1/PS2 values */
#define AN_PAUSE_NONE           0
#define AN_PAUSE_SYM            1
#define AN_PAUSE_ASYM_TWRD_LP   2
#define AN_PAUSE_ASYM_TWRD_LCL  3

int link_pause_map_bcm84823[4] = {
    AN_PAUSE_NONE,
    AN_PAUSE_ASYM_TWRD_LCL,
    AN_PAUSE_ASYM_TWRD_LP,
    AN_PAUSE_SYM
};
/****************************************************************************
* Programming sequences for BCM84823
****************************************************************************/
static uint16_t bcm84823_pgm_adv_1G[][4]= {
  {0, 7, 0x20,   0x0001},  // Don't Advertise 10G
  {0, 7, 0xFFE9, 0x0200},  // Advertise 1G, repeater, master
  {0, 7, 0xFFE4, 0x9001},  // No 10/100 advertisement, no next pg sppt
  {0, 7, 0xFFE0, 0x1200},  // 1G, FDX, AN, restart AN
};

static uint16_t bcm84823_pgm_adv_10G[][4]= {
  {0, 7, 0x20,   0x1001},// Advertise 10G
  {0, 7, 0xFFE9, 0x0000},// No 1G Advertisement
  {0, 7, 0xFFE4, 0x9001},// No 10/100 advertisement
  {0, 7, 0xFFE0, 0x1200},// PHY control register Desired speed = 10G + restart autoneg
};

static uint16_t bcm84823_pgm_adv_nothing[][4]= {
  {0, 7, 0x20,   0x0001},// No 10G Advertisement
  {0, 7, 0xFFE9, 0x0000},// No 1G Advertisement
  {0, 7, 0xFFE4, 0x9001},// No 10/100 advertisement
  {0, 7, 0xFFE0, 0x1200},// PHY control register Desired speed = 10G + restart autoneg
};


static int sel_device(int fd, uint8_t mdio_addr, uint8_t port_addr, 
                      int dev_sel)
{
    int rc = 0;
    uint16_t data;
    if (dev_sel == BCM84823_XFI_L_PCS) {
        data = 0x1800;
    } else if (dev_sel == BCM84823_XFI_L_PMD) {
        data = 0x800;
    }
    BCM84823_WR(fd, mdio_addr, port_addr, 
            BCM84823_UD_30_ADDR, 
            BCM84823_UD_0X4131, data); 
    return (rc);
}
/*  
 * Reset PMA/PMD Logic 
 * No need for unreset function, as reset bit is self clearing
 */
static int pma_pmd_reset (int fd, uint8_t mdio_addr, uint8_t port_addr)
{
    int rc = 0;
    uint16_t data;
    BCM84823_RD(fd, mdio_addr, port_addr, BCM84823_PMA_PMD_ADDR,
                BCM84823_PMD_CTRL, &data); 
    data |= BCM84823_PMD_CTRL_FLD_RESET;
    BCM84823_WR(fd, mdio_addr, port_addr, BCM84823_PMA_PMD_ADDR,
                BCM84823_PMD_CTRL, data); 
    return (rc);
}
/*  
 * PMA/PMD Loopback Logic 
 */
/*  
 * PMD Test control Loopback Logic 
 */
static int pcs_loopback (int fd, uint8_t mdio_addr, uint8_t port_addr,
                         uint8_t ena)
{
    int rc = 0;
    uint16_t data;
    /* PCS Test Control register, bit 14 */ 
    BCM84823_RD(fd, mdio_addr, port_addr, 
            BCM84823_PCS_ADDR, 
            BCM84823_PCS_CTRL, &data); 
    if (ena) {
        // Enable loopback 
        data |= BCM84823_PCS_CTRL_FLD_LOOPBACK;
    } else {
        data &= ~BCM84823_PCS_CTRL_FLD_LOOPBACK;
    }
    BCM84823_WR(fd, mdio_addr, port_addr, 
            BCM84823_PCS_ADDR, 
            BCM84823_PCS_CTRL, data); 
    return (rc);
}

static int bcm84823_10g_loopback_get (int fd, uint8_t mdio_addr, 
                                      uint8_t port_addr, uint8_t *lpbk)
{
    int rc = 0;
    uint16_t data;
    BCM84823_RD(fd, mdio_addr, port_addr, BCM84823_PCS_ADDR,
               BCM84823_PCS_CTRL, &data); 
    *lpbk  = (data & BCM84823_PCS_CTRL_FLD_LOOPBACK) ? 1 : 0;
    return (rc);
}
static int bcm84823_xfi_line_loopback_get (int fd, uint8_t mdio_addr, 
                           uint8_t port_addr, uint8_t *lpbk)
{
    int rc = 0;
    uint16_t data;
    sel_device(fd, mdio_addr, port_addr, BCM84823_XFI_L_PMD);
    BCM84823_RD(fd, mdio_addr, port_addr, BCM84823_PCS_ADDR+1,
               BCM84823_PMD_CTRL, &data); 
    *lpbk  = (data & BCM84823_PMD_CTRL_FLD_PMD_LPBK) ? 1 : 0;
    sel_device(fd, mdio_addr, port_addr, BCM84823_XFI_L_PCS);
    return (rc);
}

static int bcm84823_link_status_get (int fd, uint8_t mdio_addr, uint8_t port_addr,
                                    uint8_t device_class, uint8_t addr, uint8_t *link)
{
    int rc=0;
    uint16_t data;
    /* Read Status 8 Register twice to get rid of sticky fault */
    BCM84823_RD(fd, mdio_addr, port_addr, device_class, 0x8, &data); 
    BCM84823_RD(fd, mdio_addr, port_addr, device_class, 0x8, &data); 
    /* Read Status  Register twice to get current link status */
    BCM84823_RD(fd, mdio_addr, port_addr, device_class, addr, &data); 
    BCM84823_RD(fd, mdio_addr, port_addr, device_class, addr, &data); 

    *link  = (data & BCM84823_PCS_STATUS_FLD_RX_LINK_STATUS) ? 1 : 0;
    return (rc);
}

static int bcm84823_led_init (int fd, uint8_t mdio_addr, uint8_t port_addr)
{
    int rc = 0;
    uint16_t data = 0;
    data = (0x2 | (0x2<<3));
    data |= BCM84823_10GBASET_LED_CONTROL_FLD_LED_SEL_10G;
    data &= ~BCM84823_10GBASET_LED_CONTROL_FLD_LED_OE_N(1);
    data &= ~BCM84823_10GBASET_LED_CONTROL_FLD_LED_OE_N(2);
    BCM84823_WR(fd, mdio_addr, port_addr, BCM84823_PMA_PMD_ADDR,
                    BCM84823_10GBASET_LED_CONTROL, data);
    data = (BCM84823_10GBASET_CTL_LED_FLD_LINK_10G |
            BCM84823_10GBASET_CTL_LED_FLD_PCS_RX_ACT);
    BCM84823_WR(fd, mdio_addr, port_addr, BCM84823_PMA_PMD_ADDR,
                BCM84823_10GBASET_CTL_LED1_MASK_LOW, data);
    BCM84823_WR(fd, mdio_addr, port_addr, BCM84823_PMA_PMD_ADDR,
                BCM84823_10GBASET_CTL_LED2_MASK_LOW, data);
    return (rc);
}

int bcm84823_reset (int fd, uint8_t mdio_addr, uint8_t port_addr)
{
    int rc = 0;
    rc = pma_pmd_reset(fd, mdio_addr, port_addr);
    if (rc) {
        return (rc);
    }
    return (rc);
}
/*
 * This loopback is towards system side.
 * BCM84823 document under XFI (MAC) to Copper (10G) 
 * BCM84823 document refers to this loopback as XFI System loopback
 *                   at PCS of 10G 
 * See Figure 6 of datasheet, it's labelled as 2.
 * PCS control register (0x0000.14) to enable the loopback
 */
int bcm84823_pcs_loopback (int fd, uint8_t mdio_addr, uint8_t port_addr,
        uint8_t ena)
{
    int rc = 0;
    /* PCS Loopback */ 
    rc =  pcs_loopback(fd, mdio_addr, port_addr,ena); 

    return (rc);
}
/*
 * This loopback is towards system side.
 * BCM84823 document under XFI (MAC) to Copper (10G) 
 * BCM84823 document refers to this loopback as XFI System loopback
 *                   at XFI 
 */
int bcm84823_xfi_loopback (int fd, uint8_t mdio_addr, uint8_t port_addr,
        uint8_t ena)
{
    int rc = 0;
    uint16_t data;
    /* XFI Loopback */ 
    printf("XFI Loopback\n");
    if (ena) {
        BCM84823_RD(fd, mdio_addr, port_addr, BCM84823_UD_30_ADDR,
                    0x40A5, &data);
        data &= ~(0x7);
        data |= (0x4);
        BCM84823_WR(fd, mdio_addr, port_addr, BCM84823_UD_30_ADDR,
                    0x40A5, data);
        printf("\tM[%d.%d] W[%x.%x]= %x\n",
                  mdio_addr, port_addr, BCM84823_UD_30_ADDR, 0x40A5, data);
        BCM84823_RD(fd, mdio_addr, port_addr, BCM84823_UD_30_ADDR,
                    0x40A2, &data);
        data &= ~(0x1);
        BCM84823_WR(fd, mdio_addr, port_addr, BCM84823_UD_30_ADDR,
                    0x40A2, data);
        printf("\tM[%d.%d] W[%x.%x]= %x\n",
                  mdio_addr, port_addr, BCM84823_UD_30_ADDR, 0x40A2, data);
    }

    return (rc);
}
/*
 * This loopback is towards line side.
 * BCM8750 document calls this XFI Line Loopback Operation
 * See Figure 7 of datasheet, it's labelled as 3.
 * PMD control register (0x0000.0) to enable the loopback
 */
int bcm84823_xfi_line_loopback (int fd, uint8_t mdio_addr, uint8_t port_addr, uint8_t ena)
{
    int rc = 0;
    uint16_t data = 0x0;
    sel_device(fd, mdio_addr, port_addr, BCM84823_XFI_L_PMD);
    /* PCS Test Control register, bit 14 */ 
    BCM84823_RD(fd, mdio_addr, port_addr, 
            BCM84823_PCS_ADDR+1, 
            BCM84823_PMD_CTRL, &data); 
    if (ena) {
        // Enable loopback 
        data |= BCM84823_PMD_CTRL_FLD_PMD_LPBK;
    } else {
        data &= ~BCM84823_PMD_CTRL_FLD_PMD_LPBK;
    }
    BCM84823_WR(fd, mdio_addr, port_addr, BCM84823_PCS_ADDR+1, 
                BCM84823_PMD_CTRL, data); 
    sel_device(fd, mdio_addr, port_addr, BCM84823_XFI_L_PCS);
    return (rc);
}

int bcm84823_xfi_pcs_link_status_get (int fd, uint8_t mdio_addr, 
                                    uint8_t port_addr, uint8_t *link)
{
    int    rc = 0;
    uint16_t data = 0;
    sel_device(fd, mdio_addr, port_addr, BCM84823_XFI_L_PCS);
    /*
     rc = bcm84823_link_status_get (fd, mdio_addr, port_addr,
                                   BCM84823_PCS_ADDR+1, BCM84823_PCS_STATUS, 
                                   link);
     */
    /* Read Status  Register twice to get current link status */
    BCM84823_RD(fd, mdio_addr, port_addr, BCM84823_PCS_ADDR+1, 
                BCM84823_PCS_10GBASE_T_STATUS1, &data); 
    BCM84823_RD(fd, mdio_addr, port_addr, BCM84823_PCS_ADDR+1, 
                BCM84823_PCS_10GBASE_T_STATUS1, &data); 
    *link = ((data & BCM84823_PCS_10GBASE_T_STATUS1_FLD_RX_LINK_STATUS) >> 12);
    return (rc);
}
/*
 * Prints LDPC/CRC Errors
 */
int bcm84823_clear_errors (int fd, uint8_t mdio_addr, 
                           uint8_t port_addr)
{
    uint16_t data = 0x0;
    /* Clear 10G PCS Status registers */
    BCM84823_RD(fd, mdio_addr, port_addr, BCM84823_PCS_ADDR,
                BCM84823_PCS_10GBASE_T_STATUS1, &data);
    /*
    printf("\tM[%d.%d] R[%x.%x]= %x\n",
           mdio_addr, port_addr, BCM84823_PCS_ADDR, 
           BCM84823_PCS_10GBASE_T_STATUS1, data);
    */
    BCM84823_RD(fd, mdio_addr, port_addr, BCM84823_PCS_ADDR,
                BCM84823_PCS_10GBASE_T_STATUS1, &data);
    /*
    printf("\tM[%d.%d] R[%x.%x]= %x\n",
           mdio_addr, port_addr, BCM84823_PCS_ADDR, 
           BCM84823_PCS_10GBASE_T_STATUS1, data);
    */
    BCM84823_RD(fd, mdio_addr, port_addr, BCM84823_PCS_ADDR,
                BCM84823_PCS_10GBASE_T_STATUS2, &data);
    /*
    printf("\tM[%d.%d] R[%x.%x]= %x\n",
           mdio_addr, port_addr, BCM84823_PCS_ADDR, 
           BCM84823_PCS_10GBASE_T_STATUS2, data);
    */
    BCM84823_RD(fd, mdio_addr, port_addr, BCM84823_PCS_ADDR,
                BCM84823_PCS_10GBASE_T_STATUS2, &data);
    /*
    printf("\tM[%d.%d] R[%x.%x]= %x\n",
           mdio_addr, port_addr, BCM84823_PCS_ADDR, 
           BCM84823_PCS_10GBASE_T_STATUS2, data);
    */
     /* Clear transmit fault on XFI */
    BCM84823_RD(fd, mdio_addr, port_addr, BCM84823_UD_30_ADDR,
                0x40A2, &data);
    data &= ~(0x30);
    data |= 0x30;
    BCM84823_WR(fd, mdio_addr, port_addr, BCM84823_UD_30_ADDR,
                0x40A2, data);
    data &= ~(0x30);
    BCM84823_WR(fd, mdio_addr, port_addr, BCM84823_UD_30_ADDR,
                0x40A2, data);
    /*
    printf("\tM[%d.%d] W[%x.%x]= %x\n",
           mdio_addr, port_addr, BCM84823_UD_30_ADDR, 0x40A2, data);
    */
     /* Clear XFI PCS Status registers */
    BCM84823_RD(fd, mdio_addr, port_addr, BCM84823_PCS_ADDR+1,
                BCM84823_PCS_10GBASE_T_STATUS1, &data);
    /*
    printf("\tM[%d.%d] R[%x.%x]= %x\n",
           mdio_addr, port_addr, BCM84823_PCS_ADDR+1, 
           BCM84823_PCS_10GBASE_T_STATUS1, data);
    */
    BCM84823_RD(fd, mdio_addr, port_addr, BCM84823_PCS_ADDR+1,
                BCM84823_PCS_10GBASE_T_STATUS1, &data);
    /*
    printf("\tM[%d.%d] R[%x.%x]= %x\n",
           mdio_addr, port_addr, BCM84823_PCS_ADDR+1, 
           BCM84823_PCS_10GBASE_T_STATUS1, data);
    */
    BCM84823_RD(fd, mdio_addr, port_addr, BCM84823_PCS_ADDR+1,
                BCM84823_PCS_10GBASE_T_STATUS2, &data);
    /*
    printf("\tM[%d.%d] R[%x.%x]= %x\n",
           mdio_addr, port_addr, BCM84823_PCS_ADDR+1, 
           BCM84823_PCS_10GBASE_T_STATUS2, data);
    */
    BCM84823_RD(fd, mdio_addr, port_addr, BCM84823_PCS_ADDR+1,
                BCM84823_PCS_10GBASE_T_STATUS2, &data);
    /*
    printf("\tM[%d.%d] R[%x.%x]= %x\n",
           mdio_addr, port_addr, BCM84823_PCS_ADDR+1, 
           BCM84823_PCS_10GBASE_T_STATUS2, data);
    */
    BCM84823_RD(fd, mdio_addr, port_addr, BCM84823_UD_30_ADDR,
                0x4085, &data);
    BCM84823_RD(fd, mdio_addr, port_addr, BCM84823_UD_30_ADDR,
                0x4085, &data);

    BCM84823_RD(fd, mdio_addr, port_addr, BCM84823_UD_30_ADDR,
                0x40A8, &data);
    BCM84823_RD(fd, mdio_addr, port_addr, BCM84823_UD_30_ADDR,
                0x40A8, &data);

    BCM84823_RD(fd, mdio_addr, port_addr, BCM84823_UD_30_ADDR,
                0x40A5, &data);
    BCM84823_RD(fd, mdio_addr, port_addr, BCM84823_UD_30_ADDR,
                0x40A5, &data);

    BCM84823_RD(fd, mdio_addr, port_addr, BCM84823_UD_30_ADDR,
                0x4093, &data);
    BCM84823_RD(fd, mdio_addr, port_addr, BCM84823_UD_30_ADDR,
                0x4093, &data);

    BCM84823_RD(fd, mdio_addr, port_addr, BCM84823_UD_30_ADDR,
                0xa093, &data);
    BCM84823_RD(fd, mdio_addr, port_addr, BCM84823_UD_30_ADDR,
                0xa093, &data);

    BCM84823_RD(fd, mdio_addr, port_addr, BCM84823_UD_30_ADDR,
                0x4187, &data);
    BCM84823_RD(fd, mdio_addr, port_addr, BCM84823_UD_30_ADDR,
                0x4187, &data);

    BCM84823_RD(fd, mdio_addr, port_addr, BCM84823_UD_30_ADDR,
                0xA816, &data);
    BCM84823_RD(fd, mdio_addr, port_addr, BCM84823_UD_30_ADDR,
                0xA816, &data);
    /* Clear additional LDPC error counters */
    data = 0x0;
    BCM84823_WR(fd, mdio_addr, port_addr, BCM84823_PMA_PMD_ADDR,
                0xB489, data);
    BCM84823_WR(fd, mdio_addr, port_addr, BCM84823_PMA_PMD_ADDR,
                0xB48A, data);
    BCM84823_WR(fd, mdio_addr, port_addr, BCM84823_PMA_PMD_ADDR,
                0xB48B, data);
    BCM84823_WR(fd, mdio_addr, port_addr, BCM84823_PMA_PMD_ADDR,
                0xB48C, data);
    /* Clear wireside copper counters */
    data = 0x10; /* Enable wire side rx counter */
    BCM84823_WR(fd, mdio_addr, port_addr, BCM84823_PMA_PMD_ADDR,
                0xB981, data);
    /* Clear wire side rx counter */
    data = 0x2;
    BCM84823_WR(fd, mdio_addr, port_addr, BCM84823_UD_30_ADDR,
                0x418A, data);
    data = 0x0; 
    BCM84823_WR(fd, mdio_addr, port_addr, BCM84823_UD_30_ADDR,
                0x418A, data);
    return (0);
}

int bcm84823_10g_pcs_link_status_get (int fd, uint8_t mdio_addr, 
                                    uint8_t port_addr, uint8_t *link)
{
    int rc = 0, count = 0;
    uint16_t data = 0x0;
    uint8_t loopback;
    rc = bcm84823_link_status_get(fd, mdio_addr, port_addr,
                                   BCM84823_PCS_ADDR, BCM84823_PCS_STATUS, 
                                   link);
    if (!(*link)) {
        bcm84823_10g_loopback_get(fd, mdio_addr, port_addr, &loopback);
        if (loopback == 1) {
            *link = 1;
        } 
        /* For 1G Links, check CU Link in 1E.400D */
        BCM84823_RD(fd, mdio_addr, port_addr, BCM84823_UD_30_ADDR,
                BCM84823_UD_STATUS, &data);
        if ((data & BCM84823_UD_STATUS_FLD_CU_LINK_STAT) &&
            (((data & BCM84823_UD_STATUS_FLD_CU_SPEED) >> 3) == 0x2)) {
            *link = 1;
        } else {
            return (rc);
        }
    }
    do {
        BCM84823_RD(fd, mdio_addr, port_addr, BCM84823_UD_30_ADDR,
                BCM84823_UD_STATUS, &data);
        sleep(1);
        count++;
    } while (((data & 0x20) != 0x20) && (count < 0x10));
    BCM84823_RD(fd, mdio_addr, port_addr, BCM84823_UD_30_ADDR,
                BCM84823_UD_STATUS, &data);
    /* 
    printf("\tM[%d.%d] R[%x.%x]= %x\n",
           mdio_addr, port_addr, BCM84823_UD_30_ADDR, BCM84823_UD_STATUS, data);
     */
     /* Setup XFI access */
    data = 0x1800;
    BCM84823_WR(fd, mdio_addr, port_addr, BCM84823_UD_30_ADDR,
                BCM84823_UD_0X4131, data);
    BCM84823_RD(fd, mdio_addr, port_addr, BCM84823_UD_30_ADDR,
                BCM84823_UD_0X4131, &data);
    /*
    printf("\tM[%d.%d] R[%x.%x]= %x\n",
           mdio_addr, port_addr, BCM84823_UD_30_ADDR,
           BCM84823_UD_0X4131, data);
     */
    rc =  bcm84823_clear_errors(fd, mdio_addr, port_addr);

    return (rc);
}

int bcm84823_10g_pmd_link_status_get (int fd, uint8_t mdio_addr, 
                                    uint8_t port_addr, uint8_t *link)
{
    int rc = 0;
    rc = bcm84823_link_status_get (fd, mdio_addr, port_addr,
                BCM84823_PMA_PMD_ADDR, BCM84823_PMD_STATUS, link);
    return (rc);
}


/*
 * TODO: Need to look at this function
 */

int bcm84823_port_ena (int fd, uint8_t mdio_addr, uint8_t port_addr, 
            uint8_t ena)
{
    int rc = 0;
    uint16_t data;

    BCM84823_RD(fd, mdio_addr, port_addr, BCM84823_PCS_ADDR, 
                    BCM84823_PCS_DEV_ID1, &data); 

    if (data == BCM84823_PCS_DEV_ID1_DFLT) {
        data = BCM84823_UD_PMD_OPTICS_DIGITAL_CTRL_FLD_PCS_TX_INVERT;
        BCM84823_WR(fd, mdio_addr, port_addr, BCM84823_PMA_PMD_ADDR, 
                   BCM84823_UD_PMD_OPTICS_DIGITAL_CTRL, data); 
    }

    return (rc);
}
/*
 * Enable User PRBS
 * PRBS7, 9, 11, 15, 23 are supported
 */
static int bcm84823_user_prbs (int fd, uint8_t mdio_addr, uint8_t port_addr,
                       uint8_t prbs_sel, int ena)
{
    uint16_t data;
    BCM84823_RD(fd, mdio_addr, port_addr, BCM84823_PCS_ADDR+1,
               0xCD15, &data);
    BCM84823_RD(fd, mdio_addr, port_addr, BCM84823_PCS_ADDR+1,
               0xCD14, &data);
    /* select PRBS type and Enable PRBS */
    data &= ~(BCM84823_UD_PMD_PRBS_CTRL_0_FLD_PRBS_SEL);
    data |= (prbs_sel);
    /* data |= 0x1000; */
    if (ena) {
        data |= BCM84823_UD_PMD_PRBS_CTRL_0_FLD_USER_PRBS_EN;
    } else {
        /* Write default value */
        data = 0x8018;
    }
    DIAG_PRINT(DIAG_PRINT_CONFIG, "prbs %x %x %d\n", prbs_sel, data, ena);
    BCM84823_WR(fd, mdio_addr, port_addr, BCM84823_PCS_ADDR+1,
               0xCD14, data);
    return (0);
}
static int bcm84823_user_prbs_check (int fd, uint8_t mdio_addr, 
                                    uint8_t port_addr)
{
    uint16_t data;
    BCM84823_RD(fd, mdio_addr, port_addr, BCM84823_PCS_ADDR+1,
               0xCD15, &data);

    if (!(data & BCM84823_UD_PMD_PRBS_STATUS_0_FLD_PRBS_LOCK)) {
        printf("RX PRBS is not locked\n");
    }
    if ((data & 0x7fff) > 0) {
        printf("PRBS RX Error Count %d\n", (data&0x7fff));
    }
    return (0);
}

static int bcm84823_xfi_init(int fd, uint8_t mdio_addr, uint8_t port_addr)
{
    uint16_t data=0x0;
    data = 0x1800;
    BCM84823_WR(fd, mdio_addr, port_addr, BCM84823_UD_30_ADDR,
                BCM84823_UD_0X4131, data);

    data = 0x0;
    BCM84823_RD(fd, mdio_addr, port_addr, BCM84823_UD_30_ADDR,
                BCM84823_UD_0X4131, &data);
    if (data != 0x1800) {
        printf("\tM[%d.%d] W[%x.%x]= %x\n",
               mdio_addr, port_addr, BCM84823_UD_30_ADDR,
               BCM84823_UD_0X4131, data);
    }
    return (0);
}

static unsigned short mdio_rd_bcm84823_aux (int fd, uint8_t mdio_addr,
                                            uint8_t port_addr, int reg)
{
    uint16_t d2, data;
    d2 = ((reg & 0x07) << 12) | 7;
    BCM84823_WR(fd, mdio_addr, port_addr, BCM84823_1GE_ADDR,
                BCM84823_UD_1000BASE_T_AUX_CTRL_SHDW_VAL_ACC, d2);
    BCM84823_RD(fd, mdio_addr, port_addr, BCM84823_1GE_ADDR,
                BCM84823_UD_1000BASE_T_AUX_CTRL_SHDW_VAL_ACC, &data);
    return (data);
}

static unsigned short mdio_wr_bcm84823_aux (int fd, uint8_t mdio_addr,
                        uint8_t port_addr, int reg, uint16_t data)
{
    uint16_t d2;
    d2 = (data & 0xFFF8) | (reg & 0x7);
    BCM84823_WR(fd, mdio_addr, port_addr, BCM84823_1GE_ADDR,
                BCM84823_UD_1000BASE_T_AUX_CTRL_SHDW_VAL_ACC, d2);
    return (0);
}

static int bcm84823_disable(int fd, uint8_t mdio_addr, uint8_t port_addr)
{
    uint16_t data = 0;
    
    data = mdio_rd_bcm84823_aux(fd, mdio_addr, port_addr, 2); 
    data |= (1<<5); // assert super isolate
    mdio_wr_bcm84823_aux(fd, mdio_addr, port_addr, 2, data);
    /* Advertise nothing */
    bcm84823_write_seq(fd, mdio_addr, port_addr, bcm84823_pgm_adv_nothing[0],
                       sizeof(bcm84823_pgm_adv_nothing));
    return (0);
}

static int bcm84823_enable(int fd, uint8_t mdio_addr, uint8_t port_addr)
{
    uint16_t data = 0;
    
    data = mdio_rd_bcm84823_aux(fd, mdio_addr, port_addr, 2); 
    data &= ~(1<<5); // deassert super isolate
    mdio_wr_bcm84823_aux(fd, mdio_addr, port_addr, 2, data);
    return (0);
}


static int bcm84823_10g_init(int fd, uint8_t mdio_addr, uint8_t port_addr)
{
    uint16_t data = 0x4, cnt;
    BCM84823_WR(fd, mdio_addr, port_addr, BCM84823_UD_30_ADDR,
                0x4005, data);
    bcm84823_disable(fd, mdio_addr, port_addr);
    bcm84823_enable(fd, mdio_addr, port_addr);
    cnt = 0;
#if defined(DIAG_BROCKWAY)
    BCM84823_WR(fd, mdio_addr, port_addr, BCM84823_PMA_PMD_ADDR,
                0xA819, 0xcfbc);
    BCM84823_WR(fd, mdio_addr, port_addr, BCM84823_PMA_PMD_ADDR,
                0xA81A, 0x3);
    BCM84823_WR(fd, mdio_addr, port_addr, BCM84823_PMA_PMD_ADDR,
                0xA81B, 0);
    BCM84823_WR(fd, mdio_addr, port_addr, BCM84823_PMA_PMD_ADDR,
                0xA81C, 0);
    BCM84823_WR(fd, mdio_addr, port_addr, BCM84823_PMA_PMD_ADDR,
                0xA817, 0x0009);
    for (cnt = 0; cnt < 0xff; cnt++) {
        data = 0x0;
        BCM84823_RD(fd, mdio_addr, port_addr, BCM84823_PMA_PMD_ADDR,
                0xA818, &data);
        if (data & 0x1) {
            printf("CUPHY %d.%d: EMI Retrain disabled\n", mdio_addr, port_addr);
            break;
        }
    }
#endif


    return (0);
}
static int read_ldpc_errors(int fd, uint8_t mdio_addr, uint8_t port_addr,
                            uint16_t *ldpc_serr_lo, uint16_t *ldpc_serr_hi,
                            uint16_t *ldpc_cerr_lo, uint16_t *ldpc_cerr_hi)
{
    int rc = 0;
    /* LDPC Error Counters */
    BCM84823_RD(fd, mdio_addr, port_addr, BCM84823_PMA_PMD_ADDR,
               0xb489, ldpc_serr_lo);
    BCM84823_RD(fd, mdio_addr, port_addr, BCM84823_PMA_PMD_ADDR,
               0xb48a, ldpc_serr_hi);
    BCM84823_RD(fd, mdio_addr, port_addr, BCM84823_PMA_PMD_ADDR,
               0xb48b, ldpc_cerr_lo);
    BCM84823_RD(fd, mdio_addr, port_addr, BCM84823_PMA_PMD_ADDR,
               0xb48c, ldpc_cerr_hi);
    return (rc);
}

int bcm84823_write_seq (int fd, uint8_t mdio, uint8_t port,
                        uint16_t *data_ptr, uint32_t data_size)
{
    uint32_t row_idx;
    int rc = 0;

    for (row_idx = 0;row_idx < data_size/(4 * sizeof(uint16_t));
         row_idx++) {
        BCM84823_WR(fd, mdio, port, data_ptr[1], data_ptr[2], data_ptr[3]);
     /*   fprintf(logfile, " mdio %x port %x device %x addr %x data %x\n",
                 mdio, port, data_ptr[1], data_ptr[2],data_ptr[3]); */
        data_ptr += 4;
    }
    return rc;
}
/*
 * Prints Internal Die Temperature
 */
int bcm84823_print_tempsensor (int fd, uint8_t mdio_addr, 
                               uint8_t port_addr, int port)
{
    uint16_t data, temp_value;
    float sensor;
    data = 0x8;
    BCM84823_WR(fd, mdio_addr, port_addr, BCM84823_UD_30_ADDR,
               0x4189, data);
    data = 0x601;
    BCM84823_WR(fd, mdio_addr, port_addr, BCM84823_UD_30_ADDR,
               0x402b, data);
    BCM84823_RD(fd, mdio_addr, port_addr, BCM84823_UD_30_ADDR,
               0x402c, &temp_value);

    sensor = 270.806 - (float) (0.7072 * temp_value);
    printf("CUPHY Port [%d] hex_value %x sensor = %f\n", port, 
            temp_value, sensor);

    return (0);
}
/*
 * Prints LDPC/CRC Errors
 */
int bcm84823_display_errors (int fd, uint8_t mdio_addr, 
                             uint8_t port_addr, int port)
{
    uint16_t data_status2, fifo_err, data;
    uint16_t ldpc_serr_0_15 = 0, ldpc_serr_16_31 = 0;
    uint16_t ldpc_cerr_0_15 = 0, ldpc_cerr_16_31 = 0;

    BCM84823_RD(fd, mdio_addr, port_addr, BCM84823_PCS_ADDR,
               BCM84823_PCS_10GBASE_T_STATUS2, &data_status2);

    BCM84823_RD(fd, mdio_addr, port_addr, BCM84823_UD_30_ADDR,
               0xa816, &fifo_err);
    BCM84823_RD(fd, mdio_addr, port_addr, BCM84823_UD_30_ADDR,
               0xa816, &fifo_err);
    /* LDPC Error Counters */
    read_ldpc_errors(fd, mdio_addr,port_addr,
                     &ldpc_serr_0_15, &ldpc_serr_16_31,
                     &ldpc_cerr_0_15, &ldpc_cerr_16_31);
    data = 0x2;
    BCM84823_WR(fd, mdio_addr, port_addr, BCM84823_UD_30_ADDR,
               0x418a, data);
    data = 0x0;
    BCM84823_WR(fd, mdio_addr, port_addr, BCM84823_UD_30_ADDR,
               0x418a, data);

    printf("%d\t0x%04x\t0x%04x\t0x%04x%04x\t0x%04x%04x\n",
            port, data_status2, fifo_err,ldpc_serr_16_31, ldpc_serr_0_15,
            ldpc_cerr_16_31, ldpc_cerr_0_15);

    return (0);
}
/*
 * Prints Wireside packet counters 
 */
int bcm84823_display_counters (int fd, uint8_t mdio_addr, 
                             uint8_t port_addr, int port)
{
    uint16_t data;
    uint16_t pkt_cnt_0_15, pkt_cnt_16_31;

    data = 0x2;
    BCM84823_WR(fd, mdio_addr, port_addr, BCM84823_UD_30_ADDR,
               0x418a, data);
    data = 0x0;
    BCM84823_WR(fd, mdio_addr, port_addr, BCM84823_UD_30_ADDR,
               0x418a, data);
    /* Packet counters on Wireside Rx */
    BCM84823_RD(fd, mdio_addr, port_addr, BCM84823_PMA_PMD_ADDR,
               0xb98c, &pkt_cnt_0_15);
    BCM84823_RD(fd, mdio_addr, port_addr, BCM84823_PMA_PMD_ADDR,
               0xb98d, &pkt_cnt_16_31);
    printf("%d\t0x%04x%04x\n", port, pkt_cnt_16_31, pkt_cnt_0_15);

    return (0);
}
/*
 * PRBS on XFI side
 */
int bcm84823_xfi_user_prbs (int fd, uint8_t mdio_addr, uint8_t port_addr,
                           uint8_t prbs_sel, int mode)
{
    int rc = 0;
    /* setup access to XFI PMA/PMD */
    sel_device(fd, mdio_addr, port_addr, BCM84823_XFI_L_PMD);
    if ((mode == BCM84823_PRBS_ENA) || (mode == BCM84823_PRBS_DIS)) {
        rc = bcm84823_user_prbs(fd, mdio_addr, port_addr, prbs_sel, mode);
    } else if (mode == BCM84823_PRBS_CHECK) {
        rc = bcm84823_user_prbs_check(fd, mdio_addr, port_addr);
    }
    /* setup access to XFI PCS - Default */
    sel_device(fd, mdio_addr, port_addr, BCM84823_XFI_L_PCS);
    return (rc);
}

int bcm84823_xfi_bist_test_start (int fd, uint8_t mdio_addr, uint8_t port_addr)
{
    int rc = 0;
    /* 
     * Enable PCS loopback on 10G as XFI is in powered down state if
     * 10G on copper side does not link up.
     */
    rc = bcm84823_pcs_loopback(fd, mdio_addr, port_addr, 1);
    /* 
     * Enable PMD loopback on XFI (towards line) to
     * loopback PRBS traffic back to XFI checker
     */
    rc = bcm84823_xfi_line_loopback(fd, mdio_addr, port_addr, 1);
    /* select PRBS 7 */
    rc = bcm84823_xfi_user_prbs(fd, mdio_addr, port_addr, BCM84823_SEL_PRBS7, 
                                BCM84823_PRBS_ENA);
    return (rc);
}

int bcm84823_xfi_bist_test_verify (int fd, uint8_t mdio_addr, uint8_t port_addr)
{
    int rc = 0;
    rc = bcm84823_xfi_user_prbs(fd, mdio_addr, port_addr, 0, 
                                BCM84823_PRBS_CHECK);
    rc = bcm84823_xfi_user_prbs(fd, mdio_addr, port_addr, 0, 
                                BCM84823_PRBS_DIS);
    /*
     * clear both XFI line side and 10GbastT PCS loopbacks
     */
    rc = bcm84823_xfi_line_loopback(fd, mdio_addr, port_addr, 0);
    rc = bcm84823_pcs_loopback(fd, mdio_addr, port_addr, 0);
    return (rc);
}

/*
 * EDC-code download. via mdio pin 
 */
int bcm84823_eeprom_init  (int fd, uint8_t mdio_addr, uint8_t port_addr)
{
    int rc = 0;
    uint16_t hiVal;
    BCM84823_RD(fd, mdio_addr, port_addr, BCM84823_UD_30_ADDR,
                        0x400F, &hiVal);
    if (hiVal != BCM84823_UCODE_LATEST_VER) {
        bcm84823_download_microcode_single_phy(fd, 
            "/diag/BCM8482-v02-25-TCM.bin", mdio_addr, port_addr);
    } else {
        printf("Version upto date, Current 0x%04X\n", hiVal);
    }
    return (rc);
}


int bcm84823_init (int fd, uint8_t mdio_addr, uint8_t port_addr)
{
    int rc = 0;
    rc = bcm84823_xfi_init(fd, mdio_addr, port_addr);
    if (rc) {
        return (rc);
    }
    rc = bcm84823_10g_init(fd, mdio_addr, port_addr);
    if (rc) {
        return (rc);
    }
    rc = bcm84823_led_init(fd, mdio_addr, port_addr);
    return (rc);

}

// led_pin take values 1 to 5.
int bcm84823_led_clr (int fd, uint8_t mdio_addr, uint8_t port_addr, 
                      uint8_t led_pin)
{
    int rc = 0;
    uint16_t data;
        
    BCM84823_RD(fd, mdio_addr, port_addr, BCM84823_PMA_PMD_ADDR,
                    BCM84823_10GBASET_LED_CONTROL, &data);
    /* Disable LED Output */
    data |= BCM84823_10GBASET_LED_CONTROL_FLD_LED_OE_N(led_pin);
    /* LED Off */
    data &= ~BCM84823_10GBASET_LED_CONTROL_FLD_LED_CTL(3, led_pin);
    BCM84823_WR(fd, mdio_addr, port_addr, BCM84823_PMA_PMD_ADDR,
                    BCM84823_10GBASET_LED_CONTROL, data);

    BCM84823_RD(fd, mdio_addr, port_addr, BCM84823_PMA_PMD_ADDR,
                (BCM84823_10GBASET_CTL_LED1_MASK_LOW+ (3*(led_pin-1))), &data);
    data &= ~BCM84823_10GBASET_CTL_LED_FLD_LED_ON;
    BCM84823_WR(fd, mdio_addr, port_addr, BCM84823_PMA_PMD_ADDR,
                (BCM84823_10GBASET_CTL_LED1_MASK_LOW+ (3*(led_pin-1))), data);

    return (rc);
}

// gpio_pin take values 0 or 1.
int bcm84823_led_set (int fd, uint8_t mdio_addr, uint8_t port_addr, 
                      uint8_t led_pin)
{
    int rc = 0;
    uint16_t data;

    BCM84823_RD(fd, mdio_addr, port_addr, BCM84823_PMA_PMD_ADDR,
                    BCM84823_10GBASET_LED_CONTROL, &data);
    /* Enabled LED Output */
    data &= ~BCM84823_10GBASET_LED_CONTROL_FLD_LED_OE_N(led_pin);
    data &= ~BCM84823_10GBASET_LED_CONTROL_FLD_LED_CTL(3, led_pin);
    /* LED based on source */
    data |= BCM84823_10GBASET_LED_CONTROL_FLD_LED_CTL(2, led_pin);
    BCM84823_WR(fd, mdio_addr, port_addr, BCM84823_PMA_PMD_ADDR,
                    BCM84823_10GBASET_LED_CONTROL, data);

    BCM84823_RD(fd, mdio_addr, port_addr, BCM84823_PMA_PMD_ADDR,
                (BCM84823_10GBASET_CTL_LED1_MASK_LOW+ (3*(led_pin-1))), &data);
    data |= BCM84823_10GBASET_CTL_LED_FLD_LED_ON;
    BCM84823_WR(fd, mdio_addr, port_addr, BCM84823_PMA_PMD_ADDR,
                (BCM84823_10GBASET_CTL_LED1_MASK_LOW+ (3*(led_pin-1))), data);
    return (rc);
}


int bcm84823_lpbk_print (int fd, uint8_t mdio_addr, uint8_t port_addr,
                         uint8_t hdr, int phy_port)
{
    int rc = 0;
    uint8_t lpbk;

    if (hdr) {
            printf("\t\t10g-pcs\txfi-line\n");
            printf("\t\t=======\t========\n");
    }

    printf("Port(%d) \t", phy_port);
    rc = bcm84823_10g_loopback_get(fd, mdio_addr, port_addr, &lpbk);
    printf("%s\t", rc? "-" : (lpbk ? "On" : "Off"));

    rc = bcm84823_xfi_line_loopback_get(fd, mdio_addr, port_addr, &lpbk);
    printf("%s\t", rc? "-" : (lpbk ? "On" : "Off"));

    printf("\n");
    return (rc);
}

int bcm84823_link_status_print (int fd, uint8_t mdio_addr, uint8_t port_addr, 
                                uint8_t hdr, int phy_port)
{
        int rc = 0;
        uint8_t link;

        if (hdr) {
                printf("\t\txfi-pcs\t10g-pcs\t10g-pmd\n");
                printf("\t\t=========================\n");
        }

        printf("Port (%d)\t", phy_port);

        rc = bcm84823_xfi_pcs_link_status_get(fd, mdio_addr, port_addr, &link);
        printf("%s\t", rc? " - " : (link ? "On" : "Off"));

        rc = bcm84823_10g_pcs_link_status_get(fd, mdio_addr, port_addr, &link);
        printf("%s\t", rc? " - " : (link ? "On" : "Off"));

        rc = bcm84823_10g_pmd_link_status_get(fd, mdio_addr, port_addr, &link);
        printf("%s\t", rc? " - " : (link ? "On" : "Off"));

        printf("\n");
        return (0);
}

extern int pca9539_wr (int, uint16_t, uint8_t, uint8_t*);
extern int ppc_i2c_open (int);
extern int ppc_i2c_close (int);
int bcm84823_poll_errs (int fd, uint8_t mdio_addr, uint8_t port_addr,
                        int slp_cnt)
{
    uint16_t ldpc_serr_0_15 = 0, ldpc_serr_16_31 = 0;
    uint16_t ldpc_cerr_0_15 = 0, ldpc_cerr_16_31 = 0;
    uint32_t ldpc_err = 0;
    uint8_t toggle_data = 0x80;
    int io_fd= 0;
    read_ldpc_errors(fd, mdio_addr,port_addr,
                     &ldpc_serr_0_15, &ldpc_serr_16_31,
                     &ldpc_cerr_0_15, &ldpc_cerr_16_31);
    ldpc_err = ((ldpc_serr_16_31 + ldpc_serr_0_15) +
                (ldpc_cerr_16_31 + ldpc_cerr_0_15));
    if (ldpc_err) {
        printf("LDPC Error on Cuphy %d.%d\n", mdio_addr, port_addr);
        io_fd = ppc_i2c_open(2);
        pca9539_wr(io_fd,0x75,0x2, &toggle_data);
        ppc_i2c_close(io_fd);
        return (1);
    }
    return (0);
}

int bcm84823_cfg_speed (int fd, uint8_t mdio_addr, uint8_t port_addr,
                        uint32_t speed)
{
    DIAG_PRINT(DIAG_PRINT_CONFIG, "BCM84823_CFG_SPEED %x %x %d\n", 
               mdio_addr, port_addr, speed);
    if (speed == 0) {
        /* Advertise 10G */
        bcm84823_write_seq(fd, mdio_addr, port_addr, bcm84823_pgm_adv_10G[0],
                           sizeof(bcm84823_pgm_adv_nothing));
    } else if (speed == 1) {
        /* Advertise 1G */
        bcm84823_write_seq(fd, mdio_addr, port_addr, bcm84823_pgm_adv_1G[0],
                           sizeof(bcm84823_pgm_adv_nothing));
    }
    return (0);
}
