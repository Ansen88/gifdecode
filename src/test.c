#include "define.h"

extern unsigned int bottom_of_heap;

void * p(uint32_t num)
{
	static unsigned int * pos=&bottom_of_heap;
	
	pos+=num;
	return (void *)(pos-num);
}