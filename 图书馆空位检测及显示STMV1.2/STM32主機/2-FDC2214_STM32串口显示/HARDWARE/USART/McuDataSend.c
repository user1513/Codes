#include "McuDataSend.h"


#define VARMAX 10

typedef struct MYFUNC
{
	u8 Reg;//ע��
	bool *FlagName;//��־���Q
	u32 DelayTime;//�ӕr�r�g
	bool  FlagState;//�˜ʠ�B
	
}MyFunc;


MyFunc FlagStateFunc[VARMAX];//���x����ͬ�r�M��10λ

bool FuncReg(bool *_FlagName,u32 _DelayTime,bool _FlagState)//׃��ָᘣ��ӕr�r�g���˜ʠ�B
{
	bool flag=FALSE;
	int i=0;
	for(i=0;i<VARMAX;i++)
	{
		if(FlagStateFunc[i].FlagName==_FlagName&&FlagStateFunc[i].Reg==0x01)
		{
			FlagStateFunc[i].DelayTime=_DelayTime;
			FlagStateFunc[i].FlagState=_FlagState;
			return flag=TRUE;
		}
	}
	for(i = 0;i < VARMAX;i++)
	{
		if(FlagStateFunc[i].Reg!=0x01)
		{
			FlagStateFunc[i].Reg=0x01;
			FlagStateFunc[i].DelayTime=_DelayTime;
			FlagStateFunc[i].FlagState=_FlagState;
			FlagStateFunc[i].FlagName=_FlagName;
			flag=TRUE;
			break;
		}
	}
	return flag;
}


void FuncTimeTravers(void)//��ʱ���ص�����
{
	int i = 0;
	for(i = 0; i < VARMAX; i++)
	{
		if(FlagStateFunc[i].Reg==0x01)
		{
			FlagStateFunc[i].DelayTime--;
			if(FlagStateFunc[i].DelayTime==0)
			{
				FlagStateFunc[i].Reg=0x00;
				FlagStateFunc[i].DelayTime=0;
				*FlagStateFunc[i].FlagName=FlagStateFunc[i].FlagState;
			}
		}
	}
}

u8 * IntTostring(u32 _data)//��int����ת����string
{
	u8 *ptemp=NULL;
	u8 length = 0,length_temp;
	u8 i=0;
	length = GetIntegerlength(_data);
	ptemp=(u8 *)malloc(sizeof(u8)*length + 2);
	length_temp = length - 1;
	for(i = 0;i < length; i++ , length_temp--)
	{
		*(ptemp + i) =(u8)(_data / pow(10,length_temp)+'0');
		_data %=(u32) pow(10,length_temp);
	}
	*(ptemp + i + 1)='\0';	
	return ptemp;
}

u8 GetIntegerlength(u32 _data)//��ȡint�������ݵĳ���
{
	u8 temp = 0;
	u8 i = 0;
	for(i = 0;;i ++)
	{
		_data/=10;
		if(_data==0)
			break;
	}
	temp=i + 1;
	return temp;
}


void FuncDataGet(u8 *data)//������ȡֵ
{
	u8 i=0,test=0;
	u8 num = *data;
	u8 *temp=data+1;
	while(test!=num)
	{
		for(i=0;i<7;)
		{
			switch(i)
			{
				case 0:if(*(temp+i+test*7) == 0X55) i++;else i=7;break;
				case 1:if(*(temp+i+test*7) == 0XAA) i++;else i=7;break;
				case 2:
				case 3:
				case 4:
				case 5:i++;break;
				case 6:
					if(*(temp+i+test*7) == 0XFF)
					{
						i++;
						iswork(pHard,temp+test*7);
						
					}
					else i=7;
					break;
				default :i=7;break;	
			}
		}
		test++;
	}
}


void isSend(pDATALIST pHard)//�жϷ��������Ƿ����
{
	pDATALIST p = pHard->pNext;
	while (p != NULL)
	{
		if (p->Reg == TRUE)
		{
			p->Reg = FALSE;
			//SendList(p->Addr,IntTostring(*(p->pArray+3)),IntTostring(*(p->pArray+4)),IntTostring(*(p->pArray+3) - *(p->pArray+4)));					
			sendseat(p);
		}
		else
		{
			SendListTV1(p);		
			DeleteList(pHard, p->Addr, &p);
		}
		p = p->pNext;
	}
}

void SendListTV1(pDATALIST  p)//�l���б픵��
{
	int i = 0;
	u8 addr = *(p -> pArray + 2); 
	while(i < 4)
	{
		
		Send_China_Data(0x0100 + 2 * i + addr * 8, "��");
		delay_ms(5);
		i++;
	}
}

void sendseat(pDATALIST  p)
{
	int i = 0;
	u8 temp = *(p -> pArray + 4);
	u8 addr = *(p -> pArray + 2); 
	while(i < 4)
	{
		if((temp & 0x01) == 0x01)
		{
			Send_China_Data(0x0100 + 2 * i + addr * 8, "��");
		}
		else
		{
			Send_China_Data(0x0100 + 2 * i + addr * 8, "��");
		}
		delay_ms(5);
		i++;
		temp >>=  1;
	}
}	
	
void iswork(pDATALIST pHard, u8 * pArray)//�Ƿ���
{
	if (isAddrRepeat(pHard, pArray) == FALSE)
	{
		AddList(pHard, pArray);
	}
}

bool isAddrRepeat(pDATALIST pHard, u8 * pArray)//�ж��Ƿ��ַ�ѱ��棬�ѱ��潫���ݸ���ԭʼ����
{
	pDATALIST p = pHard->pNext;
	while (NULL != p)
	{
		if (p->Addr == *(pArray + 2))break;
		p = p->pNext;
	}
	if (NULL == p) return FALSE;
	p->pArray = pArray;
	p->Reg = TRUE;
	return TRUE;
}

pDATALIST CreateListHard(void)//��������ͷ
{
	pDATALIST pHard = NULL;
	pHard = (pDATALIST)malloc(sizeof(DATSLIST));
	pHard->pNext = NULL;
	return pHard;
}

bool AddList(pDATALIST pHard, u8 * pArray)//�������������
{
	pDATALIST p = pHard;
	while (p->pNext!=NULL)
	{
		p = p->pNext;
	}
	p->pNext = (pDATALIST)malloc(sizeof(DATSLIST));
	if (p->pNext == NULL)
	{
		return FALSE;
	}
	p->pNext->pArray = pArray;
	p->pNext->Addr = *(pArray + 2);
	p->pNext->Reg = TRUE;
	p->pNext->pNext = NULL;
	return TRUE;
}

bool DeleteList(pDATALIST pHard, u8 addr, pDATALIST * pLast)//���ĳ����ַ������
{
	pDATALIST p = pHard->pNext, p1 = NULL;
	p1 = pHard;
	while (p != NULL && p->Addr != addr)
	{
		p1 = p;
		p = p->pNext;
	}
	if (p == NULL) return FALSE;
	*pLast = p1;
	p1->pNext = p1->pNext->pNext;
	free(p);
	p = NULL;
	return TRUE;
}

void PrintfList(pDATALIST pHard)//��ӡ��������
{
	int  i=0;
	pDATALIST p = pHard->pNext;
	while (p != NULL)
	{
		for ( i = 0; i < 7; i++)
		{
			printf("%#X\t",*(p->pArray+i));
		}
		printf("\n");
		p = p->pNext;
	}
}





