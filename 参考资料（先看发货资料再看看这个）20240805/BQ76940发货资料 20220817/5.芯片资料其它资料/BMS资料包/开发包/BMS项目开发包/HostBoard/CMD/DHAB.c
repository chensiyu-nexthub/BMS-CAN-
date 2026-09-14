
#include "DHAB.h"
#include "../include/ADC.h"


double m_current;
double m_currentH;
double m_currentL;
unsigned int m_offsetH;
unsigned int m_offsetL;
unsigned int m_valueH;
unsigned int m_valueL;

double DHAB_GetCurrent(unsigned int valueH,unsigned int valueL)
{
	double returnH,returnL;
	unsigned int offsetValueH,offsetValueL;
	unsigned int swap;

	m_valueH=valueH;
	m_valueL=valueL;

	if(valueL>=2048)
	{
		offsetValueH=valueH-2048;
		offsetValueL=valueL-2048;
	}
	else
	{
		offsetValueH=2048-valueH;
		offsetValueL=2048-valueL;
	}
	//ensure offsetValueL > offsetValueH
	if(offsetValueH>offsetValueL)
	{
		swap=offsetValueH;
		offsetValueH=offsetValueL;
		offsetValueL=swap;
	}
	m_offsetH=offsetValueH;
	m_offsetL=offsetValueL;

	m_currentL=offsetValueL*0.018432;
	m_currentH=offsetValueH*0.21416;

	if(m_currentL>29)
		m_current= m_currentH;
	else
		m_current= m_currentL;

	return m_current;
}

