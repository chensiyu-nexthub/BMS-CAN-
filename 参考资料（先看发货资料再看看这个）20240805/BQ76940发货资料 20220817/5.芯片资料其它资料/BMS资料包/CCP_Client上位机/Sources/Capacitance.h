#ifndef CAPACITANCE_H
#define CAPACITANCE_H
#include "MC9S08DZ60.h"

void Capacitance_vInit();
void Capacitance_vStartCharge(UINT8 channel);
void Capacitance_vRelease();
UINT16 Capacitance_uiGetVoltage();

#endif