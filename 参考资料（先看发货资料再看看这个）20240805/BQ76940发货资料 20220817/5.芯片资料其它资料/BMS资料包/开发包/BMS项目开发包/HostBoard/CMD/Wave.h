/*
 * Wave.h
 *
 *  Created on: May 8, 2010
 *      Author: Administrator
 */

#ifndef WAVE_H_
#define WAVE_H_

#define __C__OUT(___P) ___P=0x80;
#define __C__C(___P) ___P=~___P

#define POUT_00(___T) P##___T##_IOCR00=0x80;
#define POUT_01(___T) P##___T##_IOCR01=0x80;
#define POUT_02(___T) P##___T##_IOCR02=0x80;
#define POUT_03(___T) P##___T##_IOCR03=0x80;
#define POUT_04(___T) P##___T##_IOCR04=0x80;
#define POUT_05(___T) P##___T##_IOCR05=0x80;
#define POUT_06(___T) P##___T##_IOCR06=0x80;
#define POUT_07(___T) P##___T##_IOCR07=0x80;
#define POUT_08(___T) P##___T##_IOCR08=0x80;
#define POUT_09(___T) P##___T##_IOCR09=0x80;
#define POUT_10(___T) P##___T##_IOCR10=0x80;
#define POUT_11(___T) P##___T##_IOCR11=0x80;
#define POUT_12(___T) P##___T##_IOCR12=0x80;
#define POUT_13(___T) P##___T##_IOCR13=0x80;
#define POUT_14(___T) P##___T##_IOCR14=0x80;
#define POUT_15(___T) P##___T##_IOCR15=0x80;
#define PCHANGE(___C,___T) P##___C##_OUT_P##___T=~P##___C##_OUT_P##___T

#define PWAVE_00(___C) POUT_00(___C); PCHANGE(___C,0);
#define PWAVE_01(___C) POUT_01(___C); PCHANGE(___C,1);
#define PWAVE_02(___C) POUT_02(___C); PCHANGE(___C,2);
#define PWAVE_03(___C) POUT_03(___C); PCHANGE(___C,3);
#define PWAVE_04(___C) POUT_04(___C); PCHANGE(___C,4);
#define PWAVE_05(___C) POUT_05(___C); PCHANGE(___C,5);
#define PWAVE_06(___C) POUT_06(___C); PCHANGE(___C,6);
#define PWAVE_07(___C) POUT_07(___C); PCHANGE(___C,7);
#define PWAVE_08(___C) POUT_08(___C); PCHANGE(___C,8);
#define PWAVE_09(___C) POUT_09(___C); PCHANGE(___C,9);
#define PWAVE_10(___C) POUT_10(___C); PCHANGE(___C,10);
#define PWAVE_11(___C) POUT_11(___C); PCHANGE(___C,11);
#define PWAVE_12(___C) POUT_12(___C); PCHANGE(___C,12);
#define PWAVE_13(___C) POUT_13(___C); PCHANGE(___C,13);
#define PWAVE_14(___C) POUT_14(___C); PCHANGE(___C,14);
#define PWAVE_15(___C) POUT_15(___C); PCHANGE(___C,15);

#define POTR_WAVE(__P,__L) {\
	unsigned int portValue=P##__P##_OUT;\
	unsigned int mask,bitValue;\
	mask=~(1<<__L);\
	bitValue=(~P##__P##_OUT)&(1<<__L);\
	P0_OUT=(P##__P##_OUT&mask)|bitValue;}


void Wave_Change_1();
void Wave_Change_2();
void Wave_Change_3();
void Wave_Change_4();
void Wave_Change_5();
void Wave_Change_6();
void Wave_Change_7();
void Wave_Change_8();
void Wave_Change_9();
void Wave_Change_10();
void Wave_Change_11();
void Wave_Change_12();
void Wave_Change_13();
void Wave_Change_14();
void Wave_Change_15();
void Wave_Change_16();

#endif /* WAVE_H_ */
