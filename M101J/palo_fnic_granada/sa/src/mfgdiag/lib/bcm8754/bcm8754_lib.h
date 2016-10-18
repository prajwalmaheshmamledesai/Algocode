/********************************************************************
 * File:   bcm8754_lib.h
 * Name:   Raghu Boreda
 *
 * Description:
 * Copyright (c) 2009 by Cisco Systems, Inc.
 * All rights reserved.
 *********************************************************************/
#ifndef _BCM8754_LIB_H_
#define _BCM8754_LIB_H_

#define BCM8754_SFP_ACCESS_ERROR  1
#define BCM8754_SFP_TIMEOUT_ERROR  2
#define BCM8754_BIST_ERROR  3

#define BCM8754_PRBS_DIS  0
#define BCM8754_PRBS_ENA  1
#define BCM8754_PRBS_CHECK  2

extern int bcm8754_rd (int fd, uint8_t mdio_addr, uint8_t port_addr,
	    uint8_t device_class, uint16_t reg_addr, uint16_t *data);
extern int bcm8754_wr (int fd, uint8_t mdio_addr, uint8_t port_addr,
	    uint8_t device_class, uint16_t reg_addr, uint16_t data);

extern int bcmphy_reset (int fd, uint8_t mdio_addr, uint8_t port_addr);
extern int bcmphy_pcs_loopback (int fd, uint8_t madr, uint8_t padr, 
                                uint8_t ena);
extern int bcmphy_xfi_loopback (int fd, uint8_t madr, uint8_t padr, 
                                uint8_t ena);
extern int bcmphy_sfi_line_loopback (int fd, uint8_t madr, uint8_t padr, 
                                     uint8_t ena);
extern int bcmphy_xfi_line_loopback (int fd, uint8_t madr, uint8_t padr, 
                                     uint8_t ena);

extern int bcm8754_xfi_loopback_get (int fd, uint8_t maddr, uint8_t paddr,
           uint8_t d_class, uint16_t addr, uint16_t lpbk_bit, uint8_t *lpbk);
extern int bcm8754_sfi_loopback_get (int fd, uint8_t maddr, uint8_t paddr,
           uint8_t d_class, uint16_t addr, uint16_t lpbk_bit, uint8_t *lpbk);

extern int bcmphy_xfi_pcs_link_status_get (int fd, uint8_t mdio_addr, 
				uint8_t port_addr, uint8_t *link);
extern int bcmphy_sfi_pcs_link_status_get (int fd, uint8_t mdio_addr, 
				uint8_t port_addr, uint8_t *link);
extern int bcmphy_xfi_pmd_link_status_get (int fd, uint8_t mdio_addr, 
				uint8_t port_addr, uint8_t *link);
extern int bcmphy_sfi_pmd_link_status_get (int fd, uint8_t mdio_addr, 
				uint8_t port_addr, uint8_t *link);

extern int bcm8754_sfp_ena (int fd, uint8_t mdio_addr, uint8_t port_addr, 
                            uint8_t ena);
extern int bcm8754_port_ena (int fd, uint8_t mdio_addr, uint8_t port_addr, 
                             uint8_t ena);

extern int bcm8754_eeprom_check (int fd, uint8_t mdio_addr, uint8_t port_addr,
                                 int *prom_err);
extern int bcm8754_eeprom_test (int fd, uint8_t mdio_addr, uint8_t port_addr,
                                int *prom_err);

extern int bcmphy_bist_test (int fd, uint8_t mdio_addr, uint8_t port_addr,
                             uint16_t pkt_size, uint16_t pkt_cnt);

extern int bcm8754_sfp_prom_rd (int fd, uint8_t mdio_addr, uint8_t port_addr, 
                  uint8_t page_num, uint8_t *buf, uint32_t bufsize);

extern int bcm8754_sfp_mode_config(int fd, uint8_t mdio_addr, uint8_t dev_addr,
	                           uint8_t xcvr_code);
extern int bcm8754_eeprom_init (int fd, uint8_t mdio_addr, uint8_t port_addr);
extern int bcm8754_init(int fd, uint8_t mdio_addr, uint8_t port_addr);
extern int bcm8754_sfi_init(int fd, uint8_t mdio_addr, uint8_t port_addr);
extern int bcm8754_xfi_init(int fd, uint8_t mdio_addr, uint8_t port_addr);

extern int bcm8754_gpio_init (int, uint8_t, uint8_t, uint8_t);
extern int bcm8754_gpio_set (int, uint8_t, uint8_t, uint8_t);
extern int bcm8754_gpio_clr (int, uint8_t, uint8_t, uint8_t);
extern int bcm8754_gpio_get (int, uint8_t, uint8_t, uint8_t, uint8_t *);

extern int bcmphy_lpbk_print (int fd, uint8_t madr, uint8_t padr, uint8_t hdr);
extern int bcmphy_link_status_print (int fd, uint8_t mdio_addr, 
                                     uint8_t port_addr, uint8_t hdr);


extern int bcm8754_sfp_cc_test(uint64_t instmask, uint8_t pagemask, 
			uint32_t full_flag, uint64_t *sfp_err_mask);
extern int bcm8754_is_sfp_present(int, uint8_t, uint8_t);
extern int bcm8754_spirom_program(int, uint8_t, uint8_t, char *);
extern int bcm8754_spirom_program_all(int, char *);
extern int bcm8754_ucode_load_all(int);
extern int bcm8754_mdio_download_code(int);

extern int bcm8754_loopback_isolate (int fd, uint8_t mdio, uint8_t port, uint8_t ena);
extern int bcm8754_edc_mode_set (int fd, uint8_t mdio_addr, uint8_t port_addr, 
				 uint8_t edc_mode);

extern int sel_sfi_xfi_device (int fd, uint8_t mdio_addr, uint8_t port_addr, 
                               int xfi_sel);
extern int pma_pmd_reset (int fd, uint8_t mdio_addr, uint8_t port_addr);
extern int bcm8754_sfi_user_prbs (int, uint8_t, uint8_t, uint8_t, int);
extern int bcm8754_xfi_user_prbs (int, uint8_t, uint8_t, uint8_t, int);
extern int bcm8754_pol_inv (int fd, uint8_t mdio_addr, uint8_t port_addr, 
            uint8_t tx_inv_pol, uint8_t rx_inv_pol);
extern int bcm8754_1g_init (int fd, uint8_t mdio_addr, uint8_t port_addr);

#define BCM8754_RD(fd, mdio, port, dev, reg, rdval)  \
{ int retval = bcm8754_rd(fd, mdio, port, dev, reg, rdval); \
  if (retval) { \
      printf("MDIO Read Error mdio %d port %d reg %d\n", mdio, port, reg); \
      return (retval); \
  } \
}

#define BCM8754_WR(fd, mdio, port, dev, reg, wrval)  \
{ int rv = bcm8754_wr(fd, mdio, port, dev, reg, wrval); \
  if (rv) { \
      printf("MDIO Read Error mdio %d port %d reg %d\n", mdio, port, reg); \
      return (rv); \
  } \
}

#endif
