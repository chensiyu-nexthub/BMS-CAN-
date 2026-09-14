
//This software has been submitted to export control regulations
//The ECCN is EAR99 
//*****************************************************************************
/**
*  @file data_flash.c
*
*  @brief this file contains all the functions needed to access to the battery 
*  pack information stored in the MSP430 flash memory .
*
*  @author Daniel Torres - Texas Instruments, Inc
*  @date November 2010
*  @version 1.0 Initial version
*  @note Built with IAR for MSP430 Version: 5.10
*/


#include "bq_pack.h"
#include "types.h"
#include "data_flash.h"



/**
* @brief  Local variables        .                     
*/
//Constant Array that stores the battery pack info on flash memory
const DWORD DATA_FLASH[] = 
{
  /*Cell Voltage*/
  dCOV_THRESHOLD,                  //COV_THRESHOLD           [mV]   COV 过压阈值3700
  dCOV_RECOVERY_THRESHOLD,         //COV_RECOVERY_THRESHOLD  [mV] 	过压可恢复值3600
  dCOV_TIME,                       //COV_TIME                [100ms]	过压时间500ms
  dCUV_THRESHOLD,                  //CUV_THRESHOLD           [mV]	欠压阈值2000
  dCUV_RECOVERY_THRESHOLD,         //CUV_RECOVERY_THRESHOLD  [mV]   欠压可恢复值2200
  dCUV_TIME,                       //CUV_TIME                [100ms]  500ms
  
  /*Temperature*/
  dPACK_OVER_TEMP1, //3 PACK_OVER_TEMP1   [st C]			过温 50°
  dPACK_OT_TIME1,   //PACK_OT_TIME1       [ms]				2s
  dPACK_OVER_TEMP2,  //3 PACK_OVER_TEMP2   [st C]			50°
  dPACK_OT_TIME2,   //PACK_OT_TIME2       [ms]				2s
  
  /*Charge and Discharge*/
  dPACK_END_OF_CHARGE_VOLTAGE,     //PACK_END_OF_CHARGE_VOLTAGE    [mV]   	充电过压3700*18
  dCC_CV_QUAL_TIME,                //CC_CV_QUAL_TIME               [s]		20s  看不懂
  dPACK_END_OF_DISCHARGE_VOLTAGE,  //PACK_END_OF_DISCHARGE_VOLTAGE [mV] 	整体欠压2000*18
  dEND_OF_DISCHARGE_QUAL_TIME,     //END_OF_DISCHARGE_QUAL_TIME    [s]
  dCHARGE_CURRENT,                 //CHARGE_CURRENT                [mA]		充电电流1100mA
  dCHARGE_TAPER_CURRENT,           //CHARGE_TAPER_CURRENT          [mA]		充电尖峰300mA
  dCHARGE_TAPER_TIME,              //CHARGE_TAPER_TIME             [s]		充电最长时间240min 4h
  dMAX_CHARGE_TIME,                //MAX_CHARGE_TIME               [s]		最大充电时间200min
  dFULL_DISCHARGE_CLEAR_VOLTS,     //FULL_DISCHARGE_CLEAR_VOLTS    [mV] 	整体欠压2000mV*18
  dFULL_CHARGE_CLEAR_VOLTS,        //FULL_CHARGE_CLEAR_VOLTS       [mV] 	充电清楚3700*18
  dDELTA_CHARGE_V,                 //DELTA_CHARGE_V                [mv]		充电电压速度300mV/s
  dCHARGE_DISCHARGE_TIME,          //CHARGE_DISCHARGE_TIME         [s]		充放电时间5min
  dDELTA_DISCHARGE_V,              //DELTA_DISCHARGE_V             [mV]    	放电 速度200mV/s
    
  /*Safety*/
  dSOV_THRESHOLD,                 //SOV_THRESHOLD                 [mV]	4200
  dSOV_RECOVERY_THRESHOLD,        //SOV_RECOVERY_THRESHOLD        [mV]	3800
  dSOV_TIME,                      //SOV_TIME                      [ms]	3s
  
  /*Balancing*/
  dCELL_IMBALANCE_FAIL_THRESHOLD,    //CELL_IMBALANCE_FAIL_THRESHOLD [mV]   500
  dCELL_IMBALANCE_FAIL_TIME,         //CELL_IMBALANCE_FAIL_TIME      [s]   	120min
  dBALANCE_TIME,                     //BALANCE_TIME     [s] max value is 63  60s
  dBALANCE_VOLTS_THRESHOLD,          //BALANCE_VOLTS_THRESHOLD       [mV] 	50mV
  dMIN_BALANCE_VOLTS,                //MIN_BALANCE_VOLTS             [mV]	2200
  dMAX_BALANCE_TIME,                 //MAX_BALANCE_TIME              [s]	120min
  
};


/**
* @brief Function Name: get_u16_value.                                                 
* @brief Description  : Reads the flash memory and returns the value of the 
* parameter
* @param parameters   : battery pack parameter to be read        读取电池参数
* @return Value       : value of the specific battery parameter                                                    
*/     
unsigned short get_u16_value(param_id_t param_id)
{

  return (unsigned short)DATA_FLASH[param_id];

}

/**
* @brief Function Name: get_u32_value.                                                 
* @brief Description  : Reads the flash memory and returns the value of the 
* parameter
* @param parameters   : battery pack parameter to be read                                                     
* @return Value       : value of the specific battery parameter
*/   
DWORD get_u32_value(param_id_t param_id)
{
  return (DWORD)DATA_FLASH[param_id];  
}

/*EOF*/
