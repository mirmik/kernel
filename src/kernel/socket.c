#include <kernel/socket.h>

const struct resource_operations socket_resource_ops = 
{
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
};

int socket_resource_init(struct socket_resource * sock) 
{
	resource_init(&sock->res, socket_resource_ops);
}
