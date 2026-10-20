import fs from "node:fs/promises";

const file = "D:/xianmu/BMS-main/ppt_build/finalize_ppt_v4.mjs";
let source = await fs.readFile(file, "utf8");
source = source.replace(
  "const finalPath = \"D:/xianmu/BMS-main/ppt_output/基于STM32的多串锂电池管理系统开题答辩_v4.pptx\";",
  "const finalPath = \"D:/xianmu/BMS-main/ppt_output/基于STM32的多串锂电池管理系统开题答辩_v4_1.pptx\";"
);
source = source.replace(
  "const receiptPath = \"D:/xianmu/BMS-main/ppt_build/final_v4.validation.json\"",
  "const receiptPath = \"D:/xianmu/BMS-main/ppt_build/final_v4_1.validation.json\""
);
await fs.writeFile(file, source, "utf8");
console.log("updated-final-path");
