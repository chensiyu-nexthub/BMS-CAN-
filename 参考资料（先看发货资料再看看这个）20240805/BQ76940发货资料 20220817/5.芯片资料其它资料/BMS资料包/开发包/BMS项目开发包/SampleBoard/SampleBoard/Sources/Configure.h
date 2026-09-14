#ifndef CONFIGURE_H_
#define CONFIGURE_H_ 1

#define __RELEASE__
#define USE_BOARD_1__
#define CHANNEL_COUNTER 16  
#define FILTER_DATA 1
#define __RELEASE_BATTERY__ 1
//#define USE_DZ60_DEVELOP 1
#define FIRST_CUT 1
void Configure_Init();
unsigned char Configure_GetThisBoardID();
#endif