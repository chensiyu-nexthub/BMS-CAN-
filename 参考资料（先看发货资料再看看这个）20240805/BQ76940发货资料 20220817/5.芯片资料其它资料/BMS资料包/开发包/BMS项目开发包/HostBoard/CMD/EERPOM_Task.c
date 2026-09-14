/*
 * EERPOM_Task.c
 *
 *  Created on: May 6, 2010
 *      Author: Administrator
 */
#include "EEPROM_Task.h"
#include "../include/EEPROM.h"

unsigned char g_pubHighestVoltage[7][64];
unsigned int g_puiEEPROM_BlockWriteCounter[256];
unsigned char g_pubLowestVoltage[7][64];
static unsigned char g_pubTempBuffer[64];

unsigned int EEPORM_Task_uiCRC(unsigned char *ptr, unsigned char len)
{
	unsigned short crc;
	unsigned char i;

	crc=0;
	while(len--!=0)
	{
		for(i=0x80; i!=0; i/=2)
		{
			if((crc&0x8000)!=0)
				{crc*=2; crc^=0x1021;}
			else
				crc*=2;
			if((*ptr&i)!=0)
				crc^=0x1021;
		}
		ptr++;
	}
	return(crc);
}
void EEPORM_Task_vCRC_TempBuffer()
{
	unsigned char ubCrc[2];
	unsigned int uiCrc;

	uiCrc=EEPORM_Task_uiCRC(g_pubTempBuffer,60);
	ubCrc[0]=uiCrc&0xff;
	ubCrc[1]=uiCrc>>8;
	g_pubTempBuffer[EEPROM_DATA_CRC_LOW]=ubCrc[0];
	g_pubTempBuffer[EEPROM_DATA_CRC_HIGH]=ubCrc[1];
}
unsigned int EEPORM_Task_uiIsCRCShareDataTemp()
{
	unsigned char ubCrc[2];
	unsigned int uiCrc;
	unsigned int uiComplete=0;

	uiCrc=EEPORM_Task_uiCRC(g_pubTempBuffer,60);
	ubCrc[0]=uiCrc&0xff;
	ubCrc[1]=uiCrc>>8;

	if(g_pubTempBuffer[EEPROM_DATA_CRC_LOW]==ubCrc[0])
	{
		if(g_pubTempBuffer[EEPROM_DATA_CRC_HIGH]==ubCrc[1])
		{
			uiComplete=1;
		}
	}
	return uiComplete;
}

unsigned int EEPROM_Task_uiIsCRCEEPROM(unsigned char block)
{
	EEPROM_ReadEEPROM(block*64,g_pubTempBuffer,62);
	return EEPORM_Task_uiIsCRCShareDataTemp();
}
void EEPROM_Task_vGetWriteCounter()
{
	int i;
	unsigned int uiWriteCounter;
	unsigned char pubWriteCounter[2];
	for(i=0;i<256;i++)
	{
		EEPROM_ReadEEPROM(i*64+EEPROM_DATA_WRITE_COUNTER_LOW,
				pubWriteCounter,2);
		uiWriteCounter=pubWriteCounter[1];
		uiWriteCounter<<=8;
		uiWriteCounter+=pubWriteCounter[0];

		g_puiEEPROM_BlockWriteCounter[i]=uiWriteCounter;
	}
}
void EEPROM_Task_vGetLowestVoltage(unsigned char board)
{
	switch(board)
	{
	case 1:
		EEPROM_Task_vEEPROMToTempMemery(
				EEPROM_BLOCK_SAMPLE_1_LOWESTVOLTAGE);
		memcpy(g_pubLowestVoltage[0],g_pubTempBuffer,64);
		break;
	case 2:
		EEPROM_Task_vEEPROMToTempMemery(
				EEPROM_BLOCK_SAMPLE_2_LOWESTVOLTAGE);
		memcpy(g_pubLowestVoltage[1],g_pubTempBuffer,64);
		break;
	case 3:
		EEPROM_Task_vEEPROMToTempMemery(
				EEPROM_BLOCK_SAMPLE_3_LOWESTVOLTAGE);
		memcpy(g_pubLowestVoltage[2],g_pubTempBuffer,64);
		break;
	case 4:
		EEPROM_Task_vEEPROMToTempMemery(
				EEPROM_BLOCK_SAMPLE_4_LOWESTVOLTAGE);
		memcpy(g_pubLowestVoltage[3],g_pubTempBuffer,64);
		break;
	case 5:
		EEPROM_Task_vEEPROMToTempMemery(
				EEPROM_BLOCK_SAMPLE_5_LOWESTVOLTAGE);
		memcpy(g_pubLowestVoltage[4],g_pubTempBuffer,64);
		break;
	case 6:
		EEPROM_Task_vEEPROMToTempMemery(
				EEPROM_BLOCK_SAMPLE_6_LOWESTVOLTAGE);
		memcpy(g_pubLowestVoltage[5],g_pubTempBuffer,64);
		break;
	case 7:
		EEPROM_Task_vEEPROMToTempMemery(
				EEPROM_BLOCK_SAMPLE_7_LOWESTVOLTAGE);
		memcpy(g_pubLowestVoltage[6],g_pubTempBuffer,64);
		break;
	}
}
void EEPROM_Task_vGetHighestVoltage(unsigned char board)
{
	switch(board)
	{
	case 1:
		EEPROM_Task_vEEPROMToTempMemery(
				EEPROM_BLOCK_SAMPLE_1_HIGHESTVOLTAGE);
		break;
	case 2:
		EEPROM_Task_vEEPROMToTempMemery(
				EEPROM_BLOCK_SAMPLE_2_HIGHESTVOLTAGE);
		break;
	case 3:
		EEPROM_Task_vEEPROMToTempMemery(
				EEPROM_BLOCK_SAMPLE_3_HIGHESTVOLTAGE);
		break;
	case 4:
		EEPROM_Task_vEEPROMToTempMemery(
				EEPROM_BLOCK_SAMPLE_4_HIGHESTVOLTAGE);
		break;
	case 5:
		EEPROM_Task_vEEPROMToTempMemery(
				EEPROM_BLOCK_SAMPLE_5_HIGHESTVOLTAGE);
		break;
	case 6:
		EEPROM_Task_vEEPROMToTempMemery(
				EEPROM_BLOCK_SAMPLE_6_HIGHESTVOLTAGE);
		break;
	case 7:
		EEPROM_Task_vEEPROMToTempMemery(
				EEPROM_BLOCK_SAMPLE_7_HIGHESTVOLTAGE);
		break;
	}
}
void EEPROM_Task_vEEPROMToTempMemery(unsigned int block)
{
	unsigned int EEPROM_ReadAddress=block*64;
	unsigned int EEPROM_ReadLength=62;
	unsigned char* EEPROM_ReturnBuffer=g_pubTempBuffer;
	unsigned int uiComplet=0;
	int iErrorCounter=0;
	int i;
	if(EEPROM_ReadLength<4)
		return;
again:
	EEPROM_ReadEEPROM(EEPROM_ReadAddress,EEPROM_ReturnBuffer,EEPROM_ReadLength);

	uiComplet= EEPORM_Task_uiIsCRCShareDataTemp();
	if(uiComplet!=1)
	{
		if(iErrorCounter++<3)
			goto again;
		else
		{
			g_llErrorFlag|=ERROR_EEPROM_TO_SHAREMEMERY;
		}
	}
}
static void Event_OnShareMemeryToEEPROM()
{
	unsigned int uiEEPROM_WriteAddress=EEPROM_HOSTSHARE_MEMERYHEAD;
	unsigned int uiEEPROM_WriteLength=EEPROM_HOSTSHARE_MEMERYLENGTH;
	unsigned char* ubEEPROM_WriteBuffer=g_hostShareDataBuffer;
	unsigned int uiComplete=0;

	int iErrorCounter=0;
	int i;
	Event_vCRCShareMemery();
again:
	EEPROM_WriteEEPROM(uiEEPROM_WriteAddress,ubEEPROM_WriteBuffer,uiEEPROM_WriteLength);

	uiComplete=Event_uiIsCRCEEPROM(uiEEPROM_WriteAddress,uiEEPROM_WriteLength);

	if(uiComplete!=1)
	{
		if(iErrorCounter++<3)
			goto again;
		else
		{
			g_llErrorFlag|=ERROR_SHAREMEMERY_TO_EEPROM;
		}
	}

}
