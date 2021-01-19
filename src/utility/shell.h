#ifndef KERNEL_ASHELL_H
#define KERNEL_ASHELL_H

struct ashell 
{
	struct resource * res;
	struct sline line;

	char * history;
	int    histlen;

	struct system_executor * executor;
};

void * ashell_main(void *, int * state);

void ashell_init(struct ashell * ash, 
	char * buffer, int len, char * history, int histlen);

void ashell_set_executor(struct ashell * ash,
	struct system_executor * executor);

#endif