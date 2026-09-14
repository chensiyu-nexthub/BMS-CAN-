//主 dsp 主程序
#include "SEED-DEC2407.h" 
#include "socalgorithm.h"
#include "socalgorithm.c"
#include "pcf8583.h"
#include "pcf8583.c"
#include "eeprom.h"
#include "eeprom.c"
#include "dpram.h"
#include "dpram.c"
#include "lcd.h"
#include "lcd.c"

void SystemInit();
void IOInit();
void SPI_Init();
void KickDog();

#define TimeConstant 60	//定义放置时间常数

int i,k, flag=0,sflag=0;	//flag为数据接收完成标志，sflag为充放电标志
int data[30];			//28个电压和总电压、总电流值
int temp[28];			//28个温度值
float avtemp=0;			//平均温度
float soc[28];			//开路电压法计算各组电池的soc值
float soc1;				//当前时刻的soc
float soc0=1;			//前一时刻的soc
int oldtime[7];			//上次结束时间
int newtime[7];			//当前时间
int PT;					//放置时间
int p=0;				//soc在EEPROM中存放位置

void main()
{
	SystemInit();         	//系统初始化
   	IOInit(); 
   	SPI_Init();			//SPI初始化
	*XINT1CR=0x05;		//上升沿使能高优先级INT1
    *IFR=0xFFFF;			// 清除中断标志 
	*IMR=0x01;			//使能INT1中断	
   	asm(" CLRC INTM ");	//开总中断
	//读取系统上次停机和当前开机时间并计算差值
	k= CON_RD(0x10,oldtime,7);
	if(k==0)k=CON_RD(0x01,newtime,7);
	if(k==0)PT=CalPlaceTime(newtime,oldtime);
	if(PT<TimeConstant)
	{
		k=CON_RDF(0x17,&soc0,1);
		if(k==0)displaysoc(soc0);//显示soc
	}
	else
	//falg为数据读取标志、sflag充放电标志
	{
		while(1)
		{
			while(flag==1)
			{
				if((data[28]==0)&(sflag==0))//电流为0，且没有充放电，且开路电压法计算SOC
				{
					for(i=0; i<28; i++)
					{
						soc[i]=OCV(data[i],temp[i]);//在考虑老化的情况下开路电压随温度电压变化的表格不同
						if(soc[i]<soc0)soc0=soc[i];//最小的soc
					}
					displaysoc(soc0);//显示soc
					WR2EEPROMF(&soc0,p++,1);//写入EEPROM
					flag=0;
				}
				else if((data[28]==0)&(sflag==1))
				{
					flag=0;
					sflag=0;
					k=CON_RD(0x01,newtime,7);
					if(k==0)k=CON_WR(0x10,newtime,7);	//保存时间以及soc到pcf8583
					if(k==0)CON_WRF(0x17,&soc1,28);
				}
				else//安时法计算soc
				{
					for(k=0;k<28;k++)avtemp+=temp[k];
					avtemp=avtemp/28;
					soc1=soc0-Deltasoc(data[28],avtemp);
					displaysoc(soc1);//显示soc
					WR2EEPROMF(&soc1,p++,1);//写入EEPROM				
					soc0=soc1;
					flag=0;
					sflag=1;
				}	
			}
		}
	}	
}

void SystemInit()
{
	asm(" SETC	INTM ");		//关闭总中断 
	asm(" CLRC  SXM ");		//禁止符号位扩展 
	asm(" CLRC  CNF ");    	// B0块映射为 on-chip DARAM
	asm(" CLRC  OVM ");   	// 累加器结果正常溢出
	*SCSR1=0x83FE;         	// 系统时钟CLKOUT=20*2=40M 
	*WDCR=0x006F;			//禁止看门狗,看门狗时钟64分频 
	KickDog();    			// 初始化看门狗
}

void IOInit()
{
	*MCRA=*MCRA & 0x000F; 	    //IOPB0-7、IOPA4-7都设为IO口模式，IOPA2用作外部中断、IOPA01用作SCI
  	*PADATDIR=(*PADATDIR|0xF000)&0xFF0F;   	    //置IOPA4-7为输出口且输出0
	*MCRC=*MCRC&0xFCFF;		//IOPF0、1为IO口模式
	*PFDATDIR=*PFDATDIR|0x0303;		//IOPF0、1都输出1
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

void interrupt GISR1()		//XINT1中断服务程序
{
	int dflag;
	if(*PIVR!=0x0001)
	{
		asm(" CLRC INTM ");
		return;
	}
	//读采集板传的数据
	dflag=*INTMBR;//RDDRAM(dflag,INTMBR,1);
	if(dflag==30)//读电压电流值
	{
	RDDRAM(data, (unsigned int*)0x8000, dflag);	
	flag=1;
	}
	else RDDRAM(temp, (unsigned int*)0x801E, dflag);//读温度值	

	asm(" CLRC INTM ");
	return;
}

void KickDog()				     //踢除看门狗 
{
	*WDKEY=0x5555;
	*WDKEY=0xAAAA;
}

