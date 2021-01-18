#include <drivers/tty/tty.h>

int tty_open(struct tty * tty) 
{
	tty->controller = current_schedee();
}

int tty_release(struct tty * tty) 
{
	tty->controller = NULL;
}

int tty_write(struct tty * tty, const char * dat, int len, int flags) 
{
	return tty->driver->d_ops->write(tty->driver, dat, len, flags);
}

int tty_read(struct tty * tty, char * dat, int len, int flags) 
{
	return tty->ldisc->l_ops->read(&tty->ldisc, dat, len, flags);
}

int tty_room(struct tty * tty) 
{
	return -ENOTSUP;
}

int tty_avail(struct tty * tty) 
{
	return tty->driver->d_ops->avail(tty->driver);
}

const struct resource_operations tty_ops = 
{
	.open = tty_open,
	.release = tty_release,
	.write = tty_write,
	.read = tty_read,
	.room = tty_room,
	.avail = tty_avail,
	.ioctl = NULL
};

void tty_init(
	struct tty * tty, 
	struct tty_driver * drv, 
	struct tty_ldisc * ldisc) 
{
	resource_init(&tty->res, &tty_ops);
	tty->driver = drv;
	tty->ldisc = ldisc;
	tty->controller = NULL;	
}