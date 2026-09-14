#include "Task.h"


struct Task_Struct g_taskArray[TASK_MAX];
unsigned char g_currentTaskID;

unsigned char g_taskCount;

void Task_Init()
{
  unsigned char i=0;
  for(i=0;i<TASK_MAX;i++)
  {
    g_taskArray[i].m_state=TASK_NOTUSE;
  }
  g_currentTaskID=0;

  g_taskCount=0;
}



void Task_Wait(pfUCharFuntion checkSignelFunction,pfVoidFunction nextFunction)
{
  g_taskArray[g_currentTaskID].m_checkSignelFunction=checkSignelFunction;
  g_taskArray[g_currentTaskID].m_nextFunction=nextFunction;
  g_taskArray[g_currentTaskID].m_state=TASK_WAIT;
}

unsigned char Task_GetTask(pfVoidFunction startFunction)
{
  unsigned char i;
  if(g_taskCount>=TASK_MAX)
  {
    ERROR_TASK_NOMORETASK_AT_Task_GetTask;
  }
  else
  {
    g_taskCount++;
    for(i=0;i<TASK_MAX;i++)
    {
      if(g_taskArray[i].m_state==TASK_NOTUSE)
      {
        g_taskArray[i].m_startFunction= startFunction;
        g_taskArray[i].m_state=TASK_NEEDSTART;
        return i;
      }
    }

  }
  return 0;
}

void Task_End()
{
  if(g_currentTaskID>=g_taskCount)
  {
    ERROR_TASK_TASKIDERROR_AT_Task_KillTask;
  }
  else
  {
    g_taskArray[g_currentTaskID].m_state=TASK_NOTUSE;
    g_taskCount--;
  }
}


void Task_Scdedule()
{
  unsigned char  i;
  while(1)
  {
  for(i=0;i<TASK_MAX;i++)
  {
    g_currentTaskID=i;

    if(g_taskArray[i].m_state==TASK_NEEDSTART)
      g_taskArray[i].m_startFunction();
    else if(g_taskArray[i].m_state==TASK_WAIT)
    {
      g_taskArray[i].m_signel=g_taskArray[i].m_checkSignelFunction();
      if(g_taskArray[i].m_signel!=0)
        g_taskArray[i].m_nextFunction();
    }
  }
  }
}

