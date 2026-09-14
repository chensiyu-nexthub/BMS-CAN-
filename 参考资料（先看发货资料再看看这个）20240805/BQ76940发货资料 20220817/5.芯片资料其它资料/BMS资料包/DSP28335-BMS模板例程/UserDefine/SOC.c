
#include "SOC.h"    

//安时*毫秒
//long型 最大596ah
//限制  500ah*3600*1000 = 1800000000Ams //1A*1ms
#define MAX_LIMIT_AMS 0x6B49D200 //1800000000Ams

TSOCPARA g_SOCPara;

//进行容量保存判断标准 相差值大小
unsigned char  g_ucSOC_Back = 0;//

volatile unsigned char CalSOCTimesOut;

volatile signed long g_slAS = 0;//当前容量
volatile signed long g_slAms =0; //JACK

//SOC累计标准 初始状态上累计 对应g_slAms 变量
signed long g_slInitAS = 0;//初始容量

static unsigned char g_ucCalAdjustFlag = 0;

TCHAAHPARA g_tChaAhPara;

unsigned long diff(unsigned long w1,unsigned long w2)
{
	if(w1>w2)
		return(w1-w2);
	else 
	   return(w2-w1);
}

const unsigned int g_cuiOCVTable[ 5 ]={0,0,0,0,0};

const unsigned int g_cuiOCVSOCTable[ 5 ]={0,0,0,0,0};

/* lookUp1d
 * ========
 * Performs simple linear interpolation of a 2D map.
 * Outputs are bounded by the upper and lower values
 * 
 * Both axis are expected to be increasing in value 
 * as the index increases
 *
 * Return = -1 for error and 1 for success
 * Params = int *ans         > an address to put the result
 *          int x            > the x value to lookup
 *          const int *xAxis > array of value for x axis
 *          const int lenX   > length of x axis array
 *          int y            > the y value to lookup
 *          const int *yAxis > array of value for y axis
 *          const int lenY   > length of y axis array
 * 
*/

int lookUp1d(int *ans, 
		   int x, const int *xAxis, const int lenX, 
		   const int *tab)
{
	int ret = -1; /* -1 is the error return value */
	int ix=0;

	/* make sure the lengths are good */
	if ( (lenX <=0 ) || 
		 (xAxis == NULL) ||
		 (tab == NULL)   ||
		 (ans == NULL) )
	{
		/* this has bad input data */
	}
	else
	{

		/* make sure it is in the boundaries before we do any looping*/
		if (x<=xAxis[0])
		{
			x = xAxis[0];
		}
		else
		{
			if (x>=xAxis[lenX-1])
			{
				x = xAxis[lenX-1]; 
			}
		}

		for (ix=0;ix<(lenX-1);ix++)
		{
			if (x>=xAxis[ix] && x<=xAxis[ix+1])
			{
				int x1,x2;
				int z1,z2;

				/* copy to a lot of temp variables to make code easier to read*/
				x1=xAxis[ix];
				x2=xAxis[ix+1];						
				z1=tab[ix];
				z2=tab[(ix+1)];

				/* then interpolate in the y direction for the answer */
				*ans = z1+((z2-z1)*(x-x1))/(x2-x1);

				/* indicate a valid result */
				ret  = 1;
				break; /* exit as fast as possible */
			}
		}
	}

	return ret;
}

//返回: -1 for error and 1 for success
//参数:填充结果， OCV值
int OCV_SOC( int *resSOC, unsigned int uiVolt)
{
        int res = 0;
	
	res = lookUp1d(resSOC,uiVolt,g_cuiOCVTable,5,g_cuiOCVSOCTable);

	return res;	
}

//两种方法修正实际可用容量( 不考虑老化 )
unsigned char CalAdjustCap( unsigned char ucSelect)
{
        int res1 = 0, res2 = 0;

	unsigned char ucMaxSOC = 0, ucMinSOC = 0 ;

	unsigned long ulTemp1 = 0,ulTemp2 = 0;

	res1 = OCV_SOC( &ucMaxSOC, ECU_Rt.maxv );

	res2 = OCV_SOC( &ucMinSOC, ECU_Rt.minv );
	
	if( ( 1 == res1) && ( 1 == res2))
	{

		if( ADJUSTCAP_OCV == ucSelect)
		{
			//OCV 法 EV HEV
			//OCVmax => SOCmax   OCVmin => SOCmin
			//Cpack=SOCmin*Crate + ( 1 - SOCmax) * Crate = 60 + 30  = 90Ah
			ulTemp1 = Sys_Rt.Type_Cap;
			ulTemp1 *= ucMinSOC;

			ulTemp2 = Sys_Rt.Type_Cap;
			ulTemp2 *= 100 - ucMaxSOC;

			ulTemp1 += ulTemp2;

			ulTemp1 /= 100;

			Sys_Rt.ADJType_Cap = (UINT16)ulTemp1;

			g_ucCalAdjustFlag = 1;			
		}
		else if( ADJUSTCAP_CHARGE == ucSelect)
		{
		    //充电法 (EV)
		    //OCVmin => SOCmin
		    //Cpack=SOCmin*Crate + Ccharged
			ulTemp1 = g_tChaAhPara.ucOriSOC;
			ulTemp1 *= Sys_Rt.Type_Cap;
			ulTemp1 /= 100;

			ulTemp2 = g_tChaAhPara.g_slChaAms;
			ulTemp2 /= 1000;
			ulTemp2 /= 3600;
			
			ulTemp1 += ulTemp2;
			
			Sys_Rt.ADJType_Cap = (UINT16)ulTemp1;
			
			g_ucCalAdjustFlag = 1;			
		}
		else
		{
			return 0;			
		}
		return 1;		
	}
	else
		return 0;	
}

void CalSOC( unsigned long ulAs )  //WriteAs(void)调用
{
        unsigned long ulTemp = 0;

	unsigned long ulTemp1 = 0;

	unsigned int uiCapType = 0;
	
	g_SOCPara.ulAs = ulAs;

	ulTemp = g_SOCPara.ulAs;

        ulTemp = ulTemp / 3600;    //As => Ah
    
	if( 1 == g_ucCalAdjustFlag) 
	        uiCapType = Sys_Rt.ADJType_Cap;  //使用校准后的额定容量
	else 
	        uiCapType = Sys_Rt.Type_Cap;   //使用给定的额定容量
 

	if( ulTemp < uiCapType)  //当前容量小于额定容量（额定容量可修正）
	{
		//g_SOCPara.ulWh = ulTemp * g_tVoltDataPackage.ulSumData;//0.1Wh W=U*I*T

                 //1Ah => 0.01Ah  分辨率扩大100倍
		ulTemp = g_SOCPara.ulAs;
		ulTemp *= 100;
		ulTemp = ulTemp / 3600;
		
		ulTemp1 = uiCapType;//额定容量扩大100倍
		ulTemp1 *= 100;

		//XX% SOC
		ulTemp *= 100;//剩余容量÷额定容量 × 100%100

		ulTemp = ulTemp /  ulTemp1;  //计算SOC，剩余电量比例

		g_SOCPara.ucSOC  = (unsigned char) ulTemp;
		
	}
	else
	{
		g_SOCPara.ucSOC = 100;

		if( 1 == g_ucCalAdjustFlag)		
			ulTemp = Sys_Rt.ADJType_Cap;//校准后容量		
		else		
			ulTemp = Sys_Rt.Type_Cap;//使用给定的额定容量
		
		//g_SOCPara.ulWh = ulTemp * g_tVoltDataPackage.ulSumData;//0.1Wh		
	}	
	if( diff(g_ucSOC_Back,g_SOCPara.ucSOC) >= 1 )//SOC 百分比相差 1 SOC有改变才存储
	{	
		g_ucSOC_Back = g_SOCPara.ucSOC;//更新最后记录的SOC
		
	        //写SOC到FM31256
		(void)WriteMulti31256RAM( FLASH_SOC_ADDR, ( unsigned char *)&g_SOCPara, sizeof( g_SOCPara ) );  
	}	
}

void WriteAs(void)
{

	signed char scIncSOC  = 0;

	signed long slIncCap = 0;

	unsigned long ulCapTemp = 0;

	unsigned long ulTemp = 0;

	unsigned char ucForceSetFlag = 0;

	unsigned int uiCapType = 0;

	if(CalSOCTimesOut != 0)	return;  //1s 定时处理SOC计算
	
	CalSOCTimesOut = 200;    //设定记数值
    
	//累加当前充放电容量  正负相加减
	if( g_slAms < 0 )
	{
		ulTemp = (-1) * g_slAms;
		
		ulTemp /= 1000;//Ams => As

		g_slAS = g_slInitAS;

		g_slAS -= ulTemp;//累计上电后 放电 容量
	}
	else
	{
		ulTemp = g_slAms;
		
		ulTemp /= 1000;//Ams => As

		g_slAS = g_slInitAS;

		g_slAS += ulTemp;//累计上电 充电 容量
	}

	//计算 g_slAS 为当前容量///////////////////////////////////

	//当前容量  < 0
	if( g_slAS < 0 )  
	{
		g_slAS = 0;//容量归零

		//放电 累计容量 最大限制 = 上电初始容量
		g_slAms = g_slInitAS;
		g_slAms *= 1000;//As => Ams
		g_slAms =(-1) * g_slAms;
	}

	//当前容量  > 额定容量
	if( 1 == g_ucCalAdjustFlag) 
	        uiCapType = Sys_Rt.ADJType_Cap;//判断是否使用校准后容量
	else 
	        uiCapType = Sys_Rt.Type_Cap;//

	ulTemp = g_slAS;
	ulTemp /= 3600;  //容量转化为Ah

	if( ulTemp >= uiCapType)   //当前容量大于额定容量（可修正）
	{
		//充电 累计容量 最大限制 =  额定容量-上电初始容量

		g_slAms = uiCapType;//额定容量
		g_slAms *= 3600;//Ah => As

		g_slAS = g_slAms;//当前容量 = 额定容量  中间结果赋值

		g_slAms -= g_slInitAS;//额定容量-上电初始容量
		g_slAms *= 1000;//As => Ams
	}

	/*
	if( abs( g_slAms ) > MAX_LIMIT_AMS) 
	{
		g_slAms = MAX_LIMIT_AMS;//超界后清零
	}
	*/
        /*0 < g_slAS < uiCapType*/
	CalSOC( g_slAS );	//计算SOC 百分比 
}

/******************************************************************************
*函数名:	WriteDefaultSoc   设置SOC百分比 100%         								     	  *
*功能描述:  写默认SOC                                       				  *
*输入参数:	无                                                                *
*输出参数:	无															      *
*返回值:	无							  								      *
******************************************************************************/
/**/
void WriteDefaultSoc( void )
{        
        unsigned char ucDefaultSoc = MAX_SOC;    //SOC放大1倍

	unsigned long ulTemp = 0;

	//更新SOC
	g_SOCPara.ucSOC = ucDefaultSoc;

	g_ucSOC_Back = ucDefaultSoc;

	if( 1 == g_ucCalAdjustFlag)
		ulTemp = Sys_Rt.ADJType_Cap;//校准后容量
	else
		ulTemp = Sys_Rt.Type_Cap;//额定容量

	ulTemp *= g_SOCPara.ucSOC;

	ulTemp /= 100;//剩余AH
	
	g_SOCPara.ulAs = ulTemp * 3600;//As
		
	//g_SOCPara.ulWh = ulTemp * g_tVoltDataPackage.ulSumData;//0.1Wh
	        
         //写默认SOC到FM31256
	(void)WriteMulti31256RAM( FLASH_SOC_ADDR, ( unsigned char *)&g_SOCPara, sizeof( g_SOCPara ) );                                
}

/******************************************************************************
*函数名:	WriteSOC	设置SOC百分比  							     		              *
*功能描述:  写SOC                                    				          *
*输入参数:	usSoc:SOC                                                         *
*输出参数:	无															      *
*返回值:	无							  								      *
******************************************************************************/

void WriteSoc( unsigned char usSoc )
{
	unsigned long ulTemp = 0;
	
        if( usSoc > MAX_SOC )    //SOC越界
        {               
                usSoc = MAX_SOC;            
        }//
		
        //更新SOC
	g_SOCPara.ucSOC = usSoc;

	g_ucSOC_Back = usSoc;

	if( 1 == g_ucCalAdjustFlag)
		ulTemp = Sys_Rt.ADJType_Cap;//校准后容量
	else
		ulTemp = Sys_Rt.Type_Cap;//额定容量

	ulTemp *= 100;//1Ah => 0.01Ah

	ulTemp *= g_SOCPara.ucSOC;

	ulTemp /= 100;//剩余AH
	
	g_SOCPara.ulAs = ulTemp * 3600;//0.001As

	g_SOCPara.ulAs /= 100;//0.001As => 1As
		
	//g_SOCPara.ulWh = ulTemp * g_tVoltDataPackage.ulSumData;//0.001Wh

	//g_SOCPara.ulWh /= 100;//0.001Wh =>0.1Wh
	
       //写SOC到FM31256
	(void)WriteMulti31256RAM( FLASH_SOC_ADDR, ( unsigned char *)&g_SOCPara, sizeof( g_SOCPara ) );   

	g_slAS = g_SOCPara.ulAs;//

	g_slInitAS  =  g_SOCPara.ulAs;

	g_slAms = 0;
        
}

/******************************************************************************
*函数名:	ReadSoc          								     		      *
*功能描述:  从EEPROM中读SOC  设置标称容量                              				      *
*输入参数:	无                                                                *
*输出参数:	无															      *
*返回值:	无							  								      *
******************************************************************************/
/**/
void ReadSoc( void )
{
        unsigned long ulTemp = 0;
        
        Sys_Rt.Type_Cap = 400;
        //从FM31256读SOC 
        (void)ReadMulti31256RAM( FLASH_SOC_ADDR, ( unsigned char *)&g_SOCPara, sizeof( g_SOCPara ) ); 
            
        if( g_SOCPara.ucSOC > MAX_SOC )
        {
            g_SOCPara.ucSOC = MAX_SOC;                   
        } //

	if( 1 == g_ucCalAdjustFlag)
		ulTemp = Sys_Rt.ADJType_Cap;//校准后容量
	else
		ulTemp = Sys_Rt.Type_Cap;//额定容量
	
	ulTemp *= 100;//1Ah => 0.01Ah

	ulTemp *= g_SOCPara.ucSOC;

	ulTemp /= 100;//剩余AH
	
	g_SOCPara.ulAs = ulTemp * 3600;//0.001As

	g_SOCPara.ulAs /= 100;//0.001As => 1As
		
	//g_SOCPara.ulWh = ulTemp * g_tVoltDataPackage.ulSumData;//0.1Wh

	//g_SOCPara.ulWh /= 100;//0.001Wh =>0.1Wh

	g_slAS = g_SOCPara.ulAs;//

	g_slInitAS  =  g_SOCPara.ulAs;

	g_ucSOC_Back = g_SOCPara.ucSOC;  
}

