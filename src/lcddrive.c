/****************************************************************************************
* 文件名：LCDDRIVE.C
* 功能：图形液晶240(RGB)*320驱动
* 说明：图形液晶采用SSD1289为LCD控制芯片，并行接口(使用LPC2210驱动)。
****************************************************************************************/
#include "config.h"

#define SCS         	 (*((volatile unsigned long *) 0xE01FC1A0))
#define FIO0PIN          (*((volatile unsigned long *) 0x3FFFC014))      /* no in lpc210x*/
#define FIO0SET          (*((volatile unsigned long *) 0x3FFFC018))      /* no in lpc210x*/
#define FIO0DIR          (*((volatile unsigned long *) 0x3FFFC000))      /* no in lpc210x*/
#define FIO0CLR          (*((volatile unsigned long *) 0x3FFFC01C))      /* no in lpc210x*/
#define FIO0MASK         (*((volatile unsigned long *) 0x3FFFC010))      /* no in lpc210x*/
                       
/*当系统总线为18位时，可如下定义：
#define		LCM_BUS		0x0003ffff

一个点的RGB数据分别存储在Dat1、Dat2、Dat3中，数据格式为高6位有效则可如下写到模块中：
IO0PIN = (Dat1<<10)+(Dat2<<4)+(Dat3>>2);

若数据格式为低6位有效则可如下写到模块中：
IO0PIN = (Dat1<<12)+(Dat2<<6)+Dat3;
 */	

/***********************************************************************
* 名称：LCM_BusIni()
* 功能：总线初始化
** 输　  入: 无
** 输　  出: 无
** 全局变量:  
** 调用模块: 
***********************************************************************/
void LCM_BusIni()
{
	SCS=1;
	FIO0MASK= 0x00000000;
	PINSEL0 = 0x00000000;		                        /*使用到的P0口GPIO引脚设置*/
	PINSEL1 = 0x00000000;		                        /*使用到的P0口GPIO引脚设置*/
	FIO0DIR |= (LCM_BUS+LCM_RS+LCM_CS+LCM_RST+LCM_RD+LCM_WR);	/*P0相关输出引脚初始化*/
	FIO0SET |= LCM_CS;
		
	FIO0CLR |= LCM_RST;				                    /*复位*/
    DelayNS(20);
    FIO0SET |= LCM_RST;
    FIO0CLR = LCM_CS;									/*片选置低*/
}

/***********************************************************************
* 名称：LCD_WriteCommand()
* 功能：写命令子程序。(发送命令前，不检查液晶模块的状态)
* 入口参数：command  	要写入LCM的命令字
* 出口参数：无
* 说明：函数会设置LCM数据总线为输出方式
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
* 名称：DelayNS()
* 功能：长软件延时。
* 入口参数：dly		延时参数，值越大，延时越久
* 出口参数：无
****************************************************************************/
void  DelayNS(uint32  dly)
{  uint32  i;

   for(; dly>0; dly--) 
      for(i=3000; i>0; i--);
}

/***********************************************************************
* 名称：LCD_Initialize()
* 功能：LCM初始化。
* 入口参数：无
* 出口参数：无
***********************************************************************/
/*  LCD初始化   */

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
* 名称：LCD_FillAll()
* 功能：LCD填充。以图形方式进行填充，起始地址为0x0000。
* 入口参数：dat		要填充的数据
* 出口参数：无
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

