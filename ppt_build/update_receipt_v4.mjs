import fs from "node:fs/promises";

const file = "D:/xianmu/BMS-main/ppt_build/finalize_ppt_v4.mjs";
let source = await fs.readFile(file, "utf8");
source = source.replace(
  "D:/xianmu/BMS-main/ppt_build/final_v4.validation.json",
  "D:/xianmu/BMS-main/ppt_build/final_v4_1.validation.json"
);
await fs.writeFile(file, source, "utf8");
console.log("updated-receipt-path");
