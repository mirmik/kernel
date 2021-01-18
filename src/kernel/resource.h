#ifndef KERNEL_FILE_H
#define KERNEL_FILE_H

struct resource;

struct resource_operations
{
	int(*open)(struct resource *);
	int(*release)(struct resource *);

	int(*write)(struct resource *, const char * dat, size_t len, int flags);
	int(*read)(struct resource *, char * dat, size_t len, int flags);
	int(*room)(struct resource *);
	int(*avail)(struct resource *);

	int(*ioctl)(struct resource *, int code, void * arg);
};

struct resource
{
	const struct resource_operations * r_ops;
	int                                refs;
};

__BEGIN_DECLS

int resource_open(   struct resource * res, struct openres * ores);
int resource_release(struct resource * res, struct openres * ores);

__END_DECLS

#endif
