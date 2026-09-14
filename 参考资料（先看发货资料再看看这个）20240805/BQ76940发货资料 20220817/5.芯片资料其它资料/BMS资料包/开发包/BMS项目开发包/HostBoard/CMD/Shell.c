/*
 * Shell.c
 *
 *  Created on: May 21, 2010
 *      Author: Administrator
 */

/*
 * main.c
 *
 *  Created on: May 18, 2010
 *      Author: Administrator
 */
#ifndef IN_LIB_CAN
#define IN_LIB_CAN
#endif
#include "cmd.h"
#include "stdio.h"
#include "cmd_test.h"
#include "Cmd_call.h"
#include "Cmd_jmp.h"
#include "Cmd_math.h"
#ifdef EXTERN_LIB_CAN
#include "../include/CAN.h"
#endif
#ifdef IN_LIB_CAN
#include "CAN.h"
#endif
#include "string.h"
#include "../include/EEPROM.h"
#include "File.h"
#include "CANSendbuffer.h"
#include "current.h"
#include "Timer_Manager.h"
#include "cmd_code.h"
#include "XC22xxMREGS.h"
#include "Battery.h"
#include "CAN_Task.h"

int HexToInt(char* input)
{
	int index=2;
	int returnValue=0;

	if(input[0]=='0'&&input[1]=='x')
	{
		for(index=2;index<CMD_TABLE_LENGTH;index++)
		{
			if(input[index]<='9'&&input[index]>='0')
			{
				returnValue<<=4;
				returnValue+=input[index]-'0';
			}
			else
			{
				switch(input[index])
				{
				case 'a':
				case 'A':
					returnValue<<=4;
					returnValue+=10;
					break;
				case 'b':
				case 'B':
					returnValue<<=4;
					returnValue+=11;
					break;
				case 'c':
				case 'C':
					returnValue<<=4;
					returnValue+=12;
					break;
				case 'd':
				case 'D':
					returnValue<<=4;
					returnValue+=13;
					break;
				case 'e':
				case 'E':
					returnValue<<=4;
					returnValue+=14;
					break;
				case 'f':
				case 'F':
					returnValue<<=4;
					returnValue+=15;
					break;
				default:
					return returnValue;
				}
			}

		}
	}

	return returnValue;

}
int AddERROR(unsigned char value)
{
	return 1;
}
void CANReceiveInterruptProcess(unsigned char volatile __far* dataBuffer)
{

}




void Shell_Init()
{
	CMD_JMPInit();
	CMD_MathInit();
	FILE_Init();
	CANSendBuffer_Init();
	CAN_Task_Init();
	Current_Init();
	Timer_Init_2();
	CMD_CodeInit();
	Battery_Init();
	CMD_FuncionInit();
}
void Shell_Run()
{
	char input[CMD_TABLE_LENGTH],output[CMD_TABLE_LENGTH];
	int returnValue;
	while(1)
	{
		printf("\nPlease Input:");
		gets(input);
		CMD_ReconstructString(input,output);
		returnValue=CMD_Do(output);
		if(returnValue==CMD_ERROR)
			printf("ERROR!\n");
	}
}
