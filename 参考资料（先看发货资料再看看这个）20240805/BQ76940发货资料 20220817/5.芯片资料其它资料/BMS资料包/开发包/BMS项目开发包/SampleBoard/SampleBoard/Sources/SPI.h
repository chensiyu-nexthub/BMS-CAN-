#ifndef SPI_H
#define SPI_H
#include "MC9S08DZ60.h"

#define SPI_INIT() PTEDD_PTEDD2=1;PTEDD_PTEDD3=1;PTEDD_PTEDD4=1;
#define CLK_SetTo_0()   PTED_PTED3=0;
#define CLK_SetTo_1()   PTED_PTED3=1;

#define CS_SetTo_0() PTED_PTED2=0;
#define CS_SetTo_1() PTED_PTED2=1;

#define MOSI_SetTo_0() PTED_PTED4=0;
#define MOSI_SetTo_1() PTED_PTED4=1;

#define MISO_GetValue(__return) PTEDD_PTEDD5=0;__return=PTED_PTED5;

void SPI_Init();
void SPI_Delay(uword max);//Delay(100) -> delay 150us.
uword SPI_TransmitWord(uword sendValue);

#endif