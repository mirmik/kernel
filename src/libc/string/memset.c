#include <string.h>
#include <stdint.h>

void * memset(void *addr, int c, long unsigned n)
{
	int8_t * ptr = (int8_t *) addr;
	
	while(n--) 
	{
		*ptr++ = c;
	} 

	return addr;
}
