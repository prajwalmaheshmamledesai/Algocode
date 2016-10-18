/********************************************************************
 *
 *      File:   redwood_portmap.h
 *      Name:   Sudharshan Kadari
 *
 *      Description: PortMap Definitions.
 *
 *
 *      Copyright (c) 2001,2002,2003,2004 by Nuova Systems, Inc.
 ********************************************************************/

#ifndef _REDWOOD_PORTMAP_H_
#define _REDWOOD_PORTMAP_H_

#include "redwood_mac_hw_seq.h"
/* 
 * Generic phy level data structures.
 */

typedef enum phy_if_type_
{
    PHY_IF_TYPE_NONE,
    PHY_IF_TYPE_MII,
    PHY_IF_TYPE_GMII,
    PHY_IF_TYPE_RGMII,
    PHY_IF_TYPE_SGMII,
    PHY_IF_TYPE_XAUI,
} phy_if_type_t;

typedef enum mdio_type_
{
    MDIO_TYPE_NONE,
    MDIO_TYPE_C22_3_3V,
    MDIO_TYPE_C45_3_3V,
    MDIO_TYPE_C45_1_2V,
} mdio_type_t;

typedef enum media_type_
{
    MEDIA_TYPE_NONE,
    MEDIA_TYPE_100BASET,
    MEDIA_TYPE_1000BASET,
    MEDIA_TYPE_SFPPLUS,
} media_type_t;

typedef enum phy_model_
{
    PHY_MODEL_NONE,
    PHY_MODEL_BCM8726,
    PHY_MODEL_BCM8072,
    PHY_MODEL_BCM54980,
    PHY_MODEL_BCM5221,
} phy_model_t;

#define PHY_LANE_MAX        4

/* 
 * Generic ASIC level data structures.
 */
typedef enum asic_port_type_
{
    PORT_TYPE_HIF,
    PORT_TYPE_NIF,
    PORT_TYPE_CIF,
    PORT_TYPE_BIF,
} asic_port_type_t;


/* 
 * Redwood specific data structures.
 */
#define RW_HIF_MAX      8
#define RW_NIF_MAX      4

#define LANE_MAP(a,b,c,d)       (((a)<<6) | ((b) <<4) | ((c) << 2) | ((d)))

typedef struct _rdw_pmap_s {
        int             *nif_tx_pol;
        int             *nif_rx_pol;
        int             *hif_tx_pol;
        int             *hif_rx_pol;
        mac_cfg_t       *nif_cfg;
        mac_cfg_t       *hif_cfg;
} rdw_pmap_t;

extern int redwood_get_port_information(uint8_t asic_id, 
			rdw_pmap_t **prdw);
extern int redwood_get_port_speed(uint8_t asic_id, uint8_t port_type, 
			uint8_t port_no, uint8_t *speed);

#endif // _REDWOOD_PORTMAP_H_
