/*
 * CMD_Code.c
 *
 *  Created on: May 20, 2010
 *      Author: Administrator
 */
#include "cmd.h"
#include "string.h"
#include "cmd_code.h"
#include "string.h"
#include "stdio.h"
#include "cmd_call.h"
#include "stdlib.h"

char m_CMD_CodeTable[CMD_CODE_SIZE][CMD_CODE_LENGTH];
int m_CMD_Index=-1;
int m_CMD_Code_Index;

void CMD_ADDCode(char* codeString)
{
	m_CMD_Index++;
	if(m_CMD_Index>=CMD_CODE_SIZE-1)
		m_CMD_Index=0;
	strcpy(m_CMD_CodeTable[m_CMD_Index],codeString);
	strcpy(m_CMD_CodeTable[m_CMD_Index+1],"END");
}
void CMD_RunCode()
{
	char endString[5];
	m_CMD_Code_Index=0;
	int oldIndex;
	while(1)
	{
		oldIndex=m_CMD_Code_Index;
		strncpy(endString,m_CMD_CodeTable[m_CMD_Code_Index],5);
		CMD_UpString(endString);
		if(strcmp(endString,"END")==0)
			break;
		CMD_Do(m_CMD_CodeTable[m_CMD_Code_Index]);
		if(m_CMD_Code_Index==oldIndex)
		{
			m_CMD_Code_Index++;
			if(m_CMD_Code_Index>=CMD_CODE_SIZE)
				m_CMD_Code_Index=0;
		}
	}
}
int CMD_Lable(char* pLable)
{
	int length=strlen(pLable);
	int index;

	for(index=0;index<CMD_CODE_SIZE;index++)
	{
		if(strncmp(pLable,m_CMD_CodeTable[index],length)==0)
			if(m_CMD_CodeTable[index][length]==':')
				return index;
	}
	return CMD_ERROR;
}
char* Code_Change(char* index,char* variours,char* _3,char* _4)
{
	int codeIndex=atoi(index);
	char inString[CMD_TABLE_LENGTH],outString[CMD_TABLE_LENGTH];
	gets(inString);
	CMD_ReconstructString(inString,outString);
	strncpy(m_CMD_CodeTable[codeIndex],outString,CMD_CODE_LENGTH);
}
char* Code_Add(char* variours,char* _2,char* _3,char* _4)
{
	static char returnValue[20];
	char inString[CMD_TABLE_LENGTH],outString[CMD_TABLE_LENGTH];
	gets(inString);
	CMD_ReconstructString(inString,outString);
	if(m_CMD_Index>=CMD_CODE_SIZE-1)
	{
		sprintf(returnValue,"Limit size:%d",m_CMD_Index+1);
		return returnValue;
	}
	CMD_ADDCode(outString);
	sprintf(returnValue,"Line:%d",m_CMD_Index+1);
	return returnValue;
}
char* Code_Show(char* _1,char* _2,char* _3,char* _4)
{
	char endString[5];
	int index;

	for(index=0;index<CMD_CODE_SIZE;index++)
	{
		strncpy(endString,m_CMD_CodeTable[index],5);
		CMD_UpString(endString);
		printf("L:%d,%s\n",index,m_CMD_CodeTable[index]);
		if(strcmp(endString,"END")==0)
			break;
	}
	return 0;
}
char* Code_Start(char* _1,char* _2,char* _3,char* _4)
{
	CMD_RunCode();
	return 0;
}
void CMD_CodeInit()
{
	CMD_LoadFunction("CodeChange",Code_Change);
	CMD_LoadFunction("CodeShow",Code_Show);
	CMD_LoadFunction("CodeAdd",Code_Add);
	CMD_LoadFunction("CodeStart",Code_Start);
}
