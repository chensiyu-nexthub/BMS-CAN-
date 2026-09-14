#include "Tempture.h"
#include "MC9S08DZ60.h"
#include "SearchTable.h"
#include "ADC.h"
#include "Type.h"
extern INT16 g_piBatteryTempture[8];

void Tempture_vGetTempture() 
{
  UINT16 uiADCValue;
  INT16  iTempture;
  
  uiADCValue=ADC_uiConvert(2);
  g_piBatteryTempture[0]=SearchTable_iGetTempture(uiADCValue);
  
  uiADCValue=ADC_uiConvert(3);
  g_piBatteryTempture[1]=SearchTable_iGetTempture(uiADCValue);
  
  uiADCValue=ADC_uiConvert(4);
  g_piBatteryTempture[2]=SearchTable_iGetTempture(uiADCValue);
  
  uiADCValue=ADC_uiConvert(5);
  g_piBatteryTempture[3]=SearchTable_iGetTempture(uiADCValue);
  
  uiADCValue=ADC_uiConvert(6);
  g_piBatteryTempture[4]=SearchTable_iGetTempture(uiADCValue);
  
  uiADCValue=ADC_uiConvert(7);
  g_piBatteryTempture[5]=SearchTable_iGetTempture(uiADCValue);
  
  uiADCValue=ADC_uiConvert(8);
  g_piBatteryTempture[6]=SearchTable_iGetTempture(uiADCValue);
  
  uiADCValue=ADC_uiConvert(9);
  g_piBatteryTempture[7]=SearchTable_iGetTempture(uiADCValue);
  
}

