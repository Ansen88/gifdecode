#ifndef A
#define A

#include "lzw.h"
typedef struct GifState
{
  uint8_t outdata[240*320];                            /*ͼƬ����׵�ַ*/
  uint8_t linesize;                             /*ͼƬÿ�е��ֽ���*/
  int screen_width;               /*�߼���Ļ��*/
  int screen_height;              /*�߼���Ļ��*/
  int color_resolution;                     /*��ɫ�ֱ���*/
  int bits_per_pixel;                           /*ɫ�ʱ��С*/
  int background_color_index;           /*����ɫ����*/
  int transparent_color_index;    /*͸������*/
  LZWState *lzw;
  const uint8_t * bytestream;
  const uint8_t * bytestream_end;
  
  int gce_delay;                                    /*��ʾһ��ͼƬ����ʱʱ��*/
  int gce_disposal;                             /*��ʾһ��ͼƬ��Ĵ���ʽ*/
  
  uint32_t image_palette[256];        /*��3���ֽڵ�RGB��Ϊ4���ֽڵ�RGB*/
  
  uint8_t global_palette[256 * 3];      /*ȫ��ɫ�ʱ�*/
  uint8_t local_palette[256 * 3];           /*�ֲ�ɫ�ʱ�*/
      
    
} GifState;
#endif







