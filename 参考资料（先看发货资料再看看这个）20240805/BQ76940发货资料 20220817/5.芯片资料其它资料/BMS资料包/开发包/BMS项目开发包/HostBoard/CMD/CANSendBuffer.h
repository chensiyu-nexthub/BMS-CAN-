/*
 * CANSendBuffer.h
 *
 *  Created on: May 22, 2010
 *      Author: Administrator
 */

#ifndef CANSENDBUFFER_H_
#define CANSENDBUFFER_H_
void CANSendBuffer_Init();
void CAN_ChangeSendID(unsigned int sendID);
//void CAN_Send(unsigned char* sendBuffer);
void CANSendBuffer_Send();
void CANSendBuffer_Complete();
int CANSendBuffer_IsSendAble();
int CANSendBuffer_IsEmpty();
int CANSendBuffer_IsFull();
int CANSendBuffer_AddBuffer(unsigned char* input);
char* CANSendBuffer_GetBuffer();

#endif /* CANSENDBUFFER_H_ */
