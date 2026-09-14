/*
 * CANSendBuffer.c
 *
 *  Created on: May 22, 2010
 *      Author: Administrator
 */
#ifndef IN_LIB_CAN
#define IN_LIB_CAN
#endif
#include "CANSendBuffer.h"

#ifdef EXTERN_LIB_CAN
#include "../include/CAN.h"
#endif
#ifdef IN_LIB_CAN
#include "CAN.h"
#endif
#include  "XC22xxMREGS.H"

#include "string.h"
#include "stdio.h"
#include "CMD_Call.h"
#define CANBUFFER_SIZE 10
static unsigned char m_buffer[CANBUFFER_SIZE][8];
static int m_bufferIndex=-1;
static int m_isSendComplete=1;
char* CANAddBuffer(char* pVariours_1,char* pVariours_2,char* pVariours_3,char* pVariours_4)
{
	static char bufferCounter[20];
	CANSendBuffer_AddBuffer(pVariours_1);
	sprintf(bufferCounter,"Counter:%d\n",m_bufferIndex+1);
	return bufferCounter;
}
char* CANSendStart(char* pVariours_1,char* pVariours_2,char* pVariours_3,char* pVariours_4)
{
	char* buffer;
	int i;
	static char returnValue[16]="Send complete!\n";
	while(CANSendBuffer_IsEmpty()!=1)
	{
		buffer=CANSendBuffer_GetBuffer();
		printf("Wait for CAN Send.\n");
		while(CANSendBuffer_IsSendAble()!=1){};
		for(i=0;i<8;i++)
			CAN_HWOBJ[1].ubData[i]=buffer[i];
		CAN_vTransmit(1);
	}
	return returnValue;
}
void CANSendBuffer_Init()
{
	CMD_LoadFunction("CANAdd",CANAddBuffer);
	CMD_LoadFunction("CANSendStart",CANSendStart);
}
/*void CAN_Send(unsigned char* sendBuffer)
{
	int i;
	for(i=0;i<8;i++)
		CAN_HWOBJ[1].ubData[i]=sendBuffer[i];
    CAN_vTransmit(1);
}*/
/*void CAN_ChangeSendID(unsigned int sendID)
{
    CAN_MOAR1H = 0x4000+(sendID<<2);
}*/
int CANSendBuffer_IsSendAble()
{
	return m_isSendComplete;
}
void CANSendBuffer_Send()
{
	char* buffer;
	int i;
	while(CANSendBuffer_IsEmpty()!=1)
	{
		buffer=CANSendBuffer_GetBuffer();
		while(CANSendBuffer_IsSendAble()!=1){};
		for(i=0;i<8;i++)
			CAN_HWOBJ[1].ubData[i]=buffer[i];
		CAN_vTransmit(1);
	}
}
void CANSendBuffer_Complete()
{
	m_isSendComplete=1;
}
int CANSendBuffer_IsEmpty()
{
	if(m_bufferIndex==-1)
		return 1;
	else
		return 0;
}
int CANSendBuffer_IsFull()
{
	if(m_bufferIndex==CANBUFFER_SIZE-1)
		return 1;
	return 0;
}
int CANSendBuffer_AddBuffer(unsigned char* input)
{
	int i=0;
	m_bufferIndex++;
	if(m_bufferIndex==CANBUFFER_SIZE)
		return 0;
	for(i=0;i<8;i++)
	{
		m_buffer[m_bufferIndex][i]=input[i];
	}
	return 1;
}
char* CANSendBuffer_GetBuffer()
{
	char *returnValue;
	if(CANSendBuffer_IsEmpty())
		return 0;

	returnValue= m_buffer[m_bufferIndex];
	m_bufferIndex--;
	return returnValue;
}

