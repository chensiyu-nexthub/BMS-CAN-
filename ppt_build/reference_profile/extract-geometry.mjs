import fs from "node:fs/promises";
import path from "node:path";
import xmlJs from "xml-js";

const root = path.resolve("ppt_build", "reference_profile", "unzipped");
const slideDir = path.join(root, "ppt", "slides");

function localName(name = "") {
  return name.split(":").pop();
}

function xmlToValue(node) {
  if (node.type === "element") {
    const result = { tag: localName(node.name), children: [] };
    for (const [key, value] of Object.entries(node.attributes ?? {})) {
      result[key === "type" ? "shapeType" : key] = value;
    }
    for (const child of node.elements ?? []) result.children.push(xmlToValue(child));
    return result;
  }
  if (node.type === "text") return { tag: "#text", text: node.text ?? "", children: [] };
  return null;
}

function collectRuns(node, output = []) {
  if (!node) return output;
  for (const child of node.children ?? []) {
    if (child?.tag === "r") output.push({ runs: child.children ?? [] });
    else collectRuns(child, output);
  }
  return output;
}

const deck = [];
for (let index = 1; index <= 9; index += 1) {
  const xmlPath = path.join(slideDir, `slide${index}.xml`);
  const parsed = xmlJs.xml2js(await fs.readFile(xmlPath, "utf8"), { compact: false, alwaysArray: true });
  const slide = xmlToValue(parsed.elements[0]);
  const shapeTree = slide.children.find((child) => child.tag === "cSld")?.children?.find((child) => child.tag === "spTree");
  const records = [];
  for (const shape of shapeTree?.children ?? []) {
    if (shape.tag !== "sp") continue;
    const nv = shape.children.find((child) => child.tag === "nvSpPr");
    const name = nv?.children?.find((child) => child.tag === "cNvPr")?.name;
    const spPr = shape.children.find((child) => child.tag === "spPr");
    const xfrm = spPr?.children?.find((child) => child.tag === "xfrm");
    const off = xfrm?.children?.find((child) => child.tag === "off");
    const ext = xfrm?.children?.find((child) => child.tag === "ext");
    const geom = spPr?.children?.find((child) => child.tag === "prstGeom" || child.tag === "custGeom");
    const fill = spPr?.children?.find((child) => child.tag === "solidFill");
    const color = fill?.children?.find((child) => child.tag === "srgbClr")?.val;
    const textBody = shape.children.find((child) => child.tag === "txBody");
    const paragraphs = (textBody?.children ?? [])
      .filter((child) => child.tag === "p")
      .map((paragraph) => {
        const runs = collectRuns(paragraph).flatMap((entry) => entry.runs);
        const style = runs.find((run) => run.tag === "rPr") ?? {};
        const text = runs.filter((run) => run.tag === "t").flatMap((run) => run.children?.filter((child) => child.tag === "#text") ?? []).map((run) => run.text ?? "").join("");
        if (!text) return null;
        return {
          text,
          size: style.sz,
          bold: style.b,
          color: style.children?.find((child) => child.tag === "solidFill")?.children?.find((child) => child.tag === "srgbClr")?.val,
          font: style.children?.find((child) => child.tag === "latin" || child.tag === "ea")?.typeface,
        };
      })
      .filter(Boolean);
    records.push({
      name,
      geometry: geom?.prst,
      x: off?.x ? Number(off.x) / 914400 : undefined,
      y: off?.y ? Number(off.y) / 914400 : undefined,
      width: ext?.cx ? Number(ext.cx) / 914400 : undefined,
      height: ext?.cy ? Number(ext.cy) / 914400 : undefined,
      fill: color,
      paragraphs,
    });
  }
  deck.push({ slide: index, shapes: records });
}

await fs.writeFile(path.resolve("ppt_build", "reference_profile", "slide-geometry.json"), JSON.stringify(deck, null, 2), "utf8");
