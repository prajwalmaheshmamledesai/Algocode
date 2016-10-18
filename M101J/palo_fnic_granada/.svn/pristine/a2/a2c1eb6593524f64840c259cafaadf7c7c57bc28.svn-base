/********************************************************************
 *
 *      File:   sprom_cmds.c
 *      Name:   Curt Brune, matt strathman
 *
 *      Description:
 *       sprom misc cmd handlers
 *
 * Copyright (c) 2007 by Cisco Systems, Inc.
 * All rights reserved.
 *
 ********************************************************************/

#include <stdio.h>
#include "sys/types.h"
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include "sprom.h"
#include "diag_sprom.h"
#include "diag_sprom_defs.h"

static int 
_dump_data( diag_sprom_t *sprom, uint8_t *buf )
{
    int    i;

    sprom_printf ( "Dumping %s SPROM", sprom_get_name( sprom ) );
    sprom_printf ( " contents in hex.\n" );

    for (i = 0; i < sprom_get_size ( sprom ); i++) {
        if (i % 16==0) {
            sprom_printf ( "\n0x%04x: ", i );
        }
        sprom_printf ( "%02x ", buf[i] );
    }
    sprom_printf ( "\n" );
    
    return ( 0 );
}

int
sprom_dump ( diag_sprom_t *const sprom, uint32_t offset, uint32_t len )
{
    int rc = 0;
    uint8_t *buf;            

    // set number of bytes to dump as passed from CLI
    if ( len > (uint32_t)sprom_get_size ( sprom ) )
        len = (uint32_t)sprom_get_size ( sprom );
    
    buf = malloc( len );
    if ( buf == NULL ) {
        sprom_err ( "malloc failed\n");
        return ( -SPROM_ERR_NOMEM );
    }

    if ( ( rc = diag_sprom_rd ( sprom, offset, len, buf ) ) < 0 ) {
        sprom_err ("Unable to read sprom contents.\n");
        if ( buf ) free ( buf );
        return ( -SPROM_ERR_IO );
    }

    rc = _dump_data( sprom, buf );

    if ( buf ) free( buf );

    return rc;
}

static int
_sprom_fill_raw(diag_sprom_t *sprom, uint32_t offset, uint32_t len, 
  uint8_t fill_value)
{
    int rc;
    char   input;  
    uint8_t *buf;

    sprom_printf ( "Do you really want to fill the SPROM with "
      "0x%02x (y/n) [n]? ", fill_value);
    input = getchar();
    while( getchar() != '\n' );

    if ( input != 'y' && input != 'Y') {
        sprom_printf ( "Aborting SPROM modification ...\n" );
        return 0;
    } 
    
    // allocate data buffers
    if ( len > (uint32_t)sprom_get_size ( sprom ) ) 
        len = (uint32_t)sprom_get_size ( sprom );

    buf = malloc( len );
    if ( buf == NULL ) {
        sprom_err ( "malloc failed\n" );
        return ( -SPROM_ERR_NOMEM );
    }
 
    memset( buf, fill_value, len );
    
    if ( ( rc = diag_sprom_wr ( sprom, offset, len, buf ) ) < 0 ) {
         sprom_err ( "Unable to write sprom contents.\n" );
         if ( buf ) free( buf );
         return ( -SPROM_ERR_IO );
    }

    if ( buf ) free( buf );

    return ( 0 );

}

int 
sprom_invalidate( diag_sprom_t *const sprom )
{
    // A "fresh from the factory" untouched SPROM usually is filled
    // with 0xFF.  This appears to the software as being blank.
    return ( _sprom_fill_raw( sprom, 0, sprom_get_size ( sprom ), 0xFF) );
}

int
sprom_zero( diag_sprom_t *const sprom )
{
    return ( _sprom_fill_raw ( sprom, 0, sprom_get_size ( sprom ), 0x00) );
}


int sprom_ui (int argc, char *argv[])
{
	int	 rc = 0;
	char	 *endp;
	int	 len = 0;
	uint32_t cmd  = 0;
	uint32_t type = 0;
	uint32_t slot = 0, sup_slot = 0;
	uint32_t offset = 0;
	uint32_t length = 0;

	sprom_platform_t platform;
	diag_sprom_t *sprom;
	uint8_t *rwbuf;

	if ( argc < 3 ) {
		return (-1);
	}

	argc--;
	argv++;
	if ( strcasecmp (*argv, "fex" ) == 0 ) {
		platform = SPROM_PLATFORM_FEX;
		argc--;
		argv++;
	} else if ( strcasecmp (*argv, "iom" ) == 0 ) {
		platform = SPROM_PLATFORM_IOM;
		argc--;
		argv++;
	} else {
		if ((platform = sprom_get_platform ( ) ) < 0 ) {
			fprintf ( stderr, "Err: Can't determine platform type,"
					" please specify\n" );
			return (-2);
		}
	}

	if ( sprom_get_slot_sup ( platform, &sup_slot) < 0 ) {
		fprintf ( stderr, "Err: Could not get supervisor slot\n" );
		return (-3);
	}

	//
	// get cmd
	len = strlen ( *argv );

	if ( strncmp ( *argv, "set", len ) == 0 ) {
		cmd = SPROM_CMD_SET;
	} else if ( strncmp ( *argv, "show", len ) == 0 ) {
		cmd = SPROM_CMD_SHOW;
	} else if ( strncmp ( *argv, "set-serial", len ) == 0 ) {
		cmd = SPROM_CMD_SET_SERIAL;
	} else if ( strncmp ( *argv, "set-part-num", len ) == 0 ) {
		cmd = SPROM_CMD_SET_PART_NUM;
	} else if ( strncmp ( *argv, "set-part-rev", len ) == 0 ) {
		cmd = SPROM_CMD_SET_PART_REV;
	} else if ( strncmp ( *argv, "set-license", len ) == 0 ) {
		cmd = SPROM_CMD_SET_LICENSE;
	} else if ( strncmp ( *argv, "set-mac", len ) == 0 ) {
		cmd = SPROM_CMD_SET_MAC;
	} else if ( strncmp ( *argv, "dump", len ) == 0 ) {
		cmd = SPROM_CMD_DUMP;
		argc--;
		argv++;

		if ( argc ) {
			offset = strtoul (*argv, &endp, 0 );
			if (*endp) {
				return ( -SPROM_ERR_INVALID );
			}

			argc--;
			argv++;

			if ( argc ) {
				length = strtoul ( *argv, &endp, 0 );

				if ( *endp ) {
					return ( -SPROM_ERR_INVALID );
				}
			} else {
				return ( -SPROM_ERR_INVALID );
			}
		} else {
			return ( -SPROM_ERR_INVALID );
		}
	} else if ( strncmp ( *argv, "invalidate", len ) == 0 ) {
		cmd = SPROM_CMD_INVALIDATE;
	} else if ( strncmp ( *argv, "zero", len ) == 0 ) {
		cmd = SPROM_CMD_ZERO;
	} else {
		return (-1);
	}

	argc--;
	argv++;

	//  get type
	if (!argc) {
		printf("WARN: Forcing SPROM type to SUP\n");
		type = SPROM_UTIL_TYPE_SUP;
		if ( ( rc = sprom_get_slot_sup ( platform, &slot) ) < 0 ) {
			return  ( rc );
		}
 	} else {
		len = strlen ( *argv );
		if ( strncmp ( *argv, "supervisor", len ) == 0 ) {
			type = SPROM_UTIL_TYPE_SUP;
			if ( ( rc = sprom_get_slot_sup ( platform, &slot) ) < 0 ) {
				return  ( rc );
			}
		} else if ( ( strncmp ( *argv, "backplane", len ) == 0 ) ||
				( strcmp ( *argv, "bp" ) == 0 ) ) {
			type = SPROM_UTIL_TYPE_BP;

			argc--;
			argv++;
	
			if (argc) {
				slot = strtoul ( *argv, &endp, 0 );
	
				if ( *endp ) {
					return  ( -SPROM_ERR_INVALID );
				}
			} else {
				slot = 0;
			}
		} else if ( strncmp ( *argv, "fan", len ) == 0 ) {
			type = SPROM_UTIL_TYPE_FAN;
	
			argc--;
			argv++;
	
			if (argc) {
				slot = strtoul ( *argv, &endp, 0 );
	
				if ( *endp ) {
					return  ( -SPROM_ERR_INVALID );
				}
			}
		} else if ((strncmp(*argv, "powersupply", len ) == 0 ) ||
			( strcmp ( *argv, "ps" ) == 0 ) ) {
			type = SPROM_UTIL_TYPE_PS;
                        argc--;
                        argv++;
                        if (argc) {
                                 slot = strtoul ( *argv, &endp, 0 );
                                 if ( *endp ) {
                                         return  ( -SPROM_ERR_INVALID );
                                 }
                                /* ui, 1 based slot; backend 0 based slot */
                                if ( slot == 0 ) {
                                        return ( -SPROM_ERR_INVALID );
                                } else {
                                        slot--;
                                }
                         } else {
                                fprintf ( stderr, "Err: Invalid number of"
                                  "parameters\n" );
                                return ( -SPROM_ERR_INVALID );
                        }
		} else if ( ( strncmp ( *argv, "uplinksup", len ) == 0 )) {
			type = SPROM_UTIL_TYPE_BROCKWAY_UPLINK_SUP;

			argc--;
			argv++;
	
			if (argc) {
				slot = strtoul ( *argv, &endp, 0 );
	
				if ( *endp ) {
					return  ( -SPROM_ERR_INVALID );
				}
			} else {
				slot = 0;
			}
		} else if ( ( strncmp ( *argv, "uplinkbackplane", len ) == 0 ) ||
				( strcmp ( *argv, "uplinkbp" ) == 0 ) ) {
			type = SPROM_UTIL_TYPE_BROCKWAY_UPLINK_BP;

			argc--;
			argv++;
	
			if (argc) {
				slot = strtoul ( *argv, &endp, 0 );
	
				if ( *endp ) {
					return  ( -SPROM_ERR_INVALID );
				}
			} else {
				slot = 0;
			}
		} else {
			return  ( -SPROM_ERR_INVALID );
		}
	}


	sprom = sprom_init ( platform, type, slot);
	if ( sprom == NULL ) {
		fprintf ( stderr, "Err: No Device\n" );
			return  ( -SPROM_ERR_NODEV );
	}

	if ( sprom->type != type ) {
		fprintf ( stderr, "Err: Sprom type does not match\n" );
		return  ( 1 );
	}

	switch ( cmd ) {
		case	SPROM_CMD_SET:
			rc = sprom_set ( sprom );
			break;

		case	SPROM_CMD_SHOW:
			rwbuf = malloc ( sprom_get_size ( sprom ) );
			if ( rwbuf == NULL ) {
				return  ( -ENOMEM );
			}

			if ( ( rc = sprom_read ( sprom, rwbuf ) ) < 0 ) {
				fprintf ( stderr, "Error: sprom read failed, %d\n", rc );
				free ( rwbuf );
				return  ( rc );
			}

			if ( ( rc = sprom_show ( sprom, rwbuf ) ) < 0 ) {
				fprintf ( stderr, "Error: sprom_show returned, %d\n", rc);
				free ( rwbuf );
				return  ( rc );
			}
			free ( rwbuf );
			break;

		case	SPROM_CMD_SET_SERIAL:
			rc = sprom_set_common_blk_field ( sprom, SPROM_CMD_SET_SERIAL );
			break;

		case	SPROM_CMD_SET_LICENSE:
			rc = sprom_set_lic ( sprom );
			break;

		case	SPROM_CMD_SET_PART_NUM:
			rc = sprom_set_common_blk_field ( sprom, SPROM_CMD_SET_PART_NUM );
			break;

		case	SPROM_CMD_SET_PART_REV:
			rc = sprom_set_common_blk_field ( sprom, SPROM_CMD_SET_PART_REV );
			break;

		case	SPROM_CMD_SET_MAC:
			rc = sprom_set_mac ( sprom );
			break;

		case	SPROM_CMD_DUMP:
			rc = sprom_dump ( sprom, offset, length );
			break;

		case	SPROM_CMD_INVALIDATE:
			rc = sprom_invalidate ( sprom );
			break;

		case	SPROM_CMD_ZERO:
			rc = sprom_zero ( sprom );
			break;

		default:
			return(-SPROM_ERR_INVALID );
	}

        return 0;
}
