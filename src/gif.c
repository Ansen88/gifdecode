#include "define.h"
#include "alj.h"
#include "lzw.h"
#include "stream.h"


static int memcmp(uint8_t *dst,uint8_t *src, uint8_t num)
{
	for(;num>0;num--)
	{
		if(*dst!=*src)
			return 1;
		dst++;
		src++;
	}
	return 0;
}

int gif_head(GifState * s)
{
    uint8_t sig[6];
    uint8_t gif87a_sig[7] = "GIF87a";
    uint8_t gif89a_sig[7] = "GIF89a";


    bytestream_get_buffer(&s->bytestream, sig, 6);
    if(memcmp(sig,gif87a_sig,6)==0 || memcmp(sig,gif89a_sig,6)==0)
        return 0;
    return -1;
}

void gif_init(GifState *s)
{
    uint8_t tmp;
    int n;

    ff_lzw_decode_open(&s->lzw);				//
    s->transparent_color_index=-1;
    s->gce_delay=0;
    s->gce_disposal=0;

    s->screen_width=bytestream_get_2byte(&s->bytestream);
    s->screen_height=bytestream_get_2byte(&s->bytestream);
    tmp=bytestream_get_1byte(&s->bytestream);
    s->background_color_index=bytestream_get_1byte(&s->bytestream);
    bytestream_get_1byte(&s->bytestream);

    s->color_resolution=((tmp&0x70)>>4)+1;
    s->bits_per_pixel=(tmp&0x7)+1;
    if(tmp & 0x80)
    {
	    n=1<<s->bits_per_pixel;
        bytestream_get_buffer(&s->bytestream,s->global_palette,3*n);
     } 
}

int gif_read_extension(GifState *s)
{
    int ext_code,ext_len,i, gce_flags, gce_transparent_index;;
    
    ext_code=bytestream_get_1byte(&s->bytestream);
    ext_len=bytestream_get_1byte(&s->bytestream);
    
      switch(ext_code) {
    case 0xf9:
        if (ext_len != 4)
            goto discard_ext;
        s->transparent_color_index = -1;
        gce_flags = bytestream_get_1byte(&s->bytestream);
        s->gce_delay = bytestream_get_2byte(&s->bytestream);
        gce_transparent_index = bytestream_get_1byte(&s->bytestream);
        if (gce_flags & 0x01)
            s->transparent_color_index = gce_transparent_index;
        else
            s->transparent_color_index = -1;
        s->gce_disposal = (gce_flags >> 2) & 0x7;
        ext_len = bytestream_get_1byte(&s->bytestream);
        break;
    }

    /* NOTE: many extension blocks can come after */
 discard_ext:
    while (ext_len != 0) 
    {
        for (i = 0; i < ext_len; i++)
            bytestream_get_1byte(&s->bytestream);
        ext_len = bytestream_get_1byte(&s->bytestream);
    }
    return 0;
}


#define RL24(x)       ((((const uint8_t*)(x))[2] << 16) | \
                     (((const uint8_t*)(x))[1] <<  8) | \
                      ((const uint8_t*)(x))[0])


int gif_read_image(GifState *s)
{
    uint16_t left, top, height, width;
    uint8_t tmp,has_local_palette, is_interleaved;
    uint8_t bits_per_pixel,linesize;
    uint8_t * palette,*ptr,*ptr1;
    int n,i,code_size, y, y1, pass;

    left=bytestream_get_2byte(&s->bytestream);
    top=bytestream_get_2byte(&s->bytestream);
    width=bytestream_get_2byte(&s->bytestream);
    height=bytestream_get_2byte(&s->bytestream);
    tmp=bytestream_get_1byte(&s->bytestream);

    bits_per_pixel=(tmp&0x7)+1;
    is_interleaved=tmp&0x40;
    has_local_palette=tmp&0x80;
    if(has_local_palette)
        {
            bytestream_get_buffer(&s->bytestream,s->local_palette,3*(1<<bits_per_pixel));
            palette=s->local_palette;
        }
    else
        {
            palette=s->global_palette;
            bits_per_pixel = s->bits_per_pixel;
        }

        if((top+height)>s->screen_height||(left+width)>s->screen_width)
        {
                
            return -1;
        }

        n=(1<<bits_per_pixel);
        for (i=0;i<n;i++)
        {
            s->image_palette[i]=((uint32_t)0xff<<24)|RL24(palette);
            palette+=3;
        }
     for(;i<256;i++)
     {
            s->image_palette[i]=((uint32_t)0xff<<24);
     }
     if(s->transparent_color_index>=0)
            s->image_palette[s->transparent_color_index]=0;
     code_size=bytestream_get_1byte(&s->bytestream);
     ff_lzw_decode_init(s->lzw, code_size, s->bytestream,s->bytestream_end - s->bytestream, FF_LZW_GIF);

   linesize=s->screen_width;
   ptr1=s->outdata + top * linesize + left;
   ptr=ptr1;
   y1=0;
   pass=0;
   for(y=0;y<height;y++)
   {
    ff_lzw_decode(s->lzw, ptr, width);
        if (is_interleaved)
         {
            switch(pass)
            {
            default:
            case 0:
            case 1:
                y1 += 8;
                ptr += linesize * 8;
                if (y1 >= height) 
                {
                    y1 = pass ? 2 : 4;
                    ptr = ptr1 + linesize * y1;
                    pass++;
                }
                break;
            case 2:
                y1 += 4;
                ptr += linesize * 4;
                if (y1 >= height) 
                {
                    y1 = 1;
                    ptr = ptr1 + linesize;
                    pass++;
                }
                break;
            case 3:
                y1 += 2;
                ptr += linesize * 2;
                break;
            }
        } else 
        {
            ptr += linesize;
        }
   }
    ff_lzw_decode_tail(s->lzw);
    s->bytestream = ff_lzw_cur_ptr(s->lzw);
    return 2;  
}
