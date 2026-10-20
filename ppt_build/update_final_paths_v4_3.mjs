import fs from "node:fs/promises";

const file = "D:/xianmu/BMS-main/ppt_build/finalize_ppt_v4.mjs";
let source = await fs.readFile(file, "utf8");
source = source.replace(
  "基于STM32的多串锂电池管理系统开题答辩_v4_2.pptx",
  "基于STM32的多串锂电池管理系统开题答辩_v4_3.pptx"
);
source = source.replace(
  "D:/xianmu/BMS-main/ppt_build/final_v4_2.validation.json",
  "D:/xianmu/BMS-main/ppt_build/final_v4_3.validation.json"
);
await fs.writeFile(file, source, "utf8");
console.log("updated-v4-3-paths");
