


#ifndef _MAIN_H_
#define _MAIN_H_


#define TASKING_VIPER

#define far  __far
#define _inline  inline
#define _nop  __nop
#define _interrupt  __interrupt

int Module_InitStart();//return 1
int Module_InitEnd();//return 2
void MAIN_vUnlockProtecReg(void);
void MAIN_vLockProtecReg(void);

#include  "XC22xxMREGS.h"
#include  "SCS.h"

#endif  // ifndef _MAIN_H_
