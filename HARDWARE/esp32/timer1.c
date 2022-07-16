/*-------------------------------------------------*/
/*            超纬电子STM32系列开发板              */
/*-------------------------------------------------*/
/*                                                 */
/*            实现定时器1功能的源文件              */
/*                                                 */
/*-------------------------------------------------*/

#include "stm32f10x.h"  //包含需要的头文件

/*-------------------------------------------------*/
/*函数名：定时器2输出PWM波                  */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void TIM2_PWM_ENABLE(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;            //定义一个设置定时器的变量
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);               //设置中断向量分组：第2组 抢先优先级：0 1 2 3 子优先级：0 1 2 3		
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);           //使能TIM2时钟	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1|RCC_APB2Periph_GPIOE|RCC_APB2Periph_AFIO, ENABLE);
  TIM_DeInit(TIM2);                                             //定时器1寄存器恢复默认值
	TIM_TimeBaseInitStructure.TIM_Period = 100; 	          //设置自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=719;              //设置定时器预分频数
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;     //1分频
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);            //设置TIM1
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB  | RCC_APB2Periph_AFIO, ENABLE);  //使能GPIO外设和AFIO复用功能模块时钟
	
	GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2, ENABLE); //Timer2部分重映射  TIM2_CH2->PB5    
 	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);
   //设置该引脚为复用输出功能,输出TIM3 CH2的PWM脉冲波形	GPIOB.5
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIO	
		//初始化TIM3 Channel2 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性:TIM输出比较极性高
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM3 OC2

	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);  //使能TIM3在CCR2上的预装载寄存器
	TIM_Cmd(TIM2,ENABLE);                                         //关闭TIM1   
}

void TIM1_PWM_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 | RCC_APB2Periph_GPIOA, ENABLE);	//使能定时器1时钟
	
 
   //设置该引脚为复用输出功能,输出TIM2 CH1的PWM脉冲波形	GPIOA.0
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11; //TIM_CH1  TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIO
	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM1,ENABLE);
   //初始化TIM2
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	
	//初始化TIM Channel 1-4 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式1
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性:TIM输出比较极性高
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM1 OC1
	//TIM_OC2Init(TIM1, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM1 OC2
	//TIM_OC3Init(TIM1, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM1 OC3
	//TIM_OC4Init(TIM1, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM1 OC4
 
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);  //使能TIM1_CH1上的预装载寄存器
	//TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);  //使能TIM1_CH2上的预装载寄存器
	//TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);  //使能TIM1_CH3上的预装载寄存器
	//TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);  //使能TIM1_CH4上的预装载寄存器
	TIM_CtrlPWMOutputs(TIM1, ENABLE);                             //pwm输出
	TIM_Cmd(TIM1, ENABLE);  //使能TIM1
	TIM_ARRPreloadConfig(TIM1, ENABLE); //使能TIMx在ARR上的预装载寄存器
 
}

void TIM8_PWM_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8 | RCC_APB2Periph_GPIOC, ENABLE);	//使能定时器1时钟
	
 
   //设置该引脚为复用输出功能,输出TIM2 CH1的PWM脉冲波形	GPIOA.0
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; //TIM_CH1  TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化GPIO
//	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM1,ENABLE);
   //初始化TIM2
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	
	//初始化TIM Channel 1-4 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式1
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	//TIM_OCInitStructure.TIM_Pulse=500;
	TIM_OC1Init(TIM8, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM1 OC1
	//TIM_OC2Init(TIM1, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM1 OC2
	//TIM_OC3Init(TIM1, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM1 OC3
	//TIM_OC4Init(TIM1, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM1 OC4
 
	TIM_OC1PreloadConfig(TIM8, TIM_OCPreload_Enable);  //使能TIM1_CH1上的预装载寄存器
	//TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);  //使能TIM1_CH2上的预装载寄存器
	//TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);  //使能TIM1_CH3上的预装载寄存器
	//TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);  //使能TIM1_CH4上的预装载寄存器
	TIM_CtrlPWMOutputs(TIM8, ENABLE);                             //pwm输出
	TIM_Cmd(TIM8, ENABLE);  //使能TIM1
	TIM_ARRPreloadConfig(TIM8, ENABLE); //使能TIMx在ARR上的预装载寄存器
 
}

