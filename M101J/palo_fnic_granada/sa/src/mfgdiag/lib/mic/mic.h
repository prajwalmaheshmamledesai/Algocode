
#ifndef __MIC_H__
#define __MIC_H__

#define MIC_DEV_NAME        "mic"

#define MIC_I2C_DEV_MAX     8           // max number of slave devices.
#define MIC_I2C_OP_MAX      256         // max ops per device in a transaction.

#define LIBMIC_ERR(fmt, args...)        \
        printf("%s: " fmt, __FUNCTION__, ## args)


/* I/O operation.
 */
typedef struct mic_op_s {
    uint32_t addr;        // register address - 3 LSB's contain the address.
                        // MSB is used to specify options.
#define MIC_OP_READ         0x01000000  // read operation.
#define MIC_OP_AUTO_INCR    0x02000000  // auto increment address.
    uint32_t data;        // read or write data.
} mic_op_t;


/* I/O buffer.
 */
typedef struct mic_io_buf_s {
    uint8_t module_id;    // module id.
    uint8_t dev_id;       // device id.
    uint8_t flags;        // status flags.
#define MIC_S_I2C_TIMEOUT   0x01        // i2c operation timed out.
#define MIC_S_I2C_INPROG    0x02        // i2c operation in progress.
#define MIC_S_I2C_ERROR     0x03        // i2c error encountered.
#define MIC_S_COPYBACK      0x04        // copy back data (used by the driver).
    uint16_t n_ops;       // number of operations in the array.
    mic_op_t *op;       // array of operarions.
} mic_io_buf_t;


/* serial I/O operation.
 */
typedef struct mic_serial_op_s {
    uint8_t module_id;    // module id.
    uint8_t dev_id;       // device id.
    uint8_t flags;        // status flags.
    mic_op_t op;        // operation.
} mic_serial_op_t;


/* ioctl commands.
 */
#define MIC_IOC_MAGIC           'm'
#define MIC_IOC                 _IO(MIC_IOC_MAGIC, 0)
#define MIC_IOC_I2C             _IO(MIC_IOC_MAGIC, 1)

typedef enum mic_cmd_e {
    MIC_CMD_I2C_FLUSH,      // flush i2c bus.
    MIC_CMD_IO,             // perform I/O.
    MIC_CMD_SERIAL_IO,      // perform serial I/O.
    MIC_CMD_GET_STATUS,     // get device status.
    MIC_CMD_GET_I2C_INFO,   // get i2c info.
    MIC_CMD_REG_ACCESS,     // mic register access.
} mic_cmd_t;


/* io buf info.
 */
typedef struct mic_io_info_s {
    uint8_t n_bufs;
    mic_io_buf_t *buf;
} mic_io_info_t;


/* serial io buf info.
 */
typedef struct mic_serial_io_info_s {
    uint16_t n_ops;
    mic_serial_op_t *op;
} mic_serial_io_info_t;


/* status info.
 */
typedef struct mic_status_info_s {
    uint16_t i2c_timeout_bmp;     // bitmap of i2c controllers in timeout state.
    uint16_t i2c_error_bmp;       // bitmap of i2c controllers in error state.
} mic_status_info_t;


/* ioctl data.
 */
typedef struct mic_ioctl_data_s {
    uint8_t cmd;
    union {
        mic_io_info_t io_info;
        mic_serial_io_info_t sio_info;
        mic_status_info_t status_info;
    } u;
} mic_ioctl_data_t;


/* i2c statistics.
 */
typedef struct mic_i2c_stats_s {
    uint32_t read_cnt;    // number of read operations.
    uint32_t write_cnt;   // number of write operations.
    uint32_t auto_incr_cnt;       // number of ops with auto increment option.
    uint32_t timeout_cnt;         // number of timeouts encountered.
    uint32_t error_cnt;           // number of errors encountered.
    uint32_t flush_cnt;           // number of times the bus is flushed.
    mic_op_t failed_op;         // operation which caused failure.
} mic_i2c_stats_t;


/* i2c info.
 */
typedef struct mic_i2c_info_s {
    uint8_t module_id;    // module id.
    uint8_t dev_id;       // device id.
    uint8_t flags;        // status flags.
    mic_i2c_stats_t stats;  // statistics.
} mic_i2c_info_t;
    

/* i2c ioctl data.
 */
typedef struct mic_i2c_ioctl_data_s {
    uint8_t cmd;
    mic_i2c_info_t i2c_info;
} mic_i2c_ioctl_data_t;


/* API's.
 */

/* Flush outstanding I2C transaction on a slave device.
 */
int
mic_i2c_flush(uint8_t module_id, uint8_t dev_id);


/* Get I2C status bitmap.
 *
 * Bit 0 indicates the status of device 0, bit 1 indicates device and so on:
 *   0 - No error.
 *   1 - I2C error or timeout.
 */
int
mic_i2c_get_status(uint16_t *status_bmp);


/* Get I2C info.
 */
int
mic_i2c_get_info(mic_i2c_info_t *i2c_info);


/* Perform I/O.
 * 
 * The driver may try to parallelize I/O access between slave devices.
 */
int
mic_io(mic_io_buf_t *buf, uint8_t n_bufs);


/* Perform serial I/O.
 *
 * All the accesses are serialized.
 */
int
mic_serial_io(mic_serial_op_t *op, uint16_t n_ops);


/* Perform MIC register access.
 */
int
mic_reg_access(mic_op_t *op, uint16_t n_ops);


/* Perform MIC LED block register access.
 */
int
mic_led_block_reg_access(mic_op_t *op, uint16_t n_ops);


#endif // __MIC_H__
