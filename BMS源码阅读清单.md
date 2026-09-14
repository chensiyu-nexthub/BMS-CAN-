# BMS 源码阅读清单

> 目标：给你一个“打开就能照着看”的源码顺序和检查表。

---

## 1. 阅读顺序总原则

不要从头到尾逐行顺序读。  
正确顺序是：

1. 先看系统入口
2. 再看主链路任务
3. 再看核心驱动
4. 再看算法
5. 最后看调试和命令层

---

## 2. 第一优先级文件

### 2.1 main.c

路径：

[main.c](<E:\07xianmu\BMS资料\项目代码\BMS-main\BMS_program _stm32F103c8t6\BMS_HAL\Core\Src\main.c>)

看什么：

- 初始化顺序
- 创建了哪些任务
- 任务优先级怎么分
- `BMS_Task` 怎么跑
- `UART_Task` 怎么跑
- `xI2CMutex` 为什么存在

你读完必须能回答：

- 上电后最先做什么
- 为什么 `BMS_Task` 是主任务
- 为什么两个任务都要拿 I2C 锁

### 2.2 bq76940.h

路径：

[bq76940.h](<E:\07xianmu\BMS资料\项目代码\BMS-main\BMS_program _stm32F103c8t6\BMS_HAL\BSP\bq76940.h>)

看什么：

- 芯片寄存器定义
- 数据结构 `BQ76940_Data_t`
- 保护阈值宏
- 滤波参数宏
- 公共接口函数

你读完必须能回答：

- 系统保存了哪些电池状态
- 软件保护阈值是多少
- 采样有效范围是多少

---

## 3. 第二优先级文件

### 3.1 bq76940.c

路径：

[bq76940.c](<E:\07xianmu\BMS资料\项目代码\BMS-main\BMS_program _stm32F103c8t6\BMS_HAL\BSP\bq76940.c>)

先按下面顺序看：

1. `ALERT` 回调相关
2. 通信健康检查
3. MOS 控制函数
4. 保护检查函数
5. 初始化函数
6. 采样 / 滤波 / 更新函数
7. 均衡函数
8. 打印函数

你读完必须能回答：

- `ALERT` 是怎么唤醒任务的
- 通信失败后怎么恢复
- 过压 / 欠压 / 过流 / 过温怎么处理
- 为什么读电压前关均衡

### 3.2 bms_soc.h

路径：

[bms_soc.h](<E:\07xianmu\BMS资料\项目代码\BMS-main\BMS_program _stm32F103c8t6\BMS_HAL\BSP\bms_soc.h>)

看什么：

- SOC 核心配置
- SOH 相关结构
- Flash 保存配置
- 对外接口

---

## 4. 第三优先级文件

### 4.1 bms_soc.c

路径：

[bms_soc.c](<E:\07xianmu\BMS资料\项目代码\BMS-main\BMS_program _stm32F103c8t6\BMS_HAL\BSP\bms_soc.c>)

按下面顺序看：

1. OCV 表
2. Flash 读写
3. `SOC_Init`
4. `SOC_Update`
5. 容量学习
6. SOH 计算
7. Flash 保存条件

你读完必须能回答：

- 库仑积分怎么做
- OCV 修正什么时候触发
- 满充 / 空电校准条件是什么
- SOH 怎么得到
- 为什么要写 Flash

### 4.2 usart.c

路径：

[usart.c](<E:\07xianmu\BMS资料\项目代码\BMS-main\BMS_program _stm32F103c8t6\BMS_HAL\Core\Src\usart.c>)

按下面顺序看：

1. 接收缓冲
2. 中断接收回调
3. DMA 发送
4. `UART_CmdProcess`
5. `BMS_ParseCmd`

你读完必须能回答：

- 串口命令怎么拼成一行
- `status` 做了什么
- `bal/chg/dsg/mos` 命令分别影响什么

---

## 5. 第四优先级文件

### 5.1 sw_i2c.c

路径：

[sw_i2c.c](<E:\07xianmu\BMS资料\项目代码\BMS-main\BMS_program _stm32F103c8t6\BMS_HAL\BSP\sw_i2c.c>)

看什么：

- 起始信号
- 停止信号
- ACK / NACK
- 发送与接收

它的目标不是让你背时序，而是让你知道：

- 当前工程用的是软件 I2C
- 为什么不能在中断里跑复杂 I2C

---

## 6. 每个文件的阅读动作

每看一个文件，只做这 5 件事：

1. 找它的核心职责
2. 找它的输入
3. 找它的输出
4. 找它的异常处理
5. 找它和其它文件的调用关系

---

## 7. 你最容易走偏的地方

- 看到函数很多就开始逐行翻译
- 抓不到主链路
- 不区分“驱动代码”和“业务逻辑”
- 忽略宏定义和结构体

---

## 8. 正确的学习节奏

一天只啃一个主模块就够了：

### 第一天

- `main.c`
- `bq76940.h`

### 第二天

- `bq76940.c` 的 ALERT、保护、初始化

### 第三天

- `bq76940.c` 的采样、均衡、打印

### 第四天

- `bms_soc.h`
- `bms_soc.c`

### 第五天

- `usart.c`
- `sw_i2c.c`

---

## 9. 配套文档

配合下面这些一起看效果最好：

- [BMS项目代码理解版.md](<E:\07xianmu\BMS资料\BMS项目代码理解版.md>)
- [BMS代码面试问答版.md](<E:\07xianmu\BMS资料\BMS代码面试问答版.md>)
- [BMS项目主线流程图版.md](<E:\07xianmu\BMS资料\BMS项目主线流程图版.md>)
- [BMS方案与代码差异对照表.md](<E:\07xianmu\BMS资料\BMS方案与代码差异对照表.md>)

