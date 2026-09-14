#ifndef EEPROM_H
#define EEPROM_H
#define EEPROM_SAMPLESHARE_MEMERYHEAD 0
#define EEPROM_SAMPLESHARE_MEMERYLENGTH 62
unsigned char EEPROM_WriteEEPROM (unsigned int startAddress,unsigned char* writeAddress,unsigned int count);
unsigned char EEPROM_ReadEEPROM (unsigned int startAddress,unsigned char* returnAddress,unsigned int count);
void EEPROM_Init();
#endif