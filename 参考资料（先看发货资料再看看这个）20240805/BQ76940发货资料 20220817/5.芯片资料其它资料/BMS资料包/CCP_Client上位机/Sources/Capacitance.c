#include "Capacitance.h"
#include "MC9S08DZ60.h"
#include "HC4067.h"
#include "ADC.h"

static UINT8 g_ubCapacitanceInitFlag=0;
UINT16 g_uiCapcitanceDelayTime=24000;
extern UINT8 g_ubHC4067Channel;

static void Capacitance_vDelay(unsigned max) 
{
  UINT16 i=0;
  for(i=0;i<max;i++)
  ;
}
void Capacitance_vInit() 
{
  g_ubCapacitanceInitFlag=1;
}
void Capacitance_vStartCharge(UINT8 channel) 
{
  if(g_ubCapacitanceInitFlag!=1)
    Capacitance_vInit();
  g_ubHC4067Channel=channel;
	HC4067_vSelect();
  Capacitance_vDelay(g_uiCapcitanceDelayTime);//Delay for 20ms
  HC4067_vRelease();
}
UINT16 Capacitance_uiGetVoltage() 
{
  UINT16 returnValue;
  
  if(g_ubCapacitanceInitFlag!=1)
    Capacitance_vInit();
	PTCD_PTCD4=1;
	PTCDD_PTCDD4=1;	//VS1
  returnValue=ADC_uiConvert(10);	
	PTCD_PTCD4=0;
	returnValue=returnValue*1.221;
	return returnValue;
}
void Capacitance_vRelease() 
{
  if(g_ubCapacitanceInitFlag!=1)
    Capacitance_vInit();
  
	PTBD_PTBD4=1;
	PTBDD_PTBDD4=1;	//VS2
	Capacitance_vDelay(1000);
	PTBD_PTBD4=0;
}