#ifndef __BMS_SOC_H
#define __BMS_SOC_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "bms_config.h"

/**
 * @file    bms_soc.h
 * @brief   BMS 库仑计SOC + 容量学习 + SOH + 循环计数 + Flash持久化
 *
 *  算法概述:
 *    1) 库仑积分:  ΔmAh = -I_mA * Δt / 3600000   (正放负充)
 *    2) OCV校准:   全部单体有效且压差≤50mV, 静置30s后修正
 *    3) 满充校准:   最高单体≥4150mV且|I|<200mA → SOC=100%
 *    4) 空电检测:   最低单体≤2800mV → SOC=0%
 *    5) 容量学习:   空电→满充 记录实际Ah, EMA更新 full_cap
 *    6) SOH:       learned_cap / nominal_cap × 100
 *    7) 循环计数:   累积充电量 ≥ nominal_cap 则 +1 循环
 *    8) Flash保存:  每60s检查, 变化≥1% full_cap 才写入(减少擦写)
 */

/* ================================================================
 *  配置参数 (根据实际电池包修改)
 * ================================================================ */
#define SOC_NOMINAL_CAP_MAH     BMS_NOMINAL_CAP_MAH
#define SOC_CONNECTED_CELLS     BMS_CELL_COUNT
#define SOC_FULL_CELL_MV        BMS_FULL_CELL_MV
#define SOC_EMPTY_CELL_MV       BMS_EMPTY_CELL_MV
#define SOC_FULL_CURRENT_MA     200         /* 充满判定电流阈值 mA
                                             * |I| < 此值 且 V ≥ FULL → 认为满充 */
#define SOC_IDLE_CURRENT_MA     50          /* 静置判定电流阈值 mA */
#define SOC_IDLE_TIME_MS        30000       /* 静置时间后做OCV校准 (30s) */
#define SOC_OCV_MAX_CELL_DELTA_MV 50        /* OCV校准允许的最大单体压差 */
#define SOC_SAMPLE_PERIOD_MS    500         /* 主循环采样周期 ms */

/* ---- Flash持久化 ---- */
#define SOC_FLASH_SAVE_INTERVAL_S  60       /* Flash检查保存间隔 s */
#define SOC_FLASH_PAGE_SIZE       1024u    /* STM32F103C8 每页 1KB */
#define SOC_FLASH_ADDR          0x0800FC00  /* 存储页地址
                                             * STM32F103C8 最后 1KB 页 */
#define SOC_FLASH_MAGIC         0x534F4321  /* 魔数 "SOC!" */

/* ================================================================
 *  SOC 运行状态结构
 * ================================================================ */
typedef struct {
    /* ---- 核心SOC ---- */
    float    soc_mAh;               /* 剩余容量 mAh */
    float    full_cap_mAh;          /* 学习到的满充容量 mAh */
    uint8_t  soc_percent;           /* SOC 百分比 0~100 */

    /* ---- 健康度 ---- */
    uint8_t  soh_percent;           /* SOH 百分比 0~100 */

    /* ---- 循环计数 ---- */
    uint16_t cycle_count_x10;       /* 循环次数 ×10 (0.1次分辨率) */
    float    chg_accum_mAh;         /* 充电累积量(用于循环计数) */

    /* ---- OCV校准 ---- */
    uint32_t idle_timer_ms;         /* 静置计时 ms */

    /* ---- 容量学习 ---- */
    uint8_t  cap_learn_armed;       /* 1=经过空电, 等待满充学习 */
    float    cap_learn_start_mAh;   /* 学习起点剩余容量 */

    /* ---- Flash管理 ---- */
    uint32_t flash_timer_ms;        /* Flash保存计时 ms */
    float    last_saved_mAh;        /* 上次保存时的 soc_mAh */
} SOC_State_t;

/* ================================================================
 *  公开函数
 * ================================================================ */

/**
 * @brief  SOC模块初始化 — 从Flash恢复或设为默认值
 * @note   应在 BQ76940_Init() 之后调用
 */
void SOC_Init(void);

/**
 * @brief  SOC主更新, 每个采样周期调用一次
 * @param  current_mA  当前电流 (正=放电, 负=充电)
 * @param  avg_cell_mv 已连接电芯平均电压 mV
 * @param  max_cell_mv 最高有效单体电压 mV
 * @param  min_cell_mv 最低有效单体电压 mV
 * @param  calibration_valid 1=电压/温度有效且无数据故障, 0=禁止电压校准
 * @param  chg_on      充电MOS状态 (1=开)
 * @param  dt_ms       距上次调用的时间间隔 ms
 */
void SOC_Update(int current_mA, int avg_cell_mv,
                int max_cell_mv, int min_cell_mv,
                uint8_t calibration_valid, uint8_t chg_on, uint32_t dt_ms);

/**
 * @brief  立即将SOC数据写入Flash
 */
void SOC_SaveToFlash(void);

/**
 * @brief  获取SOC状态结构指针 (只读使用)
 */
SOC_State_t *SOC_GetState(void);

/**
 * @brief  OCV电压查表, 单体电压→SOC百分比
 * @param  cell_mv  单体电压 mV
 * @return SOC 0~100
 */
uint8_t SOC_OcvLookup(int cell_mv);

#ifdef __cplusplus
}
#endif

#endif /* __BMS_SOC_H */
