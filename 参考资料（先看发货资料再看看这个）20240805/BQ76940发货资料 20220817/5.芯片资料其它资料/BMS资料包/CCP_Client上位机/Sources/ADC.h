#ifndef ADC_H
#define ADC_H
#include "MC9S08DZ60.h"

void ADC_vInit();
UINT16* ADC_puiConvertAll();
UINT16 ADC_uiConvert(UINT8 channel);
void ADC_vDelay(UINT16 max);
#endif                                       