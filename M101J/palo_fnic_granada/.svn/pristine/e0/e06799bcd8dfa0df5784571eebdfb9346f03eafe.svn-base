#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <sys/time.h>
#include "bcm8754_lib.h"
#include "bcm8754.h"
#include "phy_rom_update.h"
extern unsigned short prog_data_8754[];

/*
 * EDC Download All ports
 * This routine is written to write ucode to RAM to all  40 ports 
 * to reduce programming time
 */ 
int bcm8754_mdio_download_code(int fd)
{
    int rc = 0, index=0;
    uint16_t data = 0;
    const uint8_t mdio_i = 3, phymax = 8; 
    uint8_t port_i; /* #of PHY's on MDIO bus */
    double t1, t2;
    struct timeval stime;

    gettimeofday(&stime, (struct timezone *)0);
    t1 = stime.tv_sec + (((double)stime.tv_usec)/1000000);

    for(port_i=0;port_i<phymax;port_i++) {
        // printf("Mdio %d Port %d\n", mdio_i, port_i);
        // Select MMF/SFI Device 1
        sel_sfi_xfi_device(fd, mdio_i, port_i, BCM8754_SFI_SELECT);
        // Initiate SPI ROM download
        BCM8754_RD(fd, mdio_i, port_i, BCM8754_PMA_PMD_ADDR, 
                            BCM8754_UD_SPI_PORT_CTRL, &data); 
        data &= ~(0xE000); /* added to get microcode download work */
        BCM8754_WR(fd, mdio_i, port_i, BCM8754_PMA_PMD_ADDR,
                            BCM8754_UD_SPI_PORT_CTRL, data);
        data |= BCM8754_UD_SPI_PORT_CTRL_FLD_SPI_BOOT;
        BCM8754_WR(fd, mdio_i, port_i, BCM8754_PMA_PMD_ADDR,
                            BCM8754_UD_SPI_PORT_CTRL, data);
        if (rc) {
            printf("Error: Wr BCM8754_UD_SPI_PORT_CTRL  PHY%d\n",
                    port_i);
            return (rc);
        }
        // Apply soft reset to channel of interest
        pma_pmd_reset(fd, mdio_i, port_i);
    }
    usleep(200);
    // Write start address of where microcode is to be stored 
    // to MSG_IN
    data = 0xC000;
    for(port_i=0;port_i<phymax;port_i++) {
        BCM8754_WR(fd, mdio_i, port_i, BCM8754_PMA_PMD_ADDR,
                        BCM8754_MSG_IN, data);
    }
    // Wait 14 microseconds for operation to complete
    usleep(20 * 1);
    // Write total number of bytes to be downloaded divide by 2.
    // For 16 KB ROM
    data = 0x2000;
    for(port_i=0;port_i<phymax;port_i++) {
        BCM8754_WR(fd, mdio_i, port_i, BCM8754_PMA_PMD_ADDR,
                            BCM8754_MSG_IN, data);
    }
    // Wait 14 microseconds for operation to complete
    usleep(20 * 1);
    // Write all 16 KB of data through the MDIO. 
    // wait 14 us between each write
    for (index = 0; index < 0x2000; index++) {
        for(port_i=0;port_i<phymax;port_i++) {
      //printf("Wr %x %x %x\n", mdio_addr, port_addr, prog_data_8754[index]);
             BCM8754_WR(fd, mdio_i, port_i, BCM8754_PMA_PMD_ADDR,
                    BCM8754_MSG_IN, prog_data_8754[index]);
             // usleep(20);
        }
    }
    // Microcode can also be downloaded by hardware serially through SPI
    // (register 0xCA85)
    // usleep(20 * 1);

    // Read MSG_OUT which will provide done status  
    for(port_i=0;port_i<phymax;port_i++) {
        BCM8754_RD(fd, mdio_i, port_i, 
                        BCM8754_PMA_PMD_ADDR, BCM8754_MSG_OUT, &data); 
        if (rc) {
            printf("Error: Failed to read BCM8754_MSG_OUT for PHY%d\n",
                    port_i);
            return (rc);
        }
        
        
        if (data != BCM8754_EEPROM_SIG) {
            printf("  PHY%d EEPROM Load Failed [0x%X] : 0x%X\n",
                      port_i, BCM8754_MSG_OUT, data);
            return (rc);
        } 
        BCM8754_RD(fd, mdio_i, port_i, 
            BCM8754_PMA_PMD_ADDR, 0x9003, &data); 
    }
    // TODO: User can also read SPI_PORT_CTRL_STATUS, bit13 should be 1.
    gettimeofday(&stime, (struct timezone *)0);
    t2 = stime.tv_sec + (((double)stime.tv_usec)/1000000);
    printf("SFP Phy Download time in %f milliseconds\n", ((t2-t1)*1000));

    printf("BCM8754 PHY EEPROM Load Successful\n");
    return (rc);
}
/*
 * This routine loads ucode from SPIROM in all 40 instances of PHY
 * BROCKWAY has no SPIROM, so this is not supported
 * look at bcm8754_tahoe_lib.c for working code. Tahoe P0 boards has SPIROM
 * and this was removed on Tahoe P1 boards.
 */
int bcm8754_spirom_program_all (int fd, char *fp)
{
    return(0);
} 
/*
 * bcm8754_ucode_load:
 * This routine loads ucode from SPIROM in all 40 instances of PHY
 * BROCKWAY has no SPIROM, so this is not supported
 */
int bcm8754_ucode_load_all(int fd) 
{
    return (0);
}

/*
 * Tahoe XFI Initialization
 * Register values are given by SI team
 * value and mask
 */
static unsigned short xfi_regs[][2] = {
   {BCM8754_UD_XFI_TX_CTRL_0, (BCM8754_UD_XFI_TX_CTRL_0_I_DRIVER|
                               BCM8754_UD_XFI_TX_CTRL_0_I_PRE_DRIVER)},
   {BCM8754_UD_XFI_TX_CTRL_1, BCM8754_UD_XFI_TX_CTRL_1_FLD_MAIN_TAP},
   {BCM8754_UD_XFI_TX_CTRL_2, BCM8754_UD_XFI_TX_CTRL_2_FLD_POSTCUR_TAP},
};
/*
 * HACK ALERT: Using 0xFFFF means this register value should not be
 * modified
 */
static unsigned short tahoe_xfi_init[][3] = {
    {0xFF00,0xF800,0x10E0}, /*port 0 */
    {0xFF00,0x9000,0x10A0}, /*port 1 */
    {0xFFFF,0x9000,0x10A0}, /*port 2 */
    {0xFF00,0x8800,0x10B0}, /*port 3 */
    {0xFF00,0x9000,0x10A0}, /*port 4 */
    {0xFFFF,0x9800,0x1090}, /*port 5 */
    {0xFFFF,0xA000,0x1080}, /*port 6 */
    {0xFF00,0xA800,0x1070}, /*port 7 */
};
/*
 * Tahoe SFI Initialization
 * Register values are given by SI team
 */
static unsigned short sfi_regs[] = {
    BCM8754_UD_PMD_TX_CTRL_2 ,
    BCM8754_UD_PMD_TX_CTRL_1 ,
    BCM8754_UD_PMD_TX_CTRL_0 ,
};

static unsigned short tahoe_sfi_init[][3] = {
    {0x1080,0xA000,0xEE08}, /*port 0 */
    {0x1080,0xA000,0xFF08}, /*port 1 */
    {0x1080,0xA000,0xEE08}, /*port 2 */
    {0x1080,0xA000,0xFF08}, /*port 3 */
    {0x1080,0xA000,0xEE08}, /*port 4 */
    {0x1080,0xA000,0xEE08}, /*port 5 */
    {0x1080,0xA000,0xEE08}, /*port 6 */
    {0x1080,0xA000,0xFF08}, /*port 7 */
};                       

int bcm8754_xfi_init(int fd, uint8_t mdio_addr, uint8_t port_addr)
{
    int index;
    unsigned short data16;
    sel_sfi_xfi_device(fd, mdio_addr, port_addr, BCM8754_XFI_SELECT);
    for (index =0; index < 3; index++) {
        BCM8754_RD(fd, mdio_addr, port_addr, BCM8754_PMA_PMD_ADDR,
                   xfi_regs[index][0], &data16);
        /* mask appropriate bits */
        if (tahoe_xfi_init[port_addr][index] != 0xFFFF) {
            data16 &= ~(xfi_regs[index][1]);
            data16 |= (tahoe_xfi_init[port_addr][index] & xfi_regs[index][1]);
            BCM8754_WR(fd, mdio_addr, port_addr, BCM8754_PMA_PMD_ADDR,
                       xfi_regs[index][0], data16);
        }
    }
    return (0);
}

int bcm8754_sfi_init(int fd, uint8_t mdio_addr, uint8_t port_addr)
{
    int index;
    sel_sfi_xfi_device(fd, mdio_addr, port_addr, BCM8754_SFI_SELECT);
    for (index =0; index < 3; index++) {
  //  printf("%d A %x V %x\n", instance, sfi_regs[index],
   //                          tahoe_sfi_init[instance][index]);
        BCM8754_WR(fd, mdio_addr, port_addr, BCM8754_PMA_PMD_ADDR,
                   sfi_regs[index], tahoe_sfi_init[port_addr][index]);
    }
    return (0);
}
