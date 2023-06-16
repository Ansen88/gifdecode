#include "define.h"
#include <stdlib.h>
#include "alj.h"
#include "display.h"
#include "gif.h"
#include "lzw.h"
#include "stream.h"
#include "display.h"
#include "gifdata.h"

extern void * p(uint32_t num);

void gif (void)
{
	GifState *s;
    uint8_t code;
    uint32_t ret;
//    int tmp;

	ret=sizeof(GifState);
	s=(GifState *)p(ret);
    s->bytestream=pic;
    ret=sizeof(pic);
    s->bytestream_end=pic+ret;
    

//    tmp=gif_head(&s);
//    if(tmp<0)
//        goto end;

	bytestream_get_2byte(&s->bytestream);
	bytestream_get_2byte(&s->bytestream);
	bytestream_get_2byte(&s->bytestream);

    gif_init(s);

   do
   {
    code=bytestream_get_1byte(&s->bytestream);
    switch(code)
    {
        case ',':
            ret=gif_read_image(s);
            break;
        case '!':
            ret=gif_read_extension(s);
            break;
        case ';':
            ret=-1;
            break;
        default:
        	  continue;
    }
    if(ret==2)
    {
     display(s);
    }
  } while(ret!=-1);


end:
    ;

}
