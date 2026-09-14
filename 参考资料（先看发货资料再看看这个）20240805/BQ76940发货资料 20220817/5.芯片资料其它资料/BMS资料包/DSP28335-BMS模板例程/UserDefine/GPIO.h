/*
 * GPIO.h
 *
 *  Created on: 2015-7-15
 *      Author: James
 */

#ifndef GPIO_H_
#define GPIO_H_


void Gpioinit(void);
void IOlogicInit2FPGA(void);
void IOlogic2FPGA(Uint16 Comtype);
void KeyInit(void);


//Comtype 的定义
//[8]   电量    [4]  电量情况
//[7]	报警    [3]  报警情况
//[6]   记录    [2]  保留
#define FullPower		0x80
#define Lowpower		0x88
#define disAlarm		0x40
#define Alarm			0x44
#define RecordLED		0x20

#define PowerH0L1			GpioDataRegs.GPCDAT.bit.GPIO87
#define AlarmY1N0			GpioDataRegs.GPBDAT.bit.GPIO39
#define SpeakerOn0Off1		GpioDataRegs.GPADAT.bit.GPIO31
#define MotorOn0Off1		GpioDataRegs.GPADAT.bit.GPIO30


#define RecordKey	GpioDataRegs.GPADAT.bit.GPIO26
#define AlarmKey	GpioDataRegs.GPADAT.bit.GPIO25



/*一些宏定义声明
 * beginning 宏定义
 * 宏定义*/
//指示灯，置0灯亮起
#define LED0 GpioDataRegs.GPBDAT.bit.GPIO50    //工作指示灯，常闪烁
#define LED1 GpioDataRegs.GPBDAT.bit.GPIO51    //调零指示灯，调零过程亮起
#define LED2 GpioDataRegs.GPBDAT.bit.GPIO52    //幅值判断指示灯，超过设定幅值亮起
#define LED3 GpioDataRegs.GPBDAT.bit.GPIO53    //SCI 485通讯指示灯
#define LED4 GpioDataRegs.GPBDAT.bit.GPIO57    //记录模式下亮起


//指示灯，置1 TOGGLE
#define LED0TOGGLE GpioDataRegs.GPBTOGGLE.bit.GPIO50
#define LED1TOGGLE GpioDataRegs.GPBTOGGLE.bit.GPIO51
#define LED2TOGGLE GpioDataRegs.GPBTOGGLE.bit.GPIO52
#define LED3TOGGLE GpioDataRegs.GPBTOGGLE.bit.GPIO53
#define LED4TOGGLE GpioDataRegs.GPBTOGGLE.bit.GPIO57

//外接LED
#define LED0_OUT_DSP GpioDataRegs.GPADAT.bit.GPIO0		//LED0_OUT_DSP
#define LED1_OUT_DSP GpioDataRegs.GPADAT.bit.GPIO1
#define LED2_OUT_DSP GpioDataRegs.GPADAT.bit.GPIO2


//与SPI相关的使能脚   和   spi脚
#define CS_AD9833	GpioDataRegs.GPBDAT.bit.GPIO58		//CS_R2
#define CS_Phase	GpioDataRegs.GPBDAT.bit.GPIO59		//CS_R1
#define CS_Amp		GpioDataRegs.GPBDAT.bit.GPIO60		//CS_R3
#define NP_Select	GpioDataRegs.GPADAT.bit.GPIO21		//对消波形正反向调整
#define Scale_A		GpioDataRegs.GPADAT.bit.GPIO20		//模拟开关选择放大倍数
#define Scale_B		GpioDataRegs.GPADAT.bit.GPIO24		//模拟开关选择放大倍数

#define CS_OLED		GpioDataRegs.GPADAT.bit.GPIO4		//CS_OLED
#define DC			GpioDataRegs.GPADAT.bit.GPIO7
#define RES 		GpioDataRegs.GPADAT.bit.GPIO5
#define SDIN		GpioDataRegs.GPADAT.bit.GPIO16
#define SCLKO 		GpioDataRegs.GPADAT.bit.GPIO12

#define SCLK		GpioDataRegs.GPBDAT.bit.GPIO56
#define SDI			GpioDataRegs.GPBDAT.bit.GPIO54


//FPGA软件复位
#define RSTFPGA 	GpioDataRegs.GPBDAT.bit.GPIO61

//按键
#define KEY0 GpioDataRegs.GPADAT.bit.GPIO1
#define KEY1 GpioDataRegs.GPADAT.bit.GPIO6
#define KEY2 GpioDataRegs.GPADAT.bit.GPIO13
/*宏定义
 * 结束
 * 结束*/






#endif /* GPIO_H_ */
