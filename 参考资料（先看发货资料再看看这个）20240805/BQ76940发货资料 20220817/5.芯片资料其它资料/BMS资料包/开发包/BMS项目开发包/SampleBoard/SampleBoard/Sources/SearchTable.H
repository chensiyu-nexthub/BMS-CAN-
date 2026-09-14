#ifndef SEARCHTABLE_H
#define SEARCHTABLE_H

typedef unsigned int uword;

typedef struct 
{
	int tempture;
	uword value;
} TemptureTable;


void SearchTable_Init();

int SearchTable_GetTempture( uword ADCValue);


#endif
