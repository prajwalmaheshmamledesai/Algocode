/********************************************************************
 *
 *      File:   ucd92xx_lib.c
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
#include "ucd92xx.h"

extern int I2CByteRead_8BitIndex(int Fh, unsigned char Bus, unsigned char Dev, 
			  unsigned char Reg, unsigned char *data, int verbose);
extern int I2CByteWrite_8BitIndex (int Fh, unsigned char Bus, unsigned char Dev, 
			unsigned char Reg, unsigned char Val, int send, int verbose);
extern int I2CByteRead_Block ( int Fh, unsigned char Bus, unsigned char Dev,
                        unsigned char Reg, unsigned char *rxBuf, int bytes, int verbose);

int ucd92xx_rd(int fd, uint8_t bus, uint8_t i2c_addr, uint8_t addr, uint8_t *data)
{
	return(I2CByteRead_8BitIndex(fd, bus, i2c_addr, addr, data, 0)); 
}

int ucd92xx_wr(int fd, uint8_t bus, uint8_t i2c_addr, uint8_t addr, uint8_t data)
{
	return(I2CByteWrite_8BitIndex (fd, bus, i2c_addr, addr, data, 0, 0));
}

int ucd92xx_rd16(int fd, uint8_t bus, uint8_t i2c_addr, uint8_t addr, uint16_t *data)
{
	return(I2CByteRead_Block(fd, bus, i2c_addr, addr, (uint8_t*)data, 2, 0)); 
}

int ucd92xx_wr16(int fd, uint8_t bus, uint8_t i2c_addr, uint8_t addr, uint16_t *data)
{
        uint8_t wr_data[3];
        uint16_t data16 = htons(*data);

        wr_data[0] = addr;
        wr_data[1] = (data16 >> 8) & 0xFF;
        wr_data[2] = data16 & 0xFF;

        return(I2CByteWrite_Block(fd, bus, i2c_addr, wr_data, 3, 0));
}

// Convert 11 bit PMBUS Exponent and Mantissa to Floating Point
int ucd_mant_exp_to_fp_print (uint16_t mant, uint8_t exp, char *mystr, char *punits)
{
	uint8_t exp_2s = exp;
	exp = exp & 0x1F; // 5 bit exponent
	if (exp > 15) {
		exp = 0x20-exp;
		printf("  %-18s : %8.4f %s\n", mystr, ((1.0*mant)/(1<<exp)), punits);
	} else {
		printf("  %-18s : %8.4f %s\n", mystr, 1.0*mant*(1<<exp), punits);
	}
}


// Reading Voltages
int ucd_voltage_wr (int fd, uint8_t bus, uint8_t i2c_addr, uint8_t exp, uint16_t data16)
{
	int	rc = 0;
	uint8_t data, mode;

	rc = ucd92xx_rd(fd, bus, i2c_addr, PMBUS_VOUT_MODE, &data);
	if (rc) {
		printf("  Error: Read Voltage Mode(rc=%d)\n", rc);
		return (rc);
	}

	mode = (data >> 5) & 0x07;
	if ((mode == 0) || (mode == 1)) {
		data = (mode << 5) | (exp & 0x1F); 
		rc = ucd92xx_wr(fd, bus, i2c_addr, PMBUS_VOUT_MODE, data);
		if (rc) {
			printf("  Error: Read Voltage Mode(rc=%d)\n", rc);
			return (rc);
		}
	} else if (mode != 2) {
		printf("  Error: Invalid mode=%d\n", mode);
		return (-1);
	} 

	rc = ucd92xx_wr16(fd, bus, i2c_addr, PMBUS_VOUT_COMMAND, &data16);
	if (rc) {
		printf("  Error: Read VOUT command(rc=%d)\n", rc);
		return (rc);
	}

	return (rc);
}

// write protect
int ucd_write_protect_ena (int fd, uint8_t bus, uint8_t i2c_addr, uint8_t wp)
{
	return(ucd92xx_wr(fd, bus, i2c_addr, PMBUS_WRITE_PROTECT, wp));
}

int ucd_write_protect_dis (int fd, uint8_t bus, uint8_t i2c_addr)
{
	return(ucd92xx_wr(fd, bus, i2c_addr, PMBUS_WRITE_PROTECT, 0x00));
}

// Operation set
int ucd_operation (int fd, uint8_t bus, uint8_t i2c_addr, uint8_t oper)
{
	return(ucd92xx_wr(fd, bus, i2c_addr, PMBUS_OPERATION, oper));
}

// Operation set
int ucd_status_byte (int fd, uint8_t bus, uint8_t i2c_addr)
{
	int rc = 0;
	uint8_t data = 0;

	rc = ucd92xx_rd(fd, bus, i2c_addr, PMBUS_STATUS_BYTE, &data);
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


int ucd_voltage_rd (int fd, uint8_t bus, uint8_t i2c_addr, uint8_t addr, char *mystr, char *units)
{
	int	rc = 0;
	uint8_t data, mode, exp;
	uint16_t data16;

	rc = ucd92xx_rd(fd, bus, i2c_addr, PMBUS_VOUT_MODE, &data);
	if (rc) {
		printf("  Error: Read Voltage Mode(rc=%d)\n", rc);
		return (rc);
	}

	mode = (data >> 5) & 0x07;
	exp  = (data&0x1F);

	rc = ucd92xx_rd16(fd, bus, i2c_addr, addr, &data16);
	if (rc) {
		printf("  Error: Read VOUT command(rc=%d)\n", rc);
		return (rc);
	}

	if (!mode) {
		// Linear Voltage Format
		ucd_mant_exp_to_fp_print (data16, exp, mystr, units);
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

int ucd_mfr_rd (int fd, uint8_t bus, uint8_t i2c_addr, uint8_t addr, char *mystr)
{
	int	rc = 0;
	uint8_t data[64];

	rc = ucd92xx_rd(fd, bus, i2c_addr, addr, data);
	if (rc) return(rc); 

        rc = I2CByteRead_Block (fd, bus, i2c_addr, addr, data, data[0]+1, 0);
	if (rc) return (rc);
	data[data[0]+1] = '\0';

	printf("  %-18s : %s\n", mystr, data+1);
	return (rc);
}

int ucd_info (int fd, uint8_t bus, uint8_t i2c_addr)
{
	int	rc = 0;
	uint8_t data[6];

	rc = ucd92xx_rd(fd, bus, i2c_addr, PMBUS_PMBUS_REVISION, data);
	if (rc) return(rc); 
	printf("  PMBUS Revision   : %d\n", data[0]);

	rc  = ucd_mfr_rd (fd, bus, i2c_addr, PMBUS_MFR_ID, "MFR ID");
	if (rc) return (rc);

	rc  = ucd_mfr_rd (fd, bus, i2c_addr, PMBUS_MFR_MODEL, "MFR MODEL");
	if (rc) return (rc);

	rc  = ucd_mfr_rd (fd, bus, i2c_addr, PMBUS_MFR_REVISION, "MFR REVISION");
	if (rc) return (rc);

	rc  = ucd_mfr_rd (fd, bus, i2c_addr, PMBUS_MFR_LOCATION, "MFR LOCATION");
	if (rc) return (rc);

	rc  = ucd_mfr_rd (fd, bus, i2c_addr, PMBUS_MFR_SERIAL, "MFR SERIAL");
	if (rc) return (rc);

	rc  = ucd_mfr_rd (fd, bus, i2c_addr, PMBUS_MFR_DATE, "MFR DATE");
	if (rc) return (rc);

	return (rc);
}

int ucd_state (int fd, uint8_t bus, uint8_t i2c_addr)
{
	int rc;
	rc = ucd_voltage_rd (fd, bus, i2c_addr, PMBUS_READ_VOUT, "Voltage Out Rd", "Volts");
	if(rc) return(rc);

	rc = ucd_voltage_rd (fd, bus, i2c_addr, PMBUS_READ_VIN, "Voltage In Rd", "Volts");
	if(rc) return(rc);

	rc = ucd_voltage_rd (fd, bus, i2c_addr, PMBUS_READ_IIN, "Current In Rd", "Amps");
	if(rc) return(rc);

	rc = ucd_voltage_rd (fd, bus, i2c_addr, PMBUS_READ_IOUT, "Current Out Rd", "Amps");
	if(rc) return(rc);

	rc = ucd_voltage_rd (fd, bus, i2c_addr, PMBUS_READ_TEMPERATURE_1, "Temperature 1", "C");
	if(rc) return(rc);

	rc = ucd_voltage_rd (fd, bus, i2c_addr, PMBUS_READ_TEMPERATURE_2, "Temperature 2", "C");
	if(rc) return(rc);

	rc = ucd_voltage_rd (fd, bus, i2c_addr, PMBUS_READ_FAN_SPEED_1, "FAN SPEED 1", "RPM");
	if(rc) return(rc);

	rc = ucd_voltage_rd (fd, bus, i2c_addr, PMBUS_VOUT_COMMAND, "Voltage Set", "Volts");
	if(rc) return(rc);

	rc = ucd_voltage_rd (fd, bus, i2c_addr, PMBUS_VOUT_MAX, "Voltage Max", "Volts");
	if(rc) return(rc);

	rc = ucd_voltage_rd (fd, bus, i2c_addr, PMBUS_VOUT_CAL_OFFSET, "Voltage Cal", "Volts");
	if(rc) return(rc);

	rc = ucd_voltage_rd (fd, bus, i2c_addr, PMBUS_VOUT_MARGIN_HI, "Margin Hi", "Volts");
	if(rc) return(rc);

	rc = ucd_voltage_rd (fd, bus, i2c_addr, PMBUS_VOUT_MARGIN_LO, "Margin Lo", "Volts");
	if(rc) return(rc);

	rc = ucd_voltage_rd (fd, bus, i2c_addr, PMBUS_MAX_DUTY, "Max Duty", "units");
	if(rc) return(rc);

	rc = ucd_voltage_rd (fd, bus, i2c_addr, PMBUS_FREQUENCY_SWITCH, "Freq Switch", "KHz");
	if(rc) return(rc);

	rc = ucd_voltage_rd (fd, bus, i2c_addr, PMBUS_VIN_ON, "Voltage In On", "Volts");
	if(rc) return(rc);

	rc = ucd_voltage_rd (fd, bus, i2c_addr, PMBUS_VIN_OFF, "Voltage In Off", "Volts");
	if(rc) return(rc);

	rc = ucd_voltage_rd (fd, bus, i2c_addr, PMBUS_FAN_CONFIG_1_2, "V_FAN_1_2", "Volts");
	if(rc) return(rc);

	rc = ucd_voltage_rd (fd, bus, i2c_addr, PMBUS_POWER_GOOD_ON, "POWER GOOD ON", "Volts");
	if(rc) return(rc);

	rc = ucd_voltage_rd (fd, bus, i2c_addr, PMBUS_POWER_GOOD_OFF, "POWER GOOD OFF", "Volts");
	if(rc) return(rc);

	return (rc);
}

int ucd_margin_test (int fd, uint8_t bus, uint8_t i2c_addr, 
		     uint16_t vout_ref, uint16_t var, uint16_t *vread)
{
	int	rc = 0;
	uint8_t data, mode, exp;
	uint16_t vout, vin;

	rc = ucd92xx_rd(fd, bus, i2c_addr, PMBUS_VOUT_MODE, &data);
	if (rc) {
		printf("  Error: Read Voltage Mode(rc=%d)\n", rc);
		return (rc);
	}

	mode = (data >> 5) & 0x07;
	exp  = (data&0x1F);

	rc = ucd92xx_rd16(fd, bus, i2c_addr, PMBUS_READ_VOUT, &vout);
	if (rc) {
		printf("  Error: Read VOUT command(rc=%d)\n", rc);
		return (rc);
	}

	if (!mode) {
		ucd_mant_exp_to_fp_print (vout, exp, "PMBUS VOUT", "Volts");
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

int ucd_voltage_cfg (int fd, uint8_t bus, uint8_t i2c_addr, uint8_t page)
{
	int rc = 0;
	uint16_t volt_set, scale_loop;
	uint8_t mode, exp;

	// Set the page number.
	rc = ucd92xx_wr(fd, bus, i2c_addr, PMBUS_PAGE, page);
	if (rc) {
		printf("  Error: Read Voltage Mode(rc=%d)\n", rc);
		return (rc);
	}

	// Read the Current Output Voltage.
	rc = ucd92xx_rd(fd, bus, i2c_addr, PMBUS_VOUT_MODE, &mode);
	if (rc) {
		printf("  Error: Read Voltage Mode(rc=%d)\n", rc);
		return (rc);
	}

	exp = mode&0x1F;
	
	rc = ucd92xx_rd16(fd, bus, i2c_addr, PMBUS_VOUT_COMMAND, &volt_set);
	if (rc) {
		printf("  Error: Read VOUT command(rc=%d)\n", rc);
		return (rc);
	}

	ucd_mant_exp_to_fp_print (volt_set, exp, "Voltage Set", "Volts");

	// Maximum allowable output voltage
	rc = ucd92xx_rd16(fd, bus, i2c_addr, PMBUS_VOUT_SCALE_LOOP, &scale_loop);
	if (rc) {
		printf("  Error: Read VOUT command(rc=%d)\n", rc);
		return (rc);
	}

	return (rc);
}

int ucd_margin_set (int fd, uint8_t bus, uint8_t i2c_addr, int pcent)
{
	int	rc = 0;
	uint8_t data, mode, exp;
	uint16_t data16;

	rc = ucd92xx_rd(fd, bus, i2c_addr, PMBUS_VOUT_MODE, &data);
	if (rc) {
		printf("  Error: Read Voltage Mode(rc=%d)\n", rc);
		return (rc);
	}

	mode = (data >> 5) & 0x07;
	exp  = (data&0x1F);

	rc = ucd92xx_rd16(fd, bus, i2c_addr, PMBUS_VOUT_COMMAND, &data16);
	if (rc) {
		printf("  Error: Read VOUT command(rc=%d)\n", rc);
		return (rc);
	}
	printf(" Command Read = 0x%X, pcent=%d\n", data16, pcent);
	data16 = (data16 *(100+pcent))/100;
	if (pcent > 0) {
		printf(" Margin Hi set = 0x%X\n", data16);
		rc = ucd92xx_wr16(fd, bus, i2c_addr, PMBUS_VOUT_MARGIN_HI, &data16);
		if (rc) {
			printf("  Error: Read VOUT command(rc=%d)\n", rc);
			return (rc);
		}
	} else {
		printf(" Margin Lo set = 0x%X\n", data16);
		rc = ucd92xx_wr16(fd, bus, i2c_addr, PMBUS_VOUT_MARGIN_LO, &data16);
		if (rc) {
			printf("  Error: Read VOUT command(rc=%d)\n", rc);
			return (rc);
		}
	}

	return (rc);
}

int ucd_margin_hi (int fd, uint8_t bus, uint8_t i2c_addr)
{
	return (ucd_operation (fd, bus, i2c_addr, PMBUS_OPERATION_MARGIN_HI));
}

int ucd_margin_lo (int fd, uint8_t bus, uint8_t i2c_addr)
{
	return (ucd_operation (fd, bus, i2c_addr, PMBUS_OPERATION_MARGIN_LOW));
}

int ucd_margin_norm (int fd, uint8_t bus, uint8_t i2c_addr)
{
	return (ucd_operation (fd, bus, i2c_addr, PMBUS_OPERATION_SOFT_OFF));
}

static uint16_t margin_1500mv_hi[] = {0x1800, 0x183D, 0x187A, 0x18B8, 0x18F5, 0x1933};
static uint16_t margin_1500mv_lo[] = {0x1800, 0x17C2, 0x1785, 0x1747, 0x170A, 0x16CC};

static uint16_t margin_1350mv_hi[] = {0x1599, 0x15D2, 0x1608, 0x1641, 0x1676, 0x1680};
static uint16_t margin_1350mv_lo[] = {0x1599, 0x1564, 0x152B, 0x14F5, 0x14BC, 0x1487};

int ucd_voltage_margin_hi (int fd, uint8_t bus, uint8_t i2c_addr, uint16_t voltage, uint8_t percentMargin)
{
	int rc = 0;
	if ((percentMargin > 5) || ((voltage != 1500) && (voltage != 1350))) {
		printf(" Invalid Margin Percent = %d, Voltage=%d\n", percentMargin, voltage);
		return (-1);
	}

	printf("  UCD (I2C=0x%X) Margin Hi %d [0x%X] : 0x%X, 0x%X\n", i2c_addr, percentMargin, 
		PMBUS_OPERATION_MARGIN_HI,
		(voltage == 1500)? margin_1500mv_hi[percentMargin] :
                                   margin_1350mv_hi[percentMargin],
		(voltage == 1500)? htons(margin_1500mv_hi[percentMargin]) :
                                   htons(margin_1350mv_hi[percentMargin]));
	rc = ucd92xx_wr16(fd, bus, i2c_addr, PMBUS_VOUT_MARGIN_HI, 
		(voltage == 1500)? &margin_1500mv_hi[percentMargin] : 
				   &margin_1350mv_hi[percentMargin]);
	if (rc) {
		printf("  Error: Programming Margin HI\n");
		return (rc);
	}
	return (ucd_operation (fd, bus, i2c_addr, PMBUS_OPERATION_MARGIN_HI));
}

int ucd_voltage_margin_lo (int fd, uint8_t bus, uint8_t i2c_addr, uint16_t voltage, uint8_t percentMargin)
{
	int rc = 0;
	if ((percentMargin > 5) || ((voltage != 1500) && (voltage != 1350))) {
		printf(" Invalid Margin Percent = %d, Voltage=%d\n", percentMargin, voltage);
		return (-1);
	}

	printf("  UCD (I2C=0x%X) Margin Lo -%d [0x%X] : 0x%X\n", i2c_addr, percentMargin,
			PMBUS_VOUT_MARGIN_LO,
			(voltage == 1500)? margin_1500mv_lo[percentMargin] :
                                   margin_1350mv_lo[percentMargin]);

	rc = ucd92xx_wr16(fd, bus, i2c_addr, PMBUS_VOUT_MARGIN_LO, 
		(voltage == 1500)? &margin_1500mv_lo[percentMargin] : 
				   &margin_1350mv_lo[percentMargin]);
	if (rc) {
		printf("  Error: Programming Margin LO\n");
		return (rc);
	}
	return (ucd_operation (fd, bus, i2c_addr, PMBUS_OPERATION_MARGIN_LOW));
}
