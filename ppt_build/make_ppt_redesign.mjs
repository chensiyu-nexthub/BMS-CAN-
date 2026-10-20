import fs from "node:fs/promises";
import path from "node:path";
import { Presentation, PresentationFile } from "@oai/artifact-tool";

const FONT = "Microsoft YaHei";
const BG = "#F4F7F9";
const PRIMARY = "#174A7C";
const SECONDARY = "#3F7EA6";
const SOFT = "#DCEAF3";
const BORDER = "#B8C6D1";
const TEXT = "#22303B";
const MUTED = "#5E6B76";
const WARN = "#B94B4B";
const OK = "#2F7D5B";
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
  name: "Cool Blue Gray Research",
  themeColors: {
    accent1: PRIMARY,
    accent2: SECONDARY,
    accent3: "#D9A25F",
    accent4: WARN,
    accent5: OK,
    accent6: "#6A7F95",
    bg1: "#FFFFFF",
    bg2: BG,
    tx1: TEXT,
    tx2: MUTED,
    dk1: "#12212B",
    dk2: "#1F3342",
    lt1: "#FFFFFF",
    lt2: SOFT,
    hlink: PRIMARY,
    folHlink: SECONDARY,
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
    fontSize: 15,
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

function addBox(slide, text, position, style = {}, fill = "#FFFFFF") {
  const shape = slide.shapes.add({
    geometry: "rect",
    position,
    fill,
    line: LINE,
    borderRadius: 8,
    shadow: "shadow-none",
  });
  shape.text = text;
  shape.text.style = {
    typeface: FONT,
    fontSize: 15,
    color: TEXT,
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

function addArrow(slide, from, to, options = {}) {
  return slide.shapes.connect(from, to, {
    kind: "straight",
    fromSide: "right",
    toSide: "left",
    line: { style: "solid", fill: SECONDARY, width: 1.6 },
    head: { type: "triangle", width: "med", length: "med" },
    ...options,
  });
}

function addTitle(slide, text) {
  const title = addText(slide, text, { left: 64, top: 38, width: 920, height: 50 }, {
    fontSize: 30,
    bold: true,
    color: PRIMARY,
    lineSpacing: 1.5,
  });
  slide.shapes.add({
    geometry: "rect",
    position: { left: 64, top: 100, width: 96, height: 4 },
    fill: SECONDARY,
    line: { style: "solid", fill: SECONDARY, width: 0 },
  });
  return title;
}

function addPage(slide, number) {
  return addText(slide, `${String(number).padStart(2, "0")} / 10`, {
    left: 1120,
    top: 655,
    width: 96,
    height: 24,
  }, {
    fontSize: 11,
    color: MUTED,
    alignment: "right",
    lineSpacing: 1.5,
  });
}

function addSource(slide, text) {
  return addText(slide, text, { left: 64, top: 655, width: 800, height: 24 }, {
    fontSize: 11,
    color: MUTED,
    lineSpacing: 1.5,
  });
}

function setNotes(slide, text) {
  slide.speakerNotes.textFrame.setText(text);
  slide.speakerNotes.setVisible(false);
}

const buildDir = "D:/xianmu/BMS-main/ppt_build";
const outputDir = "D:/xianmu/BMS-main/ppt_output";
await fs.mkdir(buildDir, { recursive: true });
await fs.mkdir(outputDir, { recursive: true });

const coverImage = await fs.readFile("D:/xianmu/BMS-main/ppt_build/cover_background.png");
const bqImage = await fs.readFile(
  "D:/xianmu/BMS-main/开题报告补充图片/最终图/图7_BQ769X0外围连接参考图.png",
);
const softwareImage = await fs.readFile(
  "D:/xianmu/BMS-main/开题报告补充图片/最终图/图3_软件任务结构框图.png",
);
const hardwareBackgroundImage = await fs.readFile(
  "D:/xianmu/BMS-main/开题报告补充图片/最终图/图6_被动均衡驱动参考电路图.png",
);

// 1. 学术封面
{
  const slide = presentation.slides.add();
  slide.background.fill = BG;
  slide.images.add({
    blob: coverImage,
    contentType: "image/png",
    alt: "锂电池管理系统主题背景",
    fit: "cover",
    position: { left: 0, top: 0, width: 1280, height: 240 },
  });
  slide.shapes.add({
    geometry: "rect",
    position: { left: 64, top: 250, width: 84, height: 4 },
    fill: PRIMARY,
    line: { style: "solid", fill: PRIMARY, width: 0 },
  });
  addText(slide, "基于STM32的多串锂电池管理系统设计与实现", {
    left: 64,
    top: 282,
    width: 860,
    height: 116,
  }, {
    fontSize: 34,
    bold: true,
    color: PRIMARY,
    lineSpacing: 1.5,
  });
  addText(slide, "本科毕业设计（论文）开题答辩", {
    left: 64,
    top: 405,
    width: 420,
    height: 40,
  }, {
    fontSize: 20,
    color: SECONDARY,
    lineSpacing: 1.5,
  });

  const fields = [
    ["汇报人", 64, 500],
    ["学号", 400, 500],
    ["专业", 64, 560],
    ["指导教师", 400, 560],
  ];
  fields.forEach(([label, left, top]) => {
    addText(slide, label, { left, top, width: 96, height: 30 }, {
      fontSize: 15,
      color: TEXT,
      lineSpacing: 1.5,
    });
    slide.shapes.add({
      geometry: "rect",
      position: { left: left + 110, top: top + 24, width: 220, height: 1 },
      fill: BORDER,
      line: { style: "solid", fill: BORDER, width: 0 },
    });
  });

  addText(slide, "背景图：Image2生成，仅作学术答辩视觉背景", {
    left: 64,
    top: 660,
    width: 500,
    height: 24,
  }, {
    fontSize: 11,
    color: MUTED,
    lineSpacing: 1.5,
  });
  setNotes(slide, "开场介绍课题名称、答辩身份和指导教师，背景为生成图，不作为实验照片。");
}

// 2. 研究背景与问题界定
{
  const slide = presentation.slides.add();
  slide.background.fill = BG;
  addTitle(slide, "研究背景与问题界定");
  const chain = [
    addBox(slide, "锂电池应用持续扩大\n储能与小型动力场景增多", { left: 64, top: 165, width: 240, height: 120 }, { fontSize: 15, fill: "#FFFFFF" }),
    addBox(slide, "多串成组带来单体差异\n电压、容量与温度不一致", { left: 350, top: 165, width: 240, height: 120 }, { fontSize: 15, fill: "#FFFFFF" }),
    addBox(slide, "运行风险需要实时约束\n过充、过放、过流、过温", { left: 636, top: 165, width: 240, height: 120 }, { fontSize: 15, fill: "#FFFFFF" }),
    addBox(slide, "管理系统成为必要环节\n采集、判断、保护与显示", { left: 922, top: 165, width: 240, height: 120 }, { fontSize: 15, fill: SOFT, color: PRIMARY, bold: true }),
  ];
  addArrow(slide, chain[0], chain[1]);
  addArrow(slide, chain[1], chain[2]);
  addArrow(slide, chain[2], chain[3]);

  slide.images.add({
    blob: hardwareBackgroundImage,
    contentType: "image/png",
    alt: "被动均衡驱动参考电路图",
    fit: "contain",
    position: { left: 830, top: 335, width: 370, height: 235 },
  });
  addText(slide, "被动均衡驱动参考电路\n来源：项目参考资料", {
    left: 830,
    top: 578,
    width: 370,
    height: 35,
  }, { fontSize: 11, color: MUTED, alignment: "center", lineSpacing: 1.5 });

  addText(slide, "多串锂电池组的安全边界与单体差异有关，电池管理系统需要在有限嵌入式资源内完成状态感知、风险判断和执行控制。", {
    left: 64,
    top: 350,
    width: 740,
    height: 90,
  }, { fontSize: 17, lineSpacing: 1.7 });
  addText(slide, "课题面向这一需求，围绕多串电池组的安全运行与状态可观测性展开设计。", {
    left: 64,
    top: 465,
    width: 740,
    height: 60,
  }, { fontSize: 17, lineSpacing: 1.7 });
  addPage(slide, 2);
  addSource(slide, "资料来源：开题报告研究背景");
  setNotes(slide, "本页用因果链说明选题来源，不夸大应用规模，重点落在多串成组后的管理需求。");
}

// 3. 研究目的与预期成果
{
  const slide = presentation.slides.add();
  slide.background.fill = BG;
  addTitle(slide, "研究目的与预期成果");
  const central = addBox(slide, "设计一套可验证的多串锂电池管理系统", { left: 390, top: 145, width: 500, height: 100 }, {
    fontSize: 20,
    bold: true,
    color: "#FFFFFF",
    fill: PRIMARY,
  });
  const branches = [
    addBox(slide, "状态感知\n电压、电流、温度", { left: 100, top: 320, width: 260, height: 110 }, { fontSize: 15, fill: "#FFFFFF" }),
    addBox(slide, "安全控制\n阈值判断与MOS动作", { left: 430, top: 320, width: 260, height: 110 }, { fontSize: 15, fill: "#FFFFFF" }),
    addBox(slide, "状态估算\nSOC计算与校正", { left: 760, top: 320, width: 260, height: 110 }, { fontSize: 15, fill: "#FFFFFF" }),
    addBox(slide, "调试交互\nUART命令与上位机", { left: 1090, top: 320, width: 130, height: 110 }, { fontSize: 13, fill: "#FFFFFF" }),
  ];
  addArrow(slide, central, branches[0], { fromSide: "bottom", toSide: "top" });
  addArrow(slide, central, branches[1], { fromSide: "bottom", toSide: "top" });
  addArrow(slide, central, branches[2], { fromSide: "right", toSide: "top" });
  addArrow(slide, central, branches[3], { fromSide: "right", toSide: "top" });

  addText(slide, "预期形成一套以STM32F103C8T6与BQ76940为核心的软硬件方案，完成采样、保护、SOC估算和基础被动均衡接口，并通过上位机进行调试与验证。", {
    left: 100,
    top: 500,
    width: 980,
    height: 90,
  }, { fontSize: 17, lineSpacing: 1.7 });
  addPage(slide, 3);
  addSource(slide, "资料来源：开题报告研究目的与预期目标");
  setNotes(slide, "强调验证型目标，不把CAN、蓝牙、串口屏写成当前已实现功能。");
}

// 4. 研究意义
{
  const slide = presentation.slides.add();
  slide.background.fill = BG;
  addTitle(slide, "研究意义");
  const layers = [
    addBox(slide, "工程意义\n降低过充、过放与过温风险，延长电池组使用寿命", { left: 150, top: 145, width: 900, height: 100 }, { fontSize: 17, fill: "#FFFFFF" }),
    addBox(slide, "学习意义\n覆盖嵌入式软硬件、通信、RTOS与电池保护策略", { left: 190, top: 275, width: 820, height: 100 }, { fontSize: 17, fill: "#FFFFFF" }),
    addBox(slide, "应用参考\n为同类多串锂电池管理系统提供可复用设计思路", { left: 230, top: 405, width: 740, height: 100 }, { fontSize: 17, fill: "#FFFFFF" }),
  ];
  addArrow(slide, layers[0], layers[1], { fromSide: "bottom", toSide: "top" });
  addArrow(slide, layers[1], layers[2], { fromSide: "bottom", toSide: "top" });
  addText(slide, "本课题的价值不在于追求复杂算法，而在于形成结构清晰、可验证、可复用的电池管理实现路径。", {
    left: 190,
    top: 545,
    width: 800,
    height: 60,
  }, { fontSize: 18, color: PRIMARY, bold: true, lineSpacing: 1.7 });
  addPage(slide, 4);
  addSource(slide, "资料来源：开题报告研究意义");
  setNotes(slide, "把工程价值、学习价值和应用参考分层说明，避免笼统表述。");
}

// 5. 国内外研究现状
{
  const slide = presentation.slides.add();
  slide.background.fill = BG;
  addTitle(slide, "国内外研究现状");
  const table = slide.tables.add({
    rows: 5,
    columns: 3,
    left: 64,
    top: 145,
    width: 1080,
    height: 340,
    values: [
      ["研究方向", "国内研究进展", "国外研究进展"],
      ["安全保护", "面向具体应用场景完善温度、过流与充放电保护策略", "保护策略与系统架构、通信集成结合更紧密"],
      ["SOC估算", "安时积分、开路电压与滤波算法逐步融合", "滤波方法与数据驱动模型融合成为重点"],
      ["均衡控制", "被动均衡与主动均衡方案均有应用", "主动均衡拓扑与控制策略优化持续发展"],
      ["系统集成", "多面向设备级监控与上位机调试", "物联网集成与嵌入式平台实现较为活跃"],
    ],
  });
  table.styleOptions = { headerRow: true, bandedRows: true };
  table.rows[0].height = 52;
  table.rows[1].height = 72;
  table.rows[2].height = 72;
  table.rows[3].height = 72;
  table.rows[4].height = 72;
  table.borders.assign({ style: "solid", fill: BORDER, width: 1 });
  for (let r = 0; r < 5; r += 1) {
    for (let c = 0; c < 3; c += 1) {
      table.getCell(r, c).text.style = {
        typeface: FONT,
        fontSize: r === 0 ? 16 : 15,
        bold: r === 0,
        color: r === 0 ? "#FFFFFF" : TEXT,
        alignment: c === 0 ? "center" : "left",
        verticalAlignment: "middle",
        lineSpacing: 1.5,
      };
      if (r === 0) table.getCell(r, c).fill = PRIMARY;
    }
  }
  addText(slide, "已有研究为本课题提供方案与算法参考，面向STM32平台时仍需处理采样时序、共享资源调度与保护响应的工程约束。", {
    left: 64,
    top: 520,
    width: 1080,
    height: 80,
  }, { fontSize: 17, lineSpacing: 1.7 });
  addPage(slide, 5);
  addSource(slide, "资料来源：开题报告国内外研究现状");
  setNotes(slide, "现状页用表格归纳，不逐篇展开文献，具体依据见开题报告参考文献。");
}

// 6. 系统总体方案
{
  const slide = presentation.slides.add();
  slide.background.fill = BG;
  addTitle(slide, "系统总体方案");
  const main = [
    addBox(slide, "9串18650\n锂电池组", { left: 64, top: 145, width: 170, height: 100 }, { fontSize: 15 }),
    addBox(slide, "BQ76940\n采集与硬件告警", { left: 270, top: 145, width: 200, height: 100 }, { fontSize: 15, fill: SOFT, color: PRIMARY, bold: true }),
    addBox(slide, "STM32F103C8T6\n数据处理与策略", { left: 505, top: 145, width: 220, height: 100 }, { fontSize: 15, fill: PRIMARY, color: "#FFFFFF", bold: true }),
    addBox(slide, "UART\n命令与状态交互", { left: 760, top: 145, width: 180, height: 100 }, { fontSize: 15 }),
    addBox(slide, "Python\ntkinter上位机", { left: 975, top: 145, width: 190, height: 100 }, { fontSize: 15 }),
  ];
  addArrow(slide, main[0], main[1]);
  addArrow(slide, main[1], main[2]);
  addArrow(slide, main[2], main[3]);
  addArrow(slide, main[3], main[4]);

  const safety = addBox(slide, "安全控制链路\n软件阈值与硬件告警共同判断", { left: 380, top: 330, width: 420, height: 100 }, {
    fontSize: 16,
    fill: "#F5E9E9",
    color: WARN,
    bold: true,
  });
  addArrow(slide, main[1], safety, { fromSide: "bottom", toSide: "top" });
  addArrow(slide, main[2], safety, { fromSide: "bottom", toSide: "top" });
  const mos = addBox(slide, "充放电MOS控制", { left: 860, top: 330, width: 220, height: 100 }, { fontSize: 16, fill: "#EAF3EA", color: OK, bold: true });
  addArrow(slide, safety, mos);
  addText(slide, "数据流从电池组经BQ76940、STM32到达上位机，控制流由告警与阈值判断驱动MOS动作。CAN、蓝牙与串口屏仅作为扩展预留。", {
    left: 64,
    top: 510,
    width: 1080,
    height: 90,
  }, { fontSize: 17, lineSpacing: 1.7 });
  addPage(slide, 6);
  addSource(slide, "资料来源：开题报告系统总体设计");
  setNotes(slide, "重点区分数据流与安全控制链路，避免把扩展功能说成已实现。");
}

// 7. 硬件选型与模块设计
{
  const slide = presentation.slides.add();
  slide.background.fill = BG;
  addTitle(slide, "硬件选型与模块设计");
  const rows = [
    ["主控", "STM32F103C8T6", "外设资源、实时性、资料成熟度与成本均衡"],
    ["模拟前端", "BQ76940", "支持多串采集、硬件告警、MOS控制与被动均衡资源"],
    ["均衡方式", "被动均衡接口", "结构简单、成本可控，主动均衡效率高但复杂"],
    ["通信方式", "UART", "当前连接Python上位机，CAN等作为扩展方向"],
  ];
  rows.forEach(([label, selected, reason], index) => {
    const top = 140 + index * 90;
    addBox(slide, label, { left: 64, top, width: 110, height: 70 }, { fontSize: 15, bold: true, fill: SOFT, color: PRIMARY });
    addBox(slide, selected, { left: 190, top, width: 210, height: 70 }, { fontSize: 16, bold: true, fill: "#FFFFFF", color: PRIMARY });
    addText(slide, reason, { left: 420, top: top + 8, width: 480, height: 54 }, { fontSize: 15, lineSpacing: 1.5 });
  });
  slide.images.add({
    blob: bqImage,
    contentType: "image/png",
    alt: "BQ769X0外围连接参考图",
    fit: "contain",
    position: { left: 920, top: 135, width: 290, height: 370 },
  });
  addText(slide, "BQ769X0外围连接参考图\n来源：BQ769X0参考设计资料", {
    left: 920,
    top: 510,
    width: 290,
    height: 50,
  }, { fontSize: 11, color: MUTED, alignment: "center", lineSpacing: 1.5 });
  addText(slide, "硬件仍处于联调阶段，软件I2C读取、上拉电阻、唤醒流程与寄存器读取需继续验证。", {
    left: 64,
    top: 565,
    width: 820,
    height: 70,
  }, { fontSize: 15, color: WARN, lineSpacing: 1.6 });
  addPage(slide, 7);
  addSource(slide, "资料来源：开题报告硬件选型与模块设计");
  setNotes(slide, "选型说明合适性，不宣称所有硬件均已完整验证成功。");
}

// 8. 软件设计与任务流程
{
  const slide = presentation.slides.add();
  slide.background.fill = BG;
  addTitle(slide, "软件设计与任务流程");
  const tasks = [
    addBox(slide, "BMS_Task\n高优先级\n500ms周期，可由ALERT唤醒", { left: 90, top: 145, width: 280, height: 120 }, { fontSize: 15, fill: PRIMARY, color: "#FFFFFF", bold: true }),
    addBox(slide, "UART_Task\n低优先级\n50ms轮询串口命令", { left: 420, top: 145, width: 260, height: 120 }, { fontSize: 15, fill: "#FFFFFF" }),
    addBox(slide, "ALERT中断\n只发送任务通知", { left: 730, top: 145, width: 190, height: 120 }, { fontSize: 14, fill: "#F5E9E9", color: WARN }),
    addBox(slide, "I2C互斥锁\n保护BQ76940访问", { left: 970, top: 145, width: 190, height: 120 }, { fontSize: 14, fill: SOFT, color: PRIMARY }),
  ];
  // The shared resource relationship is summarized in the caption instead of drawn through the task labels.

  const flow = [
    addBox(slide, "采样\n电压、电流、温度", { left: 80, top: 330, width: 180, height: 90 }, { fontSize: 14 }),
    addBox(slide, "判断\n阈值与告警", { left: 300, top: 330, width: 180, height: 90 }, { fontSize: 14 }),
    addBox(slide, "执行\nSOC更新与MOS控制", { left: 520, top: 330, width: 220, height: 90 }, { fontSize: 14 }),
    addBox(slide, "上报\nUART状态输出", { left: 780, top: 330, width: 180, height: 90 }, { fontSize: 14 }),
    addBox(slide, "显示\n上位机刷新", { left: 1000, top: 330, width: 180, height: 90 }, { fontSize: 14 }),
  ];
  addArrow(slide, flow[0], flow[1]);
  addArrow(slide, flow[1], flow[2]);
  addArrow(slide, flow[2], flow[3]);
  addArrow(slide, flow[3], flow[4]);
  addText(slide, "中断服务程序不执行I2C操作，寄存器读取、故障解析与状态更新在任务上下文完成。串口命令访问BQ76940时，同样由互斥锁保护。", {
    left: 80,
    top: 470,
    width: 1000,
    height: 80,
  }, { fontSize: 16, lineSpacing: 1.7 });
  addText(slide, "SOC采用安时积分，静置约30s后结合开路电压校正，并以满电、空电边界校准。", {
    left: 80,
    top: 560,
    width: 1000,
    height: 60,
  }, { fontSize: 16, lineSpacing: 1.7 });
  addPage(slide, 8);
  addSource(slide, "资料来源：开题报告软件设计与流程设计");
  setNotes(slide, "重点讲解任务优先级、共享资源保护和ALERT处理边界。");
}

// 9. 测试与验证方案
{
  const slide = presentation.slides.add();
  slide.background.fill = BG;
  addTitle(slide, "测试与验证方案");
  const stages = [
    addBox(slide, "通信与采集\nBQ76940初始化、寄存器读取与ALERT响应", { left: 70, top: 150, width: 230, height: 130 }, { fontSize: 14, fill: "#FFFFFF" }),
    addBox(slide, "保护与执行\n保护阈值触发与MOS动作", { left: 350, top: 150, width: 230, height: 130 }, { fontSize: 14, fill: "#FFFFFF" }),
    addBox(slide, "SOC与校正\n安时积分、OCV校正与边界校准", { left: 630, top: 150, width: 230, height: 130 }, { fontSize: 14, fill: "#FFFFFF" }),
    addBox(slide, "交互与运行\n串口命令、上位机显示与长时间运行", { left: 910, top: 150, width: 230, height: 130 }, { fontSize: 14, fill: "#FFFFFF" }),
  ];
  addArrow(slide, stages[0], stages[1]);
  addArrow(slide, stages[1], stages[2]);
  addArrow(slide, stages[2], stages[3]);
  const loop = addBox(slide, "问题反馈：修正驱动、参数与软件逻辑", { left: 320, top: 360, width: 520, height: 90 }, { fontSize: 16, fill: SOFT, color: PRIMARY, bold: true });
  addArrow(slide, stages[3], loop, { fromSide: "bottom", toSide: "right" });
  addArrow(slide, loop, stages[0], { fromSide: "left", toSide: "bottom" });
  addText(slide, "测试表述以拟验证为主，未设定具体误差指标的项目不虚构数值。", {
    left: 64,
    top: 510,
    width: 1080,
    height: 60,
  }, { fontSize: 17, lineSpacing: 1.7 });
  addPage(slide, 9);
  addSource(slide, "资料来源：开题报告测试与验证方案");
  setNotes(slide, "测试顺序从通信到整机，明确当前硬件仍在联调阶段。");
}

// 10. 研究方法与技术路线
{
  const slide = presentation.slides.add();
  slide.background.fill = BG;
  addTitle(slide, "研究方法与技术路线");
  const route = [
    addBox(slide, "需求分析\n功能范围与边界", { left: 80, top: 145, width: 220, height: 100 }, { fontSize: 15, fill: "#FFFFFF" }),
    addBox(slide, "系统设计\n硬件结构与任务划分", { left: 340, top: 145, width: 220, height: 100 }, { fontSize: 15, fill: "#FFFFFF" }),
    addBox(slide, "功能实现\n驱动、算法与通信", { left: 600, top: 145, width: 220, height: 100 }, { fontSize: 15, fill: "#FFFFFF" }),
    addBox(slide, "联调测试\n保护、SOC与上位机", { left: 860, top: 145, width: 220, height: 100 }, { fontSize: 15, fill: "#FFFFFF" }),
    addBox(slide, "总结完善\n数据整理与论文撰写", { left: 1075, top: 145, width: 150, height: 100 }, { fontSize: 13, fill: "#FFFFFF" }),
  ];
  addArrow(slide, route[0], route[1]);
  addArrow(slide, route[1], route[2]);
  addArrow(slide, route[2], route[3]);
  addArrow(slide, route[3], route[4]);

  const schedule = [
    ["2026.6.25-9.22", "选题、任务书与开题报告"],
    ["2026.9.23-10.19", "开题答辩与系统设计"],
    ["2026.10.20-11.1", "中期检查"],
    ["2026.11.2-12.22", "论文撰写、评审与送审"],
    ["2026.12.23", "毕业答辩"],
  ];
  schedule.forEach(([time, task], index) => {
    const top = 310 + index * 62;
    addBox(slide, time, { left: 90, top, width: 190, height: 46 }, { fontSize: 13, fill: SOFT, color: PRIMARY, bold: true });
    addText(slide, task, { left: 300, top: top + 7, width: 320, height: 32 }, { fontSize: 14, color: TEXT, lineSpacing: 1.5 });
  });
  const methods = [
    ["文献研究", "梳理BMS架构与算法"],
    ["系统设计", "确定软硬件边界"],
    ["实验调试", "验证通信与保护"],
    ["对比验证", "分析SOC与运行稳定性"],
  ];
  methods.forEach(([name, detail], index) => {
    const top = 310 + index * 62;
    addBox(slide, name, { left: 680, top, width: 130, height: 46 }, { fontSize: 13, fill: "#FFFFFF", color: PRIMARY, bold: true });
    addText(slide, detail, { left: 830, top: top + 7, width: 250, height: 32 }, { fontSize: 14, color: TEXT, lineSpacing: 1.5 });
  });
  addPage(slide, 10);
  addSource(slide, "资料来源：开题报告研究方法、技术路线与进度安排");
  setNotes(slide, "收束页强调从需求到总结的闭环，并对应学校进度安排。");
}

const candidatePath = path.join(buildDir, "candidate_v4.pptx");
await (await PresentationFile.exportPptx(presentation)).save(candidatePath);

const previewDir = path.join(buildDir, "previews_v4");
await fs.mkdir(previewDir, { recursive: true });
for (let index = 0; index < presentation.slides.length; index += 1) {
  const slide = presentation.slides.getItem(index);
  const preview = await presentation.export({ slide, format: "png", scale: 1 });
  await fs.writeFile(
    path.join(previewDir, `slide-${String(index + 1).padStart(2, "0")}.png`),
    new Uint8Array(await preview.arrayBuffer()),
  );
}

console.log(`slides=${presentation.slides.length}`);
console.log(candidatePath);
