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
	UINT8 pubAddress[4];
	UINT8 ubLength;
};


struct STDaqList
{
  struct STOdtEntry    pstOdt[CCP_MAX_ODT][7];
  UINT16	uiPrescaler;
  UINT16	uiCycle;
  UINT8	ubEventChannel;
  UINT8 ubLast;
  UINT8 ubFlags;
};

#define CCP_STATION_ADDR   0x111
#define CCP_BROADCAST_STATION_ADDR 0

struct STCCP
{
	UINT8* pubCANReceive;
	UINT8 pubCANTransmit[8];
	UINT8 ubSessionStatus;
	UINT8 ubUserSessionStatus;
	UINT8 ubSendStatus;
	struct STDaqList pstDaqList[CCP_MAX_DAQ];
	UINT8 pbStationID[32];
	UINT8 ubProtectionStatus;//If set the bit,means the option is not protected.
	UINT8 ubCurrentDaq;
	UINT8 ubCurrentOdt;
	struct STOdtEntry* pstOdtEntry;
	UINT8 ppubMTA[2][4];
};

void CCP_vCommand();
void CCP_vDAQTransmit();


#endif /* CCP_H_ */
#endif
