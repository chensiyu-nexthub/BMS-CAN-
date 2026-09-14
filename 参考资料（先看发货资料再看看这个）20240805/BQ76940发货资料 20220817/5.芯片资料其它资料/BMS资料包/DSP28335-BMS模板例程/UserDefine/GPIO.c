/*
 * GPIO.c
 *
 *  Created on: 2015-7-15
 *  Author: James
 */

#include "DSP28x_Project.h"
#include "SysInitDefine.h"



//GPIO初始化
void Gpioinit(void)
{
/*for output*/
EALLOW;
//for led
    GpioCtrlRegs.GPBMUX2.bit.GPIO50 = 0; 	//LED0
    LED0	= 1;
    GpioCtrlRegs.GPBDIR.bit.GPIO50 = 1;

    GpioCtrlRegs.GPBMUX2.bit.GPIO51 = 0;	//LED1
    LED1	= 1;
    GpioCtrlRegs.GPBDIR.bit.GPIO51 = 1;

    GpioCtrlRegs.GPBMUX2.bit.GPIO52 = 0;	//LED2
    LED2	= 1;
    GpioCtrlRegs.GPBDIR.bit.GPIO52 = 1;

    GpioCtrlRegs.GPBMUX2.bit.GPIO53 = 0;	//LED3
    LED3	= 1;
    GpioCtrlRegs.GPBDIR.bit.GPIO53 = 1;

    GpioCtrlRegs.GPBMUX2.bit.GPIO57 = 0; 	//LED4
    LED4	= 1;
    GpioCtrlRegs.GPBDIR.bit.GPIO57 = 1;


//for external LED
    GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 0;	//LED0_OUT_DSP
    LED0_OUT_DSP	=	1;
    GpioCtrlRegs.GPADIR.bit.GPIO0 = 1;

    GpioCtrlRegs.GPAMUX1.bit.GPIO1 = 0;	//LED1_OUT_DSP
    LED1_OUT_DSP	=	1;
    GpioCtrlRegs.GPADIR.bit.GPIO1 = 1;

    GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 0;	//LED2_OUT_DSP
    LED2_OUT_DSP	=	1;
    GpioCtrlRegs.GPADIR.bit.GPIO2 = 1;

//for oled或者是模拟版上的部分信号的控制
    GpioCtrlRegs.GPAMUX1.bit.GPIO4 = 0;	//REN==OLED_CS
    CS_OLED = 1;
    GpioCtrlRegs.GPADIR.bit.GPIO4 = 1;

    GpioCtrlRegs.GPAMUX1.bit.GPIO5 = 0;	//REN==OLED_CS
    RES = 1;
    GpioCtrlRegs.GPADIR.bit.GPIO5 = 1;

    GpioCtrlRegs.GPAMUX1.bit.GPIO7 = 0;	//SOMI/DC
    DC 	= 1;
    GpioCtrlRegs.GPADIR.bit.GPIO7 = 1;


    GpioCtrlRegs.GPAMUX2.bit.GPIO16 = 0;	//SDIN
    SDIN 	= 1;
    GpioCtrlRegs.GPADIR.bit.GPIO16 = 1;


    GpioCtrlRegs.GPAMUX1.bit.GPIO12 = 0;	//SCLKO
    SCLKO 	= 1;
    GpioCtrlRegs.GPADIR.bit.GPIO12 = 1;


    GpioCtrlRegs.GPBMUX2.bit.GPIO54 = 0;	//SIMO
    SDI 	= 1;
    GpioCtrlRegs.GPBDIR.bit.GPIO54 = 1;

    GpioCtrlRegs.GPBMUX2.bit.GPIO56 = 0;	//SCLK
    SCLK 	= 1;
    GpioCtrlRegs.GPBDIR.bit.GPIO56 = 1;

//for Control digital potentiometer  模拟板器件的使能脚

    GpioCtrlRegs.GPBMUX2.bit.GPIO59  = 0;		//CS_R1
    CS_Phase  		= 1;
    GpioCtrlRegs.GPBDIR.bit.GPIO59  = 1;

    GpioCtrlRegs.GPBMUX2.bit.GPIO58  = 0;		//CS_R2
    CS_AD9833  		= 1;
    GpioCtrlRegs.GPBDIR.bit.GPIO58  = 1;

    GpioCtrlRegs.GPBMUX2.bit.GPIO60 = 0;	//CS_R3
    CS_Amp   		= 1;
    GpioCtrlRegs.GPBDIR.bit.GPIO60 = 1;

    GpioCtrlRegs.GPAMUX2.bit.GPIO21 = 0;	//NP_Select1
    NP_Select	= 1;
    GpioCtrlRegs.GPADIR.bit.GPIO21 = 1;

    GpioCtrlRegs.GPAMUX2.bit.GPIO20 = 0;	//Scale_A
    Scale_A  	= 1;
    GpioCtrlRegs.GPADIR.bit.GPIO20 = 1;

    GpioCtrlRegs.GPAMUX2.bit.GPIO24 = 0;	//Scale_B
    Scale_B		= 1;
    GpioCtrlRegs.GPADIR.bit.GPIO24 = 1;


EDIS;


/*connect with xhold for xintf control*/
EALLOW;
//for FPGA
	GpioCtrlRegs.GPBMUX2.bit.GPIO63 = 0;		//INT_GPIO3
	GpioCtrlRegs.GPBDIR.bit.GPIO63 = 1;

	//GPIO14 GPIO15 Setting XHOLD AND XHOLDA
	GpioCtrlRegs.GPAMUX1.bit.GPIO14 = 1;         // TZ3(I)/XHOLD(I)
	GpioCtrlRegs.GPADIR.bit.GPIO14 = 0;          // input
	GpioCtrlRegs.GPAQSEL1.bit.GPIO14 = 0;        // Xint1 Synch to SYSCLKOUT only

	GpioCtrlRegs.GPAMUX1.bit.GPIO15 = 1;         // TZ4(I)/XHOLDA(O)
	GpioCtrlRegs.GPADIR.bit.GPIO15 = 1;          // output
EDIS;

/*--------GPIO2 GPIO11 Setting Input Interrupt from Fpga-------------*/
EALLOW;
    GpioCtrlRegs.GPBMUX2.bit.GPIO61 = 0;		//INT_GPIO0
    GpioCtrlRegs.GPBDIR.bit.GPIO61 = 0; 		// input
    GpioCtrlRegs.GPBQSEL2.bit.GPIO61 = 2;   //

//	GpioCtrlRegs.GPACTRL.bit.QUALPRD1 = 0xFF;   // Each sampling window is 510*SYSCLKOUT

	GpioIntRegs.GPIOXINT3SEL.bit.GPIOSEL = 29;   // Xint1 is GPIO1  INT_GPIO0

// Configure XINT1 and XINT2
	XIntruptRegs.XINT3CR.bit.POLARITY = 1;      // rising edge interrupt

// Enable XINT1 and XINT2
	XIntruptRegs.XINT3CR.bit.ENABLE = 1;        // Enable XINT1
EDIS;

/*---------------------------For SCI-------------------------------------*/
EALLOW;

	GpioCtrlRegs.GPAPUD.bit.GPIO23 = 0;    // Enable pull-up for GPIO62 (SCIRXDC)
	GpioCtrlRegs.GPAPUD.bit.GPIO22 = 0;	   // Enable pull-up for GPIO63 (SCITXDC)

/* Set qualification for selected pins to asynch only */
	GpioCtrlRegs.GPAQSEL2.bit.GPIO23 = 3;  // Asynch input GPIO62 (SCIRXDC)

/* Configure SCI-C pins using GPIO regs*/
	GpioCtrlRegs.GPAMUX2.bit.GPIO23 = 1;   // Configure GPIO62 for SCIRXDC operation
	GpioCtrlRegs.GPAMUX2.bit.GPIO22 = 1;   // Configure GPIO63 for SCITXDC operation


EDIS;

	IOlogicInit2FPGA();
	KeyInit();

}



void KeyInit(void)
{
	EALLOW;

//AlarmKey   是电平信号
		GpioCtrlRegs.GPAMUX2.bit.GPIO25 = 0;  // AlarmKey
		GpioCtrlRegs.GPADIR.bit.GPIO25  = 0;   //input
		GpioCtrlRegs.GPAPUD.bit.GPIO25  = 1;	//上拉

	EDIS;


/*for panel  input*/
	EALLOW;

//RecordKey   检测上升沿和下降沿
		GpioCtrlRegs.GPAMUX2.bit.GPIO26 = 0;  // RecordKey
		GpioCtrlRegs.GPADIR.bit.GPIO26  = 0;   //input
		GpioCtrlRegs.GPAPUD.bit.GPIO26  = 0;	//内部上拉
	    GpioCtrlRegs.GPAQSEL2.bit.GPIO26 = 2;   //

		GpioIntRegs.GPIOXINT2SEL.bit.GPIOSEL = 26;    // Xint1 is GPIO26
		XIntruptRegs.XINT2CR.bit.POLARITY = 3;      // falling and rising  edge interrupt
		XIntruptRegs.XINT2CR.bit.ENABLE   = 1;        // Enable XINT2


//按键  设定为外部中断
		GpioCtrlRegs.GPAMUX1.bit.GPIO1 	= 0;  // KEY0
	    GpioCtrlRegs.GPAQSEL1.bit.GPIO1 = 2;   //
		GpioCtrlRegs.GPADIR.bit.GPIO1	= 0;
		GpioCtrlRegs.GPAPUD.bit.GPIO1	= 1;

		GpioCtrlRegs.GPAMUX1.bit.GPIO6  = 0;  // KEY1
	    GpioCtrlRegs.GPAQSEL1.bit.GPIO6 = 2;   //
	   	GpioCtrlRegs.GPADIR.bit.GPIO6	= 0;
	   	GpioCtrlRegs.GPAPUD.bit.GPIO6	= 1;

		GpioCtrlRegs.GPAMUX1.bit.GPIO13  = 0;  // KEY2
	    GpioCtrlRegs.GPAQSEL1.bit.GPIO13 = 2;   //
		GpioCtrlRegs.GPADIR.bit.GPIO13	 = 0;
		GpioCtrlRegs.GPAPUD.bit.GPIO13	 = 1;


		GpioIntRegs.GPIOXINT1SEL.bit.GPIOSEL = 1;    // Xint1 is GPIO1
		GpioIntRegs.GPIOXINT1SEL.bit.GPIOSEL = 6;    // Xint1 is GPIO6
		GpioIntRegs.GPIOXINT1SEL.bit.GPIOSEL = 13;   // Xint1 is GPIO13

		GpioCtrlRegs.GPACTRL.bit.QUALPRD0 = 0xFF;   // Each sampling window is 510*SYSCLKOUT
		XIntruptRegs.XINT1CR.bit.POLARITY = 0;      // falling edge interrupt
		XIntruptRegs.XINT1CR.bit.ENABLE   = 1;        // Enable XINT1

	EDIS;
}


void IOlogicInit2FPGA(void)
{

	EALLOW;

	GpioCtrlRegs.GPCMUX2.bit.GPIO87 = 0;  // XA15
	PowerH0L1	= 0;
    GpioCtrlRegs.GPCDIR.bit.GPIO87  = 1;   //output


	GpioCtrlRegs.GPBMUX1.bit.GPIO39 = 0;  // XA16   开机先显示红灯报警
	AlarmY1N0	= 0;    //先显示红灯报警
    GpioCtrlRegs.GPBDIR.bit.GPIO39  = 1;   //output


	GpioCtrlRegs.GPAMUX2.bit.GPIO31 = 0;  // XA17
	SpeakerOn0Off1	= 1;
    GpioCtrlRegs.GPADIR.bit.GPIO31  = 1;   //output


	GpioCtrlRegs.GPAMUX2.bit.GPIO30 = 0;  // XA18
	MotorOn0Off1	= 1;
    GpioCtrlRegs.GPADIR.bit.GPIO30  = 1;   //output

	EDIS;
}


//Comtype 的定义
//[8]   电量    [4]  电量情况
//[7]	报警    [3]  报警情况
//[6]   提示    [2]  电机或喇叭
void IOlogic2FPGA(Uint16 Comtype)
{
	switch(Comtype&0xf0)
	{
		case 0x80:
			if(Comtype & 0x08)
				PowerH0L1 = 1;	  	//电量低
			else
				PowerH0L1 = 0;	  	//电量高
			break;
		case 0x40:
			if(Comtype & 0x04)
				AlarmY1N0 = 1;		//报警
			else
				AlarmY1N0 = 0;    	//不报警

			if(AlarmKey)
				SpeakerOn0Off1 = !AlarmY1N0;	  	//喇叭
			else
				MotorOn0Off1 = !AlarmY1N0;	  	//电机

			break;
		case 0x20:
				AlarmY1N0 = !AlarmY1N0;
			break;
		case 0x10:

			break;

	}
}



