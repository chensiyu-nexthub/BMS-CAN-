/********************************************************************************************************
*
* File                : 18B20.h
* Hardware Environment: Open103V
* Build Environment   : RealView MDK-ARM  Version: 4.20
* Version             : V1.0
* By                  : WaveShare
*
*                                  (c) Copyright 2005-2011, WaveShare
*                                       http://www.waveshare.net
*                                          All Rights Reserved
*
*********************************************************************************************************/

#include "DSP28x_Project.h"
#include "18B20.h"

void convertDs18b20(void) 
{ 
    resetOnewire(); 
    wOnewire(0xcc);  //跳过ROM
    wOnewire(0x44);    //温度装换
}


Uint16* readID(void)
{ 
	Uint16 ID[8],i,*p;
	resetOnewire(); 
	wOnewire(0x33);   //读取ROM
	for(i=0;i<8;i++)
	{ID[i]=rOnewire();}
	p=ID;
	return p;
}


Uint16 readTemp(void)
{ 
	Uint16 temp1,temp2;
	convertDs18b20();  //转换
	resetOnewire(); 
	wOnewire(0xcc);   //跳过ROM
	wOnewire(0xbe); 	//暂存
	temp1=rOnewire(); 
	temp2=rOnewire(); 
	temp2=temp2<<4;
	temp1=temp1>>4;
	temp2|=temp1;

	return (temp2&0x7F);
}

