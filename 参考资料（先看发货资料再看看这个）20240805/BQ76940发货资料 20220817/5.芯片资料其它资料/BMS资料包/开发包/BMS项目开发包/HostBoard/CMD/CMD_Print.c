/*
 * CMD_Print.c
 *
 *  Created on: May 21, 2010
 *      Author: Administrator
 */
#include "Cmd.h"
#include "string.h"
#include "stdio.h"
#include "ctype.h"
#include "CMD_Call.h"

extern char m_table_1[CMD_TABLE_SIZE][CMD_TABLE_LENGTH];
extern char m_table_2[CMD_TABLE_SIZE][CMD_TABLE_LENGTH];
extern int m_table_index;
extern char m_tempCmdString[CMD_TABLE_LENGTH];

int CMD_Print(char* cmdString)
{
	char* printString;
	int CMD_Return;
	CMD_Return=CMD_PrintDevide(cmdString,&printString);
	if(CMD_Return==CMD_ERROR)
		return CMD_ERROR;
	else
		printf("%s\n",printString);
	return CMD_Return;

}
int CMD_PrintDevide(char* cmdString,char** returnValue)
{
	char* tableString;
	int index=0;
	char printBuffer[24];
	index+=2;//filter '<<'

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
	else
	{
		char *pVariours_1,*pVariours_2,*pVariours_3,*pVariours_4;
		char *pFunction;
		int CMD_Retrun;
		pF_CMDFuntion function;
		CMD_Retrun=CMD_FunctionDevide(cmdString+index,
				&pFunction,&pVariours_1,&pVariours_2,&pVariours_3,&pVariours_4);
		if(CMD_Retrun!=CMD_OK)
			return CMD_Retrun;
		function=CMD_Function(pFunction);
		if(function==0)
			return CMD_ERROR;
		pVariours_1=CMD_Replace(pVariours_1);
		pVariours_2=CMD_Replace(pVariours_2);
		pVariours_3=CMD_Replace(pVariours_3);
		pVariours_4=CMD_Replace(pVariours_4);

		*returnValue=function(pVariours_1,pVariours_2,pVariours_3,pVariours_4);
	}

	return CMD_OK;
}
