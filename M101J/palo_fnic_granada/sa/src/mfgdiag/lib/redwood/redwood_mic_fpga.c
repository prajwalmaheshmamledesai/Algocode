 /********************************************************************
 *
 *      File:   redwood_mic_fpga.c
 *      Name:   Bruce McLoughlin
 *
 *      Description: Functions to operate on the MIC FPGA for
 *                   FEX.
 *
 *
 *      Copyright (c) 2001,2002,2003,2004 by Nuova Systems, Inc.
 ********************************************************************/
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include "mic.h"
#include "redwood_mic_fpga.h"

#define RDWD_LOG	printf
 
extern int port_get_front_port( int asic_id, int asic_port );

int mic_flush_xns(void);
int mic_verify_xns(void);

mic_op_t mic_op[ MIC_I2C_DEV_MAX ][ MIC_I2C_OP_MAX ];

#define MIC_IO_BUF( module, asic ) \
    { module, asic, 0 /*flags*/, 0 /*n_ops*/, &mic_op[asic][0] }

int max_pending_xns = 1; /*MIC_I2C_OP_MAX*/

mic_io_buf_t mic_io_buf[ MIC_I2C_DEV_MAX ] = {
    MIC_IO_BUF( 0, 0 ),
    MIC_IO_BUF( 0, 1 ),
    MIC_IO_BUF( 0, 2 ),
    MIC_IO_BUF( 0, 3 ),
    MIC_IO_BUF( 0, 4 ),
    MIC_IO_BUF( 0, 5 ),
    MIC_IO_BUF( 0, 6 ),
    MIC_IO_BUF( 0, 7 ),
};


/****************************************************************************
* mic_read_xn
****************************************************************************/
int mic_read_xn( uint32_t  asic_id, uint32_t  reg, uint32_t *data )
{
    int err = 0;
    int n = mic_io_buf[ asic_id ].n_ops;

    mic_io_buf[ asic_id ].op[ n ].addr = reg | MIC_OP_READ;
    mic_io_buf[ asic_id ].op[ n ].data = 0;
    mic_io_buf[ asic_id ].n_ops++;

    /*flush all pending operations*/
    err = mic_flush_xns();
    if (err == 0) {
        *data = mic_io_buf[ asic_id ].op[ n ].data;
        return 0;
    }
    else {
        RDWD_LOG("MIC: Error <%d> on Read of %d-%xh\n",
                 err, asic_id, reg );
        *data = 0xFFFFFFFF;
        return 1; // error
    }
}

/****************************************************************************
* mic_write_xn
****************************************************************************/
int mic_write_xn( uint32_t  asic_id, uint32_t  reg, uint32_t data )
{
    int err = 0;
    int n = mic_io_buf[ asic_id ].n_ops;

    mic_io_buf[ asic_id ].op[n].addr = reg;
    mic_io_buf[ asic_id ].op[n].data = data;
    mic_io_buf[ asic_id ].n_ops++;

    if (mic_io_buf[ asic_id ].n_ops >= max_pending_xns) {

        /* flush all pending operations */
        err = mic_flush_xns();
        if (err != 0) {
            RDWD_LOG("MIC: Error <%d> on Write (Flush) of %d-%xh = %xh\n",
                 err, asic_id, reg, data );
        }
    }
    return err;
}

/****************************************************************************
* mic_read_seq_xn
****************************************************************************/
int mic_read_seq_xn( uint32_t  asic_id, uint32_t  reg, uint32_t *data, int n_words )
{
    int err;
    int i;

    /* first, flush an pending writes */
    mic_flush_xns();
    
    for (i = 0; i < n_words; i++) {
        if (i == 0)
            mic_io_buf[ asic_id ].op[ i ].addr = reg | MIC_OP_READ;
        else
            mic_io_buf[ asic_id ].op[ i ].addr = MIC_OP_AUTO_INCR;

        mic_io_buf[ asic_id ].op[ i ].data = 0;
        mic_io_buf[ asic_id ].n_ops++;
    }

    err = mic_flush_xns();
    if (err == 0) {
        for (i = 0; i < n_words; i++) {
            *(data++) = mic_io_buf[ asic_id ].op[ i ].data;
        }
        return 0;
    }
    else {
        RDWD_LOG("MIC: Error <%d> on Read-Seq of %d-%xh (for %d)\n",
                 err, asic_id, reg, n_words );
        memset((char*)data, 0xFF, n_words*4);
        return 1; // error
    }
}

/****************************************************************************
* mic_flush_xns
****************************************************************************/
int mic_flush_xns(void)
{
    int asic_id;
    int err = 0;

    /* make sure there's one pending */
    for (asic_id = 0; asic_id < MIC_I2C_DEV_MAX; asic_id++) {

        if (mic_io_buf[ asic_id ].n_ops > 0) {
            /* found one.. */
            err = mic_io( &mic_io_buf[0], MIC_I2C_DEV_MAX );
            if (err == 0) {
                mic_verify_xns(); /* see how they went */
            }
            else {
               RDWD_LOG("MIC: Error <%d> Flushing XNs\n", err );
            }
            /* re-init the IO buffers */
            for (asic_id = 0; asic_id < MIC_I2C_DEV_MAX; asic_id++) {
                mic_io_buf[ asic_id ].n_ops = 0;
            }
            break;
        }
    }
    return err;
}

/****************************************************************************
* mic_verify_xns
****************************************************************************/
int mic_verify_xns()
{
    int err;
    uint16_t status_bmp = 0;

    err = mic_i2c_get_status( &status_bmp );
    if (err) {
        RDWD_LOG("MIC: Error <%d> getting I2C error status\n", err);
        return 0;
    }
    else {
        int asic_id;

        for (asic_id = 0; asic_id < MIC_I2C_DEV_MAX; asic_id++) {
            if (status_bmp & (1 << asic_id)) {
                RDWD_LOG("MIC: Error/Tout on ASIC %d Bus\n", asic_id);
            }
        }
    }

    return 0;
}

#if 0
mic_op_t led_regs[4] = {
    { MIC_REG_LED01, 0 },
    { MIC_REG_LED23, 0 },
    { MIC_REG_LED45, 0 },
    { MIC_REG_LEDCTL, 1 /*force scan-out*/ }
};

/* indexed by array[front_port/8].(1 << (front_port % 8)) */
unsigned char green_leds[48/8]  = {0};
unsigned char yellow_leds[48/8] = {0};

/****************************************************************************
* mic_set_port_led
*
* Colors are mapped as follows:
*
*               yellow        green
* LED_AMBER:     On           On
* LED_GREEN:     Off          On
* LED_ORANGE:    On           Off
* LED_OFF:       Off          Off
****************************************************************************/
void mic_set_port_led( int asic_id, int asic_port, int color )
{
    int array_idx, shift, err;
    int front_port = port_get_front_port( asic_id, asic_port );

    if (front_port == -1) return; // non-front-port (already logged as error)

    if (front_port > 47) {
        RDWD_LOG("MIC: Error: Invald front_port returned <%d>\n", front_port);
        return;
    }
    array_idx = front_port/8;
    shift     = (1 << (front_port % 8));

    /* Update LED arrays */
    switch( color ) {
        case LED_AMBER:
            yellow_leds[ array_idx ] |=  (1 << shift); 
            green_leds[  array_idx ] |=  (1 << shift); 
            break;
        case LED_GREEN:
            yellow_leds[ array_idx ] &= ~(1 << shift); 
            green_leds[  array_idx ] |=  (1 << shift); 
            break;
        case LED_ORANGE:
            yellow_leds[ array_idx ] |=  (1 << shift); 
            green_leds[  array_idx ] &= ~(1 << shift); 
            break;
        case LED_OFF:
            yellow_leds[ array_idx ] &= ~(1 << shift); 
            green_leds[  array_idx ] &= ~(1 << shift); 
            break;
    }
    /* program update and force a scan-out */
    led_regs[0].data = ((green_leds[1] << 24) |
                        (yellow_leds[1]<< 16) |
                        (green_leds[0] <<  8) |
                        (yellow_leds[0]<<  0));

    led_regs[1].data = ((green_leds[3] << 24) |
                        (yellow_leds[3]<< 16) |
                        (green_leds[2] <<  8) |
                        (yellow_leds[2]<<  0));

    led_regs[2].data = ((green_leds[5] << 24) |
                        (yellow_leds[5]<< 16) |
                        (green_leds[4] <<  8) |
                        (yellow_leds[4]<<  0));

    err = mic_reg_access( &led_regs[0], 4);
    if (err != 0) {
        RDWD_LOG("MIC: Error <%d> programming LEDs\n", err);
    }
}
#endif
