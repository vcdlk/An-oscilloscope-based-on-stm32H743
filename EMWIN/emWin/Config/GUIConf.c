#include "GUI.h"
#include "usart.h"
#include "sdram.h"
#include "malloc.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F746������
//STemWin�ڴ�������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2015/7/6
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 

#define USE_EXRAM  1	//ʹ���ⲿRAM
//����EMWIN�ڴ��С
#define GUI_NUMBYTES  (8*1024*1024)
#define GUI_BLOCKSIZE 0X80  //���С

//GUI_X_Config
//��ʼ����ʱ�����,��������emwin��ʹ�õ��ڴ�
void GUI_X_Config(void) {
	if(USE_EXRAM) //ʹ���ⲿRAM
	{	
		U32 *aMemory = mymalloc(SRAMEX,GUI_NUMBYTES); //���ⲿSRAM�з���GUI_NUMBYTES�ֽڵ��ڴ�
        GUI_ALLOC_AssignMemory((void*)aMemory, GUI_NUMBYTES); //Ϊ�洢����ϵͳ����һ���洢��
		GUI_ALLOC_SetAvBlockSize(GUI_BLOCKSIZE); //���ô洢���ƽ���ߴ�,����Խ��,���õĴ洢������Խ��
		GUI_SetDefaultFont(GUI_FONT_6X8); //����Ĭ������
	}else  //ʹ���ڲ�RAM
	{
		U32 *aMemory = mymalloc(SRAMIN,GUI_NUMBYTES); //���ڲ�RAM�з���GUI_NUMBYTES�ֽڵ��ڴ�
		GUI_ALLOC_AssignMemory((U32 *)aMemory, GUI_NUMBYTES); //Ϊ�洢����ϵͳ����һ���洢��
		GUI_ALLOC_SetAvBlockSize(GUI_BLOCKSIZE); //���ô洢���ƽ���ߴ�,����Խ��,���õĴ洢������Խ��
		GUI_SetDefaultFont(GUI_FONT_6X8); //����Ĭ������
	}
}

