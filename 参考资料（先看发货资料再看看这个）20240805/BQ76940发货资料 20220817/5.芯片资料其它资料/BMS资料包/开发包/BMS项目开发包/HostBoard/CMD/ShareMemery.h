/*
 * ShareMemery.h
 *
 *  Created on: Jul 10, 2010
 *      Author: Administrator
 */

#ifndef SHAREMEMERY_H_
#define SHAREMEMERY_H_

#define SHARE_MEMERY_ADDREESS_ubRELEASE_OFFSET		0
#define SHARE_MEMERY_ADDREESS_ulQUANTITIY			1
#define SHARE_MEMERY_ADDREESS_ubINCREASE_LIMIT	5
#define SHARE_MEMERY_ADDREESS_ubDECREASE_LIMIT	7
#define SHARE_MEMERY_ADDREESS_ubQUANTITY_PER_SECOND	9
#define SHARE_MEMERY_ADDREESS_ubQUANTITY_DEVIDE		10
#define SHARE_MEMERY_ADDREESS_uiLOWEST_LIMIT_VOLTAGE		11
#define SHARE_MEMERY_ADDREESS_uiHIGHEST_LIMIT_VOLTAGE		13

void ShareMemery_vInit();
unsigned char 	ShareMemery_ubGetShareMemery(unsigned int index);
unsigned int 	ShareMemery_uiGetShareMemery(unsigned int index);
unsigned long 	ShareMemery_ulGetShareMemery(unsigned int index);
long long 		ShareMemery_llGetShareMemery(unsigned int index);

void ShareMemery_vSetShareMemeryUb(unsigned int index,unsigned char value);
void ShareMemery_vSetShareMemeryUi(unsigned int index,unsigned int value);
void ShareMemery_vSetShareMemeryUl(unsigned int index,unsigned long value);
void ShareMemery_vSetShareMemeryLl(unsigned int index,long long value);

#endif /* SHAREMEMERY_H_ */
