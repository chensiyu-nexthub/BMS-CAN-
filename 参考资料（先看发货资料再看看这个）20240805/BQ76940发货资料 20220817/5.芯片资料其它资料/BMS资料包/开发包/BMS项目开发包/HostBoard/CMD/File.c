/*
 * File.c
 *
 *  Created on: May 21, 2010
 *      Author: Administrator
 */
#include "../include/EEPROM.h"
#include "shell.h"
#include "CMD_Call.h"

char* FILE_WR(char* pVariours_1,char* pVariours_2,char* pVariours_3,char* pVariours_4)
{
	int address=atoi(pVariours_1);
	int length=atoi(pVariours_2);
	EEPROM_WriteEEPROM (address,pVariours_3,length);
	return 0;
}
char* FILE_RD(char* pVariours_1,char* pVariours_2,char* pVariours_3,char* pVariours_4)
{
	static char buffer[64];
	int address=atoi(pVariours_1);
	int length=atoi(pVariours_2);
	EEPROM_ReadEEPROM (address,buffer,length);
	buffer[length]=0;
	return buffer;
}
void FILE_Init()
{
	EEPROM_Init(AddERROR,0xffff);
	CMD_LoadFunction("WRFile",FILE_WR);
	CMD_LoadFunction("RDFile",FILE_RD);
}
