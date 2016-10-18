/********************************************************************
 *
 *      File:   asic_utils.c
 *      Name:   Sudharshan Kadari 
 *
 *      Description: ASIC level functions for Redwood USD
 *
 *
 *      Copyright (c) 1984-2009 by Cisco Systems, Inc.
 ********************************************************************/
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include "woo_ops.h"

typedef struct _IRQ_REG_ {
    char *name;
    uint32_t offset;
} asic_irq_reg_t;

static int ci_irq_regs (int asic_id, uint8_t rd_flag)
{
	uint8_t  cnt;
	uint32_t count, regaddr, data, rc=0;
	uint32_t ci_addr[] = 
			{  ASIC(MISC_INT_ERR)
			,  ASIC(BI_GBE_INT)
			,  ASIC(BI_INT_VEC)
			,  ASIC(CI_INT_VEC)
			,  ASIC(CI_INT_ERR_VEC)
			,  ASIC(CI_GBE_INT)
			,  ASIC(PC_INT_ERR)
			,  ASIC(RS_PCIE_INT_ERR)
			};
	char* ci_name[] = 
			{  "MISC_INT_ERR"
			,  "BI_GBE_INT"
			,  "BI_INT_VEC"
			,  "BI_INT_VEC"
			,  "CI_INT_VEC"
			,  "CI_INT_ERR_VEC"
			,  "CI_GBE_INT"
			,  "PC_INT_ERR"
			,  "RS_PCIE_INT_ERR"
			};
	data  = 0xFFFFFFFF;
	count = sizeof(ci_addr)/sizeof(uint32_t);
	for (cnt = 0; !rc && cnt < count; cnt++) {
		regaddr = BA(ci_addr[cnt]); 
		if (rd_flag) {
			rc = asic_rd(asic_id, regaddr, &data);
			if (data) 
                            printf("[%-32s] 0x%08X: 0x%08X]\n", 
                                  ci_name[cnt], regaddr, data);
		} else {
			rc = asic_wr(asic_id, regaddr, data);
		}
	}
        return (rc);
}

static int hi_irq_regs (int asic_id, uint8_t rd_flag)
{
	uint8_t  cnt;
	uint32_t count, regaddr, base_addr, data, rc=0, port;
	uint32_t hi_addr[] = 
			{  ASIC(HI_INT_ERR_OFFSET)
			,  ASIC(HI_INT_NORM_OFFSET)
#ifndef DIAG_PORTOLA
			,  ASIC(HI_INT_XGMII_INTERRUPT_OFFSET)
			,  ASIC(HI_CT_INT_MUX_OFFSET)
			,  ASIC(HI_XE_INT_XG_OFFSET)
			,  ASIC(HI_GE_INT_OFFSET)
#endif
			};
	char* hi_name[] = 
			{  "HI_INT_ERR"
			,  "HI_INT_NORM"
#ifndef DIAG_PORTOLA
			,  "HI_INT_XGMII_INTERRUPT"
			,  "HI_CT_INT_MUX"
			,  "HI_XE_INT_XG"
			,  "HI_GE_INT"
#endif
			};
	data  = 0xFFFFFFFF;
	count = sizeof(hi_addr)/sizeof(uint32_t);
	for (port = 0; !rc && port < ASIC(HI_COUNT); port++) {
		base_addr = BA(ASIC(HI0_BASE_ADDR) + 
				(port*(ASIC(HI1_BASE_ADDR)-ASIC(HI0_BASE_ADDR))));
		for (cnt = 0; !rc && cnt < count; cnt++) {
			regaddr = base_addr + BA(hi_addr[cnt]);
			if (rd_flag) {
				rc = asic_rd(asic_id, regaddr, &data);
				if (data) 
                                    printf("[%-32s] 0x%08X (P%d): 0x%08X]\n", 
                                           hi_name[cnt], regaddr, port, data);
			} else {
				rc = asic_wr(asic_id, regaddr, data);
			}
		}
	}
        return (rc);
}

#ifndef DIAG_PORTOLA
static int hi_cnt_regs (int asic_id, uint8_t rd_flag)
{
	uint8_t  cnt;
	uint32_t count, regaddr, base_addr, data, rc=0, port;
	uint32_t hi_cnt_addr[] = 
			{  ASIC(HI_XE_CNT_XG_CODE_VIOLATION_OFFSET)};
	char* hi_cnt_name[] = 
			{  "HI_XE_CNT_XG_CODE_VIOLATION_OFFSET"};

	count = sizeof(hi_cnt_addr)/sizeof(uint32_t);
	for (port = 0; !rc && port < ASIC(HI_COUNT); port++) {
		base_addr = BA(ASIC(HI0_BASE_ADDR) + 
				(port*(ASIC(HI1_BASE_ADDR)-ASIC(HI0_BASE_ADDR))));
		for (cnt = 0; !rc && cnt < count; cnt++) {
			regaddr = base_addr + BA(hi_cnt_addr[cnt]);
			if (rd_flag) {
				rc = asic_rd(asic_id, regaddr, &data);
				if (data) 
                                    printf("[%-32s] 0x%08X (P%d): 0x%08X]\n", 
                                           hi_cnt_name[cnt], regaddr, 
                                           port, data);
			} else {
				rc = asic_wr(asic_id, regaddr, 0x00);
			}
		}
	}
        return (rc);
}
#endif
static int ni_irq_regs (int asic_id, uint8_t rd_flag)
{
	uint8_t  cnt;
	uint32_t count, regaddr, base_addr, data, rc=0, port;
	uint32_t ni_addr[] = 
			{  ASIC(NI_INT_ERR_OFFSET)
			,  ASIC(NI_INT_NORM_OFFSET)
			,  ASIC(NI_INT_XGMII_INTERRUPT_OFFSET)
			,  ASIC(NI_CT_INT_MUX_OFFSET)
			,  ASIC(NI_XE_INT_XG_OFFSET)
			};
	char* ni_name[] = 
			{  "NI_INT_ERR"
			,  "NI_INT_NORM"
			,  "NI_INT_XGMII_INTERRUPT"
			,  "NI_CT_INT_MUX"
			,  "NI_XE_INT_XG"
			};
	data  = 0xFFFFFFFF;
	count = sizeof(ni_addr)/sizeof(uint32_t);
	for (port = 0; !rc && port < ASIC(NI_COUNT); port++) {
		base_addr = BA(ASIC(NI0_BASE_ADDR) + 
				(port*(ASIC(NI1_BASE_ADDR)-ASIC(NI0_BASE_ADDR))));
		for (cnt = 0; !rc && cnt < count; cnt++) {
			regaddr = base_addr + BA(ni_addr[cnt]);
			if (rd_flag) {
				rc = asic_rd(asic_id, regaddr, &data);
				if (data) printf("[%-32s] 0x%08X (P%d): 0x%08X]\n", 
                                                 ni_name[cnt], regaddr, port, data);
			} else {
				rc = asic_wr(asic_id, regaddr, data);
			}
		}
	}
        return (rc);
}

static int ni_cnt_regs (int asic_id, uint8_t rd_flag)
{
	uint8_t  cnt;
	uint32_t count, regaddr, base_addr, data, rc=0, port;
	uint32_t ni_cnt_addr[] = 
			{  ASIC(NI_XE_CNT_XG_CODE_VIOLATION_OFFSET)};
	char* ni_cnt_name[] = 
			{  "NI_XE_CNT_XG_CODE_VIOLATION_OFFSET"};
	count = sizeof(ni_cnt_addr)/sizeof(uint32_t);
	for (port = 0; !rc && port < ASIC(NI_COUNT); port++) {
		base_addr = BA(ASIC(NI0_BASE_ADDR) + 
				(port*(ASIC(NI1_BASE_ADDR)-ASIC(NI0_BASE_ADDR))));
		for (cnt = 0; !rc && cnt < count; cnt++) {
			regaddr = base_addr + BA(ni_cnt_addr[cnt]);
			if (rd_flag) {
				rc = asic_rd(asic_id, regaddr, &data);
				if (data) 
                                    printf("[%-32s] 0x%08X (P%d): 0x%08X]\n", 
                                           ni_cnt_name[cnt], regaddr, port, data);
			} else {
				rc = asic_wr(asic_id, regaddr, 0x00);
			}
		}
	}
        return (rc);
}

static int hf_nf_irq_regs (int asic_id, uint8_t rd_flag)
{
	uint8_t  cnt;
	uint32_t count, regaddr, base_addr, data, rc=0, port;
	asic_irq_reg_t hf_regs[] = {
                 {"HF_INT_ERR",  ASIC(HF_INT_ERR_OFFSET)},
		 {"HF_INT_NORM",  ASIC(HF_INT_NORM_OFFSET)}
	};
	asic_irq_reg_t nf_regs[] = {
                 {"NF_INT_ERR",  ASIC(NF_INT_ERR)},
		 {"NF_INT_NORM", ASIC(NF_INT_NORM)}
	};
	data  = 0xFFFFFFFF;
	count = sizeof(hf_regs)/sizeof(asic_irq_reg_t);
	for (port = 0; !rc && port < ASIC(HF_COUNT); port++) {
		base_addr = BA(ASIC(HF0_BASE_ADDR) + 
				(port*(ASIC(HF1_BASE_ADDR)-ASIC(HF0_BASE_ADDR))));
		for (cnt = 0; !rc && cnt < count; cnt++) {
			regaddr = base_addr + BA(hf_regs[cnt].offset);
			if (rd_flag) {
				rc = asic_rd(asic_id, regaddr, &data);
				if (data)
					printf("[%-32s] 0x%08X (P%d): 0x%08X]\n", 
                                               hf_regs[cnt].name, 
						regaddr, port, data);
			} else {
				rc = asic_wr(asic_id, regaddr, data);
			}
		}
	}

	count = sizeof(nf_regs)/sizeof(asic_irq_reg_t);
	for (cnt = 0; !rc && cnt < count; cnt++) {
	    regaddr = BA(nf_regs[cnt].offset);
	    if (rd_flag) {
	        rc = asic_rd(asic_id, regaddr, &data);
	        if (data)
	            printf("[%-32s] 0x%08X : 0x%08X]\n", 
                           nf_regs[cnt].name, regaddr, data);
	    } else {
	        rc = asic_wr(asic_id, regaddr, data);
	    }
        }
        return (rc);
}

static int ss_irq_regs (int asic_id, uint8_t rd_flag)
{
	uint8_t  cnt;
	uint32_t count, regaddr, base_addr, data, rc=0, port;
	uint32_t ss_addr[] = 
			{  ASIC(SS_INT_NORM_OFFSET)
			,  ASIC(SS_INT_ERR_ECC1_OFFSET)
			,  ASIC(SS_INT_ERR_ECC2_OFFSET)
			,  ASIC(SS_INT_ERR_OFFSET)
			,  ASIC(SS_INT_NORM_TD_OFFSET)
			};
	char* ss_name[] = 
			{  "SS_INT_NORM"
			,  "SS_INT_ERR_ECC1"
			,  "SS_INT_ERR_ECC2"
			,  "SS_INT_ERR"
			,  "SS_INT_NORM_TD"
			};
	data  = 0xFFFFFFFF;
	count = sizeof(ss_addr)/sizeof(uint32_t);
	for (port = 0; !rc && port < ASIC(SS_COUNT); port++) {
		base_addr = BA(ASIC(SS0_BASE_ADDR) + 
				(port*(ASIC(SS1_BASE_ADDR)-ASIC(SS0_BASE_ADDR))));
		for (cnt = 0; !rc && cnt < count; cnt++) {
			regaddr = base_addr + BA(ss_addr[cnt]);
			if (rd_flag) {
				rc = asic_rd(asic_id, regaddr, &data);
				if (data) {
					printf("[%-32s] 0x%08X (P%d): 0x%08X]\n", 
                                               ss_name[cnt], regaddr, port, 
                                               data);
				}
			} else {
				rc = asic_wr(asic_id, regaddr, data);
			}
		}
	}
        return (rc);
}


static int hr_nr_irq_regs (int asic_id, uint8_t rd_flag)
{
	uint32_t regaddr, data, rc=0, oq, port=0;
	asic_irq_reg_t hr_oq_port[] = { 
	         {"HR_INT_OQ0_PORT",ASIC(HR_INT_OQ0_PORT0)},
	         {"HR_INT_OQ1_PORT",ASIC(HR_INT_OQ1_PORT0)},
		 {"HR_INT_OQ2_PORT",ASIC(HR_INT_OQ2_PORT0)},
		 {"HR_INT_OQ3_PORT",ASIC(HR_INT_OQ3_PORT0)},
#if defined(DIAG_PORTOLA)
		 {"HR_INT_OQ4_PORT",ASIC(HR_INT_OQ4_PORT0)},
		 {"HR_INT_OQ5_PORT",ASIC(HR_INT_OQ5_PORT0)}
#endif
	};
	asic_irq_reg_t hr_oq[] = { 
	         {"HR_INT_OQ0_DDROP",ASIC(HR_INT_OQ0_DDROP)},
	         {"HR_INT_OQ1_DDROP",ASIC(HR_INT_OQ1_DDROP)},
		 {"HR_INT_OQ2_DDROP",ASIC(HR_INT_OQ2_DDROP)},
		 {"HR_INT_OQ3_DDROP",ASIC(HR_INT_OQ3_DDROP)},
#if defined(DIAG_PORTOLA)
		 {"HR_INT_OQ4_DDROP",ASIC(HR_INT_OQ4_DDROP)},
		 {"HR_INT_OQ5_DDROP",ASIC(HR_INT_OQ5_DDROP)}
#endif
        };
	asic_irq_reg_t nr_oq_port[] = { 
	         {"NR_INT_OQ0_PORT",ASIC(NR_INT_OQ0_PORT0)},
	         {"NR_INT_OQ1_PORT",ASIC(NR_INT_OQ1_PORT0)},
		 {"NR_INT_OQ2_PORT",ASIC(NR_INT_OQ2_PORT0)},
		 {"NR_INT_OQ3_PORT",ASIC(NR_INT_OQ3_PORT0)},
#if defined(DIAG_PORTOLA)
		 {"NR_INT_OQ4_PORT",ASIC(NR_INT_OQ4_PORT0)},
		 {"NR_INT_OQ5_PORT",ASIC(NR_INT_OQ5_PORT0)}
#endif
	};
	asic_irq_reg_t nr_oq[] = { 
	         {"NR_INT_OQ0_DDROP",ASIC(NR_INT_OQ0_DDROP)},
	         {"NR_INT_OQ1_DDROP",ASIC(NR_INT_OQ1_DDROP)},
		 {"NR_INT_OQ2_DDROP",ASIC(NR_INT_OQ2_DDROP)},
		 {"NR_INT_OQ3_DDROP",ASIC(NR_INT_OQ3_DDROP)},
#if defined(DIAG_PORTOLA)
		 {"NR_INT_OQ4_DDROP",ASIC(NR_INT_OQ4_DDROP)},
		 {"NR_INT_OQ5_DDROP",ASIC(NR_INT_OQ5_DDROP)}
#endif
        };
	data  = 0xFFFFFFFF;
	for (oq = 0; !rc && oq < ASIC(OQ_COUNT); oq++) {
	    regaddr = BA(hr_oq[oq].offset); 
	    if (rd_flag) {
	        rc = asic_rd(asic_id, regaddr, &data);
	        if (data) {
	            printf("[%-32s%d] 0x%08X : 0x%08X]\n", 
                           hr_oq[oq].name, port,regaddr, data);
	        }
	    } else {
	        rc = asic_wr(asic_id, regaddr, data);
	    }

	    regaddr = BA(hr_oq_port[oq].offset); 

	    for (port = 0; !rc && port < ASIC(OQ_PORT_COUNT); port++) {
	        if (rd_flag) {
	            rc = asic_rd(asic_id, regaddr, &data);
	            if (data) {
	                printf("[%-32s%d] 0x%08X : 0x%08X]\n", 
                               hr_oq_port[oq].name, port,regaddr, data);
	            }
	        } else {
	            rc = asic_wr(asic_id, regaddr, data);
	        }
	        regaddr += BA(ASIC(HR_INT_OQ0_PORT1) - ASIC(HR_INT_OQ0_PORT0));
	    }

	    regaddr = BA(nr_oq[oq].offset); 
	    if (rd_flag) {
	        rc = asic_rd(asic_id, regaddr, &data);
	        if (data) {
	            printf("[%-32s%d] 0x%08X : 0x%08X]\n", 
                           nr_oq[oq].name, port,regaddr, data);
	        }
	    } else {
	        rc = asic_wr(asic_id, regaddr, data);
	    }

	    regaddr = BA(nr_oq_port[oq].offset); 
	    for (port = 0; !rc && port < ASIC(OQ_PORT_COUNT); port++) {
	        if (rd_flag) {
	            rc = asic_rd(asic_id, regaddr, &data);
	            if (data) {
	                printf("[%-32s%d] 0x%08X : 0x%08X]\n", 
                               nr_oq_port[oq].name, port,regaddr, data);
	            }
	        } else {
	            rc = asic_wr(asic_id, regaddr, data);
	        }
	        regaddr += BA(ASIC(NR_INT_OQ0_PORT1) - ASIC(NR_INT_OQ0_PORT0));
	    }
	}
        return (rc);
}

int asic_irq_regs (int asic_id, char* action)
{
	uint8_t  rd_flag;
	uint32_t rc=0;

	rd_flag = (!strncasecmp(action, "clrirq", strlen("clrirq"))) ? 0:1;

        rc = ci_irq_regs(asic_id, rd_flag);
        rc = hi_irq_regs(asic_id, rd_flag);

#ifndef DIAG_PORTOLA
        rc = hi_cnt_regs(asic_id, rd_flag);
#endif
        rc = ni_irq_regs(asic_id, rd_flag);
        rc = ni_cnt_regs(asic_id, rd_flag);
        rc = hf_nf_irq_regs(asic_id, rd_flag);
        rc = ss_irq_regs(asic_id, rd_flag);
        rc = hr_nr_irq_regs(asic_id, rd_flag);

   	return (rc);	
}
