 /*********************************************************************
 *
 *      File:   adm1066_eeprom.c
 *      Name:   Sudharshan Kadari
 *
 *      Description:
 *       adm1066_eeprom - EEPROM Library 
 *
 *
 * Copyright (c) 1985-2009 by Cisco Systems, Inc.
 * All rights reserved.
 *
 *********************************************************************/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "adm1066_lib.h"
#include "adm1066.h"

#define ADM_ERASE_TIME_UNITS	1
#if defined(DIAG_FEXT) || defined(DIAG_IOM)
#define diag_gen_i2c_acc(Fh, Bus, Dev, reg_addr, addr_size, data, data_size)    \
        diag_i2c_rd (Fh, Dev, reg_addr, addr_size, data, data_size)
#endif

void adm_get_addr_mode_data(char *pstr, uint16_t *addr, uint8_t *mode, 
			    uint32_t *pdata, uint8_t *fcs)
{
	char	addr_str[9];

	memset(addr_str, 0, sizeof(addr_str));
	memcpy(addr_str, pstr, 4);
	*addr = (uint16_t)strtoul(addr_str, NULL, 16);

	pstr += 4;
	memset(addr_str, 0, sizeof(addr_str));
	memcpy(addr_str, pstr, 2);
	*mode = (uint8_t)strtoul(addr_str, NULL, 16);

	pstr += 2;
	memset(addr_str, 0, sizeof(addr_str));

	memcpy(addr_str, pstr, 8);
	pdata[0] = strtoul(addr_str, NULL, 16);

	memcpy(addr_str, pstr+8, 8);
	pdata[1] = strtoul(addr_str, NULL, 16);

	memcpy(addr_str, pstr+16, 8);
	pdata[2] = strtoul(addr_str, NULL, 16);

	memcpy(addr_str, pstr+24, 8);
	pdata[3] = strtoul(addr_str, NULL, 16);

	memset(addr_str, 0, sizeof(addr_str));
	memcpy(addr_str, pstr+32, 2);
	*fcs = (uint8_t)strtoul(addr_str, NULL, 16);
}

int adm_checksum_verify(uint16_t caddr, uint8_t mode, uint32_t *data, uint8_t fcs)
{
	uint32_t chksum = 0;
	uint8_t  cnt, *pdata;

	chksum = 0x10 + (caddr & 0xFF) + ((caddr >> 8)&0xFF) + mode + fcs;
	for (cnt = 0, pdata = (uint8_t*)data; cnt < (4*sizeof(uint32_t)); cnt++) {
		chksum += pdata[cnt];
	}
	return (chksum & 0xFF);
}

//     Addr Mod  Data0   Data1    Data2    Data3   CheckSum
// :10 F800 00 CC02A002 1D000014 58023A02 1D000014 90
int adm_file_integrity_check(char *filename)
{
	FILE	*fd;
	uint32_t data[4];
	char	my_str[512];
	int	err = 0;
	uint16_t caddr, paddr=0xFFFF;
	uint8_t  mode, fcs;

	if (!filename) return (-1);
	fd = fopen(filename, "r");
	if (fd <= 0) {
		printf("  Error: Opening %s \n", filename);
		return (-1);
	}

        while (fgets(my_str, sizeof(my_str)-1, fd) != NULL) {
		if ((my_str[0] != ':') || (my_str[1] != '1') || (my_str[2] != '0')) 
				continue;

		adm_get_addr_mode_data(my_str+3, &caddr, &mode, data, &fcs);
		err = adm_checksum_verify(caddr, mode, data, fcs);
		if (err) {
			printf("Check Sum Error\n");
			err = -1;
		} 

		if (paddr != 0xFFFF) {
			if (caddr != (paddr+0x10)) {
				printf("  Err: Addr not incr by 16 (Cur:0x%X Prev:0x%X)\n", 
					caddr, paddr+0x10);
				err = -1;
			}
		} else {
			if ((caddr & 0x20) != 0x00) {
				printf("  Err: not a 32byte boundry (Cur:0x%X)\n",
					caddr);
				err = -1;
			}
		}
		paddr = caddr;
	} 

	fclose(fd);
	return (0);
}

extern int diag_gen_i2c_acc(int Fh, unsigned char Bus, unsigned char Dev, 
	   unsigned long reg_addr, unsigned char addr_size, 
	   unsigned char *data, unsigned char data_size);

int adm_eeprom_diff (int fd, uint8_t adm_inst, uint8_t adm_bus, 
		     uint16_t adm_addr, char *filename)
{
	FILE *fp;
	char	my_str[512];
	int err = 0, mismatch = 0;

	uint16_t caddr, cnt;	
	uint8_t  mode, *pdata, ldata, fcs;
	uint32_t data[4], sprom_data[4];

	err = adm_file_integrity_check(filename);
	if (err) {
		printf("  Err: %s integrity check\n", filename);
		return (err);
	} else {
		printf(" %s integrity check PASSED\n", filename);
	}

	fp = fopen(filename, "r");
	if (fp <= 0) {
		printf("  Error: Opening %s (fp = %p)\n", filename, fp);
		return (-1);
	}

	// Set to HALT Mode for reading SEEPROM contents.
	ldata = 0x01;
	err = adm1066_wr (fd, adm_addr, ADM1066_MISC_SECTRL, &ldata);
	if (err) {
		printf(" Err : Wr ADM%d [0x%X] : 0x01\n", adm_inst, ADM1066_MISC_SECTRL);
		fclose(fp);
		return (err);
	}

        while (fgets(my_str, sizeof(my_str)-1, fp) != NULL) {
		if ((my_str[0] != ':') || (my_str[1] != '1') || (my_str[2] != '0')) 
				continue;

		adm_get_addr_mode_data(my_str+3, &caddr, &mode, data, &fcs);

		if ((caddr >= 0xF8A0) && (caddr <= 0xF9F0)) {
			continue;
		}
		for (pdata = (uint8_t*)sprom_data, cnt = 0; cnt < 16; cnt++) {

			// LA_I2C_ADM1066_BUS 2
			err = diag_gen_i2c_acc(fd, adm_bus, adm_addr, caddr+cnt, 2, pdata+cnt, 1);
			if (err) {
				printf("  Err: seeprom rd [0x%X]\n", caddr+cnt);
				fclose(fp);
				return (-1);
			}
		}

		sprom_data[0] = htonl(sprom_data[0]);
		sprom_data[1] = htonl(sprom_data[1]);
		sprom_data[2] = htonl(sprom_data[2]);
		sprom_data[3] = htonl(sprom_data[3]);

		if ((sprom_data[0] != data[0]) ||
		    (sprom_data[1] != data[1]) ||
		    (sprom_data[2] != data[2]) ||
		    (sprom_data[3] != data[3])) {
			printf ("  EXP  [0x%04X] :  %08X-%08X-%08X-%08X\n", caddr,
				data[0], data[1], data[2], data[3]);
			printf ("  READ [0x%04X] :  %08X-%08X-%08X-%08X\n\n", caddr, sprom_data[0], 
					sprom_data[1], sprom_data[2], sprom_data[3]);
			mismatch = 1;
		}
	}

	// Set to HALT Mode for reading SEEPROM contents.
	ldata = 0x00;
	err = adm1066_wr (fd, adm_addr, ADM1066_MISC_SECTRL, &ldata);
	if (err) {
		printf(" Err : Wr ADM%d [0x%X] : 0x01\n", adm_inst, ADM1066_MISC_SECTRL);
		fclose(fp);
		return (0);
	}

	fclose(fp);
	if (mismatch) {
		printf("  Failed: Power Sequenser Check.\n");
	} else {
		printf("  Passed: Power Sequenser Check.\n");
	}
	return (0);
}

int adm_eeprom_dump (int fd, uint8_t adm_inst, uint8_t adm_bus, 
		     uint16_t adm_addr, char *filename)
{
	FILE *fp;
	char	my_str[512];
	int err = 0;

	uint16_t caddr, cnt;	
	uint8_t  mode, *pdata, ldata, fcs;
	uint32_t data[4], sprom_data[4];

	err = adm_file_integrity_check(filename);
	if (err) {
		printf("  Err: %s integrity check\n", filename);
		return (err);
	} else {
		printf(" %s integrity check PASSED\n", filename);
	}

	fp = fopen(filename, "r");
	if (fp <= 0) {
		printf("  Error: Opening %s (fp = %p)\n", filename, fp);
		return (-1);
	}

	// Set to HALT Mode for reading SEEPROM contents.
	ldata = 0x01;
	err = adm1066_wr (fd, adm_addr, ADM1066_MISC_SECTRL, &ldata);
	if (err) {
		printf(" Err : Wr ADM%d [0x%X] : 0x01\n", adm_inst, ADM1066_MISC_SECTRL);
		fclose(fp);
		return (err);
	}

        while (fgets(my_str, sizeof(my_str)-1, fp) != NULL) {
		if ((my_str[0] != ':') || (my_str[1] != '1') || (my_str[2] != '0')) 
				continue;

		adm_get_addr_mode_data(my_str+3, &caddr, &mode, data, &fcs);
		if ((caddr >= 0xF8A0) && (caddr <= 0xF9F0)) {
			continue;
		}

		for (pdata = (uint8_t*)sprom_data, cnt = 0; cnt < 16; cnt++) {

			// LA_I2C_ADM1066_BUS 2
			err = diag_gen_i2c_acc(fd, adm_bus, adm_addr, caddr+cnt, 2, pdata+cnt, 1);
			if (err) {
				printf("  Err: seeprom rd [0x%X]\n", caddr+cnt);
				fclose(fp);
				return (-1);
			}
		}

		sprom_data[0] = htonl(sprom_data[0]);
		sprom_data[1] = htonl(sprom_data[1]);
		sprom_data[2] = htonl(sprom_data[2]);
		sprom_data[3] = htonl(sprom_data[3]);

		printf ("  [0x%04X] :  %08X-%08X-%08X-%08X\n\n", caddr, sprom_data[0], 
					sprom_data[1], sprom_data[2], sprom_data[3]);
	}

	// Set to HALT Mode for reading SEEPROM contents.
	ldata = 0x00;
	err = adm1066_wr (fd, adm_addr, ADM1066_MISC_SECTRL, &ldata);
	if (err) {
		printf(" Err : Wr ADM%d [0x%X] : 0x01\n", adm_inst, ADM1066_MISC_SECTRL);
		fclose(fp);
		return (0);
	}

	fclose(fp);
	return (0);
}


int adm_eeprom_erase (int fd, uint8_t adm_inst, uint8_t adm_bus, 
		     uint16_t adm_addr, char *filename)
{
	FILE *fp;
	char	my_str[512];
	int err = 0, mismatch = 0;

	uint16_t cnt, addr, line_num =0, last_addr =0;	
	uint8_t  mode, *pdata, ldata, fcs, addr0, addr1;
	uint32_t data[4], sprom_data[4];

        err = adm_file_integrity_check(filename);
        if (err) {
                printf("  Err: %s integrity check\n", filename);
                return (err);
        } else {
                printf(" %s integrity check PASSED\n", filename);
        }

	fp = fopen(filename, "r");
	if (fp <= 0) {
		printf("  Error: Opening %s (fp = %p)\n", filename, fp);
		return (-1);
	}

	// Set to HALT Mode for reading SEEPROM contents.
printf("%s:%d\n", __func__, __LINE__);
	ldata = 0x01;
        err = diag_gen_i2c_acc(fd, adm_bus, adm_addr, ADM1066_MISC_SECTRL, 1,
                (uint8_t*)&ldata, sizeof(uint8_t));
	if (err) {
		printf(" Err : Wr ADM%d [0x%X] : 0x01\n", adm_inst, ADM1066_MISC_SECTRL);
		fclose(fp);
		return (err);
	}

	// Set UPDCFG register to enable page erase. 
printf("%s:%d\n", __func__, __LINE__);
	ldata = 0x05;
        err = diag_gen_i2c_acc(fd, adm_bus, adm_addr, ADM1066_MISC_UPDCFG, 1,
                (uint8_t*)&ldata, sizeof(uint8_t));

	if (err) {
		printf(" Err : Wr ADM%d [0x%X] : 0x05\n", adm_inst, ADM1066_MISC_UPDCFG);
		fclose(fp);
		return (err);
	}

        while (fgets(my_str, sizeof(my_str)-1, fp) != NULL) {
		printf("  My_str = %s\n", my_str);
		if ((my_str[0] != ':') && (my_str[1] != '1') && (my_str[2] != '0')) 
				continue;

		adm_get_addr_mode_data(my_str+3, &addr, &mode, data, &fcs);

		addr0 = (addr >> 8) & 0xFF;
		addr1 = addr  & 0xFF;
printf("%s:%d\n", __func__, __LINE__);
		// look for page boundry and erase 
		if ((addr & 0x20) == 0) {
printf("%s:%d\n", __func__, __LINE__);
			err = diag_gen_i2c_acc(fd, adm_bus, adm_addr, ADM1066_COMMANDS_BLKER, 1, 
						(uint8_t*)&addr, 0);
			if (err) {
				printf("  Err: seeprom BLKER [0xFE] : 0x%04X\n", addr);
				fclose(fp);
				return (-1);
			}
printf("%s:%d\n", __func__, __LINE__);
			err = diag_gen_i2c_acc(fd, adm_bus, adm_addr, addr0, 1, 
						(uint8_t*)&addr1, 1);
			if (err) {
				printf("  Err: seeprom wr [0xFE] : 0x%04X\n", addr);
				fclose(fp);
				return (-1);
			}
			printf("  Erasing page 0x%X 0x%X\n", (addr>>8)&0xFF, addr&0xFF);
		}

		for (pdata = (uint8_t*)sprom_data, cnt = 0; cnt < 16; cnt++) {
printf("%s:%d\n", __func__, __LINE__);
			err = diag_gen_i2c_acc(fd, adm_bus, adm_addr, addr+cnt, 2, pdata+cnt, 1);
			if (err) {
				printf("  Err: seeprom rd [0x%X]\n", addr+cnt);
				fclose(fp);
				return (-1);
			}
			usleep (20*ADM_ERASE_TIME_UNITS);
		}
		if (!line_num++) {
			if (addr != (last_addr + 0x10)) {
				printf("  Err: Addr not incr 0x10 (addr=0x%X, last_addr=0x%X)\n",
					addr, last_addr);
				err = -1;
			}
		}
	}

	// Set UPDCFG register to enable page erase. 
	ldata = 0x01;
printf("%s:%d\n", __func__, __LINE__);
        err = diag_gen_i2c_acc(fd, adm_bus, adm_addr, ADM1066_MISC_UPDCFG, 1,
                (uint8_t*)&ldata, sizeof(uint8_t));

	if (err) {
		printf(" Err : Wr ADM%d [0x%X] : 0x01\n", adm_inst, ADM1066_MISC_UPDCFG);
		fclose(fp);
		return (err);
	}

	// Set to HALT Mode for reading SEEPROM contents.
	ldata = 0x00;
printf("%s:%d\n", __func__, __LINE__);
        err = diag_gen_i2c_acc(fd, adm_bus, adm_addr, ADM1066_MISC_SECTRL, 1,
                (uint8_t*)&ldata, sizeof(uint8_t));

	if (err) {
		printf(" Err : Wr ADM%d [0x%X] : 0x01\n", adm_inst, ADM1066_MISC_SECTRL);
		fclose(fp);
		return (err);
	}

printf("%s:%d\n", __func__, __LINE__);
	fclose(fp);
	return (mismatch? -1 : err);
}

int adm_eeprom_write (int fd, uint8_t adm_inst, uint8_t adm_bus, 
		     uint16_t adm_addr, char *filename)
{
	FILE *fp;
	char	my_str[512];
	int err = 0;

	uint16_t cnt, addr, line_num =0, last_addr =0;	
	uint8_t  mode, *pdata, ldata, fcs;
	uint32_t data[4];

        err = adm_file_integrity_check(filename);
        if (err) {
                printf("  Err: %s integrity check\n", filename);
                return (err);
        } else {
                printf(" %s integrity check PASSED\n", filename);
        }

	fp = fopen(filename, "r");
	if (fp <= 0) {
		printf("  Error: Opening %s (fp = %p)\n", filename, fp);
		return (-1);
	}

	// Set UPDCFG register to disable config registers 
	ldata = 0x05;
        err = diag_gen_i2c_acc(fd, adm_bus, adm_addr, ADM1066_MISC_UPDCFG, 1,
                (uint8_t*)&ldata, sizeof(uint8_t));

	if (err) {
		printf(" Err : Wr ADM%d [0x%X] : 0x05\n", adm_inst, ADM1066_MISC_UPDCFG);
		fclose(fp);
		return (err);
	}

	// Set to HALT Mode for reading SEEPROM contents.
	ldata = 0x01;
        err = diag_gen_i2c_acc(fd, adm_bus, adm_addr, ADM1066_MISC_SECTRL, 1,
                (uint8_t*)&ldata, sizeof(uint8_t));

	if (err) {
		printf(" Err : Wr ADM%d [0x%X] : 0x01\n", adm_inst, ADM1066_MISC_SECTRL);
		fclose(fp);
		return (err);
	}

        while (fgets(my_str, sizeof(my_str)-1, fp) != NULL) {
		printf("  My_str = %s\n", my_str);
		if ((my_str[0] != ':') && (my_str[1] != '1') && (my_str[2] != '0')) 
				continue;

		adm_get_addr_mode_data(my_str+3, &addr, &mode, data, &fcs);

		for (pdata = (uint8_t*)data, cnt = 0; cnt < 16; cnt++) {
			err = diag_gen_i2c_acc(fd, adm_bus, adm_addr, addr+cnt, 2, pdata+cnt, 1);
			if (err) {
				printf("  Err: seeprom wr [0x%X]\n", addr+cnt);
				fclose(fp);
				return (-1);
			}
		}
		if (!line_num++) {
			if (addr != (last_addr + 0x10)) {
				printf("  Err: Addr not incr 0x10 (addr=0x%X, last_addr=0x%X)\n",
					addr, last_addr);
				err = -1;
			}
		}
	}

	// Set UPDCFG register to enable page erase. 
	ldata = 0x01;
        err = diag_gen_i2c_acc(fd, adm_bus, adm_addr, ADM1066_MISC_UPDCFG, 1,
                (uint8_t*)&ldata, sizeof(uint8_t));

	if (err) {
		printf(" Err : Wr ADM%d [0x%X] : 0x01\n", adm_inst, ADM1066_MISC_UPDCFG);
		fclose(fp);
		return (err);
	}

	// Set to HALT Mode for reading SEEPROM contents.
	ldata = 0x00;
        err = diag_gen_i2c_acc(fd, adm_bus, adm_addr, ADM1066_MISC_SECTRL, 1,
                (uint8_t*)&ldata, sizeof(uint8_t));
	if (err) {
		printf(" Err : Wr ADM%d [0x%X] : 0x01\n", adm_inst, ADM1066_MISC_SECTRL);
		fclose(fp);
		return (err);
	}

	// Download EEPROM to RAM 
#if 0
	ldata = 0x00;
        err = diag_gen_i2c_acc(fd, adm_bus, adm_addr, 0x98, 1,
                (uint8_t*)&ldata, sizeof(uint8_t));
	if (err) {
		printf(" Err : Wr ADM%d [0x98] : 0x00\n", adm_inst);
		fclose(fp);
		return (err);
	}
#else
	printf(" SKIP DOWNLOAD TO RAM FOR NOW.., ENABLE AFTER VERIFY\n");
#endif
	fclose(fp);
	return (err);
}
