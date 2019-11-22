/*
*********************************************************************************************************
*	                                  
*	模块名称 : 主界面系统信息对话框
*	文件名称 : DSO_SysInfoDlg.c
*	版    本 : V1.0
*	说    明 : 显示CPU利用率，通道增益和通道输入电压范围。
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
static GUI_MEMDEV_Handle   hMemSysDlg;


/*
*********************************************************************************************************
*				                      宏定义
*********************************************************************************************************
*/
#define ID_TEXT_0 		(GUI_ID_USER + 0x00)
#define ID_TEXT_1  		(GUI_ID_USER + 0x01)
#define ID_TEXT_2 		(GUI_ID_USER + 0x02)
#define ID_TEXT_3 		(GUI_ID_USER + 0x03)
#define ID_TEXT_4 		(GUI_ID_USER + 0x04)


/*
*********************************************************************************************************
*					GUI_WIDGET_CREATE_INFO类型数组
*********************************************************************************************************
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreateSysInfo[] = 
{ 
    { WINDOW_CreateIndirect,     "",       0,            663*1.28,  	287*1.25,  135*1.28,  81*1.25, 0},
	{ TEXT_CreateIndirect,      "CPU: 00.00%",      ID_TEXT_0,  35*1.28,   2*1.25, 100*1.28, 16*1.25, 0, 0},
	{ TEXT_CreateIndirect,      "CH1: DC 1:1",  	ID_TEXT_1,  35*1.28,  17*1.25, 100*1.28, 16*1.25, 0, 0},
	{ TEXT_CreateIndirect,      "CH2: DC 1:1",    	ID_TEXT_2,  35*1.28,  32*1.25, 100*1.28, 16*1.25, 0, 0},
	{ TEXT_CreateIndirect,      "Input1: 0V-3.3V",  ID_TEXT_3,  35*1.28,  47*1.25, 100*1.28, 16*1.25, 0, 0},
	{ TEXT_CreateIndirect,      "Input2: 0V-3.3V",  ID_TEXT_4,  35*1.28,  62*1.25, 100*1.28, 16*1.25, 0, 0},
};

/*
*********************************************************************************************************
*	函 数 名: PaintDialogSysInfo()
*	功能说明: 时基窗口的回调函数重绘
*	形    参: pMsg  指针地址         	
*	返 回 值: 无
*********************************************************************************************************
*/
static void PaintDialogSysInfo(WM_MESSAGE * pMsg)
{

	GUI_SetBkColor(0x905040);
	GUI_Clear();
	GUI_SetColor(GUI_BLACK);
	
	GUI_AA_FillRoundedRect(0, 0, 135*1.28-1, 81*1.25-1, 10);
	GUI_SetColor(0XEBCD9E);
	GUI_SetPenSize(2);
	
	GUI_AA_DrawRoundedRect(0, 0, 135*1.28-1, 81*1.25-1, 10);
	
	GUI_SetColor(GUI_GREEN);
	GUI_AA_FillRoundedRect(10*1.28, 4*1.25, 30*1.28, 16*1.25, 3);
	

	GUI_SetColor(GUI_BLACK);
    GUI_SetFont(&GUI_Font16_1);
	GUI_SetTextMode(GUI_TEXTMODE_TRANS);					
	GUI_DispCharAt('C', 16*1.28, 3*1.25);
	
	GUI_SetColor(GUI_YELLOW);
	GUI_AA_FillRoundedRect(10*1.28, 19*1.25, 30*1.28, 31*1.25, 3);
						   
	GUI_SetColor(GUI_BLACK);
    GUI_SetFont(&GUI_Font16_1);
	GUI_SetTextMode(GUI_TEXTMODE_TRANS);					
	GUI_DispCharAt('1', 16*1.28, 18*1.25);
	
	GUI_SetColor(GUI_GREEN);
	GUI_AA_FillRoundedRect(10*1.28, 34*1.25, 30*1.28, 47*1.25, 3);
	
	GUI_SetColor(GUI_BLACK);
    GUI_SetFont(&GUI_Font16_1);
	GUI_SetTextMode(GUI_TEXTMODE_TRANS);					
	GUI_DispCharAt('2', 16*1.28, 32*1.25);
	
	GUI_SetColor(GUI_YELLOW);
	GUI_AA_FillRoundedRect(10*1.28, 50*1.25, 30*1.28, 62*1.25, 3);
						   
	GUI_SetColor(GUI_BLACK);
    GUI_SetFont(&GUI_Font16_1);
	GUI_SetTextMode(GUI_TEXTMODE_TRANS);					
	GUI_DispCharAt('1', 16*1.28, 49*1.25);
	
	GUI_SetColor(GUI_GREEN);
	GUI_AA_FillRoundedRect(10*1.28, 65*1.25, 30*1.28, 77*1.25, 3);
						   
	GUI_SetColor(GUI_BLACK);
    GUI_SetFont(&GUI_Font16_1);
	GUI_SetTextMode(GUI_TEXTMODE_TRANS);					
	GUI_DispCharAt('2', 16*1.28, 64*1.25);
}

/*
*********************************************************************************************************
*	函 数 名: InitDialogSysInfo
*	功能说明: 时基窗口的回调函数的初始化
*	形    参：pMsg  指针地址         	
*	返 回 值: 无
*********************************************************************************************************
*/
static void InitDialogSysInfo(WM_MESSAGE * pMsg)
{
    WM_HWIN hWin = pMsg->hWin;

    //
    // 初始化文本控件
    //
    TEXT_SetTextColor(WM_GetDialogItem(hWin, ID_TEXT_0), GUI_GREEN);
    TEXT_SetFont(WM_GetDialogItem(hWin, ID_TEXT_0), &GUI_Font16_1);
	
	TEXT_SetTextColor(WM_GetDialogItem(hWin, ID_TEXT_1), 0x00ffff);
    TEXT_SetFont(WM_GetDialogItem(hWin, ID_TEXT_1), &GUI_Font16_1);
	
	TEXT_SetTextColor(WM_GetDialogItem(hWin, ID_TEXT_2), GUI_GREEN);
    TEXT_SetFont(WM_GetDialogItem(hWin, ID_TEXT_2), &GUI_Font16_1);
	
	TEXT_SetTextColor(WM_GetDialogItem(hWin, ID_TEXT_3), 0x00ffff);
    TEXT_SetFont(WM_GetDialogItem(hWin, ID_TEXT_3), &GUI_Font16_1);

	TEXT_SetTextColor(WM_GetDialogItem(hWin, ID_TEXT_4), GUI_GREEN);
    TEXT_SetFont(WM_GetDialogItem(hWin, ID_TEXT_4),&GUI_Font16_1);

	/* 创建定时器 */
	WM_CreateTimer(hWin,   /* 接受信息的窗口的句柄 */
				   0, 	   /* 用户定义的Id。如果不对同一窗口使用多个定时器，此值可以设置为零。 */
				   1000,   /* 周期，此周期过后指定窗口应收到消息*/
				   0);	   /* 留待将来使用，应为0 */	
}

/*
*********************************************************************************************************
*	函 数 名: _cbCallbackSysInfo()
*	功能说明: 时基窗口的回调函数
*	形    参: pMsg  指针地址         	
*	返 回 值: 无
*********************************************************************************************************
*/
static void _cbCallbackSysInfo(WM_MESSAGE * pMsg) 
{
    WM_HWIN hWin = pMsg->hWin;
	char buf[40];
	float CPU;
	
    switch (pMsg->MsgId) 
    {
        case WM_INIT_DIALOG:
            InitDialogSysInfo(pMsg);
            break;
		
		case WM_TIMER:
			CPU = (float)OSStatTaskCPUUsage / 100;
			sprintf(buf, "CPU: %5.2f%%", CPU);
			TEXT_SetText(WM_GetDialogItem(hWin, ID_TEXT_0),  buf);
		
			sprintf(buf, "CH1: DC 1:1");
			TEXT_SetText(WM_GetDialogItem(hWin, ID_TEXT_1),  buf);
			
			sprintf(buf, "CH2: DC 1:1");
			TEXT_SetText(WM_GetDialogItem(hWin, ID_TEXT_2),  buf);
			
			sprintf(buf, "Input1: 0V-3.3V");
			TEXT_SetText(WM_GetDialogItem(hWin, ID_TEXT_3),  buf);
			
			sprintf(buf, "Input2: 0V-3.3V");
			TEXT_SetText(WM_GetDialogItem(hWin, ID_TEXT_4),  buf);
			WM_RestartTimer(pMsg->Data.v, 1000);
			break;
		 
		case WM_PAINT:
			GUI_MEMDEV_WriteAt(hMemSysDlg, 663*1.28, 287*1.25);
            break;
			
        default:
            WM_DefaultProc(pMsg);
    }
}

/*
*********************************************************************************************************
*	函 数 名: CreateSysInfoDlg
*	功能说明: 系统信息对话框
*	形    参: 无   	
*	返 回 值: 窗口句柄
*********************************************************************************************************
*/
WM_HWIN CreateSysInfoDlg(void) 
{
	WM_HWIN hWin;

	hWin = GUI_CreateDialogBox(_aDialogCreateSysInfo, 
	                           GUI_COUNTOF(_aDialogCreateSysInfo), 
	                           &_cbCallbackSysInfo,
                               0, 
	                           0, 
	                           0);
	return hWin;
}

/*
*********************************************************************************************************
*	函 数 名: DrawWinSysBk
*	功能说明: 将窗口背景绘制到存储设备里面
*	形    参: pMsg  指针地址         	
*	返 回 值: 无
*********************************************************************************************************
*/
void DrawWinSysBk(void) 
{
	hMemSysDlg = GUI_MEMDEV_CreateFixed(0, 
										0, 
									    135*1.28, 
									    81*1.25, 
									    GUI_MEMDEV_HASTRANS, 
										GUI_MEMDEV_APILIST_16, 
										GUICC_M565);
	GUI_MEMDEV_Select(hMemSysDlg);
	PaintDialogSysInfo(NULL);
	GUI_MEMDEV_Select(0);
}

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
