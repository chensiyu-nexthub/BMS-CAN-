#include "EEPROM.h"
#include "IEE1.h"

void EEPROM_Init() 
{
     IEE1_Init();
}
unsigned char EEPROM_WriteEEPROM (unsigned int startAddress,unsigned char* writeAddress,unsigned int count) 
{
  unsigned int uiEEPROMAddress= startAddress+IEE1_AREA_START;
  int i;
  for(i=0;i<count;i++) 
  {
    IEE1_SetByte(uiEEPROMAddress,writeAddress[i]);
    startAddress++;
  }
}
unsigned char EEPROM_ReadEEPROM (unsigned int startAddress,unsigned char* returnAddress,unsigned int count) 
{
  unsigned int uiEEPROMAddress= startAddress+IEE1_AREA_START;
  int i;
  for(i=0;i<count;i++) 
  {
    IEE1_GetByte(uiEEPROMAddress,&(returnAddress[i]));
    startAddress++;
  }
}
