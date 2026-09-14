/*
 * ADC.h
 *
 *  Created on: Apr 16, 2010
 *      Author: Administrator
 */


#ifndef ADC_H_
#define ADC_H_

#ifndef PF_Error_AddERROR_TYPE
#define PF_Error_AddERROR_TYPE
typedef int (*PF_Error_AddERROR)(unsigned char value);
#endif
//The frequency is 1M
#define ADC_CHAINSIZE 10
void ADC_EmptyChain();
void ADC_AddChain(unsigned char channel);
unsigned int* ADC_GetCurrent();
unsigned int* ADC_StartChain();
void ADC_Init(PF_Error_AddERROR pfError,unsigned int timerCounter);
unsigned int ADC_GetChannel(unsigned char index);
unsigned int* ADC_GetChannelArray();


#endif /* ADC_H_ */
