/*
 * Current_Task.c
 *
 *  Created on: Jun 21, 2010
 *      Author: Administrator
 */

#include "Current_Task.h"
#include "CAN.h"
#include "CAN_Task.h"
#include "BMS_CAN.h"
#include "../include/ADC.h"

int g_offset_1,g_offset_2;
#define __A
unsigned int g_current_1,g_current_2;

long g_current_value=0;
long g_quanlity=0;//A.s
extern unsigned long g_timeCounter;

void Current_Task()
{
	unsigned char CANBuffer[8]={0,0,0,0,0,0,0,0};
	static unsigned int sendCounter=0;
	unsigned int* currentReturn=ADC_GetCurrent();
	static int counter=0;
	static int increaseCounter=0;
	g_current_1=currentReturn[0];
	g_current_2=currentReturn[1];

#ifdef __A
	g_offset_1=2048-g_current_1;
	g_offset_2=2048-g_current_2;


#else
	g_offset_1=g_current_1-2048;
	g_offset_2=g_current_2-2048;
#endif
	if(g_offset_1<-1750||g_offset_1>1750)
	{
		g_offset_1=(int)(g_offset_2*11.61892);
	}
	g_current_value+=g_offset_1;
	if(++counter>1000)
	{
		counter=0;
		g_current_value=(long)(g_current_value/1000);
		g_quanlity+=g_current_value;
		g_current_value=0;
	}
	//if(++increaseCounter>10000)
}
