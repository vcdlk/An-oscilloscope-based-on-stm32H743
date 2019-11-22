/*
*********************************************************************************************************
*	                                  
*	模块名称 : 示波器测量对话框
*	文件名称 : DSO_MeasureDlg.c
*	版    本 : V1.0
*	说    明 : 支持30种数据测量功能，当前实现平均值，峰峰值，最小值，最大值，RMS和频率6种数值测量。
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
*	                                  测量类型
*********************************************************************************************************
*/
const char *g_MeasureText[] =
{
	"Snapshot All",
	"Freqency",
	"Period",
	"Rise Time",
	"Fall Time",
	"Delay",
	"Phase",
	"Positive Pulse Width",
	"Negative Pulse Width",
	"Positive Duty Cycle",
	"Negative Duty Cycle",
	"Burst Width",
	"Peak-to-peak",
	"Amplitude",
	"Max",
	"Min",
	"High",
	"Low",
	"Positive Overshoot",
	"Negative Overshoot",
	"Positive Pulse Count",
	"Negative Pulse Count",
	"RMS",
	"Cycle RMS",
	"Mean",
	"Cycle Mean",
	"Rising Edge Count",
	"Falling Edge Count",
	"Area",
	"Cycle Area",
};

/*
*********************************************************************************************************
*	                                  自定义控件ID
*********************************************************************************************************
*/
#define ID_WINDOW_0  	(GUI_ID_USER + 0x00)
#define ID_CHECKBOX_0  	(GUI_ID_USER + 0x01)
#define ID_CHECKBOX_1  	(GUI_ID_USER + 0x02)
#define ID_CHECKBOX_2  	(GUI_ID_USER + 0x03)
#define ID_CHECKBOX_3  	(GUI_ID_USER + 0x04)
#define ID_CHECKBOX_4  	(GUI_ID_USER + 0x05)
#define ID_CHECKBOX_5  	(GUI_ID_USER + 0x06)
#define ID_CHECKBOX_6  	(GUI_ID_USER + 0x07)
#define ID_CHECKBOX_7  	(GUI_ID_USER + 0x08)
#define ID_CHECKBOX_8  	(GUI_ID_USER + 0x09)
#define ID_CHECKBOX_9  	(GUI_ID_USER + 0x0A)
#define ID_CHECKBOX_10  (GUI_ID_USER + 0x0B)
#define ID_CHECKBOX_11  (GUI_ID_USER + 0x0C)
#define ID_CHECKBOX_12  (GUI_ID_USER + 0x0D)
#define ID_CHECKBOX_13  (GUI_ID_USER + 0x0E)
#define ID_CHECKBOX_14  (GUI_ID_USER + 0x0F)
#define ID_CHECKBOX_15  (GUI_ID_USER + 0x10)
#define ID_CHECKBOX_16  (GUI_ID_USER + 0x11)
#define ID_CHECKBOX_17  (GUI_ID_USER + 0x12)
#define ID_CHECKBOX_18  (GUI_ID_USER + 0x13)
#define ID_CHECKBOX_19  (GUI_ID_USER + 0x14)
#define ID_CHECKBOX_20  (GUI_ID_USER + 0x15)
#define ID_CHECKBOX_21  (GUI_ID_USER + 0x16)
#define ID_CHECKBOX_22  (GUI_ID_USER + 0x17)
#define ID_CHECKBOX_23  (GUI_ID_USER + 0x18)
#define ID_CHECKBOX_24  (GUI_ID_USER + 0x19)
#define ID_CHECKBOX_25  (GUI_ID_USER + 0x1A)
#define ID_CHECKBOX_26  (GUI_ID_USER + 0x1B)
#define ID_CHECKBOX_27  (GUI_ID_USER + 0x1C)
#define ID_CHECKBOX_28  (GUI_ID_USER + 0x1D)
#define ID_CHECKBOX_29  (GUI_ID_USER + 0x1E)

#define ID_BUTTON_0  	(GUI_ID_USER + 0x1F)
#define ID_BUTTON_1  	(GUI_ID_USER + 0x20)

#define ID_TEXT_0  		(GUI_ID_USER + 0x21)

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
				GUI_DrawBitmap(&bmReturn, (40-28)*1.28, 5*1.25);			
			} 
			else 
			{
				GUI_SetColor(GUI_STCOLOR_LIGHTBLUE);
				GUI_AA_FillCircle(72*1.25, 0, 72*1.25);
				GUI_SetBkColor(GUI_STCOLOR_LIGHTBLUE);
				GUI_SetColor(GUI_WHITE);  
				GUI_DrawBitmap(&bmReturn, (40-28)*1.28, 5*1.25);				
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
				GUI_AA_FillCircle(0, 0, 72*1.28);
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
static const GUI_WIDGET_CREATE_INFO _aDialogCreateMeasure[] = 
{
	{ WINDOW_CreateIndirect,   "Window",   ID_WINDOW_0,  0,  0,  800*1.28, 480*1.25, 0, 0x64, 0 },
	
	{ CHECKBOX_CreateIndirect, "Checkbox", ID_CHECKBOX_0, 50*1.28,  80*1.25, 200*1.28, 25*1.25, 0, 0x0, 0 },
	{ CHECKBOX_CreateIndirect, "Checkbox", ID_CHECKBOX_1, 50*1.28, 120*1.25, 200*1.28, 25*1.25, 0, 0x0, 0 },
	{ CHECKBOX_CreateIndirect, "Checkbox", ID_CHECKBOX_2, 50*1.28, 160*1.25, 200*1.28, 25*1.25, 0, 0x0, 0 },
	{ CHECKBOX_CreateIndirect, "Checkbox", ID_CHECKBOX_3, 50*1.28, 200*1.25, 200*1.28, 25*1.25, 0, 0x0, 0 },
	{ CHECKBOX_CreateIndirect, "Checkbox", ID_CHECKBOX_4, 50*1.28, 240*1.25, 200*1.28, 25*1.25, 0, 0x0, 0 },
	{ CHECKBOX_CreateIndirect, "Checkbox", ID_CHECKBOX_5, 50*1.28, 280*1.25, 200*1.28, 25*1.25, 0, 0x0, 0 },
	{ CHECKBOX_CreateIndirect, "Checkbox", ID_CHECKBOX_6, 50*1.28, 320*1.25, 200*1.28, 25*1.25, 0, 0x0, 0 },
	{ CHECKBOX_CreateIndirect, "Checkbox", ID_CHECKBOX_7, 50*1.28, 360*1.25, 200*1.28, 25*1.25, 0, 0x0, 0 },
	{ CHECKBOX_CreateIndirect, "Checkbox", ID_CHECKBOX_8, 50*1.28, 400*1.25, 200*1.28, 25*1.25, 0, 0x0, 0 },
	{ CHECKBOX_CreateIndirect, "Checkbox", ID_CHECKBOX_9, 50*1.28, 440*1.25, 200*1.28, 25*1.25, 0, 0x0, 0 },
	
	{ CHECKBOX_CreateIndirect, "Checkbox", ID_CHECKBOX_10, 300*1.28, 80*1.25, 200*1.28, 25*1.25, 0, 0x0, 0 },
	{ CHECKBOX_CreateIndirect, "Checkbox", ID_CHECKBOX_11, 300*1.28, 120*1.25, 200*1.28, 25*1.25, 0, 0x0, 0 },
	{ CHECKBOX_CreateIndirect, "Checkbox", ID_CHECKBOX_12, 300*1.28, 160*1.25, 200*1.28, 25*1.25, 0, 0x0, 0 },
	{ CHECKBOX_CreateIndirect, "Checkbox", ID_CHECKBOX_13, 300*1.28, 200*1.25, 200*1.28, 25*1.25, 0, 0x0, 0 },
	{ CHECKBOX_CreateIndirect, "Checkbox", ID_CHECKBOX_14, 300*1.28, 240*1.25, 200*1.28, 25*1.25, 0, 0x0, 0 },
	{ CHECKBOX_CreateIndirect, "Checkbox", ID_CHECKBOX_15, 300*1.28, 280*1.25, 200*1.28, 25*1.25, 0, 0x0, 0 },
	{ CHECKBOX_CreateIndirect, "Checkbox", ID_CHECKBOX_16, 300*1.28, 320*1.25, 200*1.28, 25*1.25, 0, 0x0, 0 },
	{ CHECKBOX_CreateIndirect, "Checkbox", ID_CHECKBOX_17, 300*1.28, 360*1.25, 200*1.28, 25*1.25, 0, 0x0, 0 },
	{ CHECKBOX_CreateIndirect, "Checkbox", ID_CHECKBOX_18, 300*1.28, 400*1.25, 200*1.28, 25*1.25, 0, 0x0, 0 },
	{ CHECKBOX_CreateIndirect, "Checkbox", ID_CHECKBOX_19, 300*1.28, 440*1.25, 200*1.28, 25*1.25, 0, 0x0, 0 },
	
	{ CHECKBOX_CreateIndirect, "Checkbox", ID_CHECKBOX_20, 550*1.28, 80*1.25, 200*1.28, 25*1.25, 0, 0x0, 0 },
	{ CHECKBOX_CreateIndirect, "Checkbox", ID_CHECKBOX_21, 550*1.28, 120*1.25, 200*1.28, 25*1.25, 0, 0x0, 0 },
	{ CHECKBOX_CreateIndirect, "Checkbox", ID_CHECKBOX_22, 550*1.28, 160*1.25, 200*1.28, 25*1.25, 0, 0x0, 0 },
	{ CHECKBOX_CreateIndirect, "Checkbox", ID_CHECKBOX_23, 550*1.28, 200*1.25, 200*1.28, 25*1.25, 0, 0x0, 0 },
	{ CHECKBOX_CreateIndirect, "Checkbox", ID_CHECKBOX_24, 550*1.28, 240*1.25, 200*1.28, 25*1.25, 0, 0x0, 0 },
	{ CHECKBOX_CreateIndirect, "Checkbox", ID_CHECKBOX_25, 550*1.28, 280*1.25, 200*1.28, 25*1.25, 0, 0x0, 0 },
	{ CHECKBOX_CreateIndirect, "Checkbox", ID_CHECKBOX_26, 550*1.28, 320*1.25, 200*1.28, 25*1.25, 0, 0x0, 0 },
	{ CHECKBOX_CreateIndirect, "Checkbox", ID_CHECKBOX_27, 550*1.28, 360*1.25, 200*1.28, 25*1.25, 0, 0x0, 0 },
	{ CHECKBOX_CreateIndirect, "Checkbox", ID_CHECKBOX_28, 550*1.28, 400*1.25, 200*1.28, 25*1.25, 0, 0x0, 0 },
	{ CHECKBOX_CreateIndirect, "Checkbox", ID_CHECKBOX_29, 550*1.28, 440*1.25, 200*1.28, 25*1.25, 0, 0x0, 0 },
	
	{ BUTTON_CreateIndirect, "DlgBack",     ID_BUTTON_0,   728*1.28,    0,  72*1.28, 72*1.25, 0, 0, 0},	
	{ BUTTON_CreateIndirect, "MusicList",   ID_BUTTON_1,     0*1.28,    0,  72*1.28, 72*1.25, 0, 0, 0},
	
	{ TEXT_CreateIndirect,   "Text",        ID_TEXT_0,     200*1.28,   10, 400*1.28, 32*1.25, 0, 0x64, 0 },
};

/*
*********************************************************************************************************
*	函 数 名: _cbDialogMeasure
*	功能说明: 测量对话框的回调函数
*	形    参：pMsg  指针地址         	
*	返 回 值: 无
*********************************************************************************************************
*/
static void _cbDialogMeasure(WM_MESSAGE * pMsg) 
{
	WM_HWIN hItem;
	int     NCode;
	int     Id;
	int     i;
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
			// 初始化单选框
			//
			for(i = 0; i < 30; i++)
			{
				hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_0 + i);
				CHECKBOX_SetText(hItem, g_MeasureText[i]);
				CHECKBOX_SetFont(hItem, &GUI_Font20_1);
				CHECKBOX_SetState(hItem, g_DSO1->ucMeasureFlag[i]);
				if(g_DSO1->ucMeasureFlag[i] == 1)
				{
					CHECKBOX_SetTextColor(hItem, GUI_RED);    /* 标成红体表示当前支持的测量 */
				}
				else
				{
					CHECKBOX_SetTextColor(hItem, GUI_BLACK);  /* 标成黑色表示当前支持的测量 */				
				}
			}
			
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
			// 初始化文本控件
			//
			hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
			TEXT_SetFont(hItem, &GUI_FontFontSong26);
			TEXT_SetTextColor(hItem, GUI_RED);
			TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
			TEXT_SetText(hItem, "波形 测量");
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
				case ID_CHECKBOX_1: 
				case ID_CHECKBOX_2: 
				case ID_CHECKBOX_3: 
				case ID_CHECKBOX_4: 
				case ID_CHECKBOX_5: 
				case ID_CHECKBOX_6: 
				case ID_CHECKBOX_7: 
				case ID_CHECKBOX_8: 
				case ID_CHECKBOX_9: 
				case ID_CHECKBOX_10: 
				case ID_CHECKBOX_11: 
				case ID_CHECKBOX_12: 
				case ID_CHECKBOX_13: 
				case ID_CHECKBOX_14: 
				case ID_CHECKBOX_15: 
				case ID_CHECKBOX_16: 
				case ID_CHECKBOX_17: 
				case ID_CHECKBOX_18: 
				case ID_CHECKBOX_19: 
				case ID_CHECKBOX_20: 
				case ID_CHECKBOX_21: 
				case ID_CHECKBOX_22: 
				case ID_CHECKBOX_23: 
				case ID_CHECKBOX_24: 
				case ID_CHECKBOX_25: 
				case ID_CHECKBOX_26: 
				case ID_CHECKBOX_27: 
				case ID_CHECKBOX_28: 
				case ID_CHECKBOX_29:				
					switch(NCode) 
					{
						/* 复选框点击消息 */
//						case WM_NOTIFICATION_CLICKED:
//							break;
						
						/* 复选框释放消息 */
						case WM_NOTIFICATION_CLICKED:
							g_DSO1->ucMeasureFlag[Id - ID_CHECKBOX_0] = CHECKBOX_GetState(WM_GetDialogItem(pMsg->hWin, Id));
							break;
						
						/* 复选框状态改变消息 */
						case WM_NOTIFICATION_VALUE_CHANGED:
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
*	函 数 名: DSO_CreateMeasureDlg
*	功能说明: 创建测量对话框
*	形    参: 无        	
*	返 回 值: 无
*********************************************************************************************************
*/
WM_HWIN DSO_CreateMeasureDlg(void) 
{
	WM_HWIN hWin;

	hWin = GUI_CreateDialogBox(_aDialogCreateMeasure, GUI_COUNTOF(_aDialogCreateMeasure), _cbDialogMeasure, WM_HBKWIN, 0, 0);
	return hWin;
}

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
