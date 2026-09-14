#ifndef _ds18b20_h
#define _ds18b20_h

//#define nop() {asm(" nop ");}
//#define PIN_HIGH() {*PFDATDIR=*PFDATDIR|0x7F7F;}//输出高电平
//#define PIN_LOW() {*PFDATDIR=(*PFDATDIR|0x7F00)&0x7F00;}//输出低电平
//#define PIN_LEAVE() {*PFDATDIR = (*PFDATDIR &0x807F)|0x807F;}//输入高电平
#define DATA_BIT 0x00FF
/*~~~~~~~~~~传感器复位程序~~~~~~~~~~~~*/
unsigned char reset(void);
/*~~~~~~~~~~~~从传感器中读一个字节~~~~~~~~~~~~~~*/
void read_byte(unsigned char *ReadByte);
/*~~~~~~~~~将读得的温度转化为十进制~~~~~~~~~~~~~*/
float transform(unsigned char *T);
/*~~~~~~~~~~~读取温度主函数，返回十进制温度数~~~~~~~~~~~~~~~*/
float read_temp();
/*~~~~~~~~~~~~~~向传感器中写一个字节~~~~~~~~~~~~~~*/
void write_byte(unsigned char cmd);

#endif
