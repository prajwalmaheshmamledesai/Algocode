/********************************************************************
 *
 *      File:   redwood_phy_spi_rom_update.c
 *      Name:   Bruce McLoughlin
 *
 *      Description: Support for SPI ROM programming 
 *                   (BCM8073 microcde update)
 *
 *
 *      Copyright (c) 2001,2002,2003,2004 by Nuova Systems, Inc.
 ********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/types.h>
#include "phy_rom_update.h"

static int write_message(int fd, uint8_t mdio_addr, uint8_t port_addr,
			uint16_t wrdata, uint16_t * rddata)
{
    int tries = 1000;
    uint16_t     tmp_data = 0;

    phy_eeprom_rd(fd,  mdio_addr, port_addr, PMA_PMD_ADDR, 0xCA13, &tmp_data);
    phy_eeprom_wr(fd,  mdio_addr, port_addr, PMA_PMD_ADDR, 0xCA12, wrdata);

    do {
        phy_eeprom_rd(fd, mdio_addr, port_addr, PMA_PMD_ADDR, 0x9005, &tmp_data);
        if (tmp_data & 0x4) break;
    } while (--tries);

    if (!(tmp_data & 0x4)) {
        printf( "write_message failed: wrdata %04x\n",
                     wrdata);
        return 1;
    }

    phy_eeprom_rd(fd,  mdio_addr, port_addr, PMA_PMD_ADDR, 0xCA13, &tmp_data);
    *rddata = tmp_data;
    phy_eeprom_rd(fd,  mdio_addr, port_addr, PMA_PMD_ADDR, 0x9003, &tmp_data);

    return 0;
}
/*
 *  Function: 
 *      phy_rom_wait
 *  
 *  Purpose:  
 *      Wait for data to be written to the SPI-ROM.
 *  Input:
 *      mdio_addr, port
 */
static int phy_rom_wait(int fd, uint8_t mdio_addr, uint8_t port_addr)
{
    uint16_t     rd_data;
    uint16_t     wr_data;
    int        count;
    int        SPI_READY;
    int         tries = 1000;

    rd_data = 0;
    tries = 1000;
    do {
        /*
         * Write SPI Control Register Read Command. 
         */
        count = 1;
        wr_data = ((RD_CPU_CTRL_REGS * 0x0100) | count);
        write_message(fd, mdio_addr, port_addr, wr_data, &rd_data);
        wr_data = SPI_CTRL_1_L;
        write_message(fd, mdio_addr, port_addr, wr_data, &rd_data);
        if (rd_data & 0x0100) {
            break;
        }
    } while (--tries);

    if (!(rd_data & 0x0100)) {
        printf( "phy MDIO:%d, portad: %d timeout 1\n",
                     mdio_addr, port_addr);
    }

    SPI_READY = 1;
    while (SPI_READY == 1) {
        /*
         * Set-up SPI Controller To Receive SPI EEPROM Status. 
         */
        count = 1;
        wr_data = ((WR_CPU_CTRL_REGS * 0x0100) | count);
        write_message(fd, mdio_addr, port_addr, wr_data, &rd_data);
        wr_data = SPI_CTRL_2_H;
        write_message(fd, mdio_addr, port_addr, wr_data, &rd_data);
        wr_data = 0x0100;
        write_message(fd, mdio_addr, port_addr, wr_data, &rd_data);
        /*
         * Fill-up SPI Transmit Fifo To check SPI Status. 
         */
        count = 2;
        wr_data = ((WR_CPU_CTRL_FIFO * 0x0100) | count);
        write_message(fd, mdio_addr, port_addr, wr_data, &rd_data);
        /*
         * Write Tx Fifo Register Address. 
         */
        wr_data = SPI_TXFIFO;
        write_message(fd, mdio_addr, port_addr, wr_data, &rd_data);

        /*
         * Write SPI Tx Fifo Control Word-1. 
         */
        wr_data = ((1 * 0x0100) | MSGTYPE_HRD);
        write_message(fd, mdio_addr, port_addr, wr_data, &rd_data);
        /*
         * Write SPI Tx Fifo Control Word-2. 
         */
        wr_data = RDSR_OPCODE;
        write_message(fd, mdio_addr, port_addr, wr_data, &rd_data);
        /*
         * Write SPI Control Register Write Command. 
         */
        count = 2;
        wr_data = ((WR_CPU_CTRL_FIFO * 0x0100) | count);
        write_message(fd, mdio_addr, port_addr, wr_data, &rd_data);
        /*
         * Write SPI Control -1 Register Address. 
         */
        wr_data = SPI_CTRL_1_L;
        write_message(fd, mdio_addr, port_addr, wr_data, &rd_data);
        /*
         * Write SPI Control -1 Register Word-1. 
         */
        wr_data = 0x0101;
        write_message(fd, mdio_addr, port_addr, wr_data, &rd_data);
        /*
         * Write SPI Control -1 Register Word-2. 
         */
        wr_data = 0x0100;
        write_message(fd, mdio_addr, port_addr, wr_data, &rd_data);
        /*
         * Write SPI Control Register Write Command. 
         */
        count = 1;
        wr_data = ((WR_CPU_CTRL_REGS * 0x0100) | count);
        write_message(fd, mdio_addr, port_addr, wr_data, &rd_data);
        /*
         * Write SPI Control -1 Register Address. 
         */
        wr_data = SPI_CTRL_1_H;
        write_message(fd, mdio_addr, port_addr, wr_data, &rd_data);
        /*
         * Write SPI Control -1 Register Word-2. 
         */
        wr_data = 0x0103;
        write_message(fd, mdio_addr, port_addr, wr_data, &rd_data);
        /*
         * Wait For 64 bytes To be written.  
         */
        rd_data = 0x0000;
        tries = 1000;
        do {
            count = 1;
            wr_data = ((RD_CPU_CTRL_REGS * 0x0100) | count);
            write_message (fd, mdio_addr, port_addr, wr_data, &rd_data);
            wr_data = SPI_CTRL_1_L;
            write_message (fd, mdio_addr, port_addr, wr_data, &rd_data);
            if (rd_data & 0x0100) {
                break;
            }
        } while (--tries);

        if (!(rd_data & 0x0100)) {
            printf( "phy 8073 : ASIC: %d, portad: %d timeout 2\n",
                         mdio_addr, port_addr);
        }
        /*
         * Write SPI Control Register Read Command. 
         */
        count = 1;
        wr_data = ((RD_CPU_CTRL_REGS * 0x0100) | count);
        write_message(fd, mdio_addr, port_addr, wr_data, &rd_data);
        /*
         * Write SPI Control -1 Register Address. 
         */
        wr_data = SPI_RXFIFO;
        write_message(fd, mdio_addr, port_addr, wr_data, &rd_data);
        if ((rd_data & 0x1) == 0) {
            SPI_READY = 0;
        }
    }                           /* SPI_READY */
    return 0;
}

/*
 * Function: 
 *      phy_rom_write_enable_set
 *
 * Purpose:  
 *      Enable disable protection on SPI_EEPROM
 *
 * Input:
 *      mdio_addr
 *      port
 *      enable
 * Output:
 *      SOC_E_xxx
 *
 * Notes: 
 *          25AA256 256Kbit Serial EEPROM
 *          STATUS Register
 *          +------------------------------------------+
 *          | WPEN | x | x | x | BP1 | BP0 | WEL | WIP |
 *          +------------------------------------------+
 *      BP1 BP0  :   Protected Blocks
 *       0   0   :  Protect None
 *       1   1   :  Protect All
 *      
 *      WEL : Write Latch Enable
 *       0  : Do not allow writes
 *       1  : Allow writes
 */

static int phy_rom_write_enable_set(int fd, uint8_t mdio_addr,
                                    uint8_t port_addr, int enable)
{
    uint16_t     rd_data;
    uint16_t     wr_data;
    int        count;
    /*
     * Write SPI Control Register Write Command. 
     */
    count = 2;
    wr_data = ((WR_CPU_CTRL_FIFO * 0x0100) | count);
    write_message(fd, mdio_addr, port_addr, wr_data, &rd_data);
    /*
     * Write SPI Control -2 Register Address. 
     */
    wr_data = SPI_CTRL_2_L;
    write_message(fd, mdio_addr, port_addr, wr_data, &rd_data);
    /*
     * Write SPI Control -2 Register Word-1. 
     */
    wr_data = 0x8200;
    write_message(fd, mdio_addr, port_addr, wr_data, &rd_data);
    /*
     * Write SPI Control -2 Register Word-2. 
     */
    wr_data = 0x0100;
    write_message(fd, mdio_addr, port_addr, wr_data, &rd_data);
    /*
     * Fill-up SPI Transmit Fifo With SPI EEPROM Messages.
     * Write SPI Control Register Write Command.
     */
    count = 2;
    wr_data = ((WR_CPU_CTRL_FIFO * 0x0100) | count);
    write_message(fd, mdio_addr, port_addr, wr_data, &rd_data);
    /*
     * Write Tx Fifo Register Address. 
     */
    wr_data = SPI_TXFIFO;
    write_message(fd, mdio_addr, port_addr, wr_data, &rd_data);
    /*
     * Write SPI Tx Fifo Control Word-1. 
     */
    wr_data = ((2 * 0x0100) | MSGTYPE_HWR);
    write_message(fd, mdio_addr, port_addr, wr_data, &rd_data);
    /*
     * Write SPI Tx Fifo Control Word-2. 
     */
    wr_data = ((WRSR_OPCODE * 0x0100) | (enable ? 0x2 : 0xc));
    write_message(fd, mdio_addr, port_addr, wr_data, &rd_data);
    /*
     * Write SPI Control Register Write Command. 
     */
    count = 2;
    wr_data = ((WR_CPU_CTRL_FIFO * 0x0100) | count);
    write_message(fd, mdio_addr, port_addr, wr_data, &rd_data);
    /*
     * Write SPI Control -1 Register Address. 
     */
    wr_data = SPI_CTRL_1_L;
    write_message(fd, mdio_addr, port_addr, wr_data, &rd_data);
    /*
     * Write SPI Control -1 Register Word-1. 
     */
    wr_data = 0x0101;
    write_message(fd, mdio_addr, port_addr, wr_data, &rd_data);
    /*
     * Write SPI Control -1 Register Word-2. 
     */
    wr_data = 0x0003;
    write_message(fd, mdio_addr, port_addr, wr_data, &rd_data);
    /*
     * Wait For WRSR Command To be written. 
     */
    phy_rom_wait(fd, mdio_addr, port_addr);
    return 0;
}

/*
 *  Function: 
 *
 *      phy_spirom_program
 *
 *  Purpose:  
 *      Program SPI-EEPROM.
 * 
 *  Input:
 *      mdio_addr, port,  fn (.bin file)
 */
int phy_spirom_program (int fd, uint8_t mdio_addr, uint8_t port_addr, char *fn)
{
    uint16_t     rd_data;
    uint16_t     wr_data;
    int        j;
    int        i;
    int        count;
    FILE      *fp = NULL;
    uint8_t      spi_values[64];

    /*
     * ser_boot pin HIGH
     */
    phy_eeprom_rd(fd, mdio_addr, port_addr, PMA_PMD_ADDR, 0xCA85,
                  &rd_data);
    phy_eeprom_wr(fd, mdio_addr, port_addr, PMA_PMD_ADDR, 0xCA85,
                  rd_data | 1);

    //MODIFY_PHY8073_PMA_PMD_ADDR_REG
    //                    (fd, mdio_addr, port_addr, 0xCA85, 0x1, 0x1);
    /*
     * Read LASI Status registers To clear initial Failure status.
     */
    phy_eeprom_rd(fd, mdio_addr, port_addr, PMA_PMD_ADDR, 0x9003, &rd_data);
    phy_eeprom_rd(fd, mdio_addr, port_addr, PMA_PMD_ADDR, 0x9004, &rd_data);
    phy_eeprom_rd(fd, mdio_addr, port_addr, PMA_PMD_ADDR, 0x9005, &rd_data);
    /*
     * Enable the LASI For Message out. 
     */
    phy_eeprom_wr(fd, mdio_addr, port_addr, PMA_PMD_ADDR, 0x9000, 0x0004);
    /*
     * 0x9002, bit 2 Is Rx Alarm enabled For LASI. 
     */
    phy_eeprom_wr(fd, mdio_addr, port_addr, PMA_PMD_ADDR, 0x9002, 0x0004);
    /*
     * Read Any Residual Message out register.
     */
    phy_eeprom_rd(fd, mdio_addr, port_addr, PMA_PMD_ADDR, 0xCA13, &rd_data);
    /*
     * Clear LASI Message Out Status. 
     */
    phy_eeprom_rd(fd, mdio_addr, port_addr, PMA_PMD_ADDR, 0x9003, &rd_data);

    phy_rom_write_enable_set(fd, mdio_addr, port_addr, 1);

    /*
     * Fill In the Data bytes.
     */
    fp = fopen(fn, "rb");

    if (fp == NULL) {
        printf( "  phy_8073_rom_program: ASIC: %d, portad: %d\n"
                     "Cannot open file: %s\n", mdio_addr, port_addr, fn);
        return 2;
    } else {
        printf( "  phy_8073_rom_program: ASIC: %d, portad: %d"
                     " file: %s opened\n", mdio_addr, port_addr, fn);
    }

    for (j = 0; j < TOTAL_WR_BYTE; j += 64) {
        /*
         * Setup SPI Controller. 
         */
        count = 2;
        wr_data = ((WR_CPU_CTRL_FIFO * 0x0100) | count);
        write_message(fd, mdio_addr, port_addr, wr_data, &rd_data);

        /*
         * Write SPI Control -2 Register Address.
         */
        wr_data = SPI_CTRL_2_L;
        write_message(fd, mdio_addr, port_addr, wr_data, &rd_data);

        /*
         * Write SPI Control -2 Register Word-1. 
         */
        wr_data = 0x8200;
        write_message(fd, mdio_addr, port_addr, wr_data, &rd_data);

        /*
         * Write SPI Control -2 Register Word-2. 
         */
        wr_data = 0x0100;
        write_message(fd, mdio_addr, port_addr, wr_data, &rd_data);

        /*
         * Fill-up SPI Transmit Fifo.
         * Write SPI Control Register Write Command.
         */
        count = 4 + (WR_BLOCK_SIZE / 2);
        wr_data = ((WR_CPU_CTRL_FIFO * 0x0100) | count);
        write_message(fd, mdio_addr, port_addr, wr_data, &rd_data);

        /*
         * Write Tx Fifo Register Address. 
         */
        wr_data = SPI_TXFIFO;
        write_message(fd, mdio_addr, port_addr, wr_data, &rd_data);

        /*
         * Write SPI Tx Fifo Control Word-1. 
         */
        wr_data = ((1 * 0x0100) | MSGTYPE_HWR);
        write_message(fd, mdio_addr, port_addr, wr_data, &rd_data);

        /*
         * Write SPI Tx Fifo Control Word-2. 
         */
        wr_data = ((MSGTYPE_HWR * 0x0100) | WREN_OPCODE);
        write_message(fd, mdio_addr, port_addr, wr_data, &rd_data);
        /*
         * Write SPI Tx Fifo Control Word-3. 
         */
        wr_data = ((WR_OPCODE * 0x0100) | (0x3 + WR_BLOCK_SIZE));
        write_message(fd, mdio_addr, port_addr, wr_data, &rd_data);

        /*
         * Write SPI Tx Fifo Control Word-4. 
         */
        wr_data = (((j & 0x00FF) * 0x0100) | ((j & 0xFF00) / 0x0100));
        write_message(fd, mdio_addr, port_addr, wr_data, &rd_data);

        if (!(fread(spi_values, 1, sizeof(spi_values), (FILE *) fp))) {
            printf( "phy_8073_rom_program: ASIC: %d, portad: %d\n"
                         "Unable to read file\n", mdio_addr, port_addr);
        }

        for (i = 0; i < WR_BLOCK_SIZE; i += 2) {
            /*
             * Write SPI Tx Fifo Data Word-4. 
             */
            wr_data = ((spi_values[i + 1] * 0x0100) | spi_values[i]);
            write_message(fd, mdio_addr, port_addr, wr_data, &rd_data);
        }

        /*
         * Set-up SPI Controller To Transmit.
         * Write SPI Control Register Write Command.
         */
        count = 2;
        wr_data = ((WR_CPU_CTRL_FIFO * 0x0100) | count);
        write_message(fd, mdio_addr, port_addr, wr_data, &rd_data);

        wr_data = SPI_CTRL_1_L;
        write_message(fd, mdio_addr, port_addr, wr_data, &rd_data);

        /*
         * Write SPI Control -1 Register Word-1. 
         */
        wr_data = 0x0501;
        write_message(fd, mdio_addr, port_addr, wr_data, &rd_data);

        /*
         * Write SPI Control -1 Register Word-2. 
         */
        wr_data = 0x0003;
        write_message(fd, mdio_addr, port_addr, wr_data, &rd_data);

        /*
         * Wait For 64 bytes To be written.  
         */
        phy_rom_wait(fd, mdio_addr, port_addr);
    }                           /* SPI_WRITE */

    phy_rom_write_enable_set(fd, mdio_addr, port_addr, 0);
    /*
     * Disable SPI EEPROM. 
     */
    count = 2;
    wr_data = ((WR_CPU_CTRL_FIFO * 0x0100) | count);
    write_message(fd, mdio_addr, port_addr, wr_data, &rd_data);

    /*
     * Write SPI Control -2 Register Address. 
     */
    wr_data = SPI_CTRL_2_L;
    write_message(fd, mdio_addr, port_addr, wr_data, &rd_data);

    /*
     * Write SPI Control -2 Register Word-1. 
     */
    wr_data = 0x8200;
    write_message(fd, mdio_addr, port_addr, wr_data, &rd_data);

    /*
     * Write SPI Control -2 Register Word-2. 
     */
    wr_data = 0x0100;
    write_message(fd, mdio_addr, port_addr, wr_data, &rd_data);

    /*
     * Fill-up SPI Transmit Fifo With SPI EEPROM Messages. 
     */
    count = 2;
    wr_data = ((WR_CPU_CTRL_FIFO * 0x0100) | count);
    write_message(fd, mdio_addr, port_addr, wr_data, &rd_data);

    /*
     * Write Tx Fifo Register Address. 
     */
    wr_data = SPI_TXFIFO;
    write_message(fd, mdio_addr, port_addr, wr_data, &rd_data);

    /*
     * Write SPI Tx Fifo Control Word-1. 
     */
    wr_data = ((0x1 * 0x0100) | MSGTYPE_HWR);
    write_message(fd, mdio_addr, port_addr, wr_data, &rd_data);

    /*
     * Write SPI Tx Fifo Control Word-2. 
     */
    wr_data = WRDI_OPCODE;
    write_message(fd, mdio_addr, port_addr, wr_data, &rd_data);

    /*
     * Write SPI Control Register Write Command. 
     */
    count = 2;
    wr_data = ((WR_CPU_CTRL_FIFO * 0x0100) | count);
    write_message(fd, mdio_addr, port_addr, wr_data, &rd_data);

    /*
     * Write SPI Control -1 Register Address. 
     */
    wr_data = SPI_CTRL_1_L;
    write_message(fd, mdio_addr, port_addr, wr_data, &rd_data);

    /*
     * Write SPI Control -1 Register Word-1. 
     */
    wr_data = 0x0101;
    write_message(fd, mdio_addr, port_addr, wr_data, &rd_data);

    /*
     * Write SPI Control -1 Register Word-2. 
     */
    wr_data = 0x0003;
    write_message(fd, mdio_addr, port_addr, wr_data, &rd_data);

    /*
     * ser_boot pin LOW
     */
    phy_eeprom_rd(fd, mdio_addr, port_addr, PMA_PMD_ADDR, 0xCA85, &rd_data);
    phy_eeprom_wr(fd, mdio_addr, port_addr, PMA_PMD_ADDR, 0xCA85, rd_data & ~1);
    //MODIFY_PHY8073_PMA_PMD_ADDR_REG(fd, mdio_addr, port_addr, 0xCA85, 0x0, 0x1);
    printf( "phy_spirom_program: ASIC: %d, portad: %d, done\n", mdio_addr,
                 port_addr);

    fclose(fp);
    return 0;
}
