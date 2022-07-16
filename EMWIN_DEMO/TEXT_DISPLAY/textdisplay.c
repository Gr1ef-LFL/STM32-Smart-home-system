/*
 * @Description: 
 * @Version: 1.0
 * @Autor: LFL
 * @Date: 2022-03-09 17:50:15
 * @LastEditors: LFL
 * @LastEditTime: 2022-04-06 18:44:43
 */
#include "textdisplay.h"
#include "GUI.h"
#include "DLG.h"

void UI_DISPLAY()
{
	/* 创建欢迎界面 */
	CreatehuanyinFramewin();
	/* 延时发起任务调度 */
	while(1)
	{
		GUI_Delay(10);
	}
}



