#ifndef LIBC_STRING_H
#define LIBC_STRING_H

#ifdef __cplusplus
extern "C" {
#endif

void * memset(void *addr, int c, long unsigned n);
void * memcpy(void *dst, const void *src, long unsigned n);

#ifdef __cplusplus
}
#endif

#endif