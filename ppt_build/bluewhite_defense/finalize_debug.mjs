import fs from "node:fs/promises";
import path from "node:path";
import { pathToFileURL } from "node:url";
import { PresentationFile } from "@oai/artifact-tool";

const SKILL_DIR = "C:/Users/chens/.codex/plugins/cache/openai-primary-runtime/presentations/26.904.11930/skills/presentations";
const workspaceDir = "D:\\xianmu\\BMS-main";
console.log("workspace", JSON.stringify(workspaceDir), path.isAbsolute(workspaceDir));
const candidatePath = "D:/xianmu/BMS-main/ppt_build/bluewhite_defense/candidate_bluewhite_defense.pptx";
const FINAL_PPTX = "D:/xianmu/BMS-main/ppt_output/基于STM32的多串锂电池管理系统开题答辩_蓝白学术版.pptx";
const RUNTIME_PYTHON = "C:/Users/chens/.cache/codex-runtimes/codex-primary-runtime/dependencies/python/python.exe";

const { finalizePresentation } = await import(pathToFileURL(
  path.join(SKILL_DIR, "container_tools/artifact_tool_utils.mjs"),
).href);

const stagingDir = path.join(workspaceDir, "ppt_build/bluewhite_defense/.codex-finalizer");
await fs.mkdir(stagingDir, { recursive: true });
await fs.mkdir(path.dirname(FINAL_PPTX), { recursive: true });

const result = await finalizePresentation({
  explicitTotalSlideCount: 10,
  requiredNativeTableOwnerSlides: [4],
  candidatePath,
  finalPath: FINAL_PPTX,
  pythonExecutable: RUNTIME_PYTHON,
  integrityValidatorPath: path.join(SKILL_DIR, "container_tools/inspect_presentation_package_integrity.py"),
  layoutValidatorPath: path.join(SKILL_DIR, "container_tools/inspect_presentation_layout_geometry.py"),
  layoutArgs: [
    "--expected-slide-size-emu", "12192000,6858000",
    "--validate-bullet-geometry",
    "--validate-heading-fit",
    "--require-native-table-slide", "4",
  ],
  fontPolicy: {
    basis: "user_request",
    families: ["Microsoft YaHei"],
  },
  verifyArtifactToolImport: true,
  receiptPath: path.join(stagingDir, `${path.basename(FINAL_PPTX)}.validation.json`),
});

console.log(JSON.stringify(result, null, 2));
console.log(FINAL_PPTX);

console.log("workspace", JSON.stringify(workspaceDir), path.isAbsolute(workspaceDir));
