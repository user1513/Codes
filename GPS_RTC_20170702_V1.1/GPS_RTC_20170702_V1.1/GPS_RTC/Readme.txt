更新日志
V1.1
1.更新在搜索在线设备，操作其他功能，导致搜索在线设备一直处于搜索中，
  之后不能再使用，解决方案是搜索过程中，关闭所有按键的使能。

2.主控界面更名：RTC_Control     VersionNO：1.1     VersionTime：2017.7.2     Author：Tang娃      南京工业职业技术学院
3.地址为不固定为0x00了，更改所有发送的地址哦，更加贴心哦
            if (cboxNO.Text == "ALL")//地址位
            {
                DATE[1] = 0x00;
            }
            else
            {
                DATE[1] = Convert.ToByte(cboxNO.Text);
            }
4.添加修改音量功能

5.通讯出现冲突

6.可能不能同时读取所有闹钟，还是老方法，加延迟哦……（待解决）

调试步骤：
通过串口或者本上位机设置闹钟，然后分别读取，在通过串口指令，去读取看是否会返回两个时钟的闹钟数据，，

串口数据见“GPS_RTC 通讯协议.xlsx”第二十七行，会返回数据哦，波特率9600