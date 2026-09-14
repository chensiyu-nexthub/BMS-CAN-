#ifndef SEARCHTABLE_H
#define SEARCHTABLE_H
#include "Type.h"


typedef struct 
{
	INT16 tempture;
	UINT16 value;
} TemptureTable;


void SearchTable_Init();

INT16 SearchTable_GetTempture( UINT16 ADCValue);


#endif
