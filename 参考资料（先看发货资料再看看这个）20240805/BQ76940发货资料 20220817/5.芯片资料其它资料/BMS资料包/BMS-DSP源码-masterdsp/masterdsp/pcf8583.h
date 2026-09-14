#ifndef _pcf8583_h
#define _pcf8583_h
// 软件延时子程序 
void delay();
// 启动I2C总线子程序 
void START();
// 发送字节子程序 
void TRANSMIT(int TRAN);
// 从机( 即PCF8583芯片 )应答子程序。返回值为0时，代表操作成功；返回值为1时，代表操作失败 
int	SLAVE_ACK();
// I2C停止子程序 
void STOP();
// 字节写子程序，即向I2C器件写1个字节的数据，入口为地址BYTE_ADDR和需要写入的字节
// 内容T_DATA。返回值为0时，代表操作成功；返回值为1时，代表操作失败 
int	BYTE_WR(int BYTE_ADDR,int T_DATA);
// 接收一个字节子程序，出口为接收到的数据R_DATA 
int RECEIVE();	
// 主机无应答信号子程序 
void NO_ACK();
// 主机应答子程序 
void MASTER_ACK();
// 字节读子程序，即从I2C器件读出1个字节的数据，入口为需要读出的地址BYTE_ADDR，出口
// 为读出的数据R_DATA，通过 语言的参数传递功能实现。返回值为0X0FFFF时，表示操作失败；
// 否则操作成功 
int	BYTE_RD(int BYTE_ADDR);
// 连续写子程序，入口为需要写的起始地址ADDR，存储需要写入数据的数组的首地址ARRY，需要
// 写入的数据的个数N 
int	CON_WR(int ADDR,int* ARRAY,int N);
// 连续读子程序，入口为需要读的起始地址ADDR，存储读出数据的数组首地址ARRY，需要读出的
// 数据的个数N 
int	CON_RD(int ADDR,int* ARRAY,int N);
//连续写入N个浮点数
int	CON_WRF(int ADDR,float* ARRAY,int N);
//连续读出N个浮点数
int	CON_RDF(int ADDR,float* ARRAY,int N);
//计算时间差
int CalPlaceTime(int *ARRAY1,int *ARRAY2);
#endif