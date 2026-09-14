/*
 * CMD.h
 *
 *  Created on: May 18, 2010
 *      Author: Administrator
 */

#ifndef CMD_H_
#define CMD_H_
#define CMD_TABLE_SIZE 20
#define CMD_TABLE_LENGTH 64
#define CMD_OK 1
#define CMD_ERROR 0

int CMD_Do(char* cmdString);
void CMD_ReconstructString(char* input,char* output);
void CMD_ADDTable(char* inputString_1,char* inputString_2);
char* CMD_Table(char* inputString);
void CMD_ChangeTable(char* inputString_1,char* inputString_2);
int CMD_TableIndex(char* inputString);
char* CMD_Replace(char* inputString);
void CMD_UpString(char* input);
#endif /* CMD_H_ */



