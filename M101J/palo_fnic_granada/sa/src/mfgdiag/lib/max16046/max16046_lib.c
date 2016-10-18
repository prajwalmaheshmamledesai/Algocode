#include <stdio.h>
#include <stdint.h>
#include "max16046_lib.h"

extern int I2CByteRead_Block ( int Fh, unsigned char Bus, unsigned char Dev,                        
		unsigned char Reg, unsigned char *rxBuf, int bytes, int verbose);

extern int I2CByteWrite_Block (int Fh, unsigned char Bus, unsigned char Dev, 
		unsigned char *txBuf, int bytes, int verbose);

extern int I2CByteRead_8BitIndex(int Fh, unsigned char Bus, unsigned char Dev,
		unsigned char Reg, unsigned char *data, int verbose);

extern int I2CByteWrite_8BitIndex (int Fh, unsigned char Bus, unsigned char Dev,
		unsigned char Reg, unsigned char Val, int send, int verbose);

int max16046_rd(int fd, uint8_t bus, uint8_t addr, uint8_t reg, uint8_t *data, int rs) {
	return I2CByteRead_Block(fd, bus, addr, reg, data, rs, 0);
}

int max16046_wr(int fd, uint8_t bus, uint8_t addr, uint8_t reg, uint8_t data, int ws) {
	return I2CByteWrite_8BitIndex(fd, bus, addr, reg, data, ws, 0);   // ws=1 reg without data - ws=0 reg with data
}

int max16046_page_extend(int fd, uint8_t bus, uint8_t addr, int onoff) {
	return max16046_wr(fd, bus, addr, onoff ? EXTENDED_PAGE_ON : EXTENDED_PAGE_OFF, 0, 1);		
}

int max16046_page_eeprom(int fd, uint8_t bus, uint8_t addr, int onoff) {
	return max16046_wr(fd, bus, addr, onoff ? EEPROM_PAGE_ON : EEPROM_PAGE_OFF, 0, 1);		
}


static int max16046_verbose=0;

int max16046_adc_read(int fd, max16046_channel_t *chan, int *val) {
	uint8_t addr = chan->i2c_addr;
	uint8_t bus =  chan->bus;
	uint8_t reg = (chan->mon)->adc_range;
	int rc=0;
	uint8_t data[2];
	uint8_t tmp;
	int step=0;
	if( (rc=max16046_rd(fd, bus, addr, reg, data, 1)) ) {
		printf("Error reading max16046 range reg addr = %X reg = %X\n", addr, reg);
		return rc;
	}
	
	tmp = (data[0] >> ( ( (chan->index - 1) % 4) * 2 ) )  & 0x03;
	switch( tmp ) {
		case 0x0:
			step = 546;
			break;
		case 0x01:
			step = 273;
			break;
		case 0x02:
			step = 136;
			break;
		default:
			printf("MON %d is not converted or monitored\n", chan->index);
			*val = 0;
			return -1;
	}
	
	if( (rc=max16046_page_extend(fd, bus, addr, 1)) )  {
		printf("Fail to turn on extended page access addr %X\n", addr);
		return rc;
	}
	
	reg = (chan->mon)->adc_reg;
	if( (rc=max16046_rd(fd, bus, addr, reg, data, 2)) ) {
		printf("Error reading max16046 adc reg addr = %X reg = %X\n", addr, reg);
		return rc;
	}

	*val = (data[0] << 2) + (data[1] >> 6);
	*val *= step;
	*val /= 100;

	if( (rc=max16046_page_extend(fd, bus, addr, 0)) )  {
		printf("Fail to turn off extended page access addr %X\n", addr);
		return rc;
	}
	
	return rc;
}

int max16046_test_channel(int fd, max16046_channel_t *chan, int thresh) {
	int adc_val=0, rc=0;
	
	if(  (rc=max16046_adc_read(fd, chan, &adc_val)) ) {
		printf("Error: can not read adc value\n");
		return rc;
	}

	if( adc_val > ((chan->mon)->nominal * (100 + thresh ) / 100 ) ) {
		printf("Error: %s is out of range, exp -> %d mV, read -> %d mV\n",
				chan->name, (chan->mon)->nominal, adc_val);
		return -1;
	}


	if( adc_val < ((chan->mon)->nominal * (100 - thresh ) / 100 ) ) {
		printf("Error: %s is out of range, exp -> %d mV, read -> %d mV\n",
				chan->name, (chan->mon)->nominal, adc_val);
		return -1;
	}
	
	return rc;
}

int max16046_margin_channel(int fd, max16046_channel_t *chan, int level) {
	int rc=0;
	uint8_t data;
	uint8_t bus = chan->bus;
	uint8_t addr = chan->i2c_addr;
	uint8_t reg=0, mask;

	if( !(chan->trim) ) return rc;

	if( level == 1 ) reg = (chan->trim)->marg_hi_reg;
	if( level == -1) reg = (chan->trim)->marg_low_reg;

	// read margin data if marg up or down
	if( (rc=max16046_rd(fd, bus, addr, reg, &data, 1)) ) {
		printf("Error: can not read margining data\n");
		return rc;
	}

	// write to dac out reg
	if( level ) {
		if( (rc=max16046_wr(fd, bus, addr, (chan->trim)->dac_reg, data, 0)) ) {
			printf("Error: can not write to dac out reg\n");
			return rc;
		}
	}

	if( max16046_verbose )
		printf("dac reg = 0x%X    margin data = 0x%X    ", (chan->trim)->dac_reg, data);

	// enable or disable DAC output	
	if( (rc=max16046_page_extend(fd, bus, addr, 1)) )  {
		printf("Fail to turn on extended page access addr %X\n", addr);
		return rc;
	}

	if( (rc=max16046_rd(fd, bus, addr, (chan->trim)->dac_en_reg, &data, 1)) ) {
		printf("Error: can not read dac enable reg\n");
		return rc;
	}

	if( max16046_verbose )
		printf("enable data p = 0x%X   ", data);
	
	mask = 0x01 << ( (chan->index-1) % 8 );

	if( max16046_verbose )
		printf("mask = 0x%X     ", mask);
	
	if( !level ) data &= ~mask;
	else data |= mask;

	if( max16046_verbose )
		printf("enable data = 0x%X\n", data);
	
	if( (rc=max16046_wr(fd, bus, addr, (chan->trim)->dac_en_reg, data, 0)) ) {
		printf("Error: can not write to dac enable reg\n");
		return rc;
	}
	
	if( (rc=max16046_page_extend(fd, bus, addr, 0)) )  {
		printf("Fail to turn off extended page access addr %X\n", addr);
		return rc;
	}
	
	return rc;
}
