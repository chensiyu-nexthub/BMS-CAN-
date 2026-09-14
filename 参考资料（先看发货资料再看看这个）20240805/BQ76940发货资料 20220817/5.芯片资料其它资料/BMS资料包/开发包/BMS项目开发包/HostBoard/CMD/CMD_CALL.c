/*
 * CMD_CALL.c
 *
 *  Created on: May 19, 2010
 *      Author: Administrator
 */
#include "CMD.h"
#include "string.h"
#include "CMD_Call.h"

extern char m_table_1[CMD_TABLE_SIZE][CMD_TABLE_LENGTH];
extern char m_table_2[CMD_TABLE_SIZE][CMD_TABLE_LENGTH];
extern int m_table_index;
extern char m_tempCmdString[CMD_TABLE_LENGTH];
int m_functionIndex=-1;

char m_functionName[CMD_FUNCTION_SIZE][CMD_NAME_MAX];
pF_CMDFuntion m_pFuntion[CMD_FUNCTION_SIZE];

void CMD_LoadFunction(char* function,pF_CMDFuntion pFuntion)
{
	m_functionIndex++;
	if(m_functionIndex>=CMD_FUNCTION_SIZE)
		m_functionIndex=0;
	strcpy(m_functionName[m_functionIndex],function);
	m_pFuntion[m_functionIndex]=pFuntion;
}
char* CMD_ShowFunction(char* _p1,char* _p2,char* _p3,char* _p4)
{
	int index=0;
	for(index=0;index<=m_functionIndex;index++)
	{
		printf(m_functionName[index]);
		printf("\t");
	}
	printf("\n");
	return 0;
}
void CMD_FuncionInit()
{
	CMD_LoadFunction("Help",CMD_ShowFunction);
}

pF_CMDFuntion CMD_Function(char* inputString)
{
	int index=0;
	for(index=0;index<CMD_FUNCTION_SIZE;index++)
	{
		if(strcmp(m_functionName[index],inputString)==0)
			return m_pFuntion[index];
	}
	return 0;
}

int CMD_Call(char* cmdString)
{
	char *pVariours_1,*pVariours_2,*pVariours_3,*pVariours_4;
	char *pFunction;
	int CMD_Retrun;
	pF_CMDFuntion function;
	CMD_Retrun=CMD_FunctionDevide(cmdString,
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

	function(pVariours_1,pVariours_2,pVariours_3,pVariours_4);
	return CMD_OK;
}

int CMD_CallEx(char* cmdString)
{
	char *pVariours_1,*pVariours_2,*pVariours_3,*pVariours_4;
	char *pFunction,*pVarioursReturn,*pFunctionReturn;
	int CMD_Retrun;
	pF_CMDFuntion function;
	CMD_Retrun=CMD_FunctionDevideEx(cmdString,&pVarioursReturn,
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

	pFunctionReturn=function(pVariours_1,pVariours_2,pVariours_3,pVariours_4);
	if(CMD_Table(pVarioursReturn)!=0)
		CMD_ChangeTable(pVarioursReturn,pFunctionReturn);
	else
		CMD_ADDTable(pVarioursReturn,pFunctionReturn);

	return CMD_OK;
}

int CMD_FunctionDevide(char* cmdString,char** pFunction,
		char** pVariours_1,char** pVariours_2,char** pVariours_3,char** pVariours_4)
{
	unsigned char index=0;

	*pFunction=cmdString+index;

	while(cmdString[index]!='(')
	{
		if(cmdString[index]==0)
			return CMD_OK;
		index++;
	}
	cmdString[index]=0;
	index++;

	*pVariours_1=cmdString+index;
	while(cmdString[index]!=',')
	{
		if(cmdString[index]==0)
			return CMD_ERROR;
		if(cmdString[index]==')')
		{
			cmdString[index]=0;
			return CMD_OK;
		}
		index++;
	}
	cmdString[index]=0;
	index++;

	*pVariours_2=cmdString+index;
	while(cmdString[index]!=',')
	{
		if(cmdString[index]==0)
			return CMD_ERROR;
		if(cmdString[index]==')')
		{
			cmdString[index]=0;
			return CMD_OK;
		}
		index++;
	}
	cmdString[index]=0;
	index++;

	*pVariours_3=cmdString+index;
	while(cmdString[index]!=',')
	{
		if(cmdString[index]==0)
			return CMD_ERROR;
		if(cmdString[index]==')')
		{
			cmdString[index]=0;
			return CMD_OK;
		}
		index++;
	}
	cmdString[index]=0;
	index++;

	*pVariours_4=cmdString+index;
	while(cmdString[index]!=')')
	{
		if(cmdString[index]==0)
			return CMD_ERROR;
		index++;
	}
	cmdString[index]=0;
	index++;
}
int CMD_FunctionDevideEx(char* cmdString,char** returnVariours,char** pFunction,
		char** pVariours_1,char** pVariours_2,char** pVariours_3,char** pVariours_4)
{
	unsigned char index=0;
	index++;//filter '$'
	*returnVariours=cmdString+index;
	while(cmdString[index]!='=')
	{
		if(cmdString[index]==0)
			return CMD_ERROR;
		index++;
	}
	cmdString[index]=0;
	index++;

	*pFunction=cmdString+index;

	while(cmdString[index]!='(')
	{
		if(cmdString[index]==0)
			return CMD_OK;
		index++;
	}
	cmdString[index]=0;
	index++;

	*pVariours_1=cmdString+index;
	while(cmdString[index]!=',')
	{
		if(cmdString[index]==0)
			return CMD_ERROR;
		if(cmdString[index]==')')
		{
			cmdString[index]=0;
			return CMD_OK;
		}
		index++;
	}
	cmdString[index]=0;
	index++;

	*pVariours_2=cmdString+index;
	while(cmdString[index]!=',')
	{
		if(cmdString[index]==0)
			return CMD_ERROR;
		if(cmdString[index]==')')
		{
			cmdString[index]=0;
			return CMD_OK;
		}
		index++;
	}
	cmdString[index]=0;
	index++;

	*pVariours_3=cmdString+index;
	while(cmdString[index]!=',')
	{
		if(cmdString[index]==0)
			return CMD_ERROR;
		if(cmdString[index]==')')
		{
			cmdString[index]=0;
			return CMD_OK;
		}
		index++;
	}
	cmdString[index]=0;
	index++;

	*pVariours_4=cmdString+index;
	while(cmdString[index]!=')')
	{
		if(cmdString[index]==0)
			return CMD_ERROR;
		index++;
	}
	cmdString[index]=0;
	index++;
	return CMD_OK;
}
