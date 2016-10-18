/********************************************************************//**
@file    bcm84823_brockway_lib.c 
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

/* MDIO BUS #0 -> 12 PHY Ports
 *          #1 -> 10 PHY Ports
 *          #2 -> 10 PHY Ports
 *          #3 -> 8 PHY Ports only for 10G_BASE_T IO cards
 */
int bcm84823_download_microcode (int fd, char *filename)
{
    int rc = 0;
    int numphys = sys_dev_count_get(DEV_CUPHY);
    
    static uint8_t ports_mdio[] = {12,10,10,8};
    rc = bcm84823_gen_download_microcode(fd, filename, (numphys/8)-1, 
                                         &ports_mdio[0]);
    return rc;
}
