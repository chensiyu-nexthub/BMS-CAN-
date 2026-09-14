#include "ADC.h"
#include "MC9S08DZ60.h"
#include "SPI.h"
static UINT16 g_puiDataBuffer[11];
static UINT8 g_ubADCInitFlag=0;

void ADC_vInit() 
{
   g_ubADCInitFlag=1;
}
UINT16* ADC_puiConvertAll() 
{
	ubyte channel=0;
	UINT16 sendValue;
	ubyte external=0xc; //0b1100,16bits and MSB data format.
  UINT16 receiveValue;
	int isFirstTime=1;
		
  if(g_ubADCInitFlag!=1)
    ADC_vInit();
  
	for(channel=0;channel<12;channel++)//We use loop for 12 times,and get 11 channel converted data.
	{

		sendValue=(channel<<12)|external<<8;

		receiveValue=SPI_TransmitWord(sendValue);

		if(isFirstTime==1)
		{
			isFirstTime=0;
		}
		else 
			g_puiDataBuffer[channel-1]=receiveValue>>4;

	}  
	return g_puiDataBuffer;
}
UINT16 ADC_uiConvert(UINT8 channel) 
{
	UINT16 sendValue;
	ubyte external=0xc; //0b1100,16bits and MSB data format.
  UINT16 receiveValue;
  
  if(g_ubADCInitFlag!=1)
    ADC_vInit();
	sendValue=(channel<<12)|external<<8;

	SPI_uiTransmitWord(sendValue);
	ADC_vDelay(0xff);
	receiveValue=SPI_uiTransmitWord(sendValue)>>4;

	return receiveValue;
}

void ADC_vDelay(UINT16 max) 
{
  UINT16 i;
  for(i=0;i<max;i++)
  ;
}