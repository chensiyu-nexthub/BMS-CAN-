/*
 * MainProcess.h
 *
 *  Created on: Jun 22, 2010
 *      Author: Administrator
 */

#ifndef MAINPROCESS_H_
#define MAINPROCESS_H_
typedef void (*ProcessType)();

void MainProcess_Init();
void MainProcess_EqualVoltage();
void MainProcess_SendTime();
void MainProcess_SendCurrent();
void MainProcess_SendQuanlity();
void MainProcess_LoadProcess(unsigned char processID,unsigned int time,ProcessType process);
void MainProcess_Restart();
void MainProcess();
#endif /* MAINPROCESS_H_ */
