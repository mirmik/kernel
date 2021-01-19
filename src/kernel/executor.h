#ifndef KERNEL_SYSTEM_EXECUTOR_H
#define KERNEL_SYSTEM_EXECUTOR_H

#define SYSCMD_FUNCTION    0
#define SYSCMD_AUTOMATE    1
#define SYSCMD_PREEMPT     2

#define SYSCMD_FIN {NULL,NULL,NULL,0}

struct syscmd 
{
	const char * mnemo;
	void      (* func)(int,char**);
	const char * comment;
	int8_t       type;
};

struct system_executor 
{
	struct syscmd ** table;
};

__BEGIN_DECLS

void system_executor_execute(struct system_executor * executor)

static
void system_executor_init(
	struct system_executor * executor, 
	struct syscmd ** table
) 
{
	executor->table = table;
}

__END_DECLS

#endif