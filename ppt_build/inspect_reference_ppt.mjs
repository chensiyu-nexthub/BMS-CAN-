import fs from "node:fs/promises";
import path from "node:path";
import { FileBlob, PresentationFile } from "@oai/artifact-tool";

const sourcePath = "D:/ruanjian/xwechat_files/wxid_0c11hp8dgenb29_19f9/msg/file/2026-09/林鸿烨开题答辩PPT(1).pptx";
const outputDir = "D:/xianmu/BMS-main/ppt_build/reference_inspection";
await fs.mkdir(outputDir, { recursive: true });

const presentation = await PresentationFile.importPptx(await FileBlob.load(sourcePath));
const snapshot = await presentation.inspect({
  kind: "slide,textbox,shape,image,table,chart,notes,layout",
  maxChars: 20000,
});
await fs.writeFile(path.join(outputDir, "reference_snapshot.ndjson"), snapshot.ndjson);

for (let index = 0; index < 20; index += 1) {
  try {
    const slide = presentation.slides.getItem(index);
    const preview = await presentation.export({ slide, format: "png", scale: 1 });
    await fs.writeFile(
      path.join(outputDir, `slide-${String(index + 1).padStart(2, "0")}.png`),
      new Uint8Array(await preview.arrayBuffer()),
    );
  } catch (error) {
    break;
  }
}
console.log(`snapshot=${path.join(outputDir, "reference_snapshot.ndjson")}`);
console.log(`previews=${outputDir}`);
