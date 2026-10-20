import fs from "node:fs/promises";

const file = "D:/xianmu/BMS-main/ppt_build/make_ppt_redesign.mjs";
let source = await fs.readFile(file, "utf8");

const start = source.indexOf("  const taskBottom = slide.shapes.add({");
const endMarker = `  addArrow(slide, taskBottom, tasks[3], { fromSide: "right", toSide: "bottom", kind: "straight" });
`;
const end = source.indexOf(endMarker, start);
if (start < 0 || end < 0) throw new Error("Expected task connector block not found");
source = source.slice(0, start) + `  // The shared resource relationship is summarized in the caption instead of drawn through the task labels.
` + source.slice(end + endMarker.length);

await fs.writeFile(file, source, "utf8");
console.log("removed-crossing-connectors");
