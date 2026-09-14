#include "Release.h"
#include "MC9S08DZ60.h"
#include "Type.h"
#include "Timer.h"

static UINT8 g_ubReleaseInitFlag=0;
extern UINT16 g_uiReleaseFlag;
extern UINT8 g_ubReleaseTime;
static UINT8 g_ubDelayComplete=0;
void Release_vInit()
{
	V1_Control=1;
	V2_Control=1;
	V3_Control=1;
	V4_Control=1;
	V5_Control=1;
	V6_Control=1;
	V7_Control=1;
	V8_Control=1;
	V9_Control=1;
	V10_Control=1;
	V11_Control=1;
	V12_Control=1;
	V13_Control=1;
	V14_Control=1;
	V15_Control=1;
	V16_Control=1;
  V1_DS =0;
  V2_DS =0;
  V3_DS =0;
  V4_DS =0;
  V5_DS =0;
  V6_DS =0;
  V7_DS =0;
  V8_DS =0;
  V9_DS =0;
  V10_DS =0;
  V11_DS =0;
  V12_DS =0;
  V13_DS =0;
  V14_DS =0;
  V15_DS =0;
  V16_DS =0;
}

void Release_vNotReleaseAll() 
{
  if(g_ubReleaseInitFlag!=1) 
  {
    g_ubReleaseInitFlag=1;
    Release_vInit();
  }
  V1_DS =0;
  V2_DS =0;
  V3_DS =0;
  V4_DS =0;
  V5_DS =0;
  V6_DS =0;
  V7_DS =0;
  V8_DS =0;
  V9_DS =0;
  V10_DS =0;
  V11_DS =0;
  V12_DS =0;
  V13_DS =0;
  V14_DS =0;
  V15_DS =0;
  V16_DS =0;
}


void Release_vDelayComplete() 
{
  g_ubDelayComplete=1;
  Timer_vSetTimer(TIMER_CHANNEL_2,g_ubReleaseTime,0);
}
void Release_vDelay()
{
   g_ubDelayComplete=0;
   Timer_vSetTimer(TIMER_CHANNEL_2,g_ubReleaseTime,
   Release_vDelayComplete);
   while(g_ubDelayComplete==0)
   {
      __RESET_WATCHDOG(); /* feeds the dog */
   }

}
void Release_vReleaseBattery() 
{
  if(g_ubReleaseInitFlag!=1) 
  {
    g_ubReleaseInitFlag=1;
    Release_vInit();
  }
  if(g_uiReleaseFlag&0x1) 
  {
    V1_DS =1;
  }
  if(g_uiReleaseFlag&0x2) 
  {
    V2_DS =1;
  }
  if(g_uiReleaseFlag&0x4) 
  {
    V3_DS =1;
  }
  if(g_uiReleaseFlag&0x8) 
  {
    V4_DS =1;
  }
  if(g_uiReleaseFlag&0x10) 
  {
    V5_DS =1;
  }
  if(g_uiReleaseFlag&0x20) 
  {
    V6_DS =1;
  }
  if(g_uiReleaseFlag&0x40) 
  {
    V7_DS =1;
  }
  if(g_uiReleaseFlag&0x80) 
  {
    V8_DS =1;
  }
  if(g_uiReleaseFlag&0x100) 
  {
    V9_DS =1;
  }
  if(g_uiReleaseFlag&0x200) 
  {
    V10_DS =1;
  }
  if(g_uiReleaseFlag&0x400) 
  {
    V11_DS =1;
  }
  if(g_uiReleaseFlag&0x800) 
  {
    V12_DS =1;
  }
  if(g_uiReleaseFlag&0x1000) 
  {
    V13_DS =1;
  }
  if(g_uiReleaseFlag&0x2000) 
  {
    V14_DS =1;
  }
  if(g_uiReleaseFlag&0x4000) 
  {
    V15_DS =1;
  }
  if(g_uiReleaseFlag&0x8000) 
  {
    V16_DS =1;
  }
}
