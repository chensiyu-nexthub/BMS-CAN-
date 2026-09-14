//Main.c
//采集板主程序
#include "SEED-DEC2407.h" 
#include "eeprom.h"
#include "eeprom.c"
#include "dpram.h"
#include "dpram.c"
#include "ads7844.h"
#include "ads7844.c"
#include "ds18b20.h"
#include "ds18b20.c"

void SystemInit();
void IOInit();
void SPI_Init();
void Timer1Init(); 
void KickDog();  

unsigned char offlag = 0; //关机标志
float temp[28];	//温度
unsigned char snn[28][8];//28个温度传感器序列号
//int result[30];			//单电压、总电压、总电流值
unsigned char* buff;		//中间值 
int i,j,loopindex;
void main()
{  
   SystemInit();         	//系统初始化
   IOInit(); 
   SPI_Init();			//SPI初始化
   Timer1Init();          	//定时器初始化


   //采集温度
   while (reset()==1){}; /* 复位等待从机应答 */
   write_byte(0xCC); /* 忽略ROM 匹配 */
   write_byte(0x44); /* 发送温度转化命令 */
   for(loopindex=0;loopindex<65535;loopindex++){;} /* 延时300ms,等待数模转换 */
   for(loopindex=0;loopindex<65535;loopindex++){;}
   for(i=0; i<28; i++)
   {
   		while(reset()==1){}; /* 再次复位，等待从机应答 */
   		write_byte(0x55);	//匹配rom命令
   		for(j=0; j<8; j++)write_byte(snn[i][j]); /* 写序列号 */
   		write_byte(0xBE); /* 发送读温度命令 */
   		read_byte(buff); /* 读出温度低8 位 */
   		read_byte(buff+1); /* 读出温度高8 位 */
   		temp[i]=transform(buff);
   		PIN_HIGH(); /* 释放总线 */
   }
   WR2DRAM(temp,0x801E,28);//将读出的数存入双口RAM
   WT2EEPROM(temp,0x001E,28);//将数据写入EEPROM
   
   //采集电压电流
   for(i=0; i<4; i++)
   {
		if(i==0)*PBDATDIR=*PBDATDIR&0xFFFE;//模拟CS0
  		if(i==1)*PBDATDIR=*PBDATDIR&0xFFFD;//模拟CS1
   		if(i==2)*PBDATDIR=*PBDATDIR&0xFFFB;//模拟CS2
   		if(i==3)*PBDATDIR=*PBDATDIR&0xFFF7;//模拟CS3
		ADConversion(i);	//读出第i个ad的转化结果-12位二进制数，并存在result数组中
   }
	//	convert(adread);		//数据转化   
   WR2DRAM(result,0x8000,30);//将读出的数存入双口RAM
   WR2EEPROM(result,0x0000,30);//将数据写入EEPROM
   
   asm(" CLRC INTM ");
   while(offlag == 0)
   {
   		
   		int j;
   		while (reset()==1){}; /* 复位等待从机应答 */
   		write_byte(0xCC); /* 忽略ROM 匹配 */
   		write_byte(0x44); /* 发送温度转化命令 */
   		for(loopindex=0;loopindex<65535;loopindex++){;} /* 延时300ms,等待数模转换 */
   		for(loopindex=0;loopindex<65535;loopindex++){;}
   		for(i=0; i<28; i++)
   		{
   			while(reset()==1){}; /* 再次复位，等待从机应答 */
   			write_byte(0x55);	//匹配rom命令
   			for(j=0; j<8; j++)write_byte(snn[i][j]); /* 写序列号 */
   			write_byte(0xBE); /* 发送读温度命令 */
   			read_byte(buff); /* 读出温度低8 位 */
   			read_byte(buff+1); /* 读出温度高8 位 */
   			temp[i]=transform(buff);
   			PIN_HIGH(); /* 释放总线 */
   		}
  		WR2DRAM(temp,0x801E,28);//将读出的数存入双口RAM
   		WT2EEPROM(temp,0x001E,28);//将数据写入EEPROM			
   }
}

void SystemInit() 
{  
 
	asm(" SETC	INTM ");	//关闭总中断 
	asm(" CLRC  SXM  ");	//禁止符号位扩展 
	asm(" CLRC  CNF  ");    // B0块映射为 on-chip DARAM
	asm(" CLRC  OVM  ");    // 累加器结果正常溢出
	*SCSR1=0x83FE;         	// 系统时钟CLKOUT=20*2=40M 
	*WDCR=0x006F;			//禁止看门狗,看门狗时钟64分频 
	KickDog();    		// 初始化看门狗  
    *IFR=0xFFFF;			// 清除中断标志 
    *IMR=0x0002;				//打开中断2      
    
}

void IOInit()
{
   *MCRA=*MCRA & 0x10FF; 	    //除IOPB4以外IOPB0-7都设为IO口模式
   *PBDATDIR=0xCF0F;        	    //置IOPB5为输入口
   *MCRC=*MCRC & 0x80FF; 	    //IOPF0-6都设为IO口模式
}

void SPI_Init()
{
	*MCRB=*MCRB|0x001C;		//SPISIMO,SPISOMI,SPICLK特殊功能方式,SPISTE配置为通用I/O引脚以作片选信号
	*PCDATDIR=*PCDATDIR|0x0020;	//置片选信号为高
	*SPICCR=0x004F;			//上升（下降）沿输出（入）16位数据
	*SPICTL=0x0006;			//主工作方式，禁止中断
	*SPIBRR=0x0027;			//1M波特率
	*SPICCR=*SPICCR|0X80;		//开始发送或接收数据
}
  
void Timer1Init()
{ 

    *EVAIMRA=0x0080;			//  定时器1周期中断使能 
    *EVAIFRA=0xFFFF;			//  清除中断标志 
    *GPTCONA=0x0000;
	*T1PR=2500;				//  定时器1初值,定时128/40us*2500=8ms 			
	*T1CNT=0;
	*T1CON=0x174E;              //增模式, TPS系数40M/128, T1使能
				
}

void interrupt c_int2()               //定时器1中断服务程序
{

    if(*PIVR!=0x27)
		{  	asm(" CLRC INTM ");
			return;
		}
	*T1CNT=0; 
	//采集电压电流
    for(j=0; i<4; i++)
    {
		if(i==0)*PBDATDIR=*PBDATDIR&0xFFFE;//模拟CS0
  		if(i==1)*PBDATDIR=*PBDATDIR&0xFFFD;//模拟CS1
   		if(i==2)*PBDATDIR=*PBDATDIR&0xFFFB;//模拟CS2
   		if(i==3)*PBDATDIR=*PBDATDIR&0xFFF7;//模拟CS3
		AD(result,i);	//读出第i个ad的转化结果
    }
//		convert(adread);		//数据转化    
    WR2DRAM(result,0x8000,30);//将读出的数存入双口RAM
    WR2EEPROM(result,0x0000,30);//将数据写入EEPROM    
	
	*EVAIFRA=0x80; 
	asm(" CLRC	INTM ");   
	
} 
void KickDog()				     //踢除看门狗 
{
	*WDKEY=0x5555;
	*WDKEY=0xAAAA;
}

