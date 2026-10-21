/**
 * @file    bms_soc.c
 * @brief   库仑计SOC + 容量学习 + SOH + 循环计数 + Flash持久化
 *
 *  【算法流程 (每500ms调用一次)】
 *  ┌─────────────────────────────────────────────┐
 *  │ 1. 库仑积分: soc_mAh -= I * dt / 3600000   │
 *  │ 2. 循环计数: 累积|充电量| ≥ nominal → +1    │
 *  │ 3. OCV校准:  单体全有效且压差≤50mV才允许   │
 *  │ 4. 满充校准:  最高单体≥4150 & |I|<200       │
 *  │ 5. 空电检测:  最低单体≤2800 → SOC=0%       │
 *  │ 6. 容量学习:  空→满 记录Ah, EMA更新 full_cap │
 *  │ 7. 限幅 & 计算百分比                        │
 *  │ 8. 定时Flash保存(变化≥1%才真正写入)         │
 *  └─────────────────────────────────────────────┘
 *
 *  【Flash存储结构】 (共20字节, STM32F103 以半字编程)
 *    magic(4) + soc_mAh(4) + full_cap_mAh(4) + cycle_x10(2)
 *    + pad(2) + checksum(4)
 *
 *  【Flash磨损控制】
 *    最后1KB保留为日志页, 每20字节记录顺序追加, 最多51条.
 *    写满后才擦除整页, 将每次擦写周期扩展到约51次保存.
 *    启动时从页内选择地址最高且校验通过的有效记录恢复.
 */

/* Includes ------------------------------------------------------------------*/
#include "bms_soc.h"
#include "stm32f1xx_hal.h"
#include <string.h>
#include <stddef.h>               /* offsetof */

/* ================================================================
 *  OCV查找表 (单体电压 → SOC百分比)
 *  适用于三元锂电池(NMC), 根据实际曲线调整
 * ================================================================ */
static const struct {
    int     mv;
    uint8_t soc;
} s_ocv[] = {
    {4150, 100},
    {4100,  95},
    {4050,  90},
    {4000,  85},
    {3950,  80},
    {3900,  75},
    {3850,  70},
    {3800,  65},
    {3750,  60},
    {3700,  50},
    {3650,  40},
    {3600,  30},
    {3500,  20},
    {3400,  15},
    {3300,  10},
    {3200,   5},
    {3100,   3},
    {3000,   1},
    {2800,   0},
};
#define OCV_TABLE_SIZE  (sizeof(s_ocv) / sizeof(s_ocv[0]))

/* ================================================================
 *  Flash 存储布局
 * ================================================================ */
typedef struct {
    uint32_t magic;               /* SOC_FLASH_MAGIC */
    float    soc_mAh;             /* 剩余容量 */
    float    full_cap_mAh;        /* 学习到的满充容量 */
    uint16_t cycle_count_x10;     /* 循环×10 */
    uint16_t _pad;                /* 对齐填充 */
    uint32_t checksum;            /* 异或校验 */
} SOC_Flash_t;                    /* 共20字节 */

#define SOC_FLASH_END_ADDR      (SOC_FLASH_ADDR + SOC_FLASH_PAGE_SIZE)
#define SOC_FLASH_RECORD_SIZE   ((uint32_t)sizeof(SOC_Flash_t))

/* A layout change must be reviewed together with the on-flash record format. */
typedef char SOC_Flash_size_check_t[(sizeof(SOC_Flash_t) == 20u) ? 1 : -1];

/* ================================================================
 *  内部变量
 * ================================================================ */
static SOC_State_t s_soc;
static uint32_t s_flash_next_addr = SOC_FLASH_ADDR;

/* ================================================================
 *  OCV 查表
 * ================================================================ */
uint8_t SOC_OcvLookup(int cell_mv)
{
    for (uint32_t i = 0; i < OCV_TABLE_SIZE; i++) {
        if (cell_mv >= s_ocv[i].mv)
            return s_ocv[i].soc;
    }
    return 0;
}

/* ================================================================
 *  Flash 校验和 — 对 checksum 字段之前的数据做半字异或
 * ================================================================ */
static uint32_t flash_checksum(const SOC_Flash_t *p)
{
    const uint16_t *hw = (const uint16_t *)p;
    uint32_t n = offsetof(SOC_Flash_t, checksum) / 2;
    uint32_t cs = 0;
    for (uint32_t i = 0; i < n; i++)
        cs ^= hw[i];
    return cs ^ 0xA5A5A5A5u;
}

/* ================================================================
 *  Flash 读取 — 从最后页恢复SOC数据
 *  @return 0=成功, -1=无效(首次使用或数据损坏)
 * ================================================================ */
static int flash_record_valid(const SOC_Flash_t *fp)
{
    if (fp->magic != SOC_FLASH_MAGIC)
        return 0;
    if (fp->checksum != flash_checksum(fp))
        return 0;
    if (!(fp->soc_mAh >= 0.0f) || !(fp->soc_mAh <= fp->full_cap_mAh))
        return 0;
    if (!(fp->full_cap_mAh >= 1000.0f) ||
        !(fp->full_cap_mAh <= (float)SOC_NOMINAL_CAP_MAH * 1.5f))
        return 0;
    return 1;
}

static int flash_range_blank(uint32_t addr, uint32_t len)
{
    for (uint32_t offset = 0; offset < len; offset += 2u)
    {
        if (*(volatile uint16_t *)(addr + offset) != 0xFFFFu)
            return 0;
    }
    return 1;
}

static int flash_erase_soc_page(void)
{
    FLASH_EraseInitTypeDef erase = {0};
    uint32_t page_err = 0xFFFFFFFFu;

    erase.TypeErase = FLASH_TYPEERASE_PAGES;
    erase.PageAddress = SOC_FLASH_ADDR;
    erase.NbPages = 1;

    if (HAL_FLASHEx_Erase(&erase, &page_err) != HAL_OK ||
        page_err != 0xFFFFFFFFu)
        return -1;

    return flash_range_blank(SOC_FLASH_ADDR, SOC_FLASH_PAGE_SIZE) ? 0 : -1;
}

static int flash_program_record(const SOC_Flash_t *record, uint32_t addr)
{
    const uint16_t *src = (const uint16_t *)record;

    for (uint32_t i = 0; i < SOC_FLASH_RECORD_SIZE / 2u; i++)
    {
        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,
                              addr + i * 2u, src[i]) != HAL_OK)
            return -1;
    }

    for (uint32_t i = 0; i < SOC_FLASH_RECORD_SIZE / 2u; i++)
    {
        if (*(volatile uint16_t *)(addr + i * 2u) != src[i])
            return -1;
    }
    return 0;
}

static int flash_load(void)
{
    const SOC_Flash_t *latest = NULL;
    uint32_t next_addr = SOC_FLASH_ADDR;

    for (uint32_t addr = SOC_FLASH_ADDR;
         addr + SOC_FLASH_RECORD_SIZE <= SOC_FLASH_END_ADDR;
         addr += SOC_FLASH_RECORD_SIZE)
    {
        uint16_t first_halfword = *(volatile uint16_t *)addr;
        if (first_halfword == 0xFFFFu)
        {
            continue;
        }

        /* Address order is the log order; resume after the last nonblank slot. */
        next_addr = addr + SOC_FLASH_RECORD_SIZE;
        const SOC_Flash_t *fp = (const SOC_Flash_t *)addr;
        if (flash_record_valid(fp))
            latest = fp;
    }

    s_flash_next_addr = next_addr;
    if (latest == NULL)
        return -1;

    s_soc.soc_mAh = latest->soc_mAh;
    s_soc.full_cap_mAh = latest->full_cap_mAh;
    s_soc.cycle_count_x10 = latest->cycle_count_x10;
    return 0;
}

/* Append a record; erase only when the reserved page is exhausted. */
void SOC_SaveToFlash(void)
{
    SOC_Flash_t tmp;
    uint32_t write_addr = s_flash_next_addr;

    tmp.magic = SOC_FLASH_MAGIC;
    tmp.soc_mAh = s_soc.soc_mAh;
    tmp.full_cap_mAh = s_soc.full_cap_mAh;
    tmp.cycle_count_x10 = s_soc.cycle_count_x10;
    tmp._pad = 0;
    tmp.checksum = flash_checksum(&tmp);

    if (HAL_FLASH_Unlock() != HAL_OK)
        return;

    if (write_addr < SOC_FLASH_ADDR ||
        write_addr + SOC_FLASH_RECORD_SIZE > SOC_FLASH_END_ADDR ||
        !flash_range_blank(write_addr, SOC_FLASH_RECORD_SIZE))
    {
        if (flash_erase_soc_page() != 0)
            goto flash_done;
        write_addr = SOC_FLASH_ADDR;
    }

    if (flash_program_record(&tmp, write_addr) == 0)
    {
        s_flash_next_addr = write_addr + SOC_FLASH_RECORD_SIZE;
        s_soc.last_saved_mAh = s_soc.soc_mAh;
    }
    else
    {
        /* Skip a partially programmed record instead of reusing its halfwords. */
        s_flash_next_addr = write_addr + SOC_FLASH_RECORD_SIZE;
    }

flash_done:
    HAL_FLASH_Lock();
}

/* ================================================================
 *  SOC_Init — 模块初始化
 *  优先从Flash恢复, 失败则设为默认值(50%)
 * ================================================================ */
void SOC_Init(void)
{
    memset(&s_soc, 0, sizeof(s_soc));

    /* 默认值 */
    s_soc.full_cap_mAh = (float)SOC_NOMINAL_CAP_MAH;
    s_soc.soc_mAh      = (float)SOC_NOMINAL_CAP_MAH * 0.5f;   /* 默认50% */

    /* 尝试Flash恢复 */
    if (flash_load() == 0) {
        /* 恢复成功 — soc_mAh / full_cap_mAh / cycle_count_x10 已加载 */
    }

    /* 限幅 */
    if (s_soc.soc_mAh < 0.0f)
        s_soc.soc_mAh = 0.0f;
    if (s_soc.soc_mAh > s_soc.full_cap_mAh)
        s_soc.soc_mAh = s_soc.full_cap_mAh;

    /* 计算百分比 */
    s_soc.soc_percent = (uint8_t)(s_soc.soc_mAh * 100.0f
                                  / s_soc.full_cap_mAh + 0.5f);
    if (s_soc.soc_percent > 100) s_soc.soc_percent = 100;

    /* SOH */
    s_soc.soh_percent = (uint8_t)(s_soc.full_cap_mAh * 100.0f
                                  / (float)SOC_NOMINAL_CAP_MAH + 0.5f);
    if (s_soc.soh_percent > 100) s_soc.soh_percent = 100;

    /* 保存基准 */
    s_soc.last_saved_mAh = s_soc.soc_mAh;
}

/* ================================================================
 *  SOC_Update — 主更新函数 (每500ms调用一次)
 * ================================================================ */
void SOC_Update(int current_mA, int avg_cell_mv,
                int max_cell_mv, int min_cell_mv,
                uint8_t calibration_valid, uint8_t chg_on, uint32_t dt_ms)
{

    /* -------- 1. 库仑积分 --------
     * current_mA: 正=放电, 负=充电
     * 放电时 soc_mAh 减少, 充电时增加
     */
    float delta_mAh = -(float)current_mA * (float)dt_ms / 3600000.0f;
    s_soc.soc_mAh += delta_mAh;

    /* -------- 2. 循环计数 --------
     * 累积充电量 ≥ 标称容量 → 计1次循环
     */
    if (current_mA < 0) {
        float chg_mAh = (float)(-current_mA) * (float)dt_ms / 3600000.0f;
        s_soc.chg_accum_mAh += chg_mAh;
        while (s_soc.chg_accum_mAh >= (float)SOC_NOMINAL_CAP_MAH) {
            s_soc.chg_accum_mAh -= (float)SOC_NOMINAL_CAP_MAH;
            s_soc.cycle_count_x10 += 10;   /* +1.0 次循环 */
        }
    }

    /* -------- 3. OCV校准 (静置时) --------
     * |I| < 50mA 持续 30s → 用电压查表渐进修正
     */
    int cell_delta_mv = max_cell_mv - min_cell_mv;
    uint8_t ocv_input_valid =
        calibration_valid && max_cell_mv > 0 && min_cell_mv > 0 &&
        cell_delta_mv >= 0 &&
        cell_delta_mv <= (int)SOC_OCV_MAX_CELL_DELTA_MV;

    if (current_mA > -(int)SOC_IDLE_CURRENT_MA &&
        current_mA < (int)SOC_IDLE_CURRENT_MA && ocv_input_valid) {
        s_soc.idle_timer_ms += dt_ms;
        if (s_soc.idle_timer_ms >= SOC_IDLE_TIME_MS) {
            uint8_t ocv_soc = SOC_OcvLookup(avg_cell_mv);
            float ocv_mAh = s_soc.full_cap_mAh * (float)ocv_soc / 100.0f;
            /* 每次修正10%, 避免阶跃 */
            s_soc.soc_mAh += (ocv_mAh - s_soc.soc_mAh) * 0.1f;
            s_soc.idle_timer_ms = SOC_IDLE_TIME_MS;
        }
    } else {
        s_soc.idle_timer_ms = 0;
    }

    /* -------- 4. 满充检测 + 容量学习 --------
     * 条件: 充电MOS开, |I| < 200mA, V_cell ≥ 4150mV
     */
    /* Use the highest cell because it limits how far the pack can be charged. */
    if (calibration_valid && chg_on && current_mA <= 0
        && current_mA > -(int)SOC_FULL_CURRENT_MA
        && max_cell_mv >= (int)SOC_FULL_CELL_MV) {

        /* 容量学习: 空电→满充 期间累积的库仑量 */
        if (s_soc.cap_learn_armed) {
            float learned = s_soc.soc_mAh - s_soc.cap_learn_start_mAh;
            /* 合理性: 0.5×~1.5× 标称容量 */
            if (learned > (float)SOC_NOMINAL_CAP_MAH * 0.5f &&
                learned < (float)SOC_NOMINAL_CAP_MAH * 1.5f) {
                /* EMA更新: 80%旧值 + 20%新值 */
                s_soc.full_cap_mAh = s_soc.full_cap_mAh * 0.8f
                                   + learned * 0.2f;
                s_soc.soh_percent = (uint8_t)(s_soc.full_cap_mAh * 100.0f
                                              / (float)SOC_NOMINAL_CAP_MAH
                                              + 0.5f);
                if (s_soc.soh_percent > 100) s_soc.soh_percent = 100;
            }
            s_soc.cap_learn_armed = 0;
        }

        /* 满充校准: SOC钳至100% */
        s_soc.soc_mAh = s_soc.full_cap_mAh;
    }

    /* -------- 5. 空电检测 + 容量学习准备 --------
     * V_cell ≤ 2800mV → SOC=0%, 准备下次学习
     */
    /* Use the lowest cell because it limits usable discharge capacity. */
    if (calibration_valid && min_cell_mv <= (int)SOC_EMPTY_CELL_MV &&
        min_cell_mv > 0) {
        s_soc.soc_mAh = 0.0f;
        s_soc.cap_learn_armed = 1;
        s_soc.cap_learn_start_mAh = 0.0f;
    }

    /* -------- 6. 限幅 & 百分比 -------- */
    if (s_soc.soc_mAh < 0.0f) s_soc.soc_mAh = 0.0f;
    if (s_soc.soc_mAh > s_soc.full_cap_mAh)
        s_soc.soc_mAh = s_soc.full_cap_mAh;

    s_soc.soc_percent = (uint8_t)(s_soc.soc_mAh * 100.0f
                                  / s_soc.full_cap_mAh + 0.5f);
    if (s_soc.soc_percent > 100) s_soc.soc_percent = 100;

    /* -------- 7. 定时Flash保存 --------
     * 每60s检查, 变化 ≥ 1% full_cap 才真正写入
     */
    s_soc.flash_timer_ms += dt_ms;
    if (s_soc.flash_timer_ms >= (uint32_t)SOC_FLASH_SAVE_INTERVAL_S * 1000u) {
        s_soc.flash_timer_ms = 0;
        float diff = s_soc.soc_mAh - s_soc.last_saved_mAh;
        if (diff < 0.0f) diff = -diff;
        if (diff >= s_soc.full_cap_mAh * 0.01f) {
            SOC_SaveToFlash();
        }
    }
}

/* ================================================================
 *  获取状态结构指针
 * ================================================================ */
SOC_State_t *SOC_GetState(void)
{
    return &s_soc;
}
