/////////////////////////////////////////////////////////////////////////////
// $Author: pkaplan $
// $Revision: 1.2 $
//
// sbus defines
//
// references:
// [1] AVAGO 90 nm SBus Specification
/////////////////////////////////////////////////////////////////////////////
#ifndef RED_HW_ERROR
#define RED_HW_ERROR

enum _redwood_err_ 
        { REDWOOD_ERROR_DEFAULT=1
        , REDWOOD_ERROR_SBUS_READ               // a read over the SBUS returned a failed result [1, table 3]
        , REDWOOD_ERROR_SERDES_RDY_TIMEOUT      // polling for serdes ready after programming them timed out
        , REDWOOD_ERROR_SERDES_PRBS_FAILURE     // a PRBS test has failed
        , REDWOOD_ERROR_I2C_NACK                // i2c request failed with NACK 
        , REDWOOD_ERROR_I2C_TIMEOUT             // i2c request failed with timeout
        , REDWOOD_ERROR_MEMIF_AUTOTEST_TIMEOUT  // memif autotest failed with timeout
        , REDWOOD_ERROR_MEMIF_AUTOVRFY_ERR      // data mismatch in autotest
        , REDWOOD_ERROR_MBIST_TIMEOUT           // mbist did not complete within expected time
        , REDWOOD_ERROR_MBIST_FAILURE           // mbist returned a failure error
};

#endif
