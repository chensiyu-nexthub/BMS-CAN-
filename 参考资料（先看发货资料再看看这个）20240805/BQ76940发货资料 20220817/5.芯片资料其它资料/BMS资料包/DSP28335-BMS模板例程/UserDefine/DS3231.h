
#ifndef __DS3231_H__
#define __DS3231_H__


typedef struct 
{
	Uint16 hour;
	Uint16 min;
	Uint16 sec;
	Uint16 w_year;
	Uint16  w_month;
	Uint16  w_date;
	Uint16  week;
}_calendar_obj;					 
extern _calendar_obj calendar;	//日历结构体

extern Uint16 const mon_table[12];	//月份日期数据表

void DS3231_Init(void);
void get_show_time(void);      
//u8 RTC_Get_Week(u16 year,u8 month,u8 day);
void DS3231_Set(Uint16 syear,Uint16 smon,Uint16 sday,Uint16 hour,Uint16 min,Uint16 sec);//设置时间

#endif

