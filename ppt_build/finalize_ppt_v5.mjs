import path from "node:path";
import { pathToFileURL } from "node:url";

const skillDir = "C:/Users/chens/.codex/plugins/cache/openai-primary-runtime/presentations/26.904.11930/skills/presentations";
const { finalizePresentation } = await import(pathToFileURL(
  path.join(skillDir, "container_tools/artifact_tool_utils.mjs"),
).href);

process.env.RUNTIME_NODE_MODULES = "C:/Users/chens/.cache/codex-runtimes/codex-primary-runtime/dependencies/node/node_modules";
const workspaceDir = "D:/xianmu/BMS-main";
const candidatePath = "D:/xianmu/BMS-main/ppt_build/candidate_ppt_v5.pptx";
const runtimePython = "C:/Users/chens/.cache/codex-runtimes/codex-primary-runtime/dependencies/python/python.exe";
const finalPath = "D:/xianmu/BMS-main/ppt_output/基于STM32的多串锂电池管理系统开题答辩_8分钟.pptx";

const result = await finalizePresentation({
explicitTotalSlideCount: 9,
  requiredNativeTableOwnerSlides: [5],
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
    "--require-native-table-slide", "5",
  ],
  verifyArtifactToolImport: true,
  receiptPath: "D:/xianmu/BMS-main/ppt_build/final_v5.validation.json",
});

console.log(JSON.stringify(result, null, 2));
