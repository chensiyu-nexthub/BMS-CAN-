#ifndef SY_MATH_H
#define SY_MATH_H 1


#define MATH_TOTAL_NUNBER 16
#define MATH_BUFFER_NUMBER 10
#define MATH_MAX_VALUE   0xfff
#define MATH_MIN_VALUE   0x0 


#define DIFFERENT_TEMP_COUNTER MATH_BUFFER_NUMBER

struct Math_Data 
{
unsigned int dataBuffer[MATH_BUFFER_NUMBER];
unsigned char currentIndex;
unsigned char counter;
};

void Math_Init();
unsigned int Math_FilterData(unsigned char dataIndex,unsigned int dataIn);
double Math_GetDifferent_Sqrt(unsigned int* dataBuffer,unsigned char number);
unsigned long Math_GetDifferent_2(unsigned int* dataBuffer,unsigned char number);
unsigned int Math_AverageNoFilter(unsigned int* dataBuffer,unsigned char number);
#endif