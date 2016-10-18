/********************************************************************
 *
 *      File:   mic_utils.c
 *      Name:   Sudharshan Kadari
 *
 *      Description:
 *
 *
 * Copyright (c) 1985-2007 by Cisco Systems, Inc.
 * All rights reserved.
 *
 *
 *********************************************************************/

#include <stdio.h>
#include "diag_main.h"
#include "diag_reg.h"
#include "micro.h"

#include "ppc_lib.h"
#include "diag_fex.h"
#include "mic_ops.h"
#include "spi_lib.h"

extern int brd_gpio_init();
char diag_desc_fpga[] = "\nfpga:\n\
        Syntax: fpga <cmd> <filename>\n\
	cmd: init, prog, pulse\n\
	prog: .xsvf file to load RAM\n\
	flash: .bit file to load Flash\n\
	init: .xsvf .bit file to load RAM/Flash\n";

int fpga_pulse()
{ 
	int rc = 0;
	unsigned char data;

	rc = brd_fpga_prog_zr_get(&data);
	printf("PROG_B active low pulse: %2.2x\n  clearing \n",data);
	brd_fpga_prog_zr_clr();

	rc = brd_fpga_prog_zr_get(&data);
	printf("fpga prog_zr: %2.2x\n  setting \n",data);

	brd_fpga_prog_zr_set();
	rc = brd_fpga_prog_zr_get(&data);
	printf("fpga prog_zr: %2.2x\n  new \n",data);
	return (rc);
}

int fpga_flash_init (char *xsvfFile, char *bitFile, int debuglevel)
{
	int rc = 0;

	printf(" CPU GPIO Init\n");
	rc = brd_gpio_init();
	if (rc) {
		printf("Error: Failed to init GPIO (rc=%d)\n", rc);
		return (rc);
	}

	printf(" Loading %s into RAM\n", xsvfFile);
	rc = xilinx_main(xsvfFile, debuglevel);
	if (rc) {
		printf("Error: Failed to load %s into RAM\n", xsvfFile);
		return (rc);
	}

	printf(" Loading %s into Flash\n", bitFile);
	rc = spi_program(bitFile);	
	if (rc) {
		printf("Error: Failed programming %s to flash\n", bitFile);
		return (rc);
	}

	printf(" Load image from Flash to Ram\n");
	rc = fpga_pulse();
	if (rc) {
		printf("Error: Failed Loading %s flash to Ram\n", bitFile);
		return (rc);
	}
	return (0);
}

int fpga_ui (int argc, char *argv[])
{
	int rc = 0;
	int debuglevel = 0;
	char* xsvfFile = "/diag/spi_loader_0.0.xsvf";
	char* bitFile = "/diag/mic_top.bit";

		// bernward add JTAG program command
	if (argc < 2) {
		printf("%s", diag_desc_fpga);
		return (-1);
	}

	if (!strncasecmp(argv[1], "prog", strlen("prog"))) {
		if (argc >= 3) {
			xsvfFile = argv[2];
		}
		if (argc > 3) {
			debuglevel = atoi(argv[3]);
		}

		rc = xilinx_main(xsvfFile, debuglevel);
	} else if (!strncasecmp(argv[1], "gpio", strlen("gpio"))) {
		rc = brd_gpio_init();
	} else if (!strncasecmp(argv[1], "flash", strlen("flash"))) {
		if (argc >= 3) {
			bitFile = argv[2];
		}
		rc = spi_program(bitFile);	
		if (rc) {
			printf(" Failed SPI Program (rc=%d)\n", rc);
			return (rc);
		}
	} else if (!strncasecmp(argv[1], "init", strlen("init"))) {
		if (argc >= 3) {
			xsvfFile = argv[2];
		}
		if (argc >= 4) {
			bitFile = argv[3];
		}

		return (fpga_flash_init(xsvfFile, bitFile, debuglevel));
	} else if (!strncasecmp(argv[1], "pulse", strlen("pulse"))) {
		rc = fpga_pulse();
		return 0;
	}	
	return (rc);
}
