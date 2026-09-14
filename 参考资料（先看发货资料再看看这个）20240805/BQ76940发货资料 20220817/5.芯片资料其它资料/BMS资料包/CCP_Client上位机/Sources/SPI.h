#ifndef SPI_H
#define SPI_H
#include "MC9S08DZ60.h"

#define CLK_SetTo_0()   PTED_PTED3=0;
#define CLK_SetTo_1()   PTED_PTED3=1;

#define CS_SetTo_0() PTED_PTED2=0;
#define CS_SetTo_1() PTED_PTED2=1;

#define MOSI_SetTo_0() PTED_PTED4=0;
#define MOSI_SetTo_1() PTED_PTED4=1;

#define MISO_GetValue(__return) PTEDD_PTEDD5=0;__return=PTED_PTED5;

void SPI_vInit();
void SPI_vDelay(UINT16 max);//Delay(100) -> delay 150us.
UINT16 SPI_uiTransmitWord(UINT16 sendValue);

#endif