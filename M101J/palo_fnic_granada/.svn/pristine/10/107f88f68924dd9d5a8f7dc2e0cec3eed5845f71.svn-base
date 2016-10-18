
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>
#include <errno.h>
#include <string.h>

#include "diag_sprom.h"

int sprom_rd (diag_smb_dev_t *smb_dev, uint32_t offset, 
		uint32_t len, uint8_t *buf)
{
        int     fd;
        int     rc;
	char	dev_path[80];

	sprintf(dev_path, "/sys/bus/i2c/devices/%d-%04x/eeprom", 
			smb_dev->path, smb_dev->addr);
        fd = open(dev_path, O_RDONLY);
        if (fd < 0) {
                printf("Unable to open %s (%s)\n", 
				smb_dev->bus, dev_path);
                return (0);
        }
        lseek(fd, smb_dev->offset+offset, SEEK_SET);
        rc = read(fd, buf, len);
        close(fd);

	return ((rc == (int)len) ? 0 : -1);
}

int sprom_wr (diag_smb_dev_t *smb_dev, uint32_t offset, 
		uint32_t len, uint8_t *buf)
{
        FILE    *fd;
        int     rc;
	char	dev_path[80];

	sprintf(dev_path, "/sys/bus/i2c/devices/%d-%04x/eeprom", 
			smb_dev->path, smb_dev->addr);
        fd = fopen(dev_path, "w");
        if (fd == NULL) {
                printf("Unable to open %s\n", smb_dev->bus);
                return (0);
        }
        fseek(fd, smb_dev->offset+offset, SEEK_SET);
        rc = fwrite(buf, len, 1, fd);
        fclose(fd);

        return ((rc == 1) ? 0 : -1);
}

int
host_ipmi_sprom_rd (diag_smb_dev_t *smb_dev, uint32_t offset,
                uint32_t len, uint8_t *buf)
{
    int rc = 0;
    char *filename = "/var/tmp/ipmi_eeprom_read.bin";
    char cmd_str[256];
    int to = 5;
    FILE *fd = NULL;

    if ( smb_dev == NULL )
        return ( -1 );

    sprintf ( cmd_str, "ipmitool fru read %d %s &> /dev/null",
      smb_dev->addr, filename );

    remove ( filename );
    system ( cmd_str );
    while ( --to && ( ( fd = fopen ( filename, "rb" ) ) == NULL ) ) {
        sleep ( 1 );
    }
    if ( fd == NULL )
        return ( -1 );

    rc = fread ( ( void * )buf, sizeof ( uint8_t ), len, fd );

    fclose ( fd );

    return ( ( rc < 0 ) ? rc : 0 );
}

int
host_ipmi_sprom_wr (diag_smb_dev_t *smb_dev, uint32_t offset,
                uint32_t len, uint8_t *buf)
{
char *filename = "/var/tmp/ipmi_eeprom_write.bin";
    char cmd_str[256]; 
    FILE *fd;

    if ( smb_dev == NULL )
        return ( -1 );

    sprintf ( cmd_str, "ipmitool fru write %d %s > /dev/null", smb_dev->addr, filename );

    fd = fopen ( filename, "wb" );
    if ( fd == NULL )
        return ( -1 );

    if ( fwrite ( ( void * )buf, sizeof ( uint8_t ), len, fd ) != len ) {
        return ( -1 ) ;
    }

    fclose ( fd );

    return ( system ( cmd_str ) );

}

int diag_sprom_rd (diag_sprom_t *sprom, uint32_t offset, 
		   uint32_t len, uint8_t *buf)
{
	assert (sprom);
	assert (sprom->smb_acc);
	assert (buf);
	assert (sprom->smb_acc->smb_rd);

	return (sprom->smb_acc->smb_rd(sprom->smb_acc->smb_dev, 
					offset, len, buf));
}

int diag_sprom_wr (diag_sprom_t *sprom, uint32_t offset, 
			uint32_t len, uint8_t *buf)
{
	int rc = 0;
	assert (sprom);
	assert (sprom->smb_acc);
	assert (buf);
	assert (sprom->smb_acc->smb_wr);

    	if ( sprom->we ) {
		sprom->we();
	}
	rc =  (sprom->smb_acc->smb_wr(sprom->smb_acc->smb_dev, 
					offset, len, buf));
    	if ( sprom->wd ) {
		sprom->wd();
	}

	return (rc);
}
