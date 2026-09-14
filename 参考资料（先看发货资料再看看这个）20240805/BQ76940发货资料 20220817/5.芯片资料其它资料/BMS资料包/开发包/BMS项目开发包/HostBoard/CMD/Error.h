#ifndef ERROR_H
#define ERROR_H
#include "XC22xxMREGS.h"

#define ERROR_SIZE 24

#define NO_ERROR
struct S_Error{
unsigned char m_front;
unsigned char m_back;
unsigned char m_size;
unsigned char m_errorBuffer[ERROR_SIZE];
};

#ifndef SWITCH_PROGRESS
#define SWITCH_PROGRESS(N) case N:ERROR_Process_##N();
#endif

#ifndef ERROR_CALENDAR_GLAG
#define ERROR_CALENDAR_GLAG

#define ERROR_OVERFLOW_AT_Calendar_SetDate 0x21
#define ERROR_AT_Calendar_SetYearDayFromIIC 0x22
#define ERROR_TIMEROVERFLOW_AT_Calendar_SetMonthFromIIC 0x23
#define	ERROR_TIMEROVERFLOW_AT_Calendar_SetHourFromIIC 0x24
#define	ERROR_TIMEROVERFLOW_AT_Calendar_SetMinuteFromIIC 0x25
#define	ERROR_TIMEROVERFLOW_AT_Calendar_SetSecondFromIIC 0x26
#define	ERROR_TIMEROVERFLOW_AT_Calendar_GetMonthFromIIC 0x27
#define	ERROR_TIMEROVERFLOW_AT_Calendar_GetYearDayFromIIC 0x28
#define	ERROR_TIMEROVERFLOW_AT_Calendar_GetHourFromIIC 0x29
#define	ERROR_TIMEROVERFLOW_AT_Calendar_GetMinuteFromIIC 0x2a
#define	ERROR_TIMEROVERFLOW_AT_Calendar_GetSecondFromIIC 0x2b
#define	ERROR_OVERFLOW_AT_Calendar_GetYearDayFromIIC 0x2c
//#define	ERROR_OVERFLOW_AT_Calendar_GetYearDayFromIIC 0x2d
#define	ERROR_OVERFLOW_AT_Calendar_GetMonthFromIIC 0x2e
#define	ERROR_OVERFLOW_AT_Calendar_GetHourFromIIC 0x2f
#define	ERROR_OVERFLOW_AT_Calendar_GetMinuteFromIIC 0x30
#define	ERROR_OVERFLOW_AT_Calendar_GetSecondFromIIC 0x31
#define	ERROR_NOACK_AT_IIC_Write 0x32
#define ERROR_NOACK_AT_IIC_Read 0x33
#endif

#define ERROR_EEPROM_TO_SHAREMEMERY  (1<<2)
#define ERROR_SHAREMEMERY_TO_EEPROM	 (1<<3)

#define ERROR_NO_ANSWER_BOARD_1 0x100
#define ERROR_NO_ANSWER_BOARD_2 0x101

#define ERROR_EVENT_FULL 0x101

int Error_IsEmpty();
void Error_Init();
int Error_AddError(unsigned char value);
int Error_IsFull();
unsigned char Error_GetError();
void Error_Process(unsigned char ERROR);
void Error_vSendError();

#endif
