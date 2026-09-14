/*
 * Battery.c
 *
 *  Created on: May 6, 2010
 *      Author: Administrator
 */
#include "Battery.h"
#include "Stdio.h"
#include "string.h"
#include "stdlib.h"
#include "BMS_CAN.h"
#include "can.h"
#include "CAN_Task.h"
#include "Event.h"
#include "CMD_CALL.h"
#include "../include/RTC.h"
#include "battery.h"

unsigned int g_batteryStateArray[7][16];
unsigned int g_batteryVoltageArray[7][16];
unsigned int g_batteryTemptureArray[7][16];
unsigned int g_batteryEmptyArray[7];

static void Battery_ReleaseUintToChars(unsigned int in,unsigned char* out);
static unsigned int Battery_Reconstruct(int input);
static double Battery_GetVoltage(unsigned int input);
static char* Battery_GetBattery(char* _v1,char* _v2,char* _v3,char* _v4);
static char* Battery_GetBatteryAll(char* _v1,char* _v2,char* _v3,char* _v4);
static char* Battery_Release(char* _v1,char* _v2,char* _v3,char* _v4);
static char* Battery_ReleaseAll(char* _v1,char* _v2,char* _v3,char* _v4);
static char* Battery_MinVoltage(char* _v1,char* _v2,char* _v3,char* _v4);
static char* Battery_MaxVoltage(char* _v1,char* _v2,char* _v3,char* _v4);
static char* Battery_GetAverage(char* _v1,char* _v2,char* _v3,char* _v4);
static char* Battery_TotalVoltage(char* _v1,char* _v2,char* _v3,char* _v4);
static unsigned int Battery_Reconstruct(int input)
{
	unsigned int voltage;
	unsigned int voltageHigh,voltageLow;

	voltageHigh=input&0xff;
	voltageLow=input&0xff00;
	voltageLow>>=8;
	voltageHigh<<=8;
	voltage=voltageHigh+voltageLow;
	return voltage;
}

static double Battery_GetVoltage(unsigned int input)
{
	return  0.001221*input;
}

static char* Battery_GetBattery(char* _v1,char* _v2,char* _v3,char* _v4)
{
	int boardID,batteryID;
	double voltage;
	boardID=atoi(_v1);
	batteryID=atoi(_v2);

	voltage=Battery_Reconstruct(g_batteryVoltageArray[boardID][batteryID]);
	voltage*=0.001221;
	printf("V%d[%d]=%f;\n",boardID,batteryID,voltage);
	return 0;
}
static char* Battery_GetBatteryAll(char* _v1,char* _v2,char* _v3,char* _v4)
{
	int i;
	unsigned int voltageHigh,voltageLow;
	double voltage;

	printf("\n");
	for(i=0;i<16;i++)
	{
		voltage=g_batteryVoltageArray[0][i];
		voltage*=0.001221;
		printf("V0[%d]=%f; ",i,voltage);
		if((i+1)%4==0)
			printf("\n");
	}
	for(i=0;i<8;i++)
	{
		voltage=g_batteryVoltageArray[1][i];
		voltage*=0.001221;
		printf("V1[%d]=%f; ",i,voltage);
		if((i+1)%4==0)
			printf("\n");
	}

	return 0;
}
static char* Battery_ReleaseAll(char* _v1,char* _v2,char* _v3,char* _v4)
{
	unsigned int averageVolage;
	unsigned int flag=0;
	unsigned char releaseFlag[5]={0,0,0,0,0};
	unsigned int voltag;
	int batteryIndex=0;
	int boardIndex=0;
again:
	averageVolage=Battery_Average();

#ifdef QQ_TEST
	flag=0;
	for(batteryIndex=0;batteryIndex<16;batteryIndex++)
	{
		voltag=g_batteryVoltageArray[0][batteryIndex];
		if(voltag>averageVolage+1)
		{
			flag|=1<<batteryIndex;
		}
	}
	Battery_ReleaseUintToChars(flag,releaseFlag);
	CAN_ChangeSendID(0x101);
	BMS_CAN_SendToSample_1(WM_START_EQUAL,releaseFlag);
	while(CAN_Task_GetStatus()!=0x1ee2);
	printf("Send Release Flag:%x to Board 1\n",flag);

	flag=0;
	for(batteryIndex=0;batteryIndex<8;batteryIndex++)
	{
		voltag=g_batteryVoltageArray[1][batteryIndex];
		if(voltag>averageVolage+1)
		{
			flag|=1<<batteryIndex;
		}
	}
	Battery_ReleaseUintToChars(flag,releaseFlag);
	CAN_ChangeSendID(0x101);
	BMS_CAN_SendToSample_2(WM_START_EQUAL,releaseFlag);
	while(CAN_Task_GetStatus()!=0x1ee2);
	printf("Send Release Flag:%x to Board 2\n",flag);
#else
	for(boardIndex=0;boardIndex<7;boardIndex++)
	{
		for(batteryIndex=0;batteryIndex<16;batteryIndex++)
		{

		}
	}
#endif
	goto again;
	return 0;
}
static char* Battery_Release(char* _v1,char* _v2,char* _v3,char* _v4)
{
	int boardID,batteryID;
	int i;
	unsigned char releaseFlag[5]={0,0,0,0,0};

	boardID=atoi(_v1);
	batteryID=atoi(_v2);

	if(batteryID<=7)
		releaseFlag[0]=1<<(batteryID);
	else
		releaseFlag[1]=1<<(batteryID-8);

	for(i=0;i<800;i++)
	{
		RTC_vRTCRun();
		Timer_SetTimer(0);
		switch(boardID)
		{
		case 0:
			BMS_CAN_SendToSample_1(WM_START_EQUAL,releaseFlag);
			break;
		case 1:
			BMS_CAN_SendToSample_2(WM_START_EQUAL,releaseFlag);
			break;
		}
		while(Timer_GetCounter()<6250);
	}
	RTC_vRTCStop();
	return 0;
}
static void Battery_ReleaseUintToChars(unsigned int in,unsigned char* out)
{
	out[0]=in&0xff;
	out[1]=in>>8;
}
static char* Battery_MinVoltage(char* _v1,char* _v2,char* _v3,char* _v4)
{
	int index;
	unsigned int voltage;
	unsigned int minVoltage;
	int board=0;
	int battery=0;
	double f_voltage;

	minVoltage=g_batteryVoltageArray[0][0];

	for(index=0;index<16;index++)
	{
		voltage=g_batteryVoltageArray[0][index];
		if(voltage<minVoltage)
		{
			minVoltage=voltage;
			board=0;
			battery=index;
		}
	}
	for(index=0;index<8;index++)
	{
		voltage=g_batteryVoltageArray[1][index];
		if(voltage<minVoltage)
		{
			minVoltage=voltage;
			board=1;
			battery=index;
		}
	}

	f_voltage=BATTER_GETVOLTAGE(minVoltage);

	printf("Min Battery Is Board:%d,Battery%d,%f(V)\n",
			board,battery,f_voltage);
	return 0;
}
static char* Battery_MaxVoltage(char* _v1,char* _v2,char* _v3,char* _v4)
{
	int index;
	unsigned int voltage;
	unsigned int maxVoltage;
	int board=0;
	int battery=0;
	double f_voltage;

	maxVoltage=g_batteryVoltageArray[0][0];

	for(index=0;index<16;index++)
	{
		voltage=g_batteryVoltageArray[0][index];
		if(voltage>maxVoltage)
		{
			maxVoltage=voltage;
			board=0;
			battery=index;
		}
	}
	for(index=0;index<8;index++)
	{
		voltage=g_batteryVoltageArray[1][index];
		if(voltage>maxVoltage)
		{
			maxVoltage=voltage;
			board=1;
			battery=index;
		}
	}

	f_voltage=BATTER_GETVOLTAGE(maxVoltage);

	printf("Max Battery Is Board:%d,Battery%d,%f(V)\n",
			board,battery,f_voltage);
	return 0;
}
static char* Battery_GetAverage(char* _v1,char* _v2,char* _v3,char* _v4)
{
	unsigned long sumVoltage;
	unsigned int returnValue;
	double f_voltage;
	sumVoltage=Battery_Sum();

#ifdef QQ_TEST
	returnValue=sumVoltage/24;
#else
	returnValue=sumVoltage/112;
#endif
	f_voltage=BATTER_GETVOLTAGE(returnValue);

	printf("Average volage:%f\n",f_voltage);
	return 0;
}
static char* Battery_TotalVoltage(char* _v1,char* _v2,char* _v3,char* _v4)
{
	unsigned long sumVoltage;
	unsigned int returnValue;
	double f_voltage;
	sumVoltage=Battery_Sum();
	f_voltage=BATTER_GETVOLTAGE(returnValue);
	printf("Total volage:%f\n",f_voltage);
	return 0;
}
unsigned int Battery_Average()
{
	unsigned long sumVoltage;
	unsigned int returnValue;
	sumVoltage=Battery_Sum();

#ifdef QQ_TEST
	returnValue=sumVoltage/24;
#else
	returnValue=sumVoltage/112;
#endif
	return returnValue;
}

unsigned long Battery_Sum()
{
	int boardIndex;
	int batteryIndex;
	unsigned long sumVoltage=0;

#ifdef QQ_TEST
	for(batteryIndex=0;batteryIndex<16;batteryIndex++)
	{
		sumVoltage+=g_batteryVoltageArray[0][batteryIndex];
	}
	for(batteryIndex=0;batteryIndex<8;batteryIndex++)
	{
		sumVoltage+=g_batteryVoltageArray[1][batteryIndex];
	}
#else

	for(boardIndex=0;boardIndex<7;boardIndex++)
	{
		for(batteryIndex=0;batteryIndex<16;batteryIndex++)
		{
			sumVoltage+=g_batteryVoltageArrayEx[boardIndex][batteryIndex];
		}
	}

#endif
	return sumVoltage;
}
void Battery_Init()
{
	CMD_LoadFunction("BatteryVoltage",Battery_GetBattery);
	CMD_LoadFunction("BatteryGetBatteryAll",Battery_GetBatteryAll);
	CMD_LoadFunction("BatteryRelease",Battery_Release);
	CMD_LoadFunction("BatteryReleaseAll",Battery_ReleaseAll);
	CMD_LoadFunction("BatteryTotal",Battery_TotalVoltage);
	CMD_LoadFunction("BatteryMax",Battery_MaxVoltage);
	CMD_LoadFunction("BatteryMin",Battery_MinVoltage);
	CMD_LoadFunction("BatteryAverage",Battery_GetAverage);
}
