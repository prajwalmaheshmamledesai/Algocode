#ifndef _MAX16046_LIB_H_
#define _MAX16046_LIB_H_

typedef struct _max16046_trim_s_
{
	uint8_t dac_reg;
	uint8_t dac_en_reg;
	uint8_t marg_hi_reg;
	uint8_t marg_low_reg;
} max16046_trim_t;

typedef struct _max16046_mon_s_
{
	uint8_t adc_reg;
	uint8_t adc_range;
	int nominal;
} max16046_mon_t;

typedef struct _max16046_channel_s_
{
	char *name;
	uint8_t index;
	uint8_t bus;
	uint8_t i2c_addr;
	max16046_mon_t *mon;
	max16046_trim_t *trim;
	//uint8_t adc_reg;
	//uint8_t adc_range;
	//uint8_t dac_reg;
	//uint8_t dac_range;
	//int nominal;
} max16046_channel_t;

#define DEFAULT_PAGE		0x0
#define EXTENDED_PAGE		0x1
#define EEPROM_PAGE		0x2

// under default
#define ADC_RANGE_1_4		0xf		// voltage range reg for MON 1 to 4
#define ADC_RANGE_5_8		0x10		// MON 5 to 8
#define ADC_RANGE_9_12		0x11		// MON 9 to 12
#define DAC_RANGE_1_4		0x12
#define DAC_RANGE_5_8		0x13
#define DAC_RANGE_9_12		0x14

#define DAC01			0x0
#define DAC02			0x1
#define DAC03			0x2
#define DAC04			0x3
#define DAC05			0x4
#define DAC06			0x5
#define DAC07			0x6
#define DAC08			0x7
#define DAC09			0x8
#define DAC10			0x9
#define DAC11			0xa
#define DAC12			0xb

#define DAC01_HI		0x66
#define DAC02_HI		0x67
#define DAC03_HI		0x68
#define DAC04_HI		0x69
#define DAC05_HI		0x6a
#define DAC06_HI		0x6b
#define DAC07_HI		0x6c
#define DAC08_HI		0x6d
#define DAC09_HI		0x6e
#define DAC10_HI		0x6f
#define DAC11_HI		0x70
#define DAC12_HI		0x71

#define DAC01_LOW		0x72
#define DAC02_LOW		0x73
#define DAC03_LOW		0x74
#define DAC04_LOW		0x75
#define DAC05_LOW		0x76
#define DAC06_LOW		0x77
#define DAC07_LOW		0x78
#define DAC08_LOW		0x79
#define DAC09_LOW		0x7a
#define DAC10_LOW		0x7b
#define DAC11_LOW		0x7c
#define DAC12_LOW		0x7d


// under ext
#define ADC01			0x0		//all MONs are MSBs, LSB regs are MSB+1
#define ADC02			0x2
#define ADC03			0x4
#define ADC04			0x6
#define ADC05			0x8
#define ADC06			0xa
#define ADC07			0xc
#define ADC08			0xe
#define	ADC09			0x10
#define ADC10			0x12
#define ADC11			0x14
#define ADC12			0x16

#define DAC_EN_0		0x1c
#define DAC_EN_1		0x1d

#define MFG_REV			0x65
#define FW_REV			0x5C

#define DNE			0xff

#define WRITE_BLOCK		0x94
#define READ_BLOCK		0x95
#define EEPROM_REBOOT		0x96
#define FAULT_STORE		0x97
#define EXTENDED_PAGE_ON	0x98
#define EXTENDED_PAGE_OFF	0x99
#define EEPROM_PAGE_ON		0x9a
#define EEPROM_PAGE_OFF		0x9b

int max16046_rd(int fd, uint8_t bus, uint8_t addr, uint8_t reg, uint8_t *data, int rs);
int max16046_wr(int fd, uint8_t bus, uint8_t addr, uint8_t reg, uint8_t data, int ws);
int max16046_page_extend(int fd, uint8_t bus, uint8_t addr, int onoff);
int max16046_page_eeprom(int fd, uint8_t bus, uint8_t addr, int onoff);
int max16046_adc_read(int fd, max16046_channel_t *chan, int *val);
int max16046_margin(int fd, max16046_channel_t *chan, int level);

#endif // end of _MAX16046_LIB_H
