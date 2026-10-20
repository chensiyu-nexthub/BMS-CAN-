import fs from "node:fs/promises";
import path from "node:path";
import { FileBlob, PresentationFile } from "@oai/artifact-tool";

const sourcePath = "D:/xianmu/BMS-main/ppt_build/candidate_ppt_v6.pptx";
const previewDir = "D:/xianmu/BMS-main/ppt_build/previews_ppt_v6";
const slideCount = 9;
await fs.mkdir(previewDir, { recursive: true });

const presentation = await PresentationFile.importPptx(await FileBlob.load(sourcePath));
for (let index = 0; index < slideCount; index += 1) {
  const slide = presentation.slides.getItem(index);
  const preview = await presentation.export({ slide, format: "png", scale: 1 });
  await fs.writeFile(
    path.join(previewDir, `slide-${String(index + 1).padStart(2, "0")}.png`),
    new Uint8Array(await preview.arrayBuffer()),
  );
}
console.log(`rendered=${slideCount}`);
