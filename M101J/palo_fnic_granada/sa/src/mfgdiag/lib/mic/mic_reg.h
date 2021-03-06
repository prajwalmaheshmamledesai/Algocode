#ifndef _MIC_REG_H_
#define _MIC_REG_H_

#define MIC_ADDR(i, x)	((x) | (i << 5))

#define MIC_PRESCALE_LO		0x00
#define MIC_PRESCALE_HI		0x04
#define MIC_CONTROL		0x08
#define MIC_CONTROL_FLD_I2C_ENA		7
#define MIC_CONTROL_FLD_I2C_IRQ_ENA	6

#define MIC_TRANSMIT		0x0C
#define MIC_TRANSMIT_FLD_ADDR_E	7
#define MIC_TRANSMIT_FLD_ADDR_S	1
#define MIC_TRANSMIT_FLD_READ		0

#define MIC_RECEIVE		0x0C
#define MIC_COMMAND		0x10
#define MIC_COMMAND_FLD_STA	7
#define MIC_COMMAND_FLD_STO	6
#define MIC_COMMAND_FLD_RD	5
#define MIC_COMMAND_FLD_WR	4
#define MIC_COMMAND_FLD_ACK	3
#define MIC_COMMAND_FLD_IACK	0

#define MIC_STATUS		0x10
#define MIC_STATUS_FLD_RX_ACK	7
#define MIC_STATUS_FLD_I2C_BUSY	4
#define MIC_STATUS_FLD_ARB_LOST	5
#define MIC_STATUS_FLD_XIN_PROG	1
#define MIC_STATUS_FLD_IRQ_FLAG	0

#define REDWOOD_ADDR		0x14
#define REDWOOD_DATA		0x18
#define REDWOOD_CONTROL		0x1C
#define REDWOOD_STATUS		0x1C

#define MIC_IRQ_CTRL_STAT	0x100
#define MIC_IRQ_MASK		0x104

#define MIC_REG_MISC_BASE	0xE0
#define MIC_REG_INT_STS		0x00
#define MIC_REG_INT_MSK		0x04
#define MIC_REG_LED01		0x08
#define MIC_REG_LED23		0x0C
#define MIC_REG_LED45		0x10
#define MIC_REG_LEDCTL		0x14
#define MIC_REG_FREV		0x18

#define MIC_REG_LED_BLOCK_START	MIC_REG_LED01
#define MIC_REG_LED_BLOCK_END	MIC_REG_LEDCTL
#endif // _MIC_REG_H_
