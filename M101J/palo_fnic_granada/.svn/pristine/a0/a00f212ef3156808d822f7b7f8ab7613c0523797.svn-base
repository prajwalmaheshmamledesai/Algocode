/********************************************************************
 *
 *      File:   mdio_lib.h
 *      Name:   Sudharshan Kadari
 *
 *      Description:
 *       Diag MDIO Header file 
 *
 *
 * Copyright (c) 1985-2007 by Cisco Systems, Inc.
 * All rights reserved.
 *
 *
 *********************************************************************/

#ifndef _MDIO_LIB_H_
#define _MDIO_LIB_H_

extern int mdio_read(int fd, int address, int reg, uint16_t *value);
extern int mdio_write(int fd, int address, int reg, uint16_t value);
extern int mdio_hw_rd(int address, int reg, uint16_t *value);
extern int mdio_hw_wr(int address, int reg, uint16_t value);
extern int mdio_open ();

extern void mdio_close (int fd);
extern void mdio_busscan(uint32_t *bus_found);
extern int  mdio_c45_write(int fd, int mdio_addr, int mdio_port, int device_class,
                    uint16_t reg_addr, uint16_t reg_value);
extern int  mdio_c45_read(int fd, int mdio_addr, int mdio_port, int device_class,
                   uint16_t reg_addr, uint16_t *reg_value);

extern int diag_mdio_read(int fd, int address, int reg, uint16_t *value);
extern int diag_mdio_write(int fd, int address, int reg, uint16_t value);

#endif // _MDIO_LIB_H_
