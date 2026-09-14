/*
 * Battery_Task.c
 *
 *  Created on: Jun 21, 2010
 *      Author: Administrator
 */
#include "Battery_Task.h"
#include "Battery.h"
#include "CAN_Task.h"
#include "BMS_CAN.h"
#include "CAN.h"
#include "Event.h"

extern unsigned int g_batteryStateArray[7][16];
extern unsigned int g_batteryVoltageArray[7][16];
extern unsigned int g_batteryTemptureArray[7][16];
extern unsigned int g_batteryEmptyArray[7];
extern unsigned char g_hostShareDataBuffer[64];
unsigned int g_uiAverageVolage;
unsigned int g_uiHighestVoltageID;
unsigned int g_uiHighestVoltage;
unsigned long g_ulTotalVoltage;
unsigned int g_uiLowestVoltageID;
unsigned int g_uiLowestVoltage;


static void Battery_Task_GetAllInfo()
{
	int batteryIndex;
	g_ulTotalVoltage=0;

	for(batteryIndex=0;batteryIndex<16;batteryIndex++)
	{
		g_ulTotalVoltage+=g_batteryVoltageArray[0][batteryIndex];
		if(g_batteryVoltageArray[0][batteryIndex]>g_uiHighestVoltage)
		{
			g_uiHighestVoltage=g_batteryVoltageArray[0][batteryIndex];
			g_uiHighestVoltageID=batteryIndex+0x0;
		}
		if(g_batteryVoltageArray[0][batteryIndex]<g_uiLowestVoltage)
		{
			g_uiLowestVoltage=g_batteryVoltageArray[0][batteryIndex];
			g_uiLowestVoltageID=batteryIndex+0x0;
		}
	}
	for(batteryIndex=0;batteryIndex<8;batteryIndex++)
	{
		g_ulTotalVoltage+=g_batteryVoltageArray[1][batteryIndex];
		if(g_batteryVoltageArray[1][batteryIndex]>g_uiHighestVoltage)
		{
			g_uiHighestVoltage=g_batteryVoltageArray[1][batteryIndex];
			g_uiHighestVoltageID=batteryIndex+0x10;
		}
		if(g_batteryVoltageArray[1][batteryIndex]<g_uiLowestVoltage)
		{
			g_uiLowestVoltage=g_batteryVoltageArray[1][batteryIndex];
			g_uiLowestVoltageID=batteryIndex+0x10;
		}
	}

	g_uiAverageVolage=g_ulTotalVoltage/24;

}
void Battery_Task()
{
	unsigned int averageVolage;
	unsigned int flag=0;
	unsigned char releaseFlag[5]={0,0,0,0,0};
	unsigned int voltag;
	int batteryIndex=0;
	int boardIndex=0;

	Battery_Task_GetAllInfo();
	averageVolage=g_uiAverageVolage;

#ifdef QQ_TEST
	flag=0;
	for(batteryIndex=0;batteryIndex<16;batteryIndex++)
	{
		voltag=g_batteryVoltageArray[0][batteryIndex];
		if(voltag>averageVolage+BATTERY_RELEASE_OFFSET)
		{
			flag|=1<<batteryIndex;
		}
	}
	releaseFlag[0]=flag&0xff;
	releaseFlag[1]=flag>>8;

	CAN_ChangeSendID(0x101);
	BMS_CAN_SendToSample_1(WM_START_EQUAL,releaseFlag);
	while(CAN_Task_GetStatus()!=0x1ee2);
	//printf("Send Release Flag:%x to Board 1\n",flag);

	flag=0;
	for(batteryIndex=0;batteryIndex<8;batteryIndex++)
	{
		voltag=g_batteryVoltageArray[1][batteryIndex];
		if(voltag>averageVolage+BATTERY_RELEASE_OFFSET)
		{
			flag|=1<<batteryIndex;
		}
	}
	releaseFlag[0]=flag&0xff;
	releaseFlag[1]=flag>>8;

	CAN_ChangeSendID(0x101);
	BMS_CAN_SendToSample_2(WM_START_EQUAL,releaseFlag);
	while(CAN_Task_GetStatus()!=0x1ee2);
	//printf("Send Release Flag:%x to Board 2\n",flag);
#else
	for(boardIndex=0;boardIndex<7;boardIndex++)
	{
		for(batteryIndex=0;batteryIndex<16;batteryIndex++)
		{

		}
	}
#endif

}
