#ifndef _DS1338_LIB_H_
#define _DS1338_LIB_H_
extern int ds1338_rd(int fd, uint8_t i2c_addr, 
		     uint8_t offset, uint8_t *data);
extern int ds1338_wr(int fd, uint8_t i2c_addr, 
		     uint8_t offset, uint8_t data);

extern int rtc_test (int fd, uint8_t i2c_addr, 
		     uint8_t *addr, uint8_t *exp, uint8_t *read);

#endif // _DS1338_LIB_H_
