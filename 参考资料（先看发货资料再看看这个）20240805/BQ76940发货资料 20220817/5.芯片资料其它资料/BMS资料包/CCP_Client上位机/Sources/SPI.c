#include "SPI.h"
#include "MC9S08DZ60.h"
#include "Type.h"
static UINT8 g_ubSPIInitFlag=0;

void SPI_vInit() 
{
   PTEDD_PTEDD2=1;PTEDD_PTEDD3=1;PTEDD_PTEDD4=1;
   g_ubSPIInitFlag=1;
}

void SPI_vDelay(UINT16 max)//Delay(100) -> delay 150us.
{

UINT16 i=0;
for(i=0;i<max;i++)   
;
}
UINT16 SPI_uiTransmitWord(UINT16 sendValue)
{
UINT16 mask=0x8000;
UINT16 returnValue=0;
UINT8 flag=0;
UINT8 loop=0;

if(g_ubSPIInitFlag!=1)
  SPI_vInit();
  
CLK_SetTo_0();
CS_SetTo_0();

for(loop=0;loop<16;loop++) 
{
  flag =0;
  SPI_vDelay(1);
  //Master read the data from slave.
  MISO_GetValue(flag);
  
  returnValue<<=1;
  if(flag==1) 
  {
    returnValue++;
  }
  SPI_vDelay(3);
  //Master send data to slave.
  if((mask&(sendValue<<loop))!=0) 
  { 
    MOSI_SetTo_1();
   // while(1);
  }
  else
  {
    
    MOSI_SetTo_0();  
  }
  
  SPI_vDelay(1);
  //Master send clock.
  CLK_SetTo_1();
  SPI_vDelay(4);
  CLK_SetTo_0(); 
  
}

CLK_SetTo_0();
CS_SetTo_1();
SPI_vDelay(10);
return returnValue;
}