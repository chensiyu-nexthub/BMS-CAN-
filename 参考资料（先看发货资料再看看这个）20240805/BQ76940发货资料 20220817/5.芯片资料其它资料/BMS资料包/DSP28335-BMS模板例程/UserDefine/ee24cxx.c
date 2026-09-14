/*
 * ee24cxx.c
 *
 *  Created on: 2014-7-25
 *      Author: lenovo
 */
#include "ee24cxx.h"
#include "iic.h"


//初始化IIC接口
void AT24CXX_Init(void)
{
	IIC_Init();
}
//在AT24CXX指定地址读出一个数据
//ReadAddr:开始读数的地址
//返回值  :读到的数据
Uint16 AT24CXX_ReadOneByte(Uint16 ReadAddr)
{
	Uint16 temp=0;
    IIC_Start();
	if(EE_TYPE>AT24C16)
	{
		IIC_Send_Byte(0XAE);	   //发送写命令
		IIC_Wait_Ack();
		IIC_Send_Byte(ReadAddr>>8);//发送高地址
		IIC_Wait_Ack();
	}else IIC_Send_Byte(0XAE + ((ReadAddr/256)<<1));   //发送器件地址0XAE,写数据

	IIC_Wait_Ack();
    IIC_Send_Byte(ReadAddr%256);   //发送低地址
	IIC_Wait_Ack();

	IIC_Start();
	IIC_Send_Byte(0XAF);           //进入接收模式
	IIC_Wait_Ack();
    temp=IIC_Read_Byte(0);
    IIC_Stop();//产生一个停止条件
	return temp;
}
//在AT24CXX指定地址写入一个数据
//WriteAddr  :写入数据的目的地址
//DataToWrite:要写入的数据
void AT24CXX_WriteOneByte(Uint16 WriteAddr,Uint16 DataToWrite)
{
    IIC_Start();
	if(EE_TYPE>AT24C16)
	{
		IIC_Send_Byte(0XAE);	    //发送写命令
		IIC_Wait_Ack();
		IIC_Send_Byte(WriteAddr>>8);//发送高地址
 	}else
	{
		IIC_Send_Byte(0XAE + ((WriteAddr/256)<<1));   //发送器件地址0XAE,写数据
	}
	IIC_Wait_Ack();
    IIC_Send_Byte(WriteAddr%256);   //发送低地址
	IIC_Wait_Ack();
	IIC_Send_Byte(DataToWrite);     //发送字节
	IIC_Wait_Ack();
    IIC_Stop();//产生一个停止条件
    DELAY_US(5000);
}

//在AT24CXX里面的指定地址开始写入长度为Len的数据
//该函数用于写入16bit或者32bit的数据.
//WriteAddr  :开始写入的地址
//DataToWrite:数据数组首地址
//Len        :要写入数据的长度2,4
void AT24CXX_WriteLenByte(Uint16 WriteAddr,Uint16 DataToWrite,Uint16 Len)
{
	Uint16 t;
	for(t=0;t<Len;t++)
	{
		AT24CXX_WriteOneByte(WriteAddr+t,(DataToWrite>>(8*t))&0xff);
	}
}

//在AT24CXX里面的指定地址开始读出长度为Len的数据
//该函数用于读出16bit或者32bit的数据.
//ReadAddr   :开始读出的地址
//返回值     :数据
//Len        :要读出数据的长度2,4
Uint16 AT24CXX_ReadLenByte(Uint16 ReadAddr,Uint16 Len)
{
	Uint16 t;
	Uint16 temp=0;
	for(t=0;t<Len;t++)
	{
		temp<<=8;
		temp+=AT24CXX_ReadOneByte(ReadAddr+Len-t-1);
	}
	return temp;
}



