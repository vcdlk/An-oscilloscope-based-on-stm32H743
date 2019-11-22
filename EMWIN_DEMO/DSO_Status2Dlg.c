/*
*********************************************************************************************************
*	                                  
*	ģ������ : ʾ�����������ϵ�״̬����
*	�ļ����� : DSO_Status2Dlg.c
*	��    �� : V1.0
*	˵    �� : ʾ�����������ϵ�״̬���ڣ�������ʾƵ�ʺ�RMS��
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
static GUI_MEMDEV_Handle   hMemStatus2Dlg;


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


/*
*********************************************************************************************************
*					        GUI_WIDGET_CREATE_INFO��������
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
*	�� �� ��: PaintDialogStatus2()
*	����˵��: ʱ�����ڵĻص������ػ�
*	��    ��: pMsg  ָ���ַ         	
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void PaintDialogStatus2(WM_MESSAGE * pMsg)
{
	/* �屳��ɫ */
	GUI_SetBkColor(0x905040);
	GUI_Clear();

	/* �������Ŀ����Բ�Ǿ��� */	
	GUI_SetColor(GUI_BLACK);
	GUI_AA_FillRoundedRect(0, 0, 134*1.28, 65*1.25, 10);

	/* ���ƿ����Բ�Ǿ��� */	
	GUI_SetColor(0XEBCD9E);
	GUI_SetPenSize(2);
	GUI_AA_DrawRoundedRect(0, 0, 134*1.28, 65*1.25, 10);
	
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

	/* ���ƿ����Բ�Ǿ��Σ�������ֵ1 */
	GUI_SetColor(GUI_YELLOW);
	GUI_AA_FillRoundedRect(10*1.28, 34*1.25, 30*1.28, 47*1.25, 3);
	
	GUI_SetColor(GUI_BLACK);
    GUI_SetFont(&GUI_Font16_1);
	GUI_SetTextMode(GUI_TEXTMODE_TRANS);					
	GUI_DispCharAt('1', 16*1.28, 32*1.25);

	/* ���ƿ����Բ�Ǿ��Σ�������ֵ2 */
	GUI_SetColor(GUI_GREEN);
	GUI_AA_FillRoundedRect(10*1.28, 50*1.25, 30*1.28, 62*1.25, 3);
						   
	GUI_SetColor(GUI_BLACK);
    GUI_SetFont(&GUI_Font16_1);
	GUI_SetTextMode(GUI_TEXTMODE_TRANS);					
	GUI_DispCharAt('2', 16*1.28, 49*1.25);
}

/*
*********************************************************************************************************
*	�� �� ��: InitDialogStatus2
*	����˵��: ʱ�����ڵĻص������ĳ�ʼ��
*	��    ��: pMsg  ָ���ַ         	
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void InitDialogStatus2(WM_MESSAGE * pMsg)
{
	int i;
    WM_HWIN hWin = pMsg->hWin;

	//
    // ��ʼ���ı��ؼ�
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
	
	/* ������ʱ�� */
	WM_CreateTimer(hWin,   /* ������Ϣ�Ĵ��ڵľ�� */
				   0, 	   /* �û������Id���������ͬһ����ʹ�ö����ʱ������ֵ��������Ϊ�㡣 */
				   1000,   /* ���ڣ������ڹ���ָ������Ӧ�յ���Ϣ*/
				   0);	   /* ��������ʹ�ã�ӦΪ0 */	
}

/*
*********************************************************************************************************
*	�� �� ��: _cbCallbackStatus2()
*	����˵��: ʱ�����ڵĻص�����
*	��    �Σ�pMsg  ָ���ַ         	
*	�� �� ֵ: ��
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
			/* ������ */
			if(g_DSO1->ucMeasureFlag[22] == 1)
			{
				sprintf(buf, "%5.3fV", g_DSO1->WaveRMS);
				TEXT_SetText(WM_GetDialogItem(hWin, ID_TEXT_1), buf);
			
				sprintf(buf, "%5.3fV", g_DSO2->WaveRMS);
				TEXT_SetText(WM_GetDialogItem(hWin, ID_TEXT_5), buf);
			}
		
			/* ����Ƶ�ʹ��� */
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
*	�� �� ��: CreateStatus2Dlg
*	����˵��: ʱ�����ڵĻص�����
*	��    ��: pMsg  ָ���ַ         	
*	�� �� ֵ: ��
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
*	�� �� ��: DrawWinStatus2Bk
*	����˵��: �����ڱ������Ƶ��洢�豸����
*	��    ��: pMsg  ָ���ַ         	
*	�� �� ֵ: ��
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

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
