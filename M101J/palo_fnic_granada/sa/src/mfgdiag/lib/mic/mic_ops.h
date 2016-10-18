#ifndef _MIC_OPS_H_
#define _MIC_OPS_H_
void diag_mic_read(uint32_t addr, uint32_t *data);
void diag_mic_write(uint32_t addr, uint32_t data);
extern int fpga_ui (int argc, char *argv[]);
extern char diag_desc_fpga[];
#endif
