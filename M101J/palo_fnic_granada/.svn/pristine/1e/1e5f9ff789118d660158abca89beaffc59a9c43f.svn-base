#ifndef __MP_I2C_H__
#define __MP_I2C_H__
int mp_arbiter_en(int fd , unsigned short bus);
int MP_I2CByteRead_16BitIndex(int Fh, unsigned char Bus, unsigned char Dev, unsigned short Reg16,unsigned char *data);
int MP_I2CByteWrite_16BitIndex( int Fh, unsigned char Bus, unsigned char Dev, unsigned short Reg16, unsigned char Val);
int mp_i2c_open(void);
void mp_i2c_close(int fd);
#endif
