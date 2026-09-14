#include "Config.h"
#include "AllEvents.h"
#include "PE_Types.h"
#include "DS1302.h"
#include "TSCL.h"
#include "TSDA.h"
#include "TRST.h"
#include "PC.h"

#define  DS_RST_OUT            TRST_SetOutput()
#define  DS_RST0               TRST_ClrVal()
#define  DS_RST1               TRST_SetVal()

#define  DS_SCL_OUT            TSCL_SetOutput()
#define  DS_SCL0               TSCL_ClrVal()
#define  DS_SCL1               TSCL_SetVal()

#define  DS_SDA_OUT            TSDA_SetOutput()
#define  DS_SDA_IN             TSDA_SetInput()
#define  DS_SDA0               TSDA_ClrVal()
#define  DS_SDA1               TSDA_SetVal()
#define  DS_SDA_BIT            TSDA_GetVal()




/****************************************************************************
                               函数原型声明：
****************************************************************************/

void delay(word);

void Up_DataRTC(void);

void Initial_Ds1302(void);

void Reset_DS1302(void);

void Write1Byte(byte);

void Write1Byte(byte);

byte Read1Byte(void);

void W_Data(byte, byte);

byte R_Data(byte);

void BurstWrite1302(byte *);

void BurstRead1302(byte *);

void BurstWriteRAM(byte *);

void BurstReadRAM(byte *);

void Set_DS1302(byte *); 

void Get_DS1302(byte *); 

void Set_RTC(void);

void Get_RTC(void);



/****************************************************************************
函数名称：delay
输入参数：word dly
输出参数：无
描述：    
****************************************************************************/

void delay(word dly)
{
	while(dly)
	{
		dly--;		
	}
}

/****************************************************************************
函数名称：Read_Ds1302
输入参数：无
输出参数：无
描述：    无
****************************************************************************/

void Up_DataRTC(void)
{

	if(Timer.Ds1302_UpdataCnt >= TIMER_1000MS)
	{
		Timer.Ds1302_UpdataCnt=0;
		Get_RTC();			
	}		
}

/****************************************************************************
函数名称：delay
输入参数：word dly
输出参数：无
描述：    
****************************************************************************/

void Set_RTC(void)
{
	byte Set_Buf[7];
	Set_Buf[0] = DS1302.sec;
	Set_Buf[1] = DS1302.min;
	Set_Buf[2] = DS1302.hour;
	Set_Buf[3] = DS1302.day;
	Set_Buf[4] = DS1302.month;
	Set_Buf[5] = DS1302.Weeks;
	Set_Buf[6] = DS1302.year;
	
	Set_DS1302(Set_Buf);
}

/****************************************************************************
函数名称：delay
输入参数：word dly
输出参数：无
描述：    
****************************************************************************/

void Get_RTC(void)
{
	byte Rtc_buf[7];
	Get_DS1302(Rtc_buf);
	
	DS1302.sec   = Rtc_buf[0];
	DS1302.min   = Rtc_buf[1];
	DS1302.hour  = Rtc_buf[2];
	DS1302.day   = Rtc_buf[3];
	DS1302.month = Rtc_buf[4];
	DS1302.Weeks = Rtc_buf[5];
	DS1302.year  = Rtc_buf[6];
			
	(void) PC_SendChar(DS1302.sec);	
	(void) PC_SendChar(DS1302.min);
	(void) PC_SendChar(DS1302.hour);
	(void) PC_SendChar(DS1302.day);
	(void) PC_SendChar(DS1302.month);
	(void) PC_SendChar(DS1302.Weeks);
	(void) PC_SendChar(DS1302.year);			
}

/****************************************************************************
函数名称：delay
输入参数：word dly
输出参数：无
描述：    
****************************************************************************/

void Initial_Ds1302(void)
{
	Reset_DS1302();
	
	
	DS1302.sec    = 0 ;
	DS1302.min    = 24;
	DS1302.hour   = 16;  
	DS1302.day    = 27;
	DS1302.month  = 5 ;
	DS1302.Weeks  = 5 ;
	DS1302.year   = 11;
	Set_RTC(); 
}

/*******************************************
函数名称：Reset_DS1302
功    能：对DS1302进行复位操作
参    数：无
返回值  ：无
********************************************/
void Reset_DS1302(void)
{
    DS_RST_OUT;  //RST对应的IO设置为输出状态
    DS_SCL_OUT;  //SCLK对应的IO设置为输出状态
    DS_SCL0;     //SCLK=0
    DS_RST0;     //RST=0
    delay(DLY_CNT);
    DS_SCL1;    //SCLK=1
}


/*******************************************
函数名称：Write1Byte
功    能：对DS1302写入1个字节的数据
参    数：wdata--写入的数据
返回值  ：无
********************************************/
void Write1Byte(byte wdata) 
{ 
    byte i;

    DS_SDA_OUT;     //SDA对应的IO设置为输出状态
    DS_RST1;        //REST=1;

    for(i=8; i>0; i--) 
    {
        if(wdata&0x01)  DS_SDA1;
        else            DS_SDA0;
        DS_SCL0;
        delay(DLY_CNT); 
        DS_SCL1;
        delay(DLY_CNT); 
        wdata >>=1; 
    } 
}

/*******************************************
函数名称：Read1Byte
功    能：从DS1302读出1个字节的数据
参    数：无
返回值  ：读出的一个字节数据
********************************************/
byte Read1Byte(void) 
{ 
    byte i;
    byte rdata=0X00;

    DS_SDA_IN;  //SDA对应的IO设置为输入状态
    DS_RST1;    //REST=1;

    for(i=8; i>0; i--)
    {
        DS_SCL1;
        delay(DLY_CNT);
        DS_SCL0;
        delay(DLY_CNT);
        rdata >>=1;
        if(DS_SDA_BIT)  rdata |= 0x80;
    } 
    
    return(rdata); 
}

/*******************************************
函数名称：W_Data
功    能：向某个寄存器写入一个字节数据
参    数：addr--寄存器地址
          wdata--写入的数据
返回值  ：无
********************************************/
void W_Data(byte addr, byte wdata)
{
    DS_RST0; 
    DS_SCL0;   
    delay(DLY_CNT);
    DS_RST1;
    Write1Byte(addr);   //写入地址
    Write1Byte(wdata);  //写入数据
    DS_SCL1;
    DS_RST0;
} 
/*******************************************
函数名称：R_Data
功    能：从某个寄存器读出一个字节数据
参    数：addr--寄存器地址
返回值  ：读出的数据
********************************************/
byte R_Data(byte addr)
{
    byte rdata;
    
    DS_RST0; 
    DS_SCL0; 
    delay(DLY_CNT);
    DS_RST1;
    Write1Byte(addr);    //写入地址
    rdata = Read1Byte();  //读出数据 
    DS_SCL1;
    DS_RST0;
    
    return(rdata);
}

/*******************************************
函数名称：BurstWrite1302
功    能：以burst方式向DS1302写入批量时间数据
参    数：ptr--指向时间数据存放地址的指针
返回值  ：读出的数据
说    明：时间数据的存放格式是：
          秒，分，时，日，月，星期，年，控制
            【7个数据（BCD格式）+1个控制】
********************************************/
void BurstWrite1302(byte *ptr)
{
    byte i;
    
    W_Data(0x8e,0x00);      //允许写入     
    DS_RST0; 
    DS_SCL0; 
    delay(DLY_CNT);
    DS_RST1;
    Write1Byte(0xbe);       // 0xbe:时钟多字节写入命令 
    for (i=8; i>0; i--) 
    {
        Write1Byte(*ptr++); 
    }
    DS_SCL1;
    DS_RST0;
    W_Data(0x8e,0x80);      // 禁止写入
} 
/*******************************************
函数名称：BurstRead1302
功    能：以burst方式从DS1302读出批量时间数据
参    数：ptr--指向存放时间数据地址的指针
返回值  ：无
说    明：时间数据的存放格式是：
          秒，分，时，日，月，星期，年，控制
            【7个数据（BCD格式）+1个控制】
********************************************/
void BurstRead1302(byte *ptr)
{
    byte i;
  
    DS_RST0; 
    DS_SCL0; 
    delay(DLY_CNT);
    DS_RST1;
    Write1Byte(0xbf);             //0xbf:时钟多字节读命令
    for (i=8; i>0; i--) 
    {
       *ptr++ = Read1Byte(); 
    }
    DS_SCL1;
    DS_RST0;
}
/*******************************************
函数名称：BurstWriteRAM
功    能：以burst方式向DS1302的RAM中写入批量数据
参    数：ptr--指向存放数据地址的指针
返回值  ：无
说明    ：共写入31个字节的数据
********************************************/
void BurstWriteRAM(byte *ptr)
{
    byte i;
    
    W_Data(0x8e,0x00);         //允许写入
    DS_RST0; 
    DS_SCL0; 
    delay(DLY_CNT);
    DS_RST1;
    Write1Byte(0xfe);          //0xfe:RAM多字节写命令 
    for (i = 31; i>0; i--)     //RAM共有31个字节
    {
        Write1Byte(*ptr++); 
    }
    DS_SCL1;
    DS_RST0; 
    W_Data(0x8e,0x80);          //禁止写入
} 
/*******************************************
函数名称：BurstReadRAM
功    能：以burst方式从DS1302的RAM中读出批量数据
参    数：ptr--指向数据存放地址的指针
返回值  ：无
说明    ：共读出31个字节的数据
********************************************/
void BurstReadRAM(byte *ptr)
{
    byte i;
  
    DS_RST0; 
    DS_SCL0; 
    delay(DLY_CNT);
    DS_RST1;
    Write1Byte(0xff);             //0xff:RAM的多字节读命令 
    for (i=31; i>0; i--) 
    {
       *ptr++ = Read1Byte();   
    }
    DS_SCL1;
    DS_RST0;
}
/*******************************************
函数名称：Set_DS1302
功    能：设置DS1302内部的时间
参    数：ptr--指向存放数据地址的指针
返回值  ：无
说明    ：写入数据的格式：
            秒 分 时 日 月 星期 年  【共7个字节】
********************************************/
void Set_DS1302(byte *ptr) 
{
    byte i;
    byte addr = 0x80; 
    
    W_Data(0x8e,0x00);    //允许写入
    
    for(i =7;i>0;i--)
    { 
        W_Data(addr,*ptr++); 
        addr += 2;
    }
    W_Data(0x8e,0x80);      //禁止
}
/******************************************************************** 
*
* 名称: Get_DS1302
* 说明: 
* 功能: 读取DS1302当前时间
* 调用: R_Data(uchar addr) 
* 输入: ucCurtime: 保存当前时间地址。当前时间格式为: 秒 分 时 日 月 星期 年 
* 7Byte (BCD码) 1B 1B 1B 1B 1B 1B 1B
* 返回值: 无
***********************************************************************/
/*******************************************
函数名称：Get_DS1302
功    能：读取DS1302内部的时间
参    数：ptr--指向数据存放地址的指针
返回值  ：无
说明    ：读出数据的格式：
            秒 分 时 日 月 星期 年  【共7个字节】
********************************************/
void Get_DS1302(byte *ptr) 
{
    byte i;
    byte addr=0x81;
    
    for(i=0;i<7;i++)
    {
        ptr[i]=R_Data(addr);
        addr+=2;
    }
}


































