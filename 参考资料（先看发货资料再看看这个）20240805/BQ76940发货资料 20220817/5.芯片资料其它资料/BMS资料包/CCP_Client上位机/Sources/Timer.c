#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */

#include "Timer.h"
#include <mc9s08dz60.h>

UINT8 g_pubTimerCounter[8]={0,0,0,0,0,0,0,0};
UINT8 g_pubTimerReCounter[8]={0,0,0,0,0,0,0,0};

PFUTimeProcess g_ppfuTimeProcess[8]={
0,0,0,0,0,0,0,0};
void Timer_vInit() 
{
  RTCMOD=0;
  RTCSC=0x1f;  
}

interrupt VectorNumber_Vrtc void  Timer_vIntteruptProcess() 
{
  UINT8 i;
  RTCSC|=0x80;
  
  for(i=0;i<8;i++) 
  {
    if(g_pubTimerCounter[i]!=0)
      g_pubTimerCounter[i]--;
    else 
    {
      g_pubTimerCounter[i]=g_pubTimerReCounter[i];
      if(g_ppfuTimeProcess[i]!=0)
        g_ppfuTimeProcess[i]();
    }
  }
  
} 

void Timer_vSetTimer(UINT8 channel,UINT8 input,PFUTimeProcess process) 
{
  g_pubTimerCounter[channel]=input; 
  g_pubTimerReCounter[channel]=input;
  g_ppfuTimeProcess[channel]=process; 
}

UINT8 Timer_ubIsActive(UINT8 channel) 
{
   if(g_pubTimerCounter[channel]==0)
    return 1;
   else 
    return 0;
}