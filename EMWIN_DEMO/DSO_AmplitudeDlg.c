/*
*********************************************************************************************************
*	                                  
*	模块名称 : 示波器主界面上的幅值窗口
*	文件名称 : DSO_AmplititudeDlg.c
*	版    本 : V1.0
*	说    明 : 显示示波器当前的幅值。
*	修改记录 :
*		版本号    日期          作者           说明
*		V1.0    2018-01-06     Eric2013        首发 
*                                           
*	Copyright (C), 2018-2028, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/
#include "MainTask.h"
/*
*********************************************************************************************************
*	                                  变量
*********************************************************************************************************
*/
static GUI_MEMDEV_Handle   hMemAmpWindow;

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
*	                   GUI_WIDGET_CREATE_INFO类型数组
*********************************************************************************************************
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreateAmp[] = 
{
  { WINDOW_CreateIndirect,     "",      0,           2*1.28,444*1.25, 135*1.28, 35*1.25, 0},
	{ TEXT_CreateIndirect,      "Amp",    ID_TEXT_0,  35*1.28,  2*1.25,  35*1.28, 16*1.25, 0, 0},
	{ TEXT_CreateIndirect,      "1.00V",  ID_TEXT_1,  75*1.28,  2*1.25,  50*1.28, 16*1.25, 0, 0},
	
	{ TEXT_CreateIndirect,      "Amp",    ID_TEXT_2,  35*1.28,  17*1.25, 35*1.28, 16*1.25, 0, 0},
	{ TEXT_CreateIndirect,      "1.00V",  ID_TEXT_3,  75*1.28,  17*1.25, 50*1.28, 16*1.25, 0, 0},
};
/*
*********************************************************************************************************
*	函 数 名: PaintDialogAmp
*	功能说明: 幅值窗口重绘消息中的函数
*	形    参：pMsg  指针地址      	
*	返 回 值: 无
*********************************************************************************************************
*/
static void PaintDialogAmp(WM_MESSAGE * pMsg)
{
	/* 清背景色 */
	GUI_SetBkColor(0x905040);
  	GUI_Clear();
	
	/* 绘制填充的抗锯齿圆角矩形 */
	GUI_SetColor(GUI_BLACK);
	GUI_AA_FillRoundedRect(0, 0, 134*1.28, 34*1.25, 10);
	
	/* 绘制抗锯齿圆角矩形 */
	GUI_SetColor(0XEBCD9E);
	GUI_SetPenSize(2);
	GUI_AA_DrawRoundedRect(0, 0, 134*1.28, 34*1.25, 10);
	
	/* 绘制抗锯齿圆角矩形，并填数值1 */
	GUI_SetColor(GUI_YELLOW);
	GUI_AA_FillRoundedRect(10*1.28, 4*1.25, 30*1.28, 16*1.25, 3);
	
	GUI_SetColor(GUI_BLACK);
    GUI_SetFont(&GUI_Font16_1);
	GUI_SetTextMode(GUI_TEXTMODE_TRANS);					
	GUI_DispCharAt('1', 16*1.28, 3*1.25);

	/* 绘制抗锯齿圆角矩形，并填数值2 */
	GUI_SetColor(GUI_GREEN);
	GUI_AA_FillRoundedRect(10*1.28, 19*1.25, 30*1.28, 31*1.25, 3);
						   
	GUI_SetColor(GUI_BLACK);
    GUI_SetFont(&GUI_Font16_1);
	GUI_SetTextMode(GUI_TEXTMODE_TRANS);					
	GUI_DispCharAt('2', 16*1.28, 18*1.25);
}

/*
*********************************************************************************************************
*	函 数 名: PaintDialogAmp
*	功能说明: 幅值窗口初始化的消息处理
*	形    参：pMsg  指针地址      	
*	返 回 值: 无
*********************************************************************************************************
*/
static void InitDialogAmp(WM_MESSAGE * pMsg)
{
	WM_HWIN hWin = pMsg->hWin;

	//
	// 初始化文本控件
	//
	TEXT_SetTextColor(WM_GetDialogItem(hWin, ID_TEXT_0), 0x00ffff);
	TEXT_SetFont(WM_GetDialogItem(hWin, ID_TEXT_0), &GUI_Font16_1);

	TEXT_SetTextColor(WM_GetDialogItem(hWin, ID_TEXT_1), 0x00ffff);
	TEXT_SetFont(WM_GetDialogItem(hWin, ID_TEXT_1), &GUI_Font16_1);
	
	TEXT_SetTextColor(WM_GetDialogItem(hWin, ID_TEXT_2), GUI_GREEN);
	TEXT_SetFont(WM_GetDialogItem(hWin, ID_TEXT_2), &GUI_Font16_1);

	TEXT_SetTextColor(WM_GetDialogItem(hWin, ID_TEXT_3), GUI_GREEN);
	TEXT_SetFont(WM_GetDialogItem(hWin, ID_TEXT_3), &GUI_Font16_1);
}

/*
*********************************************************************************************************
*	函 数 名: _cbCallbackAmp()
*	功能说明: 幅值窗口回调函数
*	形    参: pMsg  指针地址         	
*	返 回 值: 无
*********************************************************************************************************
*/
static void _cbCallbackAmp(WM_MESSAGE * pMsg) 
{
    WM_HWIN hWin = pMsg->hWin;
	
    switch (pMsg->MsgId) 
    {
        case WM_TextUpDate:
			TEXT_SetText(WM_GetDialogItem(hWin, ID_TEXT_1), g_AttText[Ch1AmpId]);
			TEXT_SetText(WM_GetDialogItem(hWin, ID_TEXT_3), g_AttText[Ch1AmpId]);
			break;
			
		case WM_PAINT:
			GUI_MEMDEV_WriteAt(hMemAmpWindow, 2*1.28, 444*1.25);
            break;
		
        case WM_INIT_DIALOG:
            InitDialogAmp(pMsg);
            break;
		
        default:
            WM_DefaultProc(pMsg);
    }
}

/*
*********************************************************************************************************
*	函 数 名: CreateWindowAmplitude
*	功能说明: 创建幅值窗口
*	形    参: 无         	
*	返 回 值: 所创建窗口的句柄
*********************************************************************************************************
*/
WM_HWIN CreateAmplitudeDlg(void) 
{
	WM_HWIN hWin;

	hWin = GUI_CreateDialogBox(_aDialogCreateAmp, 
	                           GUI_COUNTOF(_aDialogCreateAmp), 
	                           &_cbCallbackAmp, 
	                           0, 
	                           0, 
	                           0);
	
	return hWin;
}

/*
*********************************************************************************************************
*	函 数 名: DrawWinAmpBk
*	功能说明: 将窗口背景绘制到存储设备里面
*	形    参: 无         	
*	返 回 值: 无
*********************************************************************************************************
*/
void DrawWinAmpBk(void) 
{
	hMemAmpWindow = GUI_MEMDEV_CreateFixed(0, 
												 0, 
									       135*1.28, 
									       35*1.25, 
									       GUI_MEMDEV_HASTRANS, 
										   GUI_MEMDEV_APILIST_16, 
										   GUICC_M565);
	GUI_MEMDEV_Select(hMemAmpWindow);
	PaintDialogAmp(NULL);
	GUI_MEMDEV_Select(0);
}







