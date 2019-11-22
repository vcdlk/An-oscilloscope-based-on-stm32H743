// 其实MSP函数应该写在STM32h7xx_hal_msp.c中

#include "adc.h"
#include "delay.h"
	
ADC_HandleTypeDef 					ADC1_Handler;//ADC1句柄
ADC_HandleTypeDef 					ADC2_Handler;//ADC2句柄
ADC_HandleTypeDef 					ADC3_Handler;//ADC3句柄
TIM_HandleTypeDef       		TimHandle;//配置定时器
TIM_HandleTypeDef 					TIM2_Handler;      //定时器句柄
TIM_MasterConfigTypeDef     master_timer_config;
ADC_AnalogWDGConfTypeDef   	AnalogWDGConfig_1;
DMA_HandleTypeDef       			   DmaHandle1;
DMA_HandleTypeDef       			   DmaHandle2;
/*
*********************************************************************************************************
*	                           ADC1，ADC3的数据缓存，大小均为10240
*********************************************************************************************************
*/
uint16_t ADC1ConvertedValue[10240];
uint16_t ADC3ConvertedValue[10240];
uint16_t temp[100];
/*
*********************************************************************************************************
*	                                   变量和函数                           
*********************************************************************************************************
*/
static uint16_t g_usFirstTimeIRQFlag;  /* 第一次进入定时器8的中断标志 */
static uint16_t g_usTrigCount = 0;     /* 记录普通触发进入看门狗中断时，DMA传输的位置 */
static uint16_t g_usTrigTempFlag = 0;  /* 临时变量，用于ADC中断 */
/**********************初始化ADC******************/
void MY_ADC_Init(void)
{ 
//ADC_DMA_Close();
	//第一步配置ADC、ADC通道、看门狗
		ADC1_Config();			//ADC1---DSO2---PA6---INP19
		ADC2_Config();
		ADC3_Config();			//ADC3---DSO1---PH4---INP15
		//校准ADC
		HAL_ADCEx_Calibration_Start(&ADC1_Handler,ADC_CALIB_OFFSET,ADC_SINGLE_ENDED); //ADC校准
		HAL_ADCEx_Calibration_Start(&ADC3_Handler,ADC_CALIB_OFFSET,ADC_SINGLE_ENDED); //ADC校准
  //定时器3  配置
		TIM_Config();
		TIM2_MeasureTrigConfig();
	//定时器开始
		HAL_TIM_Base_Start(&TimHandle);
//		HAL_TIM_Base_Start(&TIM2_Handler);
	//连接DMA
		HAL_ADC_Start_DMA(&ADC1_Handler,
											(uint32_t *)ADC1ConvertedValue,
											1024*10
											);
		HAL_ADC_Start_DMA(&ADC3_Handler,
											(uint32_t *)ADC3ConvertedValue,
											1024*10
											);
}
void ADC1_Config(void){//配置ADC、ADC通道、看门狗
		ADC_ChannelConfTypeDef  				           ADC1_ChanConf;
		//ADC1配置
    ADC1_Handler.Instance											 =ADC1;
    ADC1_Handler.Init.ClockPrescaler   				 =ADC_CLOCK_SYNC_PCLK_DIV4; 	//4分频，ADCCLK=PER_CK/4=64/4=16MHZ
    ADC1_Handler.Init.Resolution     					 =ADC_RESOLUTION_12B;  //16位模式
    ADC1_Handler.Init.ScanConvMode						 =ADC_SCAN_DISABLE;   //非扫描模式
    ADC1_Handler.Init.EOCSelection					   =ADC_EOC_SINGLE_CONV; //关闭EOC中断
		ADC1_Handler.Init.LowPowerAutoWait				 =DISABLE;					//自动低功耗关闭				
    ADC1_Handler.Init.ContinuousConvMode			 =DISABLE;          //关闭连续转换
    ADC1_Handler.Init.NbrOfConversion					 =1;              //1个转换在规则序列中 也就是只转换规则序列1 
    ADC1_Handler.Init.DiscontinuousConvMode		 =DISABLE;            //禁止不连续采样模式
    ADC1_Handler.Init.NbrOfDiscConversion      =1;                    //不连续采样通道数为0
    ADC1_Handler.Init.ExternalTrigConv         =ADC_EXTERNALTRIG_T3_TRGO;      //T3触发
    ADC1_Handler.Init.ExternalTrigConvEdge     =ADC_EXTERNALTRIGCONVEDGE_RISING;//上升沿
	
	  ADC1_Handler.Init.ConversionDataManagement =ADC_CONVERSIONDATA_DMA_CIRCULAR;  //规则通道的数据仅仅保存在DR寄存器里面
		ADC1_Handler.Init.BoostMode    						 =ENABLE;							//BOOT模式关闭
		ADC1_Handler.Init.Overrun									 =ADC_OVR_DATA_OVERWRITTEN;			//有新的数据的死后直接覆盖掉旧数据
		ADC1_Handler.Init.OversamplingMode			   =DISABLE;					//过采样关闭
    HAL_ADC_Init(&ADC1_Handler);                                 //初始化 
	
	//配置通道
	  ADC1_ChanConf.Channel                     =	ADC_CHANNEL_19;  //通道
    ADC1_ChanConf.Rank               					=	ADC_REGULAR_RANK_1;        //1个序列
    ADC1_ChanConf.SamplingTime								=	ADC_SAMPLETIME_8CYCLES_5;  //采样时间       
		ADC1_ChanConf.SingleDiff									=	ADC_SINGLE_ENDED;  				//单边采集          		
		ADC1_ChanConf.OffsetNumber								=	ADC_OFFSET_NONE;             	
		ADC1_ChanConf.Offset											=	0;   
    HAL_ADC_ConfigChannel(&ADC1_Handler,&ADC1_ChanConf);        //ADC的通道19 
//	//看门狗配置
//		AnalogWDGConfig.WatchdogNumber            = ADC_ANALOGWATCHDOG_2;
//		AnalogWDGConfig.WatchdogMode 							= ADC_ANALOGWATCHDOG_ALL_REG;
//		AnalogWDGConfig.Channel 									= ADC_CHANNEL_19;
//		AnalogWDGConfig.ITMode 										= ENABLE;
//		AnalogWDGConfig.HighThreshold 						= 2000;
//		AnalogWDGConfig.LowThreshold						  = 0;
//		HAL_ADC_AnalogWDGConfig(&ADC1_Handler, &AnalogWDGConfig);
}
void ADC3_Config(void){//配置ADC3、ADC3通道
		ADC_ChannelConfTypeDef  				 							ADC3_ChanConf;
		//ADC3配置	
		ADC3_Handler.Instance 										 = 	ADC3;
		ADC3_Handler.Init.ClockPrescaler					 = 	ADC_CLOCKPRESCALER_PCLK_DIV4;
		ADC3_Handler.Init.Resolution							 =	ADC_RESOLUTION_12B;
    ADC3_Handler.Init.ScanConvMode						 =	ADC_SCAN_DISABLE;   //非扫描模式
    ADC3_Handler.Init.EOCSelection					   =	ADC_EOC_SINGLE_CONV; //关闭EOC中断
		ADC3_Handler.Init.LowPowerAutoWait				 =	DISABLE;					//自动低功耗关闭				
    ADC3_Handler.Init.ContinuousConvMode			 =	DISABLE;          //关闭连续转换
    ADC3_Handler.Init.NbrOfConversion					 =	1;              //1个转换在规则序列中 也就是只转换规则序列1 
    ADC3_Handler.Init.DiscontinuousConvMode		 =	DISABLE;            //禁止不连续采样模式
    ADC3_Handler.Init.NbrOfDiscConversion      =	1;                    //不连续采样通道数为0
    ADC3_Handler.Init.ExternalTrigConv         =	ADC_EXTERNALTRIG_T3_TRGO;      //T3触发
    ADC3_Handler.Init.ExternalTrigConvEdge     =	ADC_EXTERNALTRIGCONVEDGE_RISING;//上升沿

	  ADC3_Handler.Init.ConversionDataManagement =	ADC_CONVERSIONDATA_DMA_CIRCULAR;  //规则通道的数据仅仅保存在DR寄存器里面
		ADC3_Handler.Init.BoostMode    						 =	ENABLE;							//BOOT模式关闭
		ADC3_Handler.Init.Overrun									 =	ADC_OVR_DATA_OVERWRITTEN;			//有新的数据的死后直接覆盖掉旧数据
		ADC3_Handler.Init.OversamplingMode			   =	DISABLE;					//过采样关闭
    HAL_ADC_Init(&ADC3_Handler);                                 //初始化 	
		//配置通道PH4 ADC3_INP15
	  ADC3_ChanConf.Channel                     =	ADC_CHANNEL_2;  //通道
    ADC3_ChanConf.Rank               					=	ADC_REGULAR_RANK_1;        //1个序列
    ADC3_ChanConf.SamplingTime								=	ADC_SAMPLETIME_8CYCLES_5;  //采样时间       
		ADC3_ChanConf.SingleDiff									=	ADC_SINGLE_ENDED;  				//单边采集          		
		ADC3_ChanConf.OffsetNumber								=	ADC_OFFSET_NONE;             	
		ADC3_ChanConf.Offset											=	0;   
    HAL_ADC_ConfigChannel(&ADC3_Handler,&ADC3_ChanConf);        //ADC的通道 15
	//看门狗配置
		AnalogWDGConfig_1.WatchdogNumber            = ADC_ANALOGWATCHDOG_1;
		AnalogWDGConfig_1.WatchdogMode 							= ADC_ANALOGWATCHDOG_ALL_REG;
		AnalogWDGConfig_1.Channel 									= ADC_CHANNEL_2;
		AnalogWDGConfig_1.ITMode 										= ENABLE;
		AnalogWDGConfig_1.HighThreshold 						= 4095;
		AnalogWDGConfig_1.LowThreshold						  = 0;
		HAL_ADC_AnalogWDGConfig(&ADC3_Handler, &AnalogWDGConfig_1);
}
//配置ADC2
void ADC2_Config(){
		ADC2_Handler.Instance                     				= ADC2;
    ADC2_Handler.Init.ClockPrescaler									=ADC_CLOCK_SYNC_PCLK_DIV4; 	//4分频，ADCCLK=PER_CK/4=64/4=16MHZ
    ADC2_Handler.Init.Resolution											=ADC_RESOLUTION_12B;           	//16位模式
    ADC2_Handler.Init.ScanConvMode										=DISABLE;                    	//非扫描模式
    ADC2_Handler.Init.EOCSelection										=ADC_EOC_SINGLE_CONV;       	//关闭EOC中断
		ADC2_Handler.Init.LowPowerAutoWait								=	DISABLE;					//自动低功耗关闭				
    ADC2_Handler.Init.ContinuousConvMode							=ENABLE;               //连续转换
    ADC2_Handler.Init.NbrOfConversion									=1;                        //1个转换在规则序列中 也就是只转换规则序列1 
    ADC2_Handler.Init.DiscontinuousConvMode						=DISABLE;            //禁止不连续采样模式
    ADC2_Handler.Init.NbrOfDiscConversion							=0;                    //不连续采样通道数为0
    ADC2_Handler.Init.ExternalTrigConv								=ADC_SOFTWARE_START;      //软件触发
    ADC2_Handler.Init.ExternalTrigConvEdge						=ADC_EXTERNALTRIGCONVEDGE_NONE;//使用软件触发
		ADC2_Handler.Init.BoostMode												=ENABLE;							//BOOT模式关闭
		ADC2_Handler.Init.Overrun													=ADC_OVR_DATA_OVERWRITTEN;			//有新的数据的死后直接覆盖掉旧数据
		ADC2_Handler.Init.OversamplingMode								=DISABLE;					//过采样关闭
		ADC2_Handler.Init.ConversionDataManagement				=ADC_CONVERSIONDATA_DR;  //规则通道的数据仅仅保存在DR寄存器里面
    HAL_ADC_Init(&ADC2_Handler);                                 //初始化 
		
		ADC_ChannelConfTypeDef ADC2_ChanConf;
    
    ADC2_ChanConf.Channel=ADC_CHANNEL_3;                                   //通道
    ADC2_ChanConf.Rank=ADC_REGULAR_RANK_1;                  	//1个序列
    ADC2_ChanConf.SamplingTime=ADC_SAMPLETIME_64CYCLES_5;      	//采样时间       
		ADC2_ChanConf.SingleDiff=ADC_SINGLE_ENDED;  				//单边采集          		
		ADC2_ChanConf.OffsetNumber=ADC_OFFSET_NONE;             	
		ADC2_ChanConf.Offset=0;   
    HAL_ADC_ConfigChannel(&ADC2_Handler,&ADC2_ChanConf);        //通道配置

    HAL_ADC_Start(&ADC2_Handler);                               //开启ADC
	
	
		HAL_ADCEx_Calibration_Start(&ADC2_Handler,ADC_CALIB_OFFSET,ADC_SINGLE_ENDED); //ADC校准		
}
static void TIM_Config(){//配置定时器
	//配置定时器3的参数
	TimHandle.Instance 							  					= TIM3;
	TimHandle.Init.Period       		  					= 400000000/g_SampleFreqTable[TimeBaseId][0] - 1;    //ARR自动重装载寄存器周期的值(定时时间）到设置频率后产生个更新或者中断(也是说定时时间到)
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
*	函 数 名: TIM8_MeasureTrigConfig
*	功能说明: 使用TIM8为普通触发模式下数据采集计时，定时采集触发值前后的1024个ADC数据
*	形    参：无
*	返 回 值: 无		        
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

	/* 下面5种采样频率下刷新较慢，因为采集前后1024个ADC的时间较长 */
	{42000,    4096},    //2Ksps 168000000/2000  = 84000 => 84000 * 1024
	{50000,    4096},    //1Ksps 168000000/1000  = 168000 => 168000 * 1024 
	{50000,    8192},    //500sps 168000000/500  = 336000 => 336000 * 1024 
	{42000,    20480},   //200sps 168000000/200  = 840000 => 840000 * 1024 
	{62500,    32758},   //100sps 168000000/100  = 1680000 => 1680000 * 1024
	
	/* 下面这几种采样率不做触发支持 */
	{42000,    40960},   //50sps 
	{42000,    40960},   //20sps
	{42000,    40960},   //10sps 
	{42000,    40960},   //5sps 
	{42000,    40960},   //2sps 
	{42000,    40960},   //1sps 	
};
/* 
	每次捕获采集触发值前后的1024个ADC数据(单通道)。
*/
void TIM2_MeasureTrigConfig(){
		TimeBaseId 											= 1;        /* 开机后按照ADC单通道1Msps进行配置 */
	/* 加上第一次进入中断的标志，进入中断后将其置1 */
		g_usFirstTimeIRQFlag 						= 0;
    TIM2_Handler.Instance						=	TIM2;           //通用定时器2
	  TIM2_Handler.Init.Period				=	g_TrigFreqTable[TimeBaseId][0] - 1;   //自动装载值
    TIM2_Handler.Init.Prescaler			=	g_TrigFreqTable[TimeBaseId][1] - 1;   //分频
    TIM2_Handler.Init.CounterMode		=	TIM_COUNTERMODE_UP;    //向上计数器
    TIM2_Handler.Init.ClockDivision	=	TIM_CLOCKDIVISION_DIV1;//时钟分频因子
    HAL_TIM_Base_Init(&TIM2_Handler);  //使能定时器2和定时器2更新中断：TIM_IT_UPDATE    
}
/*
*********************************************************************************************************
*	函 数 名: Time2Recorder
*	功能说明: 使用TIM2为普通触发模式下数据采集计时，定时采集触发值前后的1024个ADC数据
*	形    参：无
*	返 回 值: 无		        
*********************************************************************************************************
*/
static void Time2Recorder(void){
	/* 加上第一次进入中断的标志 */
	g_usFirstTimeIRQFlag = 0;
	TIM2_Handler.Instance						=	TIM2;           //通用定时器2
	TIM2_Handler.Init.Period				=	g_TrigFreqTable[TimeBaseId][0] - 1;   //自动装载值
  TIM2_Handler.Init.Prescaler			=	g_TrigFreqTable[TimeBaseId][1] - 1;   //分频
  TIM2_Handler.Init.CounterMode		=	TIM_COUNTERMODE_UP;    //向上计数器
  TIM2_Handler.Init.ClockDivision	=	TIM_CLOCKDIVISION_DIV1;//时钟分频因子
  HAL_TIM_Base_Init(&TIM2_Handler);  //使能定时器2和定时器2更新中断：TIM_IT_UPDATE    
}
/*************************************
HAL 库中没有直接配置看门狗上下端电压的函数
@param  hadc 是ADC句柄
				AnalogWDGConfig 是配置看门狗的句柄，应该在ADC_Config()中调用过
				HighThreshold 看门狗电压高端
				LowThreshold  看门狗电压低端
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
关闭ADC123和其对应的DMA传输
@param void
***************************************/
void ADC_DMA_Close(void){
		HAL_ADC_DeInit(&ADC1_Handler);
		HAL_ADC_DeInit(&ADC3_Handler);
		HAL_TIM_Base_DeInit(&TimHandle);
		HAL_TIM_Base_DeInit(&TIM2_Handler);
}
/***************************************
打开ADC123和其对应的DMA传输
@param void
***************************************/
void ADC_DMA_Open(void){
	//第一步配置ADC、ADC通道、看门狗
		ADC1_Config();			//ADC1---DSO2---PA6---INP19
		ADC3_Config();			//ADC3---DSO1---PH4---INP15	
  //定时器3  配置
		TIM_Config();
	//连接DMA
		HAL_ADC_Start_DMA(&ADC1_Handler,
											(uint32_t *)ADC1ConvertedValue,
											1024*10
											);
		HAL_ADC_Start_DMA(&ADC3_Handler,
											(uint32_t *)ADC3ConvertedValue,
											1024*10
											);
		//定时器3开始
		HAL_TIM_Base_Start(&TimHandle);
	/* 只有在普通触发模式在启动TIM8的计时功能 */
	if(TriggerFlag != 0)
	{
			Time2Recorder();
//			HAL_TIM_Base_Start(&TIM2_Handler);
	}


}
//ADC底层驱动，引脚配置，时钟使能
//此函数会被HAL_ADC_Init()调用
//hadc:ADC句柄
void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
		GPIO_InitTypeDef 								 GPIO_Initure;
	if(hadc->Instance  == ADC1 ){	
		//*##- 1- Configure CLK###########/
		__HAL_RCC_GPIOA_CLK_ENABLE();						//开启GPIOA时钟
		__HAL_RCC_ADC12_CLK_ENABLE();           //使能ADC1/2时钟
		__HAL_RCC_ADC_CONFIG(RCC_ADCCLKSOURCE_CLKP); //ADC外设时钟选择
		__HAL_RCC_DMA1_CLK_ENABLE();
		/*##- 2- Configure peripheral GPIO ############*/
		/* ADC1 Channel GPIO pin configuration */
		GPIO_Initure.Pin=GPIO_PIN_5;            //PA5
		GPIO_Initure.Mode=GPIO_MODE_ANALOG;     //模拟
		GPIO_Initure.Pull=GPIO_NOPULL;          //不带上下拉
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
		/* Associate the DMA handle 把结构体中的两个连到一起*/
		__HAL_LINKDMA(hadc, DMA_Handle, DmaHandle1);	
	}
	if(hadc->Instance  == ADC3 ){	
		//*##- 1- Configure CLK###########/
		__HAL_RCC_GPIOF_CLK_ENABLE();									//开启GPIOH时钟
		__HAL_RCC_ADC3_CLK_ENABLE(); 									//使能ADC3时钟
		__HAL_RCC_ADC_CONFIG(RCC_ADCCLKSOURCE_CLKP);  //ADC外设时钟选择
		__HAL_RCC_DMA2_CLK_ENABLE();
		/*##- 2- Configure peripheral GPIO ############*/
		/* ADC3 Channel GPIO pin configuration */
		GPIO_Initure.Pin=GPIO_PIN_9;            //PH4
		GPIO_Initure.Mode=GPIO_MODE_ANALOG;     //模拟
		GPIO_Initure.Pull=GPIO_NOPULL;          //不带上下拉
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
		/* Associate the DMA handle 把结构体中的两个连到一起*/
		__HAL_LINKDMA(hadc, DMA_Handle, DmaHandle2);
	
		HAL_NVIC_SetPriority(ADC3_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(ADC3_IRQn);
	}
	if(hadc->Instance == ADC2){
		__HAL_RCC_GPIOA_CLK_ENABLE();									//开启GPIOH时钟
		__HAL_RCC_ADC12_CLK_ENABLE(); 									//使能ADC3时钟
		__HAL_RCC_ADC_CONFIG(RCC_ADCCLKSOURCE_CLKP);  //ADC外设时钟选择
		/*##- 2- Configure peripheral GPIO ############*/
		/* ADC3 Channel GPIO pin configuration */
		GPIO_Initure.Pin=GPIO_PIN_6;            //PH4
		GPIO_Initure.Mode=GPIO_MODE_ANALOG;     //模拟
		GPIO_Initure.Pull=GPIO_NOPULL;          //不带上下拉
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);
	}
}
/********************ADC底层驱动
//此函数会被HAL_ADC_DeInit()调用
//hadc:ADC句柄**************/
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
			__HAL_RCC_TIM2_CLK_ENABLE();            //使能TIM2时钟
			HAL_NVIC_SetPriority(TIM2_IRQn,1,0);    //设置中断优先级，抢占优先级1，子优先级3
			HAL_NVIC_EnableIRQ(TIM2_IRQn);          //开启ITM2中断   
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
  * @brief 	看门狗触发回调函数
  * @param  hadc: ADC handle
  * @retval None
  */
void HAL_ADC_LevelOutOfWindowCallback(ADC_HandleTypeDef* hadc)
{
u16 i;
  /* Set variable to report analog watchdog out of window status to main      */
  /* program.                                                                 */
	/* 读取DMA剩余要传输的数目 */
	g_usTrigCount = DMA2_Stream1->NDTR;
	/* 确认是否是ADC3的看门狗中断 */
	if(hadc->Instance == ADC3){
		/* 取触发值的前一个点，查看是否是在阀值范围内，从而判断是上升沿还是下降沿 */
		if(g_usTrigCount == 10240){
			/* 剩余10240表示触发值是ADC3ConvertedValue[10239]是触发值，那么上一个点就是10238 */
			g_usTrigTempFlag = ADC3ConvertedValue[10238]; 
		}
		else if(g_usTrigCount == 10239){
			/* 剩余10239表示触发值是ADC3ConvertedValue[0]是触发值，那么上一个点就是10239 */
			g_usTrigTempFlag = ADC3ConvertedValue[10239]; 
		}
		else{
			/* 加入不上面两种情况，那么为10240-1（触发值）-1（上一个）- g_usTrigCount */
			g_usTrigTempFlag = ADC3ConvertedValue[10238 - g_usTrigCount]; 
		}
			/* 判断是否是上升沿，是的话开启定时器记录ADC数据 */
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
			/* 启动定时器计数 */
					i++;
			/* 关闭ADC3的看门狗中断 */
////		AnalogWDGConfig_1.ITMode 										= DISABLE;
////		HAL_ADC_AnalogWDGConfig(&ADC3_Handler, &AnalogWDGConfig_1);	
////			ADC3->IER &= 0xFFFFFF7F;   //AWD1EN为置零
//			HAL_ADC_Stop(&ADC3_Handler);
			HAL_NVIC_DisableIRQ(ADC3_IRQn);
//			ADC3->IER &= 0xFFFFFF7F;   //AWD1EN为置零
//			HAL_ADC_Start(&ADC3_Handler);	
			TriggerFlag = 1;
			/* 启动定时器计数 */
			HAL_TIM_Base_Start_IT(&TIM2_Handler);
		}
	}
}
//定时器2中断服务函数调用
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance == TIM2){
		uint16_t i, k, usCurPos;
		/* 第一次进入TIM8中断执行退出操作，因为定时器更新中断有个小bug，
			 一旦初始化了定时器并且使能中断就会有立即进入一次中断的情况，为了防止这种情况的
			 出现，作如下操作：
		*/
//		if(g_usFirstTimeIRQFlag == 0){
//			g_usFirstTimeIRQFlag = 1;
//			/* 如果是第一次进入就退出 */
//			return;
//		}
		/* 先关闭定时器 */
		HAL_TIM_Base_Stop_IT(&TIM2_Handler);
				/* 确保开启的定时器计时采集ADC数据 */
		if(TriggerFlag == 1){
			/* 定时器采集ADC数据结束 */
			TriggerFlag = 2;
			/* 开启定时器计数前DMA传输数据的位置 */
			usCurPos = 10240 - g_usTrigCount;
			/*
				下面分为三种情况获取6K的数据(三通道，单通道就是前后1024个ADC数据)：
			     1. 当前的位置 < 1024.
			     2. 1024 <= 当前的位置 <= 10240 - 1024(9216)
				 3. 当前位置 > 10240 - 1024(9216)
			*/
		/* 第一种情况：当前的位置 < 1024. */
		if(usCurPos < 1024){
				k = 1024 - usCurPos;
				usCurPos = 10240 - k;
				
				/* 前部分数据 */
				for(i = 0; i < k; i++){
					g_DSO1->usWaveBufTemp[i] = ADC3ConvertedValue[i+usCurPos];
				}
				
				usCurPos = i;
				k = 10240 - g_usTrigCount + 1024;
				
				/* 后部分数据 */
				for(i = 0; i < k; i++){
					g_DSO1->usWaveBufTemp[i + usCurPos] = ADC3ConvertedValue[i];
				}
			}
			/* 第三种情况：当前位置 > 10240 - 1024(9216) */
			else if(usCurPos > 9216){
				usCurPos = usCurPos - 1024;
				
				/* 采集前1024+g_usTrigCount的数据 */
				for(i = 0; i < (g_usTrigCount + 1024); i++){
					g_DSO1->usWaveBufTemp[i] = ADC3ConvertedValue[i+usCurPos];
				}
				
				k = i;
				usCurPos = (1024 - g_usTrigCount);
				
				/* 剩余数据的采集 */
				for(i = 0; i < usCurPos; i++){
					g_DSO1->usWaveBufTemp[i + k] = ADC3ConvertedValue[i]; 
				}
			}
			/* 第二种情况：1024 <= 当前的位置 <= 10240 - 1024(9216)  */
			else{
				usCurPos = usCurPos - 1024;
				for(i = 0; i < 2048; i++){
					g_DSO1->usWaveBufTemp[i] = ADC3ConvertedValue[i+usCurPos];
				}
			}	
			

		}
  }		
}

