#ifndef __MQ135_ADC_H__
#define	__MQ135_ADC_H__


#include "stm32f10x.h"
/**---ADC选择-----ADC1 PC5
 *
 **DMA通道选择----DMA1 CHANNEL1
 *
 **/
// ADC GPIO宏定义
// 注意：用作ADC采集的IO必须没有复用，否则采集电压会有影响
#define    ADC_GPIO_APBxClock_FUN        RCC_APB2PeriphClockCmd
#define    ADC_GPIO_CLK                  RCC_APB2Periph_GPIOC  
#define    ADC_PORT                      GPIOC
#define    ADC_PIN                       GPIO_Pin_5


// ADC 编号选择
// 可以是 ADC1/2，如果使用ADC3，中断相关的要改成ADC3的
#define    ADC_APBxClock_FUN             RCC_APB2PeriphClockCmd
#define    ADC_x                         ADC1
#define    ADC_CLK                       RCC_APB2Periph_ADC1

// ADC 通道宏定义
#define    ADC_CHANNEL                   ADC_Channel_15

#define    ADC_DMA_CLK                   RCC_AHBPeriph_DMA1
#define    ADC_DMA_CHANNEL               DMA1_Channel1

/*****一些变量的声明(定义)****/
//__IO uint16_t ADC_ConvertedValue;	//用于保存ADC采集转换到的数字量
//float AirQuality0;	 				//用于保存由数字量转化成的空气ppm值
//float ADC_ConvertedValueLocal0;  		//用于保存转换计算后的电压值    

/**
  * @概要 	MQ135_ADC初始化函数  采用ADC单通道DMA读取
  *         ADC数据存储在      ADC_ConvertedValue 
  *			调用ADC数据时需要  extern __IO uint16_t ADC_ConvertedValue;  * 
  * @操作数 无
  * @返回值 无

  **/
void ADCx_Init(void);
/**
  * @概要 	MQ135AD转化数据获取函数
  *			存储了 ：ADC_ConvertedValue		AirQuality		ADC_ConvertedValueLocal
  *			需要在主函数声明
  *			
  * @操作数 无
  * @返回值 无
  **/
void MQ135_GetValue(int* AirQuality_t);

#endif /* __MQ135_ADC_H__ */
