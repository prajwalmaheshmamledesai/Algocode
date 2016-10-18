#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>

extern uint16_t palo_mdio_read(uint32_t palo, uint32_t prtad, uint32_t devad, uint32_t addr);
extern void palo_mdio_write(uint32_t palo, uint32_t prtad, uint32_t devad, uint32_t addr, uint16_t val);

int bcm8726_open()
{
	return (1);
}

void bcm8726_close()
{
}

int bcm8726_rd (int fd, uint8_t palo, uint8_t phy,
	    uint8_t device_class, uint16_t reg_addr, uint16_t *data)
{
	int rc = 0;
	if (!device_class) {
		printf("Error: Invalid device class (%d) \n", device_class);
		return (-1);
	}
	*data = palo_mdio_read(palo, phy, device_class, reg_addr);
	return (rc);
}

int bcm8726_wr (int fd, uint8_t palo, uint8_t phy,
	    uint8_t device_class, uint16_t reg_addr, uint16_t data)
{
	if (!device_class) {
		printf("Error: Invalid device class (%d) \n", device_class);
		return (-1);
	}
	palo_mdio_write(palo, phy, device_class, reg_addr, data);
        return (0);
}
