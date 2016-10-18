
#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>
#include <errno.h>
#include <string.h>

#include "sprom.h"
#include "diag_sprom.h"
#include "ppc_lib.h"
#include "at24c512_lib.h"

#include "diag_fex.h"
int sprom_rd (diag_smb_dev_t *smb_dev, uint32_t offset, uint32_t len, uint8_t *buf)
{
        int     fd;
        int     rc;
	char	dev_path[80];

	// This is a temporary hack. NEEDS to be fixed.	
	if (smb_dev->addr == FEX_BRD_SPROM_I2C_ADDR) {
		sprintf(dev_path, "/sys/bus/i2c/devices/%d-%04x/eeprom", 
			smb_dev->path, smb_dev->addr);
		// bernward
		fd = open(dev_path, O_RDONLY);
	        if (fd < 0) {
			printf("Unable to open %s (%s)\n", 
				smb_dev->bus, dev_path);
	                return (0);
		}

		lseek (fd, smb_dev->offset+offset, SEEK_SET); 
		rc = read(fd, buf, len);
		if (rc==-1) 
			printf("rc %d, len=%d errno = %s\n", rc, len, strerror(errno));

		close(fd);

		return ((rc == (int)len) ? 0 : -1);
	} else {
		sprintf ( dev_path, "/dev/i2c-%d",smb_dev->path);
		fd = open ( dev_path, O_RDONLY );
		if ( fd < 0 ) {
			fprintf ( stderr, "Unable to open %s (%s)\n",
				smb_dev->bus, dev_path );
			return ( -ENODEV );
		}
		rc = diag_i2c_rd ( fd, smb_dev->addr, smb_dev->offset + offset, 
			smb_dev->asize, buf, len );
		close(fd);
	}
        return (rc);
}

int sprom_wr (diag_smb_dev_t *smb_dev, uint32_t offset, uint32_t len, uint8_t *buf)
{
        int     rc;
	char	dev_path[80];

	// This is a temporary hack. NEEDS to be fixed.	
	if (smb_dev->addr == FEX_BRD_SPROM_I2C_ADDR) {
        FILE    *fd;
	sprintf(dev_path, "/sys/bus/i2c/devices/%d-%04x/eeprom", 
			smb_dev->path, smb_dev->addr);
        fd = fopen(dev_path, "w");
        if (fd == NULL) {
                printf("Unable to open %s\n", smb_dev->bus);
                return (0);
        }

	fseek (fd, smb_dev->offset+offset, SEEK_SET); 
        rc = fwrite(buf, len, 1, fd);
        fclose(fd);

        return ((rc == 1) ? 0 : -1);
	} else {
         int    fd;
         sprintf ( dev_path, "/dev/i2c-%d", smb_dev->path);
         fd = open ( dev_path, O_RDWR );
         if ( fd < 0 ) {
                 fprintf ( stderr, "Unable to open %s (%s)\n",
                         smb_dev->bus, dev_path );
                 return ( -ENODEV );
         }
         rc = diag_i2c_wr (fd, smb_dev->addr, smb_dev->offset + offset, 
			   smb_dev->asize, buf, len );
         close(fd);
	}
        return (rc);
}

#ifdef DIAG_FEX
int sprom_psu_wr (diag_smb_dev_t *smb_dev, uint32_t offset, uint32_t len, uint8_t *buf)
{
        int     rc = 0;
	char	dev_path[80];
        int    fd ;
	uint32_t cnt=0;

        sprintf ( dev_path, "/dev/i2c-%d", smb_dev->path);
        fd = open ( dev_path, O_RDWR );
        if ( fd < 0 ) {
                fprintf ( stderr, "Unable to open %s (%s)\n",
                        smb_dev->bus, dev_path );
                return ( -ENODEV );
        }
	for (cnt = 0; cnt < len; cnt++) {
        	rc = diag_psu_i2c_wr (fd, smb_dev->addr, 
			smb_dev->offset + offset+cnt, 
		   	1, &buf[cnt], 1 );
		usleep(10000);
	}
        close(fd);
        return (rc);
}
#endif

int diag_sprom_rd (diag_sprom_t *sprom, uint32_t offset, uint32_t len, uint8_t *buf)
{
	assert (sprom);
	assert (sprom->smb_acc);
	assert (buf);
	assert (sprom->smb_acc->smb_rd);

	return (sprom->smb_acc->smb_rd(sprom->smb_acc->smb_dev, offset, len, buf));
}

int diag_sprom_wr (diag_sprom_t *sprom, uint32_t offset, uint32_t len, uint8_t *buf)
{
	assert (sprom);
	assert (sprom->smb_acc);
	assert (buf);
	assert (sprom->smb_acc->smb_rd);
#ifdef DIAG_FEX
	if (!strncasecmp(sprom->name, "PS", strlen("PS"))) {
		return (sprom_psu_wr (sprom->smb_acc->smb_dev, offset, len, buf));
	} else 
#endif
	{
		return (sprom->smb_acc->smb_wr(sprom->smb_acc->smb_dev, offset, len, buf));
	}
}
