#include <kernel/pipe.h>

int pipe_open(struct pipe * pipe) 
{
	ring_reset(&pipe->ring);
}

int pipe_release(struct pipe * pipe) 
{}

int pipe_write(struct pipe * pipe, void* dat, int len, int flags) 
{
	ring_write(&pipe->ring, pipe->buf, dat, len);
}

int pipe_read(struct pipe * pipe, void* dat, int len, int flags) 
{}

int pipe_room(struct pipe * pipe, void* dat, int len, int flags) 
{
	return -1;
}

int pipe_avail(struct pipe * pipe, void* dat, int len, int flags) 
{
	return -1;
}

const resource_operations resource_ops = 
{
	.open =    pipe_open,
	.release = pipe_release,
	.write =   pipe_write,
	.read =    pipe_read,
	.room =    pipe_room,
	.avail =   pipe_avail,
	.ioctl =   NULL
};

void pipe_init(struct pipe * pipe, void * buf, int len) 
{
	resource_init(&pipe->res, &resource_ops);
	pipe->buf = buf;
	pipe->len = len;
}
