/*
 * CANTask.h
 *
 *  Created on: May 6, 2010
 *      Author: Administrator
 */

#ifndef CAN_TASK_H_
#define CAN_TASK_H_

void CAN_Task_DoOrder(unsigned char volatile __far* dataBuffer);
unsigned int CAN_Task_GetStatus();
void CAN_Task_Init();

#endif /* CANTASK_H_ */
