/********************************************************************
 *
 *      File:   portola_mac_hw_dover.c
 *      Name:   Juan Munoz
 *
 *      Description:
 *      MAC dispatch layer. Appropriate driver HW layer function
 *      called thru function table.
 *
 *      Copyright (c) 2009 by Cisco Systems, Inc.
 *      All rights reserved.
 *
 ********************************************************************/
#include <stdio.h> 
#include <stdint.h> 
#include <unistd.h> 
#include "woo_ops.h"
#include "fext.h"

#define MDIO_0	0
#define MDIO_1	1
#define MDIO_2	2
#define MDIO_3	3

#define PORT_0	0
#define PORT_1	1
#define PORT_2	2
#define PORT_3	3
#define PORT_4	4
#define PORT_5	5
#define PORT_6	6
#define PORT_7	7

#define DEV_0	0
#define DEV_1	1
#define G(x)	x

#ifndef TRUE
#define TRUE	1
#endif

#ifndef FALSE
#define FALSE	0
#endif
asic_port_info_t prt_ni_info[] = 
{
/*port_id,fp_id,m,mdio_d,port,tx_pol,gpio_int,gpio_rst,xfi_rx,xfi_tx,sfi_rx,sfi_tx*/
 {0, 3, MDIO_0, DEV_0, PORT_3,0,G(0),G(10),TRUE,TRUE,FALSE,FALSE},
 {1, 0, MDIO_0, DEV_0, PORT_2,0,G(0),G(10),TRUE,TRUE,FALSE,FALSE},
 {2, 1, MDIO_0, DEV_0, PORT_1,0,G(0),G(10),TRUE,TRUE,TRUE,TRUE},
 {3, 2, MDIO_0, DEV_0, PORT_0,0,G(0),G(10),TRUE,TRUE,TRUE,TRUE},
};

asic_port_info_t prt_ridgewood_ni_info[] = 
{
/*port_id,fp_id,m,mdio_d,port,tx_pol,gpio_int,gpio_rst,xfi_rx,xfi_tx,sfi_rx,sfi_tx*/
 {0, 3, MDIO_0, DEV_0, PORT_3,0,G(0),G(10),TRUE,TRUE,FALSE,FALSE},
 {1, 0, MDIO_0, DEV_0, PORT_2,0,G(0),G(10),FALSE,TRUE,FALSE,FALSE},
 {2, 1, MDIO_0, DEV_0, PORT_1,0,G(0),G(10),TRUE,TRUE,TRUE,TRUE},
 {3, 2, MDIO_0, DEV_0, PORT_0,0,G(0),G(10),FALSE,TRUE,TRUE,TRUE},
};

asic_port_info_t prt_hi_info[] = 
{
 { 0,  4,MDIO_3,DEV_1,PORT_4,0, G(1),G(11),TRUE, TRUE ,FALSE,FALSE},
 { 1,  0,MDIO_3,DEV_1,PORT_0,0, G(1),G(11),TRUE, FALSE,FALSE,FALSE},
 { 2,  5,MDIO_3,DEV_1,PORT_5,0, G(1),G(11),TRUE, TRUE ,FALSE,FALSE},
 { 3,  1,MDIO_3,DEV_1,PORT_1,0, G(1),G(11),FALSE,FALSE,FALSE,FALSE},
 { 4,  6,MDIO_3,DEV_1,PORT_6,0, G(1),G(11),TRUE, TRUE, FALSE,FALSE},
 { 5,  2,MDIO_3,DEV_1,PORT_2,0, G(1),G(11),FALSE,FALSE,FALSE,FALSE},
 { 6,  7,MDIO_3,DEV_1,PORT_7,0, G(1),G(11),FALSE,FALSE,FALSE,FALSE},
 { 7,  3,MDIO_3,DEV_1,PORT_3,0, G(1),G(11),TRUE ,TRUE ,FALSE,FALSE},
 { 8, 12,MDIO_3,DEV_0,PORT_4,0, G(2),G(12),TRUE, TRUE ,FALSE,FALSE},
 { 9,  8,MDIO_3,DEV_0,PORT_0,0, G(2),G(12),TRUE, TRUE, FALSE,FALSE},
 {10, 13,MDIO_3,DEV_0,PORT_5,0, G(2),G(12),FALSE,TRUE ,FALSE,FALSE},
 {11,  9,MDIO_3,DEV_0,PORT_1,0, G(2),G(12),FALSE,FALSE,FALSE,FALSE},
 {12, 14,MDIO_3,DEV_0,PORT_6,0, G(2),G(12),TRUE, TRUE, FALSE,FALSE},
 {13, 10,MDIO_3,DEV_0,PORT_2,0, G(2),G(12),FALSE,TRUE, FALSE,FALSE},
 {14, 16,MDIO_1,DEV_1,PORT_0,0, G(2),G(12),FALSE,TRUE, FALSE,FALSE},
 {15, 15,MDIO_3,DEV_0,PORT_7,0, G(2),G(12),FALSE,FALSE,FALSE,FALSE},
 {16, 11,MDIO_3,DEV_0,PORT_3,0, G(3),G(13),TRUE, TRUE ,FALSE,FALSE},
 {17, 17,MDIO_1,DEV_1,PORT_1,0, G(3),G(13),FALSE,FALSE,FALSE,FALSE},
 {18, 18,MDIO_1,DEV_1,PORT_2,0, G(3),G(13),FALSE,TRUE, FALSE,FALSE},
 {19, 20,MDIO_1,DEV_1,PORT_4,0, G(3),G(13),FALSE,TRUE ,FALSE,FALSE},
 {20, 19,MDIO_1,DEV_1,PORT_3,0, G(3),G(13),FALSE,TRUE ,FALSE,FALSE},
 {21, 21,MDIO_1,DEV_1,PORT_5,0, G(3),G(13),FALSE,TRUE ,FALSE,FALSE},
 {22, 22,MDIO_1,DEV_1,PORT_6,0, G(3),G(13),FALSE,TRUE, FALSE,FALSE},
 {23, 23,MDIO_1,DEV_1,PORT_7,0, G(3),G(13),FALSE,FALSE,FALSE,FALSE},
 {24, 24,MDIO_1,DEV_0,PORT_0,0, G(4),G(14),FALSE,TRUE, FALSE,FALSE},
 {25, 28,MDIO_1,DEV_0,PORT_4,0, G(4),G(14),FALSE,TRUE ,FALSE,FALSE},
 {26, 25,MDIO_1,DEV_0,PORT_1,0, G(4),G(14),FALSE,FALSE,FALSE,FALSE},
 {27, 29,MDIO_1,DEV_0,PORT_5,0, G(4),G(14),FALSE,TRUE ,FALSE,FALSE},
 {28, 26,MDIO_1,DEV_0,PORT_2,0, G(4),G(14),FALSE,FALSE,FALSE,FALSE},
 {29, 30,MDIO_1,DEV_0,PORT_6,0, G(4),G(14),TRUE, FALSE,FALSE,FALSE},
 {30, 27,MDIO_1,DEV_0,PORT_3,0, G(4),G(14),TRUE, TRUE ,FALSE,FALSE},
 {31, 31,MDIO_1,DEV_0,PORT_7,0, G(4),G(14),FALSE,FALSE,FALSE,FALSE},
 {32, 32,MDIO_2,DEV_1,PORT_0,0, G(5),G(15),TRUE, TRUE, FALSE,FALSE},
 {33, 36,MDIO_2,DEV_1,PORT_4,0, G(5),G(15),TRUE, TRUE ,FALSE,FALSE},
 {34, 37,MDIO_2,DEV_1,PORT_5,0, G(5),G(15),TRUE, TRUE ,FALSE,FALSE},
 {35, 33,MDIO_2,DEV_1,PORT_1,0, G(5),G(15),FALSE,FALSE,FALSE,FALSE},
 {36, 38,MDIO_2,DEV_1,PORT_6,0, G(5),G(15),TRUE, TRUE, FALSE,FALSE},
 {37, 34,MDIO_2,DEV_1,PORT_2,0, G(5),G(15),FALSE,FALSE,FALSE,FALSE},
 {38, 39,MDIO_2,DEV_1,PORT_7,0, G(5),G(15),FALSE,FALSE,FALSE,FALSE},
 {39, 35,MDIO_2,DEV_1,PORT_3,0, G(5),G(15),TRUE, TRUE ,FALSE,FALSE},
 {40, 40,MDIO_2,DEV_0,PORT_0,0, G(6),G(16),FALSE,TRUE, FALSE,FALSE},
 {41, 44,MDIO_2,DEV_0,PORT_4,0, G(6),G(16),TRUE, TRUE ,FALSE,FALSE},
 {42, 41,MDIO_2,DEV_0,PORT_1,0, G(6),G(16),FALSE,FALSE,FALSE,FALSE},
 {43, 45,MDIO_2,DEV_0,PORT_5,0, G(6),G(16),TRUE, TRUE ,FALSE,FALSE},
 {44, 42,MDIO_2,DEV_0,PORT_2,0, G(6),G(16),FALSE,FALSE,FALSE,FALSE},
 {45, 46,MDIO_2,DEV_0,PORT_6,0, G(6),G(16),TRUE, TRUE, FALSE,FALSE},
 {46, 43,MDIO_2,DEV_0,PORT_3,0, G(6),G(16),FALSE,FALSE,FALSE,FALSE},
 {47, 47,MDIO_2,DEV_0,PORT_7,0, G(6),G(16),FALSE,FALSE,FALSE,FALSE},
};

asic_port_info_t prt_ni_ast24_info[] = 
{
 {0, 1, MDIO_0, DEV_0, PORT_1,1,G(0),G(10),TRUE,TRUE,TRUE,TRUE},
 {1, 0, MDIO_0, DEV_0, PORT_0,1,G(0),G(10),TRUE,TRUE,TRUE,TRUE},
};

#define PRT_DUMMY_PORT_ENTRY(portnum) { (portnum),  0, MDIO_1, DEV_0, PORT_0, 0, G(1), G(11), FALSE, TRUE, FALSE,FALSE}
asic_port_info_t prt_hi_ast24_info[] = 
{
 { 0,  1, MDIO_1, DEV_0, PORT_0, 0, G(1), G(11), FALSE, TRUE, FALSE,FALSE},
 PRT_DUMMY_PORT_ENTRY(1),
 { 2,  0, MDIO_1, DEV_0, PORT_1, 0, G(1), G(11), FALSE, FALSE,FALSE,FALSE},
 PRT_DUMMY_PORT_ENTRY(3),
 { 4,  3, MDIO_1, DEV_0, PORT_2, 0, G(1), G(11), FALSE, FALSE,FALSE,FALSE},
 PRT_DUMMY_PORT_ENTRY(5),
 { 6,  2, MDIO_1, DEV_0, PORT_3, 0, G(1), G(11), TRUE,  TRUE, FALSE,FALSE},
 PRT_DUMMY_PORT_ENTRY(7),
 { 8,  5, MDIO_1, DEV_0, PORT_4, 0, G(1), G(11), FALSE, TRUE, FALSE,FALSE},
 PRT_DUMMY_PORT_ENTRY(9),
 {10,  4, MDIO_1, DEV_0, PORT_5, 0, G(1), G(11), FALSE, TRUE, FALSE,FALSE},
 PRT_DUMMY_PORT_ENTRY(11),
 {12,  7, MDIO_1, DEV_0, PORT_6, 0, G(1), G(11), TRUE,  FALSE,FALSE,FALSE},
 PRT_DUMMY_PORT_ENTRY(13),
 {14,  6, MDIO_1, DEV_0, PORT_7, 0, G(1), G(11), FALSE, FALSE,FALSE,FALSE},
 PRT_DUMMY_PORT_ENTRY(15),
 {16,  9, MDIO_2, DEV_0, PORT_0, 0, G(2), G(12), TRUE,  TRUE, FALSE,FALSE},
 PRT_DUMMY_PORT_ENTRY(17),
 {18,  8, MDIO_2, DEV_0, PORT_1, 0, G(2), G(12), FALSE, FALSE,FALSE,FALSE},
 PRT_DUMMY_PORT_ENTRY(19),
 {20, 11, MDIO_2, DEV_0, PORT_2, 0, G(2), G(12), FALSE, FALSE,FALSE,FALSE},
 PRT_DUMMY_PORT_ENTRY(21),
 {22, 10, MDIO_2, DEV_0, PORT_3, 0, G(2), G(12), TRUE,  TRUE, FALSE,FALSE},
 PRT_DUMMY_PORT_ENTRY(23),
 {24, 13, MDIO_2, DEV_0, PORT_4, 0, G(2), G(12), TRUE,  FALSE,FALSE,FALSE},
 PRT_DUMMY_PORT_ENTRY(25),
 {26, 12, MDIO_2, DEV_0, PORT_5, 0, G(2), G(12), TRUE,  TRUE, FALSE,FALSE},
 PRT_DUMMY_PORT_ENTRY(27),
 {28, 15, MDIO_2, DEV_0, PORT_6, 0, G(2), G(12), TRUE,  FALSE,FALSE,FALSE},
 PRT_DUMMY_PORT_ENTRY(29),
 {30, 14, MDIO_2, DEV_0, PORT_7, 0, G(2), G(12), FALSE, FALSE,FALSE,FALSE},
 PRT_DUMMY_PORT_ENTRY(31),
 {32, 17, MDIO_3, DEV_0, PORT_0, 0, G(3), G(13), FALSE, TRUE, FALSE,FALSE},
 PRT_DUMMY_PORT_ENTRY(33),
 {34, 16, MDIO_3, DEV_0, PORT_1, 0, G(3), G(13), FALSE, FALSE,FALSE,FALSE},
 PRT_DUMMY_PORT_ENTRY(35),
 {36, 19, MDIO_3, DEV_0, PORT_2, 0, G(3), G(13), FALSE, FALSE,FALSE,FALSE},
 PRT_DUMMY_PORT_ENTRY(37),
 {38, 18, MDIO_3, DEV_0, PORT_3, 0, G(3), G(13), FALSE, TRUE, FALSE,FALSE},
 PRT_DUMMY_PORT_ENTRY(39),
 {40, 21, MDIO_3, DEV_0, PORT_4, 0, G(3), G(13), TRUE,  TRUE, FALSE,FALSE},
 PRT_DUMMY_PORT_ENTRY(41),
 {42, 20, MDIO_3, DEV_0, PORT_5, 0, G(3), G(13), TRUE,  FALSE,FALSE,FALSE},
 PRT_DUMMY_PORT_ENTRY(43),
 {44, 23, MDIO_3, DEV_0, PORT_6, 0, G(3), G(13), TRUE, TRUE, FALSE,FALSE},
 PRT_DUMMY_PORT_ENTRY(45),
 {46, 22, MDIO_3, DEV_0, PORT_7, 0, G(3), G(13), FALSE, FALSE,FALSE,FALSE},
 PRT_DUMMY_PORT_ENTRY(47),
};


extern int fext_get_card_index(uint16_t *card_index);

asic_port_info_t *asic_get_port_info_ptr(uint8_t is_hi)
{
    asic_port_info_t *port_ptr;
    int rc = 0;
    uint16_t card_index = 0x00;
    rc = fext_get_card_index(&card_index);
    if (rc) return (NULL);

    if (is_hi) {
        if (card_index == CARD_INDEX_ASTORIA_24) {
            port_ptr =  prt_hi_ast24_info;
        } else {
            port_ptr =  prt_hi_info;
        }
    } else {
        if (card_index == CARD_INDEX_ASTORIA_24) {
            port_ptr =  prt_ni_ast24_info;
        } else if(card_index == CARD_INDEX_RIDGEWOOD) {
            port_ptr = prt_ridgewood_ni_info;
        } else {
            port_ptr = prt_ni_info;
        }
    }
    return (port_ptr); 
}
int asic_get_xfi_polarity(uint8_t asic_id, uint8_t asic_port,
			  uint8_t *rx_inverted, uint8_t *tx_inverted, 
                          asic_port_info_t *pinfo)
{
	int rc = 0;
	*rx_inverted = pinfo[asic_port].xfi_rx_inv;
	*tx_inverted = pinfo[asic_port].xfi_tx_inv;
	return (rc);
}

int asic_get_sfi_polarity(uint8_t asic_id, uint8_t asic_port, uint8_t is_hi,
			  uint8_t *rx_inverted, uint8_t *tx_inverted)
{
    int rc;
    // Validate Port information.
    asic_port_info_t *port_ptr;
    uint16_t card_index = 0x00;
    rc = fext_get_card_index(&card_index);
    if (rc) return (rc);

    if (card_index == CARD_INDEX_ASTORIA_24) {
        port_ptr =  prt_ni_ast24_info;
    } else {
        port_ptr = prt_ni_info;
    }
    if (asic_port > (is_hi? ASIC(HI_COUNT) : ASIC(NI_COUNT))) return (-1);

    *rx_inverted = (port_ptr[asic_port].sfi_rx_inv == TRUE)? 1 : 0;
    *tx_inverted = (port_ptr[asic_port].sfi_tx_inv == TRUE)? 1 : 0;
    return (0);
}

int asic_hif_to_phy (uint8_t hif_port)
{
	uint16_t card_index = 0x00;
	int rc;

	rc = fext_get_card_index(&card_index);
	if (rc) return (rc);

	if (card_index == CARD_INDEX_ASTORIA_24) {		// Astoria 24
		return ( (prt_hi_ast24_info[hif_port].fp_id));
	} else {	// Astoria
		if (hif_port > ASIC(HI_COUNT)) return (hif_port);
		return ( (prt_hi_info[hif_port].fp_id));
	}
}

int asic_hif_to_fp (uint8_t hif_port)
{
	uint16_t card_index = 0x00;
	int rc;

	rc = fext_get_card_index(&card_index);
	if (rc) return (rc);

	if (card_index == CARD_INDEX_ASTORIA_24) {		// Astoria 24
		return (prt_hi_ast24_info[hif_port].fp_id);
	} else {	// Astoria
		if (hif_port > ASIC(HI_COUNT)) return (hif_port);
		return (prt_hi_info[hif_port].fp_id);
	}
}
