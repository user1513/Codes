

当前版本：Ball_V1.4_2017082601
上一版本：Ball_V1.3_2017082602
修改者：Tang娃

由Ball_V1.3_2017082602中数据表中固定的十七个数据更改为支持任意数据的数据个数，
无需修改其他函数，只需修改数据表buff中测得的数据，及显示学习角度的数组jddisplay。

数据表中的数据个数TableNumberOfData是通过计算buff数组大小，在除以类型，得出数组存储的个数

EEPROM部分存储仅仅需要改动for的执行次数，程序中使用宏定义TableNumberOfData实现

显示部分进行整体修改，由原来的两层循环嵌套，改为三层循环嵌套，支持任意个数据的显示，目前最大
支持255（页）*4（行）*3（个）数据的显示哦



/*

	计算表中的数据个数

*/
//		asd = sizeof(buff) / 2;
//		datadisp[0] = asd/100+'0';
//		datadisp[1] = asd/10%10+'0';
//		datadisp[2] = asd%10%10+'0';
//		Display_String(j+1,datadisp); 
//		while(1);
