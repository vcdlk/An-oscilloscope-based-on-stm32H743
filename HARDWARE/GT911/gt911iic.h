#ifndef _MYIIC_H
#define _MYIIC_H
#include "sys.h"
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//
//IIC��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2015/11/30
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	
//IO��������
#define SDA_IN()  {GPIOI->MODER&=~(3<<(3*2));GPIOI->MODER|=0<<3*2;}	//PH5����ģʽ
#define SDA_OUT() {GPIOI->MODER&=~(3<<(3*2));GPIOI->MODER|=1<<3*2;} //PH5���ģʽ
//IO����
#define IIC_SCL(n)  (n?HAL_GPIO_WritePin(GPIOH,GPIO_PIN_6,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOH,GPIO_PIN_6,GPIO_PIN_RESET)) //SCL
#define IIC_SDA(n)  (n?HAL_GPIO_WritePin(GPIOI,GPIO_PIN_3,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOI,GPIO_PIN_3,GPIO_PIN_RESET)) //SDA
#define READ_SDA    HAL_GPIO_ReadPin(GPIOI,GPIO_PIN_3)  //����SDA

#define Touch_DelayVaule 8
//IIC���в�������
void GT911_IIC_Touch_Delay(u16 a);
void GT911_IIC_Init(void);                //��ʼ��IIC��IO��				 
void GT911_IIC_Start(void);				//����IIC��ʼ�ź�
void GT911_IIC_Stop(void);	  			//����IICֹͣ�ź�
void GT911_IIC_Send_Byte(u8 txd);			//IIC����һ���ֽ�
u8 GT911_IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
u8 GT911_IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void GT911_IIC_Ack(void);					//IIC����ACK�ź�
void GT911_IIC_NAck(void);				//IIC������ACK�ź�

void GT911_IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 GT911_IIC_Read_One_Byte(u8 daddr,u8 addr);	 
#endif

