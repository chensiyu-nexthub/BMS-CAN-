/*
 * Memery.c
 *
 *  Created on: 2010-8-2
 *      Author: Administrator
 */
#include "Memery.h"
#include "Address.h"
#include "Type.h"

struct STMemery g_stMemery;
#ifdef __DZ60
#undef __DZ60
#endif 
#define __XC2000  1
#define __DZ60    2
#define __CPU     __DZ60

void Memery_vMove()
{
	UINT32 index;
#if(__CPU==__XC2000)
	UINT32 ulFrom;
	UINT32 ulTo;
	__huge UINT8* hpubFrom;
	__huge UINT8* hpubTo;

	Pub_To_Ul_HF(ulFrom,g_stMemery.pubFrom);
	hpubFrom=ulFrom;
	Pub_To_Ul_HF(ulTo,g_stMemery.pubTo);
	hpubTo=ulTo;
	for(index=0;index<g_stMemery.ulLength;index++)
		hpubTo[index]=hpubFrom[index];
#endif
#if(__CPU==__DZ60)
	UINT32 ulFrom;
	UINT32 ulTo;
	UINT8* pubFrom;
	UINT8* pubTo;

	Pub_To_Ul_HF(ulFrom,g_stMemery.pubFrom);
	pubFrom=(UINT8*)(ulFrom&0xffff);
	Pub_To_Ul_HF(ulTo,g_stMemery.pubTo);
	pubTo=(UINT8*)(ulTo&0xffff);
	for(index=0;index<g_stMemery.ulLength;index++)
		pubTo[index]=pubFrom[index];
#endif
}


