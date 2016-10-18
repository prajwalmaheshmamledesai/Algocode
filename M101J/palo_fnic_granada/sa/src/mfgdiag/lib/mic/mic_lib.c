
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/fcntl.h>
#include <sys/mman.h> 

#include "mic.h"
#include "mic_reg.h"
//#include <platform/mic_regmap.h>

#define REG_DEBUG

typedef struct libmic_env_s {
    int dev_fd;
} libmic_env_t;


libmic_env_t mic_env = {
   .dev_fd = -1,
};

#ifdef REG_DEBUG
// deubg code for register access through /dev/mem
// map physical address
volatile void* map_physical_memory(unsigned int addr)
{
#ifdef DEBUG
    return (void*)NULL;
#endif
    // Open the physical memory - need root access for this
    int fd = open("/dev/mem", O_RDWR);
    if(fd < 0)
    {
        perror("can't open /dev/mem");
        return (void*)0xffffffff;
    }
     
    // Split the address into a base and offset
    unsigned int pagesize = getpagesize();
    unsigned int phys_base = addr & ~(pagesize - 1);
    unsigned int offset = (addr & (pagesize - 1));
  
    // Map a single page
    void* virt_base = mmap(0, 
                           pagesize, 
                           PROT_READ|PROT_WRITE, 
                           MAP_SHARED, 
                           fd, 
                           phys_base);

    // Don't need to keep fd open;
    close(fd); 

    if(virt_base ==  MAP_FAILED)
    {
        perror("mmap failed");
        return (void*)0xffffffff;
    }
    else
        return virt_base + offset;
}

int unmap_physical_memory(void* addr)
{
#ifdef DEBUG
    return 0;
#endif
    // Split the address into a base and offset
    unsigned int pagesize = getpagesize();
    void* virt_base = (void*)((unsigned int)addr & ~(pagesize - 1));
  
    // Map a single page
    if(munmap(virt_base, pagesize))
    {
        perror("munmap failed");
        return -1;
    }
    return 0;
} 

#endif


static int
mic_do_ioctl(mic_ioctl_data_t *ioctl_data)
{
    int rc = 0;
    char *o_path = "/dev/mic";

    if (mic_env.dev_fd < 0)
    {

        mic_env.dev_fd = open("/dev/"MIC_DEV_NAME, O_WRONLY);
		
        if (mic_env.dev_fd < 0)
        {
            LIBMIC_ERR("failed to open device %s, %s\n",o_path, strerror(errno));
            return mic_env.dev_fd;
        }
    }

    rc = ioctl(mic_env.dev_fd, MIC_IOC, ioctl_data);
    if (rc < 0)
    {
        LIBMIC_ERR("ioctl() failed, %s\n", strerror(errno));
        return rc;
    }
#if 0
#ifdef REG_DEBUG
    {
	int i;

	unsigned int *micBaseAddr;
	unsigned int value;
	micBaseAddr = (unsigned int*) map_physical_memory(0xf0000000);

	*((unsigned int*)(((unsigned int) micBaseAddr)+0xe4)) = 0xffffffff;
	*((unsigned int*)(((unsigned int) micBaseAddr)+0xe8)) = 0xabcdef00;

	for (i=0; i< 0x0fc; i+=4) {
		value = *((unsigned int*)(((unsigned int) micBaseAddr)+i));
		printf(" 0x%8.8x :  0x%8.8x \n", (unsigned int)(((unsigned int)micBaseAddr)+i), value);
	}
	unmap_physical_memory(micBaseAddr);
     }
#endif
#endif

    return rc;
}


static int
mic_i2c_do_ioctl(mic_i2c_ioctl_data_t *ioctl_data)
{
    int rc = 0;

    if (mic_env.dev_fd < 0)
    {
        mic_env.dev_fd = open("/dev/"MIC_DEV_NAME, O_WRONLY);
        if (mic_env.dev_fd < 0)
        {
            LIBMIC_ERR("failed to open device, %s\n", strerror(errno));
            return mic_env.dev_fd;
        }
    }

    rc = ioctl(mic_env.dev_fd, MIC_IOC_I2C, ioctl_data);
    if (rc < 0)
    {
        LIBMIC_ERR("ioctl() failed, %s\n", strerror(errno));
        return rc;
    }

    return rc;
}


int
mic_io(mic_io_buf_t *buf, uint8_t n_bufs)
{
    mic_ioctl_data_t data;
    int rc = 0;

    if (n_bufs > MIC_I2C_DEV_MAX)
    {
        LIBMIC_ERR("invalid number of bufs %u\n", n_bufs);
        return -1;
    }

    memset(&data, 0, sizeof(mic_ioctl_data_t));
    data.cmd = MIC_CMD_IO;
    data.u.io_info.n_bufs = n_bufs;
    data.u.io_info.buf = buf;

    rc = mic_do_ioctl(&data);
    if (rc < 0)
    {
        LIBMIC_ERR("failed to perform I/O, err %d\n", rc);
        return rc;
    }

    return rc;
}


int
mic_serial_io(mic_serial_op_t *op, uint16_t n_ops)
{
    mic_ioctl_data_t data;
    int rc = 0;

    if (n_ops > MIC_I2C_OP_MAX)
    {
        LIBMIC_ERR("invalid number of ops %u\n", n_ops);
        return -1;
    }

    memset(&data, 0, sizeof(mic_ioctl_data_t));
    data.cmd = MIC_CMD_SERIAL_IO;
    data.u.sio_info.n_ops = n_ops;
    data.u.sio_info.op = op;

    rc = mic_do_ioctl(&data);
    if (rc < 0)
    {
        LIBMIC_ERR("failed to perform serial I/O, err %d\n", rc);
        return rc;
    }

    return rc;
}


int
mic_i2c_get_status(uint16_t *status_bmp)
{
    mic_ioctl_data_t data;
    int rc = 0;

    memset(&data, 0, sizeof(mic_ioctl_data_t));
    data.cmd = MIC_CMD_GET_STATUS;

    rc = mic_do_ioctl(&data);
    if (rc < 0)
    {
        LIBMIC_ERR("failed to get I2C status, err %d\n", rc);
        return rc;
    }

    if (status_bmp)
        *status_bmp = data.u.status_info.i2c_timeout_bmp |
                      data.u.status_info.i2c_error_bmp;

    return rc;
}


int
mic_i2c_get_info(mic_i2c_info_t *i2c_info)
{
    mic_i2c_ioctl_data_t data;
    int rc = 0;

    if (i2c_info->dev_id >= MIC_I2C_DEV_MAX)
    {
        LIBMIC_ERR("invalid dev_id %u\n", i2c_info->dev_id);
        return -1;
    }

    memset(&data, 0, sizeof(mic_i2c_ioctl_data_t));
    data.cmd = MIC_CMD_GET_I2C_INFO;
    data.i2c_info.module_id = i2c_info->module_id;
    data.i2c_info.dev_id = i2c_info->dev_id;

    rc = mic_i2c_do_ioctl(&data);
    if (rc < 0)
    {
        LIBMIC_ERR("failed to get I2C info, err %d\n", rc);
        return rc;
    }

    memcpy(i2c_info, &data.i2c_info, sizeof(mic_i2c_ioctl_data_t));

    return rc;
}


int
mic_i2c_flush(uint8_t module_id, uint8_t dev_id)
{
    mic_i2c_ioctl_data_t data;
    int rc = 0;

    if (dev_id >= MIC_I2C_DEV_MAX)
    {
        LIBMIC_ERR("invalid dev_id %u\n", dev_id);
        return -1;
    }

    memset(&data, 0, sizeof(mic_i2c_ioctl_data_t));
    data.cmd = MIC_CMD_I2C_FLUSH;
    data.i2c_info.module_id = module_id;
    data.i2c_info.dev_id = dev_id;

    rc = mic_i2c_do_ioctl(&data);
    if (rc < 0)
    {
        LIBMIC_ERR("failed to get I2C info, err %d\n", rc);
        return rc;
    }

    return rc;
}


int
mic_reg_access(mic_op_t *op, uint16_t n_ops)
{
    mic_io_buf_t buf;
    mic_ioctl_data_t data;
    int rc = 0;

    if (n_ops > MIC_I2C_OP_MAX)
    {
        LIBMIC_ERR("invalid number of ops %u\n", n_ops);
        return -1;
    }

    memset(&data, 0, sizeof(mic_ioctl_data_t));
    data.cmd = MIC_CMD_REG_ACCESS;
    data.u.io_info.n_bufs = 1;
    data.u.io_info.buf = &buf;

    memset(&buf, 0, sizeof(mic_io_buf_t));
    buf.op = op;
    buf.n_ops = n_ops;

    rc = mic_do_ioctl(&data);
    if (rc < 0)
    {
        LIBMIC_ERR("failed to register access, err %d\n", rc);
        return rc;
    }

    return rc;
}


int
mic_led_block_reg_access(mic_op_t *op, uint16_t n_ops)
{
    int i;

    for (i = 0; i < n_ops; i++)
    {
        if (op[i].addr < MIC_REG_LED_BLOCK_START ||
            op[i].addr > MIC_REG_LED_BLOCK_END)
        {
            LIBMIC_ERR("address 0x%08x out of range\n", op[i].addr);
            return -EINVAL;
        }
    }

    return mic_reg_access(op, n_ops);
}
