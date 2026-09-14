#ifndef ADC_H
#define ADC_H
#include "MC9S08DZ60.h"

void ADC_Init();
uword* ADC_ConvertAll();
unsigned int ADC_Convert(unsigned char channel);
void ADC_Delay(unsigned int max);
#endif                                       