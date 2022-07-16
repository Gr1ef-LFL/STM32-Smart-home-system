#include "MQ135_ADC.h"

__IO uint16_t ADC_ConvertedValue = 0;	//用于保存ADC采集转换到的数字量
float AirQuality = 0;	 				//用于保存由数字量转化成的空气ppm值
float ADC_ConvertedValueLocal = 0;  		//用于保存转换计算后的电压值    
//static 防止外部函数调用
static void ADCx_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	// 打开 ADC IO端口时钟
	ADC_GPIO_APBxClock_FUN ( ADC_GPIO_CLK, ENABLE );
	
	// 配置 ADC IO 引脚模式
	// 必须为模拟输入
	GPIO_InitStructure.GPIO_Pin = ADC_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	
	// 初始化 ADC IO
	GPIO_Init(ADC_PORT, &GPIO_InitStructure);	
}
// ANSI C标准，C89 标准
// C99
// C11 2011

 void ADCx_Mode_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	ADC_InitTypeDef ADC_InitStruct;	
	//时钟
	RCC_AHBPeriphClockCmd(ADC_DMA_CLK, ENABLE);
	
	DMA_DeInit(ADC_DMA_CHANNEL);
	//外设地址为：ADC数据寄存器地址(ADC_DR)
	DMA_InitStructure.DMA_PeripheralBaseAddr = ( uint32_t ) ( & ( ADC_x->DR ) );
	//存储器地址，实际上是内部SRAM的变量
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADC_ConvertedValue;
	//数据源来自于外设
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	//缓冲区大小未1,缓冲区的大小应该等于存储器的大小
	DMA_InitStructure.DMA_BufferSize = 1;
	//外设寄存器只有一个，地址不用递增
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	//存储器地址固定
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable; 
	//外设数据大小  半字即两个字节
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	//存储器数据大小也为半字节，跟外设数据大小相同
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	//循环传输模式
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;	
	//DMA传输通道优先级  高 
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	//禁止存储器到存储器模式，因为此时使用外设到存储器
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	//初始化DMA
	DMA_Init(ADC_DMA_CHANNEL, &DMA_InitStructure);
	//使能DMA通道
	DMA_Cmd(ADC_DMA_CHANNEL , ENABLE);
	//----------------------------------------------------------------------
	ADC_APBxClock_FUN ( ADC_CLK, ENABLE );
	ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;//独立
	ADC_InitStruct.ADC_ScanConvMode = DISABLE;	   //单通道,一个ADC通道不用扫描
	ADC_InitStruct.ADC_ContinuousConvMode = ENABLE;//连续转换
	ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//外部触发关闭
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;//右对齐
	ADC_InitStruct.ADC_NbrOfChannel = 1;			//采集通道数目1个通道
	ADC_Init(ADC_x, &ADC_InitStruct);	
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);				//配置为8分频
	//配置ADC通道CHANNEL的采样周期为：55.5个
	ADC_RegularChannelConfig(ADC_x, ADC_CHANNEL, 1, ADC_SampleTime_55Cycles5);	
	
 	// 使能ADC DMA 请求
	ADC_DMACmd(ADC_x, ENABLE);

	ADC_Cmd(ADC_x, ENABLE);	
	//复位校准寄存器
	ADC_ResetCalibration(ADC_x);
	// 等待复位完成
	while(ADC_GetResetCalibrationStatus(ADC_x));
	// ADC开始校准
	ADC_StartCalibration(ADC_x);
	// 等待校准完成
	while(ADC_GetCalibrationStatus(ADC_x));
	//未采用外部处罚，所以使用软件触发ADC转换
	ADC_SoftwareStartConvCmd(ADC_x, ENABLE);
}
void MQ135_GetValue(int* AirQuality_t)
{		
	ADC_ConvertedValueLocal =(float) ADC_ConvertedValue/4096*3.3; //12位AD 2^12 ADC内部参考电压3.3V 
	AirQuality = ADC_ConvertedValueLocal*990/3.3;
	*AirQuality_t = (int)AirQuality;
}
void ADCx_Init(void)
{
	ADCx_GPIO_Config();
	ADCx_Mode_Config();
}
