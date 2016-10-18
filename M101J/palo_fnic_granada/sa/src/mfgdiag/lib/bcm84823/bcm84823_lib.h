/********************************************************************
 * File:   bcm84823_lib.h
 * Name:   Raghu Boreda
 *
 * Description:
 * Copyright (c) 2009 by Cisco Systems, Inc.
 * All rights reserved.
 *********************************************************************/
#ifndef _BCM84823_LIB_H_
#define _BCM84823_LIB_H_

#define BCM84823_BIST_ERROR  3

#define BCM84823_PRBS_DIS  0
#define BCM84823_PRBS_ENA  1
#define BCM84823_PRBS_CHECK  2

enum _BCM84823_PRBS_SEL_ {
  BCM84823_SEL_PRBS7 = 1,
  BCM84823_SEL_PRBS9 = 2,
  BCM84823_SEL_PRBS11 = 3,
  BCM84823_SEL_PRBS15 = 4,
  BCM84823_SEL_PRBS23 = 5,
  BCM84823_SEL_PRBS31 = 6,
};

extern int bcm84823_rd (int fd, uint8_t mdio_addr, uint8_t port_addr,
	    uint8_t device_class, uint16_t reg_addr, uint16_t *data);
extern int bcm84823_wr (int fd, uint8_t mdio_addr, uint8_t port_addr,
	    uint8_t device_class, uint16_t reg_addr, uint16_t data);

extern int bcm84823_reset (int fd, uint8_t mdio_addr, uint8_t port_addr);
extern int bcm84823_pcs_loopback (int fd, uint8_t madr, uint8_t padr, 
                                uint8_t ena);
extern int bcm84823_xfi_loopback (int fd, uint8_t madr, uint8_t padr, 
                                uint8_t ena);
extern int bcm84823_xfi_line_loopback (int fd, uint8_t madr, uint8_t padr, 
                                     uint8_t ena);

extern int bcm84823_xfi_pcs_link_status_get (int fd, uint8_t mdio_addr, 
				uint8_t port_addr, uint8_t *link);
extern int bcm84823_10g_pcs_link_status_get (int fd, uint8_t mdio_addr, 
				uint8_t port_addr, uint8_t *link);
extern int bcm84823_10g_pmd_link_status_get (int fd, uint8_t mdio_addr, 
				uint8_t port_addr, uint8_t *link);

extern int bcm84823_port_ena (int fd, uint8_t mdio_addr, uint8_t port_addr, 
                             uint8_t ena);

extern int bcm84823_xfi_bist_test_start(int , uint8_t , uint8_t );
extern int bcm84823_xfi_bist_test_verify(int , uint8_t , uint8_t );


extern int bcm84823_eeprom_init (int fd, uint8_t mdio_addr, uint8_t port_addr);
extern int bcm84823_init(int fd, uint8_t mdio_addr, uint8_t port_addr);

extern int bcm84823_led_set (int, uint8_t, uint8_t, uint8_t);
extern int bcm84823_led_clr (int, uint8_t, uint8_t, uint8_t);

extern int bcm84823_lpbk_print (int fd, uint8_t madr, uint8_t padr, 
                                uint8_t hdr, int phy_port);
extern int bcm84823_link_status_print (int fd, uint8_t mdio_addr, 
                                uint8_t port_addr, uint8_t hdr, int phy_port);

extern int bcm84823_download_microcode(int, char *);
extern int bcm84823_gen_download_microcode(int, char *, uint8_t, uint8_t *);
extern int bcm84823_download_microcode_single_phy(int, char *, uint8_t, uint8_t);

extern int bcm84823_loopback_isolate (int fd, uint8_t mdio, uint8_t port, uint8_t ena);

extern int bcm84823_xfi_user_prbs (int, uint8_t, uint8_t, uint8_t, int);
extern int bcm84823_print_tempsensor (int, uint8_t, uint8_t, int);
extern int bcm84823_display_errors (int, uint8_t, uint8_t, int);
extern int bcm84823_display_counters (int, uint8_t, uint8_t, int);
extern int bcm84823_clear_errors (int, uint8_t, uint8_t);
extern int bcm84823_poll_errs (int, uint8_t, uint8_t, int);
extern int bcm84823_cfg_speed(int, uint8_t, uint8_t, uint32_t);
extern int bcm84823_write_seq(int, uint8_t, uint8_t, uint16_t *, uint32_t);

#define BCM84823_RD(fd, mdio, port, dev, reg, rdval)  \
{ int retval = bcm84823_rd(fd, mdio, port, dev, reg, rdval); \
  if (retval) { \
      printf("MDIO Read Error mdio %d port %d reg %d\n", mdio, port, reg); \
      return (retval); \
  } \
}

#define BCM84823_WR(fd, mdio, port, dev, reg, wrval)  \
{ int rv = bcm84823_wr(fd, mdio, port, dev, reg, wrval); \
  if (rv) { \
      printf("MDIO Read Error mdio %d port %d reg %d\n", mdio, port, reg); \
      return (rv); \
  } \
}
#endif
