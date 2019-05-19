版本号：防碰头声光报警装置—STC8子板

更新日期：2018-08-30

更新内容：增加EEPROM子程序
代测试：
EEPROM读写时间测试

void IapIdle();//关闭IAP功能
u8 IapRead(u16 addr);//读取EEPROM内地址数据
void IapProgram(u16 addr, u8 dat);//写入数据到EEPROM地址
void IapErase(u16 addr);//删除扇区（512字节/扇区）


更新日期：2018-08-31

更新内容：增加EEPROM子程序
代测试：
MY1680U读写时间测试

u8 XorCheck(u8 *_able,u8 length);//XOR校验
//起始码 长度 操作码 校验码 结束码
myenum My1680uControl(u8 _cmd,
					u8 _cmdlength,
					u16 _data);//My1680u数据发送函数
					
typedef void(*pFuncSystickCallBack)(u8 *_data,u8 _length);//设置回调函数类型
//设置my1680u.h内部自用回调函数
void MYU1680U_SetCallBack(u32 ulTimeout,
                          pFuncSystickCallBack callBack,
                          u8 *_para,
						  u8 _length);

 void HAL_MYU1680U_Callback(void);//中断回调函数

更新日期：2018-08-33

更新内容：增加EEPROM子程序
代测试：
EEPROM的测试函数
myenum IfAddrData(u16 _addr,u8 _comparisonvalue);//读取_addr的数据与_comparisonvalue比较(正确输出Enable/错误输出Disable)
myenum CollectiveWritData(u16 _RestAdrr,u8 *_able,u8 _length);//集体写入函数(默认第一扇区)
myenum CollectiveReadData(u16 _RestAdrr,u8 *_able,u8 _length);//集体读出函数(默认第一扇区)

