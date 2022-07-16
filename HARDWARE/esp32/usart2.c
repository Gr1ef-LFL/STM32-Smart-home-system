/*-------------------------------------------------*/
/*            超纬电子STM32系列开发板              */
/*-------------------------------------------------*/
/*                                                 */
/*             实现串口2功能的源文件               */
/*                                                 */
/*-------------------------------------------------*/

#include "stm32f10x.h"  //包含需要的头文件
#include "usart2.h"     //包含需要的头文件
#include "timer4.h"     //包含需要的头文件

#if  USART2_RX_ENABLE                   //如果使能接收功能
char Usart2_RxCompleted = 0;            //定义一个变量 0：表示接收未完成 1：表示接收完成 
unsigned int Usart2_RxCounter = 0;      //定义一个变量，记录串口2总共接收了多少字节的数据
char Usart2_RxBuff[USART2_RXBUFF_SIZE]; //定义一个数组，用于保存串口2接收到的数据   	
#endif

/*-------------------------------------------------*/
/*函数名：初始化串口2发送功能                      */
/*参  数：bound：波特率                            */
/*返回值：无                                       */
/*-------------------------------------------------*/
void Usart2_Init(unsigned int bound)
{  	 	
    GPIO_InitTypeDef GPIO_InitStructure;     //定义一个设置GPIO功能的变量
	USART_InitTypeDef USART_InitStructure;   //定义一个设置串口功能的变量
#if USART2_RX_ENABLE                         //如果使能接收功能
	NVIC_InitTypeDef NVIC_InitStructure;     //如果使能接收功能，定义一个设置中断的变量
#endif

#if USART2_RX_ENABLE                                 //如果使能接收功能
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //设置中断向量分组：第2组 抢先优先级：0 1 2 3 子优先级：0 1 2 3
#endif	
      
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);  //使能串口3时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);   //使能GPIOB时钟
	USART_DeInit(USART3);                                  //串口2寄存器重新设置为默认值
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;              //准备设置PB10
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;      //IO速率50M
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	       //复用推挽输出，用于串口2的发送
    GPIO_Init(GPIOB, &GPIO_InitStructure);                 //设置PA2
   
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;              //准备设置PB11
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //浮空输入，用于串口2的接收
    GPIO_Init(GPIOB, &GPIO_InitStructure);                 //设置PA3
	
	USART_InitStructure.USART_BaudRate = bound;                                    //波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;                    //8个数据位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;                         //1个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;                            //无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
#if USART2_RX_ENABLE               												   //如果使能接收模式
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;	               //收发模式
#else                                                                              //如果不使能接收模式
	USART_InitStructure.USART_Mode = USART_Mode_Tx ;	                           //只发模式
#endif        
    USART_Init(USART3, &USART_InitStructure);                                      //设置串口2	

#if USART2_RX_ENABLE  	         					        //如果使能接收模式
	USART_ClearFlag(USART3, USART_FLAG_RXNE);	            //清除接收标志位
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);          //开启接收中断
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;       //设置串口2中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0; //抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//子优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//中断通道使能
	NVIC_Init(&NVIC_InitStructure);	                        //设置串口2中断
#endif  

	USART_Cmd(USART3, ENABLE);                              //使能串口2
}

/*-------------------------------------------------*/
/*函数名：串口2 printf函数                         */
/*参  数：char* fmt,...  格式化输出字符串和参数    */
/*返回值：无                                       */
/*-------------------------------------------------*/

__align(8) char USART2_TxBuff[USART2_TXBUFF_SIZE];  

void u2_printf(char* fmt,...) 
{  
	unsigned int i,length;
	
	va_list ap;
#if SYSTEM_SUPPORT_OS 		//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntEnter();    
#endif
	va_start(ap,fmt);
	vsprintf(USART2_TxBuff,fmt,ap);
	va_end(ap);	

	length=strlen((const char*)USART2_TxBuff);		
	while((USART3->SR&0X40)==0);
	for(i = 0;i < length;i ++)
	{			
		USART3->DR = USART2_TxBuff[i];
		while((USART3->SR&0X40)==0);	
	}	
	#if SYSTEM_SUPPORT_OS 	//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntExit();  											 
#endif
}

/*-------------------------------------------------*/
/*函数名：串口2发送缓冲区中的数据                  */
/*参  数：data：数据                               */
/*返回值：无                                       */
/*-------------------------------------------------*/
void u2_TxData(unsigned char *data)
{
	
	int	i;	
	#if SYSTEM_SUPPORT_OS 		//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntEnter();    
	#endif
	
	while((USART3->SR&0X40)==0);
	for(i = 1;i <= (data[0]*256+data[1]);i ++){			
		USART3->DR = data[i+1];
		while((USART3->SR&0X40)==0);	
	}
	
	#if SYSTEM_SUPPORT_OS 	//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
		OSIntExit();  											 
	#endif
}
