#include "Event.h"
#include "System.h"
#include "CAN.h"
#include "EEPROM.h"
#define EVENT_MAP(__event,__process) case __event: __process();break;
extern unsigned char g_sampleIsEnable;
extern unsigned char m_boardID;
extern unsigned char m_isCheckEmptyBattery;
extern unsigned char m_isStartEqual;
extern unsigned int g_uiReleaseFlag;
extern unsigned char g_sampleShareDataBuffer[64];
extern unsigned char  g_CAN_ReceiveBuffer[8];
extern unsigned char g_sampleShareDataTemp[64];
long long g_llErrorFlag;

static unsigned int Event_CRC(unsigned char *ptr, unsigned char len)
{
	unsigned short crc;
	unsigned char i;

	crc=0;
	while(len--!=0)
	{
		for(i=0x80; i!=0; i/=2)
		{
			if((crc&0x8000)!=0)
				{crc*=2; crc^=0x1021;}
			else
				crc*=2;
			if((*ptr&i)!=0)
				crc^=0x1021;
		}
		ptr++;
	}
	return(crc);
}
void Event_vCRCShareMemery()
{
	unsigned char ubCrc[2];
	unsigned int uiCrc;

	uiCrc=Event_CRC(g_sampleShareDataBuffer,60);
	ubCrc[0]=uiCrc&0xff;
	ubCrc[1]=uiCrc>>8;
	g_sampleShareDataBuffer[60]=ubCrc[0];
	g_sampleShareDataBuffer[61]=ubCrc[1];
}
unsigned int Event_uiIsCRCShareDataTemp()
{
	unsigned char ubCrc[2];
	unsigned int uiCrc;
	unsigned int uiComplete=0;
	uiCrc=Event_CRC(g_sampleShareDataTemp,60);
	ubCrc[0]=uiCrc&0xff;
	ubCrc[1]=uiCrc>>8;

	if(g_sampleShareDataTemp[60]==ubCrc[0])
	{
		if(g_sampleShareDataTemp[61]==ubCrc[1])
		{
			uiComplete=1;
		}
	}
	return uiComplete;
}
unsigned int Event_uiIsCRCEEPROM(unsigned int startAddress,unsigned int count)
{
	unsigned char* EEPROM_ReturnBuffer=g_sampleShareDataTemp;
	if(count<4)
		return EVENT_NOT_CRC;
	EEPROM_ReadEEPROM(startAddress,EEPROM_ReturnBuffer,count);

	return Event_uiIsCRCShareDataTemp();
}

static void Event_OnCheckEmptyBattery() 
{
  	  m_isCheckEmptyBattery=1; 
}

static void Event_OnSetDataCRC()
{
	unsigned int crc=Event_CRC(g_sampleShareDataTemp,64);
	unsigned int _crc,_crcHigh,_crcLow;
	unsigned char returnResult[8];
  unsigned int uiSendID;

	_crcHigh=g_CAN_ReceiveBuffer[4];
	_crcLow=g_CAN_ReceiveBuffer[3];
	_crcHigh<<=8;
	_crc=_crcLow+_crcHigh;
	if(_crc==crc)
	{
		returnResult[0]=1;
		memcpy(g_sampleShareDataBuffer,g_sampleShareDataTemp,64);
	}
	else
	{
		returnResult[0]=0;
	}
	
  switch(m_boardID) 
  {
    case 1:
      uiSendID = BMS_ID_SAMPLE_1_WRITE_RETURN;
    break;
    case 2:
      uiSendID = BMS_ID_SAMPLE_2_WRITE_RETURN;
    break;
    case 3:
      uiSendID = BMS_ID_SAMPLE_3_WRITE_RETURN;
    break;
    case 4:
      uiSendID = BMS_ID_SAMPLE_4_WRITE_RETURN;
    break;
    case 5:
      uiSendID = BMS_ID_SAMPLE_5_WRITE_RETURN;
    break;
    case 6:
      uiSendID = BMS_ID_SAMPLE_6_WRITE_RETURN;
    break;
    case 7:
      uiSendID = BMS_ID_SAMPLE_7_WRITE_RETURN;
    break;
  }
  
	CAN_Send(uiSendID,returnResult);

}

static void Event_OnSetDataAll_1()
{
	memcpy(&g_sampleShareDataTemp[0],&g_CAN_ReceiveBuffer[4],4);
}
static void Event_OnSetDataAll_2()
{
	memcpy(&g_sampleShareDataTemp[1*4],&g_CAN_ReceiveBuffer[4],4);
}
static void Event_OnSetDataAll_3()
{
	memcpy(&g_sampleShareDataTemp[2*4],&g_CAN_ReceiveBuffer[4],4);
}
static void Event_OnSetDataAll_4()
{
	memcpy(&g_sampleShareDataTemp[3*4],&g_CAN_ReceiveBuffer[4],4);
}
static void Event_OnSetDataAll_5()
{
	memcpy(&g_sampleShareDataTemp[4*4],&g_CAN_ReceiveBuffer[4],4);
}
static void Event_OnSetDataAll_6()
{
	memcpy(&g_sampleShareDataTemp[5*4],&g_CAN_ReceiveBuffer[4],4);
}
static void Event_OnSetDataAll_7()
{
	memcpy(&g_sampleShareDataTemp[6*4],&g_CAN_ReceiveBuffer[4],4);
}
static void Event_OnSetDataAll_8()
{
	memcpy(&g_sampleShareDataTemp[7*4],&g_CAN_ReceiveBuffer[4],4);
}
static void Event_OnSetDataAll_9()
{
	memcpy(&g_sampleShareDataTemp[8*4],&g_CAN_ReceiveBuffer[4],4);
}
static void Event_OnSetDataAll_10()
{
	memcpy(&g_sampleShareDataTemp[9*4],&g_CAN_ReceiveBuffer[4],4);
}
static void Event_OnSetDataAll_11()
{
	memcpy(&g_sampleShareDataTemp[10*4],&g_CAN_ReceiveBuffer[4],4);
}
static void Event_OnSetDataAll_12()
{
	memcpy(&g_sampleShareDataTemp[11*4],&g_CAN_ReceiveBuffer[4],4);
}
static void Event_OnSetDataAll_13()
{
	memcpy(&g_sampleShareDataTemp[12*4],&g_CAN_ReceiveBuffer[4],4);
}
static void Event_OnSetDataAll_14()
{
	memcpy(&g_sampleShareDataTemp[13*4],&g_CAN_ReceiveBuffer[4],4);
}
static void Event_OnSetDataAll_15()
{
	memcpy(&g_sampleShareDataTemp[14*4],&g_CAN_ReceiveBuffer[4],4);
}
static void Event_OnSetDataAll_16()
{
	memcpy(&g_sampleShareDataTemp[15*4],&g_CAN_ReceiveBuffer[4],4);
}


static void Event_OnShareMemeryToEEPROM()
{
	unsigned int uiEEPROM_WriteAddress=EEPROM_SAMPLESHARE_MEMERYHEAD;
	unsigned int uiEEPROM_WriteLength=EEPROM_SAMPLESHARE_MEMERYLENGTH;
	unsigned char* ubEEPROM_WriteBuffer=g_sampleShareDataBuffer;
	unsigned int uiComplete=0;

	int iErrorCounter=0;
	int i;
	Event_vCRCShareMemery();
again:
	EEPROM_WriteEEPROM(uiEEPROM_WriteAddress,ubEEPROM_WriteBuffer,uiEEPROM_WriteLength);

	uiComplete=Event_uiIsCRCEEPROM(uiEEPROM_WriteAddress,uiEEPROM_WriteLength);

	if(uiComplete!=1)
	{
		if(iErrorCounter++<3)
			goto again;
		else
		{
			g_llErrorFlag|=ERROR_SHAREMEMERY_TO_EEPROM;
		}
	}  

}

static void Event_OnEEPROMToShareMemery()
{ 
	unsigned int EEPROM_ReadAddress=EEPROM_SAMPLESHARE_MEMERYHEAD;
	unsigned int EEPROM_ReadLength=EEPROM_SAMPLESHARE_MEMERYLENGTH;
	unsigned char* EEPROM_ReturnBuffer=g_sampleShareDataTemp;
	unsigned int uiComplet=0;
	int iErrorCounter=0;
	int i;
	if(EEPROM_ReadLength<4)
		return;
again:
	EEPROM_ReadEEPROM(EEPROM_ReadAddress,EEPROM_ReturnBuffer,EEPROM_ReadLength);

	uiComplet= Event_uiIsCRCShareDataTemp();
	if(uiComplet!=1)
	{
		if(iErrorCounter++<3)
			goto again;
		else
		{
			g_llErrorFlag|=ERROR_EEPROM_TO_SHAREMEMERY;
		}
	}
	else
	{
		for(i=0;i<64;i++)
		{
			g_sampleShareDataBuffer[i]=g_sampleShareDataTemp[i];
		}
	}  
}

static void Event_OnEmptyError()
{
	g_llErrorFlag=0;
} 

static void Event_OnStartEqual() 
{
  unsigned int uiReleaseFlagHigh;
  unsigned int uiReleaseFlagLow;
  
  if(m_isStartEqual==0) 
	{ 
    m_isStartEqual=1;
    g_sampleIsEnable=0;
	  uiReleaseFlagHigh=g_CAN_ReceiveBuffer[4];
	  uiReleaseFlagHigh<<=8;
	  
	  uiReleaseFlagLow=g_CAN_ReceiveBuffer[3];
	  
	  g_uiReleaseFlag=uiReleaseFlagHigh+uiReleaseFlagLow;
	  
    if(g_uiReleaseFlag!=0)
      System_ReleaseTask();
    else
      m_isStartEqual=0;

	}  
}
static void Event_OnGetDataAll()
{
	unsigned int crc=Event_CRC(g_sampleShareDataBuffer,64);
	unsigned char returnResult[8];
  unsigned int uiSendID_End;
  unsigned int uiSendID_Data_1,
                uiSendID_Data_2,
                uiSendID_Data_3,
                uiSendID_Data_4,
                uiSendID_Data_5,
                uiSendID_Data_6,
                uiSendID_Data_7,
                uiSendID_Data_8;
  
   switch(m_boardID) 
  {
    case 1:
    uiSendID_End = BMS_ID_SAMPLE_1_DATA_END;
    uiSendID_Data_1=BMS_ID_SAMPLE_1_DATA_1;
    uiSendID_Data_2=BMS_ID_SAMPLE_1_DATA_2;
    uiSendID_Data_3=BMS_ID_SAMPLE_1_DATA_3;
    uiSendID_Data_4=BMS_ID_SAMPLE_1_DATA_4;
    uiSendID_Data_5=BMS_ID_SAMPLE_1_DATA_5;
    uiSendID_Data_6=BMS_ID_SAMPLE_1_DATA_6;
    uiSendID_Data_7=BMS_ID_SAMPLE_1_DATA_7;
    uiSendID_Data_8=BMS_ID_SAMPLE_1_DATA_8;
    break;
    case 2:
    uiSendID_End = BMS_ID_SAMPLE_2_DATA_END;
    uiSendID_Data_1=BMS_ID_SAMPLE_2_DATA_1;
    uiSendID_Data_2=BMS_ID_SAMPLE_2_DATA_2;
    uiSendID_Data_3=BMS_ID_SAMPLE_2_DATA_3;
    uiSendID_Data_4=BMS_ID_SAMPLE_2_DATA_4;
    uiSendID_Data_5=BMS_ID_SAMPLE_2_DATA_5;
    uiSendID_Data_6=BMS_ID_SAMPLE_2_DATA_6;
    uiSendID_Data_7=BMS_ID_SAMPLE_2_DATA_7;
    uiSendID_Data_8=BMS_ID_SAMPLE_2_DATA_8;
    break;
    case 3:
    uiSendID_End = BMS_ID_SAMPLE_3_DATA_END;
    break;
    case 4:
    uiSendID_End = BMS_ID_SAMPLE_4_DATA_END;
    break;
    case 5:
    uiSendID_End = BMS_ID_SAMPLE_5_DATA_END;
    break;
    case 6:
    uiSendID_End = BMS_ID_SAMPLE_6_DATA_END;
    break;
    case 7:
    uiSendID_End = BMS_ID_SAMPLE_7_DATA_END;
    break;
  }	
	CAN_Send(uiSendID_Data_1,&g_sampleShareDataBuffer[0]);
	CAN_Send(uiSendID_Data_2,&g_sampleShareDataBuffer[1*8]);
	CAN_Send(uiSendID_Data_3,&g_sampleShareDataBuffer[2*8]);
	CAN_Send(uiSendID_Data_4,&g_sampleShareDataBuffer[3*8]);
	CAN_Send(uiSendID_Data_5,&g_sampleShareDataBuffer[4*8]);
	CAN_Send(uiSendID_Data_6,&g_sampleShareDataBuffer[5*8]);
	CAN_Send(uiSendID_Data_7,&g_sampleShareDataBuffer[6*8]);
	CAN_Send(uiSendID_Data_8,&g_sampleShareDataBuffer[7*8]);

	returnResult[0]=crc&0xff;
	returnResult[1]=crc>>8;


	CAN_Send(uiSendID_End,returnResult);

}
void Event_Progress(unsigned int event) 
{
  switch(event) 
  {
  	EVENT_MAP(WM_CHECK_EMPTY_BATTERY,Event_OnCheckEmptyBattery);
    EVENT_MAP(WM_WRITE_DATA_CRC,Event_OnSetDataCRC);
    EVENT_MAP(WM_WRITE_DATA_ALL_1,Event_OnSetDataAll_1);
    EVENT_MAP(WM_WRITE_DATA_ALL_2,Event_OnSetDataAll_2);
    EVENT_MAP(WM_WRITE_DATA_ALL_3,Event_OnSetDataAll_3);
    EVENT_MAP(WM_WRITE_DATA_ALL_4,Event_OnSetDataAll_4);
    EVENT_MAP(WM_WRITE_DATA_ALL_5,Event_OnSetDataAll_5);
    EVENT_MAP(WM_WRITE_DATA_ALL_6,Event_OnSetDataAll_6);
    EVENT_MAP(WM_WRITE_DATA_ALL_7,Event_OnSetDataAll_7);
    EVENT_MAP(WM_WRITE_DATA_ALL_8,Event_OnSetDataAll_8);
    EVENT_MAP(WM_WRITE_DATA_ALL_9,Event_OnSetDataAll_9);
    EVENT_MAP(WM_WRITE_DATA_ALL_10,Event_OnSetDataAll_10);
    EVENT_MAP(WM_WRITE_DATA_ALL_11,Event_OnSetDataAll_11);
    EVENT_MAP(WM_WRITE_DATA_ALL_12,Event_OnSetDataAll_12);
    EVENT_MAP(WM_WRITE_DATA_ALL_13,Event_OnSetDataAll_13);
    EVENT_MAP(WM_WRITE_DATA_ALL_14,Event_OnSetDataAll_14);
    EVENT_MAP(WM_WRITE_DATA_ALL_15,Event_OnSetDataAll_15);
    EVENT_MAP(WM_WRITE_DATA_ALL_16,Event_OnSetDataAll_16);
    EVENT_MAP(WM_START_EQUAL,Event_OnStartEqual);
    EVENT_MAP(WM_READ_DATA_ALL,Event_OnGetDataAll);
    EVENT_MAP(WM_SHARE_MEMERY_TO_EEPROM,Event_OnShareMemeryToEEPROM);
    EVENT_MAP(WM_EEPROM_TO_SHARE_MEMERY,Event_OnEEPROMToShareMemery);
    EVENT_MAP(WM_EMPTY_ERROR,Event_OnEmptyError);
  }
}