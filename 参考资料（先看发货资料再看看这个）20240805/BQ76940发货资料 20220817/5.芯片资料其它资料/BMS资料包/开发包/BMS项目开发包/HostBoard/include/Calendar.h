#ifndef CALENDAR_H
#define CALENDAR_H

#ifndef ERROR_ADD
#define ERROR_ADD(__E) m_pError(__E);
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
#define	ERROR_TIMEROVERFLOW_AT_Calendar_SetYearDayFromIIC 0x2d
#define	ERROR_OVERFLOW_AT_Calendar_GetMonthFromIIC 0x2e
#define	ERROR_OVERFLOW_AT_Calendar_GetHourFromIIC 0x2f
#define	ERROR_OVERFLOW_AT_Calendar_GetMinuteFromIIC 0x30
#define	ERROR_OVERFLOW_AT_Calendar_GetSecondFromIIC 0x31
#define	ERROR_NOACK_AT_IIC_Write 0x32
#define ERROR_NOACK_AT_IIC_Read 0x33
#endif

#ifndef PF_Error_AddERROR_TYPE
#define PF_Error_AddERROR_TYPE
typedef int (*PF_Error_AddERROR)(unsigned char value);
#endif

	unsigned char Calendar_GetDay();
	unsigned char Calendar_GetMonth();
	unsigned char Calendar_GetYear();
	unsigned char Calendar_GetHour();
	unsigned char Calendar_GetMinute();
	unsigned char Calendar_GetSecond();
	void Calendar_LoadDate();

	void Calendar_SetDay(unsigned char value);
	void Calendar_SetMonth(unsigned char value);
	void Calendar_SetYear(unsigned char value);
	void Calendar_SetHour(unsigned char value);
	void Calendar_SetMinute(unsigned char value);
	void Calendar_SetSecond(unsigned char value);

	void Calendar_Init(PF_Error_AddERROR pError,unsigned int timerCounter);

	void Calendar_SetDate(unsigned char yearValue,unsigned char monthValue,
			unsigned char dayValue,unsigned char hourValue,unsigned char minuteValue,unsigned char secondValue);


#endif
