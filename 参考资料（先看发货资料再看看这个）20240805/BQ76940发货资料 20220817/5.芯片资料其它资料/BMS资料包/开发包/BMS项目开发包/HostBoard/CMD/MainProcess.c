/*
 * MainProcess.c
 *
 *  Created on: Jun 22, 2010
 *      Author: Administrator
 */

#include "MainProcess.h"
#include "Timer_Manager.h"
#include "BMS_CAN.h"
#include "CAN_Task.h"
#include "CAN.h"`
#include "Battery_Task.h"
#include "../include/rtc.h"
#include "Event.h"
#include "Error.h"
#include "QuantityChange.h"

extern unsigned long g_timeCounter;
extern unsigned int g_timeLoopCounter;
extern long g_quanlity;
extern long g_current_value;
extern int g_offset_1,g_offset_2;
long g_oldQuanlity;

//#define __A
extern unsigned int g_current_1,g_current_2;

#define PROCESS_ID_BATTERY_VOLTAGE 	1
#define PROCESS_ID_BATTERY_QUANTITY	2
#define PROCESS_ID_SEND_TIME		3
#define PROCESS_ID_BATTERY_EQUAL	4
#define PROCESS_ID_SEND_CURRENT		5
#define PROCESS_ID_SEND_ERROR		6
#define PROCESS_ID_QuantityChange	7

#define PROCESS_STATUS_NOT_ACTIVITE	0
#define PROCESS_STATUS_HOLD			1
#define RPOCESS_STATUS_START		2
#define RPOCESS_STATUS_DEAD			3

#define PROCESS_MAX 				12

unsigned char g_processStatus[PROCESS_MAX];
ProcessType g_processFunction[PROCESS_MAX];
unsigned int g_processTime[PROCESS_MAX];

extern unsigned char g_hostShareDataBuffer[64];
extern unsigned char g_hostShareDataTemp[64];

void MainProcess_Init()
{
	int i;
	Event_Progress(WM_EEPROM_TO_SHARE_MEMERY);
	for(i=0;i<PROCESS_MAX;i++)
		g_processStatus[i]=PROCESS_STATUS_NOT_ACTIVITE;

	MainProcess_LoadProcess(PROCESS_ID_SEND_CURRENT,100,MainProcess_SendCurrent);
	MainProcess_LoadProcess(PROCESS_ID_SEND_ERROR,150,Error_vSendError);
	MainProcess_LoadProcess(PROCESS_ID_BATTERY_EQUAL,200,MainProcess_EqualVoltage);
	MainProcess_LoadProcess(PROCESS_ID_QuantityChange,250,QuantityChange_vProcess);
	MainProcess_LoadProcess(PROCESS_ID_SEND_TIME,300,MainProcess_SendTime);
	MainProcess_LoadProcess(PROCESS_ID_BATTERY_QUANTITY,400,MainProcess_SendQuanlity);
}
void MainProcess_EqualVoltage()
{
	Battery_Task();
}
void MainProcess_SendTime()
{
	unsigned char CANBuffer[8]={0,0,0,0,0,0,0,0};

	CANBuffer[0]=g_timeCounter&0xff;
	CANBuffer[1]=(g_timeCounter&0xff00)>>8;
	CANBuffer[2]=(g_timeCounter&0xff0000)>>16;
	CANBuffer[3]=(g_timeCounter&0xff000000)>>24;
	BMS_CANSend(BMS_ID_SYSTEM_START_TIME,CANBuffer);
}

void MainProcess_SendCurrent()
{
	unsigned char CANBuffer[8]={0,0,0,0,0,0,0,0};
//PSW_IEN=0;
	CANBuffer[0]=g_current_1&0xff;
	CANBuffer[1]=g_current_1>>8;
	CANBuffer[2]=g_current_2&0xff;
	CANBuffer[3]=g_current_2>>8;

	CANBuffer[4]=g_quanlity&0xff;
	CANBuffer[5]=(g_quanlity&0xff00)>>8;
	CANBuffer[6]=(g_quanlity&0xff0000)>>16;
	CANBuffer[7]=(g_quanlity&0xff000000)>>24;
//PSW_IEN=1;
	BMS_CANSend(BMS_ID_CURRENT,CANBuffer);
}
void MainProcess_SendQuanlity()
{
	unsigned char CANBuffer[8]={0,0,0,0,0,0,0,0};
	long quanlity=0;
	long oldQuanlity,oldQuanlity_1,oldQuanlity_2,oldQuanlity_3,oldQuanlity_4;

	//quanlity=*((long*)(&g_hostShareDataBuffer[1]));

	oldQuanlity_1=g_hostShareDataBuffer[1];
	oldQuanlity_2=g_hostShareDataBuffer[2];
	oldQuanlity_2<<=8;
	oldQuanlity_3=g_hostShareDataBuffer[3];
	oldQuanlity_3<<=16;
	oldQuanlity_4=g_hostShareDataBuffer[4];
	oldQuanlity_4<<=24;

	oldQuanlity=oldQuanlity_1+oldQuanlity_2+oldQuanlity_3+oldQuanlity_4;

	g_oldQuanlity=oldQuanlity;
	quanlity=oldQuanlity+(g_quanlity*0.00512);

	//quanlity+=g_quanlity*0.00512;//mAh

	//g_hostShareDataBuffer[1]=quanlity&0xff;
	//g_hostShareDataBuffer[2]=(quanlity&0xff00)>>8;
	//g_hostShareDataBuffer[3]=(quanlity&0xff0000)>>16;
	//g_hostShareDataBuffer[4]=(quanlity&0xff000000)>>24;


//PSW_IEN=0;
	CANBuffer[0]=quanlity&0xff;
	CANBuffer[1]=(quanlity&0xff00)>>8;
	CANBuffer[2]=(quanlity&0xff0000)>>16;
	CANBuffer[3]=(quanlity&0xff000000)>>24;
//PSW_IEN=1;
	BMS_CANSend(BMS_ID_QUANLITY,CANBuffer);
}
void MainProcess_LoadProcess(unsigned char processID,unsigned int time,ProcessType process)
{
	g_processStatus[processID]=RPOCESS_STATUS_START;
	g_processFunction[processID]=process;
	g_processTime[processID]=time;
}
void MainProcess_Restart()
{
	int i;
	unsigned char status;
	for(i=0;i<PROCESS_MAX;i++)
	{
		status=g_processStatus[i];
		switch(status)
		{
			case RPOCESS_STATUS_DEAD:
				if(g_processTime[i]<=g_timeLoopCounter)
				g_processFunction[i]();
				g_processStatus[i]=RPOCESS_STATUS_START;
				break;
		}
	}
}
void MainProcess()
{
	unsigned char status;
	int i;
	MainProcess_Init();
	RTC_vRTCRun();
	while(1)
	{
		for(i=0;i<PROCESS_MAX;i++)
		{
			status=g_processStatus[i];
			switch(status)
			{
				case RPOCESS_STATUS_START:
					if(g_processTime[i]<=g_timeLoopCounter)
					{
						g_processFunction[i]();
						g_processStatus[i]=RPOCESS_STATUS_DEAD;
					}
					break;
			}
		}
	}
}
