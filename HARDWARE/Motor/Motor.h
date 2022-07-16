#ifndef __MOTOR_H
#define __MOTOR_H	 
#include "sys.h"


#define MotorPWM PEout(2)

void MotorInit(void);	

u8 ReadMotorStatus(void);
#endif
