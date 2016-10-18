 /*********************************************************************
 *
 *      File:   ltc4215.c 
 *      Name:   Sudharshan Kadari
 *
 *      Description:
 *       ltc4215 - Header File
 *
 *
 * Copyright (c) 1985-2007 by Cisco Systems, Inc.
 * All rights reserved.
 *
 *********************************************************************/
#include <stdio.h>
#include <sys/types.h>
#include <stdint.h>
#include "diag_reg.h"
#include "ltc4215.h"
#include "ppc_lib.h"

#if 0
reg_desc_t ltc4215_reg_desc[] =
{
REG_DEF("LTC", LTC_CONTROL, "LTC_CONTROL", "", 0xFF, 0x00, "")\
REG_DEF_FLD(LTC_CONTROL, "GPIO1", LTC_CONTROL_FLD_GPIO1, LTC_CONTROL_FLD_GPIO1, "", "")\
REG_DEF_FLD(LTC_CONTROL, "GPIO0", LTC_CONTROL_FLD_GPIO0, LTC_CONTROL_FLD_GPIO0, "", "")\
REG_DEF_FLD(LTC_CONTROL, "TEST_ENA", LTC_CONTROL_FLD_TEST_ENA, LTC_CONTROL_FLD_TEST_ENA, "", "")\
REG_DEF_FLD(LTC_CONTROL, "MASS_WR_ENA", LTC_CONTROL_FLD_MASS_WR_ENA, LTC_CONTROL_FLD_MASS_WR_ENA, "", "")\
REG_DEF_FLD(LTC_CONTROL, "FET_ON", LTC_CONTROL_FLD_FET_ON	, LTC_CONTROL_FLD_FET_ON, "", "")\
REG_DEF_FLD(LTC_CONTROL, "OVER_CURRENT_AUTO_RETRY", LTC_CONTROL_FLD_OVER_CURRENT_AR, LTC_CONTROL_FLD_OVER_CURRENT_AR, "", "")\
REG_DEF_FLD(LTC_CONTROL, "UNDER_VOLT_AUTO_RETRY", LTC_CONTROL_FLD_UNDER_VOLT_AR, LTC_CONTROL_FLD_UNDER_VOLT_AR, "", "")\
REG_DEF_FLD(LTC_CONTROL, "OVER_VOLT_AUTO_RETRY", LTC_CONTROL_FLD_OVER_VOLT_AR, LTC_CONTROL_FLD_OVER_VOLT_AR, "", "")\
REG_DEF("LTC", LTC_ALERT, "LTC_ALERT", "", 0xFF, 0x00, "")\
REG_DEF_FLD(LTC_ALERT, "GPIO_OUTPUT", LTC_ALERT_FLD_GPIO_OUTPUT, LTC_ALERT_FLD_GPIO_OUTPUT, "", "")\
REG_DEF_FLD(LTC_ALERT, "FET_SHORT", LTC_ALERT_FLD_FET_SHORT, LTC_ALERT_FLD_FET_SHORT, "", "")\
REG_DEF_FLD(LTC_ALERT, "EN_STATE_CHG", LTC_ALERT_FLD_EN_STATE_CHG, LTC_ALERT_FLD_EN_STATE_CHG, "", "")\
REG_DEF_FLD(LTC_ALERT, "POWER_BAD", LTC_ALERT_FLD_POWER_BAD	, LTC_ALERT_FLD_POWER_BAD, "", "")\
REG_DEF_FLD(LTC_ALERT, "OVER_CURRENT", LTC_ALERT_FLD_OVER_CURRENT, LTC_ALERT_FLD_OVER_CURRENT, "", "")\
REG_DEF_FLD(LTC_ALERT, "UNDER_VOLT", LTC_ALERT_FLD_UNDER_VOLT, LTC_ALERT_FLD_UNDER_VOLT, "", "")\
REG_DEF_FLD(LTC_ALERT, "OVER_VOLT", LTC_ALERT_FLD_OVER_VOLT, LTC_ALERT_FLD_OVER_VOLT, "", "")\
REG_DEF("LTC", LTC_STATUS, "LTC_STATUS", "", 0xFF, 0x00, "")\
REG_DEF_FLD(LTC_STATUS, "FET_ON", LTC_STATUS_FLD_FET_ON, LTC_STATUS_FLD_FET_ON, "", "")\
REG_DEF_FLD(LTC_STATUS, "GPIO_IN", LTC_STATUS_FLD_GPIO_IN, LTC_STATUS_FLD_GPIO_IN, "", "")\
REG_DEF_FLD(LTC_STATUS, "FET_SHORT", LTC_STATUS_FLD_FET_SHORT, LTC_STATUS_FLD_FET_SHORT, "", "")\
REG_DEF_FLD(LTC_STATUS, "EN_CHANGE", LTC_STATUS_FLD_EN, LTC_STATUS_FLD_EN, "", "")\
REG_DEF_FLD(LTC_STATUS, "POWER_BAD", LTC_STATUS_FLD_POWER_BAD, LTC_STATUS_FLD_POWER_BAD, "", "")\
REG_DEF_FLD(LTC_STATUS, "OVER_CURRENT", LTC_STATUS_FLD_OVER_CURRENT, LTC_STATUS_FLD_OVER_CURRENT, "", "")\
REG_DEF_FLD(LTC_STATUS, "UNDER_VOLT", LTC_STATUS_FLD_UNDER_VOLT, LTC_STATUS_FLD_UNDER_VOLT, "", "")\
REG_DEF_FLD(LTC_STATUS, "OVER_VOLT", LTC_STATUS_FLD_OVER_VOLT, LTC_STATUS_FLD_OVER_VOLT, "", "")\
REG_DEF("LTC", LTC_FAULT, "LTC_FAULT", "", 0xFF, 0x00, "")\
REG_DEF_FLD(LTC_FAULT, "FET_SHORT", LTC_FAULT_FLD_FET_SHORT, LTC_FAULT_FLD_FET_SHORT, "", "")\
REG_DEF_FLD(LTC_FAULT, "EN_CHANGE", LTC_FAULT_FLD_EN_CHG, LTC_FAULT_FLD_EN_CHG, "", "")\
REG_DEF_FLD(LTC_FAULT, "POWER_BAD", LTC_FAULT_FLD_POWER_BAD, LTC_FAULT_FLD_POWER_BAD, "", "")\
REG_DEF_FLD(LTC_FAULT, "OVER_CURRENT", LTC_FAULT_FLD_OVER_CURRENT, LTC_FAULT_FLD_OVER_CURRENT, "", "")\
REG_DEF_FLD(LTC_FAULT, "UNDER_VOLT", LTC_FAULT_FLD_UNDER_VOLT, LTC_FAULT_FLD_UNDER_VOLT, "", "")\
REG_DEF_FLD(LTC_FAULT, "OVER_VOLT", LTC_FAULT_FLD_OVER_VOLT, LTC_FAULT_FLD_OVER_VOLT, "", "")\
REG_DEF("LTC", LTC_SENSE,  "LTC_SENSE",  "", 0xFF, 0x00, "")\
REG_DEF("LTC", LTC_SOURCE, "LTC_SOURCE", "", 0xFF, 0x00, "")\
REG_DEF("LTC", LTC_ADIN,   "LTC_ADIN",   "", 0xFF, 0x00, "")\
{ TYP_NONE, "", 0, 0, "", "", 0, 0, ""}
};
#endif

extern int I2CByteRead_8BitIndex(int Fh, unsigned char Bus, unsigned char Dev,
		                          unsigned char Reg, unsigned char *data, int verbose);
extern int I2CByteWrite_8BitIndex (int Fh, unsigned char Bus, unsigned char Dev,
		                        unsigned char Reg, unsigned char Val, int send, int verbose);

int ltc4215_rd (int fd, uint16_t i2c_addr, uint8_t offset, uint8_t *data)
{
	return I2CByteRead_8BitIndex(fd, 1, i2c_addr, offset, data, 0);
}

int ltc4215_wr (int fd, uint16_t i2c_addr, uint8_t offset, uint8_t *data)
{
	return I2CByteWrite_8BitIndex(fd, 1, i2c_addr, offset, *data, 0, 0); 
}

int ltc4215_power (int fd, uint16_t i2c_addr)
{
	int     rc = 0, src_volt, sense_volt;
	uint8_t data;

	rc = ltc4215_rd (fd, i2c_addr, LTC_SENSE, &data);
	if (rc) {
		printf("Error: access to LTC_SENSE failed (rc=%d)\n", rc);
		return (rc);
	}
	sense_volt = LTC_SENSE_VOLTAGE_UV(data);

	rc = ltc4215_rd (fd, i2c_addr, LTC_SOURCE, &data);
	if (rc) {
		printf("Error: access to LTC_SOURCE failed (rc=%d)\n", rc);
		return (rc);
	}
	src_volt = LTC_SOURCE_VOLTAGE_MV(data);

	printf ("  Source Voltage  = %6.3f V\n", src_volt/1000.0);
	printf ("  Sense  Voltage  = %6.3f mV\n", sense_volt/1000.0);
	printf ("  Sense  Current  = %6.3f Amps\n", sense_volt/4000.0);

	// Sense voltage is over 0.004 ohms. current = sense_volt/0.004
	// Convert milli volt and micro volt to volt and milli volt.
	// power = ((src_volt/1000.0) * (sense_volt/1000.0))/0.004

	printf ("  Power  Comsumed = %6.3f Watts\n", 
			(src_volt * sense_volt)/(1000.0 * 4000.0));
	return (rc);	
}

static char *ltc_reg_names[] = {
	"LTC_CONTROL",
	"LTC_ALERT",
	"LTC_STATUS",
	"LTC_FAULT",
	"LTC_SENSE",
	"LTC_SOURCE",
	"LTC_ADIN",
	""
	};

int ltc4215_access_test(int fd, uint16_t i2c_addr) 
{
	int     rc = 0;
	uint8_t addr, data;
	
	for (addr = LTC_CONTROL; addr <= LTC_ADIN; addr++) {
		rc = ltc4215_rd (fd, i2c_addr, addr, &data);
		if (rc) {
			printf("Error: access to LTC failed (rc=%d)\n", rc);
			return (rc);
		}
		printf("  LTC [%-12s (%d)] : [0x%X]\n", ltc_reg_names[addr], addr, data);
	}

	return (ltc4215_power(fd, i2c_addr));
}
