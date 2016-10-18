/********************************************************************
 *
 *      File:   asic_acc.c
 *      Name:   Sudharshan Kadari 
 *
 *      Description: ASIC level functions for Redwood USD
 *
 *
 *      Copyright (c) 1984-2009 by Cisco Systems, Inc.
 ********************************************************************/
#include <stdio.h>
#include <stdint.h>
#include "ppc_lib.h"

#define ASIC_I2C_ADDR	0x48
#define ASIC_I2C_BUS	0x05

uint32_t asic_i2c_addr[] = {ASIC_I2C_ADDR};
uint32_t asic_i2c_bus[] = {ASIC_I2C_BUS};
#ifndef DIAG_IOM2
extern uint32_t peer_ip_addr;
extern unsigned int network_util_write_model_reg( unsigned int reg_addr, unsigned int reg_data );
extern unsigned int network_util_read_model_reg( unsigned int reg_addr , unsigned int *data);
#endif

/***************************************************************************
The RNAXI protocol defines the following format for read and write operations. 

Note the difference in Endianess between the address and data values in the 
protocol.

Legend:

dev[7:1] is the I2C slave device address
A[x:y]    is address bits x through y, inclusive
D[x:y]    is data       bits x through y, inclusive

 3             2  2            1  1             
 1             4  3            6  5             8  7             1
+---------------+----------------+----------------+---------------+
|               |                |                |               |
+---------------+----------------+----------------+---------------+
Figure 4 Bit Numbering


4.5.1.1.1   RNAXI Protocol Read
+----------+-+-------------+------------+------------+
|Byte0[7:1]|R|    Byte1    |   Byte2    |   Byte3    |
+----------+-+-------------+------------+------------+
| Dev[7:1] |1|  A [23:16]  | A [15:8]   | A [7:0]    |
+----------+-+-------------+------------+------------+
Figure 5 RNAXI Read Transaction

The first byte contains a 7-bit I2C slave device address and a 1-bit command 
(Read=1, Write=0) The next three bytes specify the Redwood register address, 
in Big Endian order.  Four bytes of data will be returned by the RNAXI device, 
in Little Endian order and must be consumed by the driver.

4.5.1.1.2   RNAXI Protocol Write
+----------+-+-------------+------------+------------+
|Byte0[7:1]|W|    Byte1    |   Byte2    |   Byte3    |
+----------+-+-------------+------------+------------+
| Dev[7:1] |0|  A [23:16]  | A [15:8]   | A [7:0]    |
+----------+-+-------------+------------+------------+

followed by 4 bytes of data (in Little Endian order)

+------------+-------------+------------+------------+
|Byte4       |    Byte5    |   Byte6    |   Byte7    |
+------------+-------------+------------+------------+
| D[7:0]     |  D [15:8]   | S[23:16]   | S[31:24]   |
+------------+-------------+------------+------------+
Figure 6 RNAXI Write Transaction

The first byte contains a 7-bit I2C slave device address and a 1-bit command 
(Read=1, Write=0). The next three bytes specify the Redwood register address, 
in Big Endian order.  The next four bytes specify the data to be written to 
the addressed register within the Redwood ASIC, in Little Endian order
*****************************************************************************/

// byte-swap 32-bit value. swap of a ntohl'd value is LITTLE_ENDIAN
#define swap_32(a) (((a >> 24) & 0xff)     |\
                    ((a >> 8)  & 0xff00)   |\
                    ((a << 8 ) & 0xff0000) |\
                     (a << 24))

#define ASIC_I2C_ADDR_SIZE	3
#define ASIC_I2C_DATA_SIZE	4

static int asic_fd = -1;

static int rnaxi_read(int fd, uint32_t i2c_addr , uint32_t reg, uint32_t *data )
{
    int err = 0;
    uint32_t pdata = 0;

    err = diag_i2c_rd (fd, i2c_addr, reg, 
		ASIC_I2C_ADDR_SIZE, (uint8_t*)&pdata, 
		ASIC_I2C_DATA_SIZE);

    /* return data in "native" format. Since we know data is LE, if we 
     * swap it it will become BE (BE == network byte order). So ntohl
     * should do the trick. */
    *data = swap_32(pdata);
    return( err );
}

static int rnaxi_write(int fd, uint32_t i2c_addr, uint32_t reg, uint32_t data )
{   

    uint32_t pdata = swap_32(data); 
    return(diag_i2c_wr(fd, i2c_addr, reg,
		ASIC_I2C_ADDR_SIZE, (uint8_t*)&pdata, 
		ASIC_I2C_DATA_SIZE));
}


int asic_i2c_rd (uint8_t inst, uint32_t addr, uint32_t *data)
{
	int rc = 0;
#ifndef DIAG_IOM2
	if (peer_ip_addr) {
		printf(" Woo%d Rd 0x%X\n", inst, addr);
		rc = network_util_read_model_reg(addr, data);
	} else 
#endif
	{
		if (asic_fd == -1) {
			asic_fd = ppc_i2c_open(asic_i2c_bus[inst]);
		}

		// Multiply address by 4 to make it byte address.
		rc = rnaxi_read(asic_fd, asic_i2c_addr[inst], (addr), (uint32_t*)data);
	}
//	printf("Rd [0x%X] : 0x%X\n", addr, *data);
//	printf(" RD ADDR [0x%X.0x%X]\n", woo_mmap_addr,addr);
	return (rc);	
}

int asic_i2c_wr (uint8_t inst, uint32_t addr, uint32_t data)
{
	int rc = 0;
#ifndef DIAG_IOM2
	if (peer_ip_addr) {
		rc = network_util_write_model_reg(addr, data);
	} else 
#endif
	{
		if (asic_fd == -1) {
			asic_fd = ppc_i2c_open(asic_i2c_bus[inst]);
		}
		// Multiply address by 4 to make it byte address.
		rc = rnaxi_write(asic_fd, asic_i2c_addr[inst], (addr), data);
	}
//	printf("Wr [0x%X] : 0x%X\n", addr, data);
	return (rc);
}
