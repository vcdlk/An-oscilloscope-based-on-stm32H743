/*
*********************************************************************************************************
*	                                  
*	模块名称 : 示波器主界面上的状态窗口
*	文件名称 : DSO_Status1Dlg.c
*	版    本 : V1.0
*	说    明 : 示波器主界面上的状态窗口，用于显示最大值，峰峰值，平均值和最小值共4种
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
static GUI_MEMDEV_Handle   hMemStatus1Dlg;


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
#define ID_TEXT_5 		(GUI_ID_USER + 0x05)
#define ID_TEXT_6 		(GUI_ID_USER + 0x06)
#define ID_TEXT_7 		(GUI_ID_USER + 0x07)
#define ID_TEXT_8 		(GUI_ID_USER + 0x08)
#define ID_TEXT_9 		(GUI_ID_USER + 0x09)
#define ID_TEXT_10  	(GUI_ID_USER + 0x0A)
#define ID_TEXT_11 		(GUI_ID_USER + 0x0B)
#define ID_TEXT_12 		(GUI_ID_USER + 0x0C)

#define ID_TEXT_13 		(GUI_ID_USER + 0x0D)
#define ID_TEXT_14  	(GUI_ID_USER + 0x0E)
#define ID_TEXT_15 		(GUI_ID_USER + 0x0F)
#define ID_TEXT_16 		(GUI_ID_USER + 0x10)


/*
*********************************************************************************************************
*					GUI_WIDGET_CREATE_INFO类型数组
*********************************************************************************************************
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreateStatus1[] = 
{
    { WINDOW_CreateIndirect,     "",     0,          145*1.28,  444*1.25, 510*1.28, 35*1.25, 0},
	
	/* 第一行 */
	{ TEXT_CreateIndirect,    "Mean",    ID_TEXT_0,   35*1.28,  2*1.25, 40*1.28, 16*1.25, 0, 0},
	{ TEXT_CreateIndirect,    "0.000V",  ID_TEXT_1,   80*1.28,  2*1.25, 50*1.28, 16*1.25, 0, 0},
	
	{ TEXT_CreateIndirect,    "Pk-Pk",   ID_TEXT_2,   160*1.28, 2*1.25, 40*1.28, 16*1.25, 0, 0},
	{ TEXT_CreateIndirect,    "0.000V",  ID_TEXT_3,   205*1.28, 2*1.25, 50*1.28, 16*1.25, 0, 0},
	
	{ TEXT_CreateIndirect,    "Min",     ID_TEXT_4,   285*1.28, 2*1.25, 40*1.28, 16*1.25, 0, 0},
	{ TEXT_CreateIndirect,    "0.000V",  ID_TEXT_5,   330*1.28, 2*1.25, 50*1.28, 16*1.25, 0, 0},	

	{ TEXT_CreateIndirect,    "Max",     ID_TEXT_6,   410*1.28, 2*1.25, 40*1.28, 16*1.25, 0, 0},
	{ TEXT_CreateIndirect,    "0.000V",  ID_TEXT_7,   455*1.28, 2*1.25, 50*1.28, 16*1.25, 0, 0},		
	
	/* 第二行 */		
    { TEXT_CreateIndirect,    "Mean",    ID_TEXT_8,    35*1.28, 17*1.25, 40*1.28, 16*1.25, 0, 0},
    { TEXT_CreateIndirect,    "0.000V",  ID_TEXT_9,    80*1.28, 17*1.25, 50*1.28, 16*1.25, 0, 0},	

    { TEXT_CreateIndirect,    "Pk-Pk",   ID_TEXT_10,  160*1.28, 17*1.25, 40*1.28, 16*1.25, 0, 0},
    { TEXT_CreateIndirect,    "0.000V",  ID_TEXT_11,  205*1.28, 17*1.25, 50*1.28, 16*1.25, 0, 0},	

	{ TEXT_CreateIndirect,    "Min",     ID_TEXT_12,  285*1.28, 17*1.25, 40*1.28, 16*1.25, 0, 0},
	{ TEXT_CreateIndirect,    "0.000V",  ID_TEXT_13,  330*1.28, 17*1.25, 50*1.28, 16*1.25, 0, 0},

	{ TEXT_CreateIndirect,    "Max",     ID_TEXT_14,  410*1.28, 17*1.25, 40*1.28, 16*1.25, 0, 0},
	{ TEXT_CreateIndirect,    "0.000V",  ID_TEXT_15,  455*1.28, 17*1.25, 50*1.28, 16*1.25, 0, 0},
};

/*
*********************************************************************************************************
*	函 数 名: PaintDialogStatus1
*	功能说明: 状态窗口的回调函数重绘消息
*	形    参：pMsg  指针地址         	
*	返 回 值: 无
*********************************************************************************************************
*/
void PaintDialogStatus1(WM_MESSAGE * pMsg)
{
	/* 清背景色 */
	GUI_SetBkColor(0x905040);
  	GUI_Clear();
	
	/* 绘制填充的抗锯齿圆角矩形 */
	GUI_SetColor(GUI_BLACK);
	GUI_AA_FillRoundedRect(0, 0, 509*1.28, 34*1.25, 10);
	
	/* 绘制抗锯齿圆角矩形 */
	GUI_SetColor(0XEBCD9E);
	GUI_SetPenSize(2);
	GUI_AA_DrawRoundedRect(0, 0, 509*1.28, 34*1.25, 10);
	
	/* 绘制抗锯齿圆角矩形，并填数值1 */
	GUI_SetColor(GUI_YELLOW);
	GUI_AA_FillRoundedRect(10*1.28, 4*1.25, 30*1.28, 16*1.25, 3);
	
	GUI_SetColor(GUI_BLACK);
    GUI_SetFont(&GUI_Font16_1);
	GUI_SetTextMode(GUI_TEXTMODE_TRANS);					
	GUI_DispCharAt('1', 16*1.28, 3*1.25);

	/* 绘制抗锯齿圆角矩形，并填数值1 */
	GUI_SetColor(GUI_YELLOW);
	GUI_AA_FillRoundedRect(135*1.28, 4*1.25, 155*1.28, 16*1.25, 3);
						   
	GUI_SetColor(GUI_BLACK);
    GUI_SetFont(&GUI_Font16_1);
	GUI_SetTextMode(GUI_TEXTMODE_TRANS);					
	GUI_DispCharAt('1', 141*1.28, 3*1.25);
	
	/* 绘制抗锯齿圆角矩形，并填数值1 */
	GUI_SetColor(GUI_YELLOW);
	GUI_AA_FillRoundedRect(260*1.28, 4*1.25, 280*1.28, 16*1.25, 3);
						   
	GUI_SetColor(GUI_BLACK);
    GUI_SetFont(&GUI_Font16_1);
	GUI_SetTextMode(GUI_TEXTMODE_TRANS);					
	GUI_DispCharAt('1', 266*1.28, 2*1.25);
	
	/* 绘制抗锯齿圆角矩形，并填数值1 */	
	GUI_SetColor(GUI_YELLOW);
	GUI_AA_FillRoundedRect(385*1.28, 4*1.25, 405*1.28, 16*1.25, 3);
						   
	GUI_SetColor(GUI_BLACK);
    GUI_SetFont(&GUI_Font16_1);
	GUI_SetTextMode(GUI_TEXTMODE_TRANS);					
	GUI_DispCharAt('1', 391*1.28, 2*1.25);
	
	/* 绘制抗锯齿圆角矩形，并填数值2 */
	GUI_SetColor(GUI_GREEN);
	GUI_AA_FillRoundedRect(10*1.28, 19*1.25, 30*1.28, 31*1.25, 3);
						   
	GUI_SetColor(GUI_BLACK);
    GUI_SetFont(&GUI_Font16_1);
	GUI_SetTextMode(GUI_TEXTMODE_TRANS);					
	GUI_DispCharAt('2', 16*1.28, 18*1.25);
	
	/* 绘制抗锯齿圆角矩形，并填数值2 */
	GUI_SetColor(GUI_GREEN);
	GUI_AA_FillRoundedRect(135*1.28, 19*1.25, 155*1.28, 31*1.25, 3);
						   
	GUI_SetColor(GUI_BLACK);
    GUI_SetFont(&GUI_Font16_1);
	GUI_SetTextMode(GUI_TEXTMODE_TRANS);					
	GUI_DispCharAt('2', 141*1.28, 18*1.25);
	
	/* 绘制抗锯齿圆角矩形，并填数值2 */
	GUI_SetColor(GUI_GREEN);
	GUI_AA_FillRoundedRect(260*1.28, 19*1.25, 280*1.28, 31*1.25, 3);
						   
	GUI_SetColor(GUI_BLACK);
    GUI_SetFont(&GUI_Font16_1);
	GUI_SetTextMode(GUI_TEXTMODE_TRANS);					
	GUI_DispCharAt('2', 266*1.28, 18*1.25);

	/* 绘制抗锯齿圆角矩形，并填数值2 */
	GUI_SetColor(GUI_GREEN);
	GUI_AA_FillRoundedRect(385*1.28, 19*1.25, 405*1.28, 31*1.25, 3);
						   
	GUI_SetColor(GUI_BLACK);
    GUI_SetFont(&GUI_Font16_1);
	GUI_SetTextMode(GUI_TEXTMODE_TRANS);					
	GUI_DispCharAt('2', 391*1.28, 18*1.25);
}

/*
*********************************************************************************************************
*	函 数 名: InitDialogStatus1()
*	功能说明: 状态窗口的回调函数初始化消息
*	形    参：pMsg  指针地址         	
*	返 回 值: 无
*********************************************************************************************************
*/
void InitDialogStatus1(WM_MESSAGE * pMsg)
{
	int i;
    WM_HWIN hWin = pMsg->hWin;

	//
    // 初始化文本控件
    //
	for(i = ID_TEXT_0; i < ID_TEXT_8; i++)
	{
		TEXT_SetTextColor(WM_GetDialogItem(hWin, i), 0x00ffff);
		TEXT_SetFont(WM_GetDialogItem(hWin, i), &GUI_Font16_1);		
	}
	
	for(i = ID_TEXT_8; i < ID_TEXT_16; i++)
	{
		TEXT_SetTextColor(WM_GetDialogItem(hWin, i), GUI_GREEN);
		TEXT_SetFont(WM_GetDialogItem(hWin, i), &GUI_Font16_1);		
	}
	
	/* 创建定时器 */
	WM_CreateTimer(hWin,   /* 接受信息的窗口的句柄 */
				   0, 	   /* 用户定义的Id。如果不对同一窗口使用多个定时器，此值可以设置为零。 */
				   1000,   /* 周期，此周期过后指定窗口应收到消息*/
				   0);	   /* 留待将来使用，应为0 */	
}

/*
*********************************************************************************************************
*	函 数 名: _cbCallbackStatus1
*	功能说明: 状态窗口的回调函数
*	形    参: pMsg  指针地址         	
*	返 回 值: 无
*********************************************************************************************************
*/
static void _cbCallbackStatus1(WM_MESSAGE * pMsg) 
{
    WM_HWIN hWin = pMsg->hWin;
	char buf[20];
	
    switch (pMsg->MsgId) 
    {
        case WM_PAINT:
			GUI_MEMDEV_WriteAt(hMemStatus1Dlg, 145*1.28, 444*1.25);
            break;
		
        case WM_INIT_DIALOG:
            InitDialogStatus1(pMsg);
            break;
		
		case WM_TIMER:
			/* 平均值 */
			if(g_DSO1->ucMeasureFlag[24] == 1)
			{
				sprintf(buf, "%5.3fV", g_DSO1->WaveMean);
				TEXT_SetText(WM_GetDialogItem(hWin, ID_TEXT_0),  g_MeasureTable[20]);
				TEXT_SetText(WM_GetDialogItem(hWin, ID_TEXT_1),  buf);
				
				sprintf(buf, "%5.3fV", g_DSO2->WaveMean);
				TEXT_SetText(WM_GetDialogItem(hWin, ID_TEXT_8),  g_MeasureTable[20]);
				TEXT_SetText(WM_GetDialogItem(hWin, ID_TEXT_9),  buf);
			}
			
			/* 峰峰值 */
			if(g_DSO1->ucMeasureFlag[12] == 1)
			{
				sprintf(buf, "%5.3fV", g_DSO1->WavePkPk);
				TEXT_SetText(WM_GetDialogItem(hWin, ID_TEXT_2),  g_MeasureTable[12]);
				TEXT_SetText(WM_GetDialogItem(hWin, ID_TEXT_3),  buf);
				
				sprintf(buf, "%5.3fV", g_DSO2->WavePkPk);
				TEXT_SetText(WM_GetDialogItem(hWin, ID_TEXT_10),  g_MeasureTable[12]);
				TEXT_SetText(WM_GetDialogItem(hWin, ID_TEXT_11),  buf);				
			}

			/* 最小值 */
			if(g_DSO1->ucMeasureFlag[15] == 1)
			{
				sprintf(buf, "%5.3fV", g_DSO1->WaveMin);
				TEXT_SetText(WM_GetDialogItem(hWin, ID_TEXT_4),  g_MeasureTable[15]);
				TEXT_SetText(WM_GetDialogItem(hWin, ID_TEXT_5),  buf);
				
				sprintf(buf, "%5.3fV", g_DSO2->WaveMin);
				TEXT_SetText(WM_GetDialogItem(hWin, ID_TEXT_12),  g_MeasureTable[15]);
				TEXT_SetText(WM_GetDialogItem(hWin, ID_TEXT_13),  buf);
			}

			/* 最大值 */
			if(g_DSO1->ucMeasureFlag[14] == 1)
			{
				sprintf(buf, "%5.3fV", g_DSO1->WaveMax);
				TEXT_SetText(WM_GetDialogItem(hWin, ID_TEXT_6),  g_MeasureTable[14]);
				TEXT_SetText(WM_GetDialogItem(hWin, ID_TEXT_7),  buf);
				
				sprintf(buf, "%5.3fV", g_DSO2->WaveMax);
				TEXT_SetText(WM_GetDialogItem(hWin, ID_TEXT_14),  g_MeasureTable[14]);
				TEXT_SetText(WM_GetDialogItem(hWin, ID_TEXT_15),  buf);
			}
			
			WM_RestartTimer(pMsg->Data.v, 500);
			break;
			
        default:
            WM_DefaultProc(pMsg);
    }
}

/*
*********************************************************************************************************
*	函 数 名: CreateWindowStatus1
*	功能说明: 状态窗口的回调函数
*	形    参：pMsg  指针地址         	
*	返 回 值: 无
*********************************************************************************************************
*/
WM_HWIN CreateStatus1Dlg(void) 
{
	WM_HWIN hWin;

	hWin = GUI_CreateDialogBox(_aDialogCreateStatus1, 
	                           GUI_COUNTOF(_aDialogCreateStatus1), 
	                           &_cbCallbackStatus1, 
	                           0, 
	                           0, 
	                           0);
	
	return hWin;
}

/*
*********************************************************************************************************
*	函 数 名: DrawWinStatus1Bk
*	功能说明: 将窗口背景绘制到存储设备里面
*	形    参: 无         	
*	返 回 值: 无
*********************************************************************************************************
*/
void DrawWinStatus1Bk(void) 
{
	hMemStatus1Dlg = GUI_MEMDEV_CreateFixed(0, 
										      0, 
									        510*1.28, 
									        35*1.25, 
									        GUI_MEMDEV_HASTRANS, 
										    GUI_MEMDEV_APILIST_16, 
										    GUICC_M565);
	GUI_MEMDEV_Select(hMemStatus1Dlg);
	PaintDialogStatus1(NULL);
	GUI_MEMDEV_Select(0);
}

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
