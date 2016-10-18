/********************************************************************
 *
 *      File:   mv88e6095_reg_desc.c 
 *      Name:   Sudharshan Kadari
 *
 *      Description:
 *       Marvell register description 
 *
 *
 * Copyright (c) 1985-2007 by Cisco Systems, Inc.
 * All rights reserved.
 *
 *
 *********************************************************************/

#include <stdio.h>
#include "diag_reg.h"
#include "mv88e6095_reg_desc.h"

#ifndef DIAG_FEX
extern reg_desc_t mv88e1111_reg_desc[];
#endif

reg_desc_t mv88e6095_port_reg_desc[] =
{
MV88E6095_PORT_REG_DESC
{ TYP_NONE, "", 0, 0, "", "", 0, 0, ""}
};


reg_desc_t mv88e6095_global1_reg_desc[] =
{
MV88E6095_GLOBAL1_REG_DESC
{ TYP_NONE, "", 0, 0, "", "", 0, 0, ""}
};

reg_desc_t mv88e6095_global2_reg_desc[] =
{
MV88E6095_GLOBAL2_REG_DESC
{ TYP_NONE, "", 0, 0, "", "", 0, 0, ""}
};

reg_desc_t mv88e6095_phy_reg_desc[] =
{
MV88E6095_PHY_REG_DESC
{ TYP_NONE, "", 0, 0, "", "", 0, 0, ""}
};

reg_desc_t *mv88e6095_preg_desc[32] = {
	mv88e6095_phy_reg_desc,			// Port 0
	mv88e6095_phy_reg_desc,			// Port 1
	mv88e6095_phy_reg_desc,			// Port 2
	mv88e6095_phy_reg_desc,			// Port 3
	mv88e6095_phy_reg_desc,			// Port 4
	mv88e6095_phy_reg_desc,			// Port 5
	mv88e6095_phy_reg_desc,			// Port 6
	mv88e6095_phy_reg_desc,			// Port 7
#ifdef DIAG_FEX
	NULL,					// Port 8
	NULL,					// Port 9
	NULL,					// Port A
#else
	mv88e1111_reg_desc,			// Port 8
	mv88e1111_reg_desc,			// Port 9
	mv88e1111_reg_desc,			// Port A
#endif
	NULL,					// Port B
	NULL,					// Port C
	NULL,					// Port D
	NULL,					// Port E
	NULL,					// Port F
	mv88e6095_port_reg_desc,		// Port 10
	mv88e6095_port_reg_desc,		// Port 11
	mv88e6095_port_reg_desc,		// Port 12
	mv88e6095_port_reg_desc,		// Port 13
	mv88e6095_port_reg_desc,		// Port 14
	mv88e6095_port_reg_desc,		// Port 15
	mv88e6095_port_reg_desc,		// Port 16
	mv88e6095_port_reg_desc,		// Port 17
	mv88e6095_port_reg_desc,		// Port 18
	mv88e6095_port_reg_desc,		// Port 19
	mv88e6095_port_reg_desc,		// Port 1A
	mv88e6095_global1_reg_desc,		// Port 1B
	mv88e6095_global2_reg_desc,		// Port 1C
	NULL,					// Port 1D
	NULL,					// Port 1E
	NULL,					// Port 1F
};
