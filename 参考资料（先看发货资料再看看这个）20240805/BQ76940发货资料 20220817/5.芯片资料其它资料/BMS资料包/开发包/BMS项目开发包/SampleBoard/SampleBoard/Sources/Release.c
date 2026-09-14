#include "Release.h"
#include "MC9S08DZ60.h"
static unsigned char m_initFlag=0;

void Release_Init()
{
	V1_Control=1;
	V2_Control=1;
	V3_Control=1;
	V4_Control=1;
	V5_Control=1;
	V6_Control=1;
	V7_Control=1;
	V8_Control=1;
	V9_Control=1;
	V10_Control=1;
	V11_Control=1;
	V12_Control=1;
	V13_Control=1;
	V14_Control=1;
	V15_Control=1;
	V16_Control=1;
  V1_DS =0;
  V2_DS =0;
  V3_DS =0;
  V4_DS =0;
  V5_DS =0;
  V6_DS =0;
  V7_DS =0;
  V8_DS =0;
  V9_DS =0;
  V10_DS =0;
  V11_DS =0;
  V12_DS =0;
  V13_DS =0;
  V14_DS =0;
  V15_DS =0;
  V16_DS =0;
}

void Release_NotReleaseAll() 
{
  if(m_initFlag!=1) 
  {
    m_initFlag=1;
    Release_Init();
  }
  V1_DS =0;
  V2_DS =0;
  V3_DS =0;
  V4_DS =0;
  V5_DS =0;
  V6_DS =0;
  V7_DS =0;
  V8_DS =0;
  V9_DS =0;
  V10_DS =0;
  V11_DS =0;
  V12_DS =0;
  V13_DS =0;
  V14_DS =0;
  V15_DS =0;
  V16_DS =0;
}

void Release_ReleaseBattery(unsigned char index) 
{
  if(m_initFlag!=1) 
  {
    m_initFlag=1;
    Release_Init();
  }
  switch(index) {
    case 0:
    V1_DS =1;
    break;
    case 1:
    V2_DS =1;
    break;
    case 2:
    V3_DS =1;
    break;
    case 3:
    V4_DS =1;
    break;
    case 4:
    V5_DS =1;
    break;
    case 5:
    V6_DS =1;
    break;
    case 6:
    V7_DS =1;
    break;
    case 7:
    V8_DS =1;
    break;
    case 8:
    V9_DS =1;
    break;
    case 9:
    V10_DS =1;
    break;
    case 10:
    V11_DS =1;
    break;
    case 11:
    V12_DS =1;
    break;
    case 12:
    V13_DS =1;
    break;
    case 13:
    V14_DS =1;
    break;
    case 14:
    V15_DS =1;
    break;
    case 15:
    V16_DS =1;
    break;
  }
}
