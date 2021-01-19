#ifndef KERNEL_NAMESPACE_H
#define KERNEL_NAMESPACE_H

struct directory_pointer 
{
	struct namespace * ns;
	const char * path;
};

struct resource_pointer 
{
	struct namespace * ns;
	const char * path;
};

struct namespace_operations 
{
	void (*lookup)(const char * path, resource *);
	void (*iterate)(
		struct resource_pointer * iter, 
		struct directory_pointer * dir);
};

struct namespace 
{
	const char * prefix;
};

int namespace_iterate(struct namespace * ns);

int lookup(const char * path);

#endif