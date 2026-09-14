/*
 * SPI.c
 *
 *  Created on: 2016-5-4
 *      Author: James
 */


#include "DSP28x_Project.h"
#include "Display.h"
#include <stdio.h>
#include <stdlib.h>


char Display[4][17];   //LCD显示缓存
char ControlDisplayL[17]="  HDA_N   DDC_N  ";
char ControlDisplayH[17]="  ACI_N   DCI_N  ";


void SPI_Gpio(void)
{
   EALLOW;
	GpioCtrlRegs.GPBPUD.bit.GPIO48 = 0;   // Enable pull-up on GPIO56 (SPICLKA)
	GpioCtrlRegs.GPBMUX2.bit.GPIO48 = 0; // Configure GPIO54 as SPISIMOA
	GpioCtrlRegs.GPBDIR.bit.GPIO48 = 1;


	GpioCtrlRegs.GPBPUD.bit.GPIO54 = 0;   // Enable pull-up on GPIO56 (SPICLKA)
	GpioCtrlRegs.GPBMUX2.bit.GPIO54 = 0; // Configure GPIO54 as SPISIMOA
	GpioCtrlRegs.GPBDIR.bit.GPIO54 = 1;

	GpioCtrlRegs.GPBPUD.bit.GPIO55 = 0;   // Enable pull-up on GPIO56 (SPICLKA)
	GpioCtrlRegs.GPBMUX2.bit.GPIO55 = 0; // Configure GPIO54 as SPISIMOA
	GpioCtrlRegs.GPBDIR.bit.GPIO55 = 1;
	EDIS;
}

void SendByte(Uint16 Dbyte)
{
	Uint16 i;
	SID_OUT();
	for(i=0;i<8;i++)
	{
	   SCK = 0 ;
	 DELAY_US(1);
	   if((Dbyte<<i)&0x80)  SID = 1 ;
	   else SID = 0 ;           //移出的位给SID
	 DELAY_US(1);
	   SCK = 1 ;
	 DELAY_US(1);
	   SCK = 0 ;
	 DELAY_US(1);
	}
}


Uint16 LCD12864_busy(void)
{
	Uint16 x;
    SendByte(0xfc);             //11111,RW(1),RS(0),0
    x=(0x80 & ReceiveByte());      //BF(.7)=1 Busy
	return x;
}


void LCD12864_com(Uint16 dat)
{
	CS = 1;
//	while(LCD12864_busy()); //忙检测
	SendByte(0xf8);            //11111,RW(0),RS(0),0  Rw到lcd  rs=0写指令
    SendByte(0xf0&dat);      //高四位
    SendByte(0xf0&dat<<4);  //低四位(先执行<<)
    CS = 0;
}


void LCD12864_dat(Uint16 dat)
{
     CS = 1;
//     while(LCD12864_busy()); //忙检测

     SendByte(0xfa);            //11111,RW(0),RS(1),0
     SendByte(0xf0&dat);      //高四位
     SendByte(0xf0&dat<<4);   //低四位(先执行<<)
     
	 DELAY_US(2);
     CS = 0;
	 DELAY_US(2);
}


void SPI_LCD_Init(void)
{
	SPI_Gpio();

	DELAY_US(10000);

    LCD12864_com(0x30);
    DELAY_US(100);

    LCD12864_com(0x30);
    DELAY_US(100);

    LCD12864_com(0x08);
    DELAY_US(100);

    LCD12864_com(0x10);
    DELAY_US(100);

    LCD12864_com(0x0c);
    DELAY_US(100);

    LCD12864_com(0x01);
    DELAY_US(100);

	 LCD12864_com(0x06);
	 DELAY_US(100);

}


Uint16 ReceiveByte(void)
{
     Uint16 i,temp1,temp2;
     temp1 = 0;
     temp2 = 0;
     SID_IN()   //SID所在管脚设置成输入
     for(i=0;i<8;i++)
     {
           temp1=temp1<<1;
           SCK = 0 ;
		 DELAY_US(1);
           SCK = 1;
		 DELAY_US(1);
           SCK = 0 ;
           if(SIDIN) temp1++;
     }
     for(i=0;i<8;i++)
     {
           temp2=temp2<<1;
           SCK = 0 ;
		 DELAY_US(1);
           SCK = 1 ;
		 DELAY_US(1);
           SCK = 0 ;
           if(SIDIN) temp2++;
     }
     temp1=0xf0&temp1;
     temp2=(0xf0&temp2)>>4;

     SID_OUT()  //SID所在管脚设置成输出
     return (temp1+temp2);

}

Uint16 ReadData( void )
{
//     while(LCD12864_busy()); //忙检测
     SendByte(0xfe);         //11111,RW(1),RS(1),0
     return ReceiveByte();
}



void LCD12864_Clr(void)
{
	LCD12864_com(0x34);   //扩充指令操作	“绘图”
	DELAY_US(2000);
    LCD12864_com(0x30);    //基本指令操作
	DELAY_US(2000);
	LCD12864_com(0x01);   //清屏
	DELAY_US(2000);
}

void AD_Refresh(void)
{
//	sprintf(Display[2],"Ud:%5dUc:%5d"   ,(Uint16)Volt_Vd,(Uint16)Volt_Vc);
}






