#ifndef KERNEL_IRQTABLE_H
#define KERNEL_IRQTABLE_H

typedef void(*irq_handler_t)(void*);

struct irq_record 
{
	irq_handler_t func;
	void *        priv;

	int32_t counter;
};

#endif