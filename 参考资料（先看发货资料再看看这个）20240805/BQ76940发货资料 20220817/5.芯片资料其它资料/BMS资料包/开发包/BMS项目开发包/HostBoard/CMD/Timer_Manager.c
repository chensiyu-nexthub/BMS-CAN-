/*
 * Timer_Manager.c
 *
 *  Created on: May 7, 2010
 *      Author: Administrator
 */

#include "Timer_Manager.h"
#include "QQ_Test.h"
#include "../include/Timer.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "CMD_call.h"
#include "CMD.h"
#include "shell.h"
#include "../include/ADC.h"
#include "Current_Task.h"
#include "Battery_Task.h"
#include "MainProcess.h"

unsigned long g_timeCounter=0;
unsigned int g_timeLoopCounter=0;

void Timer_Manager_ProcessInterrupt()
{
	//QQ_Test_MainLoop();
	g_timeCounter++;
	g_timeLoopCounter++;
	if(g_timeLoopCounter>500)
	{
		g_timeLoopCounter=0;
		MainProcess_Restart();
	}
	Current_Task();
}

char* Timer_ZeroStartRun(char* _1,char* _2,char* _3,char* _4)
{
	Timer_SetTimer(0);
	RTC_vRTCRun();
}
char* Timer_StopGetTimer(char* _1,char* _2,char* _3,char* _4)
{
	static char returnValue[20];
	RTC_vRTCStop();
	unsigned int counter=Timer_GetCounter();

	counter*=32;
	sprintf(returnValue,"Delay:%ldus\n",counter);
	return returnValue;
}
char* Timer_Delay_us(char* counter,char* _2,char* _3,char* _4)
{
	static char returnValue[20];
	int _counter__=atoi(counter);
	int _counter=_counter__/32;
	Timer_SetTimer(0);
	RTC_vRTCRun();
	while(Timer_GetCounter()<=_counter){};
	RTC_vRTCStop();
	sprintf(returnValue,"Delay:%dus Complete.\n",_counter__);
	return returnValue;
}
void Timer_Init_2()
{
	CMD_LoadFunction("TimerZeroStart",Timer_ZeroStartRun);
	CMD_LoadFunction("TimerStopGetTimer",Timer_StopGetTimer);
	CMD_LoadFunction("TimerDelay",Timer_Delay_us);
}
