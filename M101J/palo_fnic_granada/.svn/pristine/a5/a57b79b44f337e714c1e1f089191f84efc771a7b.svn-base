#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/types.h>
#include "bcm8726.h"
#include "bcm8726_lib.h"

#define UC_REF_CLK_SEL_26MHZ    0 
#define UC_REF_CLK_SEL_42MHZ    1 
#define UC_REF_CLK_SEL_78MHZ    2 

int bcm8727_eeprom_test (int fd, uint8_t mdio_addr, uint8_t port_addr, int *prom_err) 
{
	int rc = 0;
	uint16_t data, chip_rev, checksum;

	rc = bcm8726_rd(fd, mdio_addr, port_addr, BCM8726_PMA_PMD_ADDR,
				BCM8726_UD_CHIP_REV, &chip_rev);
	if (rc) {
		printf("Error: Rd BCM8726_UD_CHIP_REV\n"); 
		return (rc);
	}

	rc = bcm8726_rd(fd, mdio_addr, port_addr, BCM8726_PMA_PMD_ADDR,
				BCM8726_EDC_REVISION, &data);

	if (rc) {
		printf("Error: Rd BCM8726_EDC_REVISION\n"); 
		return (rc);
	}

	rc = bcm8726_rd(fd, mdio_addr, port_addr, BCM8726_PMA_PMD_ADDR,
			BCM8726_EDC_GP_LOAD, &checksum);
	if (rc) {
		printf("Error: Rd BCM8726_EDC_GP_DATA\n"); 
		return (rc);
	}

	*prom_err = 0;
	if (chip_rev == 0x7) {
		printf("  BCM8727 Rev-C Ucode Version 0x%X (Checksum = 0x%X)\n", data, checksum);
		*prom_err = 
		   (((data == BCM8727_EDC_REV_C) && (checksum == BCM8727_EEPROM_CHKSUM_INT)) ||
		    ((data == BCM8727_EDC_REV_C1) && (checksum == BCM8727_EEPROM_CHKSUM_EXT)) ||
		    ((data == BCM8727_EDC_REV_C_EXT) && (checksum == BCM8727_EEPROM_CHKSUM_EXT)))? 
				0 : 1;
        } else if (chip_rev == 0x03) {
		printf("  BCM8727 Rev-B Ucode Version 0x%X (Checksum = 0x%X)\n", data, checksum);
		if (((data != BCM8727_EDC_REV_B) && (data != BCM8727_EDC_REV_B0)) 
			|| (checksum != BCM8727_EEPROM_CHKSUM_EXT))
			*prom_err = 1;
        } else {
		printf("Error: Invalid Chip Rev %d\n", chip_rev);
		*prom_err =  1;
        }

	return (rc);
}

// 0xCA85 = 0x0001   - set serial boot control for external load
// 0xCA10 = 0x000C   - Micro Reset.
// 0xCA10 = 0x0008   - micro reboot.
// 0xCA10 = 0x0082   - set soft reset.
// 0xCA10 = 0x0080   - clear soft reset.
// 0xCA85 = 0x0000   - Disable serial boot control 
int bcm8727_eeprom_init (int fd, uint8_t mdio_addr, uint8_t port_addr) 
{
	int rc = 0, cnt, count; //timeout = 1;
	uint16_t data, rev, checksum;
	uint16_t addr_data[][2] = {
        {0xCA10, 0x0001},
        {0xCA10, 0x008C},
        {0xCA85, 0x0001},
        {0xCA10, 0x018A},
        {0xCA10, 0x0188},
        {0xCA85, 0x0000},
        };

	count = sizeof(addr_data)/(2 * sizeof(uint16_t));

	rc = bcm8726_rd(fd, mdio_addr, port_addr, BCM8726_PMA_PMD_ADDR,
				BCM8726_UD_CHIP_REV, &rev);
	if (rc) {
		printf("Error: Rd BCM8726__UD_CHIP_REV\n"); 
		return (rc);
	}

	if (rev == 7) {		// Rev C does not need external Load.	
		checksum = BCM8727_EEPROM_CHKSUM_INT;
	} else {
		for (cnt = 0; cnt < count; cnt++) {
			if (((cnt+1) == count)) {
				printf("  Sleep 100ms\n");
				usleep(200 * 1000);
			}
			rc = bcm8726_wr(fd, mdio_addr, port_addr, BCM8726_PMA_PMD_ADDR,
			     	addr_data[cnt][0], addr_data[cnt][1]); 
			if (rc) {
				printf("Error: bcm8726 write (rc=%d)\n", rc); 
				return (rc);
			}
		}
		checksum = BCM8727_EEPROM_CHKSUM_EXT;
	}

	// Poll to check if the EEPROM Load is complete.
	rc = bcm8726_rd(fd, mdio_addr, port_addr, BCM8726_PMA_PMD_ADDR,
			BCM8726_EDC_GP_LOAD, &data);
	if (rc) {
		printf("Error: Rd BCM8726_EDC_GP_DATA\n"); 
		return (rc);
	}
	if (data != checksum) {
		printf(" Error: Checksum Mismatch - Exp=0x%X Rd=0x%X\n",
			checksum, data);
		return (-1);
	}
	return (0);
}
