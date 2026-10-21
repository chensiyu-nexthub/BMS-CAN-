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
#include "dma.h"
#include "iwdg.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
  #include "sw_i2c.h"
  #include "bq76940.h"
  #include "bms_soc.h"

  /* FreeRTOS头文件 */
  #include "FreeRTOS.h"
  #include "task.h"
  #include "semphr.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define WDG_CHECK_INTERVAL_MS       100u
#define BMS_HEARTBEAT_TIMEOUT_MS   1500u
#define UART_HEARTBEAT_TIMEOUT_MS   500u

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* ===================== FreeRTOS 任务句柄与同步量 ===================== */
static TaskHandle_t      xBmsTaskHandle  = NULL;   /* BMS采集/保护任务 */
static TaskHandle_t      xUartTaskHandle = NULL;   /* 串口命令处理任务 */
SemaphoreHandle_t        xI2CMutex       = NULL;   /* I2C总线互斥量 (全局, usart.c也可能用到) */

/* 32-bit aligned counters are atomically readable on Cortex-M3. */
static volatile uint32_t s_bms_heartbeat = 0;
static volatile uint32_t s_uart_heartbeat = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
static void BMS_Task(void *pvParameters);
static void UART_Task(void *pvParameters);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/* ===================== 串口命令接收缓冲 ===================== */

/*============================================================================
 *  BMS_Task — 电池采集与保护 (周期500ms, ALERT可立即唤醒)
 *
 *  职责: BQ76940通信检查 → 数据更新 → SOC计算 → 保护判断 → ALERT处理
 *  优先级: 3 (最高应用级, 保护响应优先)
 *  栈大小: 256字 = 1024字节 (I2C + 数据处理)
 *  唤醒: ulTaskNotifyTake — 正常500ms超时唤醒, ALERT中断可立即唤醒
 *============================================================================*/
static void BMS_Task(void *pvParameters)
{
    (void)pvParameters;
    TickType_t xLastWakeTime = xTaskGetTickCount();

    for (;;)
    {
        /* 阻塞等待: 500ms超时 或 ALERT中断的xTaskNotifyGive提前唤醒 */
        ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(500));

        xSemaphoreTake(xI2CMutex, portMAX_DELAY);

        /* 每一轮都推进时间基准; 通信失败后不能把整段停机时间补进SOC. */
        TickType_t xNow = xTaskGetTickCount();
        uint32_t dt_ms = (uint32_t)(xNow - xLastWakeTime);
        xLastWakeTime = xNow;
        if (dt_ms > BMS_SOC_MAX_DT_MS)
            dt_ms = BMS_SOC_MAX_DT_MS;

        /* ALERT 中断触发 → 立即读硬件报警 + 保护检查 */
        if (g_bq_alert_flag) {
            g_bq_alert_flag = 0;
            BQ76940_ReadAlerts();
            BQ76940_ProtectionCheck();
        }

        /* 周期性通信检查 + 数据更新 */
        if (BQ76940_CommCheck()) {
            BQ76940_UpdateAll();
            BQ76940_ReadAlerts();
            BQ76940_ProtectionCheck();

            /* SOC库仑计更新 — 每次采集后调用 */
            {
                int avg_cell_mv = bms_data.avg_cell_mv;  /* 由UpdateAll()动态计算 */
                int max_cell_mv = 0;
                int min_cell_mv = 0;
                uint8_t valid_cell_count = 0;

                for (int i = 0; i < CELL_COUNT; i++)
                {
                    if (!bms_data.cell_valid[i] || bms_data.cell_mv[i] <= 1000)
                        continue;

                    if (valid_cell_count == 0 || bms_data.cell_mv[i] > max_cell_mv)
                        max_cell_mv = bms_data.cell_mv[i];
                    if (valid_cell_count == 0 || bms_data.cell_mv[i] < min_cell_mv)
                        min_cell_mv = bms_data.cell_mv[i];
                    valid_cell_count++;
                }

                BQ76940_Protection_t protection;
                BQ76940_ProtectionGet(&protection);
                uint8_t calibration_valid =
                    (valid_cell_count == CELL_COUNT && bms_data.temp_valid &&
                     !protection.data_fault) ? 1 : 0;

                /* 电流通信失败时停止库仑积分, 防止用旧电流补算整段间隔. */
                if (bms_data.current_valid)
                {
                    SOC_Update(bms_data.current_mA,
                               avg_cell_mv,
                               max_cell_mv,
                               min_cell_mv,
                               calibration_valid,
                               bms_data.chg_on,
                               dt_ms);

                    /* 同步SOC百分比到bms_data (供串口status命令显示) */
                    SOC_State_t *soc = SOC_GetState();
                    bms_data.soc_percent = soc->soc_percent;
                }
            }
        }

        xSemaphoreGive(xI2CMutex);
        s_bms_heartbeat++;
    }
}

/*============================================================================
 *  UART_Task — 串口命令处理 (周期50ms)
 *
 *  职责: 轮询串口命令就绪标志 → 解析执行
 *  优先级: 2 (低于BMS, 命令响应可稍延迟)
 *  栈大小: 256字 = 1024字节 (snprintf + 字符串处理)
 *  I2C保护: 执行命令前获取互斥量, 避免与BMS_Task竞争总线
 *============================================================================*/
static void UART_Task(void *pvParameters)
{
    (void)pvParameters;

    for (;;)
    {
        /* 获取I2C互斥量后再处理命令 (bal/chg/dsg等命令需要I2C操作) */
        xSemaphoreTake(xI2CMutex, portMAX_DELAY);
        UART_CmdProcess();
        xSemaphoreGive(xI2CMutex);

        /* 50ms轮询, CLI响应足够灵敏 */
        vTaskDelay(pdMS_TO_TICKS(50));
        s_uart_heartbeat++;
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
  MX_DMA_Init();
  MX_USART1_UART_Init();
  MX_IWDG_Init();
  /* USER CODE BEGIN 2 */
  SW_I2C_Init();
  BQ76940_Init();
  SOC_Init();                       /* SOC模块初始化(Flash恢复) */
  UART_StartReceive();              /* 启动串口命令接收 */
  UART_DMA_Init();                   /* 初始化DMA发送信号量 */

  /* ---- 创建FreeRTOS同步量 ---- */
  xI2CMutex = xSemaphoreCreateMutex();
  configASSERT(xI2CMutex);          /* 创建失败=堆不足, 直接停机 */

  /* ---- 创建应用任务 ----
   *  任务              优先级  栈(字)   说明
   *  BMS_Task          3       256      电池采集+保护, 最高
   *  UART_Task         2       256      串口命令处理
   *  Idle(内建)        0       128      空闲任务
   */
  xTaskCreate(BMS_Task,  "BMS",  256, NULL, 3, &xBmsTaskHandle);
  xTaskCreate(UART_Task, "UART", 256, NULL, 2, &xUartTaskHandle);

  /* 注册BMS_Task接收ALERT中断的任务通知 */
  BQ76940_SetAlertTask((void *)xBmsTaskHandle);

  /* ---- 启动调度器 (不返回) ---- */
  vTaskStartScheduler();

  /* 到这里说明堆不足, 无法创建Idle任务 */
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    /* 不应到达此处 — FreeRTOS调度器异常 */
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
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

/*============================================================================
 *  vApplicationIdleHook — FreeRTOS空闲钩子
 *  当CPU无其他任务可调度时自动调用
 *  职责:
 *    1. 检查BMS/UART心跳, 两者按时推进才刷新IWDG
 *       任一任务死循环、返回或卡死超过时限都会停止喂狗并复位
 *    2. WFI省电 — 等待中断唤醒, 降低CPU功耗
 *============================================================================*/
void vApplicationIdleHook(void)
{
    static uint8_t initialized = 0;
    static uint32_t last_check_tick = 0;
    static uint32_t last_bms_heartbeat = 0;
    static uint32_t last_uart_heartbeat = 0;
    static uint32_t last_bms_change_tick = 0;
    static uint32_t last_uart_change_tick = 0;

    uint32_t now = HAL_GetTick();

    if (!initialized)
    {
        initialized = 1;
        last_check_tick = now;
        last_bms_heartbeat = s_bms_heartbeat;
        last_uart_heartbeat = s_uart_heartbeat;
        last_bms_change_tick = now;
        last_uart_change_tick = now;
        __WFI();
        return;
    }

    if ((uint32_t)(now - last_check_tick) < WDG_CHECK_INTERVAL_MS)
    {
        __WFI();
        return;
    }
    last_check_tick = now;

    uint32_t bms_heartbeat = s_bms_heartbeat;
    uint32_t uart_heartbeat = s_uart_heartbeat;

    if (bms_heartbeat != last_bms_heartbeat)
    {
        last_bms_heartbeat = bms_heartbeat;
        last_bms_change_tick = now;
    }

    if (uart_heartbeat != last_uart_heartbeat)
    {
        last_uart_heartbeat = uart_heartbeat;
        last_uart_change_tick = now;
    }

    uint8_t bms_alive =
        (uint32_t)(now - last_bms_change_tick) <= BMS_HEARTBEAT_TIMEOUT_MS;
    uint8_t uart_alive =
        (uint32_t)(now - last_uart_change_tick) <= UART_HEARTBEAT_TIMEOUT_MS;

    if (bms_alive && uart_alive)
        HAL_IWDG_Refresh(&hiwdg);

    __WFI();
}

/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM3 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM3)
  {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

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
#ifdef USE_FULL_ASSERT
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
