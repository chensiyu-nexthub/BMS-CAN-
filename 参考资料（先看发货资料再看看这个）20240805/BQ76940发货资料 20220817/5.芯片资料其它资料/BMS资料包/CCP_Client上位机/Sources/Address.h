/*
 * Address.h
 *
 *  Created on: 2010-8-7
 *      Author: Administrator
 */

#ifndef ADDRESS_H_
#define ADDRESS_H_
#include "type.h"
struct STAddress
{
	UINT8 ubShortAdd;
	UINT8 ubShortSub;
	UINT8*   pubAddress;
};
#define Pub_To_Ul_HF(__ul,__pub) (__ul)=__pub[0];(__ul)<<=8;(__ul)+=(__pub)[1];(__ul)<<=8;(__ul)+=(__pub)[2];(__ul)<<=8;(__ul)+=(__pub)[3];
#define Pub_To_Ui_HF(__ui,__pub) (__ui)=(__pub)[0];(__ui)<<=8;(__ui)+=(__pub)[1];
#define Ul_To_Pub_HF(__pub,__ul) (__pub)[3]=(__ul)&0xff;(__ul)>>=8;(__pub)[2]=(__ul)&0xff;(__ul)>>=8;(__pub)[1]=(__ul)&0xff;(__ul)>>=8;(__pub)[0]=(__ul)&0xff;
#define Ui_To_Pub_HF(__pub,__ui) (__pub)[1]=(__ui)&0xff;(__ui)>>=8;(__pub)[0]=(__ui)&0xff;
#define Pub4_Move(__pubT,__pubF) (__pubT)[0]=(__pubF)[0];(__pubT)[1]=(__pubF)[1];(__pubT)[2]=(__pubF)[2];(__pubT)[3]=(__pubF)[3];
#define Pub2_Move(__pubT,__pubF) (__pubT)[0]=(__pubF)[0];(__pubT)[1]=(__pubF)[1];

#define Pub_To_Ul_LF(__ul,__pub) (__ul)=__pub[3];(__ul)<<=8;(__ul)+=(__pub)[2];(__ul)<<=8;(__ul)+=(__pub)[1];(__ul)<<=8;(__ul)+=(__pub)[0];
#define Pub_To_Ui_LF(__ui,__pub) (__ui)=(__pub)[1];(__ui)<<=8;(__ui)+=(__pub)[0];
#define Ul_To_Pub_LF(__pub,__ul) (__pub)[0]=(__ul)&0xff;(__ul)>>=8;(__pub)[1]=(__ul)&0xff;(__ul)>>=8;(__pub)[2]=(__ul)&0xff;(__ul)>>=8;(__pub)[3]=(__ul)&0xff;
#define Ui_To_Pub_LF(__pub,__ui) (__pub)[0]=(__ui)&0xff;(__ui)>>=8;(__pub)[1]=(__ui)&0xff;

#define ADDRESS_ADD(__addr,__v)  	g_stAddress.ubShortAdd=__v;g_stAddress.pubAddress=__addr;Address_vAdd();

#define ADDRESS_SUB(__addr,__v)  	g_stAddress.ubShortSub=__v;g_stAddress.pubAddress=__addr;Address_vSub();

void Address_vAdd();
void Address_vAddTest();
#endif /* ADDRESS_H_ */
