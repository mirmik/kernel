#ifndef KERNEL_DRIVERS_TTY_DRIVER_H
#define KERNEL_DRIVERS_TTY_DRIVER_H

struct tty_driver_operations 
{
	void (*putchar)(struct tty_driver *, char);
}

struct tty_driver 
{
	struct tty_struct * tty;
}

#endif