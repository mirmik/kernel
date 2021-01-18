#include <drivers/tty/n_tty_ldisc.h>


struct tty_ldisc_operations tty_ldisc = 
{
	n_tty_read,
	n_tty_newchar	
};

void
