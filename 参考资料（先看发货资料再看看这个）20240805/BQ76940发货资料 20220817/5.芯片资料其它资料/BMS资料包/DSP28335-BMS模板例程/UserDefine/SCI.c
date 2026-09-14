/*
 * SCI.c
 *
 *  Created on: 2015-7-14
 *      Author: James
 */

#include "DSP28x_Project.h"
#include "SysInitDefine.h"


void scib_fifo_init(void)
{

	ScibRegs.SCICCR.bit.SCICHAR = 7;         //8bit
	ScibRegs.SCICCR.bit.ADDRIDLE_MODE = 0;   // 1 ADDR/0 IDLE Mode control
	ScibRegs.SCICCR.bit.LOOPBKENA = 0;       // Loop Back enable
	ScibRegs.SCICCR.bit.PARITYENA = 1;       // Parity enable
	ScibRegs.SCICCR.bit.PARITY = 1;          // 1_Even or 0_Odd Parity
	ScibRegs.SCICCR.bit.STOPBITS = 0;        // Number of Stop Bits 0_one 1_two

	ScibRegs.SCICTL1.bit.RXENA = 1;           // SCI receiver enable
	ScibRegs.SCICTL1.bit.TXENA = 1;           // SCI transmitter enable
	ScibRegs.SCICTL1.bit.SLEEP = 0;           // SCI sleep
	ScibRegs.SCICTL1.bit.TXWAKE = 0;          // Transmitter wakeup method
	ScibRegs.SCICTL1.bit.SWRESET = 0;         // Software reset,After a system reset, re-enable the SCI by writing a 1 to this bit.
	ScibRegs.SCICTL1.bit.RXERRINTENA = 0;     // Receive error interrupt

	ScibRegs.SCICTL2.bit.TXINTENA =0;
	ScibRegs.SCICTL2.bit.RXBKINTENA =1;

	/*
	//25MHz 9600_144  19200_A1
	ScibRegs.SCIHBAUD = 0x0001;
	ScibRegs.SCILBAUD = 0X0044;
	*/
	//25MHz 9600_144  19200_A1
	ScibRegs.SCIHBAUD = 0x0000;
	ScibRegs.SCILBAUD = 0X00A1;

	ScibRegs.SCIFFCT.all=0x00;
	ScibRegs.SCIFFTX.bit.SCIRST = 1;          // 15     SCI reset rx/tx channels
	ScibRegs.SCICTL1.bit.SWRESET = 1;     // Relinquish SCI from Reset

}

interrupt void scibRxFifoIsr(void)
{
	Uint16 RxData;
    while(ScibRegs.SCIRXST.bit.RXRDY != 1)
   	{}
	ScibRegs.SCICTL1.bit.RXENA = 0; // SCI receiver enable
	RxData = ScibRegs.SCIRXBUF.all;// Read data
	ScibRegs.SCITXBUF = RxData;
	while(ScibRegs.SCICTL2.bit.TXRDY!=1){}
	//Not receive other digital broad 485 information
	ScibRegs.SCICTL1.bit.RXENA = 1; // SCI receiver enable
//	LED3 = ~ LED3;//SCI工作指示灯
	PieCtrlRegs.PIEACK.all|=0x0080;       // Issue PIE ack

}

/*
interrupt void scicRxFifoIsr(void)
{
	Uint16 RxAdd,RxIns;
	Uint16 i;
	//Uint16 TxData;
    ScicRxCount++;
    //while(ScibRegs.SCIRXST.bit.RXRDY != 1)
   	//{}
	RxData = ScibRegs.SCIRXBUF.all;// Read data
	RxAdd = RxData&0xf0;//Address
	RxIns = RxData&0x0f;//Instrust

	LED3 = 0;//SCI工作指示灯

	if(CHADIFFValid == 1)//detect cu
	{
		TxDataCHA[0] = RxData;//RxData;
		TxDataCHA[1] = CrossCHAXaxisTran + 10;//交点数据
		TxDataCHA[2] = ChADIFFMaxTran;//最大变化量
		TxDataCHA[3] =  (Uint16)CrossCHAXaxisFilter1DetleSum;//交点方差
		TxDataCHA[4] =  0x00;
	}
	else
	{
		TxDataCHA[0] = RxData;//RxData;
		TxDataCHA[1] = 0x00;//0x00; other for test;
		TxDataCHA[2] = 0x00;//0x00; other for test
		TxDataCHA[3] = 0x00;
		TxDataCHA[4] = 0x00;
	}
	if(CHBDIFFValid == 1)//detect cu
	{
		TxDataCHB[0] = RxData;
		TxDataCHB[1] = CrossCHBXaxisTran + 10;//交点数据
		TxDataCHB[2] = ChBDIFFMaxTran;//最大变化量
		TxDataCHB[3] =  (Uint16)CrossCHBXaxisFilter1DetleSum;//交点方差;
		TxDataCHB[4] =  0x00;
	}
	else
	{
		TxDataCHB[0] = RxData;
		TxDataCHB[1] = 0x00;//0x00; other for test
		TxDataCHB[2] = 0x00;//0x00; other for test
		TxDataCHB[3] = 0x00;
		TxDataCHB[4] = 0x00;
	}

	if(RxAdd == SlaveAddressChA)
	{
		//close all timer interrupt and delay
		CpuTimer2Regs.TCR.bit.TIE = 0;
		TxDataEven = 1;
		switch(RxIns)
		{
		case 0x0f:
			{
				GpioDataRegs.GPBDAT.bit.GPIO61 = 1;//RS485Control 1_TX 0_RX
				asm(" RPT #5 || NOP");
				for(i=0;i<5;i++)
				{
					ScibRegs.SCITXBUF = TxDataCHA[i];
					while(ScibRegs.SCICTL2.bit.TXRDY!=1){}
				}

			}break;
		default:
			{

			}break;
		}
		//open all timer interrupt
		CpuTimer2Regs.TCR.bit.TIE = 1;
	}
	else if(RxAdd == SlaveAddressChB)
	{
		//close all timer interrupt and delay
		CpuTimer2Regs.TCR.bit.TIE = 0;
		TxDataEven = 1;
		switch(RxIns)
		{
		case 0x0f:
			{
				GpioDataRegs.GPBDAT.bit.GPIO61 = 1;//RS485Control 1_TX 0_RX
				asm(" RPT #5 || NOP");
				for(i=0;i<5;i++)
				{
					ScibRegs.SCITXBUF = TxDataCHB[i];
					while(ScibRegs.SCICTL2.bit.TXRDY!=1){}
				}
				if(!TESTMODE)//进入测试模式，低为测试模式
				{

					if(!WriteDone)//写入ROM
					{
						DELAY_US(5000);//延时，弱化影响
						Rec1LocaRom = Rec1LocAvg;
						Rec2LocaRom = Rec2LocAvg;
						Coi1LocaRom = Coi1LocAvg;
						Coi2LocaRom = Coi2LocAvg;
						AT24CXX_WriteLenByte(0x00,Rec1LocAvg,2);
						AT24CXX_WriteLenByte(0x02,Coi1LocAvg,2);
						AT24CXX_WriteLenByte(0x04,Rec2LocAvg,2);
						AT24CXX_WriteLenByte(0x06,Coi2LocAvg,2);
						WriteDone = 1;//标志位复位
					}
				}
				else
				{
					WriteDone = 0;
				}
			}break;
		default:
			{}break;
		}
		//open all timer interrupt
		CpuTimer2Regs.TCR.bit.TIE = 1;
	}
	else if(RxAdd == BroadcastAddress)
	{
		switch(RxIns)
		{
		case 0x01:
			{
				YaxisThreSet = YaxisThre[0];
			}break;
		case 0x02:
			{
				YaxisThreSet = YaxisThre[1];
			}break;
		case 0x03:
			{
				YaxisThreSet = YaxisThre[2];
			}break;
		case 0x0A:
			{
				WorkState = 2;
			}break;
		default:
			{
				YaxisThreSet = YaxisThre[0];
			}break;
		}
	}

	TxDataEven = 0;

	//Not receive other digital broad 485 information
	ScibRegs.SCICTL1.bit.RXENA = 0; // SCI receiver enable
	//GpioDataRegs.GPBDAT.bit.GPIO61 = 1;//RS485Control 1_TX 0_RX
	RxEnableIndex = 0;
	//asm("  NOP");
	DELAY_US(4000L);//8ms
	ScibRegs.SCICTL1.bit.RXENA = 1; // SCI receiver enable
	GpioDataRegs.GPBDAT.bit.GPIO61 = 0;//RS485Control 1_TX 0_RX
	LED3 = 1;//SCI工作指示灯
	PieCtrlRegs.PIEACK.all|=0x0080;       // Issue PIE ack

}
*/






//===========================================================================
// No more.
//===========================================================================
