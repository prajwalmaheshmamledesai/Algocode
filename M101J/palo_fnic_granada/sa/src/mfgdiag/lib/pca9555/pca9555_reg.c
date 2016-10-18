/********************************************************************
 *
 *      File:   pca9555_reg.c
 *      Name:   Sudharshan Kadari
 *
 *      Description:
 *       Digital PWM System Controller register description
 *
 *
 * Copyright (c) 1985-2009 by Cisco Systems, Inc.
 * All rights reserved.
 *
 *
 *********************************************************************/

#include <stdio.h>
#include <stdint.h>
#include "diag_reg.h"
#include "pca9555.h"

reg_desc_t pca9555_reg_desc[] =
{
RCFG(PCA9555_INPUT_0)
RCFG(PCA9555_INPUT_1)
RCFG(PCA9555_OUTPUT_0)
RCFG(PCA9555_OUTPUT_1)
RCFG(PCA9555_POL_0)
RCFG(PCA9555_POL_1)
RCFG(PCA9555_CFG_0)
RCFG(PCA9555_CFG_1)

{ TYP_NONE, "", 0, 0, "", "", 0, 0, ""}
};
