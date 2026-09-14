/*
 * Current.c
 *
 *  Created on: May 21, 2010
 *      Author: Administrator
 */
#include "../include/ADC.h"
#include "DHAB.h"
#include "stdio.h"
#include "string.h"
#include "CMD_Call.h"
#include "wave.h"

unsigned int Current_SampleDelay=0x0;
unsigned int Current_ShowTimer=0x2;
unsigned int Current_SampleCouter=10;
double Current_Buffer[200];

char* Current_SetCurrent(char* pVariours_1,char* pVariours_2,char* pVariours_3,char* pVariours_4)
{
	unsigned int variours_1;
	unsigned int variours_2;
	variours_1=atoi(pVariours_1);
	variours_2=atoi(pVariours_2);
	Current_SampleDelay=variours_1;
	Current_SampleCouter=variours_2;
	return 0;
}
void Current_Delay(unsigned max)
{
	int i;
	for(i=0;i<max;i++)
		__nop();
}
char* Current_DelayTest(char* Counter,char* _2,char* _3,char* _4)
{
	unsigned int ADC_0;
	unsigned int ADC_1;
	double current;
	Current_SampleDelay=atoi(Counter);
	while(1)
	{
		ADC_0=ADC_GetChannel(0);
		ADC_1=ADC_GetChannel(1);
		current=DHAB_GetCurrent(ADC_0,ADC_1);
		Current_Buffer[0]=current;

		Wave_Change_1();
		Current_Delay(Current_SampleDelay);
	}
}

char* Current_Start(char* pVariours_1,char* pVariours_2,char* pVariours_3,char* pVariours_4)
{
	int i;
	unsigned int ADC_0;
	unsigned int ADC_1;
	double current;
	for(i=0;i<Current_SampleCouter;i++)
	{
		ADC_0=ADC_GetChannel(0);
		ADC_1=ADC_GetChannel(1);
		current=DHAB_GetCurrent(ADC_1,ADC_0);
		Current_Buffer[i]=current;
		Wave_Change_1();
		Current_Delay(Current_SampleDelay);
	}
	for(i=0;i<Current_SampleCouter;i++)
	{
		current=Current_Buffer[i];
		printf("C[%d]=%f; ",i,current);
	}
	return 0;
}
char* Current_Sample(char* pVariours_1,char* pVariours_2,char* pVariours_3,char* pVariours_4)
{
	unsigned int ADC_0=ADC_GetChannel(0);
	unsigned int ADC_1=ADC_GetChannel(1);
	double current=DHAB_GetCurrent(ADC_0,ADC_1);
	static char returnBuffer[20];
	sprintf(returnBuffer,"Current:%f\n",current);
	return returnBuffer;
}
void Current_Init()
{
	CMD_LoadFunction("CurrentSet",Current_SetCurrent);
	CMD_LoadFunction("CurrentStart",Current_Start);
	CMD_LoadFunction("CurrentSample",Current_Sample);
	CMD_LoadFunction("CurrentDelayTest",Current_DelayTest);
}

