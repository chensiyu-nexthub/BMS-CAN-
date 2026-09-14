#ifndef CAN_H
#define CAN_H
#include "MC9S08DZ60.h"

typedef unsigned char   UINT8;  /*unsigned 8 bit definition */
typedef unsigned short  UINT16; /*unsigned 16 bit definition*/
typedef unsigned long   UINT32; /*unsigned 32 bit definition*/

/* MSCAN module configuration constants */
/* Baud rate = CANCLK/(Prescaler * time quantas) */

#define CAN_CLK  0 /* Clock source: 0 for external oscillator, 1 for bus clock */

#define TIME_SEGM1  0xa   /* Time Segment 1 = 5 */
#define TIME_SEGM2  3   /* Time Segment 2 = 2 */
                        /* TSEG1 + TSEG2 + SYNCH_SEG = 8 time quantas */   
                        
/* Prescaler = CANCLK/(Baud rate * time quantas) = 4MHz/(500kHz * 8) = 1 */                                               
#define CAN_PRESC   1   /* Baud rate prescaler = 1 */ 
#define CAN_SYNJW   2   /* Sinchronization jump width = 2 clock cycles */ 

                                
/* Identifiers */
//#define CAN_TXID    0x401   /* Transmit message */
#define CAN_RXID    0x101   /* Receive message */

#define CAN_MASK    0x000   /* Four LSBs don't care for the filtering */
                
                                        
/* CAN ID definition: Standard Id formatted to be loaded in IDRx */
#define STD_ID ((UINT32)(CAN_TXID) << 21)

/* Acceptance Code Definitions */ 
#define ACC_CODE_ID (CAN_RXID << 5)   /* Standard Id */
#define ACC_CODE_ID_HIGH ((ACC_CODE_ID&0xFF00)>>8)
#define ACC_CODE_ID_LOW (ACC_CODE_ID&0x00FF)

/* Mask Code Definitions */
#define MASK_CODE_ST_ID (CAN_MASK << 5)
#define MASK_CODE_ST_ID_HIGH ((MASK_CODE_ST_ID&0xFF00)>>8)
#define MASK_CODE_ST_ID_LOW (MASK_CODE_ST_ID&0xFF)   
                   
/* Error Flags Definition */
#define NO_ERR 0x00
#define ERR_BUFFER_FULL 0x80 


void CAN_Init();
void CAN_Send(word CAN_TXID,byte* sendBufferHead);
void CAN_Receive(byte* returnAddress); 

/* CAN reception interupt routine */


#endif