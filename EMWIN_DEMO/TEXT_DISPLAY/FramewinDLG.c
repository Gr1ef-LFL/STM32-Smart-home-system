/*********************************************************************
*                                                                    *
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
*                                                                    *
**********************************************************************
*                                                                    *
* C-file generated by:                                               *
*                                                                    *
*        GUI_Builder for emWin version 5.26                          *
*        Compiled Aug  8 2014, 14:49:54                              *
*        (c) 2013 Segger Microcontroller GmbH & Co. KG               *
*                                                                    *
**********************************************************************
*                                                                    *
*        Internet: www.segger.com  Support: support@segger.com       *
*                                                                    *
**********************************************************************
*/



#include "DIALOG.h"
#include "DLG.h"
#include "usart.h"
#include "rtc.h"
#include "fontupd.h"

extern  GUI_BITMAP bmshezhi;
extern  GUI_BITMAP bmdengguang;
extern  GUI_BITMAP bmdata_icon;
extern  GUI_BITMAP bmdianqi_icon;
extern  GUI_BITMAP bmguanyu_icon;

extern  GUI_FONT GUI_Fontsongit24;
extern  GUI_FONT GUI_Fonticon24;
/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_FRAMEWIN_0        (GUI_ID_USER + 0x02)
#define ID_ICONVIEW_0        (GUI_ID_USER + 0x03)
#define ID_TEXT_0            (GUI_ID_USER + 0x04)
#define ID_TEXT_1            (GUI_ID_USER + 0x05)

int     indx;
int     timerconter;
char    date[30];

enum UiIcon{
	Light,
	data,
	control,
	config,
	info
};




/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

// USER START (Optionally insert additional static data)
// USER END

/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { FRAMEWIN_CreateIndirect, "Framewin", ID_FRAMEWIN_0, 0, 0, 800, 480, 0, 0x64, 0 },
  { ICONVIEW_CreateIndirect, "Iconview", ID_ICONVIEW_0, 0, 0, 800, 480, 0, 0x00640064, 0 },
	{ TEXT_CreateIndirect, "Text", ID_TEXT_0, 0, 0, 795, 35, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "Text1", ID_TEXT_1, 0, 0, 795, 35, 0, 0x0, 0 }
  // USER START (Optionally insert additional widgets)
  // USER END
};

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/

// USER START (Optionally insert additional static code)
// USER END

/*********************************************************************
*
*       _cbDialog
*/
static void _cbDialog(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int     NCode;
  int     Id;
	WM_HWIN ICONVIEW_hItem;

  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
		/*****************************************************************************************
																					样式设置
		*****************************************************************************************/
		hItem = pMsg->hWin;
		FRAMEWIN_SetTitleVis(hItem,0);
		
		GUI_UC_SetEncodeUTF8();
		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
    TEXT_SetFont(hItem, &GUI_Fontsongit24);
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
		TEXT_SetText(hItem,"Capstone课程设计-智能家居");
		TEXT_SetBkColor(hItem,0x000000);
		TEXT_SetTextColor(hItem,0xffffff);
	
		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
    TEXT_SetFont(hItem, GUI_FONT_COMIC24B_1);
    TEXT_SetTextAlign(hItem, TEXT_CF_RIGHT | GUI_TA_VCENTER);
		sprintf(date,"%0.2d/%0.2d/%0.2d %0.2d:%0.2d:%0.2d",calendar.w_year,calendar.w_month,calendar.w_date,calendar.hour,calendar.min,calendar.sec);
		TEXT_SetText(hItem,date);
		TEXT_SetTextColor(hItem,0xffffff);
		
		ICONVIEW_hItem = WM_GetDialogItem(pMsg->hWin, ID_ICONVIEW_0);
		ICONVIEW_SetIconAlign(ICONVIEW_hItem, ICONVIEW_IA_TOP);
		ICONVIEW_AddBitmapItem(ICONVIEW_hItem,&bmdengguang,"灯光控制");
		ICONVIEW_AddBitmapItem(ICONVIEW_hItem,&bmdata_icon,"家庭数据");
		ICONVIEW_AddBitmapItem(ICONVIEW_hItem,&bmdianqi_icon,"电器控制");
		ICONVIEW_AddBitmapItem(ICONVIEW_hItem,&bmshezhi,"系统设置");
		ICONVIEW_AddBitmapItem(ICONVIEW_hItem,&bmguanyu_icon,"关于");
		ICONVIEW_SetFont(ICONVIEW_hItem,&GUI_Fontsongit24);
		ICONVIEW_SetFrame(ICONVIEW_hItem, GUI_COORD_X, 50);//?????IconView?????
		ICONVIEW_SetFrame(ICONVIEW_hItem, GUI_COORD_Y, 50);
		ICONVIEW_SetSpace(ICONVIEW_hItem, GUI_COORD_X, (800-64*4)/3);//????????????
		ICONVIEW_SetSpace(ICONVIEW_hItem, GUI_COORD_Y, (480-64*2)/2);
		ICONVIEW_SetBkColor(ICONVIEW_hItem,ICONVIEW_CI_BK,0x4080ff);
		ICONVIEW_SetBkColor(ICONVIEW_hItem, ICONVIEW_CI_SEL, GUI_WHITE | 0x80000000);	
    break;
		/*****************************************************************************************
		*****************************************************************************************/
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case ID_ICONVIEW_0: // Notifications sent by 'Iconview'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
					/* 当小图标按下且松开时 检测是哪个小图标被按下 */
					indx = ICONVIEW_GetSel(WM_GetDialogItem(pMsg->hWin, ID_ICONVIEW_0));
					/* 根据按下的小图标来切换界面 */
					switch(indx){
						/* 如果是灯光界面被按下 */
						case Light:
						GUI_EndDialog(pMsg->hWin,0);
						CreateLightFramewin();	
						break;
						/* 如果是关于界面被按下 */
						case info:
						GUI_EndDialog(pMsg->hWin,0);
						CreatehuanyinFramewin();	
						break;
						/* 如果是居家数据界面被按下 */
						case data:
						GUI_EndDialog(pMsg->hWin,0);
						CreateDataUi();
						break;
						/* 如果是电气控制界面被按下 */
						case control:
						GUI_EndDialog(pMsg->hWin,0);
						ElectricalControl();
						break;
						/* 如果是系统配置界面被按下 */
						case config:
						GUI_EndDialog(pMsg->hWin,0);
						CreateConfig();
						break;
					}
        break;
      case WM_NOTIFICATION_MOVED_OUT:
        break;
      case WM_NOTIFICATION_SCROLL_CHANGED:
        break;
      case WM_NOTIFICATION_SEL_CHANGED:
        break;
      }
      break;
    }
    break;
	case WM_TIMER:
		sprintf(date,"%0.2d/%0.2d/%0.2d %0.2d:%0.2d:%0.2d",calendar.w_year,calendar.w_month,calendar.w_date,calendar.hour,calendar.min,calendar.sec);
		TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_1),date);
		WM_RestartTimer(pMsg->Data.v, 1000);
		break;
  default:
    WM_DefaultProc(pMsg);
    break;
  }
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       CreateFramewin
*/
WM_HWIN CreateFramewin(void);
WM_HWIN CreateFramewin(void) {
  WM_HWIN hWin;
	WM_HTIMER hTimer;
	WM_SetCreateFlags(WM_CF_MEMDEV_ON_REDRAW);
	hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
	hTimer = WM_CreateTimer(WM_GetClientWindow(hWin), 0, 1000, 0);
	return hWin;
}


/*************************** End of file ****************************/