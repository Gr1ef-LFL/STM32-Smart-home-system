#include "bsp.h"
#include "CMD.h"
/****************************************************************
	                       全局变量
****************************************************************/

extern WM_HWIN hWin_temp_Edit;
extern WM_HWIN hWin_humi_Edit;
extern WM_HWIN hWin_lux_Edit;
extern WM_HWIN hWin_ppm_Edit;
extern u8 windos_flag;
extern u8 control_flag;

/****************************************************************
	                       其他变量
****************************************************************/

/* 蜂鸣器定时器 */
OS_TMR BeepTmr;
/* 消息队列指针 */
OS_Q SensorDataMsg;


/****************************************************************
	                       定义任务优先级
****************************************************************/
#define START_TASK_PRIO							3
#define TOUCH_TASK_PRIO							4
#define LED0_TASK_PRIO 							5
#define DATACOLLECTION_TASK_PRIO 		6
#define UI_TASK_PRIO				      	7
#define CONNECTONENET_PRIO					8
#define DATAUPLOAD_PRIO							9
#define CMD_PRIO									  10
#define AUTOCONTROL_PRIO						11
/****************************************************************
	                       定义任务堆栈大小	
****************************************************************/
#define START_STK_SIZE 						128
#define TOUCH_STK_SIZE						128
#define LED0_STK_SIZE							128
#define DATACOLLECTION_STK_SIZE		128
#define UI_TASK_SIZE				      2048
#define CONNECTONENET_STK_SIZE		256
#define DATAUPLOAD_STK_SIZE			  256
#define CMD_STK_SIZE						  128
#define AUTOCONTROL_STK_SIZE			128
/****************************************************************
	                       定义任务控制块	
****************************************************************/
OS_TCB StartTaskTCB;
OS_TCB TouchTaskTCB;
OS_TCB Led0TaskTCB;
OS_TCB DATACOLLECTIONTaskTCB;
OS_TCB UI_TASK_TCB;
OS_TCB CONNECTONENET_TCB;
OS_TCB DATAUPLOAD_TCB;
OS_TCB CMD_TCB;
OS_TCB AUTOCONTROL_TCB;
/****************************************************************
	                       定义任务堆栈	
****************************************************************/
CPU_STK START_TASK_STK[START_STK_SIZE];
CPU_STK TOUCH_TASK_STK[TOUCH_STK_SIZE];
CPU_STK LED0_TASK_STK[LED0_STK_SIZE];
CPU_STK DATACOLLECTION_TASK_STK[DATACOLLECTION_STK_SIZE];
CPU_STK UI_TASK_STK[UI_TASK_SIZE];
CPU_STK CONNECTONENET_STK[CONNECTONENET_STK_SIZE];
CPU_STK DATAUPLOAD_STK[DATAUPLOAD_STK_SIZE];
CPU_STK CMD_STK[CMD_STK_SIZE];
CPU_STK AUTOCONTROL_STK[AUTOCONTROL_STK_SIZE];
/****************************************************************
	                       任务执行函数声明	
****************************************************************/
void start_task(void *p_arg);
void touch_task(void *p_arg);
void SystemLED_task(void *p_arg);
void datacollection_task(void *p_arg);
void UI_task(void *p_arg);
void connectOneNet_task (void *p_arg);
void dataupload_task(void *p_arg);
void cmd_task(void *p_arg);
void Autocontrol_task(void *p_arg);
void Beep_task(void *p_tmr, void *p_arg);


int main(void)
{	
	OS_ERR err;
	CPU_SR_ALLOC();
	BSP_Init();
	control_flag = 1;
	OSInit(&err);			//初始化UCOSIII
	OS_CRITICAL_ENTER();	//进入临界区
	//创建开始任务
	OSTaskCreate((OS_TCB 	* )&StartTaskTCB,		//任务控制块
				 (CPU_CHAR	* )"start task", 		//任务名字
                 (OS_TASK_PTR )start_task, 			//任务函数
                 (void		* )0,					//传递给任务函数的参数
                 (OS_PRIO	  )START_TASK_PRIO,     //任务优先级
                 (CPU_STK   * )&START_TASK_STK[0],	//任务堆栈基地址
                 (CPU_STK_SIZE)START_STK_SIZE/10,	//任务堆栈深度限位
                 (CPU_STK_SIZE)START_STK_SIZE,		//任务堆栈大小
                 (OS_MSG_QTY  )0,					//任务内部消息队列能够接收的最大消息数目,为0时禁止接收消息
                 (OS_TICK	  )0,					//当使能时间片轮转时的时间片长度，为0时为默认长度，
                 (void   	* )0,					//用户补充的存储区
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //任务选项
                 (OS_ERR 	* )&err);				//存放该函数错误时的返回值
	OS_CRITICAL_EXIT();	//退出临界区	 
	OSStart(&err);  //开启UCOSIII
	while(1);
}

//开始任务函数
void start_task(void *p_arg)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;

	CPU_Init();
#if OS_CFG_STAT_TASK_EN > 0u
   OSStatTaskCPUUsageInit(&err);  	//统计任务                
#endif
	
#ifdef CPU_CFG_INT_DIS_MEAS_EN		//如果使能了测量中断关闭时间
    CPU_IntDisMeasMaxCurReset();	
#endif

#if	OS_CFG_SCHED_ROUND_ROBIN_EN  //当使用时间片轮转的时候
	 //使能时间片轮转调度功能,时间片长度为1个系统时钟节拍，既1*5=5ms
	OSSchedRoundRobinCfg(DEF_ENABLED,1,&err);  
#endif		
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC,ENABLE);//开启CRC时钟
	GUI_Init();  			//STemWin初始化
	
	OS_CRITICAL_ENTER();	//进入临界区
	
	//创建传感器数据消息队列
	OSQCreate ((OS_Q         *)&SensorDataMsg,            //指向消息队列的指针
               (CPU_CHAR     *)"DATA of Sensor",  //队列的名字
               (OS_MSG_QTY    )30,                //最多可存放消息的数目
               (OS_ERR       *)&err); 
	//创建蜂鸣器鸣叫定时器
	OSTmrCreate((OS_TMR		*)&BeepTmr,		//定时器1
                (CPU_CHAR	*)"BeepTmr",		//定时器名字
                (OS_TICK	 )0,			//无延迟
                (OS_TICK	 )50,          //50*10=500ms
                (OS_OPT		 )OS_OPT_TMR_PERIODIC, //周期模式
                (OS_TMR_CALLBACK_PTR)Beep_task,//定时器1回调函数
                (void	    *)0,			//参数为0
                (OS_ERR	    *)&err);		//返回的错误码
	
	
	//UI_Task任务	
	OSTaskCreate((OS_TCB*     )&UI_TASK_TCB,		
				 (CPU_CHAR*   )"UI_Task任务	", 		
                 (OS_TASK_PTR )UI_task, 			
                 (void*       )0,					
                 (OS_PRIO	  )UI_TASK_PRIO,     
                 (CPU_STK*    )&UI_TASK_STK[0],	
                 (CPU_STK_SIZE)UI_TASK_SIZE/10,	
                 (CPU_STK_SIZE)UI_TASK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,  					
                 (void*       )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR*     )&err);
	//触摸任务
	OSTaskCreate((OS_TCB*     )&TouchTaskTCB,		
				 (CPU_CHAR*   )"Touch task", 		
                 (OS_TASK_PTR )touch_task, 			
                 (void*       )0,					
                 (OS_PRIO	  )TOUCH_TASK_PRIO,     
                 (CPU_STK*    )&TOUCH_TASK_STK[0],	
                 (CPU_STK_SIZE)TOUCH_STK_SIZE/10,	
                 (CPU_STK_SIZE)TOUCH_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,  					
                 (void*       )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR*     )&err);			 
	//采集数据任务
	OSTaskCreate((OS_TCB*     )&DATACOLLECTIONTaskTCB,		
				 (CPU_CHAR*   )"采集数据任务", 		
                 (OS_TASK_PTR )datacollection_task, 			
                 (void*       )0,					
                 (OS_PRIO	  )DATACOLLECTION_TASK_PRIO,     
                 (CPU_STK*    )&DATACOLLECTION_TASK_STK[0],	
                 (CPU_STK_SIZE)DATACOLLECTION_STK_SIZE/10,	
                 (CPU_STK_SIZE)DATACOLLECTION_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,  					
                 (void*       )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR*     )&err);						 
	//LED心跳灯任务
	OSTaskCreate((OS_TCB*     )&Led0TaskTCB,		
				 (CPU_CHAR*   )"LED心跳灯任务", 		
                 (OS_TASK_PTR )SystemLED_task, 			
                 (void*       )0,					
                 (OS_PRIO	  )LED0_TASK_PRIO,     
                 (CPU_STK*    )&LED0_TASK_STK[0],	
                 (CPU_STK_SIZE)LED0_STK_SIZE/10,	
                 (CPU_STK_SIZE)LED0_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,  					
                 (void*       )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR*     )&err);
	/* OneNet连接任务 */
    OSTaskCreate((OS_TCB     *)&CONNECTONENET_TCB,                
                 (CPU_CHAR   *)"OneNet连接任务",
                 (OS_TASK_PTR ) connectOneNet_task ,
                 (void       *) 0,
                 (OS_PRIO     ) CONNECTONENET_PRIO,
                 (CPU_STK    *)	&CONNECTONENET_STK[0],
                 (CPU_STK_SIZE) CONNECTONENET_STK_SIZE / 10,
                 (CPU_STK_SIZE) CONNECTONENET_STK_SIZE,
                 (OS_MSG_QTY  ) 0u,
                 (OS_TICK     ) 0u,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);
			
	/* OneNet上传数据任务 */
	    OSTaskCreate((OS_TCB     *)&DATAUPLOAD_TCB,                
                 (CPU_CHAR   *)"OneNet上传数据任务",
                 (OS_TASK_PTR ) dataupload_task,
                 (void       *) 0,
                 (OS_PRIO     ) DATAUPLOAD_PRIO,
                 (CPU_STK    *)	&DATAUPLOAD_STK[0],
                 (CPU_STK_SIZE) DATAUPLOAD_STK_SIZE / 10,
                 (CPU_STK_SIZE) DATAUPLOAD_STK_SIZE,
                 (OS_MSG_QTY  ) 0u,
                 (OS_TICK     ) 0u,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);
			OS_TaskSuspend((OS_TCB*)&DATAUPLOAD_TCB,&err);
	/* OneNet下发命令任务 */
	    OSTaskCreate((OS_TCB     *)&CMD_TCB,                
                 (CPU_CHAR   *)"OneNet下发命令任务",
                 (OS_TASK_PTR ) cmd_task,
                 (void       *) 0,
                 (OS_PRIO     ) CMD_PRIO,
                 (CPU_STK    *)	&CMD_STK[0],
                 (CPU_STK_SIZE) CMD_STK_SIZE / 10,
                 (CPU_STK_SIZE) CMD_STK_SIZE,
                 (OS_MSG_QTY  ) 0u,
                 (OS_TICK     ) 0u,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);
	/* 场景处理任务 */
	    OSTaskCreate((OS_TCB     *)&AUTOCONTROL_TCB,                
                 (CPU_CHAR   *)"场景处理任务",
                 (OS_TASK_PTR ) Autocontrol_task,
                 (void       *) 0,
                 (OS_PRIO     ) AUTOCONTROL_PRIO,
                 (CPU_STK    *)	&AUTOCONTROL_STK[0],
                 (CPU_STK_SIZE) AUTOCONTROL_STK_SIZE / 10,
                 (CPU_STK_SIZE) AUTOCONTROL_STK_SIZE,
                 (OS_MSG_QTY  ) 0u,
                 (OS_TICK     ) 0u,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);
	
	OS_TaskSuspend((OS_TCB*)&CMD_TCB,&err);							 
	OS_TaskSuspend((OS_TCB*)&AUTOCONTROL_TCB,&err);							 
	OS_TaskSuspend((OS_TCB*)&StartTaskTCB,&err);		//挂起开始任务			 
	OS_CRITICAL_EXIT();	//退出临界区
}

/* 蜂鸣器报警 */
void Beep_task(void *p_tmr, void *p_arg)
{
	BEEP = ~BEEP;
}


//UI显示任务
void UI_task(void *p_arg)
{
	while(1)
	{
		UI_DISPLAY();
	}
	
}


//TOUCH任务
void touch_task(void *p_arg)
{
	OS_ERR err;
	while(1)
	{
		GUI_TOUCH_Exec();	
		OSTimeDlyHMSM(0,0,0,5,OS_OPT_TIME_PERIODIC,&err);//延时5ms
	}
}

//传感器检测任务
void datacollection_task(void *p_arg)
{
	int SensorData[5] = {0};
	char temp_data[7];
	char humi_data[7];
	char lux_data[25];
	char ppm_data[10];
	OS_ERR err;
	DHT11_Init();
	iic_by30_init();
	while(1)
	{
		OSIntEnter();	
		DHT11_Read_Data(&SensorData[0],&SensorData[1]);
		get_sunlight_value(&SensorData[2]);
		OSIntExit();
		MQ135_GetValue(&SensorData[3]);	
		if(windos_flag){
			sprintf(temp_data,"%d C",SensorData[0]);
			sprintf(humi_data,"%d%%",SensorData[1]);
			sprintf(lux_data,"%d Lux",SensorData[2]);
			sprintf(ppm_data,"%d PPM",SensorData[3]);
			TEXT_SetText(hWin_temp_Edit,temp_data);
			TEXT_SetText(hWin_humi_Edit,humi_data);
			TEXT_SetText(hWin_lux_Edit,lux_data);
			TEXT_SetText(hWin_ppm_Edit,ppm_data);
		}
		
		if(SensorData[2] >= 500)
			SensorData[2] = 500;
		/* 发送消息队列给OneEnt上传任务 */
		OSQPost((OS_Q        *)&SensorDataMsg,                             //消息变量指针
					 (void        *)SensorData,                //要发送的数据的指针，将内存块首地址通过队列“发送出去”
					 (OS_MSG_SIZE  )30,     //数据字节大小
					 (OS_OPT       )OS_OPT_POST_FIFO | OS_OPT_POST_ALL, //先进先出和发布给全部任务的形式
					 (OS_ERR      *)&err);	                            //返回错误类型		
		//printf("已经发送信号量给上云任务！\r\n");
		OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_PERIODIC,&err);//延时1ms
	}
}

//LED心跳灯
void SystemLED_task(void *p_arg)
{
	OS_ERR err;
	CPU_STK_SIZE free,used;  
	while(1)
	{
		LED0 = !LED0;
		OSTaskStkChk (&UI_TASK_TCB,&free,&used,&err);  
		//printf("control_flag: %d\r\n",control_flag);
    //printf("EmwindemoTaskTCB      used/free:%d/%d  usage:%%%d\r\n",used,free,(used*100)/(used+free));
		OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_PERIODIC,&err);//延时500ms
	}
}

//上云检测任务
void connectOneNet_task(void *p_arg)
{
	OS_ERR err;
	WiFi_ResetIO_Init();            //初始化WiFi的复位IO
  MQTT_Buff_Init();               //初始化接收,发送,命令数据的 缓冲区 以及各状态参数
	OneNetIoT_Parameter_Init();	    //初始化连接OneNet平台MQTT服务器的参数	
	while(1)                        //主循环
	{		
		/*--------------------------------------------------------------------*/
		/*   Connect_flag=1同服务器建立了连接,我们可以发布数据和接收推送了    */
		/*--------------------------------------------------------------------*/
		if(Connect_flag==1)
		{     
			/*-------------------------------------------------------------*/
			/*                     处理发送缓冲区数据                      */
			/*-------------------------------------------------------------*/
				if(MQTT_TxDataOutPtr != MQTT_TxDataInPtr)
			{                //if成立的话，说明发送缓冲区有数据了
				//3种情况可进入if
				//第1种：0x10 连接报文
				//第2种：0x82 订阅报文，且ConnectPack_flag置位，表示连接报文成功
				//第3种：SubcribePack_flag置位，说明连接和订阅均成功，其他报文可发
				if((MQTT_TxDataOutPtr[2]==0x10)||((MQTT_TxDataOutPtr[2]==0x82)&&(ConnectPack_flag==1))||(SubcribePack_flag==1))
				{    
					printf("发送数据:0x%x\r\n",MQTT_TxDataOutPtr[2]);  //串口提示信息
					MQTT_TxData(MQTT_TxDataOutPtr);                       //发送数据
					MQTT_TxDataOutPtr += BUFF_UNIT;                       //指针下移
					if(MQTT_TxDataOutPtr==MQTT_TxDataEndPtr)              //如果指针到缓冲区尾部了
						MQTT_TxDataOutPtr = MQTT_TxDataBuf[0];            //指针归位到缓冲区开头
				} 				
			}//处理发送缓冲区数据的else if分支结尾
			
			/*-------------------------------------------------------------*/
			/*                     处理接收缓冲区数据                      */
			/*-------------------------------------------------------------*/
			if(MQTT_RxDataOutPtr != MQTT_RxDataInPtr){  //if成立的话，说明接收缓冲区有数据了														
				printf("接收到数据:");
				/*-----------------------------------------------------*/
				/*                    处理CONNACK报文                  */
				/*-----------------------------------------------------*/				
				//if判断，如果第一个字节是0x20，表示收到的是CONNACK报文
				//接着我们要判断第4个字节，看看CONNECT报文是否成功
				if(MQTT_RxDataOutPtr[2]==0x20){             			
				    switch(MQTT_RxDataOutPtr[5]){					
						case 0x00 : printf("CONNECT报文成功\r\n");                            //串口输出信息	
								    ConnectPack_flag = 1;                                        //CONNECT报文成功，订阅报文可发
									break;                                                       //跳出分支case 0x00                                              
						case 0x01 : printf("连接已拒绝，不支持的协议版本，准备重启\r\n");     //串口输出信息
									Connect_flag = 0;                                            //Connect_flag置零，重启连接
									break;                                                       //跳出分支case 0x01   
						case 0x02 : printf("连接已拒绝，不合格的客户端标识符，准备重启\r\n"); //串口输出信息
									Connect_flag = 0;                                            //Connect_flag置零，重启连接
									break;                                                       //跳出分支case 0x02 
						case 0x03 : printf("连接已拒绝，服务端不可用，准备重启\r\n");         //串口输出信息
									Connect_flag = 0;                                            //Connect_flag置零，重启连接
									break;                                                       //跳出分支case 0x03
						case 0x04 : printf("连接已拒绝，无效的用户名或密码，准备重启\r\n");   //串口输出信息
									Connect_flag = 0;                                            //Connect_flag置零，重启连接						
									break;                                                       //跳出分支case 0x04
						case 0x05 : printf("连接已拒绝，未授权，准备重启\r\n");               //串口输出信息
									Connect_flag = 0;                                            //Connect_flag置零，重启连接						
									break;                                                       //跳出分支case 0x05 		
						default   : printf("连接已拒绝，未知状态，准备重启\r\n");             //串口输出信息 
									Connect_flag = 0;                                            //Connect_flag置零，重启连接					
									break;                                                       //跳出分支case default 								
					}
				}			
				//if判断，第一个字节是0x90，表示收到的是SUBACK报文
				//接着我们要判断订阅回复，看看是不是成功
				else if(MQTT_RxDataOutPtr[2]==0x90){ 
						switch(MQTT_RxDataOutPtr[6]){					
						case 0x00 :
						case 0x01 : printf("订阅成功\r\n");            //串口输出信息
												SubcribePack_flag = 1;                //SubcribePack_flag置1，表示订阅报文成功，其他报文可发送
												Ping_flag = 0;                        //Ping_flag清零
												LED1 = 0;
												TIM3_ENABLE_30S();                    //启动30s的PING定时器
												OS_TaskResume(&DATAUPLOAD_TCB,&err);
												OS_TaskResume(&CMD_TCB,&err);
												OS_TaskResume(&AUTOCONTROL_TCB,&err);
												break;                                //跳出分支                                             
						default   : printf("订阅失败，准备重启\r\n");  //串口输出信息 
												Connect_flag = 0;                     //Connect_flag置零，重启连接
												break;                                //跳出分支 								
					}					
				}
				//if判断，第一个字节是0xD0，表示收到的是PINGRESP报文
				else if(MQTT_RxDataOutPtr[2]==0xD0)
			{ 
					printf("PING报文回复\r\n"); 		  //串口输出信息 
					if(Ping_flag==1){                     //如果Ping_flag=1，表示第一次发送
						 Ping_flag = 0;    				  //要清除Ping_flag标志
					}else if(Ping_flag>1){ 				  //如果Ping_flag>1，表示是多次发送了，而且是2s间隔的快速发送
						Ping_flag = 0;     				  //要清除Ping_flag标志
						TIM3_ENABLE_30S(); 				  //PING定时器重回30s的时间
					}				
				}	
				//if判断，如果第一个字节是0x30，表示收到的是服务器发来的推送数据
				//我们要提取控制命令
				else if((MQTT_RxDataOutPtr[2]==0x30))
				{ 
					printf("服务器等级0推送\r\n"); 		   //串口输出信息 
					MQTT_DealPushdata_Qs0(MQTT_RxDataOutPtr);  //处理等级0推送数据
				}				
								
				MQTT_RxDataOutPtr += BUFF_UNIT;                     //指针下移
				if(MQTT_RxDataOutPtr==MQTT_RxDataEndPtr)            //如果指针到缓冲区尾部了
					MQTT_RxDataOutPtr = MQTT_RxDataBuf[0];          //指针归位到缓冲区开头                        
			}//处理接收缓冲区数据的else if分支结尾		
		}//Connect_flag=1的if分支的结尾
		
		/*--------------------------------------------------------------------*/
		/*      Connect_flag=0同服务器断开了连接,我们要重启连接服务器         */
		/*--------------------------------------------------------------------*/
		else
		{ 
			printf("需要连接服务器\r\n");                 //串口输出信息
			TIM_Cmd(TIM4,DISABLE);                           //关闭TIM4 
			TIM_Cmd(TIM3,DISABLE);                           //关闭TIM3  
			WiFi_RxCounter=0;                                //WiFi接收数据量变量清零                        
			memset(WiFi_RX_BUF,0,WiFi_RXBUFF_SIZE);          //清空WiFi接收缓冲区 
			if(WiFi_Connect_IoTServer()==0)
			{   			     //如果WiFi连接云服务器函数返回0，表示正确，进入if
				printf("建立TCP连接成功\r\n");            //串口输出信息
				Connect_flag = 1;                            //Connect_flag置1，表示连接成功	
				WiFi_RxCounter=0;                            //WiFi接收数据量变量清零                        
				memset(WiFi_RX_BUF,0,WiFi_RXBUFF_SIZE);      //清空WiFi接收缓冲区 
				MQTT_Buff_ReInit();                          //重新初始化发送缓冲区                    
			}				
		}
		
		delay_ms(100);
	}
}

void dataupload_task(void *p_arg)
{
	
	OS_ERR err;
	OS_MSG_SIZE MsgSize;
	while(1)
	{		
		char head1[3];
		char temp[50];				//定义一个临时缓冲区1,不包括报头
		char tempAll[150];			//定义一个临时缓冲区2，包括所有数据
		int	dataLen = 0;			//报文长度
		int *TEXT_Buffer;	
		
		TEXT_Buffer = OSQPend ((OS_Q         *)&SensorDataMsg,               
									(OS_TICK       )0,                   
									(OS_OPT        )OS_OPT_PEND_BLOCKING,  //如果没有获取到信号量就等待
									(OS_MSG_SIZE  *)&MsgSize,          
									(CPU_TS       *)0,                     
									(OS_ERR       *)&err); 
		//printf("已经接受到信号量！\r\n");
		//printf("接受到的温:%d，湿度：%d，光:%d,空:%d\r\n",TEXT_Buffer[0],TEXT_Buffer[1],TEXT_Buffer[2],TEXT_Buffer[3]);
		memset(temp,    0, 50);				    //清空缓冲区1
		memset(tempAll, 0, 100);				//清空缓冲区2
		memset(head1,   0, 3);					//清空MQTT头
		sprintf(temp,"{\"temperature\":\"%d\",\"humidity\":\"%d\",\"Lux\":\"%d\",\"ppm\":\"%d\"}",
		TEXT_Buffer[0], TEXT_Buffer[1],TEXT_Buffer[2],TEXT_Buffer[3]);//构建报文
		head1[0] = 0x03; 						//固定报头
		head1[1] = 0x00; 						//固定报头
		head1[2] = strlen(temp);  				//剩余长度	
		sprintf(tempAll, "%c%c%c%s", head1[0], head1[1], head1[2], temp);
		dataLen = strlen(temp) + 3;
		//printf("%s",tempAll);
		MQTT_PublishQs0(P_TOPIC_NAME,tempAll, dataLen);//添加数据，发布给服务器
		//printf("Seed Data OK!\r\n ");
		OSTimeDlyHMSM(0,0,5,0,OS_OPT_TIME_PERIODIC,&err);//延时1ms
	}
}
/* 云端命令处理任务 */
void cmd_task(void *p_arg)
{
	OS_ERR err;
	
	char online_cmd[20];
	char *cmd;
	unsigned char OnlinePwmControl;
	while(1)
	{
		
		/* 云端指令处理 */
		if(MQTT_CMDOutPtr != MQTT_CMDInPtr)							  //if成立的话，说明命令缓冲区有数据了	
			{                             		       
					printf("命令:%s\r\n",&MQTT_CMDOutPtr[2]);              //串口输出信息
				
				/* 接收到云控制信号之后关闭智能调节模式 */
				control_flag = 0;
				
				
				/**************************  
				
					接受到控制灯的指令
				
				**************************/
				if(!memcmp(&MQTT_CMDOutPtr[2],LED1_ON,strlen(LED1_ON)))//判断指令，如果是LED1_ON
				{                                            
					Light1 =  1;
					printf("已打开LED1！\r\n");
				}
				if(!memcmp(&MQTT_CMDOutPtr[2],LED1_OFF,strlen(LED1_OFF)))//判断指令，如果是LED1_ON
				{                                            
					Light1 =  0;
					printf("已关闭LED1！\r\n");
				}
				
					if(!memcmp(&MQTT_CMDOutPtr[2],LED2_ON,strlen(LED2_ON)))//判断指令，如果是LED1_ON
				{                                            
					Light2 =  1;
					printf("已打开LED2！\r\n");
				}
				if(!memcmp(&MQTT_CMDOutPtr[2],LED2_OFF,strlen(LED2_OFF)))//判断指令，如果是LED1_ON
				{                                            
					Light2 =  0;
					printf("已关闭LED2！\r\n");
				}
				
					if(!memcmp(&MQTT_CMDOutPtr[2],LED3_ON,strlen(LED3_ON)))//判断指令，如果是LED1_ON
				{                                            
					Light3 =  1;
					printf("已打开LED3！\r\n");
				}
				if(!memcmp(&MQTT_CMDOutPtr[2],LED3_OFF,strlen(LED3_OFF)))//判断指令，如果是LED1_ON
				{                                            
					Light3 =  0;
					printf("已关闭LED3！\r\n");
				}
				
					if(!memcmp(&MQTT_CMDOutPtr[2],LED4_ON,strlen(LED4_ON)))//判断指令，如果是LED1_ON
				{                                            
					Light4 =  1;
					printf("已打开LED4！\r\n");
				}
				if(!memcmp(&MQTT_CMDOutPtr[2],LED4_OFF,strlen(LED4_OFF)))//判断指令，如果是LED1_ON
				{                                            
					Light4 =  0;
					printf("已关闭LED4！\r\n");
				}
				
				
				/************************** 
				
				接受到窗户和排气扇的指令 
				
				**************************/
				
				if(!memcmp(&MQTT_CMDOutPtr[2],Paiqishan_ON,strlen(Paiqishan_ON)))//判断指令，如果是LED1_ON
				{                                            
					MotorPWM = 1;
					printf("已打开排气扇！\r\n");
				}
				if(!memcmp(&MQTT_CMDOutPtr[2],Paiqishan_OFF,strlen(Paiqishan_OFF)))//判断指令，如果是LED1_ON
				{                                            
					MotorPWM = 0;
					printf("已关闭排气扇！\r\n");
				}
				
				if(!memcmp(&MQTT_CMDOutPtr[2],Windos_ON,strlen(Windos_ON)))//判断指令，如果是LED1_ON
				{                                            
					TIM_SetCompare1(TIM8,175); // 180度;
					printf("已打开窗户！\r\n");
				}
				if(!memcmp(&MQTT_CMDOutPtr[2],Windos_OFF,strlen(Windos_OFF)))//判断指令，如果是LED1_ON
				{                                            
					TIM_SetCompare1(TIM8,195);
					printf("已关闭窗户！\r\n");
				}
				
				
				/**************************  
				
					接受到控制电机的指令
				
				**************************/
				if(!memcmp(&MQTT_CMDOutPtr[2],dianji,strlen(dianji)))
				{
					/* 
						OneNet控制命令为：fangshan:控制数值 
						控制数值范围为:0 - 100
						取出指令中的控制数值字符串并转化为u8类型
					*/
					strcpy(online_cmd,(char*)&MQTT_CMDOutPtr[2]);
					cmd = &online_cmd[9];
					OnlinePwmControl = atoi(cmd);
					printf("%d",OnlinePwmControl);
					TIM_SetCompare2(TIM2,OnlinePwmControl);
					
				}					
				//不做处理，后面会发送状态
				else printf("未知指令\r\n");				//串口输出信息	
				MQTT_CMDOutPtr += BUFF_UNIT;                             	 //指针下移
				if(MQTT_CMDOutPtr==MQTT_CMDEndPtr)           	             //如果指针到缓冲区尾部了
					MQTT_CMDOutPtr = MQTT_CMDBuf[0];          	             //指针归位到缓冲区开头		
			}
		OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_PERIODIC,&err);//延时1ms
	}
}

/* 场景处理任务 */
void Autocontrol_task(void *p_arg)
{
	OS_ERR err;
	OS_MSG_SIZE MsgSize;
	while(1)
	{
		int *TEXT_Buffer;	
		TEXT_Buffer = OSQPend ((OS_Q         *)&SensorDataMsg,               
									(OS_TICK       )0,                   
									(OS_OPT        )OS_OPT_PEND_BLOCKING,  //如果没有获取到信号量就等待
									(OS_MSG_SIZE  *)&MsgSize,          
									(CPU_TS       *)0,                     
									(OS_ERR       *)&err);
		
		/* 
			 TEXT_Buffer[0] = 温度 
			 TEXT_Buffer[1] = 湿度
			 TEXT_Buffer[2] = 光强
		   TEXT_Buffer[3] = 煤气含量检测
		*/
		
		/**********************************
		
			场景1：出现煤气泄漏的情况 
		
		**********************************/
		
		/* 可能出现煤气泄漏 */
		if(TEXT_Buffer[3] >= 300){
			printf("场景1\r\n");
			/* 蜂鸣器报警 */
			OSTmrStart(&BeepTmr,&err);
			/* 开窗通风 */
			TIM_SetCompare1(TIM8,175);
			/* 开排气扇 */
			//MotorPWM = 1;
		}else if(TEXT_Buffer[3] <= 300){
			/* 关闭蜂鸣器报警 */
			BEEP = 0;
			OSTmrStop(&BeepTmr,OS_OPT_TMR_NONE,0,&err);
			/* 关闭窗口 */
			TIM_SetCompare1(TIM8,195);
			/* 关闭排气扇 */
			//MotorPWM = 0;			
		}
		
		/**********************************
		
			场景2：根据光照强度调节窗帘的开关 
		
		**********************************/
		
			if(TEXT_Buffer[2] <= 10 && control_flag){
				/* 到了深夜 */
				TIM_SetCompare1(TIM1,195);
			}else if(TEXT_Buffer[2] <= 30 && TEXT_Buffer[2] >= 10 && control_flag){
				/* 有一点光 */
				TIM_SetCompare1(TIM1,190); // 45度
		}else if(TEXT_Buffer[2] <= 120 && TEXT_Buffer[2] >= 30 && control_flag){
			 /* 有光照  */
			TIM_SetCompare1(TIM1,185); // 90度
		}else if (TEXT_Buffer[2] <= 400 && TEXT_Buffer[2] >= 120 && control_flag){
			/* 日出 */
			TIM_SetCompare1(TIM1,175); // 180度
		}
		
		/**********************************
		
			场景3：根据温度和湿度开关风扇或开关窗户
		
		**********************************/
		
		if(TEXT_Buffer[1] >= 30 && control_flag){
			TIM_SetCompare2(TIM2,50);
		}
		else if(TEXT_Buffer[1] >= 35 && control_flag){
			TIM_SetCompare2(TIM2,100);
			TIM_SetCompare1(TIM8,175);
		}else if (TEXT_Buffer[1] <= 30 && control_flag) 
		{
			TIM_SetCompare2(TIM2,0);
			TIM_SetCompare1(TIM8,195);
		}
	}
}
