#include "led.h"
#include "string.h"

/* -------------------------------- begin  -------------------------------- */
/**
  * @Name    LED_Init
  * @brief  
  * @param   None
  * @retval
  * @author  ZERO 
  * @Data    2019-04-10
  * 1. ...
  * <modify staff>:
  * <data>        :
  * <description> :
  * 2. ...
 **/
/* -------------------------------- end -------------------------------- */

uint8_t cmd_able[4] = {0};

void Peripheral_Handler(void)
{
	if(cmd_able[3])
	{
		cmd_able[3] = 0;
		if(cmd_able[2] == 1)
		{
			switch(cmd_able[0])
			{
				case 1:
				switch(cmd_able[1])
				{
					case 1:P0 &= ~0X01; break;
					case 2:P0 &= ~0X02; break;
					case 3:P0 &= ~0X04; break;
					case 4:P0 &= ~0X08; break;
					case 5:P0 &= ~0X10; break;
					case 6:P0 &= ~0X20; break;
					case 7:P0 &= ~0X40; break;
					case 8:P0 &= ~0X80; break;
					case 0xff:P0 &= ~0Xff; break;
					default :break;
				}					
				break;
				case 2:P2 &= ~0x01; break;
				case 3:
				switch(cmd_able[1])
				{
					case 1:P2 &= ~0X10; break;
					case 2:P2 &= ~0X20; break;
					case 3:P2 &= ~0X40; break;
					case 4:P2 &= ~0X80; break;
					case 0xff:P2 &= ~0Xf0; break;
					default :break;
				}			break;
				default :break;
			}
		}
		else if(cmd_able[2] == 2)
		{
			switch(cmd_able[0])
			{
				case 1:
				switch(cmd_able[1])
				{
					case 1:P0 |= 0X01; break;
					case 2:P0 |= 0X02; break;
					case 3:P0 |= 0X04; break;
					case 4:P0 |= 0X08; break;
					case 5:P0 |= 0X10; break;
					case 6:P0 |= 0X20; break;
					case 7:P0 |= 0X40; break;
					case 8:P0 |= 0X80; break;
					case 0xff:P0 |= 0Xff; break;
					default :break;	
				}					
				break;
				case 2:P2 |= 0x01; break;
				case 3:
				switch(cmd_able[1])
				{
					case 1:P2 |= 0X10; break;
					case 2:P2 |= 0X20; break;
					case 3:P2 |= 0X40; break;
					case 4:P2 |= 0X80; break;
					case 0xff:P2 |= 0Xf0; break;
					default :break;
				}			break;
				default :break;
			}
		}
	}	
}
