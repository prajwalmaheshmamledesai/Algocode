/********************************************************************//**
@file    diag_sprom_defs.h
@author  Curt Brune -- cbrune@cisco.com
@brief   Diag platform SPROM library public definitions

@verbatim
  CVS $Id:$
  CVS $Source:$
  CVS $Author: cbrune $
@endverbatim

These definitions need to be kept in sync with the DCOS definitions of
the SPROMS.

See these files:

Definition of sprom_type_t
platform/storage/common/kspace/sprom/export/isan/sprom_lib_intf.h

Definition of the various SPROM blocks
platform/storage/common/kspace/sprom/export/isan/sprom_defs.h

************************************************************************/

#ifndef __DIAG_SPROM_DEFS_H
#define __DIAG_SPROM_DEFS_H

#define MAX_CMP_SPROMS         1
#define MAX_FAN_SPROMS         8
#define MAX_PS_SPROMS          4

#define PACKED __attribute__((packed))

typedef uint8_t pm_phy_port_layer_t;
typedef uint8_t port_num_t;

typedef enum {
    SPROM_TYPE_START=1,
    SPROM_TYPE_SUP,
    SPROM_TYPE_BPLANE,
    SPROM_TYPE_PS,
    SPROM_TYPE_FAN,
    SPROM_TYPE_END
} sprom_type_t;

/*
** For the selected SPROM use the A or B bus
*/
typedef enum {
    SPROM_BUS_NONE = 0,  // Not defined yet which bus to use
    SPROM_BUS_A=1,       // Use bus A
    SPROM_BUS_B=2        // Use bus B
} sprom_bus_t;

/*
** For passing parameters to the SPROM interface functions
**
** m_type  : The type of SPROM to access
**
** m_bus : Which bus of the redundant buses to use for access.
** Applies to SPROM_TYPE_LCARD_ATW, SPROM_TYPE_BPLANE, SPROM_TYPE_PS,
** SPROM_TYPE_FAN.
**
** m_lc_no : slot number. used with (m_type == SPROM_TYPE_LCARD_ATW)
**
** m_cs_idx: card index 0-3. used to index into (m_type, m_lc_no)
** tuple.  Index 0 is the main board SPROM and indices 1-3 are for
** daughter cards.
**
**    (m_type == SPROM_TYPE_LCARD)     -- daughter card SPROM access
**    (m_type == SPROM_TYPE_SUP)       -- daughter card SPROM access
**    (m_type == SPROM_TYPE_LCARD_ATW) -- daughter card SPROM access
**    (m_type == SPROM_TYPE_PS)        -- PS SPROM 1 to 4 SPROM access
**    (m_type == SPROM_TYPE_FAN)       -- FAN SPROM 1 to 4 SPROM access
**
** m_offset: offset within SPROM for read/write operation
**
** m_data  : in/out data parameter for read/write operation
**
** m_size  : length of m_data in bytes
*/
typedef struct _sprom_parms_t {
    sprom_type_t     m_type;  // SPROM type
    sprom_bus_t      m_bus;  // which bus to use for access
    uint32_t         m_lc_no;  // slot number
    uint32_t         m_cs_idx;  // card index, used for daughter cards
    uint32_t         m_offset;  // offset within SPROM
    void*            m_data;  // in/out parameter for write/read operations
    size_t           m_size;  // length of m_data
} sprom_parms_t;

/*
** zero-based definitions of m_cs_idx.
*/
#define SPROM_IDX_MAIN_CARD (0)
#define SPROM_IDX_EARL1_DC  (1)
#define SPROM_IDX_EARL2_DC  (2)
#define SPROM_IDX_POE_DC    (3)

/* Other sprom-related defines: */
typedef uint16_t sprom_block_sig_t;

/*
** SPROM structures:
*/

/*
** *******************************************************************
** Common SPROM Block Header -- Used by All SPROMs
**
** NOTE: MAKE SURE THAT BLOCK CHECKSUM OCCURS AT A 16-BIT BOUNDARY!
** *******************************************************************
*/
typedef struct sprom_block_header_
{
    sprom_block_sig_t   block_signature;
    uint8_t               block_version;
    uint8_t               block_length;
    uint16_t             block_checksum;
    uint8_t               block_data[0];
}PACKED sprom_block_header_t;

/*
** *******************************************************************
** SPROM Common Block definitions
**
** NOTE: MAKE SURE THIS STRUCT IS PADDED TO END ON A 16-BIT BOUNDARY!
**
** *******************************************************************
*/

#define OEM_STRING_SIZE         20
#define PRODUCT_NUMBER_SIZE     20
#define SERIAL_NUMBER_SIZE      20
#define PART_NUMBER_SIZE        16
#define PART_REVISION_SIZE      4
#define MFG_DEVIATION_SIZE      20
#define SNMP_OID_SIZE           8
#define RMA_FAILURE_CODE_SIZE   4
#define CLEI_SIZE		15

/* CLEI codes are 10 characters, but with null-padded strings we use 12 bytes
 * to keep it an even number: */
#define CLEI_CODE_SIZE          12
#define VID_SIZE                4
#define MAC_ADDR_SIZE           6

typedef struct sprom_common_block_
{
    sprom_block_header_t        block_header;
    uint16_t                    sprom_size;
    uint16_t                    block_count;
    uint16_t                    fru_major_t;
    uint16_t                    fru_minor_t;
    char                        OEM_string[OEM_STRING_SIZE];
    char                        product_number[PRODUCT_NUMBER_SIZE];
    char                        serial_number[SERIAL_NUMBER_SIZE];
    char                        part_number[PART_NUMBER_SIZE];
    char                        part_revision[PART_REVISION_SIZE];
    char                        mfg_deviation[MFG_DEVIATION_SIZE];
    uint16_t                    hw_rev_major;
    uint16_t                    hw_rev_minor;
    uint16_t                    mfg_bits;
    uint16_t                    eng_bits;
    uint16_t                    snmpOID[SNMP_OID_SIZE];
    short              		power_consumption;
    uint8_t                     RMA_failure_code[RMA_FAILURE_CODE_SIZE];
    char                        CLEI_code[CLEI_CODE_SIZE];
    char                        VID[VID_SIZE];
}PACKED sprom_common_block_t;

#define SPROM_COMMON_BLOCK_OFFSET     0
#define SPROM_COMMON_BLOCK_DATA_SIZE  sizeof(sprom_common_block_t)
#define SPROM_COMMON_BLOCK_SIG        0xABAB
#define SPROM_COMMON_BLOCK_CUR_VER    3
#define SPROM_CUR_EEPROM_SIZE         65535
#define SPROM_COMMON_BLOCK_OEM_STRING "Cisco Systems, Inc."

/*
** *******************************************************************
** Power Supply Block definitions
**
** The DC3 Power Supply SPROM consists of 2 blocks:
**
** 1. Common Block
** 2. Power Supply Block
**
** *******************************************************************
*/

/* Entire Power Supply SPROM contents: */
typedef struct power_supply_sprom_fex_rev2_
{
    sprom_block_sig_t		block_signature;
    uint8_t			block_version;
    uint8_t			block_length;
    uint16_t			block_checksum;
    uint16_t                    sprom_size;
    uint16_t                    block_count;
    uint16_t                    fru_major_t;
    uint16_t                    fru_minor_t;
    char                        OEM_string[OEM_STRING_SIZE];
    char                        product_number[PRODUCT_NUMBER_SIZE];
    char                        serial_number[SERIAL_NUMBER_SIZE];
    char                        part_number[PART_NUMBER_SIZE];
    char                        part_revision[PART_REVISION_SIZE];
    char                        mfg_deviation[MFG_DEVIATION_SIZE];
    uint16_t                    hw_rev_major;
    uint16_t                    hw_rev_minor;
    short			power_consumption;
    uint8_t                     RMA_failure_code[RMA_FAILURE_CODE_SIZE];
    char                       	fault_byte; 
    char                       	fan_speed_byte; 
    char                       	temp_byte; 
    char                       	null_char; 
}PACKED power_supply_sprom_fex_rev2_t;

#define FEX_PS_PRODUCT_NUMBER_SIZE	15
#define FEX_PS_SERIAL_NUMBER_SIZE	17
#define FEX_PS_PART_NUMBER_SIZE		11
#define FEX_PS_CLEI_SIZE		12
#define FEX_PS_VID_SIZE			5

typedef struct power_supply_sprom_fex_
{
    sprom_block_sig_t		block_signature;
    uint8_t			block_version;
    uint8_t			block_length;
    uint16_t			block_checksum;
    uint16_t                    sprom_size;
    uint16_t                    block_count;
    uint16_t                    fru_major_t;
    uint16_t                    fru_minor_t;
    char                        OEM_string[OEM_STRING_SIZE];
    char                        product_number[FEX_PS_PRODUCT_NUMBER_SIZE];
    char                        serial_number[FEX_PS_SERIAL_NUMBER_SIZE];
    char                        part_number[FEX_PS_PART_NUMBER_SIZE];
    char                        part_revision[PART_REVISION_SIZE];
    char                        clei[FEX_PS_CLEI_SIZE];
    char                        version_id[FEX_PS_VID_SIZE];
    uint16_t                    snmpOID[SNMP_OID_SIZE];
    uint16_t                    hw_rev_major;
    uint16_t                    hw_rev_minor;
    short			power_consumption;
    uint8_t                     RMA_failure_code[RMA_FAILURE_CODE_SIZE];
    char                       	fault_byte; 
    char                       	fan_speed_byte; 
    char                       	temp_byte; 
    char                       	null_char; 
}PACKED power_supply_sprom_fex_t;

/* Entire Power Supply SPROM contents: */
typedef struct power_supply_common_block_n2k_s_
{
    // Common Block.
    sprom_block_sig_t		block_signature;
    uint8_t			block_version;
    uint8_t			block_length;
    uint16_t			block_checksum;
    uint16_t                    sprom_size;
    uint16_t                    block_count;
    uint16_t                    fru_major_t;
    uint16_t                    fru_minor_t;
    char                        OEM_string[OEM_STRING_SIZE];
    char                        product_number[PRODUCT_NUMBER_SIZE];
    char                        serial_number[SERIAL_NUMBER_SIZE];
    char                        part_number[PART_NUMBER_SIZE];
    char                        part_revision[PART_REVISION_SIZE];
    char                        mfg_deviation[MFG_DEVIATION_SIZE];
    uint16_t                    hw_rev_major;
    uint16_t                    hw_rev_minor;
    uint16_t			mfg_bits;
    uint16_t			eng_bits;
    uint16_t			snmpOID[SNMP_OID_SIZE];
    short			power_consumption;
    uint8_t                     RMA_failure_code[RMA_FAILURE_CODE_SIZE];
    char                        CLEI_code[CLEI_CODE_SIZE];
    char                        VID[VID_SIZE];
}PACKED sprom_common_block_n2k_t;
   
typedef struct sprom_power_supply_block_s_
{
    // Power Supply Specific Block 
    sprom_block_sig_t		block_signature;
    uint8_t			block_version;
    uint8_t			block_length;
    uint16_t			block_checksum;
    uint64_t                    feature_bits;
    uint16_t                    current_110v;
    uint16_t                    current_220v;
    uint16_t                    stackmib_oid;
    uint16_t                    current_mode1;
    uint16_t                    current_mode2;
    uint16_t                    current_mode3;
    uint16_t                    current_mode4;
    uint16_t                    max_float_current_mode1;
    uint16_t                    max_float_current_mode2;
    uint16_t                    max_float_current_mode3;
    uint16_t                    max_float_current_mode4;
}PACKED sprom_power_supply_block_t;

typedef struct power_supply_sprom_n2k_s_
{
    sprom_common_block_n2k_t        	common_block;
    sprom_power_supply_block_t  	power_supply_block;
}PACKED power_supply_sprom_t;

// Structure is 128 bytes. But for some reason the last
// 4 bytes are not included in checksum :)
#define SPROM_POWER_SUPPLY_FEX_DATA_SIZE       124 
#define SPROM_POWER_SUPPLY_FEX_NUM_BLOCKS      1

#define SPROM_POWER_SUPPLY_BLOCK_OFFSET    sizeof(sprom_common_block_n2k_t)
#define SPROM_POWER_SUPPLY_BLOCK_DATA_SIZE sizeof(sprom_power_supply_block_t)
#define SPROM_POWER_SUPPLY_BLOCK_SIG_1     0xAB01
#define SPROM_POWER_SUPPLY_BLOCK_SIG       0xABAB
#define SPROM_POWER_SUPPLY_DATA_SIZE       sizeof(power_supply_sprom_t)

#define SPROM_POWER_SUPPLY_BLOCK_FEX_VER   3
#define SPROM_POWER_SUPPLY_BLOCK_CUR_VER   2
#define SPROM_POWER_SUPPLY_NUM_BLOCKS      2

/*
** *******************************************************************
** Chassis Backplane Block definitions
**
** The DC3 backplane SPROM consists of 4 blocks:
**
** 1. Common Block
** 2. Backplane Block
** 3. WWN Block - World Wide Name used for Fibre Channel
** 4. License Block
** 5. 2nd Serial Number Block
**
** *******************************************************************
*/
typedef struct sprom_backplane_block_
{
    sprom_block_header_t        block_header;
    uint64_t                      feature_bits;
    uint64_t                      hw_changes_bits;
    uint16_t                     stackmib_oid;
    uint8_t                       mac_base[MAC_ADDR_SIZE];
    uint16_t                     mac_length;
    uint16_t                     OEM_Enterprise;
    uint16_t                     OEM_MIB_Offset;
    uint16_t                     max_connector_power;
    uint8_t                       cooling_coefficient;
}PACKED sprom_backplane_block_t;

/*
** *******************************************************************
** WWN Block definition, part of the Chassis Backplane SPROM
** *******************************************************************
*/
#define WWN_USAGE_SIZE          250
typedef struct sprom_wwn_block_
{
    sprom_block_header_t        block_header;
    uint8_t                       wwn_usage_bits[WWN_USAGE_SIZE];
}PACKED sprom_wwn_block_t;

/*
** *******************************************************************
** License Block definition, part of the Chassis Backplane SPROM
** *******************************************************************
*/
#define LIC_USAGE_SIZE          8
typedef struct sprom_lic_block_
{
    sprom_block_header_t        block_header;
    uint8_t                       lic_usage_bits[LIC_USAGE_SIZE];
    uint8_t                       pad[2];
} PACKED sprom_lic_block_t;

/*
** *******************************************************************
** 2nd Serial Number Block definition, part of the Chassis Backplane SPROM
** *******************************************************************
*/
typedef struct sprom_sn2_block_
{
    sprom_block_header_t        block_header;
    char                        serial_number[SERIAL_NUMBER_SIZE];
    uint8_t                       pad[2];
} PACKED sprom_sn2_block_t;

/* Entire Backplane SPROM Contents: */
typedef struct backplane_sprom_
{
    sprom_common_block_t        common_block;
    sprom_backplane_block_t     backplane_block;
    sprom_wwn_block_t           wwn_block;
    sprom_lic_block_t           lic_block;
    sprom_sn2_block_t           sn2_block;
}PACKED backplane_sprom_t;

#define SPROM_BACKPLANE_BLOCK_DATA_SIZE sizeof(sprom_backplane_block_t)

/*
** The WWN block is a little strange.  It's size is 256 bytes, however
** we only have a uint8_t to store the block size in the header.  The
** convention is to treat a block size of 0 as 256.  This affects the
** WWN block display and validation routines.
*/

#define SPROM_WWN_BLOCK_DATA_SIZE       sizeof(sprom_wwn_block_t)
#define SPROM_LIC_BLOCK_DATA_SIZE       sizeof(sprom_lic_block_t)
#define SPROM_SN2_BLOCK_DATA_SIZE       sizeof(sprom_sn2_block_t)
#define SPROM_BACKPLANE_DATA_SIZE       sizeof(backplane_sprom_t)

#define SPROM_BACKPLANE_BLOCK_OFFSET    SPROM_COMMON_BLOCK_DATA_SIZE
#define SPROM_WWN_BLOCK_OFFSET \
        (SPROM_BACKPLANE_BLOCK_OFFSET+SPROM_BACKPLANE_BLOCK_DATA_SIZE)
#define SPROM_LIC_BLOCK_OFFSET \
        (SPROM_WWN_BLOCK_OFFSET+SPROM_WWN_BLOCK_DATA_SIZE)
#define SPROM_SN2_BLOCK_OFFSET \
        (SPROM_LIC_BLOCK_OFFSET+SPROM_LIC_BLOCK_DATA_SIZE)

#define SPROM_BACKPLANE_BLOCK_SIG     0x6001
#define SPROM_BACKPLANE_BLOCK_CUR_VER 3
#define SPROM_BACKPLANE_NUM_BLOCKS    5

#define SPROM_WWN_BLOCK_SIG           0x6005
#define SPROM_WWN_BLOCK_CUR_VER       1
#define SPROM_LIC_BLOCK_SIG           0x6006
#define SPROM_LIC_BLOCK_CUR_VER       1
#define SPROM_SN2_BLOCK_SIG           0x6007
#define SPROM_SN2_BLOCK_CUR_VER       1

/*
** *******************************************************************
** Supervisor SPROM definitions
**
** The DC3 supervisor SPROM consists of 3 blocks:
**
** 1. Common Block
** 2. Supervisor Block
** 3. Temp Sensor Block
**
** *******************************************************************
*/

/*
** Structure to hold port data
*/
typedef struct sprom_port_data_
{
    pm_phy_port_layer_t      type_of_port;
    port_num_t               num_ports;
}PACKED sprom_port_data_t;

/*
** Structure to hold temperature sensor data
*/
typedef struct sprom_sensor_data_
{
    int8_t       major_threshold;
    int8_t       minor_threshold;
}PACKED sprom_sensor_data_t;

#define EPLD_VERSIONS_SIZE      30
#define MAX_EPLD_VERSIONS       EPLD_VERSIONS_SIZE
#define MAX_PORT_TYPES          8
#define MAX_CARD_SENSORS        8

typedef struct sprom_sup_block_
{
    sprom_block_header_t        block_header;
    uint64_t                      feature_bits;
    uint64_t                      hw_changes_bits;
    uint16_t                     card_index;
    uint8_t                       mac_base[MAC_ADDR_SIZE];
    uint16_t                     mac_length;
    uint8_t                       eobc_connections;
    uint8_t                       epld_num;
    uint8_t                       epld_versions[EPLD_VERSIONS_SIZE];
    sprom_port_data_t           port_data[MAX_PORT_TYPES];
    uint16_t                     sram_size;  // in kilobytes
    sprom_sensor_data_t         sensor_data[MAX_CARD_SENSORS];
    uint16_t                     max_connector_power;
    uint16_t                     cooling_requirement;
    char                        ambient_temperature;
}PACKED sprom_sup_block_t;

/*
** *******************************************************************
** Temperature Sensor Block definition, part of the Supervisor SPROM
** *******************************************************************
*/
#define MAX_TEMP_SENSORS        32
typedef struct sprom_temp_sensor_block_
{
    sprom_block_header_t        block_header;
    uint8_t                       num_sensors;
    sprom_sensor_data_t         sensor_data[MAX_TEMP_SENSORS];
}PACKED sprom_temp_sensor_block_t;

/*
** *******************************************************************
** SPD Block definition, part of the CMP SPROM
** *******************************************************************
*/
#define MAX_SPD_PARAMS          16
typedef struct sprom_spd_block_
{
    sprom_block_header_t        block_header;
    uint32_t                    spd_data[MAX_SPD_PARAMS];
}PACKED sprom_spd_block_t;

/* Entire Sup SPROM contents: */
typedef struct sup_sprom_
{
    sprom_common_block_t       common_block;
    sprom_sup_block_t          sup_block;
    sprom_temp_sensor_block_t  sensor_block;
}PACKED sup_sprom_t;

#define SPROM_SUP_BLOCK_DATA_SIZE     sizeof(sprom_sup_block_t)
#define SPROM_SENSOR_BLOCK_DATA_SIZE  sizeof(sprom_temp_sensor_block_t)
#define SPROM_SUP_DATA_SIZE           sizeof(sup_sprom_t)

#define SPROM_SUP_BLOCK_OFFSET        SPROM_COMMON_BLOCK_DATA_SIZE
#define SPROM_SUP_SENSOR_BLOCK_OFFSET \
        (SPROM_SUP_BLOCK_OFFSET+SPROM_SUP_BLOCK_DATA_SIZE)

#define SPROM_SUP_BLOCK_SIG           0x6002
#define SPROM_SUP_BLOCK_CUR_VER       2
#define SPROM_SUP_NUM_BLOCKS          3

#define SPROM_SENSOR_BLOCK_SIG        0x6008
#define SPROM_SENSOR_BLOCK_CUR_VER    1

/* Supervisor block feature bit definitions: */
#define SPROM_SUP_BLOCK_XBAR_MODE_BIT   0
#define SPROM_SUP_BLOCK_XBAR_MODE_20G   0
#define SPROM_SUP_BLOCK_XBAR_MODE_8G    1

/*
** *******************************************************************
** CMP SPROM definitions
**
** The DC3 CMP SPROM consists of 4 blocks:
**
** 1. Common Block
** 2. Line card Block
** 3. Temp Sensor Block
** 4. SPD Param Block
**
** *******************************************************************
*/

typedef struct cmp_sprom_
{
    sprom_common_block_t       common_block;
    sprom_sup_block_t          sup_block;
    sprom_temp_sensor_block_t  sensor_block;
    sprom_spd_block_t          spd_block;
}PACKED cmp_sprom_t;

#define SPROM_SPD_BLOCK_DATA_SIZE     sizeof(sprom_spd_block_t)
#define SPROM_CMP_DATA_SIZE           sizeof(cmp_sprom_t)

#define SPROM_SUP_SPD_BLOCK_OFFSET \
        (SPROM_SUP_SENSOR_BLOCK_OFFSET+SPROM_SENSOR_BLOCK_DATA_SIZE)

#define SPROM_CMP_NUM_BLOCKS          4

#define SPROM_SPD_BLOCK_SIG           0x6009
#define SPROM_SPD_BLOCK_CUR_VER       1

/*
** *******************************************************************
** Line Card SPROM definitions
**
** The DC3 line card SPROM consists of 3 blocks:
**
** 1. Common Block
** 2. Line card Block
** 3. Temp Sensor Block
**
** *******************************************************************
*/
typedef struct sprom_line_card_block_
{
    sprom_block_header_t        block_header;
    uint64_t                      feature_bits;
    uint64_t                      hw_changes_bits;
    uint16_t                     card_index;
    uint8_t                       mac_base[MAC_ADDR_SIZE];
    uint16_t                     mac_length;
    uint8_t                       eobc_connections;
    uint8_t                       epld_num;
    uint8_t                       epld_versions[EPLD_VERSIONS_SIZE];
    sprom_port_data_t           port_data[MAX_PORT_TYPES];
    uint16_t                     sram_size; // in kilobytes
    sprom_sensor_data_t         sensor_data[MAX_CARD_SENSORS];
    uint16_t                     max_connector_power;
    uint16_t                     cooling_requirement;
    char                        ambient_temperature;
}PACKED sprom_line_card_block_t;

/* Entire Line Card SPROM contents: */
typedef struct line_card_sprom_
{
    sprom_common_block_t       common_block;
    sprom_line_card_block_t    lc_block;
    sprom_temp_sensor_block_t  sensor_block;
}PACKED line_card_sprom_t;

#define SPROM_LINE_CARD_BLOCK_DATA_SIZE sizeof(sprom_line_card_block_t)
#define SPROM_LINE_CARD_DATA_SIZE       sizeof(line_card_sprom_t)

#define SPROM_LINE_CARD_BLOCK_OFFSET SPROM_COMMON_BLOCK_DATA_SIZE
#define SPROM_LINE_CARD_SENSOR_BLOCK_OFFSET \
        (SPROM_LINE_CARD_BLOCK_OFFSET+SPROM_LINE_CARD_BLOCK_DATA_SIZE)

#define SPROM_LINE_CARD_BLOCK_SIG           0x6003
#define SPROM_LINE_CARD_BLOCK_CUR_VER       2
#define SPROM_LINE_CARD_NUM_BLOCKS          3

/* Line Card block feature bit definitions: */
#define SPROM_LINE_CARD_BLOCK_XBAR_MODE_BIT     0
#define SPROM_LINE_CARD_BLOCK_XBAR_MODE_20G     0
#define SPROM_LINE_CARD_BLOCK_XBAR_MODE_8G      1

/*
** *******************************************************************
** Spine Card SPROM definitions
**
** The DC3 spine card SPROM is the same as the line card format
**
** *******************************************************************
*/

/*
** *******************************************************************
** EARL8 Daughter Card SPROM definitions
**
** The DC3 EARL8 Daughter Card SPROM is the same as the line card format
**
** *******************************************************************
*/

/*****************************************************************************
 * Clock block type:
 */
typedef struct sprom_clock_block_
{
    sprom_block_header_t        block_header;
    uint64_t                      feature_bits;
    uint64_t                      hw_changes_bits;
    uint16_t                     stackmib_oid;
}PACKED sprom_clock_block_t;

/* Entire Clock SPROM contents: */
typedef struct clock_sprom_
{
    sprom_common_block_t        common_block;
    sprom_clock_block_t         clock_block;
}PACKED clock_sprom_t;

#define SPROM_CLOCK_BLOCK_DATA_SIZE sizeof(sprom_clock_block_t)
#define SPROM_CLOCK_DATA_SIZE       sizeof(clock_sprom_t)

#define SPROM_CLOCK_BLOCK_OFFSET    SPROM_COMMON_BLOCK_DATA_SIZE

#define SPROM_CLOCK_BLOCK_SIG       0xAB03
#define SPROM_CLOCK_BLOCK_CUR_VER   1
#define SPROM_CLOCK_NUM_BLOCKS      2

/*
** *******************************************************************
** Fan SPROM definitions
**
** The DC3 Chassis Fan SPROM consists of 2 blocks:
**
** 1. Common Block
** 2. Fan Block
** 3. 2nd Serial Number Block
**
** *******************************************************************
*/
typedef struct sprom_fan_block_
{
    sprom_block_header_t        block_header;
    uint64_t                      feature_bits;
    uint64_t                      hw_changes_bits;
    uint16_t                     stackmib_oid;
    uint16_t                     cooling_capacity;
    char                         ambient_temperature;
    uint8_t                      reserved;
    short               	 power_consumption2;
    uint16_t                     cooling_capacity2;
}PACKED sprom_fan_block_t;

/* Entire Fan SPROM contents: */
typedef struct fan_sprom_
{
    sprom_common_block_t        common_block;
    sprom_fan_block_t           fan_block;
    sprom_sn2_block_t           sn2_block;
}PACKED fan_sprom_t;

#define SPROM_FAN_BLOCK_OFFSET SPROM_COMMON_BLOCK_DATA_SIZE
#define SPROM_FAN_BLOCK_DATA_SIZE sizeof(sprom_fan_block_t)
#define FAN_SPROM_DATA_SIZE sizeof(fan_sprom_t)

#define SPROM_FAN_BLOCK_SIG         0xAB04
#define SPROM_FAN_BLOCK_CUR_VER     3
#define SPROM_FAN_NUM_BLOCKS        3

/********************************************************************/

/* Other defines: */

#define CARD_CTRL_MAIN_BOARD_SPROM_INSTANCE               0
#define SPROM_INSTANCE_0                                  0
#define SPROM_INSTANCE_1                                  1
#define SPROM_INSTANCE_2                                  2
#define SPROM_INSTANCE_3                                  3
#define CARD_CTRL_CPU_DCARD_SPROM_INSTANCE                SPROM_INSTANCE_1
#define CARD_CTRL_MIRAGE_OR_OASIS_DCARD_SPROM_INSTANCE    SPROM_INSTANCE_2
#define CARD_CTRL_CASA_SUPER20_SPROM_INSTANCE             SPROM_INSTANCE_3

#endif
