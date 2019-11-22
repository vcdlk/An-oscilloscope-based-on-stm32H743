/*
*********************************************************************************************************
*	                                  
*	ģ������ : ����ʾ���������α�
*	�ļ����� : DSO_DrawCursorV.c
*	��    �� : V1.0
*	˵    �� : ����ʾ�����Ĵ�ֱ�����α꣬���ڲ���ʱ�䡣
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
*	�� �� ��: DSO_DrawCursorV
*	����˵��: ����ʾ�����Ĵ�ֱ�����α꣬���ڲ�����ʱ�䡣
*	��    ��: ��         	
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void DSO_DrawCursorV(void) 
{
	char buf[60];
	
	/* ��1�����������ڲ����������α���********************************************************/
	GUI_SetColor(0x0040f0);
	
	/* ��������A */
	GUI_DrawVLine(g_Cursors->sCursorVA, DSOSCREEN_STARTY, DSOSCREEN_ENDY);
	GUI_DrawVLine(g_Cursors->sCursorVA+1, DSOSCREEN_STARTY, DSOSCREEN_ENDY);  
	GUI_FillRoundedRect(g_Cursors->sCursorVA+5, 47, g_Cursors->sCursorVA+16, 62, 3);

	/* ��������B */
	GUI_DrawVLine(g_Cursors->sCursorVB, DSOSCREEN_STARTY, DSOSCREEN_ENDY);
	GUI_DrawVLine(g_Cursors->sCursorVB-1, DSOSCREEN_STARTY, DSOSCREEN_ENDY); 
	GUI_FillRoundedRect(g_Cursors->sCursorVB-16, 47, g_Cursors->sCursorVB-5, 62, 3);
	
	/* ������A������B�Աߵ�СԲȦ����ʾ��ĸa����ĸb */
	GUI_SetColor(GUI_BLACK);
	GUI_SetFont(&GUI_Font20_ASCII);
	GUI_SetTextMode(GUI_TEXTMODE_TRANS);					
	GUI_DispCharAt('a', g_Cursors->sCursorVA+6, 44);
	GUI_DispCharAt('b', g_Cursors->sCursorVB-15, 45);

	/* ��2���������α�������ʾ����********************************************************/
	GUI_SetColor(GUI_BLACK);
	GUI_FillRoundedRect(500*1.25, 55*1.28, 635*1.25, 135*1.28, 4);
	GUI_SetColor(GUI_WHITE);
	GUI_DrawRoundedRect( 499*1.25, 54*1.28, 636*1.25, 136*1.28, 4);
					 
	GUI_SetColor(0x0040f0);
	GUI_FillRoundedRect((75 + 433)*1.25 , (47 + 15)*1.28, (86 + 433)*1.25, (62 + 15)*1.28, 3);				 
	GUI_FillRoundedRect((75 + 433)*1.25 , (45 + 45)*1.28, (86 + 433)*1.25, (60 + 45)*1.28, 3);
					 
	GUI_SetColor(GUI_BLACK);
	GUI_SetFont(&GUI_Font20_1);
	GUI_SetTextMode(GUI_TEXTMODE_TRANS);					
	GUI_DispCharAt('a', 509*1.25, 59*1.28);
	GUI_DispCharAt('b', 509*1.25, 88*1.28);

	GUI_SetColor(GUI_YELLOW);
	GUI_SetFont(&GUI_Font16_1);
	GUI_DispCharAt('#', 513*1.25, 115*1.28);

	/* ��3������������A����ֵ***********************************************************/
	g_Cursors->WaveCursorA = (float)((g_Cursors->sCursorVA - 340) * g_CursorUintTable[TimeBaseId][0])/1000;
	if(g_CursorUintTable[TimeBaseId][1] == 1)
	{
		sprintf(buf, "%5.1fus", g_Cursors->WaveCursorA);
	}
	else if(g_CursorUintTable[TimeBaseId][1] == 2)
	{
		sprintf(buf, "%5.3fms", g_Cursors->WaveCursorA);
	}
	else if(g_CursorUintTable[TimeBaseId][1] == 3)
	{
		sprintf(buf, "%5.2fms", g_Cursors->WaveCursorA);
	}
	else if(g_CursorUintTable[TimeBaseId][1] == 4)
	{
		sprintf(buf, "%5.1fms", g_Cursors->WaveCursorA);
	}
	else if(g_CursorUintTable[TimeBaseId][1] == 5)
	{
		sprintf(buf, "%5.3fs", g_Cursors->WaveCursorA);
	}
	else if(g_CursorUintTable[TimeBaseId][1] == 6)
	{
		sprintf(buf, "%5.2fs",g_Cursors->WaveCursorA);
	}
	else if(g_CursorUintTable[TimeBaseId][1] == 7)
	{
		sprintf(buf, "%5.1fs", g_Cursors->WaveCursorA);
	}

	/* ��ʾ����A��ֵ */
	GUI_DispStringAt(buf, 523*1.25, 63*1.28);

	/* ��4������������B����ֵ***********************************************************/
	g_Cursors->WaveCursorB = (float)((g_Cursors->sCursorVB - 340) * g_CursorUintTable[TimeBaseId][0])/1000;
	if(g_CursorUintTable[TimeBaseId][1] == 1)
	{
		sprintf(buf, "%5.1fus", g_Cursors->WaveCursorB);
	}
	else if(g_CursorUintTable[TimeBaseId][1] == 2)
	{
		sprintf(buf, "%5.3fms", g_Cursors->WaveCursorB);
	}
	else if(g_CursorUintTable[TimeBaseId][1] == 3)
	{
		sprintf(buf, "%5.2fms", g_Cursors->WaveCursorB);
	}
	else if(g_CursorUintTable[TimeBaseId][1] == 4)
	{
		sprintf(buf, "%5.1fms", g_Cursors->WaveCursorB);
	}
	else if(g_CursorUintTable[TimeBaseId][1] == 5)
	{
		sprintf(buf, "%5.3fs", g_Cursors->WaveCursorB);
	}
	else if(g_CursorUintTable[TimeBaseId][1] == 6)
	{
		sprintf(buf, "%5.2fs", g_Cursors->WaveCursorB);
	}
	else if(g_CursorUintTable[TimeBaseId][1] == 7)
	{
		sprintf(buf, "%5.1fs", g_Cursors->WaveCursorB);
	}
	
	/* ��ʾ����B��ֵ */
	GUI_DispStringAt(buf, 523*1.25, 91*1.28);

	/* ��5������������A����B�Ĳ�ֵ***********************************************************/
	if(g_CursorUintTable[TimeBaseId][1] == 1)
	{
		sprintf(buf, "%5.1fus", (g_Cursors->WaveCursorB - g_Cursors->WaveCursorA));
	}
	else if(g_CursorUintTable[TimeBaseId][1] == 2)
	{
		sprintf(buf, "%5.3fms", (g_Cursors->WaveCursorB - g_Cursors->WaveCursorA));
	}
	else if(g_CursorUintTable[TimeBaseId][1] == 3)
	{
		sprintf(buf, "%5.2fms", (g_Cursors->WaveCursorB - g_Cursors->WaveCursorA));
	}
	else if(g_CursorUintTable[TimeBaseId][1] == 4)
	{
		sprintf(buf, "%5.1fms", (g_Cursors->WaveCursorB - g_Cursors->WaveCursorA));
	}
	else if(g_CursorUintTable[TimeBaseId][1] == 5)
	{
		sprintf(buf, "%5.3fs", (g_Cursors->WaveCursorB - g_Cursors->WaveCursorA));
	}
	else if(g_CursorUintTable[TimeBaseId][1] == 6)
	{
		sprintf(buf, "%5.2fs", (g_Cursors->WaveCursorB - g_Cursors->WaveCursorA));
	}
	else if(g_CursorUintTable[TimeBaseId][1] == 7)
	{
		sprintf(buf, "%5.1fs",(g_Cursors->WaveCursorB - g_Cursors->WaveCursorA));
	}
	
	/* ��ʾ����A������B�Ĳ�ֵ */
	GUI_DispStringAt(buf, 523*1.25, 115*1.28);

	/* ��6������ʾˮƽ�ߵļ�����ֵ***********************************************************/
	GUI_DispCharAt('#', (509+72)*1.25, 115*1.28);

	g_Cursors->WaveCursorA = (float)((240 - g_Cursors->sCursorHA) * g_AttTable[Ch1AmpId][1]) / 50000;
	sprintf(buf, "%5.3fV", g_Cursors->WaveCursorA);
	GUI_DispStringAt(buf, (518+72)*1.25, 63*1.28);

	g_Cursors->WaveCursorB = (float)((240 - g_Cursors->sCursorHB) * g_AttTable[Ch1AmpId][1]) / 50000;
	sprintf(buf, "%5.3fV", g_Cursors->WaveCursorB);	
	GUI_DispStringAt(buf, (518+70)*1.25, 91*1.28);

	sprintf(buf, "%5.3fV", g_Cursors->WaveCursorA - g_Cursors->WaveCursorB);
	GUI_DispStringAt(buf, (520+70)*1.25, 115*1.28);
}

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
