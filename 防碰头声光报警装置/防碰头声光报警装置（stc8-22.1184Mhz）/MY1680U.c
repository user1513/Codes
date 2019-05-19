#include "MY1680U.h"

u32 CallBackNum=0;//����ѭ�����ʹ���
myenum My1680uFlag;//����uart2������ձ�־λ
typedef xdata struct
{
	u32 ulTimeout;//����ʱ��
	u32 ulTimesave;//����ʱ��洢
	u8 ucInuse;//�жϺ����Ƿ�ע��
	u8 para[7];//��������
	u8 length; //�������ݳ���
	pFuncSystickCallBack  Callback_Func;//���ûص�����

}stSystickCallBack;

stSystickCallBack Myu1680CallbackBuf;

//����my1680u.h�ڲ����ûص�����
void MYU1680U_SetCallBack(u32 ulTimeout,
                          pFuncSystickCallBack callBack,
                          u8 *_para,
						  u8 _length)
{
    if(Myu1680CallbackBuf.ucInuse==0)
    {
        Myu1680CallbackBuf.Callback_Func=callBack;
        Myu1680CallbackBuf.ulTimeout=ulTimeout;
        Myu1680CallbackBuf.ulTimesave=ulTimeout;
		Myu1680CallbackBuf.length=_length;
		memcpy(&Myu1680CallbackBuf.para, _para, _length);//ȡ�ڲ���;
        Myu1680CallbackBuf.ucInuse=0x01;
    }
}
//�жϻص�����
 void HAL_MYU1680U_Callback(void)
{
    if(Myu1680CallbackBuf.ucInuse!=0)
    {
        Myu1680CallbackBuf.ulTimeout--;
        if(Myu1680CallbackBuf.ulTimeout==0)
        {
			CallBackNum++;
			Myu1680CallbackBuf.ucInuse=0x00;
			Myu1680CallbackBuf.ulTimeout=Myu1680CallbackBuf.ulTimesave;
            Myu1680CallbackBuf.Callback_Func(Myu1680CallbackBuf.para,
											 Myu1680CallbackBuf.length); 
        }
    }
}
//�رջص�����
 void MYU1680U_CallBackClose()
 {
	 CallBackNum=0;
    Myu1680CallbackBuf.ucInuse=0;
    Myu1680CallbackBuf.Callback_Func=0;
    Myu1680CallbackBuf.ulTimeout=0;
    Myu1680CallbackBuf.ulTimesave=0;
 }

//��ʼ�� ���� ������ У���� ������
myenum My1680uControl(u8 _cmd,
					u8 _cmdlength,
					u16 _data)
{
	u8 xdata _able[7]; 
    myenum MY1680UFLAG=Disable;
	_able[0]=0X7E;//��ʼ��
	_able[1]=_cmdlength;//���ȣ�������->�����룩
	if((_cmdlength>2)&&(_cmdlength<6))
	{
		if(_cmdlength==3)
		{
           _able[2]=_cmd;
           _able[3]=XorCheck(_able+1,2);
           _able[4]=0XEF;
           
        }
        if(_cmdlength==4)
		{
           _able[2]=_cmd;
           _able[3]=(u8)_data;
           _able[4]=XorCheck(_able+1,3);
           _able[5]=0XEF;
        }
        if(_cmdlength==5)
		{
           _able[2]=_cmd;
           _able[3]=(u8)(_data>>8);
           _able[4]=(u8)(_data);
           _able[5]=XorCheck(_able+1,4);
           _able[6]=0XEF;
        }
			My1680uFlag=Enable;
			Uart2SendStrLen(_able,_cmdlength+2);
		    while(1)
           {
			   if(My1680uFlag==Disable)
			   {
				   MYU1680U_CallBackClose();
				   MY1680UFLAG=Enable;
				   break;
			   }
			   if(CallBackNum==5)
			   {
				   MYU1680U_CallBackClose();
				   break;
			   }
              MYU1680U_SetCallBack( 100,
                                   (pFuncSystickCallBack) Uart2SendStrLen,
									_able,
                                    _cmdlength+2);
           }
	}
	return MY1680UFLAG;
}

u8 XorCheck(u8 *_able,u8 length)
{
    u8 xorval=*_able;
    int i=0;
    for(i=0;i<length-1;i++)
    {
        xorval ^=*(_able+1+i);
    }
    return xorval;
}

void musicplaymode(u8 num)
{
		My1680uControl(STOPMODE,DATALENGTH3,DATANONE);
		Delayms(20);
		My1680uControl(CHOOSEPLAYSONGS,DATALENGTH5,num);
}




















