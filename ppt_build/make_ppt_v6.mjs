import fs from "node:fs/promises";
import { Presentation, PresentationFile } from "@oai/artifact-tool";

const FONT = "Microsoft YaHei";
const BG = "#F5F7FA";
const PRIMARY = "#17395C";
const ACCENT = "#2C6B96";
const SOFT = "#DCE8F0";
const BORDER = "#B9C8D2";
const TEXT = "#1F2A36";
const MUTED = "#5D6B75";
const WARN = "#8F4242";

const proto = {
  id: "",
  slides: [],
  layouts: [],
  theme: {
    fontScheme: {
      name: FONT,
      majorFont: { latinTypeface: FONT, eastAsianTypeface: FONT, complexScriptTypeface: FONT, supplementalFonts: [] },
      minorFont: { latinTypeface: FONT, eastAsianTypeface: FONT, complexScriptTypeface: FONT, supplementalFonts: [] },
    },
  },
  charts: [], images: [], contentReferences: [], textStyles: [], fonts: [], people: [], threads: [],
};

const presentation = Presentation.load(proto, { slideSize: { width: 1280, height: 720 } });

presentation.theme.colorScheme = {
  name: "Cool Blue Gray Academic",
  themeColors: {
    accent1: PRIMARY, accent2: ACCENT, accent3: "#C4A55D", accent4: WARN,
    accent5: "#2F6E52", accent6: "#70849A", bg1: "#FFFFFF", bg2: BG,
    tx1: TEXT, tx2: MUTED, dk1: "#121C27", dk2: "#1B2B3A", lt1: "#FFFFFF", lt2: SOFT,
    hlink: PRIMARY, folHlink: ACCENT,
  },
};

const buildDir = "D:/xianmu/BMS-main/ppt_build";
const outputDir = "D:/xianmu/BMS-main/ppt_output";
await fs.mkdir(buildDir, { recursive: true });
await fs.mkdir(outputDir, { recursive: true });

function text(slide, value, position, style = {}) {
  const shape = slide.shapes.add({
    geometry: "textbox", position, fill: "none",
    line: { style: "solid", fill: "none", width: 0 },
  });
  shape.text = value;
  shape.text.style = {
    typeface: FONT, fontSize: 14, color: TEXT, alignment: "left", verticalAlignment: "top",
    lineSpacing: 1.5, wrap: "square", autoFit: "none", ...style,
  };
  return shape;
}

function box(slide, value, position, style = {}, fill = "#FFFFFF") {
  const shape = slide.shapes.add({
    geometry: "rect", position, fill,
    line: { style: "solid", fill: BORDER, width: 1 }, borderRadius: 3, shadow: "shadow-none",
  });
  shape.text = value;
  shape.text.style = {
    typeface: FONT, fontSize: 13, color: TEXT, alignment: "left", verticalAlignment: "middle",
    lineSpacing: 1.5, wrap: "square", autoFit: "none",
    insets: { top: 6, right: 10, bottom: 6, left: 10 }, ...style,
  };
  return shape;
}

function line(slide, from, to, options = {}) {
  return slide.shapes.connect(from, to, {
    kind: "straight", fromSide: "right", toSide: "left",
    line: { style: "solid", fill: ACCENT, width: 1.5 },
    head: { type: "triangle", width: "med", length: "med" }, ...options,
  });
}

function header(slide, number, title, subtitle) {
  slide.background.fill = BG;
  box(slide, number, { left: 56, top: 32, width: 54, height: 54 }, {
    fontSize: 20, bold: true, color: "#FFFFFF", fill: PRIMARY, alignment: "center",
  });
  text(slide, title, { left: 126, top: 38, width: 650, height: 36 }, {
    fontSize: 27, bold: true, color: PRIMARY,
  });
  text(slide, subtitle, { left: 126, top: 74, width: 850, height: 26 }, {
    fontSize: 13, color: MUTED,
  });
  box(slide, "", { left: 56, top: 118, width: 1168, height: 58 }, {
    fill: "#FFFFFF", line: { style: "solid", fill: SOFT, width: 1 },
  });
  text(slide, "基于STM32的多串锂电池管理系统设计与实现", { left: 76, top: 134, width: 700, height: 30 }, {
    fontSize: 16, bold: true, color: PRIMARY,
  });
  text(slide, "开题答辩", { left: 1050, top: 134, width: 150, height: 30 }, {
    fontSize: 14, color: ACCENT, alignment: "right", bold: true,
  });
}

function footer(slide, n) {
  text(slide, `${String(n).padStart(2, "0")} / 09`, { left: 1130, top: 665, width: 90, height: 22 }, {
    fontSize: 10, color: MUTED, alignment: "right",
  });
  text(slide, "资料来源：开题报告与项目资料", { left: 56, top: 665, width: 600, height: 22 }, {
    fontSize: 10, color: MUTED,
  });
}

function note(slide, value) {
  slide.speakerNotes.textFrame.setText(value);
  slide.speakerNotes.setVisible(false);
}

const coverImage = await fs.readFile("D:/xianmu/BMS-main/ppt_build/cover_background_v2.png");
const balancingImage = await fs.readFile("D:/xianmu/BMS-main/ppt_build/balancing_reference_slide.png");
const batteryImage = await fs.readFile("D:/xianmu/BMS-main/ppt_build/bq769x0_reference_slide.png");

// 1 封面
{
  const slide = presentation.slides.add();
  slide.background.fill = "#FFFFFF";
  slide.images.add({
    blob: coverImage, contentType: "image/png", alt: "锂电池管理系统背景",
    fit: "cover", position: { left: 0, top: 0, width: 1280, height: 300 },
  });
  text(slide, "基于STM32的多串锂电池管理系统设计与实现", {
    left: 64, top: 350, width: 1000, height: 82,
  }, { fontSize: 34, bold: true, color: PRIMARY, lineSpacing: 1.5 });
  text(slide, "本科毕业设计（论文）开题答辩", {
    left: 64, top: 442, width: 420, height: 32,
  }, { fontSize: 18, color: ACCENT, bold: true });
  const fields = [["汇报人", 64], ["学号", 345], ["专业", 626], ["指导教师", 907]];
  fields.forEach(([label, left]) => {
    text(slide, label, { left, top: 520, width: 90, height: 24 }, { fontSize: 13, color: MUTED });
    box(slide, "", { left, top: 550, width: 225, height: 30 }, { fill: "#FFFFFF" });
  });
  text(slide, "背景图：Image2生成，仅用于答辩视觉背景", {
    left: 64, top: 665, width: 420, height: 20,
  }, { fontSize: 10, color: MUTED });
  note(slide, "各位老师好。我汇报的题目是基于STM32的多串锂电池管理系统设计与实现。课题围绕9串18650锂电池组，完成状态采集、安全保护、SOC估算和上位机交互的完整设计。");
}

// 2 背景意义
{
  const slide = presentation.slides.add();
  header(slide, "01", "研究背景与选题依据", "从多串电池组的运行风险到实时管理的必要性");
  text(slide, "多串锂电池组存在单体差异，若缺少实时监测和保护，容易出现过充、过放、过流、过温等问题，影响使用寿命与使用安全。", {
    left: 76, top: 192, width: 1100, height: 32, fontSize: 15, color: PRIMARY, bold: true,
  });
  const leftItems = [
    ["电池安全", "高倍率放电会显著增加温升，徐伟巍等[1]强调温度保护与过流保护必须综合考虑。"],
    ["状态可见", "黄新星[2]提出远程监测可弥补本地计算和存储能力不足，说明数据交互是BMS的重要方向。"],
    ["工程基础", "黄青青等[3]以STM32F103为核心实现大功率电池组管理，验证了本类方案的工程可行性。"],
  ];
  leftItems.forEach(([name, detail], index) => {
    const top = 250 + index * 100;
    box(slide, name, { left: 76, top, width: 120, height: 82 }, {
      fontSize: 15, bold: true, color: PRIMARY, fill: SOFT, alignment: "center",
    });
    box(slide, detail, { left: 210, top, width: 490, height: 82 }, { fontSize: 13, lineSpacing: 1.5 });
  });
  slide.images.add({
    blob: balancingImage, contentType: "image/png", alt: "被动均衡驱动电路局部参考",
    fit: "cover", position: { left: 730, top: 212, width: 480, height: 270 },
  });
  box(slide, "被动均衡驱动电路局部参考　来源：项目参考资料", {
    left: 730, top: 496, width: 480, height: 46,
  }, { fontSize: 10, color: MUTED, alignment: "center" });
  const chain = [
    box(slide, "多串成组", { left: 76, top: 570, width: 170, height: 44 }, { alignment: "center", bold: true, fill: SOFT }),
    box(slide, "单体差异", { left: 270, top: 570, width: 170, height: 44 }, { alignment: "center" }),
    box(slide, "安全风险", { left: 464, top: 570, width: 170, height: 44 }, { alignment: "center" }),
    box(slide, "实时管理", { left: 658, top: 570, width: 170, height: 44 }, { alignment: "center", bold: true, color: PRIMARY, fill: SOFT }),
    box(slide, "课题实现", { left: 852, top: 570, width: 170, height: 44 }, { alignment: "center", bold: true, color: "#FFFFFF", fill: PRIMARY }),
  ];
  for (let i = 1; i < chain.length; i += 1) line(slide, chain[i - 1], chain[i]);
  footer(slide, 2);
  note(slide, "背景部分说明为什么需要BMS。多串成组后单体差异会放大风险，本课题用实时采集、阈值判断和MOS控制把这些风险纳入可控范围。");
}

// 3 研究目的意义
{
  const slide = presentation.slides.add();
  header(slide, "02", "研究目的与研究意义", "以安全控制和工程验证为中心，不做功能堆叠");
  const goal = box(slide, "研究目的", { left: 76, top: 190, width: 170, height: 48 }, {
    fontSize: 17, bold: true, color: "#FFFFFF", fill: PRIMARY, alignment: "center",
  });
  const goals = [
    ["状态感知", "采集单体电压、电流和温度，形成可用的运行数据基础。"],
    ["安全控制", "结合BQ76940硬件告警与STM32软件阈值，控制充放电MOS。"],
    ["状态估算", "采用安时积分并配合静置OCV校正，提高SOC可用性。"],
    ["调试交互", "通过UART连接Python上位机，支持状态显示和命令调试。"],
  ];
  goals.forEach(([name, detail], index) => {
    const top = 258 + index * 76;
    box(slide, name, { left: 76, top, width: 130, height: 62 }, {
      fontSize: 14, bold: true, color: PRIMARY, fill: SOFT, alignment: "center",
    });
    box(slide, detail, { left: 220, top, width: 440, height: 62 }, { fontSize: 13, lineSpacing: 1.5 });
  });
  const meaning = box(slide, "研究意义", { left: 710, top: 190, width: 170, height: 48 }, {
    fontSize: 17, bold: true, color: "#FFFFFF", fill: ACCENT, alignment: "center",
  });
  const meanings = [
    ["工程价值", "降低过充、过放、过流、过温风险，延长电池组寿命。"],
    ["学习价值", "覆盖嵌入式软硬件、I2C通信、FreeRTOS调度与保护策略。"],
    ["应用价值", "为同类小规模多串锂电池管理系统提供可复用设计。"],
  ];
  meanings.forEach(([name, detail], index) => {
    const top = 258 + index * 96;
    box(slide, name, { left: 710, top, width: 130, height: 82 }, {
      fontSize: 14, bold: true, color: PRIMARY, fill: SOFT, alignment: "center",
    });
    box(slide, detail, { left: 854, top, width: 350, height: 82 }, { fontSize: 13, lineSpacing: 1.5 });
  });
  box(slide, "边界说明：CAN、蓝牙、串口屏只作为扩展预留，不计入当前已实现功能范围。", {
    left: 76, top: 550, width: 770, height: 48,
  }, { fontSize: 13, color: WARN, fill: "#F5EDED", lineSpacing: 1.5 });
  box(slide, "目标概括：形成结构清晰、安全边界明确、可在实验平台验证的完整系统。", {
    left: 860, top: 550, width: 344, height: 48,
  }, { fontSize: 13, bold: true, color: PRIMARY, fill: SOFT, lineSpacing: 1.5 });
  footer(slide, 3);
  note(slide, "目的部分分四项说明。意义部分强调工程价值、学习价值和应用价值，同时明确CAN、蓝牙和串口屏只是扩展方向，避免夸大当前实现范围。");
}

// 4 研究现状与内容
{
  const slide = presentation.slides.add();
  header(slide, "03", "国内外研究现状与研究内容", "从系统实现、状态估算、均衡保护三个方向归纳");
  const table = slide.tables.add({
    rows: 4, columns: 3, left: 76, top: 142, width: 680, height: 328,
    values: [
      ["研究方向", "国内研究现状", "国外研究现状"],
      ["系统实现", "STM32与多串采集保护方案", "STM32、LTC6804与物联网交互"],
      ["状态估算", "安时积分结合OCV与滤波修正", "EKF、LSTM与优化算法融合"],
      ["均衡保护", "主动均衡、被动均衡与温度保护", "拓扑优化与多级安全保护"],
    ],
  });
  table.rows[0].height = 52;
  for (let r = 1; r < 4; r += 1) table.rows[r].height = 92;
  table.borders.assign({ style: "solid", fill: BORDER, width: 1 });
  table.styleOptions = { headerRow: true, bandedRows: true };
  for (let r = 0; r < 4; r += 1) {
    for (let c = 0; c < 3; c += 1) {
      table.getCell(r, c).text.style = {
        typeface: FONT, fontSize: r === 0 ? 14 : 13,
        color: r === 0 ? "#FFFFFF" : TEXT,
        alignment: c === 0 ? "center" : "left",
        verticalAlignment: "middle", lineSpacing: 1.5,
        fill: r === 0 ? PRIMARY : (r % 2 === 0 ? "#F2F6F8" : "#FFFFFF"),
        insets: { top: 6, right: 8, bottom: 6, left: 8 }, bold: r === 0,
      };
    }
  }
  const content = [
    ["系统总体架构", "主控、模拟前端、电池组、保护电路与上位机的连接关系。"],
    ["采集与保护", "单体电压、电流、温度采集，硬件告警与软件阈值判断。"],
    ["SOC估算", "安时积分、静置约30s后的OCV校正和边界校准。"],
    ["通信与交互", "UART命令解析、状态上报和Python上位机显示。"],
  ];
  content.forEach(([name, detail], index) => {
    const top = 142 + index * 82;
    box(slide, name, { left: 780, top, width: 170, height: 62 }, {
      fontSize: 14, bold: true, color: PRIMARY, fill: SOFT, alignment: "center",
    });
    box(slide, detail, { left: 964, top, width: 240, height: 62 }, { fontSize: 12, lineSpacing: 1.5 });
  });
  box(slide, "研究切入点：在STM32F103C8T6的有限资源下，解决多串采集稳定性、保护响应速度和SOC可验证性之间的平衡问题。", {
    left: 76, top: 500, width: 1128, height: 64,
  }, { fontSize: 15, bold: true, color: PRIMARY, fill: "#FFFFFF", lineSpacing: 1.6 });
  footer(slide, 4);
  note(slide, "文献综述不逐篇读，按三个方向归纳。国内研究强调工程实现，国外研究在估算算法和均衡拓扑上更活跃，本课题关注嵌入式实现的落点。");
}

// 5 总体方案
{
  const slide = presentation.slides.add();
  header(slide, "04", "系统总体方案与硬件选型", "四层结构、四项选型，扩展能力与当前边界分开说明");
  const system = [
    ["电池层", "9串18650锂电池组，提供被管理对象"],
    ["采集层", "BQ76940采集电压、电流、温度并给出硬件告警"],
    ["控制层", "STM32读取数据，完成阈值判断、SOC估算与命令交互"],
    ["交互层", "UART连接Python上位机，完成状态显示与调试命令"],
  ];
  system.forEach(([name, detail], index) => {
    const top = 142 + index * 78;
    box(slide, name, { left: 76, top, width: 110, height: 58 }, {
      fontSize: 14, bold: true, color: index === 2 ? "#FFFFFF" : PRIMARY,
      fill: index === 2 ? PRIMARY : SOFT, alignment: "center",
    });
    box(slide, detail, { left: 200, top, width: 430, height: 58 }, { fontSize: 13, lineSpacing: 1.5 });
  });
  const choices = [
    ["主控方案", "STM32F103C8T6", "外设资源较完整，实时性好，资料和成本适合课题验证。"],
    ["模拟前端", "BQ76940", "面向多串电池，支持采集、硬件告警和MOS控制。"],
    ["均衡方式", "被动均衡接口", "结构简单、易于验证；主动均衡效率高但结构复杂。"],
    ["通信方式", "UART", "当前连接Python上位机；CAN等通信作为扩展方向。"],
  ];
  choices.forEach(([name, selected, reason], index) => {
    const top = 142 + index * 78;
    box(slide, name, { left: 670, top, width: 100, height: 58 }, {
      fontSize: 13, bold: true, color: PRIMARY, fill: SOFT, alignment: "center",
    });
    box(slide, selected, { left: 784, top, width: 145, height: 58 }, {
      fontSize: 12, bold: true, color: PRIMARY, alignment: "center",
    });
    box(slide, reason, { left: 941, top, width: 263, height: 58 }, { fontSize: 10, lineSpacing: 1.5 });
  });
  slide.images.add({
    blob: batteryImage, contentType: "image/png", alt: "BQ769X0外围连接局部参考",
    fit: "cover", position: { left: 670, top: 468, width: 534, height: 132 },
  });
  text(slide, "BQ769X0外围连接局部参考　来源：BQ769X0参考设计资料", {
    left: 670, top: 610, width: 534, height: 34,
  }, { fontSize: 10, color: MUTED, alignment: "center" });
  box(slide, "安全控制链路：BQ76940采集与硬件告警，STM32完成数据读取和保护判断，充放电MOS执行动作。", {
    left: 76, top: 480, width: 560, height: 80,
  }, { fontSize: 14, bold: true, color: PRIMARY, fill: "#FFFFFF", lineSpacing: 1.6 });
  footer(slide, 5);
  note(slide, "总体方案按数据流说明。BQ76940是采集和硬件告警的前端，STM32是读取、判断和交互的控制核心。CAN、蓝牙和串口屏不写成当前功能。");
}

// 6 软件设计
{
  const slide = presentation.slides.add();
  header(slide, "05", "软件设计与任务流程", "FreeRTOS任务调度、共享资源保护与数据闭环");
  const tasks = [
    box(slide, "BMS_Task\n高优先级\n500ms周期，可由ALERT唤醒", { left: 76, top: 142, width: 310, height: 82 }, {
      fontSize: 13, bold: true, color: "#FFFFFF", fill: PRIMARY, alignment: "center",
    }),
    box(slide, "ALERT中断\n只发送任务通知，不执行I2C", { left: 420, top: 142, width: 260, height: 82 }, {
      fontSize: 12, color: WARN, fill: "#F5EDED", alignment: "center",
    }),
    box(slide, "UART_Task\n低优先级\n50ms轮询串口命令", { left: 714, top: 142, width: 240, height: 82 }, {
      fontSize: 12, alignment: "center",
    }),
    box(slide, "I2C互斥锁\n保护BQ76940访问", { left: 984, top: 142, width: 220, height: 82 }, {
      fontSize: 12, color: PRIMARY, fill: SOFT, alignment: "center",
    }),
  ];
  line(slide, tasks[1], tasks[0], { fromSide: "left", toSide: "right" });
  line(slide, tasks[2], tasks[3], { fromSide: "right", toSide: "left" });
  const steps = [
    ["采样", "电压、电流、温度"], ["判断", "硬件告警与软件阈值"],
    ["执行", "SOC更新与MOS控制"], ["上报", "UART状态输出"],
    ["显示", "上位机刷新"],
  ];
  const stepShapes = steps.map(([name, detail], index) => box(slide, `${name}\n${detail}`, {
    left: 76 + index * 230, top: 270, width: 205, height: 72,
  }, { fontSize: 12, alignment: "center" }));
  for (let i = 1; i < stepShapes.length; i += 1) line(slide, stepShapes[i - 1], stepShapes[i]);
  const details = [
    ["中断边界", "ALERT中断服务程序只发送任务通知，寄存器读取、故障解析和状态更新在任务上下文完成。"],
    ["资源保护", "采样流程与串口命令都可能访问BQ76940，通过I2C互斥锁避免并发冲突。"],
    ["SOC策略", "安时积分作为主方法，静置约30s后结合OCV校正，并用满电、空电边界校准。"],
    ["上位机", "Python tkinter界面显示关键状态，接收命令后由STM32解析并执行。"],
  ];
  details.forEach(([name, detail], index) => {
    const top = 374 + index * 72;
    box(slide, name, { left: 76, top, width: 120, height: 54 }, {
      fontSize: 13, bold: true, color: PRIMARY, fill: SOFT, alignment: "center",
    });
    box(slide, detail, { left: 210, top, width: 994, height: 54 }, { fontSize: 12, lineSpacing: 1.5 });
  });
  footer(slide, 6);
  note(slide, "软件部分说明任务结构和执行流程。高优先级BMS_Task保证采样和保护，低优先级UART_Task处理命令。互斥锁和中断边界是稳定性的关键。");
}

// 7 技术路线
{
  const slide = presentation.slides.add();
  header(slide, "06", "研究方法与技术路线", "从需求分析到联调验证，测试结果反哺参数修正");
  const route = [
    ["需求分析", "确定9串电池组、保护阈值与交互范围"],
    ["系统设计", "完成总体架构、任务划分与硬件接口设计"],
    ["功能实现", "完成驱动、SOC估算、通信与上位机程序"],
    ["联调测试", "验证采集、保护、SOC与交互功能"],
    ["总结完善", "整理测试数据并撰写毕业设计论文"],
  ];
  const routeShapes = route.map(([name, detail], index) => box(slide, `${name}\n${detail}`, {
    left: 76 + index * 230, top: 142, width: 205, height: 78,
  }, { fontSize: 12, alignment: "center" }));
  for (let i = 1; i < routeShapes.length; i += 1) line(slide, routeShapes[i - 1], routeShapes[i]);
  const methods = [
    ["文献研究法", "查阅BMS、SOC估算、均衡策略与STM32实现文献，确定技术路线。"],
    ["模块设计法", "按主控、模拟前端、保护电路、通信和上位机划分设计单元。"],
    ["实验验证法", "从寄存器读取、告警响应到保护动作逐级验证，记录测试数据。"],
  ];
  methods.forEach(([name, detail], index) => {
    const top = 252 + index * 90;
    box(slide, name, { left: 76, top, width: 130, height: 66 }, {
      fontSize: 14, bold: true, color: PRIMARY, fill: SOFT, alignment: "center",
    });
    box(slide, detail, { left: 220, top, width: 440, height: 66 }, { fontSize: 12, lineSpacing: 1.5 });
  });
  const schedule = [
    ["2026.06.25-07.02", "选题"], ["2026.07.03-07.16", "文献调研"],
    ["2026.07.17-09.22", "开题报告"], ["2026.09.24-10.19", "系统设计"],
    ["2026.10.20-11.01", "硬件加工"], ["2026.11.02-11.30", "论文撰写"],
  ];
  schedule.forEach(([time, task], index) => {
    const top = 250 + index * 58;
    box(slide, time, { left: 710, top, width: 195, height: 40 }, {
      fontSize: 11, color: PRIMARY, fill: SOFT, alignment: "center",
    });
    box(slide, task, { left: 919, top, width: 285, height: 40 }, { fontSize: 13, alignment: "center" });
  });
  box(slide, "技术路线闭环：测试阶段发现的采样偏差、响应延迟或估算误差，会回溯到驱动、参数与软件逻辑进行修正。", {
    left: 76, top: 550, width: 1128, height: 64,
  }, { fontSize: 15, bold: true, color: PRIMARY, fill: "#FFFFFF", lineSpacing: 1.6 });
  footer(slide, 7);
  note(slide, "方法和技术路线合并说明。重点不是流程名词，而是每一阶段留下可检查的输出，包括接口定义、驱动代码、测试数据和论文材料。");
}

// 8 测试方案
{
  const slide = presentation.slides.add();
  header(slide, "07", "测试与验证方案", "以拟验证方式表述，不虚构未取得的误差数值");
  const tests = [
    ["通信与采集", "验证BQ76940初始化、寄存器读取、数据刷新与ALERT响应。"],
    ["保护与执行", "验证过充、过放、过流、过温阈值触发后充放电MOS的动作。"],
    ["SOC与校正", "验证安时积分、静置OCV校正和满电、空电边界校准效果。"],
    ["交互与显示", "验证UART命令解析、状态上报和Python上位机显示刷新。"],
    ["运行稳定性", "验证连续运行、异常恢复和均衡接口工作状态。"],
  ];
  tests.forEach(([name, detail], index) => {
    const top = 142 + index * 80;
    box(slide, name, { left: 76, top, width: 150, height: 60 }, {
      fontSize: 14, bold: true, color: PRIMARY, fill: SOFT, alignment: "center",
    });
    box(slide, detail, { left: 240, top, width: 500, height: 60 }, { fontSize: 12, lineSpacing: 1.5 });
  });
  const expected = box(slide, "预期成果", { left: 780, top: 142, width: 150, height: 60 }, {
    fontSize: 16, bold: true, color: "#FFFFFF", fill: PRIMARY, alignment: "center",
  });
  const outcomes = [
    "完成多串锂电池管理系统软硬件设计。",
    "实现状态采集、保护判断、SOC估算与上位机交互。",
    "形成测试记录、测试数据和毕业设计论文。",
  ];
  outcomes.forEach((value, index) => {
    box(slide, value, { left: 950, top: 142 + index * 90, width: 254, height: 70 }, {
      fontSize: 12, lineSpacing: 1.5,
    });
  });
  box(slide, "验证重点：采样稳定性、保护链路及时性、SOC校正可用性和上位机交互连续性。", {
    left: 780, top: 410, width: 424, height: 62,
  }, { fontSize: 13, bold: true, color: PRIMARY, fill: "#FFFFFF", lineSpacing: 1.6 });
  box(slide, "测试表述采用拟验证或计划验证，硬件仍处于联调阶段，不宣称全部功能已经完成验证。", {
    left: 76, top: 560, width: 1128, height: 54,
  }, { fontSize: 13, color: MUTED, fill: "#F2F6F8", lineSpacing: 1.5 });
  footer(slide, 8);
  note(slide, "测试方案强调可检查性。对没有明确数值指标的项，不编具体误差要求，只说明验证对象和验证方法。");
}

// 9 结束页
{
  const slide = presentation.slides.add();
  slide.background.fill = PRIMARY;
  box(slide, "汇报结束", { left: 320, top: 230, width: 640, height: 88 }, {
    fontSize: 36, bold: true, color: "#FFFFFF", fill: PRIMARY, alignment: "center", line: { style: "solid", fill: "none", width: 0 },
  });
  box(slide, "基于STM32的多串锂电池管理系统设计与实现", { left: 260, top: 350, width: 760, height: 46 }, {
    fontSize: 18, color: SOFT, fill: PRIMARY, alignment: "center", line: { style: "solid", fill: "none", width: 0 },
  });
  box(slide, "请各位老师批评指正", { left: 380, top: 430, width: 520, height: 42 }, {
    fontSize: 16, color: "#FFFFFF", fill: PRIMARY, alignment: "center", line: { style: "solid", fill: "none", width: 0 },
  });
  note(slide, "感谢老师聆听，请老师对方案和验证安排提出指导意见。");
}

const file = await PresentationFile.exportPptx(presentation);
await file.save(`${buildDir}/candidate_ppt_v6.pptx`);
console.log(`${buildDir}/candidate_ppt_v6.pptx`);
