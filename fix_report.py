from docx import Document
from docx.shared import Pt
from docx.enum.text import WD_ALIGN_PARAGRAPH
from docx.oxml.ns import qn
from copy import deepcopy

SRC = r"C:\Users\chens\Desktop\linshi\桌面补充图片检查.docx"
DST = r"D:\xianmu\BMS-main\桌面补充图片检查_格式修正版.docx"

d = Document(SRC)
body = d.element.body


def delete_element(el):
    el.getparent().remove(el)


def remove_numpr(par):
    ppr = par._p.get_or_add_pPr()
    for e in ppr.findall(qn("w:numPr")):
        ppr.remove(e)


def set_run_font(run, size=12, bold=None, east_asia="宋体"):
    run.font.name = east_asia
    run.font.size = Pt(size)
    if bold is not None:
        run.font.bold = bold
    rpr = run._r.get_or_add_rPr()
    rfonts = rpr.find(qn("w:rFonts"))
    if rfonts is None:
        rfonts = rpr.makeelement(qn("w:rFonts"), {})
        rpr.insert(0, rfonts)
    rfonts.set(qn("w:ascii"), east_asia)
    rfonts.set(qn("w:hAnsi"), east_asia)
    rfonts.set(qn("w:eastAsia"), east_asia)


def format_normal_paragraph(par):
    par.style = d.styles["Normal"]
    pf = par.paragraph_format
    pf.line_spacing_rule = 4  # EXACTLY
    pf.line_spacing = Pt(20)
    pf.space_before = Pt(0)
    pf.space_after = Pt(0)
    for run in par.runs:
        set_run_font(run, size=12)
    ppr = par._p.get_or_add_pPr()
    for tag in ("w:autoSpaceDE", "w:autoSpaceDN"):
        e = ppr.find(qn(tag))
        if e is None:
            e = ppr.makeelement(qn(tag), {})
            ppr.append(e)
        e.set(qn("w:val"), "0")


def format_caption(par):
    format_normal_paragraph(par)
    par.alignment = WD_ALIGN_PARAGRAPH.CENTER


def format_subheading(par):
    format_normal_paragraph(par)
    par.alignment = WD_ALIGN_PARAGRAPH.LEFT
    for run in par.runs:
        set_run_font(run, size=12, bold=True)


def format_main_heading(par):
    par.style = d.styles["Normal"]
    pf = par.paragraph_format
    pf.line_spacing_rule = 4
    pf.line_spacing = Pt(20)
    pf.space_before = Pt(0)
    pf.space_after = Pt(0)
    par.alignment = WD_ALIGN_PARAGRAPH.LEFT
    for run in par.runs:
        set_run_font(run, size=14, bold=True)


# 1 删除学校说明页。保留封面标题、封面表格和正式正文。
for el in list(body[:30]):
    if not (el.tag == qn("w:tbl") or el.xpath(".//w:t[contains(text(), '杭州电子科技大学信息工程学院')]")):
        delete_element(el)

# 2 封面题目去掉末尾空格
cover_cell = d.tables[0].cell(0, 1)
for par in cover_cell.paragraphs:
    for run in par.runs:
        run.text = run.text.rstrip()

# 3 去掉研究目的的自动编号，避免与正式二级标题冲突
for i, par in enumerate(d.paragraphs):
    if par.text.strip() == "研究目的：":
        remove_numpr(par)
        break

# 4 统一国内、国外研究现状的小标题
for par in d.paragraphs:
    t = par.text.strip()
    if t == "第一部分：国内研究现状正文":
        par.runs[0].text = "1、国内研究现状"
        for run in par.runs[1:]:
            run.text = ""
        format_subheading(par)
    elif t == "第一部分：国外研究现状正文":
        par.runs[0].text = "2、国外研究现状"
        for run in par.runs[1:]:
            run.text = ""
        format_subheading(par)

# 5 修正图2图题
for par in d.paragraphs:
    if par.text.strip() == "图2BQ769X0内部结构框图":
        par.runs[0].text = "图2BQ76940内部结构框图"
        for run in par.runs[1:]:
            run.text = ""
        break

# 6 统一正文格式
for i, par in enumerate(d.paragraphs):
    t = par.text.strip()
    if not t:
        continue
    if t.startswith("综述本课题国内外研究动态"):
        format_main_heading(par)
    elif t.startswith(("二、研究的基本内容", "三、研究步骤", "四、研究工作进度", "五、主要参考文献")):
        format_main_heading(par)
    elif t.startswith(("研究目的：", "研究意义：", "研究的基本内容：", "预期目标：", "拟解决关键问题：", "研究方法：", "技术路线：")):
        format_subheading(par)
    elif t.startswith(("研究方案与总体设计：", "硬件选型与模块设计：", "软件设计与流程设计：", "测试与验证方案：")):
        format_subheading(par)
        format_subheading(par)
    elif t.startswith(("图1", "图2", "图3", "图4")):
        format_caption(par)
    elif t.startswith("[") and ":" in t:
        par.paragraph_format.line_spacing_rule = 4
        par.paragraph_format.line_spacing = Pt(20)
        par.paragraph_format.space_before = Pt(0)
        par.paragraph_format.space_after = Pt(0)
    else:
        format_normal_paragraph(par)

# 7 删除图片与标题之间的多余空段，避免图片单独占一页
paras = list(d.paragraphs)
for idx in range(len(paras) - 1, -1, -1):
    t = paras[idx].text.strip()
    if t.startswith(("图1", "图2", "图3", "图4")):
        prev = idx - 1
        while prev >= 0 and not paras[prev].text.strip() and not paras[prev]._p.xpath(".//w:drawing | .//w:pict"):
            delete_element(paras[prev]._p)
            prev -= 1

# 8 将图片和图题放到对应小节标题之后
paras = list(d.paragraphs)
caption_map = {}
for idx, par in enumerate(paras):
    t = par.text.strip()
    if t.startswith(("图1", "图2", "图3", "图4")):
        caption_map[t[:2]] = idx

section_targets = {}
for idx, par in enumerate(paras):
    t = par.text.strip()
    if t.startswith(("研究方案与总体设计：", "硬件选型与模块设计：", "软件设计与流程设计：")):
        section_targets[t.split("：")[0]] = idx

moves = {
    "图1": "研究方案与总体设计",
    "图2": "硬件选型与模块设计",
    "图3": "软件设计与流程设计",
    "图4": "软件设计与流程设计",
}
for caption_no, section in moves.items():
    if caption_no not in caption_map or section not in section_targets:
        continue
    cap_idx = caption_map[caption_no]
    cap = paras[cap_idx]
    img_idx = cap_idx - 1
    while img_idx >= 0 and not paras[img_idx]._p.xpath(".//w:drawing | .//w:pict"):
        img_idx -= 1
    if img_idx < 0:
        continue
    img = paras[img_idx]
    target = paras[section_targets[section]]
    target._p.addnext(cap._p)
    img._p.addnext(cap._p)

# 图片与图题间距
for par in d.paragraphs:
    if par._p.xpath(".//w:drawing | .//w:pict"):
        par.paragraph_format.space_after = Pt(0)
        par.paragraph_format.space_before = Pt(0)

# 9 参考文献行距统一
for par in d.paragraphs:
    if par.text.strip().startswith("[") and "]" in par.text:
        par.paragraph_format.line_spacing_rule = 4
        par.paragraph_format.line_spacing = Pt(20)
        par.paragraph_format.space_before = Pt(0)
        par.paragraph_format.space_after = Pt(0)

# 10 从源文件恢复封面标题及原有封面格式
src_doc = Document(SRC)
source_paragraph_texts = [p.text.strip() for p in src_doc.paragraphs]
cover_title = None
source_school = None
for p in src_doc.paragraphs:
    if p.text.strip() == "杭州电子科技大学信息工程学院" and source_school is None:
        source_school = p
    if p.text.strip() == "毕业设计（论文）开题报告" and cover_title is None:
        cover_title = p

if source_school is not None and cover_title is not None:
    current_first = d.paragraphs[0]._p
    inserted_school = deepcopy(source_school._p)
    inserted_title = deepcopy(cover_title._p)
    current_first.addprevious(inserted_school)
    current_first.addprevious(inserted_title)
    delete_element(current_first)

    # 恢复封面标题与封面表格之间的两个空段
    for p in src_doc.paragraphs:
        if p.text.strip() == "毕业设计（论文）开题报告":
            title_idx = source_paragraph_texts.index(p.text.strip())
            for blank in reversed(src_doc.paragraphs[title_idx + 1:title_idx + 3]):
                inserted_title.addnext(deepcopy(blank._p))
            break

# 11 修正参考文献作者拼写
for par in d.paragraphs:
    if par.text.startswith("[7]Khan N,Ooi C A,Shreasth"):
        for run in par.runs:
            run.text = run.text.replace("Shreasth", "Shrestha")
    if par.text.startswith("[11]Khan N,Ooi C A,Shreasth"):
        for run in par.runs:
            run.text = run.text.replace("Shreasth", "Shrestha")

d.save(DST)
print(DST)
