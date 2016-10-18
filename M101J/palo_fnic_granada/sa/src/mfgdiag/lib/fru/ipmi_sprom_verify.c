#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <stdint.h>
#include "diag_sprom.h"

static int
ipmi_sprom_verify_lc ( const uint8_t *buf )
{
    int rc = 0;
    return rc;
}

static int 
ipmi_sprom_verify_bp( const uint8_t *buf )
{   
    int rc = 0;
    return rc;
}

static int
ipmi_sprom_verify_sup ( const uint8_t *buf )
{
    int rc = 0;
    return rc;
}

static int
ipmi_sprom_verify_ps( const uint8_t *buf )
{
    int rc = 0;
    return rc;
}

static int
ipmi_sprom_verify_fan ( const uint8_t *buf )
{
    int rc = 0;
    return rc;
}

int
ipmi_sprom_verify ( diag_sprom_t * const sprom, const uint8_t *buf )
{
    switch ( sprom->type ) {
    case SPROM_UTIL_TYPE_SUP:
        return ( ipmi_sprom_verify_sup ( buf ) );
	break;

    case SPROM_UTIL_TYPE_LC:
        return ( ipmi_sprom_verify_lc ( buf ) );
	break;
    case SPROM_UTIL_TYPE_BP:
        return ( ipmi_sprom_verify_bp ( buf ) );
    case SPROM_UTIL_TYPE_PS:
	break;
        return ( ipmi_sprom_verify_ps ( buf ) );
    case SPROM_UTIL_TYPE_FAN:
        return ( ipmi_sprom_verify_fan ( buf ) );
	break;

    case SPROM_UTIL_TYPE_MOD_HOST:
    case SPROM_UTIL_TYPE_IBMC:
    case SPROM_UTIL_TYPE_MENLO:
    case SPROM_UTIL_TYPE_MENLO_E:
    case SPROM_UTIL_TYPE_OPLIN:
    case SPROM_UTIL_TYPE_MOD_MEZZ:
    case SPROM_UTIL_TYPE_PALO:
    case SPROM_UTIL_TYPE_MONTEREYPARK:
    case SPROM_UTIL_TYPE_NIANTIC:
    case SPROM_UTIL_TYPE_EVEREST:
    case SPROM_UTIL_TYPE_TIGERSHARK:
    case SPROM_UTIL_TYPE_NETEFFECT:
    case SPROM_UTIL_TYPE_SCHULTZ:
    case SPROM_UTIL_TYPE_BBU:
        break;

    default:
        return ( -SPROM_ERR_NODEV );
    }

    return ( 0 );
}
