#include "E2PROM.h"



#define WT_30M          0x80
#define WT_24M          0x81
#define WT_20M          0x82
#define WT_12M          0x83
#define WT_6M           0x84
#define WT_3M           0x85
#define WT_2M           0x86
#define WT_1M           0x87

void IapIdle()//�ر�IAP����
{
    IAP_CONTR = 0;                              //�ر�IAP����
    IAP_CMD = 0;                                //�������Ĵ���
    IAP_TRIG = 0;                               //��������Ĵ���
    IAP_ADDRH = 0x80;                           //����ַ���õ���IAP����
    IAP_ADDRL = 0;
}

u8 IapRead(u16 addr)//��ȡָ��addr������
{
    char dat;

    IAP_CONTR = WT_24M;                         //ʹ��IAP
    IAP_CMD = 1;                                //����IAP������
    IAP_ADDRL = addr;                           //����IAP�͵�ַ
    IAP_ADDRH = addr >> 8;                      //����IAP�ߵ�ַ
    IAP_TRIG = 0x5a;                            //д��������(0x5a)
    IAP_TRIG = 0xa5;                            //д��������(0xa5)
    _nop_();
    dat = IAP_DATA;                             //��IAP����
    IapIdle();                                  //�ر�IAP����

    return dat;
}

void IapProgram(u16 addr, u8 dat)//��ָ����addrд��DATA
{
    IAP_CONTR = WT_24M;                         //ʹ��IAP
    IAP_CMD = 2;                                //����IAPд����
    IAP_ADDRL = addr;                           //����IAP�͵�ַ
    IAP_ADDRH = addr >> 8;                      //����IAP�ߵ�ַ
    IAP_DATA = dat;                             //дIAP����
    IAP_TRIG = 0x5a;                            //д��������(0x5a)
    IAP_TRIG = 0xa5;                            //д��������(0xa5)
    _nop_();
	_nop_();
    IapIdle();                                  //�ر�IAP����
}

void IapErase(u16 addr)//���ָ����������
{
    IAP_CONTR = WT_24M;                         //ʹ��IAP
    IAP_CMD = 3;                                //����IAP��������
    IAP_ADDRL = addr;                           //����IAP�͵�ַ
    IAP_ADDRH = addr >> 8;                      //����IAP�ߵ�ַ
    IAP_TRIG = 0x5a;                            //д��������(0x5a)
    IAP_TRIG = 0xa5;                            //д��������(0xa5)
    _nop_();  
	_nop_();	//
    IapIdle();                                  //�ر�IAP����
}


myenum IfAddrData(u16 _addr,u8 _comparisonvalue)//��ȡ_addr��������_comparisonvalue�Ƚ�(��ȷ���Enable/�������Disable)
{
    u8 _tempval=0x00;
    myenum _stateflag=Disable;
    _tempval=IapRead(_addr);
    if(_tempval==_comparisonvalue)
    {
        _stateflag=Enable;
    }
    return _stateflag;
}

myenum CollectiveWritData(u16 _RestAdrr,u8 *_able,u8 _ablelength,u8 _length)//����д�뺯��(Ĭ�ϵ�һ����)
{
    int _RestAdrrVal=0;
    myenum _stateflag=Enable;
	if(_ablelength<_length)
	{
		_stateflag=Disable;
		return _stateflag;
	}
		
    IapErase(0x0000);//Ĭ�������һ����(0-512 byte)
    for(_RestAdrrVal;_RestAdrrVal<_length;_RestAdrrVal++,_RestAdrr++,_able++)
    {
        IapProgram(_RestAdrr,*_able);
    }
    return _stateflag;
}

myenum CollectiveReadData(u16 _RestAdrr,u8 *_able,u8 _ablelength,u8 _length)//�����������(Ĭ�ϵ�һ����)
{
    int _RestAdrrVal=0;
    myenum _stateflag=Enable;
	if(_ablelength<_length)
	{
		_stateflag=Disable;
		return _stateflag;
	}
	for(_RestAdrrVal=0;_RestAdrrVal<_length;_RestAdrrVal++,_RestAdrr++,_able++)
    {
        *_able=IapRead(_RestAdrr);
    }
    return _stateflag;
}



    

