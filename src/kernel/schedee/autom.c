#include <kernal/schedee/autom.h>

void autom_execute(struct schedee * sch) 
{
	struct automate_schedee * asch = container_of(
		sch, struct automate_schedee, sch);
	
	asch->func(asch->priv, &asch->state);
}

const struct schedee_operations automate_schedee_operations = 
{
	.execute = autom_execute,
	.displace = NULL,
	.finalize = NULL
};

void automate_schedee_init(struct automate_schedee * asch) 
{
	schedee_init(&asch->sch, &automate_schedee_operations); 
}