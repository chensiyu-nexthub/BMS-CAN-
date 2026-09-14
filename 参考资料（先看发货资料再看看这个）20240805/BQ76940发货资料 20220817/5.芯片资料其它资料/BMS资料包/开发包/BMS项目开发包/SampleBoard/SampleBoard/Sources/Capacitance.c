#include "Capacitance.h"
#include "System.h"
#include "MC9S08DZ60.h"
#include "HC4067.h"
#include "ADC.h"

static unsigned char m_isCapacitanceInit=0;

static void Capacitance_Delay(unsigned max) 
{
  unsigned int i=0;
  for(i=0;i<max;i++)
  ;
}
void Capacitance_Init() 
{
  m_isCapacitanceInit=1;
}
void Capacitance_StartCharge(unsigned char channel) 
{
  if(m_isCapacitanceInit!=1)
    Capacitance_Init();
  
	HC4067_Select(channel);
  Capacitance_Delay(24000);//Delay for 20ms
  HC4067_Release();
}
uword Capacitance_GetVoltage() 
{
  uword returnValue;
  
  if(m_isCapacitanceInit!=1)
    Capacitance_Init();
	PTCD_PTCD4=1;
	PTCDD_PTCDD4=1;	//VS1
  returnValue=ADC_Convert(10);	
	PTCD_PTCD4=0;
	return returnValue;
}
void Capacitance_Release() 
{
  if(m_isCapacitanceInit!=1)
    Capacitance_Init();
  
	PTBD_PTBD4=1;
	PTBDD_PTBDD4=1;	//VS2
	Capacitance_Delay(1000);
	PTBD_PTBD4=0;
}