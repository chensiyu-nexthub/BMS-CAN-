/*
 * CAN_Task.c
 *
 *  Created on: 2010-8-14
 *      Author: Administrator
 */
#include "CAN.h"
#include "Timer.h"

#ifndef __DZ60
#define __DZ60
#endif
#include "Type.h"
UINT8 g_ubDAQTime=1;

void CAN_vChangeSendID(UINT16 uiSendID)
{

}



void BMS_vCANSend(UINT16 uiID,UINT8 *pubBuffer)
{

#ifdef __DZ60
  CAN_vSend(uiID,pubBuffer);
#endif
}
void BMS_vCANDAQ() 
{
    CCP_vDAQTransmit();
    Timer_vSetTimer(TIMER_CHANNEL_1,
    g_ubDAQTime,BMS_vCANDAQ);
}