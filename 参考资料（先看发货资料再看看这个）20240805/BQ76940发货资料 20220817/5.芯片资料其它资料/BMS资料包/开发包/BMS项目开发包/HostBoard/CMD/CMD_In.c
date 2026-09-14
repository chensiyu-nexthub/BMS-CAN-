/*
 * CMD_In.c
 *
 *  Created on: May 19, 2010
 *      Author: Administrator
 */
#include "CMD.h"
#include "string.h"
#include "stdio.h"
#include "ctype.h"
#include "CMD_In.h"

extern char m_table_1[CMD_TABLE_SIZE][CMD_TABLE_LENGTH];
extern char m_table_2[CMD_TABLE_SIZE][CMD_TABLE_LENGTH];
extern int m_table_index;
extern char m_tempCmdString[CMD_TABLE_LENGTH];

int CMD_In(char* cmdString)
{
	char inString[CMD_TABLE_LENGTH],outString[CMD_TABLE_LENGTH];
	char *pVariours;
	int CMD_Return;
	CMD_Return=CMD_InDevide(cmdString,&pVariours);
	gets(inString);
	CMD_ReconstructString(inString,outString);
	if(CMD_Table(pVariours)!=0)
		CMD_ChangeTable(pVariours,outString);
	else
		CMD_ADDTable(pVariours,outString);
	return CMD_Return;
}
int CMD_InDevide(char* cmdString,char** returnValue)
{
	char* tableString;
	int index=0;
	char inBuffer[24];
	index++;//filter '>'
	strncpy(inBuffer,cmdString+1,2);
	inBuffer[2]=0;
	CMD_UpString(inBuffer);
	if(strcmp(inBuffer,"IN")!=0)
		return CMD_ERROR;
	index+=2;
	if(cmdString[index]=='$')
	{
		index++;
		if(cmdString[index]==0)
			return CMD_ERROR;
		else
			*returnValue=cmdString+index;
	}
	return CMD_OK;
}
