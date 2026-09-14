#include "SEED-DEC2407.h"
#include "dpram.h"
/*该模块实现2407对双口ram的读写访问
在完成写操作的同时，对对方端口邮箱即8FFF/8FFE进行写操作以产生中断，
通知对方读取数据；读操作通过中断在中断服务程序中实现*/

void WR2DRAM(float* array,unsigned int address,int n)
{
	unsigned int* p=(unsigned int*)address;
	for(;n!=0;n--,array++, p++)*p=*array;
	*INTMBR=n;
}

void RDDRAM(int* array, unsigned int* address, int n)
{
	for(;n!=0;n--,array++, address++)
	{*address=*array;}
}