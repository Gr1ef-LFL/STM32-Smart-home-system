#include "bsp.h"


void BSP_Init(void)
{
	delay_init();	    	//延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	//设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);	 	//串口初始化为115200
 	LED_Init();			    //LED端口初始化
	TFTLCD_Init();			//LCD初始化	
	KEY_Init();	 			//按键初始化
	BEEP_Init();			//初始化蜂鸣器
	FSMC_SRAM_Init();		//初始化SRAM
	my_mem_init(SRAMIN); 	//初始化内部内存池
	my_mem_init(SRAMEX);  	//初始化外部内存池
  RTC_Init();         //初始化RTC
	exfuns_init();			//为fatfs文件系统分配内存
	f_mount(fs[0],"0:",1);	//挂载SD卡
	f_mount(fs[1],"1:",1);	//挂载FLASH
	TP_Init();				//触摸屏初始化
	font_init();
	Usart2_Init(115200);
	TIM4_Init(300,7200); 
	ADCx_Init();
	MotorInit();
	TIM2_PWM_ENABLE();
	TIM_SetCompare2(TIM2,0);
	TIM8_PWM_Init(199,7199);
	TIM1_PWM_Init(199,7199);
	TIM_SetCompare1(TIM8,195); // 0度
	LightControlInit();
	control_flag = 1;
}