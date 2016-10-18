#ifndef _TYLERSBURG_H_
#define _TYLERSBURG_H_

#define S08 0xff	//  8 bit PCIE reg mask
#define S16 0xffff 	// 16 bit PCIE reg mask
#define S32 0xffffffff 	// 32 bit PCIE reg mask
// for 64 reg, use 2 separate 32 bit read

#define PCIREG(name, bus, dev, func, offset, mask, size) \
	pci_reg_t name = {#name, bus, dev, func, offset, mask, size}

typedef struct _pcierr_s {
	char *name;
	uint8_t bit;
} pcierr_t;

typedef struct _ioh_reg_s {
	char *name;
	unsigned char bus;
	unsigned char dev;
	unsigned char func;
	unsigned int offset;
	unsigned int mask;    // mask to filter out the 8-bit and 16-bit registers
	unsigned char shift;  // use shift to get 8-bit, 16 bit registers 
} pci_reg_t;

int ioh_pcitest(int fd);
int ioh_pciscan(int fd);
int ioh_cfgdump(int fd, uint8_t bus, uint8_t dev, uint8_t func);
int ioh_reg_read(int fd, pci_reg_t *reg, int verbose);
int ioh_info_dump(int fd);
int ioh_global_err_dump(int fd);
int ioh_qpi_err_dump(int fd);
int ioh_other_err_dump(int fd);
#endif
