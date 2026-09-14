
#include "Error.h"

#include "BMS_CAN.h"
#include "Event.h"

struct S_Error g_error;
extern long long g_llErrorFlag;
void Error_Init(){
 g_error.m_front=ERROR_SIZE-1;
 g_error.m_back=0;
 g_error.m_size=0;
}
void Error_vSendError()
{
	unsigned char pubCANSendBuffer[8];
	pubCANSendBuffer[0]=g_llErrorFlag&0xff;
	pubCANSendBuffer[1]=(g_llErrorFlag&0xff00)>>8;
	pubCANSendBuffer[2]=(g_llErrorFlag&0xff0000)>>16;
	pubCANSendBuffer[3]=(g_llErrorFlag&0xff000000)>>24;
	pubCANSendBuffer[4]=(g_llErrorFlag&0xff00000000)>>32;
	pubCANSendBuffer[5]=(g_llErrorFlag&0xff0000000000)>>40;
	pubCANSendBuffer[6]=(g_llErrorFlag&0xff000000000000)>>48;
	pubCANSendBuffer[7]=(g_llErrorFlag&0xff00000000000000)>>56;

	BMS_CANSend(BMS_ID_HOST_ERROR,pubCANSendBuffer);
}
int Error_IsEmpty() {
  if(g_error.m_size>0)
  return  0;
  return  1;
}

int Error_AddError(unsigned char value) {
  if(Error_IsFull())
  return 0;

  if(g_error.m_front>=ERROR_SIZE-1) //At the end of Error buffer.
  {
     g_error.m_front=0;
  }
  else
  {
    g_error.m_front++;
  }

  g_error.m_errorBuffer[g_error.m_front]=value;
  g_error.m_size++;

  return 1;
}

int Error_IsFull() {
  if(ERROR_SIZE<=g_error.m_size)
  return 1;
  return 0;
}
unsigned char Error_GetError() {
unsigned char old=g_error.m_back;
if(g_error.m_size<=0)
return 0;

if(g_error.m_back>=ERROR_SIZE-1)
 g_error.m_back=0;
else
 g_error.m_back++;

g_error.m_size--;
return  g_error.m_errorBuffer[old];
}

void ERROR_Process_0x1()
{
#ifdef COMPUTE_USB_DEBUG
	printf("ERROR_Process_1");
#endif
}
void ERROR_Process_0x2()
{
#ifdef COMPUTE_USB_DEBUG
	printf("ERROR_Process_2");
#endif
}
void ERROR_Process_0x3()
{
#ifdef	COMPUTE_USB_DEBUG
	printf("ERROR_Process_3");
#endif
}
void ERROR_Process_0x4()
{
#ifdef COMPUTE_USB_DEBUG
	printf("ERROR_Process_4");
#endif
}
void ERROR_Process_0x5()
{
#ifdef COMPUTE_USB_DEBUG
	printf("ERROR_Process_5");
#endif
}
void ERROR_Process_0x6()
{
#ifdef COMPUTE_USB_DEBUG
	printf("ERROR_Process_6");
#endif
}
void ERROR_Process_0x7()
{
#ifdef	COMPUTE_USB_DEBUG
	printf("ERROR_Process_7");
#endif
}
void ERROR_Process_0x8()
{

}
void ERROR_Process_0x9()
{

}
void ERROR_Process_0xa()
{

}
void ERROR_Process_0xb()
{

}
void ERROR_Process_0xc()
{

}
void ERROR_Process_0xd()
{

}
void ERROR_Process_0xe()
{

}
void ERROR_Process_0xf()
{

}
void ERROR_Process_0x10()
{

}
void ERROR_Process_0x11()
{

}
void ERROR_Process_0x12()
{

}
void ERROR_Process_0x13()
{

}
void ERROR_Process_0x14()
{

}
void ERROR_Process_0x15()
{

}
void ERROR_Process_0x16()
{

}
void ERROR_Process_0x17()
{

}
void ERROR_Process_0x18()
{

}
void ERROR_Process_0x19()
{

}
void ERROR_Process_0x1a()
{

}
void ERROR_Process_0x1b()
{

}
void ERROR_Process_0x1c()
{

}
void ERROR_Process_0x1d()
{

}
void ERROR_Process_0x1e()
{

}
void ERROR_Process_0x1f()
{

}
void ERROR_Process_0x20()
{

}
void ERROR_Process_0x21()
{

}
void ERROR_Process_0x22()
{

}
void ERROR_Process_0x23()
{

}
void ERROR_Process_0x24()
{

}
void ERROR_Process_0x25()
{

}
void ERROR_Process_0x26()
{

}
void ERROR_Process_0x27()
{

}
void ERROR_Process_0x28()
{

}
void ERROR_Process_0x29()
{

}
void ERROR_Process_0x2a()
{

}
void ERROR_Process_0x2b()
{

}
void ERROR_Process_0x2c()
{

}
void ERROR_Process_0x2d()
{

}
void ERROR_Process_0x2e()
{

}
void ERROR_Process_0x2f()
{

}void ERROR_Process_0x30()
{

}
void ERROR_Process_0x31()
{

}
void ERROR_Process_0x32()
{

}
void ERROR_Process_0x33()
{

}
void ERROR_Process_0x34()
{

}
void ERROR_Process_0x35()
{

}
void ERROR_Process_0x36()
{

}
void ERROR_Process_0x37()
{

}
void ERROR_Process_0x38()
{

}
void ERROR_Process_0x39()
{

}
void ERROR_Process_0x3a()
{

}
void ERROR_Process_0x3b()
{

}
void ERROR_Process_0x3c()
{

}
void ERROR_Process_0x3d()
{

}
void ERROR_Process_0x3e()
{

}
void ERROR_Process_0x3f()
{

}void ERROR_Process_0x40()
{

}
void ERROR_Process_0x41()
{

}
void ERROR_Process_0x42()
{

}
void ERROR_Process_0x43()
{

}
void ERROR_Process_0x44()
{

}
void ERROR_Process_0x45()
{

}
void ERROR_Process_0x46()
{

}
void ERROR_Process_0x47()
{

}
void ERROR_Process_0x48()
{

}
void ERROR_Process_0x49()
{

}
void ERROR_Process_0x4a()
{

}
void ERROR_Process_0x4b()
{

}
void ERROR_Process_0x4c()
{

}
void ERROR_Process_0x4d()
{

}
void ERROR_Process_0x4e()
{

}
void ERROR_Process_0x4f()
{

}void ERROR_Process_0x50()
{

}
void ERROR_Process_0x51()
{

}
void ERROR_Process_0x52()
{

}
void ERROR_Process_0x53()
{

}
void ERROR_Process_0x54()
{

}
void ERROR_Process_0x55()
{

}
void ERROR_Process_0x56()
{

}
void ERROR_Process_0x57()
{

}
void ERROR_Process_0x58()
{

}
void ERROR_Process_0x59()
{

}
void ERROR_Process_0x5a()
{

}
void ERROR_Process_0x5b()
{

}
void ERROR_Process_0x5c()
{

}
void ERROR_Process_0x5d()
{

}
void ERROR_Process_0x5e()
{

}
void ERROR_Process_0x5f()
{

}void ERROR_Process_0x60()
{

}
void ERROR_Process_0x61()
{

}
void ERROR_Process_0x62()
{

}
void ERROR_Process_0x63()
{

}
void ERROR_Process_0x64()
{

}
void ERROR_Process_0x65()
{

}
void ERROR_Process_0x66()
{

}
void ERROR_Process_0x67()
{

}
void ERROR_Process_0x68()
{

}
void ERROR_Process_0x69()
{

}
void ERROR_Process_0x6a()
{

}
void ERROR_Process_0x6b()
{

}
void ERROR_Process_0x6c()
{

}
void ERROR_Process_0x6d()
{

}
void ERROR_Process_0x6e()
{

}
void ERROR_Process_0x6f()
{

}
void ERROR_Process_0x70()
{

}
void ERROR_Process_0x71()
{

}
void ERROR_Process_0x72()
{

}
void ERROR_Process_0x73()
{

}
void ERROR_Process_0x74()
{

}
void ERROR_Process_0x75()
{

}
void ERROR_Process_0x76()
{

}
void ERROR_Process_0x77()
{

}
void ERROR_Process_0x78()
{

}
void ERROR_Process_0x79()
{

}
void ERROR_Process_0x7a()
{

}
void ERROR_Process_0x7b()
{

}
void ERROR_Process_0x7c()
{

}
void ERROR_Process_0x7d()
{

}
void ERROR_Process_0x7e()
{

}
void ERROR_Process_0x7f()
{

}
void ERROR_Process_0x80()
{

}
void ERROR_Process_0x81()
{

}
void ERROR_Process_0x82()
{

}
void ERROR_Process_0x83()
{

}
void ERROR_Process_0x84()
{

}
void ERROR_Process_0x85()
{

}
void ERROR_Process_0x86()
{

}
void ERROR_Process_0x87()
{

}
void ERROR_Process_0x88()
{

}
void ERROR_Process_0x89()
{

}
void ERROR_Process_0x8a()
{

}
void ERROR_Process_0x8b()
{

}
void ERROR_Process_0x8c()
{

}
void ERROR_Process_0x8d()
{

}
void ERROR_Process_0x8e()
{

}
void ERROR_Process_0x8f()
{

}


void Error_Process(unsigned char ERROR)
{
  switch(ERROR) {
	  SWITCH_PROGRESS(0x1);
	  break;
	  SWITCH_PROGRESS(0x2);
	  break;
	  SWITCH_PROGRESS(0x3);
	  break;
	  SWITCH_PROGRESS(0x4);
	  break;
	  SWITCH_PROGRESS(0x5);
	  break;
	  SWITCH_PROGRESS(0x6);
	  break;
	  SWITCH_PROGRESS(0x7);
	  break;
	  SWITCH_PROGRESS(0x8);
	  break;
	  SWITCH_PROGRESS(0x9);
	  break;
	  SWITCH_PROGRESS(0xa);
	  break;
	  SWITCH_PROGRESS(0xb);
	  break;
	  SWITCH_PROGRESS(0xc);
	  break;
	  SWITCH_PROGRESS(0xd);
	  break;
	  SWITCH_PROGRESS(0xe);
	  break;
	  SWITCH_PROGRESS(0xf);
	  break;
	  SWITCH_PROGRESS(0x10);
	  break;
	  SWITCH_PROGRESS(0x11);
	  break;
	  SWITCH_PROGRESS(0x12);
	  break;
	  SWITCH_PROGRESS(0x13);
	  break;
	  SWITCH_PROGRESS(0x14);
	  break;
	  SWITCH_PROGRESS(0x15);
	  break;
	  SWITCH_PROGRESS(0x16);
	  break;
	  SWITCH_PROGRESS(0x17);
	  break;
	  SWITCH_PROGRESS(0x18);
	  break;
	  SWITCH_PROGRESS(0x19);
	  break;
	  SWITCH_PROGRESS(0x1a);
	  break;
	  SWITCH_PROGRESS(0x1b);
	  break;
	  SWITCH_PROGRESS(0x1c);
	  break;
	  SWITCH_PROGRESS(0x1d);
	  break;
	  SWITCH_PROGRESS(0x1e);
	  break;
	  SWITCH_PROGRESS(0x1f);
	  break;
	  SWITCH_PROGRESS(0x20);
	  break;
	  SWITCH_PROGRESS(0x21);
	  break;
	  SWITCH_PROGRESS(0x22);
	  break;
	  SWITCH_PROGRESS(0x23);
	  break;
	  SWITCH_PROGRESS(0x24);
	  break;
	  SWITCH_PROGRESS(0x25);
	  break;
	  SWITCH_PROGRESS(0x26);
	  break;
	  SWITCH_PROGRESS(0x27);
	  break;
	  SWITCH_PROGRESS(0x28);
	  break;
	  SWITCH_PROGRESS(0x29);
	  break;
	  SWITCH_PROGRESS(0x2a);
	  break;
	  SWITCH_PROGRESS(0x2b);
	  break;
	  SWITCH_PROGRESS(0x2c);
	  break;
	  SWITCH_PROGRESS(0x2d);
	  break;
	  SWITCH_PROGRESS(0x2e);
	  break;
	  SWITCH_PROGRESS(0x2f);
	  break;
	  SWITCH_PROGRESS(0x30);
	  break;
	  SWITCH_PROGRESS(0x31);
	  break;
	  SWITCH_PROGRESS(0x32);
	  break;
	  SWITCH_PROGRESS(0x33);
	  break;
	  SWITCH_PROGRESS(0x34);
	  break;
	  SWITCH_PROGRESS(0x35);
	  break;
	  SWITCH_PROGRESS(0x36);
	  break;
	  SWITCH_PROGRESS(0x37);
	  break;
	  SWITCH_PROGRESS(0x38);
	  break;
	  SWITCH_PROGRESS(0x39);
	  break;
	  SWITCH_PROGRESS(0x3a);
	  break;
	  SWITCH_PROGRESS(0x3b);
	  break;
	  SWITCH_PROGRESS(0x3c);
	  break;
	  SWITCH_PROGRESS(0x3d);
	  break;
	  SWITCH_PROGRESS(0x3e);
	  break;
	  SWITCH_PROGRESS(0x3f);
	  break;
	  SWITCH_PROGRESS(0x40);
	  break;
	  SWITCH_PROGRESS(0x41);
	  break;
	  SWITCH_PROGRESS(0x42);
	  break;
	  SWITCH_PROGRESS(0x43);
	  break;
	  SWITCH_PROGRESS(0x44);
	  break;
	  SWITCH_PROGRESS(0x45);
	  break;
	  SWITCH_PROGRESS(0x46);
	  break;
	  SWITCH_PROGRESS(0x47);
	  break;
	  SWITCH_PROGRESS(0x48);
	  break;
	  SWITCH_PROGRESS(0x49);
	  break;
	  SWITCH_PROGRESS(0x4a);
	  break;
	  SWITCH_PROGRESS(0x4b);
	  break;
	  SWITCH_PROGRESS(0x4c);
	  break;
	  SWITCH_PROGRESS(0x4d);
	  break;
	  SWITCH_PROGRESS(0x4e);
	  break;
	  SWITCH_PROGRESS(0x4f);
	  break;
	  SWITCH_PROGRESS(0x50);
	  break;
	  SWITCH_PROGRESS(0x51);
	  break;
	  SWITCH_PROGRESS(0x52);
	  break;
	  SWITCH_PROGRESS(0x53);
	  break;
	  SWITCH_PROGRESS(0x54);
	  break;
	  SWITCH_PROGRESS(0x55);
	  break;
	  SWITCH_PROGRESS(0x56);
	  break;
	  SWITCH_PROGRESS(0x57);
	  break;
	  SWITCH_PROGRESS(0x58);
	  break;
	  SWITCH_PROGRESS(0x59);
	  break;
	  SWITCH_PROGRESS(0x5a);
	  break;
	  SWITCH_PROGRESS(0x5b);
	  break;
	  SWITCH_PROGRESS(0x5c);
	  break;
	  SWITCH_PROGRESS(0x5d);
	  break;
	  SWITCH_PROGRESS(0x5e);
	  break;
	  SWITCH_PROGRESS(0x5f);
	  break;
	  SWITCH_PROGRESS(0x60);
	  break;
	  SWITCH_PROGRESS(0x61);
	  break;
	  SWITCH_PROGRESS(0x62);
	  break;
	  SWITCH_PROGRESS(0x63);
	  break;
	  SWITCH_PROGRESS(0x64);
	  break;
	  SWITCH_PROGRESS(0x65);
	  break;
	  SWITCH_PROGRESS(0x66);
	  break;
	  SWITCH_PROGRESS(0x67);
	  break;
	  SWITCH_PROGRESS(0x68);
	  break;
	  SWITCH_PROGRESS(0x69);
	  break;
	  SWITCH_PROGRESS(0x6a);
	  break;
	  SWITCH_PROGRESS(0x6b);
	  break;
	  SWITCH_PROGRESS(0x6c);
	  break;
	  SWITCH_PROGRESS(0x6d);
	  break;
	  SWITCH_PROGRESS(0x6e);
	  break;
	  SWITCH_PROGRESS(0x6f);
	  break;
	  SWITCH_PROGRESS(0x70);
	  break;
	  SWITCH_PROGRESS(0x71);
	  break;
	  SWITCH_PROGRESS(0x72);
	  break;
	  SWITCH_PROGRESS(0x73);
	  break;
	  SWITCH_PROGRESS(0x74);
	  break;
	  SWITCH_PROGRESS(0x75);
	  break;
	  SWITCH_PROGRESS(0x76);
	  break;
	  SWITCH_PROGRESS(0x77);
	  break;
	  SWITCH_PROGRESS(0x78);
	  break;
	  SWITCH_PROGRESS(0x79);
	  break;
	  SWITCH_PROGRESS(0x7a);
	  break;
	  SWITCH_PROGRESS(0x7b);
	  break;
	  SWITCH_PROGRESS(0x7c);
	  break;
	  SWITCH_PROGRESS(0x7d);
	  break;
	  SWITCH_PROGRESS(0x7e);
	  break;
	  SWITCH_PROGRESS(0x7f);
	  break;
	  SWITCH_PROGRESS(0x80);
	  break;
	  SWITCH_PROGRESS(0x81);
	  break;
	  SWITCH_PROGRESS(0x82);
	  break;
	  SWITCH_PROGRESS(0x83);
	  break;
	  SWITCH_PROGRESS(0x84);
	  break;
	  SWITCH_PROGRESS(0x85);
	  break;
	  SWITCH_PROGRESS(0x86);
	  break;
	  SWITCH_PROGRESS(0x87);
	  break;
	  SWITCH_PROGRESS(0x88);
	  break;
	  SWITCH_PROGRESS(0x89);
	  break;
	  SWITCH_PROGRESS(0x8a);
	  break;
	  SWITCH_PROGRESS(0x8b);
	  break;
	  SWITCH_PROGRESS(0x8c);
	  break;
	  SWITCH_PROGRESS(0x8d);
	  break;
	  SWITCH_PROGRESS(0x8e);
	  break;
	  SWITCH_PROGRESS(0x8f);
	  break;
  }
}
