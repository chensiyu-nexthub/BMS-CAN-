#include "configure.h"
#include "MC9S08DZ60.h"

unsigned char m_boardID;
unsigned char g_configureInit=0;
#define PORT_CONFIGURE(_P,_N,_F) PT##_P##_F##_PT##_P##_F##_N

#define CONFIGURE_BOARD_4 PTFD_PTFD5
#define CONFIGURE_BOARD_3 PTFD_PTFD6
#define CONFIGURE_BOARD_2 PTED_PTED0
#define CONFIGURE_BOARD_1 PTED_PTED1

void Configure_Init() 
{
  unsigned char flag=0;
  
     
  PORT_CONFIGURE(F,6,DD)=0;  //As input
  PORT_CONFIGURE(E,0,DD)=0;
  PORT_CONFIGURE(E,1,DD)=0;
  g_configureInit=1;

  if(CONFIGURE_BOARD_1==1) 
  {
    flag|=1;
  }
  if(CONFIGURE_BOARD_2==1) 
  {
    flag|=2;
  }
  if(CONFIGURE_BOARD_3==1) 
  {
    flag|=4;
  }
  
  m_boardID=flag+1;
}

//return value:
//1:board 1
//2:board 2
//...

unsigned char Configure_GetThisBoardID() 
{
  if(g_configureInit==0)
   Configure_Init();
  
  return m_boardID;
  
}