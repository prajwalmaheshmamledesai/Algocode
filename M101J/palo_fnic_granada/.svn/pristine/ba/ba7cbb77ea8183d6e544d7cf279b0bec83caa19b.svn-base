#ifndef _BCM8726_LIB_H_
#define _BCM8726_LIB_H_

extern int bcm8726_rd (int fd, uint8_t mdio_addr, uint8_t port_addr,
	    uint8_t device_class, uint16_t reg_addr, uint16_t *data);
extern int bcm8726_wr (int fd, uint8_t mdio_addr, uint8_t port_addr,
	    uint8_t device_class, uint16_t reg_addr, uint16_t data);

extern int bcmphy_reset (int fd, uint8_t mdio_addr, uint8_t port_addr);
extern int bcm8726_reset_toggle (int fd, uint8_t mdio_addr, uint8_t port_addr,
                uint8_t device_class, uint8_t addr, uint8_t ena);

extern int bcm8726_sfp_ena (int fd, uint8_t mdio_addr, uint8_t port_addr, uint8_t ena);
extern int bcm8726_port_ena (int fd, uint8_t mdio_addr, uint8_t port_addr, 
                uint8_t ena);

extern int bcm8726_eeprom_check (int fd, uint8_t mdio_addr, uint8_t port_addr,
                int *prom_err);
extern int bcm8726_eeprom_test (int fd, uint8_t mdio_addr, uint8_t port_addr,
                int *prom_err);

extern int bcmphy_bist_test (int fd, uint8_t mdio_addr, uint8_t port_addr,
                uint16_t pkt_size, uint16_t pkt_cnt);


extern int bcm8726_sfp_prom_rd (int fd, uint8_t mdio_addr, uint8_t port_addr, uint8_t page_num,
                        uint8_t *buf, uint32_t bufsize);

extern int bcm8726_sfp_test (int fd, uint8_t mdio_addr, uint8_t port_addr);
extern int bcm8726_eeprom_init  (int fd, uint8_t mdio_addr, uint8_t port_addr);
extern int bcmphy_xgxs_link_status (int fd, uint8_t mdio_addr, 
				uint8_t port_addr, uint8_t *link);

extern int bcmphy_pcs_loopback 	(int fd, uint8_t madr, uint8_t padr, uint8_t ena);
extern int bcmphy_pma_pmd_loopback 	(int fd, uint8_t madr, uint8_t padr, uint8_t ena);
extern int bcmphy_digital_lane_looppback(int fd, uint8_t madr, uint8_t padr, uint8_t ena);
extern int bcmphy_xaui_system_loopback	(int fd, uint8_t madr, uint8_t padr, uint8_t ena);

extern int bcmphy_xgxs_loopback 	(int fd, uint8_t madr, uint8_t padr, uint8_t ena);
extern int bcmphy_xaui_loopback 	(int fd, uint8_t madr, uint8_t padr, uint8_t ena);
extern int bcmphy_lpbk_print 		(int fd, uint8_t madr, uint8_t padr, uint8_t flag);
extern int bcmphy_link_status_print	(int fd, uint8_t mdio_addr, uint8_t port_addr, uint8_t hdr);
extern int bcm8726_init  (int fd, uint8_t mdio_addr,
                        uint8_t port_addr, uint8_t lane_swap);

extern int phy_pcs_loopback(int port, int lpbk_ena);
extern int bcmphy_pcs_link_status_get (int fd, uint8_t mdio_addr, 
			uint8_t port_addr, uint8_t *link);

extern int bcm8726_gpio_clr  (int fd, uint8_t mdio_addr, 
				uint8_t port_addr, uint8_t gpio_pin);
extern int bcm8726_gpio_set  (int fd, uint8_t mdio_addr, 
				uint8_t port_addr, uint8_t gpio_pin);
extern int bcm8726_gpio_get  (int fd, uint8_t mdio_addr, uint8_t port_addr, 
		uint8_t gpio_pin, uint8_t *gpio_val);
extern int bcm8726_gpio_init (int fd, uint8_t mdio_addr,
                        uint8_t port_addr, uint8_t output);

extern int bcm8726_sfp_cc_test(uint8_t instmask, uint8_t pagemask, 
			uint8_t full_flag, uint8_t *sfp_err_mask);

extern int bcmphy_xgxs_get_lane_status (int fd, uint8_t mdio_addr,
                uint8_t port_addr, uint8_t *lane_status);
extern int bcmphy_premep_set (int fd, uint8_t mdio_addr, uint8_t port_addr,
                        uint8_t preemp);
extern int bcm8726_sfp_mode_config(int fd, uint8_t mdio_addr, uint8_t dev_addr,
	uint8_t xcvr_code);

// BCM8727
extern int bcm8727_eeprom_test (int fd, uint8_t mdio_addr, uint8_t port_addr, int *prom_err);
extern int bcm8727_eeprom_init (int fd, uint8_t mdio_addr, uint8_t port_addr);
extern int bcmphy_version (int fd, uint8_t mdio_addr, uint8_t port_addr, uint16_t *rev);

#endif
