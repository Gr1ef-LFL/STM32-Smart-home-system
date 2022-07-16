#ifndef DLH_H            // Guard against multiple inclusion
#define DLH_H
#include "DIALOG.h"

/* 主界面UI */
WM_HWIN CreateFramewin(void);

/* 灯光控制界面UI */
WM_HWIN CreateLightFramewin(void);

/* 欢迎界面UI */
WM_HWIN CreatehuanyinFramewin(void);

/* 居家数据UI */
WM_HWIN CreateDataUi(void);

/* 电气控制UI */
WM_HWIN ElectricalControl(void);

/* 系统设置UI */
WM_HWIN CreateConfig(void);

/* 设置系统时间子UI*/
WM_HWIN CreateTimeSetWindow(void);

#endif
