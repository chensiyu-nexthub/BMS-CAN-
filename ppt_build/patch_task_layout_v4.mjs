import fs from "node:fs/promises";

const file = "D:/xianmu/BMS-main/ppt_build/make_ppt_redesign.mjs";
let source = await fs.readFile(file, "utf8");

const from = `  addArrow(slide, tasks[0], tasks[3], { fromSide: "bottom", toSide: "bottom" });
`;
if (!source.includes(from)) throw new Error("Expected task connector not found");

// Put the shared-resource relationship under the task boxes, with enough vertical clearance.
source = source.replace(
  from,
  `  const taskBottom = slide.shapes.add({
    geometry: "rect",
    position: { left: 90, top: 292, width: 1070, height: 0 },
    fill: "none",
    line: { style: "solid", fill: "none", width: 0 },
  });
  taskBottom.text = " ";
  taskBottom.text.style = {
    typeface: FONT,
    fontSize: 1,
    color: "transparent",
    alignment: "center",
    verticalAlignment: "middle",
  };
  addArrow(slide, tasks[0], taskBottom, { fromSide: "bottom", toSide: "top", kind: "straight" });
  addArrow(slide, taskBottom, tasks[3], { fromSide: "right", toSide: "bottom", kind: "straight" });
`
);

await fs.writeFile(file, source, "utf8");
console.log("patched=task-layout");
