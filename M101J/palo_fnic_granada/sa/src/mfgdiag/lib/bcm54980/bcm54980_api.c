/********************************************************************
 *
 *      File:   bcm54980_api.c
 *      Name:   Sudharshan Kadari
 *
 *      Description:
 *       Broadcom Serdes 54980 register access
 *
 *
 * Copyright (c) 1985-2008 by Cisco Systems, Inc.
 * All rights reserved.
 *
 *
 *********************************************************************/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include "bcm54980_reg.h"
#include "bcm54980_api.h"

int bcm54980_soft_reset (int fd, uint8_t inst, uint8_t port_addr)
{
	uint16_t data, reset_b = (1 <<  BCM54X80_MII_CONTROL_FLD_RESET);
	int	 rc = 0, timeout = 100;	

	rc = bcm54980_reg_read (fd, inst, port_addr,  BCM54X80_MII_CONTROL, &data);
	if (rc) {
		printf("Error: read MII_CONTROL (rc=%d)\n", rc);
		return (rc);
	}

	// Soft Reset is a self clearing bit. 
	data |= reset_b; 

	// Write the reset bit
	rc = bcm54980_reg_write (fd, inst, port_addr,  BCM54X80_MII_CONTROL, data);
	if (rc) {
		printf("Error: write MII_CONTROL (rc=%d)\n", rc);
		return (rc);
	}

	// Poll to check the bit is cleared
	do {
		usleep(10);	// Sleep for 10us
		rc = bcm54980_reg_read (fd, inst, port_addr,  BCM54X80_MII_CONTROL, &data);
	} while (!rc && timeout-- && (data & reset_b)); 

	return( (data & reset_b) ? -1 : rc);
}

int bcm54980_aneg_init (int fd, uint8_t inst, uint8_t port_addr)
{
	uint16_t data;
	int	 rc = 0;	

	rc = bcm54980_reg_read (fd, inst, port_addr,  BCM54X80_ANEG_ADV, &data);
	if (rc) {
		printf("Error: read BCM54X80_ANEG_ADV (rc=%d)\n", rc);
		return (rc);
	}
	data |= 0x01;
	rc = bcm54980_reg_write (fd, inst, port_addr,  BCM54X80_ANEG_ADV, data);
	if (rc) {
		printf("Error: write BCM54X80_ANEG_ADV (rc=%d)\n", rc);
		return (rc);
	}

	// FDX only.
	data = (0x01 << BCM54X80_1000BT_CONTROL_FLD_ADV_FD_ABLE);
	rc = bcm54980_reg_write (fd, inst, port_addr,  BCM54X80_1000BT_CONTROL, data);
	if (rc) {
		printf("Error: write BCM54X80_1000BT_CONTROL (rc=%d)\n", rc);
		return (rc);
	}

	// Restart ANEG
	data = ((0x01 << BCM54X80_MII_CONTROL_FLD_CU_ANET_ENA)  |
		(0x01 << BCM54X80_MII_CONTROL_FLD_RESTART_ANEG) |
		(0x01 << BCM54X80_MII_CONTROL_FLD_DUPLEX_MODE)  |
		(0x01 << BCM54X80_MII_CONTROL_FLD_SPEED_SEL_MSB));
	rc = bcm54980_reg_write (fd, inst, port_addr,  BCM54X80_MII_CONTROL, data);
	if (rc) {
		printf("Error: write MII_CONTROL (rc=%d)\n", rc);
		return (rc);
	}
	return (0);
}

int bcm54980_pcs_loopback (int fd, uint8_t inst, uint8_t port_addr, uint8_t ena)
{
	uint16_t data, lpbk_b = (1 << BCM54X80_MII_CONTROL_FLD_LOOPBACK);
	int	 rc = 0;	

	rc = bcm54980_reg_read (fd, inst, port_addr,  BCM54X80_MII_CONTROL, &data);
	if (rc) {
		printf("Error: read MII_CONTROL (rc=%d)\n", rc);
		return (rc);
	}

	if (ena) {
		data |= lpbk_b;
	} else {
		data &= ~lpbk_b;
	}

	rc = bcm54980_reg_write (fd, inst, port_addr,  BCM54X80_MII_CONTROL, data);
	if (rc) {
		printf("Error: write MII_CONTROL (rc=%d)\n", rc);
		return (rc);
	}
	return (0);
}


int bcm54980_link_status_get (int fd, uint8_t inst, uint8_t port_addr, uint8_t *link_up)
{
	uint16_t data, link_b = (1 <<  BCM54X80_MII_STATUS_FLD_CU_LINK_STATUS);
	uint16_t fault_b = (1 << BCM54X80_MII_STATUS_FLD_REMOTE_FAULT);
	int	 rc = 0;

	// Read twice to avoid sticky bit.
	rc = bcm54980_reg_read (fd, inst, port_addr,  BCM54X80_MII_STATUS, &data);
	if (rc) {
		printf("Error: read MII_STATUS (rc=%d)\n", rc);
		return (rc);
	}

	rc = bcm54980_reg_read (fd, inst, port_addr,  BCM54X80_MII_STATUS, &data);
	if (rc) {
		printf("Error: read MII_STATUS (rc=%d)\n", rc);
		return (rc);
	}

	if (data & fault_b) {
		printf(" BCM54X80-%d Remote Fault seen\n", inst);
		*link_up = 0;
	} else {
		*link_up = (data & link_b) ? 1 : 0;
	}	
	return (rc);
}

// Expanded register routines.
int bcm54980_exp_reg_read (int fd, uint8_t inst, uint8_t port_addr, uint8_t addr, uint16_t *data)
{
	uint16_t ldata = BCM54X80_EXP_REG_SELECT | addr;
	int rc = 0, err = 0;

	// Enable the Expansion register access.
	ldata = BCM54X80_EXP_REG_SELECT | addr;
	rc = bcm54980_reg_write (fd, inst, port_addr,  BCM54X80_EXP_ACCESS_REG, ldata);
	if (rc) {
		printf("Error: wr BCM54X80_EXP_ACCESS_REG (rc=%d)\n", rc);
		return (rc);
	}

	err = bcm54980_reg_read (fd, inst, port_addr,  BCM54X80_EXP_DATA_REG, data);
	if (err) {
		printf("Error: wr BCM54X80_EXP_ACCESS_REG (rc=%d)\n", err);
	}

	// Clear Exp Reg Access. 
	ldata = 0x00; 
	rc = bcm54980_reg_write (fd, inst, port_addr,  BCM54X80_EXP_ACCESS_REG, ldata);
	if (rc) {
		printf("Error: wr BCM54X80_EXP_ACCESS_REG (rc=%d)\n", rc);
		return (rc);
	}

	return (err? err : rc);
}

int bcm54980_exp_reg_write (int fd, uint8_t inst, uint8_t port_addr, uint8_t addr, uint16_t data)
{
	uint16_t ldata = BCM54X80_EXP_REG_SELECT | addr;
	int rc = 0, err = 0;

	// Enable the Expansion register access.
	ldata = BCM54X80_EXP_REG_SELECT | addr;
	rc = bcm54980_reg_write (fd, inst, port_addr,  BCM54X80_EXP_ACCESS_REG, ldata);
	if (rc) {
		printf("Error: wr BCM54X80_EXP_ACCESS_REG (rc=%d)\n", rc);
		return (rc);
	}

	err = bcm54980_reg_write (fd, inst, port_addr,  BCM54X80_EXP_DATA_REG, data);
	if (err) {
		printf("Error: wr BCM54X80_EXP_ACCESS_REG (rc=%d)\n", err);
	}

	// Clear Exp Reg Access. 
	ldata = 0x00; 
	rc = bcm54980_reg_write (fd, inst, port_addr,  BCM54X80_EXP_ACCESS_REG, ldata);
	if (rc) {
		printf("Error: wr BCM54X80_EXP_ACCESS_REG (rc=%d)\n", rc);
		return (rc);
	}

	return (err? err : rc);
}



// Shadow register routines.
int bcm54980_shadow_reg_read (int fd, uint8_t inst, uint8_t port_addr, uint8_t addr, 
				uint16_t *data)
{
	uint16_t ldata = ((addr & 0x1F) << BCM54X80_SPARE_CONTROL_FLD_ADDR_S);
	int rc = 0, err = 0;

	// Write the shadow address 
	rc = bcm54980_reg_write (fd, inst, port_addr,  BCM54X80_SPARE_CONTROL, ldata);
	if (rc) {
		printf("Error: wr BCM54X80_SPARE_CONTROL (rc=%d)\n", rc);
		return (rc);
	}

	// read the data
	err = bcm54980_reg_read (fd, inst, port_addr,  BCM54X80_SPARE_CONTROL, data);
	if (err) {
		printf("Error: rd BCM54X80_SPARE_CONTROL (rc=%d)\n", err);
	} 

	return (err? err : rc);
}


int bcm54980_shadow_reg_write (int fd, uint8_t inst, uint8_t port_addr, uint8_t addr, 
				uint16_t data)
{
	uint16_t ldata = ((addr&0x1F) << BCM54X80_SPARE_CONTROL_FLD_ADDR_S) |
			 (0x01 << BCM54X80_SPARE_CONTROL_FLD_WRITE) |
			 (data & 0x3FF);
	int rc = 0;

	// Write the shadow address 
	rc = bcm54980_reg_write (fd, inst, port_addr,  BCM54X80_SPARE_CONTROL, ldata);
	if (rc) {
		printf("Error: wr BCM54X80_SPARE_CONTROL (rc=%d)\n", rc);
	}

	return (rc);
}


// AUX Shadow register routines.
int bcm54980_aux_shadow_reg_read (int fd, uint8_t inst, uint8_t port_addr, uint8_t addr, 
				uint16_t *data)
{
	uint16_t ldata = ((addr & 0x07) << 12) | 
			  BCM54X80_AUX_SHADOW_MISC_CTRL;
	int rc = 0;

	// Write the aux_shadow address 
	rc = bcm54980_reg_write (fd, inst, port_addr,  BCM54X80_AUX_CONTROL, ldata);
	if (rc) {
		printf("Error: wr BCM54X80_AUX_CONTROL (rc=%d)\n", rc);
		return (rc);
	}

	// read the data
	rc = bcm54980_reg_read (fd, inst, port_addr,  BCM54X80_AUX_CONTROL, data);
	if (rc) {
		printf("Error: rd BCM54X80_AUX_CONTROL (rc=%d)\n", rc);
	} 

	return (rc);
}


int bcm54980_aux_shadow_reg_write (int fd, uint8_t inst, uint8_t port_addr, uint8_t addr, 
				uint16_t data)
{
	uint16_t ldata = (data & 0xFFF8) | (addr & 0x07);
	int rc = 0;

	if ((addr &0x7) == 0x7)
		ldata |= 0x8000;

	// Write the aux_shadow address 
	rc = bcm54980_reg_write (fd, inst, port_addr,  BCM54X80_AUX_CONTROL, ldata);
	if (rc) {
		printf("Error: wr BCM54X80_AUX_CONTROL (rc=%d)\n", rc);
	}

	return (rc);
}

// External & line loopback
int bcm54980_loopback(int fd, uint8_t inst, uint8_t port_addr, uint8_t ext, uint8_t ena)
{
	uint16_t data, lpbk_b = (1 << BCM54X80_AUX_SHADOW_CTRL_FLD_EXT_LPBK);
	int	 rc = 0;	
	uint8_t	 addr = ext ? BCM54X80_AUX_SHADOW_CTRL :
				BCM54X80_AUX_SHADOW_MISC_TEST;

	rc = bcm54980_aux_shadow_reg_read (fd, inst, port_addr,  addr, &data);
	if (rc) {
		printf("Error: read addr=0x%X (rc=%d)\n", addr, rc);
		return (rc);
	}

	if (ena) {
		data |= lpbk_b;
	} else {
		data &= ~lpbk_b;
	}

	rc = bcm54980_aux_shadow_reg_write (fd, inst, port_addr,  addr, data);
	if (rc) {
		printf("Error: write addr=0x%X (rc=%d)\n", addr, rc);
		return (rc);
	}
	return (0);
}

int bcm54980_ext_loopback(int fd, uint8_t inst, uint8_t port_addr, uint8_t ena)
{
	return (bcm54980_loopback(fd, inst, port_addr,  1, ena));
}

int bcm54980_line_loopback(int fd, uint8_t inst, uint8_t port_addr, uint8_t ena)
{
	return (bcm54980_loopback(fd, inst, port_addr,  0, ena));
}

int bcm54980_ext_volt_regulate(int fd, uint8_t inst, uint8_t port_addr, uint8_t volt)
{
	uint16_t data;
	int	 rc = 0;	

	rc = bcm54980_aux_shadow_reg_read (fd, inst, port_addr,  
			BCM54X80_AUX_SHADOW_CTRL, &data);
	if (rc) {
		printf("Error: read BCM54X80_AUX_SHADOW_CTRL (rc=%d)\n", rc);
		return (rc);
	}

	data &= (~0xF << 7);
	data |= ((volt & 0xF) << 7);

	rc = bcm54980_aux_shadow_reg_write (fd, inst, port_addr,  
			BCM54X80_AUX_SHADOW_CTRL, data);
	if (rc) {
		printf("Error: write BCM54X80_AUX_SHADOW_CTRL (rc=%d)\n", rc);
		return (rc);
	}
	return (0);
}

int bcm54980_external_lpbk_plug_init (int fd, uint8_t inst, 
			uint8_t port_addr, uint8_t flag)
{
	uint16_t data;
	int rc = 0;

	data = (1<< BCM54X80_1000BT_CONTROL_FLD_MS_CFG_ENA) |
		(1 << BCM54X80_1000BT_CONTROL_FLD_MS_CFG_VAL);
	rc = bcm54980_reg_write (fd, inst, port_addr,  
			BCM54X80_1000BT_CONTROL, data);
	if (rc) return (rc);

	data = (1 << BCM54X80_MII_CONTROL_FLD_SPEED_SEL_MSB);
	rc = bcm54980_reg_write (fd, inst, port_addr,  
			BCM54X80_MII_CONTROL, data);
	if (rc) return (rc);

	data = 0x8400;
	rc = bcm54980_reg_write (fd, inst, port_addr,  
			BCM54X80_AUX_CONTROL, data);
	if (rc) return (rc);

	return (rc);
}

// Super Isolate Mode Enable or Disable.
int bcm54980_super_isolate_ena (int fd, uint8_t inst, uint8_t port_addr, uint8_t ena)
{
	int	 rc = 0;
	uint16_t data;

        // Set the Super Isolate Mode
        rc = bcm54980_aux_shadow_reg_read (fd, inst, port_addr,
                BCM54X80_AUX_SHADOW_MII_CTRL, &data);
        if (rc) {
                printf("Error: read addr=0x%X (rc=%d)\n",
                        BCM54X80_AUX_SHADOW_MII_CTRL, rc);
                return (rc);
        }

	if (ena) {
        	data |= BCM54X80_AUX_SHADOW_MII_CTRL_FLD_SUPER_ISOLATE;
	} else {
        	data &= ~BCM54X80_AUX_SHADOW_MII_CTRL_FLD_SUPER_ISOLATE;
	}

        rc = bcm54980_aux_shadow_reg_write (fd, inst, port_addr,
                        BCM54X80_AUX_SHADOW_MII_CTRL, data);
        if (rc) {
                printf("Error: write addr=0x%X (rc=%d)\n", 
			BCM54X80_AUX_SHADOW_MII_CTRL, rc);
                return (rc);
        }

	return (rc);
}

// SGMII Mode Set.
int bcm54980_sgmii_mode (int fd, uint8_t inst, uint8_t port_addr, uint8_t ena)
{
	int	 rc = 0;
	uint16_t data;

        // SGMII Enabled. 
        rc = bcm54980_shadow_reg_read (fd, inst, port_addr,
                BCM54X80_SHADOW_MODE_CTRL, &data);
        if (rc) {
                printf("Error: read addr=0x%X (rc=%d)\n",
                        BCM54X80_SHADOW_MODE_CTRL, rc);
                return (rc);
        }
	if (ena) {
       		data |= BCM54X80_SHADOW_MODE_CTRL_FLD_SGMII_ENA;
	} else {
       		data &= ~BCM54X80_SHADOW_MODE_CTRL_FLD_SGMII_ENA;
       		data |= BCM54X80_SHADOW_MODE_CTRL_MEDIA_CONV;
	}
        rc = bcm54980_shadow_reg_write (fd, inst, port_addr,
                        BCM54X80_SHADOW_MODE_CTRL, data);
        if (rc) {
                printf("Error: write addr=0x%X (rc=%d)\n", 
			BCM54X80_SHADOW_MODE_CTRL, rc);
                return (rc);
        }

	// Set Low Power Mode.
	if (!ena) {
		data = (0x01 << BCM54X80_MII_CONTROL_FLD_POWER_DOWN);
	        rc = bcm54980_reg_write (fd, inst, port_addr,  BCM54X80_MII_CONTROL, data);
		if (rc) {
			printf("Error: read MII_CONTROL (rc=%d)\n", rc);
			return (rc);
		}
		usleep(100);
	}
	data = (0x01 << BCM54X80_MII_CONTROL_FLD_DUPLEX_MODE) |
	       (0x01 << BCM54X80_MII_CONTROL_FLD_SPEED_SEL_MSB);
        rc = bcm54980_reg_write (fd, inst, port_addr,  BCM54X80_MII_CONTROL, data);
        if (rc) {
                printf("Error: read MII_CONTROL (rc=%d)\n", rc);
                return (rc);
        }
	
        // Clear the SGMII Mode. 
        rc = bcm54980_shadow_reg_read (fd, inst, port_addr,
                BCM54X80_SHADOW_MODE_CTRL, &data);
        if (rc) {
                printf("Error: read addr=0x%X (rc=%d)\n",
                        BCM54X80_SHADOW_MODE_CTRL, rc);
                return (rc);
        }
       	data &= ~BCM54X80_SHADOW_MODE_CTRL_FLD_SGMII_ENA;
        rc = bcm54980_shadow_reg_write (fd, inst, port_addr,
                        BCM54X80_SHADOW_MODE_CTRL, data);
        if (rc) {
                printf("Error: write addr=0x%X (rc=%d)\n", 
			BCM54X80_SHADOW_MODE_CTRL, rc);
                return (rc);
        }

	return (rc);
}

// Need to update this. The chip needs to be programmed in
// Media Convertor Mode.
int bcm54980_init (int fd, uint8_t inst, uint8_t port_addr)
{
	int	rc = 0;
	uint32_t cnt;
	uint16_t 	sha_adr_data[][2] = {
		// Select Media Converter
		{BCM54X80_SHADOW_MODE_CTRL,
#ifdef DIAG_PORTOLA
			BCM54X80_SHADOW_MODE_CTRL_FLD_SGMII_SEL
#else
			BCM54X80_SHADOW_MODE_CTRL_MEDIA_CONV
#endif
		},
		// Enable 1G T Link.
		{BCM54X80_SHADOW_SPARE_CTRL1,
			(1 << BCM54X80_SHADOW_SPARE_CTRL1_FLD_LINKSPD)},
		{BCM54X80_SHADOW_AUX_CTRL, 0 },
		{BCM54X80_SHADOW_LED_CONTROL,
			(1<<BCM54X80_SHADOW_LED_CONTROL_FLD_ACTIVITY_ENA) |
			(1<<BCM54X80_SHADOW_LED_CONTROL_FLD_REMOTE_FAULT) |
			(BCM54X80_SHADOW_LED_CONTROL_TX_RX_SEL) },
		};
	uint16_t 	addr_data[][2] = {
		{BCM54X80_MII_CONTROL, 
			((0x01 << BCM54X80_MII_CONTROL_FLD_DUPLEX_MODE)|
			 (0x01 << BCM54X80_MII_CONTROL_FLD_CU_ANET_ENA)|
			 (0x01 << BCM54X80_MII_CONTROL_FLD_RESTART_ANEG)|
			 BCM54X80_SPEED_SEL_1000_MSB)},
		{BCM54X80_TEST_REG1, 
			((1 << BCM54X80_TEST_REG1_FLD_CRC_ERR_CNT) |
			 (1 << BCM54X80_TEST_REG1_FLD_TX_ERR_CNT))},
		{BCM54X80_PHY_EXT_CONTROL, 
			((0x01 << BCM54X80_PHY_EXT_CONTROL_FLD_LED_TRF_EN)|
			 (0x01 << BCM54X80_PHY_EXT_CONTROL_FLD_LED_ON))},
		};

	// Set the mode control to Media Convertor Mode
	for (cnt = 0; !rc && cnt < 
		(sizeof(sha_adr_data)/(2*(sizeof(uint16_t)))); cnt++) {
		rc = bcm54980_shadow_reg_write (fd, inst, port_addr,  
			(uint8_t)sha_adr_data[cnt][0],
			sha_adr_data[cnt][1]);
		if (rc)  {
			printf("Error: Wr Adr [0x%X] (rc = %d)\n", 
				sha_adr_data[cnt][0], rc);
		}
	}


	// Set the MII control to FULL DUPLEX / 1G.
	for (cnt = 0; !rc && cnt < 
		(sizeof(addr_data)/(2*(sizeof(uint16_t)))); cnt++) {
		rc = bcm54980_reg_write (fd, inst, port_addr,  
			(uint8_t)addr_data[cnt][0], addr_data[cnt][1]);
	}

	return (rc);
}

int bcm54980_status_check (int fd, uint8_t inst, uint8_t port_addr)
{
	int		rc = 0;
	uint32_t        cnt = 0;
	uint16_t	data = 0;
	uint8_t 	addr[] = {
			BCM54X80_MII_STATUS,
			BCM54X80_IEEE_EXT_STATUS,
			BCM54X80_PHY_EXT_STATUS,
			};

	for (cnt = 0; !rc && cnt < 
		(sizeof(addr)/sizeof(uint8_t)); cnt++) {
		rc = bcm54980_reg_read (fd, inst, port_addr,  addr[cnt], &data);
		if (rc) {
			printf("Error: Rd 0x%X\n", addr[cnt]);
		}
	}

	return (rc);
}

int bcm54980_led_ena (int fd, uint8_t inst, uint8_t port_addr)
{
	unsigned short int data;
	int rc = 0;

	/* Set EN, RST, SLED_MUX[1], SLED_IN[1] */
	data = ((1   << 15) | // Write Enable
		(0xB << 10) | // SLED_1 register
		(0   << 7)  | // SLED_MUX[1]=0
		(1   << 3)  | // SLED_RESET
		(1   << 2)  | // ENABLE_SLED_MODE
		(1   << 0));  // Enable SLED_IN[1]

	rc = bcm54980_shadow_reg_write(fd, inst, port_addr, 
			BCM54X80_SHADOW_SLED_1, data);
	if (rc)  {
		printf("Error: Wr BCM54X80_SHADOW_SLED_1 (rc = %d)\n", rc);
		return (rc);
	}


	/* set SLED_MUX[0] */
	data = ((1   << 15) | // Write Enable
		(0xC << 10) | // SLED_2 register
		(1   << 6));  // SLED_MUX[0]=1
	rc = bcm54980_shadow_reg_write(fd, inst, port_addr, 
			BCM54X80_SHADOW_SLED_2, data);
	if (rc)  {
		printf("Error: Wr BCM54X80_SHADOW_SLED_2 (rc = %d)\n", rc);
		return (rc);
	}

	/* Release RST */
	data = ((1   << 15) | // Write Enable
		(0xB << 10) | // SLED_1 register
		(0   << 7)  | // SLED_MUX[1]=0
		(1   << 2)  | // ENABLE_SLED_MODE
		(1   << 0));  // Enable SLED_IN[1]
	rc = bcm54980_shadow_reg_write(fd, inst, port_addr, 
			BCM54X80_SHADOW_SLED_1, data);
	if (rc)  {
		printf("Error: Wr BCM54X80_SHADOW_SLED_1 (rc = %d)\n", rc);
		return (rc);
	}

    	/* SLED_MUX[1:0] is now 01b for 24-bit mode, SLED enabled using SLED_IN[1] */

	return (rc);
}

int bcm54980_led_set (int fd, uint8_t inst, uint8_t port_addr, uint8_t green, uint8_t mode)
{
	uint16_t data;
	int	 rc = 0;	
	uint8_t  addr = green ? BCM54X80_SHADOW_LED_SEL2 :
				BCM54X80_SHADOW_LED_SEL1;

	rc = bcm54980_shadow_reg_read (fd, inst, port_addr,  addr, &data);
	if (rc) {
		printf("Error: read addr=0x%X (rc=%d)\n", addr, rc);
		return (rc);
	}

	data &= ~0x0F; 
	data |=  mode;
	rc = bcm54980_shadow_reg_write (fd, inst, port_addr,  addr, data);
	if (rc) {
		printf("Error: write addr=0x%X (rc=%d)\n", addr, rc);
		return (rc);
	}
	return (0);
}

int bcm54980_led_green_on (int fd, uint8_t inst, uint8_t port_addr)
{
	return (bcm54980_led_set (fd, inst, port_addr,  1, BCM54X80_LED_MODE_ON));
}

int bcm54980_led_green_off (int fd, uint8_t inst, uint8_t port_addr)
{
	return (bcm54980_led_set (fd, inst, port_addr,  1, BCM54X80_LED_MODE_OFF));
}

int bcm54980_led_red_on (int fd, uint8_t inst, uint8_t port_addr)
{
	return (bcm54980_led_set (fd, inst, port_addr,  0, BCM54X80_LED_MODE_ON));
}

int bcm54980_led_red_off (int fd, uint8_t inst, uint8_t port_addr)
{
	return (bcm54980_led_set (fd, inst, port_addr,  0, BCM54X80_LED_MODE_OFF));
}

int bcm54980_loopback_print (int fd, uint8_t inst, uint8_t port_addr)
{
	int rc = 0;
	uint8_t ext_lpbk = 0, line_lpbk = 0, pcs_lpbk = 0;
        uint16_t data, lpbk_b;

        lpbk_b = (1 << BCM54X80_MII_CONTROL_FLD_LOOPBACK);
        rc = bcm54980_reg_read (fd, inst, port_addr,  BCM54X80_MII_CONTROL, &data);
        if (rc) {
                printf("Error: read MII_CONTROL (rc=%d)\n", rc);
                return (rc);
        }
	pcs_lpbk = (data & lpbk_b) ? 1 : 0;


        lpbk_b = (1 << BCM54X80_AUX_SHADOW_CTRL_FLD_EXT_LPBK);
        rc = bcm54980_aux_shadow_reg_read (fd, inst, port_addr,  BCM54X80_AUX_SHADOW_CTRL, &data);
        if (rc) {
                printf("Error: BCM54X80_AUX_SHADOW_CTRL (rc=%d)\n", rc);
                return (rc);
        }
	ext_lpbk = (data & lpbk_b) ? 1 : 0;

        lpbk_b = (1 << BCM54X80_AUX_SHADOW_CTRL_FLD_EXT_LPBK);
        rc = bcm54980_aux_shadow_reg_read (fd, inst, port_addr,  
			BCM54X80_AUX_SHADOW_MISC_TEST, &data);
        if (rc) {
                printf("Error: BCM54X80_AUX_SHADOW_MISC_TEST (rc=%d)\n", rc);
                return (rc);
        }
	line_lpbk = (data & lpbk_b) ? 1 : 0;

	printf("\t\tPCS\tEXT\tLINE\n");
	printf("\t\t=================================================\n\n");
	printf("OCTPHY%d.%d\t%s\t%s\t%s", inst, port_addr,  
			pcs_lpbk ? "Ena" : "Dis",
			ext_lpbk ? "Ena" : "Dis",
			line_lpbk ? "Ena" : "Dis");

	return (rc);
}

int bcm54980_sgmii_pcs_loopback(int fd, uint8_t inst, uint8_t port_addr, uint8_t ena)
{
	return (bcm54980_exp_reg_write (fd, inst, port_addr, BCM54X80_EXP_LINE_LPBK_CONTROL, 
		 ena? 0x4 : 0x00));
}

int bcm54980_set_100mbps (int fd, uint8_t inst, uint8_t port_addr)
{
	int	 rc = 0;
	uint16_t data;

	data = (0x01 << BCM54X80_ANEG_ADV_FLD_100BTX_FD) |
		(0x01 << BCM54X80_ANEG_ADV_FLD_CLR_10_100);
	rc = bcm54980_reg_write (fd, inst, port_addr,  BCM54X80_ANEG_ADV, data);
	if (rc) {
		printf("  Error: Wr BCM54X80_ANEG_ADV [0x%X.%d] : 0x%X\n",
			BCM54X80_ANEG_ADV, port_addr, data);
		return (rc);
	}

	data = ((0x01 << BCM54X80_1000BT_CONTROL_FLD_MS_CFG_VAL) |
		(0x01 << BCM54X80_1000BT_CONTROL_FLD_REPEATER) |
		(0x01 << BCM54X80_1000BT_CONTROL_FLD_ADV_HD_ABLE));
	rc = bcm54980_reg_write (fd, inst, port_addr,  BCM54X80_1000BT_CONTROL, data);
	if (rc) {
		printf("  Error: Wr BCM54X80_1000BT_CONTROL [0x%X.%d] : 0x%X\n",
			BCM54X80_1000BT_CONTROL, port_addr, data);
		return (rc);
	}

	data = ((0x01 << BCM54X80_MII_CONTROL_FLD_CU_ANET_ENA) |
		(0x01 << BCM54X80_MII_CONTROL_FLD_RESTART_ANEG) |
		(0x01 << BCM54X80_MII_CONTROL_FLD_DUPLEX_MODE)  |
		(0x01 << BCM54X80_MII_CONTROL_FLD_SPEED_SEL_MSB));
	rc = bcm54980_reg_write (fd, inst, port_addr,  BCM54X80_MII_CONTROL, data);
	if (rc) {
		printf("  Error: Wr BCM54X80_MII_CONTROL [0x%X.%d] : 0x%X\n",
			BCM54X80_MII_CONTROL, port_addr, data);
		return (rc);
	}

	return (rc);
}


int bcm54980_set_10mbps (int fd, uint8_t inst, uint8_t port_addr)
{
	int	 rc = 0;
	uint16_t data;

	data = (0x01 << BCM54X80_ANEG_ADV_FLD_100BT_FD) |
		(0x01 << BCM54X80_ANEG_ADV_FLD_CLR_10_100);
	rc = bcm54980_reg_write (fd, inst, port_addr,  BCM54X80_ANEG_ADV, data);
	if (rc) {
		printf("  Error: Wr BCM54X80_ANEG_ADV [0x%X.%d] : 0x%X\n",
			BCM54X80_ANEG_ADV, port_addr, data);
		return (rc);
	}

	data = ((0x01 << BCM54X80_1000BT_CONTROL_FLD_MS_CFG_VAL) |
		(0x01 << BCM54X80_1000BT_CONTROL_FLD_REPEATER) |
		(0x01 << BCM54X80_1000BT_CONTROL_FLD_ADV_HD_ABLE));
	rc = bcm54980_reg_write (fd, inst, port_addr,  BCM54X80_1000BT_CONTROL, data);
	if (rc) {
		printf("  Error: Wr BCM54X80_1000BT_CONTROL [0x%X.%d] : 0x%X\n",
			BCM54X80_1000BT_CONTROL, port_addr, data);
		return (rc);
	}

	data = ((0x01 << BCM54X80_MII_CONTROL_FLD_CU_ANET_ENA) |
		(0x01 << BCM54X80_MII_CONTROL_FLD_RESTART_ANEG) |
		(0x01 << BCM54X80_MII_CONTROL_FLD_DUPLEX_MODE)  |
		(0x01 << BCM54X80_MII_CONTROL_FLD_SPEED_SEL_MSB));
	rc = bcm54980_reg_write (fd, inst, port_addr,  BCM54X80_MII_CONTROL, data);
	if (rc) {
		printf("  Error: Wr BCM54X80_MII_CONTROL [0x%X.%d] : 0x%X\n",
			BCM54X80_MII_CONTROL, port_addr, data);
		return (rc);
	}

	return (rc);
}
