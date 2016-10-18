#include <stdio.h>
#include <stdint.h>
#include "diag_main.h"
#include "tylersburg.h"

// PCI-E Register Definition
PCIREG(QPIFREQ, 0, 20, 2, 0xD0, S32, 0);
PCIREG(IOHMISCSS, 0, 20, 0, 0x9C, S32, 0);
PCIREG(TSTHRCATA, 0, 20, 3, 0xE2, S16, 16);
PCIREG(TSFSC, 0, 20, 3, 0xF0, S08, 24);  // to get original offset F3, use shift 24 and S08 mask
PCIREG(TOLM, 0, 20, 0, 0xD0, S32, 0);
PCIREG(TOHM_L, 0, 20, 0, 0xD4, S32, 0); // lower 32 bits
PCIREG(TOHM_H, 0, 20, 0, 0xD8, S32, 0); // higher 32 bits


// PCI-E Registers for General Fatal/Non-Fatal errors
PCIREG(GNERRST, 0, 20, 2, 0x1C0, S32, 0);
PCIREG(GFERRST, 0, 20, 2, 0x1C4, S32, 0);

// PCI-E Registers for QPI links errors
// QPI Link 0
PCIREG(QPI0ERRST, 0, 20, 2, 0x200, S32, 0);
PCIREG(QPIP0ERRST, 0, 20, 2, 0x230, S32, 0);

// QPI Link 1
PCIREG(QPI1ERRST, 0, 20, 2, 0x280, S32, 0);
PCIREG(QPIP1ERRST, 0, 20, 2, 0x2B0, S32, 0);

// Other errors
PCIREG(IOHERRST, 0, 20, 2, 0x300, S32, 0);
PCIREG(THRERRST, 0, 20, 2, 0x360, S32, 0);
PCIREG(MIERRST,  0, 20, 2, 0x380, S32, 0);


pcierr_t globalerr[] = {
	{"QPI[0] Error Status", 0},
	{"QPI[1] Error Status", 1},
	{"QPI[0] Protocol Error Status", 2},
	{"QPI[1] Protocol Error Status", 3},
	{"PCIe[0] Error status", 5}, //associated with ESI
	{"PCIe[1] Error status", 6},
	{"PCIe[2] Error status", 7},
	{"PCIe[3] Error status", 8},
	{"PCIe[4] Error status", 9},
	{"PCIe[5] Error status", 10},
	{"PCIe[6] Error status", 11},
	{"PCIe[7] Error status", 12},
	{"PCIe[8] Error status", 13},
	{"PCIe[9] Error status", 14},
	{"PCIe[10] Error status", 15},
	{"ESI Error Status", 20},
	{"Thermal Error Status", 21},
	{"IOH Core Error Status", 23},
	{"Miscellaneous Error Status", 24},
	{"VTd Error Status", 25}
};

pcierr_t qpierr[] = {
	{"B2-Intel QPI Physcial Layer Successful Reset at same width", 0},
	{"C7-Intel QPI Physcial Layer Reset Successful with Width Change", 1},
	{"D0-Intel QPI Physical Layer Detected Drift Buffer Alarm", 2},
	{"D1-Intel QPI Physical Layer Detected Latency Buffer Rollover", 3},
	{"D2-Intel QPI Physical Layer Initialization Failure", 4},
	{"B6-Intel QPI Link Layer CRC error", 5},
	{"B5-Potential Spurious CRC error on L0s/L1 Exit", 6},
	{"DG-Intel QPI Link Layer Detected packet Error", 7},
	{"B0-Intel QPI Link Layer CRC", 8},
	{"B1-Intel QPI Link Layer detected CRC error", 9},
	{"C0-Intel QPI Link Layer detected CRC error", 10},
	{"D3-Intel QPI Link Layer Control Error", 11},
	{"D4-Intel QPI Link Layer Internal Parity Error", 12}
};

pcierr_t qpiperr[] = {
	{"B3-Intel QPI CPEI Error Status", 0},
	{"B4-Write Cache Correctable ECC", 1},
	{"C1-Protocol Layer Received Poisoned Packet", 2},
	{"C2-Write Cache Un-correctable ECC", 3},
	{"C3-CSR access crossing 32-bit boundary", 4},
	{"D5-Protocol Layer Detected Time-Out in ORB", 8},
	{"D6-Protocol Layer Received Failed Response", 9},
	{"D7-Protocol Layer Recieved Unexpected Response/Completion", 10},
	{"D8-Protocol Layer Recieved Illegal Packet fiend/Incorrect Target NodeID", 11},
	{"D9-Protocol Layer Received Viral from Intel QPI", 12},
	{"DA-Protocol Queue /Table Overflow or Underflow", 13},
	{"DB-Protocol Parity Error", 14},
	{"DC-Protocol SAD illegal/non existent memory for outbound snoop", 15},
	{"DE-Routing Table Invalid", 17},
	{"DF-Illegal Inbound Request", 18}
};

pcierr_t ioherr[] = {
	{"C4-Master abort address error", 4},
	{"C5-Completor abort address error", 5},
	{"C6-FIFO Overflow/Underflow error", 6}
};

pcierr_t threrr[] = {
	{"F0-Thermal Alert", 0},
	{"F2-Catastrophic Thermal Event", 2},
	{"F3-Throttling History", 3}
};

pcierr_t miscerr[] = {
	{"20-IOH Configuration Register Parity Error status", 0},
	{"21-SM Bus Port Error Status", 1},
	{"22-JTAG TAP Port Status", 2},
	{"23-Virtual Pin Port Error Status", 3}
};

int ioh_pciscan(int fd) {
	uint32_t pci_reg;
	uint8_t dev, func, bus=0;
	for (bus = 0; bus < 0xFF; bus++) {
		for (dev = 0; dev < 32; dev++) {
			for (func = 0; func < 0x08; func++) {
				pci_reg = (unsigned int) I2c_PCI_Read(fd, 0, bus, dev, func, 0x00, 0);
				if (pci_reg == -1) {
					usleep(5000);
					pci_reg = (unsigned int) I2c_PCI_Read(fd, 0, bus, dev, func, 0x00, 0);
					if (pci_reg == -1) {
						if( func == 0 ) break; // no other func exists
						else continue; // scan through the rest funcs
					}
				}
				printf(" Bus %2.2x Dev %2.2x Func%2.2x : %8.8x\n",
						bus, dev, func, pci_reg);
				usleep(5000);
			}
		}
	}

	return 0;
}

int ioh_cfgdump(int fd, uint8_t bus, uint8_t dev, uint8_t func) {
	uint32_t pci_reg;
        printf("*************bus %02x dev %02x func %02x***************\n",bus,dev,func);        
	int offset, i;
	for(offset=0; offset<=0xf0; offset+=0x10) {
		printf("%02x: ", offset);
		for(i=0; i<4; i++) {
			pci_reg = (unsigned int) I2c_PCI_Read(fd, 0, bus, dev, func, offset+4*i, 0);
			if( pci_reg == -1 ) {
				usleep(5000);
				pci_reg = (unsigned int) I2c_PCI_Read(fd, 0, bus, dev, func, offset+4*i, 0);
				if( pci_reg == -1) {
					printf("xx xx xx xx ");
					continue;
				}
			}
			printf("%02x %02x %02x %02x ", pci_reg & 0xff, (pci_reg >> 8) & 0xff, (pci_reg >> 16) & 0xff, (pci_reg >> 24) & 0xff);
		usleep(5000);
		}
		printf("\n");
	}
	return 0;

}

int ioh_reg_read(int fd, pci_reg_t *reg, int verbose) {
	int rc = I2c_PCI_Read(fd, 0, reg->bus, reg->dev, reg->func, reg->offset, 0);
	if( rc == -1 ) 
		printf("error reading reg %s\n", reg->name);
	rc >>= reg->shift;
	rc &= reg->mask;
	if( verbose ) {
		if( reg->mask == S08 ) 
			printf("%s: 0x%02X\n", reg->name, rc);
		else if( reg->mask == S16 )
			printf("%s: 0x%04X\n", reg->name, rc);
		else 
			printf("%s: 0x%08X\n", reg->name, rc);
	}
	return rc;
}

static int qpifreq_decode(uint8_t code) {
	switch(code) {
		case 0:
			printf("4.800 GT/s\n");
			break;
		case 1:
			printf("5.860 GT/s\n");
			break;
		case 2:
			printf("6.400 GT/s\n");
			break;
		default: 
			printf("RSVD\n");
			break;
	}
	return 0;
}

static int iou_decode(uint8_t code) {
	switch(code) {
		case 0:
			printf("x4  x4  x4  x4    ");
			break;
		case 1:
			printf("x8  N/A x4  x4    ");
			break;
		case 2:
			printf("x4  x4  x8  N/A   ");
			break;
		case 3:
			printf("x8  N/A x8  N/A   ");
			break;
		case 4:
			printf("x16 N/A N/A N/A   ");
	}
	return 0;
}

static void print_bin(uint8_t code) {
	uint8_t j=0;

	if( code != 0 ) {
		j = code;
		print_bin(code>>1);
		printf("%d", j&0x01);
	}
}

static int pcewidth_decode(uint8_t code) {
	printf("               IOU2      IOU0              IOU1           \n");
	printf("PEWIDTH[5:0]   P01 P02   P03 P04 P05 P06   P07 P08 P09 P10\n");
	//printf("0x%02x           ", code);
	print_bin(code);
	printf("b        ");
	if( code > 0x3b ) {
		printf("unknown/reserved value ....................\n");
		return 0;
	}
	if( code >= 0x1c && code <= 0x1f ) {
		printf("Wait-on-BIOS ..............................\n");
		return 0;
	}
	if( code & 0x20 ) 
		printf("x4  N/A   ");
	else
		printf("x2  x2    ");

	uint8_t sub_code = code & 0x1f;
	if( (sub_code >= 0x18) && (sub_code <= 0x1b) ) {
		iou_decode(4);
		iou_decode(4);
		printf("\n");
		return 0;
	}
	else if( (sub_code >= 0x14) && (sub_code <= 0x17) ) {
		iou_decode( sub_code % 4 );
		iou_decode(4);
		printf("\n");
		return 0;
	}
	else {
		iou_decode( sub_code / 4 );
		iou_decode( sub_code % 4 );
		printf("\n");
	}
				
	return 0;
}

static int ddrfreq_decode(uint8_t code) {
	switch(code) {
		case 0:
			printf("133Mhz input, 200Mhz core\n");
			break;
		case 1:
			printf("100Mhz input, 200Mhz core\n");
			break;
		default:
			printf("RSVD\n");
			break;
	}
	return 0;
}

static int pcierrst_decode(uint32_t code, pcierr_t *err, int sz) {
	int i;
	for(i=0; i<sz; i++) 
		printf("%-75s: %d\n", err[i].name, (code & (0x01 << err[i].bit)) ? 1 : 0 );
	
	return 0;
}

int ioh_info_dump(int fd)
{
	uint32_t pci_reg, pci_reg2;
	if( (pci_reg = (uint32_t) ioh_reg_read(fd, &IOHMISCSS, 0)) < 0 )
		return -1;
	
	printf("Legacy IOH: %s\n", ( ((pci_reg>>17) & 0x01) ? "Yes" : "No") );
	printf("Dual IOH: %s\n\n", ( ((pci_reg>>25) & 0x01) ? "Yes" : "No") ); 
	// QPI Freq Info
	printf("Intel QPI High Frequency: ");
	qpifreq_decode(pci_reg & 0x03);
	printf("\n");
	
	// PCI-E Config Info
	printf("PCI Express Configuration\n");
	pcewidth_decode( (pci_reg>>10) & 0x3f);
	printf("\n");

	printf("DDR FREQ: ");
	ddrfreq_decode( (pci_reg>>8) & 0x03);
	printf("\n");
	
	// get On-Die Thermal Sensor Threshold Register
	if( (pci_reg = (uint32_t) ioh_reg_read(fd, &TSTHRCATA, 0)) < 0 )
		return -1;
	printf("On-Die Thermal Sensor Threshhold Limit: %3.1f C\n", 0.5 * pci_reg );
	
	// get On-Die THermal Sensor Temp Margin
	if( (pci_reg = (uint32_t) ioh_reg_read(fd, &TSFSC, 0)) < 0 )
		return -1;
	printf("On-Die Thermal Sensor Temp Margin: %3.1f C\n\n", 0.5 * pci_reg );
	
	// get Top of Low Memory
	if( (pci_reg = (uint32_t) ioh_reg_read(fd, &TOLM, 0)) < 0 )
		return -1;
	printf("Top of Low Memory: %d Mb\n", (pci_reg >> 26) * 64);

	// get Top of High Memory
	if( (pci_reg = (uint32_t) ioh_reg_read(fd, &TOHM_L, 0)) < 0 )
		return -1;
	if( (pci_reg2 = (uint32_t) ioh_reg_read(fd, &TOHM_H, 0)) < 0 )
		return -1;
	printf("Top of High Memory: %d Mb\n", ((pci_reg>>26) + (pci_reg2<<6)) * 64 );
	
	return 0;
}

int ioh_global_err_dump(int fd) {
	uint32_t pci_reg;
	if( (pci_reg = (uint32_t) ioh_reg_read(fd, &GFERRST, 0)) < 0 ) 
		return -1;
	printf("Global Fatal Error:\n");
	pcierrst_decode(pci_reg, globalerr, sizeof(globalerr)/sizeof(pcierr_t)); 
	printf("\n");

	if( (pci_reg = (uint32_t) ioh_reg_read(fd, &GNERRST, 0)) < 0 ) 
		return -1;
	printf("Global Non-Fatal Error:\n");
	pcierrst_decode(pci_reg, globalerr, sizeof(globalerr)/sizeof(pcierr_t)); 
	printf("\n");	
	return 0;
}

int ioh_qpi_err_dump(int fd) {
	uint32_t pci_reg;
	if( (pci_reg = (uint32_t) ioh_reg_read(fd, &QPI0ERRST, 0)) < 0 ) 
		return -1;
	printf("QPI Link 0 Error:\n");
	pcierrst_decode(pci_reg, qpierr, sizeof(qpierr)/sizeof(pcierr_t)); 
	if( (pci_reg = (uint32_t) ioh_reg_read(fd, &QPIP0ERRST, 0)) < 0 )
		return -1;
	pcierrst_decode(pci_reg, qpiperr, sizeof(qpiperr)/sizeof(pcierr_t));
	printf("\n");

	if( (pci_reg = (uint32_t) ioh_reg_read(fd, &QPI1ERRST, 0)) < 0 ) 
		return -1;
	printf("QPI Link 1 Error:\n");
	pcierrst_decode(pci_reg, qpierr, sizeof(qpierr)/sizeof(pcierr_t));
	if( (pci_reg = (uint32_t) ioh_reg_read(fd, &QPIP1ERRST, 0)) < 0 )
		return -1;
	pcierrst_decode(pci_reg, qpiperr, sizeof(qpiperr)/sizeof(pcierr_t));
	printf("\n");
	
	return 0;
}

int ioh_other_err_dump(int fd) {
	uint32_t pci_reg;
	printf("IOH Error:\n");
	if( (pci_reg = (uint32_t) ioh_reg_read(fd, &IOHERRST, 0)) < 0 ) 
		return -1;
	pcierrst_decode(pci_reg, ioherr, sizeof(ioherr)/sizeof(pcierr_t));
	printf("\n");

	printf("Thermal Error:\n");
	if( (pci_reg = (uint32_t) ioh_reg_read(fd, &THRERRST, 0)) < 0 ) 
		return -1;
	pcierrst_decode(pci_reg, threrr, sizeof(threrr)/sizeof(pcierr_t));
	printf("\n");
	
	printf("Miscellaneous Error:\n");
	if( (pci_reg = (uint32_t) ioh_reg_read(fd, &MIERRST, 0)) < 0 ) 
		return -1;
	pcierrst_decode(pci_reg, miscerr, sizeof(miscerr)/sizeof(pcierr_t));
	printf("\n");
	
	
	return 0;
}

static int find_pcie_width(int fd, uint8_t bus, uint8_t dev) {
	
	unsigned int offset = 0x34;
	uint8_t id=0;
	uint32_t pci_reg;

	pci_reg = (unsigned int) I2c_PCI_Read(fd, 0, bus, dev, 0, offset, 0);
	if(pci_reg == -1) return pci_reg;
	offset = pci_reg & 0xff;
	
	while(1) { // find location of link status capability registers
		pci_reg = (unsigned int) I2c_PCI_Read(fd, 0, bus, dev, 0, offset, 0);
		usleep(5000);
		if( pci_reg == -1 ) {
			printf("error reading bus %d dev %d func 0 offset 0x%X\n", bus, dev, offset);
			return pci_reg;
		}
		
		
		id = pci_reg & 0xff;

		if( id == 0x10 ) break;
		
		
		offset = (pci_reg>>8) & 0xff;
	}

	// reading link status reg
	pci_reg = (unsigned int) I2c_PCI_Read(fd, 0, bus, dev, 0, offset+0x12, 0);

	if( pci_reg == -1) return pci_reg;

	return ( (pci_reg>>20) & 0x3f );
	
}

static int scan_plug_in_dev(int fd, uint8_t bus, int pci_width) {
	uint8_t dev=0;
	uint8_t sbus=0;
	uint8_t bridge=0;
	uint8_t link_width=0;
	uint32_t pci_reg;


	for(dev=0; dev<32; dev++) {
			
		pci_reg = (unsigned int) I2c_PCI_Read(fd, 0, bus, dev, 0, 0, 0);
		if( pci_reg == -1 ) {
			usleep(5000);
			pci_reg = (unsigned int) I2c_PCI_Read(fd, 0, bus, dev, 0, 0, 0);
			if( pci_reg == -1 ) continue;
		}
		
		// if dev exists, check if it's bridge or actual dev
		pci_reg = (unsigned int) I2c_PCI_Read(fd, 0, bus, dev, 0, 0xc, 0);
		if( pci_reg == -1 ) {
			printf("error obtain dev header info\n");
			return pci_reg;
		}

		bridge = (pci_reg >> 16) & 0xff;

		if( bridge != 0x01 ) { // actual dev
			link_width = find_pcie_width(fd, bus, dev);
			printf("bus %d dev %d link width = x%d -- ", bus, dev, link_width);
			printf("%s\n", link_width == pci_width ? "PASS" : "FAIL");
		}
		else {
			printf("found pci-pci bridge at bus %d dev %d\n", bus, dev);
			// obtain secondary bus no.
			pci_reg = (unsigned int) I2c_PCI_Read(fd, 0, bus, dev, 0, 0x18, 0);
			if( pci_reg == -1 ) {
				printf("error obtaining secondary bus no.\n");
				return pci_reg;
			}

			sbus = (pci_reg >> 8 ) & 0xff;
			scan_plug_in_dev(fd, sbus, pci_width);
		}

	}
	

	return 0;
}

int ioh_pcitest(int fd) {

	uint8_t dev=0, bus=0, sdev=0;
	uint32_t pci_reg;
	int dev_total_width=0;
	int port_link_width=0;
	int port_max_width=0;


	// checking bios post complete
	
	for(dev=1; dev<=10; dev++) { // go through 10 pcie port
		port_link_width=0;
		// check if dev is present
		printf("Scanning pci-e port-%d: ", dev);
		pci_reg = (unsigned int) I2c_PCI_Read(fd, 0, 0, dev, 0, 0, 0);
		if( pci_reg == -1 ) {
			usleep(5000);
			pci_reg = (unsigned int) I2c_PCI_Read(fd, 0, 0, dev, 0, 0, 0);
			if( pci_reg == -1 ) {
				printf("port does not exist\n\n");
				continue; 
			}
		}
		
		// check if link is active (0xa2 offset bit 13)
		pci_reg = (unsigned int) I2c_PCI_Read(fd, 0, 0, dev, 0, 0xa0, 0);
		usleep(5000);
		if( !( (pci_reg>>29) & 0x1) ) {
			printf("data link inactive\n\n");
			continue;
		}

		printf("active device found at port-%d\n", dev);
		port_link_width = (pci_reg>>20) & 0x3f;
		printf("negotiated link width = x%d\n", port_link_width);



		
		dev_total_width = 0;
		// find secondary bus no.
		pci_reg = (unsigned int) I2c_PCI_Read(fd, 0, 0, dev, 0, 0x18, 0);
		usleep(5000);
		if( pci_reg == -1 ) { 
			printf("error reading secondary bus no.\n");
			continue; 
		}
		bus = (pci_reg >> 8) & 0xff;
	

		scan_plug_in_dev(fd, bus, port_link_width);
		/*
		// finding device on secondary-bus
		for(sdev=0; sdev<32; sdev++) {
			pci_reg = (unsigned int) I2c_PCI_Read(fd, 0, bus, sdev, 0, 0, 0);
			usleep(5000);
			if( pci_reg == -1 ) continue; 

			printf("dev found at bus %d dev %d\n", bus, sdev);
			dev_total_width = find_pcie_width(fd, bus, sdev);
			scan_plug_in_dev(fd, bus, sdev, port_link_width);
			break;
		}
		
		printf("total link width for devices on port-%d = x%d\n", dev, dev_total_width);	
		
		if( dev_total_width != port_link_width ) printf("link width mismatch\n");
		
		printf("\n");
		*/
	}

	return 0;
}
