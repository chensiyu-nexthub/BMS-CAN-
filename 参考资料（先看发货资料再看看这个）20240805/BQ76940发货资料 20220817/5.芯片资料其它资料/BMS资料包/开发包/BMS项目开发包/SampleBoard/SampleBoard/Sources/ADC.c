#include "ADC.h"
#include "MC9S08DZ60.h"
#include "SPI.h"
static unsigned int m_dataBuffer[11];
static unsigned char m_isADCInit=0;

void ADC_Init() 
{
   m_isADCInit=1;
}
uword* ADC_ConvertAll() 
{
	ubyte channel=0;
	uword sendValue;
	ubyte external=0xc; //0b1100,16bits and MSB data format.
  uword receiveValue;
	int isFirstTime=1;
		
  if(m_isADCInit!=1)
    ADC_Init();
  
	for(channel=0;channel<12;channel++)//We use loop for 12 times,and get 11 channel converted data.
	{

		sendValue=(channel<<12)|external<<8;

		receiveValue=SPI_TransmitWord(sendValue);

		if(isFirstTime==1)
		{
			isFirstTime=0;
		}
		else 
			m_dataBuffer[channel-1]=receiveValue>>4;

	}  
	return m_dataBuffer;
}
unsigned int ADC_Convert(unsigned char channel) 
{
	uword sendValue;
	ubyte external=0xc; //0b1100,16bits and MSB data format.
  uword receiveValue;
  
  if(m_isADCInit!=1)
    ADC_Init();
	sendValue=(channel<<12)|external<<8;

	SPI_TransmitWord(sendValue);
	ADC_Delay(0xff);
	receiveValue=SPI_TransmitWord(sendValue)>>4;

	return receiveValue;
}

void ADC_Delay(unsigned int max) 
{
  unsigned int i;
  for(i=0;i<max;i++)
  ;
}