/*
 * API.c
 *
 *  Created on: 2015-7-19
 *      Author: James
 */

#include "DSP28x_Project.h"
#include "API.h"

#pragma CODE_SECTION(API_Erase,"ramfuncs");
#pragma CODE_SECTION(API_Read,"ramfuncs");
#pragma CODE_SECTION(API_Write,"ramfuncs");
#pragma CODE_SECTION(API_Init,"ramfuncs");


Uint16 Read_Buffer[BufLength];
Uint16 Write_Buffer[BufLength];

FLASH_ST FlashStatus;
FLASH_ST ProgStatus;

void API_Init(void)
{
	// Copy the Flash API functions to SARAM
    MemCopy(&Flash28_API_LoadStart, &Flash28_API_LoadEnd, &Flash28_API_RunStart);
	Flash_CPUScaleFactor = SCALE_FACTOR;
	Flash_CallbackPtr = NULL;
}


/*先检查数据段是不是到了末端，如果是到了末端，则报警不读信息
 * 如果不是到了末端，那么就把所有数据读取到Buf中
 * 读取的过程中检查*/
Uint16 API_Read(Uint16 RelativeAddr, Uint16 *Buf, Uint16 Length)
{
	Uint16 temp;
	Uint16 NullFlag=1;
	Uint16 *AbsAddr;

	AbsAddr=Begin_SECTORAddress+RelativeAddr;
	//先检查一下这部分的长度是不是到了flash的末端了
	if(AbsAddr > (End_SECTORAddress-Length))			// Test if sector is full
	{
	//	EEPROM_Erase();
		//LED错误灯亮
	//	asm(" ESTOP0");
		return 0;
	}
	for(temp=0;temp<Length;temp++)
	{
		Buf[temp]=*AbsAddr;
		AbsAddr++;
		if(Buf[temp] != 0xffff)
		{
			NullFlag=0;
		}
	}
	return NullFlag;

}

Uint16 API_Write(Uint16 RelativeAddr, Uint16 *Buf, Uint16 Length)
{
	// Variables needed for Flash API Functions
	Uint16 Tempbuf[100];
	Uint16 *AbsAddr;
	Uint16 Status;

	//如果这一个数据段是空的，才可以写
	if(API_Read(RelativeAddr,Tempbuf,Length))
	{
		AbsAddr=Begin_SECTORAddress+RelativeAddr;

		Status = Flash_Program(AbsAddr,Buf,Length,&ProgStatus);

		// Modify Page Status from Blank Page to Current Page if flash programming was successful
		if (Status == API_STATUS_SUCCESS)
			return 1;
		else
			return 0;
	}
	else
	{
		return 0;
	}

}

//############################# EEPROM_ERASE #################################
Uint16 API_Erase()
{
	Uint16  Status;

	#ifdef F28332					// If F28332/F28232 is being used erase Sector D
	Status = Flash_Erase((SECTORD),&FlashStatus);
	#else							// Otherwise erase Sector H for other devices
	Status = Flash_Erase((SECTORH),&FlashStatus);
	#endif

   	if(Status != API_STATUS_SUCCESS)	// If Erase fails halt program or handle error
   	{

//      	asm(" ESTOP0");
      	return 0;
   	}
   	else
   		return 1;
}





