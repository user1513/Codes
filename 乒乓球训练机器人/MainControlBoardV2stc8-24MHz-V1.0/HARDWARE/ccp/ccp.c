#include "ccp.h"


/*
void PCA_Isr() interrupt 7 using 1
{
    if (CF)
    {
        CF = 0;
//        cnt++;                                  //PCA��ʱ�������+1
//		Uart1_SendStr("CF�жϣ�");
    }
    if (CCF0)
    {
        CCF0 = 0;
		Uart1_SendStr("CF�жϣ�");
//        count0 = count1;                        //������һ�εĲ���ֵ
//        ((unsigned char *)&count1)[3] = CCAP0L;
//        ((unsigned char *)&count1)[2] = CCAP0H;
//        ((unsigned char *)&count1)[1] = cnt;
//        ((unsigned char *)&count1)[0] = 0;
//        length = count1 - count0;              //length����ļ�Ϊ�����������
    }
}

void ccp_Init()
{

    CCON = 0x00;
    CMOD = 0x09;                                //PCAʱ��Ϊϵͳʱ��,ʹ��PCA��ʱ�ж�
    CL = 0x00;
    CH = 0x00;
    CCAPM0 = 0x11;                              //PCAģ��0Ϊ16λ����ģʽ���½��ز���
    //CCAPM0 = 0x21;                              //PCAģ��0Ϊ16λ����ģʽ���½��ز���
    //CCAPM0 = 0x31;                              //PCAģ��0Ϊ16λ����ģʽ�����½��ز���
    CCAP0L = 0x00;
    CCAP0H = 0x00;
    CR = 1;                                     //����PCA��ʱ��
    EA = 1;

}
*/

