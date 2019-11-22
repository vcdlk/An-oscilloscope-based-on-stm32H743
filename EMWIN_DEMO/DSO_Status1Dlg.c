/*
*********************************************************************************************************
*	                                  
*	ģ������ : ʾ�����������ϵ�״̬����
*	�ļ����� : DSO_Status1Dlg.c
*	��    �� : V1.0
*	˵    �� : ʾ�����������ϵ�״̬���ڣ�������ʾ���ֵ�����ֵ��ƽ��ֵ����Сֵ��4��
*	�޸ļ�¼ :
*		�汾��    ����          ����           ˵��
*		V1.0    2018-01-06     Eric2013        �׷� 
*                                           
*	Copyright (C), 2018-2028, ���������� www.armfly.com
*
*********************************************************************************************************
*/
#include "includes.h"
#include "MainTask.h"



/*
*********************************************************************************************************
*	                                  ����
*********************************************************************************************************
*/
static GUI_MEMDEV_Handle   hMemStatus1Dlg;


/*
*********************************************************************************************************
*				                      �궨��
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
*					GUI_WIDGET_CREATE_INFO��������
*********************************************************************************************************
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreateStatus1[] = 
{
    { WINDOW_CreateIndirect,     "",     0,          145*1.28,  444*1.25, 510*1.28, 35*1.25, 0},
	
	/* ��һ�� */
	{ TEXT_CreateIndirect,    "Mean",    ID_TEXT_0,   35*1.28,  2*1.25, 40*1.28, 16*1.25, 0, 0},
	{ TEXT_CreateIndirect,    "0.000V",  ID_TEXT_1,   80*1.28,  2*1.25, 50*1.28, 16*1.25, 0, 0},
	
	{ TEXT_CreateIndirect,    "Pk-Pk",   ID_TEXT_2,   160*1.28, 2*1.25, 40*1.28, 16*1.25, 0, 0},
	{ TEXT_CreateIndirect,    "0.000V",  ID_TEXT_3,   205*1.28, 2*1.25, 50*1.28, 16*1.25, 0, 0},
	
	{ TEXT_CreateIndirect,    "Min",     ID_TEXT_4,   285*1.28, 2*1.25, 40*1.28, 16*1.25, 0, 0},
	{ TEXT_CreateIndirect,    "0.000V",  ID_TEXT_5,   330*1.28, 2*1.25, 50*1.28, 16*1.25, 0, 0},	

	{ TEXT_CreateIndirect,    "Max",     ID_TEXT_6,   410*1.28, 2*1.25, 40*1.28, 16*1.25, 0, 0},
	{ TEXT_CreateIndirect,    "0.000V",  ID_TEXT_7,   455*1.28, 2*1.25, 50*1.28, 16*1.25, 0, 0},		
	
	/* �ڶ��� */		
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
*	�� �� ��: PaintDialogStatus1
*	����˵��: ״̬���ڵĻص������ػ���Ϣ
*	��    �Σ�pMsg  ָ���ַ         	
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void PaintDialogStatus1(WM_MESSAGE * pMsg)
{
	/* �屳��ɫ */
	GUI_SetBkColor(0x905040);
  	GUI_Clear();
	
	/* �������Ŀ����Բ�Ǿ��� */
	GUI_SetColor(GUI_BLACK);
	GUI_AA_FillRoundedRect(0, 0, 509*1.28, 34*1.25, 10);
	
	/* ���ƿ����Բ�Ǿ��� */
	GUI_SetColor(0XEBCD9E);
	GUI_SetPenSize(2);
	GUI_AA_DrawRoundedRect(0, 0, 509*1.28, 34*1.25, 10);
	
	/* ���ƿ����Բ�Ǿ��Σ�������ֵ1 */
	GUI_SetColor(GUI_YELLOW);
	GUI_AA_FillRoundedRect(10*1.28, 4*1.25, 30*1.28, 16*1.25, 3);
	
	GUI_SetColor(GUI_BLACK);
    GUI_SetFont(&GUI_Font16_1);
	GUI_SetTextMode(GUI_TEXTMODE_TRANS);					
	GUI_DispCharAt('1', 16*1.28, 3*1.25);

	/* ���ƿ����Բ�Ǿ��Σ�������ֵ1 */
	GUI_SetColor(GUI_YELLOW);
	GUI_AA_FillRoundedRect(135*1.28, 4*1.25, 155*1.28, 16*1.25, 3);
						   
	GUI_SetColor(GUI_BLACK);
    GUI_SetFont(&GUI_Font16_1);
	GUI_SetTextMode(GUI_TEXTMODE_TRANS);					
	GUI_DispCharAt('1', 141*1.28, 3*1.25);
	
	/* ���ƿ����Բ�Ǿ��Σ�������ֵ1 */
	GUI_SetColor(GUI_YELLOW);
	GUI_AA_FillRoundedRect(260*1.28, 4*1.25, 280*1.28, 16*1.25, 3);
						   
	GUI_SetColor(GUI_BLACK);
    GUI_SetFont(&GUI_Font16_1);
	GUI_SetTextMode(GUI_TEXTMODE_TRANS);					
	GUI_DispCharAt('1', 266*1.28, 2*1.25);
	
	/* ���ƿ����Բ�Ǿ��Σ�������ֵ1 */	
	GUI_SetColor(GUI_YELLOW);
	GUI_AA_FillRoundedRect(385*1.28, 4*1.25, 405*1.28, 16*1.25, 3);
						   
	GUI_SetColor(GUI_BLACK);
    GUI_SetFont(&GUI_Font16_1);
	GUI_SetTextMode(GUI_TEXTMODE_TRANS);					
	GUI_DispCharAt('1', 391*1.28, 2*1.25);
	
	/* ���ƿ����Բ�Ǿ��Σ�������ֵ2 */
	GUI_SetColor(GUI_GREEN);
	GUI_AA_FillRoundedRect(10*1.28, 19*1.25, 30*1.28, 31*1.25, 3);
						   
	GUI_SetColor(GUI_BLACK);
    GUI_SetFont(&GUI_Font16_1);
	GUI_SetTextMode(GUI_TEXTMODE_TRANS);					
	GUI_DispCharAt('2', 16*1.28, 18*1.25);
	
	/* ���ƿ����Բ�Ǿ��Σ�������ֵ2 */
	GUI_SetColor(GUI_GREEN);
	GUI_AA_FillRoundedRect(135*1.28, 19*1.25, 155*1.28, 31*1.25, 3);
						   
	GUI_SetColor(GUI_BLACK);
    GUI_SetFont(&GUI_Font16_1);
	GUI_SetTextMode(GUI_TEXTMODE_TRANS);					
	GUI_DispCharAt('2', 141*1.28, 18*1.25);
	
	/* ���ƿ����Բ�Ǿ��Σ�������ֵ2 */
	GUI_SetColor(GUI_GREEN);
	GUI_AA_FillRoundedRect(260*1.28, 19*1.25, 280*1.28, 31*1.25, 3);
						   
	GUI_SetColor(GUI_BLACK);
    GUI_SetFont(&GUI_Font16_1);
	GUI_SetTextMode(GUI_TEXTMODE_TRANS);					
	GUI_DispCharAt('2', 266*1.28, 18*1.25);

	/* ���ƿ����Բ�Ǿ��Σ�������ֵ2 */
	GUI_SetColor(GUI_GREEN);
	GUI_AA_FillRoundedRect(385*1.28, 19*1.25, 405*1.28, 31*1.25, 3);
						   
	GUI_SetColor(GUI_BLACK);
    GUI_SetFont(&GUI_Font16_1);
	GUI_SetTextMode(GUI_TEXTMODE_TRANS);					
	GUI_DispCharAt('2', 391*1.28, 18*1.25);
}

/*
*********************************************************************************************************
*	�� �� ��: InitDialogStatus1()
*	����˵��: ״̬���ڵĻص�������ʼ����Ϣ
*	��    �Σ�pMsg  ָ���ַ         	
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void InitDialogStatus1(WM_MESSAGE * pMsg)
{
	int i;
    WM_HWIN hWin = pMsg->hWin;

	//
    // ��ʼ���ı��ؼ�
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
	
	/* ������ʱ�� */
	WM_CreateTimer(hWin,   /* ������Ϣ�Ĵ��ڵľ�� */
				   0, 	   /* �û������Id���������ͬһ����ʹ�ö����ʱ������ֵ��������Ϊ�㡣 */
				   1000,   /* ���ڣ������ڹ���ָ������Ӧ�յ���Ϣ*/
				   0);	   /* ��������ʹ�ã�ӦΪ0 */	
}

/*
*********************************************************************************************************
*	�� �� ��: _cbCallbackStatus1
*	����˵��: ״̬���ڵĻص�����
*	��    ��: pMsg  ָ���ַ         	
*	�� �� ֵ: ��
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
			/* ƽ��ֵ */
			if(g_DSO1->ucMeasureFlag[24] == 1)
			{
				sprintf(buf, "%5.3fV", g_DSO1->WaveMean);
				TEXT_SetText(WM_GetDialogItem(hWin, ID_TEXT_0),  g_MeasureTable[20]);
				TEXT_SetText(WM_GetDialogItem(hWin, ID_TEXT_1),  buf);
				
				sprintf(buf, "%5.3fV", g_DSO2->WaveMean);
				TEXT_SetText(WM_GetDialogItem(hWin, ID_TEXT_8),  g_MeasureTable[20]);
				TEXT_SetText(WM_GetDialogItem(hWin, ID_TEXT_9),  buf);
			}
			
			/* ���ֵ */
			if(g_DSO1->ucMeasureFlag[12] == 1)
			{
				sprintf(buf, "%5.3fV", g_DSO1->WavePkPk);
				TEXT_SetText(WM_GetDialogItem(hWin, ID_TEXT_2),  g_MeasureTable[12]);
				TEXT_SetText(WM_GetDialogItem(hWin, ID_TEXT_3),  buf);
				
				sprintf(buf, "%5.3fV", g_DSO2->WavePkPk);
				TEXT_SetText(WM_GetDialogItem(hWin, ID_TEXT_10),  g_MeasureTable[12]);
				TEXT_SetText(WM_GetDialogItem(hWin, ID_TEXT_11),  buf);				
			}

			/* ��Сֵ */
			if(g_DSO1->ucMeasureFlag[15] == 1)
			{
				sprintf(buf, "%5.3fV", g_DSO1->WaveMin);
				TEXT_SetText(WM_GetDialogItem(hWin, ID_TEXT_4),  g_MeasureTable[15]);
				TEXT_SetText(WM_GetDialogItem(hWin, ID_TEXT_5),  buf);
				
				sprintf(buf, "%5.3fV", g_DSO2->WaveMin);
				TEXT_SetText(WM_GetDialogItem(hWin, ID_TEXT_12),  g_MeasureTable[15]);
				TEXT_SetText(WM_GetDialogItem(hWin, ID_TEXT_13),  buf);
			}

			/* ���ֵ */
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
*	�� �� ��: CreateWindowStatus1
*	����˵��: ״̬���ڵĻص�����
*	��    �Σ�pMsg  ָ���ַ         	
*	�� �� ֵ: ��
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
*	�� �� ��: DrawWinStatus1Bk
*	����˵��: �����ڱ������Ƶ��洢�豸����
*	��    ��: ��         	
*	�� �� ֵ: ��
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

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
