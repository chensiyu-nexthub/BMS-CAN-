#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */

#include "Timer.h"
#include <mc9s08dz60.h>
#include "system.h"
#include "wave.h"

#define SAMPLE_RELEASE_BATTERY_TIMER 0
extern unsigned char g_sampleShareDataBuffer[64];

unsigned char f_batteryRelease=0;

unsigned char g_timerCounter[8]={0,0,0,0,0,0,0,0};
unsigned char g_timerReCounter[8]={0,0,0,0,0,0,0,0};
TimeProcess g_timeProcess[8]={
0,0,0,0,0,0,0,0};
void Timer_Init() 
{
  RTCMOD=0;
  RTCSC=0x1f;  
}

interrupt VectorNumber_Vrtc void  Timer_IntteruptProcess() 
{
  static char batteryReleaseTime;
  unsigned char i;
  RTCSC|=0x80;
  if(batteryReleaseTime>g_sampleShareDataBuffer[SAMPLE_RELEASE_BATTERY_TIMER])
   f_batteryRelease=1;
  
  for(i=0;i<8;i++) 
  {
    if(g_timerCounter[i]!=0)
      g_timerCounter[i]--;
    else 
    {
      g_timerCounter[i]=g_timerReCounter[i];
      if(g_timeProcess[i]!=0)
        g_timeProcess[i]();
    }
  }
  
} 

void Timer_SetTimer(unsigned char channel,unsigned char input,TimeProcess process) 
{
  g_timerCounter[channel]=input; 
  g_timerReCounter[channel]=input;
  g_timeProcess[channel]=process; 
}

unsigned char Timer_IsActive(unsigned char channel) 
{
   if(g_timerCounter[channel]==0)
    return 1;
   else 
    return 0;
}