import fs from "node:fs/promises";

const file = "D:/xianmu/BMS-main/ppt_build/make_ppt_redesign.mjs";
let source = await fs.readFile(file, "utf8");
source = source.replace(`  addArrow(slide, tasks[2], tasks[0], { fromSide: "left", toSide: "right" });
`, "");
source = source.replace(`  addArrow(slide, tasks[1], tasks[3], { fromSide: "right", toSide: "left" });
`, "");
await fs.writeFile(file, source, "utf8");
console.log("removed-task-crossing-connectors");
