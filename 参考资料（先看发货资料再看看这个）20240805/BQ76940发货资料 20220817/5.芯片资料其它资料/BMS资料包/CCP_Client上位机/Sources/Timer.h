#ifndef TIMER_H
#define TIMER_H
#include "type.h"
#define TIMER_CHANNEL_0  0
#define TIMER_CHANNEL_1  1
#define TIMER_CHANNEL_2  2
#define TIMER_CHANNEL_3  3
#define TIMER_CHANNEL_4  4
#define TIMER_CHANNEL_5  5
#define TIMER_CHANNEL_6  6
#define TIMER_CHANNEL_7  7

typedef  void (*PFUTimeProcess)();
void Timer_vInit();
void Timer_vSetTimer(UINT8 channel,UINT8 input,PFUTimeProcess process); 
UINT8 Timer_ubIsActive(UINT8 channel);
#endif