import path from "node:path";
import { pathToFileURL } from "node:url";
const skillDir = "C:/Users/chens/.codex/plugins/cache/openai-primary-runtime/presentations/26.904.11930/skills/presentations";
const { finalizePresentation } = await import(pathToFileURL(
  path.join(skillDir, "container_tools/artifact_tool_utils.mjs"),
).href);

const workspaceDir = "D:/xianmu/BMS-main";
const candidatePath = "D:/xianmu/BMS-main/ppt_output/candidate.pptx";
const finalPath = "D:/xianmu/BMS-main/ppt_output/基于STM32的多串锂电池管理系统开题答辩_v3.pptx";
const runtimePython = "C:/Users/chens/.cache/codex-runtimes/codex-primary-runtime/dependencies/python/python.exe";

const result = await finalizePresentation({
  explicitTotalSlideCount: 10,
  requiredNativeTableOwnerSlides: [],
  requiredNativeChartOwnerSlides: [],
  fontPolicy: {
    basis: "user_request",
    families: ["Microsoft YaHei"],
  },
  workspaceDir,
  candidatePath,
  finalPath,
  pythonExecutable: runtimePython,
  integrityValidatorPath: path.join(skillDir, "container_tools/inspect_presentation_package_integrity.py"),
  layoutValidatorPath: path.join(skillDir, "container_tools/inspect_presentation_layout_geometry.py"),
  layoutArgs: [
    "--expected-slide-size-emu", "12192000,6858000",
    "--validate-bullet-geometry",
    "--validate-heading-fit",
  ],
  verifyArtifactToolImport: true,
  receiptPath: "D:/xianmu/BMS-main/ppt_build/final_v3.validation.json",
});

console.log(JSON.stringify(result, null, 2));
