#include "system.h"
#include "MC9S08DZ60.h"
#include "Capacitance.h"
#include "ADC.h"
#include "HC4067.h"
#include "SPI.h"
#include "Tempture.h"
#include "CAN.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "Release.h"
#include "Wave.h"
#include "Configure.h"
#include "SYMath.h"
#include "Timer.h"
#include "EEPROM.h"

extern unsigned char g_HC4067Channel;
static void System_Copy(unsigned char* to,unsigned char* from,unsigned char len);
static void System_Delay(unsigned int max);
unsigned char g_sampleIsEnable=0;
word m_batteryVoltage[16];
int m_tempture[8];
uword m_SPIReturn;
uword m_ADCReturn;
extern unsigned char m_boardID;
unsigned char m_isCheckEmptyBattery;
unsigned char m_isStartEqual;
unsigned int g_uiReleaseFlag=0;
unsigned char g_sampleShareDataBuffer[64]={
60,2,4,    
};

unsigned char g_sampleShareDataTemp[64];

void System_Init()//It cann't be interruptted.  
{
  System_ChangeFrequency();
  
  m_isStartEqual=0;
  g_uiReleaseFlag=0;
  Configure_Init();
  CAN_Init();
  HC4067_Init();
  Capacitance_Init();
  SearchTable_Init();
  Tempture_Init();
  ADC_Init();
  Release_Init();
  Timer_Init();
  System_ShareDataInit();
  EEPROM_Init();
}
 

void System_ShareDataInit() 
{
    g_sampleShareDataBuffer[BATTERY_RELEASE_TIME_INDEX] =BATTERY_RELEASE_TIME;
    g_sampleShareDataBuffer[BATTERY_RECORVERY_TIME_INDEX] =BATTERY_RECORVERY_TIME;
    g_sampleShareDataBuffer[BATTERY_SAMPLE_TIME_INDEX] =BATTERY_SAMPLE_TIME;   
}
void System_GetVoltage(unsigned char index)
{
  unsigned int ADCData;
  g_HC4067Channel=index;
  Capacitance_StartCharge(index);
  ADCData=Capacitance_GetVoltage();
  
#ifdef FILTER_DATA
  m_batteryVoltage[index]=Math_FilterData(index,ADCData);
#else
  m_batteryVoltage[index]=ADCData;//Math_FilterData(index,ADCData);
#endif
}
void System_GetVoltageRelease(unsigned char index)
{
  Capacitance_StartCharge(index);
  m_batteryVoltage[index]=Capacitance_GetVoltage();
  
  Capacitance_Release();
}


void System_StartSampleVoltage() 
{

  unsigned char channel;
  unsigned int ADCData;
  Capacitance_StartCharge(0);
  
  ADCData=Capacitance_GetVoltage();
  m_batteryVoltage[0]=ADCData;
  Capacitance_Release();

  for(channel=0;channel<16;channel++)
  {
    Capacitance_StartCharge(channel);
    m_batteryVoltage[channel]=Capacitance_GetVoltage();
    Capacitance_Release();
  }

}

void System_ChangeFrequency() 
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


void System_Delay(unsigned int max) 
{
  unsigned int i;
  for(i=0;i<max;i++)
  ;
}

static void System_Copy(unsigned char* to,unsigned char* from,unsigned char len) 
{
  unsigned char i;
  for(i=0;i<len;i++) 
  {
    to[i]=from[i];
  }
}
void System_DoOrder(unsigned char* dataBuffer) 
{
	unsigned char sender=dataBuffer[0];
	unsigned char* parameters=&dataBuffer[3];
  unsigned int orderHigh;
  unsigned int orderLow;
  unsigned int order;
  unsigned int releaseFlagHigh;
  unsigned int releaseFlagLow;

#ifndef FIRST_CUT    
  switch(Configure_GetThisBoardID()) 
  {
    case 1:
    if(sender!=SAMPLE_BOARD1)
	  	return;
    break;
    case 2:
    if(sender!=SAMPLE_BOARD2)
	  	return;
    break;
    case 3:
    break;
    case 4:
    break;
    case 5:
    break;
    case 6:
    break;
    case 7:
    break;
  }
#endif
  orderLow=(unsigned int)dataBuffer[1];
	orderHigh=(unsigned int)dataBuffer[2]; 
	orderHigh<<=8;
	order=orderLow+ orderHigh;
		
  Event_Progress(order);
 	PWAVE(A,1);
}
void System_SwapVoltage() 
{
   unsigned char i=0;
   unsigned int a_i;
   unsigned int b_i;
   unsigned int temp;
   for(i=0;i<16;i++) 
   {
      a_i=m_batteryVoltage[i]&0xff;
      b_i=m_batteryVoltage[i]&0xff00;
      b_i>>=8;
      temp=(a_i<<8)+b_i;
      m_batteryVoltage[i]=temp;
   }
}
void System_SendToCAN()
{
  System_SwapVoltage();
  switch(Configure_GetThisBoardID()) 
  {
    case 1:
    CAN_Send(0x11,(unsigned char*)m_batteryVoltage);
    CAN_Send(0x12,(unsigned char*)&m_batteryVoltage[4]);
    CAN_Send(0x13,(unsigned char*)&m_batteryVoltage[8]);
    CAN_Send(0x14,(unsigned char*)&m_batteryVoltage[12]);
    CAN_Send(0x15,(unsigned char*)m_tempture);
    break;
    case 2:
    CAN_Send(0x21,(unsigned char*)m_batteryVoltage);
    CAN_Send(0x22,(unsigned char*)&m_batteryVoltage[4]);
    CAN_Send(0x23,(unsigned char*)&m_batteryVoltage[8]);
    CAN_Send(0x24,(unsigned char*)&m_batteryVoltage[12]);
    CAN_Send(0x25,(unsigned char*)m_tempture);
    break;
  }
}
void System_CheckProject() 
{
  unsigned char i;
  unsigned char Board1CheckEnd[8]={HOST_BOARD,WM_BOARD_1_CHECKEND,0};
	unsigned char Board2CheckEnd[8]={HOST_BOARD,WM_BOARD_2_CHECKEND,0};
  unsigned char EmptyFlagHigh,EmptyFlagLow;
  unsigned char mask=0;
  	
  System_GetVoltageRelease(0); 

  for(i=0;i<16;i++)//Start get the voltage. 
  {
     DisableInterrupts;
     System_GetVoltageRelease(i);
     __RESET_WATCHDOG(); /* feeds the dog */
     PWAVE(A,2);
     EnableInterrupts;
  }
     
    Release_NotReleaseAll();
     	  
    for(i=0;i<16;i++) 
    {
     	 if((g_uiReleaseFlag<<i)&0x8000)
     	 Release_ReleaseBattery(i); 
    }
     	
    g_uiReleaseFlag=0;
 
    for(i=0;i<8;i++) 
    {
      mask=1;
      if(m_batteryVoltage[i]<0x4ff) 
      {
         mask<<=i;
         EmptyFlagLow|=mask;
      }
    }
    for(i=0;i<8;i++) 
    {
      mask=1;
      if(m_batteryVoltage[i+8]<0x4ff) 
      {
         mask<<=i;
         EmptyFlagHigh|=mask;
      }
    }


    
 #ifdef USE_BOARD_1__
    Board1CheckEnd[3]= EmptyFlagLow;
    Board1CheckEnd[4]= EmptyFlagHigh;
    CAN_Send(2,Board1CheckEnd);
 #endif
 #ifdef USE_BOARD_2__
    Board2CheckEnd[3]= EmptyFlagLow;
    Board2CheckEnd[4]= EmptyFlagHigh;
    CAN_Send(2,Board2CheckEnd);
 #endif
    PWAVE(A,3);    
    __RESET_WATCHDOG(); /* feeds the dog */
   	//release.NotReleaseAll();
}
void System_ReleaseTask()
{
    unsigned char i;
    unsigned int releaseMask=1;
    unsigned char CANBuffer[8];

    if(m_isStartEqual==1) 
    {
      DisableInterrupts;

      for(i=0;i<16;i++) 
      {  
          releaseMask=1;
          releaseMask<<=i;
         	if(g_uiReleaseFlag&releaseMask)
         	  Release_ReleaseBattery(i); 
      }
      
      CANBuffer[0]= g_uiReleaseFlag&0xff;
      CANBuffer[1]= g_uiReleaseFlag>>8;
      CAN_Send(m_boardID+BMS_ID_RELEASE_BOARD_1-1,CANBuffer);
      EnableInterrupts;
    }  
    Timer_SetTimer(BATTERY_RELEASE_TIME_INDEX,
    g_sampleShareDataBuffer[BATTERY_RELEASE_TIME_INDEX],System_NotReleaseTask);
    
}

void System_NotReleaseTask() 
{
  Timer_SetTimer(BATTERY_RELEASE_TIME_INDEX,
    g_sampleShareDataBuffer[BATTERY_RELEASE_TIME_INDEX],0); 
  
  Release_NotReleaseAll();
  
  Timer_SetTimer(BATTERY_RECORVERY_TIME_INDEX,
    g_sampleShareDataBuffer[BATTERY_RECORVERY_TIME_INDEX],System_TrueSampleTime);  
}

void System_TrueSampleTime() 
{
	
	unsigned char Board1End[8]={HOST_BOARD,BOARD_1_END,};
	unsigned char Board2End[8]={HOST_BOARD,BOARD_2_END,}; 
	
  Timer_SetTimer(BATTERY_RECORVERY_TIME_INDEX,
    g_sampleShareDataBuffer[BATTERY_RECORVERY_TIME_INDEX],0);  
 	g_sampleIsEnable=1;
 /* System_SendToCAN();
  switch(Configure_GetThisBoardID()) 
  {
      case 1:
      CAN_Send(0x100,Board1End);
      break;
      case 2:
      CAN_Send(0x100,Board2End);
      break;
  }  
        */
  
  Timer_SetTimer(BATTERY_SAMPLE_TIME,g_sampleShareDataBuffer[BATTERY_SAMPLE_TIME],System_SampleTimeEnd); 
}
void System_SampleTimeEnd() 
{
  m_isStartEqual=0;
  Timer_SetTimer(BATTERY_SAMPLE_TIME,g_sampleShareDataBuffer[BATTERY_SAMPLE_TIME],0); 
}
void System_TestProject() 
{
  unsigned char i;
  unsigned char Board1End[8]={HOST_BOARD,BOARD_1_END,};
	unsigned char Board2End[8]={HOST_BOARD,BOARD_2_END,};
  unsigned char DevelopDataReture[8]={DEVELOP_BOARD_DZ60,WM_DATA_RETURN,0,};
  unsigned int releaseMask=1;

  System_GetVoltage(0);

  while(1) 
  {
    
#ifdef __CHECK_EMPTY_BATTERY__
    if(m_isCheckEmptyBattery==1) 
    {
      m_isCheckEmptyBattery=0;
      System_CheckProject();
    }
#endif
    
    for(i=0;i<CHANNEL_COUNTER;i++)//Start get the voltage. 
    {
      DisableInterrupts;
      g_HC4067Channel=i;
      System_GetVoltage(i);
      __RESET_WATCHDOG(); 
      PWAVE(A,2);
      EnableInterrupts;
    }

    if(m_isStartEqual==0||g_sampleIsEnable==1) 
    {
      
      System_SendToCAN();
      switch(Configure_GetThisBoardID()) 
      {
          case 1:
          CAN_Send(0x100,Board1End);
          break;
          case 2:
          CAN_Send(0x100,Board2End);
          break;
      } 
    }  

    PWAVE(A,3);    
    __RESET_WATCHDOG(); /* feeds the dog */
 //  	release.NotReleaseAll();
  }
} 