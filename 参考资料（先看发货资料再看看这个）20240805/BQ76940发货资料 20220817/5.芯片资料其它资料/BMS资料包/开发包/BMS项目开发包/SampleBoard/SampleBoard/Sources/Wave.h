/*
 * Wave.h
 *
 *  Created on: May 8, 2010
 *      Author: Administrator
 */

#ifndef WAVE_H_
#define WAVE_H_

#define PORT_CONFIGURE(_P,_N,_F) PT##_P##_F##_PT##_P##_F##_N

 
#define PWAVE(___P,___N) PORT_CONFIGURE(___P,___N,DD)=1; PORT_CONFIGURE(___P,___N,D)=~ PORT_CONFIGURE(___P,___N,D);

 
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
