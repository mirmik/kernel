#ifndef KERNEL_UTILITY_CONTTY_H
#define KERNEL_UTILITY_CONTTY_H

#include <kernel/utility/executor.h>

#define RESOURCE_TABLE_SIZE 5

struct contty_context
{
	struct openres restbl_array[RESOURCE_TABLE_SIZE];
	struct resource_table restbl;

	struct system_executor * sysexec;
};

void contty_main(void * priv, int * state);

#endif
