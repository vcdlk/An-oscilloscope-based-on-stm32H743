/*
*********************************************************************************************************
*	                                  
*	模块名称 : 示波器主界面上的状态窗口
*	文件名称 : DSO_Status2Dlg.c
*	版    本 : V1.0
*	说    明 : 示波器主界面上的状态窗口，用于显示频率和RMS。
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
static GUI_MEMDEV_Handle   hMemStatus2Dlg;


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


/*
*********************************************************************************************************
*					        GUI_WIDGET_CREATE_INFO类型数组
*********************************************************************************************************
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreateStatus2[] = 
{ 
  { WINDOW_CreateIndirect,    "",       0,        663*1.28,  373*1.25,  135*1.28,  66*1.25, 0},
	
	{ TEXT_CreateIndirect,   "RMS",      ID_TEXT_0,  35*1.28,  2*1.25,  30*1.28, 16*1.25, 0, 0},
	{ TEXT_CreateIndirect,   "0.000V",   ID_TEXT_1,  72*1.28,  2*1.25,  50*1.28, 16*1.25, 0, 0},
	{ TEXT_CreateIndirect,   "Freq",     ID_TEXT_2,  35*1.28,  32*1.25,  25*1.28, 16*1.25, 0, 0},
	{ TEXT_CreateIndirect,   "000000Hz", ID_TEXT_3,  72*1.28,  32*1.25,  70*1.28, 16*1.25, 0, 0},
	
	{ TEXT_CreateIndirect,   "RMS",      ID_TEXT_4,  35*1.28,  17*1.25, 30*1.28, 16*1.25, 0, 0},
	{ TEXT_CreateIndirect,   "0.000V",   ID_TEXT_5,  72*1.28,  17*1.25, 50*1.28, 16*1.25, 0, 0},
	{ TEXT_CreateIndirect,   "Freq",     ID_TEXT_6,  35*1.28,  47*1.25,  25*1.28, 16*1.25, 0, 0},
	{ TEXT_CreateIndirect,   "000000Hz", ID_TEXT_7,  72*1.28,  47*1.25,  70*1.28, 16*1.25, 0, 0},
};

/*
*********************************************************************************************************
*	函 数 名: PaintDialogStatus2()
*	功能说明: 时基窗口的回调函数重绘
*	形    参: pMsg  指针地址         	
*	返 回 值: 无
*********************************************************************************************************
*/
static void PaintDialogStatus2(WM_MESSAGE * pMsg)
{
	/* 清背景色 */
	GUI_SetBkColor(0x905040);
	GUI_Clear();

	/* 绘制填充的抗锯齿圆角矩形 */	
	GUI_SetColor(GUI_BLACK);
	GUI_AA_FillRoundedRect(0, 0, 134*1.28, 65*1.25, 10);

	/* 绘制抗锯齿圆角矩形 */	
	GUI_SetColor(0XEBCD9E);
	GUI_SetPenSize(2);
	GUI_AA_DrawRoundedRect(0, 0, 134*1.28, 65*1.25, 10);
	
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

	/* 绘制抗锯齿圆角矩形，并填数值1 */
	GUI_SetColor(GUI_YELLOW);
	GUI_AA_FillRoundedRect(10*1.28, 34*1.25, 30*1.28, 47*1.25, 3);
	
	GUI_SetColor(GUI_BLACK);
    GUI_SetFont(&GUI_Font16_1);
	GUI_SetTextMode(GUI_TEXTMODE_TRANS);					
	GUI_DispCharAt('1', 16*1.28, 32*1.25);

	/* 绘制抗锯齿圆角矩形，并填数值2 */
	GUI_SetColor(GUI_GREEN);
	GUI_AA_FillRoundedRect(10*1.28, 50*1.25, 30*1.28, 62*1.25, 3);
						   
	GUI_SetColor(GUI_BLACK);
    GUI_SetFont(&GUI_Font16_1);
	GUI_SetTextMode(GUI_TEXTMODE_TRANS);					
	GUI_DispCharAt('2', 16*1.28, 49*1.25);
}

/*
*********************************************************************************************************
*	函 数 名: InitDialogStatus2
*	功能说明: 时基窗口的回调函数的初始化
*	形    参: pMsg  指针地址         	
*	返 回 值: 无
*********************************************************************************************************
*/
static void InitDialogStatus2(WM_MESSAGE * pMsg)
{
	int i;
    WM_HWIN hWin = pMsg->hWin;

	//
    // 初始化文本控件
    //
	for(i = ID_TEXT_0; i < ID_TEXT_4; i++)
	{
		TEXT_SetTextColor(WM_GetDialogItem(hWin, i), GUI_GREEN);
		TEXT_SetFont(WM_GetDialogItem(hWin, i), &GUI_Font16_1);		
	}
	
	for(i = ID_TEXT_4; i < ID_TEXT_8; i++)
	{
		TEXT_SetTextColor(WM_GetDialogItem(hWin, i), 0x00ffff);
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
*	函 数 名: _cbCallbackStatus2()
*	功能说明: 时基窗口的回调函数
*	形    参：pMsg  指针地址         	
*	返 回 值: 无
*********************************************************************************************************
*/
static void _cbCallbackStatus2(WM_MESSAGE * pMsg) 
{
	char buf[20];
    WM_HWIN hWin = pMsg->hWin;
	
	
    switch (pMsg->MsgId) 
    {
		case WM_INIT_DIALOG:
            InitDialogStatus2(pMsg);
            break;
				
		case WM_PAINT:
			GUI_MEMDEV_WriteAt(hMemStatus2Dlg, 663*1.28, 373*1.25);
            break;
		
		case WM_TIMER:
			/* 均方根 */
			if(g_DSO1->ucMeasureFlag[22] == 1)
			{
				sprintf(buf, "%5.3fV", g_DSO1->WaveRMS);
				TEXT_SetText(WM_GetDialogItem(hWin, ID_TEXT_1), buf);
			
				sprintf(buf, "%5.3fV", g_DSO2->WaveRMS);
				TEXT_SetText(WM_GetDialogItem(hWin, ID_TEXT_5), buf);
			}
		
			/* 波形频率估算 */
			if(g_DSO1->ucMeasureFlag[1] == 1)
			{
				if(TimeBaseId < 10)
				{
					sprintf(buf, "%06dHz", g_DSO1->uiFreq);
					TEXT_SetText(WM_GetDialogItem(hWin, ID_TEXT_3), buf);				
				}
				else
				{
					TEXT_SetText(WM_GetDialogItem(hWin, ID_TEXT_3), "000000Hz");					
				}
			}

			WM_RestartTimer(pMsg->Data.v, 500);
			break;
			
        default:
            WM_DefaultProc(pMsg);
    }
}

/*
*********************************************************************************************************
*	函 数 名: CreateStatus2Dlg
*	功能说明: 时基窗口的回调函数
*	形    参: pMsg  指针地址         	
*	返 回 值: 无
*********************************************************************************************************
*/
WM_HWIN CreateStatus2Dlg(void) 
{
	WM_HWIN hWin;

	hWin = GUI_CreateDialogBox(_aDialogCreateStatus2, 
	                           GUI_COUNTOF(_aDialogCreateStatus2), 
	                           &_cbCallbackStatus2, 
	                           0, 
	                           0, 
	                           0);

	return hWin;
}

/*
*********************************************************************************************************
*	函 数 名: DrawWinStatus2Bk
*	功能说明: 将窗口背景绘制到存储设备里面
*	形    参: pMsg  指针地址         	
*	返 回 值: 无
*********************************************************************************************************
*/
void DrawWinStatus2Bk(void) 
{
	hMemStatus2Dlg = GUI_MEMDEV_CreateFixed(0, 
										    0, 
									        135*1.28, 
									        66*1.25, 
									        GUI_MEMDEV_HASTRANS, 
										    GUI_MEMDEV_APILIST_16, 
										    GUICC_M565);
	GUI_MEMDEV_Select(hMemStatus2Dlg);
	PaintDialogStatus2(NULL);
	GUI_MEMDEV_Select(0);
}

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
