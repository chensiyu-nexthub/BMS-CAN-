/*
 * Shell.h
 *
 *  Created on: May 21, 2010
 *      Author: Administrator
 */

#ifndef SHELL_H_
#define SHELL_H_
int AddERROR(unsigned char value);
void Shell_Run();
int HexToInt(char* input);
void CANReceiveInterruptProcess(unsigned char volatile __far* dataBuffer);
void Shell_Init();
#endif /* SHELL_H_ */
