#include "Event.h"
#include "Battery.h"
#include "XC22xxMREGS.h"
#include "CAN.h"
#include "../include/calendar.h"
#include "stdio.h"
#include "string.h"
#include "WAVE.h"
#include "../include/EEPROM.h"
#include "CAN_Task.h"
#include "BMS_CAN.h"
#include "Error.h"
#include "EEPROM_Task.h"
#include "ShareMemery.h"

extern unsigned int g_batteryStateArray[7][16];
extern unsigned int g_batteryVoltageArray[7][16];
extern unsigned int g_batteryTemptureArray[7][16];
extern unsigned int g_batteryEmptyArray[7];

unsigned char g_Flag_Board_1_End;
unsigned char g_Flag_Board_2_End;
unsigned char g_Flag_Board_3_End;
unsigned char g_Flag_Board_4_End;
unsigned char g_Flag_Board_5_End;
unsigned char g_Flag_Board_6_End;
unsigned char g_Flag_Board_7_End;

struct S_Event{
unsigned int m_front;
unsigned int m_back;
unsigned int m_size;
unsigned int m_eventBuffer[EVENT_SIZE];
}g_event;
extern unsigned char g_hostShareDataBuffer[64];
extern unsigned char g_CAN_ReceiveBuffer[8];
long long g_llErrorFlag=0;

unsigned char g_hostShareDataTemp[64];

void Event_Init()
{
 g_event.m_front=EVENT_SIZE-1;
 g_event.m_back=0;
 g_event.m_size=0;
}

int Event_IsEmpty() {
  if(g_event.m_size>0)
  return  0;
  return  1;
}

int Event_AddEvent(unsigned int value)
{
  if(Event_IsFull())
  return 0;

  if(g_event.m_front>=EVENT_SIZE-1) //At the end of event buffer.
  {
     g_event.m_front=0;
  }
  else
  {
    g_event.m_front++;
  }

  g_event.m_eventBuffer[g_event.m_front]=value;
  g_event.m_size++;

  return 1;
}

int Event_IsFull()
{
  if(EVENT_SIZE<=g_event.m_size)
  return 1;
  return 0;
}
unsigned int Event_GetEvent()
{
	unsigned int old=g_event.m_back;
	if(g_event.m_size<=0)
	return NO_EVENT;

	if(g_event.m_back>=EVENT_SIZE-1)
	 g_event.m_back=0;
	else
	 g_event.m_back++;

	g_event.m_size--;
	return  g_event.m_eventBuffer[old];
}
unsigned int Event_CRC(unsigned char *ptr, unsigned char len)
{
	unsigned short crc;
	unsigned char i;

	crc=0;
	while(len--!=0)
	{
		for(i=0x80; i!=0; i/=2)
		{
			if((crc&0x8000)!=0)
				{crc*=2; crc^=0x1021;}
			else
				crc*=2;
			if((*ptr&i)!=0)
				crc^=0x1021;
		}
		ptr++;
	}
	return(crc);
}
void Event_vCRCShareMemery()
{
	unsigned char ubCrc[2];
	unsigned int uiCrc;

	uiCrc=Event_CRC(g_hostShareDataBuffer,60);
	ubCrc[0]=uiCrc&0xff;
	ubCrc[1]=uiCrc>>8;
	g_hostShareDataBuffer[60]=ubCrc[0];
	g_hostShareDataBuffer[61]=ubCrc[1];
}
unsigned int Event_uiIsCRCShareDataTemp()
{
	unsigned char ubCrc[2];
	unsigned int uiCrc;
	unsigned int uiComplete=0;
	uiCrc=Event_CRC(g_hostShareDataTemp,60);
	ubCrc[0]=uiCrc&0xff;
	ubCrc[1]=uiCrc>>8;

	if(g_hostShareDataTemp[60]==ubCrc[0])
	{
		if(g_hostShareDataTemp[61]==ubCrc[1])
		{
			uiComplete=1;
		}
	}
	return uiComplete;
}
unsigned int Event_uiIsCRCEEPROM(unsigned int startAddress,unsigned int count)
{
	unsigned char* EEPROM_ReturnBuffer=g_hostShareDataTemp;
	if(count<4)
		return EVENT_NOT_CRC;
	EEPROM_ReadEEPROM(startAddress,EEPROM_ReturnBuffer,count);

	return Event_uiIsCRCShareDataTemp();
}
static void Event_OnBoard_1_End()
{
	g_batteryVoltageArray[0][0]=BOARD1_BATTERY_VOLTAGE_1;
	g_batteryVoltageArray[0][1]=BOARD1_BATTERY_VOLTAGE_2;
	g_batteryVoltageArray[0][2]=BOARD1_BATTERY_VOLTAGE_3;
	g_batteryVoltageArray[0][3]=BOARD1_BATTERY_VOLTAGE_4;
	g_batteryVoltageArray[0][4]=BOARD1_BATTERY_VOLTAGE_5;
	g_batteryVoltageArray[0][5]=BOARD1_BATTERY_VOLTAGE_6;
	g_batteryVoltageArray[0][6]=BOARD1_BATTERY_VOLTAGE_7;
	g_batteryVoltageArray[0][7]=BOARD1_BATTERY_VOLTAGE_8;
	g_batteryVoltageArray[0][8]=BOARD1_BATTERY_VOLTAGE_9;
	g_batteryVoltageArray[0][9]=BOARD1_BATTERY_VOLTAGE_10;
	g_batteryVoltageArray[0][10]=BOARD1_BATTERY_VOLTAGE_11;
	g_batteryVoltageArray[0][11]=BOARD1_BATTERY_VOLTAGE_12;
	g_batteryVoltageArray[0][12]=BOARD1_BATTERY_VOLTAGE_13;
	g_batteryVoltageArray[0][13]=BOARD1_BATTERY_VOLTAGE_14;
	g_batteryVoltageArray[0][14]=BOARD1_BATTERY_VOLTAGE_15;
	g_batteryVoltageArray[0][15]=BOARD1_BATTERY_VOLTAGE_16;

	g_batteryTemptureArray[0][0]=BOARD1_BATTERY_TEMPTURE_1;
	g_batteryTemptureArray[0][1]=BOARD1_BATTERY_TEMPTURE_2;
	g_batteryTemptureArray[0][2]=BOARD1_BATTERY_TEMPTURE_3;
	g_batteryTemptureArray[0][3]=BOARD1_BATTERY_TEMPTURE_4;
	g_batteryTemptureArray[0][4]=BOARD1_BATTERY_TEMPTURE_5;
	g_batteryTemptureArray[0][5]=BOARD1_BATTERY_TEMPTURE_6;
	g_batteryTemptureArray[0][6]=BOARD1_BATTERY_TEMPTURE_7;
	g_batteryTemptureArray[0][7]=BOARD1_BATTERY_TEMPTURE_8;

	g_batteryTemptureArray[0][8]=BOARD1_BATTERY_TEMPTURE_9;
	g_batteryTemptureArray[0][9]=BOARD1_BATTERY_TEMPTURE_10;
	g_batteryTemptureArray[0][10]=BOARD1_BATTERY_TEMPTURE_11;
	g_batteryTemptureArray[0][11]=BOARD1_BATTERY_TEMPTURE_12;
	g_batteryTemptureArray[0][12]=BOARD1_BATTERY_TEMPTURE_13;
	g_batteryTemptureArray[0][13]=BOARD1_BATTERY_TEMPTURE_14;
	g_batteryTemptureArray[0][14]=BOARD1_BATTERY_TEMPTURE_15;
	g_batteryTemptureArray[0][15]=BOARD1_BATTERY_TEMPTURE_16;

	g_Flag_Board_1_End=1;

	Wave_Change_1();
}
static void Event_OnBoard_2_End()
{
	g_batteryVoltageArray[1][0]=BOARD2_BATTERY_VOLTAGE_1;
	g_batteryVoltageArray[1][1]=BOARD2_BATTERY_VOLTAGE_2;
	g_batteryVoltageArray[1][2]=BOARD2_BATTERY_VOLTAGE_3;
	g_batteryVoltageArray[1][3]=BOARD2_BATTERY_VOLTAGE_4;
	g_batteryVoltageArray[1][4]=BOARD2_BATTERY_VOLTAGE_5;
	g_batteryVoltageArray[1][5]=BOARD2_BATTERY_VOLTAGE_6;
	g_batteryVoltageArray[1][6]=BOARD2_BATTERY_VOLTAGE_7;
	g_batteryVoltageArray[1][7]=BOARD2_BATTERY_VOLTAGE_8;
	g_batteryVoltageArray[1][8]=BOARD2_BATTERY_VOLTAGE_9;
	g_batteryVoltageArray[1][9]=BOARD2_BATTERY_VOLTAGE_10;
	g_batteryVoltageArray[1][10]=BOARD2_BATTERY_VOLTAGE_11;
	g_batteryVoltageArray[1][11]=BOARD2_BATTERY_VOLTAGE_12;
	g_batteryVoltageArray[1][12]=BOARD2_BATTERY_VOLTAGE_13;
	g_batteryVoltageArray[1][13]=BOARD2_BATTERY_VOLTAGE_14;
	g_batteryVoltageArray[1][14]=BOARD2_BATTERY_VOLTAGE_15;
	g_batteryVoltageArray[1][15]=BOARD2_BATTERY_VOLTAGE_16;

	g_batteryTemptureArray[1][0]=BOARD2_BATTERY_TEMPTURE_1;
	g_batteryTemptureArray[1][1]=BOARD2_BATTERY_TEMPTURE_2;
	g_batteryTemptureArray[1][2]=BOARD2_BATTERY_TEMPTURE_3;
	g_batteryTemptureArray[1][3]=BOARD2_BATTERY_TEMPTURE_4;
	g_batteryTemptureArray[1][4]=BOARD2_BATTERY_TEMPTURE_5;
	g_batteryTemptureArray[1][5]=BOARD2_BATTERY_TEMPTURE_6;
	g_batteryTemptureArray[1][6]=BOARD2_BATTERY_TEMPTURE_7;
	g_batteryTemptureArray[1][7]=BOARD2_BATTERY_TEMPTURE_8;
	g_batteryTemptureArray[1][8]=BOARD2_BATTERY_TEMPTURE_9;
	g_batteryTemptureArray[1][9]=BOARD2_BATTERY_TEMPTURE_10;
	g_batteryTemptureArray[1][10]=BOARD2_BATTERY_TEMPTURE_11;
	g_batteryTemptureArray[1][11]=BOARD2_BATTERY_TEMPTURE_12;
	g_batteryTemptureArray[1][12]=BOARD2_BATTERY_TEMPTURE_13;
	g_batteryTemptureArray[1][13]=BOARD2_BATTERY_TEMPTURE_14;
	g_batteryTemptureArray[1][14]=BOARD2_BATTERY_TEMPTURE_15;
	g_batteryTemptureArray[1][15]=BOARD2_BATTERY_TEMPTURE_16;

	g_Flag_Board_2_End=1;
	Wave_Change_2();
}
static void Event_OnBoard_3_End()
{
	Wave_Change_3();
}
static void Event_OnBoard_4_End()
{
	Wave_Change_4();
}
static void Event_OnBoard_5_End()
{
	Wave_Change_5();
}
static void Event_OnBoard_6_End()
{
	Wave_Change_6();
}
static void Event_OnBoard_7_End()
{
	Wave_Change_7();
}
static void Event_OnGetADCValue_0()
{

}
static void Event_OnGetADCValue_1()
{

}
static void Event_OnGetADCValue_2()
{

}
static void Event_OnGetADCValue_3()
{
}
static void Event_OnGetADCValue_4()
{
}
static void Event_OnGetADCValue_5()
{
}
static void Event_OnGetADCValue_6()
{
}
static void Event_OnGetADCValue_7()
{
}
static void Event_OnGetADCValue_8()
{
}
static void Event_OnGetADCValue_9()
{
}
static void Event_OnGetADCValue_10()
{
}
#ifndef BOARD_DEFINE
#define BOARD_DEFINE
#define HOST_BOARD 0x10
#define SAMPLE_BOARD1 0x11
#define SAMPLE_BOARD2 0x12
#define DEVELOP_BOARD 0x13
#endif




static void Event_OnGetBatteryVoltage_1_1()
{
	GET_BATTERY_VOLTAGE(1,1);
	while(1);
}
static void Event_OnGetBatteryVoltage_1_2()
{
	GET_BATTERY_VOLTAGE(1,2);
}
static void Event_OnGetBatteryVoltage_1_3()
{
	GET_BATTERY_VOLTAGE(1,3);
}
static void Event_OnGetBatteryVoltage_1_4()
{
	GET_BATTERY_VOLTAGE(1,4);
}
static void Event_OnGetBatteryVoltage_1_5()
{
	GET_BATTERY_VOLTAGE(1,5);
}
static void Event_OnGetBatteryVoltage_1_6()
{
	GET_BATTERY_VOLTAGE(1,6);
}
static void Event_OnGetBatteryVoltage_1_7()
{
	GET_BATTERY_VOLTAGE(1,7);
}
static void Event_OnGetBatteryVoltage_1_8()
{
	GET_BATTERY_VOLTAGE(1,8);;
}
static void Event_OnGetBatteryVoltage_1_9()
{
	GET_BATTERY_VOLTAGE(1,9);
}
static void Event_OnGetBatteryVoltage_1_10()
{
	GET_BATTERY_VOLTAGE(1,10);
}
static void Event_OnGetBatteryVoltage_1_11()
{
	GET_BATTERY_VOLTAGE(1,11);
}
static void Event_OnGetBatteryVoltage_1_12()
{
	GET_BATTERY_VOLTAGE(1,12);
}
static void Event_OnGetBatteryVoltage_1_13()
{
	GET_BATTERY_VOLTAGE(1,13);
}
static void Event_OnGetBatteryVoltage_1_14()
{
	GET_BATTERY_VOLTAGE(1,14);
}
static void Event_OnGetBatteryVoltage_1_15()
{
	GET_BATTERY_VOLTAGE(1,15);
}
static void Event_OnGetBatteryVoltage_1_16()
{
	GET_BATTERY_VOLTAGE(1,16);
}
static void Event_OnGetBatteryVoltage_2_1()
{
	GET_BATTERY_VOLTAGE(2,1);
}
static void Event_OnGetBatteryVoltage_2_2()
{
	GET_BATTERY_VOLTAGE(2,2);
}
static void Event_OnGetBatteryVoltage_2_3()
{
	GET_BATTERY_VOLTAGE(2,3);
}
static void Event_OnGetBatteryVoltage_2_4()
{
	GET_BATTERY_VOLTAGE(2,4);
}
static void Event_OnGetBatteryVoltage_2_5()
{
	GET_BATTERY_VOLTAGE(2,5);
}
static void Event_OnGetBatteryVoltage_2_6()
{
	GET_BATTERY_VOLTAGE(2,6);
}
static void Event_OnGetBatteryVoltage_2_7()
{
	GET_BATTERY_VOLTAGE(2,7);
}
static void Event_OnGetBatteryVoltage_2_8()
{
	GET_BATTERY_VOLTAGE(2,8);
}
static void Event_OnGetBatteryVoltage_2_9()
{
	GET_BATTERY_VOLTAGE(2,9);
}
static void Event_OnGetBatteryVoltage_2_10()
{
	GET_BATTERY_VOLTAGE(2,10);
}
static void Event_OnGetBatteryVoltage_2_11()
{
	GET_BATTERY_VOLTAGE(2,11);
}
static void Event_OnGetBatteryVoltage_2_12()
{
	GET_BATTERY_VOLTAGE(2,12);
}
static void Event_OnGetBatteryVoltage_2_13()
{
	GET_BATTERY_VOLTAGE(2,13);
}
static void Event_OnGetBatteryVoltage_2_14()
{
	GET_BATTERY_VOLTAGE(2,14);
}
static void Event_OnGetBatteryVoltage_2_15()
{
	GET_BATTERY_VOLTAGE(2,15);
}
static void Event_OnGetBatteryVoltage_2_16()
{
	GET_BATTERY_VOLTAGE(2,16);
}

static void Event_OnGetBatteryVoltage_3_1()
{}
static void Event_OnGetBatteryVoltage_3_2()
{}
static void Event_OnGetBatteryVoltage_3_3()
{}
static void Event_OnGetBatteryVoltage_3_4()
{}
static void Event_OnGetBatteryVoltage_3_5()
{}
static void Event_OnGetBatteryVoltage_3_6()
{}
static void Event_OnGetBatteryVoltage_3_7()
{}
static void Event_OnGetBatteryVoltage_3_8()
{}
static void Event_OnGetBatteryVoltage_3_9()
{}
static void Event_OnGetBatteryVoltage_3_10()
{}
static void Event_OnGetBatteryVoltage_3_11()
{}
static void Event_OnGetBatteryVoltage_3_12()
{}
static void Event_OnGetBatteryVoltage_3_13()
{}
static void Event_OnGetBatteryVoltage_3_14()
{}
static void Event_OnGetBatteryVoltage_3_15()
{}
static void Event_OnGetBatteryVoltage_3_16()
{}
static void Event_OnGetBatteryVoltage_4_1()
{}
static void Event_OnGetBatteryVoltage_4_2()
{}
static void Event_OnGetBatteryVoltage_4_3()
{}
static void Event_OnGetBatteryVoltage_4_4()
{}
static void Event_OnGetBatteryVoltage_4_5()
{}
static void Event_OnGetBatteryVoltage_4_6()
{}
static void Event_OnGetBatteryVoltage_4_7()
{}
static void Event_OnGetBatteryVoltage_4_8()
{}
static void Event_OnGetBatteryVoltage_4_9()
{}
static void Event_OnGetBatteryVoltage_4_10()
{}
static void Event_OnGetBatteryVoltage_4_11()
{}
static void Event_OnGetBatteryVoltage_4_12()
{}
static void Event_OnGetBatteryVoltage_4_13()
{}
static void Event_OnGetBatteryVoltage_4_14()
{}
static void Event_OnGetBatteryVoltage_4_15()
{}
static void Event_OnGetBatteryVoltage_4_16()
{}
static void Event_OnGetBatteryVoltage_5_1()
{}
static void Event_OnGetBatteryVoltage_5_2()
{}
static void Event_OnGetBatteryVoltage_5_3()
{}
static void Event_OnGetBatteryVoltage_5_4()
{}
static void Event_OnGetBatteryVoltage_5_5()
{}
static void Event_OnGetBatteryVoltage_5_6()
{}
static void Event_OnGetBatteryVoltage_5_7()
{}
static void Event_OnGetBatteryVoltage_5_8()
{}
static void Event_OnGetBatteryVoltage_5_9()
{}
static void Event_OnGetBatteryVoltage_5_10()
{}
static void Event_OnGetBatteryVoltage_5_11()
{}
static void Event_OnGetBatteryVoltage_5_12()
{}
static void Event_OnGetBatteryVoltage_5_13()
{}
static void Event_OnGetBatteryVoltage_5_14()
{}
static void Event_OnGetBatteryVoltage_5_15()
{}
static void Event_OnGetBatteryVoltage_5_16()
{}
static void Event_OnGetBatteryVoltage_6_1()
{}
static void Event_OnGetBatteryVoltage_6_2()
{}
static void Event_OnGetBatteryVoltage_6_3()
{}
static void Event_OnGetBatteryVoltage_6_4()
{}
static void Event_OnGetBatteryVoltage_6_5()
{}
static void Event_OnGetBatteryVoltage_6_6()
{}
static void Event_OnGetBatteryVoltage_6_7()
{}
static void Event_OnGetBatteryVoltage_6_8()
{}
static void Event_OnGetBatteryVoltage_6_9()
{}
static void Event_OnGetBatteryVoltage_6_10()
{}
static void Event_OnGetBatteryVoltage_6_11()
{}
static void Event_OnGetBatteryVoltage_6_12()
{}
static void Event_OnGetBatteryVoltage_6_13()
{}
static void Event_OnGetBatteryVoltage_6_14()
{}
static void Event_OnGetBatteryVoltage_6_15()
{}
static void Event_OnGetBatteryVoltage_6_16()
{}
static void Event_OnGetBatteryVoltage_7_1()
{}
static void Event_OnGetBatteryVoltage_7_2()
{}
static void Event_OnGetBatteryVoltage_7_3()
{}
static void Event_OnGetBatteryVoltage_7_4()
{}
static void Event_OnGetBatteryVoltage_7_5()
{}
static void Event_OnGetBatteryVoltage_7_6()
{}
static void Event_OnGetBatteryVoltage_7_7()
{}
static void Event_OnGetBatteryVoltage_7_8()
{}
static void Event_OnGetBatteryVoltage_7_9()
{}
static void Event_OnGetBatteryVoltage_7_10()
{}
static void Event_OnGetBatteryVoltage_7_11()
{}
static void Event_OnGetBatteryVoltage_7_12()
{}
static void Event_OnGetBatteryVoltage_7_13()
{}
static void Event_OnGetBatteryVoltage_7_14()
{}
static void Event_OnGetBatteryVoltage_7_15()
{}
static void Event_OnGetBatteryVoltage_7_16()
{}
static void Event_OnGetBatteryTempture_1_1()
{}
static void Event_OnGetBatteryTempture_1_2()
{}
static void Event_OnGetBatteryTempture_1_3()
{}
static void Event_OnGetBatteryTempture_1_4()
{}
static void Event_OnGetBatteryTempture_1_5()
{}
static void Event_OnGetBatteryTempture_1_6()
{}
static void Event_OnGetBatteryTempture_1_7()
{}
static void Event_OnGetBatteryTempture_1_8()
{}
static void Event_OnGetBatteryTempture_1_9()
{}
static void Event_OnGetBatteryTempture_1_10()
{}
static void Event_OnGetBatteryTempture_1_11()
{}
static void Event_OnGetBatteryTempture_1_12()
{}
static void Event_OnGetBatteryTempture_1_13()
{}
static void Event_OnGetBatteryTempture_1_14()
{}
static void Event_OnGetBatteryTempture_1_15()
{}
static void Event_OnGetBatteryTempture_1_16()
{}
static void Event_OnGetBatteryTempture_2_1()
{}
static void Event_OnGetBatteryTempture_2_2()
{}
static void Event_OnGetBatteryTempture_2_3()
{}
static void Event_OnGetBatteryTempture_2_4()
{}
static void Event_OnGetBatteryTempture_2_5()
{}
static void Event_OnGetBatteryTempture_2_6()
{}
static void Event_OnGetBatteryTempture_2_7()
{}
static void Event_OnGetBatteryTempture_2_8()
{}
static void Event_OnGetBatteryTempture_2_9()
{}
static void Event_OnGetBatteryTempture_2_10()
{}
static void Event_OnGetBatteryTempture_2_11()
{}
static void Event_OnGetBatteryTempture_2_12()
{}
static void Event_OnGetBatteryTempture_2_13()
{}
static void Event_OnGetBatteryTempture_2_14()
{}
static void Event_OnGetBatteryTempture_2_15()
{}
static void Event_OnGetBatteryTempture_2_16()
{}

static void Event_OnGetBatteryTempture_3_1()
{}
static void Event_OnGetBatteryTempture_3_2()
{}
static void Event_OnGetBatteryTempture_3_3()
{}
static void Event_OnGetBatteryTempture_3_4()
{}
static void Event_OnGetBatteryTempture_3_5()
{}
static void Event_OnGetBatteryTempture_3_6()
{}
static void Event_OnGetBatteryTempture_3_7()
{}
static void Event_OnGetBatteryTempture_3_8()
{}
static void Event_OnGetBatteryTempture_3_9()
{}
static void Event_OnGetBatteryTempture_3_10()
{}
static void Event_OnGetBatteryTempture_3_11()
{}
static void Event_OnGetBatteryTempture_3_12()
{}
static void Event_OnGetBatteryTempture_3_13()
{}
static void Event_OnGetBatteryTempture_3_14()
{}
static void Event_OnGetBatteryTempture_3_15()
{}
static void Event_OnGetBatteryTempture_3_16()
{}
static void Event_OnGetBatteryTempture_4_1()
{}
static void Event_OnGetBatteryTempture_4_2()
{}
static void Event_OnGetBatteryTempture_4_3()
{}
static void Event_OnGetBatteryTempture_4_4()
{}
static void Event_OnGetBatteryTempture_4_5()
{}
static void Event_OnGetBatteryTempture_4_6()
{}
static void Event_OnGetBatteryTempture_4_7()
{}
static void Event_OnGetBatteryTempture_4_8()
{}
static void Event_OnGetBatteryTempture_4_9()
{}
static void Event_OnGetBatteryTempture_4_10()
{}
static void Event_OnGetBatteryTempture_4_11()
{}
static void Event_OnGetBatteryTempture_4_12()
{}
static void Event_OnGetBatteryTempture_4_13()
{}
static void Event_OnGetBatteryTempture_4_14()
{}
static void Event_OnGetBatteryTempture_4_15()
{}
static void Event_OnGetBatteryTempture_4_16()
{}
static void Event_OnGetBatteryTempture_5_1()
{}
static void Event_OnGetBatteryTempture_5_2()
{}
static void Event_OnGetBatteryTempture_5_3()
{}
static void Event_OnGetBatteryTempture_5_4()
{}
static void Event_OnGetBatteryTempture_5_5()
{}
static void Event_OnGetBatteryTempture_5_6()
{}
static void Event_OnGetBatteryTempture_5_7()
{}
static void Event_OnGetBatteryTempture_5_8()
{}
static void Event_OnGetBatteryTempture_5_9()
{}
static void Event_OnGetBatteryTempture_5_10()
{}
static void Event_OnGetBatteryTempture_5_11()
{}
static void Event_OnGetBatteryTempture_5_12()
{}
static void Event_OnGetBatteryTempture_5_13()
{}
static void Event_OnGetBatteryTempture_5_14()
{}
static void Event_OnGetBatteryTempture_5_15()
{}
static void Event_OnGetBatteryTempture_5_16()
{}
static void Event_OnGetBatteryTempture_6_1()
{}
static void Event_OnGetBatteryTempture_6_2()
{}
static void Event_OnGetBatteryTempture_6_3()
{}
static void Event_OnGetBatteryTempture_6_4()
{}
static void Event_OnGetBatteryTempture_6_5()
{}
static void Event_OnGetBatteryTempture_6_6()
{}
static void Event_OnGetBatteryTempture_6_7()
{}
static void Event_OnGetBatteryTempture_6_8()
{}
static void Event_OnGetBatteryTempture_6_9()
{}
static void Event_OnGetBatteryTempture_6_10()
{}
static void Event_OnGetBatteryTempture_6_11()
{}
static void Event_OnGetBatteryTempture_6_12()
{}
static void Event_OnGetBatteryTempture_6_13()
{}
static void Event_OnGetBatteryTempture_6_14()
{}
static void Event_OnGetBatteryTempture_6_15()
{}
static void Event_OnGetBatteryTempture_6_16()
{}
static void Event_OnGetBatteryTempture_7_1()
{}
static void Event_OnGetBatteryTempture_7_2()
{}
static void Event_OnGetBatteryTempture_7_3()
{}
static void Event_OnGetBatteryTempture_7_4()
{}
static void Event_OnGetBatteryTempture_7_5()
{}
static void Event_OnGetBatteryTempture_7_6()
{}
static void Event_OnGetBatteryTempture_7_7()
{}
static void Event_OnGetBatteryTempture_7_8()
{}
static void Event_OnGetBatteryTempture_7_9()
{}
static void Event_OnGetBatteryTempture_7_10()
{}
static void Event_OnGetBatteryTempture_7_11()
{}
static void Event_OnGetBatteryTempture_7_12()
{}
static void Event_OnGetBatteryTempture_7_13()
{}
static void Event_OnGetBatteryTempture_7_14()
{}
static void Event_OnGetBatteryTempture_7_15()
{}
static void Event_OnGetBatteryTempture_7_16()
{}
static unsigned char Int_To_BCD(unsigned char value)
{
	unsigned char BCD_High,BCD_Low,BCD;

	BCD_High=(unsigned char)value/10;
	BCD_Low=value%10;
	BCD_High<<=4;
	BCD=BCD_High+BCD_Low;

	return BCD;
}
static void Event_OnGetSecond()
{
	unsigned char value=Calendar_GetSecond();
	unsigned char DevelopDataReture[8]={DEVELOP_BOARD,WM_DATA_RETURN,0,};
	DevelopDataReture[4]=Int_To_BCD(value);//Second two
	DevelopDataReture[3]=0;//First two
	CAN_Send(DevelopDataReture);
}
static void Event_OnGetMinute()
{
	unsigned char value=Calendar_GetMinute();
	unsigned char DevelopDataReture[8]={DEVELOP_BOARD,WM_DATA_RETURN,0,};
	DevelopDataReture[4]=Int_To_BCD(value);//Second two
	DevelopDataReture[3]=0;//First two
	CAN_Send(DevelopDataReture);
}
static void Event_OnGetHour()
{
	unsigned char value=Calendar_GetHour();
	unsigned char DevelopDataReture[8]={DEVELOP_BOARD,WM_DATA_RETURN,0,};
	DevelopDataReture[4]=Int_To_BCD(value);//Second two
	DevelopDataReture[3]=0;//First two
	CAN_Send(DevelopDataReture);
}
static void Event_OnGetDay()
{
	unsigned char value=Calendar_GetDay();
	unsigned char DevelopDataReture[8]={DEVELOP_BOARD,WM_DATA_RETURN,0,};
	DevelopDataReture[4]=Int_To_BCD(value);//Second two
	DevelopDataReture[3]=0;//First two
	CAN_Send(DevelopDataReture);
}
static void Event_OnGetMonth()
{
	unsigned char value=Calendar_GetMonth();
	unsigned char DevelopDataReture[8]={DEVELOP_BOARD,WM_DATA_RETURN,0,};
	DevelopDataReture[4]=Int_To_BCD(value);//Second two
	DevelopDataReture[3]=0;//First two
	CAN_Send(DevelopDataReture);
}
static void Event_OnGetYear()
{
	unsigned char value=Calendar_GetYear();
	unsigned char DevelopDataReture[8]={DEVELOP_BOARD,WM_DATA_RETURN,0,};
	DevelopDataReture[4]=Int_To_BCD(value);//First two
	DevelopDataReture[3]=0;//Second two
	CAN_Send(DevelopDataReture);
}
static void Event_OnSetSecond()
{}
static void Event_OnSetMinute()
{}
static void Event_OnSetHour()
{}
static void Event_OnSetDay()
{}
static void Event_OnSetMonth()
{}
static void Event_OnSetYear()
{}

#ifndef CAN_OBJECT_DEFINE__
#define CAN_OBJECT_DEFINE__  1

#define CAN_ORDER_HIGH 2
#define CAN_ORDER_LOW  1
#define CAN_BOARD_ID   0
#define CAN_DATA_0 		3
#define CAN_DATA_1 		4
#define CAN_DATA_2 		5
#define CAN_DATA_3 		6
#define CAN_DATA_4 		7

#endif
static void Event_OnCheckBoard_1_End()
{
	unsigned int emptyBatteryArray=0;
	emptyBatteryArray=g_CAN_ReceiveBuffer[CAN_DATA_1];
	emptyBatteryArray<<=8;
	emptyBatteryArray+=g_CAN_ReceiveBuffer[CAN_DATA_0];

	g_batteryEmptyArray[0]=emptyBatteryArray;
}
static void Event_OnCheckBoard_2_End()
{
	unsigned int emptyBatteryArray=0;
	emptyBatteryArray=g_CAN_ReceiveBuffer[CAN_DATA_1];
	emptyBatteryArray<<=8;
	emptyBatteryArray+=g_CAN_ReceiveBuffer[CAN_DATA_0];

	g_batteryEmptyArray[1]=emptyBatteryArray;
}
static void Event_OnCheckBoard_3_End()
{
	unsigned int emptyBatteryArray=0;
	emptyBatteryArray=g_CAN_ReceiveBuffer[CAN_DATA_1];
	emptyBatteryArray<<=8;
	emptyBatteryArray+=g_CAN_ReceiveBuffer[CAN_DATA_0];

	g_batteryEmptyArray[2]=emptyBatteryArray;
}
static void Event_OnCheckBoard_4_End()
{
	unsigned int emptyBatteryArray=0;
	emptyBatteryArray=g_CAN_ReceiveBuffer[CAN_DATA_1];
	emptyBatteryArray<<=8;
	emptyBatteryArray+=g_CAN_ReceiveBuffer[CAN_DATA_0];

	g_batteryEmptyArray[3]=emptyBatteryArray;
}
static void Event_OnCheckBoard_5_End()
{
	unsigned int emptyBatteryArray=0;
	emptyBatteryArray=g_CAN_ReceiveBuffer[CAN_DATA_1];
	emptyBatteryArray<<=8;
	emptyBatteryArray+=g_CAN_ReceiveBuffer[CAN_DATA_0];

	g_batteryEmptyArray[4]=emptyBatteryArray;
}
static void Event_OnCheckBoard_6_End()
{
	unsigned int emptyBatteryArray=0;
	emptyBatteryArray=g_CAN_ReceiveBuffer[CAN_DATA_1];
	emptyBatteryArray<<=8;
	emptyBatteryArray+=g_CAN_ReceiveBuffer[CAN_DATA_0];

	g_batteryEmptyArray[5]=emptyBatteryArray;
}
static void Event_OnCheckBoard_7_End()
{
	unsigned int emptyBatteryArray=0;
	emptyBatteryArray=g_CAN_ReceiveBuffer[CAN_DATA_1];
	emptyBatteryArray<<=8;
	emptyBatteryArray+=g_CAN_ReceiveBuffer[CAN_DATA_0];

	g_batteryEmptyArray[6]=emptyBatteryArray;
}
#define EEPROM_MAX_ADDRESS 0x3fff;

static void Event_OnGetBlockEEPROM()
{
	unsigned int EEPROM_ReadAddress=0;
	unsigned int EEPROM_ReadAddressHigh=g_CAN_ReceiveBuffer[4];
	unsigned int EEPROM_ReadAddressLow=	g_CAN_ReceiveBuffer[3];
	unsigned char EEPROM_ReturnBuffer[64];

	EEPROM_ReadAddress=EEPROM_ReadAddressHigh;
	EEPROM_ReadAddress<<=8;
	EEPROM_ReadAddress+=EEPROM_ReadAddressLow;
	EEPROM_ReadEEPROM(EEPROM_ReadAddress,EEPROM_ReturnBuffer,64);

	CAN_ChangeSendID(BMS_ID_EEPROM_DATA_1);
	CAN_Send(&EEPROM_ReturnBuffer[0]);
	while(CAN_Task_GetStatus()!=0x1ee2);
	CAN_ChangeSendID(BMS_ID_EEPROM_DATA_2);
	CAN_Send(&EEPROM_ReturnBuffer[1*8]);
	while(CAN_Task_GetStatus()!=0x1ee2);
	CAN_ChangeSendID(BMS_ID_EEPROM_DATA_3);
	CAN_Send(&EEPROM_ReturnBuffer[2*8]);
	while(CAN_Task_GetStatus()!=0x1ee2);
	CAN_ChangeSendID(BMS_ID_EEPROM_DATA_4);
	CAN_Send(&EEPROM_ReturnBuffer[3*8]);
	while(CAN_Task_GetStatus()!=0x1ee2);
	CAN_ChangeSendID(BMS_ID_EEPROM_DATA_5);
	CAN_Send(&EEPROM_ReturnBuffer[4*8]);
	while(CAN_Task_GetStatus()!=0x1ee2);
	CAN_ChangeSendID(BMS_ID_EEPROM_DATA_6);
	CAN_Send(&EEPROM_ReturnBuffer[5*8]);
	while(CAN_Task_GetStatus()!=0x1ee2);
	CAN_ChangeSendID(BMS_ID_EEPROM_DATA_7);
	CAN_Send(&EEPROM_ReturnBuffer[6*8]);
	while(CAN_Task_GetStatus()!=0x1ee2);
	CAN_ChangeSendID(BMS_ID_EEPROM_DATA_8);
	CAN_Send(&EEPROM_ReturnBuffer[7*8]);
	while(CAN_Task_GetStatus()!=0x1ee2);
}

static void Event_OnSetDataCRC()
{
	unsigned int crc=Event_CRC(g_hostShareDataTemp,64);
	unsigned int _crc,_crcHigh,_crcLow;
	unsigned char returnResult[8];

	_crcHigh=g_CAN_ReceiveBuffer[4];
	_crcLow=g_CAN_ReceiveBuffer[3];
	_crcHigh<<=8;
	_crc=_crcLow+_crcHigh;
	if(_crc==crc)
	{
		returnResult[0]=1;
		memcpy(g_hostShareDataBuffer,g_hostShareDataTemp,64);
	}
	else
	{
		returnResult[0]=0;
	}
	CAN_ChangeSendID(BMS_ID_HOST_WRITE_RETURN);
	CAN_Send(returnResult);
	while(CAN_Task_GetStatus()!=0x1ee2);
}
static void Event_OnSetDataAll_1()
{
	memcpy(&g_hostShareDataTemp[0],&g_CAN_ReceiveBuffer[4],4);
}
static void Event_OnSetDataAll_2()
{
	memcpy(&g_hostShareDataTemp[1*4],&g_CAN_ReceiveBuffer[4],4);
}
static void Event_OnSetDataAll_3()
{
	memcpy(&g_hostShareDataTemp[2*4],&g_CAN_ReceiveBuffer[4],4);
}
static void Event_OnSetDataAll_4()
{
	memcpy(&g_hostShareDataTemp[3*4],&g_CAN_ReceiveBuffer[4],4);
}
static void Event_OnSetDataAll_5()
{
	memcpy(&g_hostShareDataTemp[4*4],&g_CAN_ReceiveBuffer[4],4);
}
static void Event_OnSetDataAll_6()
{
	memcpy(&g_hostShareDataTemp[5*4],&g_CAN_ReceiveBuffer[4],4);
}
static void Event_OnSetDataAll_7()
{
	memcpy(&g_hostShareDataTemp[6*4],&g_CAN_ReceiveBuffer[4],4);
}
static void Event_OnSetDataAll_8()
{
	memcpy(&g_hostShareDataTemp[7*4],&g_CAN_ReceiveBuffer[4],4);
}
static void Event_OnSetDataAll_9()
{
	memcpy(&g_hostShareDataTemp[8*4],&g_CAN_ReceiveBuffer[4],4);
}
static void Event_OnSetDataAll_10()
{
	memcpy(&g_hostShareDataTemp[9*4],&g_CAN_ReceiveBuffer[4],4);
}
static void Event_OnSetDataAll_11()
{
	memcpy(&g_hostShareDataTemp[10*4],&g_CAN_ReceiveBuffer[4],4);
}
static void Event_OnSetDataAll_12()
{
	memcpy(&g_hostShareDataTemp[11*4],&g_CAN_ReceiveBuffer[4],4);
}
static void Event_OnSetDataAll_13()
{
	memcpy(&g_hostShareDataTemp[12*4],&g_CAN_ReceiveBuffer[4],4);
}
static void Event_OnSetDataAll_14()
{
	memcpy(&g_hostShareDataTemp[13*4],&g_CAN_ReceiveBuffer[4],4);
}
static void Event_OnSetDataAll_15()
{
	memcpy(&g_hostShareDataTemp[14*4],&g_CAN_ReceiveBuffer[4],4);
}
static void Event_OnSetDataAll_16()
{
	memcpy(&g_hostShareDataTemp[15*4],&g_CAN_ReceiveBuffer[4],4);
}
static void Event_OnSetDataAll_17()
{}
static void Event_OnSetDataAll_18()
{}
static void Event_OnSetDataAll_19()
{}
static void Event_OnSetDataAll_20()
{}
static void Event_OnSetDataAll_21()
{}
static void Event_OnSetDataAll_22()
{}
static void Event_OnSetDataAll_23()
{}
static void Event_OnSetDataAll_24()
{}
static void Event_OnShareMemeryToEEPROM()
{
	unsigned int uiEEPROM_WriteAddress=EEPROM_HOSTSHARE_MEMERYHEAD;
	unsigned int uiEEPROM_WriteLength=EEPROM_HOSTSHARE_MEMERYLENGTH;
	unsigned char* ubEEPROM_WriteBuffer=g_hostShareDataBuffer;
	unsigned int uiComplete=0;

	int iErrorCounter=0;
	int i;
	Event_vCRCShareMemery();
again:
	EEPROM_WriteEEPROM(uiEEPROM_WriteAddress,ubEEPROM_WriteBuffer,uiEEPROM_WriteLength);

	uiComplete=Event_uiIsCRCEEPROM(uiEEPROM_WriteAddress,uiEEPROM_WriteLength);

	if(uiComplete!=1)
	{
		if(iErrorCounter++<3)
			goto again;
		else
		{
			g_llErrorFlag|=ERROR_SHAREMEMERY_TO_EEPROM;
		}
	}

}
static void Event_OnEEPROMToShareMemery()
{
	unsigned int EEPROM_ReadAddress=EEPROM_HOSTSHARE_MEMERYHEAD;
	unsigned int EEPROM_ReadLength=EEPROM_HOSTSHARE_MEMERYLENGTH;
	unsigned char* EEPROM_ReturnBuffer=g_hostShareDataTemp;
	unsigned int uiComplet=0;
	int iErrorCounter=0;
	int i;
	if(EEPROM_ReadLength<4)
		return;
again:
	EEPROM_ReadEEPROM(EEPROM_ReadAddress,EEPROM_ReturnBuffer,EEPROM_ReadLength);

	uiComplet= Event_uiIsCRCShareDataTemp();
	if(uiComplet!=1)
	{
		if(iErrorCounter++<3)
			goto again;
		else
		{
			g_llErrorFlag|=ERROR_EEPROM_TO_SHAREMEMERY;
		}
	}
	else
	{
		for(i=0;i<64;i++)
		{
			g_hostShareDataBuffer[i]=g_hostShareDataTemp[i];
		}
	}
}
static void Event_OnEmptyError()
{
	g_llErrorFlag=0;
}
static void Event_OnGetDataAll()
{
	unsigned int crc=Event_CRC(g_hostShareDataBuffer,64);
	unsigned char returnResult[8];

	BMS_CANSend(BMS_ID_HOST_DATA_1,&g_hostShareDataBuffer[0]);
	BMS_CANSend(BMS_ID_HOST_DATA_2,&g_hostShareDataBuffer[1*8]);
	BMS_CANSend(BMS_ID_HOST_DATA_3,&g_hostShareDataBuffer[2*8]);
	BMS_CANSend(BMS_ID_HOST_DATA_4,&g_hostShareDataBuffer[3*8]);
	BMS_CANSend(BMS_ID_HOST_DATA_5,&g_hostShareDataBuffer[4*8]);
	BMS_CANSend(BMS_ID_HOST_DATA_6,&g_hostShareDataBuffer[5*8]);
	BMS_CANSend(BMS_ID_HOST_DATA_7,&g_hostShareDataBuffer[6*8]);
	BMS_CANSend(BMS_ID_HOST_DATA_8,&g_hostShareDataBuffer[7*8]);

	returnResult[0]=crc&0xff;
	returnResult[1]=crc>>8;
	BMS_CANSend(BMS_ID_HOST_DATA_END,returnResult);

}
static void Event_OnGetOutTimer()
{
	unsigned char ubDay,ubYear,ubMonth,
	ubHour,ubMinute,ubSecond;
	unsigned char pubCANBuffer[8];
	//Calendar_LoadDate();

	ubDay=Calendar_GetDay();
	ubYear=Calendar_GetYear();
	ubMonth=Calendar_GetMonth();
	ubHour=Calendar_GetHour();
	ubMinute=Calendar_GetMinute();
	ubSecond=Calendar_GetSecond();
	pubCANBuffer[0]=ubSecond;
	pubCANBuffer[1]=ubMinute;
	pubCANBuffer[2]=ubHour;
	pubCANBuffer[3]=ubDay;
	pubCANBuffer[4]=ubMonth;
	pubCANBuffer[5]=ubYear;
	pubCANBuffer[6]=0;
	pubCANBuffer[7]=0;
	BMS_CANSend(BMS_ID_HOST_OUT_TIMER,pubCANBuffer);
}
static void Event_OnSetOutTimer()
{
	unsigned char ubDay,ubYear,ubMonth,
	ubHour,ubMinute,ubSecond;

	ubMinute=g_CAN_ReceiveBuffer[3];
	ubHour=g_CAN_ReceiveBuffer[4];
	ubDay=g_CAN_ReceiveBuffer[5];
	ubMonth=g_CAN_ReceiveBuffer[6];
	ubYear=g_CAN_ReceiveBuffer[7];
	Calendar_SetDate(ubYear,ubMonth,ubDay,ubHour,ubMinute,0);
}
#define EVENT_MAP(__event,__process) case __event: __process();break;
void Event_Progress(unsigned int event)
{

  switch(event) {

    EVENT_MAP(WM_GET_ADC_CHANNEL_0,Event_OnGetADCValue_0);
    EVENT_MAP(WM_GET_ADC_CHANNEL_1,Event_OnGetADCValue_1);
    EVENT_MAP(WM_GET_ADC_CHANNEL_2,Event_OnGetADCValue_2);
    EVENT_MAP(WM_GET_ADC_CHANNEL_3,Event_OnGetADCValue_3);
    EVENT_MAP(WM_GET_ADC_CHANNEL_4,Event_OnGetADCValue_4);
    EVENT_MAP(WM_GET_ADC_CHANNEL_5,Event_OnGetADCValue_5);
    EVENT_MAP(WM_GET_ADC_CHANNEL_6,Event_OnGetADCValue_6);
    EVENT_MAP(WM_GET_ADC_CHANNEL_7,Event_OnGetADCValue_7);
    EVENT_MAP(WM_GET_ADC_CHANNEL_8,Event_OnGetADCValue_8);
    EVENT_MAP(WM_GET_ADC_CHANNEL_9,Event_OnGetADCValue_9);
    EVENT_MAP(WM_GET_ADC_CHANNEL_10,Event_OnGetADCValue_10);

    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_1_1,Event_OnGetBatteryVoltage_1_1);
    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_1_2,Event_OnGetBatteryVoltage_1_2);
    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_1_3,Event_OnGetBatteryVoltage_1_3);
    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_1_4,Event_OnGetBatteryVoltage_1_4);
    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_1_5,Event_OnGetBatteryVoltage_1_5);
    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_1_6,Event_OnGetBatteryVoltage_1_6);
    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_1_7,Event_OnGetBatteryVoltage_1_7);
    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_1_8,Event_OnGetBatteryVoltage_1_8);
    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_1_9,Event_OnGetBatteryVoltage_1_9);
    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_1_10,Event_OnGetBatteryVoltage_1_10);
    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_1_11,Event_OnGetBatteryVoltage_1_11);
    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_1_12,Event_OnGetBatteryVoltage_1_12);
    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_1_13,Event_OnGetBatteryVoltage_1_13);
    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_1_14,Event_OnGetBatteryVoltage_1_14);
    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_1_15,Event_OnGetBatteryVoltage_1_15);
    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_1_16,Event_OnGetBatteryVoltage_1_16);

    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_2_1,Event_OnGetBatteryVoltage_2_1);
    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_2_2,Event_OnGetBatteryVoltage_2_2);
    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_2_3,Event_OnGetBatteryVoltage_2_3);
    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_2_4,Event_OnGetBatteryVoltage_2_4);
    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_2_5,Event_OnGetBatteryVoltage_2_5);
    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_2_6,Event_OnGetBatteryVoltage_2_6);
    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_2_7,Event_OnGetBatteryVoltage_2_7);
    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_2_8,Event_OnGetBatteryVoltage_2_8);
    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_2_9,Event_OnGetBatteryVoltage_2_9);
    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_2_10,Event_OnGetBatteryVoltage_2_10);
    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_2_11,Event_OnGetBatteryVoltage_2_11);
    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_2_12,Event_OnGetBatteryVoltage_2_12);
    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_2_13,Event_OnGetBatteryVoltage_2_13);
    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_2_14,Event_OnGetBatteryVoltage_2_14);
    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_2_15,Event_OnGetBatteryVoltage_2_15);
    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_2_16,Event_OnGetBatteryVoltage_2_16);

    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_3_1,Event_OnGetBatteryVoltage_3_1);
    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_3_2,Event_OnGetBatteryVoltage_3_2);
    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_3_3,Event_OnGetBatteryVoltage_3_3);
    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_3_4,Event_OnGetBatteryVoltage_3_4);
    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_3_5,Event_OnGetBatteryVoltage_3_5);
    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_3_6,Event_OnGetBatteryVoltage_3_6);
    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_3_7,Event_OnGetBatteryVoltage_3_7);
    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_3_8,Event_OnGetBatteryVoltage_3_8);
    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_3_9,Event_OnGetBatteryVoltage_3_9);
    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_3_10,Event_OnGetBatteryVoltage_3_10);
    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_3_11,Event_OnGetBatteryVoltage_3_11);
    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_3_12,Event_OnGetBatteryVoltage_3_12);
    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_3_13,Event_OnGetBatteryVoltage_3_13);
    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_3_14,Event_OnGetBatteryVoltage_3_14);
    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_3_15,Event_OnGetBatteryVoltage_3_15);
    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_3_16,Event_OnGetBatteryVoltage_3_16);

    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_4_1,Event_OnGetBatteryVoltage_4_1);
    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_4_2,Event_OnGetBatteryVoltage_4_2);
    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_4_3,Event_OnGetBatteryVoltage_4_3);
    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_4_4,Event_OnGetBatteryVoltage_4_4);
    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_4_5,Event_OnGetBatteryVoltage_4_5);
    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_4_6,Event_OnGetBatteryVoltage_4_6);
    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_4_7,Event_OnGetBatteryVoltage_4_7);
    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_4_8,Event_OnGetBatteryVoltage_4_8);
    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_4_9,Event_OnGetBatteryVoltage_4_9);
    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_4_10,Event_OnGetBatteryVoltage_4_10);
    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_4_11,Event_OnGetBatteryVoltage_4_11);
    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_4_12,Event_OnGetBatteryVoltage_4_12);
    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_4_13,Event_OnGetBatteryVoltage_4_13);
    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_4_14,Event_OnGetBatteryVoltage_4_14);
    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_4_15,Event_OnGetBatteryVoltage_4_15);
    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_4_16,Event_OnGetBatteryVoltage_4_16);

    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_5_1,Event_OnGetBatteryVoltage_5_1);
    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_5_2,Event_OnGetBatteryVoltage_5_2);
    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_5_3,Event_OnGetBatteryVoltage_5_3);
    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_5_4,Event_OnGetBatteryVoltage_5_4);
    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_5_5,Event_OnGetBatteryVoltage_5_5);
    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_5_6,Event_OnGetBatteryVoltage_5_6);
    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_5_7,Event_OnGetBatteryVoltage_5_7);
    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_5_8,Event_OnGetBatteryVoltage_5_8);
    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_5_9,Event_OnGetBatteryVoltage_5_9);
    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_5_10,Event_OnGetBatteryVoltage_5_10);
    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_5_11,Event_OnGetBatteryVoltage_5_11);
    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_5_12,Event_OnGetBatteryVoltage_5_12);
    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_5_13,Event_OnGetBatteryVoltage_5_13);
    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_5_14,Event_OnGetBatteryVoltage_5_14);
    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_5_15,Event_OnGetBatteryVoltage_5_15);
    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_5_16,Event_OnGetBatteryVoltage_5_16);

    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_6_1,Event_OnGetBatteryVoltage_6_1);
    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_6_2,Event_OnGetBatteryVoltage_6_2);
    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_6_3,Event_OnGetBatteryVoltage_6_3);
    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_6_4,Event_OnGetBatteryVoltage_6_4);
    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_6_5,Event_OnGetBatteryVoltage_6_5);
    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_6_6,Event_OnGetBatteryVoltage_6_6);
    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_6_7,Event_OnGetBatteryVoltage_6_7);
    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_6_8,Event_OnGetBatteryVoltage_6_8);
    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_6_9,Event_OnGetBatteryVoltage_6_9);
    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_6_10,Event_OnGetBatteryVoltage_6_10);
    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_6_11,Event_OnGetBatteryVoltage_6_11);
    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_6_12,Event_OnGetBatteryVoltage_6_12);
    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_6_13,Event_OnGetBatteryVoltage_6_13);
    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_6_14,Event_OnGetBatteryVoltage_6_14);
    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_6_15,Event_OnGetBatteryVoltage_6_15);
    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_6_16,Event_OnGetBatteryVoltage_6_16);

    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_7_1,Event_OnGetBatteryVoltage_7_1);
    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_7_2,Event_OnGetBatteryVoltage_7_2);
    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_7_3,Event_OnGetBatteryVoltage_7_3);
    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_7_4,Event_OnGetBatteryVoltage_7_4);
    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_7_5,Event_OnGetBatteryVoltage_7_5);
    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_7_6,Event_OnGetBatteryVoltage_7_6);
    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_7_7,Event_OnGetBatteryVoltage_7_7);
    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_7_8,Event_OnGetBatteryVoltage_7_8);
    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_7_9,Event_OnGetBatteryVoltage_7_9);
    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_7_10,Event_OnGetBatteryVoltage_7_10);
    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_7_11,Event_OnGetBatteryVoltage_7_11);
    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_7_12,Event_OnGetBatteryVoltage_7_12);
    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_7_13,Event_OnGetBatteryVoltage_7_13);
    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_7_14,Event_OnGetBatteryVoltage_7_14);
    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_7_15,Event_OnGetBatteryVoltage_7_15);
    EVENT_MAP(WM_GET_BATTERY_VOLTAGE_7_16,Event_OnGetBatteryVoltage_7_16);

    EVENT_MAP(WM_GET_BATTERY_TEMPTURE_1_1,Event_OnGetBatteryTempture_1_1);
     EVENT_MAP(WM_GET_BATTERY_TEMPTURE_1_2,Event_OnGetBatteryTempture_1_2);
     EVENT_MAP(WM_GET_BATTERY_TEMPTURE_1_3,Event_OnGetBatteryTempture_1_3);
     EVENT_MAP(WM_GET_BATTERY_TEMPTURE_1_4,Event_OnGetBatteryTempture_1_4);
     EVENT_MAP(WM_GET_BATTERY_TEMPTURE_1_5,Event_OnGetBatteryTempture_1_5);
     EVENT_MAP(WM_GET_BATTERY_TEMPTURE_1_6,Event_OnGetBatteryTempture_1_6);
     EVENT_MAP(WM_GET_BATTERY_TEMPTURE_1_7,Event_OnGetBatteryTempture_1_7);
     EVENT_MAP(WM_GET_BATTERY_TEMPTURE_1_8,Event_OnGetBatteryTempture_1_8);
     EVENT_MAP(WM_GET_BATTERY_TEMPTURE_1_9,Event_OnGetBatteryTempture_1_9);
     EVENT_MAP(WM_GET_BATTERY_TEMPTURE_1_10,Event_OnGetBatteryTempture_1_10);
     EVENT_MAP(WM_GET_BATTERY_TEMPTURE_1_11,Event_OnGetBatteryTempture_1_11);
     EVENT_MAP(WM_GET_BATTERY_TEMPTURE_1_12,Event_OnGetBatteryTempture_1_12);
     EVENT_MAP(WM_GET_BATTERY_TEMPTURE_1_13,Event_OnGetBatteryTempture_1_13);
     EVENT_MAP(WM_GET_BATTERY_TEMPTURE_1_14,Event_OnGetBatteryTempture_1_14);
     EVENT_MAP(WM_GET_BATTERY_TEMPTURE_1_15,Event_OnGetBatteryTempture_1_15);
     EVENT_MAP(WM_GET_BATTERY_TEMPTURE_1_16,Event_OnGetBatteryTempture_1_16);

     EVENT_MAP(WM_GET_BATTERY_TEMPTURE_2_1,Event_OnGetBatteryTempture_2_1);
     EVENT_MAP(WM_GET_BATTERY_TEMPTURE_2_2,Event_OnGetBatteryTempture_2_2);
     EVENT_MAP(WM_GET_BATTERY_TEMPTURE_2_3,Event_OnGetBatteryTempture_2_3);
     EVENT_MAP(WM_GET_BATTERY_TEMPTURE_2_4,Event_OnGetBatteryTempture_2_4);
     EVENT_MAP(WM_GET_BATTERY_TEMPTURE_2_5,Event_OnGetBatteryTempture_2_5);
     EVENT_MAP(WM_GET_BATTERY_TEMPTURE_2_6,Event_OnGetBatteryTempture_2_6);
     EVENT_MAP(WM_GET_BATTERY_TEMPTURE_2_7,Event_OnGetBatteryTempture_2_7);
     EVENT_MAP(WM_GET_BATTERY_TEMPTURE_2_8,Event_OnGetBatteryTempture_2_8);
     EVENT_MAP(WM_GET_BATTERY_TEMPTURE_2_9,Event_OnGetBatteryTempture_2_9);
     EVENT_MAP(WM_GET_BATTERY_TEMPTURE_2_10,Event_OnGetBatteryTempture_2_10);
     EVENT_MAP(WM_GET_BATTERY_TEMPTURE_2_11,Event_OnGetBatteryTempture_2_11);
     EVENT_MAP(WM_GET_BATTERY_TEMPTURE_2_12,Event_OnGetBatteryTempture_2_12);
     EVENT_MAP(WM_GET_BATTERY_TEMPTURE_2_13,Event_OnGetBatteryTempture_2_13);
     EVENT_MAP(WM_GET_BATTERY_TEMPTURE_2_14,Event_OnGetBatteryTempture_2_14);
     EVENT_MAP(WM_GET_BATTERY_TEMPTURE_2_15,Event_OnGetBatteryTempture_2_15);
     EVENT_MAP(WM_GET_BATTERY_TEMPTURE_2_16,Event_OnGetBatteryTempture_2_16);

     EVENT_MAP(WM_GET_BATTERY_TEMPTURE_3_1,Event_OnGetBatteryTempture_3_1);
     EVENT_MAP(WM_GET_BATTERY_TEMPTURE_3_2,Event_OnGetBatteryTempture_3_2);
     EVENT_MAP(WM_GET_BATTERY_TEMPTURE_3_3,Event_OnGetBatteryTempture_3_3);
     EVENT_MAP(WM_GET_BATTERY_TEMPTURE_3_4,Event_OnGetBatteryTempture_3_4);
     EVENT_MAP(WM_GET_BATTERY_TEMPTURE_3_5,Event_OnGetBatteryTempture_3_5);
     EVENT_MAP(WM_GET_BATTERY_TEMPTURE_3_6,Event_OnGetBatteryTempture_3_6);
     EVENT_MAP(WM_GET_BATTERY_TEMPTURE_3_7,Event_OnGetBatteryTempture_3_7);
     EVENT_MAP(WM_GET_BATTERY_TEMPTURE_3_8,Event_OnGetBatteryTempture_3_8);
     EVENT_MAP(WM_GET_BATTERY_TEMPTURE_3_9,Event_OnGetBatteryTempture_3_9);
     EVENT_MAP(WM_GET_BATTERY_TEMPTURE_3_10,Event_OnGetBatteryTempture_3_10);
     EVENT_MAP(WM_GET_BATTERY_TEMPTURE_3_11,Event_OnGetBatteryTempture_3_11);
     EVENT_MAP(WM_GET_BATTERY_TEMPTURE_3_12,Event_OnGetBatteryTempture_3_12);
     EVENT_MAP(WM_GET_BATTERY_TEMPTURE_3_13,Event_OnGetBatteryTempture_3_13);
     EVENT_MAP(WM_GET_BATTERY_TEMPTURE_3_14,Event_OnGetBatteryTempture_3_14);
     EVENT_MAP(WM_GET_BATTERY_TEMPTURE_3_15,Event_OnGetBatteryTempture_3_15);
     EVENT_MAP(WM_GET_BATTERY_TEMPTURE_3_16,Event_OnGetBatteryTempture_3_16);

     EVENT_MAP(WM_GET_BATTERY_TEMPTURE_4_1,Event_OnGetBatteryTempture_4_1);
     EVENT_MAP(WM_GET_BATTERY_TEMPTURE_4_2,Event_OnGetBatteryTempture_4_2);
     EVENT_MAP(WM_GET_BATTERY_TEMPTURE_4_3,Event_OnGetBatteryTempture_4_3);
     EVENT_MAP(WM_GET_BATTERY_TEMPTURE_4_4,Event_OnGetBatteryTempture_4_4);
     EVENT_MAP(WM_GET_BATTERY_TEMPTURE_4_5,Event_OnGetBatteryTempture_4_5);
     EVENT_MAP(WM_GET_BATTERY_TEMPTURE_4_6,Event_OnGetBatteryTempture_4_6);
     EVENT_MAP(WM_GET_BATTERY_TEMPTURE_4_7,Event_OnGetBatteryTempture_4_7);
     EVENT_MAP(WM_GET_BATTERY_TEMPTURE_4_8,Event_OnGetBatteryTempture_4_8);
     EVENT_MAP(WM_GET_BATTERY_TEMPTURE_4_9,Event_OnGetBatteryTempture_4_9);
     EVENT_MAP(WM_GET_BATTERY_TEMPTURE_4_10,Event_OnGetBatteryTempture_4_10);
     EVENT_MAP(WM_GET_BATTERY_TEMPTURE_4_11,Event_OnGetBatteryTempture_4_11);
     EVENT_MAP(WM_GET_BATTERY_TEMPTURE_4_12,Event_OnGetBatteryTempture_4_12);
     EVENT_MAP(WM_GET_BATTERY_TEMPTURE_4_13,Event_OnGetBatteryTempture_4_13);
     EVENT_MAP(WM_GET_BATTERY_TEMPTURE_4_14,Event_OnGetBatteryTempture_4_14);
     EVENT_MAP(WM_GET_BATTERY_TEMPTURE_4_15,Event_OnGetBatteryTempture_4_15);
     EVENT_MAP(WM_GET_BATTERY_TEMPTURE_4_16,Event_OnGetBatteryTempture_4_16);

     EVENT_MAP(WM_GET_BATTERY_TEMPTURE_5_1,Event_OnGetBatteryTempture_5_1);
     EVENT_MAP(WM_GET_BATTERY_TEMPTURE_5_2,Event_OnGetBatteryTempture_5_2);
     EVENT_MAP(WM_GET_BATTERY_TEMPTURE_5_3,Event_OnGetBatteryTempture_5_3);
     EVENT_MAP(WM_GET_BATTERY_TEMPTURE_5_4,Event_OnGetBatteryTempture_5_4);
     EVENT_MAP(WM_GET_BATTERY_TEMPTURE_5_5,Event_OnGetBatteryTempture_5_5);
     EVENT_MAP(WM_GET_BATTERY_TEMPTURE_5_6,Event_OnGetBatteryTempture_5_6);
     EVENT_MAP(WM_GET_BATTERY_TEMPTURE_5_7,Event_OnGetBatteryTempture_5_7);
     EVENT_MAP(WM_GET_BATTERY_TEMPTURE_5_8,Event_OnGetBatteryTempture_5_8);
     EVENT_MAP(WM_GET_BATTERY_TEMPTURE_5_9,Event_OnGetBatteryTempture_5_9);
     EVENT_MAP(WM_GET_BATTERY_TEMPTURE_5_10,Event_OnGetBatteryTempture_5_10);
     EVENT_MAP(WM_GET_BATTERY_TEMPTURE_5_11,Event_OnGetBatteryTempture_5_11);
     EVENT_MAP(WM_GET_BATTERY_TEMPTURE_5_12,Event_OnGetBatteryTempture_5_12);
     EVENT_MAP(WM_GET_BATTERY_TEMPTURE_5_13,Event_OnGetBatteryTempture_5_13);
     EVENT_MAP(WM_GET_BATTERY_TEMPTURE_5_14,Event_OnGetBatteryTempture_5_14);
     EVENT_MAP(WM_GET_BATTERY_TEMPTURE_5_15,Event_OnGetBatteryTempture_5_15);
     EVENT_MAP(WM_GET_BATTERY_TEMPTURE_5_16,Event_OnGetBatteryTempture_5_16);

     EVENT_MAP(WM_GET_BATTERY_TEMPTURE_6_1,Event_OnGetBatteryTempture_6_1);
     EVENT_MAP(WM_GET_BATTERY_TEMPTURE_6_2,Event_OnGetBatteryTempture_6_2);
     EVENT_MAP(WM_GET_BATTERY_TEMPTURE_6_3,Event_OnGetBatteryTempture_6_3);
     EVENT_MAP(WM_GET_BATTERY_TEMPTURE_6_4,Event_OnGetBatteryTempture_6_4);
     EVENT_MAP(WM_GET_BATTERY_TEMPTURE_6_5,Event_OnGetBatteryTempture_6_5);
     EVENT_MAP(WM_GET_BATTERY_TEMPTURE_6_6,Event_OnGetBatteryTempture_6_6);
     EVENT_MAP(WM_GET_BATTERY_TEMPTURE_6_7,Event_OnGetBatteryTempture_6_7);
     EVENT_MAP(WM_GET_BATTERY_TEMPTURE_6_8,Event_OnGetBatteryTempture_6_8);
     EVENT_MAP(WM_GET_BATTERY_TEMPTURE_6_9,Event_OnGetBatteryTempture_6_9);
     EVENT_MAP(WM_GET_BATTERY_TEMPTURE_6_10,Event_OnGetBatteryTempture_6_10);
     EVENT_MAP(WM_GET_BATTERY_TEMPTURE_6_11,Event_OnGetBatteryTempture_6_11);
     EVENT_MAP(WM_GET_BATTERY_TEMPTURE_6_12,Event_OnGetBatteryTempture_6_12);
     EVENT_MAP(WM_GET_BATTERY_TEMPTURE_6_13,Event_OnGetBatteryTempture_6_13);
     EVENT_MAP(WM_GET_BATTERY_TEMPTURE_6_14,Event_OnGetBatteryTempture_6_14);
     EVENT_MAP(WM_GET_BATTERY_TEMPTURE_6_15,Event_OnGetBatteryTempture_6_15);
     EVENT_MAP(WM_GET_BATTERY_TEMPTURE_6_16,Event_OnGetBatteryTempture_6_16);

     EVENT_MAP(WM_GET_BATTERY_TEMPTURE_7_1,Event_OnGetBatteryTempture_7_1);
     EVENT_MAP(WM_GET_BATTERY_TEMPTURE_7_2,Event_OnGetBatteryTempture_7_2);
     EVENT_MAP(WM_GET_BATTERY_TEMPTURE_7_3,Event_OnGetBatteryTempture_7_3);
     EVENT_MAP(WM_GET_BATTERY_TEMPTURE_7_4,Event_OnGetBatteryTempture_7_4);
     EVENT_MAP(WM_GET_BATTERY_TEMPTURE_7_5,Event_OnGetBatteryTempture_7_5);
     EVENT_MAP(WM_GET_BATTERY_TEMPTURE_7_6,Event_OnGetBatteryTempture_7_6);
     EVENT_MAP(WM_GET_BATTERY_TEMPTURE_7_7,Event_OnGetBatteryTempture_7_7);
     EVENT_MAP(WM_GET_BATTERY_TEMPTURE_7_8,Event_OnGetBatteryTempture_7_8);
     EVENT_MAP(WM_GET_BATTERY_TEMPTURE_7_9,Event_OnGetBatteryTempture_7_9);
     EVENT_MAP(WM_GET_BATTERY_TEMPTURE_7_10,Event_OnGetBatteryTempture_7_10);
     EVENT_MAP(WM_GET_BATTERY_TEMPTURE_7_11,Event_OnGetBatteryTempture_7_11);
     EVENT_MAP(WM_GET_BATTERY_TEMPTURE_7_12,Event_OnGetBatteryTempture_7_12);
     EVENT_MAP(WM_GET_BATTERY_TEMPTURE_7_13,Event_OnGetBatteryTempture_7_13);
     EVENT_MAP(WM_GET_BATTERY_TEMPTURE_7_14,Event_OnGetBatteryTempture_7_14);
     EVENT_MAP(WM_GET_BATTERY_TEMPTURE_7_15,Event_OnGetBatteryTempture_7_15);
     EVENT_MAP(WM_GET_BATTERY_TEMPTURE_7_16,Event_OnGetBatteryTempture_7_16);

     EVENT_MAP(WM_BOARD_1_END,Event_OnBoard_1_End);
     EVENT_MAP(WM_BOARD_2_END,Event_OnBoard_2_End);
     EVENT_MAP(WM_BOARD_3_END,Event_OnBoard_3_End);
     EVENT_MAP(WM_BOARD_4_END,Event_OnBoard_4_End);
     EVENT_MAP(WM_BOARD_5_END,Event_OnBoard_5_End);
     EVENT_MAP(WM_BOARD_6_END,Event_OnBoard_6_End);
     EVENT_MAP(WM_BOARD_7_END,Event_OnBoard_7_End);

     EVENT_MAP(WM_GET_SECOND,Event_OnGetSecond);
     EVENT_MAP(WM_GET_MINUTE,Event_OnGetMinute);
     EVENT_MAP(WM_GET_HOUR,Event_OnGetHour);
     EVENT_MAP(WM_GET_DAY,Event_OnGetDay);
     EVENT_MAP(WM_GET_MONTH,Event_OnGetMonth);
     EVENT_MAP(WM_GET_YEAR,Event_OnGetYear);

     EVENT_MAP(WM_SET_SECOND,Event_OnSetSecond);
     EVENT_MAP(WM_SET_MINUTE,Event_OnSetMinute);
     EVENT_MAP(WM_SET_HOUR,Event_OnSetHour);
     EVENT_MAP(WM_SET_DAY,Event_OnSetDay);
     EVENT_MAP(WM_SET_MONTH,Event_OnSetMonth);
     EVENT_MAP(WM_SET_YEAR,Event_OnSetYear);

     EVENT_MAP(WM_BOARD_1_CHECKEND,Event_OnCheckBoard_1_End);
     EVENT_MAP(WM_BOARD_2_CHECKEND,Event_OnCheckBoard_2_End);
     EVENT_MAP(WM_BOARD_3_CHECKEND,Event_OnCheckBoard_3_End);
     EVENT_MAP(WM_BOARD_4_CHECKEND,Event_OnCheckBoard_4_End);
     EVENT_MAP(WM_BOARD_5_CHECKEND,Event_OnCheckBoard_5_End);
     EVENT_MAP(WM_BOARD_6_CHECKEND,Event_OnCheckBoard_6_End);
     EVENT_MAP(WM_BOARD_7_CHECKEND,Event_OnCheckBoard_7_End);

     EVENT_MAP(WM_GETBLOCK_EEPROM,Event_OnGetBlockEEPROM);

     EVENT_MAP(WM_WRITE_DATA_CRC,Event_OnSetDataCRC);
     EVENT_MAP(WM_WRITE_DATA_ALL_1,Event_OnSetDataAll_1);
     EVENT_MAP(WM_WRITE_DATA_ALL_2,Event_OnSetDataAll_2);
     EVENT_MAP(WM_WRITE_DATA_ALL_3,Event_OnSetDataAll_3);
     EVENT_MAP(WM_WRITE_DATA_ALL_4,Event_OnSetDataAll_4);
     EVENT_MAP(WM_WRITE_DATA_ALL_5,Event_OnSetDataAll_5);
     EVENT_MAP(WM_WRITE_DATA_ALL_6,Event_OnSetDataAll_6);
     EVENT_MAP(WM_WRITE_DATA_ALL_7,Event_OnSetDataAll_7);
     EVENT_MAP(WM_WRITE_DATA_ALL_8,Event_OnSetDataAll_8);
     EVENT_MAP(WM_WRITE_DATA_ALL_9,Event_OnSetDataAll_9);
     EVENT_MAP(WM_WRITE_DATA_ALL_10,Event_OnSetDataAll_10);
     EVENT_MAP(WM_WRITE_DATA_ALL_11,Event_OnSetDataAll_11);
     EVENT_MAP(WM_WRITE_DATA_ALL_12,Event_OnSetDataAll_12);
     EVENT_MAP(WM_WRITE_DATA_ALL_13,Event_OnSetDataAll_13);
     EVENT_MAP(WM_WRITE_DATA_ALL_14,Event_OnSetDataAll_14);
     EVENT_MAP(WM_WRITE_DATA_ALL_15,Event_OnSetDataAll_15);
     EVENT_MAP(WM_WRITE_DATA_ALL_16,Event_OnSetDataAll_16);
     EVENT_MAP(WM_WRITE_DATA_ALL_17,Event_OnSetDataAll_17);
     EVENT_MAP(WM_WRITE_DATA_ALL_18,Event_OnSetDataAll_18);
     EVENT_MAP(WM_WRITE_DATA_ALL_19,Event_OnSetDataAll_19);
     EVENT_MAP(WM_WRITE_DATA_ALL_20,Event_OnSetDataAll_20);
     EVENT_MAP(WM_WRITE_DATA_ALL_21,Event_OnSetDataAll_21);
     EVENT_MAP(WM_WRITE_DATA_ALL_22,Event_OnSetDataAll_22);
     EVENT_MAP(WM_WRITE_DATA_ALL_23,Event_OnSetDataAll_23);
     EVENT_MAP(WM_WRITE_DATA_ALL_24,Event_OnSetDataAll_24);
     EVENT_MAP(WM_READ_DATA_ALL,Event_OnGetDataAll);
     EVENT_MAP(WM_SHARE_MEMERY_TO_EEPROM,Event_OnShareMemeryToEEPROM);
     EVENT_MAP(WM_EEPROM_TO_SHARE_MEMERY,Event_OnEEPROMToShareMemery);
     EVENT_MAP(WM_EMPTY_ERROR,Event_OnEmptyError);
     EVENT_MAP(WM_GET_OUT_TIME,Event_OnGetOutTimer);
     EVENT_MAP(WM_SET_OUT_TIME,Event_OnSetOutTimer);
  }
}
