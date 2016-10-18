/********************************************************************
 *
 *      File:   pci_regs.h
 *      Name:   Sudharshan Kadari
 *
 *      Description:
 *      pci_regs header file. 
 *
 *
 * Copyright (c) 1985-2009 by Cisco Systems, Inc.
 * All rights reserved.
 *
 *
 *********************************************************************/

#ifndef _PCI_REGS_H_
#define _PCI_REGS_H_

// Command Definitions
#define PCI_IRQ_ACK		0x00
#define PCI_SPL_CYCLE		0x01
#define PCI_IO_READ		0x02
#define PCI_IO_WRITE		0x03
#define PCI_IO_READ		0x02
#define PCI_MEM_READ		0x06
#define PCI_MEM_WRITE		0x07
#define PCI_CFG_READ		0x0A
#define PCI_CFG_WRITE		0x0B
#define PCI_MEM_MUL_READ	0x0C
#define PCI_DUAL_ADDR_CYCLE	0x0D
#define PCI_MEM_READ_LINE	0x0E
#define PCI_MEM_WR_INV		0x0F

#define PCI_TYPE_0		0x00
#define PCI_TYPE_1		0x01

#define PCI_CFG_ADDR(bn, dn, fn, rn, tn)	\
	((((bn) << 16)&0xFF) | (((dn) << 11)&0x1F) | \
	 (((fn) << 8) &0x07) | (((rn) << 2) & 0x3F)| (tn))


// PCI Config Space Registers.
#define PCI_VEN_DEV_ID		0x00
#define PCI_CMD_STATUS		0x04
#define PCI_REV_CLASS		0x08
#define PCI_CACHE_CFG		0x0C
#define PCI_BASE_ADDR0		0x10
#define PCI_BASE_ADDR1		0x14
#define PCI_BASE_ADDR2		0x18
#define PCI_BASE_ADDR3		0x1C
#define PCI_BASE_ADDR4		0x20
#define PCI_BASE_ADDR5		0x24
#define PCI_CIS_PTR		0x28
#define PCI_SYS_ID		0x2C
#define PCI_EXP_ROM_BASE	0x30
#define PCI_CAPABILITY		0x34
#define PCI_RESERVED0		0x38
#define PCI_INTERRUPT		0x3C

// Address Mapping
#define PCI_MEM32_MAP(addr)	(((addr) << 4) | 0x08)	
#define PCI_MEM64_MAP(addr)	(((addr) << 4) | 0x0C)	
#define PCI_IO_MAP(addr)	(((addr) << 2) | 0x01)	

#endif // _PCI_REGS_H_
