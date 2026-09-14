#ifndef SYSTEM_H
#define SYSTEM_H
#include "MC9S08DZ60.h"
#include "BMS_0_4.h"

/*******************************************************************************/

/* External crystal oscillator = 4MHz */
/* FOR PEE mode:
Bus_Clock = (1/2)*[(Osc_clock/Ref_div)*VCO_div]/(Bus_div) = 8MHz */
   
#define BUS_DIV 0       /* Bus divider = 0, divide by 1 */
#define REF_DIV 2       /* Reference divider = 2, divide by 4 */          
#define VCO_DIV 4       /* VCO divider = 4, multiply by 16 */


/* TPM1 Configuration values */
/* Overflow interrupt period is Prescaler*65535/Clock_source   */
/* Clock source options */
#define NO_CLK      0
#define BUS_CLK     1 
#define FIX_CLK     2
#define EXT_CLK     3

#define PS_DIV  4   /* Prescale factor = 2^PS_DIV = 16 */

#define RELAY_CLOSENEG 1
#define RELAY_CLOSEPLUS 2
#define RELAY_CLOSEPRE 3
#define RELAY_OPENNEG 4
#define RELAY_OPENPLUS 5
#define RELAY_OPENPRE 6
#define READ_WORD 0x82
#define BOARD_1_END 7
#define BOARD_2_END 8

#define RELEASE_1 0x1
#define RELEASE_2 0x2
#define RELEASE_3 0x4
#define RELEASE_4 0x8
#define RELEASE_5 0x10
#define RELEASE_6 0x20
#define RELEASE_7 0x40
#define RELEASE_8 0x80
#define RELEASE_9 0x100
#define RELEASE_10 0x200
#define RELEASE_11 0x400
#define RELEASE_12 0x800
#define RELEASE_13 0x1000
#define RELEASE_14 0x2000
#define RELEASE_15 0x4000
#define RELEASE_16 0x8000

#define BATTERY_RELEASE_TIME_INDEX   0
#define BATTERY_RELEASE_TIME         30

#define BATTERY_RECORVERY_TIME_INDEX   1
#define BATTERY_RECORVERY_TIME         2

#define BATTERY_SAMPLE_TIME_INDEX    2
#define BATTERY_SAMPLE_TIME          2

void System_SwapVoltage();
void System_StartSampleVoltage();
void System_ChangeFrequency();
void System_Delay(unsigned int max);
void System_Init();
void System_TestProject();
void System_GetVoltage(unsigned char index);
void System_DoOrder(unsigned char* dataBuffer);
void System_TrueSampleTime();
void System_NotReleaseTask();
void System_ReleaseTask();
void System_ShareDataInit();
void System_SampleTimeEnd();

#endif