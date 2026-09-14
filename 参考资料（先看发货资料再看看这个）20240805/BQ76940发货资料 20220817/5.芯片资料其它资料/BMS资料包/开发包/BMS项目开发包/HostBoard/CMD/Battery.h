/*
 * Battery.h
 *
 *  Created on: May 6, 2010
 *      Author: Administrator
 */

#ifndef BATTERY_H_
#define BATTERY_H_

#define BATTER_GETVOLTAGE(__input) (__input*0.001221)
#define QQ_TEST 1

void Battery_Init();
unsigned long Battery_Sum();
unsigned int Battery_Average();

#endif /* BATTERY_H_ */
