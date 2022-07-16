#include "light.h"

void LightControlInit(void)
{
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOG,ENABLE);	 //使能PB,PE端口时钟
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_4|GPIO_Pin_9;				 //LED0-->PB.5 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.5
 GPIO_ResetBits(GPIOB,GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_4|GPIO_Pin_9);
 
 Light1 = 0;
 Light2 = 0;
 Light3 = 0;
 Light4 = 0;
}

u8 ReadLight1Status(void)
{
	return GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_4);
}

u8 ReadLight2Status(void)
{
	return GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6);
}

u8 ReadLight3Status(void)
{
	return GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7);
}

u8 ReadLight4Status(void)
{
	return GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9);
}
	
