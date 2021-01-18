#ifndef KERNEL_DRIVERS_TTY_H
#define KERNEL_DRIVERS_TTY_H

#include <drivers/tty/tty_driver.h>
#include <drivers/tty/tty_ldisc.h>

struct tty 
{
	struct resource res;

	struct tty_driver * driver;
	struct tty_ldisc *  ldisc;

	struct schedee * controller;
};

__BEGIN_DECLS

void tty_receive_newchar(struct tty * tty, char c);
void tty_init(struct tty * tty);

__END_DECLS

#endif