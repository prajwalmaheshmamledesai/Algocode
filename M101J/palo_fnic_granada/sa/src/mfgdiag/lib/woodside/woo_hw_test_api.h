/*****************************************************************************
 *      File:  woo_hw_test_api.h
 *      Name:   Raghu Boreda
 *
 *      Description: Woodside test api's. 
 *                   Put all woodside test api's here. All other files which
 *                   will use test functions should include this file.
 *      Copyright (c) 2009 by Cisco Systems, Inc.
 *****************************************************************************/
#ifndef _WOO_HW_TEST_API_H_
#define _WOO_HW_TEST_API_H_
int woo_hw_sat_reg_test(int, const int, const int, unsigned int*, unsigned int*,
                        unsigned int* );
int woo_hw_sta_reg_test(int, unsigned int*, unsigned int*, unsigned int* );
int asic_hw_mbist_test(int, unsigned char, unsigned int*, unsigned int*, unsigned int* );
int woo_sbus_rdacc_test(int , unsigned int* , unsigned int *, unsigned int *);
#if defined(DIAG_PORTOLA)
int prt_hw_sta_reg_test(int, unsigned int *, unsigned int*, unsigned int*);
int prt_sbus_rdacc_test(int, unsigned int *, unsigned int*, unsigned int*);
int prt_serdes_prbs_test(int, uint64_t, const int, uint64_t*);
#endif /* DIAG_PORTOLA */
#endif
