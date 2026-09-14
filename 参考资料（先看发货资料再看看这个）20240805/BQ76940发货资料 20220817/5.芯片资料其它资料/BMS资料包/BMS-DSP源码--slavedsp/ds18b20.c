#include "SEED-DEC2407.h"
#include "ds18b20.h"

#define nop() {asm(" nop");}
void PIN_HIGH() {*PFDATDIR=*PFDATDIR|0x7F7F;}//输出高电平
void PIN_LOW() {*PFDATDIR=(*PFDATDIR|0x7F00)&0x7F00;}//输出低电平
void PIN_LEAVE() {*PFDATDIR = (*PFDATDIR &0x807F)|0x807F;}//输入高电平
/*~~~~~~~~~~传感器复位程序~~~~~~~~~~~~*/
unsigned char reset(void)
{
	unsigned char retval;
	unsigned int loopindex=0;
	PIN_HIGH();
	nop();nop();nop();nop(); /* 延时1us*/
	PIN_LOW();
	for(loopindex=0;loopindex<213;loopindex++){;} /*置总线为低电平并保持至少480us */
	PIN_HIGH();
	for(loopindex=0;loopindex<26;loopindex++){;} /*等电阻拉高总线并保持15－60us */
	PIN_LEAVE(); /*接受应答信号*/
	nop();
	if((*PFDATDIR & DATA_BIT) == 0x0000)
	{ retval = 0; }
	else
	{ retval = 1; }
	PIN_HIGH();
	for(loopindex=0;loopindex<106;loopindex++){;} /*延时60－240us */
	return(retval); /*返回应答信号 */
}

/*~~~~~~~~~~~~从传感器中读一个字节~~~~~~~~~~~~~~*/
void read_byte(unsigned char *ReadByte)
{
	int i;
	unsigned char temp=0;
	unsigned int loopindex=0;
	PIN_HIGH();
	for(i=0;i<8;i++)
	{
		temp=temp>>1;
		PIN_LOW();
		nop();nop();nop();nop(); /*延时2us */
		nop();nop();nop();nop();
		PIN_HIGH();
		for(loopindex=0;loopindex<6;loopindex++){;} /* 延时14us */
		PIN_LEAVE();
		nop();
		if((*PFDATDIR & DATA_BIT) == 0x0000)
		{ temp=temp & 0x7F; }
		else
		{ temp=temp | 0x80; }
		PIN_HIGH();
		for(loopindex=0;loopindex<26;loopindex++){;} /* 延时60us */
	}
	*ReadByte=temp;
	return;
}

/*~~~~~~~~~将读得的温度转化为十进制~~~~~~~~~~~~~*/
float transform(unsigned char *T)
{
	unsigned char temp=0;
	float temprature=0;
	temp=temp|(*(T+1)<<8);
	temp=temp|*T;
	if((temp&0x0F800)==0x0F800)
	{ temprature=0-((~temp)+1)*0.0625; }
	else
	{ temprature=temp*0.0625; }
	return(temprature);
}

/*~~~~~~~~~~~读取温度主函数，返回十进制温度数~~~~~~~~~~~~~~~*/
float read_temp()
{
	unsigned char buff[2];
	float temprature=0;
	unsigned int loopindex=0;
	while (reset()==1){}; /* 复位等待从机应答 */
	write_byte(0xCC); /* 忽略ROM 匹配 */
	write_byte(0x44); /* 发送温度转化命令 */
	for(loopindex=0;loopindex<65535;loopindex++){;} /* 延时300ms,等待数模转换 */
	for(loopindex=0;loopindex<65535;loopindex++){;}
	while(reset()==1){}; /* 再次复位，等待从机应答 */
	write_byte(0xCC); /* 忽略ROM 匹配 */
	write_byte(0xBE); /* 发送读温度命令 */
	read_byte(buff); /* 读出温度低8 位 */
	read_byte(buff+1); /* 读出温度高8 位 */
	temprature=transform(buff);
	PIN_HIGH(); /* 释放总线 */
	return(temprature);
}

/*~~~~~~~~~~~~~~向传感器中写一个字节~~~~~~~~~~~~~~*/
void write_byte(unsigned char cmd)
{
	unsigned char i; 
	unsigned char j; 
	PIN_HIGH();
	for(i=0;i<8;i++) 
	{ 
		PIN_LOW();//DQ=0;
		nop();nop();nop();nop(); /*延时2us */
		nop();nop();nop();nop();
      	if((cmd & 0x01)==0) 
        { 
          	for(j=35;j>0;j--); //延时大于60us
          	PIN_HIGH();//DQ=1; 
          	nop();nop();nop();nop(); /*延时2us */
			nop();nop();nop();nop();
        } 
        else 
        { 
        	PIN_HIGH();//DQ=1; 
            for(j=33;j>0;j--); 
        } 
      	cmd=_cror_(cmd,1);      
	} 
	nop();nop();nop();nop(); //两次写之间延时大于1us
	nop();nop();nop();nop();
}
