/*
 * Memery.h
 *
 *  Created on: 2010-8-2
 *      Author: Administrator
 */

#ifndef MEMERY_H_
#define MEMERY_H_
#include "Type.h"

struct STMemery
{
	UINT8 *pubFrom;
	UINT8 *pubTo;
	UINT32 ulLength;
};


#define MEMERY_MOVE(__To,__From,__Length) g_stMemery.pubFrom=(__From);g_stMemery.pubTo=(__To);g_stMemery.ulLength=(__Length);Memery_vMove();

void Memery_vMove();

#define MEMERY_IMB_Register_Start 0xffff00
#define MEMERY_IMB_Register_Size  256
#define MEMERY_Emulated_PSRAM_Start 0xE80000
#define MEMERY_Emulated_PSRAM_Size 32*1024
#define MEMERY_Program_SRAM_Start   0xE00000
#define MEMERY_Program_SRAM_Size   32*1024
#define MEMERY_Program_Flash_3_Start     0xCC0000
#define MEMERY_Program_Flash_3_Size		64*1024
#define MEMERY_Program_Flash_2_Start     0xC80000
#define MEMERY_Program_Flash_2_Size		64*1024
#define MEMERY_Program_Flash_1_Start     0xC40000
#define MEMERY_Program_Flash_1_Size		64*1024
#define MEMERY_Program_Flash_0_Start     0xC00000
#define MEMERY_Program_Flash_0_Size		64*1024
#define MEMERY_MultiCAN_USIC_Regs_Start 0x208000
#define MEMERY_MultiCAN_USIC_Regs_Size 16*1024
#define MEMERY_USIC_Regs_Start			0x204000
#define MEMERY_USIC_Regs_Size			8*1024
#define MEMERY_MultiCAN_Regs_Start      0x200000
#define MEMERY_MultiCAN_Regs_Size      16*1024
#define MEMERY_SFR_Start				0x00FE00
#define MEMERY_SFR_Size				    512
#define MEMERY_Dual_Port_RAM_Start		0x00F600
#define MEMERY_Dual_Port_RAM_Size		2*1024
#define MEMERY_ESFR_Start				0x00F000
#define MEMERY_ESFR_Size				512
#define MEMERY_XSFR_Start				0x00E000
#define MEMERY_XSFR_Size				4*1024
#define MEMERY_Data_SRAM_Start			0x00A000
#define MEMERY_Data_SRAM_Size			16*1024

#endif /* MEMERY_H_ */
