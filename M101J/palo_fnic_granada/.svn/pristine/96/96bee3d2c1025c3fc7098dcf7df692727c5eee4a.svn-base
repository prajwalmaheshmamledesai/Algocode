
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ioctl.h>

#include "mic.h"

typedef enum mic_cfg_cmd_e {
    MIC_CFG_CMD_GET_STATUS,
    MIC_CFG_CMD_READ_I2C,
    MIC_CFG_CMD_WRITE_I2C,
    MIC_CFG_CMD_READ_MIC,
    MIC_CFG_CMD_WRITE_MIC,
    MIC_CFG_CMD_FLUSH_I2C,
} mic_cfg_cmd_t;


void
diag_mic_get_status()
{
    int i;
    mic_i2c_info_t info;
    char flag_str[8] = "";
    char fop_str[16] = "";

    printf("Dev  %-10s  %-10s  %-10s  %-10s  Flags  FailedOp\n",
           "Reads", "Writes", "Timeouts", "Errors");
   
    for (i = 0; i < MIC_I2C_DEV_MAX; i++)
    {
        memset(&info, 0, sizeof(mic_i2c_info_t));
        info.dev_id = i;

        if (mic_i2c_get_info(&info))
        {
            printf("Failed to get I2C info, dev %u\n", i);
            continue;
        }

        if (info.flags & MIC_S_I2C_TIMEOUT)
            strncat(flag_str, "T ", 8);
        if (info.flags & MIC_S_I2C_ERROR)
            strncat(flag_str, "E ", 8);
        if (info.flags & MIC_S_I2C_INPROG)
            strncat(flag_str, "P ", 8);

        sprintf(fop_str, "0x%06x", info.stats.failed_op.addr);
        if (info.stats.failed_op.addr & MIC_OP_READ)
            strncat(fop_str, " R", 16);
        if (info.stats.failed_op.addr & MIC_OP_AUTO_INCR)
            strncat(fop_str, " A", 16);

        printf("%-3u  %-10u  %-10u  %-10u  %-10u  %-5s  %-12s\n",
               i, info.stats.read_cnt, info.stats.write_cnt,
               info.stats.timeout_cnt, info.stats.error_cnt,
               flag_str, fop_str);
    }
}


void
diag_mic_i2c_read(uint8_t module_id, uint8_t dev_id, uint32_t addr, uint16_t n_regs, uint8_t auto_incr)
{
    mic_io_buf_t buf;
    mic_op_t ops[MIC_I2C_OP_MAX];
    int i;

    if (n_regs > MIC_I2C_OP_MAX)
    {
        printf("Dev %u: Cannot read more than %u registers\n",
               dev_id, MIC_I2C_OP_MAX);
        return;
    }

    memset(&buf, 0, sizeof(mic_io_buf_t));
    buf.module_id = module_id;
    buf.dev_id = dev_id;
    buf.n_ops = n_regs;
    buf.op = ops;

    for (i = 0; i < n_regs; i++)
    {
        memset(&ops[i], 0, sizeof(mic_op_t));
        ops[i].addr = addr + (i * 4);
        ops[i].addr |= MIC_OP_READ;

        if (auto_incr && i > 0)
            ops[i].addr |= MIC_OP_AUTO_INCR;
    }

    if (mic_io(&buf, 1))
    {
        printf("Dev %u: Read operation failed - %s\n", dev_id,
               (buf.flags & MIC_S_I2C_TIMEOUT) ? "I2C Timeout" :
               (buf.flags & MIC_S_I2C_ERROR) ? "I2C Error" : "Unknown Error");
        return;
    }

    printf("Dev %u:\n", dev_id);
    for (i = 0; i < n_regs; i++)
        printf("0x%06x: 0x%08x\n", (ops[i].addr & 0xffffff), ops[i].data);

    return;
}


void
diag_mic_i2c_serial_read(uint8_t module_id, uint8_t dev_id, uint32_t addr, uint16_t n_regs)
{
    mic_serial_op_t ops[MIC_I2C_OP_MAX];
    int i;

    if (n_regs > MIC_I2C_OP_MAX)
    {
        printf("Dev %u: Cannot read more than %u registers\n",
               dev_id, MIC_I2C_OP_MAX);
        return;
    }

    for (i = 0; i < n_regs; i++)
    {
        memset(&ops[i], 0, sizeof(mic_serial_op_t));

        ops[i].module_id = module_id;
        ops[i].dev_id = dev_id;
        ops[i].op.addr = addr + (i * 4);
        ops[i].op.addr |= MIC_OP_READ;
    }

    if (mic_serial_io(ops, n_regs))
    {
        printf("Dev %u: Serial read operation failed\n", dev_id);
        return;
    }

    printf("Dev %u:\n", dev_id);
    for (i = 0; i < n_regs; i++)
        printf("0x%06x: 0x%08x\n", (ops[i].op.addr & 0xffffff), ops[i].op.data);

    return;
}


void
diag_mic_i2c_write(uint8_t module_id, uint8_t dev_id, uint32_t addr, uint32_t data)
{
    mic_io_buf_t buf;
    mic_op_t op;

    memset(&buf, 0, sizeof(mic_io_buf_t));
    buf.dev_id = dev_id;
    buf.n_ops = 1;
    buf.op = &op;

    memset(&op, 0, sizeof(mic_op_t));
    op.addr = addr;
    op.data = data;

    if (mic_io(&buf, 1))
    {
        printf("Dev %u: Write operation failed - %s\n", dev_id,
               (buf.flags & MIC_S_I2C_TIMEOUT) ? "I2C Timeout" :
               (buf.flags & MIC_S_I2C_ERROR) ? "I2C Error" : "Unknown Error");
        return;
    }

    return;
}


void
diag_mic_read(uint32_t addr, uint32_t *data)
{
    mic_op_t ops;

    memset(&ops, 0, sizeof(mic_op_t));
    ops.addr = addr;
    ops.addr |= MIC_OP_READ;

    if (mic_reg_access(&ops, 1))
    {
        printf("MIC read operation failed\n");
        return;
    }

    *data = ops.data;
    return;
}


void
diag_mic_write(uint32_t addr, uint32_t data)
{
    mic_op_t op;

    memset(&op, 0, sizeof(mic_op_t));
    op.addr = addr;
    op.data = data;

    if (mic_reg_access(&op, 1))
    {
        printf("Write operation failed\n");
        return;
    }

    return;
}


void
diag_mic_i2c_flush(uint8_t module_id, uint8_t dev_id)
{
    if (mic_i2c_flush(module_id, dev_id))
    {
        printf("Dev %u: I2C flush failed\n", dev_id);
        return;
    }

    return;
}

#if 0
static void
print_usage()
{
    printf("Usage: -h, print help\n");
    printf("       -c <command>,   0: get status (default)\n");
    printf("                       1: read I2C register\n");
    printf("                       2: write I2C register\n");
    printf("                       3: read MIC register\n");
    printf("                       4: write MIC register\n");
    printf("                       5: flush I2C\n");
    printf("       -m <module_id>, module ID (default: 0)\n");
    printf("       -i <dev_id>, device ID (default: 0)\n");
    printf("       -a <addr>, register address (default: 0)\n");
    printf("       -d <data>, register data (default: 0)\n");
    printf("       -n <n_regs>, number of registers (default: 1)\n");
    printf("       -s <serial_io>, use serial I/O (default: NO)\n");
    printf("       -t <auto_incr>, auto increment I2C address (default: NO)\n");
};


static const struct option long_options[] =
{
    {"help", no_argument, 0, 'h'},
    {"command", optional_argument, 0, 'c'},
    {"module_id", optional_argument, 0, 'm'},
    {"dev_id", optional_argument, 0, 'i'},
    {"addr", optional_argument, 0, 'a'},
    {"data", optional_argument, 0, 'd'},
    {"n_regs", optional_argument, 0, 'n'},
    {"serial_io", optional_argument, 0, 's'},
    {"auto_incr", optional_argument, 0, 't'},
};


static const char short_options[] = "hc:m:i:a:d:n:st";


int
main(int argc, char *argv[])
{
    int c;
    uint8_t cmd = MIC_CFG_CMD_GET_STATUS;
    uint8_t module_id = 0;
    uint8_t dev_id = 0;
    uint32_t addr = 0;
    uint32_t data = 0;
    uint16_t n_regs = 1;
    bool serial_io = FALSE;
    bool auto_incr = FALSE;

    while ((c = getopt_long(argc, argv,
                            short_options, long_options, NULL)) != -1)
    {
        switch(c)
        {
            case 'h':
                print_usage();
                exit(0);

            case 'c':
                cmd = strtoul(optarg, NULL, 0);
                break;

            case 'i':
                dev_id = strtoul(optarg, NULL, 0);
                break;

            case 'a':
                addr = strtoul(optarg, NULL, 0);
                break;

            case 'd':
                data = strtoul(optarg, NULL, 0);
                break;

            case 'n':
                n_regs = strtoul(optarg, NULL, 0);
                break;

            case 's':
                serial_io = TRUE;
                break;

            case 't':
                auto_incr = TRUE;
                break;

            default:
                print_usage();
                exit(-1);
        }
    }

    switch(cmd)
    {
        case MIC_CFG_CMD_GET_STATUS:
            get_status();
            break;

        case MIC_CFG_CMD_READ_I2C:
            if (serial_io)
                i2c_serial_read(module_id, dev_id, addr, n_regs);
            else
                i2c_read(module_id, dev_id, addr, n_regs, auto_incr);
            break;

        case MIC_CFG_CMD_WRITE_I2C:
            i2c_write(module_id, dev_id, addr, data);
            break;

        case MIC_CFG_CMD_READ_MIC:
            mic_read(addr, n_regs);
            break;

        case MIC_CFG_CMD_WRITE_MIC:
            mic_write(addr, data);
            break;

        case MIC_CFG_CMD_FLUSH_I2C:
            i2c_flush(module_id, dev_id);
            break;

        default:
            printf("Invalid command\n");
            exit(-1);
    }

    exit(0);
}
#endif
