/*
 * CMD_Call.h
 *
 *  Created on: May 19, 2010
 *      Author: Administrator
 */

#ifndef CMD_CALL_H_
#define CMD_CALL_H_

#define CMD_FUNCTION_SIZE 100
#define CMD_NAME_MAX   64

typedef char*(*pF_CMDFuntion)(char* variours_1,char* variours_2,char* variours_3,char* variours_4);
int CMD_CallEx(char* cmdString);
int CMD_Call(char* cmdString);
void CMD_FuncionInit();
void CMD_LoadFunction(char* function,pF_CMDFuntion pFuntion);
pF_CMDFuntion CMD_Function(char* inputString);
int CMD_FunctionDevide(char* cmdString,char** pFunction,
		char** pVariours_1,char** pVariours_2,char** pVariours_3,char** pVariours_4);
int CMD_FunctionDevideEx(char* cmdString,char** returnVariours,char** pFunction,
		char** pVariours_1,char** pVariours_2,char** pVariours_3,char** pVariours_4);

#endif /* CMD_CALL_H_ */
