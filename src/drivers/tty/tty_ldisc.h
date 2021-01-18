#ifndef KERNEL_DRIVERS_TTY_LDISC
#define KERNEL_DRIVERS_TTY_LDISC

#include <sys/cdefs.h>

struct tty_struct;
struct tty_ldisc;

struct tty_ldisc_operations 
{
	int (*read)(struct tty_ldisc *, char* drv, int len, int flags);
	int (*newchar)(struct tty_ldisc *, char c);
};

struct tty_ldisc 
{
	struct tty_struct * tty;
};

__BEGIN_DECLS

static
void tty_ldisc_init(
	struct tty_ldisc * ldisc,
	const struct tty_ldisc_operations * ops) 
{
	ldisc->l_ops = ops;
}

__END_DECLS

#endif