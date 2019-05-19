/**
 * MAX6921驱动
 * 芯片简介：
 * 
 * 作者：    Blanboom
 * 最后更新：2014-07-17
 *****************************************************************************
 */
 
#include "HV57708.h"
#include "PinDef.h"
#include "delay.h"

/**
 * void MAX6921_Init(bool MAX6921_Mode)
 * 简介：MAX6921 初始化
 * 功能：MAX6921 初始化，并设置输出模式
 * 输入：
 *       MAX6921_Mode:
 *           MAX6921_MODE_NORMAL    - 标准输出模式
 *           MAX6921_MODE_INVERTION - 反相输出模式
 * 输出：无
 */


/**
 * void HV57708_SendData(uint32_t datapart1, uint32_t datapart2)
 * 简介：向 HV57708 发送数据
 * 功能：向 HV57708 发送 64 位的数据
 * 输入：
 *       datapart1 - 第一部分数据，32位
 *       datapart2 - 第二部分数据，32位
 * 输出：无
 */


/**
 * void HV57708_OutputData(void)
 * 简介：将 HV57708 寄存器中的数据输出至引脚
 * 功能：将 HV57708 寄存器中的数据输出至引脚
 * 输入：无
 * 输出：无
 */
void HV57708_OutputData(void)
{

}