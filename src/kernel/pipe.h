#ifndef KERNEL_PIPE_H
#define KERNEL_PIPE_H

#include <kernel/resource.h>

struct pipe 
{
	struct resource res;

	void * buf;
	size_t len;

	struct ring_head head;
};

extern const resource_operations resource_ops;

__BEGIN_DECLS

void pipe_init(struct pipe * pipe, void* buffer, size_t len);

__END_DECLS

#endif
