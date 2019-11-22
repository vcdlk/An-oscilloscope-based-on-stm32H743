/*
*********************************************************************************************************
*	                                  
*	模块名称 : 示波器主界面的初始化
*	文件名称 : DSO_Init.c
*	版    本 : V1.0
*	说    明 : 示波器初始化
*********************************************************************************************************
*/

#include "MainTask.h"
/*
*********************************************************************************************************
*	                                  变量
*********************************************************************************************************
*/
WM_HWIN hDlgAmp;         /* 用于显示幅度的窗口句柄 */
WM_HWIN hDlgStatus1;     /* 用于显示平均值，峰峰值，最大值和最小值窗口句柄 */
WM_HWIN hDlgStatus2;     /* 用于显示频率和RMS窗口句柄 */
WM_HWIN hDlgScale;       /* 用于显示采样率窗口句柄 */
WM_HWIN hDlgSysInfo;     /* 用于显示系统信息窗口句柄 */

BUTTON_Handle hButton0;  /* 5个按钮句柄 */
BUTTON_Handle hButton1;
BUTTON_Handle hButton2;
BUTTON_Handle hButton3;
BUTTON_Handle hButton4;
/*
*********************************************************************************************************
*	函 数 名: _cbButton
*	功能说明: 按钮回调函数
*	形    参: pMsg  消息指针
*	返 回 值: 无
*********************************************************************************************************
*/
/*
*********************************************************************************************************
*	函 数 名: _cbButton
*	功能说明: 按钮回调函数
*	形    参: pMsg  消息指针
*	返 回 值: 无
*********************************************************************************************************
*/
static void _cbButton(WM_MESSAGE * pMsg) 
{
	WM_HWIN  hWin;
	GUI_RECT Rect;
	char buf[20];
	
	hWin  = pMsg->hWin;

	switch (pMsg->MsgId) 
	{
		case WM_PAINT:
			WM_GetClientRect(&Rect);
			if (BUTTON_IsPressed(hWin)) 
			{
				GUI_SetColor(GUI_DARKGRAY);
				GUI_AA_FillRoundedRect(Rect.x0, Rect.y0, Rect.x1, Rect.y1, 10);
				GUI_SetBkColor(GUI_DARKGRAY);
				GUI_SetColor(GUI_WHITE); 
			} 
			else 
			{
				GUI_SetColor(GUI_STCOLOR_LIGHTBLUE);
				GUI_AA_FillRoundedRect(Rect.x0, Rect.y0, Rect.x1, Rect.y1, 10);
				
				GUI_SetColor(0XEBCD9E);
				GUI_SetPenSize(2);
				GUI_AA_DrawRoundedRect(Rect.x0, Rect.y0, Rect.x1, Rect.y1, 10);
				
				GUI_SetBkColor(GUI_STCOLOR_LIGHTBLUE);
				GUI_SetColor(GUI_BLACK);   
			}
			
			BUTTON_GetText(hWin, buf, 20);
			GUI_SetFont(&GUI_FontFontSong26);
			GUI_DispStringInRect(buf, &Rect, GUI_TA_HCENTER | GUI_TA_VCENTER);
			break;
			
		default:
			BUTTON_Callback(pMsg);
	}
}

/*
*********************************************************************************************************
*	函 数 名: DSO_Init
*	功能说明: 示波器主界面初始化
*	形    参: ucCreateFlag  1:表示需要创建按键和窗口等。
*                           0:不需要创建。
*	返 回 值: 无
*********************************************************************************************************
*/
void DSO_Init(uint8_t ucCreateFlag){
	char buf[10];
	uint32_t ulTrigPos;
	/******第1步：刷新背景**************/
	GUI_SetBkColor(0x905040);
    GUI_Clear();
	GUI_MEMDEV_WriteAt(hMemDSO,40*1.28,40*1.25);
	/*******第2步：显示基本的信息******/
	GUI_SetBkColor(0x905040);
    GUI_SetColor(GUI_WHITE);
    GUI_SetFont(&GUI_FontFontSong26);
   GUI_DispStringInRect("示波器", &rClient,GUI_TA_HCENTER | GUI_TA_VCENTER);
    WM_ShowWin(WM_HBKWIN);
	/* 自动触发模式 */
	if(TriggerFlag == 0){
	/* 按键K2 :设置波形显示运行或暂停 */
			if(g_Flag->hWinRunStop == 0)
		{
			GUI_DispStringInRect("运行", &rRunMode, GUI_TA_HCENTER | GUI_TA_VCENTER);
		}
		else
		{
			GUI_DispStringInRect("暂停", &rRunMode, GUI_TA_HCENTER | GUI_TA_VCENTER);
		}
	}
		/* 普通触发模式 */
	else
	{
		/* 按键K2 :设置波形显示运行或暂停 */
		if(g_Flag->ucTrig == 0)
		{
			GUI_DispStringInRect("运行", &rRunMode, GUI_TA_HCENTER | GUI_TA_VCENTER);
		}
		else
		{
			GUI_DispStringInRect("暂停", &rRunMode, GUI_TA_HCENTER | GUI_TA_VCENTER);;
		}
	}
	/* 按键K3 :设置普通触发方式或自动触发 */
	if(TriggerFlag == 0)
	{
		GUI_DispStringInRect("自动", &rTrigMode, GUI_TA_HCENTER | GUI_TA_VCENTER);
	}
	else
	{
		GUI_DispStringInRect("触发", &rTrigMode, GUI_TA_HCENTER | GUI_TA_VCENTER);
	}
	/*****第3步：显示自动触发的触发电压********/		
	g_TrigVol->ufTrigValue = 240 - g_TrigVol->usPos;
	g_TrigVol->ufTrigValue =g_TrigVol->ufTrigValue * g_AttTable[Ch1AmpId][1] / 50000;
	sprintf(buf, "%5.3fV", g_TrigVol->ufTrigValue);
	
	GUI_DispStringInRect(buf, &rTrigValue, GUI_TA_HCENTER | GUI_TA_VCENTER);
	
	/* 显示上升沿触发的标志 */
	GUI_DrawHLine(rTrigValue.y1-10, rTrigValue.x0+10, rTrigValue.x0 + 19);
	GUI_DrawLine(rTrigValue.x0 + 19, rTrigValue.y1-10, rTrigValue.x0+30, rTrigValue.y0+8);
	GUI_DrawHLine(rTrigValue.y0+8, rTrigValue.x0+31, rTrigValue.x0 + 41);
  /* 第4步：设置摇杆按键的调节状态，并将其显示出来******/	
		if(g_Flag->hWinButState == 0)
	{
		GUI_DispStringInRect("频率", &rButState, GUI_TA_HCENTER | GUI_TA_VCENTER);
	}
	else if(g_Flag->hWinButState == 1)
	{
		GUI_DispStringInRect("波形移动A", &rButState, GUI_TA_HCENTER | GUI_TA_VCENTER);
	}
	else if(g_Flag->hWinButState == 2)
	{
		GUI_DispStringInRect("波形移动B", &rButState, GUI_TA_HCENTER | GUI_TA_VCENTER);
	}
	else if(g_Flag->hWinButState == 3)
	{
		GUI_DispStringInRect("游标垂直A", &rButState, GUI_TA_HCENTER | GUI_TA_VCENTER);
	}
	else if(g_Flag->hWinButState == 4)
	{
		GUI_DispStringInRect("游标垂直B", &rButState, GUI_TA_HCENTER | GUI_TA_VCENTER);
	}
	else if(g_Flag->hWinButState == 5)
	{
		GUI_DispStringInRect("游标水平A", &rButState, GUI_TA_HCENTER | GUI_TA_VCENTER);
	}
	else if(g_Flag->hWinButState == 6)
	{
		GUI_DispStringInRect("游标水平B", &rButState, GUI_TA_HCENTER | GUI_TA_VCENTER);
	}
	else if(g_Flag->hWinButState == 7)
	{
		GUI_DispStringInRect("触发位置", &rButState, GUI_TA_HCENTER | GUI_TA_VCENTER);
	}	
	else if(g_Flag->hWinButState == 8)
	{
		GUI_DispStringInRect("浏览波形", &rButState, GUI_TA_HCENTER | GUI_TA_VCENTER);
	}
	
	/* 第5步：实现波形的放缩***********/
	GUI_SetBkColor(GUI_BLACK);
	GUI_ClearRect(210*1.28, 6*1.25, 470*1.28, 33*1.25); 
	GUI_SetColor(GUI_YELLOW);
	GUI_DrawHLine(20*1.25, 220*1.25, 220*1.25+239*1.25);
	GUI_DrawHLine(21*1.25, 220*1.25, 220*1.25+239*1.25);
	
	GUI_SetColor(0x0040f0);
	
	/* 自动触发模式 */
	if(TriggerFlag == 0)
	{
		if((g_DSO1->sCurTriStep + g_DSO1->sCurTriPos) < 0)
		{
			ulTrigPos = 0;			
		}
		else if((g_DSO1->sCurTriStep + g_DSO1->sCurTriPos) > 1448)
		{
			ulTrigPos = 1448 * 240;				
		}
		else
		{
			ulTrigPos = (g_DSO1->sCurTriStep + g_DSO1->sCurTriPos) * 240;			
		}
	}
	/* 普通触发模式 */
	else
	{
		ulTrigPos = (724 + g_DSO1->sCurTriStep)*240;
	}
	
	/* 根据上面求得数据的触发位置来更新屏上的触发图标位置 */
	ulTrigPos = ulTrigPos / 2048;	
	GUI_FillPolygon(&aPointsTrigBrowser[0], GUI_COUNTOF(aPointsTrigBrowser), ulTrigPos+220*1.28, 13*1.25);
	
	/* 记录专门的触发位置 */
	GUI_SetColor(GUI_RED);
	GUI_DrawPixel(326*1.28, 20*1.25);
	GUI_DrawPixel(326*1.28, 21*1.25);
	
	GUI_DrawPixel(327*1.28, 20*1.25);
	GUI_DrawPixel(327*1.28, 21*1.25);
	
	GUI_DrawPixel(328*1.28, 20*1.25);
	GUI_DrawPixel(328*1.28, 21*1.25);
	
	GUI_DrawPixel(329*1.28, 20*1.25);
	GUI_DrawPixel(329*1.28, 21*1.25);
	
	GUI_DrawPixel(330*1.28, 20*1.25);
	GUI_DrawPixel(330*1.28, 21*1.25);
	
	/* 第6步：波形显示区的边框******/
	GUI_SetColor(0XEBCD9E);
	GUI_DrawRect(DSOSCREEN_STARTX - 1,  /* 左上角X轴位置 */
							 DSOSCREEN_STARTY - 1,  /* 左上角Y轴位置 */
	             DSOSCREEN_ENDX + 1,    /* 右下角X轴位置 */
	             DSOSCREEN_ENDY + 1);   /* 右下角Y轴位置 */

	GUI_SetColor(0XB37F63);
	GUI_DrawRect(DSOSCREEN_STARTX - 2,  /* 左上角X轴位置 */
				 DSOSCREEN_STARTY - 2,  /* 左上角Y轴位置 */
	             DSOSCREEN_ENDX + 2,    /* 右下角X轴位置 */
	             DSOSCREEN_ENDY + 2);   /* 右下角Y轴位置 */
		/* 根据需要是否需要重新创建按键和窗口 */
		if(ucCreateFlag == 1)
	{	
		/* 第7步：创建状态窗口*************************************************************/
		hDlgAmp =   CreateAmplitudeDlg();       
		hDlgStatus1 = CreateStatus1Dlg();
		hDlgStatus2 = CreateStatus2Dlg();     
		hDlgScale = CreateScaleDlg();      
		hDlgSysInfo = CreateSysInfoDlg();   	
//		/* 第6步：创建需要的按钮*************************************************************/
		hButton0 = BUTTON_Create(670*1.28, 40*1.25, 100*1.28, 44*1.25, GUI_ID_BUTTON0, WM_CF_SHOW);
		BUTTON_SetText(hButton0, "测量");
		WM_SetHasTrans(hButton0);
		WM_SetCallback(hButton0, _cbButton);

		hButton1 = BUTTON_Create(670*1.28, 90*1.25-1, 100*1.28, 44*1.25, GUI_ID_BUTTON1, WM_CF_SHOW);
		BUTTON_SetText(hButton1, "ADC");
		WM_SetHasTrans(hButton1);
		WM_SetCallback(hButton1, _cbButton);

		hButton2 = BUTTON_Create(670*1.28, 140*1.25-2, 100*1.28, 44*1.25, GUI_ID_BUTTON2, WM_CF_SHOW);
		BUTTON_SetText(hButton2, "DAC");
		WM_SetHasTrans(hButton2);
		WM_SetCallback(hButton2, _cbButton);
		
		hButton3 = BUTTON_Create(670*1.28, 190*1.25-3, 100*1.28, 44*1.25, GUI_ID_BUTTON3, WM_CF_SHOW);
		BUTTON_SetText(hButton3, "数学");
		WM_SetHasTrans(hButton3);
		WM_SetCallback(hButton3, _cbButton);

		hButton4 = BUTTON_Create(670*1.28, 240*1.25-4, 100*1.28, 44*1.25, GUI_ID_BUTTON4, WM_CF_SHOW);
		BUTTON_SetText(hButton4, "设置");
		WM_SetHasTrans(hButton4);
		WM_SetCallback(hButton4, _cbButton);
	}
/****第8步：显示参考坐标***********/
	GUI_SetColor(GUI_YELLOW);
	GUI_FillPolygon(&aPoints[0], GUI_COUNTOF(aPoints), 5, g_DSO1->usRefPos);
	
	GUI_SetColor(GUI_BLACK);
  GUI_SetFont(&GUI_Font20_1);
	GUI_SetTextMode(GUI_TEXTMODE_TRANS);					
	GUI_DispCharAt('1', 10, g_DSO1->usRefPos - 9);
	
	GUI_SetColor(GUI_GREEN);
	GUI_FillPolygon(&aPoints[0], GUI_COUNTOF(aPoints), 5, g_DSO2->usRefPos);
	
	GUI_SetColor(GUI_BLACK);
    GUI_SetFont(&GUI_Font20_1);
	GUI_SetTextMode(GUI_TEXTMODE_TRANS);					
	GUI_DispCharAt('2', 10, g_DSO2->usRefPos - 9);
	
	GUI_SetColor(GUI_BLACK);
	GUI_FillPolygon(&aPoints[0], GUI_COUNTOF(aPoints), 5, 430+40);	
	GUI_SetColor(GUI_RED);
	GUI_DrawPolygon(&aPoints[0], GUI_COUNTOF(aPoints), 5, 430+40);

	GUI_SetColor(GUI_RED);
  GUI_SetFont(&GUI_Font20_1);
	GUI_SetTextMode(GUI_TEXTMODE_TRANS);					
	GUI_DispCharAt('M', 9, 430 + 33);
}













