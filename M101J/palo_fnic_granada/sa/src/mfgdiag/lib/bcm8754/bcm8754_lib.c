#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include "bcm8754_lib.h"
#include "bcm8754.h"
#include "phy_rom_update.h"
#include "bcm8754_prog.h"
#include "diag_main.h"

int sel_sfi_xfi_device (int fd, uint8_t mdio_addr, uint8_t port_addr, 
                               int xfi_sel)
{
    int rc = 0;
    uint16_t data;
    /* 
     * To access SFI registers, write 1.FFFF.0 = 0
     * To access XFI registers, write 1.FFFF.0 = 1
     */
    BCM8754_RD(fd, mdio_addr, port_addr, 
            BCM8754_PMA_PMD_ADDR, 
            BCM8754_XFI_SFI_SEL, &data); 
    if (xfi_sel) {
        data |= (BCM8754_XFI_SFI_FLD_DEV_SEL);
    } else {
        data &= ~(BCM8754_XFI_SFI_FLD_DEV_SEL);
    }
    BCM8754_WR(fd, mdio_addr, port_addr, 
            BCM8754_PMA_PMD_ADDR, 
            BCM8754_XFI_SFI_SEL, data); 
    return (rc);
}
/*  
 * Reset PMA/PMD Logic 
 * No need for unreset function, as reset bit is self clearing
 */
int pma_pmd_reset (int fd, uint8_t mdio_addr, uint8_t port_addr)
{
    int rc = 0;
    uint16_t data;
    BCM8754_RD(fd, mdio_addr, port_addr, BCM8754_PMA_PMD_ADDR,
                BCM8754_PMD_CTRL, &data); 
    data |= BCM8754_PMD_CTRL_FLD_RESET;
    BCM8754_WR(fd, mdio_addr, port_addr, BCM8754_PMA_PMD_ADDR,
                BCM8754_PMD_CTRL, data); 
    return (rc);
}
/*  
 * PMA/PMD Loopback Logic 
 */
static int pma_pmd_loopback (int fd, uint8_t mdio_addr, uint8_t port_addr,
                     uint8_t ena)
{
    int rc = 0;
    uint16_t data;
    BCM8754_RD(fd, mdio_addr, port_addr, 
            BCM8754_PMA_PMD_ADDR, 
            BCM8754_PMD_CTRL, &data); 
    if (ena) {
        // Enable loopback 
        data |= BCM8754_PMD_CTRL_FLD_PMD_LPBK;
    } else {
        data &= ~BCM8754_PMD_CTRL_FLD_PMD_LPBK;
    }
    BCM8754_WR(fd, mdio_addr, port_addr, 
            BCM8754_PMA_PMD_ADDR, 
            BCM8754_PMD_CTRL, data); 
    return (rc);
}
/*  
 * PMD Test control Loopback Logic 
 */
static int pmd_test_ctrl_loopback (int fd, uint8_t mdio_addr, uint8_t port_addr,
                                   uint8_t ena)
{
    int rc = 0;
    uint16_t data;
    /* XFI PMD & PCS Test Control register, bit 0 */ 
    BCM8754_RD(fd, mdio_addr, port_addr, 
            BCM8754_PMA_PMD_ADDR, 
            BCM8754_UD_PMD_TEST_CTRL, &data); 
    if (ena) {
        // Enable loopback 
        data |= BCM8754_UD_PMD_TEST_CTRL_FLD_PMD_PMA_LINELPBK;
    } else {
        data &= ~BCM8754_UD_PMD_TEST_CTRL_FLD_PMD_PMA_LINELPBK;
    }
    BCM8754_WR(fd, mdio_addr, port_addr, 
            BCM8754_PMA_PMD_ADDR, 
            BCM8754_UD_PMD_TEST_CTRL, data); 
    return (rc);
}

static int bcm8754_link_status_get (int fd, uint8_t mdio_addr, uint8_t port_addr,
                                    uint8_t device_class, uint8_t addr, uint8_t *link)
{
    int rc=0;
    uint16_t data;
    /* Read Status 2 Register twice to get rid of sticky fault */
    BCM8754_RD(fd, mdio_addr, port_addr, device_class, 0x8, &data); 
    BCM8754_RD(fd, mdio_addr, port_addr, device_class, 0x8, &data); 
    /* Read Status  Register twice to get current link status */
    BCM8754_RD(fd, mdio_addr, port_addr, device_class, addr, &data); 
    BCM8754_RD(fd, mdio_addr, port_addr, device_class, addr, &data); 

    *link  = (data & BCM8754_PCS_STATUS_FLD_RX_LINK_STATUS) ? 1 : 0;
    return (rc);
}

static int pma_pmd_link_status_get (int fd, uint8_t mdio_addr, 
                                uint8_t port_addr, uint8_t *link)
{
    return(bcm8754_link_status_get (fd, mdio_addr, port_addr,
                BCM8754_PMA_PMD_ADDR, BCM8754_PMD_STATUS, link));
}

static int bcm8754_eeprom_load_test (int fd, uint8_t mdio_addr, 
                                     uint8_t port_addr, int *prom_err, int flag)
{
    int rc = 0;
    uint16_t data;

    *prom_err = 0x00;

    BCM8754_RD(fd, mdio_addr, port_addr, BCM8754_PMA_PMD_ADDR, 
               BCM8754_MSG_OUT, &data); 

    if (data != BCM8754_EEPROM_SIG) {
            *prom_err = -1;
        if (flag) {
             printf("  EEPROM Load Failed [0x%X] : 0x%X\n",
                   BCM8754_MSG_OUT, data);
        }
        return (rc);
    } 
    BCM8754_RD(fd, mdio_addr, port_addr, BCM8754_PMA_PMD_ADDR, 0x9003,
                    &data); 
    return (rc);    
}

int bcmphy_reset (int fd, uint8_t mdio_addr, uint8_t port_addr)
{
    int rc = 0;
    sel_sfi_xfi_device(fd, mdio_addr, port_addr, BCM8754_SFI_SELECT);
    rc = pma_pmd_reset(fd, mdio_addr, port_addr);
    if (rc) {
        return (rc);
    }
    return (rc);
}
/*
 * This loopback is towards system side.
 * BCM8750 document calls this MMF PCS/PMD System Loopback Operation
 * See Figure 7 of datasheet, it's labelled as 2.
 * PMD control register (0x0000.0) to enable the loopback
 */
int bcmphy_pcs_loopback (int fd, uint8_t mdio_addr, uint8_t port_addr,
        uint8_t ena)
{
    int rc = 0;
    uint16_t data;
    sel_sfi_xfi_device(fd, mdio_addr, port_addr, BCM8754_SFI_SELECT);
    /* MMF/SFI PMD Ctrl register, bit 0 */ 
    rc =  pma_pmd_loopback(fd, mdio_addr, port_addr,ena); 

    BCM8754_RD(fd, mdio_addr, port_addr, BCM8754_PMA_PMD_ADDR,
               BCM8754_UD_PHY_ID, &data); 
    if (ena) {
        data &= ~BCM8754_UD_PHY_ID_FLD_OPRXLOS_LVL;
    } else {
        data |= BCM8754_UD_PHY_ID_FLD_OPRXLOS_LVL;
    }
    BCM8754_WR(fd, mdio_addr, port_addr, BCM8754_PMA_PMD_ADDR,
               BCM8754_UD_PHY_ID, data); 
    return (rc);
}
/*
 * This loopback is towards system side.
 * BCM8750 document calls this XFI Line Loopback Operation
 * See Figure 7 of datasheet, it's labelled as 4.
 * PMD & PCS Test control register (0xCD0A.0) to enable the loopback
 */
int bcmphy_xfi_loopback (int fd, uint8_t mdio_addr, uint8_t port_addr,
                         uint8_t ena)
{
    int rc = 0;
    sel_sfi_xfi_device(fd, mdio_addr, port_addr, BCM8754_XFI_SELECT);
    /* XFI PMD & PCS Test Control register, bit 0 */ 
    rc =  pmd_test_ctrl_loopback(fd, mdio_addr, port_addr, ena); 
    return (rc);
}
/*
 * This loopback is towards Line side.
 * BCM8750 document calls this MMF Line Loopback Operation
 * See Figure 7 of datasheet, it's labelled as 1.
 * PMD & PCS Test control register (0xCD0A.0) to enable the loopback
 */
int bcmphy_sfi_line_loopback (int fd, uint8_t mdio_addr, uint8_t port_addr, uint8_t ena)
{
    int rc = 0;
    sel_sfi_xfi_device(fd, mdio_addr, port_addr, BCM8754_SFI_SELECT);
    /* XFI PMD & PCS Test Control register, bit 0 */ 
    rc =  pmd_test_ctrl_loopback(fd, mdio_addr, port_addr, ena); 
    return (rc);
}
/*
 * This loopback is towards line side.
 * BCM8750 document calls this XFI Line Loopback Operation
 * See Figure 7 of datasheet, it's labelled as 3.
 * PMD control register (0x0000.0) to enable the loopback
 */

int bcmphy_xfi_line_loopback (int fd, uint8_t mdio_addr, uint8_t port_addr, uint8_t ena)
{
    int rc = 0;
    sel_sfi_xfi_device(fd, mdio_addr, port_addr, BCM8754_XFI_SELECT);
    /* XFI PMD Ctrl register, bit 0 */ 
    rc =  pma_pmd_loopback(fd, mdio_addr, port_addr, ena); 
    sel_sfi_xfi_device(fd, mdio_addr, port_addr, BCM8754_SFI_SELECT);
    return (rc);
}

int bcmphy_xfi_pcs_link_status_get (int fd, uint8_t mdio_addr, 
                                    uint8_t port_addr, uint8_t *link)
{
    int    rc = 0;
    sel_sfi_xfi_device(fd, mdio_addr, port_addr, BCM8754_XFI_SELECT);
    rc = bcm8754_link_status_get (fd, mdio_addr, port_addr,
                                      BCM8754_PCS_ADDR, BCM8754_PCS_STATUS, 
                                      link);
    sel_sfi_xfi_device(fd, mdio_addr, port_addr, BCM8754_SFI_SELECT);
    return (rc);
}


int bcm8754_xfi_loopback_get (int fd, uint8_t mdio_addr, uint8_t port_addr,
                          uint8_t device_class, uint16_t addr, 
                              uint16_t lpbk_bit, uint8_t *lpbk)
{
    int rc = 0;
    uint16_t data;
    sel_sfi_xfi_device(fd, mdio_addr, port_addr, BCM8754_XFI_SELECT);
    BCM8754_RD(fd, mdio_addr, port_addr, device_class,
               addr, &data); 
    *lpbk  = (data & lpbk_bit) ? 1 : 0;
    sel_sfi_xfi_device(fd, mdio_addr, port_addr, BCM8754_SFI_SELECT);
    return (rc);
}

int bcm8754_sfi_loopback_get (int fd, uint8_t mdio_addr, uint8_t port_addr,
                          uint8_t device_class, uint16_t addr, 
                              uint16_t lpbk_bit, uint8_t *lpbk)
{
    int rc = 0;
    uint16_t data;
    sel_sfi_xfi_device(fd, mdio_addr, port_addr, BCM8754_SFI_SELECT);
    BCM8754_RD(fd, mdio_addr, port_addr, device_class,
               addr, &data); 
    *lpbk  = (data & lpbk_bit) ? 1 : 0;
    return (rc);
}

int bcmphy_sfi_pcs_link_status_get (int fd, uint8_t mdio_addr, 
                                    uint8_t port_addr, uint8_t *link)
{
    int rc = 0;
    sel_sfi_xfi_device(fd, mdio_addr, port_addr, BCM8754_SFI_SELECT);
    rc = bcm8754_link_status_get (fd, mdio_addr, port_addr,
                                      BCM8754_PCS_ADDR, BCM8754_PCS_STATUS, 
                                      link);
    return (rc);
}

int bcmphy_xfi_pmd_link_status_get (int fd, uint8_t mdio_addr, 
                                    uint8_t port_addr, uint8_t *link)
{
    int rc = 0;
    sel_sfi_xfi_device(fd, mdio_addr, port_addr, BCM8754_XFI_SELECT);
    rc = pma_pmd_link_status_get(fd, mdio_addr, port_addr, link);
    sel_sfi_xfi_device(fd, mdio_addr, port_addr, BCM8754_SFI_SELECT);
    return (rc);
}

int bcmphy_sfi_pmd_link_status_get (int fd, uint8_t mdio_addr, 
                                    uint8_t port_addr, uint8_t *link)
{
    int rc = 0;
    sel_sfi_xfi_device(fd, mdio_addr, port_addr, BCM8754_SFI_SELECT);
    rc = pma_pmd_link_status_get(fd, mdio_addr, port_addr, link);
    return (rc);
}


int bcm8754_sfp_ena (int fd, uint8_t mdio_addr, uint8_t port_addr, uint8_t ena)
{
    int rc = 0;
    return (rc);
}
/*
 * TODO: Need to look at this function
 */

int bcm8754_port_ena (int fd, uint8_t mdio_addr, uint8_t port_addr, 
            uint8_t ena)
{
    int rc = 0;
    uint16_t data;

    BCM8754_RD(fd, mdio_addr, port_addr, BCM8754_PCS_ADDR, 
                    BCM8754_PCS_DEV_ID1, &data); 

    if (data == BCM8754_PCS_DEV_ID1_DFLT) {
        data = BCM8754_UD_PMD_OPTICS_DIGITAL_CTRL_FLD_PCS_TX_INVERT;
        BCM8754_WR(fd, mdio_addr, port_addr, BCM8754_PMA_PMD_ADDR, 
                   BCM8754_UD_PMD_OPTICS_DIGITAL_CTRL, data); 
    }

    rc = bcm8754_sfp_ena(fd, mdio_addr, port_addr, ena);    
    if (rc) {
        printf("Error: SFP Enable for Phy mdio %d port %d\n", mdio_addr, 
               port_addr);
        return (rc);
    }
    return (rc);
}

int bcm8754_eeprom_check (int fd, uint8_t mdio_addr, uint8_t port_addr, 
        int *prom_err)
{
    return (bcm8754_eeprom_load_test (fd, mdio_addr, port_addr, prom_err, 0));
}

int bcm8754_eeprom_test (int fd, uint8_t mdio_addr, uint8_t port_addr, 
        int *prom_err)
{
    return (bcm8754_eeprom_load_test (fd, mdio_addr, port_addr, prom_err, 1));
}
/*
 * return 1 if SFP present, 0 if SFP is not present
 *       access_error if read/write fails
 * CODE from 8727_ucodes_history document
 */
int bcm8754_is_sfp_present (int fd, uint8_t mdio_addr, uint8_t port_addr)
{
    int rc = 0;
    uint16_t data;
    sel_sfi_xfi_device(fd, mdio_addr, port_addr, BCM8754_SFI_SELECT);
    data = 0x1800;
    BCM8754_WR(fd, mdio_addr, port_addr, BCM8754_PMA_PMD_ADDR,
                    BCM8754_UD_PCS_DIGITAL_CTRL, data); 
    data = 0x3EBF;
    BCM8754_WR(fd, mdio_addr, port_addr, BCM8754_PMA_PMD_ADDR,
                    BCM8754_UD_PHY_ID, data); 
    data = 0x0;
    /*
     * Read it twice to make sure mod_abs_flt reads the right value
     */
    BCM8754_RD(fd, mdio_addr, port_addr, BCM8754_PMA_PMD_ADDR,
                    BCM8754_UD_RX_ALARM_STATUS, &data); 
    BCM8754_RD(fd, mdio_addr, port_addr, BCM8754_PMA_PMD_ADDR,
                    BCM8754_UD_RX_ALARM_STATUS, &data); 
    if (data & BCM8754_UD_RX_ALARM_STATUS_FLD_MOD_ABS_FLT) {
        rc = 0x1;
    } else {
        rc = 0x0;
    }
    return (rc);
}

int bcmphy_bist_test (int fd, uint8_t mdio_addr, uint8_t port_addr,
                      uint16_t pkt_size, uint16_t pkt_cnt)
{
    int rc = 0;
    uint16_t data, sfi_data, xfi_data; 
    int timeout;
    rc = bcmphy_reset(fd, mdio_addr, port_addr);
    usleep(200000);
    /*
     * Read Bist Error registers to clear them
     */
    BCM8754_RD(fd, mdio_addr, port_addr, BCM8754_PMA_PMD_ADDR, 
                         BCM8754_UD_XFI_BIST_ERROR, &xfi_data); 

    BCM8754_RD(fd, mdio_addr, port_addr, BCM8754_PMA_PMD_ADDR, 
                         BCM8754_UD_PCS_BIST_ERROR, &sfi_data); 
    // Enable XFI line loopback
    rc = bcmphy_xfi_line_loopback (fd, mdio_addr, port_addr, 1);
    if (rc) {
        printf("Error: xfi line loopback disable\n");
        return (rc);
    }
    bcm8754_edc_mode_set(fd, mdio_addr, port_addr,0x44);

    // Enable PCS loopback
    rc = bcmphy_pcs_loopback (fd, mdio_addr, port_addr, 1);
    if (rc) {
        printf("Error: pcs loopback enable\n");
        return (rc);
    }
    /*
     * send a small packet to wake up link
     */
    // PCS User space is only valid for MMF/SFI
    BCM8754_WR(fd, mdio_addr, port_addr, BCM8754_PMA_PMD_ADDR, 
                    BCM8754_UD_BIST_TX_CNT_CTRL, 0x1); 
    data = BCM8754_UD_BIST_CTRL_FLD_XFI_RX_BIST_ENA;
    BCM8754_WR(fd, mdio_addr, port_addr, BCM8754_PMA_PMD_ADDR,
                        BCM8754_UD_BIST_CTRL, data); 
    data = 0x0;
    BCM8754_WR(fd, mdio_addr, port_addr, BCM8754_PMA_PMD_ADDR,
                        BCM8754_UD_BIST_CTRL, data); 
    BCM8754_WR(fd, mdio_addr, port_addr, BCM8754_PMA_PMD_ADDR, 
                    BCM8754_UD_BIST_TX_CNT_CTRL, 0x0); 
    /* read it twice to get the real value */
    BCM8754_RD(fd, mdio_addr, port_addr, BCM8754_PMA_PMD_ADDR, 
               BCM8754_UD_PMD_OPTICS_DIGITAL_STATUS, &data); 
    BCM8754_RD(fd, mdio_addr, port_addr, BCM8754_PMA_PMD_ADDR, 
               BCM8754_UD_PMD_OPTICS_DIGITAL_STATUS, &data); 
    /* Check for PMD Sync acquired */
    if (!(data & BCM8754_UD_PMD_OPTICS_DIGITAL_STATUS_FLD_R64_SYNC_ACQ)) {
        printf("PMD Sync not acquired on SFI\n");
    }
    // PCS User space is only valid for MMF/SFI
    BCM8754_WR(fd, mdio_addr, port_addr, BCM8754_PMA_PMD_ADDR,
                        BCM8754_UD_BIST_PKT_SIZE, pkt_size); 
    BCM8754_WR(fd, mdio_addr, port_addr, BCM8754_PMA_PMD_ADDR, 
                    BCM8754_UD_BIST_TX_CNT_CTRL, pkt_cnt); 

    // Read Bist ctrl
    BCM8754_RD(fd, mdio_addr, port_addr, BCM8754_PMA_PMD_ADDR,
                        BCM8754_UD_BIST_CTRL, &data); 
    // Enable bist and check receive
    data |= (BCM8754_UD_BIST_CTRL_FLD_XFI_RX_BIST_ENA |
             BCM8754_UD_BIST_CTRL_FLD_PMD_TX_BIST_ENA |
             BCM8754_UD_BIST_CTRL_FLD_PMD_RX_BIST_ENA);

    // Clear the error count
    data &= ~BCM8754_UD_BIST_CTRL_FLD_BIST_RESULT_SEL;

    BCM8754_WR(fd, mdio_addr, port_addr,BCM8754_PMA_PMD_ADDR, 
                        BCM8754_UD_BIST_CTRL, data); 
    timeout = 100;
    while (timeout--) {
             // Check if BIST has started. 
        usleep(10000);
        BCM8754_RD(fd, mdio_addr, port_addr, BCM8754_PMA_PMD_ADDR, 
                         BCM8754_UD_BIST_CTRL, &data); 
        
         if ((data & BCM8754_UD_BIST_CTRL_FLD_PBIST_ON) &&
             (data & BCM8754_UD_BIST_CTRL_FLD_XBIST_ON)) 
              break;
    }
         
    if (!((data & BCM8754_UD_BIST_CTRL_FLD_PBIST_ON) &&
         (data & BCM8754_UD_BIST_CTRL_FLD_XBIST_ON))) {
        printf("Error: BIST has not started 0x%X\n", data);
        return (BCM8754_BIST_ERROR);
    }

    // Wait for 100 milli seconds
    usleep(100000);

    // Check for errors.
    BCM8754_RD(fd, mdio_addr, port_addr, BCM8754_PMA_PMD_ADDR, 
                         BCM8754_UD_XFI_BIST_ERROR, &xfi_data); 

    BCM8754_RD(fd, mdio_addr, port_addr, BCM8754_PMA_PMD_ADDR, 
                         BCM8754_UD_PCS_BIST_ERROR, &sfi_data); 

    if (xfi_data || sfi_data) {
        printf("Error: BIST Failed PCS_ERR(0x%X), XS_ERR(0x%X)\n",
            sfi_data, xfi_data);
        return (BCM8754_BIST_ERROR);
    }

    // Check for the packet count.
    BCM8754_RD(fd, mdio_addr, port_addr, BCM8754_PMA_PMD_ADDR, 
               BCM8754_UD_BIST_CTRL, &data); 
    data |= BCM8754_UD_BIST_CTRL_FLD_BIST_RESULT_SEL;
    BCM8754_WR(fd, mdio_addr, port_addr, BCM8754_PMA_PMD_ADDR, 
               BCM8754_UD_BIST_CTRL, data); 
    usleep(1000);
    BCM8754_RD(fd, mdio_addr, port_addr, BCM8754_PMA_PMD_ADDR, 
                         BCM8754_UD_XFI_BIST_ERROR, &xfi_data); 

    BCM8754_RD(fd, mdio_addr, port_addr, BCM8754_PMA_PMD_ADDR, 
                         BCM8754_UD_PCS_BIST_ERROR, &sfi_data); 

    if ((xfi_data != pkt_cnt) || (sfi_data != pkt_cnt)) {
        printf("Error: BIST Failed PCS_PKT_CNT(0x%X), XS_PKT_CNT(0x%X)\n",
            sfi_data, xfi_data);
        printf("Expected PKT CNT (0x%X)\n", pkt_cnt);
        return (BCM8754_BIST_ERROR);
    }

    // Read again to make sure the count is cleared.
    BCM8754_RD(fd, mdio_addr, port_addr, BCM8754_PMA_PMD_ADDR, 
                         BCM8754_UD_XFI_BIST_ERROR, &xfi_data); 

    BCM8754_RD(fd, mdio_addr, port_addr, BCM8754_PMA_PMD_ADDR, 
                         BCM8754_UD_PCS_BIST_ERROR, &sfi_data); 

    if (xfi_data || sfi_data) {
        printf("Error: BIST Failed PCS_PKT_CNT(0x%X), XS_PKT_CNT(0x%X)\n",
               sfi_data, xfi_data);
        printf("PKT CNT registers should be cleared\n");
        return (BCM8754_BIST_ERROR);
    }
    /* 
     * Stop the BIST
     * zero out pkt len and pkt cnt
     */
    data = 0x00;
    BCM8754_WR(fd, mdio_addr, port_addr, BCM8754_PMA_PMD_ADDR, 
                     BCM8754_UD_BIST_CTRL, data); 
    BCM8754_WR(fd, mdio_addr, port_addr, BCM8754_PMA_PMD_ADDR, 
                     BCM8754_UD_BIST_PKT_SIZE, data); 
    BCM8754_WR(fd, mdio_addr, port_addr, BCM8754_PMA_PMD_ADDR, 
                     BCM8754_UD_BIST_TX_CNT_CTRL, data); 
    /*
     * Remove both XFI and SFI loopbacks
     */
    /* Disable PCS loopback */
    rc = bcmphy_pcs_loopback (fd, mdio_addr, port_addr, 0);
    if (rc) {
        printf("Error: pcs loopback disable\n");
        return (rc);
    }
    /* Disable XFI line loopback */
    rc = bcmphy_xfi_line_loopback (fd, mdio_addr, port_addr, 0);
    if (rc) {
        printf("Error: xfi line loopback disable\n");
        return (rc);
    }
    usleep(100);
    return (rc);
}

#define BCM8754_SFP_TRANSFER_SIZE    0x100
#define BCM8754_2WIRE_DATA_ADDR    0x8007

int bcm8754_sfp_prom_rd (int fd, uint8_t mdio_addr, uint8_t port_addr, 
                         uint8_t page_num, uint8_t *buf, uint32_t bufsize)
{
    int        rc = 0;
    uint16_t    addr_2w = page_num ? 0x51 : 0x50;  
    uint16_t    cnt, data, timeout = 10;    

    /* Enable Two Wire Interface */
    sel_sfi_xfi_device(fd, mdio_addr, port_addr, BCM8754_SFI_SELECT);
    /* Write local BCM8754 Address where data will be stored */ 
    data = BCM8754_2WIRE_DATA_ADDR; 
    BCM8754_WR(fd, mdio_addr, port_addr, BCM8754_PMA_PMD_ADDR, 
                     BCM8754_UD_2WIRE_INTERNAL_ADDR, data); 
    /* Write SFP EEPROM Address start location */ 
    data = 0x0; 
    BCM8754_WR(fd, mdio_addr, port_addr, BCM8754_PMA_PMD_ADDR, 
                     BCM8754_UD_2WIRE_NVM_ADDR, data); 
    /* Write the byte count to be read. */
    data = BCM8754_SFP_TRANSFER_SIZE;
    BCM8754_WR(fd, mdio_addr, port_addr, BCM8754_PMA_PMD_ADDR, 
                     BCM8754_UD_2WIRE_TRANSFER_SZ, data); 
    /* set the SFP EEPROM device address */
    data = ((addr_2w << BCM8754_UD_2WIRE_SLAVE_ID_FLD_DEV_ADD_SHIFT) |
             BCM8754_UD_2WIRE_SLAVE_ID_FLD_IMC_2W_SPEED | 0x0001);
    BCM8754_WR(fd, mdio_addr, port_addr, BCM8754_PMA_PMD_ADDR, 
                     BCM8754_UD_2WIRE_SLAVE_ID, data); 
    /* Initiate the data transfer */
    data = 0x8002; /* bit 1 = 1; bit 0 =0; */ 
    BCM8754_WR(fd, mdio_addr, port_addr, BCM8754_PMA_PMD_ADDR, 
                     BCM8754_UD_2WIRE_CTRL, data); 

    // Check for complete
    usleep(10000);
    while (timeout--) {
        // sleep for 10 ms
        BCM8754_RD(fd, mdio_addr, port_addr, BCM8754_PMA_PMD_ADDR,
                         BCM8754_UD_2WIRE_CTRL, &data); 
        if (rc) {
            printf("Error: Rd UD_2WIRE_CTRL  PHY%d\n", port_addr);
            return (rc);
        }
        data = (data & BCM8754_UD_2WIRE_CTRL_FLD_2W_STATUS) >> 2;
        if (data == 1) { /* command completed succesfully */
            break;
        } else if (data == 3) {
            printf("SFP I2C Accesses failed\n");
            /* return error here */
            rc = BCM8754_SFP_ACCESS_ERROR;
            break;
        } else if (data == 2) {
          usleep(1000);
        }
    }
    
    if (rc) {
        printf("Error: Phy %d-%d SFP EEPROM access\n", mdio_addr, port_addr);
        return (rc);
    }
    if (!timeout) {
        printf("Error: Phy %d -%d SFP EEPROM Rd timeout\n", 
               mdio_addr,port_addr);
        return (BCM8754_SFP_TIMEOUT_ERROR);
    }    

    /* 
     * data comes into memory at 0x8007 address in LSB [0-7]
     */
    for (cnt = 0; cnt < bufsize; cnt++, buf++) {
        BCM8754_RD(fd, mdio_addr, port_addr, BCM8754_PMA_PMD_ADDR,
                         (BCM8754_2WIRE_DATA_ADDR+cnt), &data); 
        if (rc) {
            printf("Error: Rd 0x8007  PHY %d %d\n", mdio_addr, port_addr);
            return (rc);
        } 
        *buf = (uint8_t) data;
    }
    return (rc);
}

/* TODO: Check this function */ 
int bcm8754_sfp_mode_config(int fd, uint8_t mdio_addr, uint8_t port_addr,
    uint8_t xcvr_code)
{
    int    rc = 0;

    if (xcvr_code & 0x40) {
        printf(" SFP(addr=%d) LRM mode init\n", port_addr);
        BCM8754_WR (fd, mdio_addr, port_addr, BCM8754_PMA_PMD_ADDR, 0xCA3F, 
                    0x0000);
        BCM8754_WR (fd, mdio_addr, port_addr, BCM8754_PMA_PMD_ADDR, 0xCA1A,
                    0x0124);
        BCM8754_WR (fd, mdio_addr, port_addr, BCM8754_PMA_PMD_ADDR, 0xCA23, 
                    0x4008);
        BCM8754_WR (fd, mdio_addr, port_addr, BCM8754_PMA_PMD_ADDR, 0xCA3F, 
                    0xAAAA);

    } else if (xcvr_code & 0x20) {
        printf(" SFP(addr=%d) LR mode init\n", port_addr);
        BCM8754_WR(fd, mdio_addr, port_addr, BCM8754_PMA_PMD_ADDR, 0xCA1A, 
                   0x0044);
    } else if (xcvr_code & 0x10) {
        printf(" SFP(addr=%d) SR mode init\n", port_addr);
        BCM8754_WR (fd, mdio_addr, port_addr, BCM8754_PMA_PMD_ADDR, 0xCA1A,
                    0x0044);
    } else  {
        printf(" SFP(addr=%d) default mode (no init)\n", port_addr);
    }
        
    return (rc);
}

static int check_latest_ucode (int fd, uint8_t mdio_addr, uint8_t port_addr,
                               uint16_t *data)
{
    int rc=0;
    // Select MMF/SFI Device 1
    sel_sfi_xfi_device(fd, mdio_addr, port_addr, BCM8754_SFI_SELECT);
    // Initiate SPI ROM download
    BCM8754_RD(fd, mdio_addr, port_addr, BCM8754_PMA_PMD_ADDR, 
                        BCM8754_EDC_GP_REG_2, data); 
    if (*data != BCM8754_UCODE_LATEST_VER) {
        rc = 1;
    }
    return (rc);
}
/*
 * EDC-code download. via mdio pin 
 */
int bcm8754_eeprom_init  (int fd, uint8_t mdio_addr, uint8_t port_addr)
{
    int rc = 0, prom_err = 0, index=0;
    uint16_t data = 0;
    rc = check_latest_ucode(fd, mdio_addr, port_addr, &data);
    if (!rc) {
        printf("  PHY u-code(mdio_addr = 0x%X port_addr = 0x%X) "
               "is upto date %x\n", mdio_addr, port_addr, data);
        return (0);
    }
    /* 
     * EDC mode changes alter CA1A (after fextsys is run)
     * Apply soft reset to channel of interest to load ucode
     */ 
    bcmphy_reset(fd, mdio_addr, port_addr);
    usleep(200*1000);

    rc = check_latest_ucode(fd, mdio_addr, port_addr, &data);
    if (!rc) {
        printf("  PHY u-code(mdio_addr = 0x%X port_addr = 0x%X) "
               "is upto date %x\n", mdio_addr, port_addr, data);
        return (0);
    }
    // Select MMF/SFI Device 1
    sel_sfi_xfi_device(fd, mdio_addr, port_addr, BCM8754_SFI_SELECT);
    // Initiate SPI ROM download
    BCM8754_RD(fd, mdio_addr, port_addr, BCM8754_PMA_PMD_ADDR, 
                        BCM8754_UD_SPI_PORT_CTRL, &data); 
    data &= ~(0xE000); /* added to get microcode download work */
    BCM8754_WR(fd, mdio_addr, port_addr, BCM8754_PMA_PMD_ADDR,
                        BCM8754_UD_SPI_PORT_CTRL, data);
    data |= BCM8754_UD_SPI_PORT_CTRL_FLD_SPI_BOOT;
    BCM8754_WR(fd, mdio_addr, port_addr, BCM8754_PMA_PMD_ADDR,
                        BCM8754_UD_SPI_PORT_CTRL, data);
    // Apply soft reset to channel of interest
    pma_pmd_reset(fd, mdio_addr, port_addr);
    usleep(500);
    // Write start address of where microcode is to be stored to MSG_IN
    data = 0xC000;
    BCM8754_WR(fd, mdio_addr, port_addr, BCM8754_PMA_PMD_ADDR,
                        BCM8754_MSG_IN, data);
    // Wait 14 microseconds for operation to complete
    usleep(30 * 1);
    // Write total number of bytes to be downloaded divide by 2.
    // For 16 KB ROM
    data = 0x2000;
    BCM8754_WR(fd, mdio_addr, port_addr, BCM8754_PMA_PMD_ADDR,
                        BCM8754_MSG_IN, data);
    // Wait 14 microseconds for operation to complete
    usleep(20 * 1);
    // Write all 16 KB of data through the MDIO. 
    // wait 14 us between each write
    for (index = 0; index < 0x2000; index++) {
         //     printf("Wr %x %x %x\n", mdio_addr, port_addr, prog_data_8754[index]);
         BCM8754_WR(fd, mdio_addr, port_addr, BCM8754_PMA_PMD_ADDR,
                BCM8754_MSG_IN, prog_data_8754[index]);
         usleep(20 * 1);
    }
    // Microcode can also be downloaded by hardware serially through SPI
    // (register 0xCA85)
    usleep(20 * 1);

        // Read MSG_OUT which will provide done status  
    rc = bcm8754_eeprom_test (fd, mdio_addr, port_addr, &prom_err);
    if (rc) {
        printf("Error: EEPROM Load failed mdio %d port %d\n", 
               mdio_addr, port_addr);
        return (rc);
    }
        // TODO: Read RX_ALARM_STATUS register (0x9003) 
        // TODO: User can also read SPI_PORT_CTRL_STATUS, bit13 should be 1.

    if (prom_err) {
        printf("  PHY EEPROM Load (mdio_addr = 0x%X port_addr = 0x%X) "
               "Failed\n", mdio_addr, port_addr);  
        return (-1);
    } else {
        printf("  PHY EEPROM Load (mdio_addr = 0x%X port_addr = 0x%X) "
               "Successful\n", mdio_addr, port_addr);
    }
    return (rc);
}

static int bcm8754_gpio_led_init (int fd, uint8_t mdio_addr, uint8_t port_addr)
{
    int rc = 0;
    uint16_t data;
    // Enable GPIO Mode
    BCM8754_RD(fd, mdio_addr, port_addr, BCM8754_PMA_PMD_ADDR,
            BCM8754_UD_PCS_DIGITAL_CTRL, &data);
    data &= ~BCM8754_UD_PCS_DIGITAL_CTRL_FLD_GPIO_SEL;
    data |= BCM8754_UD_PCS_DIGITAL_CTRL_LED_LINK_UP;
    BCM8754_WR(fd, mdio_addr, port_addr, BCM8754_PMA_PMD_ADDR,
            BCM8754_UD_PCS_DIGITAL_CTRL, data);
    return (rc);
}

int bcm8754_init (int fd, uint8_t mdio_addr, uint8_t port_addr)
{
    int rc = 0;
    rc = bcm8754_xfi_init(fd, mdio_addr, port_addr);
    if (rc) {
        return (rc);
    }
    rc = bcm8754_sfi_init(fd, mdio_addr, port_addr);
    if (rc) {
        return (rc);
    }
    rc = bcm8754_gpio_led_init(fd, mdio_addr, port_addr);
    return (rc);

}

int bcm8754_gpio_init (int fd, uint8_t mdio_addr, 
                       uint8_t port_addr, uint8_t output)
{
    int rc = 0;
    uint16_t data;
        
    // Enable GPIO Mode
    BCM8754_RD(fd, mdio_addr, port_addr, BCM8754_PMA_PMD_ADDR,
            BCM8754_UD_PCS_DIGITAL_CTRL, &data);
    data &= ~BCM8754_UD_PCS_DIGITAL_CTRL_FLD_GPIO_SEL;
    BCM8754_WR(fd, mdio_addr, port_addr, BCM8754_PMA_PMD_ADDR,
            BCM8754_UD_PCS_DIGITAL_CTRL, data);

    // Set output or input..,
    BCM8754_RD(fd, mdio_addr, port_addr, BCM8754_PMA_PMD_ADDR,
            BCM8754_UD_GPIO_CTRL, &data);
    if (output) {
        data &= ~BCM8754_UD_GPIO_CTRL_FLD_OUT_GP_DS;
    } else {
        data |= BCM8754_UD_GPIO_CTRL_FLD_OUT_GP_DS;
    }
    BCM8754_WR(fd, mdio_addr, port_addr, BCM8754_PMA_PMD_ADDR,
            BCM8754_UD_GPIO_CTRL, data);

    return (rc);
}


// gpio_pin take values 0 or 1.
int bcm8754_gpio_set  (int fd, uint8_t mdio_addr, uint8_t port_addr, uint8_t gpio_pin)
{
    int rc = 0;
    uint16_t data;
        
    BCM8754_RD(fd, mdio_addr, port_addr, BCM8754_PMA_PMD_ADDR,
                    BCM8754_UD_GPIO_CTRL, &data);
    data |= (0x01 << gpio_pin);
    BCM8754_WR(fd, mdio_addr, port_addr, BCM8754_PMA_PMD_ADDR,
                    BCM8754_UD_GPIO_CTRL, data);
    return (rc);
}

// gpio_pin take values 0 or 1.
int bcm8754_gpio_clr  (int fd, uint8_t mdio_addr, uint8_t port_addr, uint8_t gpio_pin)
{
    int rc = 0;
    uint16_t data;
        
    BCM8754_RD(fd, mdio_addr, port_addr, BCM8754_PMA_PMD_ADDR,
                    BCM8754_UD_GPIO_CTRL, &data);
    data &= ~(0x01 << gpio_pin);
    BCM8754_WR(fd, mdio_addr, port_addr, BCM8754_PMA_PMD_ADDR,
                    BCM8754_UD_GPIO_CTRL, data);
    return (rc);
}

// gpio_pin take values 0 or 1.
int bcm8754_gpio_get (int fd, uint8_t mdio_addr, uint8_t port_addr, uint8_t gpio_pin, 
            uint8_t *gpio_val)
{
    int rc = 0;
    uint16_t data;
        
    BCM8754_RD(fd, mdio_addr, port_addr, BCM8754_PMA_PMD_ADDR,
                    BCM8754_UD_GPIO_CTRL, &data);
    *gpio_val = (data & (0x01 << (gpio_pin+8))) ? 1 : 0;
    return (rc);
}

int bcm8754_spirom_program (int fd, uint8_t mdio_addr, uint8_t port_addr,
                            char *fp)
{
    int rc = 0;
    // Select MMF/SFI Device 1
    sel_sfi_xfi_device(fd, mdio_addr, port_addr, BCM8754_SFI_SELECT);
    rc = phy_spirom_program(fd, mdio_addr, port_addr, fp);
    return(rc);
} 

int bcm8754_loopback_isolate (int fd, uint8_t mdio_addr, uint8_t port_addr, uint8_t phy_lpbk) 
{ 
    int rc = 0;
    uint16_t data;

    sel_sfi_xfi_device(fd, mdio_addr, port_addr, BCM8754_SFI_SELECT);

    BCM8754_RD(fd, mdio_addr, port_addr, BCM8754_PMA_PMD_ADDR,
                    0xC800, &data);

    if (phy_lpbk) {
        data &= ~(0x01 << 9);
    } else {
        data |= (0x01 << 9);
    }

    BCM8754_WR(fd, mdio_addr, port_addr, BCM8754_PMA_PMD_ADDR,
                    0xC800, data);

    return (rc);
}

int bcm8754_edc_mode_set (int fd, uint8_t mdio_addr, uint8_t port_addr, uint8_t edc_mode)
{
    uint16_t data;

    sel_sfi_xfi_device(fd, mdio_addr, port_addr, BCM8754_SFI_SELECT);

    BCM8754_RD(fd, mdio_addr, port_addr, BCM8754_PMA_PMD_ADDR,
                    0xC800, &data);
    // Induce LOS Condition
    data &= ~(0x01 << 9);
    BCM8754_WR(fd, mdio_addr, port_addr, BCM8754_PMA_PMD_ADDR,
                    0xC800, data);
    // Set EDC Mode.
    BCM8754_WR(fd, mdio_addr, port_addr, BCM8754_PMA_PMD_ADDR,
                    0xCA1A, (0x0400 | edc_mode));
    // Remove LOS condition. 
    data |= (0x01 << 9);
    BCM8754_WR(fd, mdio_addr, port_addr, BCM8754_PMA_PMD_ADDR,
                    0xC800, data);

    return(0);
}
/*
 * Enable User PRBS
 * PRBS7, 9, 11, 15, 23 are supported
 */
static int bcm8754_user_prbs (int fd, uint8_t mdio_addr, uint8_t port_addr,
                       uint8_t prbs_sel, int ena)
{
    uint16_t data;
    BCM8754_RD(fd, mdio_addr, port_addr, BCM8754_PMA_PMD_ADDR,
               0xCD14, &data);
    /* select PRBS type and Enable PRBS */
    data &= ~(BCM8754_UD_PMD_PRBS_CTRL_0_FLD_PRBS_SEL);
    data |= (prbs_sel);
    if (ena) {
        data |= BCM8754_UD_PMD_PRBS_CTRL_0_FLD_USER_PRBS_EN;
    } else {
        /* Write default value */
        data = 0x8018;
    }
    DIAG_PRINT(DIAG_PRINT_CONFIG, "prbs %x %x %d\n", prbs_sel, data, ena);
    BCM8754_WR(fd, mdio_addr, port_addr, BCM8754_PMA_PMD_ADDR,
               0xCD14, data);
    return (0);
}
static int bcm8754_user_prbs_check (int fd, uint8_t mdio_addr, 
                                    uint8_t port_addr)
{
    uint16_t data;
    BCM8754_RD(fd, mdio_addr, port_addr, BCM8754_PMA_PMD_ADDR,
               0xCD15, &data);

    if (data & BCM8754_UD_PMD_PRBS_STATUS_0_FLD_PRBS_LOCK) {
        printf("RX PRBS is locked\n");
    } else {
        printf("RX PRBS is not locked\n");
    }

    printf("PRBS RX Error Count %d\n", (data&0x7fff));
    return (0);
}
/*
 * PRBS on SFI side
 */
int bcm8754_sfi_user_prbs (int fd, uint8_t mdio_addr, uint8_t port_addr,
                           uint8_t prbs_sel, int mode)
{
    int rc = 0;
    sel_sfi_xfi_device(fd, mdio_addr, port_addr, BCM8754_SFI_SELECT);
    if ((mode == BCM8754_PRBS_ENA) || (mode == BCM8754_PRBS_DIS)) {
        rc = bcm8754_user_prbs(fd, mdio_addr, port_addr, prbs_sel, mode);
    } else if (mode == BCM8754_PRBS_CHECK) {
        rc = bcm8754_user_prbs_check(fd, mdio_addr, port_addr);
    }
    return (rc);
}
/*
 * PRBS on XFI side
 */
int bcm8754_xfi_user_prbs (int fd, uint8_t mdio_addr, uint8_t port_addr,
                           uint8_t prbs_sel, int mode)
{
    int rc = 0;
    sel_sfi_xfi_device(fd, mdio_addr, port_addr, BCM8754_XFI_SELECT);
    if ((mode == BCM8754_PRBS_ENA) || (mode == BCM8754_PRBS_DIS)) {
        rc = bcm8754_user_prbs(fd, mdio_addr, port_addr, prbs_sel, mode);
    } else if (mode == BCM8754_PRBS_CHECK) {
        rc = bcm8754_user_prbs_check(fd, mdio_addr, port_addr);
    }
    sel_sfi_xfi_device(fd, mdio_addr, port_addr, BCM8754_SFI_SELECT);
    return (rc);
}

int bcm8754_pol_inv (int fd, uint8_t mdio_addr, uint8_t port_addr, 
            uint8_t tx_inv_pol, uint8_t rx_inv_pol)
{
    int rc = 0;
    uint16_t data;


    BCM8754_RD(fd, mdio_addr, port_addr, BCM8754_PMA_PMD_ADDR, 
                   BCM8754_UD_PMD_OPTICS_DIGITAL_CTRL, &data); 
  
    if (tx_inv_pol) 
    	data |= BCM8754_UD_PMD_OPTICS_DIGITAL_CTRL_FLD_PCS_TX_INVERT;
    else
    	data &= ~BCM8754_UD_PMD_OPTICS_DIGITAL_CTRL_FLD_PCS_TX_INVERT;

    if (rx_inv_pol) 
    	data |= BCM8754_UD_PMD_OPTICS_DIGITAL_CTRL_FLD_PCS_RX_INVERT;
    else
    	data &= ~BCM8754_UD_PMD_OPTICS_DIGITAL_CTRL_FLD_PCS_RX_INVERT;
    BCM8754_WR(fd, mdio_addr, port_addr, BCM8754_PMA_PMD_ADDR, 
                   BCM8754_UD_PMD_OPTICS_DIGITAL_CTRL, data); 

    DIAG_PRINT(DIAG_PRINT_CONFIG, 
               "  PHY (Mdio=0x%X, Port=0x%X) Tx_INV=%d Rx_INV=%d\n",
	       mdio_addr, port_addr, tx_inv_pol, rx_inv_pol);
    return (rc);
}

int bcm8754_1g_init (int fd, uint8_t mdio_addr, uint8_t port_addr) 
{ 
    int rc = 0;

    sel_sfi_xfi_device(fd, mdio_addr, port_addr, BCM8754_SFI_SELECT);

    BCM8754_WR(fd, mdio_addr, port_addr, BCM8754_PMA_PMD_ADDR,
                    BCM8754_PMD_CTRL, 0x40);

    BCM8754_WR(fd, mdio_addr, port_addr, BCM8754_PMA_PMD_ADDR,
                    BCM8754_PMD_CTRL2, 0x0D);

    return (rc);
}

