#include "SPI.h"
#include "MC9S08DZ60.h"

static unsigned char m_isSPIInit=0;

void SPI_Init() 
{
   SPI_INIT();
   m_isSPIInit=1;
}

void SPI_Delay(uword max)//Delay(100) -> delay 150us.
{

uword i=0;
for(i=0;i<max;i++)   
;
}
uword SPI_TransmitWord(uword sendValue)
{
uword mask=0x8000;
uword returnValue=0;
ubyte flag=0;
ubyte loop=0;

if(m_isSPIInit!=1)
  SPI_Init();

CLK_SetTo_0();
CS_SetTo_0();

for(loop=0;loop<16;loop++) 
{
  flag =0;
  SPI_Delay(1);
  //Master read the data from slave.
  MISO_GetValue(flag);
  
  returnValue<<=1;
  if(flag==1) 
  {
    returnValue++;
  }
  SPI_Delay(4);
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
  
  SPI_Delay(1);
  //Master send clock.
  CLK_SetTo_1();
  SPI_Delay(4);
  CLK_SetTo_0(); 
  
}

CLK_SetTo_0();
CS_SetTo_1();
SPI_Delay(10);
return returnValue;
}