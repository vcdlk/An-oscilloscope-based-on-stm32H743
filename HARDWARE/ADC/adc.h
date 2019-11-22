#ifndef __ADC_H
#define __ADC_H
#include "sys.h"
#include "MainTask.h"
#include "led.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32H7开发板
//ADC驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2017/8/13
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

void MY_ADC_Init(void); 						//ADC通道初始化
void ADC1_Config(void);							//初始化ADC1
void ADC2_Config(void);							//初始化ADC2
void ADC3_Config(void);							//初始化ADC3
static void TIM_Config(void);							//定时器3用来做触发信号				
void ADC_DMA_Close(void);						//关闭ADC1，ADC2，ADC3和对应的DMA
void ADC_DMA_Open(void);						//打开ADC1，ADC2，ADC3和对应的DMA
void TIM2_MeasureTrigConfig(void);	//初始化TIM8来记录一段波形
static void Time2Recorder(void);	
void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc);
void ADC_AnalogWatchdogThresholdsChange(ADC_HandleTypeDef* hadc, ADC_AnalogWDGConfTypeDef* AnalogWDGConfig,
																				uint16_t HighThreshold,uint16_t LowThreshold);
						
extern uint16_t ADC1ConvertedValue[10240];
extern uint16_t ADC3ConvertedValue[10240];
#endif 
