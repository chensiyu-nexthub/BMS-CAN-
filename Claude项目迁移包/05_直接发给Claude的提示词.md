# 直接发给Claude的提示词

你现在要接手我的毕业设计BMS项目。请先阅读这个迁移包里的所有md文件，再继续指导我。

项目根路径：

`E:\07xianmu\BMS资料`

请重点先读：

1. `E:\07xianmu\BMS资料\Claude项目迁移包\01_项目总览_给Claude先读.md`
2. `E:\07xianmu\BMS资料\Claude项目迁移包\02_硬件与资料路径.md`
3. `E:\07xianmu\BMS资料\Claude项目迁移包\03_已完成工作与当前状态.md`
4. `E:\07xianmu\BMS资料\Claude项目迁移包\04_接下来执行路线.md`

然后再根据需要读这些原始资料：

- `E:\07xianmu\BMS资料\BQ76940发货资料 20220817\1.产品规格说明书\BQ76940  BMS规格说明书20210706.pdf`
- `E:\07xianmu\BMS资料\BQ76940发货资料 20220817\1.产品规格说明书\bq76940通信协议文档（20210104更新）.pdf`
- `E:\07xianmu\BMS资料\BQ76940发货资料 20220817\2.原理图\BQ76940_CAN_20210906.pdf`
- `E:\07xianmu\BMS资料\BQ76940发货资料 20220817\5.芯片资料其它资料\9.HC-08蓝牙资料.pdf`
- `E:\07xianmu\BMS资料\项目代码\BMS-main\BMS_program _stm32F103c8t6\BMS_HAL`
- `E:\07xianmu\BMS资料\daima\BMS_BQ76940_Min`

重要要求：

- 全程使用中文简体
- 不要编造资料里没有的型号、引脚、协议
- 不要把F407 Bootloader项目和当前BMS商家板主线混在一起
- 当前主线是STM32F103C8T6 + BQ76940商家BMS板
- 先带我打通BQ76940通信，不要一上来做蓝牙/屏幕/上位机
- 每一步都要告诉我“为什么这么做、怎么接、怎么验证、错了怎么排查”
- 我需要真正理解项目，不是复制代码

当前关键状态：

- Keil工程已能编译
- 串口已有启动输出
- 目前BQ76940读寄存器失败，串口显示：

```text
BMS_BQ76940_Min start
=== BQ76940 bring-up ===
SYS_STAT read fail
ADC gain/offset read fail
VC1~VC15 read fail
```

请从这个状态继续带我做。
