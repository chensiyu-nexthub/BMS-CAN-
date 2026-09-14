#include "CAN.h"
#include "MC9S08DZ60.h"
#include "CCP.h"
extern struct STCCP g_stCCP;

UINT8  g_pubCANReceiveBuffer[8];


void CAN_vInit()
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
void CAN_vSend(word CAN_TXID,byte* sendBufferHead) {
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
	  //UINT8  returnAddress[8];
	  UINT8 boardID;
	  /* Read and store each of the received data */
	  
    g_pubCANReceiveBuffer[0]=CANRDSR0;
    g_pubCANReceiveBuffer[1]=CANRDSR1;
    g_pubCANReceiveBuffer[2]=CANRDSR2;
    g_pubCANReceiveBuffer[3]=CANRDSR3;
    g_pubCANReceiveBuffer[4]=CANRDSR4;
    g_pubCANReceiveBuffer[5]=CANRDSR5;
    g_pubCANReceiveBuffer[6]=CANRDSR6;
    g_pubCANReceiveBuffer[7]=CANRDSR7;
/*	  for (Index=0; Index < 8; Index++)
  	   g_pubCANReceiveBuffer[Index] = *(&CANRDSR0 + Index); */
    
    g_stCCP.pubCANReceive=g_pubCANReceiveBuffer;
            // USER CODE END
   	CCP_vCommand();
    CANRFLG_RXF = 1;   /* Clear reception flag */             
}
