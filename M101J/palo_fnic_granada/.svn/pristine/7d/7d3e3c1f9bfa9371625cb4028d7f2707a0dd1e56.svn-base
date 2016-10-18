#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/types.h>
#include "bcm8726_lib.h"
#include "bcm8726.h"

static int bcm8726_loopback_get (int fd, uint8_t mdio_addr, uint8_t port_addr,
		uint8_t device_class, uint8_t addr, uint8_t *lpbk)
{
	int rc;
	uint16_t data;

	rc = bcm8726_rd(fd, mdio_addr, port_addr, device_class,
		addr, &data); 
	if (rc) {
		printf("Error: Failed to read PCS_CTRL for PHY%d\n",
				port_addr);
		return (rc);
	}

	*lpbk  = (data & BCM8726_PCS_CTRL_FLD_LOOPBACK) ? 1 : 0;
	return (rc);
}

static int bcm8726_pcs_loopback_print (int fd, uint8_t mdio_addr, 
		uint8_t port_addr, uint8_t *lpbk)
{
	return(bcm8726_loopback_get (fd, mdio_addr, port_addr,
                BCM8726_PCS_ADDR, BCM8726_PCS_CTRL, lpbk));
}

static int bcmphy_xgxs_loopback_print (int fd, uint8_t mdio_addr, uint8_t port_addr,
		uint8_t* lpbk)
{
	return(bcm8726_loopback_get (fd, mdio_addr, port_addr,
                BCM8726_XGXS_ADDR, BCM8726_PHY_XS_CTRL, lpbk));
}

static int bcmphy_pma_pmd_loopback_print (int fd, uint8_t mdio_addr, 
			uint8_t port_addr, uint8_t *lpbk)
{
	int rc;
	uint16_t data;

	rc = bcm8726_rd(fd, mdio_addr, port_addr, BCM8726_PMA_PMD_ADDR,
		BCM8726_PMD_CTRL, &data); 
	if (rc) {
		printf("Error: Failed to read PCS_CTRL for PHY%d\n",
				port_addr);
		return (rc);
	}

	*lpbk = (data & BCM8726_PMD_CTRL_FLD_PMD_LPBK) ? 1 : 0;
	return (rc);
}


static int bcmphy_xaui_loopback_print (int fd, uint8_t mdio_addr, 
			uint8_t port_addr, uint8_t *lpbk)
{
	uint16_t mode_ctrl, lane_ctrl;
	int	 rc = 0;

	rc = bcm8726_rd(fd, mdio_addr, port_addr, BCM8726_XGXS_ADDR,
		BCM8726_XGXS_MODE_CTRL, &mode_ctrl); 
	if (rc) {
		printf("Error: Failed to read MODE_CTRL for PHY%d\n",
				port_addr);
		return (rc);
	}

	rc = bcm8726_rd(fd, mdio_addr, port_addr, BCM8726_XGXS_ADDR,
		BCM8726_XGXS_LANE_CTRL, &lane_ctrl); 
	if (rc) {
		printf("Error: Failed to read LANE_CTRL for PHY%d\n",
				port_addr);
		return (rc);
	}


	mode_ctrl &= ~BCM8726_XGXS_MODE_CTRL_FLD_MODE_MASK; 
	*lpbk = ((mode_ctrl == BCM8726_XGXS_MODE_CTRL_FLD_MODE_INDEPENDANT_LANE) &&
		 (lane_ctrl & BCM8726_XGXS_LANE_CTRL_FLD_LANE_LPBK_MASK)) ? 1 : 0;

	return (rc);
}


static int bcmphy_xaui_system_loopback_print (int fd, uint8_t mdio_addr, 
			uint8_t port_addr, uint8_t *lpbk)
{
	uint16_t mode_ctrl;
	int	 rc = 0;

	rc = bcm8726_rd(fd, mdio_addr, port_addr, BCM8726_XGXS_ADDR,
		BCM8726_XGXS_MODE_CTRL, &mode_ctrl); 
	if (rc) {
		printf("Error: Failed to read MODE_CTRL for PHY%d\n",
				port_addr);
		return (rc);
	}

	mode_ctrl &= ~BCM8726_XGXS_MODE_CTRL_FLD_MODE_MASK; 
	*lpbk = (mode_ctrl == BCM8726_XGXS_MODE_CTRL_FLD_MODE_LPBK) ? 1 : 0;

	return (rc);
}


static int bcmphy_digital_lane_loopback_print (int fd, uint8_t mdio_addr, 
			uint8_t port_addr, uint8_t *lpbk)
{
	uint16_t data;
	int	 rc = 0;

	rc = bcm8726_rd(fd, mdio_addr, port_addr, BCM8726_PMA_PMD_ADDR,
		BCM8726_UD_TEST_CTRL, &data); 
	if (rc) {
		printf("Error: Failed to read UD_TEST_CTRL for PHY%d\n",
				port_addr);
		return (rc);
	}

	*lpbk = (data & BCM8726_UD_TEST_CTRL_FLD_DIG_LPBK) ? 1 : 0;

	return (rc);
}

int bcmphy_lpbk_print(int fd, uint8_t mdio_addr, uint8_t port_addr, uint8_t hdr)
{
	int rc = 0;
	uint8_t lpbk;

	if (hdr) {	
		printf("\t\txaui\tpcs\tpma\tsys\txgxs\tline\n");
		printf("\t\t=============================================\n");
	}

	printf("Port(0x%X) \t", port_addr);
	rc = bcmphy_xaui_loopback_print (fd, mdio_addr, port_addr, &lpbk);
        printf("%s\t", rc? "-" : (lpbk ? "On" : "Off"));

	rc = bcm8726_pcs_loopback_print (fd, mdio_addr, port_addr, &lpbk);
        printf("%s\t", rc? "-" : (lpbk ? "On" : "Off"));

	rc = bcmphy_pma_pmd_loopback_print (fd, mdio_addr, port_addr, &lpbk);
        printf("%s\t", rc? "-" : (lpbk ? "On" : "Off"));

	rc = bcmphy_xaui_system_loopback_print (fd, mdio_addr, port_addr, &lpbk);
        printf("%s\t", rc? "-" : (lpbk ? "On" : "Off"));

	rc = bcmphy_xgxs_loopback_print (fd, mdio_addr, port_addr, &lpbk);
        printf("%s\t", rc? "-" : (lpbk ? "On" : "Off"));

	rc = bcmphy_digital_lane_loopback_print (fd, mdio_addr, port_addr, &lpbk);
        printf("%s\n", rc? "-" : (lpbk ? "On" : "Off"));

	return (rc);
}


int bcmphy_xgxs_get_lane_status (int fd, uint8_t mdio_addr, 
		uint8_t port_addr, uint8_t *lane_status)
{
	int rc;
	uint16_t data;

	rc = bcm8726_rd(fd, mdio_addr, port_addr, BCM8726_XGXS_ADDR,
		BCM8726_PHY_XS_XGXS_LANE_STATUS, &data); 
	if (rc) {
		printf("Error: Failed to read 0x%X for PHY%d\n",
				BCM8726_PHY_XS_XGXS_LANE_STATUS, port_addr);
		return (rc);
	}

	*lane_status  = (data & 0x0F);
	return (rc);
}

static int bcm8726_link_status_get (int fd, uint8_t mdio_addr, uint8_t port_addr,
		uint8_t device_class, uint8_t addr, uint8_t *link)
{
	int rc;
	uint16_t data;
	uint8_t  lane_status = 0x00;

	rc = bcm8726_rd(fd, mdio_addr, port_addr, device_class,
		addr, &data); 
	if (rc) {
		printf("Error: Failed to read 0x%X for PHY%d\n",
				addr, port_addr);
		return (rc);
	}
	rc = bcm8726_rd(fd, mdio_addr, port_addr, device_class,
		addr, &data); 
	if (rc) {
		printf("Error: Failed to read 0x%X for PHY%d\n",
				addr, port_addr);
		return (rc);
	}

	*link  = (data & BCM8726_PCS_STATUS_FLD_RX_LINK_STATUS) ? 1 : 0;

	if (*link == 0) {
		rc = bcmphy_xgxs_get_lane_status (fd, mdio_addr, 
			port_addr, &lane_status);
		if (!rc) {
//			printf(" MdioAddr(0x%X) PortAddr(0x%X), LaneStatus = 0x%X\n", 
//			mdio_addr, port_addr, lane_status);
		}
	}
	return (rc);
}


int bcmphy_pcs_link_status_get (int fd, uint8_t mdio_addr, 
		uint8_t port_addr, uint8_t *link)
{
	return(bcm8726_link_status_get (fd, mdio_addr, port_addr,
                BCM8726_PCS_ADDR, BCM8726_PCS_STATUS, link));
}

static int bcmphy_xgxs_link_status_get (int fd, uint8_t mdio_addr, uint8_t port_addr,
		uint8_t* link)
{
	return(bcm8726_link_status_get (fd, mdio_addr, port_addr,
                BCM8726_XGXS_ADDR, BCM8726_PHY_XS_STATUS, link));
}

static int bcmphy_pma_pmd_link_status_get (int fd, uint8_t mdio_addr, 
			uint8_t port_addr, uint8_t *link)
{
	return(bcm8726_link_status_get (fd, mdio_addr, port_addr,
                BCM8726_PMA_PMD_ADDR, BCM8726_PMD_STATUS, link));
}


static int bcmphy_xaui_link_status_get (int fd, uint8_t mdio_addr, 
			uint8_t port_addr, uint8_t *link)
{
	return (-1);
}


static int bcmphy_xaui_system_link_status_get (int fd, uint8_t mdio_addr, 
			uint8_t port_addr, uint8_t *link)
{
	return (-1);
}


static int bcmphy_digital_lane_link_status_get (int fd, uint8_t mdio_addr, 
			uint8_t port_addr, uint8_t *link)
{
	return (-1);
}

int bcmphy_link_status_print(int fd, uint8_t mdio_addr, uint8_t port_addr, uint8_t hdr)
{
	int rc = 0;
	uint8_t link;

	if (hdr) {	
		printf("\t\txaui\tpcs\tpma\tsys\txgxs\tline\n");
		printf("\t\t=============================================\n");
	}

	printf("Port(0x%X)\t", port_addr);

	rc = bcmphy_xaui_link_status_get (fd, mdio_addr, port_addr, &link);
	printf("%s\t", rc? " - " : (link ? "On" : "Off"));

	rc = bcmphy_pcs_link_status_get (fd, mdio_addr, port_addr, &link);
	printf("%s\t", rc? " - " : (link ? "On" : "Off"));

	rc = bcmphy_pma_pmd_link_status_get (fd, mdio_addr, port_addr, &link);
	printf("%s\t", rc? " - " : (link ? "On" : "Off"));

	rc = bcmphy_xaui_system_link_status_get (fd, mdio_addr, port_addr, &link);
	printf("%s\t", rc? " - " : (link ? "On" : "Off"));

	rc = bcmphy_xgxs_link_status_get (fd, mdio_addr, port_addr, &link);
	printf("%s\t", rc? " - " : (link ? "On" : "Off"));

	rc = bcmphy_digital_lane_link_status_get (fd, mdio_addr, port_addr, &link);
	printf("%s\n", rc? " - " : (link ? "On" : "Off"));

	return (0);
}
