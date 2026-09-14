#ifndef TASKPROCESS_H
#define TASKPROCESS_H

void Process_Init();

void Process_1_Start();
unsigned char Process_1_Check();
void Process_1_End();

void Process_2_Start();
unsigned char Process_2_Check();

void Process_3_Start();
unsigned char Process_3_Check();
void Process_3_Loop();

void Process_4_Start();
unsigned char Process_4_Check();
#endif