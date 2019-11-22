/*
*********************************************************************************************************
*	                                  
*	ģ������ : ʾ�����������ϵ�ʱ������
*	�ļ����� : DSO_ScaleDlg.c
*	��    �� : V1.0
*	˵    �� : ��ʾʾ������ǰ��ʱ��
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
static GUI_MEMDEV_Handle   hMemScaleDlg;


/*
*********************************************************************************************************
*				                      �궨��
*********************************************************************************************************
*/
#define ID_TEXT_0 		(GUI_ID_USER + 0x00)
#define ID_TEXT_1  		(GUI_ID_USER + 0x01)
#define ID_TEXT_2 		(GUI_ID_USER + 0x02)
#define ID_TEXT_3 		(GUI_ID_USER + 0x03)


/*
*********************************************************************************************************
*					GUI_WIDGET_CREATE_INFO��������
*********************************************************************************************************
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreateScale[] = 
{ 
    { WINDOW_CreateIndirect,   "",       0,      663*1.28,  444*1.25,  135*1.28,  35*1.25, 0},
	
	{ TEXT_CreateIndirect,     "",             ID_TEXT_0, 35*1.28,  2*1.25,  0,  16*1.25, 0, 0},
	{ TEXT_CreateIndirect,     "2Msps 500ns",  ID_TEXT_1, 35*1.28,  2*1.25, 90*1.28,  16*1.25, 0, 0},
	
	{ TEXT_CreateIndirect,     "",             ID_TEXT_2, 35*1.28, 17*1.25,  0,  16*1.25, 0, 0},
	{ TEXT_CreateIndirect,     "2Msps 500ns",  ID_TEXT_3, 35*1.28, 17*1.25,  90*1.28, 16*1.25, 0, 0},
};

/*
*********************************************************************************************************
*	�� �� ��: PaintDialogScale()
*	����˵��: ʱ�����ڵĻص������ػ�
*	��    �Σ�pMsg  ָ���ַ         	
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void PaintDialogScale(WM_MESSAGE * pMsg)
{
	/* �屳��ɫ */	
	GUI_SetBkColor(0x905040);
	GUI_Clear();

	/* �������Ŀ����Բ�Ǿ��� */		
	GUI_SetColor(GUI_BLACK);
	GUI_AA_FillRoundedRect(0, 0, 134*1.28, 34*1.25, 10);

	/* ���ƿ����Բ�Ǿ��� */	
	GUI_SetColor(0XEBCD9E);
	GUI_SetPenSize(2);
	GUI_AA_DrawRoundedRect(0, 0, 134*1.28, 34*1.25, 10);

	/* ���ƿ����Բ�Ǿ��Σ�������ֵ1 */	
	GUI_SetColor(GUI_YELLOW);
	GUI_AA_FillRoundedRect(10*1.28, 4*1.25, 30*1.28, 16*1.25, 3);
	
	GUI_SetColor(GUI_BLACK);
    GUI_SetFont(&GUI_Font16_1);
	GUI_SetTextMode(GUI_TEXTMODE_TRANS);					
	GUI_DispCharAt('1', 16*1.28, 3*1.25);

	/* ���ƿ����Բ�Ǿ��Σ�������ֵ2 */
	GUI_SetColor(GUI_GREEN);
	GUI_AA_FillRoundedRect(10*1.28, 19*1.25, 30*1.28, 31*1.25, 3);
						   
	GUI_SetColor(GUI_BLACK);
    GUI_SetFont(&GUI_Font16_1);
	GUI_SetTextMode(GUI_TEXTMODE_TRANS);					
	GUI_DispCharAt('2', 16*1.28, 18*1.25);
}

/*
*********************************************************************************************************
*	�� �� ��: InitDialogScale
*	����˵��: ʱ�����ڵĻص������ĳ�ʼ��
*	��    ��: pMsg  ָ���ַ         	
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void InitDialogScale(WM_MESSAGE * pMsg)
{
    WM_HWIN hWin = pMsg->hWin;

    //
    // ��ʼ���ı��ؼ�
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
*	�� �� ��: _cbCallbackScale()
*	����˵��: ʱ�����ڵĻص�����
*	��    ��: pMsg  ָ���ַ         	
*	�� �� ֵ: ��
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
			GUI_MEMDEV_WriteAt(hMemScaleDlg, 663*1.28, 444*1.25);
			break;

		default:
			WM_DefaultProc(pMsg);
    }
}

/*
*********************************************************************************************************
*	�� �� ��: CreateWindowScale
*	����˵��: ʱ�����ڵĻص�����
*	��    �Σ�pMsg  ָ���ַ         	
*	�� �� ֵ: ��
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
*	�� �� ��: DrawWinScaleBk
*	����˵��: �����ڱ������Ƶ��洢�豸����
*	��    ��: pMsg  ָ���ַ         	
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void DrawWinScaleBk(void) 
{
	hMemScaleDlg = GUI_MEMDEV_CreateFixed(0, 
										  0, 
									      135*1.28, 
									      35*1.25, 
									      GUI_MEMDEV_HASTRANS, 
										  GUI_MEMDEV_APILIST_16, 
										  GUICC_M565);
	GUI_MEMDEV_Select(hMemScaleDlg);
	PaintDialogScale(NULL);
	GUI_MEMDEV_Select(0);
}

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
