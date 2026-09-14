/*
 * main.c
 *
 *  Created on: May 18, 2010
 *      Author: Administrator
 */
#include "cmd.h"
#include "stdio.h"
#include "cmd_test.h"
#include "Cmd_call.h"
#include "Cmd_jmp.h"
#include "Cmd_math.h"
#include "Frequency.h"
#include "CAN.h"
#include "Error.h"
#include "Shell.h"
#include "Timer_Manager.h"
#include "../include/Calendar.h"
#include "../include/EEPROM.h"
#include "../include/RTC.h"
#include "CAN.h"
#include "../include/ADC.h"
#include "../include/Relay.h"
#include "QQ_Test.h"
#include "CAN_Task.h"
#include "stdio.h"
#include "string.h"
#include "MainProcess.h"
#include "Event.h"
#include "ShareMemery.h"

void main()
{
	/*Calendar_Init(Error_AddError,0xffff);
	while(1)
	{
		Event_Progress(WM_GET_OUT_TIME);
	}*/
	Module_InitStart();
	Shell_Init();
#ifdef SEHLL_MODULE
	CAN_Init(AddERROR,CANReceiveInterruptProcess,0xff);
#else
	CAN_Init(AddERROR,CAN_Task_DoOrder,0xffff);
#endif
	Calendar_Init(Error_AddError,0xffff);
	EEPROM_Init(Error_AddError,0xffff);
#ifdef CAN_3
	CAN_Init(Error_AddError,CAN_Task_DoOrder,CANSendBuffer_Complete,0xffff);
#endif
	Timer_Init(Timer_Manager_ProcessInterrupt);
	ADC_Init(Error_AddError,0xffff);
	Relay_Init(Error_AddError,0xffff);
	ShareMemery_vInit();
	Module_InitEnd();

	//QQ_Test_MainLoop();
	//MainLoop();
	//Shell_Init();

	//Shell_Run();
	MainProcess();
}
