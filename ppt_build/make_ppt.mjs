import fs from "node:fs/promises";
import path from "node:path";
import { Presentation, PresentationFile } from "@oai/artifact-tool";

const FONT = "Microsoft YaHei";
const BG = "#F5F7FA";
const TITLE = "#1F2937";
const BODY = "#374151";
const ACCENT = "#1D4ED8";
const ACCENT_SOFT = "#DBEAFE";
const ACCENT_TEAL = "#0F766E";
const BORDER = "#CBD5E1";
const MUTED = "#64748B";
const CARD = "#FFFFFF";
const LINE = { style: "solid", fill: BORDER, width: 1 };

const proto = {
  id: "",
  slides: [],
  layouts: [],
  theme: {
    fontScheme: {
      name: FONT,
      majorFont: {
        latinTypeface: FONT,
        eastAsianTypeface: FONT,
        complexScriptTypeface: FONT,
        supplementalFonts: [],
     },
    minorFont: {
      latinTypeface: FONT,
      eastAsianTypeface: FONT,
      complexScriptTypeface: FONT,
      supplementalFonts: [],
    },
    },
  },
  charts: [],
  images: [],
  contentReferences: [],
  textStyles: [],
  fonts: [],
  people: [],
  threads: [],
};

const presentation = Presentation.load(proto, {
  slideSize: { width: 1280, height: 720 },
});

presentation.theme.colorScheme = {
  name: "Cool Blue Gray",
  themeColors: {
    accent1: ACCENT,
    accent2: ACCENT_TEAL,
    accent3: "#F59E0B",
    accent4: "#DC2626",
    accent5: "#7C3AED",
    accent6: "#16A34A",
    bg1: "#FFFFFF",
    bg2: BG,
    tx1: TITLE,
    tx2: BODY,
    dk1: "#000000",
    dk2: "#1E293B",
    lt1: "#FFFFFF",
    lt2: "#E2E8F0",
    hlink: ACCENT,
    folHlink: "#7C3AED",
  },
};

function addText(slide, text, position, style = {}) {
  const shape = slide.shapes.add({
    geometry: "textbox",
    position,
    fill: "none",
    line: { style: "solid", fill: "none", width: 0 },
  });
  shape.text = text;
  shape.text.style = {
    typeface: FONT,
    fontSize: 14,
    color: BODY,
    alignment: "left",
    verticalAlignment: "top",
    lineSpacing: 1.5,
    wrap: "square",
    autoFit: "none",
    ...style,
  };
  return shape;
}

function addCard(slide, text, position, style = {}, fill = CARD) {
  const shape = slide.shapes.add({
    geometry: "rect",
    position,
    fill,
    line: LINE,
  });
  shape.text = text;
  shape.text.style = {
    typeface: FONT,
    fontSize: 14,
    color: BODY,
    alignment: "center",
    verticalAlignment: "middle",
    lineSpacing: 1.5,
    wrap: "square",
    autoFit: "none",
    insets: { top: 8, right: 10, bottom: 8, left: 10 },
    ...style,
  };
  return shape;
}

function addNode(slide, text, position, style = {}) {
  return addCard(slide, text, position, style, "#FFFFFF");
}

function addArrow(slide, from, to, options = {}) {
  return slide.shapes.connect(from, to, {
    kind: "straight",
    fromSide: "right",
    toSide: "left",
    line: { style: "solid", fill: MUTED, width: 1.5 },
    head: { type: "arrow", width: "med", length: "med" },
    ...options,
  });
}

function addTitle(slide, text) {
  const title = addText(slide, text, { left: 64, top: 42, width: 920, height: 62 }, {
    fontSize: 26,
    bold: true,
    color: TITLE,
    lineSpacing: 1.3,
  });
  slide.shapes.add({
    geometry: "rect",
    position: { left: 64, top: 104, width: 96, height: 4 },
    fill: ACCENT,
    line: { style: "solid", fill: ACCENT, width: 0 },
  });
  return title;
}

function addFooter(slide, text) {
  return addText(slide, text, { left: 64, top: 660, width: 1120, height: 32 }, {
    fontSize: 11,
    color: MUTED,
    lineSpacing: 1.3,
  });
}

function setNotes(slide, text) {
  slide.speakerNotes.textFrame.setText(text);
  slide.speakerNotes.setVisible(false);
}

const outputDir = path.resolve("D:/xianmu/BMS-main/ppt_output");
await fs.mkdir(outputDir, { recursive: true });

const coverBytes = await fs.readFile("D:/xianmu/BMS-main/ppt_build/cover_background.png");
const bqImageBytes = await fs.readFile(
  "D:/xianmu/BMS-main/开题报告补充图片/最终图/图2_BQ769X0内部结构框图.png",
);

// 1. 封面
{
  const slide = presentation.slides.add();
  slide.background.fill = BG;
  slide.images.add({
    blob: coverBytes,
    contentType: "image/png",
    alt: "锂电池管理系统背景图",
    fit: "cover",
    position: { left: 0, top: 0, width: 1280, height: 720 },
  });
  slide.shapes.add({
    geometry: "rect",
    position: { left: 0, top: 0, width: 620, height: 720 },
    fill: "#0F172A",
    line: { style: "solid", fill: "none", width: 0 },
  });
  slide.shapes.add({
    geometry: "rect",
    position: { left: 64, top: 166, width: 64, height: 4 },
    fill: ACCENT,
    line: { style: "solid", fill: ACCENT, width: 0 },
  });
  addText(slide, "基于STM32的多串锂电池管理系统设计与实现", {
    left: 64,
    top: 82,
    width: 470,
    height: 170,
  }, {
    fontSize: 30,
    bold: true,
    color: "#FFFFFF",
    lineSpacing: 1.3,
  });
  addText(slide, "开题答辩", {
    left: 64,
    top: 275,
    width: 300,
    height: 42,
  }, {
    fontSize: 21,
    color: "#DBEAFE",
    lineSpacing: 1.3,
  });
  addText(slide, "杭州电子科技大学信息工程学院", {
    left: 64,
    top: 650,
    width: 420,
    height: 34,
  }, {
    fontSize: 15,
    color: "#E2E8F0",
  });
  const infoItems = [
    "姓名：＿＿＿＿＿＿",
    "学号：＿＿＿＿＿＿",
    "专业：＿＿＿＿＿＿",
    "指导教师：＿＿＿＿＿＿",
  ];
  infoItems.forEach((item, index) => {
    const left = 64 + (index % 2) * 252;
    const top = 385 + Math.floor(index / 2) * 48;
    addText(slide, item, { left, top, width: 232, height: 38 }, {
      fontSize: 15,
      color: "#FFFFFF",
    });
  });
  setNotes(slide, "封面背景由本项目资料整理生成。答辩时先介绍课题名称、专业与指导教师。");
}

// 2. 研究背景与选题依据
{
  const slide = presentation.slides.add();
  slide.background.fill = BG;
  addTitle(slide, "研究背景与选题依据");
  addText(slide, "多串锂电池已广泛用于便携设备、储能装置和小型动力系统。电池组在充放电过程中会出现单体电压不一致、温度分布不均和过流风险，仅依靠人工检测难以满足实时管理要求。", {
    left: 64,
    top: 140,
    width: 560,
    height: 170,
  }, {
    fontSize: 16,
  });
  addText(slide, "电池管理系统可持续采集电压、电流和温度，根据状态变化执行保护动作，并在异常发生前给出提示。围绕多串锂电池设计一套结构清晰、功能可验证的BMS，具有明确的应用需求和研究价值。", {
    left: 64,
    top: 310,
    width: 560,
    height: 170,
  }, {
    fontSize: 16,
  });

  const nodes = [
    addNode(slide, "应用需求\n储能与小型动力系统", { left: 700, top: 150, width: 220, height: 90 }, { fontSize: 14 }),
    addNode(slide, "安全风险\n过充、过放、过流、过温", { left: 970, top: 150, width: 220, height: 90 }, { fontSize: 14 }),
    addNode(slide, "管理需求\n实时采集与保护控制", { left: 700, top: 290, width: 220, height: 90 }, { fontSize: 14 }),
    addNode(slide, "研究价值\n嵌入式软硬件闭环验证", { left: 970, top: 290, width: 220, height: 90 }, { fontSize: 14 }),
  ];
  addArrow(slide, nodes[0], nodes[1], { fromSide: "right", toSide: "left" });
  addArrow(slide, nodes[2], nodes[3], { fromSide: "right", toSide: "left" });
  addArrow(slide, nodes[1], nodes[3], { fromSide: "bottom", toSide: "top" });
  addFooter(slide, "资料来源：项目开题报告");
  setNotes(slide, "背景页说明选题来自多串锂电池的实际安全需求，不强调产品化，而强调验证型BMS主线。");
}

// 3. 研究目的与预期目标
{
  const slide = presentation.slides.add();
  slide.background.fill = BG;
  addTitle(slide, "研究目的与预期目标");
  addText(slide, "研究目的", { left: 64, top: 130, width: 200, height: 36 }, { fontSize: 20, bold: true, color: ACCENT });
  addText(slide, "设计一套基于STM32F103C8T6与BQ76940的多串锂电池管理系统，完成状态采集、安全保护、SOC估算、基础均衡接口和上位机调试，形成可运行的嵌入式软硬件方案。", {
    left: 64,
    top: 170,
    width: 520,
    height: 180,
  }, { fontSize: 16 });
  const goals = [
    "完成整体方案与软硬件功能划分",
    "实现电压、电流、温度周期采集",
    "建立软硬件结合的保护控制机制",
    "完成SOC估算与OCV校正",
    "实现UART命令与Python上位机调试",
  ];
  goals.forEach((text, index) => {
    addNode(slide, text, { left: 640, top: 136 + index * 82, width: 540, height: 66 }, {
      fontSize: 15,
      alignment: "left",
    });
  });
  addFooter(slide, "资料来源：项目开题报告");
  setNotes(slide, "重点说明本课题目标是验证型系统主线，避免把CAN、蓝牙等扩展功能说成已实现。");
}

// 4. 研究意义
{
  const slide = presentation.slides.add();
  slide.background.fill = BG;
  addTitle(slide, "研究意义");
  const cards = [
    addCard(slide, "工程意义\n减少过充、过放和过热风险，延长电池组使用寿命", { left: 64, top: 140, width: 360, height: 150 }, { fontSize: 15 }),
    addCard(slide, "学习意义\n覆盖嵌入式软硬件、通信、RTOS和电池保护策略", { left: 450, top: 140, width: 360, height: 150 }, { fontSize: 15 }),
    addCard(slide, "应用参考\n为同类多串锂电池管理系统设计提供可借鉴方案", { left: 836, top: 140, width: 360, height: 150 }, { fontSize: 15 }),
  ];
  addText(slide, "本课题采用STM32F103C8T6作为主控，配合BQ76940完成电池信息采集与保护控制。方案成本可控，开发流程完整，能够体现嵌入式实时系统在资源调度与共享资源保护中的应用。", {
    left: 64,
    top: 340,
    width: 1130,
    height: 110,
  }, { fontSize: 17 });
  addText(slide, "SOC估算结合安时积分与静置开路电压校正，用于降低长期计算的累积误差。相比单纯依赖芯片保护的传统方案，软件与硬件结合更利于分析电池运行规律，也为后续算法改进留下空间。", {
    left: 64,
    top: 460,
    width: 1130,
    height: 110,
  }, { fontSize: 17 });
  addArrow(slide, cards[0], cards[1], { fromSide: "right", toSide: "left" });
  addArrow(slide, cards[1], cards[2], { fromSide: "right", toSide: "left" });
  addFooter(slide, "资料来源：项目开题报告");
  setNotes(slide, "讲意义时将安全价值、学习价值和工程参考价值分层说明，避免空泛表述。");
}

// 5. 国内外研究现状
{
  const slide = presentation.slides.add();
  slide.background.fill = BG;
  addTitle(slide, "国内外研究现状");
  const domestic = [
    addCard(slide, "国内研究\n面向安全保护、SOC估算、均衡控制与具体应用场景", { left: 64, top: 140, width: 360, height: 130 }, { fontSize: 15 }),
    addCard(slide, "国外研究\n硬件架构、状态估算、主动均衡拓扑与物联网集成", { left: 836, top: 140, width: 360, height: 130 }, { fontSize: 15 }),
    addCard(slide, "共同趋势\n保护策略、估计算法与多任务系统设计不断融合", { left: 450, top: 320, width: 360, height: 120 }, { fontSize: 15 }),
  ];
  addArrow(slide, domestic[0], domestic[2], { fromSide: "bottom", toSide: "left" });
  addArrow(slide, domestic[1], domestic[2], { fromSide: "bottom", toSide: "right" });
  addText(slide, "国内文献关注具体应用中的温度保护、远程监控和SOC算法改进；国外文献在主动均衡拓扑、滤波与数据驱动融合、STM32与专用AFE集成方面有较多探索。", {
    left: 64,
    top: 500,
    width: 1130,
    height: 90,
  }, { fontSize: 16 });
  addText(slide, "已有研究为系统架构和算法选择提供依据，但面向STM32平台的工程实现仍需处理采样时序、共享资源调度和保护响应等约束。", {
    left: 64,
    top: 590,
    width: 1130,
    height: 70,
  }, { fontSize: 16 });
  addFooter(slide, "资料来源：开题报告参考文献[1]至[12]");
  setNotes(slide, "现状页只概括趋势，具体文献结论已在开题报告中展开。");
}

// 6. 系统总体方案与数据流
{
  const slide = presentation.slides.add();
  slide.background.fill = BG;
  addTitle(slide, "系统总体方案与数据流");
  const row1 = [
    addNode(slide, "9串18650锂电池组", { left: 64, top: 160, width: 180, height: 80 }, { fontSize: 14 }),
    addNode(slide, "BQ76940\n采集与硬件告警", { left: 290, top: 160, width: 180, height: 80 }, { fontSize: 14 }),
    addNode(slide, "STM32F103C8T6\n数据处理与策略", { left: 516, top: 160, width: 190, height: 80 }, { fontSize: 14 }),
    addNode(slide, "UART通信", { left: 742, top: 160, width: 150, height: 80 }, { fontSize: 14 }),
    addNode(slide, "Python\ntkinter上位机", { left: 930, top: 160, width: 190, height: 80 }, { fontSize: 14 }),
  ];
  addArrow(slide, row1[0], row1[1]);
  addArrow(slide, row1[1], row1[2]);
  addArrow(slide, row1[2], row1[3]);
  addArrow(slide, row1[3], row1[4]);

  const control = addNode(slide, "安全控制链路\n保护优先于显示与命令交互", { left: 440, top: 350, width: 380, height: 90 }, {
    fontSize: 15,
    fill: ACCENT_SOFT,
    color: "#1E3A8A",
  });
  addArrow(slide, row1[2], control, { fromSide: "bottom", toSide: "top" });
  addArrow(slide, control, row1[1], { fromSide: "top", toSide: "bottom" });

  addText(slide, "数据流：电压、电流、温度由BQ76940采集，STM32通过软件I2C读取后完成阈值判断、SOC更新和状态整理，再经UART上传至上位机显示。", {
    left: 64,
    top: 500,
    width: 560,
    height: 120,
  }, { fontSize: 15 });
  addText(slide, "控制流：BQ76940提供硬件告警，ALERT通知STM32，软件结合阈值判断风险等级，并通过充放电MOS执行保护。CAN、蓝牙、串口屏仅作为扩展预留。", {
    left: 670,
    top: 500,
    width: 540,
    height: 120,
  }, { fontSize: 15 });
  addFooter(slide, "资料来源：项目开题报告研究方案与总体设计");
  setNotes(slide, "这页是全局主链路，重点区分数据流和安全控制链路。");
}

// 7. 硬件选型与模块设计
{
  const slide = presentation.slides.add();
  slide.background.fill = BG;
  addTitle(slide, "硬件选型与模块设计");
  const selections = [
    ["主控", "STM32F103C8T6", "外设资源、实时性、资料成熟度和成本均衡"],
    ["模拟前端", "BQ76940", "支持多串采集、硬件告警、MOS控制和被动均衡资源"],
    ["均衡方式", "被动均衡接口", "结构简单、成本可控，主动均衡效率高但复杂"],
    ["通信方式", "UART", "当前连接Python上位机，CAN等作为扩展方向"],
  ];
  selections.forEach(([label, selected, reason], index) => {
    const top = 135 + index * 108;
    addCard(slide, label, { left: 64, top, width: 110, height: 82 }, { fontSize: 15, bold: true, color: TITLE });
    addCard(slide, selected, { left: 190, top, width: 210, height: 82 }, { fontSize: 16, bold: true, color: ACCENT });
    addCard(slide, reason, { left: 420, top, width: 480, height: 82 }, { fontSize: 14, alignment: "left" });
  });
  slide.images.add({
    blob: bqImageBytes,
    contentType: "image/png",
    alt: "BQ769X0内部结构框图",
    fit: "contain",
    position: { left: 925, top: 132, width: 285, height: 390 },
  });
  addText(slide, "图片来源：项目资料BQ769X0应用资料", {
    left: 925,
    top: 528,
    width: 285,
    height: 34,
  }, { fontSize: 10, color: MUTED, alignment: "center" });
  addText(slide, "硬件仍处于联调阶段，软件I2C读取BQ76940的稳定性、上拉电阻、唤醒流程和寄存器读取需要后续验证。", {
    left: 64,
    top: 580,
    width: 820,
    height: 70,
  }, { fontSize: 15, color: "#B45309" });
  addFooter(slide, "资料来源：项目开题报告硬件选型与模块设计");
  setNotes(slide, "选型页说明为什么合适，不写成所有硬件均已完整验证成功。");
}

// 8. 软件设计与任务流程
{
  const slide = presentation.slides.add();
  slide.background.fill = BG;
  addTitle(slide, "软件设计与任务流程");
  const bms = addCard(slide, "BMS_Task\n高优先级，500ms周期\n可由ALERT通知唤醒", {
    left: 90,
    top: 140,
    width: 290,
    height: 110,
  }, { fontSize: 15, bold: true, color: "#1E3A8A" });
  const uart = addCard(slide, "UART_Task\n低优先级，50ms轮询\n解析串口命令", {
    left: 430,
    top: 140,
    width: 270,
    height: 110,
  }, { fontSize: 15, bold: true, color: ACCENT_TEAL });
  const alert = addCard(slide, "ALERT中断\n只发送任务通知", {
    left: 745,
    top: 140,
    width: 190,
    height: 110,
  }, { fontSize: 14 });
  const lock = addCard(slide, "I2C互斥锁\n保护共享BQ76940访问", {
    left: 970,
    top: 140,
    width: 190,
    height: 110,
  }, { fontSize: 14 });
  addArrow(slide, alert, bms, { fromSide: "bottom", toSide: "top" });
  addArrow(slide, uart, lock, { fromSide: "top", toSide: "bottom" });
  addArrow(slide, bms, lock, { fromSide: "top", toSide: "bottom" });

  const steps = [
    addNode(slide, "采样\n电压、电流、温度", { left: 80, top: 330, width: 170, height: 80 }, { fontSize: 13 }),
    addNode(slide, "判断\n软件阈值与硬件告警", { left: 290, top: 330, width: 190, height: 80 }, { fontSize: 13 }),
    addNode(slide, "执行\nSOC更新与MOS控制", { left: 520, top: 330, width: 200, height: 80 }, { fontSize: 13 }),
    addNode(slide, "上报\nUART状态输出", { left: 760, top: 330, width: 170, height: 80 }, { fontSize: 13 }),
    addNode(slide, "显示\n上位机刷新", { left: 970, top: 330, width: 150, height: 80 }, { fontSize: 13 }),
  ];
  addArrow(slide, steps[0], steps[1]);
  addArrow(slide, steps[1], steps[2]);
  addArrow(slide, steps[2], steps[3]);
  addArrow(slide, steps[3], steps[4]);

  addText(slide, "中断服务程序不执行I2C操作，寄存器读取、故障解析和状态更新在任务上下文中完成。串口命令若访问BQ76940，也必须在互斥锁保护下进行。", {
    left: 80,
    top: 470,
    width: 1100,
    height: 100,
  }, { fontSize: 16 });
  addText(slide, "SOC采用安时积分，静置约30s后结合开路电压校正，并以满电、空电边界校准。基础被动均衡接口支持掩码与手动控制，不夸大为完整自主策略。", {
    left: 80,
    top: 570,
    width: 1100,
    height: 90,
  }, { fontSize: 16 });
  addFooter(slide, "资料来源：项目开题报告软件设计与流程设计");
  setNotes(slide, "重点讲解任务优先级、共享资源保护和ALERT处理边界。");
}

// 9. 测试与验证方案
{
  const slide = presentation.slides.add();
  slide.background.fill = BG;
  addTitle(slide, "测试与验证方案");
  const tests = [
    ["通信与采集", "验证BQ76940初始化、寄存器读取、ALERT响应和重初始化机制"],
    ["保护与执行", "拟验证过压、欠压、过流、过温、低温充电阈值与MOS动作"],
    ["SOC与校正", "观察安时积分、静置OCV校正和边界校准的误差变化"],
    ["交互与调试", "验证串口命令解析、上位机显示和调试控制功能"],
    ["连续运行", "拟进行长时间运行测试，观察通信异常、任务阻塞和数据保存"],
  ];
  tests.forEach(([title, detail], index) => {
    const left = 64 + (index % 3) * 385;
    const top = 145 + Math.floor(index / 3) * 165;
    addCard(slide, `${title}\n${detail}`, { left, top, width: 350, height: 145 }, {
      fontSize: 15,
      alignment: "left",
    });
  });
  addText(slide, "验证顺序：通信稳定后开展采集精度检查，再验证保护动作，最后完成整机连续运行测试。", {
    left: 64,
    top: 500,
    width: 780,
    height: 80,
  }, { fontSize: 17 });
  const sequence = addCard(slide, "通信→采集→保护→SOC→上位机→连续运行", {
    left: 870,
    top: 500,
    width: 300,
    height: 80,
  }, { fontSize: 15, fill: ACCENT_SOFT, color: "#1E3A8A" });
  addFooter(slide, "资料来源：项目开题报告测试与验证方案");
  setNotes(slide, "测试表述以拟验证为主，不宣称当前全部完成。");
}

// 10. 研究方法、技术路线与进度
{
  const slide = presentation.slides.add();
  slide.background.fill = BG;
  addTitle(slide, "研究方法与技术路线");
  const methods = addCard(slide, "研究方法\n文献研究、系统设计、实验调试、软件工程、对比验证", {
    left: 64,
    top: 140,
    width: 280,
    height: 130,
  }, { fontSize: 14 });
  const stages = [
    addNode(slide, "需求分析\n确定功能范围", { left: 420, top: 140, width: 200, height: 90 }, { fontSize: 14 }),
    addNode(slide, "系统设计\n硬件连接与任务划分", { left: 660, top: 140, width: 210, height: 90 }, { fontSize: 14 }),
    addNode(slide, "功能实现\n驱动、算法与通信", { left: 910, top: 140, width: 210, height: 90 }, { fontSize: 14 }),
  ];
  addArrow(slide, stages[0], stages[1]);
  addArrow(slide, stages[1], stages[2]);
  addArrow(slide, methods, stages[0], { fromSide: "right", toSide: "left" });

  const validate = addNode(slide, "联调测试\n通信、保护、SOC与上位机", { left: 660, top: 300, width: 210, height: 90 }, { fontSize: 14 });
  const summary = addNode(slide, "总结完善\n整理数据与论文材料", { left: 910, top: 300, width: 210, height: 90 }, { fontSize: 14 });
  addArrow(slide, stages[2], validate, { fromSide: "bottom", toSide: "top" });
  addArrow(slide, validate, summary, { fromSide: "right", toSide: "left" });

  const schedule = [
    ["2026.6.25-9.22", "选题、任务书、开题报告"],
    ["2026.9.23-10.19", "开题答辩与系统设计"],
    ["2026.10.20-11.1", "中期检查"],
    ["2026.11.2-12.22", "论文撰写、评审与送审"],
    ["2026.12.23", "毕业答辩"],
  ];
  const scheduleItems = schedule.map(([time, task], index) => addNode(
    slide,
    `${time}  ${task}`,
    { left: 64, top: 300 + index * 52, width: 520, height: 44 },
    { fontSize: 12, alignment: "left", lineSpacing: 1.2 },
  ));
  scheduleItems.slice(0, -1).forEach((item, index) => {
    addArrow(slide, item, scheduleItems[index + 1], {
      fromSide: "bottom",
      toSide: "top",
      kind: "straight",
      line: { style: "solid", fill: "#94A3B8", width: 1 },
    });
  });

  addText(slide, "本课题按需求分析、系统设计、软件开发、功能验证和总结完善的顺序推进，测试结论用于调整阈值参数与软件逻辑。", {
    left: 700,
    top: 450,
    width: 480,
    height: 110,
  }, { fontSize: 16 });
  addText(slide, "资料来源：项目开题报告研究方法与技术路线", { left: 64, top: 660, width: 1120, height: 32 }, {
    fontSize: 11,
    color: MUTED,
  });
  setNotes(slide, "收束页强调方法与路线，并按学校进度安排展示后续计划。");
}

const candidatePath = path.join(outputDir, "candidate.pptx");
await (await PresentationFile.exportPptx(presentation)).save(candidatePath);

const previewDir = path.join(outputDir, "previews");
await fs.mkdir(previewDir, { recursive: true });
for (let index = 0; index < 10; index += 1) {
  const slide = presentation.slides.getItem(index);
  const preview = await presentation.export({ slide, format: "png", scale: 1 });
  await fs.writeFile(
    path.join(previewDir, `slide-${String(index + 1).padStart(2, "0")}.png`),
    new Uint8Array(await preview.arrayBuffer()),
  );
}

console.log(`slides=${presentation.slides.length}`);
console.log(candidatePath);
