#include <kernel/socket.h>

int socket_resource_init(struct socket_resource * sock) 
{
	resource_init(&sock->res);
}
