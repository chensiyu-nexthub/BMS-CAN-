#ifndef _ONEWIRE_H
#define _ONEWIRE_H


#include "DSP28x_Project.h"

#define SET_DQ() 	1//	GPIO_SetBits(GPIOA,GPIO_Pin_8)
#define CLR_DQ() 	1//	GPIO_ResetBits(GPIOA,GPIO_Pin_8)

#define GET_DQ() 	1//	GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8)

void Onewire_Enable_GPIO_Port(void);
void Onewire_OUT_PULL_UP(void);
void Onewire_OUT_FLOATING(void);
void Onewire_IN_FLOATING(void);
void Onewire_IN_PULL_UP(void);
void resetOnewire(void) ;
Uint16 rOnewire(void);
void wOnewire(Uint16 data);

#endif /*_ONEWIRE_H*/
