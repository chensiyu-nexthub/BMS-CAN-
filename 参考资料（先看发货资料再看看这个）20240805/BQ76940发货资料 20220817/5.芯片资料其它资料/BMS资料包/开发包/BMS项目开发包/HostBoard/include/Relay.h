/*
 * Relay.h
 *
 *  Created on: Apr 19, 2010
 *      Author: Administrator
 */

#ifndef RELAY_H_
#define RELAY_H_

#ifndef PF_Error_AddERROR_TYPE
#define PF_Error_AddERROR_TYPE
typedef int (*PF_Error_AddERROR)(unsigned char value);
#endif

void Relay_Start();
int Relay_IsUse();
int Relay_IsPlusRelayOn();
int Relay_IsNegRelayOn();
int Relay_IsPreRelayOn();

int Relay_IsComplete1();
int Relay_IsComplete2();
int Relay_IsComplete3();
void Relay_Init(PF_Error_AddERROR pfError,unsigned int timerCounter);

void Relay_ClosePlus();
void Relay_OpenPlus();
void Relay_CloseNeg();
void Relay_OpenNeg();
void Relay_ClosePre();
void Relay_OpenPre();

#endif /* RELAY_H_ */
