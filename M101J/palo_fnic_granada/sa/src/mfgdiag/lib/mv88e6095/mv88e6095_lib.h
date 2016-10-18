/********************************************************************
 *
 *      File:   mv88e6095_lib.h 
 *      Name:   Sudharshan Kadari
 *
 *      Description:
 *       Marvell switch access library 
 *
 *
 * Copyright (c) 1985-2007 by Cisco Systems, Inc.
 * All rights reserved.
 *
 *
 *********************************************************************/

#ifndef _MV88E6095_LIB_H_
#define _MV88E6095_LIB_H_

extern int mv88e6095_read(int fd, int addr, int dev, int reg, uint16_t *rp);
extern int mv88e6095_write(int fd, int address, int dev, int reg, uint16_t value);

extern int mv_atu_dump (int fd, uint32_t addr, uint8_t dbnum);
extern int mv_atu_flush_dbnum (int fd, uint32_t addr, uint8_t dbnum);
extern int mv_atu_flush_all (int fd, uint32_t addr);
extern int mv_atu_add(int fd, uint32_t addr, uint8_t dbnum, char *mac_addr, uint8_t mac_pri,
                uint16_t port_vec, uint16_t entry_state);
extern int mv_atu_add_static(int fd, uint32_t addr, uint8_t dbnum, char *mac_addr, uint8_t mac_pri,
                uint16_t port_vec);
extern int mv_atu_del(int fd, uint32_t addr, uint8_t dbnum, char *mac_addr, uint8_t mac_pri,
                uint16_t port_vec);

extern int  mv88e6095_stats_egress_dump (int fd, int address, int port);
extern int  mv88e6095_stats_ingress_dump (int fd, int address, int port);
extern int mv88e6095_flush_port_stats(int fd, int address, int port);
extern int mv88e6095_flush_all_stats(int fd, int address);

extern int mv88e6095_ppu(int fd, uint32_t addr, int ena);
extern int mv88e6095_lpbk_print(int fd, uint32_t addr, int port);
extern int mv88e6095_lpbk_ena(int fd, uint32_t addr, int port);
extern int mv88e6095_lpbk_dis(int fd, uint32_t addr, int port);
extern int mv88e6095_link_status_print(int fd, uint32_t addr, int port);

extern int mv88e6095_link_status(int fd, uint32_t addr, int port, uint8_t *up);
extern int mv88e6095_vlan_table_config(int fd, uint32_t addr, int port, uint16_t vlan_table, uint16_t, uint16_t);
//extern int mv88e6095_reset(int fd, int address, int rt);
extern int mv88e6095_diag_init(int fd, uint32_t mdio_addr, int port_addr);
extern int mv88e6095_ingress_stats_get(int fd, int addr, int port, uint32_t *data);
extern int mv88e6095_capture_ingress_port_stats(int fd, int addr, int port);
extern int mv88e6095_stats_reg_rd(int fd, int addr, int port,
  int reg, uint32_t *pdata, uint8_t ingress);

#endif // _MV88E6095_LIB_H_
