#ifndef __BMS_CONFIG_H
#define __BMS_CONFIG_H

/*
 * BMS hardware and battery configuration.
 *
 * Keep hardware topology here. Algorithm modules must not redefine the
 * connected-cell count or active BQ76940 channel map independently.
 */

/* Logical cells currently connected to the pack. */
#define BMS_CELL_COUNT              9

/* BQ76940 cell-voltage register slots, VC1..VC15. */
#define BQ_HW_CELL_SLOTS            15

/*
 * Active BQ76940 voltage-register slots for the 9-cell configuration.
 * Zero-based values correspond to VC1, VC2, VC5, VC6, VC7,
 * VC10, VC11, VC12 and VC15.
 */
#define BMS_ACTIVE_CELL_MAP_INIT    {0u, 1u, 4u, 5u, 6u, 9u, 10u, 11u, 14u}
#define BMS_ACTIVE_CELL_MAP_COUNT   9

/* CELLBAL register bits for the active cells above. */
#define BMS_ACTIVE_BALANCE_MASK     0x4E73u

/* Current-sense resistor value. */
#define BMS_SHUNT_MILLIOHM          4u

/* Current software thresholds are intentionally unchanged. */
#define BMS_NOMINAL_CAP_MAH         20000
#define BMS_FULL_CELL_MV            4150
#define BMS_EMPTY_CELL_MV           2800
#define BMS_OV_TRIP_MV              4200
#define BMS_OV_RECOVER_MV           4100
#define BMS_UV_TRIP_MV              2800
#define BMS_UV_RECOVER_MV           3000
#define BMS_OC_TRIP_MA              5000
#define BMS_OC_RECOVER_MA           4500
#define BMS_OT_TRIP_C               60
#define BMS_OT_RECOVER_C            55
#define BMS_LT_TRIP_C               0
#define BMS_LT_RECOVER_C            5

/* Consecutive invalid samples before entering a fail-safe data fault. */
#define BMS_INVALID_SAMPLE_LIMIT    3

/* Consecutive fully-valid samples required before clearing a data fault. */
#define BMS_DATA_RECOVER_SAMPLES    3

#if BMS_CELL_COUNT == 0
#error "BMS_CELL_COUNT must be greater than zero"
#endif

#if BMS_CELL_COUNT > BQ_HW_CELL_SLOTS
#error "BMS_CELL_COUNT cannot exceed BQ_HW_CELL_SLOTS"
#endif

#if BMS_ACTIVE_CELL_MAP_COUNT != BMS_CELL_COUNT
#error "Active-cell map size must equal BMS_CELL_COUNT"
#endif

#endif /* __BMS_CONFIG_H */
