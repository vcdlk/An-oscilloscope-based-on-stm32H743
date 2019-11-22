///*
//*********************************************************************************************************
//*	                                  
//*	模块名称 : 示波器DAC对话框
//*	文件名称 : DSO_DACDlg.c
//*	版    本 : V1.0
//*	说    明 : 示波器DAC对话框。
//*
//*	修改记录 :
//*		版本号    日期          作者           说明
//*		V1.0    2018-01-06     Eric2013        首发 
//*                                           
//*	Copyright (C), 2018-2028, 安富莱电子 www.armfly.com
//*
//*********************************************************************************************************
//*/
//#include "includes.h"
//#include "MainTask.h"



///*
//*********************************************************************************************************
//*                                       宏定义
//*********************************************************************************************************
//*/
//#define MAG           3
//#define ID_WINDOW_0   (GUI_ID_USER + 0x00)
//#define ID_GRAPH_0    (GUI_ID_USER + 0x01)
//#define ID_BUTTON_0   (GUI_ID_USER + 0x02)
//#define ID_BUTTON_1   (GUI_ID_USER + 0x03)
//#define ID_BUTTON_2   (GUI_ID_USER + 0x04)
//#define ID_EDIT_0     (GUI_ID_USER + 0x05)
//#define ID_SLIDER_0   (GUI_ID_USER + 0x06)
//#define ID_SLIDER_1   (GUI_ID_USER + 0x07)
//#define ID_TEXT_0     (GUI_ID_USER + 0x08)
//#define ID_TEXT_1     (GUI_ID_USER + 0x09)
//#define ID_TEXT_2     (GUI_ID_USER + 0x0A)
//#define ID_TEXT_3     (GUI_ID_USER + 0x0B)

//#define ID_BUTTON_3   (GUI_ID_USER + 0x0C)
//#define ID_BUTTON_4   (GUI_ID_USER + 0x0D)

//#define MSG_CARD_INSERTED   (WM_USER + 0)
//#define MSG_CARD_REMOVED    (WM_USER + 1)
//#define MSG_MONEY_REMOVED   (WM_USER + 2)
//#define MSG_PIN_CHANGED     (WM_USER + 3)
//#define MSG_PIN_OK          (WM_USER + 4)
//#define MSG_PIN_CANCEL      (WM_USER + 5)
//#define MSG_PIN_ERROR       (WM_USER + 6)

///* 小键盘相关的宏定义 */
//#define NUMPAD_BKCOLOR            GUI_LIGHTGRAY
//#define NUMPAD_EFFECT             (&WIDGET_Effect_3D2L)
//#define NUMPAD_BORDER             20
//#define NUMPAD_PIN                1685

//#define NUMPAD_BUTTON_BKCOLOR0    0xB8B8B8              /* Unpressed */
//#define NUMPAD_BUTTON_BKCOLOR1    0xE0E0E0              /* Pressed   */
//#define NUMPAD_BUTTON_BKCOLOR2    0xC0C0C0              /* Disabled  */
//#define NUMPAD_BUTTON_COLOR0      0x700000              /* Unpressed */
//#define NUMPAD_BUTTON_COLOR1      0x700000              /* Pressed   */
//#define NUMPAD_BUTTON_RED0        0x0060FF              /* Unpressed */
//#define NUMPAD_BUTTON_RED1        0x0030E8              /* Pressed   */
//#define NUMPAD_BUTTON_BLUE0       0x00FF00              /* Unpressed */
//#define NUMPAD_BUTTON_BLUE1       0x00FF7F              /* Pressed   */
//#define NUMPAD_BUTTON_GREEN0      0x00B040              /* Unpressed */
//#define NUMPAD_BUTTON_GREEN1      0x00D000              /* Pressed   */
//#define NUMPAD_BUTTON_FONT        (&GUI_Font16_1)
//#define NUMPAD_BUTTON_EFFECT      (&WIDGET_Effect_3D1L)
//#define NUMPAD_BUTTON_WIDTH       70
//#define NUMPAD_BUTTON_HEIGHT      50
//#define NUMPAD_BUTTON_DIST_X      8
//#define NUMPAD_BUTTON_DIST_Y      8

//#define NUMPAD_ID_0         (GUI_ID_USER +  0 + 100)
//#define NUMPAD_ID_1         (GUI_ID_USER +  1 + 100)
//#define NUMPAD_ID_2         (GUI_ID_USER +  2 + 100)
//#define NUMPAD_ID_3         (GUI_ID_USER +  3 + 100)
//#define NUMPAD_ID_4         (GUI_ID_USER +  4 + 100)
//#define NUMPAD_ID_5         (GUI_ID_USER +  5 + 100)
//#define NUMPAD_ID_6         (GUI_ID_USER +  6 + 100)
//#define NUMPAD_ID_7         (GUI_ID_USER +  7 + 100)
//#define NUMPAD_ID_8         (GUI_ID_USER +  8 + 100)
//#define NUMPAD_ID_9         (GUI_ID_USER +  9 + 100)
//#define NUMPAD_ID_X         (GUI_ID_USER + 10 + 100)
//#define NUMPAD_ID_Y         (GUI_ID_USER + 11 + 100)
//#define NUMPAD_ID_A         (GUI_ID_USER + 12 + 100)
//#define NUMPAD_ID_B         (GUI_ID_USER + 13 + 100)
//#define NUMPAD_ID_C         (GUI_ID_USER + 14 + 100)
//#define NUMPAD_ID_D         (GUI_ID_USER + 15 + 100)

//#define TEXT_ID_ABBRUCH             1
//#define TEXT_ID_KORREKTUR           2
//#define TEXT_ID_BESTAETIGEN         3
//#define TEXT_ID_PIN_EINGEBEN        4
//#define TEXT_ID_KARTE_EINFUEHREN    5
//#define TEXT_ID_GELD_ABHEBEN        6
//#define TEXT_ID_KONTOSTAND_ANSEHEN  7
//#define TEXT_ID_AUSWAHL_TREFFEN     8
//#define TEXT_ID_ANDERER_BETRAG      9
//#define TEXT_ID_BITTE_GEDULD        10
//#define TEXT_ID_KARTE_ENTNEHMEN     11
//#define TEXT_ID_GELD_ENTNEHMEN      12
//#define TEXT_ID_VIELEN_DANK         13
//#define TEXT_ID_GELDAUTOMAT         14
//#define TEXT_ID_KONTOSTAND          15
//#define TEXT_ID_AUSWAHL_BETRAG      16
//#define TEXT_ID_FALSCHE_PIN         17
//#define TEXT_ID_BETRAG_EINGEBEN     18


///*
//*********************************************************************************************************
//*	                                     变量
//*********************************************************************************************************
//*/
//const char *g_WaveType[] =
//{
//    "Sine",
//	"Square",
//	"Ramp",
//};
//  
//static GRAPH_DATA_Handle  ahData;
//static WM_HWIN   hGraphDAC;
//static WM_HTIMER hTimerDac = WM_HWIN_NULL;
//static WM_HWIN   _hNumPad;

//static unsigned int _PIN_Value = 10000;
//static unsigned int _PIN_Digits = 5;

//DAC_T g_DAC1;

///*
//*********************************************************************************************************
//*	函 数 名: _cbButtonBack
//*	功能说明: 按钮回调函数
//*	形    参: pMsg  消息指针
//*	返 回 值: 无
//*********************************************************************************************************
//*/
//static void _cbButtonBack(WM_MESSAGE * pMsg) 
//{
//	WM_HWIN  hWin;

//	hWin  = pMsg->hWin;
//	switch (pMsg->MsgId) 
//	{
//		case WM_PAINT:
//			if (BUTTON_IsPressed(hWin)) 
//			{
//				GUI_SetColor(GUI_DARKGRAY);
//				GUI_AA_FillCircle(100, 0, 72);
//				GUI_SetBkColor(GUI_DARKGRAY);
//				GUI_SetColor(GUI_WHITE); 
//				GUI_DrawBitmap(&bmReturn, 40, 5);
//			} 
//			else 
//			{
//				GUI_SetColor(GUI_STCOLOR_LIGHTBLUE);
//				GUI_AA_FillCircle(100, 0, 72);
//				GUI_SetBkColor(GUI_STCOLOR_LIGHTBLUE);
//				GUI_SetColor(GUI_WHITE);  
//				GUI_DrawBitmap(&bmReturn, 40, 5);				
//			}
//			break;
//			
//		default:
//			BUTTON_Callback(pMsg);
//	}
//}

///*
//*********************************************************************************************************
//*	函 数 名: _cbButtonSettings
//*	功能说明: 按钮回调函数
//*	形    参: pMsg  消息指针
//*	返 回 值: 无
//*********************************************************************************************************
//*/
//static void _cbButtonSettings(WM_MESSAGE * pMsg) 
//{
//	WM_HWIN  hWin;

//	hWin  = pMsg->hWin;
//	switch (pMsg->MsgId) 
//	{
//		case WM_PAINT:
//			if (BUTTON_IsPressed(hWin)) 
//			{
//				GUI_SetColor(GUI_DARKGRAY);
//				GUI_AA_FillCircle(0, 0, 72);
//				GUI_SetBkColor(GUI_DARKGRAY);
//				GUI_SetColor(GUI_WHITE); 
//				
//				GUI_DrawBitmap(&bmSettings, 0, 5);
//			} 
//			else 
//			{
//				GUI_SetColor(GUI_STCOLOR_LIGHTBLUE);
//				GUI_AA_FillCircle(0, 0, 72);
//				GUI_SetBkColor(GUI_STCOLOR_LIGHTBLUE);
//				GUI_SetColor(GUI_WHITE);  

//				GUI_DrawBitmap(&bmSettings, 0, 5);
//			}
//			break;
//			
//		default:
//			BUTTON_Callback(pMsg);
//	}
//}

///*
//*********************************************************************************************************
//* 函 数 名: _cbButtonCom
//* 功能说明: 按钮回调函数
//* 形    参: pMsg 消息指针
//* 返 回 值: 无
//*********************************************************************************************************
//*/
//static void _cbButtonCom(WM_MESSAGE * pMsg)
//{
//	WM_HWIN hWin;
//	GUI_RECT Rect;
//	char buf[30];
//	
//	hWin = pMsg->hWin;
//	
//	switch (pMsg->MsgId)
//	{
//		case WM_PAINT:
//			WM_GetClientRect(&Rect);
//			if (BUTTON_IsPressed(hWin))
//			{
//				GUI_SetColor(GUI_DARKGRAY);
//				GUI_AA_FillRoundedRect(Rect.x0, Rect.y0, Rect.x1, Rect.y1, 5);
//				GUI_SetBkColor(GUI_DARKGRAY);
//				GUI_SetColor(GUI_BLACK);
//			}
//			else
//			{
//				GUI_SetColor(GUI_LIGHTBLUE);
//				GUI_AA_FillRoundedRect(Rect.x0, Rect.y0, Rect.x1, Rect.y1, 5);
//				GUI_SetBkColor(GUI_LIGHTBLUE);
//				GUI_SetColor(GUI_BLACK);
//			}
//			GUI_SetFont(&GUI_FontLubalGraph24);
//			BUTTON_GetText(hWin, buf, 20);
//			GUI_DispStringInRect(buf, &Rect, GUI_TA_HCENTER | GUI_TA_VCENTER);
//			break;
//			
//		default:
//			BUTTON_Callback(pMsg);
//	}
//}

///*
//*********************************************************************************************************
//*	函 数 名: _cbNumPad
//*	功能说明: 小键盘的回调函数
//*	形    参: pMsg  参数指针
//*	返 回 值: 无
//*********************************************************************************************************
//*/
//static void _cbNumPad(WM_MESSAGE* pMsg) 
//{
//	WM_HWIN hWin = pMsg->hWin;
//	
//	switch (pMsg->MsgId) 
//	{
//		case WM_CREATE:
//			/* 创建后设置聚焦 */
//			WM_SetFocus(hWin);
//			break;

//		case WM_PAINT: 
//			GUI_SetBkColor(GUI_WHITE);
//			GUI_Clear();
//			break;
//			
//		case WM_NOTIFY_PARENT:
//			if (pMsg->Data.v == WM_NOTIFICATION_RELEASED) 
//			{
//				int Id = WM_GetId(pMsg->hWinSrc);
//				switch (Id) 
//				{
//					case NUMPAD_ID_0:
//					case NUMPAD_ID_1:
//					case NUMPAD_ID_2:
//					case NUMPAD_ID_3:
//					case NUMPAD_ID_4:
//					case NUMPAD_ID_5:
//					case NUMPAD_ID_6:
//					case NUMPAD_ID_7:
//					case NUMPAD_ID_8:
//					case NUMPAD_ID_9:
//						/* 通过这种方式将每次输入的数值添加到之前数值的末尾 */						
//						/* _PIN_Digits是设置密码位数的 */
//						if (_PIN_Digits < 5) 
//						{
//							_PIN_Value = (_PIN_Value * 10) + (Id - NUMPAD_ID_0);
//							_PIN_Digits++;
//							WM_SendMessageNoPara(WM_GetParent(hWin), MSG_PIN_CHANGED);
//						}
//						break;
//					
//					/* 清空输入 */
//					case NUMPAD_ID_A:   
//						_PIN_Value = 0;
//						_PIN_Digits = 0;
//						WM_SendMessageNoPara(WM_GetParent(hWin), MSG_PIN_CANCEL);
//						break;
//					
//					/* 更正输入值 */
//					case NUMPAD_ID_B:   
//						if (_PIN_Digits) 
//						{
//							_PIN_Value /= 10;
//							_PIN_Digits--;
//							WM_SendMessageNoPara(WM_GetParent(hWin), MSG_PIN_CHANGED);
//						}
//						break;
//					
//					/* 确定频率并更新 */
//					case NUMPAD_ID_D: 
//						/* 范围要是1Hz - 50KHz，否则按输入错误处理 */
//						if ((_PIN_Value == 0)||(_PIN_Value > 50000)) 
//						{
//							_PIN_Value = 0;
//							_PIN_Digits = 0;
//							WM_SendMessageNoPara(WM_GetParent(hWin), MSG_PIN_ERROR);
//						} 
//						else 
//						{
//							WM_SendMessageNoPara(WM_GetParent(hWin), MSG_PIN_OK);
//						}
//						break;
//				}
//			}
//			break;
//		
//		default:
//			WM_DefaultProc(pMsg);
//	}
//}

///*
//*********************************************************************************************************
//*	函 数 名: _CreateNumPadButton
//*	功能说明: 创建小键盘所需要的按钮
//*	形    参: hParent  父窗口
//*             pText    按键上显示的文本
//*             Id       按钮Id         
//*             x        x轴坐标
//*             y        y轴坐标
//*             w        按钮宽
//*             h        按钮高
//*             Color    按钮颜色
//*             PressedColor  按钮被按下后的颜色
//*             Enable   使能或者禁止状态
//*             TextId   文本的ID          
//*	返 回 值: 无
//*********************************************************************************************************
//*/
//static WM_HWIN _CreateNumPadButton(WM_HWIN hParent, const char* pText, int Id,
//                                   int x, int y, int w, int h,
//                                   GUI_COLOR Color, GUI_COLOR PressedColor, int Enable, unsigned TextId)
//{
//	WM_HWIN hButton;
//	
//	hButton = BUTTON_CreateEx(x, y, w, h, hParent, WM_CF_SHOW, 0, Id);
//	if (Enable) 
//	{
//		if (TextId) 
//		{
//			//pText = _GetLang(TextId);
//		}
//		
//		BUTTON_SetText(hButton, pText);
//	} 
//	else 
//	{
//		WM_DisableWindow(hButton);
//	}
//	
//	WM_SetHasTrans(hButton);
//	WM_SetCallback(hButton, _cbButtonCom);
//	
//	return hButton;
//}

///*
//*********************************************************************************************************
//*	函 数 名: _CreateNumPad
//*	功能说明: 创建小键盘
//*	形    参: hParent  父窗口
//*             x        x轴坐标
//*             y        y轴坐标
//*	返 回 值: 无
//*********************************************************************************************************
//*/
//static WM_HWIN _CreateNumPad(WM_HWIN hParent, int x, int y) 
//{
//	int w, h, bx, by, bw, bh, dx, dy;

//	/* 位置和大小 */
//	bx = NUMPAD_BORDER;
//	by = NUMPAD_BORDER;
//	bw = NUMPAD_BUTTON_WIDTH;
//	bh = NUMPAD_BUTTON_HEIGHT;
//	dx = NUMPAD_BUTTON_DIST_X;
//	dy = NUMPAD_BUTTON_DIST_Y;
//	w  = (bx * 2) + (bw * 9/2) + (dx * 5);
//	h  = (by * 2) + (bh * 4)    + (dy * 3);
//	x -= (w >> 1);
//	y -= (h >> 1);

//	/* 创建键盘窗口   */
//	_hNumPad = WM_CreateWindowAsChild(x, y, w, h, hParent, WM_CF_SHOW, _cbNumPad, 0);
//	
//	/* 为键盘窗口创建按钮 */
//	_CreateNumPadButton(_hNumPad, "1", NUMPAD_ID_1, bx + 0*(bw+dx),   by + 0*(bh+dy), bw,     bh, NUMPAD_BUTTON_COLOR0,  NUMPAD_BUTTON_COLOR1,  1, 0);
//	_CreateNumPadButton(_hNumPad, "2", NUMPAD_ID_2, bx + 1*(bw+dx),   by + 0*(bh+dy), bw,     bh, NUMPAD_BUTTON_COLOR0,  NUMPAD_BUTTON_COLOR1,  1, 0);
//	_CreateNumPadButton(_hNumPad, "3", NUMPAD_ID_3, bx + 2*(bw+dx),   by + 0*(bh+dy), bw,     bh, NUMPAD_BUTTON_COLOR0,  NUMPAD_BUTTON_COLOR1,  1, 0);
//	_CreateNumPadButton(_hNumPad, "4", NUMPAD_ID_4, bx + 0*(bw+dx),   by + 1*(bh+dy), bw,     bh, NUMPAD_BUTTON_COLOR0,  NUMPAD_BUTTON_COLOR1,  1, 0);
//	_CreateNumPadButton(_hNumPad, "5", NUMPAD_ID_5, bx + 1*(bw+dx),   by + 1*(bh+dy), bw,     bh, NUMPAD_BUTTON_COLOR0,  NUMPAD_BUTTON_COLOR1,  1, 0);
//	_CreateNumPadButton(_hNumPad, "6", NUMPAD_ID_6, bx + 2*(bw+dx),   by + 1*(bh+dy), bw,     bh, NUMPAD_BUTTON_COLOR0,  NUMPAD_BUTTON_COLOR1,  1, 0);
//	_CreateNumPadButton(_hNumPad, "7", NUMPAD_ID_7, bx + 0*(bw+dx),   by + 2*(bh+dy), bw,     bh, NUMPAD_BUTTON_COLOR0,  NUMPAD_BUTTON_COLOR1,  1, 0);
//	_CreateNumPadButton(_hNumPad, "8", NUMPAD_ID_8, bx + 1*(bw+dx),   by + 2*(bh+dy), bw,     bh, NUMPAD_BUTTON_COLOR0,  NUMPAD_BUTTON_COLOR1,  1, 0);
//	_CreateNumPadButton(_hNumPad, "9", NUMPAD_ID_9, bx + 2*(bw+dx),   by + 2*(bh+dy), bw,     bh, NUMPAD_BUTTON_COLOR0,  NUMPAD_BUTTON_COLOR1,  1, 0);
//	_CreateNumPadButton(_hNumPad, "-", NUMPAD_ID_X, bx + 0*(bw+dx),   by + 3*(bh+dy), bw,     bh, NUMPAD_BUTTON_COLOR0,  NUMPAD_BUTTON_COLOR1,  1, 0) ;
//	_CreateNumPadButton(_hNumPad, "0", NUMPAD_ID_0, bx + 1*(bw+dx),   by + 3*(bh+dy), bw,     bh, NUMPAD_BUTTON_COLOR0,  NUMPAD_BUTTON_COLOR1,  1, 0);
//	_CreateNumPadButton(_hNumPad, "-", NUMPAD_ID_Y, bx + 2*(bw+dx),   by + 3*(bh+dy), bw,     bh, NUMPAD_BUTTON_COLOR0,  NUMPAD_BUTTON_COLOR1,  1, 0);
//	_CreateNumPadButton(_hNumPad, "CL",     NUMPAD_ID_A, bx + 3*bw + 5*dx, by + 0*(bh+dy), bw*3/2, bh, NUMPAD_BUTTON_RED0,    NUMPAD_BUTTON_RED1,    1, TEXT_ID_ABBRUCH);
//	_CreateNumPadButton(_hNumPad, "C",      NUMPAD_ID_B, bx + 3*bw + 5*dx, by + 1*(bh+dy), bw*3/2, bh, NUMPAD_BUTTON_BLUE0,   NUMPAD_BUTTON_BLUE1,   1, TEXT_ID_KORREKTUR);
//	_CreateNumPadButton(_hNumPad, "-",      NUMPAD_ID_C, bx + 3*bw + 5*dx, by + 2*(bh+dy), bw*3/2, bh, NUMPAD_BUTTON_COLOR0,  NUMPAD_BUTTON_COLOR1,  1, 0);
//	_CreateNumPadButton(_hNumPad, "Output", NUMPAD_ID_D, bx + 3*bw + 5*dx, by + 3*(bh+dy), bw*3/2, bh, NUMPAD_BUTTON_GREEN0,  NUMPAD_BUTTON_GREEN1,  1, TEXT_ID_BESTAETIGEN);
//	
//	return _hNumPad;
//}

///*
//*********************************************************************************************************
//*	               					对话框控件列表
//*********************************************************************************************************
//*/
//static const GUI_WIDGET_CREATE_INFO _aDialogCreateDAC[] = {
//	{ WINDOW_CreateIndirect, "Window",  ID_WINDOW_0,  0,  0, 800, 480,  0, 0x0, 0},

//	{ GRAPH_CreateIndirect,  "Graph",  ID_GRAPH_0, 	  8, 100, 384, 190, 0, 0x0, 0 },
//	{ BUTTON_CreateIndirect, "Sine",   ID_BUTTON_0,   2, 425, 120,  45, 0, 0x0, 0 },
//	{ BUTTON_CreateIndirect, "Square", ID_BUTTON_1, 140, 425, 120,  45, 0, 0x0, 0 },
//	{ BUTTON_CreateIndirect, "Ramp",   ID_BUTTON_2, 278, 425, 120,  45, 0, 0x0, 0 },
//	
//	{ EDIT_CreateIndirect, "Edit", ID_EDIT_0, 470, 140, 260, 45, 0, 0x64, 0 },
//	{ TEXT_CreateIndirect, "Text", ID_TEXT_0,   2, 300, 300, 25, 0, 0x64, 0 },
//	
//	{ SLIDER_CreateIndirect, "Slider", ID_SLIDER_0,  10, 330, 380, 25, 0, 0x0,  0 },
//	{ TEXT_CreateIndirect,   "Text",     ID_TEXT_1,   2, 360, 300, 25, 0, 0x64, 0 },
//	{ TEXT_CreateIndirect,   "Text",     ID_TEXT_2, 200,  10, 400, 32, 0, 0x64, 0 },
//	{ TEXT_CreateIndirect,   "Text",     ID_TEXT_3,  10, 102, 382, 16, 0, 0x64, 0 },
//		
//	{ SLIDER_CreateIndirect, "Slider", ID_SLIDER_1, 10, 390, 380, 25, 0, 0x0, 0 },
//	
//	{ BUTTON_CreateIndirect, "DlgBack",    ID_BUTTON_3,  700,  0, 100, 100, 0, 0, 0},	
//	{ BUTTON_CreateIndirect, "MusicList",  ID_BUTTON_4,    0,  0, 100, 100, 0, 0, 0},
//};

///*
//*********************************************************************************************************
//*	函 数 名: InitDialogSys
//*	功能说明: 对话框回调函数的初始化消息
//*	形    参: pMsg   消息指针变量
//*	返 回 值: 无
//*********************************************************************************************************
//*/
//static void InitDialogReserved(WM_MESSAGE * pMsg)
//{
//    WM_HWIN hWin = pMsg->hWin;
//	WM_HWIN hItem;
//	uint16_t i;
//	char acBuffer[50] = {0};
//	char *pBuffer;
//	

//    //
//    // 初始化按钮
//    //	
//	hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
//    BUTTON_SetFont(hItem, GUI_FONT_16_1);
//	WM_SetHasTrans(hItem);
//	WM_SetCallback(hItem, _cbButtonCom);
//	
//	hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1);
//    BUTTON_SetFont(hItem, GUI_FONT_16_1);
//	WM_SetHasTrans(hItem);
//	WM_SetCallback(hItem, _cbButtonCom);
//	
//	hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_2);
//    BUTTON_SetFont(hItem, GUI_FONT_16_1);
//	WM_SetHasTrans(hItem);
//	WM_SetCallback(hItem, _cbButtonCom);
//	
//	hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_3);
//	WM_SetHasTrans(hItem);
//	WM_SetCallback(hItem, _cbButtonBack);
//	
//	hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_4);
//	WM_SetHasTrans(hItem);
//	WM_SetCallback(hItem, _cbButtonSettings);
//	
//    //
//    // 初始化编辑框
//    //
//    hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_0);
//	if(_PIN_Digits == 0)
//	{
//		EDIT_SetText(hItem, "");		
//	}
//	else
//	{
//		pBuffer = acBuffer;
//		/* 将字符转换成10进制数放在编辑框里 */
//		GUI_AddDecShift(_PIN_Value, _PIN_Digits, 0, &pBuffer);
//		hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_0);
//		EDIT_SetText(hItem, acBuffer);
//	}

//    EDIT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
//    EDIT_SetFont(hItem, &GUI_FontLubalGraph32);
//	
//    //
//    // 初始化文本控件
//    //
//    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
//    TEXT_SetText(hItem, "DAC:0 - 4095");
//    TEXT_SetFont(hItem, &GUI_FontLubalGraph24);
//	
//    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
//    TEXT_SetFont(hItem, &GUI_FontLubalGraph24);
//    TEXT_SetText(hItem, "Duty:2% - 98%");
//	
//	hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_2);
//    TEXT_SetFont(hItem, &GUI_FontLubalGraph32);
//	TEXT_SetTextColor(hItem, GUI_RED);
//	TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
//    TEXT_SetText(hItem, "Waveform Generator");
//	
//	hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_3);
//    TEXT_SetFont(hItem, &GUI_Font16_1);
//	TEXT_SetTextColor(hItem, GUI_GREEN);
//	
//	sprintf(acBuffer, "Type:%s  AMP:%5.3fV  DAC:%d  Duty:%d%%  Freq:%dHz", 
//	        g_WaveType[g_DAC1.ucWaveType],
//			(float)g_DAC1.ulAMP*3.3f/4095,
//			g_DAC1.ulAMP,
//			g_DAC1.ucDuty,
//			g_DAC1.ulFreq);
//			 
//    TEXT_SetText(hItem, acBuffer);

//    //
//    // 创建小键盘
//    //	
//	_CreateNumPad(hWin, 600, 358);

//    //
//    // 初始化slider控件
//    //
//	hItem = WM_GetDialogItem(pMsg->hWin, ID_SLIDER_0); 
//	SLIDER_SetBkColor(hItem, GUI_WHITE);
//	SLIDER_SetFocusColor (hItem, GUI_STCOLOR_LIGHTBLUE);
//	SLIDER_SetRange(hItem,  0, 4095);
//	SLIDER_SetValue(hItem,  g_DAC1.ulAMP);
//	SLIDER_SetWidth(hItem,  0);
//	SLIDER_SetSTSkin(hItem);
//	
//	hItem = WM_GetDialogItem(pMsg->hWin, ID_SLIDER_1); 
//	SLIDER_SetBkColor(hItem, GUI_WHITE);
//	SLIDER_SetFocusColor (hItem, GUI_STCOLOR_LIGHTBLUE);
//	SLIDER_SetRange(hItem,  2, 98);
//	SLIDER_SetValue(hItem,  g_DAC1.ucDuty);
//	SLIDER_SetWidth(hItem,  0);
//	SLIDER_SetSTSkin(hItem);

//	//
//	// 初始化Graph控件
//	//
//	hGraphDAC = WM_GetDialogItem(pMsg->hWin, ID_GRAPH_0);

//	/* 创建数据对象 *******************************************************/
//	ahData = GRAPH_DATA_YT_Create(GUI_GREEN, 384, 0, 0);

//	/* 数据对象添加到图形控件 */
//	GRAPH_AttachData(hGraphDAC, ahData);

//	/* 设置Y轴方向的栅格间距 */
//	GRAPH_SetGridDistY(hGraphDAC, 50);

//	/* 固定X轴方向的栅格 */
//	GRAPH_SetGridFixedX(hGraphDAC, 1);

//	/* 设置栅格可见 */
//	GRAPH_SetGridVis(hGraphDAC, 1);
//	
//	GRAPH_SetLineStyleH(hGraphDAC, GUI_LS_DOT);
//	GRAPH_SetLineStyleV(hGraphDAC, GUI_LS_DOT);

//	/* 设置上下左右边界的大小 */
//	GRAPH_SetBorder(hGraphDAC, 0, 0, 0, 0);

//	for(i = 0; i < 384; i++)
//	{
//		GRAPH_DATA_YT_AddValue(ahData, 150*g_Wave1[i%128]*g_DAC1.ulAMP/4095/4095);
//	}
//	
//	GRAPH_DATA_YT_SetOffY(ahData, 5);
//}

///*
//*********************************************************************************************************
//*	函 数 名: _cbCallbackDAC
//*	功能说明: 回调函数
//*	形    参: pMsg   消息指针变量
//*	返 回 值: 无
//*********************************************************************************************************
//*/
//static void _cbCallbackDAC(WM_MESSAGE * pMsg) 
//{
//    int NCode, Id, i;
//	WM_HWIN hItem;
//	char acBuffer[50] = {0};
//	char * pBuffer;
//	static int PinError;
//	
//	
//	WM_HWIN hWin = pMsg->hWin;
//	
//    switch (pMsg->MsgId) 
//    {
//		/* 对话框关闭后使能一次波形刷新，防止在波形暂停状态下(通过K2按键将波形暂停)
//		   关闭对话框并返回的时候，波形区域没有刷新，一片空白。
//		*/
//		case WM_DELETE:
//			/* Graph控件要单独删除 */
//			WM_DeleteWindow(hGraphDAC);
//		
//			/* 自动触发暂停状态 */
//			if(g_Flag->hWinRunStop == 1)
//			{
//				g_Flag->ucWaveRefresh = 1;
//			}
//		
//			/* 普通触发暂停状态 */
//			if(TriggerFlag == 1)
//			{
//				TriggerFlag = 2;
//			}
//			break;
//		
//		case WM_INIT_DIALOG:
//            InitDialogReserved(pMsg);
//            break;
//		
//		case WM_PAINT:
//			GUI_SetBkColor(GUI_WHITE);
//		    GUI_Clear();
//			if (PinError) 
//			{
//				_PIN_Digits = 0;
//				GUI_SetColor(GUI_RED);
//				GUI_SetFont(&GUI_FontLubalGraph24);
//				GUI_DispStringHCenterAt("Wrong Value:1Hz - 50KHz", 599, 110);
//				if(hTimerDac == WM_HWIN_NULL)
//				{
//					hTimerDac = WM_CreateTimer(hWin, 0, 1500, 0);					
//				}
//				else
//				{
//					WM_RestartTimer(hTimerDac, 1500);							
//				}
//			}
//			else
//			{
//				GUI_SetColor(GUI_BLACK);
//				GUI_SetFont(&GUI_FontLubalGraph24);
//				GUI_DispStringHCenterAt("Input Freq:1Hz - 50KHz", 599, 110);
//			}
//			break;
//			
//		/* 更新编辑框频率值 */
//		case MSG_PIN_CHANGED:
//			pBuffer = acBuffer;
//		    /* 将字符转换成10进制数放在编辑框里 */
//			GUI_AddDecShift(_PIN_Value, _PIN_Digits, 0, &pBuffer);
//		    hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_0);
//			EDIT_SetText(hItem, acBuffer);
//			break;	

//		/* 清空编辑框频率值 */
//		case MSG_PIN_CANCEL:
//			hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_0);
//			EDIT_SetText(hItem, "");
//			break;
//		
//		case WM_TIMER:
//			PinError = 0;
//			EDIT_SetText(WM_GetDialogItem(hWin, ID_EDIT_0), "");
//			WM_InvalidateWindow(hWin);
//			/* 这里没有做删除操作，不管创建多少，关闭其父窗口后就自动删除了 */
//			//WM_DeleteTimer(hTimerDac);
//			break;
//		
//		/* 频率值超出了范围 */
//		case MSG_PIN_ERROR:
//			PinError++;
//			WM_InvalidateWindow(hWin);
//			break;
//		
//		/* 更新频率值 */
//		case MSG_PIN_OK:
//			g_DAC1.ulFreq = _PIN_Value;
//			if(g_DAC1.ucWaveType == 0)
//			{
//				dac1_SetSinWave(g_DAC1.ulAMP, g_DAC1.ulFreq);
//			}
//			else if(g_DAC1.ucWaveType == 1)
//			{
//				dac1_SetRectWave(0, g_DAC1.ulAMP, g_DAC1.ulFreq, g_DAC1.ucDuty);
//			}
//			else if(g_DAC1.ucWaveType == 2)
//			{
//				dac1_SetTriWave(0, g_DAC1.ulAMP, g_DAC1.ulFreq, g_DAC1.ucDuty);
//			}
//			
//			for(i = 0; i < 384; i++)
//			{
//				GRAPH_DATA_YT_AddValue(ahData, 150*g_Wave1[i%128]*g_DAC1.ulAMP/4095/4095);
//			}
//			
//			hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_3);
//			sprintf(acBuffer, "Type:%s  AMP:%5.3fV  DAC:%d  Duty:%d%%  Freq:%dHz", 
//					 g_WaveType[g_DAC1.ucWaveType],
//					 (float)g_DAC1.ulAMP*3.3f/4095,
//					 g_DAC1.ulAMP,
//					 g_DAC1.ucDuty,
//					 g_DAC1.ulFreq);
//					 
//			TEXT_SetText(hItem, acBuffer);
//			break;
//	
//        case WM_NOTIFY_PARENT:
//            Id = WM_GetId(pMsg->hWinSrc); 
//            NCode = pMsg->Data.v;        
//            switch (Id) 
//            {
//				/* 关闭对话框 */
//				case ID_BUTTON_3:
//                    switch(NCode)
//                    {
//                        case WM_NOTIFICATION_RELEASED:
//							GUI_EndDialog(hWin, 0);
//                            break;
//                    }
//                    break;
//					
//				/* 切换到正弦波 */
//				case ID_BUTTON_0: 
//					switch(NCode) 
//					{
//						case WM_NOTIFICATION_RELEASED:
//							g_DAC1.ucWaveType = 0;
//							dac1_SetSinWave(g_DAC1.ulAMP, g_DAC1.ulFreq);
//							for(i = 0; i < 384; i++)
//							{
//								GRAPH_DATA_YT_AddValue(ahData, 150*g_Wave1[i%128]*g_DAC1.ulAMP/4095/4095);
//							}

//							hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_3);
//							sprintf(acBuffer, "Type:%s  AMP:%5.3fV  DAC:%d  Duty:%d%%  Freq:%dHz", 
//								    g_WaveType[g_DAC1.ucWaveType],
//								    (float)g_DAC1.ulAMP*3.3f/4095,
//								    g_DAC1.ulAMP,
//								    g_DAC1.ucDuty,
//								    g_DAC1.ulFreq);

//							TEXT_SetText(hItem, acBuffer);
//							break;
//					}
//					break;
//					 
//				/* 切换到方波 */					
//				case ID_BUTTON_1: 
//					switch(NCode) 
//					{
//						case WM_NOTIFICATION_RELEASED:
//						g_DAC1.ucWaveType = 1;
//						dac1_SetRectWave(0, g_DAC1.ulAMP, g_DAC1.ulFreq, g_DAC1.ucDuty);
//						for(i = 0; i < 384; i++)
//						{
//							GRAPH_DATA_YT_AddValue(ahData, 150*g_Wave1[i%128]*g_DAC1.ulAMP/4095/4095);
//						}

//						hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_3);
//						sprintf(acBuffer, "Type:%s  AMP:%5.3fV  DAC:%d  Duty:%d%%  Freq:%dHz", 
//							    g_WaveType[g_DAC1.ucWaveType],
//							    (float)g_DAC1.ulAMP*3.3f/4095,
//							    g_DAC1.ulAMP,
//							    g_DAC1.ucDuty,
//							    g_DAC1.ulFreq);

//						TEXT_SetText(hItem, acBuffer);
//						break;
//					}
//					break;
//					
//				/* 切换到三角波 */						
//				case ID_BUTTON_2:
//					switch(NCode) 
//					{
//						case WM_NOTIFICATION_RELEASED:
//							g_DAC1.ucWaveType = 2;
//							dac1_SetTriWave(0, g_DAC1.ulAMP, g_DAC1.ulFreq, g_DAC1.ucDuty);
//							for(i = 0; i < 384; i++)
//							{
//								GRAPH_DATA_YT_AddValue(ahData, 150*g_Wave1[i%128]*g_DAC1.ulAMP/4095/4095);
//							}

//							hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_3);
//							sprintf(acBuffer, "Type:%s  AMP:%5.3fV  DAC:%d  Duty:%d%%  Freq:%dHz", 
//								    g_WaveType[g_DAC1.ucWaveType],
//								    (float)g_DAC1.ulAMP*3.3f/4095,
//								    g_DAC1.ulAMP,
//								    g_DAC1.ucDuty,
//								    g_DAC1.ulFreq);

//							TEXT_SetText(hItem, acBuffer);
//							break;
//					}
//					break;

//				/* 更新三角波，方波和正弦波的幅值 */					  
//				case ID_SLIDER_0: 
//					switch(NCode) 
//					{
//						case WM_NOTIFICATION_RELEASED:
//							hItem = WM_GetDialogItem(pMsg->hWin, ID_SLIDER_0);
//							g_DAC1.ulAMP = SLIDER_GetValue(hItem);
//							if(g_DAC1.ucWaveType == 0)
//							{
//								dac1_SetSinWave(g_DAC1.ulAMP, g_DAC1.ulFreq);
//							}
//							else if(g_DAC1.ucWaveType == 1)
//							{
//								dac1_SetRectWave(0, g_DAC1.ulAMP, g_DAC1.ulFreq, g_DAC1.ucDuty);
//							}
//							else if(g_DAC1.ucWaveType == 2)
//							{
//								dac1_SetTriWave(0, g_DAC1.ulAMP, g_DAC1.ulFreq, g_DAC1.ucDuty);
//							}

//							for(i = 0; i < 384; i++)
//							{
//								GRAPH_DATA_YT_AddValue(ahData, 150*g_Wave1[i%128]*g_DAC1.ulAMP/4095/4095);
//							}

//							hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_3);
//							sprintf(acBuffer, "Type:%s  AMP:%5.3fV  DAC:%d  Duty:%d%%  Freq:%dHz", 
//								 g_WaveType[g_DAC1.ucWaveType],
//								 (float)g_DAC1.ulAMP*3.3f/4095,
//								 g_DAC1.ulAMP,
//								 g_DAC1.ucDuty,
//								 g_DAC1.ulFreq);
//									 
//							TEXT_SetText(hItem, acBuffer);
//							break;
//					}
//					break;
//				  
//				/* 更新方波和三角波的占空比 */
//				case ID_SLIDER_1: 
//					switch(NCode) 
//					{
//						case WM_NOTIFICATION_RELEASED:
//							hItem = WM_GetDialogItem(pMsg->hWin, ID_SLIDER_1);
//							g_DAC1.ucDuty  = SLIDER_GetValue(hItem);
//							if(g_DAC1.ucWaveType == 1)
//							{
//								dac1_SetRectWave(0, g_DAC1.ulAMP, g_DAC1.ulFreq, g_DAC1.ucDuty);
//							}
//							else if(g_DAC1.ucWaveType == 2)
//							{
//								dac1_SetTriWave(0, g_DAC1.ulAMP, g_DAC1.ulFreq, g_DAC1.ucDuty);
//							}

//							if(g_DAC1.ucWaveType != 0)
//							{
//								for(i = 0; i < 384; i++)
//								{
//									GRAPH_DATA_YT_AddValue(ahData, 150*g_Wave1[i%128]*g_DAC1.ulAMP/4095/4095);
//								}
//							}

//							hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_3);
//							sprintf(acBuffer, "Type:%s  AMP:%5.3fV  DAC:%d  Duty:%d%%  Freq:%dHz", 
//								 g_WaveType[g_DAC1.ucWaveType],
//								 (float)g_DAC1.ulAMP*3.3f/4095,
//								 g_DAC1.ulAMP,
//								 g_DAC1.ucDuty,
//								 g_DAC1.ulFreq);
//									 
//							TEXT_SetText(hItem, acBuffer);
//							break;
//					}
//					break;
//            }
//            break;
//        default:
//            WM_DefaultProc(pMsg);
//    }
//}

///*
//*********************************************************************************************************
//*	函 数 名: DSO_CreateDacDlg
//*	功能说明: 创建对话框
//*	形    参: 无        	
//*	返 回 值: 无
//*********************************************************************************************************
//*/
//WM_HWIN DSO_CreateDacDlg(void) 
//{
//	WM_HWIN hWin;

//	hWin = GUI_CreateDialogBox(_aDialogCreateDAC, GUI_COUNTOF(_aDialogCreateDAC), _cbCallbackDAC, WM_HBKWIN, 0, 0);

//	return hWin;
//}

///***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
