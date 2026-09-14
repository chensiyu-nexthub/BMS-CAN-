/*
 * iic.c
 *
 *  Created on: 2014-7-24
 *      Author: lenovo
 */

#include "iic.h"
#include "DSP28x_Project.h"



void IIC_Init(void)
{
    EALLOW;
	GpioCtrlRegs.GPAMUX2.bit.GPIO18 = 0x0;  // SCL
	GpioCtrlRegs.GPAMUX2.bit.GPIO19 = 0x0;  // SDA

	GpioCtrlRegs.GPADIR.bit.GPIO18 = 0x1;//SCL output
	GpioCtrlRegs.GPADIR.bit.GPIO19 = 0x1;//SDA output

	GpioCtrlRegs.GPAPUD.bit.GPIO18 = 0;   // Enable pull-up
	GpioCtrlRegs.GPAPUD.bit.GPIO19 = 0;   // Enable pull-up
    EDIS;

    GpioDataRegs.GPADAT.bit.GPIO18 = 1;
    asm(" RPT #5 || NOP");
    GpioDataRegs.GPADAT.bit.GPIO19 = 1;
}

//产生IIC起始信号
void IIC_Start(void)
{
	SDA_OUT();     //sda线输出
	IIC_SDA = 1;
	asm(" RPT #5 || NOP");
	IIC_SCL=1;
	DELAY_US(1);
 	IIC_SDA=0;//START:when CLK is high,DATA change form high to low
 	DELAY_US(1);
	IIC_SCL=0;//钳住I2C总线，准备发送或接收数据
	DELAY_US(1);
}

//产生IIC停止信号
void IIC_Stop(void)
{
	SDA_OUT();//sda线输出
	IIC_SCL=0;
	asm(" RPT #5 || NOP");
	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	DELAY_US(1);
	IIC_SCL=1;
	DELAY_US(1);
	IIC_SDA=1;//发送I2C总线结束信号
	DELAY_US(1);
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
Uint16 IIC_Wait_Ack(void)
{
	Uint16 ucErrTime=0;

	//为什么加这两个呢？
	SDA_OUT();      //SDA设置为输出
	IIC_SDA=1;DELAY_US(1);

	SDA_IN();      //SDA设置为输入
	IIC_SCL=1;DELAY_US(1);
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			//asm ("      ESTOP0");
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL=0;//时钟输出0
	SDA_OUT();      //SDA设置为输出
	return 0;
}
//产生ACK应答
void IIC_Ack(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=0;
	DELAY_US(1);
	IIC_SCL=1;
	DELAY_US(1);
	IIC_SCL=0;
	DELAY_US(1);
	IIC_SDA=1;
	DELAY_US(1);
}
//不产生ACK应答
void IIC_NAck(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=1;
	DELAY_US(1);
	IIC_SCL=1;
	DELAY_US(1);
	IIC_SCL=0;
}
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答
void IIC_Send_Byte(Uint16 txd)
{
    Uint16 t;
	SDA_OUT();
    IIC_SCL=0;//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {
        //IIC_SDA=(txd&0x80)>>7;
		if((txd&0x80)>>7)
			IIC_SDA=1;
		else
			IIC_SDA=0;
		txd<<=1;
		DELAY_US(1);   //对TEA5767这三个延时都是必须的
		IIC_SCL=1;
		DELAY_US(1);
		IIC_SCL=0;
		DELAY_US(1);
    }
}
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK
Uint16 IIC_Read_Byte(Uint16 ack)
{
	Uint16 i,receive=0;
	SDA_IN();//SDA设置为输入
    for(i=0;i<8;i++ )
	{
        IIC_SCL=0;
        DELAY_US(1);
		IIC_SCL=1;
        receive<<=1;
        if(READ_SDA)receive++;
		DELAY_US(1);
    }
    if (!ack)
        IIC_NAck();//发送nACK
    else
        IIC_Ack(); //发送ACK
    return receive;
}
