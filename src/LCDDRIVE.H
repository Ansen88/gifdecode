/****************************************************************************************
* 文件名：LCDDRIVE.H
* 功能：LCD驱动程序，包括底层驱动，刷新显示子程序。
* 说明：
****************************************************************************************/
#ifndef  LCDDRIVE_H
#define  LCDDRIVE_H


/******** Select Module *******/
/* 模块选择:0为非选择;1为选择 */
#define JY07A15_VA      0
#define JY06B63_VH      1
#define JY05B39_VB      0
/********* Select Chip ********/
/* 芯片选择:0为非选择;1为选择 */
#define SSD1289         1
#define R61505U         0
#define ILI9320         0
#define HX8312A         0
/**********************************************/
#if JY06B63_VH
/* 定义LCM地址 */
#define		LCM_BUS		0x0088ffff	/* ARM32 NEW TESTER和LCM接口直接相连,p0.00~p0.15 */
#define		LCM_RS		(1<<18)    	/* ARM32 NEW TESTER和LCM接口直接相连,p0.18 */
#define		LCM_CS		(1<<17)    	/* ARM32 NEW TESTER和LCM接口直接相连,p0.17 */
#define		LCM_RST		(1<<16)    	/* ARM32 NEW TESTER和LCM接口直接相连,p0.16 */
#define		LCM_RD		(1<<23)    	/* ARM32 NEW TESTER和LCM接口直接相连,p0.23 */
#define		LCM_WR		(1<<22)   	/* ARM32 NEW TESTER和LCM接口直接相连,p0.22 */
#endif

/**********************************************/

/**********************************************/
#if SSD1289
#define RamAddrSet_X          0x4E
#define RamAddrSet_Y          0x4F
#define WriteRamData_Index    0x22
#endif
/**********************************************/

/**********************************************/

/**********************************************/
extern void  LCD_Initialize(void);
extern void  LCD_FillAll(uint32 dat1);
extern void  DelayNS(uint32  dly);
//extern void  LCD_DispPic(const uint8 PIC_Addr[]);
extern void  LCD_CtrlWrite(uint16 command);
extern void  LCD_DataWrite(uint32 dat);
extern void  LCM_BusIni(void);
//extern void  Set_LCD_REG(uint16 reg,uint16 dat);
extern void  set_palette(uint32 *p);
extern void  lcd_flush(uint8 *dat,int num);
extern void	 LCD_FillRect(uint32 dat1,unsigned short top,unsigned short left,unsigned short w,unsigned short h);

/*
extern const uint8 PIC01[];

extern const uint8 PIC02[];

extern const uint8 PIC03[];

extern const uint8 PIC04[];

extern const uint8 PIC05[];

extern const uint8 PIC06[];

extern const uint8 PIC07[];

*/

//extern const uint8 PIC08[];

#endif