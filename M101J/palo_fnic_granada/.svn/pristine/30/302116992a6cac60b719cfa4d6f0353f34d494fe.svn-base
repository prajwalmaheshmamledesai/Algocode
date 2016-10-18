/********************************************************************
 *
 * File:   phy_rom_update.h
 * Name:   Raghu Boreda/Sudharshan Kadari
 *
 * Description: Support for SPI ROM programming 
 *              (BCM8073/BCM8754 microcode update)
 * Copyright (c) 2001,2002,2003,2004 by Nuova Systems, Inc.
 ********************************************************************/
#ifndef _PHY_ROM_UPDATE_H_
#define _PHY_ROM_UPDATE_H_
#define PMA_PMD_ADDR        1

/*
 * SPI-ROM Program related defs
 */
#define SPI_CTRL_1_L        0xC000
#define SPI_CTRL_1_H        0xC002
#define SPI_CTRL_2_L        0xC400
#define SPI_CTRL_2_H        0xC402
#define SPI_TXFIFO          0xD000
#define SPI_RXFIFO          0xD400

#define WR_CPU_CTRL_REGS    0x11
#define RD_CPU_CTRL_REGS    0xEE
#define WR_CPU_CTRL_FIFO    0x66
/*
 * SPI Controller Commands(Messages).
 */
#define MSGTYPE_HWR          0x40
#define MSGTYPE_HRD          0x80
#define WRSR_OPCODE          0x01
#define WR_OPCODE            0x02
#define WRDI_OPCODE          0x04
#define RDSR_OPCODE          0x05
#define WREN_OPCODE          0x06
#define WR_BLOCK_SIZE        0x40
#define TOTAL_WR_BYTE        0x4000

#define WR_TIMEOUT   1000000

int phy_eeprom_rd (int fd, uint8_t mdio_addr, uint8_t port_addr,
            uint8_t device_class, uint16_t reg_addr, uint16_t *data);
int phy_eeprom_wr (int fd, uint8_t mdio_addr, uint8_t port_addr,
            uint8_t device_class, uint16_t reg_addr, uint16_t data);

int phy_spirom_program(int, uint8_t ,uint8_t ,char *);
#endif
