#ifndef __GT911_H
#define __GT911_H	
#include "sys.h"	
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//4.3����ݴ�����-GT911 ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2015/12/28
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved		 
////////////////////////////////////////////////////////////////////////////////// 


//IO��������	 
#define GT_RST(n)  (n?HAL_GPIO_WritePin(GPIOI,GPIO_PIN_8,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOI,GPIO_PIN_8,GPIO_PIN_RESET))//GT911��λ����
#define GT_INT      HAL_GPIO_ReadPin(GPIOH,GPIO_PIN_7)  //GT911�ж�����		
 
//I2C��д����	
#define GT_CMD_WR 		0X28     	//д����
#define GT_CMD_RD 		0X29		//������
  
//GT911 ���ּĴ������� 
#define GT_CTRL_REG 	0X8040   	//GT911���ƼĴ���
#define GT_CFGS_REG 	0X8047   	//GT911������ʼ��ַ�Ĵ���
#define GT_CHECK_REG 	0X80FF   	//GT911У��ͼĴ���
#define GT_PID_REG 		0X8140   	//GT911��ƷID�Ĵ���

#define GT_GSTID_REG 	0X814E   	//GT911��ǰ��⵽�Ĵ������
#define GT_TP1_REG 		0X8150  	//��һ�����������ݵ�ַ
#define GT_TP2_REG 		0X8158		//�ڶ������������ݵ�ַ
#define GT_TP3_REG 		0X8160		//���������������ݵ�ַ
#define GT_TP4_REG 		0X8168		//���ĸ����������ݵ�ַ
#define GT_TP5_REG 		0X8170		//��������������ݵ�ַ  
 
 
u8 GT911_Send_Cfg(u8 mode);
u8 GT911_WR_Reg(u16 reg,u8 *buf,u8 len);
void GT911_RD_Reg(u16 reg,u8 *buf,u8 len); 
u8 GT911_Init(void);
u8 GT911_Scan(u8 mode); 
#endif













