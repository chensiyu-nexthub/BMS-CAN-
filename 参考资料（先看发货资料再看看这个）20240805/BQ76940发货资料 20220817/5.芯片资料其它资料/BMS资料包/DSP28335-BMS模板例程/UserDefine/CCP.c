/*
 * CCP.c
 *
 *  Created on: 2010-8-3
 *      Author: Administrator
 */
#ifndef CCP_FILE
#define CCP_FILE
#endif

#ifdef CCP_FILE
#include "CCP.h"
#include "string.h"
//#include "stdio.h"
UINT16 g_uiCCPCANSendID=0x101;

struct STCCP g_stCCP;
static UINT32 g_ulAddressFrom;
static UINT32 g_ulAddressTo;
static Uint16 g_pubAddressFrom[4];
static Uint16 g_pubAddressTo[4];
extern struct STAddress  g_stAddress;
#define CCP_Transmit_CTR 				(g_stCCP.pubCANTransmit[2])
#define CCP_Transmit_PID 				(g_stCCP.pubCANTransmit[0])
#define CCP_Transmit_ERR 				(g_stCCP.pubCANTransmit[1])
#define CCP_Transmit_ID_Length 			(g_stCCP.pubCANTransmit[3])
#define CCP_Transmit_Type 				(g_stCCP.pubCANTransmit[4])
#define CCP_Transmit_Availabitity_Mask 	(g_stCCP.pubCANTransmit[5])
#define CCP_Transmit_Protection_Mask 	(g_stCCP.pubCANTransmit[6])
#define CCP_Transmit_Address_Head		&(g_stCCP.pubCANReceive[4])
#define CCP_Transmit_Extension			(g_stCCP.pubCANReceive[3])
#define CCP_Transmit_DAQ_Size 			(g_stCCP.pubCANTransmit[3])
#define CCP_Transmit_First_PID			(g_stCCP.pubCANTransmit[4])
#define CCP_Transmit  					(g_stCCP.pubCANTransmit)

#define EVENT_MAP(__event,__process) case __event: __process();break;
#define CCP_StationAddress 			g_stCCP.uiStationAddress
#define CCP_LoadStationAddress()    g_stCCP.uiStationAddress=g_stCCP.pubCANReceive[3];g_stCCP.uiStationAddress<<=8;g_stCCP.uiStationAddress+=g_stCCP.pubCANReceive[2];

#define CCP_Session_Status    		g_stCCP.ubSessionStatus
#define CCP_User_SessionStatus		g_stCCP.ubUserSessionStatus
#define CCP_SendStatus				g_stCCP.ubSendStatus

#define CCP_Receive_ResourceMask g_stCCP.pubCANReceive[2]
#define CCP_Protection_Status g_stCCP.ubProtectionStatus
#define CCP_Transmit_ProtectionStatus (g_stCCP.pubCANTransmit[3])

#define CCP_Receive_CMD  			(g_stCCP.pubCANReceive[0])
#define CCP_Receive_CTR  			(g_stCCP.pubCANReceive[1])
#define CCP_Receive_MTA_Number  	(g_stCCP.pubCANReceive[2])
#define CCP_Receive_MTA_Extension 	(g_stCCP.pubCANReceive[3])
#define CCP_Receive_Address_0    	(g_stCCP.pubCANReceive[4])
#define CCP_Receive_Address_1    	(g_stCCP.pubCANReceive[5])
#define CCP_Receive_Address_2    	(g_stCCP.pubCANReceive[6])
#define CCP_Receive_Address_3    	(g_stCCP.pubCANReceive[7])
#define CCP_Receive_Data_Size		(g_stCCP.pubCANReceive[2])
#define CCP_Receive_Data_Head		&(g_stCCP.pubCANReceive[3])
#define CCP_Receive_DAQ_List     	(g_stCCP.pubCANReceive[2])
#define CCP_Receive_DAQ_Select      (g_stCCP.pubCANReceive[2])
#define CCP_Receive_ODT_Select		(g_stCCP.pubCANReceive[3])
#define CCP_Receive_Element_Select	(g_stCCP.pubCANReceive[4])
#define CCP_Receive_Daq_Mode		(g_stCCP.pubCANReceive[2])
#define CCP_Receive_DAQ_Select_2	(g_stCCP.pubCANReceive[3])
#define CCP_Receive_Last_ODT		(g_stCCP.pubCANReceive[4])
#define CCP_Receive_EventChannel	(g_stCCP.pubCANReceive[5])
#define CCP_Receive_RatePrescalerHead	&(g_stCCP.pubCANReceive[6])
#define CCP_Receive_DisconnectCmd   (g_stCCP.pubCANReceive[2])
#define CCP_Receive_StationAddressHead  &(g_stCCP.pubCANReceive[4])
#define CCP_Receive_Session_Status  (g_stCCP.pubCANReceive[2])

#define CCP_MTA                		g_stCCP.ppubMTA
#define CCP_MTA_Current       		CCP_MTA[CCP_Receive_MTA_Number]
#define CCP_CurrentDaq				g_stCCP.ubCurrentDaq
#define CCP_CurrentOdt				g_stCCP.ubCurrentOdt
#define CCP_ODT_Entry        		(g_stCCP.pstOdtEntry)
#define CCP_DaqList              	(g_stCCP.pstDaqList)
#define CCP_DAQ_Stop	0
#define CCP_DAQ_Start	1
#define CCP_DAQ_Prepare	2
#define CCP_StationID g_stCCP.pbStationID
//PGM(Memery programming),DAQ(Data Acquistion),CAL(Calibration)
#define CCP_Resource_PGM   0x40
#define CCP_Resource_DAQ   2
#define CCP_Resource_CAL   1



struct STAddress g_stAddress;
static UINT32 g_ulAddress;

#ifdef ADDRESS_TEST
Uint16 g_pubAddess[4]={1,0,2,3};

void Address_vAddTest()
{
	g_stAddress.ubShortAdd=0x10;
	g_stAddress.pubAddress=g_pubAddess;
	Address_vAdd();
}
#endif



void Address_vAdd()
{
	Pub_To_Ul_HF(g_ulAddress,g_stAddress.pubAddress);
	g_ulAddress+=g_stAddress.ubShortAdd;
	Ul_To_Pub_HF(g_stAddress.pubAddress,g_ulAddress);
}
void Address_vSub()
{
	Pub_To_Ul_HF(g_ulAddress,g_stAddress.pubAddress);
	g_ulAddress-=g_stAddress.ubShortSub;
	Ul_To_Pub_HF(g_stAddress.pubAddress,g_ulAddress);
}


struct STMemery g_stMemery;
#ifdef __DZ60
#undef __DZ60
#endif
#define __XC2000  1
#define __DZ60    2
#define __CPU     __DZ60

void Memery_vMove()
{
	UINT32 index;
#if(__CPU==__XC2000)
	UINT32 ulFrom;
	UINT32 ulTo;
	__huge Uint16* hpubFrom;
	__huge Uint16* hpubTo;

	Pub_To_Ul_HF(ulFrom,g_stMemery.pubFrom);
	hpubFrom=ulFrom;
	Pub_To_Ul_HF(ulTo,g_stMemery.pubTo);
	hpubTo=ulTo;
	for(index=0;index<g_stMemery.ulLength;index++)
		hpubTo[index]=hpubFrom[index];
#endif
#if(__CPU==__DZ60)
	UINT32 ulFrom;
	UINT32 ulTo;
	Uint16* pubFrom;
	Uint16* pubTo;

	Pub_To_Ul_HF(ulFrom,g_stMemery.pubFrom);
	pubFrom=(Uint16*)(ulFrom&0xffff);
	Pub_To_Ul_HF(ulTo,g_stMemery.pubTo);
	pubTo=(Uint16*)(ulTo&0xffff);
	for(index=0;index<g_stMemery.ulLength;index++)
		pubTo[index]=pubFrom[index];
#endif
}




void CCP_vStopAllDaq()
{
	Uint16 i;

	for (i=0;i<CCP_MAX_DAQ;i++)
		CCP_DaqList[i].ubFlags = 0;
	CCP_Session_Status &= ~SS_RUN;
}




static void CCP_vStopDaq ()
{
  Uint16 i;

  if (CCP_Receive_DAQ_List>=CCP_MAX_DAQ)
	  return;
  CCP_DaqList[CCP_Receive_DAQ_List].ubFlags = 0;

  /* check if all DAQ lists are stopped */
  for (i=0;i<CCP_MAX_DAQ;i++)
  {
    if (CCP_DaqList[CCP_Receive_DAQ_List].ubFlags&DAQ_FLAG_START)
    	return;
  }

  CCP_Session_Status &= ~SS_RUN;
}

static void CCP_vClearDaqList()
{
	  Uint16* pubAddress;
	  UINT16 uiLength;
	  Uint16 i;

	  if (CCP_Receive_DAQ_List>=CCP_MAX_DAQ)
	  {
		  CCP_Transmit_DAQ_Size=0;
		  return;
	  }

	  /* Clear this daq list to zero */
	  uiLength=sizeof(struct STDaqList);
	  pubAddress=(Uint16*)&(CCP_DaqList[CCP_Receive_DAQ_List]);
	  for(i=0;i<uiLength;i++)
	  {
		  pubAddress[i]=0;
	  }

	  /* @@@@ Not DAQ list specific */
	  CCP_Session_Status |= SS_DAQ;

	  CCP_CurrentDaq=0;
	  CCP_CurrentOdt=0;

	  CCP_Transmit_DAQ_Size=CCP_MAX_ODT;
}

void CCP_vTransmit()
{
//	BMS_vCANSend(g_uiCCPCANSendID,CCP_Transmit);
}

void CCP_vGetSeed()
{

}
void CCP_vCommandConnect()
{
	UINT16 uiStationAddress;

	Pub_To_Ui_LF(uiStationAddress,(Uint16*)&(g_stCCP.pubCANReceive[2]));
	if(uiStationAddress!=CCP_STATION_ADDR&&
			uiStationAddress!=CCP_BROADCAST_STATION_ADDR)
	{
	      if (CCP_Session_Status&SS_CONNECTED)
	      {
	    	  CCP_Session_Status &= ~SS_CONNECTED;
	    	  CCP_Session_Status |= SS_TMP_DISCONNECTED;
	      }
	      return;
	}

	if (!(CCP_Session_Status&SS_TMP_DISCONNECTED))
	{
	 CCP_vStopAllDaq();
	 CCP_SendStatus = 0; /* Clear all transmission flags */
	}

	CCP_Session_Status |= SS_CONNECTED;
	CCP_Session_Status &= ~SS_TMP_DISCONNECTED;

	CCP_Transmit_PID=0xff;
	CCP_Transmit_ERR=0;//CRC_OK;
	CCP_Transmit_CTR=CCP_Receive_CTR;

	CCP_vTransmit();
}

void CCP_vCommandSetMTA()
{
	if((CCP_Session_Status&SS_CONNECTED)==0)
		return;

	Pub4_Move(CCP_MTA_Current,((Uint16*)&CCP_Receive_Address_0));

/*	pubMTA[0]=CCP_Receive_Address_0;
	pubMTA[1]=CCP_Receive_Address_1;
	pubMTA[2]=CCP_Receive_Address_2;
	pubMTA[3]=CCP_Receive_Address_3;*/

	//CCP_MTA_Current+=CCP_Receive_MTA_Extension;

	CCP_Transmit_PID=0xff;
	CCP_Transmit_ERR=0;//CRC_OK;
	CCP_Transmit_CTR=CCP_Receive_CTR;

	CCP_vTransmit();
}


extern struct STMemery g_stMemery;

void CCP_vCommandDNLoad()
{
	Uint16 *pubCCPTransmitAddress;
	Uint16 *pubMTA0;

	if((CCP_Session_Status&SS_CONNECTED)==0)
		return;

    if (CCP_Protection_Status&CCP_Resource_CAL)
    {
    	CCP_Transmit_ERR = CRC_ACCESS_DENIED;
    }
    else
    {
    	CCP_Transmit_ERR=CRC_OK;

    	g_ulAddressFrom=(UINT32)CCP_Receive_Data_Head;
    	Ul_To_Pub_HF(g_pubAddressFrom,g_ulAddressFrom);
    	MEMERY_MOVE(CCP_MTA[0],g_pubAddressFrom,CCP_Receive_Data_Size);

    	ADDRESS_ADD(CCP_MTA[0],CCP_Receive_Data_Size);
    	CCP_Transmit_Extension=0;
/*
    	pubCCPTransmitAddress=CCP_Transmit_Address_Head;

    	pubMTA0=&CCP_MTA[0];*/
    	Pub4_Move(((Uint16*)CCP_Transmit_Address_Head),CCP_MTA[0]);
    /*	pubCCPTransmitAddress[0]=pubMTA0[0];
    	pubCCPTransmitAddress[1]=pubMTA0[1];
    	pubCCPTransmitAddress[2]=pubMTA0[2];
    	pubCCPTransmitAddress[3]=pubMTA0[3];*/
    }

	CCP_Transmit_PID=0xff;
	CCP_Transmit_CTR=CCP_Receive_CTR;

	CCP_vTransmit();
}
#ifdef CCP_Receive_Data_Size
#undef CCP_Receive_Data_Size
#endif
#ifdef CCP_Transmit_DataHead
#undef CCP_Transmit_DataHead
#endif

#define CCP_Receive_Data_Size		(g_stCCP.pubCANReceive[2])
#define CCP_Transmit_DataHead       &(g_stCCP.pubCANTransmit[3])

void CCP_vCommandUpLoad()
{
	Uint16 *pubCCPTransmitAddress;
	Uint16 *pubMTA0;

	if((CCP_Session_Status&SS_CONNECTED)==0)
		return;

    if (CCP_Protection_Status&CCP_Resource_CAL)
    {
    	CCP_Transmit_ERR = CRC_ACCESS_DENIED;
    }
    else
    {
    	CCP_Transmit_ERR=CRC_OK;
/*
    	MEMERY_From= CCP_MTA[0];
    	MEMERY_To= CCP_Transmit_DataHead;
    	MEMERY_Length=CCP_Receive_Data_Size;
    	Memery_vMove();
    	*/
    	g_ulAddressTo=(UINT32)CCP_Transmit_DataHead;
    	Ul_To_Pub_HF(g_pubAddressTo,g_ulAddressTo);
    	MEMERY_MOVE(g_pubAddressTo,CCP_MTA[0],CCP_Receive_Data_Size);
    	ADDRESS_ADD(CCP_MTA[0],CCP_Receive_Data_Size);
    }

	CCP_Transmit_PID=0xff;
	CCP_Transmit_CTR=CCP_Receive_CTR;

	CCP_vTransmit();
}
void CCP_vCommandTest()
{
	if((CCP_Session_Status&SS_CONNECTED)==0)
		return;
}
void CCP_vCommandStartStop()
{
	Uint16 i;
	Uint16* puiRatePrescaler;

	puiRatePrescaler=(Uint16 *)CCP_Receive_RatePrescalerHead;
	if((CCP_Session_Status&SS_CONNECTED)==0)
		return;
#ifdef KEY_SEED
    if ((CCP_Protection_Status&CCP_Resource_DAQ)==0)
    {
    	CCP_Transmit_ERR = CRC_ACCESS_DENIED;
    	goto Exit;
    }
	if ((CCP_Session_Status&SS_DAQ)==0)
	{
		CCP_Transmit_ERR = CRC_DAQ_INIT_REQUEST;
		goto Exit;
	}
#endif
	if (CCP_Receive_DAQ_Select_2>=CCP_MAX_DAQ)
	{
	  CCP_Transmit_ERR=CRC_CMD_SYNTAX;
	  goto Exit;
	}

	switch(CCP_Receive_Daq_Mode)
	{
	case CCP_DAQ_Stop:

		CCP_DaqList[CCP_Receive_DAQ_Select_2].ubFlags = 0;
		CCP_Session_Status &= ~SS_RUN;

		break;
	case CCP_DAQ_Start:

		CCP_DaqList[CCP_Receive_DAQ_Select_2].ubEventChannel =
			CCP_Receive_EventChannel;
		 if (*puiRatePrescaler==0)
			 *puiRatePrescaler = 1;
		 CCP_DaqList[CCP_Receive_DAQ_Select_2].uiPrescaler = *puiRatePrescaler;
		 CCP_DaqList[CCP_Receive_DAQ_Select_2].uiCycle = 1;
		 CCP_DaqList[CCP_Receive_DAQ_Select_2].ubLast = CCP_Receive_Last_ODT;
		 CCP_DaqList[CCP_Receive_DAQ_Select_2].ubFlags = DAQ_FLAG_START;
		 CCP_Session_Status |= SS_RUN;

		break;
	case CCP_DAQ_Prepare:

		CCP_DaqList[CCP_Receive_DAQ_Select_2].ubEventChannel =
			CCP_Receive_EventChannel;
		 if (*puiRatePrescaler==0)
			 *puiRatePrescaler = 1;
		 CCP_DaqList[CCP_Receive_DAQ_Select_2].uiPrescaler = *puiRatePrescaler;
		 CCP_DaqList[CCP_Receive_DAQ_Select_2].uiCycle = 1;
		 CCP_DaqList[CCP_Receive_DAQ_Select_2].ubLast = CCP_Receive_Last_ODT;
		 CCP_DaqList[CCP_Receive_DAQ_Select_2].ubFlags = DAQ_FLAG_PREPARED;

		break;
	default:
		CCP_Transmit_ERR = CRC_CMD_SYNTAX;
		goto Exit;
		break;
	}

Exit:
	CCP_Transmit_PID=0xff;
	CCP_Transmit_CTR=CCP_Receive_CTR;

	CCP_vTransmit();
}
void CCP_vCommandDisconnect()
{
	Uint16 i;

	if((CCP_Session_Status&SS_CONNECTED)==0)
		return;

	CCP_Session_Status &= ~SS_CONNECTED;

	if (CCP_Receive_DisconnectCmd==0x00)
	{ /* Temporary */
	  CCP_Session_Status |= SS_TMP_DISCONNECTED;
	}
	else
	{           /* End of session */
		for(i=0;i<CCP_MAX_DAQ;i++)
		{
			CCP_DaqList[i].ubFlags = 0;
		}
		CCP_Session_Status &= ~SS_RUN;
		CCP_Protection_Status = 0; /* Clear Protection Status */
	}

	CCP_Transmit_PID=0xff;
	CCP_Transmit_CTR=CCP_Receive_CTR;

	CCP_vTransmit();
}
void CCP_vCommandStartStopAll()
{
	Uint16 i;
	UINT16* puiRatePrescaler;

	if((CCP_Session_Status&SS_CONNECTED)==0)
		return;

#ifdef KEY_SEED
	if ((CCP_Session_Status&SS_DAQ)==0)
	{
		CCP_Transmit_ERR = CRC_DAQ_INIT_REQUEST;
		goto Exit;
	}
#endif

	switch(CCP_Receive_Daq_Mode)
	{
	case CCP_DAQ_Stop:
		for(i=0;i<CCP_MAX_DAQ;i++)
		{
			CCP_DaqList[i].ubFlags = 0;
		}
		CCP_Session_Status &= ~SS_RUN;
		break;
	case CCP_DAQ_Start:

		for (i=0;i<CCP_MAX_DAQ;i++)
		{
			if (CCP_DaqList[i].ubFlags==DAQ_FLAG_PREPARED)
				CCP_DaqList[i].ubFlags= DAQ_FLAG_START;
		}
		CCP_Session_Status |= SS_RUN;

		break;

	default:
		CCP_Transmit_ERR = CRC_CMD_SYNTAX;
		goto Exit;
		break;
	}

Exit:
	CCP_Transmit_PID=0xff;
	CCP_Transmit_CTR=CCP_Receive_CTR;

	CCP_vTransmit();
}

void CCP_vCommandShortUpLoad()
{
	Uint16* pubUpLoad;
	UINT32* plAddress;

	if((CCP_Session_Status&SS_CONNECTED)==0)
		return;
    if (CCP_Protection_Status&CCP_Resource_CAL)
    {
    	CCP_Transmit_ERR = CRC_ACCESS_DENIED;
    }
    else
    {
    	CCP_Transmit_ERR=CRC_OK;
/*
        pubUpLoad=(Uint16*)(&MEMERY_From);
        plAddress=(UINT32*)(&CCP_Receive_Address_0);
        (*plAddress)+=CCP_Receive_MTA_Extension;
        pubUpLoad[0]=CCP_Receive_Address_0;
        pubUpLoad[1]=CCP_Receive_Address_1;
        pubUpLoad[2]=CCP_Receive_Address_2;
        pubUpLoad[3]=CCP_Receive_Address_3;
        */
        g_ulAddressTo=(UINT32)CCP_Transmit_DataHead;
        Ul_To_Pub_HF(g_pubAddressTo,g_ulAddressTo);
        MEMERY_MOVE(g_pubAddressTo,((Uint16*)&CCP_Receive_Address_0),CCP_Receive_Data_Size);
 /*
        MEMERY_To= CCP_Transmit_DataHead;
    	MEMERY_Length=CCP_Receive_Data_Size;
    	Memery_vMove();*/
    }

	CCP_Transmit_PID=0xff;
	CCP_Transmit_CTR=CCP_Receive_CTR;

	CCP_vTransmit();

}
void CCP_vCommandDAQSize()
{
	UINT32* pulIdentifier;
	if((CCP_Session_Status&SS_CONNECTED)==0)
		return;

	CCP_Transmit_ERR=CRC_OK;
	CCP_vStopDaq();
	pulIdentifier=(UINT32*)(&CCP_Receive_Address_0);

	if((*pulIdentifier)!=CCP_DTO_ID)
		CCP_Transmit_ERR=CRC_CMD_SYNTAX;

	CCP_vClearDaqList();

	CCP_Transmit_First_PID=CCP_MAX_ODT*CCP_Receive_DAQ_List;

	CCP_Transmit_PID=0xff;
	CCP_Transmit_CTR=CCP_Receive_CTR;

	CCP_vTransmit();
}

void CCP_vCommandDAQPtr()
{
	if((CCP_Session_Status&SS_CONNECTED)==0)
		return;

    if (CCP_Receive_DAQ_Select>=CCP_MAX_DAQ||
    		CCP_Receive_ODT_Select>=CCP_MAX_ODT||
    		CCP_Receive_Element_Select>7)
    {
    	CCP_Transmit_ERR = CRC_CMD_SYNTAX;
    	CCP_ODT_Entry = 0;
     }
    else
    {
    	CCP_Transmit_ERR = CRC_OK;
    	CCP_ODT_Entry =
    		&CCP_DaqList[CCP_Receive_DAQ_Select].pstOdt[CCP_Receive_ODT_Select][CCP_Receive_Element_Select];
    }
	CCP_Transmit_PID=0xff;
	CCP_Transmit_CTR=CCP_Receive_CTR;

	CCP_vTransmit();
}

void CCP_vCommandWriteDAQ()
{
	//Uint16* pubAddress;
	UINT32* pulFrom;
	UINT32* pulTo;
	if((CCP_Session_Status&SS_CONNECTED)==0)
		return;

    if (CCP_Receive_Data_Size!=4&&CCP_Receive_Data_Size!=2&&CCP_Receive_Data_Size!=1)
    {
    	CCP_Transmit_ERR = CRC_CMD_SYNTAX;
    	goto Exit;
    }
    if(CCP_ODT_Entry==0)
    {
    	CCP_Transmit_ERR = CRC_CMD_SYNTAX;
    	goto Exit;
    }

    Pub4_Move(CCP_ODT_Entry->pubAddress,(Uint16*)&CCP_Receive_Address_0);

    CCP_ODT_Entry->ubLength=CCP_Receive_Data_Size;

    Exit:
	CCP_Transmit_PID=0xff;
	CCP_Transmit_CTR=CCP_Receive_CTR;

	CCP_vTransmit();
}

static Uint16 g_ubResourceMask;

void CCP_vCommandExchangeID()
{
	if((CCP_Session_Status&SS_CONNECTED)==0)
		return;
	CCP_Transmit_PID=0xff;
	CCP_Transmit_ERR=CRC_OK;
	CCP_Transmit_CTR=CCP_Receive_CTR;
	CCP_Transmit_ID_Length=(Uint16)strlen((const char *)CCP_StationID);
	CCP_Transmit_Type=0;

	CCP_Transmit_Availabitity_Mask=CCP_Resource_DAQ;
	CCP_Transmit_Availabitity_Mask+=CCP_Resource_CAL;

	CCP_Transmit_Protection_Mask=CCP_Resource_DAQ;
	CCP_Transmit_Protection_Mask+=CCP_Resource_CAL;

	CCP_vTransmit();
}
void CCP_vCommandCCPVersion()
{
	if((CCP_Session_Status&SS_CONNECTED)==0)
		return;
}

#ifdef CCP_Receive_Data_Size
#undef CCP_Receive_Data_Size
#endif
#ifdef CCP_Receive_Data_Head
#undef CCP_Receive_Data_Head
#endif
#define CCP_Receive_Data_Size		6
#define CCP_Receive_Data_Head		&(g_stCCP.pubCANReceive[2])
void CCP_vCommandDNLoad_6()
{
	Uint16 *pubCCPTransmitAddress;
	Uint16 *pubMTA0;

	if((CCP_Session_Status&SS_CONNECTED)==0)
		return;

    if (CCP_Protection_Status&CCP_Resource_CAL)
    {
    	CCP_Transmit_ERR = CRC_ACCESS_DENIED;
    }
    else
    {
    	CCP_Transmit_ERR=CRC_OK;
/*
    	MEMERY_From=CCP_Receive_Data_Head;
    	MEMERY_To=CCP_MTA[0];
    	MEMERY_Length=CCP_Receive_Data_Size;
    	Memery_vMove();*/

    	g_ulAddressFrom=(UINT32)CCP_Receive_Data_Head;
    	Ul_To_Pub_HF(g_pubAddressFrom,g_ulAddressFrom);
    	MEMERY_MOVE(CCP_MTA[0],g_pubAddressFrom,CCP_Receive_Data_Size);

    	ADDRESS_ADD(CCP_MTA[0],CCP_Receive_Data_Size);
    	//CCP_MTA[0]+=CCP_Receive_Data_Size;
    	CCP_Transmit_Extension=0;

    	Pub4_Move(CCP_Transmit_Address_Head,CCP_MTA[0]);
    	/*
    	pubCCPTransmitAddress=CCP_Transmit_Address_Head;
    	pubMTA0=CCP_MTA[0];

    	pubCCPTransmitAddress[0]=pubMTA0[0];
    	pubCCPTransmitAddress[1]=pubMTA0[1];
    	pubCCPTransmitAddress[2]=pubMTA0[2];
    	pubCCPTransmitAddress[3]=pubMTA0[3];*/
    }

	CCP_Transmit_PID=0xff;
	CCP_Transmit_CTR=CCP_Receive_CTR;

	CCP_vTransmit();
}
void CCP_vCommandGetCalPage()
{
	if((CCP_Session_Status&SS_CONNECTED)==0)
		return;
}
void CCP_vCommandSetSStatus()
{
	if((CCP_Session_Status&SS_CONNECTED)==0)
		return;
    /* Set Resume and Store mode in SessionStatus */
	CCP_Session_Status &= ~(SS_STORE|SS_RESUME);
	CCP_Session_Status |= (CCP_Receive_Session_Status&(SS_STORE|SS_RESUME));

    /* Save as UserSessionStatus */
	CCP_User_SessionStatus = CCP_Receive_Session_Status;

	CCP_Transmit_ERR=CRC_OK;

	CCP_Transmit_PID=0xff;
	CCP_Transmit_CTR=CCP_Receive_CTR;

	CCP_vTransmit();
}
void CCP_vCommandGetSStatus()
{
	if((CCP_Session_Status&SS_CONNECTED)==0)
		return;
}
void CCP_vCommandBuildCheckSum()
{
	if((CCP_Session_Status&SS_CONNECTED)==0)
		return;
}
void CCP_vCommandClearMemery()
{
	if((CCP_Session_Status&SS_CONNECTED)==0)
		return;
}
void CCP_vCommandSetCalPage()
{
	if((CCP_Session_Status&SS_CONNECTED)==0)
		return;

}
void CCP_vCommandGetSeed()
{
	if((CCP_Session_Status&SS_CONNECTED)==0)
		return;
	CCP_Transmit_PID=0xff;
	CCP_Transmit_ERR=CRC_OK;
	CCP_Transmit_CTR=CCP_Receive_CTR;

	if(CCP_Protection_Status&CCP_Receive_ResourceMask)
		CCP_Transmit_ProtectionStatus=1;
	else
		CCP_Transmit_ProtectionStatus=0;
	//Get Seed not define.
	CCP_vGetSeed();

	CCP_vTransmit();
}

void CCP_vCommandUnLock()
{
	if((CCP_Session_Status&SS_CONNECTED)==0)
		return;
	CCP_Transmit_PID=0xff;
	CCP_Transmit_ERR=0;//CRC_OK;
	CCP_Transmit_CTR=CCP_Receive_CTR;
    /* Check key */
	//CCP_Protection_Status |= ccpUnlock(&com[2]); /* Reset the appropriate resource protection mask bit */
	CCP_Transmit_ProtectionStatus = CCP_Protection_Status; /* Current Protection Status */
}
void CCP_vCommandProgram()
{
	if((CCP_Session_Status&SS_CONNECTED)==0)
		return;
}
void CCP_vCommandMoveMemery()
{
	if((CCP_Session_Status&SS_CONNECTED)==0)
		return;
}
void CCP_vCommandDiagService()
{
	if((CCP_Session_Status&SS_CONNECTED)==0)
		return;
}
void CCP_vCommandActionService()
{
	if((CCP_Session_Status&SS_CONNECTED)==0)
		return;
}
void CCP_vCommandProgram_6()
{
	if((CCP_Session_Status&SS_CONNECTED)==0)
		return;
}
void CCP_vCommandProgramPrepare()
{
	if((CCP_Session_Status&SS_CONNECTED)==0)
		return;
}
void CCP_vCommandProgramStart()
{
	if((CCP_Session_Status&SS_CONNECTED)==0)
		return;
}
void CCP_vCommand()
{
	switch(CCP_Receive_CMD)
	{
		EVENT_MAP(CC_CONNECT,		CCP_vCommandConnect);
		EVENT_MAP(CC_SET_MTA,		CCP_vCommandSetMTA);
		EVENT_MAP(CC_DNLOAD,		CCP_vCommandDNLoad);
		EVENT_MAP(CC_UPLOAD,		CCP_vCommandUpLoad);
		EVENT_MAP(CC_TEST,			CCP_vCommandTest);
		EVENT_MAP(CC_START_STOP,	CCP_vCommandStartStop);
		EVENT_MAP(CC_DISCONNECT,	CCP_vCommandDisconnect);
		EVENT_MAP(CC_START_STOP_ALL,CCP_vCommandStartStopAll);
		EVENT_MAP(CC_SHORT_UPLOAD,	CCP_vCommandShortUpLoad);
		EVENT_MAP(CC_GET_DAQ_SIZE,	CCP_vCommandDAQSize);
		EVENT_MAP(CC_SET_DAQ_PTR,	CCP_vCommandDAQPtr);
		EVENT_MAP(CC_WRITE_DAQ,		CCP_vCommandWriteDAQ);
		EVENT_MAP(CC_EXCHANGE_ID,	CCP_vCommandExchangeID);
		EVENT_MAP(CC_GET_CCP_VERSION,CCP_vCommandCCPVersion);
		EVENT_MAP(CC_DNLOAD_6,		CCP_vCommandDNLoad_6);
		EVENT_MAP(CC_GET_CAL_PAGE,	CCP_vCommandGetCalPage);
		EVENT_MAP(CC_SET_S_STATUS,	CCP_vCommandSetSStatus);
		EVENT_MAP(CC_GET_S_STATUS,	CCP_vCommandGetSStatus);
		EVENT_MAP(CC_BUILD_CHKSUM,	CCP_vCommandBuildCheckSum);
		EVENT_MAP(CC_CLEAR_MEMORY,	CCP_vCommandClearMemery);
		EVENT_MAP(CC_SET_CAL_PAGE,	CCP_vCommandSetCalPage);
		EVENT_MAP(CC_GET_SEED,		CCP_vCommandGetSeed);
		EVENT_MAP(CC_UNLOCK,		CCP_vCommandUnLock);
		EVENT_MAP(CC_PROGRAM,		CCP_vCommandProgram);
		EVENT_MAP(CC_MOVE_MEMORY,	CCP_vCommandMoveMemery);
		EVENT_MAP(CC_DIAG_SERVICE,	CCP_vCommandDiagService);
		EVENT_MAP(CC_ACTION_SERVICE,CCP_vCommandActionService);
		EVENT_MAP(CC_PROGRAM_6,		CCP_vCommandProgram_6);
		EVENT_MAP(CC_PROGRAM_PREPARE,CCP_vCommandProgramPrepare);
		EVENT_MAP(CC_PROGRAM_START,	CCP_vCommandProgramStart);
	}
}

#define Element_Length   	CCP_DaqList[i].pstOdt[j][k].ubLength
#define Element_Address  	CCP_DaqList[i].pstOdt[j][k].pubAddress
#define ODT_Last			CCP_DaqList[i].ubLast

void CCP_vDAQTransmit()
{
	Uint16 i,j,k;
	Uint16 ubIndex;

	if((CCP_Session_Status & SS_RUN)==0)
		return;

	for(i=0;i<CCP_MAX_DAQ;i++)
	{
		if(CCP_DaqList[i].ubFlags&DAQ_FLAG_START)
		{
			//CCP_DaqList[i].ubFlags&=~DAQ_FLAG_START;
			for(j=0;j<=ODT_Last;j++)
			{
				CCP_Transmit_PID=CCP_MAX_DAQ*i+j;
				ubIndex=1;

				for(k=0;k<7;k++)
				{
					//switch(Element_Length)
					//{
					if(Element_Length==0)
						goto Transmit;
					if(ubIndex+Element_Length>8)
							goto Transmit;
					
					g_ulAddressTo=(UINT32)(&CCP_Transmit[ubIndex]);
					Ul_To_Pub_HF(g_pubAddressTo,g_ulAddressTo);
          Pub4_Move(g_pubAddressFrom, Element_Address);
					MEMERY_MOVE(g_pubAddressTo,g_pubAddressFrom,Element_Length);
					ubIndex+=Element_Length;
					//}
				}
Transmit:
				CCP_vTransmit();

			}
		}
	}
}
#endif
