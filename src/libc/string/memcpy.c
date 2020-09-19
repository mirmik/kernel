/* This file is derived from Newlib: http://sourceware.org/newlib/  */

/**
 * @file
 * @brief Implementation of #memcpy() function.
 *
 * @date 20.02.13
 * @author Eldar Abusalimov
 */

#include <string.h>
#include <stdint.h>

void * memcpy(void *dst, const void *src, long unsigned n)
{
	char * a = (char*) dst;
	char * b = (char*) src;

	while(n--) 
	{
		*a++ = *b++;
	}

	return dst;
}
