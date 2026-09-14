/*
 * Cmd_test.c
 *
 *  Created on: May 18, 2010
 *      Author: Administrator
 */
#include "cmd.h"
#include "cmd_call.h"
#include "stdio.h"
#include "cmd_code.h"

extern char m_table_1[CMD_TABLE_SIZE][CMD_TABLE_LENGTH];
extern char m_table_2[CMD_TABLE_SIZE][CMD_TABLE_LENGTH];
extern int m_table_index;
extern char m_tempCmdString[CMD_TABLE_LENGTH];

void Host_Fun_1(unsigned char variours_1,
		unsigned char variours_2,unsigned char variours_3,
		unsigned char variours_4)
{
	printf("Host do function Fun_1,and variours are:%d,%d,%d,%d",
			variours_1,variours_2,variours_3,variours_4);
}
void CMD_EchoTest()
{
	char cmdEcho_1[]=">Echo\"Hello world\"";
	CMD_Do(cmdEcho_1);
}
void CMD_InTest()
{
	char cmdIn_1[]=">In$Val";
	CMD_Do(cmdIn_1);
}
void CMD_InEchoTest()
{
	char cmdIn_1[]=">In$Val";
	char cmdEcho_1[]=">Echo$Val";
	CMD_Do(cmdIn_1);

	CMD_Do(cmdEcho_1);
}
void CMD_Host_Test()
{
	char cmdString[]="Host/Fun_1($A,$B,$C,$D)";
	char cmdDefine_1[]="$A=10";
	char cmdDefine_2[]="$B=11";
	char cmdDefine_3[]="$C=12";
	char cmdDefine_4[]="$D=13";

	CMD_Do(cmdDefine_1);
	CMD_Do(cmdDefine_2);
	CMD_Do(cmdDefine_3);
	CMD_Do(cmdDefine_4);
}
void Cmd_DefineTest()
{
	char cmdString_1[]="#A=12";
	char cmdString_2[]=">Echo$A";
	CMD_Do(cmdString_1);
	CMD_Do(cmdString_2);

}
void Cmd_CallTest()
{
	char *pVariours_1,*pVariours_2,*pVariours_3,*pVariours_4;
	char *pFunction;
	char cmdString_1[]="Fun()";
	char cmdString_2[]="Fun(a)";
	char cmdString_3[]="Fun(a,b)";
	char cmdString_4[]="Fun(a,b,c)";
	char cmdString_5[]="Fun(a,b,c,d)";
	CMD_FunctionDevide(cmdString_1,
			&pFunction,&pVariours_1,&pVariours_2,&pVariours_3,&pVariours_4);
	puts(pFunction);
	puts(pVariours_1);
	puts(pVariours_2);
	puts(pVariours_3);
	puts(pVariours_4);

	CMD_FunctionDevide(cmdString_2,
			&pFunction,&pVariours_1,&pVariours_2,&pVariours_3,&pVariours_4);
	puts(pFunction);
	puts(pVariours_1);
	puts(pVariours_2);
	puts(pVariours_3);
	puts(pVariours_4);

	CMD_FunctionDevide(cmdString_3,
			&pFunction,&pVariours_1,&pVariours_2,&pVariours_3,&pVariours_4);
	puts(pFunction);
	puts(pVariours_1);
	puts(pVariours_2);
	puts(pVariours_3);
	puts(pVariours_4);

	CMD_FunctionDevide(cmdString_4,
			&pFunction,&pVariours_1,&pVariours_2,&pVariours_3,&pVariours_4);
	puts(pFunction);
	puts(pVariours_1);
	puts(pVariours_2);
	puts(pVariours_3);
	puts(pVariours_4);


	CMD_FunctionDevide(cmdString_5,
			&pFunction,&pVariours_1,&pVariours_2,&pVariours_3,&pVariours_4);
	puts(pFunction);
	puts(pVariours_1);
	puts(pVariours_2);
	puts(pVariours_3);
	puts(pVariours_4);


}
char* T(char* variours_1,char* variours_2,char* variours_3,char* variours_4)
{
	printf("Function T is called.\n");
}
char* T_2(char* variours_1,char* variours_2,char* variours_3,char* variours_4)
{
	printf("Function T2 is called.variours 1:%s\n",variours_1);
}
void Cmd_CallTest_2()
{
	CMD_LoadFunction("T",T);
	CMD_LoadFunction("T_2",T_2);
	char cmdString_1[]="T()";
	char cmdString_2[]="T_2(asd)";
	CMD_Call(cmdString_1);
	CMD_Call(cmdString_2);
}
void Cmd_CodeTest()
{
	char cmdString_1[CMD_CODE_LENGTH]=">Echo\"1\" ";
	char cmdString_2[CMD_CODE_LENGTH]=">Echo\"2\" ";
	char cmdString_3[CMD_CODE_LENGTH]="AGAIN:";
	char cmdString_4[CMD_CODE_LENGTH]="ZJMP(AGAIN,0)";
	CMD_ADDCode(cmdString_1);
	CMD_ADDCode(cmdString_2);
	CMD_ADDCode(cmdString_3);
	CMD_ADDCode(cmdString_4);
	CMD_RunCode();
}
char _ADDReturn[CMD_CODE_LENGTH];
char* ADD(char* variours_1,char* variours_2,char* variours_3,char* variours_4)
{
	int val_1=atoi(variours_1);
	int val_2=atoi(variours_2);
	val_2+=val_1;
	sprintf(_ADDReturn,"%d",val_2);

	return _ADDReturn;
}
void Cmd_CodeTest_2()
{
	char cmdString[][CMD_CODE_LENGTH]={"$A=ADD(1,2)",
										">Echo$A",
										"$B=SUB($A,1)",
										">Echo$B",
										"$A=MUL($B,2)",
										">Echo$A"

	};

	CMD_ADDCode(cmdString[0]);
	CMD_ADDCode(cmdString[1]);
	CMD_ADDCode(cmdString[2]);
	CMD_ADDCode(cmdString[3]);
	CMD_ADDCode(cmdString[4]);
	CMD_ADDCode(cmdString[5]);
	CMD_RunCode();
}
void Cmd_Test()
{
	//CMD_EchoTest();
	//CMD_InTest();
	//CMD_InEchoTest();
	//Cmd_DefineTest();
	//Cmd_CallTest_2();
	//Cmd_CodeTest();
	Cmd_CodeTest_2();
}
