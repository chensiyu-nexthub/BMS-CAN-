/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usart.h"
#include "gpio.h"
#include <stdio.h>
#include <stdarg.h>

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

#define BQ76940_ADDR_7BIT  0x08U

static void BQ_DelayShort(void)
{
  for (volatile uint32_t i = 0; i < 120U; i++)
  {
    __NOP();
  }
}

static void UART1_Printf(const char *fmt, ...)
{
  char buffer[192];
  va_list args;
  int len;

  va_start(args, fmt);
  len = vsnprintf(buffer, sizeof(buffer), fmt, args);
  va_end(args);

  if (len <= 0)
  {
    return;
  }

  if (len > (int)sizeof(buffer))
  {
    len = (int)sizeof(buffer);
  }

  HAL_UART_Transmit(&huart1, (uint8_t *)buffer, (uint16_t)len, HAL_MAX_DELAY);
}

static void BQ_SCL_H(void)
{
  HAL_GPIO_WritePin(BQ_SCL_GPIO_Port, BQ_SCL_Pin, GPIO_PIN_SET);
}

static void BQ_SCL_L(void)
{
  HAL_GPIO_WritePin(BQ_SCL_GPIO_Port, BQ_SCL_Pin, GPIO_PIN_RESET);
}

static void BQ_SDA_H(void)
{
  HAL_GPIO_WritePin(BQ_SDA_GPIO_Port, BQ_SDA_Pin, GPIO_PIN_SET);
}

static void BQ_SDA_L(void)
{
  HAL_GPIO_WritePin(BQ_SDA_GPIO_Port, BQ_SDA_Pin, GPIO_PIN_RESET);
}

static GPIO_PinState BQ_SDA_Read(void)
{
  return HAL_GPIO_ReadPin(BQ_SDA_GPIO_Port, BQ_SDA_Pin);
}

static void BQ_I2C_ReleaseBus(void)
{
  BQ_SDA_H();
  BQ_SCL_H();
  BQ_DelayShort();
}

static HAL_StatusTypeDef BQ_I2C_Start(void)
{
  BQ_I2C_ReleaseBus();
  if (BQ_SDA_Read() == GPIO_PIN_RESET)
  {
    return HAL_ERROR;
  }

  BQ_SDA_L();
  BQ_DelayShort();
  BQ_SCL_L();
  BQ_DelayShort();
  return HAL_OK;
}

static void BQ_I2C_Stop(void)
{
  BQ_SCL_L();
  BQ_SDA_L();
  BQ_DelayShort();
  BQ_SCL_H();
  BQ_DelayShort();
  BQ_SDA_H();
  BQ_DelayShort();
}

static HAL_StatusTypeDef BQ_I2C_WriteBit(uint8_t bit)
{
  if (bit != 0U)
  {
    BQ_SDA_H();
  }
  else
  {
    BQ_SDA_L();
  }

  BQ_DelayShort();
  BQ_SCL_H();
  BQ_DelayShort();
  BQ_SCL_L();
  BQ_DelayShort();
  return HAL_OK;
}

static uint8_t BQ_I2C_ReadBit(void)
{
  uint8_t bit;

  BQ_SDA_H();
  BQ_DelayShort();
  BQ_SCL_H();
  BQ_DelayShort();
  bit = (BQ_SDA_Read() == GPIO_PIN_SET) ? 1U : 0U;
  BQ_SCL_L();
  BQ_DelayShort();
  return bit;
}

static HAL_StatusTypeDef BQ_I2C_WriteByte(uint8_t data)
{
  for (uint8_t i = 0; i < 8U; i++)
  {
    (void)BQ_I2C_WriteBit((data & 0x80U) != 0U);
    data <<= 1;
  }

  BQ_SDA_H();
  BQ_DelayShort();
  BQ_SCL_H();
  BQ_DelayShort();

  if (BQ_SDA_Read() == GPIO_PIN_SET)
  {
    BQ_SCL_L();
    return HAL_ERROR;
  }

  BQ_SCL_L();
  BQ_DelayShort();
  return HAL_OK;
}

static uint8_t BQ_I2C_ReadByte(uint8_t ack)
{
  uint8_t data = 0U;

  BQ_SDA_H();
  for (uint8_t i = 0; i < 8U; i++)
  {
    data <<= 1;
    BQ_SCL_H();
    BQ_DelayShort();
    if (BQ_SDA_Read() == GPIO_PIN_SET)
    {
      data |= 0x01U;
    }
    BQ_SCL_L();
    BQ_DelayShort();
  }

  if (ack != 0U)
  {
    BQ_SDA_L();
  }
  else
  {
    BQ_SDA_H();
  }

  BQ_DelayShort();
  BQ_SCL_H();
  BQ_DelayShort();
  BQ_SCL_L();
  BQ_SDA_H();
  return data;
}

static HAL_StatusTypeDef BQ_ReadReg(uint8_t reg, uint8_t *value)
{
  if ((value == NULL) || (BQ_I2C_Start() != HAL_OK))
  {
    BQ_I2C_Stop();
    return HAL_ERROR;
  }

  if (BQ_I2C_WriteByte((uint8_t)(BQ76940_ADDR_7BIT << 1)) != HAL_OK)
  {
    BQ_I2C_Stop();
    return HAL_ERROR;
  }

  if (BQ_I2C_WriteByte(reg) != HAL_OK)
  {
    BQ_I2C_Stop();
    return HAL_ERROR;
  }

  if (BQ_I2C_Start() != HAL_OK)
  {
    BQ_I2C_Stop();
    return HAL_ERROR;
  }

  if (BQ_I2C_WriteByte((uint8_t)((BQ76940_ADDR_7BIT << 1) | 0x01U)) != HAL_OK)
  {
    BQ_I2C_Stop();
    return HAL_ERROR;
  }

  *value = BQ_I2C_ReadByte(0U);
  BQ_I2C_Stop();
  return HAL_OK;
}

static void BQ_Wake(void)
{
  HAL_GPIO_WritePin(BQ_WAKE_GPIO_Port, BQ_WAKE_Pin, GPIO_PIN_SET);
  HAL_Delay(100);
  HAL_GPIO_WritePin(BQ_WAKE_GPIO_Port, BQ_WAKE_Pin, GPIO_PIN_RESET);
  HAL_Delay(100);
}

static void BQ_BringupAndPrint(void)
{
  uint8_t value;
  uint8_t adc_gain1 = 0U;
  uint8_t adc_offset = 0U;
  uint8_t adc_gain2 = 0U;

  UART1_Printf("\r\n=== BQ76940 bring-up ===\r\n");

  if (BQ_ReadReg(0x00U, &value) == HAL_OK)
  {
    UART1_Printf("SYS_STAT = 0x%02X\r\n", value);
  }
  else
  {
    UART1_Printf("SYS_STAT read fail\r\n");
  }

  if (BQ_ReadReg(0x50U, &adc_gain1) == HAL_OK &&
      BQ_ReadReg(0x51U, &adc_offset) == HAL_OK &&
      BQ_ReadReg(0x59U, &adc_gain2) == HAL_OK)
  {
    UART1_Printf("ADCGAIN1 = 0x%02X, ADCOFFSET = 0x%02X, ADCGAIN2 = 0x%02X\r\n",
                 adc_gain1, adc_offset, adc_gain2);
  }
  else
  {
    UART1_Printf("ADC gain/offset read fail\r\n");
  }

  for (uint8_t cell = 1U; cell <= 15U; cell++)
  {
    uint8_t reg_hi = (uint8_t)(0x0CU + (uint8_t)((cell - 1U) * 2U));
    uint8_t reg_lo = (uint8_t)(reg_hi + 1U);
    uint8_t hi = 0U;
    uint8_t lo = 0U;

    if (BQ_ReadReg(reg_hi, &hi) == HAL_OK && BQ_ReadReg(reg_lo, &lo) == HAL_OK)
    {
      uint16_t raw = (uint16_t)(((uint16_t)hi << 8) | lo);
      UART1_Printf("VC%u raw = 0x%04X (%u)\r\n", cell, raw, raw);
    }
    else
    {
      UART1_Printf("VC%u read fail\r\n", cell);
    }
  }
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  UART1_Printf("\r\nBMS_BQ76940_Min start\r\n");
  BQ_Wake();
  BQ_BringupAndPrint();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    HAL_Delay(1000);
    BQ_BringupAndPrint();
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
