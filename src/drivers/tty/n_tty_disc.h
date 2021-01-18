#ifndef KERNEL_DRIVERS_N_TTY_DISC_H
#define KERNEL_DRIVERS_N_TTY_DISC_H

#include <drivers/tty/tty_ldisc.h>

struct n_tty_disc 
{
	struct tty_ldisc ldisc;
};

#endif