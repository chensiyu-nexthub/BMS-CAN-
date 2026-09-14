/*
 * CAN_Task.c
 *
 *  Created on: May 6, 2010
 *      Author: Administrator
 */
#include "CAN_Task.h"
#include "Battery.h"
#include "event.h"
#include "Error.h"
#include "XC22xxMREGS.h"
#include "CAN.h"
#include "CMD_CALL.h"
#include "stdio.h"
#include "string.h"

/****************When CAN receive object from 0x501*******************************************/
#ifndef BOARD_DEFINE
#define BOARD_DEFINE 1
#define HOST_BOARD 0x10
#define SAMPLE_BOARD1 0x11
#define SAMPLE_BOARD2 0x12
#define DEVELOP_BOARD 0x13
#endif
unsigned char g_CAN_ReceiveBuffer[8];
static char* CAN_Task_CANStatus(char* _1,char* _2,char* _3,char* _4);
static char* CAN_Task_CANSend(char* pVariours_1,char* pVariours_2,char* pVariours_3,char* pVariours_4);

static char* CAN_Task_CANSend(char* pVariours_1,char* pVariours_2,char* pVariours_3,char* pVariours_4)
{
	int ID=HexToInt(pVariours_1);
	unsigned int messageStatus;
	messageStatus=CAN_MOSTAT1H;
	printf("Status High:%x\n",CAN_MOSTAT1H);
	printf("Status Low:%x\n",CAN_MOSTAT1L);
	CAN_ChangeSendID(ID);
	printf("Status High:%x\n",CAN_MOSTAT1H);
	printf("Status Low:%x\n",CAN_MOSTAT1L);
	CAN_Send(pVariours_2);
	printf("Status High:%x\n",CAN_MOSTAT1H);
	printf("Status Low:%x\n",CAN_MOSTAT1L);
	return 0;
}
static char* CAN_Task_CANStatus(char* _1,char* _2,char* _3,char* _4)
{
	int i=0;
	for(i=0;i<8;i++)
		printf("%02x",g_CAN_ReceiveBuffer[i]);
	printf("\nStatus High:%x\n",CAN_MOSTAT1H);
	printf("Status Low:%x\n",CAN_MOSTAT1L);
	return 0;
}
unsigned int CAN_Task_GetStatus()
{
	return CAN_MOSTAT1L;
}
void CAN_Task_Init()
{
	CMD_LoadFunction("CANSend",CAN_Task_CANSend);
	CMD_LoadFunction("CANStatus",CAN_Task_CANStatus);
}

void CAN_Task_DoOrder(unsigned char volatile __far* dataBuffer)
{
	unsigned char sender=dataBuffer[0];
	unsigned char volatile __far* parameters=&dataBuffer[3];
	unsigned char low=dataBuffer[1];
	unsigned char high=dataBuffer[2];
	unsigned int order=low+(high<<8);
	int i=0;

	//while(1);
	for(i=0;i<8;i++)
	{
		g_CAN_ReceiveBuffer[i]=dataBuffer[i];
	}

	if(sender!=HOST_BOARD)
		return;
	if(Event_IsFull()==1)
	{
		Error_AddError(ERROR_EVENT_FULL);
	}
	else
	{
#ifdef EVENT_ADDEVENT
		Event_AddEvent(order);
#else
		Event_Progress(order);
#endif
	}
}
