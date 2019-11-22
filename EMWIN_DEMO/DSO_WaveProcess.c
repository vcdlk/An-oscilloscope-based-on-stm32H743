/*
*********************************************************************************************************
*	                                  
*	ģ������ : ��ADCֵ��ȡ����
*	�ļ����� : DSO_WaveProcess.c
*	��    �� : V1.0
*	˵    �� : ʵ�ּ򵥵�ADCֵ��ȡ��
*	�޸ļ�¼ :
*		�汾��    ����          ����           ˵��
*		V1.0    2018-01-06     Eric2013        �׷� 
*                                           
*	Copyright (C), 2018-2028, ���������� www.armfly.com
*
*********************************************************************************************************
*/
#include "includes.h"
#include "MainTask.h"


extern ADC_AnalogWDGConfTypeDef   	AnalogWDGConfig_1;
/*
*********************************************************************************************************
*	                                  ��������
*********************************************************************************************************
*/
float32_t *g_RMSBUF;


/*
*********************************************************************************************************
*	�� �� ��: DSO1_WaveTrig
*	����˵��: ����ͨ��1�Ĵ���
*	��    ��: usCurPos  DMA��ADC�����д����λ�á�
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void DSO1_WaveTrig(uint16_t usCurPos)
{
	uint16_t i,j,k;  
	int16_t temp;
	
	/*
		1. �Զ�����ʱ�Ĳ�����ʾ
		2. ÿ������Ҫ��֤�ɼ�2KB�����ݣ�Ҳ���ǵ�ͨ��2Ksps��500us��ʱ������������2Ksps�������²���
	       ���ٷ�ʽ��ʾ��
	*/
	if(TriggerFlag == 0){
		/* ��ȡ����ADC3��λ�� */
		//usCurPos = 10240 - DMA2_Stream1->NDTR;
		
		/* ǰ10�ֲ����ʲ���ֱ��ˢ�µķ�ʽ������ʾ****************************************/
		if(TimeBaseId < 10){
			/* ADC1��ADC3����10k 16bit�����ݻ�������������ÿ�βɼ�ʱ��ȷ����ǰDMA�Ѿ����䵽��λ�ã�
			   Ȼ��ȡ���λ��֮ǰ��2K���ݡ�
			*/
			/* ��һ���������ǰDMA����λ����С��2k�ĵط� */
			if(usCurPos < 2048){
				//j = 2048 - usCurPos;
				//j = 10240 - j;
				j = 8192 + usCurPos;
				
				/* ��ȡ2k���ݵ�ǰ����  */
				for(i = j; i < 10240; i++){
					k = i - j;
					g_DSO1->usWaveBufTemp[k] = ADC3ConvertedValue[i];
				}
				
				j = 2048 - usCurPos;
				
				/* ��ȡ2K���ݵĺ󲿷� */
				for(i = 0; i < usCurPos; i++){
					g_DSO1->usWaveBufTemp[j+i] = ADC3ConvertedValue[i];
				}		
			}
			/* �ڶ����������ǰDMA����λ���ڴ��ڵ���2k�ĵط� */
			else{
				usCurPos = usCurPos - 2048;
				for(i = 0; i < 2048; i++)
				{
					g_DSO1->usWaveBufTemp[i] = ADC3ConvertedValue[i+usCurPos];
				}	
			}
			
			/* ͨ��������ʵ�������ش��������������600�����ݲ�����⣬����ֱ����ʾ 
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
			
			/* ��¼���������ش�����λ�� */
			g_DSO1->sCurTriPos = j;
			
			/* ����g_usCurTriStep����ʵ��2K���ݵ��ƶ�����ֹ����С��0 */
			if((g_DSO1->sCurTriPos + g_DSO1->sCurTriStep) < 0)
			{
				/* ��g_usCurTriStepȡ��ӽ�g_usCurTriPos + g_usCurTriStep = 0 */
				//temp = g_usCurTriPos/100*100;
				temp = g_DSO1->sCurTriPos;
				g_DSO1->sCurTriStep = -temp;
			}
			
			/* ���ɼ�2*1024 = 2048�����ݣ��ƶ���������ٻ���ʣ600�����ݣ���2048-600=1448��
			   ��ֹg_usCurTriPos + g_usCurTriStep��λ�ó���1448��
			*/
			if((g_DSO1->sCurTriPos + g_DSO1->sCurTriStep) > 1280)
			{
				/* ��g_usCurTriStepȡ��ӽ�g_usCurTriPos + g_usCurTriStep < 1448����ֵ */
				//temp = (1448 - g_usCurTriPos)/100*100;
				temp = (1280 - g_DSO1->sCurTriPos);
				g_DSO1->sCurTriStep = temp;
			}
			
			/* ��λ��g_usCurTriPos+g_usCurTriStep��ʼ��600������ȫ����ֵ��g_usWaveBuf */
			for(i = 0; i < DSOSCREEN_LENGTH; i++)
			{
				g_DSO1->usWaveBuf[i] = g_DSO1->usWaveBufTemp[i+g_DSO1->sCurTriPos+g_DSO1->sCurTriStep];
			}
		}
		/* ǰ10�ֲ�����֮�����ֱ��ˢ�µķ�ʽ������ʾ****************************************/
		else
		{
			/* g_ucSlowWaveFlag = 0 ��ʾ����������ʾģʽ��Ҳ���ǹ�����ʾ */
			if(g_DSO1->ucSlowWaveFlag == 0)
			{
				/* g_DSO1->ulSlowRefresh0��ʾDMA��ǰ���䵽��λ�� */
				//g_DSO1->ulSlowRefresh0 = 10240 - DMA2_Stream1->NDTR;
				g_DSO1->ulSlowRefresh1 = usCurPos;
				
				/* g_DSO1->ulSlowRefresh1��ʾ��������Ƶ�ʺ�DMA���䵽��λ�� 
				   ͨ��g_DSO1->ulSlowRefresh1��g_DSO1->ulSlowRefresh0�Ĳ�ֵȷ���տ�ʼ��ʾ
				   ����ʱ600�����ݵ���ʼλ�á�(�տ�ʼ��ʾ������һ���������ҵ��ƶ�����)
				*/
				if(g_DSO1->ulSlowRefresh1 >= g_DSO1->ulSlowRefresh0)
				{
					g_DSO1->ulSlowRefresh1 = g_DSO1->ulSlowRefresh1 - g_DSO1->ulSlowRefresh0;
				}
				else
				{
					g_DSO1->ulSlowRefresh1 = 10240 + g_DSO1->ulSlowRefresh1 - g_DSO1->ulSlowRefresh0;
				}
				
				/* ����ֵ����600��ʱ��ֹͣ�ƶ����̣���ʼ����ˢ�� */
				if(g_DSO1->ulSlowRefresh1 >= 768)
				{
					g_DSO1->ucSlowWaveFlag = 1;
				}
			}
			
			/* ��һ���������ǰDMA����λ����С��600�ĵط� */
			if(usCurPos < 768)
			{
				//j = 600 - usCurPos;
				//j = 10240 - j;
				j = 9472 + usCurPos;
				
				/* ��ȡ600�����ݵ�ǰ����  */
				for(i = j; i < 10240; i++)
				{
					k = i - j;
					g_DSO1->usWaveBufTemp[k] = ADC3ConvertedValue[i];
				}
				
				j = 768 - usCurPos;
				
				/* ��ȡ600�����ݵĺ󲿷�  */
				for(i = 0; i < usCurPos; i++)
				{
					g_DSO1->usWaveBufTemp[j+i] = ADC3ConvertedValue[i];
				}
			}
			/* �ڶ����������ǰDMA����λ���ڴ��ڵ���600�ĵط� */
			else
			{
				usCurPos = usCurPos - 768;
				for(i = 0; i < 768; i++)
				{
					g_DSO1->usWaveBufTemp[i] = ADC3ConvertedValue[i+usCurPos];
				}
			}

			/* ����ȡ�����ݸ�ֵ��g_usWaveBuf */
			for(i = 0; i < DSOSCREEN_LENGTH; i++)
			{
				g_DSO1->usWaveBuf[i] = g_DSO1->usWaveBufTemp[767-i];
			}
		}
	}
	/* ѡ�� ��ͨ���� ģʽ��Ĳ�����ʾ 
	   ��ͨ����ģʽһ��������ǲ�ˢ����ʾ�ģ�ֻ�б�����������ڲ��������ˢ�¡�
	*/
	else
	{
		/* �˱�־������ʾ���������ͣ������ʾ��1��ʾ��ͣ��0��ʾ���� */
		g_Flag->hWinRunStop = 1;
		
		/* TriggerFlag = 2 ��ʾ�ﵽ�����������Ѿ��ɼ��괥��ֵǰ���1K���ݣ���ͨ���� */
		if(TriggerFlag == 2)
		{
			/* ��������ʾ */
			g_Flag->hWinRunStop = 0;
			
			/* �ɼ���ʱ���ǲɼ��˵�ͨ��ǰ���1024�����ݣ���ͨ���ܹ�����1024*2 = 2048
			   2048/2 = 1024 ��1024�����Ǵ���ֵ��������ȡ��1024��ǰ300���ͺ�300������
			   �Ͻ�����ʾ��ͨ������g_usCurTriStep��ϰ�������ʵ�ֲ��ε��ƶ��۲졣
			*/
			for(i = 640; i < 1408; i++)
			{
				g_DSO1->usWaveBuf[i-640] = g_DSO1->usWaveBufTemp[i+g_DSO1->sCurTriStep] ;
			}
			
			/* TriggerFlag = 1 ��ʾ�ȴ��´δ��� */
			TriggerFlag = 1;
			
			/* ʹ��ADC3�Ŀ��Ź� */
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
*	�� �� ��: DSO1_WaveProcess
*	����˵��: ����ͨ��1��FFT��FIR�����ֵ����Сֵ��ƽ��ֵ�����ֵ��Ƶ�ʺ�RMS�ļ���
*	��    ��: �ޡ�
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void DSO1_WaveProcess(void)
{
	uint16_t  i; 
	uint32_t  uiCycle, uiCount = 0;

	g_DSO1->WaveMin = 4095;
	g_DSO1->WaveMax = 0.0f;
	g_DSO1->WaveMean = 0.0f;	
	
	/* �Զ�����ģʽ�ż���FFT */
	if(TriggerFlag == 0)
	{
		/* ǰ10�ֲ����ʼ���FFT */
		if(TimeBaseId < 10)
		{
			/* ִ��2048�㸡��FFT���� */
			for(i=0; i<2048; i++)
			{
				testInput_fft_2048[i] = g_DSO1->usWaveBufTemp[i];
			}
			
			/* 2048��ʵ���п���FFT, ���� */ 
			arm_rfft_fast_f32(&S, testInput_fft_2048, testOutput_fft_2048, ifftFlag);
			
			/* ��1204��ķ�ֵ */ 
			arm_cmplx_mag_f32(testOutput_fft_2048, testInput_fft_2048, 1024);
			
			/* ������ķ�ֵ�����ڵ�λ�� */
			uiCycle = testInput_fft_2048[1];
			
			for(i = 2; i < 1024; i++)
			{
				if(uiCycle < testInput_fft_2048[i])
				{
					uiCycle =testInput_fft_2048[i];
					uiCount = i;
				}
			}
			
			/* ����Ƶ�� */
			g_DSO1->uiFreq = uiCount * g_SampleFreqTable[TimeBaseId][0] / 2048;
			
			/* ִ��650�㣬80��Fir�˲��� */
			/* ��ͨ��2Msps */
			if((TimeBaseId == 1)&&(g_DSO1->ucFirFlter_Step100KHz !=0 ))
			{
				DSO_FirFilter_Step100KHz();
			}
				
			/* ��ͨ��200Ksps */
			if((TimeBaseId == 4)&&(g_DSO1->ucFirFlter_Step10KHz !=0 ))
			{
				DSO_FirFilter_Step10KHz();
			}
				
			/* ��ͨ��20Ksps */
			if((TimeBaseId == 7)&&(g_DSO1->ucFirFlter_Step1KHz !=0 ))
			{
				DSO_FirFilter_Step1KHz();
			}
		}
	}
		
	/* ��600����ֵ�����ֵ����Сֵ */
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
	
	/* ��RMS */
	arm_rms_f32(g_RMSBUF, DSOSCREEN_LENGTH, &g_DSO1->WaveRMS);	
	g_DSO1->WaveRMS = g_DSO1->WaveRMS * 3.3f / 4095;
	
	/* ��ƽ��ֵ */
	g_DSO1->WaveMean = g_DSO1->WaveMean / 600 *3.3f / 4095;
	
	/* �����ֵ */
	g_DSO1->WaveMax =  g_DSO1->WaveMax * 3.3f / 4095;
	
	/* ����Сֵ */
	g_DSO1->WaveMin = g_DSO1->WaveMin *3.3f / 4095;
	
	/* ����ֵ */
	g_DSO1->WavePkPk = g_DSO1->WaveMax - g_DSO1->WaveMin;		
}

/*
*********************************************************************************************************
*	�� �� ��: DSO2_WaveTrig
*	����˵��: ����ͨ��2�Ĵ���
*	��    ��: usCurPos  DMA��ADC�����д����λ�á�
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void DSO2_WaveTrig(uint16_t usCurPos)
{
	uint16_t i,j,k; 	
	/*
		1. �Զ�����ʱ�Ĳ�����ʾ
		2. ÿ������Ҫ��֤�ɼ�2KB�����ݣ�Ҳ���ǵ�ͨ��2Ksps��500us��ʱ������������2Ksps�������²���
	       ���ٷ�ʽ��ʾ��
	*/
	if(TriggerFlag == 0)
	{
		/* ��ȡ����ADC1��λ�� */
		usCurPos = 10240 - DMA1_Stream1->NDTR;
		
		/* ǰ10�ֲ����ʲ���ֱ��ˢ�µķ�ʽ������ʾ */
		if(TimeBaseId < 10)
		{
			/*ADC1��ADC3����10k 16bit�����ݻ�������������ÿ�βɼ�ʱ��ȷ����ǰDMA�Ѿ����䵽��λ�ã�
			  Ȼ��ȡ���λ��֮ǰ��2K���ݡ�
			*/
			/* ��һ���������ǰDMA����λ����С��2k�ĵط� */
			if(usCurPos < 2048)
			{
				//j = 10240 -(2048 - usCurPos);
				j = 8192 + usCurPos;
				
				/* ��ȡ2k���ݵ�ǰ����  */
				for(i = j; i < 10240; i++)
				{
					k = i - j;
					g_DSO2->usWaveBufTemp[k] = ADC1ConvertedValue[i];
				}
				
				j = 2048 - usCurPos;
				
				/* ��ȡ2K���ݵĺ󲿷� */
				for(i = 0; i < usCurPos; i++)
				{
					g_DSO2->usWaveBufTemp[j+i] = ADC1ConvertedValue[i];
				}		
			}
			/* �ڶ����������ǰDMA����λ���ڴ��ڵ���2k�ĵط� */
			else
			{
				usCurPos = usCurPos - 2048;
				for(i = 0; i < 2048; i++)
				{
					g_DSO2->usWaveBufTemp[i] = ADC1ConvertedValue[i+usCurPos];
				}	
			}
			
			/* ͨ��������ʵ�������ش��������������600�����ݲ�����⣬����ֱ����ʾ 
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
			
			/* ��¼���������ش�����λ�� */
			g_DSO2->sCurTriPos = j;
		
			/* ��λ�� g_usCurTriPos+g_usCurTriStep ��ʼ��600������ȫ����ֵ��g_usWaveBuf */
			for(i = 0; i < DSOSCREEN_LENGTH; i++)
			{
				g_DSO2->usWaveBuf[i] = g_DSO2->usWaveBufTemp[i+g_DSO2->sCurTriPos];
			}
		}
		/* ǰ10�ֲ�����֮�����ֱ��ˢ�µķ�ʽ������ʾ */
		else
		{
				/* g_ucSlowWaveFlag = 0 ��ʾ����������ʾģʽ��Ҳ���ǹ�����ʾ */
			if(g_DSO2->ucSlowWaveFlag == 0)
			{
				/* g_DSO2->ulSlowRefresh0 ��ʾDMA��ǰ���䵽��λ�� */
				//g_DSO2->ulSlowRefresh0 = 10240 - DMA2_Stream1->NDTR;
				g_DSO2->ulSlowRefresh1 = usCurPos;
				
				/* g_DSO2->ulSlowRefresh1��ʾ��������Ƶ�ʺ�DMA���䵽��λ�� 
				   ͨ��g_DSO2->ulSlowRefresh1��g_DSO2->ulSlowRefresh0�Ĳ�ֵȷ���տ�ʼ��ʾ
				   ����ʱ600�����ݵ���ʼλ�á�(�տ�ʼ��ʾ������һ���������ҵ��ƶ�����)
				*/
				if(g_DSO2->ulSlowRefresh1 >= g_DSO2->ulSlowRefresh0)
				{
					g_DSO2->ulSlowRefresh1 = g_DSO2->ulSlowRefresh1 - g_DSO2->ulSlowRefresh0;
				}
				else
				{
					g_DSO2->ulSlowRefresh1 = 10240 + g_DSO2->ulSlowRefresh1 - g_DSO2->ulSlowRefresh0;
				}
				
				/* ����ֵ����600��ʱ��ֹͣ�ƶ����̣���ʼ����ˢ�� */
				if(g_DSO2->ulSlowRefresh1 >= 768)
				{
					g_DSO2->ucSlowWaveFlag = 1;
				}
			}
			
			/* ��һ���������ǰDMA����λ����С��600�ĵط� */
			if(usCurPos < 768)
			{
				//j = 600 - usCurPos;
				//j = 10240 - j;
				j = 9472 + usCurPos;
				
				/* ��ȡ600�����ݵ�ǰ����  */
				for(i = j; i < 10240; i++)
				{
					k = i - j;
					g_DSO2->usWaveBufTemp[k] = ADC1ConvertedValue[i];
				}
				
				j = 768 - usCurPos;
				
				/* ��ȡ600�����ݵĺ󲿷�  */
				for(i = 0; i < usCurPos; i++)
				{
					g_DSO2->usWaveBufTemp[j+i] = ADC1ConvertedValue[i];
				}
			}
			/* �ڶ����������ǰDMA����λ���ڴ��ڵ���600�ĵط� */
			else
			{
				usCurPos = usCurPos - 768;
				for(i = 0; i < 768; i++)
				{
					g_DSO2->usWaveBufTemp[i] = ADC1ConvertedValue[i+usCurPos];
				}
			}

			/* ����ȡ�����ݸ�ֵ��g_usWaveBuf */
			for(i = 0; i < DSOSCREEN_LENGTH; i++)
			{
				g_DSO2->usWaveBuf[i] = g_DSO2->usWaveBufTemp[767-i];
			}
		}			
	}
}

/*
*********************************************************************************************************
*	�� �� ��: DSO2_WaveProcess
*	����˵��: ����ͨ��2�����ֵ����Сֵ��ƽ��ֵ�����ֵ��RMS�ļ���
*	��    ��: �ޡ�
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void DSO2_WaveProcess(void)
{
	uint16_t  i; 
	
	g_DSO1->WaveMin = 4095;
	g_DSO1->WaveMax = 0.0f;
	g_DSO1->WaveMean = 0.0f;	

	/* ��600����ֵ�����ֵ����Сֵ */
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
	
	/* ��RMS */
	arm_rms_f32(g_RMSBUF, DSOSCREEN_LENGTH, &g_DSO2->WaveRMS);
	g_DSO2->WaveRMS = g_DSO2->WaveRMS * 3.3f / 4095;
		
	/* ��ƽ��ֵ */
	g_DSO2->WaveMean = g_DSO2->WaveMean / 600 *3.3f / 4095;
	
	/* �����ֵ */
	g_DSO2->WaveMax =  g_DSO2->WaveMax * 3.3f / 4095;
	
	/* ����Сֵ */
	g_DSO2->WaveMin = g_DSO2->WaveMin *3.3f / 4095;
	
	/* ����ֵ */
	g_DSO2->WavePkPk = g_DSO2->WaveMax - g_DSO2->WaveMin;			
}

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
