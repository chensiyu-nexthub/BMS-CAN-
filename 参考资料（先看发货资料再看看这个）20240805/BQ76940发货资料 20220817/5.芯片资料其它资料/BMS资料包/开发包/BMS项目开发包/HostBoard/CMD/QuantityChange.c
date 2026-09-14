/*
 * QuantityChange.c
 *
 *  Created on: Jul 9, 2010
 *      Author: Administrator
 */

#include "QuantityChange.h"
#include "BMS_CAN.h"
#include "ShareMemery.h"

long g_lOldQuantity=0;
extern long g_quanlity;//A.s
unsigned int g_uiOldAverageVoltage;
unsigned int g_uiOldLowestVoltage;
unsigned int g_uiOldHighestVoltage;

int g_iAverageVoltageChange;
int g_iLowestVoltageChange;
int g_iHighestVoltageChange;

extern unsigned int g_uiAverageVolage;
extern unsigned int g_uiLowestVoltage;
extern unsigned int g_uiHighestVoltage;
extern unsigned long g_ulTotalVoltage;
extern unsigned int g_uiLowestVoltageID;
extern unsigned int g_uiHighestVoltageID;

unsigned char g_pubQuantityFlag[160];
static unsigned char s_ubIsQuantityChangeInit=0;

static void QuantityChange_vInit()
{
	int i;
	long uiOffsetQuantity;
	g_uiOldAverageVoltage=g_uiAverageVolage;
	g_uiOldLowestVoltage=g_uiLowestVoltage;
	g_uiOldHighestVoltage=g_uiHighestVoltage;
	g_lOldQuantity=g_quanlity;
	s_ubIsQuantityChangeInit=1;
}
void QuantityChange_vProcess()
{
	long lOffsetQuantity;
	int iOffsetAverageVolatge;
	int iOffsetLowestVoltage;
	int iOffsetHighestVoltage;
	int iOffsetQuantity;
	unsigned char ubQuantityPerSecond;
	int iQuantityDevide;

	static unsigned int uiCounter=0;

	unsigned char pubCANSendBuffer[8];

	ubQuantityPerSecond=ShareMemery_ubGetShareMemery(
				SHARE_MEMERY_ADDREESS_ubQUANTITY_PER_SECOND
				);

	if(s_ubIsQuantityChangeInit==0)
	{
		QuantityChange_vInit();
	}
	if(uiCounter++>=ubQuantityPerSecond*120)//This will be processed every 100s.
	{

		iQuantityDevide=ShareMemery_ubGetShareMemery(
					SHARE_MEMERY_ADDREESS_ubQUANTITY_DEVIDE
					);

		uiCounter=0;
		lOffsetQuantity=g_lOldQuantity-g_quanlity;
		iOffsetAverageVolatge=g_uiAverageVolage-g_uiOldAverageVoltage;
		iOffsetLowestVoltage=g_uiLowestVoltage-g_uiOldLowestVoltage;
		iOffsetHighestVoltage=g_uiHighestVoltage-g_uiOldHighestVoltage;
		//iOffsetQuantity:A.h
		iOffsetQuantity=lOffsetQuantity*0.00000512/iQuantityDevide;

		//iOffsetQuantity/=3600000;

		if(iOffsetQuantity!=0)
		{
			g_iAverageVoltageChange=iOffsetAverageVolatge/iOffsetQuantity;
			g_iLowestVoltageChange=iOffsetLowestVoltage/iOffsetQuantity;
			g_iHighestVoltageChange=iOffsetHighestVoltage/iOffsetQuantity;
		}
		else
		{
			g_iAverageVoltageChange=0;
			g_iLowestVoltageChange=0;
			g_iHighestVoltageChange=0;
		}

		g_uiOldAverageVoltage=g_uiAverageVolage;
		g_uiOldLowestVoltage=g_uiLowestVoltage;
		g_uiOldHighestVoltage=g_uiHighestVoltage;
		g_lOldQuantity=g_quanlity;

		pubCANSendBuffer[0]=g_iAverageVoltageChange&0xff;
		pubCANSendBuffer[1]=g_iAverageVoltageChange>>8;

		pubCANSendBuffer[2]=g_iLowestVoltageChange&0xff;
		pubCANSendBuffer[3]=g_iLowestVoltageChange>>8;

		pubCANSendBuffer[4]=g_iHighestVoltageChange&0xff;
		pubCANSendBuffer[5]=g_iHighestVoltageChange>>8;

		pubCANSendBuffer[6]=iOffsetQuantity&0xff;
		pubCANSendBuffer[7]=iOffsetQuantity>>8;

		BMS_CANSend(BMS_ID_HOST_QUANTITY_CHANGE,pubCANSendBuffer);

	}

}
