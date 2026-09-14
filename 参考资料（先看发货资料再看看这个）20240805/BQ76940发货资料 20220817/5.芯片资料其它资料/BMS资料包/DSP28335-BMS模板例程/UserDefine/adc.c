
#include "DSP28x_Project.h"
#include "SysInitDefine.h"

#define ADC_CKPS   0x6   // ADC module clock = HSPCLK/2*ADC_CKPS   = 25.0MHz/(1*2) = 12.5MHz
#define ADC_SHCLK  0xf   // S/H width in ADC module periods                        = 16 ADC clocks

//	float Uin;
void adc_read(float Uinthreshold)
{

	Uint16 GNDADC=0,UADC;
	Uint16 ADthreshold;

	ADthreshold = Uinthreshold / 122 * 22 / 3 * 4096;

    while (AdcRegs.ADCST.bit.INT_SEQ1== 0) {} // Wait for interrupt

	AdcRegs.ADCST.bit.INT_SEQ1_CLR = 1;

	GNDADC =((AdcRegs.ADCRESULT1>>4) );
	UADC =((AdcRegs.ADCRESULT0>>4) );

}            
 
void adc_init()
{

	InitAdc();  // For this example, init the ADC


   AdcRegs.ADCTRL1.bit.ACQ_PS = ADC_SHCLK;			//设置采集窗的大小，最大
   AdcRegs.ADCTRL3.bit.ADCCLKPS = ADC_CKPS;		//时钟分频
   AdcRegs.ADCTRL1.bit.SEQ_CASC = 0;        // 1  DISCascaded mode非级联模式
   AdcRegs.ADCCHSELSEQ1.bit.CONV00 = 0xf;   //Voltage
   AdcRegs.ADCCHSELSEQ1.bit.CONV01 = 0xe;   //GND
   AdcRegs.ADCTRL1.bit.CONT_RUN = 1;       // Setup continuous run连续运行模式
   AdcRegs.ADCREFSEL.bit.REF_SEL= 0;     //内部参考
   AdcRegs.ADCMAXCONV.bit.MAX_CONV2 = 1;


   AdcRegs.ADCTRL2.all = 0x2000;

   adc_read(UinMIN);

}         
