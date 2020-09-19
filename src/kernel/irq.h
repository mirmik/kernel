#ifndef KERNEL_IRQ_H
#define KERNEL_IRQ_H

#include <sys/cdefs.h>
#include <asm/irq.h>

__BEGIN_DECLS
// Глобальное запрещение прерываний
void irqs_enable();

// Глобальное разрешение прерываний
void irqs_disable();

// Глобальное запрещение прерываний с запоминанием текущего состояния
irqsave_t irqs_save();

// Востановление ранее сохранённого состояния глобального разрешения прерываний
void irqs_restore(irqsave_t save);

__END_DECLS

#endif