/********************************************************************
 *
 *      File:   mv88e1111_atu.c 
 *      Name:   Sudharshan Kadari
 *
 *      Description:
 *       Marvell LookUp Table Library 
 *
 *
 * Copyright (c) 1985-2007 by Cisco Systems, Inc.
 * All rights reserved.
 *
 *
 *********************************************************************/

#include <stdio.h>
#include <stdint.h>
#include "mv88e6095.h"
#include "mv88e6095_lib.h"

#define G1_ATU_CTRL_DBNUM			12	
#define G1_ATU_CTRL_DBNUM_MASK			0xF000
	
#define G1_ATU_OP_BUSY 				15	
#define G1_ATU_OP_BUSY_MASK 			0x8000	
#define G1_ATU_CTRL_OP	 			12
#define G1_ATU_OP_NOP 				0x00
#define G1_ATU_OP_FLUSH_ALL  			0x01
#define G1_ATU_OP_FLUSH_ALL_NON_STATIC		0x02
#define G1_ATU_OP_LOAD_OR_PURGE_DBNUM		0x03
#define G1_ATU_OP_GET_NEXT_DBNUM		0x04
#define G1_ATU_OP_FLUSH_DBNUM			0x05
#define G1_ATU_OP_FLUSH_NON_STATIC_DBNUM	0x06
#define G1_ATU_OP_GET_CLR_VIOLATION_DATA	0x07

#define G1_ATU_OP_MAC_PRI 			8	

#define G1_ATU_DATA_TRUNK			15 
#define G1_ATU_DATA_TRUNK_MASK			0x8000 
#define G1_ATU_DATA_PVEC 			4
#define G1_ATU_DATA_ENTRY_STATE_INVALID		0x00
#define G1_ATU_DATA_ENTRY_STATE_MGMT_OVERRIDE	0x0D
#define G1_ATU_DATA_ENTRY_STATE_STATIC		0x0E
#define G1_ATU_DATA_ENTRY_STATE_PRI_OVERRIDE	0x0F

#define G1_ATU_DATA_ENTRY_STATE_MCAST_NON_RATE_LIMITED	0x05
#define G1_ATU_DATA_ENTRY_STATE_MCAST_FILTERING		0x07

#define MV_ATU_LOCK()		
#define MV_ATU_UNLOCK()		

#define MV_RETERR(func)				\
{						\
	int rc;					\
	if ((rc = func)) {			\
		printf("Failed %s\n", __func__);\
		MV_ATU_UNLOCK();		\
		return (rc);			\
	}					\
}

typedef struct _mv_atu_flds_s
{
	uint8_t		rsvd		: 5;
	uint8_t		mac_pri		: 3;
	uint64_t	trunk		: 1;
	uint64_t	dpv_trunk_id	: 11;
	uint64_t	entry_state	: 4;
	uint64_t	mac_addr	: 48;
} __attribute__((packed)) mv_atu_flds_t;

static int mv_atu_busy(int fd, uint32_t address, uint8_t *busy)
{
	int rc = 0;
	uint16_t data = 0;
	uint8_t poll = 20;

	do {
		rc = mv88e6095_read(fd, address, PORT_GLOBAL1, 
				    MARVELL_G1_ATU_OP, &data);
		if (rc) {
			printf("Error: Failed to read ATU Ctrl (%d)\n", rc);
			return (rc);
		}

		*busy = (data & G1_ATU_OP_BUSY_MASK) ? 1 : 0;
	} while (*busy && --poll);

	return (rc);
}


static int mv_atu_wr ( int fd, uint32_t address, uint8_t dbnum, uint8_t atu_op, char *mac_addr, 
		uint8_t mac_pri, uint16_t port_vec, uint16_t entry_state)
{
	uint16_t data = 0;
	uint8_t busy = 0;

	MV_ATU_LOCK();

	MV_RETERR(mv_atu_busy(fd, address, &busy));
	if (busy) {
		printf("Error: ATU is continuously busy\n");
		MV_ATU_UNLOCK();
		return (-1);
	}

	// Load high part of dbnum.
	MV_RETERR(mv88e6095_read(fd, address, 
				PORT_GLOBAL1, MARVELL_G1_ATU_CTRL, &data));
	data &= ~G1_ATU_CTRL_DBNUM_MASK;		
	data |= (((dbnum >> 4) & 0x0F) << G1_ATU_CTRL_DBNUM); 
	MV_RETERR(mv88e6095_read(fd, address, 
				PORT_GLOBAL1, MARVELL_G1_ATU_CTRL, &data));

	// Enter Port Vector and Trunk ID
	data =   (port_vec << G1_ATU_DATA_PVEC) | entry_state;
	MV_RETERR(mv88e6095_write(fd, address, PORT_GLOBAL1, 
				  MARVELL_G1_ATU_DATA, data));

	// Load the Mac Address bytes 0 & 1.
	MV_RETERR(mv88e6095_write(fd, address, PORT_GLOBAL1, 
			MARVELL_G1_ATU_MAC01, *((uint16_t*)mac_addr)));

	// Load the Mac Address bytes 2 & 3.
	MV_RETERR(mv88e6095_write(fd, address, PORT_GLOBAL1, 
			MARVELL_G1_ATU_MAC23, *((uint16_t*)(mac_addr+2))));

	// Load the Mac Address bytes 4 & 5.
	MV_RETERR(mv88e6095_write(fd, address, PORT_GLOBAL1, 
			MARVELL_G1_ATU_MAC45, *((uint16_t*)(mac_addr+4))));

	// Load the ATU Op bytes.
	data =   G1_ATU_OP_BUSY_MASK | (atu_op << G1_ATU_CTRL_OP) |
		(mac_pri << G1_ATU_OP_MAC_PRI) | (dbnum & 0x0F);

	MV_RETERR(mv88e6095_write(fd, address, PORT_GLOBAL1, 
			MARVELL_G1_ATU_OP, data));

	MV_RETERR(mv_atu_busy(fd, address, &busy));
	if (busy) {
		printf("Failed to create ATU Entry\n");
		MV_ATU_UNLOCK();
		return (-1);
	}

	MV_ATU_UNLOCK();
	return (0);
}

int mv_atu_add( int fd, uint32_t address, uint8_t dbnum, char *mac_addr, uint8_t mac_pri, 
		uint16_t port_vec, uint16_t entry_state)
{
	return (mv_atu_wr (fd, address, dbnum, G1_ATU_OP_LOAD_OR_PURGE_DBNUM, mac_addr, 
		mac_pri, port_vec, entry_state));
}

int mv_atu_add_static( int fd, uint32_t address, uint8_t dbnum, char *mac_addr, uint8_t mac_pri, 
		uint16_t port_vec)
{
	return (mv_atu_wr (fd, address, dbnum, G1_ATU_OP_LOAD_OR_PURGE_DBNUM, mac_addr, 
		mac_pri, port_vec, G1_ATU_DATA_ENTRY_STATE_STATIC));
}

int mv_atu_del( int fd, uint32_t address, uint8_t dbnum, char *mac_addr, uint8_t mac_pri, 
		uint16_t port_vec)
{
	return (mv_atu_wr (fd, address, dbnum, G1_ATU_OP_LOAD_OR_PURGE_DBNUM, mac_addr, 
		mac_pri, port_vec, 0));
}

int mv_atu_dump (int fd, uint32_t address, uint8_t dbnum)
{
	uint16_t data = 0xFFFF, pvec;
	uint8_t  entry_state, busy, trunk, mac_pri;
	uint8_t  mac_addr[6] = "\xFF\xFF\xFF\xFF\xFF\xFF";

	MV_ATU_LOCK();


	// Initially program all FF's and read the control registers.
	MV_RETERR(mv_atu_wr (fd, address, dbnum, G1_ATU_OP_GET_NEXT_DBNUM, 
				mac_addr,  0, 0, 0x0));

	printf("DB : MacPri : T : PVEC/TID : ES/SPID :       MAC\n");
	do {
		MV_RETERR(mv88e6095_read(fd, address, PORT_GLOBAL1, 
				MARVELL_G1_ATU_DATA, &data));

		entry_state = data & 0x0F;
		pvec        = (data >> G1_ATU_DATA_PVEC) & 0x7FF; 
		trunk       = (data &  G1_ATU_DATA_TRUNK_MASK) ? 1: 0; 

		if (entry_state) {
			MV_RETERR(mv88e6095_read(fd, address, 
				PORT_GLOBAL1, MARVELL_G1_ATU_OP, &data));
			mac_pri = (data >> G1_ATU_OP_MAC_PRI) & 0x0F;

			MV_RETERR(mv88e6095_read(fd, address, 
				PORT_GLOBAL1, MARVELL_G1_ATU_MAC01, 
				(uint16_t*)mac_addr));
			MV_RETERR(mv88e6095_read(fd, address, 
				PORT_GLOBAL1, MARVELL_G1_ATU_MAC23, 
				(uint16_t*)&mac_addr[2]));
			MV_RETERR(mv88e6095_read(fd, address, 
				PORT_GLOBAL1, MARVELL_G1_ATU_MAC45, 
				(uint16_t*)&mac_addr[4]));

			printf ("%02X :   %02X   : %01X :   %04X   :   %02X    :" 
				"%02X-%02X-%02X-%02X-%02X-%02X\n",
				dbnum, mac_pri, trunk, pvec, entry_state, 
				mac_addr[0], mac_addr[1], mac_addr[2], 
				mac_addr[3], mac_addr[4], mac_addr[5]);

			// Get Next entry - Load the ATU Op bytes.
			data =   G1_ATU_OP_BUSY_MASK | 
				 (G1_ATU_OP_GET_NEXT_DBNUM << G1_ATU_CTRL_OP) | dbnum;
			MV_RETERR(mv88e6095_write(fd, address, 
				PORT_GLOBAL1, MARVELL_G1_ATU_OP, data));
		}
		MV_RETERR(mv_atu_busy(fd, address, &busy));
	} while (entry_state && !busy);

	MV_ATU_UNLOCK();
	return (0);
}

int mv_atu_flush_dbnum (int fd, uint32_t address, uint8_t dbnum)
{
	int		rc = 0;
	uint16_t	data = 0;
	uint8_t		busy;
	MV_ATU_LOCK();

	MV_RETERR(mv_atu_busy(fd, address, &busy));
	if (!busy) {

		data =   G1_ATU_OP_BUSY_MASK | 
			(G1_ATU_OP_FLUSH_DBNUM << G1_ATU_CTRL_OP) | dbnum;
		MV_RETERR(mv88e6095_write(fd, address, 
				PORT_GLOBAL1, MARVELL_G1_ATU_OP, data));
		MV_RETERR(mv_atu_busy(fd, address, &busy));

		if (busy) {
			printf("Error: Failed to Flush dbnum %d entries\n", 
				dbnum);
			MV_ATU_UNLOCK();	
			return (-1);
		}
	}
	MV_ATU_UNLOCK();	
	return (rc);
}

int mv_atu_flush_all (int fd, uint32_t address)
{
	int		rc = 0;
	uint16_t	data = 0;
	uint8_t		busy;
	MV_ATU_LOCK();

	MV_RETERR(mv_atu_busy(fd, address, &busy));
	if (!busy) {

		data =   G1_ATU_OP_BUSY_MASK | 
			(G1_ATU_OP_FLUSH_ALL << G1_ATU_CTRL_OP);
		MV_RETERR(mv88e6095_write(fd, address, 
			PORT_GLOBAL1, MARVELL_G1_ATU_OP, data));
		MV_RETERR(mv_atu_busy(fd, address, &busy));

		if (busy) {
			printf("Error: Failed to Flush All entries\n");
			MV_ATU_UNLOCK();	
			return (-1);
		}
	}
	MV_ATU_UNLOCK();	
	return (rc);
}
