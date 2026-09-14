//****************************************************************************
// @Module        MultiCAN Module (CAN)
// @Filename      CAN.c
// @Project       Dave.dav
//----------------------------------------------------------------------------
// @Controller    Infineon XC2287M-104F80
//
// @Compiler      Tasking Viper
//
// @Codegenerator 2.0
//
// @Description   This file contains functions that use the CAN module.
//
//----------------------------------------------------------------------------
// @Date          2010-6-3 10:01:43
//
//****************************************************************************



// USER CODE BEGIN (CAN_General,1)

// USER CODE END



//****************************************************************************
// @Project Includes
//****************************************************************************



#include "XC22xxMREGS.h"
#include "CAN.h"
#include "wave.h"

PF_Error_AddERROR m_pError_3;
PF_CANReceiveInterruptProcess m_pInterruptProcess_2;

unsigned int m_startTimer_3;
unsigned int m_timerCounter_3;

// USER CODE BEGIN (CAN_General,2)

// USER CODE END


//****************************************************************************
// @Macros
//****************************************************************************

// USER CODE BEGIN (CAN_General,3)

// USER CODE END


//****************************************************************************
// @Defines
//****************************************************************************

// USER CODE BEGIN (CAN_General,4)

// USER CODE END


//****************************************************************************
// @Typedefs
//****************************************************************************

// USER CODE BEGIN (CAN_General,5)

// USER CODE END


//****************************************************************************
// @Imported Global Variables
//****************************************************************************

// USER CODE BEGIN (CAN_General,6)

// USER CODE END


//****************************************************************************
// @Global Variables
//****************************************************************************


// USER CODE BEGIN (CAN_General,7)

// USER CODE END


//****************************************************************************
// @External Prototypes
//****************************************************************************

// USER CODE BEGIN (CAN_General,8)

// USER CODE END


//****************************************************************************
// @Prototypes Of Local Functions
//****************************************************************************

// USER CODE BEGIN (CAN_General,9)

// USER CODE END


//****************************************************************************
// @Function      void CAN_vInit(void)
//
//----------------------------------------------------------------------------
// @Description   This is the initialization function of the CAN function
//                library. It is assumed that the SFRs used by this library
//                are in reset state.
//
//----------------------------------------------------------------------------
// @Returnvalue   None
//
//----------------------------------------------------------------------------
// @Parameters    None
//
//----------------------------------------------------------------------------
// @Date          2010-6-3
//
//****************************************************************************
static void CAN_vInit(void);

// USER CODE BEGIN (Init,1)

// USER CODE END

void CAN_Init(PF_Error_AddERROR pfError,PF_CANReceiveInterruptProcess pfInterrupt,unsigned int timerCounter)
{

	m_pError_3=pfError;
	m_pInterruptProcess_2=pfInterrupt;
	m_timerCounter_3=timerCounter;
	CAN_vInit();
}

static void CAN_vInit(void)
{

  // USER CODE BEGIN (Init,2)

  // USER CODE END

  ///  -----------------------------------------------------------------------
  ///  Configuration of Kernel State Configuration Register:
  ///  -----------------------------------------------------------------------
  ///  - Enable the CAN module(MODEN)
  ///  - Enable Bit Protection for MODEN

    MCAN_KSCCFG  =  0x0003;      // load Kernel State Configuration Register

    _nop();  // one cycle delay

    _nop();  // one cycle delay


  ///  -----------------------------------------------------------------------
  ///  Configuration of the Module Clock:
  ///  -----------------------------------------------------------------------
  ///  - the CAN module clock = 20.00 MHz
  ///  - Normal divider mode selected

    CAN_FDRL     =  0x43FE;      // load Fractional Divider Register

  ///  -----------------------------------------------------------------------
  ///  Panel Control
  ///  -----------------------------------------------------------------------
  ///  - wait until Panel has finished the initialisation

    while(CAN_PANCTRL & CAN_PANCTR_BUSY){ // wait until Panel has finished
                                          // the initialisation
      }

  ///  -----------------------------------------------------------------------
  ///  Configuration of CAN Node 0:
  ///  -----------------------------------------------------------------------

  ///  General Configuration of the Node 0:
  ///  - set INIT and CCE

    CAN_NCR0     =  0x0041;      // load NODE 0 control register[15-0]

  ///  - load NODE 0 interrupt pointer register

    CAN_NIPR0    =  0x0000;      // load NIPR0_LECINP, ALINP, CFCINP and TRINP

  ///  Configuration of the used CAN Port Pins:
  ///  - Loop-back mode is disabled
  ///  - P2.0 is used for CAN0 Receive input(RXDC0C)
  ///  - P2.1 is used for CAN0 Transmit output(TXDC0C)

  P2_IOCR01 = 0x0090;    //set direction register
    CAN_NPCR0    =  0x0002;      // load node0 port control register


  ///  Configuration of the Node 0 Baud Rate:
  ///  - required baud rate = 500.000 kbaud
  ///  - real baud rate     = 500.000 kbaud
  ///  - sample point       = 60.00 %
  ///  - there are 5 time quanta before sample point
  ///  - there are 4 time quanta after sample point
  ///  - the (re)synchronization jump width is 2 time quanta

    CAN_NBTR0L   =  0x3443;      // load NBTR0_DIV8, TSEG2, TSEG1, SJW and BRP

  ///  Configuration of the Node 0 Error Counter:
  ///  - the error warning threshold value (warning level) is 96

    CAN_NECNT0H = 0x0060;        // load NECNT0_EWRNLVL register
    CAN_NECNT0L = 0x0000;

  ///  Configuration of the Frame Counter:
  ///  - Frame Counter Mode: the counter is incremented upon the reception
  ///    and transmission of frames
  ///  - frame counter: 0x0000

    CAN_NFCR0H = 0x0000;         // load NFCR0_CFCOV, CFCIE, CFMOD, CFSEL
    CAN_NFCR0L = 0x0000;         // load NFCR0_CFC

  ///  -----------------------------------------------------------------------
  ///  Configuration of CAN Node 1:
  ///  -----------------------------------------------------------------------

  ///  General Configuration of the Node 1:
  ///  - set INIT and CCE

    CAN_NCR1     =  0x0041;      // load NODE 1 control register[15-0]

  ///  - load NODE 1 interrupt pointer register

    CAN_NIPR1    =  0x0000;      // load NIPR1_LECINP, ALINP, CFCINP and TRINP

  ///  Configuration of the used CAN Port Pins:
  ///  - Loop-back mode is disabled
  ///  - P2.7 is used for CAN1 Receive input(RXDC1C)
  ///  - P2.2 is used for CAN1 Transmit output(TXDC1B)

  P2_IOCR02 = 0x0090;    //set direction register
    CAN_NPCR1    =  0x0002;      // load node1 port control register


  ///  Configuration of the Node 1 Baud Rate:
  ///  - required baud rate = 500.000 kbaud
  ///  - real baud rate     = 500.000 kbaud
  ///  - sample point       = 60.00 %
  ///  - there are 5 time quanta before sample point
  ///  - there are 4 time quanta after sample point
  ///  - the (re)synchronization jump width is 2 time quanta

    CAN_NBTR1L   =  0x3443;      // load NBTR1_DIV8, TSEG2, TSEG1, SJW and BRP

  ///  Configuration of the Node 1 Error Counter:
  ///  - the error warning threshold value (warning level) is 96

    CAN_NECNT1H = 0x0060;        // load NECNT1_EWRNLVL register
    CAN_NECNT1L = 0x0000;

  ///  Configuration of the Frame Counter:
  ///  - Frame Counter Mode: the counter is incremented upon the reception
  ///    and transmission of frames
  ///  - frame counter: 0x0000

    CAN_NFCR1H = 0x0000;         // load NFCR1_CFCOV, CFCIE, CFMOD, CFSEL
    CAN_NFCR1L = 0x0000;         // load NFCR1_CFC

  ///  -----------------------------------------------------------------------
  ///  Configuration of CAN Node 2:
  ///  -----------------------------------------------------------------------

  ///  General Configuration of the Node 2:
  ///  - set INIT and CCE

    CAN_NCR2     =  0x0041;      // load NODE 2 control register[15-0]

  ///  -----------------------------------------------------------------------
  ///  Configuration of CAN Node 3:
  ///  -----------------------------------------------------------------------

  ///  General Configuration of the Node 3:
  ///  - set INIT and CCE

    CAN_NCR3     =  0x0041;      // load NODE 3 control register[15-0]

  ///  -----------------------------------------------------------------------
  ///  Configuration of CAN Node 4:
  ///  -----------------------------------------------------------------------

  ///  General Configuration of the Node 4:
  ///  - set INIT and CCE

    CAN_NCR4     =  0x0041;      // load NODE 4 control register[15-0]

  ///  -----------------------------------------------------------------------
  ///  Configuration of CAN Node 5:
  ///  -----------------------------------------------------------------------

  ///  General Configuration of the Node 5:
  ///  - set INIT and CCE

    CAN_NCR5     =  0x0041;      // load NODE 5 control register[15-0]

  ///  -----------------------------------------------------------------------
  ///  Configuration of the CAN Message Object List Structure:
  ///  -----------------------------------------------------------------------

  ///  Allocate MOs for list 1:

    SetListCommand(0x014F,0x0002);  // MO79 for list 1 (Node 0)

    SetListCommand(0x014E,0x0002);  // MO78 for list 1 (Node 0)

    SetListCommand(0x014D,0x0002);  // MO77 for list 1 (Node 0)

    SetListCommand(0x014C,0x0002);  // MO76 for list 1 (Node 0)

    SetListCommand(0x014B,0x0002);  // MO75 for list 1 (Node 0)

    SetListCommand(0x014A,0x0002);  // MO74 for list 1 (Node 0)

    SetListCommand(0x0149,0x0002);  // MO73 for list 1 (Node 0)

    SetListCommand(0x0148,0x0002);  // MO72 for list 1 (Node 0)

    SetListCommand(0x0147,0x0002);  // MO71 for list 1 (Node 0)

    SetListCommand(0x0145,0x0002);  // MO69 for list 1 (Node 0)

    SetListCommand(0x0144,0x0002);  // MO68 for list 1 (Node 0)

    SetListCommand(0x0143,0x0002);  // MO67 for list 1 (Node 0)

    SetListCommand(0x0142,0x0002);  // MO66 for list 1 (Node 0)

    SetListCommand(0x0141,0x0002);  // MO65 for list 1 (Node 0)

    SetListCommand(0x0140,0x0002);  // MO64 for list 1 (Node 0)

    SetListCommand(0x013F,0x0002);  // MO63 for list 1 (Node 0)

    SetListCommand(0x013E,0x0002);  // MO62 for list 1 (Node 0)

    SetListCommand(0x013D,0x0002);  // MO61 for list 1 (Node 0)

    SetListCommand(0x013B,0x0002);  // MO59 for list 1 (Node 0)

    SetListCommand(0x013A,0x0002);  // MO58 for list 1 (Node 0)

    SetListCommand(0x0139,0x0002);  // MO57 for list 1 (Node 0)

    SetListCommand(0x0138,0x0002);  // MO56 for list 1 (Node 0)

    SetListCommand(0x0137,0x0002);  // MO55 for list 1 (Node 0)

    SetListCommand(0x0136,0x0002);  // MO54 for list 1 (Node 0)

    SetListCommand(0x0135,0x0002);  // MO53 for list 1 (Node 0)

    SetListCommand(0x0134,0x0002);  // MO52 for list 1 (Node 0)

    SetListCommand(0x0133,0x0002);  // MO51 for list 1 (Node 0)

    SetListCommand(0x0131,0x0002);  // MO49 for list 1 (Node 0)

    SetListCommand(0x0130,0x0002);  // MO48 for list 1 (Node 0)

    SetListCommand(0x012F,0x0002);  // MO47 for list 1 (Node 0)

    SetListCommand(0x012E,0x0002);  // MO46 for list 1 (Node 0)

    SetListCommand(0x012D,0x0002);  // MO45 for list 1 (Node 0)

    SetListCommand(0x012C,0x0002);  // MO44 for list 1 (Node 0)

    SetListCommand(0x012B,0x0002);  // MO43 for list 1 (Node 0)

    SetListCommand(0x012A,0x0002);  // MO42 for list 1 (Node 0)

    SetListCommand(0x0129,0x0002);  // MO41 for list 1 (Node 0)

    SetListCommand(0x0127,0x0002);  // MO39 for list 1 (Node 0)

    SetListCommand(0x0126,0x0002);  // MO38 for list 1 (Node 0)

    SetListCommand(0x0125,0x0002);  // MO37 for list 1 (Node 0)

    SetListCommand(0x0124,0x0002);  // MO36 for list 1 (Node 0)

    SetListCommand(0x0123,0x0002);  // MO35 for list 1 (Node 0)

    SetListCommand(0x0122,0x0002);  // MO34 for list 1 (Node 0)

    SetListCommand(0x0121,0x0002);  // MO33 for list 1 (Node 0)

    SetListCommand(0x0120,0x0002);  // MO32 for list 1 (Node 0)

    SetListCommand(0x011F,0x0002);  // MO31 for list 1 (Node 0)

    SetListCommand(0x011D,0x0002);  // MO29 for list 1 (Node 0)

    SetListCommand(0x011C,0x0002);  // MO28 for list 1 (Node 0)

    SetListCommand(0x011B,0x0002);  // MO27 for list 1 (Node 0)

    SetListCommand(0x011A,0x0002);  // MO26 for list 1 (Node 0)

    SetListCommand(0x0119,0x0002);  // MO25 for list 1 (Node 0)

    SetListCommand(0x0118,0x0002);  // MO24 for list 1 (Node 0)

    SetListCommand(0x0117,0x0002);  // MO23 for list 1 (Node 0)

    SetListCommand(0x0116,0x0002);  // MO22 for list 1 (Node 0)

    SetListCommand(0x0115,0x0002);  // MO21 for list 1 (Node 0)

    SetListCommand(0x0113,0x0002);  // MO19 for list 1 (Node 0)

    SetListCommand(0x0112,0x0002);  // MO18 for list 1 (Node 0)

    SetListCommand(0x0111,0x0002);  // MO17 for list 1 (Node 0)

    SetListCommand(0x0110,0x0002);  // MO16 for list 1 (Node 0)

    SetListCommand(0x010F,0x0002);  // MO15 for list 1 (Node 0)

    SetListCommand(0x010E,0x0002);  // MO14 for list 1 (Node 0)

    SetListCommand(0x010D,0x0002);  // MO13 for list 1 (Node 0)

    SetListCommand(0x010C,0x0002);  // MO12 for list 1 (Node 0)

    SetListCommand(0x010B,0x0002);  // MO11 for list 1 (Node 0)

    SetListCommand(0x0103,0x0002);  // MO3 for list 1 (Node 0)

    SetListCommand(0x0102,0x0002);  // MO2 for list 1 (Node 0)

    SetListCommand(0x0101,0x0002);  // MO1 for list 1 (Node 0)

    SetListCommand(0x0100,0x0002);  // MO0 for list 1 (Node 0)

  ///  Allocate MOs for list 2:

    SetListCommand(0x0246,0x0002);  // MO70 for list 2 (Node 1)

    SetListCommand(0x023C,0x0002);  // MO60 for list 2 (Node 1)

    SetListCommand(0x0232,0x0002);  // MO50 for list 2 (Node 1)

    SetListCommand(0x0228,0x0002);  // MO40 for list 2 (Node 1)

    SetListCommand(0x021E,0x0002);  // MO30 for list 2 (Node 1)

    SetListCommand(0x0214,0x0002);  // MO20 for list 2 (Node 1)

    SetListCommand(0x020A,0x0002);  // MO10 for list 2 (Node 1)

    SetListCommand(0x0209,0x0002);  // MO9 for list 2 (Node 1)

    SetListCommand(0x0208,0x0002);  // MO8 for list 2 (Node 1)

    SetListCommand(0x0207,0x0002);  // MO7 for list 2 (Node 1)

    SetListCommand(0x0206,0x0002);  // MO6 for list 2 (Node 1)

    SetListCommand(0x0205,0x0002);  // MO5 for list 2 (Node 1)

    SetListCommand(0x0204,0x0002);  // MO4 for list 2 (Node 1)

  ///  -----------------------------------------------------------------------
  ///  Configuration of the CAN Message Objects 0 - 254:
  ///  -----------------------------------------------------------------------
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 0:
  ///  -----------------------------------------------------------------------

  ///  General Configuration of the Message Object 0 :
  ///  - message object 0 is valid
  ///  - message object is used as receive object
  ///  - this message object is assigned to list 1 (node 0)

    CAN_MOCTR0H = 0x00A0;        // load MO0 control register high
    CAN_MOCTR0L = 0x0000;        // load MO0 control register low

  ///  Configuration of Message Object 0 Arbitration:
  ///  - priority class 1; transmit acceptance filtering is based on the list
  ///    order
  ///  - standard 11-bit identifier
  ///  - identifier 11-bit:      0x100

    CAN_MOAR0H = 0x4000+(0x100<<2);         // load MO0 arbitration register high
    CAN_MOAR0L = 0x0000;         // load MO0 arbitration register low

  ///  Configuration of Message Object 0 acceptance mask:
  ///  - only accept receive frames with matching IDE bit
  ///  - acceptance mask 11-bit: 0x7FF

    CAN_MOAMR0H = 0x3FFF;        // load MO0 acceptance mask register high
    CAN_MOAMR0L = 0xFFFF;        // load MO0 acceptance mask register low

  ///  Configuration of Message Object 0 interrupt pointer:
  ///  - use message pending register 0 bit position 0
  ///  - receive interrupt node pointer: MultiCAN SRN 0

    CAN_MOIPR0H = 0x0000;        // load MO0 interrupt pointer register high
    CAN_MOIPR0L = 0x0000;        // load MO0 interrupt pointer register low

  ///  Configuration of Message Object 0 FIFO/Gateway pointer:

    CAN_MOFGPR0H = 0x0000;       // load MO0 FIFO/gateway pointer register
                                 // high
    CAN_MOFGPR0L = 0x0000;       // load MO0 FIFO/gateway pointer register low


  ///  Configuration of Message Object 0 Function control:
  ///  - this object is a STANDARD MESSAGE OBJECT
  ///  - 8 valid data bytes
  ///  - enable receive interrupt; bit RXPND is set after successful
  ///    reception of a frame

    CAN_MOFCR0H = 0x0801;        // load MO0 function control register high
    CAN_MOFCR0L = 0x0000;        // load MO0 function control register low

  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 1:
  ///  -----------------------------------------------------------------------

  ///  General Configuration of the Message Object 1 :
  ///  - message object 1 is valid
  ///  - message object is used as transmit object
  ///  - this message object is assigned to list 1 (node 0)

    CAN_MOCTR1H = 0x0EA8;        // load MO1 control register high
    CAN_MOCTR1L = 0x0000;        // load MO1 control register low

  ///  Configuration of Message Object 1 Arbitration:
  ///  - priority class 1; transmit acceptance filtering is based on the list
  ///    order
  ///  - standard 11-bit identifier
  ///  - identifier 11-bit:      0x001

    CAN_MOAR1H = 0x4000+(0x101<<2);         // load MO1 arbitration register high
    CAN_MOAR1L = 0x0000;         // load MO1 arbitration register low

  ///  Configuration of Message Object 1 Data:

    CAN_MODATA1HH = 0x0000;      // load MO1 Data Bytes(DB7 & DB6)
    CAN_MODATA1HL = 0x0000;      // load MO1 Data Bytes(DB5 & DB4)
    CAN_MODATA1LH = 0x0000;      // load MO1 Data Bytes(DB3 & DB2)
    CAN_MODATA1LL = 0x0000;      // load MO1 Data Bytes(DB1 & DB0)

  ///  Configuration of Message Object 1 acceptance mask:
  ///  - only accept receive frames with matching IDE bit
  ///  - acceptance mask 11-bit: 0x7FF

    CAN_MOAMR1H = 0x3FFF;        // load MO1 acceptance mask register high
    CAN_MOAMR1L = 0xFFFF;        // load MO1 acceptance mask register low

  ///  Configuration of Message Object 1 interrupt pointer:
  ///  - use message pending register 0 bit position 1
  ///  - transmit interrupt node pointer: MultiCAN SRN 0

    CAN_MOIPR1H = 0x0000;        // load MO1 interrupt pointer register high
    CAN_MOIPR1L = 0x0100;        // load MO1 interrupt pointer register low

  ///  Configuration of Message Object 1 FIFO/Gateway pointer:

    CAN_MOFGPR1H = 0x0000;       // load MO1 FIFO/gateway pointer register
                                 // high
    CAN_MOFGPR1L = 0x0000;       // load MO1 FIFO/gateway pointer register low


  ///  Configuration of Message Object 1 Function control:
  ///  - this object is a STANDARD MESSAGE OBJECT
  ///  - 8 valid data bytes
  ///  - enable transmit interrupt; bit TXPND is set after successful
  ///    transmission of a frame

    CAN_MOFCR1H = 0x0802;        // load MO1 function control register high
    CAN_MOFCR1L = 0x0000;        // load MO1 function control register low

  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 2:
  ///  -----------------------------------------------------------------------

  ///  General Configuration of the Message Object 2 :
  ///  - message object 2 is valid
  ///  - message object is used as receive object
  ///  - this message object is assigned to list 1 (node 0)

    CAN_MOCTR2H = 0x00A0;        // load MO2 control register high
    CAN_MOCTR2L = 0x0000;        // load MO2 control register low

  ///  Configuration of Message Object 2 Arbitration:
  ///  - priority class 1; transmit acceptance filtering is based on the list
  ///    order
  ///  - standard 11-bit identifier
  ///  - identifier 11-bit:      0x002

    CAN_MOAR2H = 0x4008;         // load MO2 arbitration register high
    CAN_MOAR2L = 0x0000;         // load MO2 arbitration register low

  ///  Configuration of Message Object 2 acceptance mask:
  ///  - only accept receive frames with matching IDE bit
  ///  - acceptance mask 11-bit: 0x7FF

    CAN_MOAMR2H = 0x3FFF;        // load MO2 acceptance mask register high
    CAN_MOAMR2L = 0xFFFF;        // load MO2 acceptance mask register low

  ///  Configuration of Message Object 2 interrupt pointer:
  ///  - use message pending register 0 bit position 2
  ///  - receive interrupt node pointer: MultiCAN SRN 1

    CAN_MOIPR2H = 0x0000;        // load MO2 interrupt pointer register high
    CAN_MOIPR2L = 0x0201;        // load MO2 interrupt pointer register low

  ///  Configuration of Message Object 2 FIFO/Gateway pointer:

    CAN_MOFGPR2H = 0x0000;       // load MO2 FIFO/gateway pointer register
                                 // high
    CAN_MOFGPR2L = 0x0000;       // load MO2 FIFO/gateway pointer register low


  ///  Configuration of Message Object 2 Function control:
  ///  - this object is a STANDARD MESSAGE OBJECT
  ///  - 8 valid data bytes
  ///  - enable receive interrupt; bit RXPND is set after successful
  ///    reception of a frame

    CAN_MOFCR2H = 0x0801;        // load MO2 function control register high
    CAN_MOFCR2L = 0x0000;        // load MO2 function control register low

  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 3:
  ///  -----------------------------------------------------------------------

  ///  General Configuration of the Message Object 3 :
  ///  - message object 3 is valid
  ///  - message object is used as transmit object
  ///  - this message object is assigned to list 1 (node 0)

    CAN_MOCTR3H = 0x0EA8;        // load MO3 control register high
    CAN_MOCTR3L = 0x0000;        // load MO3 control register low

  ///  Configuration of Message Object 3 Arbitration:
  ///  - priority class 1; transmit acceptance filtering is based on the list
  ///    order
  ///  - standard 11-bit identifier
  ///  - identifier 11-bit:      0x003

    CAN_MOAR3H = 0x400C;         // load MO3 arbitration register high
    CAN_MOAR3L = 0x0000;         // load MO3 arbitration register low

  ///  Configuration of Message Object 3 Data:

    CAN_MODATA3HH = 0x0000;      // load MO3 Data Bytes(DB7 & DB6)
    CAN_MODATA3HL = 0x0000;      // load MO3 Data Bytes(DB5 & DB4)
    CAN_MODATA3LH = 0x0000;      // load MO3 Data Bytes(DB3 & DB2)
    CAN_MODATA3LL = 0x0000;      // load MO3 Data Bytes(DB1 & DB0)

  ///  Configuration of Message Object 3 acceptance mask:
  ///  - only accept receive frames with matching IDE bit
  ///  - acceptance mask 11-bit: 0x7FF

    CAN_MOAMR3H = 0x3FFF;        // load MO3 acceptance mask register high
    CAN_MOAMR3L = 0xFFFF;        // load MO3 acceptance mask register low

  ///  Configuration of Message Object 3 interrupt pointer:
  ///  - use message pending register 0 bit position 3
  ///  - transmit interrupt node pointer: MultiCAN SRN 1

    CAN_MOIPR3H = 0x0000;        // load MO3 interrupt pointer register high
    CAN_MOIPR3L = 0x0310;        // load MO3 interrupt pointer register low

  ///  Configuration of Message Object 3 FIFO/Gateway pointer:

    CAN_MOFGPR3H = 0x0000;       // load MO3 FIFO/gateway pointer register
                                 // high
    CAN_MOFGPR3L = 0x0000;       // load MO3 FIFO/gateway pointer register low


  ///  Configuration of Message Object 3 Function control:
  ///  - this object is a STANDARD MESSAGE OBJECT
  ///  - 8 valid data bytes
  ///  - enable transmit interrupt; bit TXPND is set after successful
  ///    transmission of a frame

    CAN_MOFCR3H = 0x0802;        // load MO3 function control register high
    CAN_MOFCR3L = 0x0000;        // load MO3 function control register low

  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 4:
  ///  -----------------------------------------------------------------------
  ///  - message object 4 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 5:
  ///  -----------------------------------------------------------------------
  ///  - message object 5 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 6:
  ///  -----------------------------------------------------------------------
  ///  - message object 6 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 7:
  ///  -----------------------------------------------------------------------
  ///  - message object 7 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 8:
  ///  -----------------------------------------------------------------------
  ///  - message object 8 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 9:
  ///  -----------------------------------------------------------------------
  ///  - message object 9 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 10:
  ///  -----------------------------------------------------------------------
  ///  - message object 10 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 11:
  ///  -----------------------------------------------------------------------

  ///  General Configuration of the Message Object 11 :
  ///  - message object 11 is valid
  ///  - message object is used as receive object
  ///  - this message object is assigned to list 1 (node 0)

    CAN_MOCTR11H = 0x00A0;       // load MO11 control register high
    CAN_MOCTR11L = 0x0000;       // load MO11 control register low

  ///  Configuration of Message Object 11 Arbitration:
  ///  - priority class 1; transmit acceptance filtering is based on the list
  ///    order
  ///  - standard 11-bit identifier
  ///  - identifier 11-bit:      0x011

    CAN_MOAR11H = 0x4044;        // load MO11 arbitration register high
    CAN_MOAR11L = 0x0000;        // load MO11 arbitration register low

  ///  Configuration of Message Object 11 acceptance mask:
  ///  - only accept receive frames with matching IDE bit
  ///  - acceptance mask 11-bit: 0x7FF

    CAN_MOAMR11H = 0x3FFF;       // load MO11 acceptance mask register high
    CAN_MOAMR11L = 0xFFFF;       // load MO11 acceptance mask register low

  ///  Configuration of Message Object 11 interrupt pointer:
  ///  - use message pending register 0 bit position 11

    CAN_MOIPR11H = 0x0000;       // load MO11 interrupt pointer register high
    CAN_MOIPR11L = 0x0B00;       // load MO11 interrupt pointer register low

  ///  Configuration of Message Object 11 FIFO/Gateway pointer:

    CAN_MOFGPR11H = 0x0000;      // load MO11 FIFO/gateway pointer register
                                 // high
    CAN_MOFGPR11L = 0x0000;      // load MO11 FIFO/gateway pointer register
                                 // low


  ///  Configuration of Message Object 11 Function control:
  ///  - this object is a STANDARD MESSAGE OBJECT
  ///  - 8 valid data bytes

    CAN_MOFCR11H = 0x0800;       // load MO11 function control register high
    CAN_MOFCR11L = 0x0000;       // load MO11 function control register low

  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 12:
  ///  -----------------------------------------------------------------------

  ///  General Configuration of the Message Object 12 :
  ///  - message object 12 is valid
  ///  - message object is used as receive object
  ///  - this message object is assigned to list 1 (node 0)

    CAN_MOCTR12H = 0x00A0;       // load MO12 control register high
    CAN_MOCTR12L = 0x0000;       // load MO12 control register low

  ///  Configuration of Message Object 12 Arbitration:
  ///  - priority class 1; transmit acceptance filtering is based on the list
  ///    order
  ///  - standard 11-bit identifier
  ///  - identifier 11-bit:      0x012

    CAN_MOAR12H = 0x4048;        // load MO12 arbitration register high
    CAN_MOAR12L = 0x0000;        // load MO12 arbitration register low

  ///  Configuration of Message Object 12 acceptance mask:
  ///  - only accept receive frames with matching IDE bit
  ///  - acceptance mask 11-bit: 0x7FF

    CAN_MOAMR12H = 0x3FFF;       // load MO12 acceptance mask register high
    CAN_MOAMR12L = 0xFFFF;       // load MO12 acceptance mask register low

  ///  Configuration of Message Object 12 interrupt pointer:
  ///  - use message pending register 0 bit position 12

    CAN_MOIPR12H = 0x0000;       // load MO12 interrupt pointer register high
    CAN_MOIPR12L = 0x0C00;       // load MO12 interrupt pointer register low

  ///  Configuration of Message Object 12 FIFO/Gateway pointer:

    CAN_MOFGPR12H = 0x0000;      // load MO12 FIFO/gateway pointer register
                                 // high
    CAN_MOFGPR12L = 0x0000;      // load MO12 FIFO/gateway pointer register
                                 // low


  ///  Configuration of Message Object 12 Function control:
  ///  - this object is a STANDARD MESSAGE OBJECT
  ///  - 8 valid data bytes

    CAN_MOFCR12H = 0x0800;       // load MO12 function control register high
    CAN_MOFCR12L = 0x0000;       // load MO12 function control register low

  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 13:
  ///  -----------------------------------------------------------------------

  ///  General Configuration of the Message Object 13 :
  ///  - message object 13 is valid
  ///  - message object is used as receive object
  ///  - this message object is assigned to list 1 (node 0)

    CAN_MOCTR13H = 0x00A0;       // load MO13 control register high
    CAN_MOCTR13L = 0x0000;       // load MO13 control register low

  ///  Configuration of Message Object 13 Arbitration:
  ///  - priority class 1; transmit acceptance filtering is based on the list
  ///    order
  ///  - standard 11-bit identifier
  ///  - identifier 11-bit:      0x013

    CAN_MOAR13H = 0x404C;        // load MO13 arbitration register high
    CAN_MOAR13L = 0x0000;        // load MO13 arbitration register low

  ///  Configuration of Message Object 13 acceptance mask:
  ///  - only accept receive frames with matching IDE bit
  ///  - acceptance mask 11-bit: 0x7FF

    CAN_MOAMR13H = 0x3FFF;       // load MO13 acceptance mask register high
    CAN_MOAMR13L = 0xFFFF;       // load MO13 acceptance mask register low

  ///  Configuration of Message Object 13 interrupt pointer:
  ///  - use message pending register 0 bit position 13

    CAN_MOIPR13H = 0x0000;       // load MO13 interrupt pointer register high
    CAN_MOIPR13L = 0x0D00;       // load MO13 interrupt pointer register low

  ///  Configuration of Message Object 13 FIFO/Gateway pointer:

    CAN_MOFGPR13H = 0x0000;      // load MO13 FIFO/gateway pointer register
                                 // high
    CAN_MOFGPR13L = 0x0000;      // load MO13 FIFO/gateway pointer register
                                 // low


  ///  Configuration of Message Object 13 Function control:
  ///  - this object is a STANDARD MESSAGE OBJECT
  ///  - 8 valid data bytes

    CAN_MOFCR13H = 0x0800;       // load MO13 function control register high
    CAN_MOFCR13L = 0x0000;       // load MO13 function control register low

  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 14:
  ///  -----------------------------------------------------------------------

  ///  General Configuration of the Message Object 14 :
  ///  - message object 14 is valid
  ///  - message object is used as receive object
  ///  - this message object is assigned to list 1 (node 0)

    CAN_MOCTR14H = 0x00A0;       // load MO14 control register high
    CAN_MOCTR14L = 0x0000;       // load MO14 control register low

  ///  Configuration of Message Object 14 Arbitration:
  ///  - priority class 1; transmit acceptance filtering is based on the list
  ///    order
  ///  - standard 11-bit identifier
  ///  - identifier 11-bit:      0x014

    CAN_MOAR14H = 0x4050;        // load MO14 arbitration register high
    CAN_MOAR14L = 0x0000;        // load MO14 arbitration register low

  ///  Configuration of Message Object 14 acceptance mask:
  ///  - only accept receive frames with matching IDE bit
  ///  - acceptance mask 11-bit: 0x7FF

    CAN_MOAMR14H = 0x3FFF;       // load MO14 acceptance mask register high
    CAN_MOAMR14L = 0xFFFF;       // load MO14 acceptance mask register low

  ///  Configuration of Message Object 14 interrupt pointer:
  ///  - use message pending register 0 bit position 14

    CAN_MOIPR14H = 0x0000;       // load MO14 interrupt pointer register high
    CAN_MOIPR14L = 0x0E00;       // load MO14 interrupt pointer register low

  ///  Configuration of Message Object 14 FIFO/Gateway pointer:

    CAN_MOFGPR14H = 0x0000;      // load MO14 FIFO/gateway pointer register
                                 // high
    CAN_MOFGPR14L = 0x0000;      // load MO14 FIFO/gateway pointer register
                                 // low


  ///  Configuration of Message Object 14 Function control:
  ///  - this object is a STANDARD MESSAGE OBJECT
  ///  - 8 valid data bytes

    CAN_MOFCR14H = 0x0800;       // load MO14 function control register high
    CAN_MOFCR14L = 0x0000;       // load MO14 function control register low

  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 15:
  ///  -----------------------------------------------------------------------

  ///  General Configuration of the Message Object 15 :
  ///  - message object 15 is valid
  ///  - message object is used as receive object
  ///  - this message object is assigned to list 1 (node 0)

    CAN_MOCTR15H = 0x00A0;       // load MO15 control register high
    CAN_MOCTR15L = 0x0000;       // load MO15 control register low

  ///  Configuration of Message Object 15 Arbitration:
  ///  - priority class 1; transmit acceptance filtering is based on the list
  ///    order
  ///  - standard 11-bit identifier
  ///  - identifier 11-bit:      0x015

    CAN_MOAR15H = 0x4054;        // load MO15 arbitration register high
    CAN_MOAR15L = 0x0000;        // load MO15 arbitration register low

  ///  Configuration of Message Object 15 acceptance mask:
  ///  - only accept receive frames with matching IDE bit
  ///  - acceptance mask 11-bit: 0x7FF

    CAN_MOAMR15H = 0x3FFF;       // load MO15 acceptance mask register high
    CAN_MOAMR15L = 0xFFFF;       // load MO15 acceptance mask register low

  ///  Configuration of Message Object 15 interrupt pointer:
  ///  - use message pending register 0 bit position 15

    CAN_MOIPR15H = 0x0000;       // load MO15 interrupt pointer register high
    CAN_MOIPR15L = 0x0F00;       // load MO15 interrupt pointer register low

  ///  Configuration of Message Object 15 FIFO/Gateway pointer:

    CAN_MOFGPR15H = 0x0000;      // load MO15 FIFO/gateway pointer register
                                 // high
    CAN_MOFGPR15L = 0x0000;      // load MO15 FIFO/gateway pointer register
                                 // low


  ///  Configuration of Message Object 15 Function control:
  ///  - this object is a STANDARD MESSAGE OBJECT
  ///  - 8 valid data bytes

    CAN_MOFCR15H = 0x0800;       // load MO15 function control register high
    CAN_MOFCR15L = 0x0000;       // load MO15 function control register low

  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 16:
  ///  -----------------------------------------------------------------------

  ///  General Configuration of the Message Object 16 :
  ///  - message object 16 is valid
  ///  - message object is used as receive object
  ///  - this message object is assigned to list 1 (node 0)

    CAN_MOCTR16H = 0x00A0;       // load MO16 control register high
    CAN_MOCTR16L = 0x0000;       // load MO16 control register low

  ///  Configuration of Message Object 16 Arbitration:
  ///  - priority class 1; transmit acceptance filtering is based on the list
  ///    order
  ///  - standard 11-bit identifier
  ///  - identifier 11-bit:      0x016

    CAN_MOAR16H = 0x4058;        // load MO16 arbitration register high
    CAN_MOAR16L = 0x0000;        // load MO16 arbitration register low

  ///  Configuration of Message Object 16 acceptance mask:
  ///  - only accept receive frames with matching IDE bit
  ///  - acceptance mask 11-bit: 0x7FF

    CAN_MOAMR16H = 0x3FFF;       // load MO16 acceptance mask register high
    CAN_MOAMR16L = 0xFFFF;       // load MO16 acceptance mask register low

  ///  Configuration of Message Object 16 interrupt pointer:
  ///  - use message pending register 0 bit position 16

    CAN_MOIPR16H = 0x0000;       // load MO16 interrupt pointer register high
    CAN_MOIPR16L = 0x1000;       // load MO16 interrupt pointer register low

  ///  Configuration of Message Object 16 FIFO/Gateway pointer:

    CAN_MOFGPR16H = 0x0000;      // load MO16 FIFO/gateway pointer register
                                 // high
    CAN_MOFGPR16L = 0x0000;      // load MO16 FIFO/gateway pointer register
                                 // low


  ///  Configuration of Message Object 16 Function control:
  ///  - this object is a STANDARD MESSAGE OBJECT
  ///  - 8 valid data bytes

    CAN_MOFCR16H = 0x0800;       // load MO16 function control register high
    CAN_MOFCR16L = 0x0000;       // load MO16 function control register low

  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 17:
  ///  -----------------------------------------------------------------------

  ///  General Configuration of the Message Object 17 :
  ///  - message object 17 is valid
  ///  - message object is used as receive object
  ///  - this message object is assigned to list 1 (node 0)

    CAN_MOCTR17H = 0x00A0;       // load MO17 control register high
    CAN_MOCTR17L = 0x0000;       // load MO17 control register low

  ///  Configuration of Message Object 17 Arbitration:
  ///  - priority class 1; transmit acceptance filtering is based on the list
  ///    order
  ///  - standard 11-bit identifier
  ///  - identifier 11-bit:      0x000

    CAN_MOAR17H = 0x4000;        // load MO17 arbitration register high
    CAN_MOAR17L = 0x0000;        // load MO17 arbitration register low

  ///  Configuration of Message Object 17 acceptance mask:
  ///  - only accept receive frames with matching IDE bit
  ///  - acceptance mask 11-bit: 0x7FF

    CAN_MOAMR17H = 0x3FFF;       // load MO17 acceptance mask register high
    CAN_MOAMR17L = 0xFFFF;       // load MO17 acceptance mask register low

  ///  Configuration of Message Object 17 interrupt pointer:
  ///  - use message pending register 0 bit position 17

    CAN_MOIPR17H = 0x0000;       // load MO17 interrupt pointer register high
    CAN_MOIPR17L = 0x1100;       // load MO17 interrupt pointer register low

  ///  Configuration of Message Object 17 FIFO/Gateway pointer:

    CAN_MOFGPR17H = 0x0000;      // load MO17 FIFO/gateway pointer register
                                 // high
    CAN_MOFGPR17L = 0x0000;      // load MO17 FIFO/gateway pointer register
                                 // low


  ///  Configuration of Message Object 17 Function control:
  ///  - this object is a STANDARD MESSAGE OBJECT
  ///  - 8 valid data bytes

    CAN_MOFCR17H = 0x0800;       // load MO17 function control register high
    CAN_MOFCR17L = 0x0000;       // load MO17 function control register low

  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 18:
  ///  -----------------------------------------------------------------------

  ///  General Configuration of the Message Object 18 :
  ///  - message object 18 is valid
  ///  - message object is used as receive object
  ///  - this message object is assigned to list 1 (node 0)

    CAN_MOCTR18H = 0x00A0;       // load MO18 control register high
    CAN_MOCTR18L = 0x0000;       // load MO18 control register low

  ///  Configuration of Message Object 18 Arbitration:
  ///  - priority class 1; transmit acceptance filtering is based on the list
  ///    order
  ///  - standard 11-bit identifier
  ///  - identifier 11-bit:      0x018

    CAN_MOAR18H = 0x4060;        // load MO18 arbitration register high
    CAN_MOAR18L = 0x0000;        // load MO18 arbitration register low

  ///  Configuration of Message Object 18 acceptance mask:
  ///  - only accept receive frames with matching IDE bit
  ///  - acceptance mask 11-bit: 0x7FF

    CAN_MOAMR18H = 0x3FFF;       // load MO18 acceptance mask register high
    CAN_MOAMR18L = 0xFFFF;       // load MO18 acceptance mask register low

  ///  Configuration of Message Object 18 interrupt pointer:
  ///  - use message pending register 0 bit position 18

    CAN_MOIPR18H = 0x0000;       // load MO18 interrupt pointer register high
    CAN_MOIPR18L = 0x1200;       // load MO18 interrupt pointer register low

  ///  Configuration of Message Object 18 FIFO/Gateway pointer:

    CAN_MOFGPR18H = 0x0000;      // load MO18 FIFO/gateway pointer register
                                 // high
    CAN_MOFGPR18L = 0x0000;      // load MO18 FIFO/gateway pointer register
                                 // low


  ///  Configuration of Message Object 18 Function control:
  ///  - this object is a STANDARD MESSAGE OBJECT
  ///  - 8 valid data bytes

    CAN_MOFCR18H = 0x0800;       // load MO18 function control register high
    CAN_MOFCR18L = 0x0000;       // load MO18 function control register low

  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 19:
  ///  -----------------------------------------------------------------------

  ///  General Configuration of the Message Object 19 :
  ///  - message object 19 is valid
  ///  - message object is used as receive object
  ///  - this message object is assigned to list 1 (node 0)

    CAN_MOCTR19H = 0x00A0;       // load MO19 control register high
    CAN_MOCTR19L = 0x0000;       // load MO19 control register low

  ///  Configuration of Message Object 19 Arbitration:
  ///  - priority class 1; transmit acceptance filtering is based on the list
  ///    order
  ///  - standard 11-bit identifier
  ///  - identifier 11-bit:      0x000

    CAN_MOAR19H = 0x4000;        // load MO19 arbitration register high
    CAN_MOAR19L = 0x0000;        // load MO19 arbitration register low

  ///  Configuration of Message Object 19 acceptance mask:
  ///  - only accept receive frames with matching IDE bit
  ///  - acceptance mask 11-bit: 0x7FF

    CAN_MOAMR19H = 0x3FFF;       // load MO19 acceptance mask register high
    CAN_MOAMR19L = 0xFFFF;       // load MO19 acceptance mask register low

  ///  Configuration of Message Object 19 interrupt pointer:
  ///  - use message pending register 0 bit position 19

    CAN_MOIPR19H = 0x0000;       // load MO19 interrupt pointer register high
    CAN_MOIPR19L = 0x1300;       // load MO19 interrupt pointer register low

  ///  Configuration of Message Object 19 FIFO/Gateway pointer:

    CAN_MOFGPR19H = 0x0000;      // load MO19 FIFO/gateway pointer register
                                 // high
    CAN_MOFGPR19L = 0x0000;      // load MO19 FIFO/gateway pointer register
                                 // low


  ///  Configuration of Message Object 19 Function control:
  ///  - this object is a STANDARD MESSAGE OBJECT
  ///  - 8 valid data bytes

    CAN_MOFCR19H = 0x0800;       // load MO19 function control register high
    CAN_MOFCR19L = 0x0000;       // load MO19 function control register low

  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 20:
  ///  -----------------------------------------------------------------------
  ///  - message object 20 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 21:
  ///  -----------------------------------------------------------------------

  ///  General Configuration of the Message Object 21 :
  ///  - message object 21 is valid
  ///  - message object is used as receive object
  ///  - this message object is assigned to list 1 (node 0)

    CAN_MOCTR21H = 0x00A0;       // load MO21 control register high
    CAN_MOCTR21L = 0x0000;       // load MO21 control register low

  ///  Configuration of Message Object 21 Arbitration:
  ///  - priority class 1; transmit acceptance filtering is based on the list
  ///    order
  ///  - standard 11-bit identifier
  ///  - identifier 11-bit:      0x021

    CAN_MOAR21H = 0x4084;        // load MO21 arbitration register high
    CAN_MOAR21L = 0x0000;        // load MO21 arbitration register low

  ///  Configuration of Message Object 21 acceptance mask:
  ///  - only accept receive frames with matching IDE bit
  ///  - acceptance mask 11-bit: 0x7FF

    CAN_MOAMR21H = 0x3FFF;       // load MO21 acceptance mask register high
    CAN_MOAMR21L = 0xFFFF;       // load MO21 acceptance mask register low

  ///  Configuration of Message Object 21 interrupt pointer:
  ///  - use message pending register 0 bit position 21

    CAN_MOIPR21H = 0x0000;       // load MO21 interrupt pointer register high
    CAN_MOIPR21L = 0x1500;       // load MO21 interrupt pointer register low

  ///  Configuration of Message Object 21 FIFO/Gateway pointer:

    CAN_MOFGPR21H = 0x0000;      // load MO21 FIFO/gateway pointer register
                                 // high
    CAN_MOFGPR21L = 0x0000;      // load MO21 FIFO/gateway pointer register
                                 // low


  ///  Configuration of Message Object 21 Function control:
  ///  - this object is a STANDARD MESSAGE OBJECT
  ///  - 8 valid data bytes

    CAN_MOFCR21H = 0x0800;       // load MO21 function control register high
    CAN_MOFCR21L = 0x0000;       // load MO21 function control register low

  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 22:
  ///  -----------------------------------------------------------------------

  ///  General Configuration of the Message Object 22 :
  ///  - message object 22 is valid
  ///  - message object is used as receive object
  ///  - this message object is assigned to list 1 (node 0)

    CAN_MOCTR22H = 0x00A0;       // load MO22 control register high
    CAN_MOCTR22L = 0x0000;       // load MO22 control register low

  ///  Configuration of Message Object 22 Arbitration:
  ///  - priority class 1; transmit acceptance filtering is based on the list
  ///    order
  ///  - standard 11-bit identifier
  ///  - identifier 11-bit:      0x022

    CAN_MOAR22H = 0x4088;        // load MO22 arbitration register high
    CAN_MOAR22L = 0x0000;        // load MO22 arbitration register low

  ///  Configuration of Message Object 22 acceptance mask:
  ///  - only accept receive frames with matching IDE bit
  ///  - acceptance mask 11-bit: 0x7FF

    CAN_MOAMR22H = 0x3FFF;       // load MO22 acceptance mask register high
    CAN_MOAMR22L = 0xFFFF;       // load MO22 acceptance mask register low

  ///  Configuration of Message Object 22 interrupt pointer:
  ///  - use message pending register 0 bit position 22

    CAN_MOIPR22H = 0x0000;       // load MO22 interrupt pointer register high
    CAN_MOIPR22L = 0x1600;       // load MO22 interrupt pointer register low

  ///  Configuration of Message Object 22 FIFO/Gateway pointer:

    CAN_MOFGPR22H = 0x0000;      // load MO22 FIFO/gateway pointer register
                                 // high
    CAN_MOFGPR22L = 0x0000;      // load MO22 FIFO/gateway pointer register
                                 // low


  ///  Configuration of Message Object 22 Function control:
  ///  - this object is a STANDARD MESSAGE OBJECT
  ///  - 8 valid data bytes

    CAN_MOFCR22H = 0x0800;       // load MO22 function control register high
    CAN_MOFCR22L = 0x0000;       // load MO22 function control register low

  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 23:
  ///  -----------------------------------------------------------------------

  ///  General Configuration of the Message Object 23 :
  ///  - message object 23 is valid
  ///  - message object is used as receive object
  ///  - this message object is assigned to list 1 (node 0)

    CAN_MOCTR23H = 0x00A0;       // load MO23 control register high
    CAN_MOCTR23L = 0x0000;       // load MO23 control register low

  ///  Configuration of Message Object 23 Arbitration:
  ///  - priority class 1; transmit acceptance filtering is based on the list
  ///    order
  ///  - standard 11-bit identifier
  ///  - identifier 11-bit:      0x023

    CAN_MOAR23H = 0x408C;        // load MO23 arbitration register high
    CAN_MOAR23L = 0x0000;        // load MO23 arbitration register low

  ///  Configuration of Message Object 23 acceptance mask:
  ///  - only accept receive frames with matching IDE bit
  ///  - acceptance mask 11-bit: 0x7FF

    CAN_MOAMR23H = 0x3FFF;       // load MO23 acceptance mask register high
    CAN_MOAMR23L = 0xFFFF;       // load MO23 acceptance mask register low

  ///  Configuration of Message Object 23 interrupt pointer:
  ///  - use message pending register 0 bit position 23

    CAN_MOIPR23H = 0x0000;       // load MO23 interrupt pointer register high
    CAN_MOIPR23L = 0x1700;       // load MO23 interrupt pointer register low

  ///  Configuration of Message Object 23 FIFO/Gateway pointer:

    CAN_MOFGPR23H = 0x0000;      // load MO23 FIFO/gateway pointer register
                                 // high
    CAN_MOFGPR23L = 0x0000;      // load MO23 FIFO/gateway pointer register
                                 // low


  ///  Configuration of Message Object 23 Function control:
  ///  - this object is a STANDARD MESSAGE OBJECT
  ///  - 8 valid data bytes

    CAN_MOFCR23H = 0x0800;       // load MO23 function control register high
    CAN_MOFCR23L = 0x0000;       // load MO23 function control register low

  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 24:
  ///  -----------------------------------------------------------------------

  ///  General Configuration of the Message Object 24 :
  ///  - message object 24 is valid
  ///  - message object is used as receive object
  ///  - this message object is assigned to list 1 (node 0)

    CAN_MOCTR24H = 0x00A0;       // load MO24 control register high
    CAN_MOCTR24L = 0x0000;       // load MO24 control register low

  ///  Configuration of Message Object 24 Arbitration:
  ///  - priority class 1; transmit acceptance filtering is based on the list
  ///    order
  ///  - standard 11-bit identifier
  ///  - identifier 11-bit:      0x024

    CAN_MOAR24H = 0x4090;        // load MO24 arbitration register high
    CAN_MOAR24L = 0x0000;        // load MO24 arbitration register low

  ///  Configuration of Message Object 24 acceptance mask:
  ///  - only accept receive frames with matching IDE bit
  ///  - acceptance mask 11-bit: 0x7FF

    CAN_MOAMR24H = 0x3FFF;       // load MO24 acceptance mask register high
    CAN_MOAMR24L = 0xFFFF;       // load MO24 acceptance mask register low

  ///  Configuration of Message Object 24 interrupt pointer:
  ///  - use message pending register 0 bit position 24

    CAN_MOIPR24H = 0x0000;       // load MO24 interrupt pointer register high
    CAN_MOIPR24L = 0x1800;       // load MO24 interrupt pointer register low

  ///  Configuration of Message Object 24 FIFO/Gateway pointer:

    CAN_MOFGPR24H = 0x0000;      // load MO24 FIFO/gateway pointer register
                                 // high
    CAN_MOFGPR24L = 0x0000;      // load MO24 FIFO/gateway pointer register
                                 // low


  ///  Configuration of Message Object 24 Function control:
  ///  - this object is a STANDARD MESSAGE OBJECT
  ///  - 8 valid data bytes

    CAN_MOFCR24H = 0x0800;       // load MO24 function control register high
    CAN_MOFCR24L = 0x0000;       // load MO24 function control register low

  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 25:
  ///  -----------------------------------------------------------------------

  ///  General Configuration of the Message Object 25 :
  ///  - message object 25 is valid
  ///  - message object is used as receive object
  ///  - this message object is assigned to list 1 (node 0)

    CAN_MOCTR25H = 0x00A0;       // load MO25 control register high
    CAN_MOCTR25L = 0x0000;       // load MO25 control register low

  ///  Configuration of Message Object 25 Arbitration:
  ///  - priority class 1; transmit acceptance filtering is based on the list
  ///    order
  ///  - standard 11-bit identifier
  ///  - identifier 11-bit:      0x025

    CAN_MOAR25H = 0x4094;        // load MO25 arbitration register high
    CAN_MOAR25L = 0x0000;        // load MO25 arbitration register low

  ///  Configuration of Message Object 25 acceptance mask:
  ///  - only accept receive frames with matching IDE bit
  ///  - acceptance mask 11-bit: 0x7FF

    CAN_MOAMR25H = 0x3FFF;       // load MO25 acceptance mask register high
    CAN_MOAMR25L = 0xFFFF;       // load MO25 acceptance mask register low

  ///  Configuration of Message Object 25 interrupt pointer:
  ///  - use message pending register 0 bit position 25

    CAN_MOIPR25H = 0x0000;       // load MO25 interrupt pointer register high
    CAN_MOIPR25L = 0x1900;       // load MO25 interrupt pointer register low

  ///  Configuration of Message Object 25 FIFO/Gateway pointer:

    CAN_MOFGPR25H = 0x0000;      // load MO25 FIFO/gateway pointer register
                                 // high
    CAN_MOFGPR25L = 0x0000;      // load MO25 FIFO/gateway pointer register
                                 // low


  ///  Configuration of Message Object 25 Function control:
  ///  - this object is a STANDARD MESSAGE OBJECT
  ///  - 8 valid data bytes

    CAN_MOFCR25H = 0x0800;       // load MO25 function control register high
    CAN_MOFCR25L = 0x0000;       // load MO25 function control register low

  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 26:
  ///  -----------------------------------------------------------------------

  ///  General Configuration of the Message Object 26 :
  ///  - message object 26 is valid
  ///  - message object is used as receive object
  ///  - this message object is assigned to list 1 (node 0)

    CAN_MOCTR26H = 0x00A0;       // load MO26 control register high
    CAN_MOCTR26L = 0x0000;       // load MO26 control register low

  ///  Configuration of Message Object 26 Arbitration:
  ///  - priority class 1; transmit acceptance filtering is based on the list
  ///    order
  ///  - standard 11-bit identifier
  ///  - identifier 11-bit:      0x026

    CAN_MOAR26H = 0x4098;        // load MO26 arbitration register high
    CAN_MOAR26L = 0x0000;        // load MO26 arbitration register low

  ///  Configuration of Message Object 26 acceptance mask:
  ///  - only accept receive frames with matching IDE bit
  ///  - acceptance mask 11-bit: 0x7FF

    CAN_MOAMR26H = 0x3FFF;       // load MO26 acceptance mask register high
    CAN_MOAMR26L = 0xFFFF;       // load MO26 acceptance mask register low

  ///  Configuration of Message Object 26 interrupt pointer:
  ///  - use message pending register 0 bit position 26

    CAN_MOIPR26H = 0x0000;       // load MO26 interrupt pointer register high
    CAN_MOIPR26L = 0x1A00;       // load MO26 interrupt pointer register low

  ///  Configuration of Message Object 26 FIFO/Gateway pointer:

    CAN_MOFGPR26H = 0x0000;      // load MO26 FIFO/gateway pointer register
                                 // high
    CAN_MOFGPR26L = 0x0000;      // load MO26 FIFO/gateway pointer register
                                 // low


  ///  Configuration of Message Object 26 Function control:
  ///  - this object is a STANDARD MESSAGE OBJECT
  ///  - 8 valid data bytes

    CAN_MOFCR26H = 0x0800;       // load MO26 function control register high
    CAN_MOFCR26L = 0x0000;       // load MO26 function control register low

  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 27:
  ///  -----------------------------------------------------------------------

  ///  General Configuration of the Message Object 27 :
  ///  - message object 27 is valid
  ///  - message object is used as receive object
  ///  - this message object is assigned to list 1 (node 0)

    CAN_MOCTR27H = 0x00A0;       // load MO27 control register high
    CAN_MOCTR27L = 0x0000;       // load MO27 control register low

  ///  Configuration of Message Object 27 Arbitration:
  ///  - priority class 1; transmit acceptance filtering is based on the list
  ///    order
  ///  - standard 11-bit identifier
  ///  - identifier 11-bit:      0x000

    CAN_MOAR27H = 0x4000;        // load MO27 arbitration register high
    CAN_MOAR27L = 0x0000;        // load MO27 arbitration register low

  ///  Configuration of Message Object 27 acceptance mask:
  ///  - only accept receive frames with matching IDE bit
  ///  - acceptance mask 11-bit: 0x7FF

    CAN_MOAMR27H = 0x3FFF;       // load MO27 acceptance mask register high
    CAN_MOAMR27L = 0xFFFF;       // load MO27 acceptance mask register low

  ///  Configuration of Message Object 27 interrupt pointer:
  ///  - use message pending register 0 bit position 27

    CAN_MOIPR27H = 0x0000;       // load MO27 interrupt pointer register high
    CAN_MOIPR27L = 0x1B00;       // load MO27 interrupt pointer register low

  ///  Configuration of Message Object 27 FIFO/Gateway pointer:

    CAN_MOFGPR27H = 0x0000;      // load MO27 FIFO/gateway pointer register
                                 // high
    CAN_MOFGPR27L = 0x0000;      // load MO27 FIFO/gateway pointer register
                                 // low


  ///  Configuration of Message Object 27 Function control:
  ///  - this object is a STANDARD MESSAGE OBJECT
  ///  - 8 valid data bytes

    CAN_MOFCR27H = 0x0800;       // load MO27 function control register high
    CAN_MOFCR27L = 0x0000;       // load MO27 function control register low

  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 28:
  ///  -----------------------------------------------------------------------

  ///  General Configuration of the Message Object 28 :
  ///  - message object 28 is valid
  ///  - message object is used as receive object
  ///  - this message object is assigned to list 1 (node 0)

    CAN_MOCTR28H = 0x00A0;       // load MO28 control register high
    CAN_MOCTR28L = 0x0000;       // load MO28 control register low

  ///  Configuration of Message Object 28 Arbitration:
  ///  - priority class 1; transmit acceptance filtering is based on the list
  ///    order
  ///  - standard 11-bit identifier
  ///  - identifier 11-bit:      0x000

    CAN_MOAR28H = 0x4000;        // load MO28 arbitration register high
    CAN_MOAR28L = 0x0000;        // load MO28 arbitration register low

  ///  Configuration of Message Object 28 acceptance mask:
  ///  - only accept receive frames with matching IDE bit
  ///  - acceptance mask 11-bit: 0x7FF

    CAN_MOAMR28H = 0x3FFF;       // load MO28 acceptance mask register high
    CAN_MOAMR28L = 0xFFFF;       // load MO28 acceptance mask register low

  ///  Configuration of Message Object 28 interrupt pointer:
  ///  - use message pending register 0 bit position 28

    CAN_MOIPR28H = 0x0000;       // load MO28 interrupt pointer register high
    CAN_MOIPR28L = 0x1C00;       // load MO28 interrupt pointer register low

  ///  Configuration of Message Object 28 FIFO/Gateway pointer:

    CAN_MOFGPR28H = 0x0000;      // load MO28 FIFO/gateway pointer register
                                 // high
    CAN_MOFGPR28L = 0x0000;      // load MO28 FIFO/gateway pointer register
                                 // low


  ///  Configuration of Message Object 28 Function control:
  ///  - this object is a STANDARD MESSAGE OBJECT
  ///  - 8 valid data bytes

    CAN_MOFCR28H = 0x0800;       // load MO28 function control register high
    CAN_MOFCR28L = 0x0000;       // load MO28 function control register low

  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 29:
  ///  -----------------------------------------------------------------------

  ///  General Configuration of the Message Object 29 :
  ///  - message object 29 is valid
  ///  - message object is used as receive object
  ///  - this message object is assigned to list 1 (node 0)

    CAN_MOCTR29H = 0x00A0;       // load MO29 control register high
    CAN_MOCTR29L = 0x0000;       // load MO29 control register low

  ///  Configuration of Message Object 29 Arbitration:
  ///  - priority class 1; transmit acceptance filtering is based on the list
  ///    order
  ///  - standard 11-bit identifier
  ///  - identifier 11-bit:      0x000

    CAN_MOAR29H = 0x4000;        // load MO29 arbitration register high
    CAN_MOAR29L = 0x0000;        // load MO29 arbitration register low

  ///  Configuration of Message Object 29 acceptance mask:
  ///  - only accept receive frames with matching IDE bit
  ///  - acceptance mask 11-bit: 0x7FF

    CAN_MOAMR29H = 0x3FFF;       // load MO29 acceptance mask register high
    CAN_MOAMR29L = 0xFFFF;       // load MO29 acceptance mask register low

  ///  Configuration of Message Object 29 interrupt pointer:
  ///  - use message pending register 0 bit position 29

    CAN_MOIPR29H = 0x0000;       // load MO29 interrupt pointer register high
    CAN_MOIPR29L = 0x1D00;       // load MO29 interrupt pointer register low

  ///  Configuration of Message Object 29 FIFO/Gateway pointer:

    CAN_MOFGPR29H = 0x0000;      // load MO29 FIFO/gateway pointer register
                                 // high
    CAN_MOFGPR29L = 0x0000;      // load MO29 FIFO/gateway pointer register
                                 // low


  ///  Configuration of Message Object 29 Function control:
  ///  - this object is a STANDARD MESSAGE OBJECT
  ///  - 8 valid data bytes

    CAN_MOFCR29H = 0x0800;       // load MO29 function control register high
    CAN_MOFCR29L = 0x0000;       // load MO29 function control register low

  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 30:
  ///  -----------------------------------------------------------------------
  ///  - message object 30 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 31:
  ///  -----------------------------------------------------------------------

  ///  General Configuration of the Message Object 31 :
  ///  - message object 31 is valid
  ///  - message object is used as receive object
  ///  - this message object is assigned to list 1 (node 0)

    CAN_MOCTR31H = 0x00A0;       // load MO31 control register high
    CAN_MOCTR31L = 0x0000;       // load MO31 control register low

  ///  Configuration of Message Object 31 Arbitration:
  ///  - priority class 1; transmit acceptance filtering is based on the list
  ///    order
  ///  - standard 11-bit identifier
  ///  - identifier 11-bit:      0x000

    CAN_MOAR31H = 0x4000;        // load MO31 arbitration register high
    CAN_MOAR31L = 0x0000;        // load MO31 arbitration register low

  ///  Configuration of Message Object 31 acceptance mask:
  ///  - only accept receive frames with matching IDE bit
  ///  - acceptance mask 11-bit: 0x7FF

    CAN_MOAMR31H = 0x3FFF;       // load MO31 acceptance mask register high
    CAN_MOAMR31L = 0xFFFF;       // load MO31 acceptance mask register low

  ///  Configuration of Message Object 31 interrupt pointer:
  ///  - use message pending register 0 bit position 31

    CAN_MOIPR31H = 0x0000;       // load MO31 interrupt pointer register high
    CAN_MOIPR31L = 0x1F00;       // load MO31 interrupt pointer register low

  ///  Configuration of Message Object 31 FIFO/Gateway pointer:

    CAN_MOFGPR31H = 0x0000;      // load MO31 FIFO/gateway pointer register
                                 // high
    CAN_MOFGPR31L = 0x0000;      // load MO31 FIFO/gateway pointer register
                                 // low


  ///  Configuration of Message Object 31 Function control:
  ///  - this object is a STANDARD MESSAGE OBJECT
  ///  - 8 valid data bytes

    CAN_MOFCR31H = 0x0800;       // load MO31 function control register high
    CAN_MOFCR31L = 0x0000;       // load MO31 function control register low

  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 32:
  ///  -----------------------------------------------------------------------

  ///  General Configuration of the Message Object 32 :
  ///  - message object 32 is valid
  ///  - message object is used as receive object
  ///  - this message object is assigned to list 1 (node 0)

    CAN_MOCTR32H = 0x00A0;       // load MO32 control register high
    CAN_MOCTR32L = 0x0000;       // load MO32 control register low

  ///  Configuration of Message Object 32 Arbitration:
  ///  - priority class 1; transmit acceptance filtering is based on the list
  ///    order
  ///  - standard 11-bit identifier
  ///  - identifier 11-bit:      0x000

    CAN_MOAR32H = 0x4000;        // load MO32 arbitration register high
    CAN_MOAR32L = 0x0000;        // load MO32 arbitration register low

  ///  Configuration of Message Object 32 acceptance mask:
  ///  - only accept receive frames with matching IDE bit
  ///  - acceptance mask 11-bit: 0x7FF

    CAN_MOAMR32H = 0x3FFF;       // load MO32 acceptance mask register high
    CAN_MOAMR32L = 0xFFFF;       // load MO32 acceptance mask register low

  ///  Configuration of Message Object 32 interrupt pointer:
  ///  - use message pending register 1 bit position 0

    CAN_MOIPR32H = 0x0000;       // load MO32 interrupt pointer register high
    CAN_MOIPR32L = 0x2000;       // load MO32 interrupt pointer register low

  ///  Configuration of Message Object 32 FIFO/Gateway pointer:

    CAN_MOFGPR32H = 0x0000;      // load MO32 FIFO/gateway pointer register
                                 // high
    CAN_MOFGPR32L = 0x0000;      // load MO32 FIFO/gateway pointer register
                                 // low


  ///  Configuration of Message Object 32 Function control:
  ///  - this object is a STANDARD MESSAGE OBJECT
  ///  - 8 valid data bytes

    CAN_MOFCR32H = 0x0800;       // load MO32 function control register high
    CAN_MOFCR32L = 0x0000;       // load MO32 function control register low

  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 33:
  ///  -----------------------------------------------------------------------

  ///  General Configuration of the Message Object 33 :
  ///  - message object 33 is valid
  ///  - message object is used as receive object
  ///  - this message object is assigned to list 1 (node 0)

    CAN_MOCTR33H = 0x00A0;       // load MO33 control register high
    CAN_MOCTR33L = 0x0000;       // load MO33 control register low

  ///  Configuration of Message Object 33 Arbitration:
  ///  - priority class 1; transmit acceptance filtering is based on the list
  ///    order
  ///  - standard 11-bit identifier
  ///  - identifier 11-bit:      0x000

    CAN_MOAR33H = 0x4000;        // load MO33 arbitration register high
    CAN_MOAR33L = 0x0000;        // load MO33 arbitration register low

  ///  Configuration of Message Object 33 acceptance mask:
  ///  - only accept receive frames with matching IDE bit
  ///  - acceptance mask 11-bit: 0x7FF

    CAN_MOAMR33H = 0x3FFF;       // load MO33 acceptance mask register high
    CAN_MOAMR33L = 0xFFFF;       // load MO33 acceptance mask register low

  ///  Configuration of Message Object 33 interrupt pointer:
  ///  - use message pending register 1 bit position 1

    CAN_MOIPR33H = 0x0000;       // load MO33 interrupt pointer register high
    CAN_MOIPR33L = 0x2100;       // load MO33 interrupt pointer register low

  ///  Configuration of Message Object 33 FIFO/Gateway pointer:

    CAN_MOFGPR33H = 0x0000;      // load MO33 FIFO/gateway pointer register
                                 // high
    CAN_MOFGPR33L = 0x0000;      // load MO33 FIFO/gateway pointer register
                                 // low


  ///  Configuration of Message Object 33 Function control:
  ///  - this object is a STANDARD MESSAGE OBJECT
  ///  - 8 valid data bytes

    CAN_MOFCR33H = 0x0800;       // load MO33 function control register high
    CAN_MOFCR33L = 0x0000;       // load MO33 function control register low

  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 34:
  ///  -----------------------------------------------------------------------

  ///  General Configuration of the Message Object 34 :
  ///  - message object 34 is valid
  ///  - message object is used as receive object
  ///  - this message object is assigned to list 1 (node 0)

    CAN_MOCTR34H = 0x00A0;       // load MO34 control register high
    CAN_MOCTR34L = 0x0000;       // load MO34 control register low

  ///  Configuration of Message Object 34 Arbitration:
  ///  - priority class 1; transmit acceptance filtering is based on the list
  ///    order
  ///  - standard 11-bit identifier
  ///  - identifier 11-bit:      0x000

    CAN_MOAR34H = 0x4000;        // load MO34 arbitration register high
    CAN_MOAR34L = 0x0000;        // load MO34 arbitration register low

  ///  Configuration of Message Object 34 acceptance mask:
  ///  - only accept receive frames with matching IDE bit
  ///  - acceptance mask 11-bit: 0x7FF

    CAN_MOAMR34H = 0x3FFF;       // load MO34 acceptance mask register high
    CAN_MOAMR34L = 0xFFFF;       // load MO34 acceptance mask register low

  ///  Configuration of Message Object 34 interrupt pointer:
  ///  - use message pending register 1 bit position 2

    CAN_MOIPR34H = 0x0000;       // load MO34 interrupt pointer register high
    CAN_MOIPR34L = 0x2200;       // load MO34 interrupt pointer register low

  ///  Configuration of Message Object 34 FIFO/Gateway pointer:

    CAN_MOFGPR34H = 0x0000;      // load MO34 FIFO/gateway pointer register
                                 // high
    CAN_MOFGPR34L = 0x0000;      // load MO34 FIFO/gateway pointer register
                                 // low


  ///  Configuration of Message Object 34 Function control:
  ///  - this object is a STANDARD MESSAGE OBJECT
  ///  - 8 valid data bytes

    CAN_MOFCR34H = 0x0800;       // load MO34 function control register high
    CAN_MOFCR34L = 0x0000;       // load MO34 function control register low

  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 35:
  ///  -----------------------------------------------------------------------

  ///  General Configuration of the Message Object 35 :
  ///  - message object 35 is valid
  ///  - message object is used as receive object
  ///  - this message object is assigned to list 1 (node 0)

    CAN_MOCTR35H = 0x00A0;       // load MO35 control register high
    CAN_MOCTR35L = 0x0000;       // load MO35 control register low

  ///  Configuration of Message Object 35 Arbitration:
  ///  - priority class 1; transmit acceptance filtering is based on the list
  ///    order
  ///  - standard 11-bit identifier
  ///  - identifier 11-bit:      0x035

    CAN_MOAR35H = 0x40D4;        // load MO35 arbitration register high
    CAN_MOAR35L = 0x0000;        // load MO35 arbitration register low

  ///  Configuration of Message Object 35 acceptance mask:
  ///  - only accept receive frames with matching IDE bit
  ///  - acceptance mask 11-bit: 0x7FF

    CAN_MOAMR35H = 0x3FFF;       // load MO35 acceptance mask register high
    CAN_MOAMR35L = 0xFFFF;       // load MO35 acceptance mask register low

  ///  Configuration of Message Object 35 interrupt pointer:
  ///  - use message pending register 1 bit position 3

    CAN_MOIPR35H = 0x0000;       // load MO35 interrupt pointer register high
    CAN_MOIPR35L = 0x2300;       // load MO35 interrupt pointer register low

  ///  Configuration of Message Object 35 FIFO/Gateway pointer:

    CAN_MOFGPR35H = 0x0000;      // load MO35 FIFO/gateway pointer register
                                 // high
    CAN_MOFGPR35L = 0x0000;      // load MO35 FIFO/gateway pointer register
                                 // low


  ///  Configuration of Message Object 35 Function control:
  ///  - this object is a STANDARD MESSAGE OBJECT
  ///  - 8 valid data bytes

    CAN_MOFCR35H = 0x0800;       // load MO35 function control register high
    CAN_MOFCR35L = 0x0000;       // load MO35 function control register low

  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 36:
  ///  -----------------------------------------------------------------------

  ///  General Configuration of the Message Object 36 :
  ///  - message object 36 is valid
  ///  - message object is used as receive object
  ///  - this message object is assigned to list 1 (node 0)

    CAN_MOCTR36H = 0x00A0;       // load MO36 control register high
    CAN_MOCTR36L = 0x0000;       // load MO36 control register low

  ///  Configuration of Message Object 36 Arbitration:
  ///  - priority class 1; transmit acceptance filtering is based on the list
  ///    order
  ///  - standard 11-bit identifier
  ///  - identifier 11-bit:      0x000

    CAN_MOAR36H = 0x4000;        // load MO36 arbitration register high
    CAN_MOAR36L = 0x0000;        // load MO36 arbitration register low

  ///  Configuration of Message Object 36 acceptance mask:
  ///  - only accept receive frames with matching IDE bit
  ///  - acceptance mask 11-bit: 0x7FF

    CAN_MOAMR36H = 0x3FFF;       // load MO36 acceptance mask register high
    CAN_MOAMR36L = 0xFFFF;       // load MO36 acceptance mask register low

  ///  Configuration of Message Object 36 interrupt pointer:
  ///  - use message pending register 1 bit position 4

    CAN_MOIPR36H = 0x0000;       // load MO36 interrupt pointer register high
    CAN_MOIPR36L = 0x2400;       // load MO36 interrupt pointer register low

  ///  Configuration of Message Object 36 FIFO/Gateway pointer:

    CAN_MOFGPR36H = 0x0000;      // load MO36 FIFO/gateway pointer register
                                 // high
    CAN_MOFGPR36L = 0x0000;      // load MO36 FIFO/gateway pointer register
                                 // low


  ///  Configuration of Message Object 36 Function control:
  ///  - this object is a STANDARD MESSAGE OBJECT
  ///  - 8 valid data bytes

    CAN_MOFCR36H = 0x0800;       // load MO36 function control register high
    CAN_MOFCR36L = 0x0000;       // load MO36 function control register low

  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 37:
  ///  -----------------------------------------------------------------------

  ///  General Configuration of the Message Object 37 :
  ///  - message object 37 is valid
  ///  - message object is used as receive object
  ///  - this message object is assigned to list 1 (node 0)

    CAN_MOCTR37H = 0x00A0;       // load MO37 control register high
    CAN_MOCTR37L = 0x0000;       // load MO37 control register low

  ///  Configuration of Message Object 37 Arbitration:
  ///  - priority class 1; transmit acceptance filtering is based on the list
  ///    order
  ///  - standard 11-bit identifier
  ///  - identifier 11-bit:      0x000

    CAN_MOAR37H = 0x4000;        // load MO37 arbitration register high
    CAN_MOAR37L = 0x0000;        // load MO37 arbitration register low

  ///  Configuration of Message Object 37 acceptance mask:
  ///  - only accept receive frames with matching IDE bit
  ///  - acceptance mask 11-bit: 0x7FF

    CAN_MOAMR37H = 0x3FFF;       // load MO37 acceptance mask register high
    CAN_MOAMR37L = 0xFFFF;       // load MO37 acceptance mask register low

  ///  Configuration of Message Object 37 interrupt pointer:
  ///  - use message pending register 1 bit position 5

    CAN_MOIPR37H = 0x0000;       // load MO37 interrupt pointer register high
    CAN_MOIPR37L = 0x2500;       // load MO37 interrupt pointer register low

  ///  Configuration of Message Object 37 FIFO/Gateway pointer:

    CAN_MOFGPR37H = 0x0000;      // load MO37 FIFO/gateway pointer register
                                 // high
    CAN_MOFGPR37L = 0x0000;      // load MO37 FIFO/gateway pointer register
                                 // low


  ///  Configuration of Message Object 37 Function control:
  ///  - this object is a STANDARD MESSAGE OBJECT
  ///  - 8 valid data bytes

    CAN_MOFCR37H = 0x0800;       // load MO37 function control register high
    CAN_MOFCR37L = 0x0000;       // load MO37 function control register low

  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 38:
  ///  -----------------------------------------------------------------------

  ///  General Configuration of the Message Object 38 :
  ///  - message object 38 is valid
  ///  - message object is used as receive object
  ///  - this message object is assigned to list 1 (node 0)

    CAN_MOCTR38H = 0x00A0;       // load MO38 control register high
    CAN_MOCTR38L = 0x0000;       // load MO38 control register low

  ///  Configuration of Message Object 38 Arbitration:
  ///  - priority class 1; transmit acceptance filtering is based on the list
  ///    order
  ///  - standard 11-bit identifier
  ///  - identifier 11-bit:      0x000

    CAN_MOAR38H = 0x4000;        // load MO38 arbitration register high
    CAN_MOAR38L = 0x0000;        // load MO38 arbitration register low

  ///  Configuration of Message Object 38 acceptance mask:
  ///  - only accept receive frames with matching IDE bit
  ///  - acceptance mask 11-bit: 0x7FF

    CAN_MOAMR38H = 0x3FFF;       // load MO38 acceptance mask register high
    CAN_MOAMR38L = 0xFFFF;       // load MO38 acceptance mask register low

  ///  Configuration of Message Object 38 interrupt pointer:
  ///  - use message pending register 1 bit position 6

    CAN_MOIPR38H = 0x0000;       // load MO38 interrupt pointer register high
    CAN_MOIPR38L = 0x2600;       // load MO38 interrupt pointer register low

  ///  Configuration of Message Object 38 FIFO/Gateway pointer:

    CAN_MOFGPR38H = 0x0000;      // load MO38 FIFO/gateway pointer register
                                 // high
    CAN_MOFGPR38L = 0x0000;      // load MO38 FIFO/gateway pointer register
                                 // low


  ///  Configuration of Message Object 38 Function control:
  ///  - this object is a STANDARD MESSAGE OBJECT
  ///  - 8 valid data bytes

    CAN_MOFCR38H = 0x0800;       // load MO38 function control register high
    CAN_MOFCR38L = 0x0000;       // load MO38 function control register low

  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 39:
  ///  -----------------------------------------------------------------------

  ///  General Configuration of the Message Object 39 :
  ///  - message object 39 is valid
  ///  - message object is used as receive object
  ///  - this message object is assigned to list 1 (node 0)

    CAN_MOCTR39H = 0x00A0;       // load MO39 control register high
    CAN_MOCTR39L = 0x0000;       // load MO39 control register low

  ///  Configuration of Message Object 39 Arbitration:
  ///  - priority class 1; transmit acceptance filtering is based on the list
  ///    order
  ///  - standard 11-bit identifier
  ///  - identifier 11-bit:      0x000

    CAN_MOAR39H = 0x4000;        // load MO39 arbitration register high
    CAN_MOAR39L = 0x0000;        // load MO39 arbitration register low

  ///  Configuration of Message Object 39 acceptance mask:
  ///  - only accept receive frames with matching IDE bit
  ///  - acceptance mask 11-bit: 0x7FF

    CAN_MOAMR39H = 0x3FFF;       // load MO39 acceptance mask register high
    CAN_MOAMR39L = 0xFFFF;       // load MO39 acceptance mask register low

  ///  Configuration of Message Object 39 interrupt pointer:
  ///  - use message pending register 1 bit position 7

    CAN_MOIPR39H = 0x0000;       // load MO39 interrupt pointer register high
    CAN_MOIPR39L = 0x2700;       // load MO39 interrupt pointer register low

  ///  Configuration of Message Object 39 FIFO/Gateway pointer:

    CAN_MOFGPR39H = 0x0000;      // load MO39 FIFO/gateway pointer register
                                 // high
    CAN_MOFGPR39L = 0x0000;      // load MO39 FIFO/gateway pointer register
                                 // low


  ///  Configuration of Message Object 39 Function control:
  ///  - this object is a STANDARD MESSAGE OBJECT
  ///  - 8 valid data bytes

    CAN_MOFCR39H = 0x0800;       // load MO39 function control register high
    CAN_MOFCR39L = 0x0000;       // load MO39 function control register low

  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 40:
  ///  -----------------------------------------------------------------------
  ///  - message object 40 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 41:
  ///  -----------------------------------------------------------------------

  ///  General Configuration of the Message Object 41 :
  ///  - message object 41 is valid
  ///  - message object is used as receive object
  ///  - this message object is assigned to list 1 (node 0)

    CAN_MOCTR41H = 0x00A0;       // load MO41 control register high
    CAN_MOCTR41L = 0x0000;       // load MO41 control register low

  ///  Configuration of Message Object 41 Arbitration:
  ///  - priority class 1; transmit acceptance filtering is based on the list
  ///    order
  ///  - standard 11-bit identifier
  ///  - identifier 11-bit:      0x000

    CAN_MOAR41H = 0x4000;        // load MO41 arbitration register high
    CAN_MOAR41L = 0x0000;        // load MO41 arbitration register low

  ///  Configuration of Message Object 41 acceptance mask:
  ///  - only accept receive frames with matching IDE bit
  ///  - acceptance mask 11-bit: 0x7FF

    CAN_MOAMR41H = 0x3FFF;       // load MO41 acceptance mask register high
    CAN_MOAMR41L = 0xFFFF;       // load MO41 acceptance mask register low

  ///  Configuration of Message Object 41 interrupt pointer:
  ///  - use message pending register 1 bit position 9

    CAN_MOIPR41H = 0x0000;       // load MO41 interrupt pointer register high
    CAN_MOIPR41L = 0x2900;       // load MO41 interrupt pointer register low

  ///  Configuration of Message Object 41 FIFO/Gateway pointer:

    CAN_MOFGPR41H = 0x0000;      // load MO41 FIFO/gateway pointer register
                                 // high
    CAN_MOFGPR41L = 0x0000;      // load MO41 FIFO/gateway pointer register
                                 // low


  ///  Configuration of Message Object 41 Function control:
  ///  - this object is a STANDARD MESSAGE OBJECT
  ///  - 8 valid data bytes

    CAN_MOFCR41H = 0x0800;       // load MO41 function control register high
    CAN_MOFCR41L = 0x0000;       // load MO41 function control register low

  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 42:
  ///  -----------------------------------------------------------------------

  ///  General Configuration of the Message Object 42 :
  ///  - message object 42 is valid
  ///  - message object is used as receive object
  ///  - this message object is assigned to list 1 (node 0)

    CAN_MOCTR42H = 0x00A0;       // load MO42 control register high
    CAN_MOCTR42L = 0x0000;       // load MO42 control register low

  ///  Configuration of Message Object 42 Arbitration:
  ///  - priority class 1; transmit acceptance filtering is based on the list
  ///    order
  ///  - standard 11-bit identifier
  ///  - identifier 11-bit:      0x000

    CAN_MOAR42H = 0x4000;        // load MO42 arbitration register high
    CAN_MOAR42L = 0x0000;        // load MO42 arbitration register low

  ///  Configuration of Message Object 42 acceptance mask:
  ///  - only accept receive frames with matching IDE bit
  ///  - acceptance mask 11-bit: 0x7FF

    CAN_MOAMR42H = 0x3FFF;       // load MO42 acceptance mask register high
    CAN_MOAMR42L = 0xFFFF;       // load MO42 acceptance mask register low

  ///  Configuration of Message Object 42 interrupt pointer:
  ///  - use message pending register 1 bit position 10

    CAN_MOIPR42H = 0x0000;       // load MO42 interrupt pointer register high
    CAN_MOIPR42L = 0x2A00;       // load MO42 interrupt pointer register low

  ///  Configuration of Message Object 42 FIFO/Gateway pointer:

    CAN_MOFGPR42H = 0x0000;      // load MO42 FIFO/gateway pointer register
                                 // high
    CAN_MOFGPR42L = 0x0000;      // load MO42 FIFO/gateway pointer register
                                 // low


  ///  Configuration of Message Object 42 Function control:
  ///  - this object is a STANDARD MESSAGE OBJECT
  ///  - 8 valid data bytes

    CAN_MOFCR42H = 0x0800;       // load MO42 function control register high
    CAN_MOFCR42L = 0x0000;       // load MO42 function control register low

  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 43:
  ///  -----------------------------------------------------------------------

  ///  General Configuration of the Message Object 43 :
  ///  - message object 43 is valid
  ///  - message object is used as receive object
  ///  - this message object is assigned to list 1 (node 0)

    CAN_MOCTR43H = 0x00A0;       // load MO43 control register high
    CAN_MOCTR43L = 0x0000;       // load MO43 control register low

  ///  Configuration of Message Object 43 Arbitration:
  ///  - priority class 1; transmit acceptance filtering is based on the list
  ///    order
  ///  - standard 11-bit identifier
  ///  - identifier 11-bit:      0x000

    CAN_MOAR43H = 0x4000;        // load MO43 arbitration register high
    CAN_MOAR43L = 0x0000;        // load MO43 arbitration register low

  ///  Configuration of Message Object 43 acceptance mask:
  ///  - only accept receive frames with matching IDE bit
  ///  - acceptance mask 11-bit: 0x7FF

    CAN_MOAMR43H = 0x3FFF;       // load MO43 acceptance mask register high
    CAN_MOAMR43L = 0xFFFF;       // load MO43 acceptance mask register low

  ///  Configuration of Message Object 43 interrupt pointer:
  ///  - use message pending register 1 bit position 11

    CAN_MOIPR43H = 0x0000;       // load MO43 interrupt pointer register high
    CAN_MOIPR43L = 0x2B00;       // load MO43 interrupt pointer register low

  ///  Configuration of Message Object 43 FIFO/Gateway pointer:

    CAN_MOFGPR43H = 0x0000;      // load MO43 FIFO/gateway pointer register
                                 // high
    CAN_MOFGPR43L = 0x0000;      // load MO43 FIFO/gateway pointer register
                                 // low


  ///  Configuration of Message Object 43 Function control:
  ///  - this object is a STANDARD MESSAGE OBJECT
  ///  - 8 valid data bytes

    CAN_MOFCR43H = 0x0800;       // load MO43 function control register high
    CAN_MOFCR43L = 0x0000;       // load MO43 function control register low

  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 44:
  ///  -----------------------------------------------------------------------

  ///  General Configuration of the Message Object 44 :
  ///  - message object 44 is valid
  ///  - message object is used as receive object
  ///  - this message object is assigned to list 1 (node 0)

    CAN_MOCTR44H = 0x00A0;       // load MO44 control register high
    CAN_MOCTR44L = 0x0000;       // load MO44 control register low

  ///  Configuration of Message Object 44 Arbitration:
  ///  - priority class 1; transmit acceptance filtering is based on the list
  ///    order
  ///  - standard 11-bit identifier
  ///  - identifier 11-bit:      0x044

    CAN_MOAR44H = 0x4110;        // load MO44 arbitration register high
    CAN_MOAR44L = 0x0000;        // load MO44 arbitration register low

  ///  Configuration of Message Object 44 acceptance mask:
  ///  - only accept receive frames with matching IDE bit
  ///  - acceptance mask 11-bit: 0x7FF

    CAN_MOAMR44H = 0x3FFF;       // load MO44 acceptance mask register high
    CAN_MOAMR44L = 0xFFFF;       // load MO44 acceptance mask register low

  ///  Configuration of Message Object 44 interrupt pointer:
  ///  - use message pending register 1 bit position 12

    CAN_MOIPR44H = 0x0000;       // load MO44 interrupt pointer register high
    CAN_MOIPR44L = 0x2C00;       // load MO44 interrupt pointer register low

  ///  Configuration of Message Object 44 FIFO/Gateway pointer:

    CAN_MOFGPR44H = 0x0000;      // load MO44 FIFO/gateway pointer register
                                 // high
    CAN_MOFGPR44L = 0x0000;      // load MO44 FIFO/gateway pointer register
                                 // low


  ///  Configuration of Message Object 44 Function control:
  ///  - this object is a STANDARD MESSAGE OBJECT
  ///  - 8 valid data bytes

    CAN_MOFCR44H = 0x0800;       // load MO44 function control register high
    CAN_MOFCR44L = 0x0000;       // load MO44 function control register low

  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 45:
  ///  -----------------------------------------------------------------------

  ///  General Configuration of the Message Object 45 :
  ///  - message object 45 is valid
  ///  - message object is used as receive object
  ///  - this message object is assigned to list 1 (node 0)

    CAN_MOCTR45H = 0x00A0;       // load MO45 control register high
    CAN_MOCTR45L = 0x0000;       // load MO45 control register low

  ///  Configuration of Message Object 45 Arbitration:
  ///  - priority class 1; transmit acceptance filtering is based on the list
  ///    order
  ///  - standard 11-bit identifier
  ///  - identifier 11-bit:      0x045

    CAN_MOAR45H = 0x4114;        // load MO45 arbitration register high
    CAN_MOAR45L = 0x0000;        // load MO45 arbitration register low

  ///  Configuration of Message Object 45 acceptance mask:
  ///  - only accept receive frames with matching IDE bit
  ///  - acceptance mask 11-bit: 0x7FF

    CAN_MOAMR45H = 0x3FFF;       // load MO45 acceptance mask register high
    CAN_MOAMR45L = 0xFFFF;       // load MO45 acceptance mask register low

  ///  Configuration of Message Object 45 interrupt pointer:
  ///  - use message pending register 1 bit position 13

    CAN_MOIPR45H = 0x0000;       // load MO45 interrupt pointer register high
    CAN_MOIPR45L = 0x2D00;       // load MO45 interrupt pointer register low

  ///  Configuration of Message Object 45 FIFO/Gateway pointer:

    CAN_MOFGPR45H = 0x0000;      // load MO45 FIFO/gateway pointer register
                                 // high
    CAN_MOFGPR45L = 0x0000;      // load MO45 FIFO/gateway pointer register
                                 // low


  ///  Configuration of Message Object 45 Function control:
  ///  - this object is a STANDARD MESSAGE OBJECT
  ///  - 8 valid data bytes

    CAN_MOFCR45H = 0x0800;       // load MO45 function control register high
    CAN_MOFCR45L = 0x0000;       // load MO45 function control register low

  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 46:
  ///  -----------------------------------------------------------------------

  ///  General Configuration of the Message Object 46 :
  ///  - message object 46 is valid
  ///  - message object is used as receive object
  ///  - this message object is assigned to list 1 (node 0)

    CAN_MOCTR46H = 0x00A0;       // load MO46 control register high
    CAN_MOCTR46L = 0x0000;       // load MO46 control register low

  ///  Configuration of Message Object 46 Arbitration:
  ///  - priority class 1; transmit acceptance filtering is based on the list
  ///    order
  ///  - standard 11-bit identifier
  ///  - identifier 11-bit:      0x046

    CAN_MOAR46H = 0x4118;        // load MO46 arbitration register high
    CAN_MOAR46L = 0x0000;        // load MO46 arbitration register low

  ///  Configuration of Message Object 46 acceptance mask:
  ///  - only accept receive frames with matching IDE bit
  ///  - acceptance mask 11-bit: 0x7FF

    CAN_MOAMR46H = 0x3FFF;       // load MO46 acceptance mask register high
    CAN_MOAMR46L = 0xFFFF;       // load MO46 acceptance mask register low

  ///  Configuration of Message Object 46 interrupt pointer:
  ///  - use message pending register 1 bit position 14

    CAN_MOIPR46H = 0x0000;       // load MO46 interrupt pointer register high
    CAN_MOIPR46L = 0x2E00;       // load MO46 interrupt pointer register low

  ///  Configuration of Message Object 46 FIFO/Gateway pointer:

    CAN_MOFGPR46H = 0x0000;      // load MO46 FIFO/gateway pointer register
                                 // high
    CAN_MOFGPR46L = 0x0000;      // load MO46 FIFO/gateway pointer register
                                 // low


  ///  Configuration of Message Object 46 Function control:
  ///  - this object is a STANDARD MESSAGE OBJECT
  ///  - 8 valid data bytes

    CAN_MOFCR46H = 0x0800;       // load MO46 function control register high
    CAN_MOFCR46L = 0x0000;       // load MO46 function control register low

  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 47:
  ///  -----------------------------------------------------------------------

  ///  General Configuration of the Message Object 47 :
  ///  - message object 47 is valid
  ///  - message object is used as receive object
  ///  - this message object is assigned to list 1 (node 0)

    CAN_MOCTR47H = 0x00A0;       // load MO47 control register high
    CAN_MOCTR47L = 0x0000;       // load MO47 control register low

  ///  Configuration of Message Object 47 Arbitration:
  ///  - priority class 1; transmit acceptance filtering is based on the list
  ///    order
  ///  - standard 11-bit identifier
  ///  - identifier 11-bit:      0x047

    CAN_MOAR47H = 0x411C;        // load MO47 arbitration register high
    CAN_MOAR47L = 0x0000;        // load MO47 arbitration register low

  ///  Configuration of Message Object 47 acceptance mask:
  ///  - only accept receive frames with matching IDE bit
  ///  - acceptance mask 11-bit: 0x7FF

    CAN_MOAMR47H = 0x3FFF;       // load MO47 acceptance mask register high
    CAN_MOAMR47L = 0xFFFF;       // load MO47 acceptance mask register low

  ///  Configuration of Message Object 47 interrupt pointer:
  ///  - use message pending register 1 bit position 15

    CAN_MOIPR47H = 0x0000;       // load MO47 interrupt pointer register high
    CAN_MOIPR47L = 0x2F00;       // load MO47 interrupt pointer register low

  ///  Configuration of Message Object 47 FIFO/Gateway pointer:

    CAN_MOFGPR47H = 0x0000;      // load MO47 FIFO/gateway pointer register
                                 // high
    CAN_MOFGPR47L = 0x0000;      // load MO47 FIFO/gateway pointer register
                                 // low


  ///  Configuration of Message Object 47 Function control:
  ///  - this object is a STANDARD MESSAGE OBJECT
  ///  - 8 valid data bytes

    CAN_MOFCR47H = 0x0800;       // load MO47 function control register high
    CAN_MOFCR47L = 0x0000;       // load MO47 function control register low

  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 48:
  ///  -----------------------------------------------------------------------

  ///  General Configuration of the Message Object 48 :
  ///  - message object 48 is valid
  ///  - message object is used as receive object
  ///  - this message object is assigned to list 1 (node 0)

    CAN_MOCTR48H = 0x00A0;       // load MO48 control register high
    CAN_MOCTR48L = 0x0000;       // load MO48 control register low

  ///  Configuration of Message Object 48 Arbitration:
  ///  - priority class 1; transmit acceptance filtering is based on the list
  ///    order
  ///  - standard 11-bit identifier
  ///  - identifier 11-bit:      0x048

    CAN_MOAR48H = 0x4120;        // load MO48 arbitration register high
    CAN_MOAR48L = 0x0000;        // load MO48 arbitration register low

  ///  Configuration of Message Object 48 acceptance mask:
  ///  - only accept receive frames with matching IDE bit
  ///  - acceptance mask 11-bit: 0x7FF

    CAN_MOAMR48H = 0x3FFF;       // load MO48 acceptance mask register high
    CAN_MOAMR48L = 0xFFFF;       // load MO48 acceptance mask register low

  ///  Configuration of Message Object 48 interrupt pointer:
  ///  - use message pending register 1 bit position 16

    CAN_MOIPR48H = 0x0000;       // load MO48 interrupt pointer register high
    CAN_MOIPR48L = 0x3000;       // load MO48 interrupt pointer register low

  ///  Configuration of Message Object 48 FIFO/Gateway pointer:

    CAN_MOFGPR48H = 0x0000;      // load MO48 FIFO/gateway pointer register
                                 // high
    CAN_MOFGPR48L = 0x0000;      // load MO48 FIFO/gateway pointer register
                                 // low


  ///  Configuration of Message Object 48 Function control:
  ///  - this object is a STANDARD MESSAGE OBJECT
  ///  - 8 valid data bytes

    CAN_MOFCR48H = 0x0800;       // load MO48 function control register high
    CAN_MOFCR48L = 0x0000;       // load MO48 function control register low

  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 49:
  ///  -----------------------------------------------------------------------

  ///  General Configuration of the Message Object 49 :
  ///  - message object 49 is valid
  ///  - message object is used as receive object
  ///  - this message object is assigned to list 1 (node 0)

    CAN_MOCTR49H = 0x00A0;       // load MO49 control register high
    CAN_MOCTR49L = 0x0000;       // load MO49 control register low

  ///  Configuration of Message Object 49 Arbitration:
  ///  - priority class 1; transmit acceptance filtering is based on the list
  ///    order
  ///  - standard 11-bit identifier
  ///  - identifier 11-bit:      0x000

    CAN_MOAR49H = 0x4000;        // load MO49 arbitration register high
    CAN_MOAR49L = 0x0000;        // load MO49 arbitration register low

  ///  Configuration of Message Object 49 acceptance mask:
  ///  - only accept receive frames with matching IDE bit
  ///  - acceptance mask 11-bit: 0x7FF

    CAN_MOAMR49H = 0x3FFF;       // load MO49 acceptance mask register high
    CAN_MOAMR49L = 0xFFFF;       // load MO49 acceptance mask register low

  ///  Configuration of Message Object 49 interrupt pointer:
  ///  - use message pending register 1 bit position 17

    CAN_MOIPR49H = 0x0000;       // load MO49 interrupt pointer register high
    CAN_MOIPR49L = 0x3100;       // load MO49 interrupt pointer register low

  ///  Configuration of Message Object 49 FIFO/Gateway pointer:

    CAN_MOFGPR49H = 0x0000;      // load MO49 FIFO/gateway pointer register
                                 // high
    CAN_MOFGPR49L = 0x0000;      // load MO49 FIFO/gateway pointer register
                                 // low


  ///  Configuration of Message Object 49 Function control:
  ///  - this object is a STANDARD MESSAGE OBJECT
  ///  - 8 valid data bytes

    CAN_MOFCR49H = 0x0800;       // load MO49 function control register high
    CAN_MOFCR49L = 0x0000;       // load MO49 function control register low

  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 50:
  ///  -----------------------------------------------------------------------
  ///  - message object 50 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 51:
  ///  -----------------------------------------------------------------------

  ///  General Configuration of the Message Object 51 :
  ///  - message object 51 is valid
  ///  - message object is used as receive object
  ///  - this message object is assigned to list 1 (node 0)

    CAN_MOCTR51H = 0x00A0;       // load MO51 control register high
    CAN_MOCTR51L = 0x0000;       // load MO51 control register low

  ///  Configuration of Message Object 51 Arbitration:
  ///  - priority class 1; transmit acceptance filtering is based on the list
  ///    order
  ///  - standard 11-bit identifier
  ///  - identifier 11-bit:      0x000

    CAN_MOAR51H = 0x4000;        // load MO51 arbitration register high
    CAN_MOAR51L = 0x0000;        // load MO51 arbitration register low

  ///  Configuration of Message Object 51 acceptance mask:
  ///  - only accept receive frames with matching IDE bit
  ///  - acceptance mask 11-bit: 0x7FF

    CAN_MOAMR51H = 0x3FFF;       // load MO51 acceptance mask register high
    CAN_MOAMR51L = 0xFFFF;       // load MO51 acceptance mask register low

  ///  Configuration of Message Object 51 interrupt pointer:
  ///  - use message pending register 1 bit position 19

    CAN_MOIPR51H = 0x0000;       // load MO51 interrupt pointer register high
    CAN_MOIPR51L = 0x3300;       // load MO51 interrupt pointer register low

  ///  Configuration of Message Object 51 FIFO/Gateway pointer:

    CAN_MOFGPR51H = 0x0000;      // load MO51 FIFO/gateway pointer register
                                 // high
    CAN_MOFGPR51L = 0x0000;      // load MO51 FIFO/gateway pointer register
                                 // low


  ///  Configuration of Message Object 51 Function control:
  ///  - this object is a STANDARD MESSAGE OBJECT
  ///  - 8 valid data bytes

    CAN_MOFCR51H = 0x0800;       // load MO51 function control register high
    CAN_MOFCR51L = 0x0000;       // load MO51 function control register low

  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 52:
  ///  -----------------------------------------------------------------------

  ///  General Configuration of the Message Object 52 :
  ///  - message object 52 is valid
  ///  - message object is used as receive object
  ///  - this message object is assigned to list 1 (node 0)

    CAN_MOCTR52H = 0x00A0;       // load MO52 control register high
    CAN_MOCTR52L = 0x0000;       // load MO52 control register low

  ///  Configuration of Message Object 52 Arbitration:
  ///  - priority class 1; transmit acceptance filtering is based on the list
  ///    order
  ///  - standard 11-bit identifier
  ///  - identifier 11-bit:      0x000

    CAN_MOAR52H = 0x4000;        // load MO52 arbitration register high
    CAN_MOAR52L = 0x0000;        // load MO52 arbitration register low

  ///  Configuration of Message Object 52 acceptance mask:
  ///  - only accept receive frames with matching IDE bit
  ///  - acceptance mask 11-bit: 0x7FF

    CAN_MOAMR52H = 0x3FFF;       // load MO52 acceptance mask register high
    CAN_MOAMR52L = 0xFFFF;       // load MO52 acceptance mask register low

  ///  Configuration of Message Object 52 interrupt pointer:
  ///  - use message pending register 1 bit position 20

    CAN_MOIPR52H = 0x0000;       // load MO52 interrupt pointer register high
    CAN_MOIPR52L = 0x3400;       // load MO52 interrupt pointer register low

  ///  Configuration of Message Object 52 FIFO/Gateway pointer:

    CAN_MOFGPR52H = 0x0000;      // load MO52 FIFO/gateway pointer register
                                 // high
    CAN_MOFGPR52L = 0x0000;      // load MO52 FIFO/gateway pointer register
                                 // low


  ///  Configuration of Message Object 52 Function control:
  ///  - this object is a STANDARD MESSAGE OBJECT
  ///  - 8 valid data bytes

    CAN_MOFCR52H = 0x0800;       // load MO52 function control register high
    CAN_MOFCR52L = 0x0000;       // load MO52 function control register low

  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 53:
  ///  -----------------------------------------------------------------------

  ///  General Configuration of the Message Object 53 :
  ///  - message object 53 is valid
  ///  - message object is used as receive object
  ///  - this message object is assigned to list 1 (node 0)

    CAN_MOCTR53H = 0x00A0;       // load MO53 control register high
    CAN_MOCTR53L = 0x0000;       // load MO53 control register low

  ///  Configuration of Message Object 53 Arbitration:
  ///  - priority class 1; transmit acceptance filtering is based on the list
  ///    order
  ///  - standard 11-bit identifier
  ///  - identifier 11-bit:      0x000

    CAN_MOAR53H = 0x4000;        // load MO53 arbitration register high
    CAN_MOAR53L = 0x0000;        // load MO53 arbitration register low

  ///  Configuration of Message Object 53 acceptance mask:
  ///  - only accept receive frames with matching IDE bit
  ///  - acceptance mask 11-bit: 0x7FF

    CAN_MOAMR53H = 0x3FFF;       // load MO53 acceptance mask register high
    CAN_MOAMR53L = 0xFFFF;       // load MO53 acceptance mask register low

  ///  Configuration of Message Object 53 interrupt pointer:
  ///  - use message pending register 1 bit position 21

    CAN_MOIPR53H = 0x0000;       // load MO53 interrupt pointer register high
    CAN_MOIPR53L = 0x3500;       // load MO53 interrupt pointer register low

  ///  Configuration of Message Object 53 FIFO/Gateway pointer:

    CAN_MOFGPR53H = 0x0000;      // load MO53 FIFO/gateway pointer register
                                 // high
    CAN_MOFGPR53L = 0x0000;      // load MO53 FIFO/gateway pointer register
                                 // low


  ///  Configuration of Message Object 53 Function control:
  ///  - this object is a STANDARD MESSAGE OBJECT
  ///  - 8 valid data bytes

    CAN_MOFCR53H = 0x0800;       // load MO53 function control register high
    CAN_MOFCR53L = 0x0000;       // load MO53 function control register low

  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 54:
  ///  -----------------------------------------------------------------------

  ///  General Configuration of the Message Object 54 :
  ///  - message object 54 is valid
  ///  - message object is used as receive object
  ///  - this message object is assigned to list 1 (node 0)

    CAN_MOCTR54H = 0x00A0;       // load MO54 control register high
    CAN_MOCTR54L = 0x0000;       // load MO54 control register low

  ///  Configuration of Message Object 54 Arbitration:
  ///  - priority class 1; transmit acceptance filtering is based on the list
  ///    order
  ///  - standard 11-bit identifier
  ///  - identifier 11-bit:      0x000

    CAN_MOAR54H = 0x4000;        // load MO54 arbitration register high
    CAN_MOAR54L = 0x0000;        // load MO54 arbitration register low

  ///  Configuration of Message Object 54 acceptance mask:
  ///  - only accept receive frames with matching IDE bit
  ///  - acceptance mask 11-bit: 0x7FF

    CAN_MOAMR54H = 0x3FFF;       // load MO54 acceptance mask register high
    CAN_MOAMR54L = 0xFFFF;       // load MO54 acceptance mask register low

  ///  Configuration of Message Object 54 interrupt pointer:
  ///  - use message pending register 1 bit position 22

    CAN_MOIPR54H = 0x0000;       // load MO54 interrupt pointer register high
    CAN_MOIPR54L = 0x3600;       // load MO54 interrupt pointer register low

  ///  Configuration of Message Object 54 FIFO/Gateway pointer:

    CAN_MOFGPR54H = 0x0000;      // load MO54 FIFO/gateway pointer register
                                 // high
    CAN_MOFGPR54L = 0x0000;      // load MO54 FIFO/gateway pointer register
                                 // low


  ///  Configuration of Message Object 54 Function control:
  ///  - this object is a STANDARD MESSAGE OBJECT
  ///  - 8 valid data bytes

    CAN_MOFCR54H = 0x0800;       // load MO54 function control register high
    CAN_MOFCR54L = 0x0000;       // load MO54 function control register low

  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 55:
  ///  -----------------------------------------------------------------------

  ///  General Configuration of the Message Object 55 :
  ///  - message object 55 is valid
  ///  - message object is used as receive object
  ///  - this message object is assigned to list 1 (node 0)

    CAN_MOCTR55H = 0x00A0;       // load MO55 control register high
    CAN_MOCTR55L = 0x0000;       // load MO55 control register low

  ///  Configuration of Message Object 55 Arbitration:
  ///  - priority class 1; transmit acceptance filtering is based on the list
  ///    order
  ///  - standard 11-bit identifier
  ///  - identifier 11-bit:      0x000

    CAN_MOAR55H = 0x4000;        // load MO55 arbitration register high
    CAN_MOAR55L = 0x0000;        // load MO55 arbitration register low

  ///  Configuration of Message Object 55 acceptance mask:
  ///  - only accept receive frames with matching IDE bit
  ///  - acceptance mask 11-bit: 0x7FF

    CAN_MOAMR55H = 0x3FFF;       // load MO55 acceptance mask register high
    CAN_MOAMR55L = 0xFFFF;       // load MO55 acceptance mask register low

  ///  Configuration of Message Object 55 interrupt pointer:
  ///  - use message pending register 1 bit position 23

    CAN_MOIPR55H = 0x0000;       // load MO55 interrupt pointer register high
    CAN_MOIPR55L = 0x3700;       // load MO55 interrupt pointer register low

  ///  Configuration of Message Object 55 FIFO/Gateway pointer:

    CAN_MOFGPR55H = 0x0000;      // load MO55 FIFO/gateway pointer register
                                 // high
    CAN_MOFGPR55L = 0x0000;      // load MO55 FIFO/gateway pointer register
                                 // low


  ///  Configuration of Message Object 55 Function control:
  ///  - this object is a STANDARD MESSAGE OBJECT
  ///  - 8 valid data bytes

    CAN_MOFCR55H = 0x0800;       // load MO55 function control register high
    CAN_MOFCR55L = 0x0000;       // load MO55 function control register low

  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 56:
  ///  -----------------------------------------------------------------------

  ///  General Configuration of the Message Object 56 :
  ///  - message object 56 is valid
  ///  - message object is used as receive object
  ///  - this message object is assigned to list 1 (node 0)

    CAN_MOCTR56H = 0x00A0;       // load MO56 control register high
    CAN_MOCTR56L = 0x0000;       // load MO56 control register low

  ///  Configuration of Message Object 56 Arbitration:
  ///  - priority class 1; transmit acceptance filtering is based on the list
  ///    order
  ///  - standard 11-bit identifier
  ///  - identifier 11-bit:      0x000

    CAN_MOAR56H = 0x4000;        // load MO56 arbitration register high
    CAN_MOAR56L = 0x0000;        // load MO56 arbitration register low

  ///  Configuration of Message Object 56 acceptance mask:
  ///  - only accept receive frames with matching IDE bit
  ///  - acceptance mask 11-bit: 0x7FF

    CAN_MOAMR56H = 0x3FFF;       // load MO56 acceptance mask register high
    CAN_MOAMR56L = 0xFFFF;       // load MO56 acceptance mask register low

  ///  Configuration of Message Object 56 interrupt pointer:
  ///  - use message pending register 1 bit position 24

    CAN_MOIPR56H = 0x0000;       // load MO56 interrupt pointer register high
    CAN_MOIPR56L = 0x3800;       // load MO56 interrupt pointer register low

  ///  Configuration of Message Object 56 FIFO/Gateway pointer:

    CAN_MOFGPR56H = 0x0000;      // load MO56 FIFO/gateway pointer register
                                 // high
    CAN_MOFGPR56L = 0x0000;      // load MO56 FIFO/gateway pointer register
                                 // low


  ///  Configuration of Message Object 56 Function control:
  ///  - this object is a STANDARD MESSAGE OBJECT
  ///  - 8 valid data bytes

    CAN_MOFCR56H = 0x0800;       // load MO56 function control register high
    CAN_MOFCR56L = 0x0000;       // load MO56 function control register low

  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 57:
  ///  -----------------------------------------------------------------------

  ///  General Configuration of the Message Object 57 :
  ///  - message object 57 is valid
  ///  - message object is used as receive object
  ///  - this message object is assigned to list 1 (node 0)

    CAN_MOCTR57H = 0x00A0;       // load MO57 control register high
    CAN_MOCTR57L = 0x0000;       // load MO57 control register low

  ///  Configuration of Message Object 57 Arbitration:
  ///  - priority class 1; transmit acceptance filtering is based on the list
  ///    order
  ///  - standard 11-bit identifier
  ///  - identifier 11-bit:      0x000

    CAN_MOAR57H = 0x4000;        // load MO57 arbitration register high
    CAN_MOAR57L = 0x0000;        // load MO57 arbitration register low

  ///  Configuration of Message Object 57 acceptance mask:
  ///  - only accept receive frames with matching IDE bit
  ///  - acceptance mask 11-bit: 0x7FF

    CAN_MOAMR57H = 0x3FFF;       // load MO57 acceptance mask register high
    CAN_MOAMR57L = 0xFFFF;       // load MO57 acceptance mask register low

  ///  Configuration of Message Object 57 interrupt pointer:
  ///  - use message pending register 1 bit position 25

    CAN_MOIPR57H = 0x0000;       // load MO57 interrupt pointer register high
    CAN_MOIPR57L = 0x3900;       // load MO57 interrupt pointer register low

  ///  Configuration of Message Object 57 FIFO/Gateway pointer:

    CAN_MOFGPR57H = 0x0000;      // load MO57 FIFO/gateway pointer register
                                 // high
    CAN_MOFGPR57L = 0x0000;      // load MO57 FIFO/gateway pointer register
                                 // low


  ///  Configuration of Message Object 57 Function control:
  ///  - this object is a STANDARD MESSAGE OBJECT
  ///  - 8 valid data bytes

    CAN_MOFCR57H = 0x0800;       // load MO57 function control register high
    CAN_MOFCR57L = 0x0000;       // load MO57 function control register low

  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 58:
  ///  -----------------------------------------------------------------------

  ///  General Configuration of the Message Object 58 :
  ///  - message object 58 is valid
  ///  - message object is used as receive object
  ///  - this message object is assigned to list 1 (node 0)

    CAN_MOCTR58H = 0x00A0;       // load MO58 control register high
    CAN_MOCTR58L = 0x0000;       // load MO58 control register low

  ///  Configuration of Message Object 58 Arbitration:
  ///  - priority class 1; transmit acceptance filtering is based on the list
  ///    order
  ///  - standard 11-bit identifier
  ///  - identifier 11-bit:      0x000

    CAN_MOAR58H = 0x4000;        // load MO58 arbitration register high
    CAN_MOAR58L = 0x0000;        // load MO58 arbitration register low

  ///  Configuration of Message Object 58 acceptance mask:
  ///  - only accept receive frames with matching IDE bit
  ///  - acceptance mask 11-bit: 0x7FF

    CAN_MOAMR58H = 0x3FFF;       // load MO58 acceptance mask register high
    CAN_MOAMR58L = 0xFFFF;       // load MO58 acceptance mask register low

  ///  Configuration of Message Object 58 interrupt pointer:
  ///  - use message pending register 1 bit position 26

    CAN_MOIPR58H = 0x0000;       // load MO58 interrupt pointer register high
    CAN_MOIPR58L = 0x3A00;       // load MO58 interrupt pointer register low

  ///  Configuration of Message Object 58 FIFO/Gateway pointer:

    CAN_MOFGPR58H = 0x0000;      // load MO58 FIFO/gateway pointer register
                                 // high
    CAN_MOFGPR58L = 0x0000;      // load MO58 FIFO/gateway pointer register
                                 // low


  ///  Configuration of Message Object 58 Function control:
  ///  - this object is a STANDARD MESSAGE OBJECT
  ///  - 8 valid data bytes

    CAN_MOFCR58H = 0x0800;       // load MO58 function control register high
    CAN_MOFCR58L = 0x0000;       // load MO58 function control register low

  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 59:
  ///  -----------------------------------------------------------------------

  ///  General Configuration of the Message Object 59 :
  ///  - message object 59 is valid
  ///  - message object is used as receive object
  ///  - this message object is assigned to list 1 (node 0)

    CAN_MOCTR59H = 0x00A0;       // load MO59 control register high
    CAN_MOCTR59L = 0x0000;       // load MO59 control register low

  ///  Configuration of Message Object 59 Arbitration:
  ///  - priority class 1; transmit acceptance filtering is based on the list
  ///    order
  ///  - standard 11-bit identifier
  ///  - identifier 11-bit:      0x000

    CAN_MOAR59H = 0x4000;        // load MO59 arbitration register high
    CAN_MOAR59L = 0x0000;        // load MO59 arbitration register low

  ///  Configuration of Message Object 59 acceptance mask:
  ///  - only accept receive frames with matching IDE bit
  ///  - acceptance mask 11-bit: 0x7FF

    CAN_MOAMR59H = 0x3FFF;       // load MO59 acceptance mask register high
    CAN_MOAMR59L = 0xFFFF;       // load MO59 acceptance mask register low

  ///  Configuration of Message Object 59 interrupt pointer:
  ///  - use message pending register 1 bit position 27

    CAN_MOIPR59H = 0x0000;       // load MO59 interrupt pointer register high
    CAN_MOIPR59L = 0x3B00;       // load MO59 interrupt pointer register low

  ///  Configuration of Message Object 59 FIFO/Gateway pointer:

    CAN_MOFGPR59H = 0x0000;      // load MO59 FIFO/gateway pointer register
                                 // high
    CAN_MOFGPR59L = 0x0000;      // load MO59 FIFO/gateway pointer register
                                 // low


  ///  Configuration of Message Object 59 Function control:
  ///  - this object is a STANDARD MESSAGE OBJECT
  ///  - 8 valid data bytes

    CAN_MOFCR59H = 0x0800;       // load MO59 function control register high
    CAN_MOFCR59L = 0x0000;       // load MO59 function control register low

  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 60:
  ///  -----------------------------------------------------------------------
  ///  - message object 60 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 61:
  ///  -----------------------------------------------------------------------

  ///  General Configuration of the Message Object 61 :
  ///  - message object 61 is valid
  ///  - message object is used as receive object
  ///  - this message object is assigned to list 1 (node 0)

    CAN_MOCTR61H = 0x00A0;       // load MO61 control register high
    CAN_MOCTR61L = 0x0000;       // load MO61 control register low

  ///  Configuration of Message Object 61 Arbitration:
  ///  - priority class 1; transmit acceptance filtering is based on the list
  ///    order
  ///  - standard 11-bit identifier
  ///  - identifier 11-bit:      0x000

    CAN_MOAR61H = 0x4000;        // load MO61 arbitration register high
    CAN_MOAR61L = 0x0000;        // load MO61 arbitration register low

  ///  Configuration of Message Object 61 acceptance mask:
  ///  - only accept receive frames with matching IDE bit
  ///  - acceptance mask 11-bit: 0x7FF

    CAN_MOAMR61H = 0x3FFF;       // load MO61 acceptance mask register high
    CAN_MOAMR61L = 0xFFFF;       // load MO61 acceptance mask register low

  ///  Configuration of Message Object 61 interrupt pointer:
  ///  - use message pending register 1 bit position 29

    CAN_MOIPR61H = 0x0000;       // load MO61 interrupt pointer register high
    CAN_MOIPR61L = 0x3D00;       // load MO61 interrupt pointer register low

  ///  Configuration of Message Object 61 FIFO/Gateway pointer:

    CAN_MOFGPR61H = 0x0000;      // load MO61 FIFO/gateway pointer register
                                 // high
    CAN_MOFGPR61L = 0x0000;      // load MO61 FIFO/gateway pointer register
                                 // low


  ///  Configuration of Message Object 61 Function control:
  ///  - this object is a STANDARD MESSAGE OBJECT
  ///  - 8 valid data bytes

    CAN_MOFCR61H = 0x0800;       // load MO61 function control register high
    CAN_MOFCR61L = 0x0000;       // load MO61 function control register low

  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 62:
  ///  -----------------------------------------------------------------------

  ///  General Configuration of the Message Object 62 :
  ///  - message object 62 is valid
  ///  - message object is used as receive object
  ///  - this message object is assigned to list 1 (node 0)

    CAN_MOCTR62H = 0x00A0;       // load MO62 control register high
    CAN_MOCTR62L = 0x0000;       // load MO62 control register low

  ///  Configuration of Message Object 62 Arbitration:
  ///  - priority class 1; transmit acceptance filtering is based on the list
  ///    order
  ///  - standard 11-bit identifier
  ///  - identifier 11-bit:      0x000

    CAN_MOAR62H = 0x4000;        // load MO62 arbitration register high
    CAN_MOAR62L = 0x0000;        // load MO62 arbitration register low

  ///  Configuration of Message Object 62 acceptance mask:
  ///  - only accept receive frames with matching IDE bit
  ///  - acceptance mask 11-bit: 0x7FF

    CAN_MOAMR62H = 0x3FFF;       // load MO62 acceptance mask register high
    CAN_MOAMR62L = 0xFFFF;       // load MO62 acceptance mask register low

  ///  Configuration of Message Object 62 interrupt pointer:
  ///  - use message pending register 1 bit position 30

    CAN_MOIPR62H = 0x0000;       // load MO62 interrupt pointer register high
    CAN_MOIPR62L = 0x3E00;       // load MO62 interrupt pointer register low

  ///  Configuration of Message Object 62 FIFO/Gateway pointer:

    CAN_MOFGPR62H = 0x0000;      // load MO62 FIFO/gateway pointer register
                                 // high
    CAN_MOFGPR62L = 0x0000;      // load MO62 FIFO/gateway pointer register
                                 // low


  ///  Configuration of Message Object 62 Function control:
  ///  - this object is a STANDARD MESSAGE OBJECT
  ///  - 8 valid data bytes

    CAN_MOFCR62H = 0x0800;       // load MO62 function control register high
    CAN_MOFCR62L = 0x0000;       // load MO62 function control register low

  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 63:
  ///  -----------------------------------------------------------------------

  ///  General Configuration of the Message Object 63 :
  ///  - message object 63 is valid
  ///  - message object is used as receive object
  ///  - this message object is assigned to list 1 (node 0)

    CAN_MOCTR63H = 0x00A0;       // load MO63 control register high
    CAN_MOCTR63L = 0x0000;       // load MO63 control register low

  ///  Configuration of Message Object 63 Arbitration:
  ///  - priority class 1; transmit acceptance filtering is based on the list
  ///    order
  ///  - standard 11-bit identifier
  ///  - identifier 11-bit:      0x000

    CAN_MOAR63H = 0x4000;        // load MO63 arbitration register high
    CAN_MOAR63L = 0x0000;        // load MO63 arbitration register low

  ///  Configuration of Message Object 63 acceptance mask:
  ///  - only accept receive frames with matching IDE bit
  ///  - acceptance mask 11-bit: 0x7FF

    CAN_MOAMR63H = 0x3FFF;       // load MO63 acceptance mask register high
    CAN_MOAMR63L = 0xFFFF;       // load MO63 acceptance mask register low

  ///  Configuration of Message Object 63 interrupt pointer:
  ///  - use message pending register 1 bit position 31

    CAN_MOIPR63H = 0x0000;       // load MO63 interrupt pointer register high
    CAN_MOIPR63L = 0x3F00;       // load MO63 interrupt pointer register low

  ///  Configuration of Message Object 63 FIFO/Gateway pointer:

    CAN_MOFGPR63H = 0x0000;      // load MO63 FIFO/gateway pointer register
                                 // high
    CAN_MOFGPR63L = 0x0000;      // load MO63 FIFO/gateway pointer register
                                 // low


  ///  Configuration of Message Object 63 Function control:
  ///  - this object is a STANDARD MESSAGE OBJECT
  ///  - 8 valid data bytes

    CAN_MOFCR63H = 0x0800;       // load MO63 function control register high
    CAN_MOFCR63L = 0x0000;       // load MO63 function control register low

  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 64:
  ///  -----------------------------------------------------------------------

  ///  General Configuration of the Message Object 64 :
  ///  - message object 64 is valid
  ///  - message object is used as receive object
  ///  - this message object is assigned to list 1 (node 0)

    CAN_MOCTR64H = 0x00A0;       // load MO64 control register high
    CAN_MOCTR64L = 0x0000;       // load MO64 control register low

  ///  Configuration of Message Object 64 Arbitration:
  ///  - priority class 1; transmit acceptance filtering is based on the list
  ///    order
  ///  - standard 11-bit identifier
  ///  - identifier 11-bit:      0x000

    CAN_MOAR64H = 0x4000;        // load MO64 arbitration register high
    CAN_MOAR64L = 0x0000;        // load MO64 arbitration register low

  ///  Configuration of Message Object 64 acceptance mask:
  ///  - only accept receive frames with matching IDE bit
  ///  - acceptance mask 11-bit: 0x7FF

    CAN_MOAMR64H = 0x3FFF;       // load MO64 acceptance mask register high
    CAN_MOAMR64L = 0xFFFF;       // load MO64 acceptance mask register low

  ///  Configuration of Message Object 64 interrupt pointer:
  ///  - use message pending register 2 bit position 0

    CAN_MOIPR64H = 0x0000;       // load MO64 interrupt pointer register high
    CAN_MOIPR64L = 0x4000;       // load MO64 interrupt pointer register low

  ///  Configuration of Message Object 64 FIFO/Gateway pointer:

    CAN_MOFGPR64H = 0x0000;      // load MO64 FIFO/gateway pointer register
                                 // high
    CAN_MOFGPR64L = 0x0000;      // load MO64 FIFO/gateway pointer register
                                 // low


  ///  Configuration of Message Object 64 Function control:
  ///  - this object is a STANDARD MESSAGE OBJECT
  ///  - 8 valid data bytes

    CAN_MOFCR64H = 0x0800;       // load MO64 function control register high
    CAN_MOFCR64L = 0x0000;       // load MO64 function control register low

  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 65:
  ///  -----------------------------------------------------------------------

  ///  General Configuration of the Message Object 65 :
  ///  - message object 65 is valid
  ///  - message object is used as receive object
  ///  - this message object is assigned to list 1 (node 0)

    CAN_MOCTR65H = 0x00A0;       // load MO65 control register high
    CAN_MOCTR65L = 0x0000;       // load MO65 control register low

  ///  Configuration of Message Object 65 Arbitration:
  ///  - priority class 1; transmit acceptance filtering is based on the list
  ///    order
  ///  - standard 11-bit identifier
  ///  - identifier 11-bit:      0x000

    CAN_MOAR65H = 0x4000;        // load MO65 arbitration register high
    CAN_MOAR65L = 0x0000;        // load MO65 arbitration register low

  ///  Configuration of Message Object 65 acceptance mask:
  ///  - only accept receive frames with matching IDE bit
  ///  - acceptance mask 11-bit: 0x7FF

    CAN_MOAMR65H = 0x3FFF;       // load MO65 acceptance mask register high
    CAN_MOAMR65L = 0xFFFF;       // load MO65 acceptance mask register low

  ///  Configuration of Message Object 65 interrupt pointer:
  ///  - use message pending register 2 bit position 1

    CAN_MOIPR65H = 0x0000;       // load MO65 interrupt pointer register high
    CAN_MOIPR65L = 0x4100;       // load MO65 interrupt pointer register low

  ///  Configuration of Message Object 65 FIFO/Gateway pointer:

    CAN_MOFGPR65H = 0x0000;      // load MO65 FIFO/gateway pointer register
                                 // high
    CAN_MOFGPR65L = 0x0000;      // load MO65 FIFO/gateway pointer register
                                 // low


  ///  Configuration of Message Object 65 Function control:
  ///  - this object is a STANDARD MESSAGE OBJECT
  ///  - 8 valid data bytes

    CAN_MOFCR65H = 0x0800;       // load MO65 function control register high
    CAN_MOFCR65L = 0x0000;       // load MO65 function control register low

  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 66:
  ///  -----------------------------------------------------------------------

  ///  General Configuration of the Message Object 66 :
  ///  - message object 66 is valid
  ///  - message object is used as receive object
  ///  - this message object is assigned to list 1 (node 0)

    CAN_MOCTR66H = 0x00A0;       // load MO66 control register high
    CAN_MOCTR66L = 0x0000;       // load MO66 control register low

  ///  Configuration of Message Object 66 Arbitration:
  ///  - priority class 1; transmit acceptance filtering is based on the list
  ///    order
  ///  - standard 11-bit identifier
  ///  - identifier 11-bit:      0x000

    CAN_MOAR66H = 0x4000;        // load MO66 arbitration register high
    CAN_MOAR66L = 0x0000;        // load MO66 arbitration register low

  ///  Configuration of Message Object 66 acceptance mask:
  ///  - only accept receive frames with matching IDE bit
  ///  - acceptance mask 11-bit: 0x7FF

    CAN_MOAMR66H = 0x3FFF;       // load MO66 acceptance mask register high
    CAN_MOAMR66L = 0xFFFF;       // load MO66 acceptance mask register low

  ///  Configuration of Message Object 66 interrupt pointer:
  ///  - use message pending register 2 bit position 2

    CAN_MOIPR66H = 0x0000;       // load MO66 interrupt pointer register high
    CAN_MOIPR66L = 0x4200;       // load MO66 interrupt pointer register low

  ///  Configuration of Message Object 66 FIFO/Gateway pointer:

    CAN_MOFGPR66H = 0x0000;      // load MO66 FIFO/gateway pointer register
                                 // high
    CAN_MOFGPR66L = 0x0000;      // load MO66 FIFO/gateway pointer register
                                 // low


  ///  Configuration of Message Object 66 Function control:
  ///  - this object is a STANDARD MESSAGE OBJECT
  ///  - 8 valid data bytes

    CAN_MOFCR66H = 0x0800;       // load MO66 function control register high
    CAN_MOFCR66L = 0x0000;       // load MO66 function control register low

  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 67:
  ///  -----------------------------------------------------------------------

  ///  General Configuration of the Message Object 67 :
  ///  - message object 67 is valid
  ///  - message object is used as receive object
  ///  - this message object is assigned to list 1 (node 0)

    CAN_MOCTR67H = 0x00A0;       // load MO67 control register high
    CAN_MOCTR67L = 0x0000;       // load MO67 control register low

  ///  Configuration of Message Object 67 Arbitration:
  ///  - priority class 1; transmit acceptance filtering is based on the list
  ///    order
  ///  - standard 11-bit identifier
  ///  - identifier 11-bit:      0x000

    CAN_MOAR67H = 0x4000;        // load MO67 arbitration register high
    CAN_MOAR67L = 0x0000;        // load MO67 arbitration register low

  ///  Configuration of Message Object 67 acceptance mask:
  ///  - only accept receive frames with matching IDE bit
  ///  - acceptance mask 11-bit: 0x7FF

    CAN_MOAMR67H = 0x3FFF;       // load MO67 acceptance mask register high
    CAN_MOAMR67L = 0xFFFF;       // load MO67 acceptance mask register low

  ///  Configuration of Message Object 67 interrupt pointer:
  ///  - use message pending register 2 bit position 3

    CAN_MOIPR67H = 0x0000;       // load MO67 interrupt pointer register high
    CAN_MOIPR67L = 0x4300;       // load MO67 interrupt pointer register low

  ///  Configuration of Message Object 67 FIFO/Gateway pointer:

    CAN_MOFGPR67H = 0x0000;      // load MO67 FIFO/gateway pointer register
                                 // high
    CAN_MOFGPR67L = 0x0000;      // load MO67 FIFO/gateway pointer register
                                 // low


  ///  Configuration of Message Object 67 Function control:
  ///  - this object is a STANDARD MESSAGE OBJECT
  ///  - 8 valid data bytes

    CAN_MOFCR67H = 0x0800;       // load MO67 function control register high
    CAN_MOFCR67L = 0x0000;       // load MO67 function control register low

  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 68:
  ///  -----------------------------------------------------------------------

  ///  General Configuration of the Message Object 68 :
  ///  - message object 68 is valid
  ///  - message object is used as receive object
  ///  - this message object is assigned to list 1 (node 0)

    CAN_MOCTR68H = 0x00A0;       // load MO68 control register high
    CAN_MOCTR68L = 0x0000;       // load MO68 control register low

  ///  Configuration of Message Object 68 Arbitration:
  ///  - priority class 1; transmit acceptance filtering is based on the list
  ///    order
  ///  - standard 11-bit identifier
  ///  - identifier 11-bit:      0x000

    CAN_MOAR68H = 0x4000;        // load MO68 arbitration register high
    CAN_MOAR68L = 0x0000;        // load MO68 arbitration register low

  ///  Configuration of Message Object 68 acceptance mask:
  ///  - only accept receive frames with matching IDE bit
  ///  - acceptance mask 11-bit: 0x7FF

    CAN_MOAMR68H = 0x3FFF;       // load MO68 acceptance mask register high
    CAN_MOAMR68L = 0xFFFF;       // load MO68 acceptance mask register low

  ///  Configuration of Message Object 68 interrupt pointer:
  ///  - use message pending register 2 bit position 4

    CAN_MOIPR68H = 0x0000;       // load MO68 interrupt pointer register high
    CAN_MOIPR68L = 0x4400;       // load MO68 interrupt pointer register low

  ///  Configuration of Message Object 68 FIFO/Gateway pointer:

    CAN_MOFGPR68H = 0x0000;      // load MO68 FIFO/gateway pointer register
                                 // high
    CAN_MOFGPR68L = 0x0000;      // load MO68 FIFO/gateway pointer register
                                 // low


  ///  Configuration of Message Object 68 Function control:
  ///  - this object is a STANDARD MESSAGE OBJECT
  ///  - 8 valid data bytes

    CAN_MOFCR68H = 0x0800;       // load MO68 function control register high
    CAN_MOFCR68L = 0x0000;       // load MO68 function control register low

  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 69:
  ///  -----------------------------------------------------------------------

  ///  General Configuration of the Message Object 69 :
  ///  - message object 69 is valid
  ///  - message object is used as receive object
  ///  - this message object is assigned to list 1 (node 0)

    CAN_MOCTR69H = 0x00A0;       // load MO69 control register high
    CAN_MOCTR69L = 0x0000;       // load MO69 control register low

  ///  Configuration of Message Object 69 Arbitration:
  ///  - priority class 1; transmit acceptance filtering is based on the list
  ///    order
  ///  - standard 11-bit identifier
  ///  - identifier 11-bit:      0x000

    CAN_MOAR69H = 0x4000;        // load MO69 arbitration register high
    CAN_MOAR69L = 0x0000;        // load MO69 arbitration register low

  ///  Configuration of Message Object 69 acceptance mask:
  ///  - only accept receive frames with matching IDE bit
  ///  - acceptance mask 11-bit: 0x7FF

    CAN_MOAMR69H = 0x3FFF;       // load MO69 acceptance mask register high
    CAN_MOAMR69L = 0xFFFF;       // load MO69 acceptance mask register low

  ///  Configuration of Message Object 69 interrupt pointer:
  ///  - use message pending register 2 bit position 5

    CAN_MOIPR69H = 0x0000;       // load MO69 interrupt pointer register high
    CAN_MOIPR69L = 0x4500;       // load MO69 interrupt pointer register low

  ///  Configuration of Message Object 69 FIFO/Gateway pointer:

    CAN_MOFGPR69H = 0x0000;      // load MO69 FIFO/gateway pointer register
                                 // high
    CAN_MOFGPR69L = 0x0000;      // load MO69 FIFO/gateway pointer register
                                 // low


  ///  Configuration of Message Object 69 Function control:
  ///  - this object is a STANDARD MESSAGE OBJECT
  ///  - 8 valid data bytes

    CAN_MOFCR69H = 0x0800;       // load MO69 function control register high
    CAN_MOFCR69L = 0x0000;       // load MO69 function control register low

  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 70:
  ///  -----------------------------------------------------------------------
  ///  - message object 70 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 71:
  ///  -----------------------------------------------------------------------

  ///  General Configuration of the Message Object 71 :
  ///  - message object 71 is valid
  ///  - message object is used as receive object
  ///  - this message object is assigned to list 1 (node 0)

    CAN_MOCTR71H = 0x00A0;       // load MO71 control register high
    CAN_MOCTR71L = 0x0000;       // load MO71 control register low

  ///  Configuration of Message Object 71 Arbitration:
  ///  - priority class 1; transmit acceptance filtering is based on the list
  ///    order
  ///  - standard 11-bit identifier
  ///  - identifier 11-bit:      0x000

    CAN_MOAR71H = 0x4000;        // load MO71 arbitration register high
    CAN_MOAR71L = 0x0000;        // load MO71 arbitration register low

  ///  Configuration of Message Object 71 acceptance mask:
  ///  - only accept receive frames with matching IDE bit
  ///  - acceptance mask 11-bit: 0x7FF

    CAN_MOAMR71H = 0x3FFF;       // load MO71 acceptance mask register high
    CAN_MOAMR71L = 0xFFFF;       // load MO71 acceptance mask register low

  ///  Configuration of Message Object 71 interrupt pointer:
  ///  - use message pending register 2 bit position 7

    CAN_MOIPR71H = 0x0000;       // load MO71 interrupt pointer register high
    CAN_MOIPR71L = 0x4700;       // load MO71 interrupt pointer register low

  ///  Configuration of Message Object 71 FIFO/Gateway pointer:

    CAN_MOFGPR71H = 0x0000;      // load MO71 FIFO/gateway pointer register
                                 // high
    CAN_MOFGPR71L = 0x0000;      // load MO71 FIFO/gateway pointer register
                                 // low


  ///  Configuration of Message Object 71 Function control:
  ///  - this object is a STANDARD MESSAGE OBJECT
  ///  - 8 valid data bytes

    CAN_MOFCR71H = 0x0800;       // load MO71 function control register high
    CAN_MOFCR71L = 0x0000;       // load MO71 function control register low

  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 72:
  ///  -----------------------------------------------------------------------

  ///  General Configuration of the Message Object 72 :
  ///  - message object 72 is valid
  ///  - message object is used as receive object
  ///  - this message object is assigned to list 1 (node 0)

    CAN_MOCTR72H = 0x00A0;       // load MO72 control register high
    CAN_MOCTR72L = 0x0000;       // load MO72 control register low

  ///  Configuration of Message Object 72 Arbitration:
  ///  - priority class 1; transmit acceptance filtering is based on the list
  ///    order
  ///  - standard 11-bit identifier
  ///  - identifier 11-bit:      0x000

    CAN_MOAR72H = 0x4000;        // load MO72 arbitration register high
    CAN_MOAR72L = 0x0000;        // load MO72 arbitration register low

  ///  Configuration of Message Object 72 acceptance mask:
  ///  - only accept receive frames with matching IDE bit
  ///  - acceptance mask 11-bit: 0x7FF

    CAN_MOAMR72H = 0x3FFF;       // load MO72 acceptance mask register high
    CAN_MOAMR72L = 0xFFFF;       // load MO72 acceptance mask register low

  ///  Configuration of Message Object 72 interrupt pointer:
  ///  - use message pending register 2 bit position 8

    CAN_MOIPR72H = 0x0000;       // load MO72 interrupt pointer register high
    CAN_MOIPR72L = 0x4800;       // load MO72 interrupt pointer register low

  ///  Configuration of Message Object 72 FIFO/Gateway pointer:

    CAN_MOFGPR72H = 0x0000;      // load MO72 FIFO/gateway pointer register
                                 // high
    CAN_MOFGPR72L = 0x0000;      // load MO72 FIFO/gateway pointer register
                                 // low


  ///  Configuration of Message Object 72 Function control:
  ///  - this object is a STANDARD MESSAGE OBJECT
  ///  - 8 valid data bytes

    CAN_MOFCR72H = 0x0800;       // load MO72 function control register high
    CAN_MOFCR72L = 0x0000;       // load MO72 function control register low

  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 73:
  ///  -----------------------------------------------------------------------

  ///  General Configuration of the Message Object 73 :
  ///  - message object 73 is valid
  ///  - message object is used as receive object
  ///  - this message object is assigned to list 1 (node 0)

    CAN_MOCTR73H = 0x00A0;       // load MO73 control register high
    CAN_MOCTR73L = 0x0000;       // load MO73 control register low

  ///  Configuration of Message Object 73 Arbitration:
  ///  - priority class 1; transmit acceptance filtering is based on the list
  ///    order
  ///  - standard 11-bit identifier
  ///  - identifier 11-bit:      0x000

    CAN_MOAR73H = 0x4000;        // load MO73 arbitration register high
    CAN_MOAR73L = 0x0000;        // load MO73 arbitration register low

  ///  Configuration of Message Object 73 acceptance mask:
  ///  - only accept receive frames with matching IDE bit
  ///  - acceptance mask 11-bit: 0x7FF

    CAN_MOAMR73H = 0x3FFF;       // load MO73 acceptance mask register high
    CAN_MOAMR73L = 0xFFFF;       // load MO73 acceptance mask register low

  ///  Configuration of Message Object 73 interrupt pointer:
  ///  - use message pending register 2 bit position 9

    CAN_MOIPR73H = 0x0000;       // load MO73 interrupt pointer register high
    CAN_MOIPR73L = 0x4900;       // load MO73 interrupt pointer register low

  ///  Configuration of Message Object 73 FIFO/Gateway pointer:

    CAN_MOFGPR73H = 0x0000;      // load MO73 FIFO/gateway pointer register
                                 // high
    CAN_MOFGPR73L = 0x0000;      // load MO73 FIFO/gateway pointer register
                                 // low


  ///  Configuration of Message Object 73 Function control:
  ///  - this object is a STANDARD MESSAGE OBJECT
  ///  - 8 valid data bytes

    CAN_MOFCR73H = 0x0800;       // load MO73 function control register high
    CAN_MOFCR73L = 0x0000;       // load MO73 function control register low

  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 74:
  ///  -----------------------------------------------------------------------

  ///  General Configuration of the Message Object 74 :
  ///  - message object 74 is valid
  ///  - message object is used as receive object
  ///  - this message object is assigned to list 1 (node 0)

    CAN_MOCTR74H = 0x00A0;       // load MO74 control register high
    CAN_MOCTR74L = 0x0000;       // load MO74 control register low

  ///  Configuration of Message Object 74 Arbitration:
  ///  - priority class 1; transmit acceptance filtering is based on the list
  ///    order
  ///  - standard 11-bit identifier
  ///  - identifier 11-bit:      0x000

    CAN_MOAR74H = 0x4000;        // load MO74 arbitration register high
    CAN_MOAR74L = 0x0000;        // load MO74 arbitration register low

  ///  Configuration of Message Object 74 acceptance mask:
  ///  - only accept receive frames with matching IDE bit
  ///  - acceptance mask 11-bit: 0x7FF

    CAN_MOAMR74H = 0x3FFF;       // load MO74 acceptance mask register high
    CAN_MOAMR74L = 0xFFFF;       // load MO74 acceptance mask register low

  ///  Configuration of Message Object 74 interrupt pointer:
  ///  - use message pending register 2 bit position 10

    CAN_MOIPR74H = 0x0000;       // load MO74 interrupt pointer register high
    CAN_MOIPR74L = 0x4A00;       // load MO74 interrupt pointer register low

  ///  Configuration of Message Object 74 FIFO/Gateway pointer:

    CAN_MOFGPR74H = 0x0000;      // load MO74 FIFO/gateway pointer register
                                 // high
    CAN_MOFGPR74L = 0x0000;      // load MO74 FIFO/gateway pointer register
                                 // low


  ///  Configuration of Message Object 74 Function control:
  ///  - this object is a STANDARD MESSAGE OBJECT
  ///  - 8 valid data bytes

    CAN_MOFCR74H = 0x0800;       // load MO74 function control register high
    CAN_MOFCR74L = 0x0000;       // load MO74 function control register low

  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 75:
  ///  -----------------------------------------------------------------------

  ///  General Configuration of the Message Object 75 :
  ///  - message object 75 is valid
  ///  - message object is used as receive object
  ///  - this message object is assigned to list 1 (node 0)

    CAN_MOCTR75H = 0x00A0;       // load MO75 control register high
    CAN_MOCTR75L = 0x0000;       // load MO75 control register low

  ///  Configuration of Message Object 75 Arbitration:
  ///  - priority class 1; transmit acceptance filtering is based on the list
  ///    order
  ///  - standard 11-bit identifier
  ///  - identifier 11-bit:      0x000

    CAN_MOAR75H = 0x4000;        // load MO75 arbitration register high
    CAN_MOAR75L = 0x0000;        // load MO75 arbitration register low

  ///  Configuration of Message Object 75 acceptance mask:
  ///  - only accept receive frames with matching IDE bit
  ///  - acceptance mask 11-bit: 0x7FF

    CAN_MOAMR75H = 0x3FFF;       // load MO75 acceptance mask register high
    CAN_MOAMR75L = 0xFFFF;       // load MO75 acceptance mask register low

  ///  Configuration of Message Object 75 interrupt pointer:
  ///  - use message pending register 2 bit position 11

    CAN_MOIPR75H = 0x0000;       // load MO75 interrupt pointer register high
    CAN_MOIPR75L = 0x4B00;       // load MO75 interrupt pointer register low

  ///  Configuration of Message Object 75 FIFO/Gateway pointer:

    CAN_MOFGPR75H = 0x0000;      // load MO75 FIFO/gateway pointer register
                                 // high
    CAN_MOFGPR75L = 0x0000;      // load MO75 FIFO/gateway pointer register
                                 // low


  ///  Configuration of Message Object 75 Function control:
  ///  - this object is a STANDARD MESSAGE OBJECT
  ///  - 8 valid data bytes

    CAN_MOFCR75H = 0x0800;       // load MO75 function control register high
    CAN_MOFCR75L = 0x0000;       // load MO75 function control register low

  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 76:
  ///  -----------------------------------------------------------------------

  ///  General Configuration of the Message Object 76 :
  ///  - message object 76 is valid
  ///  - message object is used as receive object
  ///  - this message object is assigned to list 1 (node 0)

    CAN_MOCTR76H = 0x00A0;       // load MO76 control register high
    CAN_MOCTR76L = 0x0000;       // load MO76 control register low

  ///  Configuration of Message Object 76 Arbitration:
  ///  - priority class 1; transmit acceptance filtering is based on the list
  ///    order
  ///  - standard 11-bit identifier
  ///  - identifier 11-bit:      0x000

    CAN_MOAR76H = 0x4000;        // load MO76 arbitration register high
    CAN_MOAR76L = 0x0000;        // load MO76 arbitration register low

  ///  Configuration of Message Object 76 acceptance mask:
  ///  - only accept receive frames with matching IDE bit
  ///  - acceptance mask 11-bit: 0x7FF

    CAN_MOAMR76H = 0x3FFF;       // load MO76 acceptance mask register high
    CAN_MOAMR76L = 0xFFFF;       // load MO76 acceptance mask register low

  ///  Configuration of Message Object 76 interrupt pointer:
  ///  - use message pending register 2 bit position 12

    CAN_MOIPR76H = 0x0000;       // load MO76 interrupt pointer register high
    CAN_MOIPR76L = 0x4C00;       // load MO76 interrupt pointer register low

  ///  Configuration of Message Object 76 FIFO/Gateway pointer:

    CAN_MOFGPR76H = 0x0000;      // load MO76 FIFO/gateway pointer register
                                 // high
    CAN_MOFGPR76L = 0x0000;      // load MO76 FIFO/gateway pointer register
                                 // low


  ///  Configuration of Message Object 76 Function control:
  ///  - this object is a STANDARD MESSAGE OBJECT
  ///  - 8 valid data bytes

    CAN_MOFCR76H = 0x0800;       // load MO76 function control register high
    CAN_MOFCR76L = 0x0000;       // load MO76 function control register low

  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 77:
  ///  -----------------------------------------------------------------------

  ///  General Configuration of the Message Object 77 :
  ///  - message object 77 is valid
  ///  - message object is used as receive object
  ///  - this message object is assigned to list 1 (node 0)

    CAN_MOCTR77H = 0x00A0;       // load MO77 control register high
    CAN_MOCTR77L = 0x0000;       // load MO77 control register low

  ///  Configuration of Message Object 77 Arbitration:
  ///  - priority class 1; transmit acceptance filtering is based on the list
  ///    order
  ///  - standard 11-bit identifier
  ///  - identifier 11-bit:      0x000

    CAN_MOAR77H = 0x4000;        // load MO77 arbitration register high
    CAN_MOAR77L = 0x0000;        // load MO77 arbitration register low

  ///  Configuration of Message Object 77 acceptance mask:
  ///  - only accept receive frames with matching IDE bit
  ///  - acceptance mask 11-bit: 0x7FF

    CAN_MOAMR77H = 0x3FFF;       // load MO77 acceptance mask register high
    CAN_MOAMR77L = 0xFFFF;       // load MO77 acceptance mask register low

  ///  Configuration of Message Object 77 interrupt pointer:
  ///  - use message pending register 2 bit position 13

    CAN_MOIPR77H = 0x0000;       // load MO77 interrupt pointer register high
    CAN_MOIPR77L = 0x4D00;       // load MO77 interrupt pointer register low

  ///  Configuration of Message Object 77 FIFO/Gateway pointer:

    CAN_MOFGPR77H = 0x0000;      // load MO77 FIFO/gateway pointer register
                                 // high
    CAN_MOFGPR77L = 0x0000;      // load MO77 FIFO/gateway pointer register
                                 // low


  ///  Configuration of Message Object 77 Function control:
  ///  - this object is a STANDARD MESSAGE OBJECT
  ///  - 8 valid data bytes

    CAN_MOFCR77H = 0x0800;       // load MO77 function control register high
    CAN_MOFCR77L = 0x0000;       // load MO77 function control register low

  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 78:
  ///  -----------------------------------------------------------------------

  ///  General Configuration of the Message Object 78 :
  ///  - message object 78 is valid
  ///  - message object is used as receive object
  ///  - this message object is assigned to list 1 (node 0)

    CAN_MOCTR78H = 0x00A0;       // load MO78 control register high
    CAN_MOCTR78L = 0x0000;       // load MO78 control register low

  ///  Configuration of Message Object 78 Arbitration:
  ///  - priority class 1; transmit acceptance filtering is based on the list
  ///    order
  ///  - standard 11-bit identifier
  ///  - identifier 11-bit:      0x000

    CAN_MOAR78H = 0x4000;        // load MO78 arbitration register high
    CAN_MOAR78L = 0x0000;        // load MO78 arbitration register low

  ///  Configuration of Message Object 78 acceptance mask:
  ///  - only accept receive frames with matching IDE bit
  ///  - acceptance mask 11-bit: 0x7FF

    CAN_MOAMR78H = 0x3FFF;       // load MO78 acceptance mask register high
    CAN_MOAMR78L = 0xFFFF;       // load MO78 acceptance mask register low

  ///  Configuration of Message Object 78 interrupt pointer:
  ///  - use message pending register 2 bit position 14

    CAN_MOIPR78H = 0x0000;       // load MO78 interrupt pointer register high
    CAN_MOIPR78L = 0x4E00;       // load MO78 interrupt pointer register low

  ///  Configuration of Message Object 78 FIFO/Gateway pointer:

    CAN_MOFGPR78H = 0x0000;      // load MO78 FIFO/gateway pointer register
                                 // high
    CAN_MOFGPR78L = 0x0000;      // load MO78 FIFO/gateway pointer register
                                 // low


  ///  Configuration of Message Object 78 Function control:
  ///  - this object is a STANDARD MESSAGE OBJECT
  ///  - 8 valid data bytes

    CAN_MOFCR78H = 0x0800;       // load MO78 function control register high
    CAN_MOFCR78L = 0x0000;       // load MO78 function control register low

  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 79:
  ///  -----------------------------------------------------------------------

  ///  General Configuration of the Message Object 79 :
  ///  - message object 79 is valid
  ///  - message object is used as receive object
  ///  - this message object is assigned to list 1 (node 0)

    CAN_MOCTR79H = 0x00A0;       // load MO79 control register high
    CAN_MOCTR79L = 0x0000;       // load MO79 control register low

  ///  Configuration of Message Object 79 Arbitration:
  ///  - priority class 1; transmit acceptance filtering is based on the list
  ///    order
  ///  - standard 11-bit identifier
  ///  - identifier 11-bit:      0x000

    CAN_MOAR79H = 0x4000;        // load MO79 arbitration register high
    CAN_MOAR79L = 0x0000;        // load MO79 arbitration register low

  ///  Configuration of Message Object 79 acceptance mask:
  ///  - only accept receive frames with matching IDE bit
  ///  - acceptance mask 11-bit: 0x7FF

    CAN_MOAMR79H = 0x3FFF;       // load MO79 acceptance mask register high
    CAN_MOAMR79L = 0xFFFF;       // load MO79 acceptance mask register low

  ///  Configuration of Message Object 79 interrupt pointer:
  ///  - use message pending register 2 bit position 15

    CAN_MOIPR79H = 0x0000;       // load MO79 interrupt pointer register high
    CAN_MOIPR79L = 0x4F00;       // load MO79 interrupt pointer register low

  ///  Configuration of Message Object 79 FIFO/Gateway pointer:

    CAN_MOFGPR79H = 0x0000;      // load MO79 FIFO/gateway pointer register
                                 // high
    CAN_MOFGPR79L = 0x0000;      // load MO79 FIFO/gateway pointer register
                                 // low


  ///  Configuration of Message Object 79 Function control:
  ///  - this object is a STANDARD MESSAGE OBJECT
  ///  - 8 valid data bytes

    CAN_MOFCR79H = 0x0800;       // load MO79 function control register high
    CAN_MOFCR79L = 0x0000;       // load MO79 function control register low

  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 80:
  ///  -----------------------------------------------------------------------
  ///  - message object 80 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 81:
  ///  -----------------------------------------------------------------------
  ///  - message object 81 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 82:
  ///  -----------------------------------------------------------------------
  ///  - message object 82 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 83:
  ///  -----------------------------------------------------------------------
  ///  - message object 83 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 84:
  ///  -----------------------------------------------------------------------
  ///  - message object 84 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 85:
  ///  -----------------------------------------------------------------------
  ///  - message object 85 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 86:
  ///  -----------------------------------------------------------------------
  ///  - message object 86 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 87:
  ///  -----------------------------------------------------------------------
  ///  - message object 87 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 88:
  ///  -----------------------------------------------------------------------
  ///  - message object 88 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 89:
  ///  -----------------------------------------------------------------------
  ///  - message object 89 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 90:
  ///  -----------------------------------------------------------------------
  ///  - message object 90 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 91:
  ///  -----------------------------------------------------------------------
  ///  - message object 91 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 92:
  ///  -----------------------------------------------------------------------
  ///  - message object 92 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 93:
  ///  -----------------------------------------------------------------------
  ///  - message object 93 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 94:
  ///  -----------------------------------------------------------------------
  ///  - message object 94 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 95:
  ///  -----------------------------------------------------------------------
  ///  - message object 95 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 96:
  ///  -----------------------------------------------------------------------
  ///  - message object 96 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 97:
  ///  -----------------------------------------------------------------------
  ///  - message object 97 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 98:
  ///  -----------------------------------------------------------------------
  ///  - message object 98 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 99:
  ///  -----------------------------------------------------------------------
  ///  - message object 99 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 100:
  ///  -----------------------------------------------------------------------
  ///  - message object 100 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 101:
  ///  -----------------------------------------------------------------------
  ///  - message object 101 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 102:
  ///  -----------------------------------------------------------------------
  ///  - message object 102 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 103:
  ///  -----------------------------------------------------------------------
  ///  - message object 103 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 104:
  ///  -----------------------------------------------------------------------
  ///  - message object 104 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 105:
  ///  -----------------------------------------------------------------------
  ///  - message object 105 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 106:
  ///  -----------------------------------------------------------------------
  ///  - message object 106 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 107:
  ///  -----------------------------------------------------------------------
  ///  - message object 107 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 108:
  ///  -----------------------------------------------------------------------
  ///  - message object 108 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 109:
  ///  -----------------------------------------------------------------------
  ///  - message object 109 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 110:
  ///  -----------------------------------------------------------------------
  ///  - message object 110 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 111:
  ///  -----------------------------------------------------------------------
  ///  - message object 111 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 112:
  ///  -----------------------------------------------------------------------
  ///  - message object 112 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 113:
  ///  -----------------------------------------------------------------------
  ///  - message object 113 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 114:
  ///  -----------------------------------------------------------------------
  ///  - message object 114 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 115:
  ///  -----------------------------------------------------------------------
  ///  - message object 115 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 116:
  ///  -----------------------------------------------------------------------
  ///  - message object 116 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 117:
  ///  -----------------------------------------------------------------------
  ///  - message object 117 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 118:
  ///  -----------------------------------------------------------------------
  ///  - message object 118 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 119:
  ///  -----------------------------------------------------------------------
  ///  - message object 119 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 120:
  ///  -----------------------------------------------------------------------
  ///  - message object 120 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 121:
  ///  -----------------------------------------------------------------------
  ///  - message object 121 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 122:
  ///  -----------------------------------------------------------------------
  ///  - message object 122 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 123:
  ///  -----------------------------------------------------------------------
  ///  - message object 123 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 124:
  ///  -----------------------------------------------------------------------
  ///  - message object 124 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 125:
  ///  -----------------------------------------------------------------------
  ///  - message object 125 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 126:
  ///  -----------------------------------------------------------------------
  ///  - message object 126 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 127:
  ///  -----------------------------------------------------------------------
  ///  - message object 127 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 128:
  ///  -----------------------------------------------------------------------
  ///  - message object 128 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 129:
  ///  -----------------------------------------------------------------------
  ///  - message object 129 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 130:
  ///  -----------------------------------------------------------------------
  ///  - message object 130 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 131:
  ///  -----------------------------------------------------------------------
  ///  - message object 131 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 132:
  ///  -----------------------------------------------------------------------
  ///  - message object 132 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 133:
  ///  -----------------------------------------------------------------------
  ///  - message object 133 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 134:
  ///  -----------------------------------------------------------------------
  ///  - message object 134 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 135:
  ///  -----------------------------------------------------------------------
  ///  - message object 135 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 136:
  ///  -----------------------------------------------------------------------
  ///  - message object 136 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 137:
  ///  -----------------------------------------------------------------------
  ///  - message object 137 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 138:
  ///  -----------------------------------------------------------------------
  ///  - message object 138 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 139:
  ///  -----------------------------------------------------------------------
  ///  - message object 139 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 140:
  ///  -----------------------------------------------------------------------
  ///  - message object 140 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 141:
  ///  -----------------------------------------------------------------------
  ///  - message object 141 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 142:
  ///  -----------------------------------------------------------------------
  ///  - message object 142 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 143:
  ///  -----------------------------------------------------------------------
  ///  - message object 143 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 144:
  ///  -----------------------------------------------------------------------
  ///  - message object 144 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 145:
  ///  -----------------------------------------------------------------------
  ///  - message object 145 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 146:
  ///  -----------------------------------------------------------------------
  ///  - message object 146 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 147:
  ///  -----------------------------------------------------------------------
  ///  - message object 147 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 148:
  ///  -----------------------------------------------------------------------
  ///  - message object 148 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 149:
  ///  -----------------------------------------------------------------------
  ///  - message object 149 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 150:
  ///  -----------------------------------------------------------------------
  ///  - message object 150 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 151:
  ///  -----------------------------------------------------------------------
  ///  - message object 151 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 152:
  ///  -----------------------------------------------------------------------
  ///  - message object 152 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 153:
  ///  -----------------------------------------------------------------------
  ///  - message object 153 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 154:
  ///  -----------------------------------------------------------------------
  ///  - message object 154 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 155:
  ///  -----------------------------------------------------------------------
  ///  - message object 155 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 156:
  ///  -----------------------------------------------------------------------
  ///  - message object 156 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 157:
  ///  -----------------------------------------------------------------------
  ///  - message object 157 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 158:
  ///  -----------------------------------------------------------------------
  ///  - message object 158 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 159:
  ///  -----------------------------------------------------------------------
  ///  - message object 159 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 160:
  ///  -----------------------------------------------------------------------
  ///  - message object 160 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 161:
  ///  -----------------------------------------------------------------------
  ///  - message object 161 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 162:
  ///  -----------------------------------------------------------------------
  ///  - message object 162 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 163:
  ///  -----------------------------------------------------------------------
  ///  - message object 163 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 164:
  ///  -----------------------------------------------------------------------
  ///  - message object 164 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 165:
  ///  -----------------------------------------------------------------------
  ///  - message object 165 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 166:
  ///  -----------------------------------------------------------------------
  ///  - message object 166 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 167:
  ///  -----------------------------------------------------------------------
  ///  - message object 167 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 168:
  ///  -----------------------------------------------------------------------
  ///  - message object 168 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 169:
  ///  -----------------------------------------------------------------------
  ///  - message object 169 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 170:
  ///  -----------------------------------------------------------------------
  ///  - message object 170 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 171:
  ///  -----------------------------------------------------------------------
  ///  - message object 171 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 172:
  ///  -----------------------------------------------------------------------
  ///  - message object 172 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 173:
  ///  -----------------------------------------------------------------------
  ///  - message object 173 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 174:
  ///  -----------------------------------------------------------------------
  ///  - message object 174 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 175:
  ///  -----------------------------------------------------------------------
  ///  - message object 175 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 176:
  ///  -----------------------------------------------------------------------
  ///  - message object 176 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 177:
  ///  -----------------------------------------------------------------------
  ///  - message object 177 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 178:
  ///  -----------------------------------------------------------------------
  ///  - message object 178 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 179:
  ///  -----------------------------------------------------------------------
  ///  - message object 179 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 180:
  ///  -----------------------------------------------------------------------
  ///  - message object 180 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 181:
  ///  -----------------------------------------------------------------------
  ///  - message object 181 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 182:
  ///  -----------------------------------------------------------------------
  ///  - message object 182 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 183:
  ///  -----------------------------------------------------------------------
  ///  - message object 183 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 184:
  ///  -----------------------------------------------------------------------
  ///  - message object 184 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 185:
  ///  -----------------------------------------------------------------------
  ///  - message object 185 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 186:
  ///  -----------------------------------------------------------------------
  ///  - message object 186 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 187:
  ///  -----------------------------------------------------------------------
  ///  - message object 187 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 188:
  ///  -----------------------------------------------------------------------
  ///  - message object 188 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 189:
  ///  -----------------------------------------------------------------------
  ///  - message object 189 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 190:
  ///  -----------------------------------------------------------------------
  ///  - message object 190 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 191:
  ///  -----------------------------------------------------------------------
  ///  - message object 191 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 192:
  ///  -----------------------------------------------------------------------
  ///  - message object 192 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 193:
  ///  -----------------------------------------------------------------------
  ///  - message object 193 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 194:
  ///  -----------------------------------------------------------------------
  ///  - message object 194 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 195:
  ///  -----------------------------------------------------------------------
  ///  - message object 195 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 196:
  ///  -----------------------------------------------------------------------
  ///  - message object 196 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 197:
  ///  -----------------------------------------------------------------------
  ///  - message object 197 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 198:
  ///  -----------------------------------------------------------------------
  ///  - message object 198 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 199:
  ///  -----------------------------------------------------------------------
  ///  - message object 199 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 200:
  ///  -----------------------------------------------------------------------
  ///  - message object 200 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 201:
  ///  -----------------------------------------------------------------------
  ///  - message object 201 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 202:
  ///  -----------------------------------------------------------------------
  ///  - message object 202 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 203:
  ///  -----------------------------------------------------------------------
  ///  - message object 203 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 204:
  ///  -----------------------------------------------------------------------
  ///  - message object 204 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 205:
  ///  -----------------------------------------------------------------------
  ///  - message object 205 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 206:
  ///  -----------------------------------------------------------------------
  ///  - message object 206 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 207:
  ///  -----------------------------------------------------------------------
  ///  - message object 207 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 208:
  ///  -----------------------------------------------------------------------
  ///  - message object 208 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 209:
  ///  -----------------------------------------------------------------------
  ///  - message object 209 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 210:
  ///  -----------------------------------------------------------------------
  ///  - message object 210 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 211:
  ///  -----------------------------------------------------------------------
  ///  - message object 211 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 212:
  ///  -----------------------------------------------------------------------
  ///  - message object 212 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 213:
  ///  -----------------------------------------------------------------------
  ///  - message object 213 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 214:
  ///  -----------------------------------------------------------------------
  ///  - message object 214 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 215:
  ///  -----------------------------------------------------------------------
  ///  - message object 215 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 216:
  ///  -----------------------------------------------------------------------
  ///  - message object 216 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 217:
  ///  -----------------------------------------------------------------------
  ///  - message object 217 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 218:
  ///  -----------------------------------------------------------------------
  ///  - message object 218 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 219:
  ///  -----------------------------------------------------------------------
  ///  - message object 219 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 220:
  ///  -----------------------------------------------------------------------
  ///  - message object 220 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 221:
  ///  -----------------------------------------------------------------------
  ///  - message object 221 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 222:
  ///  -----------------------------------------------------------------------
  ///  - message object 222 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 223:
  ///  -----------------------------------------------------------------------
  ///  - message object 223 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 224:
  ///  -----------------------------------------------------------------------
  ///  - message object 224 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 225:
  ///  -----------------------------------------------------------------------
  ///  - message object 225 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 226:
  ///  -----------------------------------------------------------------------
  ///  - message object 226 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 227:
  ///  -----------------------------------------------------------------------
  ///  - message object 227 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 228:
  ///  -----------------------------------------------------------------------
  ///  - message object 228 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 229:
  ///  -----------------------------------------------------------------------
  ///  - message object 229 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 230:
  ///  -----------------------------------------------------------------------
  ///  - message object 230 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 231:
  ///  -----------------------------------------------------------------------
  ///  - message object 231 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 232:
  ///  -----------------------------------------------------------------------
  ///  - message object 232 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 233:
  ///  -----------------------------------------------------------------------
  ///  - message object 233 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 234:
  ///  -----------------------------------------------------------------------
  ///  - message object 234 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 235:
  ///  -----------------------------------------------------------------------
  ///  - message object 235 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 236:
  ///  -----------------------------------------------------------------------
  ///  - message object 236 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 237:
  ///  -----------------------------------------------------------------------
  ///  - message object 237 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 238:
  ///  -----------------------------------------------------------------------
  ///  - message object 238 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 239:
  ///  -----------------------------------------------------------------------
  ///  - message object 239 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 240:
  ///  -----------------------------------------------------------------------
  ///  - message object 240 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 241:
  ///  -----------------------------------------------------------------------
  ///  - message object 241 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 242:
  ///  -----------------------------------------------------------------------
  ///  - message object 242 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 243:
  ///  -----------------------------------------------------------------------
  ///  - message object 243 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 244:
  ///  -----------------------------------------------------------------------
  ///  - message object 244 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 245:
  ///  -----------------------------------------------------------------------
  ///  - message object 245 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 246:
  ///  -----------------------------------------------------------------------
  ///  - message object 246 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 247:
  ///  -----------------------------------------------------------------------
  ///  - message object 247 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 248:
  ///  -----------------------------------------------------------------------
  ///  - message object 248 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 249:
  ///  -----------------------------------------------------------------------
  ///  - message object 249 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 250:
  ///  -----------------------------------------------------------------------
  ///  - message object 250 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 251:
  ///  -----------------------------------------------------------------------
  ///  - message object 251 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 252:
  ///  -----------------------------------------------------------------------
  ///  - message object 252 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 253:
  ///  -----------------------------------------------------------------------
  ///  - message object 253 is not valid
  ///  -----------------------------------------------------------------------
  ///  Configuration of Message Object 254:
  ///  -----------------------------------------------------------------------
  ///  - message object 254 is not valid

  ///  -----------------------------------------------------------------------
  ///  Configuration of Service Request Nodes 0 - 15:
  ///  -----------------------------------------------------------------------
  ///  SRN0 service request node configuration:
  ///  - SRN0 interrupt priority level (ILVL) = 15
  ///  - SRN0 interrupt group level (GLVL) = 0
  ///  - SRN0 group priority extension (GPX) = 0

  CAN_0IC        =  0x007C;

  ///  Use PEC channel 4 for CAN INT 0:
  ///  - normal interrupt
  ///  - pointers are not modified
  ///  - transfer a word
  ///  - service End of PEC interrrupt by a EOP interrupt node is disabled
  ///  - channel link mode is disabled

  PECC4          =  0x0000;      // load PECC4 control register


  ///  SRN1 service request node configuration:
  ///  - SRN1 interrupt priority level (ILVL) = 14
  ///  - SRN1 interrupt group level (GLVL) = 0
  ///  - SRN1 group priority extension (GPX) = 0

  CAN_1IC        =  0x0078;

  ///  Use PEC channel 0 for CAN INT 1:
  ///  - normal interrupt
  ///  - pointers are not modified
  ///  - transfer a word
  ///  - service End of PEC interrrupt by a EOP interrupt node is disabled
  ///  - channel link mode is disabled

  PECC0          =  0x0000;      // load PECC0 control register



  //   -----------------------------------------------------------------------
  //   Start the CAN Nodes:
  //   -----------------------------------------------------------------------

  ///  - ------------- CAN_NCR0 ----------------------------------------------

    CAN_NCR0 &= ~ (uword) 0x0041; // reset INIT and CCE
  ///  - ------------- CAN_NCR1 ----------------------------------------------

    CAN_NCR1 &= ~ (uword) 0x0041; // reset INIT and CCE



  // USER CODE BEGIN (Init,3)

  // USER CODE END

} //  End of function CAN_vInit

//****************************************************************************
// @Function      void CAN_vTransmit(ubyte ubObjNr)
//
//----------------------------------------------------------------------------
// @Description   This function triggers the CAN controller to send the
//                selected message.
//                If the selected message object is a TRANSMIT OBJECT then
//                this function triggers the sending of a data frame. If
//                however the selected message object is a RECEIVE OBJECT
//                this function triggers the sending of a remote frame.
//
//----------------------------------------------------------------------------
// @Returnvalue   None
//
//----------------------------------------------------------------------------
// @Parameters    ubObjNr:
//                Number of the message object (0-254)
//
//----------------------------------------------------------------------------
// @Date          2010-6-3
//
//****************************************************************************

// USER CODE BEGIN (Transmit,1)

// USER CODE END

void CAN_vTransmit(ubyte ubObjNr)
{

  CAN_HWOBJ[ubObjNr].uwMOCTRH = 0x0720;  // set TXRQ,TXEN0,TXEN1,MSGVAL

} //  End of function CAN_vTransmit


void CAN_Send(unsigned char* sendBuffer)
{
	int i;
	for(i=0;i<8;i++)
		CAN_HWOBJ[1].ubData[i]=sendBuffer[i];

    CAN_vTransmit(1);
}

//****************************************************************************
// @Macro         CAN_viSRN0()
//
//----------------------------------------------------------------------------
// @Description
//
//----------------------------------------------------------------------------
// @Returnvalue
//
//----------------------------------------------------------------------------
// @Parameters    None
//
//----------------------------------------------------------------------------
// @Date          2010-6-3
//
//****************************************************************************

// USER CODE BEGIN (SRN0,1)

// USER CODE END

_interrupt(CAN_SRN0INT)  void CAN_viSRN0(void)
{

  uword uwSRN0ObjHandler;

  // USER CODE BEGIN (SRN0,2)

  // USER CODE END

  CAN_MSIMASKH = 0x0000;          // set message index mask register
  CAN_MSIMASKL = 0x0003;

  while (CAN_MSID0 != 0x0020)
  {
    switch(CAN_MSID0){

    case 0: // message object 0 interrupt

      uwSRN0ObjHandler = CAN_HWOBJ[0].uwMOCTRL;
      m_pInterruptProcess_2(CAN_HWOBJ[0].ubData);

      if(uwSRN0ObjHandler & MOSTAT_RXPND)          // if message object 0 receive interrupt
      {
            // USER CODE BEGIN (SRN0_OBJ0,1)

            // USER CODE END

        if(uwSRN0ObjHandler & MOSTAT_NEWDAT)       // if NEWDAT is set
        {
            // USER CODE BEGIN (SRN0_OBJ0,2)

            // USER CODE END

          if (uwSRN0ObjHandler & MOSTAT_MSGLST)    // if MSGLST is set
          {
            // Indicates that the CAN controller has stored a new
            // message into this object, while NEWDAT was still set,
            // ie. the previously stored message is lost.

            // USER CODE BEGIN (SRN0_OBJ0,3)

            // USER CODE END

          }
          else
          {
            // The CAN controller has stored a new message into this object.

            // USER CODE BEGIN (SRN0_OBJ0,4)

            // USER CODE END
          }

        }

        CAN_HWOBJ[0].uwMOCTRL = MOSTAT_RST_MNR;            // reset MSGLST + NEWDAT + RXPND

      }  // End of RXPND0


      CAN_MSPND0H = ~(uword) 0x0000; // reset PND bit
      CAN_MSPND0L = ~(uword) 0x0001;

      // USER CODE BEGIN (SRN0_OBJ0,7)

      // USER CODE END

      break;


    case 1: // message object 1 interrupt

      uwSRN0ObjHandler = CAN_HWOBJ[1].uwMOCTRL;

      if(uwSRN0ObjHandler & MOSTAT_TXPND)                      // if message object 1 transmit interrupt
      {

        // The transmission of the last message object
        // was successful.

            // USER CODE BEGIN (SRN0_OBJ1,6)

            // USER CODE END


        CAN_HWOBJ[1].uwMOCTRL = MOSTAT_RST_NT;               // reset NEWDAT + TXPND

      }  // End of TXPND1

      CAN_MSPND0H = ~(uword) 0x0000; // reset PND bit
      CAN_MSPND0L = ~(uword) 0x0002;

      // USER CODE BEGIN (SRN0_OBJ1,7)

      // USER CODE END

      break;

    default:

      // USER CODE BEGIN (SRN0,3)

      // USER CODE END

      break;

    }  // end switch

    // USER CODE BEGIN (SRN0,7)

    // USER CODE END

  }  // end of while CAN_MSID


  // USER CODE BEGIN (SRN0,15)

  // USER CODE END

} //  End of function CAN_viSRN0


//****************************************************************************
// @Macro         CAN_viSRN1()
//
//----------------------------------------------------------------------------
// @Description
//
//----------------------------------------------------------------------------
// @Returnvalue
//
//----------------------------------------------------------------------------
// @Parameters    None
//
//----------------------------------------------------------------------------
// @Date          2010-6-3
//
//****************************************************************************

// USER CODE BEGIN (SRN1,1)

// USER CODE END

_interrupt(CAN_SRN1INT)  void CAN_viSRN1(void)
{

  uword uwSRN1ObjHandler;

  // USER CODE BEGIN (SRN1,2)

  // USER CODE END

  CAN_MSIMASKH = 0x0000;          // set message index mask register
  CAN_MSIMASKL = 0x000C;

  while (CAN_MSID0 != 0x0020)
  {
    switch(CAN_MSID0){

    case 2: // message object 2 interrupt

      uwSRN1ObjHandler = CAN_HWOBJ[2].uwMOCTRL;

      if(uwSRN1ObjHandler & MOSTAT_RXPND)          // if message object 2 receive interrupt
      {
            // USER CODE BEGIN (SRN1_OBJ2,1)

            // USER CODE END

        if(uwSRN1ObjHandler & MOSTAT_NEWDAT)       // if NEWDAT is set
        {
            // USER CODE BEGIN (SRN1_OBJ2,2)

            // USER CODE END

          if (uwSRN1ObjHandler & MOSTAT_MSGLST)    // if MSGLST is set
          {
            // Indicates that the CAN controller has stored a new
            // message into this object, while NEWDAT was still set,
            // ie. the previously stored message is lost.

            // USER CODE BEGIN (SRN1_OBJ2,3)

            // USER CODE END

          }
          else
          {
            // The CAN controller has stored a new message into this object.

            // USER CODE BEGIN (SRN1_OBJ2,4)

            // USER CODE END
          }

        }

        CAN_HWOBJ[2].uwMOCTRL = MOSTAT_RST_MNR;            // reset MSGLST + NEWDAT + RXPND

      }  // End of RXPND2


      CAN_MSPND0H = ~(uword) 0x0000; // reset PND bit
      CAN_MSPND0L = ~(uword) 0x0004;

      // USER CODE BEGIN (SRN1_OBJ2,7)

      // USER CODE END

      break;


    case 3: // message object 3 interrupt

      uwSRN1ObjHandler = CAN_HWOBJ[3].uwMOCTRL;

      if(uwSRN1ObjHandler & MOSTAT_TXPND)                      // if message object 3 transmit interrupt
      {

        // The transmission of the last message object
        // was successful.

            // USER CODE BEGIN (SRN1_OBJ3,6)

            // USER CODE END


        CAN_HWOBJ[3].uwMOCTRL = MOSTAT_RST_NT;               // reset NEWDAT + TXPND

      }  // End of TXPND3

      CAN_MSPND0H = ~(uword) 0x0000; // reset PND bit
      CAN_MSPND0L = ~(uword) 0x0008;

      // USER CODE BEGIN (SRN1_OBJ3,7)

      // USER CODE END

      break;

    default:

      // USER CODE BEGIN (SRN1,3)

      // USER CODE END

      break;

    }  // end switch

    // USER CODE BEGIN (SRN1,7)

    // USER CODE END

  }  // end of while CAN_MSID


  // USER CODE BEGIN (SRN1,15)

  // USER CODE END

} //  End of function CAN_viSRN1

void CAN_ChangeSendID(unsigned int sendID)
{
	  ///  -----------------------------------------------------------------------
	  ///  Configuration of Message Object 1:
	  ///  -----------------------------------------------------------------------

	  ///  General Configuration of the Message Object 1 :
	  ///  - message object 1 is valid
	  ///  - message object is used as transmit object
	  ///  - this message object is assigned to list 1 (node 0)

	    CAN_MOCTR1H = 0x0EA8;        // load MO1 control register high
	    CAN_MOCTR1L = 0x0000;        // load MO1 control register low

	  ///  Configuration of Message Object 1 Arbitration:
	  ///  - priority class 1; transmit acceptance filtering is based on the list
	  ///    order
	  ///  - standard 11-bit identifier


	    CAN_MOAR1H = 0x4000+(sendID<<2);         // load MO1 arbitration register high
	    CAN_MOAR1L = 0x0000;         // load MO1 arbitration register low

	  ///  Configuration of Message Object 1 Data:

	    CAN_MODATA1HH = 0x0807;      // load MO1 Data Bytes(DB7 & DB6)
	    CAN_MODATA1HL = 0x0605;      // load MO1 Data Bytes(DB5 & DB4)
	    CAN_MODATA1LH = 0x0403;      // load MO1 Data Bytes(DB3 & DB2)
	    CAN_MODATA1LL = 0x0201;      // load MO1 Data Bytes(DB1 & DB0)

	  ///  Configuration of Message Object 1 acceptance mask:
	  ///  - only accept receive frames with matching IDE bit
	  ///  - acceptance mask 11-bit: 0x7FF

	    CAN_MOAMR1H = 0x3FFF;        // load MO1 acceptance mask register high
	    CAN_MOAMR1L = 0xFFFF;        // load MO1 acceptance mask register low

	  ///  Configuration of Message Object 1 interrupt pointer:
	  ///  - use message pending register 0 bit position 1

	    CAN_MOIPR1H = 0x0000;        // load MO1 interrupt pointer register high
	    CAN_MOIPR1L = 0x0100;        // load MO1 interrupt pointer register low

	  ///  Configuration of Message Object 1 FIFO/Gateway pointer:

	    CAN_MOFGPR1H = 0x0000;       // load MO1 FIFO/gateway pointer register
	                                 // high
	    CAN_MOFGPR1L = 0x0000;       // load MO1 FIFO/gateway pointer register low


	  ///  Configuration of Message Object 1 Function control:
	  ///  - this object is a STANDARD MESSAGE OBJECT
	  ///  - 8 valid data bytes

	    CAN_MOFCR1H = 0x0800;        // load MO1 function control register high
	    CAN_MOFCR1L = 0x0000;        // load MO1 function control register low
}

// USER CODE BEGIN (CAN_General,10)

// USER CODE END

