/********************************************************************
 *
 *      File:   sprom_set.c
 *      Name:   Curt Brune
 *
 *      Description:
 *       sprom set routines - writes the sprom
 *
 * Copyright (c) 1985-2002, 2003, 2004 by Cisco Systems, Inc.
 * All rights reserved.
 *
 ********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include "sprom.h"
#include "diag_sprom.h"
#include "diag_sprom_defs.h"

#define __USE_ISOC99
#include <ctype.h>

#define FALSE 0
#define TRUE  1

#define isblank(c)    __isctype((c), _ISblank)

static char epld_char[] = {'A','B','C','D','E','F','G','H','I','J',
                           'K','L','M','N','O','P','Q','R','S','T',
                           'U','V','W','X','Y','Z','a','b','c','d'}; /* max 30 eplds */

#define INDENT_PER_LEVEL (2)
#define PINDENT(format , args...)  \
    do {                                        \
        if ( _in_strm == stdin) {               \
            _print_indent(indent);              \
            sprom_printf ( format , ## args );  \
        }                                       \
    } while (0)

#define PMSG(format , args...)                  \
    do {                                        \
        if ( _in_strm == stdin) {               \
            sprom_printf ( format , ## args );  \
        }                                       \
    } while (0)

static void _print_indent( uint32_t indent)
{
    uint32_t i;
    char   buf[64];

    buf[0] = '\0';
    for ( i = 0; i < indent; i++)
        strcat( buf, " ");

    sprom_printf ( "%s", buf );
}

int
sprom_confirm_set (const diag_sprom_t* sprom )
{   
    char         buffer;
    int          rtn_val;

    sprom_printf ("\n");
    sprom_printf ("Program %s SPROM", sprom_get_name( sprom ) );
    sprom_printf ( " with the above information (y/n) [n]? "); 
    buffer = getchar();
    while( getchar() != '\n' )
        ;
    
    if (buffer == 'y' || buffer == 'Y') {
        rtn_val = 1;
    }
    else {
        sprom_printf("Aborting SPROM modification ...\n");
        rtn_val = 0;
    }

    return rtn_val;
}


static void
_fill_blk_header( sprom_block_header_t* const blk_hdr, const uint32_t sig,
                  const uint32_t ver, const uint32_t len)
{
    blk_hdr->block_signature = sig;
    blk_hdr->block_version   = ver;
    blk_hdr->block_length    = len;
    blk_hdr->block_checksum  = diag_sprom_compute_xsum(blk_hdr, len);
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

static int
_parse_snmpOID (char *inbuf, u_int16_t *outbuf, int len)
{
  char *token;
  int count=0;

  token = strtok(inbuf, ".");
  while (token) {
    outbuf[count++] = atoi(token);
    if (count > len)
      return -1;
    token = strtok(NULL, ".");
  }

  if ( count != len)
      return -1;

  //  for ( ; count < len; ++count)  /* fill 0 at end */
  //    outbuf[count] = 0;

  return 0;
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

static int
_parse_RMA (char *inbuf, char *outbuf, int len)
{
  char *token;
  int count=0;

  token = strtok(inbuf, "-");
  while (token) {
    outbuf[count++] = atoi(token);
    if (count > len)
      return -1;
    token = strtok(NULL, "-");
  }

  if ( count != len)
      return -1;

  //  for ( ; count < len; ++count)  /* fill 0 at end */
  //    outbuf[count] = 0;

  return 0;
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

static inline int
_isxdigit (char c)
{
    return (((c >= '0') && (c <= '9')) ||
            ((c >= 'a') && (c <= 'f')) ||
            ((c >= 'A') && (c <= 'F')));
}

static int
_ishexstr (char *s)
{
  char *p;

  if (strncmp(s, "0x", 2) == 0
      || strncmp(s, "0X", 2) == 0)
    s+=2;
  for (p=s ; *p; p++) {
    if (!_isxdigit(*p))
      return FALSE;
  }
  return TRUE;
}

static inline int
_isintdigit (char c)
{
    return (c >= '0') && (c <= '9');
}

static int
_isintstr (char *s)
{
  char *p;

  if (strncmp(s, "+", 1) == 0
      || strncmp(s, "-", 1) == 0)
    s+=1;
  for (p=s ; *p; p++) {
    if (!_isintdigit(*p))
      return FALSE;
  }
  return TRUE;
}

static int
_parseMacAddrNoSwap (char *str, u_int8_t addr[], int errDisp)
{
  int i, slen;
  u_int32_t temp;
  u_int64_t temp64;
  char *ptr, *ptemp;
  u_int8_t non_canonical = FALSE, canonical = FALSE;

  slen = strlen(str);
  if (slen< 11 || slen>17)
    goto InvalidMac;

  for (i=0; i<slen; i++) {
    if (str[i]=='-')
       canonical = TRUE;
    else if (str[i]==':')
       non_canonical = TRUE;
    else if (!_isxdigit(str[i]))
       goto InvalidMac;
  }

  if (canonical == TRUE && non_canonical == TRUE)
     goto InvalidMac;

  if (canonical == FALSE && non_canonical == FALSE) {
    	temp64 = strtoull(str, (char**)NULL, 16); 
	ptemp = (char*)&temp64;
	ptemp+=2;
	memcpy(addr, ptemp, 6);
  } else {
    for (i = 0; i< 6; i++ ) {
      ptr = strtok(str,":-");
      str = 0;
      if (ptr && _ishexstr(ptr)) {
        temp = strtol(ptr,(char **)NULL,16);
        if (temp > 255)
          goto InvalidMac;
        addr[i] = temp;
      } else
        goto InvalidMac;
    }
  }

  return 0;

InvalidMac:
  if (errDisp)
    sprom_printf ("Invalid MAC address format.\n");
  return -1;
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

static int
_parse_PortNum (char *inbuf, sprom_port_data_t *outbuf, int len)
{
  char *token;
  int count=0;
  int type, num;

  token = strtok(inbuf, "-");
  while (token) {
    type = atoi(token);
    token = strtok(NULL, ";");
    if (token == NULL)
      return -1;
    num = atoi(token);
    outbuf[count].type_of_port = type;
    outbuf[count].num_ports = num;
    count++;
    if (count > len)
      return -1;
    token = strtok(NULL, "-");
  }
  for ( ; count < len; ++count)  /* fill 0 at end */ {
    outbuf[count].type_of_port = count+1;
    outbuf[count].num_ports = 0;
  }
  return 0;
}

static int
_parse_LicUsageBit (char *str, uint8_t* addr)
{
  int i;
  uint32_t temp;
  char*  tok;

  tok = strtok( str, " ");
  for (i = 0; i< LIC_USAGE_SIZE; i++ ) {
      if (tok && _ishexstr(tok)) {
          temp = strtol( tok, NULL, 16);
          addr[i] = (uint8_t)temp;
      }
      else {
          goto InvalidLicUsageBits;
      }
      tok = strtok( NULL, " ");
  }

  return 0;

InvalidLicUsageBits:
  sprom_printf ("The License Usage Bits are %d hex numbers 0-FF\n",
    LIC_USAGE_SIZE);
  return -1;
}

static char   _set_buf[256];
static FILE*  _in_strm;
static char   _in_fname[128];
//static char   _def_fname[128];
static uint32_t _in_line    = 0;

static int _readval( void)
{
    uint32_t  i;
    uint32_t  is_blank;
   
    do {
        if ( fgets( _set_buf, sizeof(_set_buf), _in_strm)) {
            _in_line++;
            // zap trailing newline
            _set_buf[strlen(_set_buf)-1] = '\0';
            if ( _in_strm != stdin) {
                // reading from a file - more processing
                // skip lines of all white space
                for ( i = 0, is_blank=1; i < strlen(_set_buf); i++) {
                    if ( _set_buf[i] && !isblank(_set_buf[i])) {
                        is_blank = 0;
                        break;
                    }
                }
                if ( is_blank) {
                    continue;
                }
                if ( i != 0) {
                    // trim leading white space
                    memmove( _set_buf, &_set_buf[i], strlen(&_set_buf[i])+1);
                }
                // skip lines that begin "#"
                if ( !strncmp( _set_buf, "#", 1)) {
                    continue;
                }
            }
            return 1;
        }
        else {
            // error
            if ( _in_strm != stdin) {
                sprom_err ( "Problems reading data from '%s'\n", _in_fname);
            }
            else {
                sprom_err ( "Problems reading data from stdin.\n");
            }
            if ( feof( _in_strm)) {
                sprom_err ( "Reason: End of File reached.\n");
            }
            else if ( ferror( _in_strm)) {
                sprom_err ( "Reason: %s.\n", strerror(errno));
            }
            exit(-1);
        }
    } while (1);

    return 0;

}

#define INPUT_FAILURE(fld_name, fmt , args... )              \
    if ( _in_strm != stdin) {                                \
        sprom_err ( "\nProblems parsing input file.\n");         \
        sprom_err ( "File name      : %s\n", _in_fname);         \
        sprom_err ( "Line number    : %u\n", _in_line);          \
        sprom_err ( "Input Field    : %s\n", fld_name);          \
        sprom_err ( "Offending Input: %s\n", _set_buf);          \
        sprom_err ( "Reason         : "fmt".\n" , ## args);      \
        sprom_err ( "Exiting ...\n\n");                          \
        exit( -SPROM_ERR_IO );                                            \
    }

#define FILL_INT(str,pfmt,sfmt,field)                                   \
    do {                                                                \
        PINDENT("  Enter %s ["#pfmt"]:  ", str, blk->field);            \
        if ( _readval()) {                                              \
            if ( !strlen(_set_buf)) break;                              \
            if ( _isintstr(_set_buf) ) {                                \
                sscanf( _set_buf, #sfmt, &(blk->field));                \
                break;                                                  \
            }                                                           \
            else {                                                      \
                INPUT_FAILURE(str, "Not an integer string");            \
            }                                                           \
        }                                                               \
    } while (1)

#define FILL_HEX(str,pfmt,sfmt,field)                                   \
    do {                                                                \
        PINDENT("  Enter %s ["#pfmt"]:  ", str, blk->field);            \
        if ( _readval()) {                                              \
            if ( !strlen(_set_buf)) break;                              \
            if ( _ishexstr(_set_buf) ) {                                \
                sscanf( _set_buf, #sfmt, &(blk->field));                \
                break;                                                  \
            }                                                           \
            else {                                                      \
                INPUT_FAILURE(str, "Not a hex string");                 \
            }                                                           \
        }                                                               \
    } while (1)

#define FILL_RANGE(str,pfmt,field,min,max)                              \
    do {                                                                \
        int32_t tmp;                                                      \
        PINDENT("  Enter %s ["#pfmt"]:  ", str, blk->field);            \
        if ( _readval()) {                                              \
            if ( !strlen(_set_buf)) break;                              \
            if ( _isintstr(_set_buf) ) {                                \
                sscanf( _set_buf, "%d", &tmp);                          \
                if ( (tmp >= min) && (tmp <= max)) {                    \
                    blk->field = tmp;                                   \
                    break;                                              \
                }                                                       \
                else {                                                  \
                    INPUT_FAILURE(str, "Value out of range.  Expecting min: %d, max: %d", min, max); \
                }                                                       \
            }                                                           \
            else {                                                      \
                INPUT_FAILURE(str, "Not an integer string");            \
            }                                                           \
        }                                                               \
    } while (1)

#define FILL16(str,pfmt,sfmt,field)                                     \
    do {                                                                \
        PINDENT("  Enter %s ["#pfmt"]:  ", str, *((uint16_t*)&(blk->field))); \
        if ( _readval()) {                                              \
            if ( !strlen(_set_buf)) break;                              \
            if ( _ishexstr(_set_buf) ) {                                \
                sscanf( _set_buf, #sfmt, (uint16_t*)&(blk->field));       \
                break;                                                  \
            }                                                           \
            else {                                                      \
                INPUT_FAILURE(str, "Not a hex string");                 \
            }                                                           \
        }                                                               \
    } while (1)

// For gathering strings
#define FILL_STR(str,field,len)                                 \
    do {                                                        \
	int my_len = (strlen(blk->field) > len)? len : 		\
			strlen(blk->field);			\
	memset(_set_buf, '\0', sizeof(_set_buf));		\
	strncpy(_set_buf, blk->field, my_len);			\
        PINDENT("  Enter %s [%s]:  ", str, _set_buf);           \
        if ( _readval()) {                                      \
            if ( strlen(_set_buf)) {                            \
                strncpy( blk->field, _set_buf, len);            \
            }                                                   \
            break;                                              \
        }                                                       \
    } while (1)

// For gathering strings
#define FILL_CHARS(str,field,len)                                 \
    do {                                                        \
        PINDENT("  Enter %s [%s]:  ", str, blk->field);         \
        if ( _readval()) {                                      \
            if ( strlen(_set_buf)) {                            \
                strncpy( blk->field, _set_buf, len);          \
            }                                                   \
            break;                                              \
        }                                                       \
    } while (1)

// For gathering SNMP OID strings
#define FILL_SNMP(str,field,len)                                        \
    do {                                                                \
        PINDENT("  Enter %s [%s]:  ", str, _convert_snmpOID(blk->field, len)); \
        if ( _readval()) {                                      \
            if ( !strlen(_set_buf)) break;                              \
            if ( _parse_snmpOID( _set_buf, blk->field, len) == 0) {     \
                break;                                                  \
            }                                                           \
            else {                                                      \
                INPUT_FAILURE(str, "Unable to parse SNMP OID");         \
            }                                                           \
        }                                                               \
    } while (1)

// For gathering RMA strings
#define FILL_RMA(str,field,len)                                         \
    do {                                                                \
        PINDENT("  Enter %s [%s]:  ", str, _convert_RMA(blk->field, len)); \
        if ( _readval()) {                                              \
            if ( !strlen(_set_buf)) break;                              \
            if ( _parse_RMA( _set_buf, blk->field, len) == 0) {         \
                break;                                                  \
            }                                                           \
            else {                                                      \
                INPUT_FAILURE(str, "Unable to parse RMA");              \
            }                                                           \
        }                                                               \
    } while (1)

// For gathering MAC address
#define FILL_MAC(str,field)                                             \
    do {                                                                \
        PINDENT("  Enter %s [%s]:  ", str, _ether_sprintf(blk->field)); \
        if ( _readval()) {                                              \
            if ( !strlen(_set_buf)) break;                              \
            if ( _parseMacAddrNoSwap( _set_buf, blk->field, 1) == 0) {  \
                break;                                                  \
            }                                                           \
            else {                                                      \
                INPUT_FAILURE(str, "Unable to parse MAC Address");      \
            }                                                           \
        }                                                               \
    } while (1)

// check that max is not exceeded
#define FILL_MAX(str,pfmt,field,max)                                    \
    do {                                                                \
        uint32_t tmp;                                                     \
        PINDENT("  Enter %s ["#pfmt"]:  ", str, blk->field);            \
        if ( _readval()) {                                              \
            if ( !strlen(_set_buf)) break;                              \
            if ( _isintstr(_set_buf) ) {                                \
                sscanf( _set_buf, "%u", &tmp);                          \
                if ( tmp < max) {                                       \
                    blk->field = tmp;                                   \
                    break;                                              \
                }                                                       \
                else {                                                  \
                    INPUT_FAILURE(str, "Exceeds maximum value of %d", max); \
                }                                                       \
            }                                                           \
            else {                                                      \
                INPUT_FAILURE(str, "Not an integer string");            \
            }                                                           \
        }                                                               \
    } while (1)

// For gathering port types strings
#define FILL_PORT_TYPE(str,field,len)                                   \
    do {                                                                \
        PINDENT("  Enter %s [%s]:  ", str, _convert_PortNum(blk->field, len)); \
        if ( _readval()) {                                              \
            if ( !strlen(_set_buf)) break;                              \
            if ( _parse_PortNum( _set_buf, blk->field, len) == 0) {     \
                break;                                                  \
            }                                                           \
            else {                                                      \
                INPUT_FAILURE(str, "Unable to parse Port Numbers");     \
            }                                                           \
        }                                                               \
    } while (1)

// For hardware revision
#define FILL_HW(str,maj,min)                                            \
    do {                                                                \
        int nrd;                                                        \
        PINDENT("  Enter %s [%d.%d]:  ", str, blk->maj, blk->min);      \
        if ( _readval()) {                                              \
            if ( !strlen(_set_buf)) break;                              \
            if ( sscanf( _set_buf, "%hd.%hd%n", &(blk->maj), &(blk->min), &nrd) == 2 ) { \
                if (nrd == (int)strlen(_set_buf)) {                          \
                    break;                                              \
                }                                                       \
            }                                                           \
            INPUT_FAILURE(str, "Unable to parse HW revision. Expecting maj.min ");          \
        }                                                               \
    } while (1)

// For Backplane License Usage Bits
#define FILL_LIC(field)                                                 \
    do {                                                                \
        int i;                                                          \
        PINDENT("  License Usage Bits:\n");                             \
        i=-1;                                                           \
        PINDENT("   ");                                                 \
        while (++i < LIC_USAGE_SIZE) {                                  \
            PMSG(" %02x", blk->lic_usage_bits[i]);                      \
            if ((i & 7) == 7) {                                         \
                PINDENT("\n");                                          \
                PINDENT("   ");                                         \
            }                                                           \
        }                                                               \
        PMSG("\n");                                                     \
        PINDENT("Enter License Usage Bits in %d Consecutive Hex number 0-FF:\n", \
                LIC_USAGE_SIZE);                                        \
        PINDENT("    ");                                                \
        if ( _readval()) {                                              \
            if ( !strlen(_set_buf)) break;                              \
            if ( _parse_LicUsageBit( _set_buf, blk->field) == 0) {      \
                break;                                                  \
            }                                                           \
            else {                                                      \
                INPUT_FAILURE("License Usage Bits", "Unable to parse License Usage Bits"); \
            }                                                           \
        }                                                               \
    } while (1)

static void
_fill_common_blk( sprom_common_block_t* const blk, const char* name,
                  uint16_t nblocks, uint16_t eeprom_size, uint32_t indent)
{
    PINDENT("Filling %s Common Block:\n", name);

    // fill common block fields
    blk->block_count  = nblocks;
    blk->sprom_size   = eeprom_size;

    FILL_HEX("FRU Major Type", 0x%04x, %hx, fru_major_t);
    FILL_HEX("FRU Minor Type", 0x%04x, %hx, fru_minor_t);
    if ( strlen( blk->OEM_string) == 0) {
        strncpy( blk->OEM_string, SPROM_COMMON_BLOCK_OEM_STRING, OEM_STRING_SIZE-1);
    }
    FILL_STR("OEM String", OEM_string, OEM_STRING_SIZE);
    FILL_STR("Product Number", product_number, PRODUCT_NUMBER_SIZE);
    FILL_STR("Serial Number", serial_number, SERIAL_NUMBER_SIZE);
    FILL_STR("Part Number", part_number, PART_NUMBER_SIZE);
    FILL_STR("Part Revision", part_revision, PART_REVISION_SIZE);
    FILL_STR("Mfg Deviation", mfg_deviation, MFG_DEVIATION_SIZE);
    FILL_HW("HW Revision", hw_rev_major, hw_rev_minor);
    FILL_HEX("Mfg Bits", 0x%04x, %hx, mfg_bits);
    FILL_HEX("Engineering Use", 0x%04x, %hx, eng_bits);
    FILL_SNMP("SNMP OID", snmpOID, SNMP_OID_SIZE);
    FILL_INT("Power Consumption", %d, %hd, power_consumption);
    FILL_RMA("RMA Code", RMA_failure_code, RMA_FAILURE_CODE_SIZE);
    FILL_STR("CLEI Code", CLEI_code, CLEI_CODE_SIZE);
    FILL_STR("Volume ID", VID, VID_SIZE);

    // update the block header
    _fill_blk_header( (sprom_block_header_t*)blk,
                      SPROM_COMMON_BLOCK_SIG,
                      SPROM_COMMON_BLOCK_CUR_VER,
                      SPROM_COMMON_BLOCK_DATA_SIZE);

}

void
_fill_common_blk_n2k( sprom_common_block_n2k_t* const blk, const char* name,
                  uint16_t nblocks, uint16_t eeprom_size, uint32_t indent)
{
    PINDENT("Filling %s Common Block N2K:\n", name);

    // fill common block fields
    blk->block_count  = nblocks;
    blk->sprom_size   = eeprom_size;

    FILL_HEX("FRU Major Type", 0x%04x, %hx, fru_major_t);
    FILL_HEX("FRU Minor Type", 0x%04x, %hx, fru_minor_t);
    if ( strlen( blk->OEM_string) == 0) {
        strncpy( blk->OEM_string, SPROM_COMMON_BLOCK_OEM_STRING, OEM_STRING_SIZE-1);
    }
    FILL_STR("OEM String", OEM_string, OEM_STRING_SIZE);
    FILL_STR("Product Number", product_number, PRODUCT_NUMBER_SIZE);
    FILL_STR("Serial Number", serial_number, SERIAL_NUMBER_SIZE);
    FILL_STR("Part Number", part_number, PART_NUMBER_SIZE);
    FILL_STR("Part Revision", part_revision, PART_REVISION_SIZE);
    FILL_STR("Mfg Deviation", mfg_deviation, MFG_DEVIATION_SIZE);
    FILL_HW("HW Revision", hw_rev_major, hw_rev_minor);
    FILL_HEX("Mfg Bits", 0x%04x, %hx, mfg_bits);
    FILL_HEX("Engineering Use", 0x%04x, %hx, eng_bits);
    FILL_SNMP("SNMP OID", snmpOID, SNMP_OID_SIZE);
    FILL_INT("Power Consumption", %d, %hd, power_consumption);
    FILL_RMA("RMA Code", RMA_failure_code, RMA_FAILURE_CODE_SIZE);
    FILL_STR("CLEI Code", CLEI_code, CLEI_CODE_SIZE);
    FILL_STR("Volume ID", VID, VID_SIZE);

    // update the block header
    _fill_blk_header( (sprom_block_header_t*)blk,
                      SPROM_COMMON_BLOCK_SIG,
                      SPROM_COMMON_BLOCK_CUR_VER,
                      SPROM_COMMON_BLOCK_DATA_SIZE);

}

static void
_fill_lc_blk_no_hdr( sprom_line_card_block_t* const blk,
                     const char* name, uint32_t indent)
{
    int  i;
    char __buf[64];

    PINDENT("Filling %s Main Block:\n", name);

    // fill lc block fields
    FILL_HEX("Feature Bits", 0x%llx, %llx, feature_bits);
    FILL_HEX("HW Changes Bits", 0x%llx, %llx, hw_changes_bits);
    FILL_INT("Card Index", %hu, %hu, card_index);
    FILL_MAC("MAC Address", mac_base);
    FILL_INT("Number of MAC Addr", %hu, %hu, mac_length);
    FILL_INT("Number of EOBC links", %hhu, %hhu, eobc_connections);
    FILL_MAX("Number of EPLDs", %hhu, epld_num, MAX_EPLD_VERSIONS);

    if (blk->epld_num > 0) {
        if ( blk->epld_num > 15) {
            // epld version data is 1-byte per epld
            for (i=0; i < blk->epld_num; ++i) {
                sprintf( __buf,"EPLD %c", epld_char[i]);
                FILL_HEX( __buf, 0x%02hhx, %hhx, epld_versions[i]);
            }
        }
        else {
            // epld version data is 2-bytes per epld
            for (i=0; i < blk->epld_num; ++i) {
                sprintf( __buf,"EPLD %c", epld_char[i]);
                FILL16( __buf, 0x%04hx, %hx, epld_versions[i*2]);
            }
        }
    }

    FILL_PORT_TYPE("Port-Num (format: type-num;type-num)",
                   port_data, MAX_PORT_TYPES);
    FILL_INT("SRAM size", %hu, %hu, sram_size);

    for (i=0; i<MAX_CARD_SENSORS; ++i) {
        sprintf( __buf,"Sensor #%d major", i+1);
        FILL_RANGE( __buf, %hhd,
                    sensor_data[i].major_threshold, -128, 127);
        sprintf( __buf,"Sensor #%d minor", i+1);
        FILL_RANGE( __buf, %hhd,
                    sensor_data[i].minor_threshold, -128, 127);
    }

    FILL_INT("MAX Connector Power", %hu, %hu, max_connector_power);
    FILL_INT("Cooling Requirement", %hu, %hu, cooling_requirement);
    FILL_INT("Ambient Temperature", %hhd, %hhd, ambient_temperature);
}

static void
_fill_lc_blk( sprom_line_card_block_t* const blk, const char* name, uint32_t indent)
{

    // fill lc block fields
    _fill_lc_blk_no_hdr( blk, name, indent);

    // update the block header
    _fill_blk_header( &blk->block_header,
                      SPROM_LINE_CARD_BLOCK_SIG,
                      SPROM_LINE_CARD_BLOCK_CUR_VER,
                      SPROM_LINE_CARD_BLOCK_DATA_SIZE);
}

static void
_fill_sensor_blk( sprom_temp_sensor_block_t* const blk, const char* name, uint32_t indent)
{
    int  i;
    char __buf[64];

    PINDENT("Filling %s Temperature Sensor Block:\n", name);

    // fill temp sensor block fields
    FILL_MAX("Number of Sensors", %hhu, num_sensors, MAX_TEMP_SENSORS);
    for (i=0; i<MAX_TEMP_SENSORS; i++) {
        if ( i < blk->num_sensors) {
            sprintf( __buf,"Sensor #%d major", i+1);
            FILL_RANGE( __buf, %hhd,
                        sensor_data[i].major_threshold, -128, 127);
            sprintf( __buf,"Sensor #%d minor", i+1);
            FILL_RANGE( __buf, %hhd,
                        sensor_data[i].minor_threshold, -128, 127);
        }
        else {
            // zero out the remaining
            blk->sensor_data[i].major_threshold = 0;
            blk->sensor_data[i].minor_threshold = 0;
        }
    }

    // update the block header
    _fill_blk_header( &blk->block_header,
                      SPROM_SENSOR_BLOCK_SIG,
                      SPROM_SENSOR_BLOCK_CUR_VER,
                      SPROM_SENSOR_BLOCK_DATA_SIZE);

}

static void
_fill_lc( diag_sprom_t* const sprom, uint8_t *buf )
{
    line_card_sprom_t* lc_sprom = (line_card_sprom_t*)buf;

    // fill common header
    _fill_common_blk( &lc_sprom->common_block,
                      sprom_get_name ( sprom ),
                      SPROM_LINE_CARD_NUM_BLOCKS,
                      SPROM_CUR_EEPROM_SIZE,
                      INDENT_PER_LEVEL);

    // fill line card block
    _fill_lc_blk( &lc_sprom->lc_block,
                  sprom_get_name( sprom ),
                  INDENT_PER_LEVEL);

    // fill extra sensor block
    _fill_sensor_blk( &lc_sprom->sensor_block,
                      sprom_get_name( sprom ),
                      INDENT_PER_LEVEL);
}

static void
_fill_sup_blk( sprom_sup_block_t* const blk, const char* name, uint32_t indent)
{
    // At the current moment the SUP and LC blocks have identical
    // formats even though two seperate structs are defined.  For now
    // use the line card routines previously defined, except for the
    // header.

    // fill lc/sup block fields
    _fill_lc_blk_no_hdr( (sprom_line_card_block_t*)blk, name, indent);


    // update the block header
    _fill_blk_header( &blk->block_header,
                      SPROM_SUP_BLOCK_SIG,
                      SPROM_SUP_BLOCK_CUR_VER,
                      SPROM_SUP_BLOCK_DATA_SIZE);

}

static void
_fill_sup( diag_sprom_t* const sprom, uint8_t *buf )
{
    sup_sprom_t* sup_sprom = (sup_sprom_t*)buf;

    // fill common header
    _fill_common_blk( &sup_sprom->common_block,
                      sprom_get_name( sprom ),
                      SPROM_SUP_NUM_BLOCKS,
                      SPROM_CUR_EEPROM_SIZE,
                      INDENT_PER_LEVEL);

    // fill sup block
    _fill_sup_blk( &sup_sprom->sup_block,
                   sprom_get_name( sprom ),
                   INDENT_PER_LEVEL);

    // fill extra sensor block
    _fill_sensor_blk( &sup_sprom->sensor_block,
                      sprom_get_name( sprom ),
                      INDENT_PER_LEVEL);
}

static void
_fill_bp_blk( sprom_backplane_block_t* const blk, const char* name, uint32_t indent)
{
    PINDENT("Filling %s Main Block:\n", name);

    // fill backplane block fields
    FILL_HEX("Feature Bits", 0x%llx, %llx, feature_bits);
    FILL_HEX("HW Changes Bits", 0x%llx, %llx, hw_changes_bits);
    FILL_INT("Stackmib OID", %hu, %hu, stackmib_oid);
    FILL_MAC("MAC Address", mac_base);
    FILL_INT("Number of MAC Addr", %hu, %hu, mac_length);
    FILL_INT("OEM Enterprise Number", %hu, %hu, OEM_Enterprise);
    FILL_INT("OEM MIB Offset", %hu, %hu, OEM_MIB_Offset);
    FILL_INT("MAX Connector Power", %hu, %hu, max_connector_power);
    FILL_INT("Cooling Coefficient", %hhu, %hhu, cooling_coefficient);

    // update the block header
    _fill_blk_header( &blk->block_header,
                      SPROM_BACKPLANE_BLOCK_SIG,
                      SPROM_BACKPLANE_BLOCK_CUR_VER,
                      SPROM_BACKPLANE_BLOCK_DATA_SIZE);
}

static int
_confirm_wwn_block_clear (uint32_t indent)
{
    char buffer[16];
    int rtn_val;

    if ( _in_strm != stdin) {
        // assume yes when reading input from a file
        return 1;
    }

    do {
        PINDENT("Clear wwn-block on backplane sprom (y/n) [n]? ");
    } while (fgets( buffer, sizeof(buffer)-1, stdin) == NULL);

    if (buffer[0] == 'y' || buffer[0] == 'Y') {
        rtn_val = 1;
    } else {
        rtn_val = 0;
    }

    return rtn_val;
}

static void
_fill_bp_wwn_blk( sprom_wwn_block_t* const blk, const char* name, uint32_t indent)
{

    PINDENT("Filling %s WWN Block:\n", name);

    // fill backplane WWN block fields
    if (_confirm_wwn_block_clear(indent)) {
        memset(blk->wwn_usage_bits, 0, sizeof(blk->wwn_usage_bits));
    }

    // update the block header
    _fill_blk_header( &blk->block_header,
                      SPROM_WWN_BLOCK_SIG,
                      SPROM_WWN_BLOCK_CUR_VER,
                      SPROM_WWN_BLOCK_DATA_SIZE);
}

static void
_fill_bp_lic_blk( sprom_lic_block_t* const blk, const char* name, uint32_t indent)
{
    PINDENT("Filling %s License Block:\n", name);

    // fill backplane License block fields
    FILL_LIC( lic_usage_bits);
    memset( blk->pad, 0, sizeof( blk->pad));

    // update the block header
    _fill_blk_header( &blk->block_header,
                      SPROM_LIC_BLOCK_SIG,
                      SPROM_LIC_BLOCK_CUR_VER,
                      SPROM_LIC_BLOCK_DATA_SIZE);
}

static void
_fill_sn2_blk( sprom_sn2_block_t* const blk, const char* name, uint32_t indent)
{
    PINDENT("Filling %s 2nd Serial Number Block:\n", name);

    // fill 2nd serial number block fields
    FILL_STR("Serial Number", serial_number, SERIAL_NUMBER_SIZE);
    memset( blk->pad, 0, sizeof( blk->pad));

    // update the block header
    _fill_blk_header( &blk->block_header,
                      SPROM_SN2_BLOCK_SIG,
                      SPROM_SN2_BLOCK_CUR_VER,
                      SPROM_SN2_BLOCK_DATA_SIZE);
}

static void
_fill_bp( diag_sprom_t* const sprom, uint8_t *buf )
{
    backplane_sprom_t* bp_sprom = (backplane_sprom_t*)buf;

    // fill common header
    _fill_common_blk( &bp_sprom->common_block,
                      sprom_get_name( sprom ),
                      SPROM_BACKPLANE_NUM_BLOCKS,
                      SPROM_CUR_EEPROM_SIZE,
                      INDENT_PER_LEVEL);

    // fill backplane block
    _fill_bp_blk( &bp_sprom->backplane_block,
                  sprom_get_name( sprom ),
                  INDENT_PER_LEVEL);

    // fill backplane WWN block
    _fill_bp_wwn_blk( &bp_sprom->wwn_block,
                      sprom_get_name( sprom ),
                      INDENT_PER_LEVEL);

    // fill backplane License block
    _fill_bp_lic_blk( &bp_sprom->lic_block,
                      sprom_get_name( sprom ),
                      INDENT_PER_LEVEL);
    // fill backplane 2nd serial number block
    _fill_sn2_blk( &bp_sprom->sn2_block,
                   sprom_get_name( sprom ),
                   INDENT_PER_LEVEL);
}

#ifndef DIAG_FEX
static void
_fill_ps_blk( sprom_power_supply_block_t* const blk, const char* name, uint32_t indent)
{
    PINDENT("Filling %s Main Block:\n", name);

    // fill power supply block fields
    FILL_HEX("Feature Bits", 0x%llx, %llx, feature_bits);
    FILL_INT("Current in cA for 110-Volts", %hu, %hu, current_110v);
    FILL_INT("Current in cA for 220-Volts", %hu, %hu, current_220v);
    FILL_INT("Stackmib OID", %hu, %hu, stackmib_oid);
    FILL_INT("Current Mode 1 in cA", %hu, %hu, current_mode1);
    FILL_INT("Current Mode 2 in cA", %hu, %hu, current_mode2);
    FILL_INT("Current Mode 3 in cA", %hu, %hu, current_mode3);
    FILL_INT("Current Mode 4 in cA", %hu, %hu, current_mode4);
    FILL_INT("Max Float for Current Mode 1 in cA", %hu, %hu,
             max_float_current_mode1);
    FILL_INT("Max Float for Current Mode 2 in cA", %hu, %hu,
             max_float_current_mode2);
    FILL_INT("Max Float for Current Mode 3 in cA", %hu, %hu,
             max_float_current_mode3);
    FILL_INT("Max Float for Current Mode 4 in cA", %hu, %hu,
             max_float_current_mode4);

    // update the block header
    _fill_blk_header( (sprom_block_header_t*)blk,
                      SPROM_POWER_SUPPLY_BLOCK_SIG_1,
                      SPROM_POWER_SUPPLY_BLOCK_CUR_VER,
                      SPROM_POWER_SUPPLY_BLOCK_DATA_SIZE);
}

static void
_fill_ps( diag_sprom_t* const sprom, uint8_t *buf )
{
    power_supply_sprom_t* ps_sprom = (power_supply_sprom_t*)buf;

    // fill common header
    _fill_common_blk_n2k( &ps_sprom->common_block,
                      sprom_get_name( sprom ),
                      SPROM_POWER_SUPPLY_NUM_BLOCKS,
                      SPROM_CUR_EEPROM_SIZE,
                      INDENT_PER_LEVEL);

    // fill power supply block
    _fill_ps_blk( &ps_sprom->power_supply_block,
                  sprom_get_name( sprom ),
                  INDENT_PER_LEVEL);

}
#else
static void
_fill_ps( diag_sprom_t* const sprom, uint8_t *buf )
{
    int indent = INDENT_PER_LEVEL;
    power_supply_sprom_fex_t* blk = (power_supply_sprom_fex_t*)buf;


    PINDENT("Filling %s Common Block:\n", sprom_get_name(sprom));

    // fill common block fields
    blk->sprom_size       = SPROM_POWER_SUPPLY_FEX_DATA_SIZE;
    blk->block_count      = 1;
    blk->null_char        = 0;

    FILL_HEX("FRU Major Type", 0x%04x, %hx, fru_major_t);
    FILL_HEX("FRU Minor Type", 0x%04x, %hx, fru_minor_t);
    if ( strlen( blk->OEM_string) == 0) {
        strncpy( blk->OEM_string, SPROM_COMMON_BLOCK_OEM_STRING, OEM_STRING_SIZE-1);
    }
    FILL_STR("OEM String", OEM_string, OEM_STRING_SIZE);
    FILL_STR("Product Number", product_number, FEX_PS_PRODUCT_NUMBER_SIZE);
    FILL_STR("Serial Number", serial_number, FEX_PS_SERIAL_NUMBER_SIZE);
    FILL_STR("Part Number", part_number, FEX_PS_PART_NUMBER_SIZE);
    FILL_STR("Part Revision", part_revision, PART_REVISION_SIZE);
    FILL_STR("CLEI", clei, FEX_PS_CLEI_SIZE);
    FILL_CHARS("Version ID", version_id, FEX_PS_VID_SIZE);
    FILL_SNMP("SNMP OID", snmpOID, SNMP_OID_SIZE);
    FILL_HW("HW Revision", hw_rev_major, hw_rev_minor);
    FILL_INT("Power Consumption", %d, %hd, power_consumption);
    FILL_RMA("RMA Code", RMA_failure_code, RMA_FAILURE_CODE_SIZE);

//    FILL_INT("Fault Byte", %hhu, %hhu, fault_byte);
//    FILL_INT("Fan Speed Byte", %hhu, %hhu, fan_speed_byte);
//    FILL_INT("Temp Byte", %hhu, %hhu, temp_byte);

    // update the block header
    blk->block_signature = SPROM_COMMON_BLOCK_SIG;
    blk->block_version   = SPROM_POWER_SUPPLY_BLOCK_FEX_VER;
    blk->block_length    = SPROM_POWER_SUPPLY_FEX_DATA_SIZE;
    blk->block_checksum  = diag_sprom_compute_xsum(blk, blk->block_length);
}
#endif

static void
_fill_fan_blk( sprom_fan_block_t* const blk, const char* name, uint32_t indent)
{
    PINDENT("Filling %s Main Block:\n", name);

    // fill fan block fields
    FILL_HEX("Feature Bits", 0x%llx, %llx, feature_bits);
    FILL_HEX("HW Changes Bits", 0x%llx, %llx, hw_changes_bits);
    FILL_INT("Stackmib OID", %hu, %hu, stackmib_oid);
    FILL_INT("Cooling Capacity", %hu, %hu, cooling_capacity);
    FILL_INT("Ambient Temperature", %hhd, %hhd, ambient_temperature);
    FILL_INT("Power Consumption 2", %hd, %hd, power_consumption2);
    FILL_INT("Cooling Capacity 2", %hu, %hu, cooling_capacity2);

    // update the block header
    _fill_blk_header( &blk->block_header,
                      SPROM_FAN_BLOCK_SIG,
                      SPROM_FAN_BLOCK_CUR_VER,
                      SPROM_FAN_BLOCK_DATA_SIZE);
}

static void
_fill_fan( diag_sprom_t* const sprom, uint8_t *buf )
{
    fan_sprom_t* fan_sprom = (fan_sprom_t*)buf;

    // fill common header
    _fill_common_blk( &fan_sprom->common_block,
                      sprom_get_name( sprom ),
                      SPROM_FAN_NUM_BLOCKS,
                      SPROM_CUR_EEPROM_SIZE,
                      INDENT_PER_LEVEL);

    // fill fan block
    _fill_fan_blk( &fan_sprom->fan_block,
                   sprom_get_name( sprom ),
                   INDENT_PER_LEVEL);

    // fill fan 2nd serial number block
    _fill_sn2_blk( &fan_sprom->sn2_block,
                   sprom_get_name( sprom ),
                   INDENT_PER_LEVEL);
}

static int 
_fill_sprom ( diag_sprom_t * const sprom, uint8_t *buf )
{
    int rc = 0;

    switch( sprom->type) {
    case SPROM_UTIL_TYPE_LC:
        _fill_lc( sprom, buf );
        break;
    case SPROM_UTIL_TYPE_SUP:
        _fill_sup( sprom, buf );
        break;
    case SPROM_UTIL_TYPE_BP:
        _fill_bp( sprom, buf );
        break;
    case SPROM_UTIL_TYPE_PS:
        _fill_ps( sprom, buf );
        break;
    case SPROM_UTIL_TYPE_FAN:
        _fill_fan( sprom, buf );
        break;
    case SPROM_UTIL_TYPE_BROCKWAY_UPLINK_SUP:
        _fill_sup( sprom, buf );
        break;
    case SPROM_UTIL_TYPE_BROCKWAY_UPLINK_BP:
        _fill_bp( sprom, buf );
        break;
    default:
        break;
    };

    return rc;
}

/*
** General set sprom entry point, for all SPROM types
*/
int 
sprom_set ( diag_sprom_t * const sprom )
{
    int rc = 0;
    uint8_t *rwbuf;

    _in_strm = stdin;

    rwbuf = malloc ( sprom_get_size ( sprom ) );
    if ( rwbuf == NULL ) {
        return ( -SPROM_ERR_NOMEM );
    }

    if ( ( rc = sprom_read ( sprom, rwbuf ) ) < 0 ){
        goto sprom_set_exit;
    }

    if ( ( rc = _fill_sprom( sprom, rwbuf ) ) < 0 ) {
        goto sprom_set_exit;
    }

    if ( ( rc = sprom_show ( sprom, rwbuf ) ) < 0 ) {
        goto sprom_set_exit;
    }

    if ( sprom_confirm_set ( sprom ) ) {
        if ( ( rc = sprom_write ( sprom, rwbuf ) ) < 0 ) {
            sprom_printf ( "Unable to write sprom contents.\n" );
            goto sprom_set_exit;
        }
    }

sprom_set_exit:
    free(rwbuf);

    return rc;
}

/*
** Update mac_base and mac_length information. Only valid
** for SUP, LC and backplane SPROM type.
*/
int 
sprom_set_mac ( diag_sprom_t * const sprom )
{
    int rc = 0;
    uint32_t indent = INDENT_PER_LEVEL;
    uint8_t *rwbuf;

    if ( ( sprom->type != SPROM_UTIL_TYPE_BP ) &&
      ( sprom->type != SPROM_UTIL_TYPE_SUP) &&
      ( sprom->type != SPROM_UTIL_TYPE_BROCKWAY_UPLINK_SUP) &&
      ( sprom->type != SPROM_UTIL_TYPE_BROCKWAY_UPLINK_BP) &&
      ( sprom->type != SPROM_UTIL_TYPE_LC ) ) {
        sprom_err ("The set-mac command is only used for the SUP, "
          "LC and Backplane SPROM.\n");
        return ( -SPROM_ERR_INVALID );
    }

    // prompt user for input values, using stdin
    _in_strm = stdin;

    // first fetch current contents
    
    rwbuf = malloc ( sprom_get_size ( sprom ) );
    if ( rwbuf == NULL ) {
        return ( -SPROM_ERR_NOMEM );
    }

    if ( ( rc = sprom_read ( sprom, rwbuf ) ) < 0 ){
        goto sprom_set_mac_exit;
    }

    if ( sprom->type == SPROM_UTIL_TYPE_LC ) {
        line_card_sprom_t*       lc;
        sprom_line_card_block_t* blk;

        lc  = (line_card_sprom_t*)rwbuf;
        blk = &lc->lc_block;
        PINDENT("Updating Line Card Data:\n");
        FILL_MAC("MAC Address", mac_base);
        FILL_INT("Number of MAC Addr", %hu, %hu, mac_length);

        // update the block header
        _fill_blk_header( &blk->block_header,
                          SPROM_LINE_CARD_BLOCK_SIG,
                          SPROM_LINE_CARD_BLOCK_CUR_VER,
                          SPROM_LINE_CARD_BLOCK_DATA_SIZE);

        PINDENT("\n");
        PINDENT("Verify Line Card Data:\n");
        PINDENT("  Line Card:  MAC Address: %s\n",
                _ether_sprintf(blk->mac_base));
        PINDENT("  Line Card:  Number of MAC Addr: %hu\n",
                blk->mac_length);
    } else if ( (sprom->type == SPROM_UTIL_TYPE_SUP) ||
                (sprom->type == SPROM_UTIL_TYPE_BROCKWAY_UPLINK_SUP)) {
        sup_sprom_t*       sup;
        sprom_sup_block_t* blk;

        sup  = (sup_sprom_t*)rwbuf;
        blk = &sup->sup_block;
        PINDENT("Updating Supervisor Data:\n");
        FILL_MAC("MAC Address", mac_base);
        FILL_INT("Number of MAC Addr", %hu, %hu, mac_length);

        // update the block header
        _fill_blk_header( &blk->block_header,
                          SPROM_SUP_BLOCK_SIG,
                          SPROM_SUP_BLOCK_CUR_VER,
                          SPROM_SUP_BLOCK_DATA_SIZE);

        PINDENT("\n");
        PINDENT("Verify Supervisor Data:\n");
        PINDENT("  Supervisor:  MAC Address: %s\n",
                _ether_sprintf(blk->mac_base));
        PINDENT("  Supervisor:  Number of MAC Addr: %hu\n",
                blk->mac_length);
    } else {
        backplane_sprom_t*       bp;
        bp  = (backplane_sprom_t*)rwbuf;

        {
            sprom_backplane_block_t* blk;

            blk = &bp->backplane_block;
            PINDENT("Updating Backplane Data:\n");
            FILL_MAC("MAC Address", mac_base);
            FILL_INT("Number of MAC Addr", %hu, %hu, mac_length);
            // update the block header
            _fill_blk_header( &blk->block_header,
                              SPROM_BACKPLANE_BLOCK_SIG,
                              SPROM_BACKPLANE_BLOCK_CUR_VER,
                              SPROM_BACKPLANE_BLOCK_DATA_SIZE);
        }

        PINDENT("\n");
        PINDENT("Verify Backplane Data:\n");
        PINDENT("  Backplane:  MAC Address: %s\n",
                _ether_sprintf(bp->backplane_block.mac_base));
        PINDENT("  Backplane:  Number of MAC Addr: %hu\n",
                bp->backplane_block.mac_length);
    }

    // ask if OK to write?
    if ( sprom_confirm_set ( sprom ) ) {
        // write new data back if OK
        if ( ( rc = sprom_write ( sprom, rwbuf ) ) < 0 ) {
            sprom_err ( "Unable to write sprom contents.\n" );
            rc = SPROM_ERR_IO;
            goto sprom_set_mac_exit;
        }
    }

 sprom_set_mac_exit:
    free(rwbuf);

    return rc;
}

/*
** Update a field in the common block, for all SPROM types.
*/
#define UNKNOWN_SPROM_OPC (0)

int
sprom_set_common_blk_field ( diag_sprom_t * const sprom, sprom_cmd_t field )
{
    int rc = 0;
    sprom_common_block_t* blk;
    uint32_t indent = INDENT_PER_LEVEL;
    uint8_t *rwbuf;

    // prompt user for input values, using stdin
    _in_strm = stdin;

    rwbuf = malloc ( sprom_get_size ( sprom ) );
    if ( rwbuf == NULL ) {
        return ( -SPROM_ERR_NOMEM );
    }

    if ( ( rc = sprom_read ( sprom, rwbuf ) ) < 0 ){
        goto sprom_set_common_blk_field_exit;
    }

    // Irrespective of type all SPROMs start with the common block.
    blk = (sprom_common_block_t*)rwbuf;

    // The callback parm contains the opcode
    switch ( (sprom_cmd_t) field ) {
    case SPROM_CMD_SET_SERIAL:
        FILL_STR("Serial Number", serial_number, SERIAL_NUMBER_SIZE);
        PINDENT("  The new serial number is: %s\n", blk->serial_number);
        break;
    case SPROM_CMD_SET_PART_NUM:
        FILL_STR("Part Number", part_number, PART_NUMBER_SIZE);
        PINDENT("  The new part number is: %s\n", blk->part_number);
        break;
    case SPROM_CMD_SET_PART_REV:
        FILL_STR("Part Revision", part_revision, PART_REVISION_SIZE);
        PINDENT("  The new part revision is: %s\n", blk->part_revision);
        break;
    case SPROM_CMD_SET_PART_VER:
        FILL_HW("HW Revision", hw_rev_major, hw_rev_minor);
        PINDENT("  The new HW version is: %u.%u\n", blk->hw_rev_major, blk->hw_rev_minor);
        break;
    default:
        sprom_err ("%s(): Unknown SPROM_OPC: %u.  Exiting...\n",
                __FUNCTION__, field );
        return ( -SPROM_ERR_INVALID );
    };

    // update the block header
    _fill_blk_header( &blk->block_header,
                      SPROM_COMMON_BLOCK_SIG,
                      SPROM_COMMON_BLOCK_CUR_VER,
                      SPROM_COMMON_BLOCK_DATA_SIZE);

    // ask if OK to write?
    if ( sprom_confirm_set ( sprom ) ) {
        // write new data back if OK
        if ( ( rc = sprom_write ( sprom, rwbuf ) ) < 0 ) {
            sprom_err ( "Unable to write sprom contents.\n" );
            rc = SPROM_ERR_IO;
            goto sprom_set_common_blk_field_exit;
        }
    }

sprom_set_common_blk_field_exit:
    free(rwbuf);

    return rc;
}

/*
** Update license information in backplane License block.  Only valid
** for backplane SPROM type.
*/
int
sprom_set_lic ( diag_sprom_t * const sprom )
{
    int rc = 0;
    backplane_sprom_t* bp;
    sprom_lic_block_t* blk;
    uint32_t indent = INDENT_PER_LEVEL;
    uint32_t i;
    uint8_t *rwbuf;

    if ( sprom->type != SPROM_TYPE_BPLANE) {
        sprom_err ("The set-license command is only used for the backplane SPROM.\n");
        return SPROM_ERR_INVALID;
    }

    // prompt user for input values, using stdin
    _in_strm = stdin;

    // first fetch current contents
    rwbuf = malloc ( sprom_get_size ( sprom ) );
    if ( rwbuf == NULL ) {
        return ( -SPROM_ERR_NOMEM );
    }

    if ( ( rc = sprom_read ( sprom, rwbuf ) ) < 0 ){
        goto sprom_set_lic_exit;
    }

    bp  = (backplane_sprom_t*)rwbuf;
    blk = &bp->lic_block;

    // fill backplane License block fields
    FILL_LIC( lic_usage_bits);

    // update the block header
    _fill_blk_header( &blk->block_header,
                      SPROM_LIC_BLOCK_SIG,
                      SPROM_LIC_BLOCK_CUR_VER,
                      SPROM_LIC_BLOCK_DATA_SIZE);

    PINDENT("  License Usage Bits:\n");
    PINDENT("   ");
    for ( i = 0; i < LIC_USAGE_SIZE; i++) {
        sprom_printf( " %02x", blk->lic_usage_bits[i]);
    }
    sprom_printf ( "\n");

    // ask if OK to write?
    if ( sprom_confirm_set ( sprom ) ) {
        // write new data back if OK
        if ( ( rc = sprom_write ( sprom, rwbuf ) ) < 0 ) {
            sprom_err ( "Unable to write sprom contents.\n" );
            rc = SPROM_ERR_IO;
            goto sprom_set_lic_exit;
        }
    }    

sprom_set_lic_exit:
    free(rwbuf);

    return rc;
}
