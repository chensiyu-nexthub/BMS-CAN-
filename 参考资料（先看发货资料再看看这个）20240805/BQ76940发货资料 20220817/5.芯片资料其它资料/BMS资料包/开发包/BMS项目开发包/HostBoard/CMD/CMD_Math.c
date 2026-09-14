/*
 * CMD_Math.c
 *
 *  Created on: May 21, 2010
 *      Author: Administrator
 */
#include "CMD_Math.h"
#include "CMD.h"
#include "string.h"
#include "stdio.h"
#include "cmd_code.h"
#include "cmd_call.h"
void CMD_MathInit()
{
	CMD_LoadFunction("ADD",CMD_ADD);
	CMD_LoadFunction("SUB",CMD_SUB);
	CMD_LoadFunction("DIV",CMD_DIV);
	CMD_LoadFunction("MUL",CMD_MUL);
	CMD_LoadFunction("LEFT",CMD_LEFT);
	CMD_LoadFunction("RIGTH",CMD_RIGHT);
}
char* CMD_ADD(char* pVariours_1,char* pVariours_2,char* pVariours_3,char* pVariours_4)
{
	static char returnBuffer[CMD_TABLE_LENGTH];
	int returnValue;
	int variours_1=atoi(pVariours_1);
	int variours_2=atoi(pVariours_2);
	returnValue=variours_1+variours_2;
	sprintf(returnBuffer,"%d",returnValue);

	return returnBuffer;
}
char* CMD_SUB(char* pVariours_1,char* pVariours_2,char* pVariours_3,char* pVariours_4)
{
	static char returnBuffer[CMD_TABLE_LENGTH];
	int returnValue;
	int variours_1=atoi(pVariours_1);
	int variours_2=atoi(pVariours_2);
	returnValue=variours_1-variours_2;
	sprintf(returnBuffer,"%d",returnValue);

	return returnBuffer;
}
char* CMD_MUL(char* pVariours_1,char* pVariours_2,char* pVariours_3,char* pVariours_4)
{
	static char returnBuffer[CMD_TABLE_LENGTH];
	int returnValue;
	int variours_1=atoi(pVariours_1);
	int variours_2=atoi(pVariours_2);
	returnValue=variours_1*variours_2;
	sprintf(returnBuffer,"%d",returnValue);

	return returnBuffer;
}
char* CMD_DIV(char* pVariours_1,char* pVariours_2,char* pVariours_3,char* pVariours_4)
{
	static char returnBuffer[CMD_TABLE_LENGTH];
	int returnValue;
	int variours_1=atoi(pVariours_1);
	int variours_2=atoi(pVariours_2);
	returnValue=variours_1/variours_2;
	sprintf(returnBuffer,"%d",returnValue);

	return returnBuffer;
}
char* CMD_LEFT(char* pVariours_1,char* pVariours_2,char* pVariours_3,char* pVariours_4)
{
	static char returnBuffer[CMD_TABLE_LENGTH];
	int returnValue;
	int variours_1=atoi(pVariours_1);
	int variours_2=atoi(pVariours_2);
	variours_1<<=variours_2;
	returnValue=variours_1;
	sprintf(returnBuffer,"%d",returnValue);

	return returnBuffer;
}
char* CMD_RIGHT(char* pVariours_1,char* pVariours_2,char* pVariours_3,char* pVariours_4)
{
	static char returnBuffer[CMD_TABLE_LENGTH];
	int returnValue;
	int variours_1=atoi(pVariours_1);
	int variours_2=atoi(pVariours_2);
	variours_1>>=variours_2;
	returnValue=variours_1;
	sprintf(returnBuffer,"%d",returnValue);

	return returnBuffer;
}
