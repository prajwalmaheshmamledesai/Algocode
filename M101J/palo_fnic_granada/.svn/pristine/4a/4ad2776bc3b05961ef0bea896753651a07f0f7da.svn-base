#ifndef _BCM54X80_API_H_
#define _BCM54X80_API_H_
extern int  bcm54980_soft_reset (int fd, uint8_t inst, uint8_t port_addr);
extern int  bcm54980_pcs_loopback (int fd, uint8_t inst, uint8_t port_addr, uint8_t ena);
extern int  bcm54980_link_status_get (int fd, uint8_t inst, uint8_t port_addr, uint8_t *link_up);
extern int  bcm54980_exp_reg_read (int fd, uint8_t inst, uint8_t port_addr, uint8_t addr, uint16_t *data);
extern int  bcm54980_exp_reg_write (int fd, uint8_t inst, uint8_t port_addr, uint8_t addr, uint16_t data);
extern int  bcm54980_shadow_reg_read (int fd, uint8_t inst, uint8_t port_addr, uint8_t addr, uint16_t *data);
extern int  bcm54980_shadow_reg_write (int fd, uint8_t inst, uint8_t port_addr, uint8_t addr, uint16_t data);
extern int  bcm54980_aux_shadow_reg_read (int fd, uint8_t inst, uint8_t port_addr, uint8_t addr, uint16_t *data);
extern int  bcm54980_aux_shadow_reg_write (int fd, uint8_t inst, uint8_t port_addr, uint8_t addr, uint16_t data);
extern int  bcm54980_loopback(int fd, uint8_t inst, uint8_t port_addr, uint8_t ext, uint8_t ena);
extern int  bcm54980_ext_loopback(int fd, uint8_t inst, uint8_t port_addr, uint8_t ena);
extern int  bcm54980_line_loopback(int fd, uint8_t inst, uint8_t port_addr, uint8_t ena);
extern int  bcm54980_ext_volt_regulate(int fd, uint8_t inst, uint8_t port_addr, uint8_t volt);
extern int  bcm54980_init (int fd, uint8_t inst, uint8_t port_addr);
extern int  bcm54980_status_check (int fd, uint8_t inst, uint8_t port_addr);
extern int  bcm54980_led_set (int fd, uint8_t inst, uint8_t port_addr, uint8_t green, uint8_t mode);
extern int  bcm54980_led_green_on (int fd, uint8_t inst, uint8_t port_addr);
extern int  bcm54980_led_green_off (int fd, uint8_t inst, uint8_t port_addr);
extern int  bcm54980_led_red_on (int fd, uint8_t inst, uint8_t port_addr);
extern int  bcm54980_led_red_off (int fd, uint8_t inst, uint8_t port_addr);
extern int  bcm54980_loopback_print (int fd, uint8_t inst, uint8_t port_addr);
extern int bcm54980_reg_read (int fd, uint8_t inst, uint8_t port_addr, uint8_t addr, uint16_t *data);
extern int bcm54980_reg_write (int fd, uint8_t inst, uint8_t port_addr, uint8_t addr, uint16_t data);
extern int bcm54980_aneg_init (int fd, uint8_t inst, uint8_t port_addr);
extern int bcm54980_external_lpbk_plug_init (int fd, uint8_t inst, 
				uint8_t port_addr, uint8_t flag);
extern int bcm54980_super_isolate_ena (int fd, uint8_t inst, uint8_t port_addr, uint8_t ena);
extern int bcm54980_sgmii_mode (int fd, uint8_t inst, uint8_t port_addr, uint8_t ena);
extern int bcm54980_sgmii_pcs_loopback(int fd, uint8_t inst, uint8_t port_addr, uint8_t ena);

extern int bcm54980_set_100mbps (int fd, uint8_t inst, uint8_t port_addr);
extern int bcm54980_set_10mbps  (int fd, uint8_t inst, uint8_t port_addr);

#endif // _BCM54X80_API_H_
