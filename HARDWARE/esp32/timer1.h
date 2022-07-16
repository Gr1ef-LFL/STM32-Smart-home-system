/*-------------------------------------------------*/
/*            超纬电子STM32系列开发板              */
/*-------------------------------------------------*/
/*                                                 */
/*            实现定时器1功能的头文件              */
/*                                                 */
/*-------------------------------------------------*/

#ifndef _TIMER1_H
#define _TIMER1_H
#include "sys.h"
void TIM2_PWM_ENABLE(void);
void TIM8_PWM_Init(u16 arr,u16 psc);
void TIM1_PWM_Init(u16 arr,u16 psc);
#endif
