/*
 * ShareMemery.c
 *
 *  Created on: Jul 10, 2010
 *      Author: Administrator
 */

#include "Event.h"
#include "ShareMemery.h"

unsigned char g_hostShareDataBuffer[64]={20,0,0,0,0,10,10};

void ShareMemery_vInit()
{
	ShareMemery_vSetShareMemeryUb(
			SHARE_MEMERY_ADDREESS_ubRELEASE_OFFSET,
			20);

	ShareMemery_vSetShareMemeryUl(
			SHARE_MEMERY_ADDREESS_ulQUANTITIY,
			0);

	ShareMemery_vSetShareMemeryUb(
			SHARE_MEMERY_ADDREESS_ubINCREASE_LIMIT,
			10);

	ShareMemery_vSetShareMemeryUb(
			SHARE_MEMERY_ADDREESS_ubDECREASE_LIMIT,
			10);

	ShareMemery_vSetShareMemeryUb(
			SHARE_MEMERY_ADDREESS_ubQUANTITY_PER_SECOND,
			3);

	ShareMemery_vSetShareMemeryUb(
			SHARE_MEMERY_ADDREESS_ubQUANTITY_DEVIDE,
			1);
	ShareMemery_vSetShareMemeryUi(
			SHARE_MEMERY_ADDREESS_uiLOWEST_VOLTAGE,
			2458);

	ShareMemery_vSetShareMemeryUi(
			SHARE_MEMERY_ADDREESS_uiLOWEST_VOLTAGE,
			2869);
}

unsigned char ShareMemery_ubGetShareMemery(unsigned int index)
{
	unsigned char ubReturn;
	ubReturn=g_hostShareDataBuffer[index];
	return ubReturn;
}

unsigned int ShareMemery_uiGetShareMemery(unsigned int index)
{
	unsigned int uiReturn;
	uiReturn=g_hostShareDataBuffer[index+1];
	uiReturn<<=8;
	uiReturn+=g_hostShareDataBuffer[index];
	return uiReturn;
}

unsigned long ShareMemery_ulGetShareMemery(unsigned int index)
{
	unsigned long ulReturn=0;
	unsigned int uiHighIndex,uiLowIndex;
	unsigned int i;

	uiHighIndex=index+3;
	uiLowIndex=index;

	for(i=uiHighIndex;i>=uiLowIndex;i--)
	{
		ulReturn<<=8;
		ulReturn+=g_hostShareDataBuffer[i];
	}

	return ulReturn;
}
long long ShareMemery_llGetShareMemery(unsigned int index)
{
	long long llReturn=0;
	unsigned int uiHighIndex,uiLowIndex;
	unsigned int i;
	uiHighIndex=index+7;
	uiLowIndex=index;

	for(i=uiHighIndex;i>=uiLowIndex;i--)
	{
		llReturn<<=8;
		llReturn+=g_hostShareDataBuffer[i];
	}

	return llReturn;
}

void ShareMemery_vSetShareMemeryUb(unsigned int index,unsigned char value)
{

	g_hostShareDataBuffer[index]=value;

}
void ShareMemery_vSetShareMemeryUi(unsigned int index,unsigned int value)
{

	g_hostShareDataBuffer[index]=value&0xff;
	g_hostShareDataBuffer[index+1]=value>>8;

}
void ShareMemery_vSetShareMemeryUl(unsigned int index,unsigned long value)
{
	unsigned int uiHighIndex,uiLowIndex;
	unsigned int i;
	unsigned long ulValue=value;
	uiHighIndex=index+3;
	uiLowIndex=index;

	for(i=uiLowIndex;i<=uiHighIndex;i++)
	{
		g_hostShareDataBuffer[i]=ulValue&0xff;
		ulValue>>=8;
	}
}
void ShareMemery_vSetShareMemeryLl(unsigned int index,long long value)
{

	unsigned int uiHighIndex,uiLowIndex;
	unsigned int i;
	long long llValue=value;
	uiHighIndex=index+7;
	uiLowIndex=index;

	for(i=uiLowIndex;i<=uiHighIndex;i++)
	{
		g_hostShareDataBuffer[i]=llValue&0xff;
		llValue>>=8;
	}

}
