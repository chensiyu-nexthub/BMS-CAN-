/*
 * CMD_Echo.c
 *
 *  Created on: May 19, 2010
 *      Author: Administrator
 */
#include "Cmd.h"
#include "string.h"
#include "stdio.h"
#include "ctype.h"

extern char m_table_1[CMD_TABLE_SIZE][CMD_TABLE_LENGTH];
extern char m_table_2[CMD_TABLE_SIZE][CMD_TABLE_LENGTH];
extern int m_table_index;
extern char m_tempCmdString[CMD_TABLE_LENGTH];

int CMD_Echo(char* cmdString)
{
	char* echoString;
	int CMD_Return;
	CMD_Return=CMD_EchoDevide(cmdString,&echoString);
	if(CMD_Return==CMD_ERROR)
		return CMD_ERROR;
	else
		printf("%s\n",echoString);
	return CMD_Return;

}
int CMD_EchoDevide(char* cmdString,char** returnValue)
{
	char* tableString;
	int index=0;
	char echoBuffer[24];
	index++;//filter '>'
	strncpy(echoBuffer,cmdString+1,4);
	echoBuffer[4]=0;
	CMD_UpString(echoBuffer);
	if(strcmp(echoBuffer,"ECHO")!=0)
		return CMD_ERROR;
	index+=4;
	if(cmdString[index]=='$')
	{

		*returnValue=CMD_Replace(cmdString+index);
	}
	else if(cmdString[index]=='\"')
	{
		index++;
		*returnValue=cmdString+index;
		if(cmdString[index]==0)
			return CMD_ERROR;
		while(cmdString[index]!='\"')
		{
			if(cmdString[index]==0)
				return CMD_ERROR;
			index++;
		}
		cmdString[index]=0;
	}
	cmdString[index]=0;
	index++;
	return CMD_OK;
}
