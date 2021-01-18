#ifndef COOPERATIVE_SCHEDEE_H
#define COOPERATIVE_SCHEDEE_H

#include <kernel/schedee.h>

struct cooperative_schedee 
{
	struct schedee sch;

	void* (*func) (void*);
	void*   priv;

	void * heap;
	int    heap_size;

	struct context ctx;
};

__BEGIN_DECLS

void cooperative_schedee_starter(void * arg) 
{
	struct cooperative_schedee * csch = container_of(
		arg, struct cooperative_schedee, sch);
}

void cooperative_schedee_init(
	struct cooperative_schedee * csch
) 
{
	context_init(&csch->ctx, cooperative_schedee_starter, csh);
}

__END_DECLS

#endif