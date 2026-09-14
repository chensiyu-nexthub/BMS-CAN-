#ifndef _eeprom_h
#define _eeprom_h
//array保存待发送数据，address为EEPROM地址，n为待发送数据的字节数
void WR2EEPROM(float* array, int address, int n);
//array保存接收倒的数据，address为EEPROM地址，n为拟接收数据的字节数
void RDEEPROM(int* array, int address, int n);
//等待发送成功并清除中断标志
void WTTX();
#endif