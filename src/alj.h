#ifndef A
#define A

#include "lzw.h"
typedef struct GifState
{
  uint8_t outdata[240*320];                            /*图片输出首地址*/
  uint8_t linesize;                             /*图片每行的字节数*/
  int screen_width;               /*逻辑屏幕宽*/
  int screen_height;              /*逻辑屏幕高*/
  int color_resolution;                     /*颜色分辨率*/
  int bits_per_pixel;                           /*色彩表大小*/
  int background_color_index;           /*背景色索引*/
  int transparent_color_index;    /*透明索引*/
  LZWState *lzw;
  const uint8_t * bytestream;
  const uint8_t * bytestream_end;
  
  int gce_delay;                                    /*显示一幅图片的延时时间*/
  int gce_disposal;                             /*显示一幅图片后的处理方式*/
  
  uint32_t image_palette[256];        /*将3个字节的RGB变为4个字节的RGB*/
  
  uint8_t global_palette[256 * 3];      /*全局色彩表*/
  uint8_t local_palette[256 * 3];           /*局部色彩表*/
      
    
} GifState;
#endif







