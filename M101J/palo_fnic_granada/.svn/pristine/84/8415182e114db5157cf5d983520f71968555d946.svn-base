/********************************************************************
 *
 *      File:   pmbus_lib_alpine.c
 *      Name:   Harry Zhao
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

int pwr_info (int fd, uint8_t bus, uint8_t i2c_addr)
{
       int     rc = 0;
       uint8_t data[16];

       rc = pmbus_rd(fd, bus, i2c_addr, PMBUS_PMBUS_REVISION, data);
       if (rc) return(rc);
       printf("  PMBUS Revision   : %d.%d\n", (data[0]>>4)&0xF, data[0]&0xF);

       rc  = pmbus_rd_blk (fd, bus, i2c_addr, PMBUS_CLA_GAINS, data, 8);
       if (rc) return (rc);
       data[8] = '\0';
       printf("  %-18s : %s\n", "MFG SPECIFIC 05", data);

       rc  = pmbus_rd_blk (fd, bus, i2c_addr, PMBUS_ROM_MODE, data, 7);
       if (rc) return (rc);
       data[7] = '\0';
       printf("  %-18s : %s\n", "MFG SPECIFIC 09", data);

       return (rc);
}

int pmbus_info (int fd, uint8_t bus, uint8_t i2c_addr)
{
	int	rc = 0;
	uint8_t data[6];

	rc = pmbus_rd(fd, bus, i2c_addr, PMBUS_PMBUS_REVISION, data);
	if (rc) return(rc); 
	printf("  PMBUS Revision   : %d\n", data[0]);

	rc  = pmbus_mfr_rd (fd, bus, i2c_addr, PMBUS_MFR_ID, "MFR ID");
	if (rc) return (rc);

	rc  = pmbus_mfr_rd (fd, bus, i2c_addr, PMBUS_MFR_MODEL, "MFR MODEL");
	if (rc) return (rc);

	rc  = pmbus_mfr_rd (fd, bus, i2c_addr, PMBUS_MFR_REVISION, "MFR REVISION");
	if (rc) return (rc);

	rc  = pmbus_mfr_rd (fd, bus, i2c_addr, PMBUS_MFR_LOCATION, "MFR LOCATION");
	if (rc) return (rc);

	rc  = pmbus_mfr_rd (fd, bus, i2c_addr, PMBUS_MFR_SERIAL, "MFR SERIAL");
	if (rc) return (rc);

	rc  = pmbus_mfr_rd (fd, bus, i2c_addr, PMBUS_MFR_DATE, "MFR DATE");
	if (rc) return (rc);

	return (rc);
}

int ltc_state (int fd, uint8_t bus, uint8_t i2c_addr)
{
	int rc;
	rc = pmbus_scale_mode_rd (fd, bus, i2c_addr, PMBUS_READ_VOUT, "Voltage Out Rd", "Volts", 1265);
	if(rc) return(rc);

	rc = pmbus_data_literal_rd (fd, bus, i2c_addr, PMBUS_READ_VIN, "Voltage In Rd", "Volts");
	if(rc) return(rc);

	rc = pmbus_data_literal_rd (fd, bus, i2c_addr, PMBUS_READ_IOUT, "Current Out Rd", "Amps");
	if(rc) return(rc);

	rc = pmbus_data_literal_rd (fd, bus, i2c_addr, PMBUS_READ_TEMPERATURE_1, "Temperature 1", "C");
	if(rc) return(rc);

	rc = pmbus_data_literal_rd (fd, bus, i2c_addr, PMBUS_READ_TEMPERATURE_2, "Temperature 2", "C");
	if(rc) return(rc);

	rc = pmbus_data_mode_rd (fd, bus, i2c_addr, PMBUS_VOUT_COMMAND, "Voltage Set", "Volts");
	if(rc) return(rc);

	rc = pmbus_data_mode_rd (fd, bus, i2c_addr, PMBUS_VOUT_MAX, "Voltage Max", "Volts");
	if(rc) return(rc);

	rc = pmbus_data_mode_rd (fd, bus, i2c_addr, PMBUS_VOUT_CAL_OFFSET, "Voltage Cal", "Volts");
	if(rc) return(rc);

	rc = pmbus_data_mode_rd (fd, bus, i2c_addr, PMBUS_VOUT_MARGIN_HI, "Margin Hi", "Volts");
	if(rc) return(rc);

	rc = pmbus_data_mode_rd (fd, bus, i2c_addr, PMBUS_VOUT_MARGIN_LO, "Margin Lo", "Volts");
	if(rc) return(rc);

	rc = pmbus_data_literal_rd (fd, bus, i2c_addr, PMBUS_MAX_DUTY, "Max Duty", "units");
	if(rc) return(rc);

	rc = pmbus_data_literal_rd (fd, bus, i2c_addr, PMBUS_FREQUENCY_SWITCH, "Freq Switch", "KHz");
	if(rc) return(rc);

	rc = pmbus_data_literal_rd (fd, bus, i2c_addr, PMBUS_VIN_ON, "Voltage In On", "Volts");
	if(rc) return(rc);

	rc = pmbus_data_literal_rd (fd, bus, i2c_addr, PMBUS_VIN_OFF, "Voltage In Off", "Volts");
	if(rc) return(rc);

	return (rc);
}

int pwr_state (int fd, uint8_t bus, uint8_t i2c_addr)
{
	int rc;

	rc = pmbus_data_mode_rd (fd, bus, i2c_addr, PMBUS_READ_VOUT, "Voltage Out Rd", "Volts");
	if(rc) return(rc);

	rc = pmbus_data_literal_rd (fd, bus, i2c_addr, PMBUS_READ_IIN, "Current In Rd", "Amps");
	if(rc) return(rc);

	rc = pmbus_data_literal_rd (fd, bus, i2c_addr, PMBUS_READ_IOUT, "Current Out Rd", "Amps");
	if(rc) return(rc);

	rc = pmbus_data_literal_rd (fd, bus, i2c_addr, PMBUS_READ_TEMPERATURE_1, "Temperature 1", "C");
	if(rc) return(rc);

	return (rc);
}

int pmbus_state (int fd, uint8_t bus, uint8_t i2c_addr)
{
	int rc;

	rc = ltc_state(fd, bus, i2c_addr);
	if (rc) return (rc);

	rc = pmbus_data_literal_rd (fd, bus, i2c_addr, PMBUS_READ_IIN, "Current In Rd", "Amps");
	if(rc) return(rc);

	rc = pmbus_data_mode_rd (fd, bus, i2c_addr, PMBUS_READ_FAN_SPEED_1, "FAN SPEED 1", "RPM");
	if(rc) return(rc);

	rc = pmbus_data_mode_rd (fd, bus, i2c_addr, PMBUS_FAN_CONFIG_1_2, "V_FAN_1_2", "Volts");
	if(rc) return(rc);

	rc = pmbus_data_mode_rd (fd, bus, i2c_addr, PMBUS_POWER_GOOD_ON, "POWER GOOD ON", "Volts");
	if(rc) return(rc);

	rc = pmbus_data_mode_rd (fd, bus, i2c_addr, PMBUS_POWER_GOOD_OFF, "POWER GOOD OFF", "Volts");
	return (rc);
}


int ltc_lock (int fd, uint8_t bus, uint8_t i2c_addr, uint8_t lock_on)
{
	return (pmbus_broadcast_wr(fd, bus, i2c_addr, PMBUS_SPECIFIC_01, lock_on? 0x00 : 0x55));
}

int ltc_fix_margin (int fd, uint8_t bus, uint8_t i2c_addr)
{
	uint16_t data16 = 0x0D44;

	return(pmbus_broadcast_wr16(fd, bus, i2c_addr, PMBUS_VOUT_MARGIN_HI, &data16));
}

int pmbus_broadcast_margin_hi (int fd, uint8_t bus, uint8_t i2c_addr)
{
	int rc = 0;

	printf(" Lock\n");
	rc = ltc_lock(fd, bus, i2c_addr, 0);
	if (rc) return (rc);

	printf(" Fix Margin\n");
	rc = ltc_fix_margin (fd, bus, i2c_addr);
	if (rc) return (rc);

	printf(" Set Hi Margin\n");
	rc = pmbus_broadcast_operation (fd, bus, i2c_addr, PMBUS_OPERATION_MARGIN_HI);
	if (rc) return (rc);

	printf(" UnLock\n");
	rc = ltc_lock(fd, bus, i2c_addr, 1);
	return (rc);
}

int pmbus_broadcast_margin_lo (int fd, uint8_t bus, uint8_t i2c_addr)
{
	int rc = 0;

	rc = ltc_lock(fd, bus, i2c_addr, 0);
	if (rc) return (rc);

	rc = pmbus_broadcast_operation (fd, bus, i2c_addr, PMBUS_OPERATION_MARGIN_LOW);
	if (rc) return (rc);

	rc = ltc_lock(fd, bus, i2c_addr, 1);
	return (rc);
}

int pmbus_broadcast_margin_norm (int fd, uint8_t bus, uint8_t i2c_addr)
{
	int rc = 0;

	rc = ltc_lock(fd, bus, i2c_addr, 0);
	if (rc) return (rc);

	rc = pmbus_broadcast_operation (fd, bus, i2c_addr, PMBUS_OPERATION_ON);
	if (rc) return (rc);

	rc = ltc_lock(fd, bus, i2c_addr, 1);
	return (rc);
}
