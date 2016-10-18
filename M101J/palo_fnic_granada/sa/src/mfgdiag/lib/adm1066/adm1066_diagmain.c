#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include "ppc_lib.h"
#include "diag_fex.h"

int verbose = 0;

char diag_desc_monitor[] = "\nmonitor:\n"
    " monitor init\n"
    " monitor info\n"
    " monitor margin <all/name> <step>\n"
    " monitor [test] [voltage [margin]]\n"
    " monitor [voltage [all/name]]\n"
    " monitor margin all [step]\n"
    "    test    : tests if voltage is in range\n"
    "    voltage : string name for voltage or \"all\"\n"
    "    step  : margin value percent\n";

static void monitor_help()
{
        printf ("%s", diag_desc_monitor);
}

extern int monitor_ui(int argc, char **argv, int fd);
static int dsh_cmd_monitor (int argc, char *argv[])
{
        int rc = 0, fd;

        fd = ppc_i2c_open(FEX_ADM1066_I2C_BUS);
        if (fd == -1) {
                perror("I2C0");
                return (-1);
        }

        rc = monitor_ui(argc, argv, fd);
        if (rc) {
                monitor_help();
        }

        close(fd);
        return (rc);
}

int main (int argc, char *argv[])
{
	int rc = 0;
	rc = dsh_cmd_monitor(argc, argv);
	return (rc);
}
