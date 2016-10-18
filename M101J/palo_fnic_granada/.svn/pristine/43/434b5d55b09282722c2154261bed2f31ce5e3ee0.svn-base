#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
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
    uint8_t mdio_i = 0, port_i;

    for(port_i=0;port_i<4;port_i++) {
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
           printf("Error: Wr BCM8754_UD_SPI_PORT_CTRL  PHY%d\n", port_i);
           return (rc);
       }
       // Apply soft reset to channel of interest
       pma_pmd_reset(fd, mdio_i, port_i);
    }
    usleep(200);
    // Write start address of where microcode is to be stored 
    // to MSG_IN
    data = 0xC000;
    for(port_i=0;port_i<4;port_i++) {
        BCM8754_WR(fd, mdio_i, port_i, BCM8754_PMA_PMD_ADDR,
                            BCM8754_MSG_IN, data);
    }
    // Wait 14 microseconds for operation to complete
    usleep(20 * 1);
    // Write total number of bytes to be downloaded divide by 2.
    // For 16 KB ROM
    data = 0x2000;
    for(port_i=0;port_i<4;port_i++) {
        BCM8754_WR(fd, mdio_i, port_i, BCM8754_PMA_PMD_ADDR,
                                BCM8754_MSG_IN, data);
    }
    // Wait 14 microseconds for operation to complete
    usleep(20 * 1);
    // Write all 16 KB of data through the MDIO. 
    // wait 14 us between each write
    for (index = 0; index < 0x2000; index++) {
        for(port_i=0;port_i<4;port_i++) {
      // printf("Wr %x %x %x\n", mdio_addr, port_addr, prog_data_8754[index]);
             BCM8754_WR(fd, mdio_i, port_i, BCM8754_PMA_PMD_ADDR,
                        BCM8754_MSG_IN, prog_data_8754[index]);
                 // usleep(20);
        }
    }
    // Microcode can also be downloaded by hardware serially through SPI
    // (register 0xCA85)
    // usleep(20 * 1);

    // Read MSG_OUT which will provide done status  
    for(port_i=0;port_i<4;port_i++) {
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

    printf("  PHY EEPROM Load Successful\n");
    return (rc);
}

static int bcm8754_ucode_preload(int fd, uint8_t mdio_addr, 
                                 uint8_t port_addr) 
{
    int rc = 0, cnt, count; //timeout = 1;
    int pa;

    uint16_t pre_load[][2] = {
      {0xCA10, 0x018F},
      {0xC848, 0xC0F1},
      {0xC843, 0xF},
      {0xC840, 0xC},
    };
    count = sizeof(pre_load)/(2*sizeof(uint16_t));

    for(pa=0;pa<4;pa++) {
        for (cnt = 0; cnt < count; cnt++) {
            BCM8754_WR(fd, mdio_addr, (port_addr+pa), 
                            BCM8754_PMA_PMD_ADDR, 
                            pre_load[cnt][0] , pre_load[cnt][1]);
        }
    }
    return (rc);
}

static int bcm8754_ucode_load_start (int fd, uint8_t mdio_addr, 
                                     uint8_t port_addr) 
{
    int rc = 0, count, cnt; //timeout = 1;
    uint16_t p_load[][2] = {
      {0xC843, 0x0},
      {0xC840, 0x0},
      {0xCA10, 0x188},
    };
    count = sizeof(p_load)/(2*sizeof(uint16_t));

    for (cnt = 0; cnt < count; cnt++) {
        BCM8754_WR(fd, mdio_addr, port_addr, 
               BCM8754_PMA_PMD_ADDR, p_load[cnt][0] ,
               p_load[cnt][1]);
    }
    return (rc);
}

static int bcm8754_ucode_load_verify (int fd, uint8_t mdio_addr, 
                                     uint8_t port_addr) { 
    uint16_t rev;
    BCM8754_RD(fd, mdio_addr, port_addr, BCM8754_PMA_PMD_ADDR,
                    0xCA1C, &rev);
    if (rev != 0x600D) {
        printf("SPI Load Error 0xCA1C reads 0x%04x\n", rev);
    }
    BCM8754_RD(fd, mdio_addr, port_addr, BCM8754_PMA_PMD_ADDR,
                    0xCA1A, &rev);
    if (rev != 0x0403) {
        printf("SPI Load Error 0xCA1A reads 0x%04x\n", rev);
    }
    BCM8754_WR(fd, mdio_addr, port_addr, BCM8754_PMA_PMD_ADDR,
                    0xC843, 0xF);
    BCM8754_WR(fd, mdio_addr, port_addr, BCM8754_PMA_PMD_ADDR,
                    0xC840, 0xC);
    return (0);
}
/*
 * bcm8754_ucode_load:
 * This routine loads ucode from SPIROM in all 40 instances of PHY
 */
#define MAX_PHYS_PER_MDIO_BUS 3
#define MAX_MDIO_BUS          4

int bcm8754_ucode_load_all(int fd) 
{
    int rc = 0, phyindex, index, pi = 1; /*port index */
    int mdio_info[] = {2,3,3,2}; /* PHY's per Mdio Bus */
    /* Put ALL SPI's in reset */
    for (phyindex=0;phyindex < MAX_PHYS_PER_MDIO_BUS; phyindex++) {
        for (index=0;index < MAX_MDIO_BUS; index++) {
            if (phyindex < mdio_info[index]) {
                rc = bcm8754_ucode_preload(fd, index, phyindex *4);
            }
        }
    }
    /* Start Loading Microcode */
    for (phyindex=0;phyindex < MAX_PHYS_PER_MDIO_BUS; phyindex++) {
        for (pi=3;pi >= 0; pi--) {
            for (index=0;index < MAX_MDIO_BUS; index++) {
                if (phyindex < mdio_info[index]){
                    rc = bcm8754_ucode_load_start(fd, index, (phyindex *4)+pi);
                }
            }
            usleep(100 * 1000);
        }
    }
    /* Verify microcode and enable SPI */
    for (phyindex=0;phyindex < MAX_PHYS_PER_MDIO_BUS; phyindex++) {
        for (pi=3;pi >= 0; pi--) {
            for (index=0;index < MAX_MDIO_BUS; index++) {
                if (phyindex < mdio_info[index]) {
                    rc = bcm8754_ucode_load_verify(fd, index,
                                                  (phyindex *4)+pi);
                }
            }
        }
    }
    return (rc);
}

int bcm8754_spirom_program_all (int fd, char *fp)
{
    int rc = 0;
    uint8_t mdio_i = 0;

    // Select MMF/SFI Device 1
    sel_sfi_xfi_device(fd, mdio_i, 0, BCM8754_SFI_SELECT);
    /*port_addr is 0 */
    rc = phy_spirom_program(fd, mdio_i, 0, fp);
    return(rc);
} 

/*
 * Tahoe SFI Initialization
 * Register values are given by SI team
 */
static unsigned short sfi_regs[] = {
    BCM8754_UD_PMD_TX_CTRL_2 ,
    BCM8754_UD_PMD_TX_CTRL_1 ,
    BCM8754_UD_PMD_TX_CTRL_0 ,
};

static unsigned short astoria_sfi_init[][3] = {
    {0x1090,0xC000,0xFF08}, /*port 0 */
    {0x1080,0xA000,0xEE08}, /*port 1 */
    {0x1080,0xA000,0xEE08}, /*port 2 */
    {0x1090,0xC000,0xFF08}, /*port 3 */
};
/*
 * XFI settings from SI team
 */
static unsigned short xfi_regs[][2] = {
   {BCM8754_UD_XFI_TX_CTRL_0, (BCM8754_UD_XFI_TX_CTRL_0_I_DRIVER|
                               BCM8754_UD_XFI_TX_CTRL_0_I_PRE_DRIVER)},
   {BCM8754_UD_XFI_TX_CTRL_1, BCM8754_UD_XFI_TX_CTRL_1_FLD_MAIN_TAP},
   {BCM8754_UD_XFI_TX_CTRL_2, BCM8754_UD_XFI_TX_CTRL_2_FLD_POSTCUR_TAP},
};

static unsigned short astoria_xfi_init[][3] = {
    {0xEE08,0xA800,0x1070}, /*port 0 */
    {0xEE08,0x9800,0x1090}, /*port 1 */
    {0xEE08,0x9000,0x10a0}, /*port 2 */
    {0xEE08,0xB800,0x1050}, /*port 3 */
};

int bcm8754_xfi_init (int fd, uint8_t mdio_addr, uint8_t port_addr)
{
    int index;
    unsigned short data16;
    sel_sfi_xfi_device(fd, mdio_addr, port_addr, BCM8754_XFI_SELECT);
    for (index =0; index < 3; index++) {
        BCM8754_RD(fd, mdio_addr, port_addr, BCM8754_PMA_PMD_ADDR,
                   xfi_regs[index][0], &data16);
        /* mask appropriate bits */
        if (astoria_xfi_init[port_addr][index] != 0xFFFF) {
            data16 &= ~(xfi_regs[index][1]);
            data16 |= (astoria_xfi_init[port_addr][index] & xfi_regs[index][1]);
            BCM8754_WR(fd, mdio_addr, port_addr, BCM8754_PMA_PMD_ADDR,
                       xfi_regs[index][0], data16);
        }
    }
    return (0);
}

int bcm8754_sfi_init (int fd, uint8_t mdio_addr, uint8_t port_addr)
{
    int  index;
    sel_sfi_xfi_device(fd, mdio_addr, port_addr, BCM8754_SFI_SELECT);
    for (index =0; index < 3; index++) {
  //  printf("%d A %x V %x\n", instance, sfi_regs[index],
   //                          tahoe_sfi_init[instance][index]);
        BCM8754_WR(fd, mdio_addr, port_addr, BCM8754_PMA_PMD_ADDR,
                   sfi_regs[index], astoria_sfi_init[port_addr][index]);
    }
    return (0);
}
