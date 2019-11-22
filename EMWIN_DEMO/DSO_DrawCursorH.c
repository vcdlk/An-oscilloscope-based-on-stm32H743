/*
*********************************************************************************************************
*	                                  
*	模块名称 : 绘制示波器测量游标
*	文件名称 : DSO_DrawCursorH.c
*	版    本 : V1.0
*	说    明 : 绘制示波器的水平测量游标，用于测量幅值。
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
*	函 数 名: DSO_DrawCursorH
*	功能说明: 绘制示波器的水平测量游标，用于测量幅值。
*	形    参: 无         	
*	返 回 值: 无
*********************************************************************************************************
*/
void DSO_DrawCursorH(void) 
{
	char buf[60];
	
	/* 第1步：绘制用于测量的两个游标线********************************************************/
	GUI_SetColor(0x0040f0);
	
	/* 绘制横线A */
	GUI_DrawHLine(g_Cursors->sCursorHA, DSOSCREEN_STARTX, DSOSCREEN_ENDX);
	GUI_DrawHLine(g_Cursors->sCursorHA+1, DSOSCREEN_STARTX, DSOSCREEN_ENDX);  
	GUI_FillRoundedRect(50, g_Cursors->sCursorHA+3, 61, g_Cursors->sCursorHA+18, 3);

	/* 绘制横线B */
	GUI_DrawHLine(g_Cursors->sCursorHB, DSOSCREEN_STARTX, DSOSCREEN_ENDX);
	GUI_DrawHLine(g_Cursors->sCursorHB-1, DSOSCREEN_STARTX, DSOSCREEN_ENDX);  
	GUI_FillRoundedRect(50, g_Cursors->sCursorHB-18, 61, g_Cursors->sCursorHB-3, 3);
	
	/* 在横线A和横线B旁边的小圆圈中显示字母a和字母b */
	GUI_SetColor(GUI_BLACK);
	GUI_SetFont(&GUI_Font20_ASCII);
	GUI_SetTextMode(GUI_TEXTMODE_TRANS);					
	GUI_DispCharAt('a', 51*1.25, g_Cursors->sCursorHA);
	GUI_DispCharAt('b', 51*1.25, g_Cursors->sCursorHB-19);
	
	/* 第2步：绘制游标数据显示窗口********************************************************/
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
	GUI_DispCharAt('#', 511*1.25, 115*1.28);

	/* 第3步：计算竖线线A的数值***********************************************************/
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
		sprintf(buf, "%5.2fs", g_Cursors->WaveCursorA);
	}
	else if(g_CursorUintTable[TimeBaseId][1] == 7)
	{
		sprintf(buf, "%5.1fs", g_Cursors->WaveCursorA);
	}

	GUI_DispStringAt(buf, 523*1.25, 63*1.28);
	
	/* 第3步：计算竖线线B的数值***********************************************************/
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
	GUI_DispStringAt(buf, 523*1.25, 91*1.28);
	
	
	/* 第5步：计算竖线A竖线B的差值***********************************************************/
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
	GUI_DispStringAt(buf, 523*1.25, 115*1.28);
	
	/* 第6步：显示水平线的计算数值***********************************************************/
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

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
