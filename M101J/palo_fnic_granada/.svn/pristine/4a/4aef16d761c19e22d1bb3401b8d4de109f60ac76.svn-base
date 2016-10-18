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
    uint8_t mdio_i, port_i, phyport;
    uint8_t mdio[] = {8,12,12,8}; /* #of PHY's per MDIO bus */

    for(mdio_i=0;mdio_i<4;mdio_i++) {
        for(port_i=0;port_i<mdio[mdio_i];port_i++) {
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
                if (mdio_i == 0) {
                    phyport = port_i;
                } else {
                    phyport = mdio[mdio_i-1]+port_i;
                }
                printf("Error: Wr BCM8754_UD_SPI_PORT_CTRL  PHY%d\n",
                        phyport);
                return (rc);
            }
            // Apply soft reset to channel of interest
            pma_pmd_reset(fd, mdio_i, port_i);
         }
    }
    usleep(200);
    // Write start address of where microcode is to be stored 
    // to MSG_IN
    data = 0xC000;
    for(mdio_i=0;mdio_i<4;mdio_i++) {
        for(port_i=0;port_i<mdio[mdio_i];port_i++) {
            BCM8754_WR(fd, mdio_i, port_i, BCM8754_PMA_PMD_ADDR,
                            BCM8754_MSG_IN, data);
        }
    }
    // Wait 14 microseconds for operation to complete
    usleep(20 * 1);
    // Write total number of bytes to be downloaded divide by 2.
    // For 16 KB ROM
    data = 0x2000;
    for(mdio_i=0;mdio_i<4;mdio_i++) {
        for(port_i=0;port_i<mdio[mdio_i];port_i++) {
            BCM8754_WR(fd, mdio_i, port_i, BCM8754_PMA_PMD_ADDR,
                                BCM8754_MSG_IN, data);
        }
    }
    // Wait 14 microseconds for operation to complete
    usleep(20 * 1);
    // Write all 16 KB of data through the MDIO. 
    // wait 14 us between each write
    for (index = 0; index < 0x2000; index++) {
        for(mdio_i=0;mdio_i<4;mdio_i++) {
            for(port_i=0;port_i<mdio[mdio_i];port_i++) {
      // printf("Wr %x %x %x\n", mdio_addr, port_addr, prog_data_8754[index]);
                 BCM8754_WR(fd, mdio_i, port_i, BCM8754_PMA_PMD_ADDR,
                        BCM8754_MSG_IN, prog_data_8754[index]);
                 // usleep(20);
            }
        }
    }
    // Microcode can also be downloaded by hardware serially through SPI
    // (register 0xCA85)
    // usleep(20 * 1);

    // Read MSG_OUT which will provide done status  
    for(mdio_i=0;mdio_i<4;mdio_i++) {
        for(port_i=0;port_i<mdio[mdio_i];port_i++) {
            BCM8754_RD(fd, mdio_i, port_i, 
                            BCM8754_PMA_PMD_ADDR, BCM8754_MSG_OUT, &data); 
            if (rc) {
                if (mdio_i == 0) {
                    phyport = port_i;
                } else {
                    phyport = mdio[mdio_i-1]+port_i;
                }
                printf("Error: Failed to read BCM8754_MSG_OUT for PHY%d\n",
                        phyport);
                return (rc);
            }
            
            
            if (data != BCM8754_EEPROM_SIG) {
                if (mdio_i == 0) {
                    phyport = port_i;
                } else {
                    phyport = mdio[mdio_i-1]+port_i;
                }
                printf("  PHY%d EEPROM Load Failed [0x%X] : 0x%X\n",
                          phyport, BCM8754_MSG_OUT, data);
                return (rc);
            } 
            BCM8754_RD(fd, mdio_i, port_i, 
                BCM8754_PMA_PMD_ADDR, 0x9003, &data); 
        }
    }
    // TODO: User can also read SPI_PORT_CTRL_STATUS, bit13 should be 1.

    printf("  PHY EEPROM Load Successful\n");
    return (rc);
}


int bcm8754_spirom_program_all (int fd, char *fp)
{
    int rc = 0;
    uint8_t mdio_i;
    // Select MMF/SFI Device 1
    for (mdio_i = 0; mdio_i < 4; mdio_i++) {
        sel_sfi_xfi_device(fd, mdio_i, 0, BCM8754_SFI_SELECT);
        /*port_addr is 0 */
        rc = phy_spirom_program(fd, mdio_i, 0, fp);
    }
    return(rc);
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
    if (rev != BCM8754_UCODE_LATEST_VER) {
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
    {0xFF00,0x9800,0x1090}, /*port 8 */
    {0xFFFF,0xA800,0x1070}, /*port 9 */
    {0xFFFF,0xA000,0x1080}, /*port 10 */
    {0xFFFF,0xA800,0x1070}, /*port 11 */
    {0xFFFF,0xC000,0x1040}, /*port 12 */
    {0xFFFF,0xA800,0x1070}, /*port 13 */
    {0xFFFF,0xC800,0x1030}, /*port 14 */
    {0xFFFF,0xA800,0x1070}, /*port 15 */
    {0xFFFF,0xB000,0x1060}, /*port 16 */
    {0xFFFF,0xB800,0x1050}, /*port 17 */
    {0xFFFF,0xC800,0x1030}, /*port 18 */
    {0xFFFF,0xB000,0x1060}, /*port 19 */
    {0xFFFF,0xB800,0x1050}, /*port 20 */
    {0xFFFF,0xC000,0x1040}, /*port 21 */
    {0xFFFF,0xB800,0x1050}, /*port 22 */
    {0xFFFF,0xA800,0x1070}, /*port 23 */
    {0xFFFF,0xA000,0x1080}, /*port 24 */
    {0xFFFF,0xA800,0x1070}, /*port 25 */
    {0xFFFF,0xA800,0x1070}, /*port 26 */
    {0xFFFF,0xA800,0x1070}, /*port 27 */
    {0xFFFF,0xB800,0x1050}, /*port 28 */
    {0xFFFF,0x9800,0x1090}, /*port 29 */
    {0xFFFF,0x9000,0x1090}, /*port 30 */
    {0xFF00,0xE000,0x10C0}, /*port 31 */
    {0xFFFF,0x9800,0x1090}, /*port 32 */
    {0xFF00,0x8000,0x10C0}, /*port 33 */
    {0xFFFF,0xA800,0x1070}, /*port 34 */
    {0xFFFF,0xA800,0x1070}, /*port 35 */
    {0xCC00,0x9000,0x10A0}, /*port 36 */
    {0xFF00,0x8000,0x10C0}, /*port 37 */
    {0xFF00,0x8000,0x10C0}, /*port 38 */
    {0xFF00,0x8000,0x10C0}, /*port 39 */
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
    {0x1080,0xA000,0xEE08}, /*port 8 */
    {0x1080,0xA000,0xFF08}, /*port 9 */
    {0x1080,0xA000,0xEE08}, /*port 10 */
    {0x1080,0xA000,0xFF08}, /*port 11 */
    {0x1080,0xA000,0xEE08}, /*port 12 */
    {0x1090,0x9800,0xDD08}, /*port 13 */
    {0x1080,0xA000,0xEE08}, /*port 14 */
    {0x1080,0xA000,0xFF08}, /*port 15 */
    {0x1080,0xA000,0xEE08}, /*port 16 */
    {0x1080,0xA000,0xFF08}, /*port 17 */
    {0x1080,0xA000,0xEE08}, /*port 18 */
    {0x1080,0xA000,0xFF08}, /*port 19 */
    {0x1080,0xA000,0xEE08}, /*port 20 */
    {0x1080,0xA000,0xEE08}, /*port 21 */
    {0x1080,0xA000,0xEE08}, /*port 22 */
    {0x1080,0xA000,0xEE08}, /*port 23 */
    {0x1080,0xA000,0xEE08}, /*port 24 */
    {0x1080,0xA000,0xEE08}, /*port 25 */
    {0x1080,0xA000,0xEE08}, /*port 26 */
    {0x1080,0xA000,0xEE08}, /*port 27 */
    {0x1080,0xA000,0xEE08}, /*port 28 */
    {0x10A0,0x9000,0xFF08}, /*port 29 */
    {0x1080,0xA000,0xEE08}, /*port 30 */
    {0x1090,0x9800,0xFF08}, /*port 31 */
    {0x1080,0xA000,0xEE08}, /*port 32 */
    {0x1080,0xA000,0xFF08}, /*port 33 */
    {0x1080,0xA000,0xEE08}, /*port 34 */
    {0x1090,0x9800,0xFF08}, /*port 35 */
    {0x1080,0xA000,0xEE08}, /*port 36 */
    {0x1090,0xC000,0xFF08}, /*port 37 */
    {0x1080,0xA000,0xEE08}, /*port 38 */
    {0x1080,0xA000,0xEE08}, /*port 39 */
};                       

static int get_phy_instance(uint8_t mdio_addr, uint8_t port_addr)
{
    if (mdio_addr < 1) {
        return (port_addr);
    } else if (mdio_addr < 2) {
        return (mdio_addr*8 + port_addr);
    } else if (mdio_addr < 3) {
        return (mdio_addr*10 + port_addr);
    } else if (mdio_addr > 3) {
        return (-1);
    } 

    return (32 + port_addr);
 
}
                         
int bcm8754_xfi_init(int fd, uint8_t mdio_addr, uint8_t port_addr)
{
    int instance, index;
    unsigned short data16;
    instance = get_phy_instance(mdio_addr, port_addr);
    if (instance == -1) {
        return (instance); /* TODO: return error */
    }
    sel_sfi_xfi_device(fd, mdio_addr, port_addr, BCM8754_XFI_SELECT);
    for (index =0; index < 3; index++) {
        BCM8754_RD(fd, mdio_addr, port_addr, BCM8754_PMA_PMD_ADDR,
                   xfi_regs[index][0], &data16);
        /* mask appropriate bits */
        if (tahoe_xfi_init[instance][index] != 0xFFFF) {
            data16 &= ~(xfi_regs[index][1]);
            data16 |= (tahoe_xfi_init[instance][index] & xfi_regs[index][1]);
            BCM8754_WR(fd, mdio_addr, port_addr, BCM8754_PMA_PMD_ADDR,
                       xfi_regs[index][0], data16);
        }
    }
    return (0);
}

int bcm8754_sfi_init(int fd, uint8_t mdio_addr, uint8_t port_addr)
{
    int instance, index;
    instance = get_phy_instance(mdio_addr, port_addr);
    if (instance == -1) {
        return (instance); /* TODO: return error */
    }
    sel_sfi_xfi_device(fd, mdio_addr, port_addr, BCM8754_SFI_SELECT);
    for (index =0; index < 3; index++) {
  //  printf("%d A %x V %x\n", instance, sfi_regs[index],
   //                          tahoe_sfi_init[instance][index]);
        BCM8754_WR(fd, mdio_addr, port_addr, BCM8754_PMA_PMD_ADDR,
                   sfi_regs[index], tahoe_sfi_init[instance][index]);
    }
    return (0);
}
