/********************************************************************
 *
 *      File:   asic_acc.c
 *      Name:   Sudharshan Kadari 
 *
 *      Description: ASIC level functions for Redwood USD
 *
 *
 *      Copyright (c) 1984-2009 by Cisco Systems, Inc.
 ********************************************************************/
#include <stdio.h>
#include <stdint.h>
#include "ppc_lib.h"
#include "woo_ops.h"

// byte-swap 32-bit value. swap of a ntohl'd value is LITTLE_ENDIAN
#define swap_32(a) (((a >> 24) & 0xff)     |\
                    ((a >> 8)  & 0xff00)   |\
                    ((a << 8 ) & 0xff0000) |\
                     (a << 24))

#define _READ32(a) ((*((volatile uint32_t*)(a))) & 0xFFFFFFFF)
#define _WRITE32(a,v) (*((volatile uint32_t*)(a)) = ((uint32_t)(v) & 0xFFFFFFFF))
static uint32_t woo_mmap_addr = 0x0;

int asic_rd (uint8_t inst, uint32_t addr, uint32_t *data)
{
	int rc = 0;
        uint32_t pdata = 0;
        if ((woo_get_pci_driver()) == 1) {
		if (woo_mmap_addr == 0) {
			printf("Getting WOO base addr\n");
			woo_mmap_addr = woo_get_baseaddr(); 
		}

		pdata = _READ32(woo_mmap_addr+addr);
		*data = swap_32(pdata);
        } else {
		asic_i2c_rd(inst, addr, data);
        }
//	printf("Rd [0x%X] : 0x%X\n", addr, *data);
	return (rc);	
}

int asic_wr (uint8_t inst, uint32_t addr, uint32_t data)
{
	int rc = 0;
        uint32_t pdata = swap_32(data);
        if ((woo_get_pci_driver()) == 1) {
		if (woo_mmap_addr == 0) {
			printf(" Getting WOO base addr\n");
			woo_mmap_addr = woo_get_baseaddr(); 
		}
        	_WRITE32((woo_mmap_addr+addr),pdata);
        } else {
		asic_i2c_wr(inst, addr, data);
        }
	return (rc);
}

int asic_bset (uint8_t inst, uint32_t addr, uint8_t bnum)
{
        uint32_t data;
        int rc = 0;

        rc = asic_rd (inst, addr, &data);
        if (rc) return (rc);

        data |= (0x01 << bnum);
        return (asic_wr (inst, addr, data));
}

int asic_bclr (uint8_t inst, uint32_t addr, uint8_t bnum)
{
        uint32_t data;
        int rc = 0;

        rc = asic_rd (inst, addr, &data);
        if (rc) return (rc);

        data &= ~(0x01 << bnum);
        return (asic_wr (inst, addr, data));
}
