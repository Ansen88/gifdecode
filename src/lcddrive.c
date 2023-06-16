/****************************************************************************************
* �ļ�����LCDDRIVE.C
* ���ܣ�ͼ��Һ��240(RGB)*320����
* ˵����ͼ��Һ������SSD1289ΪLCD����оƬ�����нӿ�(ʹ��LPC2210����)��
****************************************************************************************/
#include "config.h"

#define SCS         	 (*((volatile unsigned long *) 0xE01FC1A0))
#define FIO0PIN          (*((volatile unsigned long *) 0x3FFFC014))      /* no in lpc210x*/
#define FIO0SET          (*((volatile unsigned long *) 0x3FFFC018))      /* no in lpc210x*/
#define FIO0DIR          (*((volatile unsigned long *) 0x3FFFC000))      /* no in lpc210x*/
#define FIO0CLR          (*((volatile unsigned long *) 0x3FFFC01C))      /* no in lpc210x*/
#define FIO0MASK         (*((volatile unsigned long *) 0x3FFFC010))      /* no in lpc210x*/
                       
/*��ϵͳ����Ϊ18λʱ�������¶��壺
#define		LCM_BUS		0x0003ffff

һ�����RGB���ݷֱ�洢��Dat1��Dat2��Dat3�У����ݸ�ʽΪ��6λ��Ч�������д��ģ���У�
IO0PIN = (Dat1<<10)+(Dat2<<4)+(Dat3>>2);

�����ݸ�ʽΪ��6λ��Ч�������д��ģ���У�
IO0PIN = (Dat1<<12)+(Dat2<<6)+Dat3;
 */	

/***********************************************************************
* ���ƣ�LCM_BusIni()
* ���ܣ����߳�ʼ��
** �䡡  ��: ��
** �䡡  ��: ��
** ȫ�ֱ���:  
** ����ģ��: 
***********************************************************************/
void LCM_BusIni()
{
	SCS=1;
	FIO0MASK= 0x00000000;
	PINSEL0 = 0x00000000;		                        /*ʹ�õ���P0��GPIO��������*/
	PINSEL1 = 0x00000000;		                        /*ʹ�õ���P0��GPIO��������*/
	FIO0DIR |= (LCM_BUS+LCM_RS+LCM_CS+LCM_RST+LCM_RD+LCM_WR);	/*P0���������ų�ʼ��*/
	FIO0SET |= LCM_CS;
		
	FIO0CLR |= LCM_RST;				                    /*��λ*/
    DelayNS(20);
    FIO0SET |= LCM_RST;
    FIO0CLR = LCM_CS;									/*Ƭѡ�õ�*/
}

/***********************************************************************
* ���ƣ�LCD_WriteCommand()
* ���ܣ�д�����ӳ���(��������ǰ�������Һ��ģ���״̬)
* ��ڲ�����command  	Ҫд��LCM��������
* ���ڲ�������
* ˵��������������LCM��������Ϊ�����ʽ
***********************************************************************/

void  LCD_CtrlWrite(uint16 command)
{   



	FIO0CLR=LCM_CS;
	FIO0CLR=LCM_RS;
	FIO0CLR=LCM_BUS;
	FIO0SET=command;
	FIO0CLR=LCM_WR;
	FIO0SET=LCM_WR;
	FIO0SET=LCM_CS;

	
}
//***********************************************************************
void  LCD_DataWrite(uint32 dat)
{   		


	FIO0CLR=LCM_CS;
	FIO0SET=LCM_RS;
	FIO0CLR=LCM_BUS;
	FIO0SET=(dat & LCM_BUS);
	FIO0CLR=LCM_WR;
	FIO0SET=LCM_WR;
	FIO0SET=LCM_CS;

}

static uint32 * palette;

void set_palette(uint32 *p)
{
	palette=p;
}

void lcd_flush(uint8 * dat,int num)
{
	
	FIO0CLR=LCM_CS;
	FIO0CLR=LCM_RS;
	FIO0CLR=LCM_BUS;
	FIO0SET=WriteRamData_Index;
	FIO0CLR=LCM_WR;
	FIO0SET=LCM_WR;
	
	for(;num>0;num--)
	{
		FIO0CLR=LCM_BUS;
		FIO0SET=LCM_RS;
		FIO0SET=(palette[*dat]& LCM_BUS);;
		FIO0CLR=LCM_WR;
		FIO0SET=LCM_WR;
		dat++;
	}
	FIO0SET=LCM_CS;
	
}



/****************************************************************************
* ���ƣ�DelayNS()
* ���ܣ��������ʱ��
* ��ڲ�����dly		��ʱ������ֵԽ����ʱԽ��
* ���ڲ�������
****************************************************************************/
void  DelayNS(uint32  dly)
{  uint32  i;

   for(; dly>0; dly--) 
      for(i=3000; i>0; i--);
}

/***********************************************************************
* ���ƣ�LCD_Initialize()
* ���ܣ�LCM��ʼ����
* ��ڲ�������
* ���ڲ�������
***********************************************************************/
/*  LCD��ʼ��   */

void  LCD_Initialize(void)
{ 

	LCD_CtrlWrite(0x07);LCD_DataWrite(0x21);
	LCD_CtrlWrite(0x00);LCD_DataWrite(0x01);
	DelayNS(80);
	LCD_CtrlWrite(0x07);LCD_DataWrite(0x23);
	LCD_CtrlWrite(0x10);LCD_DataWrite(0x00);
	DelayNS(60);
	LCD_CtrlWrite(0x07);LCD_DataWrite(0x33);
	LCD_CtrlWrite(0x11);LCD_DataWrite(0x4070);
	DelayNS(60);
	LCD_CtrlWrite(0x02);LCD_DataWrite(0x00);
	DelayNS(60);
	LCD_CtrlWrite(0x01);LCD_DataWrite(0x633F);
	LCD_CtrlWrite(0x03);LCD_DataWrite(0xA8AA);
	LCD_CtrlWrite(0x0C);LCD_DataWrite(0x03);
	LCD_CtrlWrite(0x0D);LCD_DataWrite(0x0C);
	LCD_CtrlWrite(0x05);LCD_DataWrite(0x00);
	LCD_CtrlWrite(0x06);LCD_DataWrite(0x00);
	LCD_CtrlWrite(0x16);LCD_DataWrite(0xEF1C);
	LCD_CtrlWrite(0x17);LCD_DataWrite(0x00);
	LCD_CtrlWrite(0x0B);LCD_DataWrite(0x30);
	LCD_CtrlWrite(0x0F);LCD_DataWrite(0x00);
	LCD_CtrlWrite(0x1E);LCD_DataWrite(0xB0);
	LCD_CtrlWrite(0x41);LCD_DataWrite(0x00);
	LCD_CtrlWrite(0x42);LCD_DataWrite(0x00);
	LCD_CtrlWrite(0x44);LCD_DataWrite(0xEF00);
	LCD_CtrlWrite(0x45);LCD_DataWrite(0x00);
	LCD_CtrlWrite(0x46);LCD_DataWrite(0x013F);
	LCD_CtrlWrite(0x48);LCD_DataWrite(0x00);
	LCD_CtrlWrite(0x49);LCD_DataWrite(0x013F);
	LCD_CtrlWrite(0x4A);LCD_DataWrite(0x00);
	LCD_CtrlWrite(0x4B);LCD_DataWrite(0x00);
	LCD_CtrlWrite(0x23);LCD_DataWrite(0x00);
	LCD_CtrlWrite(0x24);LCD_DataWrite(0x00);
}     


/***********************************************************************
* ���ƣ�LCD_FillAll()
* ���ܣ�LCD��䡣��ͼ�η�ʽ������䣬��ʼ��ַΪ0x0000��
* ��ڲ�����dat		Ҫ��������
* ���ڲ�������
***********************************************************************/
/*
void  LCD_FillAll(uint32 dat1)
{  uint16  x;
   uint16  y;
     
   LCD_CtrlWrite(RamAddrSet_X);       
   LCD_DataWrite(0x0000);

   LCD_CtrlWrite(RamAddrSet_Y);       
   LCD_DataWrite(0x0000);

   LCD_CtrlWrite(WriteRamData_Index);
   for(y=0;y<320;y++)
   {
      for(x=0;x<240;x++)
        {  
           LCD_DataWrite(dat1);
        }
    }
   
}

*/

void LCD_FillRect(uint32 dat1,unsigned short top,unsigned short left,unsigned short w,unsigned short h)
{
	unsigned short x,y;
	
	for(y=top;y<(top+h);y++)
	{
		LCD_CtrlWrite(RamAddrSet_Y);       
        LCD_DataWrite(y);

   		LCD_CtrlWrite(RamAddrSet_X);       
   		LCD_DataWrite(left);
   		
   		FIO0CLR=LCM_CS;
		FIO0CLR=LCM_RS;
		FIO0CLR=LCM_BUS;
		FIO0SET=WriteRamData_Index;
		FIO0CLR=LCM_WR;
		FIO0SET=LCM_WR;
		


//		LCD_CtrlWrite(WriteRamData_Index);
		for(x=0;x<w;x++)
		{

			FIO0CLR=LCM_BUS;
			FIO0SET=LCM_RS;
			FIO0SET=(dat1 & LCM_BUS);
			FIO0CLR=LCM_WR;
			FIO0SET=LCM_WR;
			


//			LCD_DataWrite(dat1);
		}
	}
}

