#include "radiodemo.h"
#include "DIALOG.h"
#include "usart.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//STemwin RADIO�ؼ�ʹ��   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2016/3/28
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

//�û��Զ�����Ϣ
#define CHANGE_COLOR	(WM_USER +0)

//�ؼ� ID
#define ID_FRAMEWIN_0   (GUI_ID_USER + 0x00)
#define ID_RADIO_0      (GUI_ID_USER + 0x01)
#define ID_BUTTON_0     (GUI_ID_USER + 0x02)

int colortbl[]={GUI_RED,GUI_BLUE,GUI_WHITE,GUI_YELLOW,GUI_GREEN};   //������ɫ��

radio_struct radio_widg;

//�Ի�����Դ��
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = 
{
    { FRAMEWIN_CreateIndirect, "Framewin", ID_FRAMEWIN_0, 100, 100, 320, 240, FRAMEWIN_CF_MOVEABLE, 0x64, 0 },
    { RADIO_CreateIndirect, "Radio", ID_RADIO_0,    100, 10, 150, 150, 0, 0x1e05, 0 },
    { BUTTON_CreateIndirect, "Button", ID_BUTTON_0, 100, 160, 80, 40, 0, 0x0, 0 },
};

//�������ڵĻص�����
static void _BkCallback(WM_MESSAGE *pMsg)
{
	static WM_HWIN hWin;
	char text[30];
	switch(pMsg->MsgId)
	{
		case WM_PAINT:
			GUI_SetFont(&GUI_Font24_ASCII);
            GUI_SetBkColor(colortbl[radio_widg.value]);//���ñ�����ɫ
            GUI_Clear();
        
            GUI_SetColor(GUI_BLACK);                    //����ǰ��ɫ
        	GUI_DispStringAt("WIDGET_RADIO - Sample",0,0);
            GUI_SetFont(&GUI_Font32_ASCII);
            sprintf(text,"RADIO VALUE:%d",radio_widg.value);     
            GUI_DispStringAt(text,0,32);
			break;	
		case CHANGE_COLOR:
			hWin=pMsg->hWinSrc;
			WM_HideWindow(hWin);	        //���ضԻ���
			WM_InvalidateWindow(WM_HBKWIN); //����������Ч
			WM_CreateTimer(WM_HBKWIN,0,3000,0);
			break;
		case WM_TIMER:
			WM_ShowWindow(hWin);
			break;
		default:
			WM_DefaultProc(pMsg);
			break;
	}	
}

//�Ի���ص�����
static void _cbDialog(WM_MESSAGE * pMsg) 
{
    WM_HWIN hItem;
    int     NCode;
    int     Id;
    WM_MESSAGE Msg;
    
    switch (pMsg->MsgId) 
    {
        case WM_INIT_DIALOG:
            //��ʼ��FRAME
            hItem = pMsg->hWin;
            FRAMEWIN_SetText(hItem, "ALIENTEK RADIO");
            FRAMEWIN_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
            FRAMEWIN_SetFont(hItem, GUI_FONT_24_ASCII);
            FRAMEWIN_SetTextColor(hItem, 0x0000FFFF);

            //��ʼ��RADIO
            hItem = WM_GetDialogItem(pMsg->hWin, ID_RADIO_0);
            RADIO_SetTextColor(hItem, 0x00000000);
            RADIO_SetText(hItem, "RED",     0);
            RADIO_SetText(hItem, "BLUE",    1);
            RADIO_SetText(hItem, "WHITE",   2);
            RADIO_SetText(hItem, "YELLOW",  3);
            RADIO_SetText(hItem, "GREEN",   4);
            RADIO_SetFont(hItem, GUI_FONT_20_ASCII);
            
            //��ʼ��BUTTON0
            hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
            BUTTON_SetText(hItem, "OK");
            BUTTON_SetFont(hItem, GUI_FONT_24_ASCII);
            break;
        case WM_NOTIFY_PARENT:
            Id    = WM_GetId(pMsg->hWinSrc);
            NCode = pMsg->Data.v;
            switch(Id) 
            {
                case ID_RADIO_0: //RADIO0֪ͨ����
                    switch(NCode) 
                    {
                        case WM_NOTIFICATION_CLICKED:
                            break;
                        case WM_NOTIFICATION_RELEASED:
                            break;
                        case WM_NOTIFICATION_VALUE_CHANGED:
                            hItem = WM_GetDialogItem(pMsg->hWin, ID_RADIO_0);   //��ȡRADIO�ľ��
                            radio_widg.value=RADIO_GetValue(hItem);             //��ȡRADIOֵ
                            break;
                    }
                    break;
                case ID_BUTTON_0: //BUTTON0֪ͨ����
                    switch(NCode) 
                    {
                        case WM_NOTIFICATION_CLICKED:
												Msg.MsgId=CHANGE_COLOR;
												Msg.hWinSrc=pMsg->hWin;
												WM_SendMessage(WM_HBKWIN,&Msg);
                            break;
                        case WM_NOTIFICATION_RELEASED:
                            Msg.MsgId=CHANGE_COLOR;
														Msg.hWinSrc=pMsg->hWin;
														WM_SendMessage(WM_HBKWIN,&Msg);
							break;
                    }
                    break;
            }
            break;
    default:
        WM_DefaultProc(pMsg);
        break;
    }
}

//����RADIO�ؼ���ʾ����
void CreateRadioDemo(void) 
{
    WM_SetCallback(WM_HBKWIN,_BkCallback);
    
    radio_widg.value=0;         //Ĭ��Ϊ0
    radio_widg.rdadio = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
}

