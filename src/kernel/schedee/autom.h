#ifndef AUTOM_SCHEDEE_H
#define AUTOM_SCHEDEE_H

#include <kernel/schedee.h>

struct automate_schedee 
{
	struct schedee sch;

	void(*func)(void*, int*);
	void* priv;
	int   state;
};

#define AUTOMATE_SCHEDEE_INIT(name,func,priv)   \
{                                               \
	SCHEDEE_INIT(&name.sch),                    \
	func,                                       \
	priv,                                       \
	0                                           \
}

#define AUTOMATE_SCHEDEE(name,func,priv)                     \
struct automate_schedee name = AUTOMATE_SCHEDEE_INIT(        \
	name,func,priv);

__BEGIN_DECLS

void automate_schedee_init(struct automate_schedee * asch);

__END_DECLS

#endif