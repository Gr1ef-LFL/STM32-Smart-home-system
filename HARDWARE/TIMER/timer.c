#include "timer.h"
#include "led.h"
#include "GUI.h"
#include "usart.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK 战舰开发板
//定时器 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2015/1/13
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 

//void TIM3_Int_Init(u16 arr,u16 psc)
//{
//	TIM_TimeBaseInitTypeDef	TIM_TimeBaseInitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);//开启TIM3时钟 

//	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;   //分频值
//	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;	   //计数模式
//	TIM_TimeBaseInitStructure.TIM_Period=arr;		   //自动重装数值
//	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;  //设置时钟分割
//	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);
//	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);//允许更新中断

//	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority=3;
//	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
//	NVIC_Init(&NVIC_InitStructure);

//	TIM_Cmd(TIM3,ENABLE);		  //使能TIM3
//}

//void TIM3_IRQHandler(void)
//{
//	if(TIM_GetITStatus(TIM3,TIM_IT_Update)!=RESET)
//	{	
//		//OS_TimeMS++;
//	}
//	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
//}

////基本定时器6中断初始化
////这里时钟选择为APB1的2倍，而APB1为36M
////arr：自动重装值。
////psc：时钟预分频数
////这里使用的是定时器6!
//void TIM6_Int_Init(u16 arr,u16 psc)
//{
//	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE); //定时器6时钟使能
//	
//	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //设置分频值，10khz的计数频率
//	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
//	TIM_TimeBaseInitStructure.TIM_Period=arr;  //自动重装载值 计数到5000为500ms
//	TIM_TimeBaseInitStructure.TIM_ClockDivision=0; //时钟分割:TDS=Tck_Tim
//	TIM_TimeBaseInit(TIM6,&TIM_TimeBaseInitStructure);
//	
//	TIM_ITConfig(TIM6,TIM_IT_Update|TIM_IT_Trigger,ENABLE); //使能TIM6的更新中断

//	NVIC_InitStructure.NVIC_IRQChannel=TIM6_IRQn; //TIM6中断
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1; //先占优先级1级
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority=3;  //从优先级3级
//	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE; //使能通道
//	NVIC_Init(&NVIC_InitStructure);
//	
//	TIM_Cmd(TIM6,ENABLE); //定时器6使能
//}


//void TIM6_IRQHandler(void)
//{
//	if(TIM_GetITStatus(TIM6,TIM_IT_Update)!=RESET)
//	{
//		//GUI_TOUCH_Exec();
//	}
//	TIM_ClearITPendingBit(TIM6,TIM_IT_Update); //清除中断标志位
//}




