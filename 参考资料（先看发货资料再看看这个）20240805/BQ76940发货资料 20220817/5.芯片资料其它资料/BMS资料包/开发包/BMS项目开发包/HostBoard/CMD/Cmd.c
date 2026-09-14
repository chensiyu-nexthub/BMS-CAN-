/*
 * Cmd.c
 *
 *  Created on: May 18, 2010
 *      Author: Administrator
 */
#include "string.h"
#include "stdio.h"
#include "ctype.h"
#include "cmd.h"
#include "cmd_print.h"
#include "cmd_scan.h"
#include "CMD_In.h"
#include "CMD_Echo.h"
#include "CMD_CALL.h"

char m_table_1[CMD_TABLE_SIZE][CMD_TABLE_LENGTH];
char m_table_2[CMD_TABLE_SIZE][CMD_TABLE_LENGTH];
int m_table_index=-1;
char m_tempCmdString[CMD_TABLE_LENGTH];

void CMD_ADDTable(char* inputString_1,char* inputString_2)
{
	m_table_index++;
	if(m_table_index>=CMD_TABLE_SIZE)
		m_table_index=0;
	strcpy(m_table_1[m_table_index],inputString_1);
	strcpy(m_table_2[m_table_index],inputString_2);
}
int CMD_TableIndex(char* inputString)
{
	int index=0;
	for(index=0;index<CMD_TABLE_SIZE;index++)
	{
		if(strcmp(m_table_1[index],inputString)==0)
			return index;
	}
	return -1;
}
void CMD_ChangeTable(char* inputString_1,char* inputString_2)
{
	int index;
	index=CMD_TableIndex(inputString_1);
	strcpy(m_table_1[index],inputString_1);
	strcpy(m_table_2[index],inputString_2);
}
char* CMD_Table(char* inputString)
{
	int index=0;
	for(index=0;index<CMD_TABLE_SIZE;index++)
	{
		if(strcmp(m_table_1[index],inputString)==0)
			return m_table_2[index];
	}
	return 0;
}
int CMD_Local_Do(char* pFunction,char* pVariours_1,char* pVariours_2,char* pVariours_3,char* pVariour_4)
{
	return 1;
}
int CMD_Remote_Do(char* pFunction,char* pVariours_1,char* pVariours_2,char* pVariours_3,char* pVariour_4)
{
	return 1;
}
char* CMD_Replace(char* inputString)
{
	if(inputString[0]=='$')
		return CMD_Table(inputString+1);
	else
		return inputString;
}

char CMD_UpChar(char input)
{
	char returnValue=input;
	if('a'<=input&&input<='z')
	{
		returnValue-=32;
	}
	return returnValue;
}
void CMD_UpString(char* input)
{
	int index=0;
	char temp;
	while(input[index]!=0)
	{
		if(index==CMD_TABLE_LENGTH)
			return;
		temp=input[index];
		temp=CMD_UpChar(temp);
		input[index]=temp;
		index++;
	}
}
//For example,string "Host/StartTimer","Board1/Release(1)"
//For example,string "#_V=0" then "$_V=Host/GetTimer" and then ">ECHO$_V",will print the timer.
int CMD_Do(char* cmdString)
{
	int CMD_Return;
	char* returnVariours,*pFunction,*variours_1,
	*variours_2,*variours_3,*variours_4;
	strncpy(m_tempCmdString,cmdString,CMD_TABLE_LENGTH);
	if(m_tempCmdString[0]=='#')
	{
		CMD_Return=CMD_DefineDevide(m_tempCmdString,&variours_1,&variours_2);
		if(CMD_Return==CMD_ERROR)
			return CMD_ERROR;
		else
		{
			if(CMD_Table(variours_1)!=0)
				CMD_ChangeTable(variours_1,variours_2);
			else
				CMD_ADDTable(variours_1,variours_2);
		}
	}
	else if(m_tempCmdString[0]=='$')
	{
		CMD_Return=CMD_CallEx(m_tempCmdString);
			return CMD_Return;
	}
	else if(m_tempCmdString[0]=='>')
	{
		if(m_tempCmdString[1]=='e'||m_tempCmdString[1]=='E')
			CMD_Return=CMD_Echo(m_tempCmdString);
		else if(m_tempCmdString[1]=='i'||m_tempCmdString[1]=='I')
			CMD_Return=CMD_In(m_tempCmdString);
		else if(m_tempCmdString[1]=='>')
			CMD_Return=CMD_Scan(m_tempCmdString);
		return CMD_Return;
	}
	else if(m_tempCmdString[0]=='<'&&m_tempCmdString[1]=='<')
	{
		CMD_Return=CMD_Print(m_tempCmdString);
		return CMD_Return;
	}
	else
	{
		CMD_Return=CMD_Call(m_tempCmdString);
		return CMD_Return;

	}
}
void CMD_ReconstructString(char* input,char* output)
{
	int index=0;
	int top=0;
	while(1)
	{
		if(index>=CMD_TABLE_LENGTH)
		{
			output[top++]=0;
			return;
		}
		switch(input[index])
		{
		case '\r':
			output[top]=0;
			return;
			break;
		case 0:
			output[top]=0;
			top++;
			return;
			break;
		case 8:
			top--;
			index++;
			break;
		default:
			output[top]=input[index];
			top++;
			index++;
			break;
		}
	}
}
