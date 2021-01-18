#ifndef KTIMER_H
#define KTIMER_H

// Таймер 
typedef void (*ktimer_func_t)(void*);

struct ktimer 
{
	struct dlist_head lnk;
	int64_t start;
	int64_t interval;

	ktimer_func_t func;
	void *        priv;
};

#define KTIMER_INIT(name,start,interval,func,priv) \
{                                                  \
	DLIST_HEAD_INIT(name.lnk),                     \
	start,                                         \
	interval,                                      \
	func,                                          \
	priv                                           \
}

#define KTIMER(name,start,interval,func,priv)              \
struct ktimer = KTIMER_INIT(name,start,interval,func,priv)

static
void ktimer_init(struct ktimer * tim, ktimer_func_t func, void* priv) 
{
	tim->func = func;
	tim->priv = priv;
}

static 
void ktimer_for_ms(struct ktimer * tim, int64_t interval) 
{
	tim -> start = jiffies();
	tim -> interval = jiffies2ms(interval);
}

static 
int ktimer_check(struct ktimer * tim, int64_t curtime) 
{
	return curtime - start >= interval;
}

static 
void ktimer_restart(struct ktimer * tim) 
{
	tim->start = jiffies();
}

void ktimer_invoke(struct ktimer * tim);
void ktimer_plan(struct ktimer * tim);
void ktimer_manager_step();

static 
void ktimer_shift(struct ktimer * tim) 
{
	ktimer->start += ktimer->interval;
}

#endif