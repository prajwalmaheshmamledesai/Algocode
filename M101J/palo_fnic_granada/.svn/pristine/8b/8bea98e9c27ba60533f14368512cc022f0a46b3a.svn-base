/********************************************************************
 *
 *      File:   red_hw_lib.h
 *      Name:   Sudharshan Kadari
 *
 *      Description:
 *       Redwood lib prototypes 
 *
 *
 * Copyright (c) 1985-2007 by Cisco Systems, Inc.
 * All rights reserved.
 *
 *
 *********************************************************************/

#ifndef _RED_HW_LIB_H_
#define _RED_HW_LIB_H_

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include "red_hw_serdes.h"
#include "red_hw_reset.h"
#include "redwood_mdio.h"
#include "red_hw_rdarb_dhs_test.h"

extern int diag_debug;
#define BA(x) (((x) << 2))
#define RED_SLEEP usleep
#define bits( hi, lo, val ) \
    ((uint32_t)((val) << (32 - ((hi) - (lo)) - 1)) >> (32 - (hi) - 1))


typedef enum _boolean {
	FALSE,
	TRUE
} boolean;

extern int red_reg_rd(int inst, uint32_t addr, uint32_t *data);
extern int red_reg_wr(int inst, uint32_t addr, uint32_t data);
extern void red_hw_unreset (uint8_t bus, uint8_t inst);
extern void red_hw_unreset_all(uint8_t bus0, uint8_t bus1, uint8_t inst);
extern int red_hw_init(uint8_t bus, uint8_t inst);


#define reg_write( a, r, d ) red_reg_wr(a, r, (uint32_t)(d))
#define reg_read( a, r, d )  red_reg_rd(a, r, (uint32_t*)(d))


#define redwood_read_register_internal(a, r, d, name, line, flag)  reg_read( a, r, d)
#define redwood_write_register_internal(a, r, d, name, line, flag)  reg_write( a, r, d)
#define mac_write	reg_write
#define mac_read	reg_read

// Port definitions
#define cif   0
#define bif   1
#define hif0  2
#define hif1  3
#define hif2  4
#define hif3  5
#define hif4  6
#define hif5  7
#define hif6  8
#define hif7  9
#define nif0 10
#define nif1 11
#define nif2 12
#define nif3 13

extern int red_hw_cfg_reg_test (int inst, const int seed, 
				const int check_default, 
				unsigned int* addr_error, 
				unsigned int* exp_data, 
				unsigned int* got_data) ;

extern int red_hw_cnt_reg_test (int inst, const int seed, 
				const int check_default, 
				unsigned int* addr_error, 
				unsigned int* exp_data, 
				unsigned int* got_data) ;

extern int red_hw_sta_reg_test (int inst, const int seed, 
				unsigned int* addr_error, 
				unsigned int* exp_data, 
				unsigned int* got_data) ;

extern int red_hw_sat_reg_test (int inst, const int seed, 
				const int check_default, 
				unsigned int* addr_error, 
				unsigned int* exp_data, 
				unsigned int* got_data) ;

extern int red_hw_int_reg_test (int inst, const int seed, 
				unsigned int* addr_error, 
				unsigned int* exp_data, 
				unsigned int* got_data) ;

extern int red_hw_bulk_dhs_test (int inst, const int seed, 
				const int ss_bitmap, 
				const int dw_bitmap, 
				unsigned int* addr_error, 
				unsigned int* exp_data, 
				unsigned int* got_data) ;

extern int red_hw_mac_stat_dhs_test (int inst, const int seed, 
				const int dhs_bitmap, 
				unsigned int* addr_error, 
				unsigned int* exp_data, 
				unsigned int* got_data) ;

extern int red_hw_cntl_dhs_test (int inst, const int seed, 
				const int ss_bitmap, 
				const int cntl_bitmap, 
				unsigned int* addr_error, 
				unsigned int* exp_data, 
				unsigned int* got_data) ;

extern int red_hw_mcst_dhs_test (int inst, const int seed, 
				unsigned int* addr_error, 
				unsigned int* exp_data, 
				unsigned int* got_data) ;

extern int red_hw_ss_ddrop_dhs_test (int inst, const int seed, 
				const int ss_bitmap, 
				unsigned int* addr_error, 
				unsigned int* exp_data, 
				unsigned int* got_data) ;

extern int red_hw_ss_oq_dhs_test (int inst, const int seed, 
				const int ss_bitmap, 
				const int oq_bitmap, 
				unsigned int* addr_error, 
				unsigned int* exp_data, 
				unsigned int* got_data) ;

extern int red_hw_fcoe_cnt_dhs_test (int inst, const int seed, 
				const int dhs_bitmap, 
				unsigned int* addr_error, 
				unsigned int* exp_data, 
				unsigned int* got_data) ;

extern int red_hw_ss_stat_dhs_test (int inst, const int seed, 
				const int ss_bitmap, 
				unsigned int* addr_error, 
				unsigned int* exp_data, 
				unsigned int* got_data) ;

extern int red_hw_bulk_memif_autotest (int inst, const int seed, 
				const int ss_bitmap, 
				unsigned int* addr_error, 
				unsigned int* exp_data, 
				unsigned int* got_data) ;

extern int red_hw_cntl_memif_autotest (int inst, const int seed, 
				const int ss_bitmap, 
				const int cntl_bitmap,
				unsigned int* addr_error, 
				unsigned int* exp_data, 
				unsigned int* got_data) ;

extern int red_hw_jtag_id_test (int inst, const int seed,
				unsigned int* addr_error, 
				unsigned int* exp_data, 
				unsigned int* got_data) ;

extern int red_hw_mbist_test (  int inst, unsigned int* addr_error, 
				unsigned int* exp_data, 
				unsigned int* got_data) ;

extern int red_hw_mcst_memif_autotest (int inst, const int seed,
				unsigned int* addr_error, 
				unsigned int* exp_data, 
				unsigned int* got_data) ;


extern void set_field		(unsigned int* variable, 
				unsigned int field_end_bit, 
				unsigned int field_start_bit, 
				unsigned int value);

extern void set_bit		(unsigned int* variable, 
				unsigned int field_bit, 
				unsigned int value);

extern int asic_ss_init		(int asic_id);
extern int red_hw_pll_unreset	(int i2c_fd, uint32_t inst);
extern int red_hw_pll_reset	(int i2c_fd, uint32_t inst);
extern int red_hw_asic_unreset	(int i2c_fd, uint32_t inst);
extern int red_hw_asic_reset	(int i2c_fd, uint32_t inst);
extern int redwood_init		(int i2c_fd, uint32_t inst);
extern void redwood_bring_out_of_reset( int acc_fd , int reset_fd, int asic_id);
extern void redwood_phy_unreset(int asic_id);
extern void redwood_phy_reset(int asic_id);
extern int mac_loopback_nif( int asic_id, int mac_id /*0..3*/ );
extern int mac_loopback_hif( int asic_id, int mac_id /*0..7*/ );
extern void mac_rmon_stats_dump( int asic_id, int port, int width);
extern void mac_rmon_stats_clear( int asic_id, int port, int width);
extern void mac_rmon_stats_clear_all (int asic_id);
extern void mac_rmon_cif_stats_get(int  asic_id, uint32_t *tx_cnt, 
					uint32_t *rx_cnt);

extern void redwood_ss_stats_dump(int asic_id, int ssx, uint8_t portmask);
extern uint8_t red_name_to_port (uint8_t *name, uint8_t *port);

extern void redwood_lane_map_set(uint8_t asic_id, uint8_t hif, uint8_t tx, uint8_t port, uint8_t data);
extern void redwood_polarity_set(uint8_t asic_id, uint8_t hif, uint8_t tx, uint8_t port, uint8_t data);
extern void redwood_lane_pol_print(uint8_t asic_id, uint8_t hif, uint8_t port);
extern void redwood_lane_pol_print_all(uint8_t asic_id);
extern int redwood_ss_eos_checks(int inst, unsigned int* addr_error, unsigned  int* exp_data, unsigned  int* got_data);


extern int redwood_10G_mac_status_get (int inst, uint8_t hif, uint8_t port, uint8_t *link, uint8_t *fault);
extern int redwood_gbe_mac_status_get(int inst, uint8_t cpuif, uint8_t *plink);
extern int redwood_serdes_mac_init(int asic_id, int flag);
extern int redwood_serdes_nif_init(int asic_id, int flag);
extern int redwood_serdes_hif_init(int asic_id, int flag);
extern int redwood_serdes_hif_port_init(int asic_id, int port);

extern void mac_pgm_serdes( int asic_id, int mac_id, int first_lane, int num_lanes,
                     int rx_polarity, int tx_polarity );

extern void redwood_config_hif_cif(int inst, int port, int entry);
extern int redwood_snake_config(uint8_t asic_id, uint8_t fp_mask, uint8_t bp_mask);
extern int redwood_fex_snake_config(uint8_t asic_id, uint8_t fp_mask, uint8_t bp_mask);
extern int redwood_rewrite_snake_config(uint8_t asic_id, uint8_t fp_mask, uint8_t bp_mask);
extern int redwood_pif_config(uint8_t asic_id, uint8_t fp_mask, uint8_t bp_mask);
extern int redwood_hton_config(uint8_t asic_id, uint8_t fp_mask, uint8_t bp_mask);
extern int redwood_ntoh_config(uint8_t asic_id, uint8_t fp_mask, uint8_t bp_mask);
extern int redwood_rewrite_config(uint8_t asic_id, uint8_t fp_mask, uint8_t bp_mask);
extern int redwood_port_mesh_config(uint8_t asic_id, uint8_t fp_mask, uint8_t bp_mask);
extern int redwood_system_config(uint8_t asic_id, uint8_t fp_mask, uint8_t bp_mask, uint32_t nif_mask);
extern void redwood_config_system_hif_cif(int asic_id, uint8_t fp_mask, uint8_t bp_mask,uint8_t inst);
extern void redwood_config_portmesh_hif_cif(int asic_id, uint8_t fp_mask, uint8_t bp_mask);
extern void redwood_config_portmesh_nif_cif(int asic_id, uint8_t fp_mask, uint8_t bp_mask);
extern int redwood_multicast_config(uint8_t asic_id, uint8_t fp_mask, uint8_t bp_mask);

extern void invalidate_fwd_ranges( int asic_id, unsigned int base );
extern int redwood_hif_loopback_cfg( int asic_id, int mac_id /*0..7*/ );
extern int redwood_nspi_timer_cfg(int inst, uint32_t data);
extern void ss_fc_rd_port_release(int inst, uint8_t which_ss, uint8_t release);
extern void ss_fc_rd_port_release_all(int inst, uint8_t release);
extern int redwood_hi_fin_enable(int inst, uint8_t hif, uint8_t ena);
extern char *redwood_reg_addr_to_name(uint32_t addr);
extern int redwood_dump_drop_regs(int inst, uint32_t flag);

extern int redwood_dump_intr_regs(int inst, uint32_t flag);
extern int redwood_dump_intr_mask_regs(int inst, uint32_t flag);
extern int redwood_get_sfp_mask(int inst, int asic_id, uint8_t *sfp_mask);

extern int redwood_normal_polarity(int asic_id);
extern int redwood_ignore_signal_strength(int asic_id);

extern int redwood_core_init(int fd, uint8_t inst);
extern int redwood_port_init(int fd, uint8_t inst);
extern int red_core_init(uint8_t bus, uint8_t inst);
extern int red_port_init(uint8_t bus, uint8_t inst);
extern int redwood_gpio_dump (int inst);

extern void redwood_octphy_unreset(int asic_id);
extern void redwood_octphy_reset(int asic_id);

extern void mac_assert_flush_hif_ss( int asic_id, int mac_id /*0..7*/ );
extern void mac_assert_flush_hif( int asic_id, int mac_id /*0..7*/ );
extern void mac_deassert_flush_hif_ss( int asic_id, int mac_id /*0..7*/ );
extern void mac_deassert_flush_hif( int asic_id, int mac_id /*0..7*/ );
extern void mac_assert_flush_nif_ss( int asic_id, int mac_id /*0..3*/ );
extern void mac_assert_flush_nif( int asic_id, int mac_id /*0..3*/ );
extern void mac_deassert_flush_nif_ss( int asic_id, int mac_id /*0..3*/ );
extern void mac_deassert_flush_nif( int asic_id, int mac_id /*0..3*/ );
extern int mac_enable_hif_1g_hw_seq( int asic_id, int mac_id /*0..7*/ );
extern int redwood_1G_rx_sync_get(int inst, uint8_t port, uint8_t *link);

extern void redwood_bcmsrd_unreset(int asic_id);
extern void redwood_bcmsrd_reset(int asic_id);
extern void redwood_config_syssnake_hif_cif(int asic_id, uint8_t fp_mask, uint8_t bp_mask, uint8_t inst);
extern int  redwood_lanemap_dump(uint8_t asic_id);
extern int red_hw_serdes_disable(int inst, uint32_t nimask, uint32_t himask);
extern int red_hw_serdes_prbs_test_start( int inst, const int serdes_sbus_address
                           , const int wait_time // microseconds
                           , const int pattern // microseconds
                           );
extern int red_hw_serdes_prbs_test_check( int inst, const int serdes_sbus_address
                           , const int wait_time // microseconds
                           );
extern int red_hw_serdes_prbs_test_stop( int inst, const int serdes_sbus_address);
extern int mac_rmon_err_stats_dump(int inst, int port, int width);
extern int redwood_err_dump(int inst, uint32_t *pmask);
extern int serdes_init_preemp( int asic_id, int sbus_port, 
		int first_lane, int n_lanes, uint8_t preemp);
extern int redwood_rmon_clear(int inst);

extern uint32_t system_snake_config(uint8_t *nif_lpbk, uint8_t *hif_lpbk,
                                       uint8_t no_check_flag);
extern int redwood_pause_enable(int inst);
extern int red_hw_serdes_debug_info(int inst, const int serdes_sbus_address);
extern int red_hw_serdes_avago_prbs(int inst, const int serdes_sbus_address);
extern int red_hw_serdes_diag(int inst, const int serdes_sbus_address);


extern int redwood_packet_drop (int inst, uint8_t ena);
extern int red_hw_bulk_dhs_rd_diag (int inst, uint32_t start, uint32_t slen, uint8_t which_ss, uint8_t which_col);
extern int red_hw_bulk_dhs_wr_diag (int inst, uint32_t start, uint32_t slen, uint8_t which_ss, 
				uint8_t which_col, uint32_t data);
extern int redwood_get_die_id(int inst);
extern int redwood_nif2hif(uint8_t asic_id, uint8_t hi_port, uint8_t ni_port, uint32_t offset);
extern int redwood_hif2nif(uint8_t asic_id, uint8_t hi_port, uint8_t ni_port, uint32_t offset);
#endif // _RED_HW_LIB_H_
