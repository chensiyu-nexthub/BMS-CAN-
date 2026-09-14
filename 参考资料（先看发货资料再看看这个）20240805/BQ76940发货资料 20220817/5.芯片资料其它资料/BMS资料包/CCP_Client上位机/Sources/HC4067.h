#ifndef HC4067_H
#define HC4067_H
#include "MC9S08DZ60.h"

#define HC4067_SELECT_BIT     PTFD_PTFD5  
#define HC4067_SELECT_CONTROL PTFDD_PTFDD5

void HC4067_vInit();
void HC4067_vRelease();
void HC4067_vSelect();

#endif