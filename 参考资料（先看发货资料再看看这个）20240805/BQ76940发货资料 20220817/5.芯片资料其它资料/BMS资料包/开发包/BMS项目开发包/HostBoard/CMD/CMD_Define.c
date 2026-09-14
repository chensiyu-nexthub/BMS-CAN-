/*
 * CMD_Define.c
 *
 *  Created on: May 19, 2010
 *      Author: Administrator
 */
#include "Cmd.h"
extern char m_table_1[CMD_TABLE_SIZE][CMD_TABLE_LENGTH];
extern char m_table_2[CMD_TABLE_SIZE][CMD_TABLE_LENGTH];
extern int m_table_index;
extern char m_tempCmdString[CMD_TABLE_LENGTH];
int CMD_DefineDevide(char* cmdString,char** variours_1,char** variours_2)
{
	unsigned char index=0;
	index++;//filter '$'
	*variours_1=cmdString+index;
	while(cmdString[index]!='=')
	{
		if(cmdString[index]==0)
			return CMD_ERROR;
		index++;
	}
	cmdString[index]=0;
	index++;

	*variours_2=cmdString+index;
	return CMD_OK;
}
