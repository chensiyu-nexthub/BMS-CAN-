/*
 * BMS_Flag.c
 *
 *  Created on: Jul 12, 2010
 *      Author: Administrator
 */

#include "BMS_Flag.h"
#include "Event.h"

unsigned char g_pubStatusFlag[1600];
extern unsigned int g_batteryStateArray[7][16];
extern unsigned int g_batteryVoltageArray[7][16];
extern unsigned int g_batteryTemptureArray[7][16];
extern unsigned int g_batteryEmptyArray[7];
extern unsigned char g_hostShareDataBuffer[64];
extern unsigned int g_uiAverageVolage;
extern unsigned int g_uiHighestVoltageID;
extern unsigned int g_uiHighestVoltage;
extern unsigned long g_ulTotalVoltage;
extern unsigned int g_uiLowestVoltageID;
extern unsigned int g_uiLowestVoltage;
extern unsigned char g_CAN_ReceiveBuffer[8];

void BMS_Flag_vSetFlag(unsigned int bit)
{
	unsigned int uiGroup;
	unsigned char ubLeft;
	unsigned char ubValue;
	unsigned char ubIsOldSet;

	if(BMS_Flag_ubIsSet(bit)==0)
	{
		BMS_Flag_vOnSet(bit);
	}

	uiGroup=bit/8;
	ubLeft=bit%8;

	ubValue=1;
	ubValue<<=ubLeft;

	g_pubStatusFlag[uiGroup]|=ubValue;
}
void BMS_Flag_vClearFlag(unsigned int bit)
{
	unsigned int uiGroup;
	unsigned char ubLeft;
	unsigned char ubValue;
	if(BMS_Flag_ubIsSet(bit)==1)
	{
		BMS_Flag_vOnClear(bit);
	}
	uiGroup=bit/8;
	ubLeft=bit%8;

	ubValue=1;
	ubValue<<=ubLeft;
	ubValue=~ubValue;
	g_pubStatusFlag[uiGroup]&=ubValue;
}
unsigned char BMS_Flag_ubIsSet(unsigned int bit)
{
	unsigned int uiGroup;
	unsigned char ubLeft;
	unsigned char ubValue;
	unsigned char ubReturn=0;


	uiGroup=bit/8;
	ubLeft=bit%8;

	ubValue=1;
	ubValue<<=ubLeft;
	if(g_pubStatusFlag[uiGroup]&ubValue)
	{
		ubReturn=1;
	}

	return ubReturn;
}
void BMS_Flag_Process()
{
	unsigned int uiLowestVoltageLimit;
	unsigned int uiHighestVoltageLimit;

	uiLowestVoltageLimit=ShareMemery_uiGetShareMemery(
			SHARE_MEMERY_ADDREESS_uiLOWEST_LIMIT_VOLTAGE
				);

	uiHighestVoltageLimit=ShareMemery_uiGetShareMemery(
			SHARE_MEMERY_ADDREESS_uiHIGHEST_LIMIT_VOLTAGE
				);

	if(g_uiLowestVoltage<uiLowestVoltageLimit)
		BMS_Flag_vSetFlag(BMS_FLAG_LOWEST_LIMIT_VOLTAGE);
	if(uiLowestVoltageLimit+10<g_uiLowestVoltage)
		BMS_Flag_vClearFlag(BMS_FLAG_LOWEST_LIMIT_VOLTAGE);

	if(uiHighestVoltageLimit<g_uiHighestVoltage)
		BMS_Flag_vSetFlag(BMS_FLAG_HIGHEST_LIMIT_VOLTAGE);
	if(g_uiHighestVoltage<uiHighestVoltageLimit+10)
		BMS_Flag_vClearFlag(BMS_FLAG_HIGHEST_LIMIT_VOLTAGE);

}

static void BMS_Flag_vHighestVoltageToEEPROM()
{

}

static void BMS_Flag_vLowestVoltageToEEPROM()
{

}

#define FLAG_MAP(__event,__process) case __event: __process();break;

void BMS_Flag_vOnSet(unsigned int bit)
{
	switch(bit)
	{
		FLAG_MAP(BMS_FLAG_HIGHEST_LIMIT_VOLTAGE,BMS_Flag_vHighestVoltageToEEPROM);
		FLAG_MAP(BMS_FLAG_LOWEST_LIMIT_VOLTAGE,BMS_Flag_vLowestVoltageToEEPROM);
	}
}



void BMS_Flag_vOnClear(unsigned int bit)
{
	switch(bit)
	{
	case BMS_FLAG_HIGHEST_LIMIT_VOLTAGE:
		break;
	case BMS_FLAG_LOWEST_LIMIT_VOLTAGE:
		break;
	}
}
