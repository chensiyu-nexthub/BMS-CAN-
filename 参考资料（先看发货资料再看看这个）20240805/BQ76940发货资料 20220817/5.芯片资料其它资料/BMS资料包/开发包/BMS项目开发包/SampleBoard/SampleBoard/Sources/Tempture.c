#include "Tempture.h"
#include "MC9S08DZ60.h"
#include "SearchTable.h"
#include "ADC.h"
unsigned char m_isTemptureInit=0;

void Tempture_Init() 
{
    m_isTemptureInit=1;
}

int Tempture_GetTempture(unsigned char index) 
{
  int returnValue;
  uword ADCValue=ADC_Convert(index+1);
  
  if(m_isTemptureInit!=1)
       Tempture_Init();
  
  returnValue=SearchTable_GetTempture(ADCValue);
  return returnValue;
}

