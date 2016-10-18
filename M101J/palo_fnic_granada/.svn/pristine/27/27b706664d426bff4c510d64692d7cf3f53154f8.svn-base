/********************************************************************
 *
 *      File:   sprom_endian.c
 *      Name:   Curt Brune, matt strathman
 *
 *       Description:
 *       SPROM endian conversion routines
 *
 * Copyright (c) 1985-2002, 2003, 2004 by Cisco Systems, Inc.
 * All rights reserved.
 *
 *
 * ---------------------------------------------------------------
 *********************************************************************
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>

#include "sprom.h"
#include "diag_sprom_defs.h"
#include <asm/byteorder.h>

static int
hton_sprom_block_header_t (sprom_block_header_t *blk_hdr)
{
    int err;

    if (blk_hdr == NULL) {
        err = -1;
        goto cu0;
    }

    err = 0;
    blk_hdr->block_signature = htons(blk_hdr->block_signature);
    blk_hdr->block_checksum  = htons(blk_hdr->block_checksum);

cu0:
    return err;
}

static int
hton_snmpOID (uint16_t *snmpOID, int snmpOID_size)
{
    int err;
    int i;

    if (snmpOID == NULL || snmpOID_size == 0) {
        err = -1;
        goto cu0;
    }

    err = 0;
    i = -1;
    while (++i < snmpOID_size) {
        snmpOID[i] = htons(snmpOID[i]);
    }

cu0:
    return err;
}

static int
hton_sprom_common_block_t (sprom_common_block_t *cmn_blk)
{
    int err;

    if (cmn_blk == NULL) {
        err = -1;
        goto cu0;
    }

    err = 0;
    hton_sprom_block_header_t(&cmn_blk->block_header);
    cmn_blk->sprom_size        = htons(cmn_blk->sprom_size);
    cmn_blk->block_count       = htons(cmn_blk->block_count);
    cmn_blk->fru_major_t       = htons(cmn_blk->fru_major_t);
    cmn_blk->fru_minor_t       = htons(cmn_blk->fru_minor_t);
    cmn_blk->hw_rev_major      = htons(cmn_blk->hw_rev_major);
    cmn_blk->hw_rev_minor      = htons(cmn_blk->hw_rev_minor);
    cmn_blk->mfg_bits          = htons(cmn_blk->mfg_bits);
    cmn_blk->eng_bits          = htons(cmn_blk->eng_bits);
    hton_snmpOID(cmn_blk->snmpOID, SNMP_OID_SIZE);
    cmn_blk->power_consumption = htons(cmn_blk->power_consumption);

cu0:
    return err;
}

static int
hton_sprom_common_block_n2k_t (sprom_common_block_n2k_t *cmn_blk)
{
    int err;

    if (cmn_blk == NULL) {
        err = -1;
        goto cu0;
    }

    err = 0;
    hton_sprom_block_header_t( (sprom_block_header_t*)cmn_blk);
    cmn_blk->sprom_size        = htons(cmn_blk->sprom_size);
    cmn_blk->block_count       = htons(cmn_blk->block_count);
    cmn_blk->fru_major_t       = htons(cmn_blk->fru_major_t);
    cmn_blk->fru_minor_t       = htons(cmn_blk->fru_minor_t);
    cmn_blk->hw_rev_major      = htons(cmn_blk->hw_rev_major);
    cmn_blk->hw_rev_minor      = htons(cmn_blk->hw_rev_minor);
    cmn_blk->mfg_bits          = htons(cmn_blk->mfg_bits);
    cmn_blk->eng_bits          = htons(cmn_blk->eng_bits);
    hton_snmpOID(cmn_blk->snmpOID, SNMP_OID_SIZE);
    cmn_blk->power_consumption = htons(cmn_blk->power_consumption);

cu0:
    return err;
}

static int
hton_sprom_backplane_block_t (sprom_backplane_block_t *bp_blk)
{
    int err;

    if (bp_blk == NULL) {
        err = -1;
        goto cu0;
    }

    err = 0;
    hton_sprom_block_header_t(&bp_blk->block_header);
    bp_blk->feature_bits        = __cpu_to_be64(bp_blk->feature_bits);
    bp_blk->hw_changes_bits     = __cpu_to_be64(bp_blk->hw_changes_bits);
    bp_blk->stackmib_oid        = htons(bp_blk->stackmib_oid);
    bp_blk->mac_length          = htons(bp_blk->mac_length);
    bp_blk->OEM_Enterprise      = htons(bp_blk->OEM_Enterprise);
    bp_blk->OEM_MIB_Offset      = htons(bp_blk->OEM_MIB_Offset);
    bp_blk->max_connector_power = htons(bp_blk->max_connector_power);

cu0:
    return err;
}

static int
hton_sprom_wwn_block_t (sprom_wwn_block_t *wwn_blk)
{
    int err;

    if (wwn_blk == NULL) {
        err = -1;
        goto cu0;
    }

    err = 0;
    hton_sprom_block_header_t(&wwn_blk->block_header);

cu0:
    return err;
}

static int
hton_sprom_lic_block_t (sprom_lic_block_t *lic_blk)
{
    int err;

    if (lic_blk == NULL) {
        err = -1;
        goto cu0;
    }

    err = 0;
    hton_sprom_block_header_t(&lic_blk->block_header);

cu0:
    return err;
}

static int
hton_sprom_sn2_block_t (sprom_sn2_block_t *sn2_blk)
{
    int err;

    if (sn2_blk == NULL) {
        err = -1;
        goto cu0;
    }

    err = 0;
    hton_sprom_block_header_t(&sn2_blk->block_header);

cu0:
    return err;
}

static int
hton_sprom_temp_sensor_block_t (sprom_temp_sensor_block_t *sensor_blk)
{
    int err;

    if (sensor_blk == NULL) {
        err = -1;
        goto cu0;
    }

    err = 0;
    hton_sprom_block_header_t(&sensor_blk->block_header);

cu0:
    return err;
}

static int
hton_sprom_sup_block_t (sprom_sup_block_t *sup_blk)
{
    int err;

    if (sup_blk == NULL) {
        err = -1;
        goto cu0;
    }

    err = 0;
    hton_sprom_block_header_t(&sup_blk->block_header);
    sup_blk->feature_bits        = __cpu_to_be64(sup_blk->feature_bits);
    sup_blk->hw_changes_bits     = __cpu_to_be64(sup_blk->hw_changes_bits);
    sup_blk->card_index          = htons(sup_blk->card_index);
    sup_blk->mac_length          = htons(sup_blk->mac_length);
    sup_blk->sram_size           = htons(sup_blk->sram_size);
    sup_blk->max_connector_power = htons(sup_blk->max_connector_power);
    sup_blk->cooling_requirement = htons(sup_blk->cooling_requirement);

cu0:
    return err;
}

static int
hton_sprom_line_card_block_t (sprom_line_card_block_t *lc_blk)
{
    int err;

    if (lc_blk == NULL) {
        err = -1;
        goto cu0;
    }

    err = 0;
    hton_sprom_block_header_t(&lc_blk->block_header);
    lc_blk->feature_bits        = __cpu_to_be64(lc_blk->feature_bits);
    lc_blk->hw_changes_bits     = __cpu_to_be64(lc_blk->hw_changes_bits);
    lc_blk->card_index          = htons(lc_blk->card_index);
    lc_blk->mac_length          = htons(lc_blk->mac_length);
    lc_blk->sram_size           = htons(lc_blk->sram_size);
    lc_blk->max_connector_power = htons(lc_blk->max_connector_power);
    lc_blk->cooling_requirement = htons(lc_blk->cooling_requirement);

cu0:
    return err;
}

static int
hton_sprom_fan_block_t (sprom_fan_block_t *fan_blk)
{
    int err;

    if (fan_blk == NULL) {
        err = -1;
        goto cu0;
    }

    err = 0;
    hton_sprom_block_header_t(&fan_blk->block_header);
    fan_blk->feature_bits       = __cpu_to_be64(fan_blk->feature_bits);
    fan_blk->hw_changes_bits    = __cpu_to_be64(fan_blk->hw_changes_bits);
    fan_blk->stackmib_oid       = htons(fan_blk->stackmib_oid);
    fan_blk->cooling_capacity   = htons(fan_blk->cooling_capacity);
    //fan_blk->power_consumption2 = htons(fan_blk->power_consumption2);
    //fan_blk->cooling_capacity2  = htons(fan_blk->cooling_capacity2);

cu0:
    return err;
}

static int
hton_sprom_power_supply_block_t (sprom_power_supply_block_t *ps_blk)
{
    int err;

    if (ps_blk == NULL) {
        err = -1;
        goto cu0;
    }

    err = 0;
    hton_sprom_block_header_t( (sprom_block_header_t*)ps_blk);
    ps_blk->feature_bits = __cpu_to_be64(ps_blk->feature_bits);
    ps_blk->current_110v = htons(ps_blk->current_110v);
    ps_blk->current_220v = htons(ps_blk->current_220v);
    ps_blk->stackmib_oid = htons(ps_blk->stackmib_oid);
    ps_blk->current_mode1 = htons(ps_blk->current_mode1);
    ps_blk->current_mode2 = htons(ps_blk->current_mode2);
    ps_blk->current_mode3 = htons(ps_blk->current_mode3);
    ps_blk->current_mode4 = htons(ps_blk->current_mode4);
    ps_blk->max_float_current_mode1 = htons(ps_blk->max_float_current_mode1);
    ps_blk->max_float_current_mode2 = htons(ps_blk->max_float_current_mode2);
    ps_blk->max_float_current_mode3 = htons(ps_blk->max_float_current_mode3);
    ps_blk->max_float_current_mode4 = htons(ps_blk->max_float_current_mode4);

cu0:
    return err;
}

static int
hton_power_supply_sprom_t (power_supply_sprom_t *ps_sprom)
{
    int err;

    if (ps_sprom == NULL) {
        err = -1;
        goto cu0;
    }

    err = 0;
    if ((err = hton_sprom_common_block_n2k_t(&ps_sprom->common_block)) < 0) {
        goto cu0;
    }

    if ((err = hton_sprom_power_supply_block_t(&ps_sprom->power_supply_block))
         < 0) {
        goto cu0;
    }

cu0:
    return err;
}

static int
hton_sup_sprom_t (sup_sprom_t *sup_sprom)
{
    int err;

    if (sup_sprom == NULL) {
        err = -1;
        goto cu0;
    }

    err = 0;
    if ((err = hton_sprom_common_block_t(&sup_sprom->common_block)) < 0) {
        goto cu0;
    }

    if ((err = hton_sprom_sup_block_t(&sup_sprom->sup_block)) < 0) {
        goto cu0;
    }

    if ((err = hton_sprom_temp_sensor_block_t(&sup_sprom->sensor_block)) < 0) {
        goto cu0;
    }

cu0:
    return err;
}

static int
hton_line_card_sprom_t (line_card_sprom_t *lc_sprom)
{
    int err;

    if (lc_sprom == NULL) {
        err = -1;
        goto cu0;
    }

    err = 0;
    if ((err = hton_sprom_common_block_t(&lc_sprom->common_block)) < 0) {
        goto cu0;
    }

    if ((err = hton_sprom_line_card_block_t(&lc_sprom->lc_block)) < 0) {
        goto cu0;
    }

    //if ((err = hton_sprom_temp_sensor_block_t(&lc_sprom->sensor_block)) < 0) {
    //    goto cu0;
    //}

cu0:
    return err;
}

static int
hton_backplane_sprom_t (backplane_sprom_t *bp_sprom)
{
    int err;

    if (bp_sprom == NULL) {
        err = -1;
        goto cu0;
    }

    if ((err = hton_sprom_common_block_t(&bp_sprom->common_block)) < 0) {
        goto cu0;
    }

    if ((err = hton_sprom_backplane_block_t(&bp_sprom->backplane_block)) < 0) {
        goto cu0;
    }

    if ((err = hton_sprom_wwn_block_t(&bp_sprom->wwn_block)) < 0) {
        goto cu0;
    }

    if ((err = hton_sprom_lic_block_t(&bp_sprom->lic_block)) < 0) {
        goto cu0;
    }

    if ((err = hton_sprom_sn2_block_t(&bp_sprom->sn2_block)) < 0) {
        goto cu0;
    }

    err = 0;

cu0:
    return err;
}

static int
hton_fan_sprom_t (fan_sprom_t *fan_sprom)
{
    int err;

    if (fan_sprom == NULL) {
        err = -1;
        goto cu0;
    }

    if ((err = hton_sprom_common_block_t(&fan_sprom->common_block)) < 0) {
        goto cu0;
    }

    if ((err = hton_sprom_fan_block_t(&fan_sprom->fan_block)) < 0) {
        goto cu0;
    }

    //if ((err = hton_sprom_sn2_block_t(&fan_sprom->sn2_block)) < 0) {
    //    goto cu0;
    //}

    err = 0;

cu0:
    return err;
}

static int
ntoh_sprom_block_header_t (sprom_block_header_t *blk_hdr)
{
    int err;

    if (blk_hdr == NULL) {
        err = -1;
        goto cu0;
    }

    err = 0;
    blk_hdr->block_signature = ntohs(blk_hdr->block_signature);
    blk_hdr->block_checksum  = ntohs(blk_hdr->block_checksum);

cu0:
    return err;
}

static int
ntoh_snmpOID(uint16_t *snmpOID, int snmpOID_size)
{
    int err;
    int i;

    if (snmpOID == NULL || snmpOID_size == 0) {
        err = -1;
        goto cu0;
    }

    err = 0;
    i = -1;
    while (++i < snmpOID_size) {
        snmpOID[i] = ntohs(snmpOID[i]);
    }

cu0:
    return err;
}

static int
ntoh_sprom_common_block_t (sprom_common_block_t *cmn_blk)
{
    int err;

    if (cmn_blk == NULL) {
        err = -1;
        goto cu0;
    }

    err = 0;
    ntoh_sprom_block_header_t(&cmn_blk->block_header);
    cmn_blk->sprom_size        = ntohs(cmn_blk->sprom_size);
    cmn_blk->block_count       = ntohs(cmn_blk->block_count);
    cmn_blk->fru_major_t       = ntohs(cmn_blk->fru_major_t);
    cmn_blk->fru_minor_t       = ntohs(cmn_blk->fru_minor_t);
    cmn_blk->hw_rev_major      = ntohs(cmn_blk->hw_rev_major);
    cmn_blk->hw_rev_minor      = ntohs(cmn_blk->hw_rev_minor);
    cmn_blk->mfg_bits          = ntohs(cmn_blk->mfg_bits);
    cmn_blk->eng_bits          = ntohs(cmn_blk->eng_bits);
    ntoh_snmpOID(cmn_blk->snmpOID, SNMP_OID_SIZE);
    cmn_blk->power_consumption = ntohs(cmn_blk->power_consumption);

cu0:
    return err;
}

static int
ntoh_sprom_common_block_n2k_t (sprom_common_block_n2k_t *cmn_blk)
{
    int err;

    if (cmn_blk == NULL) {
        err = -1;
        goto cu0;
    }

    err = 0;
    ntoh_sprom_block_header_t((sprom_block_header_t*)cmn_blk);
    cmn_blk->sprom_size        = ntohs(cmn_blk->sprom_size);
    cmn_blk->block_count       = ntohs(cmn_blk->block_count);
    cmn_blk->fru_major_t       = ntohs(cmn_blk->fru_major_t);
    cmn_blk->fru_minor_t       = ntohs(cmn_blk->fru_minor_t);
    cmn_blk->hw_rev_major      = ntohs(cmn_blk->hw_rev_major);
    cmn_blk->hw_rev_minor      = ntohs(cmn_blk->hw_rev_minor);
    cmn_blk->mfg_bits          = ntohs(cmn_blk->mfg_bits);
    cmn_blk->eng_bits          = ntohs(cmn_blk->eng_bits);
    ntoh_snmpOID(cmn_blk->snmpOID, SNMP_OID_SIZE);
    cmn_blk->power_consumption = ntohs(cmn_blk->power_consumption);

cu0:
    return err;
}

static int
ntoh_sprom_backplane_block_t (sprom_backplane_block_t *bp_blk)
{
    int err;

    if (bp_blk == NULL) {
        err = -1;
        goto cu0;
    }

    err = 0;
    ntoh_sprom_block_header_t(&bp_blk->block_header);
    bp_blk->feature_bits        = __be64_to_cpu(bp_blk->feature_bits);
    bp_blk->hw_changes_bits     = __be64_to_cpu(bp_blk->hw_changes_bits);
    bp_blk->stackmib_oid        = ntohs(bp_blk->stackmib_oid);
    bp_blk->mac_length          = ntohs(bp_blk->mac_length);
    bp_blk->OEM_Enterprise      = ntohs(bp_blk->OEM_Enterprise);
    bp_blk->OEM_MIB_Offset      = ntohs(bp_blk->OEM_MIB_Offset);
    bp_blk->max_connector_power = ntohs(bp_blk->max_connector_power);

cu0:
    return err;
}

static int
ntoh_sprom_wwn_block_t (sprom_wwn_block_t *wwn_blk)
{
    int err;

    if (wwn_blk == NULL) {
        err = -1;
        goto cu0;
    }

    err = 0;
    ntoh_sprom_block_header_t(&wwn_blk->block_header);

cu0:
    return err;
}

static int
ntoh_sprom_lic_block_t (sprom_lic_block_t *lic_blk)
{
    int err;

    if (lic_blk == NULL) {
        err = -1;
        goto cu0;
    }

    err = 0;
    ntoh_sprom_block_header_t(&lic_blk->block_header);

cu0:
    return err;
}


static int
ntoh_sprom_sn2_block_t (sprom_sn2_block_t *sn2_blk)
{
    int err;

    if (sn2_blk == NULL) {
        err = -1;
        goto cu0;
    }

    err = 0;
    ntoh_sprom_block_header_t(&sn2_blk->block_header);

cu0:
    return err;
}


static int
ntoh_sprom_temp_sensor_block_t (sprom_temp_sensor_block_t *sensor_blk)
{
    int err;

    if (sensor_blk == NULL) {
        err = -1;
        goto cu0;
    }

    err = 0;
    ntoh_sprom_block_header_t(&sensor_blk->block_header);

cu0:
    return err;
}

static int
ntoh_sprom_sup_block_t (sprom_sup_block_t *sup_blk)
{
    int err;

    if (sup_blk == NULL) {
        err = -1;
        goto cu0;
    }

    err = 0;
    ntoh_sprom_block_header_t(&sup_blk->block_header);
    sup_blk->feature_bits        = __be64_to_cpu(sup_blk->feature_bits);
    sup_blk->hw_changes_bits     = __be64_to_cpu(sup_blk->hw_changes_bits);
    sup_blk->card_index          = ntohs(sup_blk->card_index);
    sup_blk->mac_length          = ntohs(sup_blk->mac_length);
    sup_blk->sram_size           = ntohs(sup_blk->sram_size);
    sup_blk->max_connector_power = ntohs(sup_blk->max_connector_power);
    sup_blk->cooling_requirement = ntohs(sup_blk->cooling_requirement);

cu0:
    return err;
}

static int
ntoh_sprom_line_card_block_t (sprom_line_card_block_t *lc_blk)
{
    int err;

    if (lc_blk == NULL) {
        err = -1;
        goto cu0;
    }

    err = 0;
    ntoh_sprom_block_header_t(&lc_blk->block_header);
    lc_blk->feature_bits        = __be64_to_cpu(lc_blk->feature_bits);
    lc_blk->hw_changes_bits     = __be64_to_cpu(lc_blk->hw_changes_bits);
    lc_blk->card_index          = ntohs(lc_blk->card_index);
    lc_blk->mac_length          = ntohs(lc_blk->mac_length);
    lc_blk->sram_size           = ntohs(lc_blk->sram_size);
    lc_blk->max_connector_power = ntohs(lc_blk->max_connector_power);
    lc_blk->cooling_requirement = ntohs(lc_blk->cooling_requirement);

cu0:
    return err;
}

static int
ntoh_sprom_fan_block_t (sprom_fan_block_t *fan_blk)
{
    int err;

    if (fan_blk == NULL) {
        err = -1;
        goto cu0;
    }

    err = 0;
    ntoh_sprom_block_header_t(&fan_blk->block_header);
    fan_blk->feature_bits       = __be64_to_cpu(fan_blk->feature_bits);
    fan_blk->hw_changes_bits    = __be64_to_cpu(fan_blk->hw_changes_bits);
    fan_blk->stackmib_oid       = ntohs(fan_blk->stackmib_oid);
    fan_blk->cooling_capacity   = ntohs(fan_blk->cooling_capacity);
    //fan_blk->power_consumption2 = ntohs(fan_blk->power_consumption2);
    //fan_blk->cooling_capacity2  = ntohs(fan_blk->cooling_capacity2);

cu0:
    return err;
}

static int
ntoh_sprom_power_supply_block_t (sprom_power_supply_block_t *ps_blk)
{
    int err;

    if (ps_blk == NULL) {
        err = -1;
        goto cu0;
    }

    err = 0;
    ntoh_sprom_block_header_t((sprom_block_header_t*)ps_blk);
    ps_blk->feature_bits = __be64_to_cpu(ps_blk->feature_bits);
    ps_blk->current_110v = ntohs(ps_blk->current_110v);
    ps_blk->current_220v = ntohs(ps_blk->current_220v);
    ps_blk->stackmib_oid = ntohs(ps_blk->stackmib_oid);
    ps_blk->current_mode1 = ntohs(ps_blk->current_mode1);
    ps_blk->current_mode2 = ntohs(ps_blk->current_mode2);
    ps_blk->current_mode3 = ntohs(ps_blk->current_mode3);
    ps_blk->current_mode4 = ntohs(ps_blk->current_mode4);
    ps_blk->max_float_current_mode1 = ntohs(ps_blk->max_float_current_mode1);
    ps_blk->max_float_current_mode2 = ntohs(ps_blk->max_float_current_mode2);
    ps_blk->max_float_current_mode3 = ntohs(ps_blk->max_float_current_mode3);
    ps_blk->max_float_current_mode4 = ntohs(ps_blk->max_float_current_mode4);

cu0:
    return err;
}

static int
ntoh_power_supply_sprom_t (power_supply_sprom_t *ps_sprom)
{
    int err;

    if (ps_sprom == NULL) {
        err = -1;
        goto cu0;
    }

    err = 0;
    if ((err = ntoh_sprom_common_block_n2k_t(&ps_sprom->common_block)) < 0) {
        goto cu0;
    }

    if ((err = ntoh_sprom_power_supply_block_t(&ps_sprom->power_supply_block))
         < 0) {
        goto cu0;
    }

cu0:
    return err;
}

static int
ntoh_sup_sprom_t (sup_sprom_t *sup_sprom)
{
    int err;

    if (sup_sprom == NULL) {
        err = -1;
        goto cu0;
    }

    err = 0;
    if ((err = ntoh_sprom_common_block_t(&sup_sprom->common_block)) < 0) {
        goto cu0;
    }

    if ((err = ntoh_sprom_sup_block_t(&sup_sprom->sup_block)) < 0) {
        goto cu0;
    }

    if ((err = ntoh_sprom_temp_sensor_block_t(&sup_sprom->sensor_block)) < 0) {
        goto cu0;
    }

cu0:
    return err;
}

static int
ntoh_line_card_sprom_t (line_card_sprom_t *lc_sprom)
{
    int err;

    if (lc_sprom == NULL) {
        err = -1;
        goto cu0;
    }

    err = 0;
    if ((err = ntoh_sprom_common_block_t(&lc_sprom->common_block)) < 0) {
        goto cu0;
    }

    if ((err = ntoh_sprom_line_card_block_t(&lc_sprom->lc_block)) < 0) {
        goto cu0;
    }

    //if ((err = ntoh_sprom_temp_sensor_block_t(&lc_sprom->sensor_block)) < 0) {
    //    goto cu0;
    //}

cu0:
    return err;
}

static int
ntoh_backplane_sprom_t (backplane_sprom_t *bp_sprom)
{
    int err;

    if (bp_sprom == NULL) {
        err = -1;
        goto cu0;
    }


    if ((err = ntoh_sprom_common_block_t(&bp_sprom->common_block)) < 0) {
        goto cu0;
    }

    if ((err = ntoh_sprom_backplane_block_t(&bp_sprom->backplane_block)) < 0) {
        goto cu0;
    }

    if ((err = ntoh_sprom_wwn_block_t(&bp_sprom->wwn_block)) < 0) {
        goto cu0;
    }

    if ((err = ntoh_sprom_lic_block_t(&bp_sprom->lic_block)) < 0) {
        goto cu0;
    }

    if ((err = ntoh_sprom_sn2_block_t(&bp_sprom->sn2_block)) < 0) {
        goto cu0;
    }

    err = 0;

cu0:
    return err;
}

static int
ntoh_fan_sprom_t (fan_sprom_t *fan_sprom)
{
    int err;

    if (fan_sprom == NULL) {
        err = -1;
        goto cu0;
    }

    if ((err = ntoh_sprom_common_block_t(&fan_sprom->common_block)) < 0) {
        goto cu0;
    }

    if ((err = ntoh_sprom_fan_block_t(&fan_sprom->fan_block)) < 0) {
        goto cu0;
    }

    //if ((err = ntoh_sprom_sn2_block_t(&fan_sprom->sn2_block)) < 0) {
    //    goto cu0;
    //}

    err = 0;

cu0:
    return err;
}

/*
** Public interface for all SPROMs ntoh
*/
int
sprom_ntoh ( const diag_sprom_t *sprom, uint8_t *buf )
{
    int rc = 0;
    uint32_t size;

    size = sprom_get_size( sprom );

    switch ( sprom->type ) {
        case SPROM_UTIL_TYPE_LC:
            ntoh_line_card_sprom_t((line_card_sprom_t*)buf);
            break;

        case SPROM_UTIL_TYPE_BP:
        case SPROM_UTIL_TYPE_BROCKWAY_UPLINK_BP:
            ntoh_backplane_sprom_t((backplane_sprom_t*)buf);
            break;

        case SPROM_UTIL_TYPE_SUP:
        case SPROM_UTIL_TYPE_BROCKWAY_UPLINK_SUP:
            ntoh_sup_sprom_t((sup_sprom_t*)buf);
            break;

        case SPROM_UTIL_TYPE_PS:
            ntoh_power_supply_sprom_t((power_supply_sprom_t *)buf);
            break;

        case SPROM_UTIL_TYPE_FAN:
            ntoh_fan_sprom_t((fan_sprom_t *)buf);
            break;
        default:
            return ( -SPROM_ERR_INVALID );
    }
    
    return rc;
}

/*
** Public interface for all SPROMs hton
*/
int
sprom_hton (const diag_sprom_t* sprom, uint8_t *buf )
{
    int rc = 0;
    uint32_t size;

    size = sprom_get_size( sprom );

    switch (sprom->type) {
        case SPROM_UTIL_TYPE_LC:
            hton_line_card_sprom_t((line_card_sprom_t*)buf);
            break;

        case SPROM_UTIL_TYPE_BP:
        case SPROM_UTIL_TYPE_BROCKWAY_UPLINK_BP:
            hton_backplane_sprom_t((backplane_sprom_t*)buf);
            break;

        case SPROM_UTIL_TYPE_SUP:
        case SPROM_UTIL_TYPE_BROCKWAY_UPLINK_SUP:
            hton_sup_sprom_t((sup_sprom_t*)buf);
            break;

        case SPROM_UTIL_TYPE_PS:
            hton_power_supply_sprom_t((power_supply_sprom_t *)buf);
            break;

        case SPROM_UTIL_TYPE_FAN:
            hton_fan_sprom_t((fan_sprom_t *)buf);
            break;
        default:
            return ( -SPROM_ERR_INVALID );
    }

    return rc;
}
