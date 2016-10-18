#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include "sys/types.h"
#include "sprom.h"
#include "diag_sprom.h"
#include "diag_sprom_defs.h"

#define INDENT_PER_LEVEL (2)
#define PINDENT		sprom_printf

static char epld_char[] = {'A','B','C','D','E','F','G','H','I','J',
                           'K','L','M','N','O','P','Q','R','S','T',
                           'U','V','W','X','Y','Z','a','b','c','d'};
static void
_show_blk_header( const sprom_block_header_t* blk_hdr, uint32_t indent)
{
    int len = 256;

    // interpret a block length of zero as 256
    if (blk_hdr->block_length != 0)
        len = blk_hdr->block_length;

    sprom_printf ( "Block Header:\n" );
    sprom_printf ( "  Block Signature : 0x%04x\n", blk_hdr->block_signature );
    sprom_printf ( "  Block Version   : %u\n", blk_hdr->block_version );
    sprom_printf ( "  Block Length    : %u (0x%04x)\n", len, len );
    sprom_printf ( "  Block Checksum  : 0x%04x\n", blk_hdr->block_checksum );

    return;
}

static char*
_convert_snmpOID (const uint16_t* snmpOID, int max_len)
{
   static char dotted_format[100];
   char *p = dotted_format;
   int i;

   memset(dotted_format,0,100);

   if (max_len > SNMP_OID_SIZE) {
       printf("max_len is too big: %d\n", max_len);
       return dotted_format;
   }

   sprintf(p, "%d", snmpOID[0]);
   p += strlen(p);
   i = 1;

   while (i < max_len) {
       sprintf(p, ".%d", snmpOID[i]);
       p += strlen(p);
       ++i;
   }
   return dotted_format;
}

static char*
_convert_RMA (const uint8_t* rma, int size)
{
   static char dashed_format[100];
   char *p = dashed_format;
   int i;

   memset(dashed_format,0,100);

   if (size > RMA_FAILURE_CODE_SIZE) {
       printf("size is too big: %d\n", size);
       return dashed_format;
   }

   sprintf(p, "%d", rma[0]);
   p += strlen(p);
   i = 1;
   while (i < size) {
       sprintf(p, "-%d", rma[i]);
       p += strlen(p);
       ++i;
   }
   return dashed_format;
}

static void
_show_common_blk( const sprom_common_block_t* blk, const char* name, uint32_t indent)
{
    sprom_printf ( "%s Common Block:\n", name );
    _show_blk_header( &blk->block_header, indent + INDENT_PER_LEVEL );

    // show rest of common block
    sprom_printf ( "  FRU Major Type  : 0x%04x\n", blk->fru_major_t );
    sprom_printf ( "  FRU Minor Type  : 0x%04x\n", blk->fru_minor_t );
    sprom_printf ( "  OEM String      : %.*s\n", 
      sizeof(blk->OEM_string)-1, blk->OEM_string );
    sprom_printf ( "  Product Number  : %.*s\n", sizeof(blk->product_number)-1,
      blk->product_number );
    sprom_printf ( "  Serial Number   : %.*s\n", sizeof(blk->serial_number)-1, 
      blk->serial_number );
    sprom_printf ( "  Part Number     : %.*s\n", sizeof(blk->part_number)-1,
      blk->part_number );
    sprom_printf ( "  Part Revision   : %.*s\n", sizeof(blk->part_revision)-1, 
      blk->part_revision );
    sprom_printf ( "  Mfg Deviation   : %.*s\n", sizeof(blk->mfg_deviation)-1,
      blk->mfg_deviation );
    sprom_printf ( "  H/W Version     : %u.%u\n", blk->hw_rev_major, 
      blk->hw_rev_minor );
    sprom_printf ( "  Mfg Bits        : %d\n", blk->mfg_bits );
    sprom_printf ( "  Engineer Use    : %d\n", blk->eng_bits );
    sprom_printf ( "  snmpOID         : %s\n", _convert_snmpOID( blk->snmpOID,
      SNMP_OID_SIZE ) );
    sprom_printf ( "  Power Consump   : %d\n", blk->power_consumption );
    sprom_printf ( "  RMA Code        : %s\n", 
      _convert_RMA(blk->RMA_failure_code,
      RMA_FAILURE_CODE_SIZE ) );
    sprom_printf ( "  CLEI Code       : %.*s\n", sizeof( blk->CLEI_code )-1, 
      blk->CLEI_code );
    sprom_printf ( "  VID             : %.*s\n", sizeof( blk->VID)-1, 
      blk->VID );

    return;
}

void
_show_common_blk_n2k( const sprom_common_block_n2k_t* blk, const char* name, uint32_t indent)
{
    sprom_printf ( "%s Common Block: (size=0x%X BlkCnt=%d)\n", 
			name, blk->sprom_size, blk->block_count);
    _show_blk_header( (sprom_block_header_t*)blk, indent + INDENT_PER_LEVEL );

    // show rest of common block
    sprom_printf ( "  FRU Major Type  : 0x%04x\n", blk->fru_major_t );
    sprom_printf ( "  FRU Minor Type  : 0x%04x\n", blk->fru_minor_t );
    sprom_printf ( "  OEM String      : %.*s\n", 
      sizeof(blk->OEM_string)-1, blk->OEM_string );
    sprom_printf ( "  Product Number  : %.*s\n", sizeof(blk->product_number)-1,
      blk->product_number );
    sprom_printf ( "  Serial Number   : %.*s\n", sizeof(blk->serial_number)-1, 
      blk->serial_number );
    sprom_printf ( "  Part Number     : %.*s\n", sizeof(blk->part_number)-1,
      blk->part_number );
    sprom_printf ( "  Part Revision   : %.*s\n", sizeof(blk->part_revision)-1, 
      blk->part_revision );
    sprom_printf ( "  Mfg Deviation   : %.*s\n", sizeof(blk->mfg_deviation)-1,
      blk->mfg_deviation );
    sprom_printf ( "  H/W Version     : %u.%u\n", blk->hw_rev_major, 
      blk->hw_rev_minor );
    sprom_printf ( "  Mfg Bits        : %d\n", blk->mfg_bits );
    sprom_printf ( "  Engineer Use    : %d\n", blk->eng_bits );
    sprom_printf ( "  snmpOID         : %s\n", _convert_snmpOID( blk->snmpOID,
      SNMP_OID_SIZE ) );
    sprom_printf ( "  Power Consump   : %d\n", blk->power_consumption );
    sprom_printf ( "  RMA Code        : %s\n", 
      _convert_RMA(blk->RMA_failure_code,
      RMA_FAILURE_CODE_SIZE ) );
    sprom_printf ( "  CLEI Code       : %.*s\n", sizeof( blk->CLEI_code )-1, 
      blk->CLEI_code );
    sprom_printf ( "  VID             : %.*s\n", sizeof( blk->VID)-1, 
      blk->VID );

    return;
}

static char*
_uint64_ttoHexStr (uint64_t lonlongint)
{
   static char buf[64];

   sprintf( buf, "%llx", lonlongint);

   return buf;
}

static char*
_ether_sprintf (const char* mac_addr)
{
   static char buf[64];
   static unsigned char m[6];

   memcpy( m, mac_addr, 6);
   sprintf(buf, "%02x-%02x-%02x-%02x-%02x-%02x",
           m[0], m[1], m[2], m[3], m[4], m[5]);

   return buf;
}

static char*
_convert_PortNum ( const sprom_port_data_t* port_num, int max_len)
{
   static char port_num_format[100];
   char *p = port_num_format;
   int i;

   memset(port_num_format,0,100);

   if (max_len > MAX_PORT_TYPES) {
       printf("max_len is too big: %d\n", max_len);
       return port_num_format;
   }

   if (port_num[0].num_ports) {
      sprintf(p, "%d-%d", port_num[0].type_of_port, port_num[0].num_ports);
      p += strlen(p);
      i = 1;

      while (i < max_len && port_num[i].num_ports) {
         sprintf(p, ";%d-%d", port_num[i].type_of_port, port_num[i].num_ports);
         p += strlen(p);
         ++i;
      }
   }

   return port_num_format;
}

static void
_show_lc_blk( const sprom_line_card_block_t* blk, const char* name, uint32_t indent)
{
    int i;
    uint8_t epld_num;

    sprom_printf ( "%s Main Block:\n", name );
    _show_blk_header( &blk->block_header, indent + INDENT_PER_LEVEL);

    // show rest of lc block
    sprom_printf ("  Feature Bits    : 0x%s\n", 
      _uint64_ttoHexStr(blk->feature_bits));
    sprom_printf ( "  HW Changes Bits : 0x%s\n", 
      _uint64_ttoHexStr(blk->hw_changes_bits));
    sprom_printf ( "  Card Index      : %d\n", 
      blk->card_index);
    sprom_printf ( "  MAC Addresses   : %s\n", _ether_sprintf(blk->mac_base) );
    sprom_printf ( "  Number of MACs  : %d\n", blk->mac_length );
    sprom_printf ( "  Number of EOBC links : %d\n", blk->eobc_connections );

    epld_num = ( blk->epld_num > EPLD_VERSIONS_SIZE ) ?
        EPLD_VERSIONS_SIZE : blk->epld_num;

    sprom_printf ( "  Number of EPLDs : %d\n", epld_num );

    if (epld_num > 0) {
        if ( epld_num > 15) {
            // epld version data is 1-byte per epld
            for (i=0; i < epld_num; ++i) {
                sprom_printf ("    EPLD %c          : 0x%02x\n",
                        epld_char[i], blk->epld_versions[i]);
            }
        }
        else {
            // epld version data is 2-bytes per epld
            for (i=0; i < epld_num; ++i) {
                sprom_printf ("    EPLD %c          : 0x%04x\n",
                        epld_char[i], *((uint16_t*)(&blk->epld_versions[i*2])));
            }
        }
    }

    sprom_printf ("  Port Type-Num   : %s\n",
           _convert_PortNum(blk->port_data, MAX_PORT_TYPES));
    sprom_printf ("  SRAM size       : %d\n", blk->sram_size);
    for (i=0; i<MAX_CARD_SENSORS; ++i) {
        sprom_printf ("  Sensor #%d       : %d,%d\n",
               i+1,
               blk->sensor_data[i].major_threshold,
               blk->sensor_data[i].minor_threshold);
    }
    sprom_printf ( "  Max Connector Power : %d\n", blk->max_connector_power );
    sprom_printf ( "  Cooling Requirement : %d\n", blk->cooling_requirement );
    sprom_printf ( "  Ambient Temperature : %d\n", blk->ambient_temperature );

    return;
}

static void
_show_sensor_blk( const sprom_temp_sensor_block_t* blk, const char* name, uint32_t indent)
{
    int i;
    uint8_t num_sensors;

    sprom_printf ( "%s Temperature Sensor Block:\n", name);
    _show_blk_header( &blk->block_header, indent + INDENT_PER_LEVEL);

    num_sensors = (blk->num_sensors > MAX_TEMP_SENSORS) ?
        MAX_TEMP_SENSORS : blk->num_sensors;

    sprom_printf ( "  Number of sensors : %u\n", num_sensors);
    for (i = 0; i < num_sensors; ++i) {
        sprom_printf ( "  Sensor #%d       : %d,%d\n",
                i+1,
                (int8_t)(blk->sensor_data[i].major_threshold),
                (int8_t)(blk->sensor_data[i].minor_threshold));
    }
}

static void
_show_sup_blk( const sprom_sup_block_t* blk, const char* name, uint32_t indent)
{
    // At the current moment the SUP and LC blocks have identical
    // formats even though two seperate structs are defined.  For now
    // use the line card decoding routines previously defined.
    _show_lc_blk( (sprom_line_card_block_t*)blk, name, indent);
}

static void
_show_bp_blk( const sprom_backplane_block_t* blk, const char* name, uint32_t indent)
{
    sprom_printf ( "%s Main Block:\n", name );
    _show_blk_header ( &blk->block_header, indent + INDENT_PER_LEVEL);

    // show rest of bp block
    sprom_printf ( "  Feature Bits    : 0x%s\n", 
      _uint64_ttoHexStr(blk->feature_bits) );
    sprom_printf ( "  HW Changes Bits : 0x%s\n", 
      _uint64_ttoHexStr(blk->hw_changes_bits) );
    sprom_printf ( "  Stackmib OID    : %d\n", 
      blk->stackmib_oid);
    sprom_printf ( "  MAC Addresses   : %s\n", 
      _ether_sprintf(blk->mac_base) );
    sprom_printf ( "  Number of MACs  : %d\n", 
      blk->mac_length );
    sprom_printf ( "  OEM Enterprise  : %u\n", 
      blk->OEM_Enterprise );
    sprom_printf ( "  OEM MIB Offset  : %u\n", 
      blk->OEM_MIB_Offset );
    sprom_printf ( "  MAX Connector Power: %u\n", 
      blk->max_connector_power );
    //sprom_printf ( "  Cooling Coefficient: %u\n", blk->cooling_coefficient);

    return;
}

static void
_show_bp_wwn_blk( const sprom_wwn_block_t* blk, const char* name, uint32_t indent)
{
    int i;

    sprom_printf ( "%s WWN Block:\n", name );
    _show_blk_header( &blk->block_header, indent + INDENT_PER_LEVEL);

    // show rest of bp WWN block
    sprom_printf ( "  WWN Usage Bits:\n" );
    i=-1;
    sprom_printf ( "   " );
    while (++i < WWN_USAGE_SIZE) {
        sprom_printf ( " %02x", blk->wwn_usage_bits[i] );
        if ((i & 7) == 7) {
            sprom_printf ( "\n" );
            sprom_printf ( "   " );
        }
    }

    if (((i-1) & 7) != 7) {
        sprom_printf ( "\n" );
    }
}

static void
_show_bp_lic_blk( const sprom_lic_block_t* blk, const char* name, uint32_t indent)
{
    int i;

    sprom_printf ( "%s License Block:\n", name );
    _show_blk_header( &blk->block_header, indent + INDENT_PER_LEVEL);

    // show rest of bp LIC block
    sprom_printf ( "  License Usage Bits:\n" );
    i=-1;
    sprom_printf ( "   " );
    while (++i < LIC_USAGE_SIZE) {
        sprom_printf ( " %02x", blk->lic_usage_bits[i] );
        if ((i & 7) == 7) {
            sprom_printf ( "\n" );
            sprom_printf ( "   " );
        }
    }

    sprom_printf ( "\n" );

}

static void
sprom_show_sup ( diag_sprom_t *sprom, uint8_t *buf )
{
    sup_sprom_t * sup_sprom = ( sup_sprom_t * ) buf;

    _show_common_blk( &sup_sprom->common_block,
                      sprom_get_name ( sprom ),
                      INDENT_PER_LEVEL);
    
    // show sup block
    _show_sup_blk( &sup_sprom->sup_block,
                   sprom_get_name( sprom ),
                   INDENT_PER_LEVEL);

    // show extra sensor block
    _show_sensor_blk( &sup_sprom->sensor_block,
                      sprom_get_name( sprom ),
                      INDENT_PER_LEVEL);   

    return;
}

static void
sprom_show_lc ( diag_sprom_t *sprom, uint8_t *buf )
{
    line_card_sprom_t* lc_sprom = ( line_card_sprom_t* ) buf;

    // show common header
    _show_common_blk( &lc_sprom->common_block,
                      sprom_get_name( sprom ),
                      INDENT_PER_LEVEL);

    // show line card block
    _show_lc_blk( &lc_sprom->lc_block,
                  sprom_get_name( sprom ),
                  INDENT_PER_LEVEL);

    // show extra sensor block
#if 0
    _show_sensor_blk( &lc_sprom->sensor_block,
                      sprom_get_name( sprom ),
                      INDENT_PER_LEVEL);
#endif

    return;
}

static void
_show_sn2_blk( const sprom_sn2_block_t* blk, const char* name, uint32_t indent)
{

    sprom_printf("%s 2nd Serial Number Block:\n", name);
    _show_blk_header( &blk->block_header, indent + INDENT_PER_LEVEL);

    // show rest of SN2 block
    sprom_printf("  Serial Number   : %.*s\n", 
		sizeof(blk->serial_number)-1, blk->serial_number);
    sprom_printf("\n");
}

static void
sprom_show_bp ( diag_sprom_t *sprom, uint8_t *buf )
{
     backplane_sprom_t* bp_sprom = (backplane_sprom_t*)buf;

    // show common header
    _show_common_blk( &bp_sprom->common_block,
                      sprom_get_name( sprom ),
                      INDENT_PER_LEVEL);

    // show backplane block
    _show_bp_blk( &bp_sprom->backplane_block,
                  sprom_get_name( sprom ),
                  INDENT_PER_LEVEL);

    // show backplane WWN block
    _show_bp_wwn_blk( &bp_sprom->wwn_block,
                      sprom_get_name( sprom ),
                      INDENT_PER_LEVEL);

    // show backplane License block
    _show_bp_lic_blk( &bp_sprom->lic_block,
                      sprom_get_name( sprom ),
                      INDENT_PER_LEVEL);
    // show backplane 2nd serial number block
    _show_sn2_blk( &bp_sprom->sn2_block,
                   sprom_get_name(sprom),
                   INDENT_PER_LEVEL);

    return;
}

#ifndef DIAG_FEX
static void
_show_ps_blk( const sprom_power_supply_block_t* blk, const char* name, uint32_t indent)
{
    sprom_printf ( "%s Main Block:\n", name );
    _show_blk_header((sprom_block_header_t*)blk, indent + INDENT_PER_LEVEL);

    // show rest of ps block
    sprom_printf ( "  Feature Bits    : 0x%s\n", 
      _uint64_ttoHexStr(blk->feature_bits));
    sprom_printf ( "  Current 110v    : %u\n", blk->current_110v);
    sprom_printf ( "  Current 220v    : %u\n", blk->current_220v);
    sprom_printf ( "  Stackmib OID    : %u\n", blk->stackmib_oid);
    PINDENT("  Current Mode 1  : %u\n", blk->current_mode1);
    PINDENT("  Current Mode 2  : %u\n", blk->current_mode2);
    PINDENT("  Current Mode 3  : %u\n", blk->current_mode3);
    PINDENT("  Current Mode 4  : %u\n", blk->current_mode4);
    PINDENT("  Max Float Current Mode 1  : %u\n", blk->max_float_current_mode1);
    PINDENT("  Max Float Current Mode 2  : %u\n", blk->max_float_current_mode2);
    PINDENT("  Max Float Current Mode 3  : %u\n", blk->max_float_current_mode3);
    PINDENT("  Max Float Current Mode 4  : %u\n", blk->max_float_current_mode4);
}

static void
sprom_show_ps ( diag_sprom_t *sprom, uint8_t *buf )
{
    power_supply_sprom_t* ps_sprom = (power_supply_sprom_t*)buf;

    // show common header
    _show_common_blk_n2k( &ps_sprom->common_block,
                      sprom_get_name( sprom ),
                      INDENT_PER_LEVEL);

    _show_ps_blk( &ps_sprom->power_supply_block,
                  sprom_get_name( sprom ),
                  INDENT_PER_LEVEL);

    return;
}
#else

static void
sprom_show_ps_rev3 ( diag_sprom_t *sprom, uint8_t *buf )
{
    int len = 128;
    power_supply_sprom_fex_t* blk = (power_supply_sprom_fex_t*)buf;

    // show common header
    sprom_printf ( "%s Fex Common Block: (size=%d)\n", 
		sprom_get_name(sprom) , sizeof(power_supply_sprom_fex_t));

    // interpret a block length of zero as 128
    if (blk->block_length != 0)
        len = blk->block_length;

    sprom_printf ( "Block Header:\n" );
    sprom_printf ( "  Block Signature : 0x%04x\n", blk->block_signature );
    sprom_printf ( "  Block Version   : %u\n", blk->block_version );
    sprom_printf ( "  Block Length    : %u (0x%04x)\n", len, len );
    sprom_printf ( "  Block Checksum  : 0x%04x\n", blk->block_checksum );


    // show rest of common block
    sprom_printf ( "  FRU Major Type  : 0x%04x\n", blk->fru_major_t );
    sprom_printf ( "  FRU Minor Type  : 0x%04x\n", blk->fru_minor_t );
    sprom_printf ( "  OEM String      : %.*s\n", 
      sizeof(blk->OEM_string)-1, blk->OEM_string );
    sprom_printf ( "  Product Number  : %.*s\n", sizeof(blk->product_number)-1,
      blk->product_number );
    sprom_printf ( "  Serial Number   : %.*s\n", sizeof(blk->serial_number)-1, 
      blk->serial_number );
    sprom_printf ( "  Part Number     : %.*s\n", sizeof(blk->part_number),
      blk->part_number );
    sprom_printf ( "  Part Revision   : %.*s\n", sizeof(blk->part_revision)-1, 
      blk->part_revision );
    sprom_printf ( "  CLEI            : %.*s\n", sizeof(blk->clei)-1, 
          blk->clei );
    sprom_printf ( "  Version ID      : %.*s\n", sizeof(blk->version_id), 
          blk->version_id );
    sprom_printf ( "  snmpOID         : %s\n", _convert_snmpOID( blk->snmpOID,
             SNMP_OID_SIZE ) );
    sprom_printf ( "  H/W Version     : %u.%u\n", blk->hw_rev_major, 
      blk->hw_rev_minor );
    sprom_printf ( "  Power Consump   : %d\n", blk->power_consumption );
    sprom_printf ( "  RMA Code        : %s\n", 
      _convert_RMA(blk->RMA_failure_code,
      RMA_FAILURE_CODE_SIZE ) );

    sprom_printf ( "  Fault Byte      : %d\n", blk->fault_byte );
    sprom_printf ( "  Fan Speed Byte  : %d\n", blk->fan_speed_byte );
    sprom_printf ( "  Temp Byte       : %d\n", blk->temp_byte );

    return;
}


static void
sprom_show_ps_rev2 ( diag_sprom_t *sprom, uint8_t *buf )
{
    int len = 128;
    power_supply_sprom_fex_rev2_t* blk = (power_supply_sprom_fex_rev2_t*)buf;

    // show common header
    sprom_printf ( "%s Fex Common Block: (size=%d)\n", 
		sprom_get_name(sprom) , sizeof(power_supply_sprom_fex_t));

    // interpret a block length of zero as 128
    if (blk->block_length != 0)
        len = blk->block_length;

    sprom_printf ( "Block Header:\n" );
    sprom_printf ( "  Block Signature : 0x%04x\n", blk->block_signature );
    sprom_printf ( "  Block Version   : %u\n", blk->block_version );
    sprom_printf ( "  Block Length    : %u (0x%04x)\n", len, len );
    sprom_printf ( "  Block Checksum  : 0x%04x\n", blk->block_checksum );


    // show rest of common block
    sprom_printf ( "  FRU Major Type  : 0x%04x\n", blk->fru_major_t );
    sprom_printf ( "  FRU Minor Type  : 0x%04x\n", blk->fru_minor_t );
    sprom_printf ( "  OEM String      : %.*s\n", 
      sizeof(blk->OEM_string)-1, blk->OEM_string );
    sprom_printf ( "  Product Number  : %.*s\n", sizeof(blk->product_number)-1,
      blk->product_number );
    sprom_printf ( "  Serial Number   : %.*s\n", sizeof(blk->serial_number)-1, 
      blk->serial_number );
    sprom_printf ( "  Part Number     : %.*s\n", sizeof(blk->part_number)-1,
      blk->part_number );
    sprom_printf ( "  Part Revision   : %.*s\n", sizeof(blk->part_revision)-1, 
      blk->part_revision );
    sprom_printf ( "  Mfg Deviation   : %.*s\n", sizeof(blk->mfg_deviation)-1,
          blk->mfg_deviation );
    sprom_printf ( "  H/W Version     : %u.%u\n", blk->hw_rev_major, 
      blk->hw_rev_minor );
    sprom_printf ( "  Power Consump   : %d\n", blk->power_consumption );
    sprom_printf ( "  RMA Code        : %s\n", 
      _convert_RMA(blk->RMA_failure_code,
      RMA_FAILURE_CODE_SIZE ) );

    sprom_printf ( "  Fault Byte      : %d\n", blk->fault_byte );
    sprom_printf ( "  Fan Speed Byte  : %d\n", blk->fan_speed_byte );
    sprom_printf ( "  Temp Byte       : %d\n", blk->temp_byte );


    return;
}

static void
sprom_show_ps ( diag_sprom_t *sprom, uint8_t *buf )
{
    power_supply_sprom_fex_t* blk = (power_supply_sprom_fex_t*)buf;

    if (blk->block_version == SPROM_POWER_SUPPLY_BLOCK_FEX_VER) {
	sprom_show_ps_rev3 (sprom, buf);
    } else {
	sprom_show_ps_rev2 (sprom, buf);
    }
}
#endif
static void
_show_fan_blk( const sprom_fan_block_t* blk, const char* name, uint32_t indent)
{
    sprom_printf("%s Main Block:\n", name);
    _show_blk_header( &blk->block_header, indent + INDENT_PER_LEVEL);

    // show rest of fan block
    sprom_printf("  Feature Bits    : 0x%s\n", _uint64_ttoHexStr(blk->feature_bits));
    sprom_printf("  HW Changes Bits : 0x%s\n", _uint64_ttoHexStr(blk->hw_changes_bits));
    sprom_printf("  Stackmib OID    : %d\n", blk->stackmib_oid);
    sprom_printf("  Cooling Capacity    : %u\n", blk->cooling_capacity);
    sprom_printf("  Ambient Temperature : %d\n", blk->ambient_temperature);
    sprom_printf("  Power Consumption 2 : %d\n", blk->power_consumption2);
    sprom_printf("  Cooling Capacity 2  : %u\n", blk->cooling_capacity2);
}

static void
sprom_show_fan( diag_sprom_t *sprom, uint8_t *buf)
{
    fan_sprom_t* fan_sprom = (fan_sprom_t*)buf;

    // show common header
    _show_common_blk( &fan_sprom->common_block,
                      sprom_get_name(sprom),
                      INDENT_PER_LEVEL);

    // show fan block
    _show_fan_blk( &fan_sprom->fan_block,
                   sprom_get_name(sprom),
                   INDENT_PER_LEVEL);

    // show fan 2nd serial number block
    _show_sn2_blk( &fan_sprom->sn2_block,
                   sprom_get_name(sprom),
                   INDENT_PER_LEVEL);
}

int
sprom_show ( diag_sprom_t *sprom, uint8_t * const buf )
{
    int rc = 0;

    assert ( sprom );
    assert ( buf );

    sprom_printf ( "Showing SPROM %s\n", sprom_get_name ( sprom ) );

    if ( ( rc = sprom_verify ( sprom, buf ) ) < 0 ) {
        if ( rc == -SPROM_ERR_INVALID ) {
            sprom_printf ( "\n" );
            sprom_printf ( "** Warning Warning Warning **\n" );
            sprom_printf ( "** The sprom contents are invalid.\n" );
            sprom_printf ( "** Please re-program the sprom.\n" );
            sprom_printf ( "** Warning Warning Warning **\n" );
            sprom_printf ( "\n" );
            sprom_printf ( "Showing the contains anyway.\n\n" );
        } else {
            sprom_printf ( "%s(): diag_sprom_is_valid() failed.\n", 
              __FUNCTION__ );
            goto sprom_show_exit;
        }
    }

    switch ( sprom->type ) {
    case SPROM_UTIL_TYPE_SUP:
        sprom_show_sup ( sprom, buf );
        break;

    case SPROM_UTIL_TYPE_LC:
        sprom_show_lc ( sprom, buf );
        break;

    case SPROM_UTIL_TYPE_BP:
        sprom_show_bp ( sprom, buf );
        break;

    case SPROM_UTIL_TYPE_PS:
        sprom_show_ps ( sprom, buf );
        break;

    case SPROM_UTIL_TYPE_FAN:
        sprom_show_fan ( sprom, buf );
        break;
    case SPROM_UTIL_TYPE_BROCKWAY_UPLINK_SUP:
        sprom_show_sup ( sprom, buf );
        break;
    case SPROM_UTIL_TYPE_BROCKWAY_UPLINK_BP:
        sprom_show_bp ( sprom, buf );
        break;
    default:
        rc = -SPROM_ERR_INVALID;
        goto sprom_show_exit;
    }

    if ( rc == -SPROM_ERR_INVALID) {
        sprom_printf ( "\n" );
        sprom_printf ( "** Warning Warning Warning **\n" );
        sprom_printf ( "** The above sprom contents are invalid.\n" );
        sprom_printf ( "** Please re-program the sprom.\n" );
        sprom_printf ( "** Warning Warning Warning **\n" );
        sprom_printf ( "\n" );
    }


sprom_show_exit:
    return ( rc );
}

