/********************************************************************
 *
 *      File:   mv88e1111_lib.h 
 *      Name:   Sudharshan Kadari
 *
 *      Description:
 *       Marvell Phy access library 
 *
 *
 * Copyright (c) 1985-2007 by Cisco Systems, Inc.
 * All rights reserved.
 *
 *
 *********************************************************************/

#ifndef _MV88E1111_LIB_H_
#define _MV88E1111_LIB_H_

extern int mv88e1111_write(int fd, uint8_t mdio_addr, 
		uint8_t port_addr, int reg, uint16_t value);
extern int mv88e1111_read(int fd, uint8_t mdio_addr, 
		uint8_t port_addr, int reg, uint16_t *value);

extern int mv88e1111_link_status_print(int fd, uint32_t address, int port_addr);
extern int mv88e1111_link_status(int fd, uint32_t address, int port_addr, uint8_t *up);
extern int mv88e1111_lpbk_dis(int fd, uint32_t address, int port_addr);
extern int mv88e1111_lpbk_ena(int fd, uint32_t address, int port_addr);
extern int mv88e1111_lpbk_print(int fd, uint32_t address, int port_addr);
extern int mv88e1111_lpbk(int fd, uint8_t mdio_addr, uint8_t port_addr, uint8_t ena);
extern int mv88e1111_init(int fd, uint32_t mdio_addr, int port_addr, uint8_t master);
extern int mv88e1111_an_restart(int fd, uint32_t mdio_addr, int port_addr);
extern int mv88e1111_clk_init(int fd, uint32_t mdio_addr, int port_addr);
 
#endif // _MV88E1111_LIB_H_
