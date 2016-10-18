/********************************************************************
 *
 *      File:   ad7416_lib.h
 *      Name:   Sudharshan Kadari
 *
 *      Description:
 *       AD7416 - Header File 
 *
 *
 * Copyright (c) 1985-2007 by Cisco Systems, Inc.
 * All rights reserved.
 *
 *********************************************************************/

#ifndef _AD7416_LIB_H_
#define _AD7416_LIB_H_

#define AD7416_TEMP_VALUE			0x00
#define AD7416_TEMP_VALUE_FLD_VALUE		0x06
#define AD7416_TEMP_VALUE_FLD_VALUE_MASK	(0x3FF << AD7416_TEMP_VALUE_FLD_VALUE)

#define AD7416_CONFIG				0x01
#define AD7416_CONFIG_FLD_SHUTDOWN		0x00
#define AD7416_CONFIG_FLD_CMP_IRQ		0x01
#define AD7416_CONFIG_FLD_OTI_POLARITY		0x02
#define AD7416_CONFIG_FLD_FAULT_QUEUE		0x03
#define AD7416_CONFIG_FLD_FAULT_QUEUE_MASK	(0x03 << AD7416_CONFIG_FLD_FAULT_QUEUE)
#define AD7416_CONFIG_FLD_CHAN_SEL		0x05
#define AD7416_CONFIG_FLD_CHAN_SEL_MASK	(0x07 << AD7416_CONFIG_FLD_CHAN_SEL)

#define AD7416_T_SETPOINT			0x02
#define AD7416_T_SETPOINT_FLD_SETPOINT		0x07
#define AD7416_T_SETPOINT_FLD_SETPOINT_MASK	(0x1FF << AD7416_T_SETPOINT_FLD_SETPOINT)

#define AD7416_T_OTI				0x03
#define AD7416_T_OTI_FLD_OTI			0x07
#define AD7416_T_OTI_FLD_OTI_MASK		(0x1FF << AD7416_T_OTI_FLD_OTI)


extern int  ad7416_rd (int fd, uint16_t i2c_addr, uint8_t offset, uint8_t *data, int rs);
extern int  ad7416_wr (int fd, uint16_t i2c_addr, uint8_t offset, uint8_t *data, int ws);

extern int  ad7416_access_test (int fd, uint16_t i2c_addr, int max, int min);
extern int  ad7416_temp_show (int fd, uint16_t i2c_addr, int max, int min);
extern void ad7416_to_c_str (uint16_t data, uint8_t *cstr);
extern int8_t ad7416_to_c (uint16_t data);
extern int  ad7416_init (int fd, uint16_t i2c_addr);
extern int ad7416_temp_get (int fd, uint16_t i2c_addr, int *temp);
extern int ad7416_temp_raw (int fd, uint16_t i2c_addr, uint16_t *temp);

#endif // _AD7416_LIB_H_
