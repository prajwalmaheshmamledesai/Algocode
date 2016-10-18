
#ifndef _SPROM_ACC_H_
#define _SPROM_ACC_H_

extern int sprom_rd (diag_smb_dev_t *sprom, uint32_t offset, uint32_t len, uint8_t *buf);
extern int sprom_wr (diag_smb_dev_t *sprom, uint32_t offset, uint32_t len, uint8_t *buf);

#endif // _SPROM_ACC_H_
