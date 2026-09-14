/*
 * BMS_CAN.c
 *
 *  Created on: May 31, 2010
 *      Author: Administrator
 */

#include "BMS_CAN.h"
#include "Event.h"
#include "CAN_TASK.h"
#include "CAN.h"

void BMS_CANSend(unsigned int ID,unsigned char *buffer)
{
	CAN_ChangeSendID(ID);
	CAN_Send(buffer);
	while(CAN_Task_GetStatus()!=0x1ee2);
}

void BMS_CAN_SendToHostBoard(unsigned int CANEvent,unsigned char* dataBuffer)
{

}
void BMS_CAN_SendToDevalopBoard(unsigned int CANEvent,unsigned char* dataBuffer)
{

}
void BMS_CAN_SendToSample_1(unsigned int CANEvent,unsigned char* dataBuffer)
{
	unsigned int temp;
	unsigned char data[8]={SAMPLE_BOARD1,0};
	unsigned int index;
	data[1]=CANEvent&0xff;
	temp=CANEvent&0xff00;
	temp>>=8;
	data[2]=temp;
	for(index=0;index<5;index++)
		data[index+3]=dataBuffer[index];

	CAN_Send(data);
}
void BMS_CAN_SendToSample_2(unsigned int CANEvent,unsigned char* dataBuffer)
{
	unsigned int temp;
	unsigned char data[8]={SAMPLE_BOARD2,0};
	unsigned int index;
	data[1]=CANEvent&0xff;
	temp=CANEvent&0xff00;
	temp>>=8;
	data[2]=temp;
	for(index=0;index<5;index++)
		data[index+3]=dataBuffer[index];

	CAN_Send(data);
}
void BMS_CAN_SendToSample_3(unsigned int CANEvent,unsigned char* dataBuffer)
{

}
void BMS_CAN_SendToSample_4(unsigned int CANEvent,unsigned char* dataBuffer)
{

}
void BMS_CAN_SendToSample_5(unsigned int CANEvent,unsigned char* dataBuffer)
{

}
void BMS_CAN_SendToSample_6(unsigned int CANEvent,unsigned char* dataBuffer)
{

}
void BMS_CAN_SendToSample_7(unsigned int CANEvent,unsigned char* dataBuffer)
{

}
void BMS_CAN_SendToSample_8(unsigned int CANEvent,unsigned char* dataBuffer)
{

}

