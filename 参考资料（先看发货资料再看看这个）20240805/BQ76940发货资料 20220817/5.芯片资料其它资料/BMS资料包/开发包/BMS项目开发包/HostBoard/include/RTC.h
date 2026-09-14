#ifndef _RTC_H_
#define _RTC_H_

typedef void (*PF_Timer_InterruptProcess)();

void Timer_Init(PF_Timer_InterruptProcess pF);

void RTC_vInit(void);
void RTC_vRTCRun(void);
void RTC_vRTCStop(void);
unsigned int Timer_GetCounter();
void Timer_SetTimer(unsigned int counter);


#define RTCINT    0x6E



#endif  // ifndef _RTC_H_
