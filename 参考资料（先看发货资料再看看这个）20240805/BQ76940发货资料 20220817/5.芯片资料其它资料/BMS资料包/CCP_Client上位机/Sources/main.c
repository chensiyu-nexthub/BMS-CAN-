#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */
#include "CCP.h"
#include "CAN.h"
#include "Type.h"
#include "System.h"

void Main_vDelay(UINT16 uiMax) 
{
  UINT16 i; 
  for(i=0;i<uiMax;i++)
  ;
}

void main(void) 
{

  EnableInterrupts; /* enable interrupts */

  System_vInit();
  System_vMainProcess();
  
  for(;;) 
  {
    __RESET_WATCHDOG(); /* feeds the dog */
  } /* loop forever */
  /* please make sure that you never leave main */
}
