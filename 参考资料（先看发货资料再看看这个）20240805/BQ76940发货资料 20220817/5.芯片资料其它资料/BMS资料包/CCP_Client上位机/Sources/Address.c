/*
 * Address.c
 *
 *  Created on: 2010-8-7
 *      Author: Administrator
 */
#include "Address.h"

struct STAddress g_stAddress;
static UINT32 g_ulAddress;

#ifdef ADDRESS_TEST
UINT8 g_pubAddess[4]={1,0,2,3};

void Address_vAddTest()
{
	g_stAddress.ubShortAdd=0x10;
	g_stAddress.pubAddress=g_pubAddess;
	Address_vAdd();
}
#endif



void Address_vAdd()
{
	Pub_To_Ul_HF(g_ulAddress,g_stAddress.pubAddress);
	g_ulAddress+=g_stAddress.ubShortAdd;
	Ul_To_Pub_HF(g_stAddress.pubAddress,g_ulAddress);
}
void Address_vSub()
{
	Pub_To_Ul_HF(g_ulAddress,g_stAddress.pubAddress);
	g_ulAddress-=g_stAddress.ubShortSub;
	Ul_To_Pub_HF(g_stAddress.pubAddress,g_ulAddress);
}
