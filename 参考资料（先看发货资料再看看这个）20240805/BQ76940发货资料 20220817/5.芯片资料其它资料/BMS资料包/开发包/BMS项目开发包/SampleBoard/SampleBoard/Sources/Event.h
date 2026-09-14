#ifndef EVENT_H
#define EVENT_H
#define EVENT_NOT_CRC 0
#define ERROR_EEPROM_TO_SHAREMEMERY  (1<<2)
#define ERROR_SHAREMEMERY_TO_EEPROM	 (1<<3)

void Event_Progress(unsigned int event);
unsigned int Event_uiIsCRCEEPROM(unsigned int startAddress,unsigned int count);
unsigned int Event_uiIsCRCShareDataTemp();
void Event_vCRCShareMemery();
#endif