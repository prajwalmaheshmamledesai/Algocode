#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <sys/types.h>

#include "sprom.h"
#include "diag_sprom_defs.h"


static int 
_verify_block_header( sprom_block_header_t *blk_hdr,
  const char*  name, const uint32_t sig, const uint32_t ver, const uint32_t len)
{
    uint32_t    err = 0;
    uint16_t    computed_chksum;
    uint16_t    blk_len;

    if (blk_hdr->block_signature != sig) {
        sprom_printf ( "%s block signature mismatch. Found 0x%04x, "
          "expected 0x%04x.\n",name, blk_hdr->block_signature, sig);
        err++;
    }

    if (blk_hdr->block_version != ver) {
        sprom_printf ( "%s block version mismatch.   Found 0x%04x, "
          "expected 0x%04x.\n", name, blk_hdr->block_version, ver);
        err++;
    }

    // interpret a block length of zero as 256
    blk_len = (blk_hdr->block_length == 0) ? 256: blk_hdr->block_length;
    if (blk_len != len) {
        sprom_printf ( "%s block length mismatch.    Found 0x%04x, "
          "expected 0x%04x.\n", name, blk_len, len);
        err++;
    }

    computed_chksum = diag_sprom_compute_xsum(blk_hdr, len);
    if (blk_hdr->block_checksum != computed_chksum) {
        sprom_printf ( "%s block checksum mismatch.  Found 0x%04x, "
          "expected 0x%04x.\n", name, blk_hdr->block_checksum, computed_chksum);
        err++;
    }

    return (err == 0) ? 0 : ( -SPROM_ERR_INVALID );

}

static int
sprom_verify_lc ( const uint8_t *buf )
{
    int rc = 0;
    sprom_block_header_t *cur_blk_hdr;
    line_card_sprom_t    *lc_sprom = (line_card_sprom_t *)buf;
    int                   err = 0;

    assert ( lc_sprom );

    /*
    ** Check common header block
    */
    cur_blk_hdr = &lc_sprom->common_block.block_header;
    rc = _verify_block_header( cur_blk_hdr, "Line card Common Header",
                               SPROM_COMMON_BLOCK_SIG,
                               SPROM_COMMON_BLOCK_CUR_VER,
                               SPROM_COMMON_BLOCK_DATA_SIZE);
    if ( rc < 0 )
        err++;

    /*
    ** Check linecard specific block
    */
    cur_blk_hdr = &lc_sprom->lc_block.block_header;
    rc = _verify_block_header( cur_blk_hdr, "Line card Main",
                               SPROM_LINE_CARD_BLOCK_SIG,
                               SPROM_LINE_CARD_BLOCK_CUR_VER,
                               SPROM_LINE_CARD_BLOCK_DATA_SIZE);
    if ( rc < 0 )
        err++;

    rc = err ? ( -SPROM_ERR_INVALID ) : err;

    return rc;
}

static int 
sprom_verify_bp( const uint8_t *buf )
{   
    int rc = 0;
    sprom_block_header_t *cur_blk_hdr;
    backplane_sprom_t    *bp_sprom = (backplane_sprom_t *) buf;
    int                   err = 0;

    assert ( bp_sprom );
    
    /*
    ** Check common header block
    */
    cur_blk_hdr = &bp_sprom->common_block.block_header; 
    rc = _verify_block_header( cur_blk_hdr, "Backplane Common Header",
                               SPROM_COMMON_BLOCK_SIG,
                               SPROM_COMMON_BLOCK_CUR_VER,
                               SPROM_COMMON_BLOCK_DATA_SIZE);
    if ( rc < 0 )
        err++;

    /*
    ** Check backplane block
    */
    cur_blk_hdr = &bp_sprom->backplane_block.block_header;
    rc = _verify_block_header( cur_blk_hdr, "Backplane Main",
                               SPROM_BACKPLANE_BLOCK_SIG,
                               SPROM_BACKPLANE_BLOCK_CUR_VER,
                               SPROM_BACKPLANE_BLOCK_DATA_SIZE);
    if ( rc < 0 )
        err++;

    /*
    ** Check backplane WWN block
    */
    cur_blk_hdr = &bp_sprom->wwn_block.block_header;
    rc = _verify_block_header( cur_blk_hdr, "Backplane WWN",
                               SPROM_WWN_BLOCK_SIG,
                               SPROM_WWN_BLOCK_CUR_VER,
                               SPROM_WWN_BLOCK_DATA_SIZE);
    if ( rc < 0 )
        err++;

    /*
    ** Check backplane license block
    */
    cur_blk_hdr = &bp_sprom->lic_block.block_header;
    rc = _verify_block_header( cur_blk_hdr, "Backplane License",
                               SPROM_LIC_BLOCK_SIG,
                               SPROM_LIC_BLOCK_CUR_VER,
                               SPROM_LIC_BLOCK_DATA_SIZE);
    if ( rc < 0 )
        err++;

#if !defined(NUOVA_OR_DIAG)
    /*
    ** 2nd serial number block
    */
    cur_blk_hdr = &bp_sprom->sn2_block.block_header;
    rc = _verify_block_header( cur_blk_hdr, "2nd Serial Number",
                               SPROM_SN2_BLOCK_SIG,
                               SPROM_SN2_BLOCK_CUR_VER,
                               SPROM_SN2_BLOCK_DATA_SIZE);
    if ( rc < 0 )
        err++;
#endif

    rc = err ? ( -SPROM_ERR_INVALID ) : err;

    return rc;
}

static int
sprom_verify_sup ( const uint8_t *buf )
{
    int rc = 0;
    sprom_block_header_t *cur_blk_hdr;
    sup_sprom_t          *sup_sprom = (sup_sprom_t *) buf;
    int                   err = 0;

    assert ( sup_sprom );

    /*
    ** Check common header block
    */
    cur_blk_hdr = &sup_sprom->common_block.block_header;
    rc = _verify_block_header( cur_blk_hdr, "Supervisor Common Header",
                               SPROM_COMMON_BLOCK_SIG,
                               SPROM_COMMON_BLOCK_CUR_VER,
                               SPROM_COMMON_BLOCK_DATA_SIZE);
    if ( rc < 0 )
        err++;

    /*
    ** Check supervisor specific block
    */
    cur_blk_hdr = &sup_sprom->sup_block.block_header;
    rc = _verify_block_header( cur_blk_hdr, "Supervisor Main",
                               SPROM_SUP_BLOCK_SIG,
                               SPROM_SUP_BLOCK_CUR_VER,
                               SPROM_SUP_BLOCK_DATA_SIZE);
    if ( rc < 0 )
        err++;

    /*
    ** Check extra sensor block
    */
    cur_blk_hdr = &sup_sprom->sensor_block.block_header;
    rc = _verify_block_header( cur_blk_hdr, "Supervisor Extra Sensor",
                               SPROM_SENSOR_BLOCK_SIG,
                               SPROM_SENSOR_BLOCK_CUR_VER,
                               SPROM_SENSOR_BLOCK_DATA_SIZE);
    if ( rc < 0 )
        err++;

    rc = err ? ( -SPROM_ERR_INVALID ) : err;

    return rc;
}

#ifdef DIAG_FEX
static int
sprom_verify_ps( const uint8_t *buf )
{
    int rc = 0;
    power_supply_sprom_fex_t *ps_sprom = (power_supply_sprom_fex_t *)buf;
    int                   err = 0;

    assert ( ps_sprom );

    /*
    ** Check common header block
    */
    rc = _verify_block_header( (sprom_block_header_t*)ps_sprom, 
				"Power Supply Common Header",
                               SPROM_COMMON_BLOCK_SIG, 
    				(ps_sprom->block_version == 
				SPROM_POWER_SUPPLY_BLOCK_FEX_VER) ?
				SPROM_POWER_SUPPLY_BLOCK_FEX_VER:
				SPROM_POWER_SUPPLY_BLOCK_CUR_VER,
				SPROM_POWER_SUPPLY_FEX_DATA_SIZE);
    if ( rc < 0 )
        err++;

    rc = err ? ( -SPROM_ERR_INVALID ) : err;

    return rc;
}
#else
static int
sprom_verify_ps( const uint8_t *buf )
{
    int rc = 0;
    sprom_block_header_t *cur_blk_hdr;
    power_supply_sprom_t *ps_sprom = (power_supply_sprom_t *)buf;
    int                   err = 0;

    assert ( ps_sprom );

    /*
    ** Check common header block
    */
    cur_blk_hdr = (sprom_block_header_t*)&ps_sprom->common_block;
    rc = _verify_block_header( cur_blk_hdr, "Power Supply Common Header",
                               SPROM_COMMON_BLOCK_SIG,
                               SPROM_COMMON_BLOCK_CUR_VER,
                               SPROM_COMMON_BLOCK_DATA_SIZE);
    if ( rc < 0 )
        err++;

    /*
    ** Check power supply block
    */
    cur_blk_hdr = (sprom_block_header_t*)&ps_sprom->power_supply_block;
    rc = _verify_block_header( cur_blk_hdr, "Power Supply Main",
                               SPROM_POWER_SUPPLY_BLOCK_SIG_1,
                               SPROM_POWER_SUPPLY_BLOCK_CUR_VER,
                               SPROM_POWER_SUPPLY_BLOCK_DATA_SIZE);
    if ( rc < 0 )
        err++;

    rc = err ? ( -SPROM_ERR_INVALID ) : err;

    return rc;
}
#endif
static int
sprom_verify_fan ( const uint8_t *buf )
{
    int rc = 0;
    sprom_block_header_t *cur_blk_hdr;
    fan_sprom_t          *fan_sprom = (fan_sprom_t *)buf;
    int                   err = 0;

    assert ( fan_sprom );

    /*
    ** Check common header block
    */
    cur_blk_hdr = &fan_sprom->common_block.block_header;
    rc = _verify_block_header( cur_blk_hdr, "Fan Common Header",
                               SPROM_COMMON_BLOCK_SIG,
                               SPROM_COMMON_BLOCK_CUR_VER,
                               SPROM_COMMON_BLOCK_DATA_SIZE);
    if ( rc < 0 )
        err++;

    /*
    ** Check fan block
    */
    cur_blk_hdr = &fan_sprom->fan_block.block_header;
    rc = _verify_block_header( cur_blk_hdr, "Fan Main",
                               SPROM_FAN_BLOCK_SIG,
                               SPROM_FAN_BLOCK_CUR_VER,
                               SPROM_FAN_BLOCK_DATA_SIZE);
    if ( rc < 0 )
        err++;

    /*
    ** 2nd serial number block
    */
#if !defined( NUOVA_OR_DIAG )
    cur_blk_hdr = &fan_sprom->sn2_block.block_header;
    rc = _verify_block_header( cur_blk_hdr, "2nd Serial Number",
                               SPROM_SN2_BLOCK_SIG,
                               SPROM_SN2_BLOCK_CUR_VER,
                               SPROM_SN2_BLOCK_DATA_SIZE);
    if ( rc < 0 )
        err++;
#endif

    rc = err ? ( -SPROM_ERR_INVALID ) : err;

    return rc;
}

int
sprom_verify ( diag_sprom_t * const sprom, const uint8_t *buf )
{
    switch ( sprom->type ) {
    case SPROM_UTIL_TYPE_SUP:
        return ( sprom_verify_sup ( buf ) );
    case SPROM_UTIL_TYPE_LC:
        return ( sprom_verify_lc ( buf ) );
    case SPROM_UTIL_TYPE_BP:
        return ( sprom_verify_bp ( buf ) );
    case SPROM_UTIL_TYPE_PS:
        return ( sprom_verify_ps ( buf ) );
    case SPROM_UTIL_TYPE_FAN:
        return ( sprom_verify_fan ( buf ) );
    case SPROM_UTIL_TYPE_BROCKWAY_UPLINK_SUP:
        return ( sprom_verify_sup ( buf ) );
    case SPROM_UTIL_TYPE_BROCKWAY_UPLINK_BP:
        return ( sprom_verify_bp ( buf ) );
    default:
        return ( -SPROM_ERR_NODEV );
    }

    return ( 0 );
}
