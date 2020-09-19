#ifndef ARCH_ARM_V7E_IRQ_H
#define ARCH_ARM_V7E_IRQ_H

// реализация kernel/irq.h для arm_v7e процессоров

#include <stdint.h>

typedef unsigned int irqsave_t;

static inline void irqs_enable(void) {
	__asm__ __volatile__ (
		"cpsie i \n\t");

}

static inline irqsave_t irqs_save(void) {
	uint32_t r;
	__asm__ __volatile__ (
		"mrs %0, PRIMASK;\n\t"
		"cpsid i \n\t"
		: "=r"(r));
	return r;
}

static inline void irqs_restore(irqsave_t state) {
	__asm__ __volatile__ (
		"msr PRIMASK, %0;\n\t"
		:
		: "r"(state));
}

static inline void irqs_disable(void) {
	(void) irqs_save();
}

#endif