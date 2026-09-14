/*
 * GPIOinit.c
 *
 *  Created on: 2015-7-14
 *      Author: James
 */

#include "DSP28x_Project.h"
#include "SysInitDefine.h"


void delays()//25s
{
	Uint16 i;
	Uint16 temp;
	for(i=0;i<1000;i++)
	{
		for(temp=0;temp<800;temp++)
		{

		}
	}
}

void delayms()
{
	Uint16 i;
	for(i=0;i<800;i++)
	{

	}
}
void delay10us()
{
	Uint16 temp;
	for(temp=0;temp<8;temp++)
	{

	}
}


void InitTimer()
{
	InitCpuTimers();   // For this example, only initialize the Cpu Timers

	#if (CPU_FRQ_150MHZ)
	   ConfigCpuTimer(&CpuTimer0, 150, 100000);
	   //ConfigCpuTimer(&CpuTimer1, 150, 1000000);
	   //ConfigCpuTimer(&CpuTimer2, 150, 1000000);
	#endif
	#if (CPU_FRQ_100MHZ)
	   ConfigCpuTimer(&CpuTimer0, 100, 50000);//50ms
	   ConfigCpuTimer(&CpuTimer1, 100, 500000);
	   ConfigCpuTimer(&CpuTimer2, 100, 3000);
	#endif

	   CpuTimer0Regs.TCR.all = 0x4001; // Use write-only instruction to set TSS bit = 0
	   CpuTimer1Regs.TCR.all = 0x0001; // Use write-only instruction to set TSS bit = 0//关闭
	   CpuTimer2Regs.TCR.all = 0x4001; // Use write-only instruction to set TSS bit = 0
}

void InitPIE()
{
	InitPieCtrl();
	IER = 0x0000;
	IFR = 0x0000;
	InitPieVectTable();

	EALLOW;  // This is needed to write to EALLOW protected registers
	PieVectTable.TINT0 = &cpu_timer0_isr;
	PieVectTable.XINT13 = &cpu_timer1_isr;
	PieVectTable.TINT2 = &cpu_timer2_isr;
	PieVectTable.SCIRXINTB = &scibRxFifoIsr;
	PieVectTable.XINT1 = &xint1_isr;
	PieVectTable.XINT2 = &xint2_isr;
	PieVectTable.XINT3 = &xint3_isr;
	EDIS;    // This is needed to disable write to EALLOW protected registers

	IER |= M_INT1 ;	                          //TINT0 XINT1 XINT2
	IER |= M_INT7 ;	                          //Enable INT7 (7.1 DMA Ch1 7.2 DMA Ch2 dmc.c中)
	IER |= M_INT9 ;	                          //Enable INT8 (9.3 SCIRXINBT)
    IER |= M_INT12;
	IER |= M_INT13;  //Enable CPU int13,CPU-Timer 1
	IER |= M_INT14;  //Enable CPU int 14,CPU-Timer 2

// Enable TINT0 in the PIE: Group 1 interrupt 7
	PieCtrlRegs.PIECTRL.bit.ENPIE = 1;          // Enable the PIE block
	PieCtrlRegs.PIEIER1.bit.INTx7 = 1;			// Enable TINT0 in the PIE: Group 1 interrupt 7
	PieCtrlRegs.PIEIER1.bit.INTx4 = 1;			// XINT1
	PieCtrlRegs.PIEIER1.bit.INTx5 = 1;			// XINT2
	PieCtrlRegs.PIEIER12.bit.INTx1 = 1;          // Enable Xint3
	PieCtrlRegs.PIEIER9.bit.INTx3 = 1;          // PIE Group 8, INT5(8.5 SCIRXINCT)

	EnableInterrupts();
    EINT;   // Enable Global interrupt INTM
    ERTM;   // Enable Global realtime interrupt DBGM

}


void SysDataStandby()
{


}

interrupt void cpu_timer2_isr(void)
{

	CpuTimer2.InterruptCount++;
}

interrupt void xint1_isr(void)      //外接面膜键盘的设置
{

	if(KEY0 == 0)
	{
		//按下按键KEY0的操作
	}
	if(KEY1 == 0)
	{
		//按下按键KEY1的操作
	}
	if(KEY2 == 0)
	{
		//按下按键KEY2的操作
	}
	PieCtrlRegs.PIEACK.all |= PIEACK_GROUP1;
}



interrupt void xint2_isr(void)        //对RecordKey 的检测
{


	PieCtrlRegs.PIEACK.all |= PIEACK_GROUP1;
}


interrupt void xint3_isr(void)
{

	StartDMACH1();
	PieCtrlRegs.PIEACK.all |= PIEACK_GROUP12;
}


interrupt void cpu_timer0_isr(void)   //利用慢速的定时器来完成慢速的事情
{
//	float flag;
	CpuTimer0.InterruptCount++;
	if(CpuTimer0.InterruptCount%1000)  //5s
	{

	}
	else
	{

	}
	PieCtrlRegs.PIEACK.all |= PIEACK_GROUP1;

}

interrupt void cpu_timer1_isr(void)
{
	CpuTimer1.InterruptCount++;
}


//===========================================================================
// No more.
//===========================================================================
