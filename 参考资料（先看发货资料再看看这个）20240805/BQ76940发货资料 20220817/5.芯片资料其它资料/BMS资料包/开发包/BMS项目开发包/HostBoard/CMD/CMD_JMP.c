/*
 * CMD_JMP.c
 *
 *  Created on: May 20, 2010
 *      Author: Administrator
 */
#include "stdlib.h"
#include "Cmd.h"
#include "cmd_code.h"
#include "cmd_jmp.h"
#include "cmd_call.h"
extern int m_CMD_Code_Index;

void CMD_JMPInit()
{
	CMD_LoadFunction("BJMP",CMD_BJMP);
	CMD_LoadFunction("EJMP",CMD_EJMP);
	CMD_LoadFunction("BEJMP",CMD_BEJMP);
	CMD_LoadFunction("SJMP",CMD_SJMP);
	CMD_LoadFunction("SEJMP",CMD_SEJMP);
	CMD_LoadFunction("NEJMP",CMD_NEJMP);
	CMD_LoadFunction("ZJMP",CMD_ZJMP);
	CMD_LoadFunction("NZJMP",CMD_NZJMP);
	CMD_LoadFunction("JMP",CMD_JMP);
}
char* CMD_BJMP(char* label,
		char* pVariours_1,char* pVariours_2,char* pVariours_3)
{
	int variours_1=atoi(pVariours_1);
	int variours_2=atoi(pVariours_2);
	int Index=CMD_Lable(label);
	if(variours_1>variours_2)
		m_CMD_Code_Index=Index;
}
char* CMD_EJMP(char* label,
		char* pVariours_1,char* pVariours_2,char* pVariours_3)
{
	int variours_1=atoi(pVariours_1);
	int variours_2=atoi(pVariours_2);
	int Index=CMD_Lable(label);
	if(variours_1==variours_2)
		m_CMD_Code_Index=Index;
}
char* CMD_BEJMP(char* label,
		char* pVariours_1,char* pVariours_2,char* pVariours_3)
{
	int variours_1=atoi(pVariours_1);
	int variours_2=atoi(pVariours_2);
	int Index=CMD_Lable(label);
	if(variours_1>=variours_2)
		m_CMD_Code_Index=Index;
}
char* CMD_SJMP(char* label,
		char* pVariours_1,char* pVariours_2,char* pVariours_3)
{
	int variours_1=atoi(pVariours_1);
	int variours_2=atoi(pVariours_2);
	int Index=CMD_Lable(label);
	if(variours_1<variours_2)
		m_CMD_Code_Index=Index;
}
char* CMD_SEJMP(char* label,
		char* pVariours_1,char* pVariours_2,char* pVariours_3)
{
	int variours_1=atoi(pVariours_1);
	int variours_2=atoi(pVariours_2);
	int Index=CMD_Lable(label);
	if(variours_1<=variours_2)
		m_CMD_Code_Index=Index;
}
char* CMD_NEJMP(char* label,
		char* pVariours_1,char* pVariours_2,char* pVariours_3)
{
	int variours_1=atoi(pVariours_1);
	int variours_2=atoi(pVariours_2);
	int Index=CMD_Lable(label);
	if(variours_1!=variours_2)
		m_CMD_Code_Index=Index;
}
char* CMD_ZJMP(char* label,
		char* pVariours_1,char* pVariours_2,char* pVariours_3)
{
	int variours_1=atoi(pVariours_1);
	int Index=CMD_Lable(label);
	if(variours_1==0)
		m_CMD_Code_Index=Index;
}
char* CMD_NZJMP(char* label,
		char* pVariours_1,char* pVariours_2,char* pVariours_3)
{
	int variours_1=atoi(pVariours_1);
	int Index=CMD_Lable(label);
	if(variours_1!=0)
		m_CMD_Code_Index=Index;
}
char* CMD_JMP(char* label,
		char* pVariours_1,char* pVariours_2,char* pVariours_3)
{
	int Index=CMD_Lable(label);
	m_CMD_Code_Index=Index;
}
