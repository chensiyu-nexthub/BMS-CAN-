#ifndef _dpram_h
#define _dpram_h
//array保存待写的数据，address为DRAM地址，n为待写数据的字节数
void WR2DRAM(float* array, unsigned int address, int n);
//array保存读取到的数据，address为DRAM地址，n为拟读取数据的字节数
void RDDRAM(int* array, unsigned int* address, int n);
#endif


