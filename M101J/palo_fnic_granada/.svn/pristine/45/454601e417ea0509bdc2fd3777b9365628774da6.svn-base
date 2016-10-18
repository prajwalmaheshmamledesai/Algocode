/*
 * (c) 2006 Nuova Systems, Inc. All Rights Reserved
 */
#ident "$Id: host_intf_mdio.c 2008-2-13 dzwang $"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

#define WOO_MEM_SPACE	0x800000
#define CISCO_PCI_VEND_ID	0x1137
#define WOODSIDE_PCI_DEV_ID	0x004C
#define PORTOLA_PCI_DEV_ID	0x0061

struct _pci_alloc_ {
    int fd;
    unsigned int baseaddr;
    int is_pci;
} woo_pci_alloc;

uint32_t pci_scan_find_mbar (int port)
{
        char buf[1024];
        FILE *fptr;
        uint32_t s_devfn;
        uint8_t bus, dev, func;
        uint32_t vend_dev_Id;
        uint32_t irq;
        uint32_t mbar;

    /*
    ** Parse the PCI bus info from the /proc/bus/pci/devices
    **
    ** That file contains one line for each device.
    **
    ** Each line contains the following fields delimited by white space:
    **
    ** 0201      -- [02] bus number, [01] devfn
    ** 8086105e  -- vendor/device
    ** 11        -- IRQ
    ** cfc00004  -- MEMBAR 0 including control IO vs. MEM info
    ** 00000000  -- MEMBAR 1
    ** 00000000  -- MEMBAR 2
    ** 00000000  -- MEMBAR 3
    ** 0000c001  -- MEMBAR 4
    ** 00000000  -- MEMBAR 5
    ** 00000000  -- MEMBAR 6
    ** 00020000  -- MEMBAR 0 length
    ** 00000000  -- MEMBAR 1 length
    ** 00000000  -- MEMBAR 2 length
    ** 00000000  -- MEMBAR 3 length
    ** 00000020  -- MEMBAR 4 length
    ** 00000000  -- MEMBAR 5 length
    ** 00000000  -- MEMBAR 6 length
    ** e1000     -- driver name, optional
    **
    */

        // vendId = 0x1137 VENDOR_ID_CISCO_SYSTEMS
        // devId = 0x8000 DEVICE_ID_CISCO_SYSTEMS_PALO
        if ((fptr = fopen( "/proc/bus/pci/devices", "r")) == NULL) {
                printf("Unable to open /proc/bus/pci/devices\n" );
                return 0;
        }

        while (fgets(buf, sizeof(buf)-1, fptr))
        {
                sscanf(buf, "%x %x %x %x ", &s_devfn, &vend_dev_Id, &irq, &mbar);
                bus = s_devfn >> 8U; 
                dev = (s_devfn >> 3) & 0x1F;
                func = s_devfn & 0x7;
                if ((vend_dev_Id == ((CISCO_PCI_VEND_ID << 16) | 
                                     WOODSIDE_PCI_DEV_ID)) ||  
                    (vend_dev_Id == ((CISCO_PCI_VEND_ID << 16) | 
                                     PORTOLA_PCI_DEV_ID))) {
                        printf("vendId == 0x%x, s_devfn = 0x%x, irq = 0x%x, mbar = 0x%x\n", vend_dev_Id, s_devfn, irq, mbar);
                        fclose(fptr);
                        return (mbar & ~(0xff));
                }
        }

        fclose(fptr);
        return 0;
}

int woo_pci_mmap() 
{ 
	uint32_t bar = 0;

        bar = pci_scan_find_mbar(0);

        if (bar == 0) {
                printf("Fail to find WOO Port0 & get BAR.\n");
                return -1;
        } else {
                printf("Probe WOO Port0 BAR = 0x%X\n", bar);
	}

	if ((woo_pci_alloc.fd = open("/dev/mem", O_RDWR | O_SYNC)) < 0) {
		printf("Fail to open /dev/mem.\n");
                return -1;
        }
	woo_pci_alloc.baseaddr = (uint32_t) mmap(0, WOO_MEM_SPACE, 
                           PROT_READ|PROT_WRITE, MAP_SHARED, woo_pci_alloc.fd, bar);
	if (woo_pci_alloc.baseaddr == (uint32_t) MAP_FAILED) {
		close(woo_pci_alloc.fd);
                woo_pci_alloc.is_pci = 0;
                printf("Fail to mmap.\n");
                return -1;
        }
        woo_pci_alloc.is_pci = 1;
	printf("bar = 0x%x, usr_addr = 0x%X\n", bar, woo_pci_alloc.baseaddr);
	return (0);
}

void woo_pci_munmap() 
{
	if (munmap((void*)woo_pci_alloc.baseaddr, WOO_MEM_SPACE)  < 0) {
		printf("Fail to munmap.\n");
        }
	close(woo_pci_alloc.fd);
}

unsigned int woo_get_baseaddr()
{
    return(woo_pci_alloc.baseaddr);
}

void woo_set_pci_driver()
{
    woo_pci_alloc.is_pci = 0;
}

int woo_get_pci_driver()
{
    return(woo_pci_alloc.is_pci);
}

