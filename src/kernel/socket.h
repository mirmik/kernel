#ifndef KERNEL_SOCKET_H
#define KERNEL_SOCKET_H

#include <kernel/resource.h>

struct socket_resource_operations
{
	int (*bind)(struct socket_resource *, int port);
	int (*connect)(struct socket_resource *);
	int (*send)(struct socket_resource *);
	int (*recv)(struct socket_resource *);
};

struct socket_resource 
{
	struct resource res;
	const struct socket_resource_operations s_ops;
};

struct tcp_socket_resource 
{
	struct socket_resource res;
	//int protocol;

	union 
	{
		struct ip_address ipaddr;
	};
};

int socket_connect();

int create_tcp_socket();

int socket_resource_init(struct socket_resource * res);

#endif