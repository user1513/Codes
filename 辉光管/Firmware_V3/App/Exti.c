
/*------------------------------------------------------------------*/
/* --- STC MCU International Limited -------------------------------*/
/* --- STC 1T Series MCU RC Demo -----------------------------------*/
/* --- Mobile: (86)13922805190 -------------------------------------*/
/* --- Fax: 86-0513-55012956,55012947,55012969 ---------------------*/
/* --- Tel: 86-0513-55012928,55012929,55012966 ---------------------*/
/* --- Web: www.GXWMCU.com -----------------------------------------*/
/* --- QQ:  800003751 ----------------------------------------------*/
/* If you want to use the program or the program referenced in the  */
/* article, please specify in which data and procedures from STC    */
/*------------------------------------------------------------------*/


/*************	����˵��	**************

���ļ�ΪSTC15xxxϵ�е����жϳ�ʼ�����жϳ���,�û�����������ļ����޸��Լ���Ҫ���жϳ���.

******************************************/



#include "Exti.h"
#include "app.h"
#include "Display.h"
#include "PinDef.h"


void	EXTI_config(void)
{
	EXTI_InitTypeDef	EXTI_InitStructure;					//�ṹ����

	EXTI_InitStructure.EXTI_Interrupt = ENABLE;				//�ж�����,     ENABLE��DISABLE
	Ext_Inilize(EXT_INT1,&EXTI_InitStructure);				//��ʼ��INT1
	
	EXTI_InitStructure.EXTI_Interrupt = ENABLE;				//�ж�����,     ENABLE��DISABLE
	Ext_Inilize(EXT_INT2,&EXTI_InitStructure);				//��ʼ��INT2

	EXTI_InitStructure.EXTI_Interrupt = ENABLE;				//�ж�����,     ENABLE��DISABLE
	Ext_Inilize(EXT_INT3,&EXTI_InitStructure);				//��ʼ��INT3


}


//========================================================================
// ����: u8	Ext_Inilize(u8 EXT, EXTI_InitTypeDef *INTx)
// ����: �ⲿ�жϳ�ʼ������.
// ����: INTx: �ṹ����,��ο�Exti.h��Ķ���.
// ����: �ɹ�����0, �ղ�������1,���󷵻�2.
// �汾: V1.0, 2012-10-22
//========================================================================
u8	Ext_Inilize(u8 EXT, EXTI_InitTypeDef *INTx)
{
	if(EXT >  EXT_INT4)	return 1;	//�ղ���
	
	if(EXT == EXT_INT0)	//���ж�0
	{
		if(INTx->EXTI_Interrupt == ENABLE)		EX0 = 1;	//�����ж�
		else									EX0 = 0;	//��ֹ�ж�
		if(INTx->EXTI_Polity == PolityHigh)		PX0 = 1;	//�����ȼ��ж�
		else									PX0 = 0;	//�����ȼ��ж�
		if(INTx->EXTI_Mode == EXT_MODE_Fall)	IT0 = 1;	//�½����ж�
		else									IT0 = 0;	//����,�½����ж�
		return	0;		//�ɹ�
	}

	if(EXT == EXT_INT1)	//���ж�1
	{
		if(INTx->EXTI_Interrupt == ENABLE)		EX1 = 1;	//�����ж�
		else									EX1 = 0;	//��ֹ�ж�
		if(INTx->EXTI_Polity == PolityHigh)		PX1 = 1;	//�����ȼ��ж�
		else									PX1 = 0;	//�����ȼ��ж�
		if(INTx->EXTI_Mode == EXT_MODE_Fall)	IT1 = 1;	//�½����ж�
		else									IT1 = 0;	//����,�½����ж�
		return	0;		//�ɹ�
	}

	if(EXT == EXT_INT2)		//���ж�2, �̶�Ϊ�½��ص����ȼ��ж�
	{
		if(INTx->EXTI_Interrupt == ENABLE)	INT_CLKO |=  (1 << 4);	//�����ж�	
		else								INT_CLKO &= ~(1 << 4);	//��ֹ�ж�
		return	0;		//�ɹ�
	}

	if(EXT == EXT_INT3)		//���ж�3, �̶�Ϊ�½��ص����ȼ��ж�
	{
		if(INTx->EXTI_Interrupt == ENABLE)	INT_CLKO |=  (1 << 5);	//�����ж�	
		else								INT_CLKO &= ~(1 << 5);	//��ֹ�ж�
		return	0;		//�ɹ�
	}

	if(EXT == EXT_INT4)		//���ж�4, �̶�Ϊ�½��ص����ȼ��ж�
	{
		if(INTx->EXTI_Interrupt == ENABLE)	INT_CLKO |=  (1 << 6);	//�����ж�	
		else								INT_CLKO &= ~(1 << 6);	//��ֹ�ж�
		return	0;		//�ɹ�
	}
	return 2;	//ʧ��
}

