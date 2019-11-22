#ifndef __MainTask_H
#define __MainTask_H

#include "sys.h"
#include "GUI.h"
#include "DIALOG.h"
#include "WM.h"
#include "BUTTON.h"
#include "CHECKBOX.h"
#include "DROPDOWN.h"
#include "EDIT.h"
#include "FRAMEWIN.h"
#include "LISTBOX.h"
#include "MULTIEDIT.h"
#include "RADIO.h"
#include "SLIDER.h"
#include "TEXT.h"
#include "PROGBAR.h"
#include "SCROLLBAR.h"
#include "LISTVIEW.h"
#include "GRAPH.h"
#include "MENU.h"
#include "MULTIPAGE.h"
#include "ICONVIEW.h"
#include "TREEVIEW.h"
#include "IMAGE.h"
#include "GUI_VNC.h"
#include "ST_GUI_Addons.h"
#include "key.h"
#include "arm_math.h"
#include "stdint.h"
#include "adc.h"
#include  <os.h>


/*
*********************************************************************************************************
*	                                  宏定义
*********************************************************************************************************
*/							   
#define LCD_YSIZE 480
#define LCD_XSIZE 630
  
/* 定义波形的显示界面区域 600*400 */
#define DSOSCREEN_STARTX     40*1.28     /* 波形显示的X起始位置 */
#define DSOSCREEN_STARTY     40*1.25	    /* 波形显示的Y起始位置 */
#define DSOSCREEN_ENDX      639 *1.28    /* 波形显示的X结束位置 */
#define DSOSCREEN_ENDY      439*1.25     /* 波形显示的Y结束位置 */
#define DSOSCREEN_LENGTH    600*1.28     /* 波形显示的Y结束位置 */

#define ATT_COUNT	        10      /* 定义支持的10种幅值单位 */  
 
#define TIME_COUNT	        21      /* 定义支持的21种采样率 */

#define WM_TextUpDate  WM_USER + 1  /* 自定义一个回调函数的消息，用于界面数据的更新 */

#define FIR_LENGTH_SAMPLES   650*1.28    /* 采样点数 */
#define BLOCK_SIZE           650*1.28    /* 调用一次arm_fir_f32处理的采样点个数 */
#define NUM_TAPS             81     /* 滤波器系数个数 */
#define FIR_StateBufSize     BLOCK_SIZE + NUM_TAPS - 1


/*
*********************************************************************************************************
*	                                  变量
*********************************************************************************************************
*/
extern const char *g_AttText[];                  /* 10种幅度单位 */
extern const char *g_TimeTable[];                /* 采样率，从2Msps到1Ksps 对应的时基 */
extern const char *g_MeasureTable[];             /* 示波器当前实际支持的测量值 */
extern const uint16_t g_CursorUintTable[][2];    /* 测量游标数据显示格式 */
extern const uint16_t g_AttTable[ATT_COUNT][2];  /* 采样率衰减倍数表 */
extern const uint32_t g_SampleFreqTable[][2];    /* 频率表 */

extern GUI_RECT rClient;       /* 用于显示示波器的logo */
extern GUI_RECT rRunMode;      /* 用于显示运行状态，运行和暂停 */
extern GUI_RECT rTrigMode;     /* 用于显示触发模式，自动触发和正常触发 */
extern GUI_RECT rTrigValue;    /* 用于显示自动和正常的触发数组 */
extern GUI_RECT rButState;     /* 当前按键需要调节的状态 */
extern GUI_RECT rRefPos;       /* 示波器最左侧波形参考的位置区域 */
extern GUI_RECT rTrigPos;      /* 2KB数据查询  */


extern const GUI_POINT aPointsTrigBrowser[3];  /* 2k数据中，波形显示的起始位置 */
extern const GUI_POINT aPointsTrig[7];         /* 波形显示区右侧边上触发值箭头 */
extern const GUI_POINT aPoints[5];             /* 波形显示区左侧边上波形显示的参考位置 */

extern uint16_t TriggerFlag;    /* 0：用于自动触发，1,2：用于普通触发 */
extern int8_t  Ch1AmpId;        /* 从g_AttText中选择每个方格表示的幅值 */ 
extern int8_t  TimeBaseId;      /* 选择相应采样率 */
/*
*********************************************************************************************************
*	                                  外部字体
*********************************************************************************************************
*/

extern GUI_CONST_STORAGE GUI_FONT GUI_FontFontSong16;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontFontSong33;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontFontSong26;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontFontSong20;
/* 用于emWin消息传递的结构体 */
typedef struct 
{
  int    iCH1Pos;
  int    iCH2Pos;
  int    FinalMove;
} PARA;
/* 示波器波形通道相关的数据结构 */
typedef struct
{
	float32_t   WaveMean;              /* 平均值 */
	float32_t   WavePkPk;              /* 峰峰值 */
	float32_t   WaveFreq;              /* 频率值 */
	float32_t   WaveMax;               /* 最大值 */
	float32_t   WaveMin;               /* 峰峰值 */
	float32_t   WaveRMS;               /* 均方根 */
	
	uint16_t    usCurPos;              /* DMA在ADC缓冲中传输的位置 */
	uint32_t    uiFreq;                /* 估算的波形频率 */
	uint32_t    ulSlowRefresh0;        /* 慢速波形显示时，DMA在ADC缓冲中传输的起始位置*/
	uint32_t    ulSlowRefresh1;        /* 慢速波形显示时，DMA在ADC缓冲中传输的当前位置*/
	uint16_t    usWaveBuf[768];        /* 波形刷新缓冲，一次刷新需要600个点 */
	uint16_t    usWaveBufTemp[1024*2]; /* 波形刷临时缓冲，用于从10K的ADC缓冲中读取2KB */
	

	uint8_t     ucMeasureFlag[30];     /* 一共有30个可用的测量选项， 1：表示选择了，0表示没有选择 */
	
	int16_t     sCurTriPos;            /* 从2KB波形缓冲区中获取波形的触发位置 */
	int16_t     sCurTriStep;           /* 浏览波形缓冲区时的，每次移动的数据个数 */
	
	uint16_t    usRefPos;              /* 左侧的参考位置，默认开机后是中间位置 */
	
	uint8_t  		ucFirFlter_Step100KHz; /* 0：表示不执行滤波，1：表示100KHz，2：表示200KHz，，以此类推 */
	uint8_t 	 	ucFirFlter_Step10KHz;  /* 0：表示不执行滤波，1：表示10KHz， 2：表示20KHz，，以此类推 */
	uint8_t  		ucFirFlter_Step1KHz;   /* 0：表示不执行滤波，1：表示1KHz，  2：表示2KHz，，以此类推 */
	
	uint8_t 		ucSlowWaveFlag;            /* 低采样率时，波形慢速刷新标志 */
}DSO_T;
extern DSO_T *g_DSO1;
extern DSO_T *g_DSO2;













/* 用于水平测量和垂直测量游标 */
typedef struct
{
	uint16_t  usPos;          /* 触发箭头的Y轴显示位置 */
	uint16_t  usTriStep;      /* 移动触发箭头时，每次移动的Y方向像素个数 */
	float32_t ufTrigValue;    /* 计算触发值，用于计算过程中的临时计算 */ 
	uint16_t  usTrigValue;    /* 计算触发值 */  
	uint32_t  ulTrigTimeLine; /* 调整触发值时，会显示一条水平的直线，这个变量用于设置显示时间，单位毫秒 */  
}TRIVOLTAGE_T;

extern TRIVOLTAGE_T *g_TrigVol;

/* 用于水平测量和垂直测量游标 */
typedef struct
{
	uint16_t usCursorStep;    /* 移动测试游标时，每次移动的像素个数 */    
	int16_t  sCursorVA;       /* 垂直测试时，游标A的位置 */ 
	int16_t  sCursorVB;       /* 垂直测量时，游标B的位置 */ 
	int16_t  sCursorHA;       /* 水平测试时，游标A的位置 */ 
	int16_t  sCursorHB;       /* 水平测量时，游标B的位置 */ 

	float32_t  WaveCursorA;   /* 用于记录游标A的计算结果 */ 
	float32_t  WaveCursorB;   /* 用于记录游标B的计算结果 */ 
}CURSORS_T;

extern CURSORS_T *g_Cursors;

/* 标志位 */
typedef struct
{
	uint8_t ucMotionXY;      /* 0：表示垂直触摸滑动，1：表示水平触摸滑动 */
	uint8_t ucFFTDisp;       /* 0：表示显示FFT波形，1：表示不显示FFT波形 */
	uint8_t hWinCursors;     /* 0：不显示测量窗口，1：显示测量窗口 */
	uint8_t hWinRunStop;     /* 0：表示运行，1：表示暂停 */
	uint8_t ucTrig;          /* 0：表示正常触发模式时，运行波形，1：表示正常触发模式时，暂停波形*/
	uint8_t hWinButState;  	 /* 8种按键功能状态 */
	uint8_t ucWaveRefresh;   /* 0：表示不执行波形区域的刷新，1：表示执行波形区域的刷新 */
	
	uint32_t ucDsoMsg;       /* 用于任务间通信标志 */
}FLAG_T;

extern FLAG_T *g_Flag;
/*
*********************************************************************************************************
*	                                  窗口句柄
*********************************************************************************************************
*/
extern WM_HWIN hDlgAmp;         /* 用于显示幅度的窗口 */
extern WM_HWIN hDlgStatus1;     /* 用于显示平均值，峰峰值，最大值和最小值窗口 */
extern WM_HWIN hDlgStatus2;     /* 用于显示频率和RMS窗口 */
extern WM_HWIN hDlgScale;       /* 用于显示采样率窗口 */
extern WM_HWIN hDlgSysInfo;     /* 用于显示系统信息窗口 */



extern BUTTON_Handle hButton0;  /* 5个按钮句柄 */
extern BUTTON_Handle hButton1;
extern BUTTON_Handle hButton2;
extern BUTTON_Handle hButton3;
extern BUTTON_Handle hButton4;



extern GUI_MEMDEV_Handle  hMemDSO; /* 波形显示区存储设备句柄 */

/*
*********************************************************************************************************
*                                       emWin字体,图标
*********************************************************************************************************
*/
extern GUI_CONST_STORAGE GUI_BITMAP bmSettings;
extern GUI_CONST_STORAGE GUI_BITMAP bmReturn;
/*
*********************************************************************************************************
*	                                  供外部文件调用的函数
*********************************************************************************************************
*/
/* 创建幅度，时基和状态窗口 */
extern WM_HWIN CreateAmplitudeDlg(void); 
extern WM_HWIN CreateStatus1Dlg(void);
extern WM_HWIN CreateStatus2Dlg(void);
extern WM_HWIN CreateScaleDlg(void);
extern WM_HWIN CreateSysInfoDlg(void);
extern void DrawWinSysBk(void);
extern void DrawWinAmpBk(void);
extern void DrawWinStatus1Bk(void);
extern void DrawWinStatus2Bk(void);
extern void DrawWinScaleBk(void);

/* 示波器界面初始化 */
extern void DSO_Init(uint8_t ucCreateFlag);

/* 按钮创建的对话框 */
extern WM_HWIN DSO_CreateMeasureDlg(void);
extern WM_HWIN DSO_CreateAdcDlg(void);
extern WM_HWIN DSO_CreateDacDlg(void);
extern WM_HWIN DSO_CreateMathDlg(void);
extern WM_HWIN DSO_CreateSettingsDlg(void);
/* 示波器界面绘制 */
extern void DSO_DrawBakFrame(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
extern void DSO_DrawCursorH(void);
extern void DSO_DrawCursorV(void);

/*
*********************************************************************************************************
*                                     信号处理FFT，FIR，信号发生器等
*********************************************************************************************************
*/

/* FFT使用 */
extern arm_rfft_fast_instance_f32 S;
extern uint32_t fftSize; 
extern uint32_t ifftFlag; 
extern float32_t *testInput_fft_2048;
extern float32_t *testOutput_fft_2048;





/* DAC使用 */






/* ADC使用 */
extern uint16_t g_uiAdcAvgSample;
extern uint16_t ADC_GetSampleAvgN(void);

/* 双通道波形处理 */
extern void DSO1_WaveTrig(uint16_t usCurPos);
extern void DSO2_WaveTrig(uint16_t usCurPos);
extern void DSO1_WaveProcess(void);
extern void DSO2_WaveProcess(void);




/* Fir 滤波 */
extern void DSO_FirFilter_Step100KHz(void);
extern void DSO_FirFilter_Step10KHz(void);
extern void DSO_FirFilter_Step1KHz(void);
extern float32_t *FirDataInput;      /* 采样点 TEST_LENGTH_SAMPLES*/
extern float32_t *FirDataOutput;     /* 滤波后的输出 TEST_LENGTH_SAMPLES*/
extern float32_t *firStateF32;       /* 状态缓存，大小BLOCK_SIZE + NUM_TAPS - 1*/
extern float32_t *g_RMSBUF;          /* 用于均方根计算缓冲地址 */


/*
*********************************************************************************************************
*                                     ADC 的外部引用 HAl库
*********************************************************************************************************
*/
extern 			 ADC_HandleTypeDef 						ADC1_Handler;//ADC句柄
extern 			 ADC_HandleTypeDef 						ADC3_Handler;//ADC句柄
extern 			 ADC_AnalogWDGConfTypeDef   	AnalogWDGConfig_1;
#endif

