#include "symath.h"
#include "math.h"


static unsigned int temp_buffer[DIFFERENT_TEMP_COUNTER];
static struct Math_Data m_mathData[MATH_TOTAL_NUNBER];
static unsigned char m_isMathInit=0;

static unsigned long Math_Total(unsigned int* dataBuffer,unsigned char number,
unsigned char filter_1,unsigned char filter_2);
static unsigned int Math_Average(unsigned int* dataBuffer,unsigned char number,
unsigned char filter_1,unsigned char filter_2);
static void Math_MaxMin(unsigned int* dataBuffer,unsigned char number,
unsigned char *maxIndexOut,unsigned char *minIndexOut);

void Math_Init() 
{
  unsigned char index_inside,index_outside;
  
  for(index_outside=0;index_outside<MATH_TOTAL_NUNBER;index_outside++) 
  {

     m_mathData[index_outside].currentIndex=MATH_BUFFER_NUMBER-1;
     m_mathData[index_outside].counter=0;
     for(index_inside=0;index_inside<MATH_BUFFER_NUMBER;index_inside++)
     {
      m_mathData[index_outside].dataBuffer[index_inside] =0;
     }
  }
  m_isMathInit=1;
}

static unsigned long Math_Total(unsigned int* dataBuffer,unsigned char number,
unsigned char filter_1,unsigned char filter_2) 
{
  unsigned long returnValue;
  unsigned char i;
  
  returnValue=0;
  for(i=0;i<number;i++) 
  {
    if(i!=filter_1&&i!=filter_2) 
    { 
      returnValue+=dataBuffer[i];
    }
  }
  return returnValue;
}
static unsigned long Math_Total_NoFilter(unsigned int* dataBuffer,unsigned char number) 
{
  unsigned long returnValue;
  unsigned char i;
  
  returnValue=0;
  for(i=0;i<number;i++) 
  {
    returnValue+=dataBuffer[i];
  }
  return returnValue;
}
unsigned int Math_AverageNoFilter(unsigned int* dataBuffer,unsigned char number) 
{
  unsigned int returnValue;
  unsigned long total=0;

  total= Math_Total_NoFilter(dataBuffer,number);

  returnValue=total/number;
  
  return returnValue;
}
static unsigned int Math_Average(unsigned int* dataBuffer,unsigned char number,
unsigned char filter_1,unsigned char filter_2)   
{
  unsigned int returnValue;
  unsigned long total=0;

  total= Math_Total(dataBuffer,number,filter_1,filter_2);
  number-=2;
  returnValue=total/number;
  
  return returnValue;
}
static void Math_MaxMin(unsigned int* dataBuffer,unsigned char number,
unsigned char *maxIndexOut,unsigned char *minIndexOut) 
{
  unsigned char index;
  unsigned char maxIndex,minIndex;
  unsigned int maxData,minData,currentData;
  
  *maxIndexOut=0;
  *minIndexOut=0;
  
  for(index=0;index<number;index++)
  {
     maxIndex=*maxIndexOut;
     minIndex=*minIndexOut;
     
     maxData=dataBuffer[maxIndex];
     minData=dataBuffer[minIndex];
     currentData=dataBuffer[index];
     
     if(currentData>=maxData)
       *maxIndexOut=index;
     if(currentData<minData)
       *minIndexOut=index;
     
  }
}
unsigned int Math_FilterData(unsigned char dataIndex,unsigned int dataIn) 
{
  unsigned char index;
  unsigned int currentIndex;
  unsigned char maxIndex,minIndex;
  unsigned int returnValue;
  unsigned int total_two;
  if(m_isMathInit!=1) 
  {
     Math_Init();
  }
  
  currentIndex=m_mathData[dataIndex].currentIndex;
  currentIndex++;
  if(currentIndex>=MATH_BUFFER_NUMBER) 
  {
    currentIndex=0;
  }
  
  m_mathData[dataIndex].currentIndex=currentIndex;
  
  m_mathData[dataIndex].dataBuffer[currentIndex]= dataIn;
  
  m_mathData[dataIndex].counter++;
  
  if(m_mathData[dataIndex].counter==1) 
  {
    returnValue=dataIn;
  } 
  else if(m_mathData[dataIndex].counter==2) 
  {
    total_two=m_mathData[dataIndex].dataBuffer[0];
    total_two+=m_mathData[dataIndex].dataBuffer[1];
    total_two>>=1;
    returnValue=total_two;   
  } 
  else 
  {
    if(m_mathData[dataIndex].counter>=MATH_BUFFER_NUMBER)
      m_mathData[dataIndex].counter=MATH_BUFFER_NUMBER;
    
    Math_MaxMin(m_mathData[dataIndex].dataBuffer,m_mathData[dataIndex].counter,&maxIndex,&minIndex);
    returnValue=Math_Average(m_mathData[dataIndex].dataBuffer,m_mathData[dataIndex].counter,maxIndex,minIndex);
  }
  return returnValue;
  
}

static void Math_GetDifferent(unsigned int* dataBuffer,unsigned int* differentBuffer,unsigned char number)//Get the value bettwen center.
{
  unsigned char i;
  unsigned int different,data,average;
  average=Math_AverageNoFilter(dataBuffer,number);
  
  for(i=0;i<number;i++) 
  {
    data=dataBuffer[i];
    if(average>data)
      differentBuffer[i]= average-data;
    else
      differentBuffer[i]= data- average;
  }
}

double Math_GetDifferent_Sqrt(unsigned int* dataBuffer,unsigned char number)
{
  double different_2_sqrt_in,different_2_sqrt_out;
  double returnValue;
  
  different_2_sqrt_in=(double)Math_GetDifferent_2(dataBuffer,number);
  different_2_sqrt_in/=number-1;
  returnValue=sqrt(different_2_sqrt_in);
   
  return returnValue;
}

unsigned long Math_GetDifferent_2(unsigned int* dataBuffer,unsigned char number) 
{
  unsigned long different_2_total=0;

  unsigned char i;
  
  Math_GetDifferent(dataBuffer,temp_buffer, number);
  asm("nop");
  for(i=0;i<number;i++) 
  {
     different_2_total+= temp_buffer[i]*temp_buffer[i];
  }
  return different_2_total;
}

