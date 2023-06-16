#include "alj.h"
#include "define.h"
#include "config.h"

void display(GifState *s)
{
	static int i=0;
	
	LCD_CtrlWrite(RamAddrSet_Y);       
 	LCD_DataWrite(i);
 	LCD_CtrlWrite(RamAddrSet_X);       
 	LCD_DataWrite(0);
 	lcd_flush(s->outdata,240);
 	if(i<320)
 		i++;
 	else i=0;
}