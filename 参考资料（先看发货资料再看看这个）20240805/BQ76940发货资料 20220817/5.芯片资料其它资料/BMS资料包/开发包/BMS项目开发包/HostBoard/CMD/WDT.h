
#ifndef _WDT_H_
#define _WDT_H_

void WDT_Init(void);

#define  WDT_vServiceWDT()  __srvwdt()

#define  WDT_vDisableWDT()  __diswdt()

#define  WDT_vEnableWDT()  __enwdt()

#define  WDT_uwWDTStatus()  SCU_WDTCS

#define  WDT_uwGetTmr() SCU_WDTTIM

#define  WDT_INT 0X6B

#endif  // ifndef _WDT_H_
