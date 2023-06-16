/****************************************************************************************
* 文件名：LCMDISP.C
* 功能：GUI测试及演示程序。
* 作者：黄绍斌
* 日期：2003.09.09
****************************************************************************************/
#include "config.h"


#define		KEY1		(1<<16)     	/* ARM32 NEW TESTER和LCM接口直接相连,p1.16 */
#define		KEY2		(1<<17)     	/* ARM32 NEW TESTER和LCM接口直接相连,p1.17 */
#define		KEY3		(1<<18)     	/* ARM32 NEW TESTER和LCM接口直接相连,p1.18 */
#define		KEY4		(1<<19)     	/* ARM32 NEW TESTER和LCM接口直接相连,p1.19 */
#define		KEY5		(1<<20)     	/* ARM32 NEW TESTER和LCM接口直接相连,p1.20 */
#define		KEY6		(1<<21)     	/* ARM32 NEW TESTER和LCM接口直接相连,p1.21 */
#define		KEY7		(1<<22)     	/* ARM32 NEW TESTER和LCM接口直接相连,p1.22 */
#define		KEY8		(1<<23)     	/* ARM32 NEW TESTER和LCM接口直接相连,p1.23 */                     
/***********************************************************************
* 名称：main()
* 功能：主程序，用于GUI测试及演示。
***********************************************************************/
extern void gif (void);

int  main(void)
{  
   
   IO1DIR &= ~(KEY1+KEY2+KEY3+KEY4+KEY5+KEY6+KEY7+KEY8);	/*P1相关输出引脚初始化*/
      
   LCM_BusIni();
        
   LCD_Initialize();
        
/*   LCD_FillAll(0x0000);
   DelayNS(400);
   
   
   LCD_FillAll(0xFFFFFF);
   DelayNS(400);
*/   
   LCD_FillRect(0x0000,0,0,240,320);
   DelayNS(400);
         


   	gif();
   	DelayNS(80);   
 
 
  while(1);  
}
