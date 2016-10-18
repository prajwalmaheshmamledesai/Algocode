#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include "bcm8754_lib.h"
#include "bcm8754.h"

static int bcm8754_pcs_loopback_print (int fd, uint8_t mdio_addr, 
		uint8_t port_addr, uint8_t *lpbk)
{
    return(bcm8754_sfi_loopback_get(fd, mdio_addr, port_addr,
                                BCM8754_PMA_PMD_ADDR, BCM8754_PMD_CTRL,
                                BCM8754_PMD_CTRL_FLD_PMD_LPBK, lpbk));
}

static int bcm8754_xfi_line_loopback_print (int fd, uint8_t mdio_addr, 
		uint8_t port_addr, uint8_t *lpbk)
{
    return(bcm8754_xfi_loopback_get(fd, mdio_addr, port_addr,
                                BCM8754_PMA_PMD_ADDR, BCM8754_PMD_CTRL,
                                BCM8754_PMD_CTRL_FLD_PMD_LPBK, lpbk));
}

static int bcm8754_sfi_line_loopback_print (int fd, uint8_t mdio_addr, 
		uint8_t port_addr, uint8_t *lpbk)
{
    return(bcm8754_sfi_loopback_get(fd, mdio_addr, port_addr,
                   BCM8754_PMA_PMD_ADDR, BCM8754_UD_PMD_TEST_CTRL,
                   BCM8754_UD_PMD_TEST_CTRL_FLD_PMD_PMA_LINELPBK, lpbk));
}

static int bcm8754_xfi_loopback_print (int fd, uint8_t mdio_addr, 
		uint8_t port_addr, uint8_t *lpbk)
{
    return(bcm8754_xfi_loopback_get(fd, mdio_addr, port_addr,
                   BCM8754_PMA_PMD_ADDR, BCM8754_UD_PMD_TEST_CTRL,
                   BCM8754_UD_PMD_TEST_CTRL_FLD_PMD_PMA_LINELPBK, lpbk));
}

int bcmphy_lpbk_print(int fd, uint8_t mdio_addr, uint8_t port_addr, uint8_t hdr)
{
	int rc = 0;
	uint8_t lpbk;

	if (hdr) {	
		printf("\t\tsfi-pcs\txfi-line\tsfi-line\txfi\n");
		printf("\t\t=============================================\n");
	}

	printf("Port(0x%X) \t", port_addr);
	rc = bcm8754_pcs_loopback_print(fd, mdio_addr, port_addr, &lpbk);
        printf("%s\t", rc? "-" : (lpbk ? "On" : "Off"));

	rc = bcm8754_xfi_line_loopback_print(fd, mdio_addr, port_addr, &lpbk);
        printf("%s\t", rc? "-" : (lpbk ? "On" : "Off"));

	rc = bcm8754_sfi_line_loopback_print(fd, mdio_addr, port_addr, &lpbk);
        printf("%s\t", rc? "-" : (lpbk ? "On" : "Off"));

	rc = bcm8754_xfi_loopback_print(fd, mdio_addr, port_addr, &lpbk);
        printf("%s\t", rc? "-" : (lpbk ? "On" : "Off"));

	printf("\n");
	return (rc);
}

int bcmphy_link_status_print(int fd, uint8_t mdio_addr, uint8_t port_addr, uint8_t hdr)
{
	int rc = 0;
	uint8_t link;

	if (hdr) {	
		printf("\t\txfi-pcs\tsfi-pcs\txfi-pmd\tsfi-pmd\n");
		printf("\t\t=============================================\n");
	}

	printf("Port(0x%X)\t", port_addr);

	rc = bcmphy_xfi_pcs_link_status_get(fd, mdio_addr, port_addr, &link);
	printf("%s\t", rc? " - " : (link ? "On" : "Off"));

	rc = bcmphy_sfi_pcs_link_status_get(fd, mdio_addr, port_addr, &link);
	printf("%s\t", rc? " - " : (link ? "On" : "Off"));

	rc = bcmphy_xfi_pmd_link_status_get(fd, mdio_addr, port_addr, &link);
	printf("%s\t", rc? " - " : (link ? "On" : "Off"));

	rc = bcmphy_sfi_pmd_link_status_get(fd, mdio_addr, port_addr, &link);
	printf("%s\t", rc? " - " : (link ? "On" : "Off"));

	printf("\n");
	return (0);
}
