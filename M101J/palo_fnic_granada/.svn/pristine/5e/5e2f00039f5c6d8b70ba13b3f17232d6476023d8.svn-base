/********************************************************************
 *
 *      File:   pilot2_mmap.c
 *      Name:   Harry Zhao
 *
 *      Description:
 *
 *
 * Copyright (c) 1985-2010 by Cisco Systems, Inc.
 * All rights reserved.
 *
 *
 *********************************************************************/

#include <errno.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <assert.h>
#include <asm/system.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "pilot2_mmap.h"


static int devmemFd = -1;


/*
**
** Memory map physical address to logical address
**
*/

int mmap_map_addr(const uint32_t physAddr, const uint32_t range, uint32_t *const pLogAddr)
{
    assert(pLogAddr);
    
    // check alignment of requested physical address
    assert((physAddr % getpagesize()) == 0);

    {
        if ((devmemFd=open("/dev/mem", O_RDWR | O_SYNC)) < 0) 
        {
            printf("%s(): failed to open /dev/mem. Errno %d:%s", 
                   __FUNCTION__, errno, strerror(errno));
            return -1;
        }
    }

    *pLogAddr = (unsigned long)mmap(0,
                                    (size_t) range,
                                    PROT_READ|PROT_WRITE,
                                    MAP_SHARED,
                                    devmemFd,
                                    (off_t) physAddr);
    
    if (*pLogAddr == (unsigned long)MAP_FAILED) 
    {
        close(devmemFd);
        devmemFd = -1;
        printf("%s(): mmap() failed. Errno %d:%s", __FUNCTION__,
               errno, strerror(errno));
        return -1;
    }

    return(0);
}


/*
**
** Memory unmap physical address to logical address
**
*/

int mmap_unmap_addr(const uint32_t virtAddr, const uint32_t len)
{
    if (munmap((void *)virtAddr, (unsigned int)len) < 0)
    {
        printf("%s(): munmap(addr=%08x, size=%08x) failed. Errno %d:%s", __FUNCTION__,
               virtAddr, len,
               errno, strerror(errno));
        return -1;
    }

    return(0);
}

