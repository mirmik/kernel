#ifndef KERNEL_WAITER_H
#define KERNEL_WAITER_H

struct waiter 
{
	dlist_head lnk;

	void(*func)(void*);
	void *priv;
};

__BEGIN_DECLS

void waiter_init(
	struct waiter * waiter,
	void(*func)(void*),
	void *priv) 
{
	waiter->func = func;
	waiter->priv = priv;
}

void waiter_wait_queue(
	struct waiter *     waiter,
	struct dlist_head * queue,
	int                 priority);

void notify_one(struct dlist_head * queue);
void notify_all(struct dlist_head * queue);

__END_DECLS

#endif