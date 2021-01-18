#include <kernel/schedee.h>

struct schedee * _current_schedee;

struct schedee * current_schedee() 
{
	return _current_schedee;
}

void set_current_schedee(struct schedee * sch) 
{
	_current_schedee = sch;
}

void schedee_init(struct schedee * sch)
{

//#if SCHEDEE_DEBUG_STRUCT

	// В дальнейшем эту провеку следует убрать, так как нод
	// должен отстыковываться от списка по завершению работы.
	if (!dlist_in(&sch->schedee_list_lnk, &schedee_list))
	{
		ctrobj_init(&sch->ctr, CTROBJ_SCHEDEE_LIST);
		dlist_add(&sch->schedee_list_lnk, &schedee_list);
	}

	else
	{
		dlist_del_init(&sch->ctr.lnk);
	}

	sch->pid = generate_new_pid();

	if (flags & SCHEDEE_USE_PARENT_GID)
		sch->gid = current_schedee()->gid;
	else
		sch->gid = generate_new_gid();

	sch->dispcounter = 0;
	sch->execcounter = 0;
//#else
//	ctrobj_init(&sch->ctr, CTROBJ_SCHEDEE_LIST);
//#endif

	sch->prio = SCHEDEE_PRIORITY_LOW;
	sch->sch_state = SCHEDEE_STATE_STOP;
	
	sch->flags = 0;
	//sch->syslock_counter_save = 0;

	sch->parent = current_schedee();

	sch->local_errno = 0;
}