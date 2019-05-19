#include "bitmapdisplay.h"
#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#include "ILI93xx.h"

extern GUI_BITMAP bmAutoparksystemInterface;  


//显示C文件格式的位图
void draw_bitmap(void)
{
	GUI_SetBkColor(GUI_WHITE);
	GUI_SetColor(GUI_YELLOW);
	GUI_Clear();
	
//	GUI_DrawBitmap(&bmAutoparksystemInterface,0,0);
	GUI_DrawBitmapEx(&bmAutoparksystemInterface,lcddev.width/2,lcddev.height/2,bmAutoparksystemInterface.XSize/2,bmAutoparksystemInterface.YSize/2,2000,2000); //按照比例绘制位图
}



