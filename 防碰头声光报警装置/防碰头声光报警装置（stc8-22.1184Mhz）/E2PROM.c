#include "E2PROM.h"



#define WT_30M          0x80
#define WT_24M          0x81
#define WT_20M          0x82
#define WT_12M          0x83
#define WT_6M           0x84
#define WT_3M           0x85
#define WT_2M           0x86
#define WT_1M           0x87

void IapIdle()//关闭IAP功能
{
    IAP_CONTR = 0;                              //关闭IAP功能
    IAP_CMD = 0;                                //清除命令寄存器
    IAP_TRIG = 0;                               //清除触发寄存器
    IAP_ADDRH = 0x80;                           //将地址设置到非IAP区域
    IAP_ADDRL = 0;
}

u8 IapRead(u16 addr)//读取指定addr的数据
{
    char dat;

    IAP_CONTR = WT_24M;                         //使能IAP
    IAP_CMD = 1;                                //设置IAP读命令
    IAP_ADDRL = addr;                           //设置IAP低地址
    IAP_ADDRH = addr >> 8;                      //设置IAP高地址
    IAP_TRIG = 0x5a;                            //写触发命令(0x5a)
    IAP_TRIG = 0xa5;                            //写触发命令(0xa5)
    _nop_();
    dat = IAP_DATA;                             //读IAP数据
    IapIdle();                                  //关闭IAP功能

    return dat;
}

void IapProgram(u16 addr, u8 dat)//向指定的addr写入DATA
{
    IAP_CONTR = WT_24M;                         //使能IAP
    IAP_CMD = 2;                                //设置IAP写命令
    IAP_ADDRL = addr;                           //设置IAP低地址
    IAP_ADDRH = addr >> 8;                      //设置IAP高地址
    IAP_DATA = dat;                             //写IAP数据
    IAP_TRIG = 0x5a;                            //写触发命令(0x5a)
    IAP_TRIG = 0xa5;                            //写触发命令(0xa5)
    _nop_();
	_nop_();
    IapIdle();                                  //关闭IAP功能
}

void IapErase(u16 addr)//清除指定扇区数据
{
    IAP_CONTR = WT_24M;                         //使能IAP
    IAP_CMD = 3;                                //设置IAP擦除命令
    IAP_ADDRL = addr;                           //设置IAP低地址
    IAP_ADDRH = addr >> 8;                      //设置IAP高地址
    IAP_TRIG = 0x5a;                            //写触发命令(0x5a)
    IAP_TRIG = 0xa5;                            //写触发命令(0xa5)
    _nop_();  
	_nop_();	//
    IapIdle();                                  //关闭IAP功能
}


myenum IfAddrData(u16 _addr,u8 _comparisonvalue)//读取_addr的数据与_comparisonvalue比较(正确输出Enable/错误输出Disable)
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

myenum CollectiveWritData(u16 _RestAdrr,u8 *_able,u8 _ablelength,u8 _length)//集体写入函数(默认第一扇区)
{
    int _RestAdrrVal=0;
    myenum _stateflag=Enable;
	if(_ablelength<_length)
	{
		_stateflag=Disable;
		return _stateflag;
	}
		
    IapErase(0x0000);//默认清除第一扇区(0-512 byte)
    for(_RestAdrrVal;_RestAdrrVal<_length;_RestAdrrVal++,_RestAdrr++,_able++)
    {
        IapProgram(_RestAdrr,*_able);
    }
    return _stateflag;
}

myenum CollectiveReadData(u16 _RestAdrr,u8 *_able,u8 _ablelength,u8 _length)//集体读出函数(默认第一扇区)
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



    

