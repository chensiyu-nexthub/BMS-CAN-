/*
 * SysInitDefine.h
 *
 *  Created on: 2015-7-15
 *      Author: James
 */

#ifndef SYSINITDEFINE_H_
#define SYSINITDEFINE_H_


#include "DSP28x_Project.h"
#include "DSP2833x_Dma_defines.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "GPIO.h"
#include "ee24cxx.h"          //定义了地址分配
#include "adc.h"

#include "IQmathLib.h"


#define RamOrFlash 1  //0_Ram 1_Flash

// These are defined by the linker (see F28335.cmd)
#if RamOrFlash  //0_Ram 1_Flash
extern Uint16 RamfuncsLoadStart;
extern Uint16 RamfuncsLoadEnd;
extern Uint16 RamfuncsRunStart;
#endif


#define ChPOS 0
#define ChNEG 1

//一秒采5M数据，一秒有5306个频率
//所以一个频率的数据量有PI2SET
//半个频率有PISET
#define ADCosc  5000000
#define FrenSet 5306
//942
#define PI2SET  (Uint16)(ADCosc/FrenSet)
//471
#define PISET   (Uint16)(PI2SET/2)

#define TESTMODE 0
#define SAMPLESTART 0


//灵敏度调节
#define HIGH 30
#define MEDIUM 50
#define LOW 100

//调零相位对准误差阈值
#define PHASETHRESHOLD 4

//调零幅值幅度控制阈值
#define MAGTHRESHOLD 5000


#define Ki       	((float)0.01)     //积分系数  浮点
#define OcsLimit	((short)2)     //LenTemp稳定震荡范围
#define CountLimit	((short)10)    //确认调整结束前需要连续记录LenTemp在稳定震荡范围内的次数


#define SAMPLESTATE 1
#define ADJUSTSTATE 2
#define IDLESTATE 3
#define AdjustTimeSet 3


/*SCI*/
void scib_fifo_init(void);
interrupt void scibRxFifoIsr(void);


/*digitalR & DDS*/
void amp_set(Uint16 multiple);
void sentdata_spi(Uint16 sdata,Uint16 cs,Uint16 channel);
Uint16 sampleupdata(Uint16* cntcurret,Uint16* cntper);
void phase_adj(Uint16* SquLen, float* IntegralValue, Uint16* sdata1,Uint16* sdata2);
void amp_adj(Uint16* amp,Uint16* AmpMin,Uint16* SdataAmplog);

void AD9833_reset(void);
void write_ad9833_d16(Uint16 x);
void ad9833_data(double frequence,unsigned int frequence_SFR,unsigned int WAVE_MODE,unsigned int Phase );


/*SysInit_xint_timer*/
void delayms();
void delay10us();
void delays();

void InitTimer();
void InitPIE();
void InitPeripherals();
void AutoAdjust();
void SysDataStandby();


interrupt void cpu_timer0_isr(void);
interrupt void cpu_timer1_isr(void);
interrupt void cpu_timer2_isr(void);
interrupt void xint1_isr(void);				//外部中断，来自面板
interrupt void xint2_isr(void);				//外部中断，来自面板
interrupt void xint3_isr(void);				//外部中断，来自FPGA,用于启动DMA


/*DMA_Xinf*/
void DMAInit(void);
void init_zone06(void);
Uint16 avgfilter_Uint(Uint16 Array[],Uint16 Input,Uint16 WinSize);
interrupt void local_DINTCH1_ISR(void);     // DMA Channel 1




#endif /* SYSINITDEFINE_H_ */


//===========================================================================
// No more.
//===========================================================================

