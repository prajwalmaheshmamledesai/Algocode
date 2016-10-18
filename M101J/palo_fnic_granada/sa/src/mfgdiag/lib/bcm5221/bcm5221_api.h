/********************************************************************
 *
 *      File:   bcm5221_api.h
 *      Name:   Sudharshan Kadari
 *
 *      Description:
 *       Broadcom Serdes 5221 register access
 *
 *
 * Copyright (c) 1985-2008 by Cisco Systems, Inc.
 * All rights reserved.
 *
 *
 *********************************************************************/

#ifndef _BCM5221_API_H_
#define _BCM5221_API_H_

extern int bcm5221_soft_reset (int fd, uint8_t inst, uint8_t port_addr);
extern int bcm5221_loopback_ena (int fd, uint8_t inst, uint8_t port_addr, uint8_t ena);
extern int bcm5221_init (int fd, uint8_t inst, uint8_t port_addr);
extern int bcm5221_link_status_get (int fd, uint8_t inst, uint8_t port_addr, uint8_t *link_up);
extern int bcm5221_loopback_print (int fd, uint8_t inst, uint8_t port_addr);
extern int bcm5221_reg_read (int fd, uint8_t inst, uint8_t port_addr, uint8_t addr, uint16_t *data);
extern int bcm5221_reg_write (int fd, uint8_t inst, uint8_t port_addr, uint8_t addr, uint16_t data);
extern int bcm5221_shadow_reg_read (int fd, uint8_t inst, uint8_t port_addr, uint8_t addr, uint16_t *data);
extern int bcm5221_shadow_reg_write (int fd, uint8_t inst, uint8_t port_addr, uint8_t addr, uint16_t data);
#endif // _BCM5221_API_H_
