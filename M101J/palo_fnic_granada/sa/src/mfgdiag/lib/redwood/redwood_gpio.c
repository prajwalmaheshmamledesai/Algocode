
#include "red_hw_lib.h" 
#include "red_red_naxislave_define.h"

extern int red_reg_rd(int inst, uint32_t addr, uint32_t *data);

#define ACTIVE_LO	0
#define ACTIVE_HI	1

#define DIR_INPUT	0
#define DIR_OUTPUT	1

typedef struct gpio_map_s
{
	uint8_t		gpio;
	uint8_t*	name;
	uint8_t		unit;
	uint8_t		dir;
	uint8_t		pol;
	uint32_t	mask;
} gpio_map_t;

gpio_map_t rdw_gpio_iom[] = 
{
        {8, 	"SFP_PHY_RST_L", 	0, DIR_OUTPUT, ACTIVE_LO, 0x0100},
        {9, 	"KR_PHY_RST_L",		0, DIR_OUTPUT, ACTIVE_LO, 0x0200},
};

gpio_map_t rdw_gpio_fex[] = 
{
        {0, 	"PHYX_RESET_L",	  	0, DIR_OUTPUT, ACTIVE_LO, 0x01},
        {1, 	"BCM5221_X_RESET_L",	0, DIR_OUTPUT, ACTIVE_LO, 0x02},
        {2, 	"BCM5221_IRQ",		0, DIR_INPUT,  ACTIVE_HI, 0x04},

        {0, 	"PHYX_RESET_L",	  	1, DIR_OUTPUT, ACTIVE_LO, 0x01},
        {1, 	"BCM5221_X_RESET_L",	1, DIR_OUTPUT, ACTIVE_LO, 0x02},
        {2, 	"BCM5221_IRQ",		1, DIR_INPUT,  ACTIVE_HI, 0x04},

        {0, 	"PHYX_RESET_L",	  	2, DIR_OUTPUT, ACTIVE_LO, 0x01},
        {1, 	"BCM5221_X_RESET_L",	2, DIR_OUTPUT, ACTIVE_LO, 0x02},
        {2, 	"BCM5221_IRQ",		2, DIR_INPUT,  ACTIVE_HI, 0x04},

        {0, 	"PHYX_RESET_L",	  	3, DIR_OUTPUT, ACTIVE_LO, 0x01},
        {1, 	"BCM5221_X_RESET_L",	3, DIR_OUTPUT, ACTIVE_LO, 0x02},
        {2, 	"BCM5221_IRQ",		3, DIR_INPUT,  ACTIVE_HI, 0x04},

        {0, 	"PHYX_RESET_L",	  	4, DIR_OUTPUT, ACTIVE_LO, 0x01},
        {1, 	"BCM5221_X_RESET_L",	4, DIR_OUTPUT, ACTIVE_LO, 0x02},
        {2, 	"BCM5221_IRQ",		4, DIR_INPUT,  ACTIVE_HI, 0x04},

        {0, 	"PHYX_RESET_L",	  	5, DIR_OUTPUT, ACTIVE_LO, 0x01},
        {1, 	"BCM5221_X_RESET_L",	5, DIR_OUTPUT, ACTIVE_LO, 0x02},
        {2, 	"BCM5221_IRQ",		5, DIR_INPUT,  ACTIVE_HI, 0x04},

        {0, 	"PHYX_RESET_L",	  	6, DIR_OUTPUT, ACTIVE_LO, 0x01},
        {1, 	"BCM5221_X_RESET_L",	6, DIR_OUTPUT, ACTIVE_LO, 0x02},
        {2, 	"BCM5221_IRQ",		6, DIR_INPUT,  ACTIVE_HI, 0x04},

        {0, 	"PHY6_LASI0",		6, DIR_INPUT,  ACTIVE_HI, 0x01},
        {1, 	"PHY6_LASI1",		6, DIR_INPUT,  ACTIVE_HI, 0x02},
        {2, 	"PHY6_LOS0",		6, DIR_INPUT,  ACTIVE_HI, 0x04},
        {3, 	"PHY6_LOS1",		6, DIR_INPUT,  ACTIVE_HI, 0x08},
        {4, 	"PHY6_TS_DIS0",		6, DIR_OUTPUT, ACTIVE_HI, 0x10},
        {6, 	"PHY6_TS_DIS1",		5, DIR_OUTPUT, ACTIVE_HI, 0x20},
        {6, 	"PHY6_I2C_FLUSH0",	6, DIR_OUTPUT, ACTIVE_HI, 0x40},
        {7, 	"PHY6_I2C_FLUSH1",	6, DIR_OUTPUT, ACTIVE_HI, 0x80},
        {8, 	"SFP6_0_PRESENT_L",	6, DIR_INPUT,  ACTIVE_LO, 0x100},
        {9, 	"SFP6_1_PRESENT_L",	6, DIR_INPUT,  ACTIVE_LO, 0x200},
        {10, 	"PHY6_RESET_L",		6, DIR_OUTPUT, ACTIVE_LO, 0x400},
        {11, 	"BCM5221_6_RST_L",	6, DIR_OUTPUT, ACTIVE_LO, 0x800},

        {12, 	"PHY7_LASI0",		6, DIR_INPUT,  ACTIVE_HI, 0x1000},
        {13, 	"PHY7_LASI1",		6, DIR_INPUT,  ACTIVE_HI, 0x2000},
        {14, 	"PHY7_LOS0",		6, DIR_INPUT,  ACTIVE_HI, 0x4000},
        {15, 	"PHY7_LOS1",		5, DIR_INPUT,  ACTIVE_HI, 0x8000},

        {16, 	"PHY7_TS_DIS0",		6, DIR_OUTPUT, ACTIVE_HI, 0x10000},
        {17, 	"PHY7_TS_DIS1",		6, DIR_OUTPUT, ACTIVE_HI, 0x20000},
        {18, 	"PHY7_I2C_FLUSH0",	6, DIR_OUTPUT, ACTIVE_HI, 0x40000},
        {19, 	"PHY7_I2C_FLUSH1",	6, DIR_OUTPUT, ACTIVE_HI, 0x80000},
        {20, 	"SFP7_0_PRESENT_L",	6, DIR_INPUT,  ACTIVE_LO, 0x100000},
        {21, 	"SFP7_1_PRESENT_L",	6, DIR_INPUT,  ACTIVE_LO, 0x200000},
        {22, 	"PHY7_RESET_L",		6, DIR_OUTPUT, ACTIVE_LO, 0x400000},
        {23, 	"BCM5221_6_IRQ",	6, DIR_INPUT,  ACTIVE_HI, 0x800000},
};

int redwood_gpio_dump (int inst)
{
#ifdef DIAG_FEX
	gpio_map_t *pgpio = rdw_gpio_fex;
	uint32_t gpio_cnt = sizeof(rdw_gpio_fex)/sizeof(gpio_map_t);
#else
	gpio_map_t *pgpio = rdw_gpio_iom;
	uint32_t gpio_cnt = sizeof(rdw_gpio_iom)/sizeof(gpio_map_t);
#endif
	uint32_t    data, cnt;
	int 	    rc = 0;

	// Read GPIO Data.
	rc = red_reg_rd(inst, BA(RED_MISC_GPIO_DATA_OUT), &data);
	if (rc) {
		printf("Error: Failed to read redwood%d GPIO\n", inst);
		return (rc);
	}

	printf(" RED_MISC_GPIO_DATA_OUT [0x%X] : 0x%X\n",
			BA(RED_MISC_GPIO_DATA_OUT), data);
	printf("%-20sRDW:PIN : I/O : ACTIVE : STATE [0x%X]\n", "NAME", data);
	printf("===================================================================\n");
	for (cnt = 0; cnt < gpio_cnt; cnt++, pgpio++) {
		if (pgpio->unit != inst)	
			continue;

		printf("%-20s[%02d:%02d] : %c   :   %c   : %c\n",
			pgpio->name, pgpio->unit, pgpio->gpio, 
			(pgpio->dir == DIR_INPUT) ? 'I' : 'O',
			(pgpio->pol == ACTIVE_HI) ? 'H' : 'L',
			(pgpio->mask & data) ?  
				((pgpio->pol == ACTIVE_HI) ? 'Y' : 'N'):
				((pgpio->pol == ACTIVE_HI) ? 'N' : 'Y'));
	}

	return (rc);
}
