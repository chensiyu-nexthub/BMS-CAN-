#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File
#include "SysInitDefine.h"

#pragma CODE_SECTION(local_DINTCH1_ISR, "ramfuncs");
#pragma CODE_SECTION(xint3_isr, "ramfuncs");//yuanlaimeiyou



#define GLOBAL_Q    24
long GlobalQ = GLOBAL_Q;      		// Used for legacy GEL & Graph Debug.



void main(void)
{

	InitSysCtrl();
	DINT;

	Gpioinit();

#if RamOrFlash
	MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);
	InitFlash();
#endif

   InitPeripherals();

//   SysDataStandby();
//   for(;;)
//   {
//	   Test();
//   }   DELAY_US

    for(; ;)
    {

    }
}


void InitPeripherals()
{


	adc_init();

	AT24CXX_Init();

	InitTimer();

	InitPIE();

	scib_fifo_init();


}


//===========================================================================
// No more.
//===========================================================================
