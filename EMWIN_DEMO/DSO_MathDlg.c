/*
*********************************************************************************************************
*	                                  
*	模块名称 : 示波器Math对话框
*	文件名称 : DSO_MathDlg.c
*	版    本 : V1.0
*	说    明 : 此对话框主要设置80阶Fir低通滤波器的截止频率选择：
*              1. Fs(采样率) = 2Msps时，  即Scale窗口显示500ns时，Fc(截止频率)的选择。
*              2. Fs(采样率) = 200Ksps时，即Scale窗口显示5us时，  Fc(截止频率)的选择。
*              3. Fs(采样率) = 20Ksps时， 即Scale窗口显示50us时， Fc(截止频率)的选择。
*
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



/*
*********************************************************************************************************
*	                                  自定义控件ID
*********************************************************************************************************
*/
#define ID_WINDOW_0    (GUI_ID_USER + 0x00)
#define ID_RADIO_0     (GUI_ID_USER + 0x01)
#define ID_TEXT_0      (GUI_ID_USER + 0x02)
#define ID_TEXT_1      (GUI_ID_USER + 0x03)
#define ID_TEXT_2      (GUI_ID_USER + 0x04)
#define ID_TEXT_3      (GUI_ID_USER + 0x05)
#define ID_RADIO_1     (GUI_ID_USER + 0x06)
#define ID_RADIO_2     (GUI_ID_USER + 0x07)

#define ID_BUTTON_0    (GUI_ID_USER + 0x08)
#define ID_BUTTON_1    (GUI_ID_USER + 0x09)

/*
*********************************************************************************************************
*	                                  变量
*********************************************************************************************************
*/
static uint32_t Radio_Step100KHz = 0;   /* 第一次打开时默认不开启滤波 */
static uint32_t Radio_Step10KHz = 0;    /* 第一次打开时默认不开启滤波  */
static uint32_t Radio_Step1KHz = 0;     /* 第一次打开时默认不开启滤波  */

/*
*********************************************************************************************************
*	函 数 名: _cbButtonBack
*	功能说明: 按钮回调函数
*	形    参: pMsg  消息指针
*	返 回 值: 无
*********************************************************************************************************
*/
static void _cbButtonBack(WM_MESSAGE * pMsg) 
{
	WM_HWIN  hWin;

	hWin  = pMsg->hWin;
	switch (pMsg->MsgId) 
	{
		case WM_PAINT:
			if (BUTTON_IsPressed(hWin)) 
			{
				GUI_SetColor(GUI_DARKGRAY);
				GUI_AA_FillCircle(72*1.25, 0, 72*1.25);
				GUI_SetBkColor(GUI_DARKGRAY);
				GUI_SetColor(GUI_WHITE); 
				GUI_DrawBitmap(&bmReturn, (40-28)*1.25, 5*1.25);
			} 
			else 
			{
				GUI_SetColor(GUI_STCOLOR_LIGHTBLUE);
				GUI_AA_FillCircle(72*1.25, 0, 72*1.25);
				GUI_SetBkColor(GUI_STCOLOR_LIGHTBLUE);
				GUI_SetColor(GUI_WHITE);  
				GUI_DrawBitmap(&bmReturn, (40-28)*1.25, 5*1.25);				
			}
			break;
			
		default:
			BUTTON_Callback(pMsg);
	}
}

/*
*********************************************************************************************************
*	函 数 名: _cbButtonSettings
*	功能说明: 按钮回调函数
*	形    参: pMsg  消息指针
*	返 回 值: 无
*********************************************************************************************************
*/
static void _cbButtonSettings(WM_MESSAGE * pMsg) 
{
	WM_HWIN  hWin;

	hWin  = pMsg->hWin;
	switch (pMsg->MsgId) 
	{
		case WM_PAINT:
			if (BUTTON_IsPressed(hWin)) 
			{
				GUI_SetColor(GUI_DARKGRAY);
				GUI_AA_FillCircle(0, 0, 72*1.25);
				GUI_SetBkColor(GUI_DARKGRAY);
				GUI_SetColor(GUI_WHITE); 
				
				GUI_DrawBitmap(&bmSettings, 0, 5*1.25);
			} 
			else 
			{
				GUI_SetColor(GUI_STCOLOR_LIGHTBLUE);
				GUI_AA_FillCircle(0, 0, 72*1.25);
				GUI_SetBkColor(GUI_STCOLOR_LIGHTBLUE);
				GUI_SetColor(GUI_WHITE);  

				GUI_DrawBitmap(&bmSettings, 0, 5*1.25);
			}
			break;
			
		default:
			BUTTON_Callback(pMsg);
	}
}

/*
*********************************************************************************************************
*					GUI_WIDGET_CREATE_INFO类型数组
*********************************************************************************************************
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreateMath[] = 
{
	{ WINDOW_CreateIndirect, "Math", ID_WINDOW_0,    0,  0, 800*1.28, 480*1.25, 0,  0x0, 0 },
	{ TEXT_CreateIndirect,   "Text",   ID_TEXT_0,  200*1.28, 10*1.25, 400*1.28,  32*1.25, 0, 0x64, 0 },
	 
	{ TEXT_CreateIndirect,  "Text",  ID_TEXT_1,   0, 100*1.25, 266*1.28,  30*1.25, 0, 0x64,   0 },
	{ RADIO_CreateIndirect, "Radio", ID_RADIO_0, 73*1.28, 140*1.25, 120*1.28, 240*1.25, 0, 0x1D08, 0 },
	
	{ TEXT_CreateIndirect,  "Text",  ID_TEXT_2,  267*1.28, 100*1.25, 266*1.28,  30*1.25, 0, 0x64,   0 },
	{ RADIO_CreateIndirect, "Radio", ID_RADIO_1, 340*1.28, 140*1.25, 120*1.28, 240*1.25, 0, 0x1D08, 0 },
	
	{ TEXT_CreateIndirect,  "Text",  ID_TEXT_3,  533*1.28, 100*1.25, 266*1.28, 30*1.25,  0,  0x64,  0 },
	{ RADIO_CreateIndirect, "Radio", ID_RADIO_2, 606*1.28, 140*1.25, 120*1.28, 240*1.25, 0, 0x1D08, 0 },

	{ BUTTON_CreateIndirect, "DlgBack",    ID_BUTTON_0,  728*1.28,  0,  72*1.28, 72*1.25, 0, 0, 0},	
	{ BUTTON_CreateIndirect, "MusicList",  ID_BUTTON_1,    0*1.28,  0,  72*1.28, 72*1.25, 0, 0, 0},
};

/*
*********************************************************************************************************
*	函 数 名: _cbDialogMath
*	功能说明: 测量对话框的回调函数
*	形    参：pMsg  指针地址         	
*	返 回 值: 无
*********************************************************************************************************
*/
static void _cbDialogMath(WM_MESSAGE * pMsg) 
{
	WM_HWIN hItem;
	int     NCode;
	int     Id;
	WM_HWIN hWin = pMsg->hWin;
	

	switch (pMsg->MsgId) 
	{
		/* 对话框关闭后使能一次波形刷新，防止在波形暂停状态下(通过K2按键将波形暂停)
		   关闭对话框并返回的时候，波形区域没有刷新，一片空白。
		*/
		case WM_DELETE:
			/* 自动触发暂停状态 */
			if(g_Flag->hWinRunStop == 1)
			{
				g_Flag->ucWaveRefresh = 1;
			}
		
			/* 普通触发暂停状态 */
			if(TriggerFlag == 1)
			{
				TriggerFlag = 2;
			}
			break;
		
	case WM_INIT_DIALOG:
		//
		// 初始化按钮
		//
		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
		WM_SetHasTrans(hItem);
		WM_SetCallback(hItem, _cbButtonBack);
		
		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1);
		WM_SetHasTrans(hItem);
		WM_SetCallback(hItem, _cbButtonSettings);
		
		//
		// 初始化ID_TEXT_0
		//
		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
		TEXT_SetFont(hItem, &GUI_FontFontSong26);
		TEXT_SetTextColor(hItem, GUI_RED);
		TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
		TEXT_SetText(hItem, "80阶Fir滤波");
		
		//
		// 初始化ID_TEXT_1
		//
		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
		TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
		TEXT_SetText(hItem, "Fs=2Msps,Scale=500ns");
		TEXT_SetFont(hItem, &GUI_Font24_1);
		
		//
		// 初始化ID_TEXT_2
		//
		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_2);
		TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
		TEXT_SetText(hItem, "Fs=200Ksps,Scale=10us");
		TEXT_SetFont(hItem, &GUI_Font24_1);
		
		//
		// 初始化ID_TEXT_3
		//
		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_3);
		TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
		TEXT_SetText(hItem, "Fs=20Ksps,Scale=100us");
		TEXT_SetFont(hItem, &GUI_Font24_1);
		
		//
		// 初始化ID_RADIO_0
		//
		hItem = WM_GetDialogItem(pMsg->hWin, ID_RADIO_0);
		RADIO_SetText(hItem, "None", 0);
		RADIO_SetFont(hItem, &GUI_Font20_ASCII);
		RADIO_SetText(hItem, "Fc=100KHz", 1);
		RADIO_SetText(hItem, "Fc=200KHz", 2);
		RADIO_SetText(hItem, "Fc=300KHz", 3);
		RADIO_SetText(hItem, "Fc=400KHz", 4);
		RADIO_SetText(hItem, "Fc=500KHz", 5);
		RADIO_SetText(hItem, "Fc=600KHz", 6);
		RADIO_SetText(hItem, "Fc=700KHz", 7);
		RADIO_SetValue(hItem, Radio_Step100KHz); /* 设置默认状态 */
		//WIDGET_SetFocusable(hItem, 0);
		
		//
		// 初始化ID_RADIO_1
		//
		hItem = WM_GetDialogItem(pMsg->hWin, ID_RADIO_1);
		RADIO_SetText(hItem, "None", 0);
		RADIO_SetText(hItem, "Fc=10KHz", 1);
		RADIO_SetFont(hItem, &GUI_Font20_ASCII);
		RADIO_SetText(hItem, "Fc=20KHz", 2);
		RADIO_SetText(hItem, "Fc=30KHz", 3);
		RADIO_SetText(hItem, "Fc=40KHz", 4);
		RADIO_SetText(hItem, "Fc=50KHz", 5);
		RADIO_SetText(hItem, "Fc=60KHz", 6);
		RADIO_SetText(hItem, "Fc=70KHz", 7);
		RADIO_SetValue(hItem, Radio_Step10KHz); /* 设置默认状态 */
		//WIDGET_SetFocusable(hItem, 0);
		
		//
		// 初始化ID_RADIO_2
		//
		hItem = WM_GetDialogItem(pMsg->hWin, ID_RADIO_2);
		RADIO_SetText(hItem, "None", 0);
		RADIO_SetText(hItem, "Fc=1KHz", 1);
		RADIO_SetFont(hItem, &GUI_Font20_1);
		RADIO_SetText(hItem, "Fc=2KHz", 2);
		RADIO_SetText(hItem, "Fc=3KHz", 3);
		RADIO_SetText(hItem, "Fc=4KHz", 4);
		RADIO_SetText(hItem, "Fc=5KHz", 5);
		RADIO_SetText(hItem, "Fc=6KHz", 6);
		RADIO_SetText(hItem, "Fc=7KHz", 7);
		RADIO_SetValue(hItem, Radio_Step1KHz); /* 设置默认状态 */
		//WIDGET_SetFocusable(hItem, 0);
		break;
		
	case WM_NOTIFY_PARENT:
		Id    = WM_GetId(pMsg->hWinSrc);
		NCode = pMsg->Data.v;
		switch(Id) 
		{
			/* 关闭对话框 */
			case ID_BUTTON_0:
				switch(NCode)
				{
					case WM_NOTIFICATION_RELEASED:
						GUI_EndDialog(hWin, 0);
						break;
				}
				break;
				
			/* 单选按钮消息 */
			case ID_RADIO_0: 
				switch(NCode) 
				{
					case WM_NOTIFICATION_CLICKED:
						break;

					case WM_NOTIFICATION_RELEASED:
						break;

					case WM_NOTIFICATION_VALUE_CHANGED:
						/* Fs(采样率) = 2Msps时，即Scale窗口显示500ns时，Fc(截止频率)的选择 */
						Radio_Step100KHz = RADIO_GetValue( WM_GetDialogItem(pMsg->hWin, ID_RADIO_0));
						g_DSO1->ucFirFlter_Step100KHz = Radio_Step100KHz;
						break;
				}
				break;
				
			/* 单选按钮消息 */
			case ID_RADIO_1: 
				switch(NCode) 
				{
					case WM_NOTIFICATION_CLICKED:
						break;

					case WM_NOTIFICATION_RELEASED:
						break;

					case WM_NOTIFICATION_VALUE_CHANGED:
						/* Fs(采样率) = 200Ksps时，即Scale窗口显示5us时，Fc(截止频率)的选择 */
						Radio_Step10KHz = RADIO_GetValue( WM_GetDialogItem(pMsg->hWin, ID_RADIO_1));
						g_DSO1->ucFirFlter_Step10KHz = Radio_Step10KHz;
						break;
				}
				break;
				
			/* 单选按钮消息 */
			case ID_RADIO_2: 
				switch(NCode) 
				{
					case WM_NOTIFICATION_CLICKED:
						break;

					case WM_NOTIFICATION_RELEASED:
						break;

					case WM_NOTIFICATION_VALUE_CHANGED:
						/* Fs(采样率) = 20Ksps时，即Scale窗口显示50us时，Fc(截止频率)的选择 */
						Radio_Step1KHz = RADIO_GetValue( WM_GetDialogItem(pMsg->hWin, ID_RADIO_2));
						g_DSO1->ucFirFlter_Step1KHz = Radio_Step1KHz;
						break;
				}
				break;
		}
		break;
		
	default:
		WM_DefaultProc(pMsg);
		break;
	}
}

/*
*********************************************************************************************************
*	函 数 名: DSO_CreateMathDlg
*	功能说明: 创建对话框
*	形    参: 无        	
*	返 回 值: 无
*********************************************************************************************************
*/
WM_HWIN DSO_CreateMathDlg(void) 
{
	WM_HWIN hWin;

	hWin = GUI_CreateDialogBox(_aDialogCreateMath, GUI_COUNTOF(_aDialogCreateMath), _cbDialogMath, WM_HBKWIN, 0, 0);
	
	return hWin;
}

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
