#include "CAN.h"
#include "MC9S08DZ60.h"
#include "System.h"
#include "Wave.h"
#include "configure.h"
#include "Event.h"

unsigned char  g_CAN_ReceiveBuffer[8];

extern unsigned char m_boardID;

void CAN_Init()
{
    CANCTL0_INITRQ = 1;    /* MSCAN in initialization mode */
        while (!(CANCTL1_INITAK))
            ;	        /* Wait for initialization mode acknowledge */
            
    CANCTL1_CANE = 1;      /* Enable MSCAN module */        
    
    CANCTL1_CLKSRC = CAN_CLK;    /* Clock source selection */    
    CANCTL1_LOOPB = 0;          /* Set to 1 for LoopBack Mode, 0 otherwise */
    CANCTL1_LISTEN = 0;         /* Not listen only mode */            
                                                                              
    CANBTR1_TSEG_10 = TIME_SEGM1;    /* Time Segment 1 */
    CANBTR1_TSEG_20 = TIME_SEGM2;    /* Time Segment 2 */                             
                                    
    CANBTR0_BRP = CAN_PRESC;    /* Baud rate prescaler */  
    CANBTR0_SJW = CAN_SYNJW;    /* Sinchronization jump width */       
                                              
    CANBTR1_SAMP = 0;      /* One sample per bit */	      
             
    CANIDAC_IDAM = 0x01;               /* Four 16-bit acceptance filters */                                                          
    
    CANIDAR0 = ACC_CODE_ID_HIGH;        /* 16 bit Filter 0 */
    CANIDMR0 = MASK_CODE_ST_ID_HIGH;   
    CANIDAR1 = ACC_CODE_ID_LOW;     
    CANIDMR1 = MASK_CODE_ST_ID_LOW;    
    
    CANIDAR2 = 0x00;                   /* 16 bit Filter 1 */
    CANIDMR2 = MASK_CODE_ST_ID_HIGH;   
    CANIDAR3 = 0x00;                   
    CANIDMR3 = MASK_CODE_ST_ID_LOW;    
    
    CANIDAR4 = 0x00;                   /* 16 bit Filter 2 */
    CANIDMR4 = MASK_CODE_ST_ID_HIGH;   
    CANIDAR5 = 0x00;                   
    CANIDMR5 = MASK_CODE_ST_ID_LOW;    
    
    CANIDAR6 = 0x00;                   /* 16 bit Filter 3 */
    CANIDMR6 = MASK_CODE_ST_ID_HIGH;   
    CANIDAR7 = 0x00;                   
    CANIDMR7 = MASK_CODE_ST_ID_LOW;    
    
    
    CANCTL0_INITRQ = 0;            /* Exit initialization mode request */
        while (CANCTL1_INITAK)
            ;               	/* Wait for normal mode */
                   
        while(!(CANCTL0_SYNCH))
            ;                   /* Wait for CAN synchronization */
   
    CANRFLG_RXF = 1;       /* Clear receiver flags */                                   
    CANRIER_RXFIE = 1;     /* Enable Full Receive Buffer interrupt */

}
void CAN_Send(word CAN_TXID,byte* sendBufferHead) {
    /* Transmission buffer */
    UINT8 TxBuffer = {0};
    UINT32 CAN_ID=((UINT32)(CAN_TXID) << 21);
    /* Index to data within the transmission buffer */ 
    UINT8 Index;
    
    if (!CANTFLG)              /* Retrun if Transmit Buffer is full */
        return ;

    CANTBSEL = CANTFLG;       /* Select lowest empty buffer */
    TxBuffer = CANTBSEL;		/* Backup selected buffer */
    
    
    *((UINT32 *) ((UINT32)(&CANTIDR0)))= CAN_ID;   /* Load Id to IDR Registers */
        
        /* Load data to Data Segment Registers */
        for (Index=0;Index<8;Index++) {
            *(&CANTDSR0 + Index) = sendBufferHead[Index];  
        }

    CANTDLR = 8;   /* Set Data Length Code */
    CANTTBPR = 0xf;    /* Set Priority */

    CANTFLG = TxBuffer;  /* Start transmission */
                
        while ( (CANTFLG & TxBuffer) != TxBuffer)
            ;  /* Wait for Transmission completion */
            
    return ;
}


interrupt VectorNumber_Vcanrx void   CANRx_ISR(void)
{    
    /* Length of received frame */
    UINT8 Index;    
	  //unsigned char  returnAddress[8];
	  unsigned char boardID;
	  /* Read and store each of the received data */

#ifdef FIRST_CUT
	boardID= CANRDSR0;


    if( 1==m_boardID) 
    {
      if(boardID!=SAMPLE_BOARD_1) 
      {
        
        CANRFLG_RXF = 1;   /* Clear reception flag */             

	  	  return;
      }
    } 
    else  if( 2==m_boardID) 
    {
      if(boardID!=SAMPLE_BOARD_2) 
      {
        CANRFLG_RXF = 1;   /* Clear reception flag */             

	  	  return;
      }
    }
   
#endif
	  for (Index=0; Index < 8; Index++)
  	   g_CAN_ReceiveBuffer[Index] = *(&CANRDSR0 + Index); 
    System_DoOrder(g_CAN_ReceiveBuffer);
    PWAVE(A,6);
    //while(1);
    CANRFLG_RXF = 1;   /* Clear reception flag */             
}
