�汾�ţ�����ͷ���ⱨ��װ�á�STC8�Ӱ�

�������ڣ�2018-08-30

�������ݣ�����EEPROM�ӳ���
�����ԣ�
EEPROM��дʱ�����

void IapIdle();//�ر�IAP����
u8 IapRead(u16 addr);//��ȡEEPROM�ڵ�ַ����
void IapProgram(u16 addr, u8 dat);//д�����ݵ�EEPROM��ַ
void IapErase(u16 addr);//ɾ��������512�ֽ�/������


�������ڣ�2018-08-31

�������ݣ�����EEPROM�ӳ���
�����ԣ�
MY1680U��дʱ�����

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

�������ڣ�2018-08-33

�������ݣ�����EEPROM�ӳ���
�����ԣ�
EEPROM�Ĳ��Ժ���
myenum IfAddrData(u16 _addr,u8 _comparisonvalue);//��ȡ_addr��������_comparisonvalue�Ƚ�(��ȷ���Enable/�������Disable)
myenum CollectiveWritData(u16 _RestAdrr,u8 *_able,u8 _length);//����д�뺯��(Ĭ�ϵ�һ����)
myenum CollectiveReadData(u16 _RestAdrr,u8 *_able,u8 _length);//�����������(Ĭ�ϵ�һ����)

