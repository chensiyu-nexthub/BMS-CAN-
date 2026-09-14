#include "SEED-DEC2407.h"
#include "ads7844.h"
float result[30];//ad读出数据对应的值
int adread[30];//ad读出的数据
int bit,data;
/*~~~~~~~~~~~置第i片ad片选为高~~~~~~~~~~~*/
void SetCS(int i)
{
	if(i==1)*PBDATDIR= *PBDATDIR|0x0001;	//cs0=1
	if(i==2)*PBDATDIR= *PBDATDIR|0x0002;	//cs1=1
	if(i==3)*PBDATDIR= *PBDATDIR|0x0004;	//cs2=1
	if(i==4)*PBDATDIR= *PBDATDIR|0x0008;	//cs3=1
}

/*~~~~~~~~~~~~~置第i片ad片选为低~~~~~~~~~~~~~~~*/
void ClcCS(int i)
{
	if(i==1)*PBDATDIR= *PBDATDIR&0xFFFE;	//cs0=0
	if(i==2)*PBDATDIR= *PBDATDIR&0xFFFD;	//cs1=0
	if(i==3)*PBDATDIR= *PBDATDIR&0xFFFB;	//cs2=0
	if(i==4)*PBDATDIR= *PBDATDIR&0xFFF7;	//cs3=0
}

/*~~~~~~~~~~~~~~~~发送控制字~~~~~~~~~~~~~~~~~~~~~~~*/
void SendConByte(int control)
{
	int j;
	for(j=0; j<8; j++, control<<1)
	{
		delay(4);
		bit=(control&0x0080)|0xFF7F;	//取最高位
		*PBDATDIR= *PBDATDIR&bit;	//从端口发送该位
		*PBDATDIR= *PBDATDIR|0x40;	//sclk=1即IOPB6=1
		delay(8);
		*PBDATDIR= *PBDATDIR&0xFFBF;	//sclk=0
		delay(2);
	}
}

/*~~~~~~~~~~~~~~~~接收12位数据~~~~~~~~~~~~~~~~*/
int RecData()
{
	int j;
	for(j=12; j>0; j--)
	{
		*PBDATDIR= *PBDATDIR|0x40;	//sclk=1
		delay(8);
		*PBDATDIR= *PBDATDIR&0xFFBF;	//sclk=0
		bit=*PBDATDIR&0x0020;			//读取数据
		data=data|(bit<<(j-6));				//与前面的数相或
	}
	return data;
}

///第21-24个clock
void Another4Clk()
{
	int j;
	for(j=4; j>0; j--)
	{
		*PBDATDIR= *PBDATDIR|0x40;	//sclk=1
		delay(8);
		*PBDATDIR= *PBDATDIR&0xFFBF;	//sclk=0
		delay(8);
	}
}

/*~~~~~~~~~~~~~~~~~延时~~~~~~~~~~~~~~~~~~~~*/
void delay(int k)				//延迟25*k ns
{for(; k>0; k--);}

/*~~~~~~~~~~~~~~~~ad转化程序ADC()~~~~~~~~~~~~~~~~~~~~~~~*/
int ADC (int control, int i)
{
	int bit, data=0; //control=0x84h
	ClcCS(i);		//PBDATDIR= PBDATDIR&0xFFFE;	//cs0=0
	//发送控制字
	SendConByte(control);
	delay(24);	//等待busy为低
	//接收ad传来的数据
	data=RecData();
	//第21-24个clock
	Another4Clk();
	SetCS(i);		//PBDATDIR= PBDATDIR|0x0001;	//cs0=1
	return data;
}
/*~~~~~~~~~将ad读出的数据装化成对应的值~~~~~~~~~~*/
void convert(int* pa)
{
	int i,Times1,Times2,Times3;
	for(i=0;i<30;i++,pa++)
	{
		if(i<28)result[i]=*pa*Times1;	//Time1为电压转化倍数
		if(i==28)result[i]=*pa*Times2;	//Times2为总电压转化倍数
		if(i==29)result[i]=*pa*Times3;	//Times3为总电流转化倍数
	}		
	
}
/*~~~~~~~对第i片ad进行转化，将结果保存在result数组中~~~~~~~~~~*/
void ADConversion(int i)
{
	int j,control=0x84;
	for(j=0; j<8; j++)
	{
		control+=j*0x10;
		adread[4*(i-1)+j]=ADC (control, i);
		if((i==4)&(j>5)) break;
	}
}


