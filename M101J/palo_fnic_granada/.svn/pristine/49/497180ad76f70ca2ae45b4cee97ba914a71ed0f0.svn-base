/*****************************************************************************
 *      File:  woo_hw_mbist_test.c
 *      Name:   Raghu Boreda
 *
 *      Based on simulation file : sce_mbist.h
 *      Description: Bist test functions for Woodside 
 *      Copyright (c) 2009 by Cisco Systems, Inc.
 *****************************************************************************/
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include "woo_ops.h"
#include "woo_hw_error.h"
#include "diag_main.h"
/*****************************************************************************
 * tcam_mbist
 ****************************************************************************/
#define RESET_ACTION 1
#define RUN_ACTION 2
#define CHECK_ACTION 3
static int tcam_mbist (uint8_t inst, unsigned int *addr_error,
                       unsigned int *exp_data, int action_type)
{
    unsigned int pass, fail, which_hf;
    int ret_val;
    switch(action_type) {
    case RESET_ACTION:
        for (which_hf = 0; which_hf < ASIC(HF_COUNT); ++which_hf) { 
            /* put in RST */
            *addr_error = ASIC(HF0_CFG_TCAM_CORE_TCAM_BIST_RST) + 
                 ((ASIC(HF1_CFG_TCAM_CORE_TCAM_BIST_RST)-
                   ASIC(HF0_CFG_TCAM_CORE_TCAM_BIST_RST))*which_hf);
            *exp_data = 0xf;
            if ((ret_val=asic_wr(inst,(*addr_error)*4, *exp_data))!=0) {
                return ret_val;
            }
         }
        for (which_hf = 0; which_hf < ASIC(HF_COUNT); ++which_hf) { 
            /* release RST */
            *addr_error = ASIC(HF0_CFG_TCAM_CORE_TCAM_BIST_RST) + 
                 ((ASIC(HF1_CFG_TCAM_CORE_TCAM_BIST_RST)-
                   ASIC(HF0_CFG_TCAM_CORE_TCAM_BIST_RST))*which_hf);
            *exp_data = 0;
            if ((ret_val=asic_wr(inst,(*addr_error)*4, *exp_data))!=0) {
                return ret_val;
            }
         }
         break;
    case RUN_ACTION:
        for (which_hf = 0; which_hf < ASIC(HF_COUNT); ++which_hf) { 
            /* set RUN */
            *addr_error = ASIC(HF0_CFG_TCAM_CORE_TCAM_BIST_RUN) + 
                 ((ASIC(HF1_CFG_TCAM_CORE_TCAM_BIST_RUN)-
                   ASIC(HF0_CFG_TCAM_CORE_TCAM_BIST_RUN))*which_hf);
            *exp_data = 0xf;
            if ((ret_val=asic_wr(inst,(*addr_error)*4, *exp_data))!=0) {
                printf("  Err: program HF %d TCAM MBIST RUN \n", which_hf);
                return ret_val;
            }
            DIAG_PRINT(DIAG_PRINT_CONFIG, "program HF %d TCAM MBIST RUN \n",
                   which_hf)
         }
         break;
    case CHECK_ACTION:
        for (which_hf = 0; which_hf < ASIC(HF_COUNT); ++which_hf) { 
            /* clear RUN and check Status */
            *addr_error = ASIC(HF0_STA_TCAM_TCAM_BIST_DONE_PASS_OUT) + 
                 ((ASIC(HF1_STA_TCAM_TCAM_BIST_DONE_PASS_OUT)-
                   ASIC(HF0_STA_TCAM_TCAM_BIST_DONE_PASS_OUT))*which_hf);
            if ((ret_val=asic_rd(inst,(*addr_error)*4, &pass))!=0) {
                return ret_val;
            }
            
            *addr_error = ASIC(HF0_STA_TCAM_TCAM_BIST_DONE_FAIL_OUT) + 
                 ((ASIC(HF1_STA_TCAM_TCAM_BIST_DONE_FAIL_OUT)-
                   ASIC(HF0_STA_TCAM_TCAM_BIST_DONE_FAIL_OUT))*which_hf);
            if ((ret_val=asic_rd(inst,(*addr_error)*4, &fail))!=0) {
                return ret_val;
            }
            if (fail & 0xf) { //failed
                printf("HF %d TCAM MBIST failed %d \n",
                        which_hf, fail);
            }
            if (!((pass & 0xf) == 0xf)) { //incomplete
                printf("HF %d TCAM MBIST incomplete %d \n",
                        which_hf, pass);
            } else {
                DIAG_PRINT(DIAG_PRINT_TEST_PROGRESS, "HF %d TCAM MBIST passed \n",
                        which_hf)
            }
         }
         break;
      default:
         break;
    }
    return (0); 
}


/*
 * hi cfg mbist 
 */
static int hi_cfg_mbist (uint8_t inst, unsigned int *addr_error, 
                         unsigned int *exp_data, 
                         unsigned int run, unsigned int reset)
{
    int ret_val = 0, which_hi;

    for (which_hi = 0; which_hi < ASIC(HI_COUNT); ++which_hi) { 
        *addr_error = ASIC(HI0_CFG_MBIST) + 
                      ((ASIC(HI1_CFG_MBIST)-ASIC(HI0_CFG_MBIST))*which_hi);
        *exp_data = ASIC(HI_CFG_MBIST_DEF);
        set_bit (exp_data, ASIC(HI_CFG_MBIST_FLD_RUN), run);
        set_bit (exp_data, ASIC(HI_CFG_MBIST_FLD_RST), reset);
        if ((ret_val=asic_wr(inst,(*addr_error)*4, *exp_data))!=0) {
            printf("  Err: WOO%d program HI %d  LVIF MBIST RUN =%d, RST =%d\n",
               inst, which_hi,run,reset);
            return ret_val;
        }
        DIAG_PRINT(DIAG_PRINT_CONFIG,
                   "program HI %d  LVIF MBIST RUN =%d, RST =%d\n",
                which_hi,run,reset)

        *addr_error = ASIC(HI0_CFG_ARF_MBIST) + 
                      ((ASIC(HI1_CFG_ARF_MBIST)-ASIC(HI0_CFG_ARF_MBIST))*which_hi);
        *exp_data = ASIC(HI_CFG_ARF_MBIST_DEF);
        set_bit (exp_data, ASIC(HI_CFG_ARF_MBIST_FLD_RUN), run);
        set_bit (exp_data, ASIC(HI_CFG_ARF_MBIST_FLD_RST), reset);
        if ((ret_val=asic_wr(inst,(*addr_error)*4, *exp_data))!=0) {
            printf("  Err: Woo-%d program HI %d  ARF MBIST RUN =%d, RST =%d\n",
                   inst, which_hi,run,reset);
            return ret_val;
        }

        DIAG_PRINT(DIAG_PRINT_CONFIG,
                   "program HI %d  ARF MBIST RUN =%d, RST =%d\n",
                   which_hi,run,reset)
    }
    return (ret_val);
}
/*
 * Check HI Bist status
 */
static int hi_mbist_status (uint8_t inst, unsigned int *addr_error, 
                            unsigned int *got_data) 
{
    int ret_val = 0, which_hi;
    unsigned int pass = 0, fail = 0;
    for (which_hi = 0; which_hi < ASIC(HI_COUNT); ++which_hi) { 
        *addr_error = ASIC(HI0_STA_ARF_MBIST) + 
                      ((ASIC(HI1_STA_ARF_MBIST)-ASIC(HI0_STA_ARF_MBIST))*which_hi);
        if ((ret_val=asic_rd(inst,(*addr_error)*4, got_data))!=0) {
            return ret_val;
        }
        fail = ((*got_data) >> ASIC(HI_STA_ARF_MBIST_FLD_FAIL))&1; 
        pass = ((*got_data) >> ASIC(HI_STA_ARF_MBIST_FLD_PASS))&1; 
        if (fail) {
            printf("HI %d  ARF MBIST Failed\n",
                   which_hi);
            ret_val = WOODSIDE_ERROR_MBIST_FAILURE;
            break;
        } else if (pass) {
            DIAG_PRINT(DIAG_PRINT_TEST_PROGRESS, "HI %d  ARF MBIST Passed\n",
                   which_hi)
            continue;
        } else {
            printf("HI %d  ARF MBIST is still running. Increase timeout\n",
                   which_hi);
            ret_val = WOODSIDE_ERROR_MBIST_TIMEOUT;
        }
    }

    for (which_hi = 0; which_hi < ASIC(HI_COUNT); ++which_hi) { 
        *addr_error = ASIC(HI0_STA_MBIST) + 
                      ((ASIC(HI1_STA_MBIST)-ASIC(HI0_STA_MBIST))*which_hi);
        if ((ret_val=asic_rd(inst,(*addr_error)*4, got_data))!=0) {
            return ret_val;
        }
        fail = ((*got_data) >> ASIC(HI_STA_MBIST_FLD_FAIL))&1; 
        pass = ((*got_data) >> ASIC(HI_STA_MBIST_FLD_PASS))&1; 
        if (fail) {
            printf("HI %d  MBIST Failed\n",
                   which_hi);
            ret_val = WOODSIDE_ERROR_MBIST_FAILURE;
            break;
        } else if (pass) {
            DIAG_PRINT(DIAG_PRINT_TEST_PROGRESS, "HI %d  MBIST Passed\n",
                   which_hi)
            continue;
        } else {
            printf("HI %d  MBIST is still running. Increase timeout\n",
                   which_hi);
            ret_val = WOODSIDE_ERROR_MBIST_TIMEOUT;
        }
    }
    return (ret_val);
}
/*
 * ni cfg mbist 
 */
static int ni_cfg_mbist (uint8_t inst, unsigned int *addr_error, 
                         unsigned int *exp_data, 
                         unsigned int run, unsigned int reset)
{
    int ret_val = 0, which_ni;

    for (which_ni = 0; which_ni < ASIC(NI_COUNT); ++which_ni) { 
        *addr_error = ASIC(NI0_CFG_ARF_MBIST) + 
                      ((ASIC(NI1_CFG_ARF_MBIST)-ASIC(NI0_CFG_ARF_MBIST))*which_ni);
        *exp_data = ASIC(NI_CFG_ARF_MBIST_DEF);
        set_bit (exp_data, ASIC(NI_CFG_ARF_MBIST_FLD_RUN), run);
        set_bit (exp_data, ASIC(NI_CFG_ARF_MBIST_FLD_RST), reset);
        if ((ret_val=asic_wr(inst,(*addr_error)*4, *exp_data))!=0) {
            printf("  Err: Woo-%d program NI %d  ARF MBIST RUN =%d, RST =%d\n",
                  inst, which_ni,run,reset);
            return ret_val;
        }
        DIAG_PRINT(DIAG_PRINT_CONFIG, 
                   "program NI %d  ARF MBIST RUN =%d, RST =%d\n",
                   which_ni,run,reset)
    }
    return (ret_val);
}
/*
 * Check NI Bist status
 */
static int ni_mbist_status (uint8_t inst, unsigned int *addr_error, 
                            unsigned int *got_data) 
{
    int ret_val = 0, which_ni;
    unsigned int pass = 0, fail = 0;
    for (which_ni = 0; which_ni < ASIC(NI_COUNT); ++which_ni) { 
        *addr_error = ASIC(NI0_STA_ARF_MBIST) + 
                      ((ASIC(NI1_STA_ARF_MBIST)-ASIC(NI0_STA_ARF_MBIST))*which_ni);
        if ((ret_val=asic_rd(inst,(*addr_error)*4, got_data))!=0) {
            return ret_val;
        }
        fail = ((*got_data) >> ASIC(NI_STA_ARF_MBIST_FLD_FAIL))&1; 
        pass = ((*got_data) >> ASIC(NI_STA_ARF_MBIST_FLD_PASS))&1; 
        if (fail) {
            printf("NI %d  ARF MBIST Failed\n",
                   which_ni);
            ret_val = WOODSIDE_ERROR_MBIST_FAILURE;
            break;
        } else if (pass) {
            DIAG_PRINT(DIAG_PRINT_TEST_PROGRESS, "NI %d  ARF MBIST Passed\n",
                   which_ni)
            continue;
        } else {
            printf("NI %d  ARF MBIST is still running. Increase timeout\n",
                   which_ni);
            ret_val = WOODSIDE_ERROR_MBIST_TIMEOUT;
        }
    }
    return (ret_val);
}
/*
 * hf cfg mbist 
 */
static int hf_cfg_mbist (uint8_t inst, unsigned int *addr_error, 
                         unsigned int *exp_data, 
                         unsigned int run, unsigned int reset)
{
    int ret_val = 0, which_hf;
    for (which_hf = 0; which_hf < ASIC(HF_COUNT); ++which_hf) { 
        *addr_error = ASIC(HF0_CFG_MBIST) + 
                      ((ASIC(HF1_CFG_MBIST)-ASIC(HF0_CFG_MBIST))*which_hf);
        *exp_data = ASIC(HF_CFG_MBIST_DEF);
        set_bit (exp_data, ASIC(HF_CFG_MBIST_FLD_RUN), run);
        set_bit (exp_data, ASIC(HF_CFG_MBIST_FLD_RST), reset);
        if ((ret_val=asic_wr(inst,(*addr_error)*4, *exp_data))!=0) {
            printf("  Err: Woo-%d program HF %d MBIST RUN =%d, RST =%d\n",
                   inst, which_hf,run,reset);
            return ret_val;
        }
        DIAG_PRINT(DIAG_PRINT_CONFIG, "program HF %d MBIST RUN =%d, RST =%d\n",
               which_hf,run,reset)
    }
    return (ret_val);
}
/*
 * hf mbist status 
 */
static int hf_mbist_status (uint8_t inst, unsigned int *addr_error, 
                            unsigned int *got_data) 
{
    int ret_val = 0, which_hf;
    unsigned int pass = 0, fail = 0;
    char *hf_desc[] = {"SVIF","LVIF","Result","Policer"};
    for (which_hf = 0; which_hf < ASIC(HF_COUNT); ++which_hf) { 
        *addr_error = ASIC(HF0_STA_MBIST) + 
                      ((ASIC(HF1_STA_MBIST)-ASIC(HF0_STA_MBIST))*which_hf);
        if ((ret_val=asic_rd(inst,(*addr_error)*4, got_data))!=0) {
            return ret_val;
        }
        fail = ((*got_data) >> ASIC(HF_STA_MBIST_FLD_FAIL_SHIFT))&0xf; 
        pass = ((*got_data) >> ASIC(HF_STA_MBIST_FLD_PASS_SHIFT))&0xf; 
        if (fail) {
            /* Mapping fail to hf_desc array */
            if (fail < 4)
                fail = fail-1;
            else if (fail == 4)
                fail = 2;
            else
                fail = 3;
            printf("HF %d %s ram MBIST Failed\n",
                   which_hf, hf_desc[fail]);
            ret_val = WOODSIDE_ERROR_MBIST_FAILURE;
            break;
        } else if (pass == 0xf) {
            DIAG_PRINT(DIAG_PRINT_TEST_PROGRESS, "HF %d  MBIST Passed\n",
                   which_hf)
            continue;
        } else {
            printf("HF %d  MBIST is still running. Increase timeout\n",
                   which_hf);
            ret_val = WOODSIDE_ERROR_MBIST_TIMEOUT;
        }
    }
    return (ret_val);
}
/*
 * ss cfg mbist 
 */
static int ss_cfg_mbist (uint8_t inst, unsigned int *addr_error, 
                         unsigned int *exp_data, 
                         unsigned int run, unsigned int reset)
{
    int ret_val = 0, which_ss;
    for (which_ss = 0; which_ss < ASIC(SS_COUNT); ++which_ss) { 
        *addr_error = ASIC(SS0_CFG_MBIST) + 
                      ((ASIC(SS1_CFG_MBIST)-ASIC(SS0_CFG_MBIST))*which_ss);
        *exp_data = ASIC(SS_CFG_MBIST_DEF);
        set_bit (exp_data, ASIC(SS_CFG_MBIST_FLD_RUN), run);
        set_bit (exp_data, ASIC(SS_CFG_MBIST_FLD_RST), reset);
        if ((ret_val=asic_wr(inst,(*addr_error)*4, *exp_data))!=0) {
            printf("  Err: Woo-%d program SS %d MBIST RUN =%d, RST =%d\n",
               inst, which_ss,run,reset);
            return ret_val;
        }
        DIAG_PRINT(DIAG_PRINT_CONFIG, "program SS %d MBIST RUN =%d, RST =%d\n",
               which_ss,run,reset)
    }
    return (ret_val);
}
/*
 * Check SS Bist status
 */
static int ss_mbist_status (uint8_t inst, unsigned int *addr_error, 
                            unsigned int *got_data) 
{
    int ret_val = 0, which_ss;
    unsigned int pass = 0, fail = 0 ;
    for (which_ss = 0; which_ss < ASIC(SS_COUNT); ++which_ss) { 
        *addr_error = ASIC(SS0_STA_MBIST) + 
                      ((ASIC(SS1_STA_MBIST)-ASIC(SS0_STA_MBIST))*which_ss);
        if ((ret_val=asic_rd(inst,(*addr_error)*4, got_data))!=0) {
            return ret_val;
        }
        fail = ((*got_data) >> ASIC(SS_STA_MBIST_FLD_FAIL_SHIFT))&0x7fff; 
        pass = ((*got_data) >> ASIC(SS_STA_MBIST_FLD_PASS_SHIFT))&0x7fff; 
        if (fail) {
            printf("SS %d  MBIST Failed\n",
                   which_ss);
            ret_val = WOODSIDE_ERROR_MBIST_FAILURE;
            break;
        } else if (pass == 0x7fff) {
            DIAG_PRINT(DIAG_PRINT_TEST_PROGRESS, "SS %d  MBIST Passed\n",
                   which_ss)
            continue;
        } else {
            printf("SS %d  MBIST is still running. Increase timeout\n",
                   which_ss);
            ret_val = WOODSIDE_ERROR_MBIST_TIMEOUT;
        }
    }
    return (ret_val);
}
/*
 * nf nr hr ci cfg mbist 
 */
static int nf_nr_hr_ci_cfg_mbist (uint8_t inst, unsigned int *addr_error, 
                                  unsigned int *exp_data, 
                                  unsigned int run, unsigned int reset)
{
    int ret_val = 0;

    *addr_error = ASIC(NF_CFG_MBIST);
    *exp_data = ASIC(NF_CFG_MBIST_DEF);
    set_bit (exp_data, ASIC(NF_CFG_MBIST_FLD_RUN), run);
    set_bit (exp_data, ASIC(NF_CFG_MBIST_FLD_RST), reset);
    if ((ret_val=asic_wr(inst,(*addr_error)*4, *exp_data))!=0) {
        printf("  Err: Woo-%d program NF  MBIST RUN =%d, RST =%d\n",
            inst, run, reset);
        return ret_val;
    }
    DIAG_PRINT(DIAG_PRINT_CONFIG, "program NF  MBIST RUN =%d, RST =%d\n",
            run, reset)

    *addr_error = ASIC(NR_CFG_MBIST);
    *exp_data = ASIC(NR_CFG_MBIST_DEF);
    set_bit (exp_data, ASIC(NR_CFG_MBIST_FLD_RUN), run);
    set_bit (exp_data, ASIC(NR_CFG_MBIST_FLD_RST), reset);
    if ((ret_val=asic_wr(inst,(*addr_error)*4, *exp_data))!=0) {
        printf("  Err: Woo-%d program NR  MBIST RUN =%d, RST =%d\n",
            inst, run, reset);
        return ret_val;
    }
    DIAG_PRINT(DIAG_PRINT_CONFIG, "program NR  MBIST RUN =%d, RST =%d\n",
            run, reset)

    *addr_error = ASIC(HR_CFG_MBIST);
    *exp_data = ASIC(HR_CFG_MBIST_DEF);
    set_bit (exp_data, ASIC(HR_CFG_MBIST_FLD_RUN), run);
    set_bit (exp_data, ASIC(HR_CFG_MBIST_FLD_RST), reset);
    if ((ret_val=asic_wr(inst,(*addr_error)*4, *exp_data))!=0) {
        printf("  Err: Woo-%d program HR  MBIST RUN =%d, RST =%d\n",
            inst, run, reset);
        return ret_val;
    }
    DIAG_PRINT(DIAG_PRINT_CONFIG, "program HR  MBIST RUN =%d, RST =%d\n",
            run, reset)

    *addr_error = ASIC(CI_CFG_MBIST);
    *exp_data = ASIC(CI_CFG_MBIST_DEF);
    set_bit (exp_data, ASIC(CI_CFG_MBIST_FLD_RUN), run);
    set_bit (exp_data, ASIC(CI_CFG_MBIST_FLD_RST), reset);
    if ((ret_val=asic_wr(inst,(*addr_error)*4, *exp_data))!=0) {
        printf("  Woo-%d program CI  MBIST RUN =%d, RST =%d\n",
            inst, run, reset);
        return ret_val;
    }
    DIAG_PRINT(DIAG_PRINT_CONFIG, "program CI  MBIST RUN =%d, RST =%d\n",
            run, reset)
    return (ret_val);
}
/*
 * Check CI/NF Bist status
 */
static int ci_nf_mbist_status (uint8_t inst, unsigned int *addr_error, 
                               unsigned int *got_data) 
{
    int ret_val = 0;
    unsigned int pass = 0, fail = 0;
    *addr_error = ASIC(CI_STA_MBIST);
    if ((ret_val=asic_rd(inst,(*addr_error)*4, got_data))!=0) {
        return ret_val;
    }
    fail = ((*got_data) >> ASIC(CI_STA_MBIST_FLD_FAIL))&1; 
    pass = ((*got_data) >> ASIC(CI_STA_MBIST_FLD_PASS))&1; 
    if (fail) {
        printf("CI MBIST Failed\n");
        ret_val = WOODSIDE_ERROR_MBIST_FAILURE;
    } else if (pass) {
        DIAG_PRINT(DIAG_PRINT_TEST_PROGRESS, "CI MBIST Passed\n")
    } else {
        printf("CI MBIST is still running. Increase timeout\n");
        ret_val = WOODSIDE_ERROR_MBIST_TIMEOUT;
    }
    if (ret_val)
        return (ret_val);
    *addr_error = ASIC(NF_STA_MBIST);
    if ((ret_val=asic_rd(inst,(*addr_error)*4, got_data))!=0) {
        return ret_val;
    }
    fail = ((*got_data) >> ASIC(NF_STA_MBIST_FLD_FAIL_SHIFT))&0x3; 
    pass = ((*got_data) >> ASIC(NF_STA_MBIST_FLD_PASS_SHIFT))&0x3; 
    if (fail) {
        if (fail == 1)
            printf("NF UCAST MBIST Failed\n");
        else if (fail == 2)
            printf("NF MCAST MBIST Failed\n");
        ret_val = WOODSIDE_ERROR_MBIST_FAILURE;
    } else if (pass == 0x3) {
        DIAG_PRINT(DIAG_PRINT_TEST_PROGRESS, "NF MBIST Passed\n")
    } else {
        printf("NF MBIST is still running. Increase timeout\n");
        ret_val = WOODSIDE_ERROR_MBIST_TIMEOUT;
    }
    return ret_val;
}
/*
 * Check NR Bist status
 */
static int nr_mbist_status (uint8_t inst, unsigned int *addr_error, 
                            unsigned int *got_data) 
{
    int ret_val = 0;
    unsigned int data1, data2,data3;
    unsigned int data4;
    data4 = 0x00;
    *addr_error = ASIC(NR_STA_MBIST_00);
    if ((ret_val=asic_rd(inst,(*addr_error)*4, &data1))!=0) {
        return ret_val;
    }
    *addr_error = ASIC(NR_STA_MBIST_01);
    if ((ret_val=asic_rd(inst,(*addr_error)*4, &data2))!=0) {
        return ret_val;
    }
    *addr_error = ASIC(NR_STA_MBIST_02);
    if ((ret_val=asic_rd(inst,(*addr_error)*4, &data3))!=0) {
        return ret_val;
    }
#if defined(DIAG_PORTOLA)
    *addr_error = ASIC(NR_STA_MBIST_03);
    if ((ret_val=asic_rd(inst,(*addr_error)*4, &data4))!=0) {
        return ret_val;
    }

    if (data3 || data4 || data2 >> 28) {
        printf("NR MBIST Failed\n");
        ret_val = WOODSIDE_ERROR_MBIST_FAILURE;
    } else if (((data2 & 0xFFFFFFF) == 0x7FBFDFE) && (data1 == 0xFFFFFFFF)) {
        DIAG_PRINT(DIAG_PRINT_TEST_PROGRESS, "NR MBIST Passed\n")
    } else {
        printf("NR MBIST is still running. Increase timeout\n");
        ret_val = WOODSIDE_ERROR_MBIST_TIMEOUT;
    }
#else
    if ((data1 == 0xbfffffff) && ((data2 & 0x7f) == 0x7f)) {
        DIAG_PRINT(DIAG_PRINT_TEST_PROGRESS, "NR MBIST Passed\n")
    } else if (data3 || (data2>>7)) {
        printf("NR MBIST Failed\n");
        ret_val = WOODSIDE_ERROR_MBIST_FAILURE;
    } else {
        printf("NR MBIST is still running. Increase timeout\n");
        ret_val = WOODSIDE_ERROR_MBIST_TIMEOUT;
    }
#endif
    return (ret_val);
}
/*
 * Check HR Bist status
 */
static int hr_mbist_status (uint8_t inst, unsigned int *addr_error, 
                            unsigned int *got_data) 
{
    int ret_val = 0;
    unsigned int pass = 0, fail = 0, passcheck = 0x0;
    *addr_error = ASIC(HR_STA_MBIST);
    if ((ret_val=asic_rd(inst,(*addr_error)*4, got_data))!=0) {
        return ret_val;
    }
#if defined(DIAG_WOODSIDE)
    fail = ((*got_data) >> ASIC(HR_STA_MBIST_FLD_FAIL_SHIFT))&0xff; 
    pass = ((*got_data) >> ASIC(HR_STA_MBIST_FLD_PASS_SHIFT))&0xff; 
    passcheck = 0xff;
#elif defined(DIAG_PORTOLA)
    fail = ((*got_data) >> ASIC(HR_STA_MBIST_FLD_FAIL_SHIFT))&0xfff; 
    pass = ((*got_data) >> ASIC(HR_STA_MBIST_FLD_PASS_SHIFT))&0xfff; 
    passcheck = 0xfff;
#endif

    if (pass == passcheck) {
        DIAG_PRINT(DIAG_PRINT_TEST_PROGRESS, "HR MBIST Passed\n")
    } else if (fail) {
        printf("HR MBIST Failed\n");
        ret_val = WOODSIDE_ERROR_MBIST_FAILURE;
    } else {
        printf("HR MBIST is still running. Increase timeout\n");
        ret_val = WOODSIDE_ERROR_MBIST_TIMEOUT;
    }
    return (ret_val);
}
static void hi_mbist_disable (int inst)  
{
    int hi;
    for (hi=0; hi < ASIC(HI_COUNT);hi++) {
        asic_wr(inst, BA(ASIC(HI0_CFG_MBIST) + 
                 ((ASIC(HI1_CFG_MBIST)-ASIC(HI0_CFG_MBIST))*hi)), 0x0); 
        asic_wr(inst, BA(ASIC(HI0_CFG_ARF_MBIST) + 
                 ((ASIC(HI1_CFG_ARF_MBIST)-ASIC(HI0_CFG_ARF_MBIST))*hi)), 0x0); 
    }
}
static void ni_mbist_disable (int inst)  
{
    int ni;
    for (ni=0; ni < ASIC(NI_COUNT);ni++) {
        asic_wr(inst, BA(ASIC(NI0_CFG_ARF_MBIST) + 
                 ((ASIC(NI1_CFG_ARF_MBIST)-ASIC(NI0_CFG_ARF_MBIST))*ni)), 0x0); 
    }
}
static void ss_mbist_disable (int inst)  
{
    int ss;
    for (ss=0; ss < ASIC(SS_COUNT);ss++) {
        asic_wr(inst, BA(ASIC(SS0_CFG_MBIST) + 
                 ((ASIC(SS1_CFG_MBIST)-ASIC(SS0_CFG_MBIST))*ss)), 0x0); 
    }
}
static void hf_mbist_disable (int inst)  
{
    int hf;
    for (hf=0; hf < ASIC(HF_COUNT);hf++) {
        asic_wr(inst, BA(ASIC(HF0_CFG_MBIST) + 
                 ((ASIC(HF1_CFG_MBIST)-ASIC(HF0_CFG_MBIST))*hf)), 0x0); 
    }
}

int asic_hw_mbist_test (int inst, uint8_t efuse_load, unsigned int* addr_error, 
                        unsigned int* exp_data, unsigned int* got_data)  { 
    int ret_val;
    DIAG_PRINT(DIAG_PRINT_TEST_PROGRESS, "Starting MBIST test ")

    /*
     * SPICO Memory bist in reset
     */
    if ((ret_val=asic_wr(inst,BA(ASIC(MISC_CORE_SRAM)), 0x9))!=0) {
        return ret_val;
    }
    /*
     * Remove Memories out of bist reset 
     */
    if ((ret_val=asic_wr(inst,BA(ASIC(MISC_CORE_SRAM)), 0x1))!=0) {
        return ret_val;
    }
     

    /* ==========================
       HI/NI/HF/NF/OQ/SS/HR/NR/CI mbist resets
       ========================== */
    /* TCAM MBIST */
    ret_val = tcam_mbist(inst, addr_error, exp_data, RESET_ACTION);
    if (ret_val) 
        return ret_val;
    ret_val = ss_cfg_mbist(inst, addr_error, exp_data, 0, 1);
    if (ret_val) 
        return ret_val;
    ret_val = nf_nr_hr_ci_cfg_mbist(inst, addr_error, exp_data, 0, 1);   
    if (ret_val) 
        return ret_val;
    ret_val = hf_cfg_mbist(inst, addr_error, exp_data, 0, 1);
    if (ret_val) 
        return ret_val;
    ret_val = hi_cfg_mbist(inst, addr_error, exp_data, 0, 1);
    if (ret_val) 
        return ret_val;
    ret_val = ni_cfg_mbist(inst, addr_error, exp_data, 0, 1);
    if (ret_val) 
        return ret_val;

    ret_val = ss_cfg_mbist(inst, addr_error, exp_data, 0, 0);
    if (ret_val) 
        return ret_val;
    ret_val = nf_nr_hr_ci_cfg_mbist(inst, addr_error, exp_data, 0, 0);   
    if (ret_val) 
        return ret_val;
    ret_val = hf_cfg_mbist(inst, addr_error, exp_data, 0, 0);
    if (ret_val) 
        return ret_val;
    ret_val = hi_cfg_mbist(inst, addr_error, exp_data, 0, 0);
    if (ret_val) 
        return ret_val;
    ret_val = ni_cfg_mbist(inst, addr_error, exp_data, 0, 0);
    if (ret_val) 
        return ret_val;

    if (efuse_load) {
        asic_fuse_load (inst);
        printf("  Woo-%d program FUSE RUN to be 0\n", inst);
    }
    usleep(1000 *1000);

    /* SPICO MBIST */
    if ((ret_val=asic_wr(inst,BA(ASIC(MISC_CORE_SRAM)), 0x10))!=0) {
        return ret_val;
    }

    /* ==========================
       HI/NI/HF/NF/OQ/SS/HR/NR/CI test, start the mbist run
       ========================== */
    /* TCAM MBIST */
    ret_val = tcam_mbist(inst, addr_error, exp_data, RUN_ACTION);
    if (ret_val) 
        return ret_val;
    ret_val = ss_cfg_mbist(inst, addr_error, exp_data, 1, 0);
    if (ret_val) 
        return ret_val;
    ret_val = nf_nr_hr_ci_cfg_mbist(inst, addr_error, exp_data, 1, 0);   
    if (ret_val) 
        return ret_val;
    ret_val = hf_cfg_mbist(inst, addr_error, exp_data, 1, 0);
    if (ret_val) 
        return ret_val;
    ret_val = hi_cfg_mbist(inst, addr_error, exp_data, 1, 0);
    if (ret_val) 
        return ret_val;
    ret_val = ni_cfg_mbist(inst, addr_error, exp_data, 1, 0);
    if (ret_val) 
        return ret_val;

    /* =================
       waiting game
       ================= */
    /*TODO: for now 1 second */
    sleep(1);
#if defined(DIAG_PORTOLA)
    /* SPICO Memories */
    if ((ret_val=asic_rd(inst,BA(ASIC(MISC_SRAM_BIST)), exp_data))!=0) {
        return ret_val;
    }
    if (*exp_data & (0x1 << ASIC(MISC_SRAM_BIST_FLD_DONE_FAIL_OUT))) {
            printf("SPICO DMEM MBIST Failed\n");
    } else if(*exp_data & (0x1 <<ASIC(MISC_SRAM_BIST_FLD_DONE_PASS_OUT))) {
        DIAG_PRINT(DIAG_PRINT_TEST_PROGRESS, "SPICO DMEM MBIST Passed\n")
    } else {
        printf("SPICO DMEM MBIST didn't complete in 1 second\n");
    }
    if (*exp_data & (0x1 << ASIC(MISC_SRAM_BIST_FLD_DONE_FAIL_OUT_IMEM))) {
            printf("SPICO IMEM MBIST Failed\n");
    } else if(*exp_data & (0x1<<ASIC(MISC_SRAM_BIST_FLD_DONE_PASS_OUT_IMEM))) {
        DIAG_PRINT(DIAG_PRINT_TEST_PROGRESS, "SPICO DMEM MBIST Passed\n")
    } else {
        printf("SPICO IMEM MBIST didn't complete in 1 second\n");
    }
#elif defined(DIAG_WOODSIDE)
    /* SPICO Memories */
    if ((ret_val=asic_rd(inst,BA(ASIC(MISC_STA_SPARE)), exp_data))!=0) {
        return ret_val;
    }
    if (*exp_data & 0xC) {
        if (*exp_data & 0x8) {
            printf("SPICO DMEM MBIST Failed\n");
        }
    } else {
        printf("SPICO DMEM MBIST didn't complete in 1 second\n");
    }
    if (*exp_data & 0x30) {
        if (*exp_data & 0x2) {
            printf("SPICO IMEM MBIST Failed\n");
        }
    } else {
        printf("SPICO IMEM MBIST didn't complete in 1 second\n");
    }
#endif
    /* TCAM MBIST */
    ret_val = tcam_mbist(inst, addr_error, exp_data, CHECK_ACTION);
    if (ret_val) 
        return ret_val;
    ret_val = ni_mbist_status(inst, addr_error, got_data);  
    if (ret_val)
        return (ret_val);
    /* 
     * AC policer, result
     */
    ret_val = hf_mbist_status(inst, addr_error, got_data);  
    if (ret_val)
        return (ret_val);
    ret_val = hi_mbist_status(inst, addr_error, got_data);  
    if (ret_val)
        return (ret_val);

    ret_val = ci_nf_mbist_status(inst, addr_error, got_data);  
    if (ret_val)
        return (ret_val);

    ret_val = nr_mbist_status(inst, addr_error, got_data);  
    if (ret_val)
        return (ret_val);

    ret_val = hr_mbist_status(inst, addr_error, got_data);  
    if (ret_val)
        return (ret_val);

    ret_val = ss_mbist_status(inst, addr_error, got_data);  
    DIAG_PRINT(DIAG_PRINT_TEST_PROGRESS, "===TEST COMPLETED===")

    /* ==========================
       HI/NI/HF/NF/OQ/SS/HR/NR/CI test, Disable mbist logic
       ========================== */
    /* TCAM MBIST */
    /*
     * SPICO Memory bist in reset
     */
    if ((ret_val=asic_wr(inst,BA(ASIC(MISC_CORE_SRAM)), 0x0))!=0) {
        return ret_val;
    }
    if ((ret_val=asic_wr(inst,BA(ASIC(CI_CFG_MBIST)), 0x0))!=0) {
        return ret_val;
    }
    hi_mbist_disable(inst);  
    ni_mbist_disable(inst);  
    ss_mbist_disable(inst);  
    hf_mbist_disable(inst);  
    if ((ret_val=asic_wr(inst,BA(ASIC(NF_CFG_MBIST)), 0x0))!=0) {
        return ret_val;
    }
    if ((ret_val=asic_wr(inst,BA(ASIC(NR_CFG_MBIST)), 0x0))!=0) {
        return ret_val;
    }
    if ((ret_val=asic_wr(inst,BA(ASIC(HR_CFG_MBIST)), 0x0))!=0) {
        return ret_val;
    }
    if ((ret_val=asic_wr(inst,BA(ASIC(HF0_CFG_TCAM_CORE_TCAM_BIST_RUN)), 
                         0x0))!=0) {
        return ret_val;
    }
    if ((ret_val=asic_wr(inst,BA(ASIC(HF1_CFG_TCAM_CORE_TCAM_BIST_RUN)), 
                         0x0))!=0) {
        return ret_val;
    }
    if ((ret_val=asic_wr(inst,BA(ASIC(HF2_CFG_TCAM_CORE_TCAM_BIST_RUN)), 
                         0x0))!=0) {
        return ret_val;
    }
    if ((ret_val=asic_wr(inst,BA(ASIC(HF3_CFG_TCAM_CORE_TCAM_BIST_RUN)), 
                         0x0))!=0) {
        return ret_val;
    }

    return (ret_val);
}
