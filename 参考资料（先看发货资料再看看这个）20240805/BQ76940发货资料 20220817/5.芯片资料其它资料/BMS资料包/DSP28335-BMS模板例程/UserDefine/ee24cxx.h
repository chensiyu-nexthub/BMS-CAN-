/*
 * ee24cxx.h
 *
 *  Created on: 2014-7-25
 *      Author: lenovo
 */



#ifndef EE24CXX_H_

#define EE24CXX_H_

#include "iic.h"



#define AT24C01		127
#define AT24C02		255
#define AT24C04		511
#define AT24C08		1023
#define AT24C16		2047
#define AT24C32		4095
#define AT24C64	    8191
#define AT24C128	16383
#define AT24C256	32767
//Mini STM32开发板使用的是24c02，所以定义EE_TYPE为AT24C02
#define EE_TYPE AT24C02

//=================EEPROM地址设置==============================
#define Rec1LocaRomAdd 0x00
#define Coi1LocaRomAdd 0x02
#define Rec2LocaRomAdd 0x04
#define Coi2LocaRomAdd 0x06
#define PhaLogChA1Add 0x08
#define PhaLogChA2Add 0x09
#define PhaLogChB1Add 0x10
#define PhaLogChB2Add 0x11
#define ChAPhasePolarityAdd 0x12
#define ChBPhasePolarityAdd 0x13
#define AmpLogChAAdd 0x14
#define AmpLogChBAdd 0x15
#define RecordPhaseAdd  0x30
#define YaxisThreAdd 0xf0
#define FrenSetAdd_1 0xf1
#define FrenSetAdd_2 0xf2


Uint16 AT24CXX_ReadOneByte(Uint16 ReadAddr);							//指定地址读取一个字节
void AT24CXX_WriteOneByte(Uint16 WriteAddr,Uint16 DataToWrite);		//指定地址写入一个字节
void AT24CXX_WriteLenByte(Uint16 WriteAddr,Uint16 DataToWrite,Uint16 Len);
Uint16 AT24CXX_ReadLenByte(Uint16 ReadAddr,Uint16 Len);

Uint16 AT24CXX_Check(void);  //检查器件
void AT24CXX_Init(void); //初始化IIC

#endif /* EE24CXX_H_ */


