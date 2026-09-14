
#include "DSP28x_Project.h"
#include "DS3231.h"
#include "iic.h"
     
_calendar_obj calendar;

#define DS3231_WriteAddress 0xD0   
#define DS3231_ReadAddress  0xD1

Uint16 BCD2HEX(Uint16 val)
{
    Uint16 i;
    i= val&0x0f;
    val >>= 4;
    val &= 0x0f;
    val *= 10;
    i += val;
    
    return i;
}

Uint16 B_BCD(Uint16 val)
{
  Uint16 i,j,k;
  i=val/10;
  j=val%10;
  k=j+(i<<4);
  return k;
}


void I2cByteWrite(Uint16 addr,Uint16 bytedata)
{
  IIC_Start();
  DELAY_US(5);
  IIC_Send_Byte(DS3231_WriteAddress);
  IIC_Wait_Ack();
  DELAY_US(5);
  IIC_Send_Byte(addr);
  IIC_Wait_Ack();
  DELAY_US(5);
  IIC_Send_Byte(bytedata);
  IIC_Wait_Ack();
  DELAY_US(5);
  IIC_Stop();
}	


Uint16 I2cByteRead(Uint16 addr)
{
  Uint16 Dat=0;
  
  IIC_Start();
  IIC_Send_Byte(DS3231_WriteAddress);
  IIC_Wait_Ack();
  DELAY_US(5);
  IIC_Send_Byte(addr);
  IIC_Wait_Ack();
  DELAY_US(5);
  IIC_Start();
  IIC_Send_Byte(DS3231_ReadAddress);
  IIC_Wait_Ack();
  DELAY_US(5);
  Dat=IIC_Read_Byte(1);
  IIC_Stop();
  return Dat;
} 



void DS3231_Init(void)
{
	IIC_Init();
	I2cByteWrite(0x0e,0);
	DELAY_US(2000);
    I2cByteWrite(0x0f,0x0);
    DELAY_US(2000);
}
void DS3231_Get(void)
{
  calendar.w_year=I2cByteRead(0x06);  
  calendar.w_month=I2cByteRead(0x05);  
  calendar.w_date=I2cByteRead(0x04); 
  calendar.hour=I2cByteRead(0x02);  
  calendar.min=I2cByteRead(0x01);
  calendar.sec=I2cByteRead(0x00);
}

void DS3231_Set(Uint16 yea,Uint16 mon,Uint16 da,Uint16 hou,Uint16 min,Uint16 sec)
{
  Uint16 temp=0;
  
  temp=B_BCD(yea);
  I2cByteWrite(0x06,temp);
  
  temp=B_BCD(mon);
  I2cByteWrite(0x05,temp);
   
  temp=B_BCD(da);
  I2cByteWrite(0x04,temp);
  
  temp=B_BCD(hou);
  I2cByteWrite(0x02,temp);
  
  temp=B_BCD(min);
  I2cByteWrite(0x01,temp);
  
  temp=B_BCD(sec);
  I2cByteWrite(0x00,temp);
}


void get_show_time(void)
{


calendar.w_year=I2cByteRead(0x06);  
calendar.w_year=BCD2HEX(calendar.w_year);


calendar.w_month=I2cByteRead(0x05); 
calendar.w_month=BCD2HEX(calendar.w_month);


calendar.w_date=I2cByteRead(0x04);  
calendar.w_date=BCD2HEX(calendar.w_date);
 

calendar.hour=I2cByteRead(0x02); 
calendar.hour&=0x3f;                   
calendar.hour=BCD2HEX(calendar.hour);


calendar.min=I2cByteRead(0x01);
calendar.min=BCD2HEX(calendar.min);
  
  
calendar.sec=I2cByteRead(0x00);
calendar.sec=BCD2HEX(calendar.sec);
}


