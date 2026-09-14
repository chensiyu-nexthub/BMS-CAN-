
//This software has been submitted to export control regulations
//The ECCN is EAR99 
//****************************************************************************

/**
*  @file bq_pack.c 
*
*  @brief this file contains all the definitions of the BQ76PL536 devices.    这个文件包含所有的定义BQ76PL536设备
*
*  @author Daniel T. & Marcin N. Texas Instruments, Inc
*  @date November 2010
*  @version 1.0 Initial version
*  @note Built with IAR for MSP430 Version: 5.10
*/

#include <string.h>
#include <stdio.h>

#include "bq_pack.h"
#include "data_flash.h"


/******************************************************************************/
/* 初始化(还需要对SPI和四个功能脚进行初始化) init_spi(); InitBQ76PL536ports();  */
/******************************************************************************/

short bq_pack_init(void)//初始化   初始化   初始化   初始化   初始化   初始化   初始化   初始化   初始化
{
  unsigned char i;

  for (i=0; i<NUMBER_OF_BQ_DEVICES; i++)
  {
    //*Init cell count for each BQ device*/
    bq_pack.bq_devs[i].cell_count = MAX_CELLS_NUMBER_IN_BQ;//6个电池

    //*Configure each BQ device*/
    conf_bq_dev(&bq_pack.bq_devs[i]);	//配置

    //*Initilize data structures*/
    init_bq_dev_data_structure(&bq_pack.bq_devs[i]);

    //*Read cell voltage*/
    bq_dev_read_cell_voltage(&bq_pack.bq_devs[i]);
  }
  //battery manager initial mode
  bq_pack.op_mode = FAULT_MODE;
  bq_pack.error_status = 0;
  bq_pack.voltage = 0;
  bq_pack.timer_status = 0;
  bq_pack.eoc_eod_timer = 0;
  bq_pack.charge_taper_timer = 0;
  bq_pack.chg_dschg_op_timer = 0;
  bq_pack.balancing_timer = 0;
  bq_pack.max_balance_timer = 0;
  bq_pack.last_imbalanced_cell_idx = 0;
  bq_pack.cell_imbalance_fail_timer = 0;
  // set the battery manager to initial mode
  update_op_mode(INITIAL_MODE);
  return 0;
}


void conf_bq_dev(bq_dev_t* this)//配置参数
{
  unsigned short temp;

  bq_dev_write_reg(this->device_address, ADC_CONTROL_REG, ADC_CONTROL_VAL_6);

  bq_dev_write_reg(this->device_address, IO_CONTROL_REG, IO_CONTROL_VAL);

  bq_dev_write_reg(this->device_address, CB_CTRL_REG, CB_CTRL_VAL);
  bq_dev_write_reg(this->device_address, CB_TIME_REG, CB_TIME_VAL);

  bq_dev_write_reg(this->device_address, SHDW_CTRL_REG, SHDW_CTRL_ACCESS_EN_VAL);

  bq_dev_write_reg(this->device_address, FUNCTION_CONFIG_REG, FUNC_CONFIG_VAL_6);

  bq_dev_write_reg(this->device_address, SHDW_CTRL_REG, SHDW_CTRL_ACCESS_EN_VAL);
  bq_dev_write_reg(this->device_address, IO_CONFIG_REG, IO_CONFIG_VAL);

  temp = get_u32_value(COV_THRESHOLD);
  if (temp > 2000)
    temp = (temp - 2000)/50;
  else
    temp = 0;

  bq_dev_write_reg(this->device_address, SHDW_CTRL_REG, SHDW_CTRL_ACCESS_EN_VAL);
  bq_dev_write_reg(this->device_address, CONFIG_COV_REG, CONFIG_COV_VAL | temp);
  bq_dev_write_reg(this->device_address, SHDW_CTRL_REG, SHDW_CTRL_ACCESS_EN_VAL);
  bq_dev_write_reg(this->device_address, CONFIG_COVT_REG, CONFIG_COVT_VAL | COV_TIME);

  temp = get_u32_value(CUV_THRESHOLD);
  if (temp > 700)
    temp = (temp - 700)/100;
  else
    temp = 13;  /*Def CUV Threshold value = 2000mV*/
  bq_dev_write_reg(this->device_address, SHDW_CTRL_REG, SHDW_CTRL_ACCESS_EN_VAL);
  /*Def CUV Threshold = 2V*/
  bq_dev_write_reg(this->device_address, CONFIG_CUV_REG, CONFIG_CUV_VAL | temp);
  bq_dev_write_reg(this->device_address, SHDW_CTRL_REG, SHDW_CTRL_ACCESS_EN_VAL);
  bq_dev_write_reg(this->device_address, CONFIG_CUVT_REG, CONFIG_CUVT_VAL | CUV_TIME);

  bq_dev_write_reg(this->device_address, SHDW_CTRL_REG, SHDW_CTRL_ACCESS_EN_VAL);
  /*TS2=TS1=50stC*/
  bq_dev_write_reg(this->device_address, CONFIG_OT_REG, CONFIG_OT_VAL);
  bq_dev_write_reg(this->device_address, SHDW_CTRL_REG, SHDW_CTRL_ACCESS_EN_VAL);
  /*Over Temp Time delay = 2000ms*/
  bq_dev_write_reg(this->device_address, CONFIG_OTT_REG, CONFIG_OTT_VAL);

  return;
}


void init_bq_dev_data_structure(bq_dev_t* this)
{
  bq_dev_read_reg(this->device_address, DEVICE_STATUS_REG, 1, DISCARD_CRC, (unsigned char*) &this->device_status);

  bq_dev_read_reg(this->device_address, TEMPERATURE1_L_REG, 2, DISCARD_CRC, (unsigned char*) &this->temperature1);
  bq_dev_read_reg(this->device_address, TEMPERATURE2_L_REG, 2, DISCARD_CRC, (unsigned char*) &this->temperature2);

  /*Errors handling*/
  bq_dev_read_reg(this->device_address, ALERT_STATUS_REG, 1, DISCARD_CRC, (unsigned char*) &this->alert_status);
  bq_dev_read_reg(this->device_address, FAULT_STATUS_REG, 1, DISCARD_CRC, (unsigned char*) &this->fault_status);
  bq_dev_read_reg(this->device_address, COV_FAULT_REG, 1, DISCARD_CRC, (unsigned char*) &this->cov_fault);
  bq_dev_read_reg(this->device_address, CUV_FAULT_REG, 1, DISCARD_CRC, (unsigned char*) &this->cuv_fault);

  /*Clear alerts and faults (May not be so trivial...)*/
  bq_dev_clear_alerts(this);
  bq_dev_clear_faults(this);

  return;
}


short bq_dev_read_cell_voltage(bq_dev_t* this)
{
  unsigned char* pPtr;
  unsigned char temp;
  short i, ret_val;
  unsigned long voltage_comput;

  ret_val = bq_dev_read_reg(this->device_address, VCELL1_L_REG,
            (MAX_CELLS_NUMBER_IN_BQ<<1) , DISCARD_CRC,
            (unsigned char *) &this->cell_voltage[0]);

  for (i=0; i<MAX_CELLS_NUMBER_IN_BQ; i++)
  {
    //Swap the data in the array as BQ dev returns data in Big Endian notation
    pPtr = (unsigned char *)(&this->cell_voltage[i]);
    temp = *pPtr;
    *(pPtr) = *(pPtr+1);
    *(pPtr+1) = temp;

    //compute cell voltage -> cell_voltage=VCELL x 6250/16383
    voltage_comput = this->cell_voltage[i] * (unsigned long)(adc_step_mul);
    this->cell_voltage[i] = voltage_comput/((unsigned long)(adc_step_div));
  }

  return ret_val;
}


void bq_dev_clear_alerts(bq_dev_t* this)
{
  unsigned char Value;

  //clear alert bit in device status register
  bq_dev_read_reg(this->device_address, DEVICE_STATUS_REG, 1, DISCARD_CRC, (unsigned char*) &Value);
  Value |= BIT5;
  //set alert bit as 1
  bq_dev_write_reg(this->device_address, DEVICE_STATUS_REG, Value);
  Value &= ~BIT5;
  //clear alert bit
  bq_dev_write_reg(this->device_address, DEVICE_STATUS_REG, Value);

  //Read ALERT_STATUS_REG register
  bq_dev_read_reg(this->device_address, ALERT_STATUS_REG, 1, DISCARD_CRC, (unsigned char*) &Value);
  //Write 1's to ALERT_STATUS_REG register
  bq_dev_write_reg(this->device_address, ALERT_STATUS_REG, Value);

  Value = 0x00;
  // Write 0's ALERT_STATUS_REG register
  bq_dev_write_reg(this->device_address, ALERT_STATUS_REG, Value);

}


void bq_dev_clear_faults(bq_dev_t* this)
{
  unsigned char Value;

    //clear fault bit in device status register
  bq_dev_read_reg(this->device_address, DEVICE_STATUS_REG, 1, DISCARD_CRC, (unsigned char*) &Value);
  Value |= BIT6;
  //set fault bit as 1
  bq_dev_write_reg(this->device_address, DEVICE_STATUS_REG, Value);
  Value &= ~BIT6;
  //clear fault bit
  bq_dev_write_reg(this->device_address, DEVICE_STATUS_REG, Value);

  //Read FAULT_STATUS register
  bq_dev_read_reg(this->device_address, FAULT_STATUS_REG, 1, DISCARD_CRC, (unsigned char*) &Value);
  //Write 1's to FAULT_STATUS register
  bq_dev_write_reg(this->device_address, FAULT_STATUS_REG, Value);

  Value = 0x00;
  // Write 0's FAULT_STATUS register
  bq_dev_write_reg(this->device_address, FAULT_STATUS_REG, Value);

}




void update_bq_pack_data(void)    //更新数据    更新数据    更新数据    更新数据    更新数据    更新数据    更新数据    更新数据    更新数据
{
  unsigned char i, cell_cnt;
  unsigned short stack_voltage = 0;
  //read the pack data
  for (i=0; i<NUMBER_OF_BQ_DEVICES; i++)
  {
    //*Read each device status*/
    bq_dev_read_status(&bq_pack.bq_devs[i]);

    //*Read each cell voltage and calculate BQ Pack voltage*/
    bq_dev_read_cell_voltage(&bq_pack.bq_devs[i]);

    //Identify the lowest and highest voltage in the pack
    if (i == 0)
    {
      bq_pack.lowest_cell_volts = bq_pack.bq_devs[i].cell_voltage[0];
      bq_pack.highest_cell_volts = bq_pack.bq_devs[i].cell_voltage[0];
    }
    //calculate the pack voltage
    for (cell_cnt=0; cell_cnt<bq_pack.bq_devs[i].cell_count; cell_cnt++)
    {
      stack_voltage += bq_pack.bq_devs[i].cell_voltage[cell_cnt];

      if (bq_pack.bq_devs[i].cell_voltage[cell_cnt] < bq_pack.lowest_cell_volts)
      {
        bq_pack.lowest_cell_volts = bq_pack.bq_devs[i].cell_voltage[cell_cnt];
      }
      else if (bq_pack.bq_devs[i].cell_voltage[cell_cnt] > bq_pack.highest_cell_volts)
      {
        bq_pack.highest_cell_volts = bq_pack.bq_devs[i].cell_voltage[cell_cnt];
      }
    }

    //*Read each device errors*/
    bq_dev_read_errors(&bq_pack.bq_devs[i]);

    //*Read each device temperature*/
    bq_dev_read_temps(&bq_pack.bq_devs[i]);
  }

  //*Save BQ Pack voltage*/
  bq_pack.voltage = stack_voltage;

  return;
}

void copy_cell_voltage_values(void)
{
  unsigned short i, j, cell_cnt;

  j = 0;
  for (i=0; i<NUMBER_OF_BQ_DEVICES; i++)
  {
    /*Read each cell voltage and compare it vs initial cell voltage*/
    for (cell_cnt=0; cell_cnt<bq_pack.bq_devs[i].cell_count; cell_cnt++)
    {
      cell_values[j++] = bq_pack.bq_devs[i].cell_voltage[cell_cnt];
    }
  }

  return;
}




void update_op_mode(op_modes_t new_mode)
{

  if (bq_pack.op_mode == new_mode)
    return;

  bq_pack.op_mode = new_mode;

  switch (new_mode)
  {
    case CHARGE_OP:

      break;

    case END_OF_CHARGE:


      break;

    case DISCHARGE_OP:


      break;

    case END_OF_DISCHARGE:


      break;

    case FAULT_MODE:

      //Disable Balancing Virtual Timer
      bq_pack.timer_status &= ~START_CELL_BALANCE_TIMER;
      break;

    case SOV_MODE:

      //Disable Cell Balancing Timer
      bq_pack.timer_status &= ~START_CELL_BALANCE_TIMER;
      break;

    case INITIAL_MODE:

      //Clear error_status
      bq_pack.error_status = 0;

      //Clear charge/discharge timer
      bq_pack.chg_dschg_op_timer = 0;

      //Start Charge/Discharge operation timer
      bq_pack.timer_status |= START_CHG_DSCHG_OP_TIMER;

      //Disable DRDY pin interrupt
   //   BQ76PL536_DRDY_PxIE  &= ~IN_BQ_DRDY;    //需要修改

      //Trigger 'PL536 ADC conversion and then..
      bq_pack_start_conv();

      //Wait for data to be ready
    //  while(!(BQ76PL536_DRDY_PxIN  & IN_BQ_DRDY)); //需要修改

      //Read data from BQ pack for overall system verification
      update_bq_pack_data();
      //Copy current cell values
      copy_cell_voltage_values();

      //Enable DRDY pin interrupt
  //    BQ76PL536_DRDY_PxIFG  &= ~IN_BQ_DRDY;       //需要修改
 //     BQ76PL536_DRDY_PxIE  |= IN_BQ_DRDY;			//需要修改


      break;


    default:
      break;
  }

  return;
}




void bq_pack_start_conv(void)
{
  //Should address all devices in the stack
  bq_dev_write_reg(BROADCAST_ADDR, ADC_CONVERT_REG, ADC_CONVERT_VAL); //spi_write_reg(0x3F,(unsigned char)ADC_CONVERT_REG,(1<<0))

}

short bq_dev_read_errors(bq_dev_t* this)
{
  bq_dev_read_reg(this->device_address, DEVICE_STATUS_REG, 1, DISCARD_CRC, (unsigned char*) &this->device_status);
  bq_dev_read_reg(this->device_address, ALERT_STATUS_REG, 1, DISCARD_CRC, (unsigned char*) &this->alert_status);
  bq_dev_read_reg(this->device_address, FAULT_STATUS_REG, 1, DISCARD_CRC, (unsigned char*) &this->fault_status);
  bq_dev_read_reg(this->device_address, COV_FAULT_REG, 1, DISCARD_CRC, (unsigned char*) &this->cov_fault);
  bq_dev_read_reg(this->device_address, CUV_FAULT_REG, 1, DISCARD_CRC, (unsigned char*) &this->cuv_fault);

  return 0;
}


short bq_dev_read_temps(bq_dev_t* this)
{
  unsigned char data[2];

  //Bytes need to be swaped as BQ device supports Big Endian
  bq_dev_read_reg(this->device_address, TEMPERATURE1_L_REG, 2, DISCARD_CRC, (unsigned char*) &data[0]);
  this->temperature1 = ((data[0] << 8) | data[1]);
  bq_dev_read_reg(this->device_address, TEMPERATURE2_L_REG, 2, DISCARD_CRC, (unsigned char*) &data[0]);
  this->temperature2 = ((data[0] << 8) | data[1]);

  return 0;
}
short bq_dev_read_status(bq_dev_t* this)
{
  return bq_dev_read_reg(this->device_address, DEVICE_STATUS_REG, 1, DISCARD_CRC, (unsigned char*) &this->device_status);
}


//不用初始化的
#define CELL_BALANCING_EN 1




void update_bq_pack_timer(void)//更新时间
{
  if (bq_pack.timer_status & START_END_OF_CHG_DSCHG_TIMER)
  {
    bq_pack.eoc_eod_timer++;
  }

  if (bq_pack.timer_status & START_CHARGE_TAPER_TIMER)
  {
    bq_pack.charge_taper_timer++;
  }


  if (bq_pack.timer_status & START_CHG_DSCHG_OP_TIMER)
  {
    bq_pack.chg_dschg_op_timer++;
  }


#if CELL_BALANCING_EN
  if (bq_pack.timer_status & START_CELL_BALANCE_TIMER)
  {
    bq_pack.balancing_timer++;
    bq_pack.max_balance_timer++;

    //Update imbalance timer if voltage between any 2 cells
    //> CELL_IMBALANCE_THRESHOLD
    if (cell_imbalance_threshold_reached())
    {
      bq_pack.cell_imbalance_fail_timer++;
    }
    else
    {
      bq_pack.cell_imbalance_fail_timer = 0;
    }
  }
#endif

  return;
}



#if CELL_BALANCING_EN
void CellBalancing(void)
{

  unsigned short dev_id;


  if (bq_pack.timer_status & START_CELL_BALANCE_TIMER)   //开始均衡了
  {
    //*Perform cell balancing only if CHARGE OPERATION is in effect*/
    //if battery pack is charging
    if (HOST_CONTROL_IN & IN_HOST_CHG_OP)   //充电中
    {
      //balancing time has expired? Yes, then...
      if (bq_pack.balancing_timer >= (get_u32_value(BALANCE_TIME)-2))
      {
        //cell balancing achieved?
        if ((bq_pack.highest_cell_volts - bq_pack.lowest_cell_volts) >= get_u32_value(BALANCE_VOLTS_THRESHOLD))    //最高电压和最低电压的差值太大
        { //cell balancing achieved? ->No,
          //then reset balancing timer & continue balancing
          for (dev_id=0; dev_id<NUMBER_OF_BQ_DEVICES; dev_id++)
          {
            //*Enable bypass resistor for all balanced cells*/
            enable_bypass_resistor(dev_id, (~find_imbalanced_cell(dev_id)));

          }
          //reset balancing virtual timer
          bq_pack.balancing_timer = 0;

        }
        else
        {
          //cell balancing achieved? ->Yes,
          //then Stop balancing timer
          bq_pack.timer_status &= ~START_CELL_BALANCE_TIMER;

          //Disable bypass resistors
          disable_all_bypass_resistors();
        }
      }
      //If max balancing time has expired, then...
      if (bq_pack.max_balance_timer > get_u32_value(MAX_BALANCE_TIME))
      {
        //flag the error and set the pack in SOV mode..
        bq_pack.error_status |= STATUS_ERROR_MAX_BALANCE_TIME;
        update_op_mode(SOV_MODE);
      }

       //If cell Imbalance timer time has expired, then...
      if(bq_pack.cell_imbalance_fail_timer > get_u32_value(CELL_IMBALANCE_FAIL_TIME))
      {
        //flag the error and set the pack in SOV mode..
        bq_pack.error_status |= STATUS_ERROR_IMBALANCE_FAIL;
        update_op_mode(SOV_MODE);
      }
    }
    else //If pack is not in charging mode then...
    {
      //Stop cell balancing timers
      bq_pack.timer_status &= ~START_CELL_BALANCE_TIMER;
      //reset all cell balancing timers
      bq_pack.balancing_timer=0;
      bq_pack.max_balance_timer=0;
      bq_pack.cell_imbalance_fail_timer=0;

      //*Disable all bypass resistors*/
      disable_all_bypass_resistors();
    }
  }
  else //if cell balancing is not active then start balancing
  {
    /*Perform cell balancing only if CHARGE OPERATION is in effect*/
    if (HOST_CONTROL_IN & IN_HOST_CHG_OP)
    {
      //check if cell balancing is required...
      if (((bq_pack.highest_cell_volts - bq_pack.lowest_cell_volts) >= get_u32_value(BALANCE_VOLTS_THRESHOLD)))
      {
        //and allowed...
        if(bq_pack.lowest_cell_volts > get_u32_value(MIN_BALANCE_VOLTS))
        {
          //Start cell balancing timer
          bq_pack.timer_status |= START_CELL_BALANCE_TIMER;
          //reset cell balancing timers
          bq_pack.balancing_timer = 0;
          bq_pack.max_balance_timer = 0;
          bq_pack.cell_imbalance_fail_timer = 0;
          //enable the bypass resistors

          for (dev_id=0; dev_id<NUMBER_OF_BQ_DEVICES; dev_id++)
          {
            //*Enable bypass resistor for all balanced cells*/
            enable_bypass_resistor(dev_id, (~find_imbalanced_cell(dev_id)));

          }
        }
      }
    }
  }
}
#endif



void enable_bypass_resistor(unsigned short in_dev_id, unsigned short in_value)
{
  bq_dev_write_reg(bq_pack.bq_devs[in_dev_id].device_address, CB_TIME_REG, get_u32_value(BALANCE_TIME));
  bq_dev_write_reg(bq_pack.bq_devs[in_dev_id].device_address, CB_CTRL_REG, (0x003F & in_value));

  return;
}


void disable_all_bypass_resistors(void)
{
  unsigned short bq_dev_id;

  for (bq_dev_id=0; bq_dev_id<NUMBER_OF_BQ_DEVICES; bq_dev_id++)
  {
    bq_dev_write_reg(bq_pack.bq_devs[bq_dev_id].device_address, CB_CTRL_REG, 0);
  }

  return;
}

unsigned short find_imbalanced_cell(unsigned short in_dev_id)
{
  unsigned short cell_id, imb_cells_mask, cnt;

  cnt = 0;
  imb_cells_mask = 0xFFFF;

  /*Read each cell voltage and compare it vs lowest cell voltage*/
  for (cell_id=0; cell_id<bq_pack.bq_devs[in_dev_id].cell_count; cell_id++)
  {
    imb_cells_mask &= ~(1<<cnt);

    if ((bq_pack.highest_cell_volts - bq_pack.bq_devs[in_dev_id].cell_voltage[cell_id]) >= get_u32_value(BALANCE_VOLTS_THRESHOLD))
    {
      imb_cells_mask |= (1<<cnt);
    }
    cnt++;
  }

  return imb_cells_mask;
}



//////unnecessary
//////unnecessary
//////unnecessary
//////unnecessary
//////unnecessary


//找出器件地址是否符合要求
short bq_pack_address_discovery(void)
{
  unsigned short i, n;
  unsigned char reg_val[2];

  i=NUMBER_OF_BQ_DEVICES; //controls iteration loop
  while (i>0)
  {
    //*Send BROADCAST_RESET to address 0x00*/
    bq_dev_write_reg(BROADCAST_ADDR, RESET_REG, BQ76PL536_RESET);
    n=0;  //controls number of discovered devices
    while (n<NUMBER_OF_BQ_DEVICES)
    {
    //*Read DEVICE_STATUS reg at address 0x00*/
    bq_dev_read_reg(DISCOVERY_ADDR, DEVICE_STATUS_REG, 1, DISCARD_CRC, reg_val);

      if (reg_val[0] & (1<<7))			//如果0的最高位是1
      {
        n = NUMBER_OF_BQ_DEVICES; //break internal loop  跳出循环 n=3
      }
      else  //
      {
        n++;
        bq_pack.bq_devs[n-1].device_address = n;  //器件地址为n

        //ADDR_CTRL = n;
        bq_dev_write_reg(DISCOVERY_ADDR, ADDRESS_CONTROL_REG, n);   //

        //read ADDR_CTRL
        bq_dev_read_reg(n, ADDRESS_CONTROL_REG, 1, DISCARD_CRC, reg_val);    //read ADDR_CTRL
        if ((reg_val[0]&0x3F) == n)
        {
          //address next device or finish device detection
          if (n==NUMBER_OF_BQ_DEVICES)
            return n;
        }
        else
        {
          //break internal loop
          n = NUMBER_OF_BQ_DEVICES;
        }
      }
    }

    i--;
  }

  return 0;
}


  //电池块管理
void BatteryPackManager(void)
{

  CheckFaultConditions();//检查错误状态s
  CheckChargeDischargeModes();//检查充放电模式
  CheckEndOfChargeOrDischargeModes();//检查是否完成了充放电模式
#if CELL_BALANCING_EN     //检查均衡
  CellBalancing();
#endif

}


void CheckFaultConditions(void)
{
  //check for POT or COV conditions
  if (check_for_pot() || check_for_cov())
  {
    bq_pack.error_status |= STATUS_ERROR_POT_COV;

    //can only return to previous mode when error condition disapears
    update_op_mode(FAULT_MODE);

  }
  //Check for CUV condition
  if (check_for_cuv())
  {
    bq_pack.error_status |= STATUS_ERROR_CUV;

    //can only return to previous mode when error condition disapears
    update_op_mode(FAULT_MODE);
  }
}



void CheckChargeDischargeModes(void)  //检查充电 还是  放电 状态
{
  //Charge/Discharge watchdog timer ON??
  if (bq_pack.timer_status & START_CHG_DSCHG_OP_TIMER)  //开始  工作了
  { //Charge/Discharge watchdog timer expired??
    if (bq_pack.chg_dschg_op_timer >= (get_u32_value(CHARGE_DISCHARGE_TIME)))   //工作时长超过21ms
    { //Check if the cells increased their voltage
      if (check_for_charge_op())      //检查电压是上升还是下降
      {
        //Charge operation detected!
        update_op_mode(CHARGE_OP);
      }//Check if the cells decreased their voltage
      else if (check_for_discharge_op())
      {
        //Discharge operation detected!
        update_op_mode(DISCHARGE_OP);
      }

      //Clear timer count
      bq_pack.chg_dschg_op_timer = 0;

      //Save current cell voltages for next comparison
      copy_cell_voltage_values();
    }
  }
}


void CheckEndOfChargeOrDischargeModes(void)  //是否达到充放电的尽头
{
   //*For Operation mode different than FAULT MODE*/
  if (bq_pack.op_mode < FAULT_MODE)
  {  //check if pack voltage is lower than the end of discharge voltage
    if (bq_pack.voltage < get_u32_value(PACK_END_OF_DISCHARGE_VOLTAGE))
    {
      update_op_mode(END_OF_DISCHARGE);

    }//check if the battery pack voltage is higher than the end of charge volt
    else if (bq_pack.voltage > get_u32_value(PACK_END_OF_CHARGE_VOLTAGE))
    {
      update_op_mode(END_OF_CHARGE);

    }
  }
}



short check_for_pot(void)
{
  /*Temp sensor connected to first BQ device only*/
  if (bq_pack.bq_devs[0].alert_status & (0x3<<ALERT_OT1_POS))
  {
      return 1; //error
  }

  return 0;
}

short check_for_cov(void)
{

  unsigned short i, cell;

  for (i=0; i<NUMBER_OF_BQ_DEVICES; i++)
  {
    if (bq_pack.bq_devs[i].fault_status & (1<<FAULT_COV_POS))
    {
      //Check if error condition is still in effect
      for (cell=0; cell < bq_pack.bq_devs[i].cell_count; cell++)
      {
        if (bq_pack.bq_devs[i].cov_fault & (1<<cell))  //过压了
        {
          if (bq_pack.bq_devs[i].cell_voltage[cell] > get_u32_value(COV_RECOVERY_THRESHOLD))  //不可恢复了
          {
            //COV condition still present
            return 1; //error
          }
          else
          {
            //COV condition cleared
            //clear FAULT flag
            bq_dev_write_reg(bq_pack.bq_devs[i].device_address, FAULT_STATUS_REG, (1<<FAULT_COV_POS));
            bq_dev_write_reg(bq_pack.bq_devs[i].device_address, FAULT_STATUS_REG, (0<<FAULT_COV_POS));

            //Assumption COV and CUV are mutually exclusive
            update_op_mode(INITIAL_MODE);
          }
        }
      }
    }
  }

  return 0;
}

short check_for_cuv(void)
{
  unsigned short i, cell;

  for (i=0; i<NUMBER_OF_BQ_DEVICES; i++)
  {
    if (bq_pack.bq_devs[i].fault_status & (1<<FAULT_CUV_POS))
    {
      //Check if error condition is still in effect
      for (cell=0; cell < bq_pack.bq_devs[i].cell_count; cell++)
      {
        if (bq_pack.bq_devs[i].cuv_fault & (1<<cell))
        {
          if (bq_pack.bq_devs[i].cell_voltage[cell] < get_u32_value(CUV_RECOVERY_THRESHOLD))
          {
            //CUV condition still present
            return 1; //error
          }
          else
          {
            //CUV condition cleared
            //clear FAULT flag
            bq_dev_write_reg(bq_pack.bq_devs[i].device_address, FAULT_STATUS_REG, (1<<FAULT_CUV_POS));
            bq_dev_write_reg(bq_pack.bq_devs[i].device_address, FAULT_STATUS_REG, (0<<FAULT_CUV_POS));

            //Assumption COV and CUV are mutually exclusive
            update_op_mode(INITIAL_MODE);
          }
        }
      }
    }
  }

  return 0;
}


unsigned short check_for_charge_op(void)
{
  unsigned short i, j, cell_cnt;

  j=0;
  for (i=0; i<NUMBER_OF_BQ_DEVICES; i++)
  {
    /*Read each cell voltage and compare it vs initial cell voltage*/
    for (cell_cnt=0; cell_cnt<bq_pack.bq_devs[i].cell_count; cell_cnt++)
    {
      if (bq_pack.bq_devs[i].cell_voltage[cell_cnt] > cell_values[j])
      {
        if ((bq_pack.bq_devs[i].cell_voltage[cell_cnt] - cell_values[j])  > get_u32_value(DELTA_CHARGE_V))
        {
          return 1;
        }
      }
      j++;
    }
  }

  return 0;
}


unsigned short check_for_discharge_op(void)
{
  unsigned short i, j, cell_cnt;

  j=0;
  for (i=0; i<NUMBER_OF_BQ_DEVICES; i++)
  {
    /*Read each cell voltage and compare it vs initial cell voltage*/
    for (cell_cnt=0; cell_cnt<bq_pack.bq_devs[i].cell_count; cell_cnt++)
    {
      if (cell_values[j] > bq_pack.bq_devs[i].cell_voltage[cell_cnt])
      {
        if ((cell_values[j] - bq_pack.bq_devs[i].cell_voltage[cell_cnt]) > get_u32_value(DELTA_DISCHARGE_V))
        {
          return 1;
        }
      }
      j++;
    }
  }

  return 0;
}







unsigned short cell_imbalance_threshold_reached(void)
{
  if ((bq_pack.highest_cell_volts - bq_pack.lowest_cell_volts) > get_u32_value(CELL_IMBALANCE_FAIL_THRESHOLD))
    return 1;
  else
    return 0;
}

















/*EOF*/

