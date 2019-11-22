/*
*********************************************************************************************************
*	                                  
*	模块名称 : 简单ADC值读取功能
*	文件名称 : DSO_WaveProcess.c
*	版    本 : V1.0
*	说    明 : 实现简单的ADC值读取。
*	修改记录 :
*		版本号    日期          作者           说明
*		V1.0    2018-01-06     Eric2013        首发 
*                                           
*	Copyright (C), 2018-2028, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/
#include "includes.h"
#include "MainTask.h"


extern ADC_AnalogWDGConfTypeDef   	AnalogWDGConfig_1;
/*
*********************************************************************************************************
*	                                  测量类型
*********************************************************************************************************
*/
float32_t *g_RMSBUF;


/*
*********************************************************************************************************
*	函 数 名: DSO1_WaveTrig
*	功能说明: 波形通道1的处理
*	形    参: usCurPos  DMA在ADC缓冲中传输的位置。
*	返 回 值: 无
*********************************************************************************************************
*/
void DSO1_WaveTrig(uint16_t usCurPos)
{
	uint16_t i,j,k;  
	int16_t temp;
	
	/*
		1. 自动触发时的波形显示
		2. 每次至少要保证采集2KB的数据，也就是单通道2Ksps（500us）时，程序中设置2Ksps及其以下采用
	       慢速方式显示。
	*/
	if(TriggerFlag == 0){
		/* 读取的是ADC3的位置 */
		//usCurPos = 10240 - DMA2_Stream1->NDTR;
		
		/* 前10种采样率采用直接刷新的方式进行显示****************************************/
		if(TimeBaseId < 10){
			/* ADC1，ADC3都有10k 16bit的数据缓存区。而我们每次采集时先确定当前DMA已经传输到的位置，
			   然后取这个位置之前的2K数据。
			*/
			/* 第一种情况：当前DMA传输位置在小于2k的地方 */
			if(usCurPos < 2048){
				//j = 2048 - usCurPos;
				//j = 10240 - j;
				j = 8192 + usCurPos;
				
				/* 获取2k数据的前部分  */
				for(i = j; i < 10240; i++){
					k = i - j;
					g_DSO1->usWaveBufTemp[k] = ADC3ConvertedValue[i];
				}
				
				j = 2048 - usCurPos;
				
				/* 获取2K数据的后部分 */
				for(i = 0; i < usCurPos; i++){
					g_DSO1->usWaveBufTemp[j+i] = ADC3ConvertedValue[i];
				}		
			}
			/* 第二种情况：当前DMA传输位置在大于等于2k的地方 */
			else{
				usCurPos = usCurPos - 2048;
				for(i = 0; i < 2048; i++)
				{
					g_DSO1->usWaveBufTemp[i] = ADC3ConvertedValue[i+usCurPos];
				}	
			}
			
			/* 通过软件检测实现上升沿触发，并保留最后600的数据不做检测，用于直接显示 
			   2048-600 = 1448;
			*/
			j = 0;
			for(i = 0; i < 1280; i++)
			{
				j++;
				if((g_DSO1->usWaveBufTemp[i] > g_TrigVol->usTrigValue) &&
				   (g_DSO1->usWaveBufTemp[i+1] < g_TrigVol->usTrigValue))
				{	
					break;
				}
			}
			
			/* 记录满足上升沿触发的位置 */
			g_DSO1->sCurTriPos = j;
			
			/* 变量g_usCurTriStep用来实现2K数据的移动，防止出现小于0 */
			if((g_DSO1->sCurTriPos + g_DSO1->sCurTriStep) < 0)
			{
				/* 将g_usCurTriStep取最接近g_usCurTriPos + g_usCurTriStep = 0 */
				//temp = g_usCurTriPos/100*100;
				temp = g_DSO1->sCurTriPos;
				g_DSO1->sCurTriStep = -temp;
			}
			
			/* 共采集2*1024 = 2048个数据，移动到最后至少还有剩600个数据，即2048-600=1448。
			   防止g_usCurTriPos + g_usCurTriStep的位置超过1448。
			*/
			if((g_DSO1->sCurTriPos + g_DSO1->sCurTriStep) > 1280)
			{
				/* 将g_usCurTriStep取最接近g_usCurTriPos + g_usCurTriStep < 1448的数值 */
				//temp = (1448 - g_usCurTriPos)/100*100;
				temp = (1280 - g_DSO1->sCurTriPos);
				g_DSO1->sCurTriStep = temp;
			}
			
			/* 从位置g_usCurTriPos+g_usCurTriStep开始的600个数据全部赋值给g_usWaveBuf */
			for(i = 0; i < DSOSCREEN_LENGTH; i++)
			{
				g_DSO1->usWaveBuf[i] = g_DSO1->usWaveBufTemp[i+g_DSO1->sCurTriPos+g_DSO1->sCurTriStep];
			}
		}
		/* 前10种采样率之后采用直接刷新的方式进行显示****************************************/
		else
		{
			/* g_ucSlowWaveFlag = 0 表示启动慢速显示模式，也就是滚动显示 */
			if(g_DSO1->ucSlowWaveFlag == 0)
			{
				/* g_DSO1->ulSlowRefresh0表示DMA当前传输到的位置 */
				//g_DSO1->ulSlowRefresh0 = 10240 - DMA2_Stream1->NDTR;
				g_DSO1->ulSlowRefresh1 = usCurPos;
				
				/* g_DSO1->ulSlowRefresh1表示按键调节频率后DMA传输到的位置 
				   通过g_DSO1->ulSlowRefresh1和g_DSO1->ulSlowRefresh0的差值确定刚开始显示
				   波形时600个数据的起始位置。(刚开始显示波形有一个从左向右的移动过程)
				*/
				if(g_DSO1->ulSlowRefresh1 >= g_DSO1->ulSlowRefresh0)
				{
					g_DSO1->ulSlowRefresh1 = g_DSO1->ulSlowRefresh1 - g_DSO1->ulSlowRefresh0;
				}
				else
				{
					g_DSO1->ulSlowRefresh1 = 10240 + g_DSO1->ulSlowRefresh1 - g_DSO1->ulSlowRefresh0;
				}
				
				/* 当差值大于600的时候，停止移动过程，开始整体刷新 */
				if(g_DSO1->ulSlowRefresh1 >= 768)
				{
					g_DSO1->ucSlowWaveFlag = 1;
				}
			}
			
			/* 第一种情况：当前DMA传输位置在小于600的地方 */
			if(usCurPos < 768)
			{
				//j = 600 - usCurPos;
				//j = 10240 - j;
				j = 9472 + usCurPos;
				
				/* 获取600个数据的前部分  */
				for(i = j; i < 10240; i++)
				{
					k = i - j;
					g_DSO1->usWaveBufTemp[k] = ADC3ConvertedValue[i];
				}
				
				j = 768 - usCurPos;
				
				/* 获取600个数据的后部分  */
				for(i = 0; i < usCurPos; i++)
				{
					g_DSO1->usWaveBufTemp[j+i] = ADC3ConvertedValue[i];
				}
			}
			/* 第二种情况：当前DMA传输位置在大于等于600的地方 */
			else
			{
				usCurPos = usCurPos - 768;
				for(i = 0; i < 768; i++)
				{
					g_DSO1->usWaveBufTemp[i] = ADC3ConvertedValue[i+usCurPos];
				}
			}

			/* 将获取的数据赋值给g_usWaveBuf */
			for(i = 0; i < DSOSCREEN_LENGTH; i++)
			{
				g_DSO1->usWaveBuf[i] = g_DSO1->usWaveBufTemp[767-i];
			}
		}
	}
	/* 选择 普通触发 模式后的波形显示 
	   普通触发模式一般情况下是不刷新显示的，只有被触发的情况在才做界面的刷新。
	*/
	else
	{
		/* 此标志用来表示允许或者暂停波形显示，1表示暂停，0表示运行 */
		g_Flag->hWinRunStop = 1;
		
		/* TriggerFlag = 2 表示达到触发条件后，已经采集完触发值前后的1K数据（单通道） */
		if(TriggerFlag == 2)
		{
			/* 允许波形显示 */
			g_Flag->hWinRunStop = 0;
			
			/* 采集的时候是采集了单通道前后各1024的数据，三通道总共就是1024*2 = 2048
			   2048/2 = 1024 第1024个就是触发值，下面是取了1024的前300个和后300个在屏
			   上进行显示，通过变量g_usCurTriStep配合按键可以实现波形的移动观察。
			*/
			for(i = 640; i < 1408; i++)
			{
				g_DSO1->usWaveBuf[i-640] = g_DSO1->usWaveBufTemp[i+g_DSO1->sCurTriStep] ;
			}
			
			/* TriggerFlag = 1 表示等待下次触发 */
			TriggerFlag = 1;
			
			/* 使能ADC3的看门狗 */
			HAL_NVIC_EnableIRQ(ADC3_IRQn);
//			ADC3->IER |= 0x80;
//			ADC_ITConfig(ADC3, ADC_IT_AWD, ENABLE);
//		AnalogWDGConfig_1.ITMode 										= ENABLE;
//		HAL_ADC_AnalogWDGConfig(&ADC3_Handler, &AnalogWDGConfig_1);
		}
	}
}

/*
*********************************************************************************************************
*	函 数 名: DSO1_WaveProcess
*	功能说明: 波形通道1的FFT，FIR，最大值，最小值，平均值，峰峰值，频率和RMS的计算
*	形    参: 无。
*	返 回 值: 无
*********************************************************************************************************
*/
void DSO1_WaveProcess(void)
{
	uint16_t  i; 
	uint32_t  uiCycle, uiCount = 0;

	g_DSO1->WaveMin = 4095;
	g_DSO1->WaveMax = 0.0f;
	g_DSO1->WaveMean = 0.0f;	
	
	/* 自动触发模式才计算FFT */
	if(TriggerFlag == 0)
	{
		/* 前10种采样率计算FFT */
		if(TimeBaseId < 10)
		{
			/* 执行2048点浮点FFT运算 */
			for(i=0; i<2048; i++)
			{
				testInput_fft_2048[i] = g_DSO1->usWaveBufTemp[i];
			}
			
			/* 2048点实序列快速FFT, 浮点 */ 
			arm_rfft_fast_f32(&S, testInput_fft_2048, testOutput_fft_2048, ifftFlag);
			
			/* 求1204点的幅值 */ 
			arm_cmplx_mag_f32(testOutput_fft_2048, testInput_fft_2048, 1024);
			
			/* 获得最大的幅值和所在的位置 */
			uiCycle = testInput_fft_2048[1];
			
			for(i = 2; i < 1024; i++)
			{
				if(uiCycle < testInput_fft_2048[i])
				{
					uiCycle =testInput_fft_2048[i];
					uiCount = i;
				}
			}
			
			/* 估算频率 */
			g_DSO1->uiFreq = uiCount * g_SampleFreqTable[TimeBaseId][0] / 2048;
			
			/* 执行650点，80阶Fir滤波器 */
			/* 单通道2Msps */
			if((TimeBaseId == 1)&&(g_DSO1->ucFirFlter_Step100KHz !=0 ))
			{
				DSO_FirFilter_Step100KHz();
			}
				
			/* 单通道200Ksps */
			if((TimeBaseId == 4)&&(g_DSO1->ucFirFlter_Step10KHz !=0 ))
			{
				DSO_FirFilter_Step10KHz();
			}
				
			/* 单通道20Ksps */
			if((TimeBaseId == 7)&&(g_DSO1->ucFirFlter_Step1KHz !=0 ))
			{
				DSO_FirFilter_Step1KHz();
			}
		}
	}
		
	/* 求600个数值的最大值和最小值 */
	for (i = 0; i < DSOSCREEN_LENGTH; i++) 
	{
		g_DSO1->WaveMean += g_DSO1->usWaveBuf[i];
		g_RMSBUF[i] = g_DSO1->usWaveBuf[i];
		
		if(g_DSO1->usWaveBuf[i] < g_DSO1->WaveMin)
		{
			g_DSO1->WaveMin = g_DSO1->usWaveBuf[i];
		}
		
		if(g_DSO1->usWaveBuf[i] > g_DSO1->WaveMax)
		{
			g_DSO1->WaveMax = g_DSO1->usWaveBuf[i];
		}
	}
	
	/* 求RMS */
	arm_rms_f32(g_RMSBUF, DSOSCREEN_LENGTH, &g_DSO1->WaveRMS);	
	g_DSO1->WaveRMS = g_DSO1->WaveRMS * 3.3f / 4095;
	
	/* 求平均值 */
	g_DSO1->WaveMean = g_DSO1->WaveMean / 600 *3.3f / 4095;
	
	/* 求最大值 */
	g_DSO1->WaveMax =  g_DSO1->WaveMax * 3.3f / 4095;
	
	/* 求最小值 */
	g_DSO1->WaveMin = g_DSO1->WaveMin *3.3f / 4095;
	
	/* 求峰峰值 */
	g_DSO1->WavePkPk = g_DSO1->WaveMax - g_DSO1->WaveMin;		
}

/*
*********************************************************************************************************
*	函 数 名: DSO2_WaveTrig
*	功能说明: 波形通道2的处理
*	形    参: usCurPos  DMA在ADC缓冲中传输的位置。
*	返 回 值: 无
*********************************************************************************************************
*/
void DSO2_WaveTrig(uint16_t usCurPos)
{
	uint16_t i,j,k; 	
	/*
		1. 自动触发时的波形显示
		2. 每次至少要保证采集2KB的数据，也就是单通道2Ksps（500us）时，程序中设置2Ksps及其以下采用
	       慢速方式显示。
	*/
	if(TriggerFlag == 0)
	{
		/* 读取的是ADC1的位置 */
		usCurPos = 10240 - DMA1_Stream1->NDTR;
		
		/* 前10种采样率采用直接刷新的方式进行显示 */
		if(TimeBaseId < 10)
		{
			/*ADC1，ADC3都有10k 16bit的数据缓存区。而我们每次采集时先确定当前DMA已经传输到的位置，
			  然后取这个位置之前的2K数据。
			*/
			/* 第一种情况：当前DMA传输位置在小于2k的地方 */
			if(usCurPos < 2048)
			{
				//j = 10240 -(2048 - usCurPos);
				j = 8192 + usCurPos;
				
				/* 获取2k数据的前部分  */
				for(i = j; i < 10240; i++)
				{
					k = i - j;
					g_DSO2->usWaveBufTemp[k] = ADC1ConvertedValue[i];
				}
				
				j = 2048 - usCurPos;
				
				/* 获取2K数据的后部分 */
				for(i = 0; i < usCurPos; i++)
				{
					g_DSO2->usWaveBufTemp[j+i] = ADC1ConvertedValue[i];
				}		
			}
			/* 第二种情况：当前DMA传输位置在大于等于2k的地方 */
			else
			{
				usCurPos = usCurPos - 2048;
				for(i = 0; i < 2048; i++)
				{
					g_DSO2->usWaveBufTemp[i] = ADC1ConvertedValue[i+usCurPos];
				}	
			}
			
			/* 通过软件检测实现上升沿触发，并保留最后600的数据不做检测，用于直接显示 
			   2048-768 = 1280;
			*/
			j = 0;
			for(i = 0; i < 1280; i++)
			{
				j++;
				if((g_DSO2->usWaveBufTemp[i] > g_TrigVol->usTrigValue) && 
				   (g_DSO2->usWaveBufTemp[i+1] < g_TrigVol->usTrigValue))
				{	
					break;
				}
			}
			
			/* 记录满足上升沿触发的位置 */
			g_DSO2->sCurTriPos = j;
		
			/* 从位置 g_usCurTriPos+g_usCurTriStep 开始的600个数据全部赋值给g_usWaveBuf */
			for(i = 0; i < DSOSCREEN_LENGTH; i++)
			{
				g_DSO2->usWaveBuf[i] = g_DSO2->usWaveBufTemp[i+g_DSO2->sCurTriPos];
			}
		}
		/* 前10种采样率之后采用直接刷新的方式进行显示 */
		else
		{
				/* g_ucSlowWaveFlag = 0 表示启动慢速显示模式，也就是滚动显示 */
			if(g_DSO2->ucSlowWaveFlag == 0)
			{
				/* g_DSO2->ulSlowRefresh0 表示DMA当前传输到的位置 */
				//g_DSO2->ulSlowRefresh0 = 10240 - DMA2_Stream1->NDTR;
				g_DSO2->ulSlowRefresh1 = usCurPos;
				
				/* g_DSO2->ulSlowRefresh1表示按键调节频率后DMA传输到的位置 
				   通过g_DSO2->ulSlowRefresh1和g_DSO2->ulSlowRefresh0的差值确定刚开始显示
				   波形时600个数据的起始位置。(刚开始显示波形有一个从左向右的移动过程)
				*/
				if(g_DSO2->ulSlowRefresh1 >= g_DSO2->ulSlowRefresh0)
				{
					g_DSO2->ulSlowRefresh1 = g_DSO2->ulSlowRefresh1 - g_DSO2->ulSlowRefresh0;
				}
				else
				{
					g_DSO2->ulSlowRefresh1 = 10240 + g_DSO2->ulSlowRefresh1 - g_DSO2->ulSlowRefresh0;
				}
				
				/* 当差值大于600的时候，停止移动过程，开始整体刷新 */
				if(g_DSO2->ulSlowRefresh1 >= 768)
				{
					g_DSO2->ucSlowWaveFlag = 1;
				}
			}
			
			/* 第一种情况：当前DMA传输位置在小于600的地方 */
			if(usCurPos < 768)
			{
				//j = 600 - usCurPos;
				//j = 10240 - j;
				j = 9472 + usCurPos;
				
				/* 获取600个数据的前部分  */
				for(i = j; i < 10240; i++)
				{
					k = i - j;
					g_DSO2->usWaveBufTemp[k] = ADC1ConvertedValue[i];
				}
				
				j = 768 - usCurPos;
				
				/* 获取600个数据的后部分  */
				for(i = 0; i < usCurPos; i++)
				{
					g_DSO2->usWaveBufTemp[j+i] = ADC1ConvertedValue[i];
				}
			}
			/* 第二种情况：当前DMA传输位置在大于等于600的地方 */
			else
			{
				usCurPos = usCurPos - 768;
				for(i = 0; i < 768; i++)
				{
					g_DSO2->usWaveBufTemp[i] = ADC1ConvertedValue[i+usCurPos];
				}
			}

			/* 将获取的数据赋值给g_usWaveBuf */
			for(i = 0; i < DSOSCREEN_LENGTH; i++)
			{
				g_DSO2->usWaveBuf[i] = g_DSO2->usWaveBufTemp[767-i];
			}
		}			
	}
}

/*
*********************************************************************************************************
*	函 数 名: DSO2_WaveProcess
*	功能说明: 波形通道2的最大值，最小值，平均值，峰峰值和RMS的计算
*	形    参: 无。
*	返 回 值: 无
*********************************************************************************************************
*/
void DSO2_WaveProcess(void)
{
	uint16_t  i; 
	
	g_DSO1->WaveMin = 4095;
	g_DSO1->WaveMax = 0.0f;
	g_DSO1->WaveMean = 0.0f;	

	/* 求600个数值的最大值和最小值 */
	for (i = 0; i < DSOSCREEN_LENGTH; i++) 
	{
		g_DSO2->WaveMean += g_DSO2->usWaveBuf[i];
		g_RMSBUF[i] = g_DSO2->usWaveBuf[i];	
		
		if(g_DSO2->usWaveBuf[i] < g_DSO2->WaveMin)
		{
			g_DSO2->WaveMin = g_DSO2->usWaveBuf[i];
		}
		
		if(g_DSO2->usWaveBuf[i] > g_DSO2->WaveMax)
		{
			g_DSO2->WaveMax = g_DSO2->usWaveBuf[i];
		}
	}
	
	/* 求RMS */
	arm_rms_f32(g_RMSBUF, DSOSCREEN_LENGTH, &g_DSO2->WaveRMS);
	g_DSO2->WaveRMS = g_DSO2->WaveRMS * 3.3f / 4095;
		
	/* 求平均值 */
	g_DSO2->WaveMean = g_DSO2->WaveMean / 600 *3.3f / 4095;
	
	/* 求最大值 */
	g_DSO2->WaveMax =  g_DSO2->WaveMax * 3.3f / 4095;
	
	/* 求最小值 */
	g_DSO2->WaveMin = g_DSO2->WaveMin *3.3f / 4095;
	
	/* 求峰峰值 */
	g_DSO2->WavePkPk = g_DSO2->WaveMax - g_DSO2->WaveMin;			
}

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
