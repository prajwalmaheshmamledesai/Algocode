#include <stdio.h>
#include <sys/types.h>
#include "mdio_lib.h"

extern int asic_mdio_read (uint8_t  inst, uint32_t mdio_num,
                   uint32_t port_addr, uint32_t dev_class,
                   uint32_t reg_addr, uint32_t *reg_data);
extern int asic_mdio_write (uint8_t  inst, uint32_t mdio_num,
                   uint32_t port_addr, uint32_t dev_class,
                   uint32_t reg_addr, uint32_t reg_data);

static int mdio_read_thru_woodside (int fd, uint8_t mdio_addr,
                uint8_t port_addr, uint8_t device_class, uint16_t reg_addr,
                uint16_t *data) 
{
	int rc = 0;
	uint32_t reg_data;
	rc = asic_mdio_read (fd, mdio_addr, port_addr, 
		device_class, reg_addr, &reg_data);

	*data = (uint16_t) reg_data;
	return (rc);
}

static int mdio_write_thru_woodside (int fd, uint8_t mdio_addr,
                uint8_t port_addr, uint8_t device_class, uint16_t reg_addr,
                uint16_t data) 
{
	return (asic_mdio_write (fd, mdio_addr, port_addr, 
		device_class, reg_addr, (uint32_t)data));
}

int bcm8754_rd (int fd, uint8_t mdio_addr, uint8_t port_addr,
	    uint8_t device_class, uint16_t reg_addr, uint16_t *data)
{
	int rc = 0;
	if (!device_class) {
		printf("Error: Invalid device class (%d) \n", device_class);
		return (-1);
	}
	rc = mdio_read_thru_woodside(0, mdio_addr, port_addr, device_class,
                  reg_addr, data);
	return (rc);
}

int bcm8754_wr (int fd, uint8_t mdio_addr, uint8_t port_addr,
	    uint8_t device_class, uint16_t reg_addr, uint16_t data)
{
	if (!device_class) {
		printf("Error: Invalid device class (%d) \n", device_class);
		return (-1);
	}
	return (mdio_write_thru_woodside(0, mdio_addr, port_addr, 
                                         device_class, reg_addr, data));
}

int phy_eeprom_rd (int fd, uint8_t mdio_addr, uint8_t port_addr,
                   uint8_t dev_class, uint16_t reg_addr, uint16_t *data)
{
    int rc = 0;
    rc = bcm8754_rd(fd, mdio_addr, port_addr, dev_class, reg_addr, data);
/*    printf("R c %d m %04x p %04x r %04x d %04x\n",
           dev_class, mdio_addr, port_addr, reg_addr, *data); */
    return (rc);
}

int phy_eeprom_wr (int fd, uint8_t mdio_addr, uint8_t port_addr,
                   uint8_t dev_class, uint16_t reg_addr, uint16_t data)
{
    int rc = 0;
/*    printf("W c %d m %04x p %04x r %04x d %04x\n",
           dev_class, mdio_addr, port_addr, reg_addr, data); */
    rc = bcm8754_wr(fd, mdio_addr, port_addr, dev_class, reg_addr, data);
    return (rc);
}
