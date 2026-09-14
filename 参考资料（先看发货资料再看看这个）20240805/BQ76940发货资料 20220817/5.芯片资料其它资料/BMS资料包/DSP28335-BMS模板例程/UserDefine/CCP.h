/*
 * CCP.h
 *
 *  Created on: 2010-8-3
 *      Author: Administrator
 */
#define CCP_FILE
#ifdef CCP_FILE
#ifndef CCP_H_
#define CCP_H_
#include "Type.h"
#include "DSP28x_Project.h"

#define CCP_DTO_ID        1           /* CAN identifier ECU -> Master */
#define CCP_CRO_ID        2           /* CAN identifier Master -> ECU */

/* Basic */
#define CC_CONNECT           0x01
#define CC_SET_MTA           0x02
#define CC_DNLOAD            0x03
#define CC_UPLOAD            0x04
#define CC_TEST              0x05 /* V2.1 */
#define CC_START_STOP        0x06
#define CC_DISCONNECT        0x07
#define CC_START_STOP_ALL    0x08 /* V2.1 */
#define CC_SHORT_UPLOAD      0x0F
#define CC_GET_DAQ_SIZE      0x14
#define CC_SET_DAQ_PTR       0x15
#define CC_WRITE_DAQ         0x16
#define CC_EXCHANGE_ID       0x17
#define CC_GET_CCP_VERSION   0x1B /* V2.1 */
#define CC_DNLOAD_6           0x23

/* Optional */
#define CC_GET_CAL_PAGE      0x09
#define CC_SET_S_STATUS      0x0C
#define CC_GET_S_STATUS      0x0D
#define CC_BUILD_CHKSUM      0x0E
#define CC_CLEAR_MEMORY      0x10
#define CC_SET_CAL_PAGE      0x11
#define CC_GET_SEED          0x12
#define CC_UNLOCK            0x13
#define CC_PROGRAM           0x18
#define CC_MOVE_MEMORY       0x19
#define CC_DIAG_SERVICE      0x20
#define CC_ACTION_SERVICE    0x21
#define CC_PROGRAM_6          0x22
/* Vector extensions */

#define CC_PROGRAM_PREPARE   0x1E   /* Prepare for flash kernel download */
#define CC_PROGRAM_START     0x1F   /* Start flash kernel at MTA[0] */

/* Returncodes */
#define CRC_OK                 0x00

/* C1 */
#define CRC_CMD_BUSY           0x10
#define CRC_DAQ_BUSY           0x11
#define CRC_KEY_REQUEST        0x18
#define CRC_STATUS_REQUEST     0x19

/* C2 */
#define CRC_COLD_START_REQUEST 0x20
#define CRC_CAL_INIT_REQUEST   0x21
#define CRC_DAQ_INIT_REQUEST   0x22
#define CRC_CODE_REQUEST       0x23

/* C3 (Errors) */
#define CRC_CMD_UNKNOWN        0x30
#define CRC_CMD_SYNTAX         0x31
#define CRC_OUT_OF_RANGE       0x32
#define CRC_ACCESS_DENIED      0x33
#define CRC_OVERLOAD           0x34
#define CRC_ACCESS_LOCKED      0x35

#define CCP_MAX_ODT          10
#define CCP_MAX_DAQ          1
/* Session Status */
#define SS_CAL                0x01
#define SS_DAQ                0x02
#define SS_RESUME             0x04
#define SS_TMP_DISCONNECTED   0x10
#define SS_CONNECTED          0x20
#define SS_STORE              0x40
#define SS_RUN                0x80

/* DAQ list */
#define DAQ_FLAG_START    0x01
#define DAQ_FLAG_SEND     0x02
#define DAQ_FLAG_PREPARED 0x04
#define DAQ_FLAG_OVERRUN  0x80

struct STOdtEntry
{
	Uint16 pubAddress[4];
	Uint16 ubLength;
};


struct STDaqList
{
  struct STOdtEntry    pstOdt[CCP_MAX_ODT][7];
  UINT16	uiPrescaler;
  UINT16	uiCycle;
  Uint16	ubEventChannel;
  Uint16 ubLast;
  Uint16 ubFlags;
};

#define CCP_STATION_ADDR   0x111
#define CCP_BROADCAST_STATION_ADDR 0

struct STCCP
{
	Uint16* pubCANReceive;
	Uint16 pubCANTransmit[8];
	Uint16 ubSessionStatus;
	Uint16 ubUserSessionStatus;
	Uint16 ubSendStatus;
	struct STDaqList pstDaqList[CCP_MAX_DAQ];
	Uint16 pbStationID[32];
	Uint16 ubProtectionStatus;//If set the bit,means the option is not protected.
	Uint16 ubCurrentDaq;
	Uint16 ubCurrentOdt;
	struct STOdtEntry* pstOdtEntry;
	Uint16 ppubMTA[2][4];
};

struct STAddress
{
	Uint16 ubShortAdd;
	Uint16 ubShortSub;
	Uint16*   pubAddress;
};
#define Pub_To_Ul_HF(__ul,__pub) (__ul)=__pub[0];(__ul)<<=8;(__ul)+=(__pub)[1];(__ul)<<=8;(__ul)+=(__pub)[2];(__ul)<<=8;(__ul)+=(__pub)[3];
#define Pub_To_Ui_HF(__ui,__pub) (__ui)=(__pub)[0];(__ui)<<=8;(__ui)+=(__pub)[1];
#define Ul_To_Pub_HF(__pub,__ul) (__pub)[3]=(__ul)&0xff;(__ul)>>=8;(__pub)[2]=(__ul)&0xff;(__ul)>>=8;(__pub)[1]=(__ul)&0xff;(__ul)>>=8;(__pub)[0]=(__ul)&0xff;
#define Ui_To_Pub_HF(__pub,__ui) (__pub)[1]=(__ui)&0xff;(__ui)>>=8;(__pub)[0]=(__ui)&0xff;
#define Pub4_Move(__pubT,__pubF) (__pubT)[0]=(__pubF)[0];(__pubT)[1]=(__pubF)[1];(__pubT)[2]=(__pubF)[2];(__pubT)[3]=(__pubF)[3];
#define Pub2_Move(__pubT,__pubF) (__pubT)[0]=(__pubF)[0];(__pubT)[1]=(__pubF)[1];

#define Pub_To_Ul_LF(__ul,__pub) (__ul)=__pub[3];(__ul)<<=8;(__ul)+=(__pub)[2];(__ul)<<=8;(__ul)+=(__pub)[1];(__ul)<<=8;(__ul)+=(__pub)[0];
#define Pub_To_Ui_LF(__ui,__pub) (__ui)=(__pub)[1];(__ui)<<=8;(__ui)+=(__pub)[0];
#define Ul_To_Pub_LF(__pub,__ul) (__pub)[0]=(__ul)&0xff;(__ul)>>=8;(__pub)[1]=(__ul)&0xff;(__ul)>>=8;(__pub)[2]=(__ul)&0xff;(__ul)>>=8;(__pub)[3]=(__ul)&0xff;
#define Ui_To_Pub_LF(__pub,__ui) (__pub)[0]=(__ui)&0xff;(__ui)>>=8;(__pub)[1]=(__ui)&0xff;

#define ADDRESS_ADD(__addr,__v)  	g_stAddress.ubShortAdd=__v;g_stAddress.pubAddress=__addr;Address_vAdd();

#define ADDRESS_SUB(__addr,__v)  	g_stAddress.ubShortSub=__v;g_stAddress.pubAddress=__addr;Address_vSub();

void Address_vAdd();
void Address_vAddTest();


struct STMemery
{
	Uint16 *pubFrom;
	Uint16 *pubTo;
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




void CCP_vCommand();
void CCP_vDAQTransmit();


#endif /* CCP_H_ */
#endif
