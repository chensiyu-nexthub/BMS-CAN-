
#include "Task.h"
#include "TaskProcess.h"
#include "Event.h"
#include "Error.h"
int  testIntterupt=0;

void Process_Init()
{
}
#ifdef DEVELOP_BOARD___
/////////////////////////////Process 1//////////////////////
void Process_1_Start()
{
   Led_Display();
   Task_Wait(Process_1_Check,Process_1_Start);
}
unsigned char Process_1_Check()
{
    return 1;
}
void Process_1_End()
{

}
///////////////////////////Process 2/////////////////////////
void Process_2_Start()
{
   Key_GetKey();
   Task_Wait(Process_2_Check,Process_2_Start);
}

unsigned char Process_2_Check()
{
    return 1;
}
//////////////////////////Process 3////////////////////////
void Process_3_Start()
{
  Task_Wait(Process_3_Check,Process_3_Loop);
}
unsigned char Process_3_Check()
{
    if(Event_IsEmpty()!=1)
    return 1;
    return 0;
}
void Process_3_Loop()
{
  Event_Progress(Event_GetEvent());
  Task_Wait(Process_3_Check,Process_3_Loop);
}



/////////////////////////////////Procress 4////////////////////////////
void Process_4_Start()
{
   unsigned int i;
   testIntterupt=1;
   EnableInterrupts;
   //TPM2CNTH=0;
   //while(TPM2CNTH<0x8f);
   //DisableInterrupts;
   for(i=0;i<0x1;i++);

   DisableInterrupts;
   testIntterupt=0;
   Task_Wait(Process_4_Check,Process_4_Start);
   //if(Led_IsLed_1_On())
		//	Led_TurnOffLed_1();
	 //else  Led_TurnOnLed_1();
}
unsigned char Process_4_Check()
{
    return 1;
}
#endif
