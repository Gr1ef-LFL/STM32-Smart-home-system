#ifndef __LIGHT_H
#define __LIGHT_H	 
#include "sys.h"

#define Light1 PBout(4)
#define Light2 PBout(6)
#define Light3 PBout(7)
#define Light4 PBout(9)

void LightControlInit(void);
u8 ReadLight1Status(void);
u8 ReadLight2Status(void);
u8 ReadLight3Status(void);
u8 ReadLight4Status(void);
#endif

