#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */
#include "System.h"
#include "MC9S08DZ60.h"
#include "HC4067.h"
#include "SearchTable.h"
#include "Release.h"
//#define RELEASE_TEST_ALL
void main(void) {

#ifdef  RELEASE_TEST_ALL
    int i; 
    DisableInterrupts; /* disable interrupts */
   // System_Init();
    EnableInterrupts; /* enable interrupts */
     /* PTFDS_PTFDS5=1;
      PTFDD_PTFDD5=1;
      PTFD_PTFD5=1;*/
     /*for(i=0;i<16;i++) 
      {  
       	 Release_ReleaseBattery(i); 
      }*/

    PTFDD_PTFDD5=1;
    while(1) 
    {
    PTFD_PTFD5=~PTFD_PTFD5;  
    for(i=0;i<0xf00;i++)
    ;
     __RESET_WATCHDOG(); /* feeds the dog */
    }
    //PTGD_PTGD5=1;
    //PTGDD_PTGDD5=1;
    //PTGD_PTGD4=1;
    //PTGDD_PTGDD4=1;
      while(1) 
      {
          __RESET_WATCHDOG(); /* feeds the dog */
      }
#endif

  DisableInterrupts; /* disable interrupts */

  System_Init();
  EnableInterrupts; /* enable interrupts */
  
  System_TestProject();
  for(;;) {
    __RESET_WATCHDOG(); /* feeds the dog */
  } /* loop forever */
  /* please make sure that you never leave main */
}
