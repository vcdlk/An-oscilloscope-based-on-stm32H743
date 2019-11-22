/*
*********************************************************************************************************
*	                                  
*	模块名称 : 二代示波器设计主函数
*	文件名称 : MainTask.c
*	版    本 : V1.0
*	说    明 : 二代示波器主界面设计
*********************************************************************************************************
*/
#include "MainTask.h"
#include "includes.h"
/*
*********************************************************************************************************
理想情况下衰减倍数表，ADC和DAC外部参考是3.3V，这里没有进行校准。
(1) 每个方格对应的是50个点，表示1V 。
(2) 3.3V 对应ADC数值4095  4095/3.3 = 1240.9;
    对应关系就是 50个点阵 == 1V == ADC计数的1240.9
    由此可得1V的衰减因子是4095/3.3/50 = 24.818，由这个关系可推得下面的数组。
*********************************************************************************************************
*/
const uint16_t g_AttTable[ATT_COUNT][2] =
{	
/*  {除数*放大100倍, 每大格电压单位mv}  被除数也得更加相应情况进行放大 这里做了四舍五入 */
  {24818,	10000}, /* 即248.18 * 100 ==  10000mv 跟上面的推导是对应的 */
	{12409,	5000},  /* 即124.09 * 100 ==  5000mv  */
	{4964,	2000},
	{2482,	1000},
	{1241,	500},
	{496,	200},
	{248,	100},
	{124,	50},
	{50,	20},
	{25,	10},
};
/* 10种幅度单位**************************************************************************************/
const char *g_AttText[] =
{
    "10.0V",
	"5.00V",
	"2.00V",
	"1.00V",
	"500mV",
	"200mV",
	"100mV",
	"50.0mV",
	"20.0mV",
	"10.0mV",
};
/* 每秒采集的数据数量，用于采集低速信号TFT显示使用**************************************************/
/* 波形显示的延迟时间设置 */
const uint16_t g_RefreshDelay[] =
{
	150, 
	150,
	150,
	150,
	150,
	
	150,
	150,
	150,
	150,
	150,
	
	150,
	150,
	150,
	150,
	150,
	
	150,
	150,
	150,
	150,
	150,
	150,
};
/* 单通道采样率，2.8Msps到1sps****************************************************************************/
const uint32_t g_SampleFreqTable[][2] =
{
	{2800000,  1},     //2.8Msps
	{2000000,  1},     //2Msps
	{1000000,  1},     //1Msps
	{500000,   1},     //500Ksps
	{200000,   1},     //200Ksps
	
	{100000,   1},     //100Ksps
	{50000,    1},     //50Ksps
	{20000,    1},     //20Ksps
	{10000,    1},     //10Ksps
	{5000,     1},     //5Ksps
	
	{16800,      5},    //2Ksps
	{16800,     10},    //1Ksps
	{16800,     20},    //500sps
	{16800,     50},    //200sps
	{16800,     100},   //100sps
	
	{16800,     200},   //50sps
	{16800,     500},   //20sps
	{16800,     1000},  //10sps
	{16800,     2000},  //5sps
	{16800,     5000},  //2sps
	{16800,     10000}, //1sps
};
/* 用于配置游标测试显示****************************************************************************/
/* 采用5.1f格式显示 ，比如222.2us 依次类推 */
const uint16_t g_CursorUintTable[][2] =
{  
	/********5.1F********************************/
	//除数*1000   数据显示时的格式
	{357,   1},  // 600 * 0.36us = 216.0us -- 0.36us.
	{500,   1},  // 600 * 0.5us  = 300.0us -- 0.5us.
	{1000,  1},  // 600 * 1us    = 600.0us -- 1.0us.
	
	/* 以下的除数均没有放大，但最后的结果需要除以1000 */
	/********5.3F********************************/
	{2,  2},  // 600 * 2us    = 1.200ms -- 0.002ms. 
	{5,  2},  // 600 * 5us    = 3.000ms -- 0.005ms.
	{10, 2},  // 600 * 10us   = 6.000ms -- 0.010ms.
	
	/********5.2F********************************/
	{20,  3},  // 600 * 20us   = 12.00ms -- 0.02ms.
	{50,  3},  // 600 * 50us   = 30.00ms -- 0.05ms.
	{100, 3},  // 600 * 100us  = 60.00ms -- 0.10ms.
	
	/********5.1F********************************/
	{200,  4},  // 600 * 200us  = 120.0ms -- 0.2ms.
	{500,  4},  // 600 * 500us  = 300.0ms -- 0.5ms.
	{1000, 4},  // 600 * 1000us = 600.0ms -- 1.0ms.
	
	/********5.3F********************************/
	{2,  5},  // 600 * 2ms  = 1.200s -- 0.002s.
	{5,  5},  // 600 * 5ms  = 3.000s -- 0.005s.
	{10, 5},  // 600 * 10ms = 6.000ms -- 0.010ms.
	
	/********5.2F********************************/
	{20,  6},  // 600 * 20ms   = 12.00s -- 0.02s.
	{50,  6},  // 600 * 50ms   = 30.00s -- 0.05s.
	{100, 6},  // 600 * 100ms  = 60.00s -- 0.10s.
	
	/********5.1F********************************/
	{200,  7},  // 600 * 200us  = 120.0s -- 0.2s.
	{500,  7},  // 600 * 500us  = 300.0s -- 0.5s.
	{1000, 7},  // 600 * 1000us = 600.0s -- 1.0s.
};
/* 采样率和对应的周期**********************************************************************************/
/* 采样率，从2Msps到1Ksps */
const char *g_TimeTable[] =
{
	"2.8Msps 357ns",
	"2Msps   500ns",
	"1Msps   1us",
	"500Ksps 2us",
	"200Ksps 5us",
	"100Ksps 10us",
	
	"50Ksps  20us",
	"20Ksps  50us",
	"10Ksps  100us",
	"5Ksps   200us",
	"2Ksps   500us",
	
	"1Ksps   1ms",
	"500sps  2ms",
	"200sps  5ms",
	"100sps  10ms",
	"50sps   20ms",

	"20sps   50ms",
	"10sps   100ms",
	"5sps    200ms",
	"2sps    500ms",
	"1sps    1s",
};
/* 表示当前实际支持的测量值******************************************************************************** */
const char *g_MeasureTable[] =
{
	"Snapshot",
	"Freq",
	"Period",
	"RiseTime",
	"FallTime",
	"Delay",
	"Phase",
	"PPW",
	"NPW",
	"PDC",
	
	"NDC",
	"BurWidth",
	"Pk-Pk",
	"Amp",
	"Max",
	"Min",
	"High",
	"Low",
	"PosOver",
	"NegOver",
	
	"Mean",
	"CMean",
	"RMS",
	"CRMS",
	"PPC",
	"NPC",
	"REC",
	"FEC",
	"Area",
	"Value",
};
/*
*********************************************************************************************************
*	231                                  变量
*********************************************************************************************************
*/
WM_HWIN hMotion;           /* 滑动窗口句柄 */

uint16_t TriggerFlag = 0;  /* 0：用于自动触发，1,2：用于普通触发 */
int8_t   TimeBaseId = 1;   /* 采样率时基 */
int8_t   Ch1AmpId = 3;     /* 3:表示默认选择每个方格表示1V的模式 */

FLAG_T       *g_Flag;      /* 汇总各种标志的结构体指针变量 */
TRIVOLTAGE_T *g_TrigVol;   /* 汇总各种触发值的结构体指针变量 */
CURSORS_T    *g_Cursors;   /* 汇总各种测量游标的结构体指针变量 */
DSO_T 	     *g_DSO1;	   /* 汇总示波器通道1的结构体指针变量 */
DSO_T        *g_DSO2;      /* 汇总示波器通道2的结构体指针变量 */

/* FFT输入和输出缓冲 */
float32_t *testInput_fft_2048;
float32_t *testOutput_fft_2048;

/* 声明一个矩形方框 */
GUI_RECT Rect ={DSOSCREEN_STARTX, DSOSCREEN_STARTY, DSOSCREEN_ENDX, DSOSCREEN_ENDY};

WM_HWIN hTimer;
GUI_MEMDEV_Handle   hMemDSO;
/*
*********************************************************************************************************
*	                                 用于绘制多边形的数组
*********************************************************************************************************
*/
/* 波形显示区左侧边上波形显示的参考位置 */
const GUI_POINT aPoints[5] = 
{
	{  0, -10*1.25},
	{ 22*1.28, -10*1.25},
	{ 32*1.28,   0*1.25},
	{ 22*1.28,  10*1.25},
	{ 0,   10*1.25}
};
/* 波形显示区右侧边上触发值箭头 */
const GUI_POINT aPointsTrig[7] = 
{
	{ 0*1.28,  0*1.25},
	{ 6*1.28, 	-6*1.25},
	{ 6*1.28, 	-1*1.25},
	{ 12*1.28, -1*1.25},
	{ 12*1.28, 1*1.25},
	{ 6*1.28,  1*1.25},
	{ 6*1.28,  6*1.25},
};
/* 2k数据中，波形显示的起始位置 */
const GUI_POINT aPointsTrigBrowser[3] = 
{   
  { -6*1.28, 0},
	{  6*1.28, 0},
	{  0*1.28, 6*1.25}
};
/*
*********************************************************************************************************
*	                                 GUI_RECT类型数组
*********************************************************************************************************
*/
GUI_RECT rClient    = {0			 , 0			 , 75*1.25 , 38*1.25 };     /* 用于显示示波器的logo */
GUI_RECT rRunMode   = {76*1.28 , 0       , 140*1.28, 38*1.25};     /* 用于显示运行状态，运行和暂停 */
GUI_RECT rTrigMode  = {141*1.28, 0			 , 205*1.28, 38*1.25};     /* 用于显示触发模式，自动触发和普通触发 */
GUI_RECT rTrigValue = {471*1.28, 0			 , 640*1.28, 38*1.25};     /* 用于显示自动和正常的触发数组 */
GUI_RECT rButState  = {621*1.28, 0			 , 799*1.28, 38*1.25};     /* 当前按键需要调节的状态 */
GUI_RECT rRefPos    = {0*1.28  , 39*1.25 , 39*1.28 , 439*1.25};     /* 示波器最左侧波形参考的位置区域 */
GUI_RECT rTrigPos   = {210*1.28, 6*1.25  , 470*1.28, 33*1.25};     /* 6KB数据查询：自动触发，普通触发 */
/*
*********************************************************************************************************
*	函 数 名: _cbBkWin
*	功能说明: 桌面窗口的回调函数 
*	形    参: pMsg
*	返 回 值: 无
*********************************************************************************************************
*/
static void _cbBkWin(WM_MESSAGE *pMsg){
	int NCode ,ID1;
	switch(pMsg->MsgId){
	/* 桌面背景重绘*/
	case WM_PAINT:
		DSO_Init(0);
		break;	
			/* 通过设置触发标志来更新一次触发游标的显示 */
	case WM_TIMER:
			g_Flag->ucWaveRefresh = 1;
			//WM_RestartTimer(pMsg->Data.v, 1000);		
			break;
	case WM_NOTIFY_PARENT:
			ID1 = WM_GetId(pMsg->hWinSrc); 
			NCode = pMsg->Data.v;
			switch (ID1){
							 /* 按钮0：打开Measure对话框 */
				 case GUI_ID_BUTTON0:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_CLICKED:
                            DSO_CreateMeasureDlg();
                            break;
                    }
					break;
							 /* 按钮1：打开ADC对话框 */
				 case GUI_ID_BUTTON1:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_CLICKED:
															DSO_CreateAdcDlg();
                            break;
                    }
					break;
							/* 按钮2：打开DAC对话框 */
				 case GUI_ID_BUTTON2:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_RELEASED:
	//													 DSO_CreateDacDlg();
                            break;
                    }
					break;
									/* 按钮3：打开Math对话框 */
				 case GUI_ID_BUTTON3:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_RELEASED:
                            DSO_CreateMathDlg();
                            break;
                    }
					break;
					
				 /* 按钮4：打开Settings对话框 */
				 case GUI_ID_BUTTON4:
                    switch(NCode)
                    {
                        case WM_NOTIFICATION_CLICKED:
														 DSO_CreateSettingsDlg();
                            break;
                    }
					break;
			}
			break;
			default:
			WM_DefaultProc(pMsg);
	}
}
/*
*********************************************************************************************************
*	函 数 名: _DrawWave
*	功能说明: 绘制波形和波形的背景显示
*	形    参: 无         	
*	返 回 值: 无
*********************************************************************************************************
*/
static void _DrawWave(void) 
{
	uint16_t i;
	uint32_t temp;
	GUI_RECT Rect = {40*1.28, 40*1.25, 640*1.28, 440*1.25};	

	/* 绘制示波器方格 */	
	GUI_MULTIBUF_Begin();	
	
	/* 绘制示波器方格 */		
	GUI_SetClipRect(&Rect);
	
	/* 第1步：绘制示波器方格 ***********************************************************************/
	GUI_MEMDEV_WriteAt(hMemDSO, 40*1.28, 40*1.25);
	
	/* 第2步：实现数据的修正，以便显示到LCD上******************************************************/
	for(i = 0; i < DSOSCREEN_LENGTH; i++)
	{
		temp = g_DSO1->usWaveBuf[i] * 100 / g_AttTable[Ch1AmpId][0];
	    temp = temp + g_DSO1->usRefPos - 40*1.28;
		g_DSO1->usWaveBuf[i] = 400*1.28 - temp + (g_DSO1->usRefPos-240*1.28) * 2;
	}
	
	for(i = 0; i < DSOSCREEN_LENGTH; i++)
	{
		temp =  g_DSO2->usWaveBuf[i] * 100 / g_AttTable[Ch1AmpId][0];
	    temp = temp + g_DSO2->usRefPos - 40*1.28;
		g_DSO2->usWaveBuf[i] = 400*1.28 - temp + (g_DSO2->usRefPos-240*1.28) * 2;
	}
	
	/* 第3步：在LCD上显示波形**********************************************************************/
	GUI_SetColor(GUI_YELLOW);
	
	/* 单通道采样率从2.8Msps到2Ksps采用下面的方式显示 */
	if(TimeBaseId < 10)
	{
		GUI_DrawGraph((I16 * )&g_DSO1->usWaveBuf,   
					  DSOSCREEN_LENGTH,     
					  DSOSCREEN_STARTX, 	 
					  DSOSCREEN_STARTY);  

		/* 普通触发模式显示通道2波形，正常触发不显示 */
		if(TriggerFlag == 0)
		{
			GUI_SetColor(GUI_GREEN);
			GUI_DrawGraph((I16 * )g_DSO2->usWaveBuf,   
						  DSOSCREEN_LENGTH,     
						  DSOSCREEN_STARTX, 	 
						  DSOSCREEN_STARTY);  	
		}
	}
	/* 其余采用下面的方式显示 */
	else
	{
		/* 小于600个采样点时显示方式，600个点是一屏能够显示的数据个数 */
		if(g_DSO1->ulSlowRefresh1 < 600*1.28)
		{
			GUI_DrawGraph((I16 * )&g_DSO1->usWaveBuf[0],   
	              g_DSO1->ulSlowRefresh1 ,     
				  DSOSCREEN_STARTX, 	 
				  DSOSCREEN_STARTY); 
		}
		/* 大于等于600个采样点时显示方式 */
		else
		{
			GUI_DrawGraph((I16 * )&g_DSO1->usWaveBuf[0],   
								  600*1.28,     
								  DSOSCREEN_STARTX, 	 
								  DSOSCREEN_STARTY); 
		}

		/* 普通触发模式显示通道2波形，正常触发不显示 */
		if(TriggerFlag == 0)
		{
			/* 小于600个采样点时显示方式，600个点是一屏能够显示的数据个数 */
			GUI_SetColor(GUI_GREEN);
			if(g_DSO2->ulSlowRefresh1 < 600*1.28)
			{
				GUI_DrawGraph((I16 * )&g_DSO2->usWaveBuf[0],   
					  g_DSO2->ulSlowRefresh1 ,     
					  DSOSCREEN_STARTX, 	 
					  DSOSCREEN_STARTY); 
			}
			/* 大于等于600个采样点时显示方式 */
			else
			{
				GUI_DrawGraph((I16 * )&g_DSO2->usWaveBuf[0],   
									  600*1.28,     
									  DSOSCREEN_STARTX, 	 
									  DSOSCREEN_STARTY); 
			}
		}
	}
	
	/* 第4步：在LCD上显示FFT幅频**********************************************************************/
	/* 在自动触发模式，采样率为2.8Msps到5Ksps时才显示FFT波形 */
	if((g_Flag->ucFFTDisp == 0)&&(TriggerFlag == 0)&&(TimeBaseId < 10))
	{
		/* 求FFT变换结果的幅值 */
		for(i = 0; i < DSOSCREEN_LENGTH; i++)
		{
			testInput_fft_2048[i] = testInput_fft_2048[i]/1024; 
		}
		
		/* 直流分量大小 */
		testInput_fft_2048[0] = testInput_fft_2048[0]/2;
		
		/* 求得幅频做一下转换，方便在LCD上显示 */
		for(i = 0; i < DSOSCREEN_LENGTH-2; i++)
		{
			testInput_fft_2048[i] = testInput_fft_2048[i]/5.0f;
			g_DSO1->usWaveBuf[i+2] = 390*1.28 - testInput_fft_2048[i] ;
		}
		
		GUI_SetColor(GUI_RED);
		GUI_DrawGraph((I16 * )&g_DSO1->usWaveBuf[2],   
					  598*1.28,     
					  DSOSCREEN_STARTX, 	 
					  DSOSCREEN_STARTY); 
	}
	
	/* 第6步：垂直测量，用于测量波形的时间************************************************************************/
	if(g_Flag->hWinCursors == 1)
	{
		DSO_DrawCursorH();
	}
	
	/* 第7步：水平测量，用于测量幅值差值************************************************************************/
	if(g_Flag->hWinCursors == 2)
	{
		DSO_DrawCursorV();
	}
	
	/* 下面这两个函数切不可忘记写了 */
	GUI_SetClipRect(NULL);

	GUI_MULTIBUF_End();

	/* 第8步：显示触发游标的位置*************************************************************************/
	GUI_SetColor(GUI_YELLOW);
	
	/* 显示触发游标 */
	GUI_FillPolygon(&aPointsTrig[0],          /* 指向要显示和填充的多边形 */
	                GUI_COUNTOF(aPointsTrig), /* 点列表中指定的点数量 */
	                627*1.28,                      /* 原点的X位置 */
	                g_TrigVol->usPos);        /* 原点的Y位置 */
	
	/* 重新设置触发值后，显示游标水平线1s时间 */
	if((GUI_GetTime() - g_TrigVol->ulTrigTimeLine) < 1000)
	{
		GUI_DrawHLine(g_TrigVol->usPos, 40*1.28, 626*1.25);
	}

}
/*
*********************************************************************************************************
*	函 数 名: _cbTouchCalibration
*	功能说明: 触摸校准窗口
*	形    参: pMsg  WM_MESSAGE类型指针变量   
*	返 回 值: 无
*********************************************************************************************************
*/
//static void _cbTouchCalibration(WM_MESSAGE * pMsg) 
//{
//	switch (pMsg->MsgId) 
//	{
//		/* 背景重绘 */
//		case WM_PAINT:
//			GUI_SetBkColor(GUI_BLUE);
//			GUI_Clear();
//			break;
//			
//		default:
//			WM_DefaultProc(pMsg);
//	}
//}
/*
*********************************************************************************************************
*	函 数 名: DSO_Graph
*	功能说明: 绘制波形和波形的背景显示
*	形    参: 无        	
*	返 回 值: 无
*********************************************************************************************************
*/
static void DSO_Graph(void) 
{ 
	uint32_t t0, t1;
	uint8_t uKeyCode;
//	WM_HWIN hTouchWin;
	OS_ERR  err;
	
	
	/* 创建桌面窗口所需的定时器，用触发电压参考线的短暂显示 */
	hTimer = WM_CreateTimer(WM_HBKWIN,  /* 接受信息的窗口的句柄 */
				   0, 	        		/* 用户定义的Id。如果不对同一窗口使用多个定时器，此值可以设置为零。 */
				   1050,        		/* 周期，此周期过后指定窗口应收到消息*/
				   0);	       			/* 留待将来使用，应为0 */	
	while(1) 
	{
		/* 记录刷新前的时刻 */
		t0 = GUI_GetTime(); //返回当前的时间int	
		/* 给信号处理任务发消息，做波形的处理和FFT */
		g_Flag->ucDsoMsg = DspFFT2048Pro_15;
		OSTaskQPost(AppTaskDsoTCB,
							 (void *)&g_Flag->ucDsoMsg,    
					     sizeof(g_Flag->ucDsoMsg),    
					     OS_OPT_POST_FIFO,
					     &err);		
		/* 
		   g_Flag->hWinRunStop = 0 表示运行态，= 1表示暂停。
		   g_ucWaveRefreshFlag 等于0：表示不执行波形区域的刷新，等于1：表示执行波形区域的刷新。
		*/
		if((g_Flag->hWinRunStop == 0)||(g_Flag->ucWaveRefresh == 1)) //或
		{
			_DrawWave();
			g_Flag->ucWaveRefresh = 0;
		}
		/* 求差值，根据不同的执行时间差，执行不同的延迟时间 */
		t1 = GUI_GetTime() - t0;
		if (t1 < g_RefreshDelay[TimeBaseId]) 
		{  
			GUI_Delay(g_RefreshDelay[TimeBaseId] - t1);
		}
		
		/////////////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////////////	
		/* 按键消息处理 */
		uKeyCode = KEY_Scan(0);
		/* 判断是否有按键按下 */
		if(uKeyCode != KEY_NONE)
		{
			switch (uKeyCode)
			{
//K2 暂停
			case KEY2_PRES:
					/* TriggerFlag = 0 表示当前用的是自动触发模式进行暂停和运行操作 */
							if(TriggerFlag == 0)
							{
								if(g_Flag->hWinRunStop == 0)
								{
									g_Flag->hWinRunStop = 1;
//									ADC_DMA_Close();
								}
								else
								{
									g_Flag->hWinRunStop = 0;						
//									ADC_DMA_Open();
								}
							}
							/* TriggerFlag = 2或3表示当前用的是正常触发模式进行暂停和运行操作 */
							else
							{
								if(g_Flag->ucTrig == 0)
								{
									g_Flag->ucTrig = 1;
//									ADC_DMA_Close();
								}
								else
								{
									g_Flag->ucTrig = 0;						
//									ADC_DMA_Open();
								}
								
								/* 设置更新一次 */
								TriggerFlag = 2;
							}
							g_Flag->ucWaveRefresh = 1;  //1表示刷新波形
							WM_InvalidateArea(&rRunMode);//使运行状态的部分无效	
			break;
//K3 改变触发方式		
			case KEY3_PRES:
					if(TriggerFlag == 0){
						/* 从自动触发切换到普通触发 */
						TriggerFlag = 1;
						g_Flag->ucTrig = g_Flag->hWinRunStop;
						g_DSO1->sCurTriStep = 0;
						/* 更新触发位置 */
						WM_InvalidateArea(&rTrigPos);
						/*  进入触发后重新设置ADC触发值为1V，切换后重新设置采样率位2Msps */
						WM_SendMessageNoPara(hDlgScale, WM_TextUpDate);
						/* 进入触发界面后，默认设置触发是1V，也就是1241 */						
						ADC_AnalogWatchdogThresholdsChange(&ADC3_Handler,&AnalogWDGConfig_1,1241,0);
					}
					else{
						/* TriggerFlag = 0 表示当前用的是自动触发模式 */
						TriggerFlag = 0;
						/* 更新状态 */
						g_Flag->hWinRunStop = g_Flag->ucTrig;
						/* 返回后，不再需要看门狗设置触发 */
						ADC_AnalogWatchdogThresholdsChange(&ADC3_Handler,&AnalogWDGConfig_1,4096,0);
					}
					/* 两种触发模式切换后，都将数值设置为默认 */
					g_TrigVol->usPos = 190;     
					g_TrigVol->usTrigValue = 1241; 
					WM_InvalidateArea(&rTrigValue);
					WM_InvalidateArea(&rTrigMode);	
					/* 刚切换到普通触发模式显示一次波形 */
					if(TriggerFlag == 1)
					{
						TriggerFlag = 2;
					}
				break;
/*****************上键*****************************************************************/
			case WKUP_PRES :
					if(g_Flag->hWinButState == 0){//增加采样率
							TimeBaseId++;
									if(TimeBaseId == TIME_COUNT){
										TimeBaseId = TIME_COUNT - 1;
									}
							ADC_DMA_Close();
							ADC_DMA_Open();
							/* 如果是低频的慢速显示需要记录当前的DMA出传输位置 */
							g_DSO1->ulSlowRefresh0 = 10240 - DMA1_Stream1->NDTR;
							g_DSO2->ulSlowRefresh0 = 10240 - DMA2_Stream1->NDTR;
							g_DSO1->ulSlowRefresh1 = 0;
							g_DSO2->ulSlowRefresh1 = 0;
							/* 更新当前采样时基显示 */
							WM_SendMessageNoPara(hDlgScale, WM_TextUpDate);
							/* 
									 g_DSO1->ucSlowWaveFlag和g_DSO2->ucSlowWaveFlag = 0 表示启动慢速显示模式，
									 也就是滚动显示在低频采样时，设置其等于0才有效。这里为了方便直接设置为0；
							*/
							g_DSO1->ucSlowWaveFlag = 0;
							g_DSO2->ucSlowWaveFlag = 0;
					}
					else if(g_Flag->hWinButState == 1){//调节幅度衰减
						Ch1AmpId++;
						if(Ch1AmpId == ATT_COUNT)
						{
							Ch1AmpId = ATT_COUNT - 1;
						}
						WM_SendMessageNoPara(hDlgAmp, WM_TextUpDate);
					}
					else if(g_Flag->hWinButState == 2){// 波形上升
						g_DSO1->usRefPos -= 10;
						if(g_DSO1->usRefPos<= 40)
						{
							g_DSO1->usRefPos = 40;
						}
						WM_InvalidateArea(&rRefPos);
					}
					else if(g_Flag->hWinButState == 3){// 水平A右移
						g_Cursors->sCursorVA += g_Cursors->usCursorStep;
						if(g_Cursors->sCursorVA >= g_Cursors->sCursorVB)
						{
							g_Cursors->sCursorVA = g_Cursors->sCursorVB;
						}
					}
					else if(g_Flag->hWinButState == 4){// 水平B右移
						g_Cursors->sCursorVB += g_Cursors->usCursorStep;
						if(g_Cursors->sCursorVB >= 640)
						{
							g_Cursors->sCursorVB = 640;
						}
					}
					else if(g_Flag->hWinButState == 5){// 垂直A上移
						g_Cursors->sCursorHA -= g_Cursors->usCursorStep;
						if(g_Cursors->sCursorHA <= 40)
						{
							g_Cursors->sCursorHA = 40;
						}
					}
					else if(g_Flag->hWinButState == 6){// 垂直B上移
						g_Cursors->sCursorHB -= g_Cursors->usCursorStep;
						if(g_Cursors->sCursorHB <= g_Cursors->sCursorHA)
						{
							g_Cursors->sCursorHB = g_Cursors->sCursorHA;
						}					
					}
					else if(g_Flag->hWinButState == 7){//触发值上升
						/* 调整触发箭头的位置，方便查看 */
						g_TrigVol->usPos -= g_TrigVol->usTriStep;
						if(g_TrigVol->usPos <= DSOSCREEN_STARTY)
						{
							g_TrigVol->usPos = DSOSCREEN_STARTY;
						}
						
						/* 根据调节的触发箭头位置获得触发值 */
						g_TrigVol->ufTrigValue = 240 - g_TrigVol->usPos;
						g_TrigVol->ufTrigValue = g_TrigVol->ufTrigValue * g_AttTable[Ch1AmpId][1] / 50000;
						g_TrigVol->ufTrigValue = g_TrigVol->ufTrigValue * 4095 / 3.3f;
						g_TrigVol->usTrigValue = g_TrigVol->ufTrigValue;
						
						/* 更新触发值 */
						WM_InvalidateArea(&rTrigValue);
						
						/* 更新后，更新一下波形显示，仅限普通触发方式 */
						if(TriggerFlag != 0)
						{
							TriggerFlag = 2;
							/* 如果是普通触发模式，更新ADC3的看门狗阀值 */
							ADC_AnalogWatchdogThresholdsChange(&ADC3_Handler,&AnalogWDGConfig_1, g_TrigVol->usTrigValue, 0);
						}
						/* 更新触发值的显示并获得当前的时间，从而实现调节触发值后，显现一条水平的直线，时长1s */
						g_TrigVol->ulTrigTimeLine = GUI_GetTime();
						WM_RestartTimer(hTimer, 1050);						
					}
					else if(g_Flag->hWinButState == 8){
						/* 自动触发模式：6K数据的显示起始位置设定 */
						if(TriggerFlag == 0)
						{
							/* 右移100个数据 */
							g_DSO1->sCurTriStep += 100;						
						}
						/* 普通触发模式：2K数据的显示起始位置设定 */
						else
						{
							/* 右移100个数据 */
							g_DSO1->sCurTriStep += 100;
							
							/* 
							   正常触发模式支持的数据显示是2K=2048，默认显示屏的正中间位置对应数据的
							   正中间位置，由于一屏可以显示600字节，所以向右最大偏移1024-300 =724字节。
							*/
							if(g_DSO1->sCurTriStep >= 724) 
							{
								g_DSO1->sCurTriStep = 724;
							}
							
							/* 通过设置为2来更新波形显示 */
							TriggerFlag = 2;
						}
						
						/* 更新当前的数据位置的箭头 */
						WM_InvalidateArea(&rTrigPos);
					
					}
					
			break;
/*****************下键*****************************************************************/
			case	KEY1_PRES:
					if(g_Flag->hWinButState == 0){//减少采样率
						TimeBaseId--;
						if(TimeBaseId < 0)
						{
							TimeBaseId = 0;
						}
						
						ADC_DMA_Close();
						ADC_DMA_Open();
						
						/* 如果是低频的慢速显示需要记录当前的DMA出传输位置 */
						g_DSO1->ulSlowRefresh0 = 10240 - DMA1_Stream1->NDTR;
						g_DSO2->ulSlowRefresh0 = 10240 - DMA2_Stream1->NDTR;
						g_DSO1->ulSlowRefresh1 = 0;
						g_DSO2->ulSlowRefresh1 = 0;
						
						/* 更新当前采样时基显示 */
						WM_SendMessageNoPara(hDlgScale, WM_TextUpDate);
						
						/* 
						   g_DSO1->ucSlowWaveFlag和g_DSO2->ucSlowWaveFlag = 0 表示启动慢速显示模式，
						   也就是滚动显示在低频采样时，设置其等于0才有效。这里为了方便直接设置为0；
						*/
						g_DSO1->ucSlowWaveFlag = 0;
						g_DSO2->ucSlowWaveFlag = 0;
					}
					else if(g_Flag->hWinButState == 1){//调节幅度增加
						Ch1AmpId--;
						if(Ch1AmpId < 0)
						{
							Ch1AmpId = 0;
						}
						WM_SendMessageNoPara(hDlgAmp, WM_TextUpDate);
					}
					else if(g_Flag->hWinButState == 2){// 波形下降
						g_DSO1->usRefPos += 10;
						if(g_DSO1->usRefPos >= 440)
						{
							g_DSO1->usRefPos = 440;
						}
						WM_InvalidateArea(&rRefPos);
					}
					else if(g_Flag->hWinButState == 3){// 水平A左移
						g_Cursors->sCursorVA -= g_Cursors->usCursorStep;
						if(g_Cursors->sCursorVA <= 40)
						{
							g_Cursors->sCursorVA = 40;
						}
					}
					else if(g_Flag->hWinButState == 4){// 水平B左移
						g_Cursors->sCursorVB -= g_Cursors->usCursorStep;
						if(g_Cursors->sCursorVB <= g_Cursors->sCursorVA)
						{
							g_Cursors->sCursorVB = g_Cursors->sCursorVA;
						}
					}
					else if(g_Flag->hWinButState == 5){// 垂直A下移
						g_Cursors->sCursorHA += g_Cursors->usCursorStep;
						if(g_Cursors->sCursorHA >= g_Cursors->sCursorHB)
						{
							g_Cursors->sCursorHA = g_Cursors->sCursorHB;
						}
					}
					else if(g_Flag->hWinButState == 6){// 垂直B下移
						g_Cursors->sCursorHB += g_Cursors->usCursorStep;
						if(g_Cursors->sCursorHB >= 440)
						{
							g_Cursors->sCursorHB = 440;
						}
					}
					else if(g_Flag->hWinButState == 7){//触发值下降
						/* 调整触发箭头的位置，方便查看 */
						g_TrigVol->usPos += g_TrigVol->usTriStep;
						if(g_TrigVol->usPos >= DSOSCREEN_ENDY)
						{
							g_TrigVol->usPos = DSOSCREEN_ENDY;
						}
						/* 根据调节的触发箭头位置获得触发值 */
						g_TrigVol->ufTrigValue = 240 - g_TrigVol->usPos;
						g_TrigVol->ufTrigValue = g_TrigVol->ufTrigValue * g_AttTable[Ch1AmpId][1] / 50000;
						g_TrigVol->ufTrigValue = g_TrigVol->ufTrigValue * 4095 / 3.3f;
						g_TrigVol->usTrigValue = g_TrigVol->ufTrigValue;
						
						WM_InvalidateArea(&rTrigValue);
						/* 更新后，更新一下波形显示，仅限普通触发方式 */
						if(TriggerFlag != 0)
						{
							TriggerFlag = 2;
							/* 如果是普通触发模式，更新ADC3的看门狗阀值 */
							ADC_AnalogWatchdogThresholdsChange(&ADC3_Handler,&AnalogWDGConfig_1, g_TrigVol->usTrigValue, 0);
						}
						/* 更新触发值的显示并获得当前的时间，从而实现调节触发值后，显现一条水平的直线，时长1s */
						g_TrigVol->ulTrigTimeLine = GUI_GetTime();
						WM_RestartTimer(hTimer, 1050);						
					}
					else if(g_Flag->hWinButState == 8){
			    /* 自动触发模式：2K数据的显示起始位置设定 */
						if(TriggerFlag == 0)
						{
							/* 左移100个数据 */
							g_DSO1->sCurTriStep -= 100;	
						}
						/* 普通触发模式：6K数据的显示起始位置设定 */
						else
						{
							/* 左移100个数据 */
							g_DSO1->sCurTriStep -= 100;
							/* 
							   正常触发模式支持的数据显示是2K=2048，默认显示屏的正中间位置对应数据的
							   正中间位置，由于一屏可以显示600字节，所以向右最大偏移-(1024-300) = -724字节。
							*/
							if(g_DSO1->sCurTriStep <= -724)
							{
								g_DSO1->sCurTriStep = -724;
							}
							
							/* 通过设置为2来更新波形显示 */
							TriggerFlag = 2;
						}
					
						/* 更新当前的数据位置的箭头 */
						WM_InvalidateArea(&rTrigPos);
					
					}
					
			break;
				/* 摇杆OK键按下 ***用key0代替************************************************/
				case KEY0_PRES:
					/* OK键用于实现8种功能的切换 */
					g_Flag->hWinButState++;
					if(g_Flag->hWinButState == 9)
					{
						g_Flag->hWinButState = 0;
					}
					WM_InvalidateArea(&rButState);
					break;
			}		
		}
	}
}
/*
*********************************************************************************************************
*	函 数 名: _cbMotion
*	功能说明: Motion窗口的回调函数，主要是桌面图标的滑动处理
*	形    参: pMsg  WM_MESSAGE类型指针变量   
*	返 回 值: 无
*********************************************************************************************************
*/
static void _cbMotion(WM_MESSAGE * pMsg) 
{
	WM_MOTION_INFO * pInfo;
	WM_HWIN          hWin = pMsg->hWin;
	PARA           * pPara;

	switch (pMsg->MsgId) 
	{
		case WM_MOTION:
			WM_GetUserData(hWin, &pPara, sizeof(pPara));
			pInfo = (WM_MOTION_INFO *)pMsg->Data.p;
			switch (pInfo->Cmd) 
			{
				/* Motion功能初始化，设置X方向和Y方向都支持滑动 */
				case WM_MOTION_INIT:
					pInfo->Flags =  WM_CF_MOTION_X | WM_CF_MOTION_Y | WM_MOTION_MANAGE_BY_WINDOW;
					pInfo->SnapY = 1;
					pInfo->SnapX = 1;
					break;

				case WM_MOTION_MOVE:
					pPara->FinalMove = pInfo->FinalMove;
					/* Y轴方向上下滑动 */
					if(g_Flag->ucMotionXY == 0)
					{
						if((pInfo->pState->Pressed == 1)&&(pInfo->pState->x <= 340*1.28))
						{
							pPara->iCH1Pos += pInfo->dy;
							if(pPara->iCH1Pos >= 440*1.25)
							{
								pPara->iCH1Pos = 440*1.25;
							}
							if(pPara->iCH1Pos <= 40)
							{
								pPara->iCH1Pos = 40;
							}

							g_DSO1->usRefPos = pPara->iCH1Pos;
						}
						else if((pInfo->pState->Pressed == 1)&&(pInfo->pState->x > 340*1.28))
						{						
							pPara->iCH2Pos += pInfo->dy;
							if(pPara->iCH2Pos >= 440*1.25)
							{
								pPara->iCH2Pos = 440*1.25;
							}
							if(pPara->iCH2Pos <= 40)
							{
								pPara->iCH2Pos = 40;
							}

							g_DSO2->usRefPos = pPara->iCH2Pos;
						}

						WM_InvalidateArea(&rRefPos);
					}
					/* X轴方向左右滑动 */
					else
					{
						if(pInfo->pState->Pressed == 1)
						{
							g_DSO1->sCurTriStep -= pInfo->dx;
						}
						else if(pInfo->pState->Pressed == 1)
						{	
							g_DSO1->sCurTriStep -= pInfo->dx;						
						}

						if(TriggerFlag == 0)
						{

						}
						else
						{
							if(g_DSO1->sCurTriStep <= -724*1.28)
							{
								g_DSO1->sCurTriStep = -724*1.28;
							}

							if(g_DSO1->sCurTriStep >= 724*1.28) 
							{
								g_DSO1->sCurTriStep = 724*1.28;
							}

							TriggerFlag = 2;
						}

						WM_InvalidateArea(&rTrigPos);
					}

					/* 更新当前的数据位置的箭头 */
					g_Flag->ucWaveRefresh = 1;
					break;

				case WM_MOTION_GETPOS:
					pInfo->yPos = pPara->iCH1Pos;
					//pInfo->xPos = pPara->iCH1Pos;
					break;
			}
			break;
	}
}
void MainTask(void){

/********第1步：变量初始化 ********/
	PARA Para;
	PARA *pPara;
	
	/* 第1步：变量初始化 ***********************************************************************/
	pPara = &Para;
  pPara->iCH1Pos = 140*1.25;
  pPara->iCH2Pos = 140*1.25;

	/* 波形Y轴位置 */
	g_DSO1->usRefPos = 140*1.25;
	g_DSO2->usRefPos = 340*1.25;	

	/* 自动触发和普通触发值位置 */
	g_TrigVol->usPos = 190;
	g_TrigVol->usTriStep =1;
	g_TrigVol->usTrigValue = 1241; /* ADC的数值1241对应1V */
  g_TrigVol->ulTrigTimeLine = 1000;
	g_DSO1->sCurTriStep = 0;
	
	/* FIR设置 */
	g_DSO1->ucFirFlter_Step100KHz = 0;
	g_DSO1->ucFirFlter_Step10KHz = 0;
	g_DSO1->ucFirFlter_Step1KHz = 0;

	/* 垂直和水平测量游标设置 */
	g_Cursors->usCursorStep = 5;
	g_Cursors->sCursorVA = 190;   
	g_Cursors->sCursorVB = 490;
	g_Cursors->sCursorHA = 90;
	g_Cursors->sCursorHB = 390;

	g_Cursors->WaveCursorA = 0;
	g_Cursors->WaveCursorB = 0;
	
	/* 开启后默认显示的是峰峰值，最大值，最小值，平均值、RMS和Freq */
	g_DSO1->ucMeasureFlag[1] = 1;
	g_DSO1->ucMeasureFlag[12] = 1;
	g_DSO1->ucMeasureFlag[14] = 1;
	g_DSO1->ucMeasureFlag[15] = 1;
	g_DSO1->ucMeasureFlag[22] = 1;
	g_DSO1->ucMeasureFlag[24] = 1;
	
	/* 刷新慢速波形设置标志 */
	g_DSO1->ucSlowWaveFlag = 0;
	g_DSO2->ucSlowWaveFlag = 0;
	
	/* 全局标志初始化*/
	g_Flag->ucMotionXY = 0;      
	g_Flag->ucFFTDisp = 0;     
	g_Flag->hWinCursors = 0;    
	g_Flag->hWinRunStop = 0;    
	g_Flag->ucTrig = 0;
	g_Flag->hWinButState = 0;  	
	g_Flag->ucWaveRefresh = 0; 	
/********第2步：emWin初始化********/
		GUI_UC_SetEncodeUTF8();  
		GUI_CURSOR_Show();   		// 显示游标（指针）
	/*******开启缓冲**********/
		WM_SetCallback(WM_HBKWIN, _cbBkWin);//创建窗口回调函数，按键
		/* 开启所有窗口使用内存设备 */
	  WM_SetCreateFlags(WM_CF_MEMDEV);
	/* 使能滑动 */ 
		WM_MOTION_Enable(1);     
		WM_MOTION_SetDefaultPeriod(100);

/********第3步：绘制各种背景到存储设备 *******/
		hMemDSO = GUI_MEMDEV_CreateFixed(0, 
	                                 0, 
	                                 600*1.28, 
	                                 400*1.25, 
									 GUI_MEMDEV_HASTRANS, 
									 GUI_MEMDEV_APILIST_16, 
									 GUICC_M565);	
	/* 绘制示波器窗口背景 */
	GUI_MEMDEV_Select(hMemDSO);
  DSO_DrawBakFrame(0,0,599*1.28,399*1.25);
	GUI_MEMDEV_Select(0);	
	/* 绘制系统信息窗口背景 */
	DrawWinSysBk();	
	/* 绘制幅值窗口背景 */	
	DrawWinAmpBk();
	/* 绘制状态窗口背景 */
	DrawWinStatus1Bk();
	DrawWinStatus2Bk();
	/* 绘制频率设置窗口 */
	DrawWinScaleBk();
	/* 第3步：保证主界面的整体显示 ***************************************************/
	WM_HideWin(WM_HBKWIN);
	/* 初始化 DSO */
	DSO_Init(1);
	WM_ShowWin(WM_HBKWIN);

	/* 第4步：做一个透明的滑动窗口，实现水平和垂直滑动 ******************************/
	hMotion = WM_CreateWindowAsChild(40*1.28, 
	                                 40*1.28, 
																	600*1.25, 
																	400*1.25, 
																	WM_HBKWIN,  
																	WM_CF_MOTION_Y | WM_CF_SHOW | WM_CF_HASTRANS, 
																	_cbMotion, 
																	sizeof(pPara));
									 
    WM_SetUserData(hMotion, &pPara, sizeof(pPara));

	
/* 显示出所有的窗口 */
	GUI_Exec();
	/* 第5步：形显示和处理 ************************************************************/
	DSO_Graph();		

}






