/*
 * DISPLAY.h
 *
 *  Created on: 2016-5-4
 *      Author: James
 */

#ifndef DISPLAY_H_
#define DISPLAY_H_

#define CS 			GpioDataRegs.GPBDAT.bit.GPIO48    //LCD_CS
#define SCK 		GpioDataRegs.GPBDAT.bit.GPIO55    //LCD_SCK

#define SIDIN 		GpioDataRegs.GPBDAT.bit.GPIO54    //LCD_SDA
#define SID 		GpioDataRegs.GPBDAT.bit.GPIO54    //LCD_SDA

#define SID_IN()  	{EALLOW;	GpioCtrlRegs.GPBDIR.bit.GPIO54 = 0 ;EDIS;}
#define SID_OUT()  	{EALLOW;	GpioCtrlRegs.GPBDIR.bit.GPIO54 = 1 ;EDIS;}

#define f2i1(data)  (Uint16)data,(Uint16) ((data-(Uint16)data)*10)
#define f2i2(data)  (Uint16)data,(Uint16) ((data-(Uint16)data)*100)


void AD_Refresh(void);
void ControlSetting_Refresh(unsigned char Num);



void SPI_Gpio(void);
void SendByte(Uint16 Dbyte);
void LCD12864_com(Uint16 dat);
void LCD12864_dat(Uint16 dat);
void SPI_LCD_Init(void);
Uint16 ReceiveByte(void);
Uint16 ReadData( void );
Uint16 LCD12864_busy(void);
void LCD12864_Clr(void);
void LCD12864_Pos(Uint16 x,Uint16 y);
void LCD_ShowString(Uint16 x,Uint16 y,Uint16 *p);
void LCD_Show16(Uint16 x,Uint16 y,Uint16 temp);


#endif /* DISPLAY_H_ */
