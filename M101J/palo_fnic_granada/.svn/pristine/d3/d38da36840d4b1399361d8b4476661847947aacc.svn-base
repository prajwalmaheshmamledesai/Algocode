/********************************************************************
 *
 *      File:   woo_mdio.c
 *      Name:   Sudharshan Kadari (Original : vpottumu)
 *
 *      Description: MDIO level functions for woodside
 *
 *      Copyright (c) 2009 by Cisco Systems, Inc.
 ********************************************************************/
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include "woo_woo_naxislave_define.h"
#include "woo_ops.h"

#define RETURN(rc)	if (rc) {printf("%s:%d (rc=%d)\n", __func__, __LINE__, rc); return (rc);}

static int asic_mdio_busy_check (uint8_t inst, uint32_t mdio_reg_num)
{
	int rc = 0, delay=200;
	uint32_t data;

	do {
		rc = asic_rd (inst, BA(ASIC(MISC_STA_MDIO0) + mdio_reg_num), &data);
		if ((rc) || !(data & 0x80000000)) {
                    return (rc);
                }
	} while (--delay);

	return (-1);
}

static
int asic_mdio_addr_cyc ( uint8_t  inst, 
			uint32_t mdio_num,
			uint32_t reg_addr,
			uint32_t prt_addr,
			uint32_t dev_addr) 
{
	int cmd_addr, rc;
	int mdio_reg_num = mdio_num*5;

	if ((rc = asic_mdio_busy_check (inst, mdio_reg_num))) {
		printf("  Error: MDIO Busy"
			"(mdio=%d, reg=0x%X, Port=%d, dev=%d\n",
			mdio_num, reg_addr, prt_addr, dev_addr);
		return (rc);
	}
	// ADDRESS CYCLE
	rc = asic_wr (inst, BA(ASIC(MISC_CFG_MDIO0_DATA) + mdio_reg_num), reg_addr);

	cmd_addr = (prt_addr << 5) + dev_addr;
	rc = asic_wr(inst, BA(ASIC(MISC_CFG_MDIO0_CMD) + mdio_reg_num), cmd_addr);

	return (0);
}

static
int asic_mdio_wdata_cyc (uint8_t  inst,
			uint32_t mdio_num,
			uint32_t reg_addr,
			uint32_t prt_addr,
			uint32_t dev_addr,	
			uint32_t reg_data) 
{
	uint32_t cmd_data;
	int 	rc, mdio_reg_num = mdio_num*5;
	if ((rc = asic_mdio_busy_check (inst, mdio_reg_num))) {
		printf("  Error: MDIO Busy"
			"(mdio=%d, reg=0x%X, Port=%d, dev=%d\n",
			mdio_num, reg_addr, prt_addr, dev_addr);
		return (rc);
	}

	//WRITE CYCLE
	rc = asic_wr (inst, BA(ASIC(MISC_CFG_MDIO0_DATA) + mdio_reg_num), reg_data);
    	RETURN(rc);
 
	cmd_data = (1<<14) + (prt_addr << 5) + dev_addr;
	rc = asic_wr (inst, BA(ASIC(MISC_CFG_MDIO0_CMD) + mdio_reg_num), cmd_data);
	RETURN (rc);

	return (0);
}

static
int asic_mdio_rdata_inc (uint8_t  inst,
			uint32_t mdio_num,
			uint32_t reg_addr,
			uint32_t prt_addr,
			uint32_t dev_addr,	
			uint8_t  inc,
			uint32_t *reg_data)
{
	int rc;
	int mdio_reg_num = mdio_num*5;
	uint32_t data;

	if ((rc = asic_mdio_busy_check (inst, mdio_reg_num))) {
		printf("  Error: MDIO Busy"
			"(mdio=%d, reg=0x%X, Port=%d, dev=%d\n",
			mdio_num, reg_addr, prt_addr, dev_addr);
		return (rc);
	}
	data = ((inc? 2 : 3) << 14) + (prt_addr << 5) + dev_addr;
	rc = asic_wr (inst, BA(ASIC(MISC_CFG_MDIO0_CMD) + mdio_reg_num), data);
	RETURN(rc);

	if ((rc = asic_mdio_busy_check (inst, mdio_reg_num))) {
		printf("  Error: MDIO Busy"
			"(mdio=%d, reg=0x%X, Port=%d, dev=%d\n",
			mdio_num, reg_addr, prt_addr, dev_addr);
		return (rc);
	}

	rc = asic_rd (inst, BA(ASIC(MISC_STA_MDIO0) + mdio_reg_num), &data);
	RETURN(rc);
	*reg_data = data & 0xFFFF;
	return (0);
} 

static
int asic_mdio_rdata_cyc (uint8_t  inst,
			uint32_t mdio_num,
			uint32_t reg_addr,
			uint32_t prt_addr,
			uint32_t dev_addr,	
			uint32_t *reg_data) 
{
	return (asic_mdio_rdata_inc (inst, mdio_num, reg_addr, prt_addr, dev_addr, 0, reg_data)); 
}

int asic_mdio_write (uint8_t  inst,
		    uint32_t mdio_num,
		    uint32_t prt_addr,
		    uint32_t dev_addr,
		    uint32_t reg_addr,
		    uint32_t reg_data) 
{
	int rc = 0;
	// ADDRESS CYCLE
	rc = asic_mdio_addr_cyc (inst, mdio_num, reg_addr, prt_addr, dev_addr);
	RETURN(rc);

	//Data WRITE CYCLE
	rc = asic_mdio_wdata_cyc(inst, mdio_num, reg_addr, prt_addr, dev_addr,	reg_data);
	RETURN(rc);

	return (rc);
}

int asic_mdio_read (uint8_t  inst,
		   uint32_t mdio_num,
		   uint32_t prt_addr,
		   uint32_t dev_addr,
		   uint32_t reg_addr,
		   uint32_t *reg_data) 
{
	int rc = 0;
	// ADDRESS CYCLE
	rc = asic_mdio_addr_cyc (inst, mdio_num, reg_addr, prt_addr, dev_addr);
	RETURN(rc);

	//READ CYCLE
	rc = asic_mdio_rdata_cyc(inst, mdio_num, reg_addr, prt_addr, dev_addr,	reg_data); 
	RETURN(rc);

	return (rc);
}

int asic_mdio_inc_read ( uint8_t  inst,
			uint32_t mdio_num, 
			uint32_t reg_addr,
			uint32_t prt_addr,
			uint32_t dev_addr,
			uint32_t *reg_data) 
{
	return (asic_mdio_rdata_inc (inst, mdio_num, reg_addr, prt_addr, dev_addr, 1, reg_data)); 
}

extern int fext_get_hw_revision(uint16_t *rev_maj, uint16_t *rev_min);
extern int fext_get_card_index(uint16_t*);

static int prt_mdio_reg_num_get (int bcm_inst)
{
        static uint16_t rev_maj=0, rev_min = 0, flag = 0;
	static uint16_t card_index = 0;
        uint32_t mdio_reg_num;

        if (!flag) {
                fext_get_hw_revision(&rev_maj, &rev_min);
               	fext_get_card_index(&card_index); 
                flag = 1;
        }

	if (card_index == 11035) {
		mdio_reg_num = ((bcm_inst>>3)+1)*5;
	} else {
		if (rev_maj > 1)  {
			if ((bcm_inst >=16) && (bcm_inst <= 31)) {
				mdio_reg_num = 5; /* MDIO-XGBE2 */
			} else if ((bcm_inst >=32) && (bcm_inst <= 47)) {
				mdio_reg_num = 10; /* MDIO-XGBE3 */
			} else {
				mdio_reg_num = 15; /* MDIO-XGBE4 */
			}
		} else {
			mdio_reg_num = ((bcm_inst>>4)+1)*5;
		}
        }

        return (mdio_reg_num);
}

int prt_mdio_clause22_read (uint8_t  inst,
		   uint32_t bcm_inst,
		   uint32_t prt_addr,
		   uint32_t reg_addr,
		   uint32_t *reg_data) 
{
	int rc = 0;
	uint32_t cmd_data, data;
	uint32_t mdio_reg_num = prt_mdio_reg_num_get(bcm_inst);

	if ((rc = asic_mdio_busy_check (inst, mdio_reg_num))) {
		printf("  Error: MDIO Busy"
			"(bcm_inst=%d, reg=0x%X, Port=%d\n",
			bcm_inst, reg_addr, prt_addr);
		return (rc);
	}

	cmd_data = (1 << ASIC(MISC_CFG_MDIO0_CMD_FLD_READ)) + 
		   (1 << ASIC(MISC_CFG_MDIO0_CMD_FLD_CL22)) + 
		   (prt_addr << 5) + reg_addr;

	rc = asic_wr (inst, BA(ASIC(MISC_CFG_MDIO0_CMD) + mdio_reg_num), cmd_data);
	if (rc) return (rc);

	if ((rc = asic_mdio_busy_check (inst, mdio_reg_num))) {
		printf("  Error: MDIO Busy"
			"(bcm_inst=%d, reg=0x%X, Port=%d\n",
			bcm_inst, reg_addr, prt_addr);
		return (rc);
	}

	rc = asic_rd (inst, BA(ASIC(MISC_STA_MDIO0) + mdio_reg_num), &data);
	if (rc) return (rc);

	*reg_data = data & 0xFFFF;
	return (rc);
}

int prt_mdio_clause22_write (uint8_t  inst,
		   uint32_t bcm_inst,
		   uint32_t prt_addr,
		   uint32_t reg_addr,
		   uint32_t reg_data) 
{
	int rc = 0;
	uint32_t cmd_data, mdio_reg_num = prt_mdio_reg_num_get(bcm_inst);

	if ((rc = asic_mdio_busy_check (inst, mdio_reg_num))) {
		printf("  Error: MDIO Busy"
			"(mdio=%d, reg=0x%X, Port=%d\n",
			bcm_inst, reg_addr, prt_addr);
		return (rc);
	}

	rc = asic_wr (inst, BA(ASIC(MISC_CFG_MDIO0_DATA) + mdio_reg_num), reg_data);
	if (rc) return (rc);

	cmd_data = (1 << ASIC(MISC_CFG_MDIO0_CMD_FLD_CL22)) + 
		   (1 << ASIC(MISC_CFG_MDIO0_CMD_FLD_RD_INC)) + 
		   (prt_addr << 5) + reg_addr;
	rc = asic_wr (inst, BA(ASIC(MISC_CFG_MDIO0_CMD) + mdio_reg_num), cmd_data);
	if (rc) return (rc);

	if ((rc = asic_mdio_busy_check (inst, mdio_reg_num))) {
		printf("  Error: MDIO Busy"
			"(mdio=%d, reg=0x%X, Port=%d\n",
			bcm_inst, reg_addr, prt_addr);
		return (rc);
	}

	return (rc);
}
