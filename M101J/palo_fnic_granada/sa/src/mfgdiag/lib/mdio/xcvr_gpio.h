/******************************************************************************
 *
 * File:  xcvr_gpio.h
 * Name:  Yulianto Ko
 *
 * Description: XCVR GPIO definition
 *
 * Copyright (c) 2007, Nuova Systems, Inc.
 * All rights reserved.
 *
 *****************************************************************************/
 
#ifndef __XCVR_GPIO_H__
#define __XCVR_GPIO_H__

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <linux/saratoga-gpio.h>
#include <gpio.h>
#include "ppc_lib.h"

#define GPIO_DELAY                          0 // in us
#define MDIO_ADDRESS_OP                     0 // 00
#define MDIO_WRITE_OP                       1 // 01
#define MDIO_READ_INC_OP                    2 // 10
#define MDIO_READ_OP                        3 // 11
 
#define XCVR_GPIO_PRTAD                     0x14 // 0x14 through 0x1b are BCM8726 addresses
#define XCVR_GPIO_DEVAD                     1 // PMA/PMD
 
#define XCVR_GPIO_BASE                      0x500
#define XCVR_GPIO_PHY_RESET_BIT             28
#define XCVR_GPIO_MDIO_DATA_BIT             1 // 33 % 32
#define XCVR_GPIO_MDIO_CLK_BIT              2 // 34 % 32
#define XCVR_GPIO_IO_SEL_OUTPUT_VAL         0
#define XCVR_GPIO_IO_SEL_INPUT_VAL          1
 
 
// Not needed since the gpio used for phy reset is unmuxed.
#define XCVR_PHY_RESET_GP_USE_SEL1_REG      0x0
 
// Set the bit to 1 for output
#define XCVR_PHY_RESET_GP_IO_SEL1_REG       0x4
 
// Set to 1 to drive high or 1 to drive low
#define XCVR_PHY_RESET_GP_LVL1_REG          0x0C
 
// Not needed since the gpio used for mdio are unmuxed.
#define XCVR_MDIO_GP_USE_SEL2_REG           0x30
 
// Set the bit to 1 for output
#define XCVR_MDIO_GP_IO_SEL2_REG            0x34
 
// Set to 1 to drive high or 1 to drive low
#define XCVR_MDIO_GP_LVL2_REG               0x38
 
typedef struct gpio_pin_ {
    int mdc_pin;
    int mdio_pin;
} gpio_pin_t;

static inline void
mdio_lo (gpio_pin_t *bus)
{
    ppc_gpio_clr_bit(2/*XXX*/, GPIO_REG_DATA, bus->mdio_pin);
}
 
static inline void
mdio_hi (gpio_pin_t *bus)
{
    ppc_gpio_set_bit(2/*XXX*/, GPIO_REG_DATA, bus->mdio_pin);
}
 
static inline void
mdc_lo (gpio_pin_t *bus)
{
    ppc_gpio_clr_bit(2/*XXX*/, GPIO_REG_DATA, bus->mdc_pin);
}
 
static inline void
mdc_hi (gpio_pin_t *bus)
{
    ppc_gpio_set_bit(2/*XXX*/, GPIO_REG_DATA, bus->mdc_pin);
}
 
static inline void
mdio_active (gpio_pin_t *bus)
{
#if 0
    u_int32_t io_sel;
    
    // Set to 0 for output, 1 for input
    io_sel = inl(XCVR_GPIO_BASE + XCVR_MDIO_GP_IO_SEL2_REG);
    io_sel = io_sel & ~((1 << bus->mdio_pin) | (1<< bus->mdc_pin));
    outl(io_sel, XCVR_GPIO_BASE + XCVR_MDIO_GP_IO_SEL2_REG);
#endif
}
 
static inline void
mdio_tristate (gpio_pin_t *bus)
{
#if 0
    u_int32_t io_sel;
    
    // Set to 0 for output, 1 for input
    io_sel = inl(XCVR_GPIO_BASE + XCVR_MDIO_GP_IO_SEL2_REG);
    io_sel = io_sel | (1 << bus->mdio_pin);
    outl(io_sel, XCVR_GPIO_BASE + XCVR_MDIO_GP_IO_SEL2_REG);
#else
    ppc_gpio_set_bit(2/*XXX*/, GPIO_REG_DATA, bus->mdio_pin);
#endif
}
 
static inline int
gpio_mdio_read (gpio_pin_t *bus)
{
    int err;
    uint8_t bit;
     
//    ppc_gpio_set_bit(2/*XXX*/, GPIO_REG_DATA, bus->mdc_pin);
    err = ppc_gpio_get_bit (2/*XXX*/, GPIO_REG_DATA, bus->mdio_pin, &bit);
    if (err) {
        printf("%s,%d: Error %d\n", __func__, __LINE__, err);
        return (-err);
    }
    return (bit);
}

// Prototypes
void xcvr_gpio_phy_reset(void);
void xcvr_gpio_mdio_write(gpio_pin_t *bus, u_int32_t prtad, u_int32_t devad,
                          u_int32_t regad, u_int16_t regval);
void xcvr_gpio_mdio_read(gpio_pin_t *bus, u_int32_t prtad, u_int32_t devad,
                         u_int32_t regad, u_int16_t *regval);
void xcvr_gpio_fcot_info_read(gpio_pin_t *bus, u_int32_t prtad,
                              u_int8_t i2c_addr, u_int32_t start_addr,
                              u_int32_t num_bytes, u_int8_t *buf);
 
#endif /* __XCVR_GPIO_H__ */
 
/* end of file */
