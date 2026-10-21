/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.c
  * @brief   This file provides code for the configuration
  *          of the USART instances.
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
#include "usart.h"

/* USER CODE BEGIN 0 */
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "bq76940.h"

/* FreeRTOS — DMA发送完成信号量 */
#include "FreeRTOS.h"
#include "semphr.h"

/* ===================== 串口命令接收缓冲 ===================== */
static uint8_t          s_rx_byte   = 0;       /* 单字节接收中转 */
static char             s_rx_line[48] = {0};   /* 行缓冲 */
static uint8_t          s_rx_pos    = 0;       /* 当前写入位置 */
static volatile uint8_t s_cmd_ready = 0;       /* 1=有一行待解析 */
/* ========================================================== */

/* =============== DMA发送完成信号量 =============== */
static SemaphoreHandle_t xUartTxDone = NULL;
/* ==================================================== */

#define UART_DMA_TX_BUFFER_SIZE  128u
#define UART_DMA_TX_TIMEOUT_MS   500u

/* DMA must never read a caller stack buffer after UART_DMA_Send returns. */
static uint8_t s_uart_tx_buf[UART_DMA_TX_BUFFER_SIZE];
static volatile uint8_t s_uart_tx_fault = 0;

static void BMS_ParseCmd(const char *line);    /* 前向声明 */
/* USER CODE END 0 */

UART_HandleTypeDef huart1;
DMA_HandleTypeDef hdma_usart1_tx;

/* USART1 init function */

void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */
    /* USART1 clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART1 DMA Init */
    /* USART1_TX Init */
    hdma_usart1_tx.Instance = DMA1_Channel4;
    hdma_usart1_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_usart1_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart1_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart1_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart1_tx.Init.Mode = DMA_NORMAL;
    hdma_usart1_tx.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_usart1_tx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmatx,hdma_usart1_tx);

    /* USART1 interrupt Init */
    /* 优先级6: 需≥5才能安全调用FreeRTOS FromISR API */
    HAL_NVIC_SetPriority(USART1_IRQn, 6, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspInit 1 */

  /* USER CODE END USART1_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspDeInit 0 */

  /* USER CODE END USART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();

    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9|GPIO_PIN_10);

    /* USART1 DMA DeInit */
    HAL_DMA_DeInit(uartHandle->hdmatx);

    /* USART1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspDeInit 1 */

  /* USER CODE END USART1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
/*============================================================
 *  启动串口中断接收 (main初始化时调一次)
 *============================================================*/
void UART_StartReceive(void)
{
    HAL_UART_Receive_IT(&huart1, &s_rx_byte, 1);
}

/*============================================================
 *  HAL串口接收完成回调 (中断上下文)
 *  每收到1字节触发一次, 收到换行符时置命令就绪标志
 *============================================================*/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance != USART1) return;

    if (s_rx_byte == '\r' || s_rx_byte == '\n') {
        /* 收到换行, 结束当前行 */
        if (s_rx_pos > 0) {
            s_rx_line[s_rx_pos] = '\0';
            s_cmd_ready = 1;
            s_rx_pos = 0;
        }
    } else {
        /* 普通字符, 追加到行缓冲(防溢出) */
        if (s_rx_pos < (uint8_t)(sizeof(s_rx_line) - 1)) {
            s_rx_line[s_rx_pos++] = (char)s_rx_byte;
        }
    }

    /* 重新挂起接收, 等下一个字节 */
    HAL_UART_Receive_IT(&huart1, &s_rx_byte, 1);
}

/*============================================================
 *  UART_DMA_Init — 初始化DMA发送信号量 (main中调一次)
 *============================================================*/
void UART_DMA_Init(void)
{
    s_uart_tx_fault = 0;
    xUartTxDone = xSemaphoreCreateBinary();
}

/*============================================================
 *  UART_DMA_Send — DMA方式发送数据, 阻塞当前任务直到完成
 *  优势: 发送期间CPU释放给其他任务, 不再空转等待
 *  注意: 只能在FreeRTOS任务中调用 (不能在中断中用)
 *============================================================*/
void UART_DMA_Send(const uint8_t *data, uint16_t len)
{
    if (data == NULL || len == 0 || len > UART_DMA_TX_BUFFER_SIZE ||
        xUartTxDone == NULL)
        return;

    /* A previous timeout may have left the peripheral busy. Abort it before reuse. */
    if (s_uart_tx_fault || huart1.gState != HAL_UART_STATE_READY ||
        hdma_usart1_tx.State != HAL_DMA_STATE_READY)
    {
        (void)HAL_UART_AbortTransmit(&huart1);
        while (xSemaphoreTake(xUartTxDone, 0) == pdTRUE)
        {
        }

        if (huart1.gState != HAL_UART_STATE_READY ||
            hdma_usart1_tx.State != HAL_DMA_STATE_READY)
        {
            s_uart_tx_fault = 1;
            return;
        }
        s_uart_tx_fault = 0;
    }

    memcpy(s_uart_tx_buf, data, len);
    if (HAL_UART_Transmit_DMA(&huart1, s_uart_tx_buf, len) != HAL_OK)
        return;

    if (xSemaphoreTake(xUartTxDone, pdMS_TO_TICKS(UART_DMA_TX_TIMEOUT_MS)) != pdTRUE)
    {
        /* Keep the static buffer untouched until DMA is definitely stopped. */
        if (HAL_UART_AbortTransmit(&huart1) != HAL_OK ||
            huart1.gState != HAL_UART_STATE_READY ||
            hdma_usart1_tx.State != HAL_DMA_STATE_READY)
        {
            s_uart_tx_fault = 1;
            return;
        }

        while (xSemaphoreTake(xUartTxDone, 0) == pdTRUE)
        {
        }
    }
}

/*============================================================
 *  HAL DMA发送完成回调 (中断上下文)
 *  DMA传输结束后由HAL调用, 唤醒等待中的UART_DMA_Send
 *============================================================*/
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1) {
        if (xUartTxDone == NULL)
            return;
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        xSemaphoreGiveFromISR(xUartTxDone, &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}

/*============================================================
 *  串口命令处理 (main循环里每轮调一次)
 *  检查是否有完整命令, 有则解析执行
 *============================================================*/
void UART_CmdProcess(void)
{
    if (s_cmd_ready) {
        s_cmd_ready = 0;
        BMS_ParseCmd(s_rx_line);
    }
}

/*============================================================
 *  解析并执行一行ASCII命令
 *  在主循环中调用(非中断), 可以安全操作I2C
 *============================================================*/
static void BMS_ParseCmd(const char *line)
{
    char     reply[64];
    uint16_t mask;

    /* ---------- bal 系列 ---------- */
    if (strncmp(line, "bal ", 4) == 0) {
        const char *arg = line + 4;

        if (strcmp(arg, "off") == 0) {
            BQ76940_ClearBalance();
            snprintf(reply, sizeof(reply), "[CMD] Balance OFF\r\n");

        } else if (strcmp(arg, "odd") == 0) {
            /* Logical cells 1,3,5,7,9: bit0,2,4,6,8 = 0x0155 */
            BQ76940_SetBalanceLogical(0x0155);
            snprintf(reply, sizeof(reply), "[CMD] Balance ODD cells\r\n");

        } else if (strcmp(arg, "even") == 0) {
            /* Logical cells 2,4,6,8: bit1,3,5,7 = 0x00AA */
            BQ76940_SetBalanceLogical(0x00AA);
            snprintf(reply, sizeof(reply), "[CMD] Balance EVEN cells\r\n");

        } else if (strcmp(arg, "status") == 0) {
            mask = BQ76940_GetBalanceMaskLogical();
            snprintf(reply, sizeof(reply), "[CMD] Balance mask = 0x%04X\r\n", mask);

        } else if (strncmp(arg, "mask ", 5) == 0) {
            /* bal mask 0x0155 - set logical 9-cell balancing mask */
            uint16_t m = (uint16_t)strtol(arg + 5, NULL, 0);
            BQ76940_SetBalanceLogical(m);
            snprintf(reply, sizeof(reply), "[CMD] Balance mask set 0x%04X\r\n", m);

        } else {
            /* Parse logical cell number 1~9. */
            int cell = atoi(arg);
            if (cell >= 1 && cell <= BMS_CELL_COUNT) {
                BQ76940_SetBalanceLogical((uint16_t)(1u << (cell - 1)));
                snprintf(reply, sizeof(reply), "[CMD] Balance cell %d ON\r\n", cell);
            } else {
                snprintf(reply, sizeof(reply), "[ERR] Unknown bal arg: %s\r\n", arg);
            }
        }

    /* ---------- chg 系列 ---------- */
    } else if (strcmp(line, "chg on") == 0) {
        BQ76940_OpenCHG();
        BQ76940_ReadAlerts();
        BQ76940_ProtectionCheck();
        snprintf(reply, sizeof(reply), "[CMD] CHG request ON, actual=%s\r\n",
                 bms_data.chg_on ? "ON" : "OFF");

    } else if (strcmp(line, "chg off") == 0) {
        BQ76940_CloseCHG();
        snprintf(reply, sizeof(reply), "[CMD] CHG MOS OFF\r\n");

    /* ---------- dsg 系列 ---------- */
    } else if (strcmp(line, "dsg on") == 0) {
        BQ76940_OpenDSG();
        BQ76940_ReadAlerts();
        BQ76940_ProtectionCheck();
        snprintf(reply, sizeof(reply), "[CMD] DSG request ON, actual=%s\r\n",
                 bms_data.dsg_on ? "ON" : "OFF");

    } else if (strcmp(line, "dsg off") == 0) {
        BQ76940_CloseDSG();
        snprintf(reply, sizeof(reply), "[CMD] DSG MOS OFF\r\n");

    /* ---------- mos 系列 ---------- */
    } else if (strcmp(line, "mos on") == 0) {
        BQ76940_OpenAll();
        BQ76940_ReadAlerts();
        BQ76940_ProtectionCheck();
        snprintf(reply, sizeof(reply), "[CMD] MOS request ON, CHG=%s DSG=%s\r\n",
                 bms_data.chg_on ? "ON" : "OFF",
                 bms_data.dsg_on ? "ON" : "OFF");

    } else if (strcmp(line, "mos off") == 0) {
        BQ76940_CloseAll();
        snprintf(reply, sizeof(reply), "[CMD] CHG+DSG MOS ALL OFF\r\n");

    /* ---------- status ---------- */
    } else if (strcmp(line, "status") == 0) {
        BQ76940_PrintData(&huart1);
        return;   /* PrintData 自己打印, 不再发 reply */

    /* ---------- 未知命令 ---------- */
    } else {
        snprintf(reply, sizeof(reply), "[ERR] Unknown cmd: %s\r\n", line);
    }

    UART_DMA_Send((uint8_t *)reply, strlen(reply));
}

/* USER CODE END 0 */

/* USER CODE END 1 */

