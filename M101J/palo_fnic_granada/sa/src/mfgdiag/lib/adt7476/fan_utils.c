/********************************************************************
 *
 *      File:   chassis_utils.c
 *      Name:   Sudharshan Kadari
 *
 *      Description:
 *
 *
 * Copyright (c) 1985-2007 by Cisco Systems, Inc.
 * All rights reserved.
 *
 *
 *********************************************************************/

#include <stdio.h>
#include <stdint.h>
#include "diag_main.h"
#include "adt7476.h"
#include "fan_lib.h"

#define FAN_LED_RED		0x03
#define FAN_LED_GREEN		0x02

typedef struct _diag_fan_behavior {
    uint32_t duty_cycle;
    uint32_t rpm;
    uint32_t delta;
} diag_fan_behavior_t;

#ifdef DIAG_LA

#define FAN_BEHV_DELTA_LO	65
#define FAN_BEHV_DELTA_HI	30
#define FAN_BEHV_DELTA_NORM	35

diag_fan_behavior_t fan_behv_del_fr[] = {
    { 0x00, 4500, FAN_BEHV_DELTA_LO },
    { 0xff, 11200, FAN_BEHV_DELTA_HI },
    { 0x80, 9000, FAN_BEHV_DELTA_NORM },
};

diag_fan_behavior_t fan_behv_del_bk[] = {
    { 0x00, 6000, FAN_BEHV_DELTA_LO },
    { 0xff, 17000, FAN_BEHV_DELTA_HI },
    { 0x80, 13250, FAN_BEHV_DELTA_NORM },
};

#else

#define FAN_BEHV_DELTA_LO	45
#define FAN_BEHV_DELTA_HI	25
#define FAN_BEHV_DELTA_NORM	30

diag_fan_behavior_t fan_behv_del_fr[] = {
    { 0x00, 1000, FAN_BEHV_DELTA_LO },
    { 0xff, 9100, FAN_BEHV_DELTA_HI },
    { 0x80, 5100, FAN_BEHV_DELTA_NORM },
};

diag_fan_behavior_t fan_behv_del_bk[] = {
    { 0x00, 1000, FAN_BEHV_DELTA_LO },
    { 0xff, 9100, FAN_BEHV_DELTA_HI },
    { 0x80, 5100, FAN_BEHV_DELTA_NORM },
};

#endif // DIAG_LA

int fan_reg_acc_test (int fd, uint8_t i2c_bus, uint16_t i2c_addr, uint8_t inst, uint8_t e_dev, 
		      uint8_t e_cmpny, uint32_t *addr_error, uint32_t *exp_data, 
		      uint32_t *got_data)
{
        int rc = 0;
        uint8_t dev_id, cmpny_id;

	printf("  Fan%d Register access test\n", inst+1);

       	rc = fan_rd (fd, i2c_bus, i2c_addr, FAN_REG_DEV_ID_REG, &dev_id);
        if (rc) {
		printf("Error: Unable to read fan%d dev id (bus=0x%x, addr=0x%x)\n", 
				inst+1, i2c_bus, i2c_addr);
		return (rc);
	}

	rc = fan_rd (fd, i2c_bus, i2c_addr, FAN_REG_CMPY_ID_REG, &cmpny_id);
	if (rc) {
		printf("Error: Unable to read fan%d dev id (bus=0x%x, addr=0x%x)\n", 
				inst+1, i2c_bus, i2c_addr);
		return (rc);
	}

	if (dev_id != e_dev) {
		*addr_error = FAN_REG_DEV_ID_REG;
		*exp_data   = e_dev;
		*got_data   = dev_id;
		printf("  Error: Fan%d Dev ID failed\n", inst+1);
		return (-1);	
	} 

	if (cmpny_id != e_cmpny) {
		*addr_error = FAN_REG_CMPY_ID_REG;
		*exp_data   = e_cmpny;
		*got_data   = cmpny_id;
		printf("  Error: Fan%d Company ID failed\n", inst+1);
		return (-1);	
	} 

	printf("  Fan%d Register access test Passed\n", inst+1);
	return (0);
}

int fan_speed_check(int fd, uint8_t i2c_bus, uint16_t i2c_addr, uint8_t inst, uint8_t behv,
			   uint32_t *addr_error, uint32_t *exp_data, 
			   uint32_t *got_data)
{
	int rc = 0;
	uint16_t tach2_val, tach1_val;
	uint32_t tach2_rpm, tach1_rpm;

	printf("  Fan%d Speed Check @ duty cycle 0x%X\n", 
		inst+1, fan_behv_del_fr[behv].duty_cycle);

	rc = fan_tach_rd(fd, i2c_bus, i2c_addr, FAN_REG_TACH1_LOW, &tach1_val);
	if (rc) {
		printf("Error: Tach read inst %d\n", inst+1);
		return (-1);
	}

	tach1_rpm = (uint32_t)tach1_val;
	rc = fan_tach_to_rpm(&tach1_rpm);
        if (rc) {
		printf("Error: Tach1 RPM read inst %d\n", inst+1);
		return (-1);
	}
	
	rc = fan_tach_rd(fd, i2c_bus, i2c_addr, FAN_REG_TACH2_LOW, &tach2_val);
	if (rc) {
		printf("Error: Tach read inst %d\n", inst+1);
		return (-1);
	}

	tach2_rpm = (uint32_t)tach2_val;
	rc = fan_tach_to_rpm(&tach2_rpm);
        if (rc) {
		printf("Error: Tach2 RPM read inst %d\n", inst+1);
		return (-1);
	}
	
	if (((double)tach1_rpm < (fan_behv_del_bk[behv].rpm * 
		((100 - fan_behv_del_bk[behv].delta)/100.0))) ||
		((double)tach1_rpm > (fan_behv_del_bk[behv].rpm * 
		((100 + fan_behv_del_bk[behv].delta)/100.0)))) 
	{
		*addr_error = FAN_REG_TACH1_LOW;
		*exp_data   = fan_behv_del_bk[behv].rpm;
		*got_data   = tach1_val;

		printf("  BACK: Fan%d Rd = %d, Exp = %f - %f\n", 
			inst+1, tach1_rpm,
			fan_behv_del_bk[behv].rpm *
			((100 - fan_behv_del_bk[behv].delta)/100.0),
			fan_behv_del_bk[behv].rpm *
				((100 + fan_behv_del_bk[behv].delta)/100.0));
		return (-2);
	}

	if (((double)tach2_rpm < (fan_behv_del_fr[behv].rpm * 
		((100 - fan_behv_del_fr[behv].delta)/100.0))) ||
		((double)tach2_rpm > (fan_behv_del_fr[behv].rpm * 
		((100 + fan_behv_del_fr[behv].delta)/100.0)))) 
	{
		*addr_error = FAN_REG_TACH1_LOW;
		*exp_data   = fan_behv_del_fr[behv].rpm;
		*got_data   = tach2_val;

		printf("  FRONT: Fan%d Rd = %d, Exp = %f - %f\n", 
			inst+1, tach2_rpm,
			fan_behv_del_fr[behv].rpm *
			((100 - fan_behv_del_fr[behv].delta)/100.0),
			fan_behv_del_fr[behv].rpm *
			((100 + fan_behv_del_fr[behv].delta)/100.0));
		return (-2);
	}
	return (rc);
}

int fan_probe_test(int fd, uint8_t i2c_bus, uint16_t i2c_addr, uint8_t inst)
{
    int rc = 0;
    uint8_t val = 0, reg = 0;

    reg = FAN_REG_DEV_ID_REG;
    rc = fan_rd(fd, i2c_bus, i2c_addr, reg, &val);
    printf("       Probe DEV  ID 0x%2x ... ", val);
    if (FAN_REG_DEV_ID_DEFAULT == val)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
        printf("[ERROR] Expected 0x%2x, Read 0x%2x\n", FAN_REG_DEV_ID_DEFAULT, val);
    }

    reg = FAN_REG_CMPY_ID_REG;
    rc = fan_rd(fd, i2c_bus, i2c_addr, reg, &val);
    printf("       Probe CMPY ID 0x%2x ... ", val);
    if (FAN_REG_CMPY_ID_DEFAULT == val)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
        printf("[ERROR] Expected 0x%2x, Read 0x%2x\n", FAN_REG_CMPY_ID_DEFAULT, val);
    }

    return rc;
}

int fan_speed_test (int fd, uint8_t i2c_bus, uint16_t i2c_addr, uint8_t inst, uint8_t slp_time,
			uint32_t *addr_error, uint32_t *exp_data,
			uint32_t *got_data)
{
    int rc = 0;
	uint32_t behv;

    printf("  Fan%d Init\n", inst+1);
	rc = fan_init(fd, i2c_bus, i2c_addr);
	if (rc) {
		printf("Error: Unable to initialize fan %d)\n", inst+1);
		return (rc);
	}

	for (behv = 0; !rc && (behv < (sizeof(fan_behv_del_fr)/sizeof(diag_fan_behavior_t))); 
			behv++) {

		printf("  Fan%d Speed Set  @ duty cycle 0x%X\n", 
			inst+1, fan_behv_del_fr[behv].duty_cycle);
	
		rc = fan_pwm_wr(fd, i2c_bus, i2c_addr, FAN_REG_PWM1,
				fan_behv_del_fr[behv].duty_cycle);
		if (rc) {
			printf("Error: Failed PWM write inst %d\n", inst+1);
			return (rc);;
		}

		sleep(slp_time);
		rc = fan_speed_check(fd, i2c_bus, i2c_addr, inst, behv,
				addr_error, exp_data, got_data);
	}
        return (rc);
}

int fan_temp_show(int fd, uint8_t i2c_bus, uint16_t i2c_addr, uint8_t inst)
{
	int rc = 0;
	int8_t tval;

	rc = fan_local_ts_rd(fd, i2c_bus, i2c_addr, &tval);
	if (rc) {
		printf("Error: %s: i2c(bus=0x%x, addr=0x%x) inst(%d) rc(%d)\n",
			__func__, i2c_bus, i2c_addr, inst + 1, rc);
		rc = -1;
	} else {
		printf(" Fan%d Local    Temp %d C\n", inst + 1, (int8_t)tval);
	}

    rc = fan_remote_1_ts_rd(fd, i2c_bus, i2c_addr, &tval);
	if (rc) {
		printf("Error: %s: i2c(bus=0x%x, addr=0x%x) inst(%d) rc(%d)\n",
			__func__, i2c_bus, i2c_addr, inst + 1, rc);
		rc = -1;
	} else {
		printf(" Fan%d Remote_1 Temp %d C\n", inst + 1, (int8_t)tval);
	}

    rc = fan_remote_2_ts_rd(fd, i2c_bus, i2c_addr, &tval);
	if (rc) {
		printf("Error: %s: i2c(bus=0x%x, addr=0x%x) inst(%d) rc(%d)\n",
			__func__, i2c_bus, i2c_addr, inst + 1, rc);
		rc = -1;
	} else {
		printf(" Fan%d Remote_2 Temp %d C\n", inst + 1, (int8_t)tval);
	}
    
	return (rc);
}

int fan_vltg_show(int fd, uint8_t i2c_bus, uint16_t i2c_addr, uint8_t inst)
{
	int rc = 0;
    uint8_t val, reg;

    printf("Fan %d :\n", inst + 1);

    reg = FAN_REG_EXT_RES1;
    rc = fan_rd(fd, i2c_bus, i2c_addr, reg, &val);
	if (rc) 
    {
		printf("Error: %s: i2c(bus=0x%x, addr=0x%x) inst(%d) rc(%d)\n",
			__func__, i2c_bus, i2c_addr, inst + 1, rc);
		rc = -1;
	}

    reg = FAN_REG_EXT_RES2;
    rc = fan_rd(fd, i2c_bus, i2c_addr, reg, &val);
	if (rc) 
    {
		printf("Error: %s: i2c(bus=0x%x, addr=0x%x) inst(%d) rc(%d)\n",
			__func__, i2c_bus, i2c_addr, inst + 1, rc);
		rc = -1;
	}

    reg = FAN_REG_2P5V_MEASURE;
    rc = fan_rd(fd, i2c_bus, i2c_addr, reg, &val);
	if (rc) 
    {
		printf("Error: %s: i2c(bus=0x%x, addr=0x%x) inst(%d) rc(%d)\n",
			__func__, i2c_bus, i2c_addr, inst+1, rc);
		rc = -1;
	} else {
		printf(" Voltage 2.5V - 0x%x\n", val);
	}

    reg = FAN_REG_VCCP_READ;
    rc = fan_rd(fd, i2c_bus, i2c_addr, reg, &val);
	if (rc) 
    {
		printf("Error: %s: i2c(bus=0x%x, addr=0x%x) inst(%d) rc(%d)\n",
			__func__, i2c_bus, i2c_addr, inst+1, rc);
		rc = -1;
	} else {
		printf(" Voltage VCCP - 0x%x\n", val);
	}
    
    reg = FAN_REG_VCC_READ;
    rc = fan_rd(fd, i2c_bus, i2c_addr, reg, &val);
	if (rc) 
    {
		printf("Error: %s: i2c(bus=0x%x, addr=0x%x) inst(%d) rc(%d)\n",
			__func__, i2c_bus, i2c_addr, inst+1, rc);
		rc = -1;
	} else {
		printf(" Voltage VCC  - 0x%x\n", val);
	}

    reg = FAN_REG_5V_MEASURE;
    rc = fan_rd(fd, i2c_bus, i2c_addr, reg, &val);
	if (rc) 
    {
		printf("Error: %s: i2c(bus=0x%x, addr=0x%x) inst(%d) rc(%d)\n",
			__func__, i2c_bus, i2c_addr, inst+1, rc);
		rc = -1;
	} else {
		printf(" Voltage 5V   - 0x%x\n", val);
	}

    reg = FAN_REG_12V_MEASURE;
    rc = fan_rd(fd, i2c_bus, i2c_addr, reg, &val);
	if (rc) 
    {
		printf("Error: %s: i2c(bus=0x%x, addr=0x%x) inst(%d) rc(%d)\n",
			__func__, i2c_bus, i2c_addr, inst+1, rc);
		rc = -1;
	} else {
		printf(" Voltage 12V  - 0x%x\n", val);
	}
   
	return (rc);
}

int fan_spd_show(int fd, uint8_t i2c_bus, uint16_t i2c_addr, uint8_t inst)
{
	int rc = 0;
	uint32_t speed;

	rc = fan_rpm_rd (fd, i2c_bus, i2c_addr, FAN_REG_TACH1_LOW, &speed);
	if (rc) {
		printf("Error: read fan%d, rc=%d\n", inst+1, rc);
		return (rc);
	}
	printf(" Rear  fan%d speed %u RPM\n", inst+1, speed);

	rc = fan_rpm_rd (fd, i2c_bus, i2c_addr, FAN_REG_TACH2_LOW, &speed);
	if (rc) {
		printf("Error: read fan%d, rc=%d\n", inst+1, rc);
		return (rc);
	}
	printf(" Front fan%d speed %u RPM\n", inst+1, speed);

	return (rc);
}

int fan_spd_set (int fd, uint8_t i2c_bus, uint16_t i2c_addr, uint8_t inst, uint8_t percent)
{
	int rc = 0;
	uint16_t speed;

	speed = (uint16_t)(percent * 255)/100;
	printf("  Set PWM to 0x%X (Fan Speed %d)\n", speed, percent);	

	rc = fan_pwm_wr(fd, i2c_bus, i2c_addr, FAN_REG_PWM1, speed);
	if (rc) {
		printf("Error: Failed to write fan%d, rc=%d\n", inst+1, rc);
	}
	return (rc);
}

int fan_led (int fd, uint8_t i2c_bus, uint16_t i2c_addr, uint8_t inst, uint8_t red, uint8_t on)
{
	int rc = 0;

#ifdef DIAG_ALPINE
    // TODO ...

#else
	rc = pca9551_led_wr(fd, i2c_bus, i2c_addr, 0, red ? 0x1 : 0x02, on);
#endif /* DIAG_ALPINE */
    
	if (rc) {
		printf("Error: %s Fan%d gpio wr i2c(0x%X) gpio(%d) rc(%d)\n",
			__func__, inst+1,
			i2c_addr, 
			red ? FAN_LED_RED : FAN_LED_GREEN,
			rc);
	}

	return (rc);
}
