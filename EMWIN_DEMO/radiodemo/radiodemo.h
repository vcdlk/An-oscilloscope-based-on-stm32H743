#ifndef _GRAHPYT_H
#define _GRAHPYT_H
#include "sys.h"
#include "WM.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//STemwin RADIO�ؼ�ʹ��   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2016/3/28
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

typedef struct
{
    WM_HWIN rdadio; //RADIO���ھ��
    u8      value;  //RADIO��ǰ��ѡ�е�ITEM
}radio_struct;

void CreateRadioDemo(void);
#endif
