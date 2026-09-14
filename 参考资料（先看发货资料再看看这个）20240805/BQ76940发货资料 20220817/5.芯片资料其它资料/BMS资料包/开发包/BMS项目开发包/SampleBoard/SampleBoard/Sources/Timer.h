#ifndef TIMER_H
#define TIMER_H
typedef  void (*TimeProcess)();
void Timer_Init();
void Timer_SetTimer(unsigned char channel,unsigned char input,TimeProcess process); 
unsigned char Timer_IsActive(unsigned char channel);
#endif