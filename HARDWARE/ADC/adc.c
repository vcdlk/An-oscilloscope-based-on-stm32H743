// ��ʵMSP����Ӧ��д��STM32h7xx_hal_msp.c��

#include "adc.h"
#include "delay.h"
	
ADC_HandleTypeDef 					ADC1_Handler;//ADC1���
ADC_HandleTypeDef 					ADC2_Handler;//ADC2���
ADC_HandleTypeDef 					ADC3_Handler;//ADC3���
TIM_HandleTypeDef       		TimHandle;//���ö�ʱ��
TIM_HandleTypeDef 					TIM2_Handler;      //��ʱ�����
TIM_MasterConfigTypeDef     master_timer_config;
ADC_AnalogWDGConfTypeDef   	AnalogWDGConfig_1;
DMA_HandleTypeDef       			   DmaHandle1;
DMA_HandleTypeDef       			   DmaHandle2;
/*
*********************************************************************************************************
*	                           ADC1��ADC3�����ݻ��棬��С��Ϊ10240
*********************************************************************************************************
*/
uint16_t ADC1ConvertedValue[10240];
uint16_t ADC3ConvertedValue[10240];
uint16_t temp[100];
/*
*********************************************************************************************************
*	                                   �����ͺ���                           
*********************************************************************************************************
*/
static uint16_t g_usFirstTimeIRQFlag;  /* ��һ�ν��붨ʱ��8���жϱ�־ */
static uint16_t g_usTrigCount = 0;     /* ��¼��ͨ�������뿴�Ź��ж�ʱ��DMA�����λ�� */
static uint16_t g_usTrigTempFlag = 0;  /* ��ʱ����������ADC�ж� */
/**********************��ʼ��ADC******************/
void MY_ADC_Init(void)
{ 
//ADC_DMA_Close();
	//��һ������ADC��ADCͨ�������Ź�
		ADC1_Config();			//ADC1---DSO2---PA6---INP19
		ADC2_Config();
		ADC3_Config();			//ADC3---DSO1---PH4---INP15
		//У׼ADC
		HAL_ADCEx_Calibration_Start(&ADC1_Handler,ADC_CALIB_OFFSET,ADC_SINGLE_ENDED); //ADCУ׼
		HAL_ADCEx_Calibration_Start(&ADC3_Handler,ADC_CALIB_OFFSET,ADC_SINGLE_ENDED); //ADCУ׼
  //��ʱ��3  ����
		TIM_Config();
		TIM2_MeasureTrigConfig();
	//��ʱ����ʼ
		HAL_TIM_Base_Start(&TimHandle);
//		HAL_TIM_Base_Start(&TIM2_Handler);
	//����DMA
		HAL_ADC_Start_DMA(&ADC1_Handler,
											(uint32_t *)ADC1ConvertedValue,
											1024*10
											);
		HAL_ADC_Start_DMA(&ADC3_Handler,
											(uint32_t *)ADC3ConvertedValue,
											1024*10
											);
}
void ADC1_Config(void){//����ADC��ADCͨ�������Ź�
		ADC_ChannelConfTypeDef  				           ADC1_ChanConf;
		//ADC1����
    ADC1_Handler.Instance											 =ADC1;
    ADC1_Handler.Init.ClockPrescaler   				 =ADC_CLOCK_SYNC_PCLK_DIV4; 	//4��Ƶ��ADCCLK=PER_CK/4=64/4=16MHZ
    ADC1_Handler.Init.Resolution     					 =ADC_RESOLUTION_12B;  //16λģʽ
    ADC1_Handler.Init.ScanConvMode						 =ADC_SCAN_DISABLE;   //��ɨ��ģʽ
    ADC1_Handler.Init.EOCSelection					   =ADC_EOC_SINGLE_CONV; //�ر�EOC�ж�
		ADC1_Handler.Init.LowPowerAutoWait				 =DISABLE;					//�Զ��͹��Ĺر�				
    ADC1_Handler.Init.ContinuousConvMode			 =DISABLE;          //�ر�����ת��
    ADC1_Handler.Init.NbrOfConversion					 =1;              //1��ת���ڹ��������� Ҳ����ֻת����������1 
    ADC1_Handler.Init.DiscontinuousConvMode		 =DISABLE;            //��ֹ����������ģʽ
    ADC1_Handler.Init.NbrOfDiscConversion      =1;                    //����������ͨ����Ϊ0
    ADC1_Handler.Init.ExternalTrigConv         =ADC_EXTERNALTRIG_T3_TRGO;      //T3����
    ADC1_Handler.Init.ExternalTrigConvEdge     =ADC_EXTERNALTRIGCONVEDGE_RISING;//������
	
	  ADC1_Handler.Init.ConversionDataManagement =ADC_CONVERSIONDATA_DMA_CIRCULAR;  //����ͨ�������ݽ���������DR�Ĵ�������
		ADC1_Handler.Init.BoostMode    						 =ENABLE;							//BOOTģʽ�ر�
		ADC1_Handler.Init.Overrun									 =ADC_OVR_DATA_OVERWRITTEN;			//���µ����ݵ�����ֱ�Ӹ��ǵ�������
		ADC1_Handler.Init.OversamplingMode			   =DISABLE;					//�������ر�
    HAL_ADC_Init(&ADC1_Handler);                                 //��ʼ�� 
	
	//����ͨ��
	  ADC1_ChanConf.Channel                     =	ADC_CHANNEL_19;  //ͨ��
    ADC1_ChanConf.Rank               					=	ADC_REGULAR_RANK_1;        //1������
    ADC1_ChanConf.SamplingTime								=	ADC_SAMPLETIME_8CYCLES_5;  //����ʱ��       
		ADC1_ChanConf.SingleDiff									=	ADC_SINGLE_ENDED;  				//���߲ɼ�          		
		ADC1_ChanConf.OffsetNumber								=	ADC_OFFSET_NONE;             	
		ADC1_ChanConf.Offset											=	0;   
    HAL_ADC_ConfigChannel(&ADC1_Handler,&ADC1_ChanConf);        //ADC��ͨ��19 
//	//���Ź�����
//		AnalogWDGConfig.WatchdogNumber            = ADC_ANALOGWATCHDOG_2;
//		AnalogWDGConfig.WatchdogMode 							= ADC_ANALOGWATCHDOG_ALL_REG;
//		AnalogWDGConfig.Channel 									= ADC_CHANNEL_19;
//		AnalogWDGConfig.ITMode 										= ENABLE;
//		AnalogWDGConfig.HighThreshold 						= 2000;
//		AnalogWDGConfig.LowThreshold						  = 0;
//		HAL_ADC_AnalogWDGConfig(&ADC1_Handler, &AnalogWDGConfig);
}
void ADC3_Config(void){//����ADC3��ADC3ͨ��
		ADC_ChannelConfTypeDef  				 							ADC3_ChanConf;
		//ADC3����	
		ADC3_Handler.Instance 										 = 	ADC3;
		ADC3_Handler.Init.ClockPrescaler					 = 	ADC_CLOCKPRESCALER_PCLK_DIV4;
		ADC3_Handler.Init.Resolution							 =	ADC_RESOLUTION_12B;
    ADC3_Handler.Init.ScanConvMode						 =	ADC_SCAN_DISABLE;   //��ɨ��ģʽ
    ADC3_Handler.Init.EOCSelection					   =	ADC_EOC_SINGLE_CONV; //�ر�EOC�ж�
		ADC3_Handler.Init.LowPowerAutoWait				 =	DISABLE;					//�Զ��͹��Ĺر�				
    ADC3_Handler.Init.ContinuousConvMode			 =	DISABLE;          //�ر�����ת��
    ADC3_Handler.Init.NbrOfConversion					 =	1;              //1��ת���ڹ��������� Ҳ����ֻת����������1 
    ADC3_Handler.Init.DiscontinuousConvMode		 =	DISABLE;            //��ֹ����������ģʽ
    ADC3_Handler.Init.NbrOfDiscConversion      =	1;                    //����������ͨ����Ϊ0
    ADC3_Handler.Init.ExternalTrigConv         =	ADC_EXTERNALTRIG_T3_TRGO;      //T3����
    ADC3_Handler.Init.ExternalTrigConvEdge     =	ADC_EXTERNALTRIGCONVEDGE_RISING;//������

	  ADC3_Handler.Init.ConversionDataManagement =	ADC_CONVERSIONDATA_DMA_CIRCULAR;  //����ͨ�������ݽ���������DR�Ĵ�������
		ADC3_Handler.Init.BoostMode    						 =	ENABLE;							//BOOTģʽ�ر�
		ADC3_Handler.Init.Overrun									 =	ADC_OVR_DATA_OVERWRITTEN;			//���µ����ݵ�����ֱ�Ӹ��ǵ�������
		ADC3_Handler.Init.OversamplingMode			   =	DISABLE;					//�������ر�
    HAL_ADC_Init(&ADC3_Handler);                                 //��ʼ�� 	
		//����ͨ��PH4 ADC3_INP15
	  ADC3_ChanConf.Channel                     =	ADC_CHANNEL_2;  //ͨ��
    ADC3_ChanConf.Rank               					=	ADC_REGULAR_RANK_1;        //1������
    ADC3_ChanConf.SamplingTime								=	ADC_SAMPLETIME_8CYCLES_5;  //����ʱ��       
		ADC3_ChanConf.SingleDiff									=	ADC_SINGLE_ENDED;  				//���߲ɼ�          		
		ADC3_ChanConf.OffsetNumber								=	ADC_OFFSET_NONE;             	
		ADC3_ChanConf.Offset											=	0;   
    HAL_ADC_ConfigChannel(&ADC3_Handler,&ADC3_ChanConf);        //ADC��ͨ�� 15
	//���Ź�����
		AnalogWDGConfig_1.WatchdogNumber            = ADC_ANALOGWATCHDOG_1;
		AnalogWDGConfig_1.WatchdogMode 							= ADC_ANALOGWATCHDOG_ALL_REG;
		AnalogWDGConfig_1.Channel 									= ADC_CHANNEL_2;
		AnalogWDGConfig_1.ITMode 										= ENABLE;
		AnalogWDGConfig_1.HighThreshold 						= 4095;
		AnalogWDGConfig_1.LowThreshold						  = 0;
		HAL_ADC_AnalogWDGConfig(&ADC3_Handler, &AnalogWDGConfig_1);
}
//����ADC2
void ADC2_Config(){
		ADC2_Handler.Instance                     				= ADC2;
    ADC2_Handler.Init.ClockPrescaler									=ADC_CLOCK_SYNC_PCLK_DIV4; 	//4��Ƶ��ADCCLK=PER_CK/4=64/4=16MHZ
    ADC2_Handler.Init.Resolution											=ADC_RESOLUTION_12B;           	//16λģʽ
    ADC2_Handler.Init.ScanConvMode										=DISABLE;                    	//��ɨ��ģʽ
    ADC2_Handler.Init.EOCSelection										=ADC_EOC_SINGLE_CONV;       	//�ر�EOC�ж�
		ADC2_Handler.Init.LowPowerAutoWait								=	DISABLE;					//�Զ��͹��Ĺر�				
    ADC2_Handler.Init.ContinuousConvMode							=ENABLE;               //����ת��
    ADC2_Handler.Init.NbrOfConversion									=1;                        //1��ת���ڹ��������� Ҳ����ֻת����������1 
    ADC2_Handler.Init.DiscontinuousConvMode						=DISABLE;            //��ֹ����������ģʽ
    ADC2_Handler.Init.NbrOfDiscConversion							=0;                    //����������ͨ����Ϊ0
    ADC2_Handler.Init.ExternalTrigConv								=ADC_SOFTWARE_START;      //�������
    ADC2_Handler.Init.ExternalTrigConvEdge						=ADC_EXTERNALTRIGCONVEDGE_NONE;//ʹ���������
		ADC2_Handler.Init.BoostMode												=ENABLE;							//BOOTģʽ�ر�
		ADC2_Handler.Init.Overrun													=ADC_OVR_DATA_OVERWRITTEN;			//���µ����ݵ�����ֱ�Ӹ��ǵ�������
		ADC2_Handler.Init.OversamplingMode								=DISABLE;					//�������ر�
		ADC2_Handler.Init.ConversionDataManagement				=ADC_CONVERSIONDATA_DR;  //����ͨ�������ݽ���������DR�Ĵ�������
    HAL_ADC_Init(&ADC2_Handler);                                 //��ʼ�� 
		
		ADC_ChannelConfTypeDef ADC2_ChanConf;
    
    ADC2_ChanConf.Channel=ADC_CHANNEL_3;                                   //ͨ��
    ADC2_ChanConf.Rank=ADC_REGULAR_RANK_1;                  	//1������
    ADC2_ChanConf.SamplingTime=ADC_SAMPLETIME_64CYCLES_5;      	//����ʱ��       
		ADC2_ChanConf.SingleDiff=ADC_SINGLE_ENDED;  				//���߲ɼ�          		
		ADC2_ChanConf.OffsetNumber=ADC_OFFSET_NONE;             	
		ADC2_ChanConf.Offset=0;   
    HAL_ADC_ConfigChannel(&ADC2_Handler,&ADC2_ChanConf);        //ͨ������

    HAL_ADC_Start(&ADC2_Handler);                               //����ADC
	
	
		HAL_ADCEx_Calibration_Start(&ADC2_Handler,ADC_CALIB_OFFSET,ADC_SINGLE_ENDED); //ADCУ׼		
}
static void TIM_Config(){//���ö�ʱ��
	//���ö�ʱ��3�Ĳ���
	TimHandle.Instance 							  					= TIM3;
	TimHandle.Init.Period       		  					= 400000000/g_SampleFreqTable[TimeBaseId][0] - 1;    //ARR�Զ���װ�ؼĴ������ڵ�ֵ(��ʱʱ�䣩������Ƶ�ʺ���������»����ж�(Ҳ��˵��ʱʱ�䵽)
	TimHandle.Init.Prescaler    		  					= g_SampleFreqTable[TimeBaseId][1]-1; 
	TimHandle.Init.ClockDivision      					= TIM_CLOCKDIVISION_DIV1;
  TimHandle.Init.CounterMode        				 	= TIM_COUNTERMODE_UP;
  TimHandle.Init.RepetitionCounter 						= 0x0;
	HAL_TIM_Base_Init(&TimHandle);
 /* Timer TRGO selection */
	master_timer_config.MasterOutputTrigger 		= TIM_TRGO_UPDATE;//TIM_TRGO_OC1
  master_timer_config.MasterOutputTrigger2 		= TIM_TRGO2_RESET;
  master_timer_config.MasterSlaveMode 				= TIM_MASTERSLAVEMODE_DISABLE;
	HAL_TIMEx_MasterConfigSynchronization(&TimHandle,&master_timer_config);		
}
/*
*********************************************************************************************************
*	�� �� ��: TIM8_MeasureTrigConfig
*	����˵��: ʹ��TIM8Ϊ��ͨ����ģʽ�����ݲɼ���ʱ����ʱ�ɼ�����ֵǰ���1024��ADC����
*	��    �Σ���
*	�� �� ֵ: ��		        
*********************************************************************************************************
*/
const uint32_t g_TrigFreqTable[][2] =
{
	{143,   1024},    //2.8Msps  400000000/2800000 = 60  => 60 * 1024 
	{200,   1024},    //2Msps    400000000/2000000 = 84  => 84 * 1024 
	{400,  1024},     //1Msps    400000000/1000000 = 168 => 168 * 1024
	{800,  1024},     //500Ksps  168000000/500000  = 336 => 336 * 1024
	{2000,  1024},     //200Ksps  168000000/200000  = 840 => 840 * 1024
	
	{4000,  1024},    //100Ksps 168000000/100000  = 1680  => 1680 * 1024
	{8000,  1024},    //50Ksps  168000000/50000   = 3360  => 3360 * 1024 
	{20000,  1024},    //20Ksps  168000000/20000   = 8400  => 8400 * 1024
	{40000, 1024},    //10Ksps  168000000/10000   = 16800 => 16800 * 1024 
	{40000, 2048},    //5Ksps   168000000/5000    = 33600 => 33600 * 1024 

	/* ����5�ֲ���Ƶ����ˢ�½�������Ϊ�ɼ�ǰ��1024��ADC��ʱ��ϳ� */
	{42000,    4096},    //2Ksps 168000000/2000  = 84000 => 84000 * 1024
	{50000,    4096},    //1Ksps 168000000/1000  = 168000 => 168000 * 1024 
	{50000,    8192},    //500sps 168000000/500  = 336000 => 336000 * 1024 
	{42000,    20480},   //200sps 168000000/200  = 840000 => 840000 * 1024 
	{62500,    32758},   //100sps 168000000/100  = 1680000 => 1680000 * 1024
	
	/* �����⼸�ֲ����ʲ�������֧�� */
	{42000,    40960},   //50sps 
	{42000,    40960},   //20sps
	{42000,    40960},   //10sps 
	{42000,    40960},   //5sps 
	{42000,    40960},   //2sps 
	{42000,    40960},   //1sps 	
};
/* 
	ÿ�β���ɼ�����ֵǰ���1024��ADC����(��ͨ��)��
*/
void TIM2_MeasureTrigConfig(){
		TimeBaseId 											= 1;        /* ��������ADC��ͨ��1Msps�������� */
	/* ���ϵ�һ�ν����жϵı�־�������жϺ�����1 */
		g_usFirstTimeIRQFlag 						= 0;
    TIM2_Handler.Instance						=	TIM2;           //ͨ�ö�ʱ��2
	  TIM2_Handler.Init.Period				=	g_TrigFreqTable[TimeBaseId][0] - 1;   //�Զ�װ��ֵ
    TIM2_Handler.Init.Prescaler			=	g_TrigFreqTable[TimeBaseId][1] - 1;   //��Ƶ
    TIM2_Handler.Init.CounterMode		=	TIM_COUNTERMODE_UP;    //���ϼ�����
    TIM2_Handler.Init.ClockDivision	=	TIM_CLOCKDIVISION_DIV1;//ʱ�ӷ�Ƶ����
    HAL_TIM_Base_Init(&TIM2_Handler);  //ʹ�ܶ�ʱ��2�Ͷ�ʱ��2�����жϣ�TIM_IT_UPDATE    
}
/*
*********************************************************************************************************
*	�� �� ��: Time2Recorder
*	����˵��: ʹ��TIM2Ϊ��ͨ����ģʽ�����ݲɼ���ʱ����ʱ�ɼ�����ֵǰ���1024��ADC����
*	��    �Σ���
*	�� �� ֵ: ��		        
*********************************************************************************************************
*/
static void Time2Recorder(void){
	/* ���ϵ�һ�ν����жϵı�־ */
	g_usFirstTimeIRQFlag = 0;
	TIM2_Handler.Instance						=	TIM2;           //ͨ�ö�ʱ��2
	TIM2_Handler.Init.Period				=	g_TrigFreqTable[TimeBaseId][0] - 1;   //�Զ�װ��ֵ
  TIM2_Handler.Init.Prescaler			=	g_TrigFreqTable[TimeBaseId][1] - 1;   //��Ƶ
  TIM2_Handler.Init.CounterMode		=	TIM_COUNTERMODE_UP;    //���ϼ�����
  TIM2_Handler.Init.ClockDivision	=	TIM_CLOCKDIVISION_DIV1;//ʱ�ӷ�Ƶ����
  HAL_TIM_Base_Init(&TIM2_Handler);  //ʹ�ܶ�ʱ��2�Ͷ�ʱ��2�����жϣ�TIM_IT_UPDATE    
}
/*************************************
HAL ����û��ֱ�����ÿ��Ź����¶˵�ѹ�ĺ���
@param  hadc ��ADC���
				AnalogWDGConfig �����ÿ��Ź��ľ����Ӧ����ADC_Config()�е��ù�
				HighThreshold ���Ź���ѹ�߶�
				LowThreshold  ���Ź���ѹ�Ͷ�
**********************************/
void ADC_AnalogWatchdogThresholdsChange(ADC_HandleTypeDef* hadc, ADC_AnalogWDGConfTypeDef* AnalogWDG,
																				uint16_t HighThreshold,uint16_t LowThreshold){
		HAL_ADC_Stop(&ADC3_Handler);																				
		AnalogWDG->HighThreshold = HighThreshold;
		AnalogWDG->LowThreshold  = LowThreshold;																																				
		HAL_ADC_AnalogWDGConfig(hadc, AnalogWDG);	
		HAL_ADC_Start(&ADC3_Handler);	
}
/***************************************
�ر�ADC123�����Ӧ��DMA����
@param void
***************************************/
void ADC_DMA_Close(void){
		HAL_ADC_DeInit(&ADC1_Handler);
		HAL_ADC_DeInit(&ADC3_Handler);
		HAL_TIM_Base_DeInit(&TimHandle);
		HAL_TIM_Base_DeInit(&TIM2_Handler);
}
/***************************************
��ADC123�����Ӧ��DMA����
@param void
***************************************/
void ADC_DMA_Open(void){
	//��һ������ADC��ADCͨ�������Ź�
		ADC1_Config();			//ADC1---DSO2---PA6---INP19
		ADC3_Config();			//ADC3---DSO1---PH4---INP15	
  //��ʱ��3  ����
		TIM_Config();
	//����DMA
		HAL_ADC_Start_DMA(&ADC1_Handler,
											(uint32_t *)ADC1ConvertedValue,
											1024*10
											);
		HAL_ADC_Start_DMA(&ADC3_Handler,
											(uint32_t *)ADC3ConvertedValue,
											1024*10
											);
		//��ʱ��3��ʼ
		HAL_TIM_Base_Start(&TimHandle);
	/* ֻ������ͨ����ģʽ������TIM8�ļ�ʱ���� */
	if(TriggerFlag != 0)
	{
			Time2Recorder();
//			HAL_TIM_Base_Start(&TIM2_Handler);
	}


}
//ADC�ײ��������������ã�ʱ��ʹ��
//�˺����ᱻHAL_ADC_Init()����
//hadc:ADC���
void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
		GPIO_InitTypeDef 								 GPIO_Initure;
	if(hadc->Instance  == ADC1 ){	
		//*##- 1- Configure CLK###########/
		__HAL_RCC_GPIOA_CLK_ENABLE();						//����GPIOAʱ��
		__HAL_RCC_ADC12_CLK_ENABLE();           //ʹ��ADC1/2ʱ��
		__HAL_RCC_ADC_CONFIG(RCC_ADCCLKSOURCE_CLKP); //ADC����ʱ��ѡ��
		__HAL_RCC_DMA1_CLK_ENABLE();
		/*##- 2- Configure peripheral GPIO ############*/
		/* ADC1 Channel GPIO pin configuration */
		GPIO_Initure.Pin=GPIO_PIN_5;            //PA5
		GPIO_Initure.Mode=GPIO_MODE_ANALOG;     //ģ��
		GPIO_Initure.Pull=GPIO_NOPULL;          //����������
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);
		/*##- 3- Configure DMA ####################*/
		/*********************** Configure DMA parameters link ADC1 with DMA1_Stream1 ***/
		DmaHandle1.Instance                 = DMA1_Stream1;
		DmaHandle1.Init.Request             = DMA_REQUEST_ADC1;
		DmaHandle1.Init.Direction           = DMA_PERIPH_TO_MEMORY;
		DmaHandle1.Init.PeriphInc           = DMA_PINC_DISABLE;
		DmaHandle1.Init.MemInc              = DMA_MINC_ENABLE;
		DmaHandle1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
		DmaHandle1.Init.MemDataAlignment    = DMA_MDATAALIGN_HALFWORD;
		DmaHandle1.Init.Mode                = DMA_CIRCULAR;
		DmaHandle1.Init.Priority            = DMA_PRIORITY_MEDIUM;
		/* Deinitialize  & Initialize the DMA for new transfer */
		HAL_DMA_DeInit(&DmaHandle1);
		HAL_DMA_Init(&DmaHandle1);
		/* Associate the DMA handle �ѽṹ���е���������һ��*/
		__HAL_LINKDMA(hadc, DMA_Handle, DmaHandle1);	
	}
	if(hadc->Instance  == ADC3 ){	
		//*##- 1- Configure CLK###########/
		__HAL_RCC_GPIOF_CLK_ENABLE();									//����GPIOHʱ��
		__HAL_RCC_ADC3_CLK_ENABLE(); 									//ʹ��ADC3ʱ��
		__HAL_RCC_ADC_CONFIG(RCC_ADCCLKSOURCE_CLKP);  //ADC����ʱ��ѡ��
		__HAL_RCC_DMA2_CLK_ENABLE();
		/*##- 2- Configure peripheral GPIO ############*/
		/* ADC3 Channel GPIO pin configuration */
		GPIO_Initure.Pin=GPIO_PIN_9;            //PH4
		GPIO_Initure.Mode=GPIO_MODE_ANALOG;     //ģ��
		GPIO_Initure.Pull=GPIO_NOPULL;          //����������
		HAL_GPIO_Init(GPIOF,&GPIO_Initure);
		/*##- 3- Configure DMA ####################*/
		/*********************** Configure DMA parameters link ADC3 with DMA1_Stream2 ***/
		DmaHandle2.Instance                 = DMA2_Stream1;
		DmaHandle2.Init.Request             = DMA_REQUEST_ADC3;
		DmaHandle2.Init.Direction           = DMA_PERIPH_TO_MEMORY;
		DmaHandle2.Init.PeriphInc           = DMA_PINC_DISABLE;
		DmaHandle2.Init.MemInc              = DMA_MINC_ENABLE;
		DmaHandle2.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
		DmaHandle2.Init.MemDataAlignment    = DMA_MDATAALIGN_HALFWORD;
		DmaHandle2.Init.Mode                = DMA_CIRCULAR;
		DmaHandle2.Init.Priority            = DMA_PRIORITY_VERY_HIGH;
		/* Deinitialize  & Initialize the DMA for new transfer */
		HAL_DMA_DeInit(&DmaHandle2);
		HAL_DMA_Init(&DmaHandle2);
		/* Associate the DMA handle �ѽṹ���е���������һ��*/
		__HAL_LINKDMA(hadc, DMA_Handle, DmaHandle2);
	
		HAL_NVIC_SetPriority(ADC3_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(ADC3_IRQn);
	}
	if(hadc->Instance == ADC2){
		__HAL_RCC_GPIOA_CLK_ENABLE();									//����GPIOHʱ��
		__HAL_RCC_ADC12_CLK_ENABLE(); 									//ʹ��ADC3ʱ��
		__HAL_RCC_ADC_CONFIG(RCC_ADCCLKSOURCE_CLKP);  //ADC����ʱ��ѡ��
		/*##- 2- Configure peripheral GPIO ############*/
		/* ADC3 Channel GPIO pin configuration */
		GPIO_Initure.Pin=GPIO_PIN_6;            //PH4
		GPIO_Initure.Mode=GPIO_MODE_ANALOG;     //ģ��
		GPIO_Initure.Pull=GPIO_NOPULL;          //����������
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);
	}
}
/********************ADC�ײ�����
//�˺����ᱻHAL_ADC_DeInit()����
//hadc:ADC���**************/
void HAL_ADC_MspDeInit(ADC_HandleTypeDef* hadc){
		if(hadc->Instance == ADC1){
//1.Reset peripherals
		__HAL_RCC_ADC12_FORCE_RESET();
		__HAL_RCC_ADC12_RELEASE_RESET();
//2.Reset GPIO		
		HAL_GPIO_DeInit(GPIOA, GPIO_PIN_5);
//3.Reset De-Initialize DMA		
			  if(hadc->DMA_Handle != NULL){
						HAL_DMA_DeInit(hadc->DMA_Handle);
				}
		}
		if(hadc->Instance == ADC3){
		//1.Reset peripherals
		__HAL_RCC_ADC3_FORCE_RESET();
		__HAL_RCC_ADC3_RELEASE_RESET();
//2.Reset GPIO		
		HAL_GPIO_DeInit(GPIOF, GPIO_PIN_9);
//3.Reset De-Initialize DMA		
			  if(hadc->DMA_Handle != NULL){
						HAL_DMA_DeInit(hadc->DMA_Handle);
				}
//		HAL_NVIC_DisableIRQ(ADC3_IRQn);
		}
}

//TIM MSP initialization
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM3){
		/* TIM peripheral clock enable */
			__HAL_RCC_TIM3_CLK_ENABLE();
		}
	if(htim->Instance == TIM2){
			__HAL_RCC_TIM2_CLK_ENABLE();            //ʹ��TIM2ʱ��
			HAL_NVIC_SetPriority(TIM2_IRQn,1,0);    //�����ж����ȼ�����ռ���ȼ�1�������ȼ�3
			HAL_NVIC_EnableIRQ(TIM2_IRQn);          //����ITM2�ж�   
	}
}
//TIM MSP Deinitialization
void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef *htim){
		if(htim->Instance == TIM3){
			__HAL_RCC_TIM3_FORCE_RESET();
			__HAL_RCC_TIM3_RELEASE_RESET();
			}
		if(htim->Instance == TIM2){
			__HAL_RCC_TIM2_FORCE_RESET();
			__HAL_RCC_TIM2_RELEASE_RESET();
		}
}
/**
  * @brief 	���Ź������ص�����
  * @param  hadc: ADC handle
  * @retval None
  */
void HAL_ADC_LevelOutOfWindowCallback(ADC_HandleTypeDef* hadc)
{
u16 i;
  /* Set variable to report analog watchdog out of window status to main      */
  /* program.                                                                 */
	/* ��ȡDMAʣ��Ҫ�������Ŀ */
	g_usTrigCount = DMA2_Stream1->NDTR;
	/* ȷ���Ƿ���ADC3�Ŀ��Ź��ж� */
	if(hadc->Instance == ADC3){
		/* ȡ����ֵ��ǰһ���㣬�鿴�Ƿ����ڷ�ֵ��Χ�ڣ��Ӷ��ж��������ػ����½��� */
		if(g_usTrigCount == 10240){
			/* ʣ��10240��ʾ����ֵ��ADC3ConvertedValue[10239]�Ǵ���ֵ����ô��һ�������10238 */
			g_usTrigTempFlag = ADC3ConvertedValue[10238]; 
		}
		else if(g_usTrigCount == 10239){
			/* ʣ��10239��ʾ����ֵ��ADC3ConvertedValue[0]�Ǵ���ֵ����ô��һ�������10239 */
			g_usTrigTempFlag = ADC3ConvertedValue[10239]; 
		}
		else{
			/* ���벻���������������ôΪ10240-1������ֵ��-1����һ����- g_usTrigCount */
			g_usTrigTempFlag = ADC3ConvertedValue[10238 - g_usTrigCount]; 
		}
			/* �ж��Ƿ��������أ��ǵĻ�������ʱ����¼ADC���� */
		if(g_usTrigTempFlag <= g_TrigVol->usTrigValue)
		{
			for (i= 0;i < 50;i++)
			{
			temp[i] = ADC3ConvertedValue[10238 - g_usTrigCount - 50+ i];
			}
			for (i= 0;i < 50;i++)
			{
			temp[i+50] = ADC3ConvertedValue[10238 - g_usTrigCount + i];
			}
			/* ������ʱ������ */
					i++;
			/* �ر�ADC3�Ŀ��Ź��ж� */
////		AnalogWDGConfig_1.ITMode 										= DISABLE;
////		HAL_ADC_AnalogWDGConfig(&ADC3_Handler, &AnalogWDGConfig_1);	
////			ADC3->IER &= 0xFFFFFF7F;   //AWD1ENΪ����
//			HAL_ADC_Stop(&ADC3_Handler);
			HAL_NVIC_DisableIRQ(ADC3_IRQn);
//			ADC3->IER &= 0xFFFFFF7F;   //AWD1ENΪ����
//			HAL_ADC_Start(&ADC3_Handler);	
			TriggerFlag = 1;
			/* ������ʱ������ */
			HAL_TIM_Base_Start_IT(&TIM2_Handler);
		}
	}
}
//��ʱ��2�жϷ���������
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance == TIM2){
		uint16_t i, k, usCurPos;
		/* ��һ�ν���TIM8�ж�ִ���˳���������Ϊ��ʱ�������ж��и�Сbug��
			 һ����ʼ���˶�ʱ������ʹ���жϾͻ�����������һ���жϵ������Ϊ�˷�ֹ���������
			 ���֣������²�����
		*/
//		if(g_usFirstTimeIRQFlag == 0){
//			g_usFirstTimeIRQFlag = 1;
//			/* ����ǵ�һ�ν�����˳� */
//			return;
//		}
		/* �ȹرն�ʱ�� */
		HAL_TIM_Base_Stop_IT(&TIM2_Handler);
				/* ȷ�������Ķ�ʱ����ʱ�ɼ�ADC���� */
		if(TriggerFlag == 1){
			/* ��ʱ���ɼ�ADC���ݽ��� */
			TriggerFlag = 2;
			/* ������ʱ������ǰDMA�������ݵ�λ�� */
			usCurPos = 10240 - g_usTrigCount;
			/*
				�����Ϊ���������ȡ6K������(��ͨ������ͨ������ǰ��1024��ADC����)��
			     1. ��ǰ��λ�� < 1024.
			     2. 1024 <= ��ǰ��λ�� <= 10240 - 1024(9216)
				 3. ��ǰλ�� > 10240 - 1024(9216)
			*/
		/* ��һ���������ǰ��λ�� < 1024. */
		if(usCurPos < 1024){
				k = 1024 - usCurPos;
				usCurPos = 10240 - k;
				
				/* ǰ�������� */
				for(i = 0; i < k; i++){
					g_DSO1->usWaveBufTemp[i] = ADC3ConvertedValue[i+usCurPos];
				}
				
				usCurPos = i;
				k = 10240 - g_usTrigCount + 1024;
				
				/* �󲿷����� */
				for(i = 0; i < k; i++){
					g_DSO1->usWaveBufTemp[i + usCurPos] = ADC3ConvertedValue[i];
				}
			}
			/* �������������ǰλ�� > 10240 - 1024(9216) */
			else if(usCurPos > 9216){
				usCurPos = usCurPos - 1024;
				
				/* �ɼ�ǰ1024+g_usTrigCount������ */
				for(i = 0; i < (g_usTrigCount + 1024); i++){
					g_DSO1->usWaveBufTemp[i] = ADC3ConvertedValue[i+usCurPos];
				}
				
				k = i;
				usCurPos = (1024 - g_usTrigCount);
				
				/* ʣ�����ݵĲɼ� */
				for(i = 0; i < usCurPos; i++){
					g_DSO1->usWaveBufTemp[i + k] = ADC3ConvertedValue[i]; 
				}
			}
			/* �ڶ��������1024 <= ��ǰ��λ�� <= 10240 - 1024(9216)  */
			else{
				usCurPos = usCurPos - 1024;
				for(i = 0; i < 2048; i++){
					g_DSO1->usWaveBufTemp[i] = ADC3ConvertedValue[i+usCurPos];
				}
			}	
			

		}
  }		
}

