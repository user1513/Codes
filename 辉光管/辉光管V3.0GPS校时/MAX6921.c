#include <MAX6921.h>
#include <intrins.h>

int xdata data_weima[9]={3,14,4,15,17,16,19,18,5};//1（0）-9（8）位

//num从1到9，表示要显示的段位 9是最高位小数点（最左边为最高位）
void display_num(int num,int seg)
{
		int data_sequ[20] = {0};//依次存储D19至DD0
		
		int i;
		MAX6921_BLANK = 1;//关闭显示
		MAX6921_LOAD = 0;//锁存
		data_sequ[ data_weima[num - 1] ] = 1;
		switch(seg)//仅供单独显示用
		{
				case(0):{data_sequ[6] = 1;data_sequ[7] = 1;data_sequ[8] = 1;data_sequ[10] = 1;data_sequ[11] = 1;data_sequ[12] = 1;}
				break;
				case(1):{data_sequ[8] = 1;data_sequ[11] = 1;}
				break;
				case(2):{data_sequ[6] = 1;data_sequ[8] = 1;data_sequ[9] = 1;data_sequ[10] = 1;data_sequ[12] = 1;}
				break;
				case(3):{data_sequ[6] = 1;data_sequ[8] = 1;data_sequ[9] = 1;data_sequ[11] = 1;data_sequ[12] = 1;}
				break;
				case(4):{data_sequ[7] = 1;data_sequ[8] = 1;data_sequ[9] = 1;data_sequ[11] = 1;}
				break;
				case(5):{data_sequ[6] = 1;data_sequ[7] = 1;data_sequ[9] = 1;data_sequ[11] = 1;data_sequ[12] = 1;}
				break;
				case(6):{data_sequ[6] = 1;data_sequ[7] = 1;data_sequ[9] = 1;data_sequ[10] = 1;data_sequ[11] = 1;data_sequ[12] = 1;}
				break;
				case(7):{data_sequ[6] = 1;data_sequ[8] = 1;data_sequ[11] = 1;}
				break;
				case(8):{data_sequ[6] = 1;data_sequ[7] = 1;data_sequ[8] = 1;data_sequ[9] = 1;data_sequ[10] = 1;data_sequ[11] = 1;data_sequ[12] = 1;}
				break;
				case(9):{data_sequ[6] = 1;data_sequ[7] = 1;data_sequ[8] = 1;data_sequ[9] = 1;data_sequ[11] = 1;data_sequ[12] = 1;}
				break;//A
				case(10):{data_sequ[6] = 1;data_sequ[7] = 1;data_sequ[8] = 1;data_sequ[9] = 1;data_sequ[10] = 1;data_sequ[11] = 1;}
				break;//C
				case(11):{data_sequ[6] = 1;data_sequ[7] = 1;data_sequ[9] = 1;data_sequ[12] = 1;}
				break;//D/0
				case(12):{data_sequ[6] = 1;data_sequ[7] = 1;data_sequ[8] = 1;data_sequ[10] = 1;data_sequ[11] = 1;data_sequ[12] = 1;}
				break;//E
				case(13):{data_sequ[6] = 1;data_sequ[7] = 1;data_sequ[9] = 1;data_sequ[10] = 1;data_sequ[12] = 1;}
				break;//F
				case(14):{data_sequ[6] = 1;data_sequ[7] = 1;data_sequ[9] = 1;data_sequ[10] = 1;}
				break;//H
				case(15):{data_sequ[7] = 1;data_sequ[8] = 1;data_sequ[9] = 1;data_sequ[10] = 1;data_sequ[11] = 1;}
				break;//L
				case(16):{data_sequ[7] = 1;data_sequ[10] = 1;data_sequ[12] = 1;}
				break;//P
				case(17):{data_sequ[6] = 1;data_sequ[7] = 1;data_sequ[8] = 1;data_sequ[9] = 1;data_sequ[10] = 1;}
				break;//Q
				case(18):{data_sequ[6] = 1;data_sequ[7] = 1;data_sequ[8] = 1;data_sequ[9] = 1;data_sequ[11] = 1;}
				break;//S
				case(19):{data_sequ[6] = 1;data_sequ[7] = 1;data_sequ[9] = 1;data_sequ[11] = 1;data_sequ[12] = 1;}
				break;//U/V
				case(20):{data_sequ[7] = 1;data_sequ[8] = 1;data_sequ[10] = 1;data_sequ[11] = 1;data_sequ[12] = 1;}
				break;//.
				case(21):{data_sequ[13] = 1;}
				break;//S
				case(22):{data_sequ[9] = 1;}
		}
		for(i = 0;i < 20;i++)
    {
				MAX6921_CLK = 0;
				_nop_();
				MAX6921_DIN = data_sequ[i];
				_nop_();
				MAX6921_CLK = 1;
				_nop_();
    }
		MAX6921_LOAD = 1;//载入并输出
		MAX6921_BLANK = 0;//开启所有显示
		_nop_();
}
