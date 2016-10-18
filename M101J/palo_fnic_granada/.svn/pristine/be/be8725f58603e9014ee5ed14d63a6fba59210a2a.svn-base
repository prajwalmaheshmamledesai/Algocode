/********************************************************************
 *
 *      File:   pmbus_lib.c
 *      Name:   Sudharshan Kadari
 *
 *      Description:
 *       Digital PWM System Controller library 
 *
 *
 * Copyright (c) 1985-2009 by Cisco Systems, Inc.
 * All rights reserved.
 *
 *
 *********************************************************************/

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include "pmbus.h"
#include "pmbus_lib.h"

// Convert 11 bit PMBUS Exponent and Mantissa to Floating Point
void pmbus_mant_exp_to_fp_print (uint16_t mant, uint8_t exp, char *mystr, char *punits)
{
	exp = exp & 0x1F; // 5 bit exponent
	if (exp > 15) {
		exp = 0x20-exp;
		printf("  %-18s : %8.4f %s\n", mystr, ((1.0*mant)/(1<<exp)), punits);
	} else {
		printf("  %-18s : %8.4f %s\n", mystr, 1.0*mant*(1<<exp), punits);
	}
}


void pmbus_mant_exp_to_fp_scale_print (uint16_t mant, uint8_t exp, char *mystr, char *punits, int scale)
{
	exp = exp & 0x1F; // 5 bit exponent
	if (exp > 15) {
		exp = 0x20-exp;
		printf("  %-18s : %8.4f %s\n", mystr, ((1.0*mant*scale)/(1000 * (1<<exp))), punits);
	} else {
		printf("  %-18s : %8.4f %s\n", mystr, (1.0*mant*scale*(1<<exp))/1000, punits);
	}
}


// Reading Voltages
int pmbus_voltage_wr (int fd, uint8_t bus, uint8_t i2c_addr, uint8_t exp, uint16_t data16)
{
	int	rc = 0;
	uint8_t data, mode;

	rc = pmbus_rd(fd, bus, i2c_addr, PMBUS_VOUT_MODE, &data);
	if (rc) {
		printf("  Error: Read Voltage Mode(rc=%d)\n", rc);
		return (rc);
	}

	mode = (data >> 5) & 0x07;
	if ((mode == 0) || (mode == 1)) {
		data = (mode << 5) | (exp & 0x1F); 
		rc = pmbus_wr(fd, bus, i2c_addr, PMBUS_VOUT_MODE, data);
		if (rc) {
			printf("  Error: Read Voltage Mode(rc=%d)\n", rc);
			return (rc);
		}
	} else if (mode != 2) {
		printf("  Error: Invalid mode=%d\n", mode);
		return (-1);
	} 

	rc = pmbus_wr16(fd, bus, i2c_addr, PMBUS_VOUT_COMMAND, &data16);
	if (rc) {
		printf("  Error: Read VOUT command(rc=%d)\n", rc);
		return (rc);
	}

	return (rc);
}

// write protect
int pmbus_write_protect_ena (int fd, uint8_t bus, uint8_t i2c_addr, uint8_t wp)
{
	return(pmbus_wr(fd, bus, i2c_addr, PMBUS_WRITE_PROTECT, wp));
}

int pmbus_write_protect_dis (int fd, uint8_t bus, uint8_t i2c_addr)
{
	return(pmbus_wr(fd, bus, i2c_addr, PMBUS_WRITE_PROTECT, 0x00));
}

// Operation set
int pmbus_operation (int fd, uint8_t bus, uint8_t i2c_addr, uint8_t oper)
{
	return(pmbus_wr(fd, bus, i2c_addr, PMBUS_OPERATION, oper));
}

// Operation set
int pmbus_broadcast_operation (int fd, uint8_t bus, uint8_t i2c_addr, uint8_t oper)
{
	return(pmbus_broadcast_wr(fd, bus, i2c_addr, PMBUS_OPERATION, oper));
}

// Operation set
int pmbus_status_byte (int fd, uint8_t bus, uint8_t i2c_addr)
{
	int rc = 0;
	uint8_t data = 0;

	rc = pmbus_rd(fd, bus, i2c_addr, PMBUS_STATUS_BYTE, &data);
	if (rc) {
		printf("  Error: Read STATUS Byte command(rc=%d)\n", rc);
		return (rc);
	}

	if (data & PMBUS_STATUS_BYTE_BUSY)
		printf("  PMBUS (i2c=0x%X) Status is Busy\n", i2c_addr);
	if (data & PMBUS_STATUS_BYTE_OFF)
		printf("  PMBUS (i2c=0x%X) Status is Off\n", i2c_addr);
	if (data & PMBUS_STATUS_BYTE_VOUT_OV)
		printf("  PMBUS (i2c=0x%X) Status is VOUT Over Voltage\n", i2c_addr);
	if (data & PMBUS_STATUS_BYTE_IOUT_OC)
		printf("  PMBUS (i2c=0x%X) Status is IOUT Over Current\n", i2c_addr);
	if (data & PMBUS_STATUS_BYTE_VIN_UV)
		printf("  PMBUS (i2c=0x%X) Status is VIN Under Voltage\n", i2c_addr);
	if (data & PMBUS_STATUS_BYTE_TEMPERATURE)
		printf("  PMBUS (i2c=0x%X) Status is Temperature Fault\n", i2c_addr);
	if (data & PMBUS_STATUS_BYTE_CML)
		printf("  PMBUS (i2c=0x%X) Status is Memory or Logic Fault\n", i2c_addr);
	if (data & PMBUS_STATUS_BYTE_OTHER)
		printf("  PMBUS (i2c=0x%X) Status is Other Fault\n", i2c_addr);

	return (data);
}


int pmbus_scale_mode_rd (int fd, uint8_t bus, uint8_t i2c_addr, uint8_t addr, char *mystr, char *units, int scale)
{
	int	rc = 0;
	uint8_t data, mode, exp;
	uint16_t data16;

	rc = pmbus_rd(fd, bus, i2c_addr, PMBUS_VOUT_MODE, &data);
	if (rc) {
		printf("  Error: Read Voltage Mode(rc=%d)\n", rc);
		return (rc);
	}

	mode = (data >> 5) & 0x07;
	exp  = (data&0x1F);

	rc = pmbus_rd16(fd, bus, i2c_addr, addr, &data16);
	if (rc) {
		printf("  Error: Read VOUT command(rc=%d)\n", rc);
		return (rc);
	}

	if (!mode) {
		// Linear Voltage Format
		pmbus_mant_exp_to_fp_scale_print (data16, exp, mystr, units,scale);
	} else if (mode == 1)  {
		// VID Voltage Format
		printf(" Mode is VID (vid code = 0x%X Daat=0x%X)\n", exp, data16);
	} else if (mode == 2) {
		// DIRECT Voltage Format
		printf(" Mode is Direct\n");
	} else {
		printf("  Error: Invalid mode=%d\n", mode);
		return (-1);
	} 
	return (rc);
}

int pmbus_data_mode_rd (int fd, uint8_t bus, uint8_t i2c_addr, uint8_t addr, char *mystr, char *units)
{
	int	rc = 0;
	uint8_t data, mode, exp;
	uint16_t data16;

	rc = pmbus_rd(fd, bus, i2c_addr, PMBUS_VOUT_MODE, &data);
	if (rc) {
		printf("  Error: Read Voltage Mode(rc=%d)\n", rc);
		return (rc);
	}

	mode = (data >> 5) & 0x07;
	exp  = (data&0x1F);

	rc = pmbus_rd16(fd, bus, i2c_addr, addr, &data16);
	if (rc) {
		printf("  Error: Read VOUT command(rc=%d)\n", rc);
		return (rc);
	}

	if (!mode) {
		// Linear Voltage Format
		pmbus_mant_exp_to_fp_print (data16, exp, mystr, units);
	} else if (mode == 1)  {
		// VID Voltage Format
		printf(" Mode is VID (vid code = 0x%X Daat=0x%X)\n", exp, data16);
	} else if (mode == 2) {
		// DIRECT Voltage Format
		printf(" Mode is Direct\n");
	} else {
		printf("  Error: Invalid mode=%d\n", mode);
		return (-1);
	} 
	return (rc);
}

int pmbus_direct_rd (int fd, uint8_t bus, uint8_t i2c_addr, 
		     uint8_t addr, char *mystr, char *units)
{
	int	rc = 0;
	uint8_t data, mode, exp;
	uint16_t data16;

	rc = pmbus_rd(fd, bus, i2c_addr, PMBUS_VOUT_MODE, &data);
	if (rc) {
		printf("  Error: Read Voltage Mode(rc=%d)\n", rc);
		return (rc);
	}

	mode = (data >> 5) & 0x07;
	exp  = (data&0x1F);

	rc = pmbus_rd16(fd, bus, i2c_addr, addr, &data16);
	if (rc) {
		printf("  Error: Read VOUT command(rc=%d)\n", rc);
		return (rc);
	}

	printf("  %-18s : %8.4f %s\n", mystr, (data16/10.0), units);
	return (rc);
}

int pmbus_data_literal_rd (int fd, uint8_t bus, uint8_t i2c_addr, uint8_t addr, char *mystr, char *units)
{
	int	rc = 0;
	uint8_t data, mode, exp;
	uint16_t data16;

	rc = pmbus_rd(fd, bus, i2c_addr, PMBUS_VOUT_MODE, &data);
	if (rc) {
		printf("  Error: Read Voltage Mode(rc=%d)\n", rc);
		return (rc);
	}

	mode = (data >> 5) & 0x07;

	rc = pmbus_rd16(fd, bus, i2c_addr, addr, &data16);
	if (rc) {
		printf("  Error: Read VOUT command(rc=%d)\n", rc);
		return (rc);
	}
	exp  = (data16 >> 11)&0x1F;
	data16 &= 0x7FF;

	if (!mode) {
		// Linear Voltage Format
		pmbus_mant_exp_to_fp_print (data16, exp, mystr, units);
	} else if (mode == 1)  {
		// VID Voltage Format
		printf(" Mode is VID (vid code = 0x%X Daat=0x%X)\n", exp, data16);
	} else if (mode == 2) {
		// DIRECT Voltage Format
		printf(" Mode is Direct\n");
	} else {
		printf("  Error: Invalid mode=%d\n", mode);
		return (-1);
	} 
	return (rc);
}

int pmbus_mfr_rd (int fd, uint8_t bus, uint8_t i2c_addr, uint8_t addr, char *mystr)
{
	int	rc = 0;
	uint8_t data[64];

	rc = pmbus_rd(fd, bus, i2c_addr, addr, data);
	if (rc) return(rc); 

        rc = pmbus_rd_blk (fd, bus, i2c_addr, addr, data, data[0]+1);
	if (rc) return (rc);
	if (mystr) {
		data[data[0]+1] = '\0';
		printf("  %-18s : %s\n", mystr, data+1);
	} else {
		int cnt;
		printf(" PMBUS I2c=0x%X [0x%X] : ", i2c_addr, addr);
		for (cnt = 0; cnt < data[0]; cnt++) {
			printf("%02X ", data[cnt+1]);
		}
		printf("\n");
	}
	return (rc);
}

int pmbus_margin_test (int fd, uint8_t bus, uint8_t i2c_addr, 
		     uint16_t vout_ref, uint16_t var, uint16_t *vread)
{
	int	rc = 0;
	uint8_t data, mode, exp;
	uint16_t vout;

	rc = pmbus_rd(fd, bus, i2c_addr, PMBUS_VOUT_MODE, &data);
	if (rc) {
		printf("  Error: Read Voltage Mode(rc=%d)\n", rc);
		return (rc);
	}

	mode = (data >> 5) & 0x07;
	exp  = (data&0x1F);

	rc = pmbus_rd16(fd, bus, i2c_addr, PMBUS_READ_VOUT, &vout);
	if (rc) {
		printf("  Error: Read VOUT command(rc=%d)\n", rc);
		return (rc);
	}

	if (!mode) {
		pmbus_mant_exp_to_fp_print (vout, exp, "PMBUS VOUT", "Volts");
	}

	if (!((vout < ((vout_ref * (100+var))/100)) && (vout > ((vout_ref * (100-var))/100)))) {
		*vread = vout;
		printf("  Vout = 0x%X Vref : 0x%X Exp Range : 0x%X - 0x%X\n", vout, 
			vout_ref,  ((vout_ref * (100-var))/100),
			((vout_ref * (100+var))/100));
		return(-1);
	}

	return (rc);
}

int pmbus_voltage_cfg (int fd, uint8_t bus, uint8_t i2c_addr, uint8_t page)
{
	int rc = 0;
	uint16_t volt_set, scale_loop;
	uint8_t mode, exp;

	// Set the page number.
	rc = pmbus_wr(fd, bus, i2c_addr, PMBUS_PAGE, page);
	if (rc) {
		printf("  Error: Read Voltage Mode(rc=%d)\n", rc);
		return (rc);
	}

	// Read the Current Output Voltage.
	rc = pmbus_rd(fd, bus, i2c_addr, PMBUS_VOUT_MODE, &mode);
	if (rc) {
		printf("  Error: Read Voltage Mode(rc=%d)\n", rc);
		return (rc);
	}

	exp = mode&0x1F;
	
	rc = pmbus_rd16(fd, bus, i2c_addr, PMBUS_VOUT_COMMAND, &volt_set);
	if (rc) {
		printf("  Error: Read VOUT command(rc=%d)\n", rc);
		return (rc);
	}

	pmbus_mant_exp_to_fp_print (volt_set, exp, "Voltage Set", "Volts");

	// Maximum allowable output voltage
	rc = pmbus_rd16(fd, bus, i2c_addr, PMBUS_VOUT_SCALE_LOOP, &scale_loop);
	if (rc) {
		printf("  Error: Read VOUT command(rc=%d)\n", rc);
		return (rc);
	}

	return (rc);
}

int pmbus_margin_set (int fd, uint8_t bus, uint8_t i2c_addr, int pcent)
{
	int	rc = 0;
	uint8_t data, mode, exp;
	uint16_t data16;

	rc = pmbus_rd(fd, bus, i2c_addr, PMBUS_VOUT_MODE, &data);
	if (rc) {
		printf("  Error: Read Voltage Mode(rc=%d)\n", rc);
		return (rc);
	}

	mode = (data >> 5) & 0x07;
	exp  = (data&0x1F);

	rc = pmbus_rd16(fd, bus, i2c_addr, PMBUS_VOUT_COMMAND, &data16);
	if (rc) {
		printf("  Error: Read VOUT command(rc=%d)\n", rc);
		return (rc);
	}
	printf(" Command Read = 0x%X, pcent=%d\n", data16, pcent);
	data16 = (data16 *(100+pcent))/100;
	if (pcent > 0) {
		printf(" Margin Hi set = 0x%X\n", data16);
		rc = pmbus_wr16(fd, bus, i2c_addr, PMBUS_VOUT_MARGIN_HI, &data16);
		if (rc) {
			printf("  Error: Read VOUT command(rc=%d)\n", rc);
			return (rc);
		}
	} else {
		printf(" Margin Lo set = 0x%X\n", data16);
		rc = pmbus_wr16(fd, bus, i2c_addr, PMBUS_VOUT_MARGIN_LO, &data16);
		if (rc) {
			printf("  Error: Read VOUT command(rc=%d)\n", rc);
			return (rc);
		}
	}

	return (rc);
}

int pmbus_margin_hi (int fd, uint8_t bus, uint8_t i2c_addr)
{
	return (pmbus_operation (fd, bus, i2c_addr, PMBUS_OPERATION_MARGIN_HI));
}

int pmbus_margin_lo (int fd, uint8_t bus, uint8_t i2c_addr)
{
	return (pmbus_operation (fd, bus, i2c_addr, PMBUS_OPERATION_MARGIN_LOW));
}

int pmbus_margin_norm (int fd, uint8_t bus, uint8_t i2c_addr)
{
	return (pmbus_operation (fd, bus, i2c_addr, PMBUS_OPERATION_SOFT_OFF));
}
