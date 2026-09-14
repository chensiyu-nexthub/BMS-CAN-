/*
 * CMD_JMP.h
 *
 *  Created on: May 20, 2010
 *      Author: Administrator
 */

#ifndef CMD_JMP_H_
#define CMD_JMP_H_
void CMD_JMPInit();
char* CMD_BJMP(char* label,
		char* pVariours_1,char* pVariours_2,char* pVariours_3);
char* CMD_SJMP(char* label,
		char* pVariours_1,char* pVariours_2,char* pVariours_3);
char* CMD_BEJMP(char* label,
		char* pVariours_1,char* pVariours_2,char* pVariours_3);
char* CMD_SEJMP(char* label,
		char* pVariours_1,char* pVariours_2,char* pVariours_3);
char* CMD_EJMP(char* label,
		char* pVariours_1,char* pVariours_2,char* pVariours_3);
char* CMD_NEJMP(char* label,
		char* pVariours_1,char* pVariours_2,char* pVariours_3);
char* CMD_JMP(char* label,
		char* pVariours_1,char* pVariours_2,char* pVariours_3);
char* CMD_NZJMP(char* label,
		char* pVariours_1,char* pVariours_2,char* pVariours_3);
char* CMD_ZJMP(char* label,
		char* pVariours_1,char* pVariours_2,char* pVariours_3);

#endif /* CMD_JMP_H_ */
