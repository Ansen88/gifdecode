#include "define.h"


#define AV_RL16(x)  ((((const uint8_t*)(x))[1] << 8) | \
                      ((const uint8_t*)(x))[0])
                      
#define AV_RB8(x)     (((const uint8_t*)(x))[0])

static void memcpy(uint8_t *dst,const uint8_t *src,unsigned int size)
{	
	for(;size>0;size--)
	{
		*dst=*src;
		dst++;
		src++;
	}
}

unsigned int bytestream_get_buffer(const uint8_t **b, uint8_t *dst, unsigned int size)
{
    memcpy(dst, *b, size);
    (*b) += size;
    return size;
}

uint16_t bytestream_get_2byte(const uint8_t **b)
{
	(*b)+=2;
	return AV_RL16(*b-2); 
}

uint8_t bytestream_get_1byte(const uint8_t **b)
{
	(*b)++;
	return AV_RB8(*b-1);
}
