#include <kernel/ktimer.h>

DLIST_HEAD(ktimer_list);

// Добавление таймера в список обработки.
// Список поддерживается в отсортрованном состоянии с тем, 
// чтобы сложность проверки необходимости вызова таймера 
// при вызове функции ktimer_manager_step была О(1). 
void ktimer_plan(struct ktimer * tim) 
{
	struct ktimer_base * it;
	struct dlist_head * sit;
	int64_t it_final;
	int64_t final;

	final = start + interval;
	sit = NULL;

	irqstate_t save = irqs_save();

	dlist_for_each_entry(it, &ktimer_list, lnk)
	{
		it_final = it->start + it->interval;

		if (final - it_final < 0)
		{
			sit = &it->lnk;
			break;
		}
	}

	dlist_add_tail(&ctr.lnk, sit ? sit : &ktimer_list);

	irqs_restore(save);
}

void ktimer_manager_step()
{
	int64_t now;
	irqstate_t save;

	now = jiffies();
	save = irqs_save();

	while (!dlist_empty(&ktimer_list))
	{
		struct ktimer * it = 
			dlist_first_entry(&ktimer_list, struct ktimer, lnk);
		
		irqs_restore(save);

		if (ktimer_check(it, now)) 
		{
			dlist_del_init(&tim->lnk);
			it->func(it, it->priv);
		}
		else 
			return;
		
		save = irqs_save();
	}

	irqs_restore(save);
}