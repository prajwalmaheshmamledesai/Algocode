/*****************************************************************************
 *      File:   woo_hw_error.h
 *      Name:   Raghu Boreda
 *
 *      Copied from redwood/red_hw_error.h
 *      Description: software error defines for Woodside 
 *      Copyright (c) 2009 by Cisco Systems, Inc.
 *****************************************************************************/
#ifndef WOODSIDE_HW_ERROR
#define WOODSIDE_HW_ERROR

enum _woodside_err_ { 
    WOODSIDE_ERROR_DEFAULT=1, 
    /* read over SBUS returns a failed result [1, table 3] , */ 
    WOODSIDE_ERROR_SBUS_READ,
    WOODSIDE_ERROR_SBUS_WRITE,
    /* polling for serdes ready after programming them timed out */
    WOODSIDE_ERROR_SERDES_RDY_TIMEOUT,
    WOODSIDE_ERROR_SERDES_PRBS_FAILURE,// a PRBS test has failed
    WOODSIDE_ERROR_I2C_NACK, // i2c request failed with NACK 
    WOODSIDE_ERROR_I2C_TIMEOUT, // i2c request failed with timeout
    WOODSIDE_ERROR_MEMIF_AUTOTEST_TIMEOUT,// memif autotest failed with timeout
    WOODSIDE_ERROR_MEMIF_AUTOVRFY_ERR, // data mismatch in autotest
    WOODSIDE_ERROR_MBIST_TIMEOUT,//mbist did not complete within expected time
    WOODSIDE_ERROR_MBIST_FAILURE,//mbist returned a failure error
    WOODSIDE_ERROR_XE_CNT_XG_CODE_VIOLATION,//mbist returned a failure error
};
#endif



