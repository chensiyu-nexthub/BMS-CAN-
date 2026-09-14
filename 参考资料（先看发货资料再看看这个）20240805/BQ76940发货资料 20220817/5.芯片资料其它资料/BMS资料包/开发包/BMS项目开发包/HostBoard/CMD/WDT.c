
#include "WDT.h"
#include "XC22xxMREGS.h"
#include "Wave.h"

void WDT_Init(void)
{
  // USER CODE BEGIN (Init,2)

  // USER CODE END

  ///  - DISWDT executable only until End Of Init
  ///  - the input frequency is Fin / 16384
  ///  - period in normal watchdog mode =  13.107 ms
  ///  - period in time-out mode =  1.638 ms

  CPUCON1       |=  0x0000;      // CPU Control Register 1
  SCU_WDTREL     =  0xFFE0;      // WDT Reload Register
  SCU_WDTCS     |=  0x0000;      // WDT Control and Status Register

  SCU_INTDIS     =  0x0000;      // Interrupt Disable Register
  SCU_INTNP1     =  0x0001;      // Interrupt Node Pointer 1 Register
  SCU_1IC        =  0x007C;      // Interrupt Request 1 Register

} //  End of function WDT_vInit


__interrupt(WDT_INT)  void WDT_viINT(void)
{

  // USER CODE BEGIN (WDT_viINT, 2)
	Wave_Change_1();
  // USER CODE END


} //  End of function WDT_viINT


// USER CODE BEGIN (WDT_viINT,3)

// USER CODE END

