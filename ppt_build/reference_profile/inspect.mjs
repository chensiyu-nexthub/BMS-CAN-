import fs from "node:fs/promises";
import path from "node:path";
import { FileBlob, PresentationFile } from "@oai/artifact-tool";

const source = path.resolve("reference_ppt.pptx");
const outDir = path.resolve("ppt_build", "reference_profile");

const presentation = await PresentationFile.importPptx(await FileBlob.load(source));
const snapshot = await presentation.inspect({
  kind: "slide,textbox,shape,image,table,chart,notes,layout",
  maxChars: 30000,
});
await fs.writeFile(path.join(outDir, "inspection.ndjson"), snapshot.ndjson, "utf8");

const layouts = await presentation.inspect({ kind: "layout", maxChars: 30000 });
await fs.writeFile(path.join(outDir, "layouts.ndjson"), layouts.ndjson, "utf8");

console.log(snapshot.ndjson);
