#ifndef _MY1680U_H
#define _MY1680U_H

#include "pbdata.h"


///////////////////////////////////////length->3
#define PLAYMODE 0X11           //����
#define SUSPENDMODE 0X12	    //��ͣ
#define THENEXTMODE 0X13        //��һ��
#define THELASTMODE 0X14        //��һ��
#define VOLUMEADDMODE 0X15      //������
#define VolMODE 0x16            //������
#define RESTMODE 0X19           //��λ
#define FASTFORWARDMODE 0X1A	//���
#define FASTBACKMODE  0X1B 		//����
#define PLAYORSUSPENDMODE 0X1C 	//����or��ͣ
#define STOPMODE 0X1E  		    //ֹͣ


///////////////////////////////////////length->4
#define VOLUMESETMODE 0X31  //��������
#define EQSETMODE 0X32      //����EQ
///////////////////////////////////////length->5
#define CHOOSEPLAYSONGS 0X41//ѡ�񲥷���Ŀ

///////////////////////////////////////
#define QUERYSTATEMODE  0X20  //��ѯ����ģʽ
#define QUERYVOLUMEMODE  0X21 //��ѯ������С
#define QUERYEQMODE  0X22     //��ѯ��ǰEQ
#define QUERYPLAYSONGS 0X2E   //��ѯ������Ŀ

///////////////////////////////////////
#define EQNOMODE 0X00      //EQ NO ģʽ
#define EQPOPMODE 0X01	   //EQ POP ģʽ
#define EQROCKMODE 0X02    //EQ ROCK ģʽ
#define EQJAZZMODE 0X03    //EQ JAZZ ģʽ
#define EQCLASSICMODE 0X04 //EQ CLASSIC ģʽ
#define EQBASSMODE 0X05    //EQ BASS ģʽ

//////////////////////////////////////
#define DATALENGTH3 0X03   //�������ݳ���3
#define DATALENGTH4 0X04   //�������ݳ���4
#define DATALENGTH5 0X05   //�������ݳ���5
#define DATANONE 0X00      //��������Ϊ��

/////////////////////////////////////
u8 XorCheck(u8 *_able,u8 length);//XORУ��
//��ʼ�� ���� ������ У���� ������
myenum My1680uControl(u8 _cmd,
					u8 _cmdlength,
					u16 _data);//My1680u���ݷ��ͺ���


typedef void(*pFuncSystickCallBack)(u8 *_data,u8 _length);//���ûص���������
//����my1680u.h�ڲ����ûص�����
void MYU1680U_SetCallBack(u32 ulTimeout,
                          pFuncSystickCallBack callBack,
                          u8 *_para,
						  u8 _length);

 void HAL_MYU1680U_Callback(void);//�жϻص�����

void musicplaymode(u8 num);

extern myenum My1680uFlag;

#endif

