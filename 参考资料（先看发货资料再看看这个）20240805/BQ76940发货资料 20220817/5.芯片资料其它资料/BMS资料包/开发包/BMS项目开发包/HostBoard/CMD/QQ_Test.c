/*
 * QQ_Test.c
 *
 *  Created on: May 7, 2010
 *      Author: Administrator
 */
#include "QQ_Test.h"
#include "WDT.h"
#include "../include/main.h"
#include "../include/Calendar.h"
#include "../include/EEPROM.h"
#include "../include/RTC.h"
#include "CAN.h"
#include "../include/ADC.h"
#include "../include/Relay.h"
#include "XC22xxMREGS.h"
#include "Error.h"
#include "Event.h"
#include "CAN_Task.h"
#include "WAVE.h"
#include "current.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"

ubyte Test_EEPROM_Receive[1024];
ubyte Test_EEPROM_OK;
uword Test_EEPROM_Status;
ubyte Test_EEPROM_Send[1024];
uword Test_ADC_Buffer[11];
double Test_Current;
uword Test_Board1_Temptrue[8];
uword Test_Board2_Temptrue[8];

uword Test_Board1_BatteryVoltage[16];
uword Test_Board2_BatteryVoltage[16];

ubyte Test_Calendar_year,Test_Calendar_month,Test_Calendar_day,Test_Calendar_minute,Test_Calendar_hour,Test_Calendar_second;
ubyte Test_Calendar_flag;
ubyte Test_Relay_Plus;
ubyte Test_Relay_Neg;
ubyte Test_Relay_Pre;
ubyte Test_Board1End=0;
ubyte Test_Board2End=0;
extern unsigned int g_batteryStateArray[7][16];
extern unsigned int g_batteryVoltageArray[7][16];
extern unsigned int g_batteryTemptureArray[7][16];
extern unsigned int g_batteryEmptyArray[7];
unsigned char g_FlagEmptyBatteryBoard_1_End=0;
unsigned char g_FlagEmptyBatteryBoard_2_End=0;
unsigned char g_FlagEmptyBatteryBoard_3_End=0;
unsigned char g_FlagEmptyBatteryBoard_4_End=0;
unsigned char g_FlagEmptyBatteryBoard_5_End=0;
unsigned char g_FlagEmptyBatteryBoard_6_End=0;
unsigned char g_FlagEmptyBatteryBoard_7_End=0;

extern 	unsigned char g_Flag_Board_1_End;
extern 	unsigned char g_Flag_Board_2_End;
extern 	unsigned char g_Flag_Board_3_End;
extern 	unsigned char g_Flag_Board_4_End;
extern 	unsigned char g_Flag_Board_5_End;
extern 	unsigned char g_Flag_Board_6_End;
extern 	unsigned char g_Flag_Board_7_End;
extern  double Current_Buffer[200];
void QQ_Test_TestCalendar()
{
	ubyte yead,month,day;

	//system.m_calendar.Calendar_SetDate(1,11,15,0,0,0);
	//system.Delay(0xffff);
	Test_Calendar_day=5;

	Calendar_LoadDate();

	Test_Calendar_day=Calendar_GetDay();

	Test_Calendar_year=Calendar_GetYear();

	Test_Calendar_month=Calendar_GetMonth();

	Test_Calendar_hour=Calendar_GetHour();

	Test_Calendar_minute=Calendar_GetMinute();

	Test_Calendar_second=Calendar_GetSecond();

}
void QQ_Test_SelfTest()
{
	PSW_IEN=0;//Enable interrupt.
	QQ_Test_TestCalendar();//Load calendar data.
	PSW_IEN=1;//Disable interrupt.

}
static unsigned char QQ_Test_IsBoard_1_End()
{
	return g_Flag_Board_1_End;
}
static unsigned char QQ_Test_IsBoard_2_End()
{
	return g_Flag_Board_2_End;
}
static unsigned char QQ_Test_IsBoard_1_CheckEnd()
{
	return g_FlagEmptyBatteryBoard_1_End;
}
static unsigned char QQ_Test_IsBoard_2_CheckEnd()
{
	return g_FlagEmptyBatteryBoard_2_End;
}
static void QQ_Test_CheckEmptyBattery()
{
	unsigned int* p;
	unsigned char i=0;

	unsigned char Board1CheckEmptyBattery[8]={SAMPLE_BOARD1,WM_CHECK_EMPTY_BATTERY,0};
	unsigned char Board2CheckEmptyBattery[8]={SAMPLE_BOARD2,WM_CHECK_EMPTY_BATTERY,0};

	RTC_vRTCRun();


	Timer_SetTimer(0);

	g_FlagEmptyBatteryBoard_1_End=0;
	g_FlagEmptyBatteryBoard_2_End=0;

		//while(CAN_ubNewData(1)!=1);
	CAN_Send(Board1CheckEmptyBattery);//

		//while(CAN_ubNewData(1)!=1);

	Wave_Change_3();
	PSW_IEN=0;
	p=ADC_GetChannelArray();
	PSW_IEN=1;

	for(i=0;i<11;i++ )
		Test_ADC_Buffer[i]=p[i];

	CAN_Send(Board2CheckEmptyBattery);//
	Wave_Change_3();

		//WDT_vServiceWDT();

	while(Timer_GetCounter()<6250);

#ifdef EVENT_ADDPROCESS
	while(Event_IsEmpty()!=1)
	{
		 Event_Progress(Event_GetEvent());
	}
#endif
	if(QQ_Test_IsBoard_1_CheckEnd()!=1)
	{
		Error_AddError(ERROR_NO_ANSWER_BOARD_1);
			//CAN_Init(Error_AddError,CAN_Task_DoOrder,0xffff);
	}

	if(QQ_Test_IsBoard_2_CheckEnd()!=1)
	{
		Error_AddError(ERROR_NO_ANSWER_BOARD_2);
			//CAN_Init(Error_AddError,CAN_Task_DoOrder,0xffff);
	}

		//WDT_vServiceWDT();

}

void QQ_Test_MainLoop()
{
	unsigned int* p;
	int i=0;
	int loopCounter=PRINT_COUNTER;
	unsigned char j=0;
	double voltage;
	unsigned char Board1StartEqual[8]={SAMPLE_BOARD1,WM_START_EQUAL,0};
	unsigned char Board2StartEqual[8]={SAMPLE_BOARD2,WM_START_EQUAL,0};
	char printValue[20];
	//QQ_Test_SelfTest();
	unsigned int voltageHigh,voltageLow;
	RTC_vRTCRun();

	while(1)
	{
		Timer_SetTimer(0);

		g_Flag_Board_1_End=0;
		g_Flag_Board_1_End=0;

		//CAN_ChangeSendID(0x1);
		//while(CAN_ubNewData(1)!=1);
		CAN_Send(Board1StartEqual);//
		while(CAN_Task_GetStatus()&2!=2);
		//CAN_ChangeSendID(0x111);
		//while(CAN_ubNewData(1)!=1);
		CAN_Send(Board2StartEqual);//
		while(CAN_Task_GetStatus()&2!=2);
		Wave_Change_3();
		PSW_IEN=0;

#ifdef __NORMAL
		p=ADC_GetChannelArray();

		Current_Start();
		for(i=0;i<11;i++ )
			Test_ADC_Buffer[i]=p[i];
#endif

		PSW_IEN=1;

		Wave_Change_3();

		//WDT_vServiceWDT();
		/*
		if(loopCounter--<0)
		{
			loopCounter=PRINT_COUNTER;

			Current_Start(0,0,0,0);
			printf("\n");
			for(i=0;i<16;i++)
			{
				voltageHigh=g_batteryVoltageArray[0][i]&0xff;
				voltageLow=g_batteryVoltageArray[0][i]&0xff00;
				voltageLow>>=8;
				voltageHigh<<=8;
				voltage=voltageLow+voltageHigh;
				voltage*=0.001221;
				printf("V0[%d]=%f; ",i,voltage);
				if((i+1)%4==0)
					printf("\n");
			}
			for(i=0;i<8;i++)
			{
				voltageHigh=g_batteryVoltageArray[1][i]&0xff;
				voltageLow=g_batteryVoltageArray[1][i]&0xff00;
				voltageLow>>=8;
				voltageHigh<<=8;
				voltage=voltageLow+voltageHigh;
				voltage*=0.001221;
				printf("V1[%d]=%f; ",i,voltage);
				if((i+1)%4==0)
					printf("\n");
			}
			voltageHigh=g_batteryVoltageArray[1][15]&0xff;
			voltageLow=g_batteryVoltageArray[1][15]&0xff00;
			voltageLow>>=8;
			voltageHigh<<=8;
			voltage=voltageLow+voltageHigh;
			voltage*=0.001221;
			printf("V1[%d]=%f; ",15,voltage);

			printf("\n");
		}
*/
		while(Timer_GetCounter()<6250);

#ifdef EVENT_ADDPROCESS
		while(Event_IsEmpty()!=1)
		{
		  Event_Progress(Event_GetEvent());
		}
#endif
		if(QQ_Test_IsBoard_1_End()!=1)
		{
			Error_AddError(ERROR_NO_ANSWER_BOARD_1);
			//CAN_Init(Error_AddError,CAN_Task_DoOrder,0xffff);
		}

		if(QQ_Test_IsBoard_2_End()!=1)
		{
			Error_AddError(ERROR_NO_ANSWER_BOARD_2);
			//CAN_Init(Error_AddError,CAN_Task_DoOrder,0xffff);
		}

		//WDT_vServiceWDT();
	}
}
