#ifndef KERNEL_NAMESPACE_H
#define KERNEL_NAMESPACE_H

struct namespace_operations 
{
	void (*lookup)(const char * path, resource *);
};

struct namespace 
{
	const char * prefix;
};

int namespace_iterate(struct namespace * ns);

int lookup(const char * path);

#endif