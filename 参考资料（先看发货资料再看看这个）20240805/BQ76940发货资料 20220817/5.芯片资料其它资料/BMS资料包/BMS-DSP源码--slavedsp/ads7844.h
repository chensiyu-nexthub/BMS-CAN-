#ifndef _ads7844_h
#define _ads7844_h
/*~~~~~~~~~~~置第i片ad片选为高~~~~~~~~~~~*/
void SetCS(int i);
/*~~~~~~~~~~~~~置第i片ad片选为低~~~~~~~~~~~~~~~*/
void ClcCS(int i);
/*~~~~~~~~~~~~~~~~发送控制字~~~~~~~~~~~~~~~~~~~~~~~*/
void SendConByte(int control);
/*~~~~~~~~~~~~~~~~接收12位数据~~~~~~~~~~~~~~~~*/
int RecData();
///第21-24个clock
void Another4Clk();
/*~~~~~~~~~~~~~~~~~延时~~~~~~~~~~~~~~~~~~~~*/
void delay(int k);				//延迟25*k ns
/*~~~~~~~~~~~~~~~~ad转化程序ADC()~~~~~~~~~~~~~~~~~~~~~~~*/
int ADC (int control, int i);
/*~~~~~~~~~将ad读出的数据装化成对应的值~~~~~~~~~~*/
void convert(int*);

/*~~~~~~~对第i片ad进行转化，将结果保存在result数组中~~~~~~~~~~*/
void ADConversion(int i);
#endif
