/*
 * BMS_CAN.h
 *
 *  Created on: May 31, 2010
 *      Author: Administrator
 */

#ifndef BMS_CAN_H_
#define BMS_CAN_H_

#ifndef BOARD_DEFINE
#define BOARD_DEFINE 1



#define HOST_BOARD 0x10
#define SAMPLE_BOARD1 0x11
#define SAMPLE_BOARD2 0x12
#define DEVELOP_BOARD 0x13

#endif
#define BMS_ID_HOST_RECEIVE_ORDER 0x100
#define BMS_ID_HOST_SEND_ORDER 0x101

#define BMS_ID_CLIENT_RECEIVE_ORDER 0x103
#define BMS_ID_CLIENT_SEND_ORDER    0x104

#define BMS_ID_HOST_WRITE_RETURN	0x105
#define BMS_ID_HOST_ERROR			0x106
#define BMS_ID_HOST_OUT_TIMER			0x107

#define BMS_ID_HOST_QUANTITY_CHANGE	0x10f

#define BMS_ID_EEPROM_DATA_1 0x131
#define BMS_ID_EEPROM_DATA_2 0x132
#define BMS_ID_EEPROM_DATA_3 0x133
#define BMS_ID_EEPROM_DATA_4 0x134
#define BMS_ID_EEPROM_DATA_5 0x135
#define BMS_ID_EEPROM_DATA_6 0x136
#define BMS_ID_EEPROM_DATA_7 0x137
#define BMS_ID_EEPROM_DATA_8 0x138



#define BMS_ID_HOST_DATA_END	0x200
#define BMS_ID_HOST_DATA_1   0x201
#define BMS_ID_HOST_DATA_2   0x202
#define BMS_ID_HOST_DATA_3   0x203
#define BMS_ID_HOST_DATA_4   0x204
#define BMS_ID_HOST_DATA_5   0x205
#define BMS_ID_HOST_DATA_6   0x206
#define BMS_ID_HOST_DATA_7   0x207
#define BMS_ID_HOST_DATA_8   0x208

#define BMS_ID_SAMPLE_1_DATA_END	0x220
#define BMS_ID_SAMPLE_1_DATA_1   0x221
#define BMS_ID_SAMPLE_1_DATA_2   0x222
#define BMS_ID_SAMPLE_1_DATA_3   0x223
#define BMS_ID_SAMPLE_1_DATA_4   0x224
#define BMS_ID_SAMPLE_1_DATA_5   0x225
#define BMS_ID_SAMPLE_1_DATA_6   0x226
#define BMS_ID_SAMPLE_1_DATA_7   0x227
#define BMS_ID_SAMPLE_1_DATA_8   0x228

#define BMS_ID_SAMPLE_2_DATA_END		0x240
#define BMS_ID_SAMPLE_2_DATA_1   0x241
#define BMS_ID_SAMPLE_2_DATA_2   0x242
#define BMS_ID_SAMPLE_2_DATA_3   0x243
#define BMS_ID_SAMPLE_2_DATA_4   0x244
#define BMS_ID_SAMPLE_2_DATA_5   0x245
#define BMS_ID_SAMPLE_2_DATA_6   0x246
#define BMS_ID_SAMPLE_2_DATA_7   0x247
#define BMS_ID_SAMPLE_2_DATA_8   0x248

#define BMS_ID_SAMPLE_3_DATA_END		0x260
#define BMS_ID_SAMPLE_3_DATA_1   0x261
#define BMS_ID_SAMPLE_3_DATA_2   0x262
#define BMS_ID_SAMPLE_3_DATA_3   0x263
#define BMS_ID_SAMPLE_3_DATA_4   0x264
#define BMS_ID_SAMPLE_3_DATA_5   0x265
#define BMS_ID_SAMPLE_3_DATA_6   0x266
#define BMS_ID_SAMPLE_3_DATA_7   0x267
#define BMS_ID_SAMPLE_3_DATA_8   0x268

#define BMS_ID_SAMPLE_4_DATA_END		0x280
#define BMS_ID_SAMPLE_4_DATA_1   0x281
#define BMS_ID_SAMPLE_4_DATA_2   0x282
#define BMS_ID_SAMPLE_4_DATA_3   0x283
#define BMS_ID_SAMPLE_4_DATA_4   0x284
#define BMS_ID_SAMPLE_4_DATA_5   0x285
#define BMS_ID_SAMPLE_4_DATA_6   0x286
#define BMS_ID_SAMPLE_4_DATA_7   0x287
#define BMS_ID_SAMPLE_4_DATA_8   0x288

#define BMS_ID_SAMPLE_5_DATA_END		0x2a0
#define BMS_ID_SAMPLE_5_DATA_1   0x2a1
#define BMS_ID_SAMPLE_5_DATA_2   0x2a2
#define BMS_ID_SAMPLE_5_DATA_3   0x2a3
#define BMS_ID_SAMPLE_5_DATA_4   0x2a4
#define BMS_ID_SAMPLE_5_DATA_5   0x2a5
#define BMS_ID_SAMPLE_5_DATA_6   0x2a6
#define BMS_ID_SAMPLE_5_DATA_7   0x2a7
#define BMS_ID_SAMPLE_5_DATA_8   0x2a8

#define BMS_ID_SAMPLE_6_DATA_END		0x2c0
#define BMS_ID_SAMPLE_6_DATA_1   0x2c1
#define BMS_ID_SAMPLE_6_DATA_2   0x2c2
#define BMS_ID_SAMPLE_6_DATA_3   0x2c3
#define BMS_ID_SAMPLE_6_DATA_4   0x2c4
#define BMS_ID_SAMPLE_6_DATA_5   0x2c5
#define BMS_ID_SAMPLE_6_DATA_6   0x2c6
#define BMS_ID_SAMPLE_6_DATA_7   0x2c7
#define BMS_ID_SAMPLE_6_DATA_8   0x2c8

#define BMS_ID_SAMPLE_7_DATA_END		0x2e0
#define BMS_ID_SAMPLE_7_DATA_1   0x2e1
#define BMS_ID_SAMPLE_7_DATA_2   0x2e2
#define BMS_ID_SAMPLE_7_DATA_3   0x2e3
#define BMS_ID_SAMPLE_7_DATA_4   0x2e4
#define BMS_ID_SAMPLE_7_DATA_5   0x2e5
#define BMS_ID_SAMPLE_7_DATA_6   0x2e6
#define BMS_ID_SAMPLE_7_DATA_7   0x2e7
#define BMS_ID_SAMPLE_7_DATA_8   0x2e8

#define BMS_ID_CURRENT         		0x301
#define BMS_ID_SYSTEM_START_TIME	0x302
#define BMS_ID_QUANLITY				0x303

void BMS_CANSend(unsigned int ID,unsigned char *buffer);
void BMS_CAN_SendToSample_1(unsigned int CANEvent,unsigned char* dataBuffer);
void BMS_CAN_SendToSample_2(unsigned int CANEvent,unsigned char* dataBuffer);
void BMS_CAN_SendToSample_3(unsigned int CANEvent,unsigned char* dataBuffer);
void BMS_CAN_SendToSample_4(unsigned int CANEvent,unsigned char* dataBuffer);
void BMS_CAN_SendToSample_5(unsigned int CANEvent,unsigned char* dataBuffer);
void BMS_CAN_SendToSample_6(unsigned int CANEvent,unsigned char* dataBuffer);

#endif /* BMS_CAN_H_ */
