#ifndef CAPACITANCE_H
#define CAPACITANCE_H
#include "MC9S08DZ60.h"

void Capacitance_Init();
void Capacitance_StartCharge(unsigned char channel);
void Capacitance_Release();
uword Capacitance_GetVoltage();

#endif