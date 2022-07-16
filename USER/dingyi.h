#ifndef __DINGYI_H
#define __DINGYI_H
//任务优先级
#define START_TASK_PRIO				3
//任务堆栈大小	
#define START_STK_SIZE 				128
//任务控制块
OS_TCB StartTaskTCB;
//任务堆栈	
CPU_STK START_TASK_STK[START_STK_SIZE];
//任务函数
void start_task(void *p_arg);

//TOUCH任务
//设置任务优先级
#define TOUCH_TASK_PRIO				4
//任务堆栈大小
#define TOUCH_STK_SIZE				128
//任务控制块
OS_TCB TouchTaskTCB;
//任务堆栈
CPU_STK TOUCH_TASK_STK[TOUCH_STK_SIZE];
//touch任务
void touch_task(void *p_arg);

//LED0心跳灯
//设置任务优先级
#define LED0_TASK_PRIO 				5
//任务堆栈大小
#define LED0_STK_SIZE				128
//任务控制块
OS_TCB Led0TaskTCB;
//任务堆栈
CPU_STK LED0_TASK_STK[LED0_STK_SIZE];
//led0任务
void led0_task(void *p_arg);



//检测动态内存任务
//设置任务优先级
#define CHECKMEMORY_TASK_PRIO 				6
//任务堆栈大小
#define CHECKMEMORY_STK_SIZE				128
//任务控制块
OS_TCB CHECKMEMORYTaskTCB;
//任务堆栈
CPU_STK CHECKMEMORY_TASK_STK[CHECKMEMORY_STK_SIZE];
//led0任务
void CheckMemory_task(void *p_arg);

//EMWINDEMO任务
//设置任务优先级
#define EMWINDEMO_TASK_PRIO			7
//任务堆栈大小
#define EMWINDEMO_STK_SIZE			2048
//任务控制块
OS_TCB EmwindemoTaskTCB;
//任务堆栈
CPU_STK EMWINDEMO_TASK_STK[EMWINDEMO_STK_SIZE];
//emwindemo_task任务
	void emwindemo_task(void *p_arg);

//OneNet检测任务
//设置任务优先级
#define TASK8_PRIO			8
//任务堆栈大小
#define TASK8_STK_SIZE			256
//任务控制块
OS_TCB TASK8_TCB;
//任务堆栈
CPU_STK Task8_Stk[TASK8_STK_SIZE];
//emwindemo_task任务
	void Task8(void *p_arg);

//OneNet发送数据任务
//设置任务优先级
#define TASK9_PRIO			9
//任务堆栈大小
#define TASK9_STK_SIZE			256
//任务控制块
OS_TCB TASK9_TCB;
//任务堆栈
CPU_STK Task9_Stk[TASK9_STK_SIZE];
//emwindemo_task任务
	void Task9(void *p_arg);

#define TASK10_PRIO			10
//任务堆栈大小
#define TASK10_STK_SIZE			128
//任务控制块
OS_TCB TASK10_TCB;
//任务堆栈
CPU_STK Task10_Stk[TASK10_STK_SIZE];
//接受Onenet命令任务
void Task10(void *p_arg);

#define TASK11_PRIO			11
//任务堆栈大小
#define TASK11_STK_SIZE			128
//任务控制块
OS_TCB TASK11_TCB;
//任务堆栈
CPU_STK Task11_Stk[TASK10_STK_SIZE];
//接受Onenet命令任务
void Task11(void *p_arg);

#endif
