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
*	                                  �궨��
*********************************************************************************************************
*/							   
#define LCD_YSIZE 480
#define LCD_XSIZE 630
  
/* ���岨�ε���ʾ�������� 600*400 */
#define DSOSCREEN_STARTX     40*1.28     /* ������ʾ��X��ʼλ�� */
#define DSOSCREEN_STARTY     40*1.25	    /* ������ʾ��Y��ʼλ�� */
#define DSOSCREEN_ENDX      639 *1.28    /* ������ʾ��X����λ�� */
#define DSOSCREEN_ENDY      439*1.25     /* ������ʾ��Y����λ�� */
#define DSOSCREEN_LENGTH    600*1.28     /* ������ʾ��Y����λ�� */

#define ATT_COUNT	        10      /* ����֧�ֵ�10�ַ�ֵ��λ */  
 
#define TIME_COUNT	        21      /* ����֧�ֵ�21�ֲ����� */

#define WM_TextUpDate  WM_USER + 1  /* �Զ���һ���ص���������Ϣ�����ڽ������ݵĸ��� */

#define FIR_LENGTH_SAMPLES   650*1.28    /* �������� */
#define BLOCK_SIZE           650*1.28    /* ����һ��arm_fir_f32����Ĳ�������� */
#define NUM_TAPS             81     /* �˲���ϵ������ */
#define FIR_StateBufSize     BLOCK_SIZE + NUM_TAPS - 1


/*
*********************************************************************************************************
*	                                  ����
*********************************************************************************************************
*/
extern const char *g_AttText[];                  /* 10�ַ��ȵ�λ */
extern const char *g_TimeTable[];                /* �����ʣ���2Msps��1Ksps ��Ӧ��ʱ�� */
extern const char *g_MeasureTable[];             /* ʾ������ǰʵ��֧�ֵĲ���ֵ */
extern const uint16_t g_CursorUintTable[][2];    /* �����α�������ʾ��ʽ */
extern const uint16_t g_AttTable[ATT_COUNT][2];  /* ������˥�������� */
extern const uint32_t g_SampleFreqTable[][2];    /* Ƶ�ʱ� */

extern GUI_RECT rClient;       /* ������ʾʾ������logo */
extern GUI_RECT rRunMode;      /* ������ʾ����״̬�����к���ͣ */
extern GUI_RECT rTrigMode;     /* ������ʾ����ģʽ���Զ��������������� */
extern GUI_RECT rTrigValue;    /* ������ʾ�Զ��������Ĵ������� */
extern GUI_RECT rButState;     /* ��ǰ������Ҫ���ڵ�״̬ */
extern GUI_RECT rRefPos;       /* ʾ��������ನ�βο���λ������ */
extern GUI_RECT rTrigPos;      /* 2KB���ݲ�ѯ  */


extern const GUI_POINT aPointsTrigBrowser[3];  /* 2k�����У�������ʾ����ʼλ�� */
extern const GUI_POINT aPointsTrig[7];         /* ������ʾ���Ҳ���ϴ���ֵ��ͷ */
extern const GUI_POINT aPoints[5];             /* ������ʾ�������ϲ�����ʾ�Ĳο�λ�� */

extern uint16_t TriggerFlag;    /* 0�������Զ�������1,2��������ͨ���� */
extern int8_t  Ch1AmpId;        /* ��g_AttText��ѡ��ÿ�������ʾ�ķ�ֵ */ 
extern int8_t  TimeBaseId;      /* ѡ����Ӧ������ */
/*
*********************************************************************************************************
*	                                  �ⲿ����
*********************************************************************************************************
*/

extern GUI_CONST_STORAGE GUI_FONT GUI_FontFontSong16;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontFontSong33;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontFontSong26;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontFontSong20;
/* ����emWin��Ϣ���ݵĽṹ�� */
typedef struct 
{
  int    iCH1Pos;
  int    iCH2Pos;
  int    FinalMove;
} PARA;
/* ʾ��������ͨ����ص����ݽṹ */
typedef struct
{
	float32_t   WaveMean;              /* ƽ��ֵ */
	float32_t   WavePkPk;              /* ���ֵ */
	float32_t   WaveFreq;              /* Ƶ��ֵ */
	float32_t   WaveMax;               /* ���ֵ */
	float32_t   WaveMin;               /* ���ֵ */
	float32_t   WaveRMS;               /* ������ */
	
	uint16_t    usCurPos;              /* DMA��ADC�����д����λ�� */
	uint32_t    uiFreq;                /* ����Ĳ���Ƶ�� */
	uint32_t    ulSlowRefresh0;        /* ���ٲ�����ʾʱ��DMA��ADC�����д������ʼλ��*/
	uint32_t    ulSlowRefresh1;        /* ���ٲ�����ʾʱ��DMA��ADC�����д���ĵ�ǰλ��*/
	uint16_t    usWaveBuf[768];        /* ����ˢ�»��壬һ��ˢ����Ҫ600���� */
	uint16_t    usWaveBufTemp[1024*2]; /* ����ˢ��ʱ���壬���ڴ�10K��ADC�����ж�ȡ2KB */
	

	uint8_t     ucMeasureFlag[30];     /* һ����30�����õĲ���ѡ� 1����ʾѡ���ˣ�0��ʾû��ѡ�� */
	
	int16_t     sCurTriPos;            /* ��2KB���λ������л�ȡ���εĴ���λ�� */
	int16_t     sCurTriStep;           /* ������λ�����ʱ�ģ�ÿ���ƶ������ݸ��� */
	
	uint16_t    usRefPos;              /* ���Ĳο�λ�ã�Ĭ�Ͽ��������м�λ�� */
	
	uint8_t  		ucFirFlter_Step100KHz; /* 0����ʾ��ִ���˲���1����ʾ100KHz��2����ʾ200KHz�����Դ����� */
	uint8_t 	 	ucFirFlter_Step10KHz;  /* 0����ʾ��ִ���˲���1����ʾ10KHz�� 2����ʾ20KHz�����Դ����� */
	uint8_t  		ucFirFlter_Step1KHz;   /* 0����ʾ��ִ���˲���1����ʾ1KHz��  2����ʾ2KHz�����Դ����� */
	
	uint8_t 		ucSlowWaveFlag;            /* �Ͳ�����ʱ����������ˢ�±�־ */
}DSO_T;
extern DSO_T *g_DSO1;
extern DSO_T *g_DSO2;













/* ����ˮƽ�����ʹ�ֱ�����α� */
typedef struct
{
	uint16_t  usPos;          /* ������ͷ��Y����ʾλ�� */
	uint16_t  usTriStep;      /* �ƶ�������ͷʱ��ÿ���ƶ���Y�������ظ��� */
	float32_t ufTrigValue;    /* ���㴥��ֵ�����ڼ�������е���ʱ���� */ 
	uint16_t  usTrigValue;    /* ���㴥��ֵ */  
	uint32_t  ulTrigTimeLine; /* ��������ֵʱ������ʾһ��ˮƽ��ֱ�ߣ������������������ʾʱ�䣬��λ���� */  
}TRIVOLTAGE_T;

extern TRIVOLTAGE_T *g_TrigVol;

/* ����ˮƽ�����ʹ�ֱ�����α� */
typedef struct
{
	uint16_t usCursorStep;    /* �ƶ������α�ʱ��ÿ���ƶ������ظ��� */    
	int16_t  sCursorVA;       /* ��ֱ����ʱ���α�A��λ�� */ 
	int16_t  sCursorVB;       /* ��ֱ����ʱ���α�B��λ�� */ 
	int16_t  sCursorHA;       /* ˮƽ����ʱ���α�A��λ�� */ 
	int16_t  sCursorHB;       /* ˮƽ����ʱ���α�B��λ�� */ 

	float32_t  WaveCursorA;   /* ���ڼ�¼�α�A�ļ����� */ 
	float32_t  WaveCursorB;   /* ���ڼ�¼�α�B�ļ����� */ 
}CURSORS_T;

extern CURSORS_T *g_Cursors;

/* ��־λ */
typedef struct
{
	uint8_t ucMotionXY;      /* 0����ʾ��ֱ����������1����ʾˮƽ�������� */
	uint8_t ucFFTDisp;       /* 0����ʾ��ʾFFT���Σ�1����ʾ����ʾFFT���� */
	uint8_t hWinCursors;     /* 0������ʾ�������ڣ�1����ʾ�������� */
	uint8_t hWinRunStop;     /* 0����ʾ���У�1����ʾ��ͣ */
	uint8_t ucTrig;          /* 0����ʾ��������ģʽʱ�����в��Σ�1����ʾ��������ģʽʱ����ͣ����*/
	uint8_t hWinButState;  	 /* 8�ְ�������״̬ */
	uint8_t ucWaveRefresh;   /* 0����ʾ��ִ�в��������ˢ�£�1����ʾִ�в��������ˢ�� */
	
	uint32_t ucDsoMsg;       /* ���������ͨ�ű�־ */
}FLAG_T;

extern FLAG_T *g_Flag;
/*
*********************************************************************************************************
*	                                  ���ھ��
*********************************************************************************************************
*/
extern WM_HWIN hDlgAmp;         /* ������ʾ���ȵĴ��� */
extern WM_HWIN hDlgStatus1;     /* ������ʾƽ��ֵ�����ֵ�����ֵ����Сֵ���� */
extern WM_HWIN hDlgStatus2;     /* ������ʾƵ�ʺ�RMS���� */
extern WM_HWIN hDlgScale;       /* ������ʾ�����ʴ��� */
extern WM_HWIN hDlgSysInfo;     /* ������ʾϵͳ��Ϣ���� */



extern BUTTON_Handle hButton0;  /* 5����ť��� */
extern BUTTON_Handle hButton1;
extern BUTTON_Handle hButton2;
extern BUTTON_Handle hButton3;
extern BUTTON_Handle hButton4;



extern GUI_MEMDEV_Handle  hMemDSO; /* ������ʾ���洢�豸��� */

/*
*********************************************************************************************************
*                                       emWin����,ͼ��
*********************************************************************************************************
*/
extern GUI_CONST_STORAGE GUI_BITMAP bmSettings;
extern GUI_CONST_STORAGE GUI_BITMAP bmReturn;
/*
*********************************************************************************************************
*	                                  ���ⲿ�ļ����õĺ���
*********************************************************************************************************
*/
/* �������ȣ�ʱ����״̬���� */
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

/* ʾ���������ʼ�� */
extern void DSO_Init(uint8_t ucCreateFlag);

/* ��ť�����ĶԻ��� */
extern WM_HWIN DSO_CreateMeasureDlg(void);
extern WM_HWIN DSO_CreateAdcDlg(void);
extern WM_HWIN DSO_CreateDacDlg(void);
extern WM_HWIN DSO_CreateMathDlg(void);
extern WM_HWIN DSO_CreateSettingsDlg(void);
/* ʾ����������� */
extern void DSO_DrawBakFrame(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
extern void DSO_DrawCursorH(void);
extern void DSO_DrawCursorV(void);

/*
*********************************************************************************************************
*                                     �źŴ���FFT��FIR���źŷ�������
*********************************************************************************************************
*/

/* FFTʹ�� */
extern arm_rfft_fast_instance_f32 S;
extern uint32_t fftSize; 
extern uint32_t ifftFlag; 
extern float32_t *testInput_fft_2048;
extern float32_t *testOutput_fft_2048;





/* DACʹ�� */






/* ADCʹ�� */
extern uint16_t g_uiAdcAvgSample;
extern uint16_t ADC_GetSampleAvgN(void);

/* ˫ͨ�����δ��� */
extern void DSO1_WaveTrig(uint16_t usCurPos);
extern void DSO2_WaveTrig(uint16_t usCurPos);
extern void DSO1_WaveProcess(void);
extern void DSO2_WaveProcess(void);




/* Fir �˲� */
extern void DSO_FirFilter_Step100KHz(void);
extern void DSO_FirFilter_Step10KHz(void);
extern void DSO_FirFilter_Step1KHz(void);
extern float32_t *FirDataInput;      /* ������ TEST_LENGTH_SAMPLES*/
extern float32_t *FirDataOutput;     /* �˲������� TEST_LENGTH_SAMPLES*/
extern float32_t *firStateF32;       /* ״̬���棬��СBLOCK_SIZE + NUM_TAPS - 1*/
extern float32_t *g_RMSBUF;          /* ���ھ��������㻺���ַ */


/*
*********************************************************************************************************
*                                     ADC ���ⲿ���� HAl��
*********************************************************************************************************
*/
extern 			 ADC_HandleTypeDef 						ADC1_Handler;//ADC���
extern 			 ADC_HandleTypeDef 						ADC3_Handler;//ADC���
extern 			 ADC_AnalogWDGConfTypeDef   	AnalogWDGConfig_1;
#endif

