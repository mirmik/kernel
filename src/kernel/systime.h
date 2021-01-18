#ifndef KERNEL_SYSTIME_H
#define KERNEL_SYSTIME_H

#include <sys/cdefs.h>

__BEGIN_DECLS

int64_t jiffies();
int64_t millis();
int64_t micros();

void systime_tick();

__END_DECLS

#endif