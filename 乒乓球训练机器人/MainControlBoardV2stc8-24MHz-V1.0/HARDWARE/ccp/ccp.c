#include "ccp.h"


/*
void PCA_Isr() interrupt 7 using 1
{
    if (CF)
    {
        CF = 0;
//        cnt++;                                  //PCA计时溢出次数+1
//		Uart1_SendStr("CF中断：");
    }
    if (CCF0)
    {
        CCF0 = 0;
		Uart1_SendStr("CF中断：");
//        count0 = count1;                        //备份上一次的捕获值
//        ((unsigned char *)&count1)[3] = CCAP0L;
//        ((unsigned char *)&count1)[2] = CCAP0H;
//        ((unsigned char *)&count1)[1] = cnt;
//        ((unsigned char *)&count1)[0] = 0;
//        length = count1 - count0;              //length保存的即为捕获的脉冲宽度
    }
}

void ccp_Init()
{

    CCON = 0x00;
    CMOD = 0x09;                                //PCA时钟为系统时钟,使能PCA计时中断
    CL = 0x00;
    CH = 0x00;
    CCAPM0 = 0x11;                              //PCA模块0为16位捕获模式（下降沿捕获）
    //CCAPM0 = 0x21;                              //PCA模块0为16位捕获模式（下降沿捕获）
    //CCAPM0 = 0x31;                              //PCA模块0为16位捕获模式（上下降沿捕获）
    CCAP0L = 0x00;
    CCAP0H = 0x00;
    CR = 1;                                     //启动PCA计时器
    EA = 1;

}
*/

