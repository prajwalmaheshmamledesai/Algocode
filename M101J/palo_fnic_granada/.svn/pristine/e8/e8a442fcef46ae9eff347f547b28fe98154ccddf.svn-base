#include <time.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stropts.h>
#include <unistd.h>
#include <asm/ioctl.h> 
#include <fcntl.h>


#include <search.h>

#define MY_FILE_NAME		"/dev/pilot2_i2c_drv"
#define MEANINGLESS		(0)
#define TRUE			(1)
#define FALSE			(0)
#define TRANSFER_BUFFER_SIZE	(0x10)
#define MEANINGLESS_DATA	(0xFF)

/* IOCTL command */
#define AESS_I2CDRV_IOC_MAGIC	0xB7
#define AESS_I2CDRV_WR		_IOWR(AESS_I2CDRV_IOC_MAGIC, 2, \
                                          mpI2CDrvBufInfoType)


/*****************************************************************************/
/* The following data structure came out of the IPMI stack.  I have no       */
/* idea why the element 'u8ErrorStatus' needs to be a volatile, but I        */
/* didn't want to change it.                                                 */
/*****************************************************************************/
typedef struct
{
           unsigned char  u8Channel;           /** I2C bus channel */
           unsigned char  u8DeviceAddr;        /** device slave address */
  volatile unsigned char  u8ErrorStatus;       /** Bus error status */
           unsigned char *pu8MsgSendBuffer;    /** Msg transmit buffer */
           unsigned char  u8MsgSendDataSize;   /** Msg transmit data length */
           unsigned char *pu8MsgRecBuffer;     /** Msg receive buffer */
           unsigned char  u8MsgRecDataSize;    /** Msg receive data length */
}mpI2CDrvBufInfoType;

int
MP_I2CByteWrite_8BitIndex (int Fh, unsigned char Bus, unsigned char Dev, 
			unsigned char Reg, unsigned char Val, int send, 
			int verbose)
{
	mpI2CDrvBufInfoType	I;
	int                	IOCTLReturn;
	unsigned char		RxBuffer[100], TxBuffer[100];

	verbose=0;
	memset(&I, 0, sizeof(mpI2CDrvBufInfoType));

	I.u8Channel		= Bus;
	I.u8DeviceAddr		= Dev;
	I.pu8MsgRecBuffer	= RxBuffer;
	I.pu8MsgSendBuffer	= TxBuffer;
	TxBuffer[0]		= Reg;
	TxBuffer[1]		= Val;
	I.u8MsgSendDataSize	= send ? 1 : 2;
	I.u8MsgRecDataSize          = 0;

	if (verbose) printf("Byte Write (0x%02X, 0x%02X, 0x%02X, %02X) ---- ",Bus, Dev, Reg, Val);

	if ((IOCTLReturn = ioctl(Fh, AESS_I2CDRV_WR, (unsigned long)&I)) != 0) {
		printf("IOCTL Fail (0x%08x)\n", IOCTLReturn);
		return -1;
	} else {
		if (I.u8ErrorStatus == 0) {
			if (verbose) printf("Success\n");
	  	} else {
			if (verbose) printf("Failed (0x%02X)\n", I.u8ErrorStatus);
			return -1;
		}
	}
	return (0);
}

int MP_I2CByteRead_8BitIndex(int Fh, unsigned char Bus, unsigned char Dev, 
			  unsigned char Reg, unsigned char *data, int verbose)
{
	mpI2CDrvBufInfoType I;
	int                IOCTLReturn;
	unsigned char      RxBuffer[100], TxBuffer[100];

	//verbose=0;
	memset(&I, 0, sizeof(mpI2CDrvBufInfoType));

	I.u8Channel                 = Bus;
	I.u8DeviceAddr              = Dev;
	I.pu8MsgRecBuffer           = RxBuffer;
	I.pu8MsgSendBuffer          = TxBuffer;
	TxBuffer[0]                 = Reg;
	I.u8MsgSendDataSize         = 1;
	I.u8MsgRecDataSize          = 1;

	if ((IOCTLReturn = ioctl(Fh, AESS_I2CDRV_WR, (unsigned long)&I)) != 0) {
		printf("IOCTL Fail (0x%08x)\n", IOCTLReturn);
		return -1;
	} else {
		if (verbose) printf("Byte Read (0x%02X, 0x%02X, 0x%02X) --->  0x%02X ---- ",Bus, Dev, Reg, RxBuffer[0]);
		if (I.u8ErrorStatus == 0) {
			if (verbose) printf("Success\n");
			*data = RxBuffer[0];
			return 0;
		} else {
			printf("Failed (0x%02X)\n", I.u8ErrorStatus);
			return -1;
		}
	}
}

int
MP_I2CByteWrite_Block (int Fh, unsigned char Bus, unsigned char Dev, unsigned char *txBuf, 
			int bytes, int verbose)
{
	mpI2CDrvBufInfoType I;
	int                i, IOCTLReturn;
	unsigned char      RxBuffer[100], TxBuffer[100];

	memset(&I, 0, sizeof(mpI2CDrvBufInfoType));

	I.u8Channel         = Bus;
	I.u8DeviceAddr      = Dev;
	I.pu8MsgRecBuffer   = RxBuffer;
	I.pu8MsgSendBuffer  = TxBuffer;
	I.u8MsgSendDataSize = bytes;
	I.u8MsgRecDataSize  = 0;
  
	for (i=0; i<bytes; i++) {
		TxBuffer[i] = txBuf[i];
		if (verbose) printf(" %2.2x", TxBuffer[i]);
	}
	if (verbose) printf("\nBlock Write (0x%02X, 0x%02X ) ---- ",Bus, Dev );

	if ((IOCTLReturn = ioctl(Fh, AESS_I2CDRV_WR, (unsigned long)&I)) != 0) {
		printf("IOCTL Fail (0x%08x)\n", IOCTLReturn);
		return -1;
	} else {
		if (I.u8ErrorStatus == 0) {
			if (verbose) printf("Success\n");
			return 0;
		} else {
			if (verbose) printf("Failed (0x%02X)\n", I.u8ErrorStatus);
			return -1;
		}
	}
}

int MP_I2CByteRead_Block ( int Fh, unsigned char Bus, unsigned char Dev, 
			unsigned char Reg, unsigned char *rxBuf, int bytes, int verbose)
{
	mpI2CDrvBufInfoType I;
	int                i, IOCTLReturn, nreads = 1, iters;
	unsigned char      RxBuffer[32], TxBuffer[16];

	memset(&I, 0, sizeof(mpI2CDrvBufInfoType));
	memset(RxBuffer, 0x00, sizeof(RxBuffer));
	memset(TxBuffer, 0x00, sizeof(TxBuffer));

	I.u8Channel                 = Bus;
	I.u8DeviceAddr              = Dev;
	I.pu8MsgRecBuffer           = RxBuffer;
	I.pu8MsgSendBuffer          = TxBuffer;
	TxBuffer[0]                 = Reg;
	I.u8MsgSendDataSize         = 1;

	nreads = (bytes >> 5) + ((bytes & 0x1F)? 1 : 0);

	for (iters = 0; iters < nreads; iters++) {	
		TxBuffer[0] = Reg + (iters*32);

        if ((iters + 1) == nreads)
        {
            if (bytes & 0x1F)
            {
                I.u8MsgRecDataSize = bytes & 0x1F;
            }
            else
            {
                I.u8MsgRecDataSize = 0x20;
            }
        }
        else
        {
            I.u8MsgRecDataSize = 0x20;
        }

		//I.u8MsgRecDataSize   = (unsigned char)((iters+1)==nreads) ? (bytes & 0x1F) : 0x20;

		if ((IOCTLReturn = ioctl(Fh, AESS_I2CDRV_WR, (unsigned long)&I)) != 0) {
			printf("IOCTL Fail (0x%08x)\n", IOCTLReturn);
			return -1;
		} else {
			if (verbose) 
				printf("Byte Read (0x%02X, 0x%02X, 0x%02X) --->  0x%02X ---- ",
					Bus, Dev, TxBuffer[0], RxBuffer[0]);
			if (I.u8ErrorStatus == 0) {
				if (verbose) printf("Success: ");
				for (i=0; i<I.u8MsgRecDataSize; i++) {
					rxBuf[(iters*32)+i] = RxBuffer[i];
					if (verbose) printf("%2.2x ", RxBuffer[i]);
				}
				if (verbose) printf("\n");
			} else {
				printf("Failed (0x%02X)\n", I.u8ErrorStatus);
				return -1;
			}
		}
	}
	return 0;
}

int MP_I2CByteRead_Block0 ( int Fh, unsigned char Bus, unsigned char Dev, 
			unsigned char Reg, unsigned char *rxBuf, int bytes, int verbose)
{
	mpI2CDrvBufInfoType I;
	int                i, IOCTLReturn, nreads = 1, iters;
	unsigned char      RxBuffer[32], TxBuffer[16];

	memset(&I, 0, sizeof(mpI2CDrvBufInfoType));
	memset(RxBuffer, 0x00, sizeof(RxBuffer));
	memset(TxBuffer, 0x00, sizeof(TxBuffer));

	I.u8Channel                 = Bus;
	I.u8DeviceAddr              = Dev;
	I.pu8MsgRecBuffer           = RxBuffer;
	I.pu8MsgSendBuffer          = TxBuffer;
	TxBuffer[0]                 = Reg;
	I.u8MsgSendDataSize         = 0;

	nreads = (bytes >> 5) + ((bytes & 0x1F)? 1 : 0);

	for (iters = 0; iters < nreads; iters++) {	
		TxBuffer[0] = Reg + (iters*32);

        if ((iters + 1) == nreads)
        {
            if (bytes & 0x1F)
            {
                I.u8MsgRecDataSize = bytes & 0x1F;
            }
            else
            {
                I.u8MsgRecDataSize = 0x20;
            }
        }
        else
        {
            I.u8MsgRecDataSize = 0x20;
        }

		//I.u8MsgRecDataSize   = (unsigned char)((iters+1)==nreads) ? (bytes & 0x1F) : 0x20;

		if ((IOCTLReturn = ioctl(Fh, AESS_I2CDRV_WR, (unsigned long)&I)) != 0) {
			printf("IOCTL Fail (0x%08x)\n", IOCTLReturn);
			return -1;
		} else {
			if (verbose) 
				printf("Byte Read (0x%02X, 0x%02X, 0x%02X) --->  0x%02X ---- ",
					Bus, Dev, TxBuffer[0], RxBuffer[0]);
			if (I.u8ErrorStatus == 0) {
				if (verbose) printf("Success: ");
				for (i=0; i<I.u8MsgRecDataSize; i++) {
					rxBuf[(iters*32)+i] = RxBuffer[i];
					if (verbose) printf("%2.2x ", RxBuffer[i]);
				}
				if (verbose) printf("\n");
			} else {
				printf("Failed (0x%02X)\n", I.u8ErrorStatus);
				return -1;
			}
		}
	}
	return 0;
}

int MP_I2CByteRead_Block2 ( int Fh, unsigned char Bus, unsigned char Dev, 
			unsigned short data, unsigned char *rxBuf, int bytes, int verbose)
{
	mpI2CDrvBufInfoType I;
	int                i, IOCTLReturn, nreads = 1, iters;
	unsigned char      RxBuffer[32], TxBuffer[16];

	memset(&I, 0, sizeof(mpI2CDrvBufInfoType));
	memset(RxBuffer, 0x00, sizeof(RxBuffer));
	memset(TxBuffer, 0x00, sizeof(TxBuffer));

	I.u8Channel                 = Bus;
	I.u8DeviceAddr              = Dev;
	I.pu8MsgRecBuffer           = RxBuffer;
	I.pu8MsgSendBuffer          = TxBuffer;
	TxBuffer[0]                 = (data >> 8) & 0xFF;
    TxBuffer[1]                 = data & 0xFF;
	I.u8MsgSendDataSize         = 2;

	nreads = (bytes >> 5) + ((bytes & 0x1F)? 1 : 0);

	for (iters = 0; iters < nreads; iters++) {	
        TxBuffer[0] = ((data + (iters*32)) >> 8) & 0xFF;
		TxBuffer[1] = (data + (iters*32)) & 0xFF;
        
        if ((iters + 1) == nreads)
        {
            if (bytes & 0x1F)
            {
                I.u8MsgRecDataSize = bytes & 0x1F;
            }
            else
            {
                I.u8MsgRecDataSize = 0x20;
            }
        }
        else
        {
            I.u8MsgRecDataSize = 0x20;
        }

		//I.u8MsgRecDataSize   = (unsigned char)((iters+1)==nreads) ? (bytes & 0x1F) : 0x20;

		if ((IOCTLReturn = ioctl(Fh, AESS_I2CDRV_WR, (unsigned long)&I)) != 0) {
			printf("IOCTL Fail (0x%08x)\n", IOCTLReturn);
			return -1;
		} else {
			if (verbose) 
				printf("Byte Read (0x%02X, 0x%02X, 0x%02X) --->  0x%02X ---- ",
					Bus, Dev, TxBuffer[0], RxBuffer[0]);
			if (I.u8ErrorStatus == 0) {
				if (verbose) printf("Success: ");
				for (i=0; i<I.u8MsgRecDataSize; i++) {
					rxBuf[(iters*32)+i] = RxBuffer[i];
					if (verbose) printf("%2.2x ", RxBuffer[i]);
				}
				if (verbose) printf("\n");
			} else {
				printf("Failed (0x%02X)\n", I.u8ErrorStatus);
				return -1;
			}
		}
	}
	return 0;
}



int MP_I2CByteWrite_16BitIndex( int Fh, unsigned char Bus, unsigned char Dev, 
				unsigned short Reg16, unsigned char Val)
{
	mpI2CDrvBufInfoType	I;
	int                	IOCTLReturn;
	unsigned char		RxBuffer[100], TxBuffer[100];

	memset(&I, 0, sizeof(mpI2CDrvBufInfoType));

	I.u8Channel		= Bus;
	I.u8DeviceAddr		= Dev;
	I.pu8MsgRecBuffer	= RxBuffer;
	I.pu8MsgSendBuffer	= TxBuffer;
	TxBuffer[0]             = (unsigned char)((Reg16 >> 8) & 0xff);
      	TxBuffer[1]             = (unsigned char)((Reg16 >> 0) & 0xff);
	TxBuffer[2]		= Val;
	I.u8MsgSendDataSize	=  3;
	I.u8MsgRecDataSize      = 0;


	if ((IOCTLReturn = ioctl(Fh, AESS_I2CDRV_WR, (unsigned long)&I)) != 0) {
		printf("IOCTL Fail (0x%08x)\n", IOCTLReturn);
		return -1;
	} else {
		if (I.u8ErrorStatus == 0) {
	  	} else {
			printf("Failed (0x%02X)\n", I.u8ErrorStatus);
			return -1;
		}
	}
	return (0);
}  /* end routine */



int 
MP_I2CByteRead_16BitIndex(int Fh, unsigned char Bus, unsigned char Dev, unsigned short Reg16,unsigned char *data)
{
	mpI2CDrvBufInfoType I;
	int                IOCTLReturn;
	unsigned char      RxBuffer[100], TxBuffer[100];

	memset(&I, 0, sizeof(mpI2CDrvBufInfoType));

	I.u8Channel                 = Bus;
	I.u8DeviceAddr              = Dev;
	I.pu8MsgRecBuffer           = RxBuffer;
	I.pu8MsgSendBuffer          = TxBuffer;
	TxBuffer[0]             = (unsigned char)((Reg16 >> 8) & 0xff);
      	TxBuffer[1]             = (unsigned char)((Reg16 >> 0) & 0xff);
	I.u8MsgSendDataSize         = 2;
	I.u8MsgRecDataSize          = 1;

	if ((IOCTLReturn = ioctl(Fh, AESS_I2CDRV_WR, (unsigned long)&I)) != 0) {
		printf("IOCTL Fail (0x%08x)\n", IOCTLReturn);
		return -1;
	} else {
		if (I.u8ErrorStatus == 0) {
			*data = RxBuffer[0];
			return 0;
		} else {
			printf("Failed (0x%02X)\n", I.u8ErrorStatus);
			return -1;
		}
	}
	
}  /* end routine */

int mp_arbiter_en(int fd , unsigned short bus)
{
    int rc = 0;
    unsigned short i2c_addr;
    unsigned char reg_data, reg_addr;

    // Enable IO arbiter
    i2c_addr = 0xE2;
    reg_addr = 1;
    rc = MP_I2CByteRead_8BitIndex (fd, bus, i2c_addr, reg_addr, &reg_data, 0);
    switch (reg_data)
    {
        case 0x0:
        case 0x1:
        case 0x5:
        case 0x6:
            reg_data = 0x4;
            rc = MP_I2CByteWrite_8BitIndex (fd, bus, i2c_addr, reg_addr, reg_data, 0, 0);
            break;

        case 0x2:
        case 0x3:
            reg_data = 0x5;
            rc = MP_I2CByteWrite_8BitIndex (fd, bus, i2c_addr, reg_addr, reg_data, 0, 0);
            break;

        case 0x9:
        case 0xC:
        case 0xD:
            reg_data = 0x0;
            rc = MP_I2CByteWrite_8BitIndex (fd, bus, i2c_addr, reg_addr, reg_data, 0, 0);
            break;
        case 0xA:
        case 0xE:
        case 0xF:
            reg_data = 0x1;
            rc = MP_I2CByteWrite_8BitIndex (fd, bus, i2c_addr, reg_addr, reg_data, 0, 0);
            break;

        default:
            break;
        }

    return rc;
}

int mp_i2c_open(void)
{
        return (open(MY_FILE_NAME, O_RDONLY, 0));
}

void mp_i2c_close(int fd)
{
        close(fd);
}


