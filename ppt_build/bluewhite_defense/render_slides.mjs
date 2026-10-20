import fs from "node:fs/promises";
import { PresentationFile } from "@oai/artifact-tool";

const source = "D:/xianmu/BMS-main/ppt_build/bluewhite_defense/candidate_bluewhite_defense.pptx";
const outDir = "D:/xianmu/BMS-main/ppt_build/bluewhite_defense/render";
await fs.mkdir(outDir, { recursive: true });

const presentation = await PresentationFile.importPptx(await (await import("@oai/artifact-tool")).FileBlob.load(source));
for (const [index, slide] of presentation.slides.entries()) {
  const preview = await slide.export({ format: "png", scale: 1 });
  await fs.writeFile(`${outDir}/slide-${String(index + 1).padStart(2, "0")}.png`, new Uint8Array(await preview.arrayBuffer()));
  const layout = await slide.export({ format: "layout" });
  await fs.writeFile(`${outDir}/slide-${String(index + 1).padStart(2, "0")}.layout.json`, await layout.text());
}
console.log(outDir);
