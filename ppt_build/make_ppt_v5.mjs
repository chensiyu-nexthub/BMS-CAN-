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
const OK = "#2F6E52";

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
  name: "Cool Blue Gray Academic",
  themeColors: {
    accent1: PRIMARY,
    accent2: ACCENT,
    accent3: "#C4A55D",
    accent4: WARN,
    accent5: OK,
    accent6: "#70849A",
    bg1: "#FFFFFF",
    bg2: BG,
    tx1: TEXT,
    tx2: MUTED,
    dk1: "#121C27",
    dk2: "#1B2B3A",
    lt1: "#FFFFFF",
    lt2: SOFT,
    hlink: PRIMARY,
    folHlink: ACCENT,
  },
};

function text(slide, value, position, style = {}) {
  const shape = slide.shapes.add({
    geometry: "textbox",
    position,
    fill: "none",
    line: { style: "solid", fill: "none", width: 0 },
  });
  shape.text = value;
  shape.text.style = {
    typeface: FONT,
    fontSize: 16,
    color: TEXT,
    alignment: "left",
    verticalAlignment: "top",
    lineSpacing: 1.5,
    wrap: "square",
    autoFit: "none",
    ...style,
  };
  return shape;
}

function box(slide, value, position, style = {}, fill = "#FFFFFF") {
  const shape = slide.shapes.add({
    geometry: "rect",
    position,
    fill,
    line: { style: "solid", fill: BORDER, width: 1 },
    borderRadius: 3,
    shadow: "shadow-none",
  });
  shape.text = value;
  shape.text.style = {
    typeface: FONT,
    fontSize: 15,
    color: TEXT,
    alignment: "center",
    verticalAlignment: "middle",
    lineSpacing: 1.5,
    wrap: "square",
    autoFit: "none",
    insets: { top: 6, right: 10, bottom: 6, left: 10 },
    ...style,
  };
  return shape;
}

function line(slide, from, to, options = {}) {
  return slide.shapes.connect(from, to, {
    kind: "straight",
    fromSide: "right",
    toSide: "left",
    line: { style: "solid", fill: ACCENT, width: 1.5 },
    head: { type: "triangle", width: "med", length: "med" },
    ...options,
  });
}

function title(slide, value) {
  text(slide, value, { left: 56, top: 36, width: 1050, height: 44 }, {
    fontSize: 29,
    bold: true,
    color: PRIMARY,
  });
  slide.shapes.add({
    geometry: "rect",
    position: { left: 56, top: 88, width: 72, height: 3 },
    fill: ACCENT,
    line: { style: "solid", fill: ACCENT, width: 0 },
  });
}

function page(slide, n) {
  text(slide, `${String(n).padStart(2, "0")} / 9`, {
    left: 1120,
    top: 665,
    width: 100,
    height: 22,
  }, { fontSize: 10, color: MUTED, alignment: "right" });
}

function source(slide, value) {
  text(slide, value, { left: 56, top: 665, width: 850, height: 22 }, {
    fontSize: 10,
    color: MUTED,
  });
}

function note(slide, value) {
  slide.speakerNotes.textFrame.setText(value);
  slide.speakerNotes.setVisible(false);
}

const buildDir = "D:/xianmu/BMS-main/ppt_build";
const outputDir = "D:/xianmu/BMS-main/ppt_output";
await fs.mkdir(buildDir, { recursive: true });
await fs.mkdir(outputDir, { recursive: true });

const coverImage = await fs.readFile("D:/xianmu/BMS-main/ppt_build/cover_background.png");
const balancingImage = await fs.readFile(
  "D:/xianmu/BMS-main/开题报告补充图片/最终图/图6_被动均衡驱动参考电路图.png",
);
const batteryImage = await fs.readFile(
  "D:/xianmu/BMS-main/开题报告补充图片/最终图/图7_BQ769X0外围连接参考图.png",
);

// 1 封面
{
  const slide = presentation.slides.add();
  slide.background.fill = "#FFFFFF";
  slide.images.add({
    blob: coverImage,
    contentType: "image/png",
    alt: "锂电池管理系统背景",
    fit: "cover",
    position: { left: 0, top: 0, width: 1280, height: 250 },
  });
  slide.shapes.add({
    geometry: "rect",
    position: { left: 56, top: 282, width: 64, height: 3 },
    fill: PRIMARY,
    line: { style: "solid", fill: PRIMARY, width: 0 },
  });
  text(slide, "基于STM32的多串锂电池管理系统设计与实现", {
    left: 56,
    top: 310,
    width: 940,
    height: 86,
  }, { fontSize: 31, bold: true, color: PRIMARY, lineSpacing: 1.5 });
  text(slide, "本科毕业设计（论文）开题答辩", {
    left: 56,
    top: 408,
    width: 420,
    height: 32,
  }, { fontSize: 17, color: ACCENT });
  const fields = [
    ["汇报人", 56],
    ["学号", 350],
    ["专业", 644],
    ["指导教师", 938],
  ];
  fields.forEach(([label, left]) => {
    text(slide, label, { left, top: 500, width: 90, height: 24 }, {
      fontSize: 12,
      color: MUTED,
    });
    slide.shapes.add({
      geometry: "rect",
      position: { left, top: 534, width: 220, height: 1 },
      fill: BORDER,
      line: { style: "solid", fill: BORDER, width: 0 },
    });
  });
  text(slide, "背景图：Image2生成，仅用于答辩视觉背景", {
    left: 56,
    top: 665,
    width: 400,
    height: 20,
  }, { fontSize: 10, color: MUTED });
  note(slide, "开场介绍题目、姓名、专业和指导教师，说明汇报围绕8分钟开题答辩展开。");
}

// 2 研究背景
{
  const slide = presentation.slides.add();
  slide.background.fill = BG;
  title(slide, "研究背景与选题依据");
  text(slide, "多串锂电池组在充放电过程中容易出现单体差异，电压、温度和容量不一致会降低可用容量，并带来过充、过放、过流和过温风险。", {
    left: 56,
    top: 128,
    width: 700,
    height: 84,
  }, { fontSize: 17, lineSpacing: 1.6 });
  const chain = [
    box(slide, "多串成组", { left: 56, top: 250, width: 150, height: 52 }, { fontSize: 15, bold: true }),
    box(slide, "单体差异", { left: 238, top: 250, width: 150, height: 52 }, { fontSize: 15 }),
    box(slide, "运行风险", { left: 420, top: 250, width: 150, height: 52 }, { fontSize: 15 }),
    box(slide, "实时管理", { left: 602, top: 250, width: 150, height: 52 }, {
      fontSize: 15,
      bold: true,
      color: PRIMARY,
      fill: SOFT,
    }),
  ];
  line(slide, chain[0], chain[1]);
  line(slide, chain[1], chain[2]);
  line(slide, chain[2], chain[3]);
  text(slide, "课题面向9串18650电池组，设计能够实时采集状态、判断风险并执行保护的管理系统。", {
    left: 56,
    top: 356,
    width: 700,
    height: 70,
  }, { fontSize: 17, lineSpacing: 1.6 });
  slide.images.add({
    blob: balancingImage,
    contentType: "image/png",
    alt: "被动均衡驱动参考电路图",
    fit: "contain",
    position: { left: 810, top: 140, width: 390, height: 260 },
  });
  text(slide, "被动均衡驱动参考电路\n来源：项目参考资料", {
    left: 810,
    top: 404,
    width: 390,
    height: 38,
  }, { fontSize: 10, color: MUTED, alignment: "center", lineSpacing: 1.5 });
  const research = [
    ["温度风险", "徐伟巍等[1]指出高倍率放电会显著增加温升。"],
    ["远程监测", "黄新星[2]说明远程数据交互可弥补本地能力不足。"],
    ["工程参考", "黄青青等[3]验证了以STM32为核心的BMS方案。"],
  ];
  research.forEach(([label, content], index) => {
    const top = 462 + index * 56;
    text(slide, label, { left: 56, top, width: 150, height: 24 }, {
      fontSize: 13,
      bold: true,
      color: PRIMARY,
    });
    text(slide, content, { left: 220, top, width: 580, height: 44 }, {
      fontSize: 13,
      color: TEXT,
      lineSpacing: 1.5,
    });
  });
  page(slide, 2);
  source(slide, "资料来源：开题报告研究背景与国内研究现状");
  note(slide, "用问题链说明选题来源，右侧图仅作为均衡硬件参考，不宣称当前已完成全部硬件验证。");
}

// 3 研究目的
{
  const slide = presentation.slides.add();
  slide.background.fill = BG;
  title(slide, "研究目的");
  const root = box(slide, "设计并实现基于STM32F103C8T6与BQ76940的多串锂电池管理系统", {
    left: 180,
    top: 132,
    width: 760,
    height: 72,
  }, { fontSize: 19, bold: true, color: "#FFFFFF", fill: PRIMARY });
  const goals = [
    ["状态感知", "采集单体电压、电流、温度", 72],
    ["安全控制", "识别阈值与硬件告警，控制MOS", 324],
    ["状态估算", "SOC计算与静置OCV校正", 576],
    ["调试交互", "UART命令与Python上位机", 828],
  ];
  goals.forEach(([name, detail, left]) => {
    const item = box(slide, `${name}\n${detail}`, { left, top: 310, width: 260, height: 96 }, {
      fontSize: 14,
      lineSpacing: 1.5,
    });
    line(slide, root, item, { fromSide: "bottom", toSide: "top" });
  });
  text(slide, "研究目标不是追求复杂算法，而是形成结构清晰、安全边界明确、可在实验平台上验证的完整实现。", {
    left: 180,
    top: 458,
    width: 760,
    height: 60,
  }, { fontSize: 17, color: PRIMARY, bold: true, lineSpacing: 1.6 });
  const extensions = ["CAN为扩展方向", "蓝牙为扩展方向", "串口屏为扩展方向"];
  extensions.forEach((value, index) => {
    box(slide, value, { left: 180 + index * 260, top: 550, width: 220, height: 46 }, {
      fontSize: 12,
      color: MUTED,
      fill: "#FFFFFF",
    });
  });
  page(slide, 3);
  source(slide, "资料来源：开题报告研究目的与预期目标");
  note(slide, "明确CAN、蓝牙、串口屏只是扩展预留，不写成当前已实现功能。");
}

// 4 研究意义
{
  const slide = presentation.slides.add();
  slide.background.fill = BG;
  title(slide, "研究意义");
  const meanings = [
    ["工程意义", "降低过充、过放、过流、过温风险，延长电池组使用寿命", "#FFFFFF"],
    ["学习意义", "覆盖嵌入式软硬件、I2C通信、FreeRTOS任务调度和电池保护策略", "#FFFFFF"],
    ["应用参考", "为同类小规模多串锂电池管理系统提供可复用设计思路", SOFT],
  ];
  meanings.forEach(([name, detail, fill], index) => {
    const top = 130 + index * 112;
    box(slide, name, { left: 100, top, width: 150, height: 70 }, {
      fontSize: 18,
      bold: true,
      color: PRIMARY,
      fill,
    });
    text(slide, detail, { left: 300, top: top + 10, width: 690, height: 52 }, {
      fontSize: 16,
      lineSpacing: 1.6,
    });
  });
  text(slide, "课题重点解决工程实现与验证闭环，价值体现在安全、可维护和可复用，而不是单纯增加功能。", {
    left: 130,
    top: 500,
    width: 800,
    height: 50,
  }, { fontSize: 18, bold: true, color: PRIMARY, lineSpacing: 1.6 });
  page(slide, 4);
  source(slide, "资料来源：开题报告研究意义");
  note(slide, "三层意义从工程、学习到应用参考，避免空泛夸大。");
}

// 5 国内外研究现状与研究内容
{
  const slide = presentation.slides.add();
  slide.background.fill = BG;
  title(slide, "国内外研究现状与研究内容");
  const table = slide.tables.add({
    rows: 4,
    columns: 3,
    left: 56,
    top: 130,
    width: 700,
    height: 310,
    values: [
      ["方向", "国内进展", "国外进展"],
      ["系统实现", "STM32与多串采集保护方案", "STM32、LTC6804与物联网交互"],
      ["状态估算", "安时积分结合OCV与滤波修正", "EKF、LSTM与优化算法融合"],
      ["均衡保护", "主动均衡、被动均衡与温度保护", "拓扑优化与多级安全保护"],
    ],
  });
  table.rows[0].height = 52;
  for (let r = 1; r < 4; r += 1) table.rows[r].height = 86;
  table.borders.assign({ style: "solid", fill: BORDER, width: 1 });
  table.styleOptions = { headerRow: true, bandedRows: true };
  for (let r = 0; r < 4; r += 1) {
    for (let c = 0; c < 3; c += 1) {
      table.getCell(r, c).text.style = {
        typeface: FONT,
        fontSize: r === 0 ? 14 : 13,
        color: r === 0 ? "#FFFFFF" : TEXT,
        alignment: c === 0 ? "center" : "left",
        verticalAlignment: "middle",
        lineSpacing: 1.5,
        fill: r === 0 ? PRIMARY : (r % 2 === 0 ? "#F2F6F8" : "#FFFFFF"),
        insets: { top: 6, right: 8, bottom: 6, left: 8 },
        bold: r === 0,
      };
    }
  }
  box(slide, "研究内容", { left: 800, top: 130, width: 360, height: 56 }, {
    fontSize: 16,
    bold: true,
    color: "#FFFFFF",
    fill: PRIMARY,
  });
  text(slide, "系统架构、采集保护、SOC估算与交互", {
    left: 800,
    top: 205,
    width: 360,
    height: 54,
  }, { fontSize: 14, lineSpacing: 1.5 });
  const keyItems = ["硬件架构", "采集保护", "SOC估算", "上位机交互"];
  keyItems.forEach((name, index) => {
    box(slide, name, { left: 800, top: 280 + index * 52, width: 360, height: 42 }, {
      fontSize: 13,
      bold: true,
      color: PRIMARY,
      fill: SOFT,
    });
  });
  text(slide, "研究空间：在有限MCU资源下实现稳定采集、及时保护和可验证状态估算。", {
    left: 56,
    top: 480,
    width: 900,
    height: 34,
  }, { fontSize: 16, bold: true, color: PRIMARY });
  page(slide, 5);
  source(slide, "资料来源：开题报告研究现状与基本内容");
  note(slide, "合并研究趋势与本课题研究内容，避免逐篇念文献。");
}

// 6 系统方案与硬件选型
{
  const slide = presentation.slides.add();
  slide.background.fill = BG;
  title(slide, "系统总体方案与硬件选型");
  const layers = [
    ["电池层", "9串18650锂电池组"],
    ["采集层", "BQ76940采集电压、电流、温度，提供硬件告警"],
    ["控制层", "STM32读取数据，执行阈值判断、SOC估算与命令交互"],
    ["交互层", "UART连接Python上位机，显示状态并接收命令"],
  ];
  const layerShapes = layers.map(([name, detail], index) => {
    const top = 126 + index * 62;
    const shape = box(slide, name, { left: 56, top, width: 110, height: 50 }, {
      fontSize: 14,
      bold: true,
      color: index === 2 ? "#FFFFFF" : PRIMARY,
      fill: index === 2 ? PRIMARY : "#FFFFFF",
    });
    text(slide, detail, { left: 180, top: top + 11, width: 490, height: 32 }, {
      fontSize: 12,
      lineSpacing: 1.5,
    });
    return shape;
  });
  for (let index = 1; index < layerShapes.length; index += 1) {
    line(slide, layerShapes[index - 1], layerShapes[index], {
      fromSide: "bottom",
      toSide: "top",
      head: { type: "none" },
    });
  }
  const choices = [
    ["主控", "STM32F103C8T6", "外设资源与成本适合课题验证"],
    ["模拟前端", "BQ76940", "多串采集、硬件告警与MOS控制"],
    ["均衡", "被动均衡接口", "结构简单，适合实验验证"],
    ["通信", "UART", "当前连接Python上位机"],
  ];
  choices.forEach(([name, selected, reason], index) => {
    const top = 120 + index * 58;
    box(slide, name, { left: 720, top, width: 90, height: 42 }, {
      fontSize: 12,
      bold: true,
      color: PRIMARY,
      fill: SOFT,
    });
    box(slide, selected, { left: 822, top, width: 130, height: 42 }, {
      fontSize: 11,
      bold: true,
      color: PRIMARY,
    });
    text(slide, reason, { left: 964, top: top + 5, width: 230, height: 34 }, {
      fontSize: 10,
      lineSpacing: 1.5,
    });
  });
  slide.images.add({
    blob: batteryImage,
    contentType: "image/png",
    alt: "BQ769X0外围连接参考图",
    fit: "contain",
    position: { left: 720, top: 370, width: 470, height: 230 },
  });
  text(slide, "BQ769X0外围连接参考图 来源：BQ769X0参考设计资料", {
    left: 720,
    top: 604,
    width: 470,
    height: 20,
  }, { fontSize: 10, color: MUTED, alignment: "center" });
  box(slide, "CAN、蓝牙、串口屏仅作为扩展预留，不计入当前实现范围。", {
    left: 56,
    top: 420,
    width: 620,
    height: 42,
  }, { fontSize: 12, color: WARN, fill: "#F5EDED" });
  box(slide, "安全控制：BQ76940硬件告警与STM32软件阈值共同判断，驱动充放电MOS", {
    left: 56,
    top: 484,
    width: 620,
    height: 62,
  }, { fontSize: 13, bold: true, color: PRIMARY, fill: SOFT, lineSpacing: 1.5 });
  page(slide, 6);
  source(slide, "资料来源：开题报告研究方案、总体设计与硬件选型");
  note(slide, "一页内同时交代系统分层与选型理由，注意说明扩展功能边界。");
}

// 7 软件设计与任务流程
{
  const slide = presentation.slides.add();
  slide.background.fill = BG;
  title(slide, "软件设计与任务流程");
  const bms = box(slide, "BMS_Task\n高优先级\n500ms周期，可由ALERT唤醒", {
    left: 80,
    top: 126,
    width: 280,
    height: 100,
  }, { fontSize: 14, bold: true, color: "#FFFFFF", fill: PRIMARY });
  const uart = box(slide, "UART_Task\n低优先级\n50ms轮询串口命令", {
    left: 400,
    top: 126,
    width: 220,
    height: 100,
  }, { fontSize: 14 });
  const alert = box(slide, "ALERT中断\n只发送任务通知", {
    left: 660,
    top: 126,
    width: 170,
    height: 100,
  }, { fontSize: 13, color: WARN, fill: "#F5EDED" });
  const mutex = box(slide, "I2C互斥锁\n保护BQ76940访问", {
    left: 870,
    top: 126,
    width: 170,
    height: 100,
  }, { fontSize: 13, color: PRIMARY, fill: SOFT });
  line(slide, alert, bms, { fromSide: "bottom", toSide: "top", fromOffset: 0.35 });
  line(slide, uart, mutex, { fromSide: "top", toSide: "top" });
  const steps = [
    ["采样", "电压、电流、温度"],
    ["判断", "软件阈值与硬件告警"],
    ["执行", "SOC更新与MOS控制"],
    ["上报", "UART状态输出"],
    ["显示", "上位机刷新"],
  ];
  const stepShapes = steps.map(([name, detail], index) => {
    const shape = box(slide, `${name}\n${detail}`, {
      left: 80 + index * 210,
      top: 310,
      width: 180,
      height: 80,
    }, { fontSize: 12 });
    return shape;
  });
  for (let index = 1; index < stepShapes.length; index += 1) {
    line(slide, stepShapes[index - 1], stepShapes[index]);
  }
  text(slide, "中断服务程序不执行I2C操作，寄存器读取、故障解析与状态更新在任务上下文完成。串口命令访问BQ76940时，同样由互斥锁保护。", {
    left: 80,
    top: 456,
    width: 760,
    height: 60,
  }, { fontSize: 15, lineSpacing: 1.6 });
  text(slide, "SOC采用安时积分，静置约30s后结合开路电压校正，并以满电、空电边界校准。", {
    left: 80,
    top: 520,
    width: 760,
    height: 34,
  }, { fontSize: 15 });
  page(slide, 7);
  source(slide, "资料来源：开题报告软件设计与流程设计");
  note(slide, "重点说明任务优先级、共享资源保护和ALERT中断边界。");
}

// 8 研究方法与技术路线
{
  const slide = presentation.slides.add();
  slide.background.fill = BG;
  title(slide, "研究方法与技术路线");
  const route = [
    ["需求分析", "功能范围与边界"],
    ["系统设计", "硬件结构与任务划分"],
    ["功能实现", "驱动、算法与通信"],
    ["联调测试", "保护、SOC与上位机"],
    ["总结完善", "数据整理与论文撰写"],
  ];
  const routeShapes = route.map(([name, detail], index) => {
    const shape = box(slide, `${name}\n${detail}`, {
      left: 56 + index * 230,
      top: 126,
      width: 200,
      height: 82,
    }, { fontSize: 13 });
    return shape;
  });
  for (let index = 1; index < routeShapes.length; index += 1) {
    line(slide, routeShapes[index - 1], routeShapes[index]);
  }
  const methods = [
    ["文献研究", "梳理BMS实现、SOC估算与均衡策略"],
    ["模块设计", "按主控、采集、保护、通信和显示划分"],
    ["实验验证", "从通信读取到保护动作逐步记录数据"],
  ];
  methods.forEach(([name, detail], index) => {
    const top = 260 + index * 62;
    box(slide, name, { left: 80, top, width: 130, height: 46 }, {
      fontSize: 14,
      bold: true,
      color: PRIMARY,
      fill: "#FFFFFF",
    });
    text(slide, detail, { left: 230, top: top + 8, width: 430, height: 32 }, {
      fontSize: 13,
      lineSpacing: 1.5,
    });
  });
  const schedule = [
    ["2026.06.25-07.02", "选题"],
    ["2026.07.03-07.16", "文献调研"],
    ["2026.07.17-09.22", "开题报告"],
    ["2026.09.24-10.19", "系统设计"],
    ["2026.10.20-11.01", "硬件加工"],
    ["2026.11.02-11.30", "论文撰写"],
  ];
  schedule.forEach(([time, task], index) => {
    const top = 245 + index * 38;
    box(slide, time, { left: 710, top, width: 175, height: 30 }, {
      fontSize: 10,
      color: PRIMARY,
      fill: SOFT,
    });
    text(slide, task, { left: 900, top: top + 5, width: 180, height: 24 }, {
      fontSize: 12,
    });
  });
  text(slide, "路线核心是从明确需求到工程实现的闭环，测试结果反过来修正驱动、参数与软件逻辑。", {
    left: 56,
    top: 520,
    width: 900,
    height: 34,
  }, { fontSize: 16, bold: true, color: PRIMARY });
  page(slide, 8);
  source(slide, "资料来源：开题报告研究方法、技术路线与进度安排");
  note(slide, "把研究方法、技术路线和学校进度合并成一页收束。");
}

// 9 测试验证与预期成果
{
  const slide = presentation.slides.add();
  slide.background.fill = BG;
  title(slide, "测试验证与预期成果");
  const tests = [
    ["通信与采集", "验证BQ76940初始化、寄存器读取与ALERT响应"],
    ["保护与执行", "验证保护阈值触发与MOS动作"],
    ["SOC与校正", "验证安时积分、OCV校正与边界校准"],
    ["交互与显示", "验证串口命令与上位机刷新"],
    ["运行稳定性", "验证长时间运行和异常恢复"],
  ];
  tests.forEach(([name, detail], index) => {
    const top = 126 + index * 64;
    box(slide, name, { left: 56, top, width: 150, height: 46 }, {
      fontSize: 13,
      bold: true,
      color: PRIMARY,
      fill: "#FFFFFF",
    });
    text(slide, detail, { left: 230, top: top + 7, width: 400, height: 34 }, {
      fontSize: 12,
      lineSpacing: 1.5,
    });
  });
  const expected = box(slide, "预期成果", { left: 700, top: 126, width: 150, height: 46 }, {
    fontSize: 16,
    bold: true,
    color: "#FFFFFF",
    fill: PRIMARY,
  });
  const outcomes = [
    "完成多串锂电池管理系统软硬件设计",
    "实现状态采集、保护判断、SOC估算与上位机交互",
    "形成测试数据与毕业设计论文",
  ];
  outcomes.forEach((value, index) => {
    box(slide, value, { left: 700, top: 200 + index * 54, width: 450, height: 42 }, {
      fontSize: 13,
      lineSpacing: 1.5,
    });
  });
  text(slide, "测试表述以拟验证为主，未设定具体误差指标的项目不虚构数值。", {
    left: 56,
    top: 480,
    width: 560,
    height: 30,
  }, { fontSize: 13, color: MUTED });
  text(slide, "结论：本课题以安全控制与工程验证为中心，形成可复用的多串锂电池管理系统实现路径。", {
    left: 56,
    top: 550,
    width: 980,
    height: 40,
  }, { fontSize: 18, bold: true, color: PRIMARY });
  page(slide, 9);
  source(slide, "资料来源：开题报告测试与验证方案");
  note(slide, "最后强调拟验证边界和预期成果，回答老师可能关心的可行性问题。");
}

const file = await PresentationFile.exportPptx(presentation);
await file.save(`${buildDir}/candidate_ppt_v5.pptx`);
console.log(`${buildDir}/candidate_ppt_v5.pptx`);
