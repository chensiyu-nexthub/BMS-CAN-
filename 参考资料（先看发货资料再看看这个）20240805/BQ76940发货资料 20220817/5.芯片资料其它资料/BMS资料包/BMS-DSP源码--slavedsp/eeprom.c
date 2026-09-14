#include "SEED-DEC2407.h"
#include "eeprom.h"
//~~~~~~~~~~~~~~~通过SPI写EEPROM函数~~~~~~~~~~~~~~~~~~~
//array保存待发送数据，address为EEPROM地址，n为待发送数据的字节数
void WR2EEPROM(float* array, int address, int n)
{
	int j;
	//设置写允许
	*PCDATDIR=(*PCDATDIR|0x2000)&0x0FFDF; //iopc.5=0即cs=0
	*SPITXBUF=0x06; //写允许指令WREN=06h
	WTTX();				//等待发送成功并清除中断标志
	*PCDATDIR=*PCDATDIR|0x20; //iopc.5=1即cs=1
	//依次写入写指令、地址、数据
	//写入写指令
	*PCDATDIR=(*PCDATDIR|0x2000)&0x0FFDF; //iopc.5=0即cs=0
	*SPITXBUF=0x0002; //写指令WRITE=02h
	WTTX();

	//写入地址
	*SPITXBUF=address; //地址address
	WTTX();

	//写入n个数据
	for(j=n;j>0;j--,array++)
	{ 
		*SPITXBUF=*array;		  //数据array
		WTTX();
	}
	*PCDATDIR=*PCDATDIR|0x20; //iopc.5=1即cs=1以完成写操作
	//设置写禁止
	*PCDATDIR=(*PCDATDIR|0x2000)&0x0FFDF; //iopc.5=0即cs=0
	*SPITXBUF=0x0004; //禁止写指令WRDI=04h
	WTTX();
	*PCDATDIR=*PCDATDIR|0x20; //iopc.5=1即cs=1
}

//array保存接收倒的数据，address为EEPROM地址，n为拟接收数据的字节数
void RDEEPROM(int* array, int address, int n)
{
	int j;
	//依次写入读指令、地址
	//写入读指令
	*PCDATDIR=(*PCDATDIR|0x2000)&0x0FFDF; //iopc.5=0即cs=0
	*SPITXBUF=0x0003; //写指令READ=03h
	WTTX();

	//写入地址
	*SPITXBUF=address; //地址address
	WTTX();
	//保存数据
	for(j=n;n!=0;n--,array++)
	{
		int flag;
		while(1)
		{
			flag=*SPISTS&0x40;			//flag=SPI的中断标志 
			if(flag==0x40)     break;		//如果SPI的中断标志的中断标志为1，
										//则证明SPI已经接收到数据，则停止等待
		}
		*array=*SPIRXBUF;					//将接收到的数据保存在array中
	}
	*PCDATDIR=*PCDATDIR|0x20; //iopc.5=1即cs=1以完成读操作
}
//等待发送成功并清除中断标志
void WTTX()
{
	int flag;
	while(1)
	{
		flag=*SPISTS&0x40;			//flag=SPI的中断标志 
		if(flag==0x40)     break;		//如果SPI的中断标志的中断标志为1，则证明SPI
									//已经发送成功，则停止等待 
	}
	*SPIRXBUF=*SPIRXBUF;	   //虚读以清除中断标志
}



