#include <stdio.h>

int verbose = 0;
////////////////////////////////////////////////////////////////////
// sprom
////////////////////////////////////////////////////////////////////
char diag_desc_sprom[] = "\nsprom:\n"
    " sprom [platform] <cmd> <type> [slot]\n"
    "\n"
    " sprom show <type> [slot]\n"
    "\n"
    " sprom set  <type> [slot]\n"
    " sprom set-mac <type> [slot]\n"
    " sprom set-serial <type> [slot]\n"
    " sprom set-part-num <type> [slot]\n"
    " sprom set-part-rev <type> [slot]\n"
    "\n"
    " sprom dump <offset> <len> <type> [slot]\n"
    " sprom invalidate <type> [slot]\n"
    " sprom zero <type> [slot]\n"
    "\n"
    " <type>  : sup, bp, mod, ps\n";


static void sprom_help()
{
        printf ("%s", diag_desc_sprom);
}

extern int sprom_ui (int argc, char *argv[]);
int main (int argc, char *argv[])
{
	int rc = 0;
	rc = sprom_ui(argc, argv);
	if (rc) {
		sprom_help();
	}
	return (rc);
}
