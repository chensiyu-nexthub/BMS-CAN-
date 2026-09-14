/*
 * iic.h
 *
 *  Created on: 2014-7-24
 *      Author: lenovo
 */

#ifndef IIC_H_
#define IIC_H_

#include "DSP28x_Project.h"

#define SDA_IN() {EALLOW;GpioCtrlRegs.GPADIR.bit.GPIO19 = 0x0;EDIS;asm(" RPT #20 || NOP");}//in
#define SDA_OUT() {EALLOW;GpioCtrlRegs.GPADIR.bit.GPIO19 = 0x1;EDIS;asm(" RPT #20 || NOP");}//out

#define IIC_SCL GpioDataRegs.GPADAT.bit.GPIO18 //
#define IIC_SDA GpioDataRegs.GPADAT.bit.GPIO19 //
#define READ_SDA  GpioDataRegs.GPADAT.bit.GPIO19  //输入SDA


//IIC所有操作函数

void IIC_Init(void);                //初始化IIC的IO口
void IIC_Start(void);				//发送IIC开始信号
void IIC_Stop(void);	  			//发送IIC停止信号
void IIC_Send_Byte(Uint16 txd);			//IIC发送一个字节
Uint16 IIC_Read_Byte(Uint16 ack);//IIC读取一个字节
Uint16 IIC_Wait_Ack(void); 				//IIC等待ACK信号
void IIC_Ack(void);					//IIC发送ACK信号
void IIC_NAck(void);				//IIC不发送ACK信号

#endif /* IIC_H_ */
