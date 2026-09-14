/*
 * CMD_Scan.c
 *
 *  Created on: May 21, 2010
 *      Author: Administrator
 */
#include "Cmd.h"
#include "string.h"
#include "stdio.h"
#include "ctype.h"
#include "cmd_scan.h"
extern char m_table_1[CMD_TABLE_SIZE][CMD_TABLE_LENGTH];
extern char m_table_2[CMD_TABLE_SIZE][CMD_TABLE_LENGTH];
extern int m_table_index;
extern char m_tempCmdString[CMD_TABLE_LENGTH];

int CMD_Scan(char* cmdString)
{
	char inString[CMD_TABLE_LENGTH],outString[CMD_TABLE_LENGTH];
	char *pVariours;
	int CMD_Return;
	CMD_Return=CMD_ScanDevide(cmdString,&pVariours);
	gets(inString);
	CMD_ReconstructString(inString,outString);
	if(CMD_Table(pVariours)!=0)
		CMD_ChangeTable(pVariours,outString);
	else
		CMD_ADDTable(pVariours,outString);
	return CMD_Return;

}
int CMD_ScanDevide(char* cmdString,char** returnValue)
{
	char* tableString;
	int index=0;
	char printBuffer[24];
	index+=2;//filter '>>'

	if(cmdString[index]=='$')
	{

		*returnValue=cmdString+index+1;
	}
	else
		return CMD_ERROR;
	return CMD_OK;
}
