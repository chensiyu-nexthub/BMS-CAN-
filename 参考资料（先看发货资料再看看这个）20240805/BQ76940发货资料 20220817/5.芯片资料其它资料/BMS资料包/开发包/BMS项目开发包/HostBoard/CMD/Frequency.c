
#include "Frequency.h"
#include "SCS.h"

int Module_InitStart()
{

	  SCS_ErrorType Error;

	  PSW_IEN        =  0;

	  ///  -----------------------------------------------------------------------
	  ///  Configuration of the System Clock:
	  ///  -----------------------------------------------------------------------
	  ///  - VCO clock used, input clock is connected
	  ///  - input frequency is 8.00 MHz
	  ///  - configured system frequency is 40.00 MHz
	  ///  - system clock is 40.00MHz

	  MAIN_vUnlockProtecReg();     // unlock write security

	  // initialize CCU6 timer T13 for SCS driver
	  SCS_InitTimer();

	  // perform transition from base mode to normal operating mode
	  Error = SCS_GoFromBaseToNormalMode();

	  // restore CCU6 timer used by SCS driver
	  SCS_RestoreTimer();
	  return 1;
}
int Module_InitEnd()
{
	  MAIN_vLockProtecReg();       // lock write security

	  //   globally enable interrupts
	  PSW_IEN        =  1;
	  return 2;
}
void MAIN_vInit(void)
{

  SCS_ErrorType Error;

  // USER CODE BEGIN (Init,2)

  // USER CODE END

  //   globally disable interrupts
  PSW_IEN        =  0;

  ///  -----------------------------------------------------------------------
  ///  Configuration of the System Clock:
  ///  -----------------------------------------------------------------------
  ///  - VCO clock used, input clock is connected
  ///  - input frequency is 8.00 MHz
  ///  - configured system frequency is 40.00 MHz
  ///  - system clock is 40.00MHz

  MAIN_vUnlockProtecReg();     // unlock write security

  // initialize CCU6 timer T13 for SCS driver
  SCS_InitTimer();

  // perform transition from base mode to normal operating mode
  Error = SCS_GoFromBaseToNormalMode();

  // restore CCU6 timer used by SCS driver
  SCS_RestoreTimer();

  //   -----------------------------------------------------------------------
  //   Initialization of the Peripherals:
  //   -----------------------------------------------------------------------


  //   -----------------------------------------------------------------------
  //   Initialization of the Bank Select registers:
  //   -----------------------------------------------------------------------


  // USER CODE BEGIN (Init,3)

  // USER CODE END

  MAIN_vLockProtecReg();       // lock write security

  //   globally enable interrupts
  PSW_IEN        =  1;

} //  End of function MAIN_vInit



void MAIN_vUnlockProtecReg(void)
{

    SCU_SLC = 0xAAAA;                   // command 0
    SCU_SLC = 0x5554;                   // command 1
    SCU_SLC = 0x96FF;                   // command 2
    SCU_SLC = 0x0000;                   // command 3

} //  End of function MAIN_vUnlockProtecReg


void MAIN_vLockProtecReg(void)
{

    SCU_SLC = 0xAAAA;                   // command 0
    SCU_SLC = 0x5554;                   // command 1
    SCU_SLC = 0x96FF;                   // command 2
    SCU_SLC = 0x1800;                   // command 3;
    SCU_SLC = 0x8EFF;                   // command 4

} //  End of function MAIN_vLockProtecReg


