#ifndef MAX30102_H_
#define MAX30102_H_

#include "sys.h"
#include "stdbool.h"

//IO方向设置
#define max30102_SDA_IN()  {GPIOB->MODER&=~(3<<(8*2));GPIOB->MODER|=0<<8*2;}	//PB8输入模式
#define max30102_SDA_OUT() {GPIOB->MODER&=~(3<<(8*2));GPIOB->MODER|=1<<8*2;GPIOB->OTYPER |= 1<<8;} //PB8输出模式

//IO方向设置
#define max30102_SCL_IN()  {GPIOB->MODER&=~(3<<(7*2));GPIOB->MODER|=0<<7*2;}	//PB9输入模式
#define max30102_SCL_OUT() {GPIOB->MODER&=~(3<<(7*2));GPIOB->MODER|=1<<7*2;GPIOB->OTYPER |= 1<<7;} //PB9输出模式
//IO操作函数	 
#define max30102_IIC_SCL    PBout(7) //SCL
#define max30102_IIC_SDA    PBout(8) //SDA	 
#define max30102_READ_SDA   PBin(8)  //输入SDA 


#define max30102_I2C_SCL_1()  PBout(7) = 1				/* SCL = 1 */
#define max30102_I2C_SCL_0()  PBout(7) = 0				/* SCL = 0 */

#define max30102_I2C_SDA_1()  PBout(8) = 1				/* SDA = 1 */
#define max30102_I2C_SDA_0()  PBout(8) = 0				/* SDA = 0 */

#define max30102_I2C_SDA_READ()  PBin(8)	/* 读SDA口线状态 */
#define max30102_I2C_SCL_READ()  PBin(7)	/* 读SCL口线状态 */

#define INT  PBin(9)//读取按键0

#define I2C_WRITE_ADDR 0xAE
#define I2C_READ_ADDR 0xAF

//register addresses
#define REG_INTR_STATUS_1 0x00
#define REG_INTR_STATUS_2 0x01
#define REG_INTR_ENABLE_1 0x02
#define REG_INTR_ENABLE_2 0x03
#define REG_FIFO_WR_PTR 0x04
#define REG_OVF_COUNTER 0x05
#define REG_FIFO_RD_PTR 0x06
#define REG_FIFO_DATA 0x07
#define REG_FIFO_CONFIG 0x08
#define REG_MODE_CONFIG 0x09
#define REG_SPO2_CONFIG 0x0A
#define REG_LED1_PA 0x0C
#define REG_LED2_PA 0x0D
#define REG_PILOT_PA 0x10
#define REG_MULTI_LED_CTRL1 0x11
#define REG_MULTI_LED_CTRL2 0x12
#define REG_TEMP_INTR 0x1F
#define REG_TEMP_FRAC 0x20
#define REG_TEMP_CONFIG 0x21
#define REG_PROX_INT_THRESH 0x30
#define REG_REV_ID 0xFE
#define REG_PART_ID 0xFF

bool maxim_max30102_init(void);
bool maxim_max30102_read_fifo(uint32_t *pun_red_led, uint32_t *pun_ir_led);
bool maxim_max30102_write_reg(uint8_t uch_addr, uint8_t uch_data);
bool maxim_max30102_read_reg(uint8_t uch_addr, uint8_t *puch_data);
bool maxim_max30102_reset(void);

void bsp_InitI2C(void);
void max30102_i2c_Start(void);
void max30102_i2c_Stop(void);
void max30102_i2c_SendByte(uint8_t _ucByte);
uint8_t max30102_i2c_ReadByte(void);
uint8_t max30102_i2c_WaitAck(void);
void max30102_i2c_Ack(void);
void max30102_i2c_NAck(void);
uint8_t max30102_i2c_CheckDevice(uint8_t _Address);

#endif
















