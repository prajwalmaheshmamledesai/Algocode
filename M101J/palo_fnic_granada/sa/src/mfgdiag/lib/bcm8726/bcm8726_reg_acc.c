#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include "mdio_lib.h"

int bcm8726_rd (int fd, uint8_t mdio_addr, uint8_t port_addr,
	    uint8_t device_class, uint16_t reg_addr, uint16_t *data)
{
	int rc = 0;
	if (!device_class) {
		printf("Error: Invalid device class (%d) \n", device_class);
		return (-1);
	}
	rc = mdio_c45_read(fd, mdio_addr, port_addr, device_class,
                  reg_addr, data);
	return (rc);
}

int bcm8726_wr (int fd, uint8_t mdio_addr, uint8_t port_addr,
	    uint8_t device_class, uint16_t reg_addr, uint16_t data)
{
	if (!device_class) {
		printf("Error: Invalid device class (%d) \n", device_class);
		return (-1);
	}
	return (mdio_c45_write(fd, mdio_addr, port_addr, device_class,
                  reg_addr, data));
}
