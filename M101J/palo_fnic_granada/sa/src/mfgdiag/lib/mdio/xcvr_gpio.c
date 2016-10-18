/******************************************************************************
 *
 * File: xcvr_gpio.c
 * Name: Yulianto Ko
 *
 * Description: XCVR GPIO operations
 *
 * Copyright (c) 2007, Nuova Systems, Inc.
 * All rights reserved.
 *
 *****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "xcvr_defines.h"
#include "xcvr_gpio.h"
#include "xcvr_brcm8726.h"

 
#define XCVR_CALC_MASK(offset, len)  (((1 << (len + offset))) - (1 << offset))
#define XCVR_2W_MAX_RETRIES     16

extern int verbose;
 
static void
clock_out (gpio_pin_t *bus, int bit)
{
    if (bit) {
        mdio_hi(bus);
    } else {
        mdio_lo(bus);
    }
//    usleep(GPIO_DELAY);
    mdc_hi(bus);
//    usleep(GPIO_DELAY);
    mdc_lo(bus);
}
 
static void
bitbang_pre (gpio_pin_t *bus, uint32_t op, uint32_t prtad, uint32_t devad)
{
    int i;
    mdio_active(bus);
 
    // Preamble
    for (i = 0; i < 33; i++) {
        clock_out(bus, 1);
    }
 
    // Start of frame
    clock_out(bus, 0);
    clock_out(bus, 0);
 
    // OP
    switch (op) {
    case MDIO_ADDRESS_OP:
        clock_out(bus, 0);
        clock_out(bus, 0);
        break;
    case MDIO_WRITE_OP:
        clock_out(bus, 0);
        clock_out(bus, 1);
        break;
    case MDIO_READ_INC_OP:
        clock_out(bus, 1);
        clock_out(bus, 0);
        break;
    case MDIO_READ_OP:
        clock_out(bus, 1);
        clock_out(bus, 1);
        break;
    }
 
    // PRTAD
    for (i = 0; i < 5; i++) {
        clock_out(bus, (prtad & 0x10) != 0);
        prtad <<= 1;
    }
 
    // DEVAD
    for (i = 0; i < 5; i++) {
        clock_out(bus, (devad & 0x10) != 0);
        devad <<= 1;
    }
}
 
// Set mdio to idle
static void
xcvr_gpio_mdio_idle(gpio_pin_t *bus)
{
    mdc_lo(bus);
//    usleep(GPIO_DELAY);
    mdc_hi(bus);
//    usleep(GPIO_DELAY);

    mdio_tristate(bus);
    ppc_gpio_set_bit(2, GPIO_REG_DATA, bus->mdc_pin);
}
 
// Send the turnaround (10)
static void
xcvr_gpio_mdio_wr_turnaround (gpio_pin_t *bus)
{
    // send the turnaround (10)
    mdc_lo(bus);
    mdio_hi(bus);
//    usleep(GPIO_DELAY);
    mdc_hi(bus);
//    usleep(GPIO_DELAY);
    mdc_lo(bus);
    mdio_lo(bus);
//    usleep(GPIO_DELAY);
    mdc_hi(bus);
//    usleep(GPIO_DELAY);
}
 
static void
xcvr_gpio_mdio_wr_addr (gpio_pin_t *bus,
                   uint32_t prtad, uint32_t devad, uint32_t regad)
{
    int i;

    bitbang_pre(bus, MDIO_ADDRESS_OP, prtad, devad);
 
    xcvr_gpio_mdio_wr_turnaround(bus);
 
    // write 16 bits of address
    for (i = 0; i < 16; i++) {
        mdc_lo(bus);
        if (regad & 0x8000) {
            mdio_hi(bus);
        } else {
            mdio_lo(bus);
        }
//        usleep(GPIO_DELAY);
        mdc_hi(bus);
//        usleep(GPIO_DELAY);
        regad <<= 1;
    }
 
    // Set mdio to idle
    xcvr_gpio_mdio_idle(bus);
}
 
static void
xcvr_gpio_mdio_wr_curr_reg (gpio_pin_t *bus,
                            uint32_t prtad, uint32_t devad, uint16_t regval)
{
    int i;
 
    bitbang_pre(bus, MDIO_WRITE_OP, prtad, devad);
 
    xcvr_gpio_mdio_wr_turnaround(bus);
 
    // write 16 bits of address
    for (i = 0; i < 16; i++) {
        mdc_lo(bus);
        if (regval & 0x8000) {
            mdio_hi(bus);
        } else {
            mdio_lo(bus);
        }
//        usleep(GPIO_DELAY);
        mdc_hi(bus);
//        usleep(GPIO_DELAY);
        regval <<= 1;
    }
 
    // Set mdio to idle
    xcvr_gpio_mdio_idle(bus);


}
 
static void
xcvr_gpio_mdio_rd_curr_reg (gpio_pin_t *bus,
                            uint32_t prtad, uint32_t devad, uint16_t *regval)
{
    uint16_t rdreg;
    int i;
 
    bitbang_pre(bus, MDIO_READ_OP, prtad, devad);
 
    ppc_gpio_clr_bit(2, GPIO_REG_DIRECTION,  bus->mdio_pin);

//    mdio_tristate(bus);
//    usleep(GPIO_DELAY);
    mdc_hi(bus);
//    usleep(GPIO_DELAY);
    mdc_lo(bus);

//    usleep(GPIO_DELAY);
    mdc_hi(bus);
//    usleep(GPIO_DELAY);

    // read 16 bits of register data
    rdreg = 0;
    for (i = 0; i < 16; i++) {
        mdc_lo(bus);
//        usleep(GPIO_DELAY);
        rdreg <<= 1;
        rdreg |= gpio_mdio_read(bus);
        mdc_hi(bus);
//        usleep(GPIO_DELAY);
    }
    *regval = rdreg;

    mdc_lo(bus);
//    usleep(GPIO_DELAY);
    mdc_hi(bus);
//    usleep(GPIO_DELAY);

    ppc_gpio_set_bit(2, GPIO_REG_DIRECTION,  bus->mdio_pin);
    xcvr_gpio_mdio_idle(bus);
}

#define GPIO_MDIO_SLEEP 0 
void
xcvr_gpio_mdio_write (gpio_pin_t *bus, uint32_t prtad, uint32_t devad,
                      uint32_t regad, uint16_t regval)
{
    xcvr_gpio_mdio_wr_addr(bus, prtad, devad, regad);
    usleep(GPIO_MDIO_SLEEP);
    xcvr_gpio_mdio_wr_curr_reg(bus, prtad, devad, regval);
 
    DBG_XCVR_ERR("%s: prtad: %x, devad: %x, regad: %x, writeval: %x\n",
                 __FUNCTION__, prtad, devad, regad, regval);
    // wait 100 milli seconds.
    usleep(GPIO_MDIO_SLEEP);
}
 
void
xcvr_gpio_mdio_read (gpio_pin_t *bus, uint32_t prtad, uint32_t devad,
                     uint32_t regad, uint16_t *regval)
{
    xcvr_gpio_mdio_wr_addr(bus, prtad, devad, regad);
//    usleep(GPIO_MDIO_SLEEP);
    xcvr_gpio_mdio_rd_curr_reg(bus, prtad, devad, regval);
 
    DBG_XCVR_ERR("%s: prtad: %x, devad: %x, regad: %x, readval: %x\n",
                 __FUNCTION__, prtad, devad, regad, *regval);
    // wait 100 milli seconds.
//    usleep(GPIO_MDIO_SLEEP);
}
 
static void
xcvr_gpio_set_high (uint32_t offset, uint8_t gpio_bit)
{
#if 0
    uint32_t val;
 
    val = inl(XCVR_GPIO_BASE + offset);
 
    DBG_XCVR_ERR("%s: reading reg: %x, val: %x\n", __FUNCTION__, offset, val);
 
    // Set the bit to high
    val = val | (1 << gpio_bit);
    DBG_XCVR_ERR("%s: writing reg: %x, val: %x\n", __FUNCTION__, offset, val);
    outl(val, XCVR_GPIO_BASE + offset);
#else
    printf("%s,%d: not implemneted\n", __func__, __LINE__);
#endif
}
static void
xcvr_gpio_set_low (uint32_t offset, uint8_t gpio_bit)
{
#if 0
    uint32_t val;
 
    val = inl(XCVR_GPIO_BASE + offset);
 
    DBG_XCVR_ERR("%s: reading reg: %x, val: %x\n", __FUNCTION__, offset, val);
    // Zero the bit
    val = val & ~(1 << gpio_bit);
    DBG_XCVR_ERR("%s: writing reg: %x, val: %x\n", __FUNCTION__, offset, val);
    outl(val, XCVR_GPIO_BASE + offset);
#else
    printf("%s,%d: not implemneted\n", __func__, __LINE__);
#endif
}
 
void
xcvr_gpio_phy_reset (void)
{
    // Set GPIO level to low
    xcvr_gpio_set_low(XCVR_PHY_RESET_GP_LVL1_REG,
                      XCVR_GPIO_PHY_RESET_BIT);
 
    // Set GPIO IO Select to output
    xcvr_gpio_set_low(XCVR_PHY_RESET_GP_IO_SEL1_REG,
                      XCVR_GPIO_PHY_RESET_BIT);
 
//    sleep(1);
    // Set GPIO level to high
    xcvr_gpio_set_high(XCVR_PHY_RESET_GP_LVL1_REG,
                       XCVR_GPIO_PHY_RESET_BIT);
}
 
void
xcvr_gpio_mdio_field_read (
    gpio_pin_t *bus, uint32_t prtad, uint32_t dev_addr, uint32_t reg_addr,
    uint32_t offset, uint32_t len, uint16_t *data)
{
    uint16_t tmp_data;

    xcvr_gpio_mdio_read(bus, prtad, dev_addr, reg_addr, &tmp_data);
 
    *data = (tmp_data & XCVR_CALC_MASK(offset, len)) >> offset;
}
 
void
xcvr_gpio_mdio_field_write (
    gpio_pin_t *bus, uint32_t prtad, uint32_t dev_addr, uint32_t reg_addr,
    uint32_t offset, uint32_t len, uint16_t data)
{
    uint16_t mask = XCVR_CALC_MASK(offset, len);
    uint16_t tmp_data;
 
    xcvr_gpio_mdio_read(bus, prtad, dev_addr, reg_addr, &tmp_data);
 
    tmp_data &= ~mask;
    tmp_data |=
        ((data << offset) & mask);
 
    xcvr_gpio_mdio_write(bus, prtad, dev_addr, reg_addr, tmp_data);
}
 
void
xcvr_gpio_xgmdio_read_nvr_range (
    gpio_pin_t *bus, uint32_t prtad, uint32_t start_addr,
    uint32_t end_addr, uint8_t *data, uint32_t *num_addr)
{
    uint32_t addr, count = 0;
    uint16_t data16;
 
    if (!bus || *num_addr < (end_addr - start_addr + 1))
        return;
 
    *num_addr = 0;
 
    for (addr = start_addr;
         addr <= end_addr;
         addr++, count++) {
 
        xcvr_gpio_mdio_read(bus, prtad, 1, addr, &data16);
        data[count] = (uint8_t) (data16 & 0xff);
    }
    *num_addr = count;
}
 
// Read SFP EEPROM content
// i2c_addr: 7 bit address (0xA0 or 0xA2)
// start_addr: Start address
// num_bytes: Number of bytes to read
void
xcvr_gpio_fcot_info_read (
    gpio_pin_t *bus, uint32_t prtad, uint8_t i2c_addr,
    uint32_t start_addr, uint32_t num_bytes, uint8_t *buf)
{
    uint16_t   data;
    uint32_t   num_to_read, num_bytes_read, retries;
 
    if (!bus)
        return;
 
    if (i2c_addr != SFP_MSA_A0 && i2c_addr != SFP_MSA_A2) {
        return;
    }
 
    if (start_addr > 255) {
        return;
    }
 
    // Shift 1 to make it 7 bit address
    i2c_addr = i2c_addr >> 1;
 
    // Set the i2c addr
    xcvr_gpio_mdio_field_write(
              bus, prtad, 1,
              BRCM_REG_2WBCOUNT,
              BRCM_BIT_2WBCOUNT_2W_CHPADDR,
              BRCM_BIT_2WBCOUNT_2W_CHPADDR_SIZE, i2c_addr);
 
    // Loop and read 16 bytes at a time
    while (num_bytes > 0) {
        if (num_bytes > BRCM_MAX_I2C_READ_CNT) {
            num_to_read = BRCM_MAX_I2C_READ_CNT;
        } else {
            num_to_read = num_bytes;
        }
 
        // Set the number of bytes to read
        xcvr_gpio_mdio_field_write(
                  bus, prtad, 1,
                  BRCM_REG_2WBCOUNT,
                  BRCM_BIT_2WBCOUNT_2W_CNTREG,
                  BRCM_BIT_2WBCOUNT_2W_CNTREG_SIZE,
                  num_to_read);
        // Set the start_addr
        xcvr_gpio_mdio_field_write(
                  bus, prtad, 1,
                  BRCM_REG_2WMEMADD,
                  BRCM_BIT_2WMEMADD_2W_MEMADDR,
                  BRCM_BIT_2WMEMADD_2W_MEMADDR_SIZE, start_addr);
 
        // Start the i2c operation
        xcvr_gpio_mdio_field_write(
                  bus, prtad, 1,
                  BRCM_REG_2WCONTROL,
                  BRCM_BIT_2WCONTROL_2WENABLE,
                  BRCM_BIT_2WCONTROL_2WENABLE_SIZE, 1);
 
        // Poll until operation is done
        data = TRUE;
        retries = 0;
        while (data == TRUE) {
            xcvr_gpio_mdio_field_read(
                      bus, prtad, 1,
                      BRCM_REG_2WCONTROL,
                      BRCM_BIT_2WCONTROL_2WENABLE,
                      BRCM_BIT_2WCONTROL_2WENABLE_SIZE, &data);
//            usleep(100);
            retries++;
            if (retries > XCVR_2W_MAX_RETRIES) {
                DBG_XCVR_ERR("%s(): Max retries reached while reading 2W\n",
                             __FUNCTION__);
                break;
            }
        }
 
        // Copy sprom data to buffer
        xcvr_gpio_xgmdio_read_nvr_range(
                  bus, prtad,
                  BRCM_SPROM_START_MDIO_ADDR,
                  BRCM_SPROM_START_MDIO_ADDR + num_to_read - 1,
                  buf, &num_bytes_read);
 
        DBG_XCVR_ERR("start_addr: %d, num_bytes: %d, num_to_read: %d\n",
                     start_addr, num_bytes, num_to_read);
        start_addr += num_to_read;
        buf        += num_to_read;
        num_bytes  -= num_to_read;
 
    } /* while */
}
 
/* end of file */
