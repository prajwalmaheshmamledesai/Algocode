/********************************************************************//**
@file    bcm84823_ridgewood_lib.c 
@author  Raghu Boreda-- boreda@cisco.com
@brief   Phy 84823 Related functions
************************************************************************/
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>
#include <sys/time.h>
#include "diag_main.h"

#include "bcm84823.h"
#include "bcm84823_lib.h"

/* MDIO BUS #0 -> 4 PHY Ports
 */
static uint8_t ri_ports_mdio[] = {4};


int bcm84823_download_microcode (int fd, char *filename)
{
    int rc = 0;
    int max_mdio = 1;
    rc = bcm84823_gen_download_microcode(fd, filename, max_mdio, &ri_ports_mdio[0]);
    return rc;
}
