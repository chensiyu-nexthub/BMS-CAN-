/*
 * Calendar_Task.c
 *
 *  Created on: May 7, 2010
 *      Author: Administrator
 */
#include "../include/calendar.h"

unsigned char Calendar_Task_Second;
unsigned char Calendar_Task_Minute;
unsigned char Calendar_Task_Hour;
unsigned char Calendar_Task_Day;
unsigned char Calendar_Task_Month;
unsigned char Calendar_Task_Year;

void Calender_Task_Test()
{

	Calendar_LoadDate();
	Calendar_Task_Second=Calendar_GetSecond();
	Calendar_Task_Minute=Calendar_GetMinute();
	Calendar_Task_Hour=Calendar_GetHour();
	Calendar_Task_Day=Calendar_GetDay();
	Calendar_Task_Month=Calendar_GetMonth();
	Calendar_Task_Year=Calendar_GetYear();
}
