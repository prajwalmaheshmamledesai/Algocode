#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/types.h>
#include "bcm8726_lib.h"
#include "bcm8726.h"

int bcmphy_reset (int fd, uint8_t mdio_addr, uint8_t port_addr)
{
	int rc = 0;
	uint16_t data;

	rc = bcm8726_rd(fd, mdio_addr, port_addr, BCM8726_PMA_PMD_ADDR,
		BCM8726_PMD_CTRL, &data); 
	if (rc) {
		printf("Error: Failed to read PMD_CTRL for PHY%d\n",
				port_addr);
		return (rc);
	}
	data |= BCM8726_PMD_CTRL_FLD_RESET;
	rc = bcm8726_wr(fd, mdio_addr, port_addr, BCM8726_PMA_PMD_ADDR,
		BCM8726_PMD_CTRL, data); 
	if (rc) {
		printf("Error: Failed to write PMD_CTRL for PHY%d\n",
				port_addr);
		return (rc);
	}
	return (rc);
}

static int bcm8726_loopback (int fd, uint8_t mdio_addr, uint8_t port_addr,
		uint8_t device_class, uint8_t addr, uint8_t ena, uint8_t rst)
{
	int rc;
	uint16_t wdata, data;

	rc = bcm8726_rd(fd, mdio_addr, port_addr, device_class,
		addr, &data); 
	if (rc) {
		printf("Error: Failed to read PCS_CTRL for PHY%d\n",
				port_addr);
		return (rc);
	}

	if (ena) {
		// Enable Loopback Mode.
		wdata = data | BCM8726_PCS_CTRL_FLD_LOOPBACK;	
		rc = bcm8726_wr(fd, mdio_addr, port_addr, device_class,
					addr, wdata); 
		if (rc) {
			printf("Error: Loopback Enable PHY%d\n",
					port_addr);
			return (rc);
		}


	} else {
		// Disable Loopback.
		wdata = data & ~BCM8726_PCS_CTRL_FLD_LOOPBACK;	
		rc = bcm8726_wr(fd, mdio_addr, port_addr, device_class,
					addr, wdata); 
		if (rc) {
			printf("Error: Loopback disable PHY%d\n",
					port_addr);
			return (rc);
		}
	}

	if (rst) {
		// reset.
		wdata |= BCM8726_PCS_CTRL_FLD_RESET;
		rc = bcm8726_wr(fd, mdio_addr, port_addr, device_class,
					addr, wdata); 
		if (rc) {
			printf("Error: Setting reset on PHY%d\n",
					port_addr);
			return (rc);
		}

		//unreset.
		wdata &= ~BCM8726_PCS_CTRL_FLD_RESET;
		rc = bcm8726_wr(fd, mdio_addr, port_addr, device_class,
					addr, wdata); 
		if (rc) {
			printf("Error: Setting reset on PHY%d\n",
					port_addr);
			return (rc);
		}
	}
	return (rc);
}

int bcm8726_reset_toggle (int fd, uint8_t mdio_addr, uint8_t port_addr,
		uint8_t device_class, uint8_t addr, uint8_t ena)
{
	int rc;
	uint16_t wdata, data;

	rc = bcm8726_rd(fd, mdio_addr, port_addr, device_class,
		addr, &data); 
	if (rc) {
		printf("Error: Failed to read PCS_CTRL for PHY%d\n",
				port_addr);
		return (rc);
	}

	// Toggle RESET.,
	wdata = data | BCM8726_PCS_CTRL_FLD_RESET;
	rc = bcm8726_wr (fd, mdio_addr, port_addr, device_class,
			addr, wdata); 
	if (rc) {
		printf("Error: Failed to read PCS_CTRL for PHY%d\n",
				port_addr);
		return (rc);
	}

	wdata = data & ~BCM8726_PCS_CTRL_FLD_RESET;
	rc = bcm8726_wr (fd, mdio_addr, port_addr, device_class,
			addr, wdata); 
	if (rc) {
		printf("Error: Failed to read PCS_CTRL for PHY%d\n",
				port_addr);
		return (rc);
	}

	return (rc);
}

int bcmphy_pcs_loopback (int fd, uint8_t mdio_addr, uint8_t port_addr,
		uint8_t ena)
{
	int rc = 0;
	uint16_t data;

	rc =  bcm8726_rd(fd, mdio_addr, port_addr, 
			BCM8726_PMA_PMD_ADDR, 
			BCM8726_UD_PMD_DIGITAL_CTRL, &data); 
	if (rc) return (rc);

	// Disable Rx Clock.
	data |= BCM8726_UD_PMD_DIGITAL_CTRL_FLD_RX_PWR_DOWN;
	rc =  bcm8726_wr(fd, mdio_addr, port_addr, 
			BCM8726_PMA_PMD_ADDR, 
			BCM8726_UD_PMD_DIGITAL_CTRL, data); 
	if (rc) return (rc);

	rc = bcm8726_loopback (fd, mdio_addr, port_addr,
                BCM8726_PCS_ADDR, BCM8726_PCS_CTRL, ena, 0);
	if (rc) return (rc);

	// Enable Rx Clock.
	data &= ~BCM8726_UD_PMD_DIGITAL_CTRL_FLD_RX_PWR_DOWN;
	rc =  bcm8726_wr(fd, mdio_addr, port_addr, 
			BCM8726_PMA_PMD_ADDR, 
			BCM8726_UD_PMD_DIGITAL_CTRL, data); 
	return (rc);
}

int bcmphy_xgxs_loopback (int fd, uint8_t mdio_addr, uint8_t port_addr,
		uint8_t ena)
{
	return(bcm8726_loopback (fd, mdio_addr, port_addr,
                BCM8726_XGXS_ADDR, BCM8726_PHY_XS_CTRL, ena, 0));
}

int bcmphy_pma_pmd_loopback (int fd, uint8_t mdio_addr, uint8_t port_addr, uint8_t ena)
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

	if (ena) {
		// Enable Loopback Mode.
		data |= BCM8726_PMD_CTRL_FLD_PMD_LPBK;	
		rc = bcm8726_wr(fd, mdio_addr, port_addr, BCM8726_PMA_PMD_ADDR,
					BCM8726_PMD_CTRL, data); 
		if (rc) {
			printf("Error: Loopback Enable PHY%d data(0x%X)\n",
					port_addr, data);
			return (rc);
		}


	} else {
		// Disable Loopback.
		data &= ~BCM8726_PMD_CTRL_FLD_PMD_LPBK;	
		rc = bcm8726_wr(fd, mdio_addr, port_addr, BCM8726_PMA_PMD_ADDR,
					BCM8726_PMD_CTRL, data); 
		if (rc) {
			printf("Error: Loopback disable PHY%d data(0x%X)\n",
					port_addr, data);
			return (rc);
		}
	}
	return (rc);
}


int bcmphy_xaui_loopback (int fd, uint8_t mdio_addr, uint8_t port_addr, uint8_t ena)
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


	if (ena) {
		// Enable independant lane loopback mode.
		mode_ctrl &= ~BCM8726_XGXS_MODE_CTRL_FLD_MODE_MASK; 
		mode_ctrl |= BCM8726_XGXS_MODE_CTRL_FLD_MODE_INDEPENDANT_LANE;

		// Enable independant lane loopback.
		lane_ctrl |= BCM8726_XGXS_LANE_CTRL_FLD_LANE_LPBK_MASK;

	} else {
		// Enable independant lane loopback mode.
		mode_ctrl &= ~BCM8726_XGXS_MODE_CTRL_FLD_MODE_MASK; 
		mode_ctrl |= BCM8726_XGXS_MODE_CTRL_FLD_MODE_FUNC;

		// Enable independant lane loopback.
		lane_ctrl &= ~BCM8726_XGXS_LANE_CTRL_FLD_LANE_LPBK_MASK;
	}

	rc = bcm8726_wr(fd, mdio_addr, port_addr, BCM8726_XGXS_ADDR,
			BCM8726_XGXS_MODE_CTRL, mode_ctrl); 
	if (rc) {
		printf("Error: Failed to read MODE_CTRL for PHY%d\n",
				port_addr);
		return (rc);
	}

	rc = bcm8726_wr(fd, mdio_addr, port_addr, BCM8726_XGXS_ADDR,
		BCM8726_XGXS_LANE_CTRL, lane_ctrl); 
	if (rc) {
		printf("Error: Failed to read LANE_CTRL for PHY%d\n",
			port_addr);
		return (rc);
	}
	return (rc);
}


int bcmphy_xaui_system_loopback (int fd, uint8_t mdio_addr, uint8_t port_addr, uint8_t ena)
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

	if (ena) {
		// Enable independant lane loopback mode.
		mode_ctrl &= ~BCM8726_XGXS_MODE_CTRL_FLD_MODE_MASK; 
		mode_ctrl |= BCM8726_XGXS_MODE_CTRL_FLD_MODE_LPBK;
	} else {
		// Enable independant lane loopback mode.
		mode_ctrl &= ~BCM8726_XGXS_MODE_CTRL_FLD_MODE_MASK; 
		mode_ctrl |= BCM8726_XGXS_MODE_CTRL_FLD_MODE_FUNC;
	}

	rc = bcm8726_wr(fd, mdio_addr, port_addr, BCM8726_XGXS_ADDR,
			BCM8726_XGXS_MODE_CTRL, mode_ctrl); 
	if (rc) {
		printf("Error: Failed to read MODE_CTRL for PHY%d\n",
				port_addr);
		return (rc);
	}

	return (rc);
}


int bcmphy_digital_lane_looppback (int fd, uint8_t mdio_addr, uint8_t port_addr, uint8_t ena)
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

	if (ena) {
		// Enable PCS Digital Lane loopback 
		data |= BCM8726_UD_TEST_CTRL_FLD_DIG_LPBK;
	} else {
		// Enable  PCS Digital Lane loopbackindependant lane loopback mode.
		data &= ~BCM8726_UD_TEST_CTRL_FLD_DIG_LPBK; 
	}

	rc = bcm8726_wr(fd, mdio_addr, port_addr, BCM8726_PMA_PMD_ADDR,
			BCM8726_UD_TEST_CTRL, data); 
	if (rc) {
		printf("Error: Failed to read UD_TEST_CTRL for PHY%d\n",
				port_addr);
		return (rc);
	}

	return (rc);
}



int bcmphy_xgxs_link_status (int fd, uint8_t mdio_addr, uint8_t port_addr, uint8_t *link)
{
	uint16_t data;
	int	rc = 0;

	// Status needs to be read twice because of sticky bit..,
	rc = bcm8726_rd (fd, mdio_addr, port_addr,
                BCM8726_XGXS_ADDR, BCM8726_PHY_XS_STATUS, &data);
	if (rc) {
		printf("Error: %s:%d mdio_addr(0x%X) port_addr(0x%X)"
			"Class(%d) RegAddr(0x%X)\n", __func__, __LINE__,
			mdio_addr, port_addr, BCM8726_XGXS_ADDR,
			BCM8726_PHY_XS_STATUS); 
	}

	rc = bcm8726_rd (fd, mdio_addr, port_addr,
                BCM8726_XGXS_ADDR, BCM8726_PHY_XS_STATUS, &data);
	if (rc) {
		printf("Error: %s:%d mdio_addr(0x%X) port_addr(0x%X)"
			"Class(%d) RegAddr(0x%X)\n", __func__, __LINE__,
			mdio_addr, port_addr, BCM8726_XGXS_ADDR,
			BCM8726_PHY_XS_STATUS); 
	}

	*link = (data & BCM8726_PHY_XS_STATUS_FLD_TX_LINK_STATUS) ? 1 : 0;	

//	printf("[0x%X] : 0x%X (mask 0x%X)\n", BCM8726_PHY_XS_STATUS,
//			data, BCM8726_PHY_XS_STATUS_FLD_TX_LINK_STATUS);

	return (rc);
}



int bcm8726_sfp_ena (int fd, uint8_t mdio_addr, uint8_t port_addr, uint8_t ena)
{
	int rc = 0;
	return (rc);
}

int bcm8726_port_ena (int fd, uint8_t mdio_addr, uint8_t port_addr, 
			uint8_t ena)
{
	int rc = 0;
	uint16_t data;

#if 0
	rc = bcmphy_reset (fd, mdio_addr, port_addr);
	if (rc) {
		printf("Error: Failed to reset PHY%d\n", port_addr);
		return (rc);
	}
#endif
	rc = bcm8726_rd(fd, mdio_addr, port_addr, 
		BCM8726_PCS_ADDR, BCM8726_PCS_DEV_ID1, &data); 
	if (rc) {
		printf("Error: Failed to read PCS_DEV_ID1 for PHY%d\n",
				port_addr);
		return (rc);
	}

	if (data == BCM8726_PCS_DEV_ID1_DFLT) {
		data = BCM8726_UD_PMD_DIGITAL_CTRL_FLD_PCS_TX_INV_B;
		rc =  bcm8726_wr (fd, mdio_addr, port_addr, 
			BCM8726_PMA_PMD_ADDR, BCM8726_UD_PMD_DIGITAL_CTRL, data); 
	        if (rc) {
			printf("Error: read UD_PMD_DIGITAL_CTRL PHY%d\n",
                                port_addr);
			return (rc);
        	}
	}

	rc = bcm8726_sfp_ena(fd, mdio_addr, port_addr, ena);	
	if (rc) {
		printf("Error: SFP Enable for Phy%d\n", port_addr);
		return (rc);
	}
	return (rc);
}


int bcm8726_eeprom_load_test (int fd, uint8_t mdio_addr, uint8_t port_addr, 
		int *prom_err, int flag)
{
	int rc = 0;
	uint16_t data;

	*prom_err = 0x00;

	rc = bcm8726_rd(fd, mdio_addr, port_addr, 
		BCM8726_PMA_PMD_ADDR, BCM8726_EDC_GP_DATA, &data); 
	if (rc) {
		printf("Error: Failed to read EDC_GP_DATA for PHY%d\n",
				port_addr);
		return (rc);
	}

	if (data != BCM8726_EEPROM_SIG) {
		*prom_err = -1;
		if (flag) {
			printf("  EEPROM Load Failed [0x%X] : 0x%X\n",
				BCM8726_EDC_GP_DATA, data);
		}
		return (rc);
	} 

#if 0
	rc = bcm8726_rd(fd, mdio_addr, port_addr, 
			BCM8726_PMA_PMD_ADDR, 0xCA1C, &data); 
	if (rc) {
		printf("Error: Failed to read EDC_GP_DATA for PHY%d\n",
				port_addr);
		return (rc);
	}
	
	if (data != BCM8726_EEPROM_SIG1) {
		*prom_err = -1;
		if (flag) {
			printf("  EEPROM Load Failed [0xCA1C] : 0x%X\n", data);
		}
		return (rc);
	} 
#endif
	return (rc);	
}

int bcm8726_eeprom_check (int fd, uint8_t mdio_addr, uint8_t port_addr, 
		int *prom_err)
{
	return (bcm8726_eeprom_load_test (fd, mdio_addr, port_addr, prom_err, 0));
}

int bcm8726_eeprom_test (int fd, uint8_t mdio_addr, uint8_t port_addr, 
		int *prom_err)
{
	return (bcm8726_eeprom_load_test (fd, mdio_addr, port_addr, prom_err, 1));
}

int bcmphy_bist_test (int fd, uint8_t mdio_addr, uint8_t port_addr,
		uint16_t pkt_size, uint16_t pkt_cnt)
{
	int rc = 0;
	uint16_t data, pcs_data, xs_data; 
	int timeout;

	rc =  bcm8726_wr (fd, mdio_addr, port_addr, 
		BCM8726_PMA_PMD_ADDR, BCM8726_UD_BIST_PKT_SIZE, pkt_size); 
        if (rc) {
		printf("Error: Wr UD_BIST_PKT_SIZE  PHY%d\n",
                               port_addr);
		return (rc);
       	}

	rc =  bcm8726_wr (fd, mdio_addr, port_addr, 
		BCM8726_PMA_PMD_ADDR, BCM8726_UD_BIST_TX_CNT_CTRL, pkt_cnt); 
        if (rc) {
		printf("Error: Wr UD_BIST_TX_CNT_CTRL  PHY%d\n",
                               port_addr);
		return (rc);
       	}

	// Enable PCS loopback
	rc = bcmphy_pcs_loopback (fd, mdio_addr, port_addr, 1);
        if (rc) {
		printf("Error: pcs loopback disable\n");
		return (rc);
       	}

	// Enable XGXS loopback
	rc = bcmphy_xgxs_loopback (fd, mdio_addr, port_addr, 1);
        if (rc) {
		printf("Error: pcs loopback disable\n");
		return (rc);
       	}

	// Read Bist ctrl
	rc =  bcm8726_rd (fd, mdio_addr, port_addr, 
		BCM8726_PMA_PMD_ADDR, BCM8726_UD_BIST_CTRL, &data); 
        if (rc) {
		printf("Error: Wr UD_BIST_TX_CNT_CTRL  PHY%d\n",
                               port_addr);
		return (rc);
       	}

	// Enable bist and check receive
	data |= BCM8726_UD_BIST_CTRL_FLD_XGXS_RX_BIST_LANE_CHECK |
		BCM8726_UD_BIST_CTRL_FLD_PMD_TX_BIST_ENA |
		BCM8726_UD_BIST_CTRL_FLD_PMD_RX_BIST_ENA;

	// Clear the error count
	data &= ~BCM8726_UD_BIST_CTRL_FLD_BIST_RESULT_SEL;

	rc =  bcm8726_wr (fd, mdio_addr, port_addr, 
		BCM8726_PMA_PMD_ADDR, BCM8726_UD_BIST_CTRL, data); 
        if (rc) {
		printf("Error: Wr UD_BIST_CTRL  PHY%d\n",
                               port_addr);
		return (rc);
       	}

	timeout = 100;
	while (timeout--) {
		// Check if BIST has started. 
		usleep(10000);
		rc =  bcm8726_rd (fd, mdio_addr, port_addr, 
			BCM8726_PMA_PMD_ADDR, BCM8726_UD_BIST_CTRL, &data); 
		if (rc) {
			printf("Error: Wr UD_BIST_TX_CNT_CTRL  PHY%d\n",
                               port_addr);
			return (rc);
		}

		if ((data & BCM8726_UD_BIST_CTRL_FLD_PBIST_ON) &&
		    (data & BCM8726_UD_BIST_CTRL_FLD_XBIST_ON)) 
			break;
	}

	if (!((data & BCM8726_UD_BIST_CTRL_FLD_PBIST_ON) &&
		    (data & BCM8726_UD_BIST_CTRL_FLD_XBIST_ON))) {
		printf("Error: BIST has not started 0x%X\n", data);
		printf("WARNING: Expects Loopback Cables\n");
		return (-1);
	}

	// Wait for 100 milli seconds
	usleep(100000);

	// Stop the BIST
	data = 0x00;
	rc =  bcm8726_wr (fd, mdio_addr, port_addr, 
		BCM8726_PMA_PMD_ADDR, BCM8726_UD_BIST_CTRL, data); 
        if (rc) {
		printf("Error: Wr UD_BIST_CTRL  PHY%d\n",
                               port_addr);
		return (rc);
       	}

	// Make sure BIST has stopped.
	timeout = 1000;
	while(timeout--) {
		usleep(200);
		rc =  bcm8726_rd (fd, mdio_addr, port_addr, 
			BCM8726_PMA_PMD_ADDR, BCM8726_UD_BIST_CTRL, &data); 
		if (rc) {
			printf("Error: Rd UD_BIST_CTRL  PHY%d\n",
	                               port_addr);
			return (rc);
		}

		if ((data & BCM8726_UD_BIST_CTRL_FLD_PBIST_ON) ||
		    (data & BCM8726_UD_BIST_CTRL_FLD_XBIST_ON)) {
			continue;
		} else {
			break;
		}
	}

	if ((data & BCM8726_UD_BIST_CTRL_FLD_PBIST_ON) ||
            (data & BCM8726_UD_BIST_CTRL_FLD_XBIST_ON)) {
		printf("Error: BIST still in progress 0x%X\n", data);
		return (-2);
	}

	// Check for errors.
	rc =  bcm8726_rd (fd, mdio_addr, port_addr, 
		BCM8726_PMA_PMD_ADDR, BCM8726_UD_XGXS_BIST_ERROR, &xs_data); 
        if (rc) {
		printf("Error: Rd UD_XGXS_BIST_ERROR  PHY%d\n",
                               port_addr);
		return (rc);
       	}

	rc =  bcm8726_rd (fd, mdio_addr, port_addr, 
		BCM8726_PMA_PMD_ADDR, BCM8726_UD_PCS_BIST_ERROR, &pcs_data); 
        if (rc) {
		printf("Error: UD_PCS_BIST_ERROR PHY%d\n",
                               port_addr);
		return (rc);
       	}

	if (xs_data || pcs_data) {
		printf("Error: BIST Failed PCS_ERR(0x%X), XS_ERR(0x%X)\n",
			pcs_data, xs_data);
		return (-1);
	}

	// Check for the packet count.
	data = BCM8726_UD_BIST_CTRL_FLD_BIST_RESULT_SEL;
	rc =  bcm8726_wr (fd, mdio_addr, port_addr, 
		BCM8726_PMA_PMD_ADDR, BCM8726_UD_BIST_CTRL, data); 
        if (rc) {
		printf("Error: Wr UD_BIST_CTRL  PHY%d\n",
                               port_addr);
		return (rc);
       	}

	usleep(100000);
	rc =  bcm8726_rd (fd, mdio_addr, port_addr, 
		BCM8726_PMA_PMD_ADDR, BCM8726_UD_XGXS_BIST_ERROR, &xs_data); 
        if (rc) {
		printf("Error: Rd UD_XGXS_BIST_ERROR  PHY%d\n",
                               port_addr);
		return (rc);
       	}

	rc =  bcm8726_rd (fd, mdio_addr, port_addr, 
		BCM8726_PMA_PMD_ADDR, BCM8726_UD_PCS_BIST_ERROR, &pcs_data); 
        if (rc) {
		printf("Error: UD_PCS_BIST_ERROR PHY%d\n",
                               port_addr);
		return (rc);
       	}

	if ((xs_data != pkt_cnt) || (pcs_data != pkt_cnt)) {
		printf("Error: BIST Failed PCS_PKT_CNT(0x%X), XS_PKT_CNT(0x%X)\n",
			pcs_data, xs_data);
		return (-1);
	} else {
		printf("  BIST Passed:  pkt_count=%d, xs_pkt_cnt=%d, pcs_pkt_cnt=%d\n",
			pkt_cnt, xs_data, pcs_data);
	}

	// Read again to make sure the count is cleared.
	rc =  bcm8726_rd (fd, mdio_addr, port_addr, 
		BCM8726_PMA_PMD_ADDR, BCM8726_UD_XGXS_BIST_ERROR, &xs_data); 
        if (rc) {
		printf("Error: Rd UD_XGXS_BIST_ERROR  PHY%d\n",
                               port_addr);
		return (rc);
       	}

	rc =  bcm8726_rd (fd, mdio_addr, port_addr, 
		BCM8726_PMA_PMD_ADDR, BCM8726_UD_PCS_BIST_ERROR, &pcs_data); 
        if (rc) {
		printf("Error: UD_PCS_BIST_ERROR PHY%d\n",
                               port_addr);
		return (rc);
       	}

	if (xs_data || pcs_data) {
		printf("Error: BIST Failed PCS_PKT_CNT(0x%X), XS_PKT_CNT(0x%X)\n",
			pcs_data, xs_data);
		return (-1);
	}

	return (rc);
}

#define SFP_PROM_RD_SIZE	16

int bcm8726_sfp_prom_rd (int fd, uint8_t mdio_addr, uint8_t port_addr, uint8_t page_num,
			uint8_t *buf, uint32_t bufsize)
{
	int		rc = 0;
	uint16_t	addr_2w = ((page_num ? 0x51 : 0x50) << 
					BCM8726_UD_2WIRE_BYTE_CNT_CHP_ADDR_START);
	uint16_t	cnt, data, timeout = 100, prom_addr, rd_size;	

	for (prom_addr = 0; prom_addr < bufsize; prom_addr+=16)
	{
		rd_size = ((bufsize - prom_addr) > SFP_PROM_RD_SIZE)? 
				SFP_PROM_RD_SIZE : (bufsize - prom_addr);
		// Write the byte count to be read.
		data = addr_2w | rd_size;
		rc =  bcm8726_wr (fd, mdio_addr, port_addr, 
			BCM8726_PMA_PMD_ADDR, BCM8726_UD_2WIRE_BYTE_CNT, data); 
	        if (rc) {
			printf("Error: Wr UD_2WIRE_BYTE_CNT  PHY%d\n",
       	                        port_addr);
			return (rc);
		}

		// Write EEPROM Address start location 
		rc =  bcm8726_rd (fd, mdio_addr, port_addr, 
			BCM8726_PMA_PMD_ADDR, BCM8726_UD_2WIRE_MEM_ADDR, &data); 
	        if (rc) {
			printf("Error: Rd UD_2WIRE_MEM_ADDR  PHY%d\n",
       	                        port_addr);
			return (rc);
		}

		data &= ~BCM8726_UD_2WIRE_MEM_ADDR_FLD_ADDR;
		data |= prom_addr; 

		rc =  bcm8726_wr (fd, mdio_addr, port_addr, 
			BCM8726_PMA_PMD_ADDR, BCM8726_UD_2WIRE_MEM_ADDR, data); 
	        if (rc) {
			printf("Error: Rd UD_2WIRE_MEM_ADDR  PHY%d\n",
       	                        port_addr);
			return (rc);
		}
	
		// Enable 2 wire control	
		rc =  bcm8726_rd (fd, mdio_addr, port_addr, 
			BCM8726_PMA_PMD_ADDR, BCM8726_UD_2WIRE_CTRL, &data); 
	        if (rc) {
			printf("Error: Rd UD_2WIRE_CTRL  PHY%d\n",
       	                        port_addr);
			return (rc);
		}

		data |= BCM8726_UD_2WIRE_CTRL_FLD_ENABLE; 
//		There is a bug in the SFP read routine that will cause the 
//		link to go down. The work around for this bug is to
//		write 2C0F to this register. The bit definitions are unknown.

		data = 0x2C0F;
		rc =  bcm8726_wr (fd, mdio_addr, port_addr, 
			BCM8726_PMA_PMD_ADDR, BCM8726_UD_2WIRE_CTRL, data); 
	        if (rc) {
			printf("Error: Rd UD_2WIRE_CTRL  PHY%d\n",
       	                        port_addr);
			return (rc);
		}

		// Check for complete
		while (timeout--) {
			// sleep for 10 ms
			usleep(10000);
			rc =  bcm8726_rd (fd, mdio_addr, port_addr, 
				BCM8726_PMA_PMD_ADDR, BCM8726_UD_2WIRE_CTRL, &data); 
		        if (rc) {
				printf("Error: Rd UD_2WIRE_CTRL  PHY%d\n",
       		                        port_addr);
				return (rc);
			}
			if (!(data & BCM8726_UD_2WIRE_CTRL_FLD_ENABLE))
				break;
		}
		
		if (!timeout) {
			printf("Error: Phy%d EEPROM Rd\n", port_addr);
			return (-1);
		}	
		for (cnt = 0; cnt < rd_size; cnt++, buf++) {
			rc =  bcm8726_rd (fd, mdio_addr, port_addr, 
				BCM8726_PMA_PMD_ADDR, 0xC820+cnt, &data); 
		        if (rc) {
				printf("Error: Rd 0xC820  PHY%d\n",
       		                        port_addr);
				return (rc);
			} 

			*buf = (uint8_t) data;
		}
	}

	return (rc);
}

int bcm8726_sfp_test (int fd, uint8_t mdio_addr, uint8_t port_addr)
{
	int 		rc = 0;
	uint16_t	data, timeout = 100;

	rc =  bcm8726_rd (fd, mdio_addr, port_addr, 
		BCM8726_PMA_PMD_ADDR, BCM8726_PMD_STATUS, &data); 
	if (rc) {
		printf("Error: Rd PMD_STATUS  PHY%d\n", port_addr);
		return (rc);
	}

	rc =  bcm8726_rd (fd, mdio_addr, port_addr, 
		BCM8726_PMA_PMD_ADDR, BCM8726_UD_2WIRE_BYTE_CNT, &data); 
	if (rc) {
		printf("Error: Rd UD_2WIRE_BYTE_CNT  PHY%d\n", port_addr);
		return (rc);
	}

	// Set Chip Addr to 0x50
	data &= BCM8726_UD_2WIRE_BYTE_CNT_FLD_CHP_DATA;
	data |= (0x50 << 9);

	rc =  bcm8726_wr (fd, mdio_addr, port_addr, 
		BCM8726_PMA_PMD_ADDR, BCM8726_UD_2WIRE_BYTE_CNT, data); 
	if (rc) {
		printf("Error: Wr UD_2WIRE_BYTE_CNT  PHY%d\n", port_addr);
		return (rc);
	}

	// Set Byte Addr to 0x50
	data &= BCM8726_UD_2WIRE_BYTE_CNT_FLD_CHP_ADDR;
	data |= 0x01; 

	rc =  bcm8726_wr (fd, mdio_addr, port_addr, 
		BCM8726_PMA_PMD_ADDR, BCM8726_UD_2WIRE_BYTE_CNT, data); 
	if (rc) {
		printf("Error: Wr UD_2WIRE_BYTE_CNT  PHY%d\n", port_addr);
		return (rc);
	}

	// Set Memaddr to 0
	rc =  bcm8726_rd (fd, mdio_addr, port_addr, 
		BCM8726_PMA_PMD_ADDR, BCM8726_UD_2WIRE_MEM_ADDR, &data); 
	if (rc) {
		printf("Error: Rd UD_2WIRE_MEM_ADDR  PHY%d\n", port_addr);
		return (rc);
	}
	data &= ~BCM8726_UD_2WIRE_MEM_ADDR_FLD_ADDR;
	rc =  bcm8726_wr (fd, mdio_addr, port_addr, 
		BCM8726_PMA_PMD_ADDR, BCM8726_UD_2WIRE_MEM_ADDR, data); 
	if (rc) {
		printf("Error: Wr UD_2WIRE_MEM_ADDR  PHY%d\n", port_addr);
		return (rc);
	}

	// Enable 2 wire control	
	rc =  bcm8726_rd (fd, mdio_addr, port_addr, 
		BCM8726_PMA_PMD_ADDR, BCM8726_UD_2WIRE_CTRL, &data); 
	if (rc) {
		printf("Error: Rd UD_2WIRE_CTRL  PHY%d\n",
                               port_addr);
		return (rc);
	}

	data |= BCM8726_UD_2WIRE_CTRL_FLD_ENABLE; 

	rc =  bcm8726_wr (fd, mdio_addr, port_addr, 
		BCM8726_PMA_PMD_ADDR, BCM8726_UD_2WIRE_CTRL, data); 
        if (rc) {
		printf("Error: Rd UD_2WIRE_CTRL  PHY%d\n",
                               port_addr);
		return (rc);
	}

	// Check for complete
	while (timeout--) {
		// sleep for 10 ms
		usleep(10000);
		rc =  bcm8726_rd (fd, mdio_addr, port_addr, 
			BCM8726_PMA_PMD_ADDR, BCM8726_UD_2WIRE_CTRL, &data); 
	        if (rc) {
			printf("Error: Rd UD_2WIRE_CTRL  PHY%d\n",
       	                        port_addr);
			return (rc);
		}
		if (!(data & BCM8726_UD_2WIRE_CTRL_FLD_ENABLE))
			break;
	}
		
	if (!timeout) {
		printf("Error: Phy%d EEPROM Rd\n", port_addr);
		return (-1);
	}	

	// Read register 1 
	rc =  bcm8726_rd (fd, mdio_addr, port_addr, 
		BCM8726_PMA_PMD_ADDR, BCM8726_PMD_STATUS, &data); 
	if (rc) {
		printf("Error: Rd PMD_STATUS  PHY%d\n",
                               port_addr);
		return (rc);
	}
	// Read register 8 
	rc =  bcm8726_rd (fd, mdio_addr, port_addr, 
		BCM8726_PMA_PMD_ADDR, BCM8726_PMD_STATUS2, &data); 
	if (rc) {
		printf("Error: Rd PMD_STATUS2  PHY%d\n",
                               port_addr);
		return (rc);
	}

	// Read register 1 
	rc =  bcm8726_rd (fd, mdio_addr, port_addr, 
		BCM8726_PMA_PMD_ADDR, BCM8726_PMD_STATUS, &data); 
	if (rc) {
		printf("Error: Rd PMD_STATUS  PHY%d\n",
                               port_addr);
		return (rc);
	}

	return (rc);
}



int bcm8726_sfp_mode_config(int fd, uint8_t mdio_addr, uint8_t port_addr,
	uint8_t xcvr_code)
{
	int	rc = 0;

	if (xcvr_code & 0x40) {
		printf(" SFP(addr=%d) LRM mode init\n", port_addr);
	        rc =  bcm8726_wr (fd, mdio_addr, port_addr,
	                BCM8726_PMA_PMD_ADDR, 0xCA3F, 0x0000);
	        if (rc) {
			printf("Error: 0xCA3F=0x0000 PHY%d\n", port_addr);
			return (rc);
		}
	        rc =  bcm8726_wr (fd, mdio_addr, port_addr,
			BCM8726_PMA_PMD_ADDR, 0xCA1A, 0x0124);
		if (rc) {
			printf("Error: 0xCA1A=0x0124 PHY%d\n", port_addr);
			return (rc);
		}
		rc =  bcm8726_wr (fd, mdio_addr, port_addr,
			BCM8726_PMA_PMD_ADDR, 0xCA23, 0x4008);
		if (rc) {
			printf("Error: 0xCA23=0x4008 PHY%d\n", port_addr);
			return (rc);
		}

		rc =  bcm8726_wr (fd, mdio_addr, port_addr,
			BCM8726_PMA_PMD_ADDR, 0xCA3F, 0xAAAA);
		if (rc) {
			printf("Error: 0xCA3F=0xAAAA PHY%d\n", port_addr);
			return (rc);
		}

	} else if (xcvr_code & 0x20) {
		printf(" SFP(addr=%d) LR mode init\n", port_addr);
	        rc =  bcm8726_wr (fd, mdio_addr, port_addr,
			BCM8726_PMA_PMD_ADDR, 0xCA1A, 0x0044);
		if (rc) {
			printf("Error: 0xCA1A=0x0044 PHY%d\n", port_addr);
			return (rc);
		}
	} else if (xcvr_code & 0x10) {
		printf(" SFP(addr=%d) SR mode init\n", port_addr);
	        rc =  bcm8726_wr (fd, mdio_addr, port_addr,
			BCM8726_PMA_PMD_ADDR, 0xCA1A, 0x0044);
		if (rc) {
			printf("Error: 0xCA1A=0x0044 PHY%d\n", port_addr);
			return (rc);
		}
	} else  {
		printf(" SFP(addr=%d) default mode (no init)\n", port_addr);
	}
		
	return (rc);
}

// 0xCA85 = 0x0001  - Set serial boot control for external load
// 0xCA10 = 0x08CC - Micro Reset
// 0xCA10 = 0x08C8  - Micro reboot
// 0xCA10 = 0x08C2  - Set Soft Reset
// 0xCA10 = 0x08C0  - Clear Soft Reset
int bcm8726_eeprom_init  (int fd, uint8_t mdio_addr, uint8_t port_addr)
{
	int rc = 0, prom_err = 0;
	uint16_t data;

	// Micro code protect off
	rc =  bcm8726_wr (fd, mdio_addr, port_addr, 
		BCM8726_PMA_PMD_ADDR, 0xCA85, 0x01); 
        if (rc) {
		printf("Error: Wr 0xCA85  PHY%d\n",
                               port_addr);
		return (rc);
	}


	// 0xCA10 = 0x08C8  - Micro reboot
	data = BCM8726_EDC_CTRL_STATUS_FLD_SEQ_DEF_BYPASS |
		BCM8726_EDC_CTRL_STATUS_FLD_UC_CLK_SEL |
		BCM8726_EDC_CTRL_STATUS_FLD_UC_REBOOT;

	rc =  bcm8726_wr (fd, mdio_addr, port_addr, 
		BCM8726_PMA_PMD_ADDR, BCM8726_EDC_CTRL_STATUS, data); 
        if (rc) {
		printf("Error: Wr EDC_CTRL_STATUS  PHY%d\n",
                               port_addr);
		return (rc);
	}

	// 0xCA10 = 0x08CA  - Set Soft Reset
	data = BCM8726_EDC_CTRL_STATUS_FLD_SEQ_DEF_BYPASS |
		BCM8726_EDC_CTRL_STATUS_FLD_UC_CLK_SEL |
		BCM8726_EDC_CTRL_STATUS_FLD_UC_REBOOT |
		BCM8726_EDC_CTRL_STATUS_FLD_SOFT_RESET;

	rc =  bcm8726_wr (fd, mdio_addr, port_addr, 
		BCM8726_PMA_PMD_ADDR, BCM8726_EDC_CTRL_STATUS, data); 
        if (rc) {
		printf("Error: Wr EDC_CTRL_STATUS  PHY%d\n",
                               port_addr);
		return (rc);
	}

	// 0xCA10 = 0x08C8  - Micro reboot
	data = BCM8726_EDC_CTRL_STATUS_FLD_SEQ_DEF_BYPASS |
		BCM8726_EDC_CTRL_STATUS_FLD_UC_CLK_SEL |
		BCM8726_EDC_CTRL_STATUS_FLD_UC_REBOOT;

	rc =  bcm8726_wr (fd, mdio_addr, port_addr, 
		BCM8726_PMA_PMD_ADDR, BCM8726_EDC_CTRL_STATUS, data); 
        if (rc) {
		printf("Error: Wr EDC_CTRL_STATUS  PHY%d\n",
                               port_addr);
		return (rc);
	}

	usleep(200 * 1000);

	// Micro code protect on
	rc =  bcm8726_wr (fd, mdio_addr, port_addr, 
		BCM8726_PMA_PMD_ADDR, 0xCA85, 0x00); 
        if (rc) {
		printf("Error: Wr 0xCA85  PHY%d\n",
                               port_addr);
		return (rc);
	}

	rc = bcm8726_eeprom_test (fd, mdio_addr, port_addr, &prom_err);
	if (rc) {
		printf("Error: EEPROM Load failed Phy%d\n", port_addr);
		return (rc);
	}

	if (prom_err) {
		printf("  PHY EEPROM Load Failed (port_addr = 0x%X)\n", port_addr);  
		return (-1);
	} else {
                printf("  PHY EEPROM Load (port_addr = 0x%X) Successful\n", port_addr);
	}
	return (rc);
}

int bcm8726_init  (int fd, uint8_t mdio_addr, 
			uint8_t port_addr, uint8_t lane_swap)
{
	int rc;
	uint16_t data;
		
        rc = bcm8726_rd(fd, mdio_addr, port_addr, BCM8726_XGXS_ADDR,
                BCM8726_RXA_LANE_CTRL, &data);
        if (rc) {
                printf("Error: Failed to read BCM8726_RXA_LANE_CTRL for PHY%d\n",
                                port_addr);
                return (rc);
        }
	
	if (lane_swap) {
		data |= BCM8726_RXA_LANE_CTRL_FLD_SWAP;
	} else {
		data &= ~BCM8726_RXA_LANE_CTRL_FLD_SWAP;
	}

        rc = bcm8726_wr(fd, mdio_addr, port_addr, BCM8726_XGXS_ADDR,
                BCM8726_RXA_LANE_CTRL, data);
        if (rc) {
                printf("Error: Failed to read BCM8726_RXA_LANE_CTRL for PHY%d\n",
                                port_addr);
                return (rc);
        }

        rc = bcm8726_rd(fd, mdio_addr, port_addr, BCM8726_XGXS_ADDR,
                BCM8726_TXA_LANE_CTRL, &data);
        if (rc) {
                printf("Error: Failed to read BCM8726_TXA_LANE_CTRL for PHY%d\n",
                                port_addr);
                return (rc);
        }
	
	if (lane_swap) {
		data |= BCM8726_TXA_LANE_CTRL_FLD_SWAP;
	} else {
		data &= ~BCM8726_TXA_LANE_CTRL_FLD_SWAP;
	}

        rc = bcm8726_wr(fd, mdio_addr, port_addr, BCM8726_XGXS_ADDR,
                BCM8726_TXA_LANE_CTRL, data);
        if (rc) {
                printf("Error: Failed to read BCM8726_TXA_LANE_CTRL for PHY%d\n",
                                port_addr);
                return (rc);
        }
#ifdef DIAG_FEX
	rc =  bcm8726_wr (fd, mdio_addr, port_addr, 
		BCM8726_PMA_PMD_ADDR, 0xCA00, 0xFF68); 
        if (rc) {
                printf("Error: 0xCA00=0xFF68 PHY%d\n", port_addr);
                return (rc);
        }
	rc =  bcm8726_wr (fd, mdio_addr, port_addr, 
		BCM8726_PMA_PMD_ADDR, 0xCA01, 0xE000); 
        if (rc) {
                printf("Error: 0xCA01=0xE000 PHY%d\n", port_addr);
                return (rc);
        }
	rc =  bcm8726_wr (fd, mdio_addr, port_addr, 
		BCM8726_PMA_PMD_ADDR, 0xCA05, 0x02A0); 
        if (rc) {
                printf("Error: 0xCA05=0x02A0 PHY%d\n", port_addr);
                return (rc);
        }
#else
	rc =  bcm8726_wr (fd, mdio_addr, port_addr, 
		BCM8726_PMA_PMD_ADDR, 0xCA00, 0x8868); 
        if (rc) {
                printf("Error: 0xCA00=0x8868 PHY%d\n", port_addr);
                return (rc);
        }
	rc =  bcm8726_wr (fd, mdio_addr, port_addr, 
		BCM8726_PMA_PMD_ADDR, 0xCA01, 0xB000); 
        if (rc) {
                printf("Error: 0xCA01=0xB000 PHY%d\n", port_addr);
                return (rc);
        }
	rc =  bcm8726_wr (fd, mdio_addr, port_addr, 
		BCM8726_PMA_PMD_ADDR, 0xCA05, 0x0260); 
        if (rc) {
                printf("Error: 0xCA05=0x0260 PHY%d\n", port_addr);
                return (rc);
        }
#endif
	return (rc);

}

int bcm8726_gpio_init  (int fd, uint8_t mdio_addr, 
			uint8_t port_addr, uint8_t output)
{
	int rc;
	uint16_t data;
		
	// Enable GPIO Mode
        rc = bcm8726_rd(fd, mdio_addr, port_addr, BCM8726_PMA_PMD_ADDR,
                BCM8726_UD_PMD_DIGITAL_CTRL, &data);
        if (rc) {
                printf("Error: Failed to read BCM8726_UD_PMD_DIGITAL_CTRL for PHY%d\n",
                                port_addr);
                return (rc);
        }
	data &= ~BCM8726_UD_PMD_DIGITAL_CTRL_FLD_GPIO_SEL;
        rc = bcm8726_wr(fd, mdio_addr, port_addr, BCM8726_PMA_PMD_ADDR,
                BCM8726_UD_PMD_DIGITAL_CTRL, data);
        if (rc) {
                printf("Error: Failed to write BCM8726_UD_PMD_DIGITAL_CTRL for PHY%d\n",
                                port_addr);
                return (rc);
        }

	// Set output or input..,
        rc = bcm8726_rd(fd, mdio_addr, port_addr, BCM8726_PMA_PMD_ADDR,
                BCM8726_UD_GPIO_CTRL, &data);
        if (rc) {
                printf("Error: Failed to read BCM8726_UD_GPIO_CTRL for PHY%d\n",
                                port_addr);
                return (rc);
        }
	if (output) {
		data &= ~BCM8726_UD_GPIO_CTRL_FLD_OUT_DS;
	} else {
		data |= BCM8726_UD_GPIO_CTRL_FLD_OUT_DS;
	}
        rc = bcm8726_wr(fd, mdio_addr, port_addr, BCM8726_PMA_PMD_ADDR,
                BCM8726_UD_GPIO_CTRL, data);
        if (rc) {
                printf("Error: Failed to write BCM8726_UD_GPIO_CTRL for PHY%d\n",
                                port_addr);
                return (rc);
        }

	return (rc);
}

// gpio_pin take values 0 or 1.
int bcm8726_gpio_set  (int fd, uint8_t mdio_addr, uint8_t port_addr, uint8_t gpio_pin)
{
	int rc;
	uint16_t data;
		
        rc = bcm8726_rd(fd, mdio_addr, port_addr, BCM8726_PMA_PMD_ADDR,
                BCM8726_UD_GPIO_CTRL, &data);
        if (rc) {
                printf("Error: Failed to read BCM8726_UD_GPIO_CTRL for PHY%d\n",
                                port_addr);
                return (rc);
        }

	data |= (0x01 << gpio_pin);
	
        rc = bcm8726_wr(fd, mdio_addr, port_addr, BCM8726_PMA_PMD_ADDR,
                BCM8726_UD_GPIO_CTRL, data);
        if (rc) {
                printf("Error: Failed to write BCM8726_UD_GPIO_CTRL for PHY%d\n",
                                port_addr);
                return (rc);
        }

	return (rc);
}

// gpio_pin take values 0 or 1.
int bcm8726_gpio_clr  (int fd, uint8_t mdio_addr, uint8_t port_addr, uint8_t gpio_pin)
{
	int rc;
	uint16_t data;
		
        rc = bcm8726_rd(fd, mdio_addr, port_addr, BCM8726_PMA_PMD_ADDR,
                BCM8726_UD_GPIO_CTRL, &data);
        if (rc) {
                printf("Error: Failed to read BCM8726_UD_GPIO_CTRL for PHY%d\n",
                                port_addr);
                return (rc);
        }

	data &= (0x01 << gpio_pin);
	
        rc = bcm8726_wr(fd, mdio_addr, port_addr, BCM8726_PMA_PMD_ADDR,
                BCM8726_UD_GPIO_CTRL, data);
        if (rc) {
                printf("Error: Failed to write BCM8726_UD_GPIO_CTRL for PHY%d\n",
                                port_addr);
                return (rc);
        }

	return (rc);
}

// gpio_pin take values 0 or 1.
int bcm8726_gpio_get  (int fd, uint8_t mdio_addr, uint8_t port_addr, uint8_t gpio_pin, 
			uint8_t *gpio_val)
{
	int rc;
	uint16_t data;
		
        rc = bcm8726_rd(fd, mdio_addr, port_addr, BCM8726_PMA_PMD_ADDR,
                BCM8726_UD_GPIO_CTRL, &data);
        if (rc) {
                printf("Error: Failed to read BCM8726_UD_GPIO_CTRL for PHY%d\n",
                                port_addr);
                return (rc);
        }
	*gpio_val = (data & (0x01 << (gpio_pin+8))) ? 1 : 0;
	return (rc);
}

int bcmphy_premep_set (int fd, uint8_t mdio_addr, uint8_t port_addr, 
			uint8_t preemp)
{
	int rc;
	uint16_t data;

        rc = bcm8726_rd(fd, mdio_addr, port_addr, BCM8726_XGXS_ADDR,
                BCM8726_TXA_ACONTROL_3, &data);
        if (rc) {
                printf("Error: Failed to rd BCM8726_TXA_ACONTROL_3 for PHY%d\n",
                                port_addr);
                return (rc);
	}

	data &= 0xFFF;
	data |= (preemp << 12);
        rc = bcm8726_wr(fd, mdio_addr, port_addr, BCM8726_XGXS_ADDR,
                BCM8726_TXA_ACONTROL_3, data);
        if (rc) {
                printf("Error: Failed to wr BCM8726_TXA_ACONTROL_3 for PHY%d\n",
                                port_addr);
                return (rc);
	}
	return (rc);
}
