/*
 * EEPROM.h
 *
 *  Created on: Apr 20, 2010
 *      Author: Administrator
 */

#ifndef EEPROM_H_
#define EEPROM_H_

#ifndef ERROR_EEPROM_GLAG
#define ERROR_EEPROM_GLAG

#define ERROR_TIMERLIMIT_AT_U1C1_SSC_ubTxDataReady 0x35
#define ERROR_TIMEROVERFLOW_AT_SPI_ReadEEPROMStatus 0x36
#define ERROR_TIMEROVERFLOW_AT_SPI_WriteEEPROMEnable 0x37
#define ERROR_TIMEROVERFLOW_AT_EEPROM_Init 0x38
#define ERROR_TIMEROVERFLOW_AT_EEPROM_ReadEEPROM 0x39
#define ERROR_TIMEROVERFLOW_AT_EEPROM_WriteEEPROM 0x3a

#endif/*ERROR_EEPROM_GLAG*/

#ifndef PF_Error_AddERROR_TYPE
#define PF_Error_AddERROR_TYPE
typedef int (*PF_Error_AddERROR)(unsigned char value);
#endif

#ifndef TIMER_ENSURE
#define TIMER_ENSURE(__S) 	if(m_startTimer_2+m_timerCounter_2<RTC_T14) m_pError_2(__S);
#endif


unsigned char EEPROM_WriteEEPROM (unsigned int startAddress,unsigned char* writeAddress,unsigned int count);
unsigned char EEPROM_ReadEEPROM (unsigned int startAddress,unsigned char* returnAddress,unsigned int count);
void EEPROM_Init(PF_Error_AddERROR pError,unsigned int timerCounter);

#endif /* EEPROM_H_ */
