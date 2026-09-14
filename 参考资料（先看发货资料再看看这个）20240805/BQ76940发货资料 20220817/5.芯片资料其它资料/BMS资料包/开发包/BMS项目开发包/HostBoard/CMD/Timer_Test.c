/*
 * Timer_Test.c
 *
 *  Created on: May 7, 2010
 *      Author: Administrator
 */
#include "Timer_Test.h"

#include "XC22xxMREGS.h"

void Timer_Test_Interrupt()//wave
{
	P2_IOCR06=0x80;
	P2_OUT_P6=~P2_OUT_P6;
}
