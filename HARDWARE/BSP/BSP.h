#ifndef __BSP_H
#define __BSP_H	 


/****************************************************************
	                       Include 
****************************************************************/


#include<stdio.h>
#include<stdlib.h>
#include "led.h"
#include "beep.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "ILI93xx.h"
#include "usart.h"	 
#include "24cxx.h"
#include "flash.h"
#include "touch.h"
#include "sram.h"
#include "timer.h"
#include "sdio_sdcard.h"
#include "malloc.h"
#include "GUI.h"
#include "ff.h"
#include "exfuns.h"
#include "w25qxx.h"
#include "includes.h"
#include "textdisplay.h"
#include "ds18b20.h"
#include "rtc.h"
#include "fontupd.h"
#include "mqtt.h"
#include "wifi.h"
#include "timer1.h"
#include "timer3.h"
#include "timer4.h"
#include "dht11.h"
#include "bh1750.h"
#include "MQ135_ADC.h"
#include "WM.h"
#include "DIALOG.h"
#include "motor.h"
#include "light.h"


/****************************************************************  
												板级外设初始化
****************************************************************/

void BSP_Init(void);


#endif

