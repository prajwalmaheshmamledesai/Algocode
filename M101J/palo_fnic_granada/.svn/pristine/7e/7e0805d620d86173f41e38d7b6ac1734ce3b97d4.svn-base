/********************************************************************
 *
 *      File:  psu_api.h
 *      Name:  Matt Strathman
 *
 *      Description: external api for psu access
 *
 * Copyright (int fd,c) 1985-2007 by Cisco Systems, Inc.
 * All rights reserved.
 *  
 *  
 * $Id:$
 * $Source:$
 * $Author:$
 * ---------------------------------------------------------------
 **********************************************************************/
/**********************************************************************
@file    psu_api.h
@author  Matt Strathman -- mstrathman@nuovasystems.com
@brief   external api for psu access
************************************************************************/
#ifndef __PSU_API_H_
#define __PSU_API_H_

#include <errno.h>

/*
 * error return codes
 */
#define PSU_ERR_INVALID EINVAL
#define PSU_ERR_NOMEM   ENOMEM
#define PSU_ERR_NODEV   ENODEV
#define PSU_ERR_IO      EIO

/*
 * list of psu utility supported platforms
 */
typedef enum {
  PSU_PLATFORM_SANTA_CLARA,
} psu_platform_t;

/*
 * list of psu utility commands
 */
typedef enum {
  PSU_CMD_SHOW_ALL,
  PSU_CMD_SHOW_PSMI_HEADER,
  PSU_CMD_SHOW_CAPABILITY,
  PSU_CMD_SHOW_SENSORS,
  PSU_CMD_SHOW_STATUS,
  PSU_CMD_SHOW_RECORDS,
} psu_cmd_t;

/**
 * @brief
 *  show psmi registers read from psu instance
 *
 * @return
 *  <b>int</b> - PASS >= 0
 *             - FAIL < 0
 *             .
 */
extern int
psu_util_show (int fd, uint32_t ps_inst, /**< psu instance index */
           psu_cmd_t cmd,    /**< display command */
           uint32_t verbose  /**< selects verbose or terse display */ );


extern int psu_rd (int fd,  uint8_t ps_inst, uint8_t offset, uint8_t *val ); 
extern int psu_wr (int fd,  uint8_t ps_inst, uint8_t offset, uint16_t val ); 
extern int psu_present(int fd, uint8_t ps_inst);
extern int psu_ui (int fd, int argc, char *argv[]);

            
/**
 * @brief
 *  return the psmi revision of a psu
 *
 * @return
 *  <b>int</b> - >= 0: PASS
 *             - < 0 : FAIL
 *             .
 */
extern int
psu_util_get_psmi_revision (int fd, uint32_t inst, uint32_t *major, uint32_t *minor );

#endif
