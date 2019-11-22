/*
*********************************************************************************************************
*	                                  
*	模块名称 : 示波器主界面上的时基窗口
*	文件名称 : DSO_ScaleDlg.c
*	版    本 : V1.0
*	说    明 : 显示示波器当前的时基
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
*	                                  变量
*********************************************************************************************************
*/
static GUI_MEMDEV_Handle   hMemScaleDlg;


/*
*********************************************************************************************************
*				                      宏定义
*********************************************************************************************************
*/
#define ID_TEXT_0 		(GUI_ID_USER + 0x00)
#define ID_TEXT_1  		(GUI_ID_USER + 0x01)
#define ID_TEXT_2 		(GUI_ID_USER + 0x02)
#define ID_TEXT_3 		(GUI_ID_USER + 0x03)


/*
*********************************************************************************************************
*					GUI_WIDGET_CREATE_INFO类型数组
*********************************************************************************************************
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreateScale[] = 
{ 
    { WINDOW_CreateIndirect,   "",       0,      663,  444,  135,  35, 0},
	
	{ TEXT_CreateIndirect,     "",             ID_TEXT_0, 35,  2,  0,  16, 0, 0},
	{ TEXT_CreateIndirect,     "2Msps 500ns",  ID_TEXT_1, 35,  2, 90,  16, 0, 0},
	
	{ TEXT_CreateIndirect,     "",             ID_TEXT_2, 35, 17,  0,  16, 0, 0},
	{ TEXT_CreateIndirect,     "2Msps 500ns",  ID_TEXT_3, 35, 17,  90, 16, 0, 0},
};

/*
*********************************************************************************************************
*	函 数 名: PaintDialogScale()
*	功能说明: 时基窗口的回调函数重绘
*	形    参：pMsg  指针地址         	
*	返 回 值: 无
*********************************************************************************************************
*/
void PaintDialogScale(WM_MESSAGE * pMsg)
{
	/* 清背景色 */	
	GUI_SetBkColor(0x905040);
	GUI_Clear();

	/* 绘制填充的抗锯齿圆角矩形 */		
	GUI_SetColor(GUI_BLACK);
	GUI_AA_FillRoundedRect(0, 0, 134, 34, 10);

	/* 绘制抗锯齿圆角矩形 */	
	GUI_SetColor(0XEBCD9E);
	GUI_SetPenSize(2);
	GUI_AA_DrawRoundedRect(0, 0, 134, 34, 10);

	/* 绘制抗锯齿圆角矩形，并填数值1 */	
	GUI_SetColor(GUI_YELLOW);
	GUI_AA_FillRoundedRect(10, 4, 30, 16, 3);
	
	GUI_SetColor(GUI_BLACK);
    GUI_SetFont(&GUI_Font16_1);
	GUI_SetTextMode(GUI_TEXTMODE_TRANS);					
	GUI_DispCharAt('1', 16, 3);

	/* 绘制抗锯齿圆角矩形，并填数值2 */
	GUI_SetColor(GUI_GREEN);
	GUI_AA_FillRoundedRect(10, 19, 30, 31, 3);
						   
	GUI_SetColor(GUI_BLACK);
    GUI_SetFont(&GUI_Font16_1);
	GUI_SetTextMode(GUI_TEXTMODE_TRANS);					
	GUI_DispCharAt('2', 16, 18);
}

/*
*********************************************************************************************************
*	函 数 名: InitDialogScale
*	功能说明: 时基窗口的回调函数的初始化
*	形    参: pMsg  指针地址         	
*	返 回 值: 无
*********************************************************************************************************
*/
void InitDialogScale(WM_MESSAGE * pMsg)
{
    WM_HWIN hWin = pMsg->hWin;

    //
    // 初始化文本控件
    //
    TEXT_SetTextColor(WM_GetDialogItem(hWin, ID_TEXT_0), 0x00ffff);
    TEXT_SetFont(WM_GetDialogItem(hWin, ID_TEXT_0),&GUI_Font16_1);
	
	TEXT_SetTextColor(WM_GetDialogItem(hWin, ID_TEXT_1), 0x00ffff);
    TEXT_SetFont(WM_GetDialogItem(hWin, ID_TEXT_1), &GUI_Font16_1);
	
	TEXT_SetTextColor(WM_GetDialogItem(hWin, ID_TEXT_2), GUI_GREEN);
    TEXT_SetFont(WM_GetDialogItem(hWin, ID_TEXT_2), &GUI_Font16_1);
	
	TEXT_SetTextColor(WM_GetDialogItem(hWin, ID_TEXT_3), GUI_GREEN);
    TEXT_SetFont(WM_GetDialogItem(hWin, ID_TEXT_3), &GUI_Font16_1);
}

/*
*********************************************************************************************************
*	函 数 名: _cbCallbackScale()
*	功能说明: 时基窗口的回调函数
*	形    参: pMsg  指针地址         	
*	返 回 值: 无
*********************************************************************************************************
*/
static void _cbCallbackScale(WM_MESSAGE * pMsg) 
{
    WM_HWIN hWin = pMsg->hWin;
	
    switch (pMsg->MsgId) 
    {
		case WM_INIT_DIALOG:
			InitDialogScale(pMsg);
			break;

		case WM_TextUpDate:
			TEXT_SetText(WM_GetDialogItem(hWin, ID_TEXT_1), g_TimeTable[TimeBaseId]);
			TEXT_SetText(WM_GetDialogItem(hWin, ID_TEXT_3), g_TimeTable[TimeBaseId]);
			break;

		case WM_PAINT:
			GUI_MEMDEV_WriteAt(hMemScaleDlg, 663, 444);
			break;

		default:
			WM_DefaultProc(pMsg);
    }
}

/*
*********************************************************************************************************
*	函 数 名: CreateWindowScale
*	功能说明: 时基窗口的回调函数
*	形    参：pMsg  指针地址         	
*	返 回 值: 无
*********************************************************************************************************
*/
WM_HWIN CreateScaleDlg(void) 
{
	WM_HWIN hWin;

	hWin = GUI_CreateDialogBox(_aDialogCreateScale, 
	                           GUI_COUNTOF(_aDialogCreateScale), 
	                           &_cbCallbackScale, 
	                           0, 
	                           0, 
	                           0);
	return hWin;
}

/*
*********************************************************************************************************
*	函 数 名: DrawWinScaleBk
*	功能说明: 将窗口背景绘制到存储设备里面
*	形    参: pMsg  指针地址         	
*	返 回 值: 无
*********************************************************************************************************
*/
void DrawWinScaleBk(void) 
{
	hMemScaleDlg = GUI_MEMDEV_CreateFixed(0, 
										  0, 
									      135, 
									      35, 
									      GUI_MEMDEV_HASTRANS, 
										  GUI_MEMDEV_APILIST_16, 
										  GUICC_M565);
	GUI_MEMDEV_Select(hMemScaleDlg);
	PaintDialogScale(NULL);
	GUI_MEMDEV_Select(0);
}

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
