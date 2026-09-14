/*
 * CAN.h
 *
 *  Created on: Apr 16, 2010
 *      Author: Administrator
 */

#ifndef CAN_H_
#define CAN_H_
#include "XC22xxMREGS.h"

#define SetListCommand(ValueH, ValueL) CAN_PANCTRH = ValueH; CAN_PANCTRL = ValueL; while(CAN_PANCTRL & CAN_PANCTR_BUSY);

 // Panel Busy Flag
 #define CAN_PANCTR_BUSY       0x0100

 // Message Object Status Register
 #define MOSTAT_RXPND          0x0001
 #define MOSTAT_TXPND          0x0002
 #define MOSTAT_NEWDAT         0x0008
 #define MOSTAT_MSGLST         0x0010
 #define MOSTAT_RST_MNR        0x0019
 #define MOSTAT_RST_NT         0x000A

#define CAN_SRN0INT    0x40

 struct stCanObj
 {
   uword  uwMOFCRL;    // Function Control Register Low
   uword  uwMOFCRH;    // Function Control Register High
   uword  uwMOFGPRL;   // FIFO/Gateway Pointer Register Low
   uword  uwMOFGPRH;   // FIFO/Gateway Pointer Register High
   uword  uwMOIPRL;    // Interrupt Pointer Register Low
   uword  uwMOIPRH;    // Interrupt Pointer Register High
   uword  uwMOAMRL;    // Acceptance Mask Register Low
   uword  uwMOAMRH;    // Acceptance Mask Register High
   ubyte  ubData[8];   // Message Data 0..7
   uword  uwMOARL;     // Arbitration Register Low
   uword  uwMOARH;     // Arbitration Register High
   uword  uwMOCTRL;    // Control Register Low
   uword  uwMOCTRH;    // Control Register High
 };

 typedef struct
 {
     ubyte ubMOCfg;    // message object configuration
     ulong ulID;       // standard (11-bit)/extended (29-bit) identifier
     ulong ulMask;     // standard (11-bit)/extended (29-bit) mask
     ubyte ubData[8];  // 8-bit data bytes
     uword uwCounter;  // frame counter(MOIPRnH[15-0])
 }stCAN_SWObj;

#ifndef PF_Error_AddERROR_TYPE
#define PF_Error_AddERROR_TYPE
typedef int (*PF_Error_AddERROR)(unsigned char value);
#endif

#ifndef PF_CANRECEIVEINTERRUPTPROCESS
#define PF_CANRECEIVEINTERRUPTPROCESS
typedef void (*PF_CANReceiveInterruptProcess)(unsigned char volatile __far* dataBuffer);
#endif

#define CAN_HWOBJ ((struct stCanObj volatile far*) 0x201000)

#define CAN_OBJECT_CONFIGURE(_num_,_ID_) \
      CAN_MOCTR##_num_##H = 0x00A0; \
      CAN_MOCTR##_num_##L = 0x0000;\
      CAN_MOAR##_num_##H = 0x4000+(_ID_<<2);\
      CAN_MOAR##_num_##L = 0x0000;\
      CAN_MOAMR##_num_##H = 0x3fff;\
      CAN_MOAMR##_num_##L = 0xffff;\
      CAN_MOIPR##_num_##H = 0x0000;\
      CAN_MOIPR##_num_##L = 0x0000;\
      CAN_MOFGPR##_num_##H = 0x0000;\
      CAN_MOFGPR##_num_##L = 0x0000;

#define CAN_ubNewData(ObjNr) (((CAN_HWOBJ[ObjNr].uwMOCTRL & 0x0008) == 0x0008) \
   ? 1 : 0)

void CAN_vTransmit(ubyte ubObjNr);
void CAN_Send(unsigned char* sendBuffer);
void CAN_Init(PF_Error_AddERROR pfError,PF_CANReceiveInterruptProcess pfInterrupt,unsigned int timerCounter);

#endif /* CAN_H_ */
