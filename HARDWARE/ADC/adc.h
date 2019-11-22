#ifndef __ADC_H
#define __ADC_H
#include "sys.h"
#include "MainTask.h"
#include "led.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32H7������
//ADC��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2017/8/13
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

void MY_ADC_Init(void); 						//ADCͨ����ʼ��
void ADC1_Config(void);							//��ʼ��ADC1
void ADC2_Config(void);							//��ʼ��ADC2
void ADC3_Config(void);							//��ʼ��ADC3
static void TIM_Config(void);							//��ʱ��3�����������ź�				
void ADC_DMA_Close(void);						//�ر�ADC1��ADC2��ADC3�Ͷ�Ӧ��DMA
void ADC_DMA_Open(void);						//��ADC1��ADC2��ADC3�Ͷ�Ӧ��DMA
void TIM2_MeasureTrigConfig(void);	//��ʼ��TIM8����¼һ�β���
static void Time2Recorder(void);	
void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc);
void ADC_AnalogWatchdogThresholdsChange(ADC_HandleTypeDef* hadc, ADC_AnalogWDGConfTypeDef* AnalogWDGConfig,
																				uint16_t HighThreshold,uint16_t LowThreshold);
						
extern uint16_t ADC1ConvertedValue[10240];
extern uint16_t ADC3ConvertedValue[10240];
#endif 
