/*
*********************************************************************************************************
*	                                  
*	模块名称 : 示波器ADC对话框
*	文件名称 : DSO_AdcDlg.c
*	版    本 : V1.0
*	说    明 : 示波器ADC对话框。
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
*                                       宏定义
*********************************************************************************************************
*/
#define ID_WINDOW_0 	(GUI_ID_USER + 0x00)
#define ID_TEXT_0 	    (GUI_ID_USER + 0x01)
#define ID_TEXT_1 	    (GUI_ID_USER + 0x02)
#define ID_TEXT_2 	    (GUI_ID_USER + 0x03)
#define ID_BUTTON_0     (GUI_ID_USER + 0x04)
#define ID_BUTTON_1     (GUI_ID_USER + 0x05)
#define ID_GRAPH_0      (GUI_ID_USER + 0x06)

#define ID_TEXT_3 	    (GUI_ID_USER + 0x07)


/*
*********************************************************************************************************
*	                                     变量
*********************************************************************************************************
*/
static GRAPH_SCALE_Handle hScaleV;     
static GRAPH_DATA_Handle  ahData;
static WM_HWIN hGraphADC;


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
				GUI_AA_FillCircle(100, 0, 72);
				GUI_SetBkColor(GUI_DARKGRAY);
				GUI_SetColor(GUI_WHITE); 
				GUI_DrawBitmap(&bmReturn, 40, 5);
			} 
			else 
			{
				GUI_SetColor(GUI_STCOLOR_LIGHTBLUE);
				GUI_AA_FillCircle(100, 0, 72);
				GUI_SetBkColor(GUI_STCOLOR_LIGHTBLUE);
				GUI_SetColor(GUI_WHITE);  
				GUI_DrawBitmap(&bmReturn, 40, 5);				
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
				GUI_AA_FillCircle(0, 0, 72);
				GUI_SetBkColor(GUI_DARKGRAY);
				GUI_SetColor(GUI_WHITE); 
				
				GUI_DrawBitmap(&bmSettings, 0, 5);
			} 
			else 
			{
				GUI_SetColor(GUI_STCOLOR_LIGHTBLUE);
				GUI_AA_FillCircle(0, 0, 72);
				GUI_SetBkColor(GUI_STCOLOR_LIGHTBLUE);
				GUI_SetColor(GUI_WHITE);  

				GUI_DrawBitmap(&bmSettings, 0, 5);
			}
			break;
			
		default:
			BUTTON_Callback(pMsg);
	}
}

/*
*********************************************************************************************************
*	               				对话框控件列表
*********************************************************************************************************
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreateADC[] = {
	{ WINDOW_CreateIndirect, "Window",         ID_WINDOW_0,   0,   0,   800*1.28,  480*1.28,  0, 0x0, 0},
	{ TEXT_CreateIndirect,   "",               ID_TEXT_0,   250*1.28,  75*1.25,   300*1.28,  100*1.25,  0, 0},
	{ TEXT_CreateIndirect,   "ADC1=1234",      ID_TEXT_1,   560*1.28,  135*1.25,  200*1.28,   50*1.25,  0, 0},
	{ GRAPH_CreateIndirect,  "Graph",          ID_GRAPH_0,   50*1.28,  200*1.25,  700*1.28,  250*1.25,  0, 0x0,  0},
	{ TEXT_CreateIndirect,   "Range:0V-3.3V",  ID_TEXT_2,    80*1.28,  230*1.25,  100*1.28,   20*1.25,  0, 0},
	{ BUTTON_CreateIndirect, "DlgBack",        ID_BUTTON_0, 700*1.28,    0*1.25,  100*1.28,  100*1.25,  0, 0, 0},	
	{ BUTTON_CreateIndirect, "MusicList",      ID_BUTTON_1,   0*1.28,    0*1.25,  100*1.28,  100*1.25,  0, 0, 0},
	{ TEXT_CreateIndirect,   "Text",           ID_TEXT_3,    200*1.28,  10*1.25,  400*1.28,   32*1.25,  0, 0x64, 0 },
};

/*
*********************************************************************************************************
*	函 数 名: InitDialogReserved
*	功能说明: 对话框回调函数的初始化消息
*	形    参：pMsg   消息指针变量
*	返 回 值: 无
*********************************************************************************************************
*/
static void InitDialogADC(WM_MESSAGE * pMsg)
{
    WM_HWIN hWin = pMsg->hWin;
	WM_HWIN hItem;
	
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
	hItem = WM_GetDialogItem(hWin, ID_TEXT_0);
	TEXT_SetTextColor(hItem, GUI_RED);
	TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
	TEXT_SetFont(hItem, &GUI_Font32_1);
    TEXT_SetText(hItem, "0.000");
	
	hItem = WM_GetDialogItem(hWin, ID_TEXT_1);
	TEXT_SetTextColor(hItem, GUI_RED);
	TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
	TEXT_SetFont(hItem, &GUI_Font32_1);
	
	hItem = WM_GetDialogItem(hWin, ID_TEXT_2);
	TEXT_SetTextColor(hItem, GUI_GREEN);
	TEXT_SetTextAlign(hItem, GUI_TA_HCENTER);
	TEXT_SetFont(hItem, &GUI_Font16_1);
	
	hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_3);
	TEXT_SetFont(hItem, &GUI_FontFontSong26);
	TEXT_SetTextColor(hItem, GUI_RED);
	TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
	TEXT_SetText(hItem, "电压测量");

	//
	// 初始化Graph控件
	//
	hGraphADC = WM_GetDialogItem(pMsg->hWin, ID_GRAPH_0);

	/* 创建数据对象 *******************************************************/
	ahData = GRAPH_DATA_YT_Create(GUI_GREEN, 700*1.28, 0, 0);

	/* 数据对象添加到图形控件 */
	GRAPH_AttachData(hGraphADC, ahData);

	/* 设置Y轴方向的栅格间距 */
	GRAPH_SetGridDistY(hGraphADC, 50);

	/* 固定X轴方向的栅格 */
	GRAPH_SetGridFixedX(hGraphADC, 1);

	/* 设置栅格可见 */
	GRAPH_SetGridVis(hGraphADC, 1);

	GRAPH_SetLineStyleH(hGraphADC, GUI_LS_DOT);
	GRAPH_SetLineStyleV(hGraphADC, GUI_LS_DOT);

	/* 创建刻度对象  ***************************************************/
	hScaleV = GRAPH_SCALE_Create(15, GUI_TA_RIGHT, GRAPH_SCALE_CF_VERTICAL, 50);

	/* 将垂直刻度对象添加到图形控件 */
	GRAPH_AttachScale(hGraphADC, hScaleV);

	/* 用于设置比例刻度的因子 */
	GRAPH_SCALE_SetFactor(hScaleV, 0.02);

	/* 设置标签字体颜色 */
	GRAPH_SCALE_SetTextColor(hScaleV, GUI_RED);

	GRAPH_SCALE_SetFont(hScaleV, &GUI_Font16_1);

	/* 设置上下左右边界的大小 */
	GRAPH_SetBorder(hGraphADC, 20*1.28, 10*1.25, 10*1.28, 10*1.25);	
			
	/* 
	 * 创建定时器，其功能是经过指定周期后，向指定窗口发送消息。
	 * 该定时器与指定窗口相关联。 
	 */
	WM_CreateTimer(hWin,   /* 接受信息的窗口的句柄 */
				   0, 	         /* 用户定义的Id。如果不对同一窗口使用多个定时器，此值可以设置为零。 */
				   100,          /* 周期，此周期过后指定窗口应收到消息*/
				   0);	         /* 留待将来使用，应为0 */	
}

/*
*********************************************************************************************************
*	函 数 名: _cbCallbackADC
*	功能说明: 回调函数
*	形    参: pMsg   消息指针变量
*	返 回 值: 无
*********************************************************************************************************
*/
static void _cbCallbackADC(WM_MESSAGE * pMsg) 
{
    int NCode, Id;
	char buf[20];
	WM_HWIN hItem;
	uint32_t temp;
	OS_ERR  err;
	
	WM_HWIN hWin = pMsg->hWin;
	
    switch (pMsg->MsgId) 
	{
		/* 对话框关闭后使能一次波形刷新，防止在波形暂停状态下(通过K2按键将波形暂停)
		   关闭对话框并返回的时候，波形区域没有刷新，一片空白。
		*/
		case WM_DELETE:
			/* Graph控件要单独删除 */
			WM_DeleteWindow(hGraphADC);
		
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
            InitDialogADC(pMsg);
            break;
		
		/* 定时更新 */
		case WM_TIMER:  
			g_Flag->ucDsoMsg = DspMultiMeterPro_14;
			OSTaskQPost(AppTaskDsoTCB,
						(void *)&g_Flag->ucDsoMsg,    
						sizeof(g_Flag->ucDsoMsg),    
						OS_OPT_POST_FIFO,
						&err);	
						
			sprintf(buf, "%5.3f", g_uiAdcAvgSample*3.3f/4095);
			hItem = WM_GetDialogItem(hWin, ID_TEXT_0);
			TEXT_SetText(hItem, buf);
		
			sprintf(buf, "ADC1=%d", g_uiAdcAvgSample);
			hItem = WM_GetDialogItem(hWin, ID_TEXT_1);
			TEXT_SetText(hItem, buf);
		
			temp = (uint32_t)(g_uiAdcAvgSample *165) /4095;
			GRAPH_DATA_YT_AddValue(ahData, temp);
			WM_RestartTimer(pMsg->Data.v, 400);
			break;

        case WM_NOTIFY_PARENT:
            Id = WM_GetId(pMsg->hWinSrc); 
            NCode = pMsg->Data.v;        
            switch (Id) 
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
            }
            break;
        default:
            WM_DefaultProc(pMsg);
    }
}

/*
*********************************************************************************************************
*	函 数 名: DSO_CreateAdcDlg
*	功能说明: 创建对话框
*	形    参: 无        	
*	返 回 值: 无
*********************************************************************************************************
*/
WM_HWIN DSO_CreateAdcDlg(void) 
{
	WM_HWIN hWin;

	hWin = GUI_CreateDialogBox(_aDialogCreateADC, GUI_COUNTOF(_aDialogCreateADC), _cbCallbackADC, WM_HBKWIN, 0, 0);
	return hWin;
}

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
