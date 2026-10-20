import fs from "node:fs/promises";
import path from "node:path";
import { FileBlob, PresentationFile } from "@oai/artifact-tool";

const source = path.resolve("reference_ppt.pptx");
const outDir = path.resolve("ppt_build", "reference_profile");

const presentation = await PresentationFile.importPptx(await FileBlob.load(source));
for (const slide of presentation.slides.items) {
  const preview = await slide.export({ format: "png", scale: 1 });
  await fs.writeFile(
    path.join(outDir, `slide-${slide.slideNumber}.png`),
    new Uint8Array(await preview.arrayBuffer()),
  );
}
console.log("slides", presentation.slides.items.length);
