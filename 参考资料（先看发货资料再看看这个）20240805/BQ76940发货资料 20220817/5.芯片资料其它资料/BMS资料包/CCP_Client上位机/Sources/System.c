#include "system.h"
#include "MC9S08DZ60.h"
#include "Capacitance.h"
#include "ADC.h"
#include "HC4067.h"
#include "SPI.h"
#include "CAN.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "Timer.h"
#include "CAN_Task.h"
#include "Release.h"
#include "Tempture.h"

extern UINT8 g_ubHC4067Channel;

UINT8 g_ubSampleEnable=0;
UINT16 g_puiBatteryVoltage[16];

INT16 g_piBatteryTempture[8];

INT16 g_iMaxTempture,g_iMinTempture;

UINT8 g_ubMaxTemptureIndex,g_ubMinTemptureIndex;

UINT16 g_uiReleaseFlag=0;

UINT8 g_ubReleaseTime=60;

UINT16 g_uiBatteryMask=0xffff;

UINT8 g_ubTemptureMask=0xff;

UINT16 g_uiMaxVoltage,g_uiMinVoltage;

UINT8  g_ubMaxVoltageIndex,g_ubMinVoltageIndex;

UINT8 g_ubFilterDataFlag=0;

UINT32 g_ulTotalVoltage=0;

UINT16 g_uiAverageVoltage=0;

void System_vInit()//It cann't be interruptted.  
{
  System_vChangeFrequency();
  CAN_vInit();
  HC4067_vInit();
  Capacitance_vInit();
  ADC_vInit();
  Timer_vInit();

}
void System_vTotalAverageVoltage() 
{
  UINT8 i;
  UINT16 uiMask;
  UINT8 ubCounter;
    
  DisableInterrupts;
  g_ulTotalVoltage=0;
  ubCounter=0;
  
  for(i=0;i<16;i++) 
  {
    uiMask=1;
    uiMask<<=i;
     
    if(g_uiBatteryMask&uiMask) 
    {
      ubCounter++;
      g_ulTotalVoltage+=g_puiBatteryVoltage[i];
    }
  }
  if(ubCounter>0) 
  {
    g_uiAverageVoltage=g_ulTotalVoltage/ ubCounter;
  }
  EnableInterrupts;  
}
void System_vSortVoltage() 
{
  UINT8 i;
  UINT8 ubFirstBattery=0;
  UINT16 uiMask;  
  DisableInterrupts;
  for(i=0;i<16;i++) 
  {
    uiMask=1;
    uiMask<<=i;
    
    if(g_uiBatteryMask&uiMask) 
    {
      if(ubFirstBattery==0) 
      {
        ubFirstBattery=1;
        g_uiMaxVoltage=g_puiBatteryVoltage[i];
        g_uiMinVoltage=g_puiBatteryVoltage[i];
        
        g_ubMaxVoltageIndex=i;
        g_ubMinVoltageIndex=i; 
      } 
      else 
      {
        if(g_uiMaxVoltage<g_puiBatteryVoltage[i]) 
        {
          g_ubMaxVoltageIndex=i;
          g_uiMaxVoltage= g_puiBatteryVoltage[i];
        }
        if(g_uiMinVoltage>g_puiBatteryVoltage[i]) 
        {
          g_ubMinVoltageIndex=i;
          g_uiMinVoltage= g_puiBatteryVoltage[i];
        }
      }
    }
  }
  EnableInterrupts;
}

void System_vSortTempture() 
{
  UINT8 i;
  UINT8 ubFirstTempture=0;
  UINT8 ubMask; 
    
  DisableInterrupts;

   
  for(i=0;i<8;i++) 
  {
    ubMask=1;
    ubMask<<=i;
    if(ubMask&g_ubTemptureMask) 
    {
      if(ubFirstTempture==0) 
      {
        ubFirstTempture=1;
        g_iMaxTempture=g_piBatteryTempture[i];
        g_iMinTempture=g_piBatteryTempture[i];
        g_ubMaxTemptureIndex=i;
        g_ubMinTemptureIndex=i;
      } 
      else 
      {  
        if(g_iMaxTempture<g_piBatteryTempture[i]) 
        {
          g_ubMaxTemptureIndex=i;
          g_iMaxTempture= g_piBatteryTempture[i];
        }
        if(g_iMinTempture>g_piBatteryTempture[i]) 
        {
          g_ubMinTemptureIndex=i;
          g_iMinTempture= g_piBatteryTempture[i];
        }
      }
    }
  }
 EnableInterrupts;
}


void System_vGetVoltage(UINT8 index)
{
  UINT16 ADCData;
  g_ubHC4067Channel=index;
  Capacitance_vStartCharge(index);
  ADCData=Capacitance_uiGetVoltage();

if(g_ubFilterDataFlag==1)
  g_puiBatteryVoltage[index]=Math_FilterData(index,ADCData);
else
  g_puiBatteryVoltage[index]=ADCData;//Math_FilterData(index,ADCData);

}
void System_vGetVoltageRelease(UINT8 index)
{
  Capacitance_vStartCharge(index);
  g_puiBatteryVoltage[index]=Capacitance_uiGetVoltage();
  Capacitance_vRelease();
}

void System_vChangeFrequency() 
{

    SOPT1_COPT = 0;         /* Watchdog disabled */

    MCGC2_BDIV = BUS_DIV;   /* Bus divider */        
    MCGC2_RANGE = 1;        /* High frequency range selected */
    MCGC2_HGO = 1;          /* High gain for external oscillator */
    MCGC2_EREFS = 1;        /* External reference is crystal oscillator */ 
    MCGC2_ERCLKEN = 1;      /* External reference enabled */
    
        while(!MCGSC_OSCINIT);  /* Wait until the initialization cycles
                                   of the external oscillator are complete */
                                 
    MCGC1_CLKS = 0x02;      /* External reference is system clock source */
    MCGC1_RDIV = 0x07;      /* Configure Reference Divider to div by 128 */      
    MCGC1_IREFS = 0;        /* External reference clock selected */
                                      
        while(MCGSC_IREFST);  /* Wait until the external reference is 
                                 the source for the reference clock */   
                                 
        while((MCGSC_CLKST)^(0x02));  /* Wait until the external reference 
                                         is directed to MCGOUT */    
                                        
    MCGC2_LP = 1;           /* BLPE mode entered */  
                

    MCGC1_RDIV = REF_DIV;   /* Configure the Reference Divider */
    MCGC3_PLLS = 1;         /* Enable PLL to use it in PBE mode */
    MCGC3_VDIV = VCO_DIV;   /* Multiplication factor applied to the reference */
    
        while(!MCGSC_PLLST);    /* Wait until the PLL is the 
                                   source for the reference clock */   
                                   
    MCGC2_LP = 0;   /* Switch to PBE mode before going to PEE mode */          
    
        while(!MCGSC_LOCK);    /* Wait until the PLL locks */     
        
    MCGC1_CLKS = 0x00;      /* PLL selected as system clock source */  
    
        while((MCGSC_CLKST)^(0x03));  /* Wait until the PLL is directed to MCGOUT */                                                                    

}


void System_vDelay(UINT16 max) 
{
  UINT16 i;
  for(i=0;i<max;i++)
  ;
}


void System_vMainProcess() 
{
  UINT8 i;
  UINT16 uiMask=1;
  
  Release_vNotReleaseAll();
  System_vGetVoltage(0);
  BMS_vCANDAQ();
  while(1) 
  {  
    for(i=0;i<16;i++)//Start get the voltage. 
    {
      uiMask=1;
      uiMask<<=i;
      DisableInterrupts;
      g_ubHC4067Channel=i;
      if(g_uiBatteryMask&uiMask)
        System_vGetVoltage(i);
      __RESET_WATCHDOG(); 
      EnableInterrupts;
    }
    
    Tempture_vGetTempture();
    System_vSortTempture();
    System_vSortVoltage();
    System_vTotalAverageVoltage();
    if(g_uiReleaseFlag!=0) 
    {
        Release_vReleaseBattery();
        Release_vDelay();
        Release_vNotReleaseAll();
    }
    __RESET_WATCHDOG(); /* feeds the dog */
 //  	release.NotReleaseAll();
  }
} 