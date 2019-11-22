/*
*********************************************************************************************************
*	                                  
*	模块名称 : 示波器设置对话框
*	文件名称 : DSO_SettingDlg.c
*	版    本 : V1.0
*	说    明 : 主要设置如下5个配置
*              1. 设置是否隐藏幅值窗口，状态窗口，时基窗口和系统窗口。
*              2. 设置是否隐藏FFT计算的幅频响应波形。
*              3. 设置水平和垂直测量游标的显示。
*              4. 设置水平和垂直测量游标的移动步数。
*              5. 设置水平滑动还是垂直滑动。
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
#define ID_WINDOW_0  	(GUI_ID_USER + 0x00)
#define ID_CHECKBOX_0   (GUI_ID_USER + 0x01)
#define ID_CHECKBOX_1   (GUI_ID_USER + 0x02)
#define ID_RADIO_0  	(GUI_ID_USER + 0x03)
#define ID_RADIO_1  	(GUI_ID_USER + 0x04)
#define ID_RADIO_2  	(GUI_ID_USER + 0x05)
#define ID_BUTTON_0  	(GUI_ID_USER + 0x06)
#define ID_BUTTON_1  	(GUI_ID_USER + 0x07)
#define ID_TEXT_0  		(GUI_ID_USER + 0x08)


/*
*********************************************************************************************************
*	                                  变量
*********************************************************************************************************
*/
static uint32_t CheckBoxHideFFT = 0;     /* 第一次打开时默认未隐藏FFT波形显示 */
static uint32_t CheckBoxHideWindow = 0;  /* 第一次打开时默认未隐藏窗口 */
static uint32_t RadioCursorDisp = 0;     /* 第一次打开时默认不显示水平和垂直测量游标 */
static uint32_t RadioCursorStep = 3;     /* 第一次打开时默认选择水平和垂直游标都是移动10个像素 */


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
				GUI_AA_FillCircle(72*1.28, 0, 72*1.28);
				GUI_SetBkColor(GUI_DARKGRAY);
				GUI_SetColor(GUI_WHITE); 
				GUI_DrawBitmap(&bmReturn, (40-28)*1.28, 5*1.28);
			} 
			else 
			{
				GUI_SetColor(GUI_STCOLOR_LIGHTBLUE);
				GUI_AA_FillCircle(72*1.28, 0, 72*1.28);
				GUI_SetBkColor(GUI_STCOLOR_LIGHTBLUE);
				GUI_SetColor(GUI_WHITE);  
				GUI_DrawBitmap(&bmReturn, (40-28)*1.28, 5*1.28);				
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
				GUI_AA_FillCircle(0, 0, 72*1.28);
				GUI_SetBkColor(GUI_DARKGRAY);
				GUI_SetColor(GUI_WHITE); 
				
				GUI_DrawBitmap(&bmSettings, 0, 5*1.28);
			} 
			else 
			{
				GUI_SetColor(GUI_STCOLOR_LIGHTBLUE);
				GUI_AA_FillCircle(0, 0, 72*1.28);
				GUI_SetBkColor(GUI_STCOLOR_LIGHTBLUE);
				GUI_SetColor(GUI_WHITE);  

				GUI_DrawBitmap(&bmSettings, 0, 5*1.28);
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
static const GUI_WIDGET_CREATE_INFO _aDialogCreateSettings[] = 
{
	{ WINDOW_CreateIndirect,   "Window",   ID_WINDOW_0, 0, 0, 800*1.28, 480*1.25, 0, 0x64, 0 },

	{ RADIO_CreateIndirect,    "Radio",    ID_RADIO_0,    10*1.28,  100*1.25, 250*1.28,  60*1.25, 0, 0x1D02, 0 },	
	{ RADIO_CreateIndirect,    "Radio",    ID_RADIO_1,    10*1.28,  190*1.25, 250*1.28,  90*1.25, 0, 0x1D03, 0 },
	{ RADIO_CreateIndirect,    "Radio",    ID_RADIO_2,    400*1.28, 100*1.25, 250*1.28, 120*1.25, 0, 0x1D04, 0 },

	{ CHECKBOX_CreateIndirect, "Checkbox", ID_CHECKBOX_0, 20*1.28, 310*1.25, 700*1.28, 25*1.25, 0, 0x0, 0 },	
	{ CHECKBOX_CreateIndirect, "Checkbox", ID_CHECKBOX_1, 20*1.28, 350*1.25, 700*1.28, 25*1.25, 0, 0x0, 0 },
	
	{ BUTTON_CreateIndirect, "DlgBack",    ID_BUTTON_0,   728*1.28,    0,  72*1.28, 72*1.25, 0, 0, 0},	
	{ BUTTON_CreateIndirect, "MusicList",  ID_BUTTON_1,     0,    0,  72*1.28, 72*1.25, 0, 0, 0},
	
	{ TEXT_CreateIndirect,   "Text", ID_TEXT_0,  200*1.28, 10*1.25, 400*1.28, 32*1.25, 0, 0x64, 0 },
};

/*
*********************************************************************************************************
*	函 数 名: _cbDialogSettings
*	功能说明: 设置对话框的回调函数
*	形    参: pMsg  指针地址         	
*	返 回 值: 无
*********************************************************************************************************
*/
static void _cbDialogSettings(WM_MESSAGE * pMsg) 
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
			// 初始化ID_RADIO_0
			//
			hItem = WM_GetDialogItem(pMsg->hWin, ID_RADIO_0);
			RADIO_SetText(hItem, "Y平移", 0);
			RADIO_SetText(hItem, "X平移", 1);
			RADIO_SetFont(hItem, &GUI_FontFontSong20);
			RADIO_SetValue(hItem, g_Flag->ucMotionXY);  /* 设置默认状态 */

			//
			// 初始化ID_RADIO_1
			//
			hItem = WM_GetDialogItem(pMsg->hWin, ID_RADIO_1);
			RADIO_SetText(hItem, "隐藏 游标", 0);
			RADIO_SetText(hItem, "隐藏 水平游标", 1);
			RADIO_SetText(hItem, "隐藏 垂直游标", 2);
			RADIO_SetFont(hItem, &GUI_FontFontSong20);
			RADIO_SetValue(hItem, RadioCursorDisp); /* 设置默认状态 */

			//
			// 初始化ID_RADIO_2
			//
			hItem = WM_GetDialogItem(pMsg->hWin, ID_RADIO_2);
			RADIO_SetText(hItem, "步进 = 1", 0);
			RADIO_SetText(hItem, "步进 = 2", 1);
			RADIO_SetText(hItem, "步进 = 5", 2);
			RADIO_SetText(hItem, "步进 = 10", 3);
			RADIO_SetFont(hItem, &GUI_FontFontSong20);
			RADIO_SetValue(hItem, RadioCursorStep);  /* 设置默认状态 */

			//
			// 初始化ID_CHECKBOX_0
			//
			hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_0);
			CHECKBOX_SetText(hItem, "显示 RFFT2048 波形");
			CHECKBOX_SetFont(hItem, &GUI_FontFontSong26);
			CHECKBOX_SetState(hItem, CheckBoxHideFFT); /* 设置默认状态 */
			
			//
			// 初始化ID_CHECKBOX_1
			//
			hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_1);
			CHECKBOX_SetText(hItem, "隐藏 幅度界面/状态界面/扫描界面/状态界面");
			CHECKBOX_SetFont(hItem, &GUI_FontFontSong26);
			CHECKBOX_SetState(hItem, CheckBoxHideWindow); /* 设置默认状态 */
			
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
			// 初始化文本
			//			
			hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
			TEXT_SetFont(hItem, &GUI_FontFontSong26);
			TEXT_SetTextColor(hItem, GUI_RED);
			TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
			TEXT_SetText(hItem, "设置");
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
						case WM_NOTIFICATION_CLICKED:
							GUI_EndDialog(hWin, 0);
							break;
					}
					break;

				/* 复选框消息 */
				case ID_CHECKBOX_0: 
					switch(NCode) 
					{


						
						/* 复选框点击释放后消息 */
						case WM_NOTIFICATION_CLICKED:
							CheckBoxHideFFT = CHECKBOX_GetState(WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_0));
							g_Flag->ucFFTDisp = CheckBoxHideFFT;
							break;

					}
					break;
					
				/* 复选框消息 */
				case ID_CHECKBOX_1: 
					switch(NCode) 
					{
						/* 复选框点击消息 */
//						case WM_NOTIFICATION_CLICKED:
//							break;
						
						/* 复选框点击释放后消息 */
						case WM_NOTIFICATION_CLICKED:
							CheckBoxHideWindow = CHECKBOX_GetState(WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_1));
							/* 选中复选框则隐藏幅值窗口，状态窗口和时基窗口 */
							if(CheckBoxHideWindow == 1)
							{
								WM_HideWindow(hDlgAmp);
								WM_HideWindow(hDlgStatus1);
								WM_HideWindow(hDlgStatus2);
								WM_HideWindow(hDlgScale);
								WM_HideWindow(hDlgSysInfo);
							}
							/* 不选中则显示出来 */
							else
							{
								WM_ShowWindow(hDlgAmp);
								WM_ShowWindow(hDlgStatus1);
								WM_ShowWindow(hDlgStatus2);
								WM_ShowWindow(hDlgScale);
								WM_ShowWindow(hDlgSysInfo);
							}
							break;

						case WM_NOTIFICATION_VALUE_CHANGED:
							break;
					}
					break;

				/* 单选按钮消息 */
				case ID_RADIO_0: 
					switch(NCode) 
					{
						case WM_NOTIFICATION_CLICKED:
							/* 水平和垂直滑动切换标志赋值给g_Flag->ucMotionXY */
							g_Flag->ucMotionXY = RADIO_GetValue( WM_GetDialogItem(pMsg->hWin, ID_RADIO_0));
							break;
					}
					break;
					
				/* 单选按钮消息 */
				case ID_RADIO_1: 
					switch(NCode) 
					{
						case WM_NOTIFICATION_CLICKED:
							/* 将水平和垂直游标的状态赋值给全局变量g_Flag->hWinCursors */
							RadioCursorDisp = RADIO_GetValue( WM_GetDialogItem(pMsg->hWin, ID_RADIO_1));
							g_Flag->hWinCursors = RadioCursorDisp;
							break;
					}
					break;
				
				/* 单选按钮状态 */
				case ID_RADIO_2: 
					switch(NCode) 
					{
						case WM_NOTIFICATION_CLICKED:
							break;

						case WM_NOTIFICATION_RELEASED:
							break;

						case WM_NOTIFICATION_VALUE_CHANGED:
						/* 水平和垂直游标的移动步数赋值给全局变量 */
						RadioCursorStep = RADIO_GetValue( WM_GetDialogItem(pMsg->hWin, ID_RADIO_2));
						if(RadioCursorStep == 0)
						{
							/* 每次移动1步 */
							g_Cursors->usCursorStep = 1;
						}
						else if(RadioCursorStep == 1)
						{
							/* 每次移动2步 */
							g_Cursors->usCursorStep = 2;			
						}
						else if(RadioCursorStep == 2)
						{
							/* 每次移动5步 */
							g_Cursors->usCursorStep = 5;			
						}
						else if(RadioCursorStep == 3)
						{
							/* 每次移动10步 */
							g_Cursors->usCursorStep = 10;			
						}
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
*	函 数 名: DSO_CreateSettingsDlg
*	功能说明: 创建设置对话框
*	形    参: 无        	
*	返 回 值: 无
*********************************************************************************************************
*/
WM_HWIN DSO_CreateSettingsDlg(void) 
{
	WM_HWIN hWin;

	hWin = GUI_CreateDialogBox(_aDialogCreateSettings, GUI_COUNTOF(_aDialogCreateSettings), _cbDialogSettings, WM_HBKWIN, 0, 0);
	return hWin;
}

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
