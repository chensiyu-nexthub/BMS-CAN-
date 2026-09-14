#ifndef TASK_H
#define TASK_H


#define SIGNEL_MAX 10
#define TASK_MAX 10
#define TASK_NOTUSE 0
#define TASK_NEEDSTART  1
#define TASK_WAIT    2
#define ERROR_TASK_TASKIDERROR_AT_Task_KillTask
#define ERROR_TASK_NOMORETASK_AT_Task_GetTask

typedef void (*pfVoidFunction)();
typedef unsigned char (*pfUCharFuntion)();
#define DEVICE_CAN 0x1
#define DEVICE_ADC 0x2
#define DEVICE_TIMER 0x3

struct Task_Struct
{
  unsigned char m_ID;
  unsigned int m_startTimer;
  unsigned int m_minTimer;
  unsigned int m_maxTimer;
  unsigned char m_state;
  unsigned char m_signel;

  pfVoidFunction  m_startFunction;
  pfUCharFuntion  m_checkSignelFunction;
  pfVoidFunction  m_nextFunction;

  unsigned long m_useDevice;
};

void Task_Init();
void Task_Wait(pfUCharFuntion checkSignelFunction,pfVoidFunction nextFunction);
unsigned char Task_GetTask(pfVoidFunction startFunction);
void Task_End();
void Task_Scdedule();

#endif
