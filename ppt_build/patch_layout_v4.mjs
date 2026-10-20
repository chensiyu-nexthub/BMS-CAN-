import fs from "node:fs/promises";

const file = "D:/xianmu/BMS-main/ppt_build/make_ppt_redesign.mjs";
let source = await fs.readFile(file, "utf8");

const replacements = [
  [
    'addArrow(slide, central, branches[0], { fromSide: "left", toSide: "top" });',
    'addArrow(slide, central, branches[0], { fromSide: "bottom", toSide: "top" });',
  ],
  [
    'addArrow(slide, tasks[0], tasks[3], { fromSide: "top", toSide: "top" });',
    'addArrow(slide, tasks[0], tasks[3], { fromSide: "bottom", toSide: "bottom" });',
  ],
  [
    'addBox(slide, "总结完善\\n数据整理与论文撰写", { left: 1100, top: 145, width: 120, height: 100 }',
    'addBox(slide, "总结完善\\n数据整理与论文撰写", { left: 1075, top: 145, width: 150, height: 100 }',
  ],
];

for (const [from, to] of replacements) {
  if (!source.includes(from)) throw new Error(`Missing expected source text: ${from}`);
  source = source.replace(from, to);
}

await fs.writeFile(file, source, "utf8");
console.log("patched=3");
