#ifndef _SOC_H
#define _SOC_H   
#define MAX_SOC 100    //1%/位	0~100%

#define MIN_SOC 0    //最小SOC     

#define  ADJUSTCAP_OCV      1
#define  ADJUSTCAP_CHARGE   2
//范围0~999AH   3596400AS 
typedef struct      
{
   unsigned long ulAs;     //1AH = 1*3600As
   
   unsigned long ulWh;    //电量 0.1Wh/bit
    
   unsigned char ucSOC;    //剩余电量 1%/位            
}TSOCPARA;

extern  TSOCPARA g_SOCPara;

typedef struct      
{
   signed long g_slChaAms; //充电累计容量
   
   unsigned char ucOriSOC; //充电起始SOC            
}TCHAAHPARA;

extern TCHAAHPARA g_tChaAhPara;

extern volatile unsigned char CalSOCTimesOut;
extern volatile signed long g_slAms; //JACK

void WriteAs(void);
	
void ReadSoc( void );    //从EEPROM读取SOC

void WriteDefaultSoc( void );    //写默认SOCEEPROM

void WriteSoc( unsigned short usSoc );    //写SOC到EEPROM

unsigned char CalAdjustCap( unsigned char ucSelect);

#endif
